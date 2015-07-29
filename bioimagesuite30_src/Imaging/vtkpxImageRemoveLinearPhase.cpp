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
  Module:    $RCSfile: vtkpxImageRemoveLinearPhase.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:02:52 $
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
#include "vtkImageFFT.h"
#include "vtkImageRFFT.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkpxMath.h"
#include "vtkpxImagePolarToEuclidean.h"
#include "vtkpxImageRemoveLinearPhase.h"
#include "vtkpxUtil.h"
#include "vtkImageExtractComponents.h"
#include "vtkImageShiftScale.h"
#include "vtkImageAppendComponents.h"
//------------------------------------------------------------------------------
vtkpxImageRemoveLinearPhase* vtkpxImageRemoveLinearPhase::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageRemoveLinearPhase");
  if(ret)
    {
      return (vtkpxImageRemoveLinearPhase*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageRemoveLinearPhase;
}
//----------------------------------------------------------------------------
vtkpxImageRemoveLinearPhase::vtkpxImageRemoveLinearPhase()
{
  this->InputIsPolarComplex=1;
  this->PhaseAxis=1;
}

//----------------------------------------------------------------------------
void vtkpxImageRemoveLinearPhase::SimpleExecute(vtkImageData* input,vtkImageData *output) 
{
  if (input->GetNumberOfScalarComponents()!=2)
    {
      vtkErrorMacro(<<"Input Must Be Complex!\n");
      return;
    }


  if (this->InputIsPolarComplex)
    {
      vtkImageData* temp=vtkImageData::New();
      vtkImageData* temp2=vtkImageData::New();

      vtkpxImagePolarToEuclidean* pol=vtkpxImagePolarToEuclidean::New();
      pol->SetInput(input);
  
      vtkImageFFT* fft=vtkImageFFT::New();
      fft->SetInput(pol->GetOutput());
      pol->Delete();
      fft->Update();

      temp->ShallowCopy(fft->GetOutput());
      fft->Delete();


      vtkpxImageRemoveLinearPhase::RemoveLinearPhase(temp2,temp,this->PhaseAxis);

      vtkImageRFFT* rfft=vtkImageRFFT::New();
      rfft->SetInput(temp2);
      
      vtkpxImagePolarToEuclidean* euc2=vtkpxImagePolarToEuclidean::New();
      euc2->InverseOn();
      euc2->SetInput(rfft->GetOutput());
      rfft->Delete();
      euc2->Update();
      
      output->ShallowCopy(euc2->GetOutput());
      euc2->Delete();

      
      temp->Delete();
      temp2->Delete();
    }
  else
    {
      fprintf(stderr,"Straight Fourier Image Processing\n");
      vtkpxImageRemoveLinearPhase::RemoveLinearPhase(output,input,this->PhaseAxis);
    }


}

//----------------------------------------------------------------------------
int vtkpxImageRemoveLinearPhase::RemoveLinearPhase(vtkImageData* output,vtkImageData* input,int axis)

{
#ifndef M_PI
#define M_PI 3.1415926854
#endif

  vtkpxImagePolarToEuclidean* pol=vtkpxImagePolarToEuclidean::New();
  pol->InverseOn();
  pol->SetInput(input);
  pol->Update();

  vtkImageData* tmpoutput=vtkImageData::New();
  tmpoutput->ShallowCopy(pol->GetOutput());
  pol->Delete();

  int dim[3];
  vtkDataArray* scal=tmpoutput->GetPointData()->GetScalars();
  tmpoutput->GetDimensions(dim);

  double r[2];
  scal->GetRange(r);
  fprintf(stderr,"Input scalar range = %f:%f\n",r[0],r[1]);
  
  int slicesize=dim[0]*dim[1];
  float Theta,R,sum=0.0;
  float a,b,abdev;	      

  fprintf(stderr,"Dim=%d,%d,%d  Axis=%d\n",dim[0],dim[1],dim[2],axis);

  if (axis==1)
    {
      float* ph=new float[dim[1]];
      float* pj=new float[dim[1]];
      for (int k=0;k<dim[2];k++)
	{
	  int j;
	  for (j=0;j<dim[1];j++)
	    {
	      ph[j]=0.0;
	      pj[j]=float(j);
	      sum=0.0;
	      for (int i=0;i<dim[0];i++)
		{
		  Theta=tmpoutput->GetScalarComponentAsDouble(i,j,k,0);
		  R=tmpoutput->GetScalarComponentAsDouble(i,j,k,1);
		  ph[j]+=Theta;
		  sum+=1.0;
		  if (i==dim[0]/2 && j==dim[1]/2 && k==dim[2]/2)
		    fprintf(stderr," ijk=%d,%d,%d th=%f\n",i,j,k,Theta);
		}
	      ph[j]/=sum;
	    }
	  
	  vtkpxMath::Meanfit(pj,ph,dim[1],a,b,abdev);
	  fprintf(stderr,"Slice = %d a=%.2f b=%.2f abdev=%.2f\n",k,a,b,abdev);
	  
	  int index=k*slicesize;
	  for (j=0;j<dim[1];j++)
	    {
	      float shift=a+b*float(j);
	      for (int i=0;i<dim[0];i++)
		{
		  Theta=tmpoutput->GetScalarComponentAsDouble(i,j,k,0);
		  R=tmpoutput->GetScalarComponentAsDouble(i,j,k,1);
		  Theta-=shift;
		  scal->SetComponent(index,0,Theta);
		  scal->SetComponent(index,1,R);
		  if (i==dim[0]/2 && j==dim[1]/2 && k==dim[2]/2)
		    fprintf(stderr," ijk=%d,%d,%d (%d) v=%f --> %f\n",i,j,k,index,Theta+shift,Theta);
		  ++index;
		}
	    }
	}
      delete [] ph;
      delete [] pj;
    }
  else
    {
      int sz=dim[0];
      float* ph=new float[sz];
      float* pi=new float[sz];
      for (int k=0;k<dim[2];k++)
	{
	  ph[0]=tmpoutput->GetScalarComponentAsDouble(0,0,k,0);
	  pi[0]=0.0;
	  int i;
	  for (i=1;i<sz;i++)
	    {
	      pi[i]=float(i);
	      float t=tmpoutput->GetScalarComponentAsDouble(i,0,k,0);
	      ph[i]=vtkpxMath::FindMinAngleDistance(t,ph[i-1])+ph[i-1];
	    }
	  
	  vtkpxMath::Medfit(pi,ph,sz,a,b,abdev);
	  fprintf(stderr,"Slice = %d a=%.2f b=%.2f abdev=%.2f\n",k,a,b,abdev);

	  int base=k*slicesize;
	  for (i=0;i<dim[0];i++)
	    {
	      float shift=-b*float(i);
	      for (int j=0;j<dim[1];j++)
		{
		  int index=base+j*dim[0]+i;
		  float t1=tmpoutput->GetScalarComponentAsDouble(i,j,k,0);
		  float t2=t1-shift;
		  while (t2<M_PI)
		    t2+=2.0*M_PI;
		  while (t2>M_PI)
		    t2-=2.0*M_PI;

		  if (j==dim[1]/2 && k==dim[2]/2)
		    {
		      fprintf(stderr,"i=%d j=%d k=%d shift=%f t1=%.1f -> t2=%.1f\n",
			      i,j,k,shift,t1,t2);
		    }

		  scal->SetComponent(index,0,t2);
		}
	    }
	}
      delete [] ph;
      delete [] pi;
    }



  vtkpxImagePolarToEuclidean* pol2=vtkpxImagePolarToEuclidean::New();
  pol2->SetInput(tmpoutput);
  pol2->Update();

  output->DeepCopy(pol2->GetOutput());
  pol2->Delete();

  return 1;


}

