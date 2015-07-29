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
  Module:    $RCSfile: vtkpxComputeStrains.cpp,v $
  Language:  C++
  Date:      $Date: 2002/04/01 14:18:13 $
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

#include "pxabaqussolid.h"
#include "pxabaqusstructures.h"
#include "pxutil.h"
#include "vtkFloatArray.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkUnsignedCharArray.h"
#include "vtkpxComputeStrains.h"
#include "vtkMath.h"
#include "vtkPointData.h"
//------------------------------------------------------------------------------
vtkpxComputeStrains* vtkpxComputeStrains::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxComputeStrains");
  if(ret)
    {
      return (vtkpxComputeStrains*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxComputeStrains;
}
//------------------------------------------------------------------------------
vtkpxComputeStrains::vtkpxComputeStrains()
{
  this->BaseSolid=NULL;
  this->Transformation=NULL;
  this->CoordinateFrame=1;
}
//------------------------------------------------------------------------------
vtkpxComputeStrains::~vtkpxComputeStrains()
{
  this->SetBaseSolid(NULL);
  this->SetTransformation(NULL);
}
//------------------------------------------------------------------------------
int vtkpxComputeStrains::SaveOutput(const char* fname)
{
  if (this->BaseSolid==NULL)
      return 0;
  
  return this->BaseSolid->SaveFemData(fname);
}
//------------------------------------------------------------------------------
int vtkpxComputeStrains::Run()
{
  if (this->BaseSolid==NULL || this->Transformation==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs");
      return 0;
    }

  this->BaseSolid->CreateFemData();
  PXAbaqusSolid* solid=this->BaseSolid->GetInternalSolid();

  int numel=solid->getelements()->getnumnodes();
  int numnodes=solid->getnodes()->getnumnodes();

  // 1. Displacements -- this is trivial
  for (int ia=0;ia<numnodes;ia++)
    {
      float p1[3],p2[3];
      for (int ib=0;ib<=2;ib++)
	p1[ib]=solid->getnodes()->getpoint(ia)->x[ib];

      this->Transformation->TransformPoint(p1,p2);
      solid->getfemdata()->setNodeInfo(ia,p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2]);
      if (ia==200)
	fprintf(stderr,"Node %d (%.1f,%.1f,%.1f) --> (%.1f,%.1f,%.1f)\n",
		ia,p1[0],p1[1],p1[2],p2[0],p2[1],p2[2]);
    }
      
  // 2. Strains 
  // ----------------------------
  PXNeighboursList* elements=solid->getelements();
  elements->Rewind();

  float R[3][3];
  float E[3][3];
  float E2[3][3];

  for (int el=0;el<numel;el++)
    {
      float p1[3],p2[3],der[3][3];
      PXIntegerList* intList=(elements->getcurrentnode())->points;
      solid->getElementCentroid(intList,p1[0],p1[1],p1[2]);

      float cz=p1[2];
      this->Transformation->InternalTransformDerivative(p1,p2,der);
      for (int i=0;i<=2;i++)
	for (int j=0;j<=2;j++)
	  E[i][j]=0.5*(der[i][j]+der[j][i]);

      int layer=0;

      for (int ka=0;ka<intList->getnumnodes();ka++)
	{
	  layer+=solid->getnodes()->getindex(intList->getcurrent(),0); 
	  intList->Next();
	}
      layer=layer/intList->getnumnodes();

      if (this->CoordinateFrame!=0)
	{
	  float x1,x2,x3,y1,y2,y3,z1,z2,z3;
	  intList->Rewind();
	  solid->getdirections(intList,
			       x2,y2,z2,x3,y3,z3,x1,y1,z1,0);
	  
	  if (this->CoordinateFrame==2)
	    {
	      float angle=solid->getFibreAngle(cz,layer);
	      
	      solid->rotateFibreAngle(x3,y3,z3,  
				      x1,y1,z1,
				      x2,y2,z2, // Keep Radial Fixed
				      angle);
	      R[0][0]=x1; R[1][0]=y1; R[2][0]=z1; // Cross
	      R[0][1]=x2; R[1][1]=y2; R[2][1]=z2; // Radial
	      R[0][2]=x3; R[1][2]=y3; R[2][2]=z3; // Fibre
	    }
	  else
	    {
	      R[0][0]=x2; R[1][0]=y2; R[2][0]=z2; // Radial
	      R[0][1]=x3; R[1][1]=y3; R[2][1]=z3; // Radial
	      R[0][2]=x1; R[1][2]=y1; R[2][2]=z1; // Radial
	  
	    }

	  for (int i=0;i<=2;i++)
	    for (int j=0;j<=2;j++)
	      {
		E2[i][j]=0.0;
		for (int k=0;k<=2;k++)
		  for (int m=0;m<=2;m++)
		    E2[i][j] += R[i][k]*E[k][m]*R[j][m];
	      }

	  E2[0][0]-=1.0;
	  E2[1][1]-=1.0;
	  E2[2][2]-=1.0;

	  solid->getfemdata()->setElementInfo(el,2,1.0,
					      E2[0][0],E2[1][1],E2[2][2],E2[0][1],E2[0][2],E2[1][2]);
	}
      else 
	{
	  E[0][0]-=1.0;
	  E[1][1]-=1.0;
	  E[2][2]-=1.0;

	  solid->getfemdata()->setElementInfo(el,2,1.0,
					      E[0][0],E[1][1],E[2][2],E[0][1],E[0][2],E[1][2]);
	}

      solid->getfemdata()->setCentroid(el,p1[0],p1[1],p1[2]);
      elements->Next();
    }

  return 1;
}

