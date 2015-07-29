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





#include "vtkbisImageReOrient.h"
#include "vtkImageExtractComponents.h"
#include "vtkbisImageReslice.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include <stdio.h>
#include "pxutil.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "nifti1_io.h"


vtkbisImageReOrient* vtkbisImageReOrient::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisImageReOrient");
  if(ret)
    {
      return (vtkbisImageReOrient*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisImageReOrient;
}


// Construct object to extract all of the input data.
vtkbisImageReOrient::vtkbisImageReOrient()
{
  this->SimpleMode=1;
  this->InputImageHeader=NULL;
  this->OutputImageHeader=NULL;
  this->FullMatrix=NULL;
  this->OutputOrientation=10;
}

void vtkbisImageReOrient::SetOutputOrientationToLPS()
{ 
  this->SetFullMatrix(NULL);
  this->SetSimpleMode(1); 
  this->SetOutputOrientation(10); 
}

void vtkbisImageReOrient::SetOutputOrientationToRAS()
{
  this->SetFullMatrix(NULL);
  this->SetSimpleMode(1); 
  this->SetOutputOrientation(13); 
}

void vtkbisImageReOrient::SetOutputOrientationToLIP()
{
  this->SetFullMatrix(NULL);
  this->SetSimpleMode(1); 
  this->SetOutputOrientation(20); 
}

void vtkbisImageReOrient::SetOutputOrientationToRSA()
{
  this->SetFullMatrix(NULL);
  this->SetSimpleMode(1); 
  this->SetOutputOrientation(27); 

}

void vtkbisImageReOrient::SetOutputOrientationToPSR()
{
  this->SetFullMatrix(NULL);
  this->SetSimpleMode(1); 
  this->SetOutputOrientation(30); 

}

void vtkbisImageReOrient::SetOutputOrientationToASR()
{
  this->SetFullMatrix(NULL);
  this->SetSimpleMode(1); 
  this->SetOutputOrientation(31); 
}


// ---------------------------------------------------------------------------

void vtkbisImageReOrient::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkSimpleImageToImageFilter::PrintSelf(os,indent);
}
// ---------------------------------------------------------------------------
int vtkbisImageReOrient::GenerateArraysAndOutputImageHeader()
{

  // Defaults
  for (int ia=0;ia<=2;ia++)
    {
      this->Axis[ia]=ia; 
      this->Flip[ia]=0;
    }

  if (this->InputImageHeader==NULL)
    return 0;

  
  nifti_1_header* hdr=this->InputImageHeader->GetRawHeader();
  mat44 B=vtkbisImageHeader::NIFTI_CreateOrientationMatrix(hdr);

  //  vtkbisImageHeader::NIFTI_print_mat44("Input B",B);

  

  
  mat44 A;
  vtkbisImageHeader::NIFTI_identity_mat44(&A);
  double spa[3]; this->InputImageHeader->GetSpacing(spa);
  
  if (this->SimpleMode==0 && this->FullMatrix!=NULL)
    {
      A=vtkbisImageHeader::NIFTI_Copy_Matrix4x4_mat44(this->FullMatrix);
    }
  else if (this->OutputOrientation==20)
    {
      A.m[0][0]=-1.0; A.m[1][1]=0.0; A.m[2][2]=0.0;
      A.m[1][2]=-1.0; A.m[2][1]=-1.0;
    }
  else if (this->OutputOrientation==27)
    {
      A.m[0][0]=1.0; A.m[1][1]=0.0; A.m[2][2]=0.0;
      A.m[1][2]=1.0; A.m[2][1]=1.0;
    }
  else if (this->OutputOrientation==30)
    {
      A.m[0][0]=0.0; A.m[1][1]=0.0; A.m[2][2]=0.0;
      A.m[0][2]=1.0; A.m[1][0]=-1.0; A.m[2][1]=1.0;
    }
  else if (this->OutputOrientation==31)
    {
      A.m[0][0]=0.0; A.m[1][1]=0.0; A.m[2][2]=0.0;
      A.m[0][2]=1.0; A.m[1][0]=1.0; A.m[2][1]=1.0;
    }
  else
    {
      // Standard Axial Stuff
      for (int ia=0;ia<=2;ia++)
	A.m[ia][ia]=spa[ia];
      if (this->OutputOrientation==10)
	{
	  A.m[0][0]=-A.m[0][0];
	  A.m[1][1]=-A.m[1][1];
	}
    }

  

  //  vtkbisImageHeader::NIFTI_print_mat44("Output A",A);

  // Out Orientation out=A*in;  A=out*in(-1)
  // To fix this one here ......
  mat44 invA=nifti_mat44_inverse(A);
  /*  for (int ia=0;ia<=2;ia++)
      origmat.m[3][ia]=0;*/
  // ...........

  // combo=A^{-1}*B;  A= inv(combo*B{-1}) = B*inv(combo)
  mat44 combo=vtkbisImageHeader::NIFTI_mat44_mmul( invA,B);
  //  vtkbisImageHeader::NIFTI_print_mat44("Mapping",combo);


  for (int ia=0;ia<=2;ia++)
    {
      this->Axis[ia]=0;
      if (combo.m[ia][0]<0)
	this->Flip[ia]=1;
      else
	this->Flip[ia]=0;


      for (int ib=0;ib<=2;ib++)
	{
	  if (fabs(combo.m[ia][ib]/spa[ib])>fabs(combo.m[ia][this->Axis[ia]]/spa[this->Axis[ia]]))
	    {
	      this->Axis[ia]=ib;
	      if (combo.m[ia][ib]<0)
		this->Flip[ia]=1;
	      else
		this->Flip[ia]=0;
	    }
	}
    }

  for (int ia=0;ia<=2;ia++)
    combo.m[ia][3]=0.0;   
  mat44 icombo=nifti_mat44_inverse(combo);
  mat44 ap=vtkbisImageHeader::NIFTI_mat44_mmul( B,icombo);
  //    vtkbisImageHeader::NIFTI_print_mat44("ap",ap);


  //  for (int ia=0;ia<=2;ia++)
  //    fprintf(stdout,"Setting output axis %d from %d (Flip=%d)\n",ia,this->Axis[ia],this->Flip[ia]);

  if (this->OutputImageHeader==NULL)
    this->OutputImageHeader=vtkbisImageHeader::New();
  
  this->OutputImageHeader->CopyHeader(this->InputImageHeader);
  this->OutputImageHeader->SetOrientationAsMatrix(ap);
  //  this->OutputImageHeader->PrintHeader(1);
  return 1;
}

