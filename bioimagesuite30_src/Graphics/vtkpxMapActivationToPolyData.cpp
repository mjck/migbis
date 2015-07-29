//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
//BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
//BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
//BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
//BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
//BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
//BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//BIOIMAGESUITE_LICENSE  See also  http://www.gnu.org/licenses/gpl.html
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
//BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
//BIOIMAGESUITE_LICENSE 
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------





/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxMapActivationToPolyData.cpp,v $
  Language:  C++
  Date:      $Date: 2004/06/04 00:05:14 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkpxMapActivationToPolyData.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataNormals.h"
#include "vtkFloatArray.h"
#include "vtkPoints.h"
#include "vtkProbeFilter.h"
#include "vtkDataSet.h"
#include "vtkLineSource.h"
#include "vtkUnsignedCharArray.h"
#include "pxutil.h"
//--------------------------------------------------------------------------
vtkpxMapActivationToPolyData* vtkpxMapActivationToPolyData::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxMapActivationToPolyData");
  if(ret)
    {
    return (vtkpxMapActivationToPolyData*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxMapActivationToPolyData;
}

vtkpxMapActivationToPolyData::vtkpxMapActivationToPolyData()
{
  this->MaxDistance = 10.0;
  this->MinDistance = 10.0;
  this->AverageMode = 0;
  this->Resolution=1.0;
  this->FunctionalData=NULL;

  this->LowThreshold=0.0;
  this->HighThreshold=1.0;

  this->ThresholdPositive=1;
  this->ThresholdNegative=0;

}


vtkpxMapActivationToPolyData::~vtkpxMapActivationToPolyData()
{
  this->ReleaseFunctionalData();
}

void vtkpxMapActivationToPolyData::Execute()
{
  vtkPolyData *source = this->GetInput();
  vtkPolyData *destination= this->GetOutput();
  
  destination->CopyStructure(source);

  if (this->FunctionalData==NULL)
    {
      vtkErrorMacro(<<"No Functional Data Leaving Surface as is\n");
      destination->GetPointData()->PassData(source->GetPointData());
      return;
    }

  if (this->FunctionalData->GetPointData()->GetScalars()==NULL)
    {
      vtkErrorMacro(<<"No Functional Data (Structures) Leaving Surface as is\n");
      destination->GetPointData()->PassData(source->GetPointData());
      return;
    }

  int np=source->GetPoints()->GetNumberOfPoints();

  if (np<20)
    return;

  vtkPolyDataNormals* polyNormals=vtkPolyDataNormals::New();
  vtkDataArray* normals=source->GetPointData()->GetNormals();
  if (normals==NULL)
    {
      polyNormals->SetInput(source);
      polyNormals->SplittingOff();
      polyNormals->ComputePointNormalsOn();
      vtkDebugMacro(<<"Computing our own normals ... bad idea\n");
      polyNormals->Update();
      normals=polyNormals->GetOutput()->GetPointData()->GetNormals();
      //destination->GetPointData()->SetNormals(normals);
    }

  this->UpdateProgress(0.2);


  vtkDataArray* newscalars = vtkDataArray::CreateDataArray(VTK_UNSIGNED_CHAR);
  newscalars->SetNumberOfComponents(3);
  newscalars->SetNumberOfTuples(np);
  destination->GetPointData()->SetScalars(newscalars);

  vtkUnsignedCharArray* scaldata=(vtkUnsignedCharArray*)newscalars;
  float c[3];
  c[0]=190;  c[1]=190; c[2]=190;
  for (int k=0;k<=np;k++)
    scaldata->SetTuple(k,c);

  vtkDebugMacro(<<"Creating " << np << "newscalars\n");

  float length=fabs(this->MaxDistance-this->MinDistance);
  if (length<0.01)
      length=1.0;

  int res=int(length/this->Resolution+0.5);
  if (res<2)
    res=2;

  vtkLineSource* ln=vtkLineSource::New();
  ln->SetResolution(res);

  vtkProbeFilter* probe=vtkProbeFilter::New();
  probe->SetSource(this->FunctionalData);

  if (this->LowThreshold<0.0)
    this->LowThreshold=0.0;

  float scalefactor=fabs(this->HighThreshold-this->LowThreshold);
  if (scalefactor==0.0)
    scalefactor=1.0;
  scalefactor=3.0/scalefactor;
  
  vtkDebugMacro(<< "Thresholds : " << this->LowThreshold << "," << this->HighThreshold << ", SF= " << scalefactor );
  int numset=0;

  this->UpdateProgress(0.3);

  int frac=int(np/10);
  float npf=float(1.25*np);

  for (int i=0;i<np;i++)
    {
      int index=int(i/frac);
      if (index*frac==i)
	{
	  float factor=float(i)/npf+0.2;
	  this->UpdateProgress(factor);
	}

      double x[3],n[3],tx[3],ox[3];
      source->GetPoints()->GetPoint(i,x);
      normals->GetTuple(i,n);

      float m=sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
      if (fabs(m-1.0)>0.001)
	fprintf(stderr,"Normal %d %.1f %.1f %.1f == %.1f\n",
	i,n[0],n[1],n[2],m);

      for (int ia=0;ia<=2;ia++)
	{
	  //	  n[ia]=Frange(n[ia],0.001,1.0);
	  ox[ia]=x[ia]-this->MinDistance*n[ia];
	  tx[ia]=x[ia]-this->MaxDistance*n[ia];
	  if (ox[ia]==tx[ia])
	    tx[ia]+=1.0;
	}

      ln->SetPoint1(ox);
      ln->SetPoint2(tx);
      ln->Modified();
      ln->Update();
      probe->SetInput(ln->GetOutput());
      probe->Modified();
      probe->Update();

      float max=0.0;
      vtkPolyData*  dat=(vtkPolyData*)probe->GetOutput();
      if (dat!=NULL)
	{
	  vtkDataArray* inten=dat->GetPointData()->GetScalars();
	  if (inten!=NULL)
	    {
	      int nump=inten->GetNumberOfTuples();
	      max=inten->GetComponent(0,0);
	      float sum=max;
	      for (int j=1;j<nump;j++)
		{
		  float v=inten->GetComponent(j,0);
		  sum+=v;
		  if (v>max)
		    max=v;
		}
	      
	      if (nump>0)
		{
		  if (this->AverageMode)
		    max=sum/nump;
		}
	    }
	}

      if ( fabs(max)>this->LowThreshold)
	{
	  float c[3];
	  int v=int(0.5+(fabs(max)-this->LowThreshold)*scalefactor);
	  if (max<0)
	    {
	      if (this->ThresholdNegative)
		{
		  switch (v)
		    {
		    case 3:
		      c[0]=1.0;c[1]=1.0;c[2]=1.0;
		      break;
		    case 2:
		      c[0]=0.3333; c[1]=0.6667; c[2]=1.0000;
		      break;
		    case 1:
		      c[0]=0.6667; c[1]=0.3333; c[2]=1.0000;
		      break;
		    default:
		      c[0]=1.0; c[1]=0.0; c[2]=1.0;
		      break;
		    }
		  for (int ia=0;ia<=2;ia++)
		    c[ia]*=255.0;
		  scaldata->SetTuple(i,c);
		  numset++;
		}
	    }
	  else
	    {
	      if (this->ThresholdPositive)
		{
		  switch (v)
		    {
		    case 3:
		      c[0]=1.0000; c[1]=1.0000; c[2]=0.6667;
		      break;
		    case 2:
		      c[0]=1.0000; c[1]=1.0000; c[2]=0.3333;
		      break;
		    case 1:
		      c[0]=1.0000; c[1]=1.0000; c[2]=0;
		      break;
		    default:
		      c[0]=1.0000; c[1]=0.0; c[2]=0.0;
		      break;
		    }
		  for (int ia=0;ia<=2;ia++)
		    c[ia]*=255.0;

		  scaldata->SetTuple(i,c);
		  numset++;
		}
	    }
	}
    }

  if (this->Debug)
    {
      double range[2];
      this->FunctionalData->GetPointData()->GetScalars()->GetRange(range);
      vtkDebugMacro(<<"Functional Range = " << range[0] << ":" << range[1] << " (res="<<res<<", distance = " << this->MaxDistance <<")\n");
      vtkDebugMacro(<<"Num Set= " << numset  << " numtotal = " << np << " Fraction = " << fabs(float(numset))/fabs(float(np))*100.0 );
    }

  polyNormals->Delete();
  newscalars->Delete();
  ln->Delete();
  probe->Delete();
}


void vtkpxMapActivationToPolyData::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkPolyDataToPolyDataFilter::PrintSelf(os,indent);
  os << indent << "Max Distance    : " << this->MaxDistance << "\n";
  os << indent << "Resolution      : " << this->Resolution  << "\n";
  os << indent << "Functional Data : " << this->FunctionalData << "\n";
  os << indent << "Thresholds      : " << this->LowThreshold << ":" <<  this->HighThreshold << "\n";
}

void vtkpxMapActivationToPolyData::ReleaseFunctionalData() 
{
  if (this->FunctionalData) 
    {
    this->FunctionalData->UnRegister(this);
    this->FunctionalData = NULL;
    }
}


void vtkpxMapActivationToPolyData::SetFunctionalData(vtkDataSet *FunctionalData)
{
  if (this->FunctionalData == FunctionalData)
    {
      return;
    }

  if (this->FunctionalData)
    {
      this->ReleaseFunctionalData();
    }

  if (FunctionalData)
    {
      FunctionalData->Register(this);
    }

  this->FunctionalData = FunctionalData;

  if (this->LowThreshold==0.0 && this->HighThreshold==1.0 )
    {
      vtkPointData* dt=this->FunctionalData->GetPointData();
      if (dt!=NULL)
	{
	  vtkDataArray* scal=dt->GetScalars();
	  if (scal!=NULL)
	    {
	      double range[2];
	      scal->GetRange(range);
	      this->LowThreshold=0.0;
	      this->HighThreshold=range[1];
	    }
	}
    }

  this->Modified();
}

