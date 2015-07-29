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


#include "vtkObjectFactory.h"
#include "vtkSource.h"
#include "vtkxqImageVesselEnhancement.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkMath.h"
#include "vtkpxMath.h"
#include "vtkxqImageHessian.h"
//#include "nrutil.h"
#include "math.h"
#include "pxisinf.h"
//------------------------------------------------------------------------------
vtkxqImageVesselEnhancement* vtkxqImageVesselEnhancement::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkxqImageVesselEnhancement");
  if(ret)
    {
    return (vtkxqImageVesselEnhancement*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkxqImageVesselEnhancement;
}

// Construct object with no children.
vtkxqImageVesselEnhancement::vtkxqImageVesselEnhancement()
{
  this->Alpha=0.5;
  this->Beta=0.5;
  this->Gamma=0.25;
  this->Aa = 0.2;
  this->Sigma=1.0;
  this->Mask = NULL;
  this->BrightStructure=1;
  this->StructureType=0;

}

// ----------------------------------------------------------------------------
vtkxqImageVesselEnhancement::~vtkxqImageVesselEnhancement()
{
  if (this->Mask != NULL)
    this->Mask->Delete();  
}

// ----------------------------------------------------------------------------
void vtkxqImageVesselEnhancement::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();
  output->SetScalarTypeToFloat();
  if (this->StructureType==3)
    output->SetNumberOfScalarComponents(6);
  else if (this->StructureType==4)
    output->SetNumberOfScalarComponents(3);
  else
    output->SetNumberOfScalarComponents(1);
}

// ----------------------------------------------------------------------------
double vtkxqImageVesselEnhancement::ComputexqVesselLikelihood(float lambda[3],int bright,double a)
{
  if (lambda[1]>0.0 || lambda[2]>0.0)
    return 0.0;

  double dnm = lambda[0]*lambda[0] + lambda[1]*lambda[1] + lambda[2]*lambda[2] - lambda[0]*lambda[1]
              - lambda[0]*lambda[2] - lambda[1]*lambda[2];
  double dom = 2*sqrt(dnm);
  double nc = (2*lambda[0]-lambda[1]-lambda[2])/dom;

  double lm = exp(-a*(nc-1.0)*(nc-1.0));
    
  double V = -lm*lambda[1];

  return V;
}

// ----------------------------------------------------------------------------
double vtkxqImageVesselEnhancement::ComputeVesselLikelihood(float lambda[3],int bright,double a2,double b2,double c2)
{
  if (lambda[1]>0.0 || lambda[2]>0.0)
    return 0.0;

  double RA= fabs(lambda[1]/lambda[2]);
  double RB= fabs(lambda[0]/sqrt(fabs(lambda[1]*lambda[2])));
  double S= sqrt(lambda[0]*lambda[0]+lambda[1]*lambda[1]+lambda[2]*lambda[2]);
  
  double V1=(1.0-exp(RA*RA*a2));
  double V2=exp(RB*RB*b2);
  double V3=(1.0-exp(S*S*c2));
  double V=V1*V2*V3;
  return V;
}
// --------------------------------------------------------------------------------------------

double vtkxqImageVesselEnhancement::ComputePlateLikelihood(float lambda[3],int bright,double a2,double b2,double c2)
{
  //	      fprintf(stderr,"does it come here?2\n");
  // modified by xq4, adopting Descoteaux's sheetness measure:
  if (lambda[2]>0.0)
    return 0.0;

  double RB= fabs(lambda[1]/lambda[2]);
  double RA= fabs(2*fabs(lambda[2])-fabs(lambda[1])-fabs(lambda[0]))/fabs(lambda[2]);
  double S= sqrt(lambda[0]*lambda[0]+lambda[1]*lambda[1]+lambda[2]*lambda[2]);
  
  double V1=(1.0-exp(RA*RA*a2));
  double V2=exp(RB*RB*b2);
  double V3=(1.0-exp(S*S*c2));
  double V=V1*V2*V3;
  return V;
 
  /*
  double V=exp(-lambda[2]/lambda[1]);
  if (V<0.0)
    return 0.0;
  if (V>40.0)
    return 40.0;
  return V;
  */
}