// ---------------------------------------------------------------------------
int vtkbisImageReOrient::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector, 
					  vtkInformationVector *outputVector)
{
  vtkInformation *inInfo =  inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  int wholeExtent[6]; inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExtent);
  double sp[3];  inInfo->Get(vtkDataObject::SPACING(), sp);
  double origin[3];  inInfo->Get(vtkDataObject::ORIGIN(),  origin);


  //vtkImageData* input=this->GetImageDataInput(0);
  
  
  this->GenerateArraysAndOutputImageHeader();


  double outorigin[3],osp[3]; int outext[6];
  for (int ia=0;ia<=2;ia++)
    {
      outorigin[ia]=0.0;
      osp[ia]=sp[this->Axis[ia]];
      outext[2*ia]=wholeExtent[this->Axis[ia]*2];
      outext[2*ia+1]=wholeExtent[this->Axis[ia]*2+1];
    }
 
  outInfo->Set(vtkDataObject::ORIGIN(), outorigin, 3);
  outInfo->Set(vtkDataObject::SPACING(), osp, 3);
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),outext,6);
  return 1;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void vtkbisImageReOrient::SimpleExecute(vtkImageData* input, vtkImageData* output) 
{
  

  int dim[3]; input->GetDimensions(dim);
  int odim[3]; output->GetDimensions(odim);
  int nc=input->GetNumberOfScalarComponents();

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();
  int ia[3];

  int outincr[3];
  outincr[0]=1;
  outincr[1]=odim[0];
  outincr[2]=odim[0]*odim[1];

/*  fprintf(stderr,"OutIncr=%d,%d,%d, odim=%d,%d,%d idim=%d,%d,%d\n",
	  outincr[0],outincr[1],outincr[2],
	  odim[0],odim[1],odim[2],
	  dim[0],dim[1],dim[2]);*/
	  

  int index=0;
  for (ia[2]=0;ia[2]<dim[2];ia[2]++)
    {
      //      fprintf(stderr,"Doing Slice=%d\n",ia[2]);
      for (ia[1]=0;ia[1]<dim[1];ia[1]++)
	for (ia[0]=0;ia[0]<dim[0];ia[0]++)
	  {
	    int ib[3];
	    for (int j=0;j<=2;j++)
	      {
		ib[j]=ia[this->Axis[j]];
		if (this->Flip[j]>0)
		  ib[j]=(odim[j]-1)-ib[j];
	      }
	    int outindex=ib[0]*outincr[0]+ib[1]*outincr[1]+ib[2]*outincr[2];
	    
	    for (int c=0;c<nc;c++)
	      out->SetComponent(outindex,c,inp->GetComponent(index,c));
	    ++index;
	  }
    }

  //  this->OutputImageHeader->ReconcileImage(output,0);
}