int vtkpxComputeStrains::GenerateStudyFile(const char* namestem,const char* sldname,int nframes)
{
  vtkpxFemSolid* sld=vtkpxFemSolid::New();
  int ok=sld->Load(sldname);
  if (ok==0)
    {
      sld->Delete();
      return 0;
    }

  fprintf(stderr,"Solid Read!\n");

  char line[400];
  sprintf(line,"%s.stat",namestem);

  int numnd=sld->GetInternalSolid()->getnodes()->getnumnodes();
  int numel=sld->GetInternalSolid()->getelements()->getnumnodes();

  
  FILE* fout=fopen(line,"w");
  fprintf(fout,"#AbaqusResults File Set frames,nodes,elements\n%d\n%d\n%d\n",nframes,numnd,numel);
  fprintf(fout,"#Solid name\n%s\n",sldname);
  fprintf(fout,"#Step name\n%s\n",namestem);
  fprintf(fout,"#Step Statistics\n");
  for (int i=1;i<=nframes;i++)
    {
      sprintf(line,"%s.%02d.dat",namestem,i);
      fprintf(fout,"%s\t\t 		(step/inc=1/1)\n",line);
    }
  fclose(fout);
  sld->Delete();
  return 1;
}


// ---------------------------------------------------------------------------------------------------------
int vtkpxComputeStrains::Run(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL || output==NULL || this->Transformation==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs");
      return 0;
    }

  output->CopyStructure(input);
  output->SetScalarTypeToFloat();
  output->SetNumberOfScalarComponents(7);
  output->AllocateScalars();
  
  //  float R[3][3];
  float E[3][3];
  //float E2[3][3];


  int dim[3]; output->GetDimensions(dim);
  double sp[3]; output->GetSpacing(sp);
  double ori[3]; output->GetOrigin(ori);
  
  float p1[3],p2[3],der[3][3];

  vtkFloatArray* outscal=(vtkFloatArray*)output->GetPointData()->GetScalars();

  int index=0;
  for (int i=0;i<dim[0];i++)
    {
      p1[0]=i*sp[0]+ori[0];
      for (int j=0;j<dim[1];j++)
	{
	  p1[1]=j*sp[1]+ori[1];
	  for (int k=0;k<dim[2];k++)
	    {
	      p2[2]=k*sp[2]+ori[2];

	      this->Transformation->InternalTransformDerivative(p1,p2,der);
	      float det=vtkMath::Determinant3x3(der);
	      for (int i=0;i<=2;i++)
		for (int j=0;j<=2;j++)
		  E[i][j]=0.5*(der[i][j]+der[j][i]);
	      
	      E[0][0]-=1.0;
	      E[1][1]-=1.0;
	      E[2][2]-=1.0;
	      

	      outscal->SetComponent(index,0,E[0][0]);
	      outscal->SetComponent(index,1,E[1][1]);
	      outscal->SetComponent(index,2,E[2][2]);
	      outscal->SetComponent(index,3,E[0][1]);
	      outscal->SetComponent(index,4,E[0][2]);
	      outscal->SetComponent(index,5,E[1][2]);
	      outscal->SetComponent(index,6,det);
	      ++index;
	    }
	}
    }

  return 1;
}