// --------------------------------------------------------------------------------------------
double vtkxqImageVesselEnhancement::ComputeBlobLikelihood(float lambda[3],int bright,double a2,double b2,double c2)
{
  if (lambda[2]>0.0)
    return 0.0;

  double RB= fabs(lambda[1]/lambda[2]);
  double RA= fabs(2*fabs(lambda[2])-fabs(lambda[1])-fabs(lambda[0]))/fabs(lambda[2]);
  double S= sqrt(lambda[0]*lambda[0]+lambda[1]*lambda[1]+lambda[2]*lambda[2]);
  
  double V1=(1.0-exp(RA*RA*a2));
  double V2=exp(RB*RB*b2);
  double V3=(1.0-exp(S*S*c2));
  double V=V1*V2*V3;
  return V;
  /*
  double V=exp(-lambda[2]/lambda[0]);
	      fprintf(stderr,"does it come here?3\n");
  if (V<0.0)
    return 0.0;
  if (V>40.0)
    return 40.0;
  return V;
  */
}

// --------------------------------------------------------------------------------------------
void vtkxqImageVesselEnhancement::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }

  int nimpc=input->GetNumberOfScalarComponents();

  if (nimpc!=1 && nimpc!=6)
    {
      vtkErrorMacro(<<"Either One or Six Component Images are valid inputs (i.e. Hessians)\n ****** \n\n\n\n\n\n");
      return;
    }

  vtkImageData* hessian=vtkImageData::New();
  if (nimpc==6)
    {
      hessian->ShallowCopy(input);
    }
  else
    {
      vtkxqImageHessian* hes=vtkxqImageHessian::New();
      hes->SetInput(input);
      hes->SetSigma(this->Sigma);
      hes->Update();
      hessian->ShallowCopy(hes->GetOutput());
      hes->Delete();
    }

  this->UpdateProgress(0.5);

  vtkDataArray* data=hessian->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();
  float A[3][3],lambda[3],W[3][3];

  int nc=out->GetNumberOfComponents();
  for (int i=0;i<nc;i++)
    out->FillComponent(i,0.0);

  int np=data->GetNumberOfTuples();

  //  fprintf(stderr,"Beginning Alpha=%.2f, Beta=%.2f Gamma=%.2f\n",Alpha,Beta,Gamma);

  double a2=-1.0/(this->Alpha*this->Alpha*2.0);
  double b2=-1.0/(this->Beta*this->Beta*2.0);
  double c2=-1.0/(this->Gamma*this->Gamma*2.0);

  double aa = this->Aa;

  vtkDataArray *mask=NULL;
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  int count=0;
  int tenth=np/10;
  for (int i=0;i<np;i++)
    {
      if (!((mask != NULL)?(mask->GetComponent(i, 0) > 0):1))
	continue;
      
      A[0][0]=data->GetComponent(i,0);
      A[0][1]=data->GetComponent(i,1);
      A[1][0]=A[0][1];

      A[0][2]=data->GetComponent(i,2);
      A[2][0]=A[0][2];

      A[1][1]=data->GetComponent(i,3);
      A[1][2]=data->GetComponent(i,4);
      A[2][1]=A[1][2];
      
      A[2][2]=data->GetComponent(i,5);
      
      vtkMath::Diagonalize3x3(A,lambda,W);

      for (int ia=0;ia<=1;ia++)
	{
	  double min=fabs(lambda[ia]);
	  int best=ia;
	  for (int ib=ia+1;ib<=2;ib++)
	    {
	      if (fabs(lambda[ib]) < min )
		{
		  min=fabs(lambda[ib]);
		  best=ib;
		}
	    }
	  if (best!=ia)
	    {
	      double tmp=lambda[ia];
	      lambda[ia]=lambda[best];
	      lambda[best]=tmp;
	    }
	}

      double V=0.0,V3=0.0,V2=0.0;
      int offset=-1;

      if (!isinf(lambda[0]) && !isinf(lambda[1]) && !isinf(lambda[2]))
	{
	  switch (this->StructureType)
	    {
	    case 0:
	      //V=this->ComputeVesselLikelihood(lambda,this->BrightStructure,a2,b2,c2);
  		  V=this->ComputexqVesselLikelihood(lambda,this->BrightStructure,aa);
	      break;
	    case 1:
	      V=this->ComputePlateLikelihood(lambda,this->BrightStructure,a2,b2,c2);
	      fprintf(stderr,"does it come here?\n");
	      break;
	    case 2:
	      V=this->ComputeBlobLikelihood(lambda,this->BrightStructure,a2,b2,c2);
	      break;
	    case 3:
	      V=this->ComputeVesselLikelihood(lambda,this->BrightStructure,a2,b2,c2);
	      V2=this->ComputePlateLikelihood(lambda,this->BrightStructure,a2,b2,c2);
	      V3=this->ComputeBlobLikelihood(lambda,this->BrightStructure,a2,b2,c2);
	      offset=3;
	      break;
	    case 4:
	      offset=0;
	    }
	  

	  if (!isinf(V))
	    out->SetComponent(i,0,V);

	  if (this->StructureType==3)
	    {
	      offset=3;
	      if (!isinf(V2))
		out->SetComponent(i,1,V2);
	      if (!isinf(V3))
		out->SetComponent(i,2,V3);
	    }

	  if (offset>-1)
	    {
	      out->SetComponent(i,offset,lambda[0]);
	      out->SetComponent(i,offset+1,lambda[1]);
	      out->SetComponent(i,offset+2,lambda[2]);
	    }
	}
      
      ++count;
      if (count==tenth)
	{
	  this->UpdateProgress(this->GetProgress()+0.05);
	  count=0;
	}
    }
  
  hessian->Delete();
}

// -----------------------------------------------------------------------------------------------------------

int vtkxqImageVesselEnhancement::UpdateHessianResponse(vtkImageData* hessian1,vtkImageData* vessel1,vtkImageData* sigma1,
							vtkImageData* hessian2,vtkImageData* vessel2,double newsigma)
{

  if (hessian1==NULL || vessel1==NULL || sigma1==NULL || hessian2 == NULL || vessel2==NULL)
    {
      fprintf(stderr,"Bad Inputs to Update Hessian Response (NULL)\n");
      return 0;
    }

  vtkDataArray* array[3][2];
  array[0][0]=hessian1->GetPointData()->GetScalars();
  array[1][0]=vessel1->GetPointData()->GetScalars();
  array[2][0]=sigma1->GetPointData()->GetScalars();

  array[0][1]=hessian2->GetPointData()->GetScalars();
  array[1][1]=vessel2->GetPointData()->GetScalars();


  int numvoxels=array[0][0]->GetNumberOfTuples();

  int flag=0;
  int nc[3];
  for (int ia=0;ia<=2;ia++)
    {
      int nt1=array[ia][0]->GetNumberOfTuples();
      int nc1=array[ia][0]->GetNumberOfComponents();

      int nt2=nt1,nc2=1;
      if (ia!=2)
	{
	  nt2=array[ia][1]->GetNumberOfTuples();
	  nc2=array[ia][1]->GetNumberOfComponents();
	}

      //      fprintf(stderr,"Checking row=%d   set1=%d x %d  set2=%d x %d\n",ia,nt1,nc1,nt2,nc2);

      if (nt1!=nt2 || nt1!=numvoxels || nc1!=nc2)
	{
	  flag=1;
	  ia=4;
	}
      nc[ia]=nc1;
    }

  if (flag==1)
    {
      fprintf(stderr,"Bad Inputs (Dimensions) to Update Hessian Response\n");
      return 0;
    }

  for (int voxel=0;voxel<numvoxels;voxel++)
    {
      double v1=array[1][0]->GetComponent(voxel,0);
      double v2=array[1][1]->GetComponent(voxel,0);
      double oldsig = array[2][0]->GetComponent(voxel,0);
      if ((v2/newsigma)>(v1/oldsig))
	{
	  for (int row=0;row<=1;row++)
	    for (int ia=0;ia<nc[row];ia++)
	      {
		if (row == 0)
		  array[row][0]->SetComponent(voxel,ia,array[row][1]->GetComponent(voxel,ia));
		else
		  array[row][0]->SetComponent(voxel,ia,(array[row][1]->GetComponent(voxel,ia)/newsigma));
	      }

	  array[2][0]->SetComponent(voxel,0,newsigma);
	}
    }

  for (int ia=0;ia<=2;ia++)
    array[ia][0]->Modified();

  //fprintf(stderr, "ok\n");

  return 1;
}

