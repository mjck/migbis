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





#include "vtkpxImageExtract.h"
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
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"



vtkpxImageExtract* vtkpxImageExtract::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageExtract");
  if(ret)
    {
      return (vtkpxImageExtract*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageExtract;
}


// Construct object to extract all of the input data.
vtkpxImageExtract::vtkpxImageExtract()
{
  this->InternalCurrentPlane=2; 
  this->SliceNo=0;
  this->Frame=0;
  this->PolarMode=0;
  this->PolarMiddle=-1.0;
  this->LimitTextureSize=0;
  this->MaximumTextureSize=256;
  this->SampleRate[0]=1;
  this->SampleRate[1]=1;
  this->ForcePowerOfTwo=0;
}

// ---------------------------------------------------------------------------

void vtkpxImageExtract::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkSimpleImageToImageFilter::PrintSelf(os,indent);

  os << indent << "  InternalCurrentPlane " << this->InternalCurrentPlane << "\n";
  os << indent << "  SliceNo " << this->SliceNo << "\n";
  os << indent << "  Frame " << this->Frame << "\n";
  os << indent << "  PolarMode " << this->PolarMode << "\n";
  os << indent << "  PolarMiddle " << this->PolarMiddle << "\n";
  os << indent << "  LimitTextureSize " << this->LimitTextureSize << "\n";
  os << indent << "  ForcePowerOfTwo " << this->ForcePowerOfTwo << "\n";
  os << indent << "  MaximumTextureSize " << this->MaximumTextureSize << "\n";
  os << indent << "  SampleRate " << this->SampleRate[0] << " " << this->SampleRate[1] << "\n";
}
// ---------------------------------------------------------------------------

void vtkpxImageExtract::SetCurrentPlane(int a)
{
  this->SetInternalCurrentPlane(a);
}

int  vtkpxImageExtract::GetCurrentPlane()
{
  return this->InternalCurrentPlane;
}

// ---------------------------------------------------------------------------
int vtkpxImageExtract::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector, 
					  vtkInformationVector *outputVector)
{
  if (this->PolarMode==1)
    return this->RequestInformationPolar(request,inputVector,outputVector);

  vtkInformation *inInfo =  inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  int wholeExtent[6]; inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExtent);
  double sp[3];  inInfo->Get(vtkDataObject::SPACING(), sp);
  double origin[3];  inInfo->Get(vtkDataObject::ORIGIN(),  origin);

  vtkImageData* input=this->GetImageDataInput(0);
  if (input!=NULL)
    {
      input->GetExtent(wholeExtent);
      input->GetSpacing(sp);
      input->GetOrigin(origin);
    }
  int nc=input->GetNumberOfScalarComponents();


  /*  if (this->InternalCurrentPlane==1)
    fprintf(stderr,"\n\nInput Whole Extent=%d:%d,%d:%d,%d:%d, spacing=%f,%f,%f\n",
	    wholeExtent[0],wholeExtent[1],
	    wholeExtent[2],wholeExtent[3],
	    wholeExtent[4],wholeExtent[5],sp[0],sp[1],sp[2]);*/
  
  int dim[3];   dim[0]=wholeExtent[1]+1;  dim[1]=wholeExtent[3]+1;  dim[2]=wholeExtent[5]+1;

  // Output is always 2D
  wholeExtent[5]=0;
  double osp[3];

  switch (this->InternalCurrentPlane)
    {
    case 2:
      wholeExtent[1] = dim[0]-1;
      wholeExtent[3] = dim[1]-1;
      osp[0]=sp[0];osp[1]=sp[1];osp[2]=sp[2];
      break;
    case 1:
      wholeExtent[1] = dim[0]-1;
      wholeExtent[3] = dim[2]-1;
      osp[0]=sp[0];osp[1]=sp[2];osp[2]=sp[1];
      break;
    case 0:
      wholeExtent[1] = dim[1]-1;
      wholeExtent[3] = dim[2]-1;
      osp[0]=sp[1];osp[1]=sp[2];osp[2]=sp[0];
      break;
    }


  if (this->ForcePowerOfTwo) 
    {
      this->unpaddedoutdim[0]=wholeExtent[1]+1;
      this->unpaddedoutdim[1]=wholeExtent[3]+1;
      this->unpaddedoutdim[2]=wholeExtent[5]+1;
      for (int i=1;i<=3;i+=2)
	{
	  int d=wholeExtent[i]+1;
	  int sz=2;
	  while (sz<d)
	    sz*=2;
	  wholeExtent[i]=sz-1;
	}
    }
  else 
    {
      if (this->LimitTextureSize)
	{
	  
	  for (int i=0;i<=1;i++)
	    {
	      int index=2*i+1;
	      int dimension=wholeExtent[index]+1;
	      this->SampleRate[i]=1;
	      while ( dimension > this->MaximumTextureSize*this->SampleRate[i])
		this->SampleRate[i]+=1;
	      
	      wholeExtent[index]=int(dimension/this->SampleRate[i])-1;
	      osp[i]=osp[i]*float(this->SampleRate[i]);
	    }
	}
      this->unpaddedoutdim[0]=wholeExtent[1]+1;
      this->unpaddedoutdim[1]=wholeExtent[3]+1;
      this->unpaddedoutdim[2]=wholeExtent[5]+1;
    }

  
  if (this->SliceNo<0)
    {
      this->SliceNo=0;
    }
  else if (this->SliceNo>=dim[this->InternalCurrentPlane])
    {
      this->SliceNo=dim[this->InternalCurrentPlane]-1;
    }


  double outorigin[3];
  switch (this->InternalCurrentPlane)
    {
    case 2:
      outorigin[0]=origin[0];outorigin[1]=origin[1];outorigin[2]=double(this->SliceNo)*sp[2]+origin[2];
      break;
    case 1:
      //      output->SetOrigin(origin[0],origin[2],double(this->SliceNo)*sp[1]+origin[1]);
      outorigin[0]=origin[0];outorigin[1]=origin[2];outorigin[2]=double(this->SliceNo)*sp[1]+origin[1];
      break;
    case 0:
      //      output->SetOrigin(origin[1],origin[2],double(this->SliceNo)*sp[0]+origin[0]);
      outorigin[0]=origin[1];outorigin[1]=origin[2];outorigin[2]=double(this->SliceNo)*sp[0]+origin[0];
      break;
    }
 
  outInfo->Set(vtkDataObject::ORIGIN(), outorigin, 3);
  outInfo->Set(vtkDataObject::SPACING(), osp, 3);
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),wholeExtent,6);

  if (this->ExtractAllFrames && this->PolarMode ==0 )
    {
      // Do Nothing
    }
  else
    vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, 1);
  return 1;
}
// ---------------------------------------------------------------------------
template<class T>
int vtkpxImageExtract_ExtractSlice(vtkpxImageExtract* self,vtkImageData* input,
				   vtkImageData* output,
				   int internalcurrentplane,int sliceno,int frame,
				   int samplerate[2],int unpaddedoutdim[3],T *)
{

  int dim[3]; input->GetDimensions(dim);
  int nframes=input->GetNumberOfScalarComponents();
  int outdim[3]; output->GetDimensions(outdim);

  vtkDataArray* inp_dat=input->GetPointData()->GetScalars();
  T* inp=(T*)inp_dat->GetVoidPointer(0);

  vtkDataArray* out_dat=output->GetPointData()->GetScalars();
  T* out=(T*)out_dat->GetVoidPointer(0);


  output->GetPointData()->GetScalars()->FillComponent(0,0.0);


  switch(internalcurrentplane)
    {
    case 2:
      {
	int iincr=nframes*samplerate[0];
	int offset=dim[0]*dim[1]*sliceno;
	int in_j=0;
	for (int j=0;j<unpaddedoutdim[1];++j)
	  {
	    int inindex = (dim[0]*in_j+offset)*nframes+frame;
	    int outindex=j*outdim[0];
	    for (int i=0;i<unpaddedoutdim[0];++i)
	      {
		out[outindex]=inp[inindex];
		++outindex;
		inindex+=iincr;
	      }
	    in_j+=samplerate[1];
	  }
      }
      break;
      
    case 1:
      {
	int dim01=dim[0]*dim[1];
	int offset=sliceno*dim[0];
	int in_k=0;
	int iincr=nframes*samplerate[0];
	for (int k=0;k<unpaddedoutdim[1];++k)
	  {
	    int inindex=(in_k*dim01+offset)*nframes+frame;
	    int outindex=k*outdim[0];
	    for (int i=0;i<unpaddedoutdim[0];++i)
	      {
		out[outindex]=inp[inindex];
		++outindex;
		inindex+=iincr;
	      }
	    in_k+=samplerate[1];
	  }
      }
      break;
      
    case 0:
      {
	int dim01=dim[0]*dim[1];
	int offset=sliceno;
	int in_k=0;
	int jincr=nframes*samplerate[0]*dim[0];
	for (int k=0;k<unpaddedoutdim[1];++k)
	  {
	    int outindex=k*outdim[0];
	    int inindex=(in_k*dim01+offset)*nframes+frame;
	    for (int j=0;j<unpaddedoutdim[0];++j)
	      {
		out[outindex]=inp[inindex];
		inindex+=jincr;
		++outindex;
	      }
	    in_k+=samplerate[1];
	  }
      }
      break;
    }
  return 0;
}

// ---------------------------------------------------------------------------
template<class T>
int vtkpxImageExtract_ExtractSliceAllFrames(vtkpxImageExtract* self,vtkImageData* input,
					    vtkImageData* output,
					    int internalcurrentplane,int sliceno,
					    int samplerate[2],int unpaddedoutdim[3],T *)
{
  //  fprintf(stderr,"In Extract Slice All Frames plane=%d, %d x %d\n",internalcurrentplane, samplerate[0],samplerate[1]);



  int dim[3]; input->GetDimensions(dim);
  int nframes=input->GetNumberOfScalarComponents();
  int outdim[3]; output->GetDimensions(outdim);
  int outframes=output->GetNumberOfScalarComponents();

  //  fprintf(stderr,"In Extract Slice All Frames inframes=%d, outframes=%d , unpad = %d x %d\n",nframes,outframes, unpaddedoutdim[0],unpaddedoutdim[1]);
  //  fprintf(stderr,"In Extract Slice All Frames dim=(%d,%d,%d), outdim=(%d,%d,%d)\n",dim[0],dim[1],dim[2],outdim[0],outdim[1],outdim[2]);

  vtkDataArray* inp_dat=input->GetPointData()->GetScalars();
  T* inp=(T*)inp_dat->GetVoidPointer(0);

  vtkDataArray* out_dat=output->GetPointData()->GetScalars();
  T* out=(T*)out_dat->GetVoidPointer(0);

  for (int ia=0;ia<nframes;ia++)
    output->GetPointData()->GetScalars()->FillComponent(ia,double(ia));

  switch(internalcurrentplane)
    {
    case 2:
      {
	int in_j=0;
	int offset=dim[0]*dim[1]*sliceno;

	for (int j=0;j<unpaddedoutdim[1];j++)
	  {
	    int inindex =(in_j*dim[0]+offset)*nframes;
	    int outindex= j*outdim[0]*nframes;
	    for (int i=0;i<unpaddedoutdim[0];i++)
	      {
		for (int c=0;c<nframes;c++)
		  {
		    out[outindex]=inp[inindex];
		    ++outindex;
		    ++inindex;
		  }
		inindex+=((samplerate[0]-1)*nframes);
	      }
	    in_j+=samplerate[1];
	  }
      }
      break;
      
    case 1:
      {
	int dim01=dim[0]*dim[1];
	int offset=sliceno*dim[0];
	int in_k=0;
	int iincr=nframes*(samplerate[0]-1);
	for (int k=0;k<unpaddedoutdim[1];++k)
	  {
	    int inindex=(in_k*dim01+offset)*nframes;
	    int outindex=(k*outdim[0])*nframes;
	    for (int i=0;i<unpaddedoutdim[0];++i)
	      {
		for (int c=0;c<nframes;c++)
		  {
		    out[outindex]=inp[inindex];
		    ++outindex;
		    ++inindex;
		  }
		inindex+=iincr;
	      }
	    in_k+=samplerate[1];
	  }
      }
      break;
      
    case 0:
      {
	int dim01=dim[0]*dim[1];
	int offset=sliceno;
	int in_k=0;
	int inincr=(dim[0]-1)*nframes*samplerate[0];
	for (int k=0;k<unpaddedoutdim[1];++k)
	  {
	    int outindex=k*outdim[0]*nframes;
	    int inindex=(in_k*dim01+offset)*nframes;
	    for (int j=0;j<unpaddedoutdim[0];++j)
	      {
		for (int c=0;c<nframes;c++)
		  {
		    out[outindex]=inp[inindex];
		    ++outindex;
		    ++inindex;
		  }
		inindex+=inincr;
	      }
	    in_k+=samplerate[1];
	  }
      }
      break;
    case -1:
      {
	int dim01=dim[0]*dim[1];
	int offset=sliceno;
	int in_k=0;
	int jincr=nframes*(samplerate[0])*dim[0];
	for (int k=0;k<unpaddedoutdim[1];++k)
	  {
	    int outindex=k*outdim[0]*nframes;
	    int inindex=(in_k*dim01+offset)*nframes;
	    for (int j=0;j<unpaddedoutdim[0];++j)
	      {
		for (int c=0;c<nframes;c++)
		  {
		    out[outindex]=inp[inindex];
		    ++outindex;
		    ++inindex;
		  }
		inindex+=jincr;
	      }
	    in_k+=samplerate[1];
	  }
      }
    }


  //  fprintf(stderr,"Done with Extract All Frames\n");
  return 0;
}

// ---------------------------------------------------------------------------
void vtkpxImageExtract::SimpleExecute(vtkImageData* input, vtkImageData* output) 
{
  if (this->PolarMode)
    {
      this->SimpleExecutePolar(input,output);
      return;
    }

  vtkDebugMacro(<< "Extracting Slice");

  int dim[3];   input->GetDimensions(dim);
  int outdim[3];  output->GetDimensions(outdim);

  //  fprintf(stderr,"OutDim= %d %d %d\n",outdim[0],outdim[1],outdim[2]);

  vtkDataArray* inp=input->GetPointData()->GetScalars();  

  if (this->InternalCurrentPlane<0)
      this->InternalCurrentPlane=0;
  else if (this->InternalCurrentPlane>2)
      this->InternalCurrentPlane=2;


  if (this->SliceNo<0)
      this->SliceNo=0;
  else if (this->SliceNo>=dim[this->InternalCurrentPlane])
    {
      this->SliceNo=dim[this->InternalCurrentPlane]-1;
    }

  // Color Mode
  if (this->ExtractAllFrames)
    {
      //      fprintf(stderr,"In Extract All Frames\n");
      //      fprintf(stderr,"outdim = %d x %d x %d, nc=%d\n",outdim[0],outdim[1],outdim[2],output->GetNumberOfScalarComponents());
      switch (input->GetScalarType())
	{
	  vtkTemplateMacro8(vtkpxImageExtract_ExtractSliceAllFrames, this, input,output,
			    this->InternalCurrentPlane,
			    this->SliceNo,
			    this->SampleRate,
			    this->unpaddedoutdim,
			    static_cast<VTK_TT *>(0));
	  
	default:
	  vtkGenericWarningMacro("Unknown input ScalarType in Write Data");
	  return;
	}


    }
  else
    {
      //      fprintf(stderr,"In Extract Single Frame\n");
      if (this->Frame<0)
	this->Frame=0;
      if ( this->Frame>=input->GetNumberOfScalarComponents())
	this->Frame=input->GetNumberOfScalarComponents()-1;
      
      switch (input->GetScalarType())
	{
	  vtkTemplateMacro9(vtkpxImageExtract_ExtractSlice, this, input,output,
			    this->InternalCurrentPlane,
			    this->SliceNo,
			    this->Frame,
			    this->SampleRate,
			    this->unpaddedoutdim,
			    static_cast<VTK_TT *>(0));
	  
	default:
	  vtkGenericWarningMacro("Unknown input ScalarType in Write Data");
	  return;
	}
    }

  

  /*  vtkDataArray* out=output->GetPointData()->GetScalars();



  switch(this->InternalCurrentPlane)
    {
    case 2:
      {
	int offset=dim[0]*dim[1]*this->SliceNo;
	int in_j=0;
	for (int j=0;j<outdim[1];++j)
	  {
	    int in_i=0;
	    for (int i=0;i<outdim[0];++i)
	      {
		out->SetComponent(i+j*outdim[0],0,inp->GetComponent(in_i+dim[0]*in_j+offset,this->Frame));
		in_i+=this->SampleRate[0];
	      }
	    in_j+=this->SampleRate[1];
	  }
      }
      break;
      
    case 1:
      {
	int dim01=dim[0]*dim[1];
	int offset=this->SliceNo*dim[0];
	int in_k=0;
	for (int k=0;k<outdim[1];++k)
	  {
	    int in_i=0;
	    for (int i=0;i<outdim[0];++i)
	      {
		out->SetComponent(i+k*outdim[0],0,
				  inp->GetComponent(in_k*dim01+offset+in_i,this->Frame));
		in_i+=this->SampleRate[0];
	      }
	    in_k+=this->SampleRate[1];
	  }
      }
      break;
      
    case 0:
      {
	int dim01=dim[0]*dim[1];
	int offset=this->SliceNo;
	int in_k=0;
	for (int k=0;k<outdim[1];++k)
	  {
	    int in_j=0;
	    for (int j=0;j<outdim[0];++j)
	      {
		out->SetComponent(j+k*outdim[0],0,
				  inp->GetComponent(in_k*dim01+in_j*dim[0]+offset,this->Frame));
		in_j+=this->SampleRate[0];
	      }
	    in_k+=this->SampleRate[1];
	  }
      }
      break;
      }*/
      
}

// ---------------------------------------------------------------------------
//             Polar Mode Stuff 
//   Plane 2 is XY of the current angle 
//   Plane 1 is XY of the current angle + 90 deg
//   Plane 0 is XZ reconstructed 
// ---------------------------------------------------------------------------
int vtkpxImageExtract::RequestInformationPolar(vtkInformation *request,  vtkInformationVector **inputVector, 
					       vtkInformationVector *outputVector)
{

  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  int wholeExtent[6]; inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExtent);
  double spa[3];  inInfo->Get(vtkDataObject::SPACING(), spa);
  double ori[3];  inInfo->Get(vtkDataObject::ORIGIN(),  ori);
  int dim[3];   dim[0]=wholeExtent[1]+1;  dim[1]=wholeExtent[3]+1;  dim[2]=wholeExtent[5]+1;

  // Uniform Output Spacing
  double outspa[3]; for (int ia=0;ia<=2;ia++) outspa[ia]=spa[0];
  //  output->SetSpacing(input->GetSpacing()[0],input->GetSpacing()[0],input->GetSpacing()[0]);

  //output->SetOrigin(input->GetOrigin());
  
  wholeExtent[0]=0;    wholeExtent[2]=0;  wholeExtent[4]=0;    wholeExtent[5]=0;
    
  switch (this->InternalCurrentPlane)
    {
    case 2:
    case 1:
      wholeExtent[1] = dim[0]-1;
      wholeExtent[3] = dim[1]-1;
      break;
    case 0:
      wholeExtent[1] = dim[0]-1;
      wholeExtent[3] = dim[0]-1;
      break;
    }
  
  
  if (this->SliceNo<0)
    {
      this->SliceNo=0;
    }
  else if (this->SliceNo>=dim[this->InternalCurrentPlane])
    {
      this->SliceNo=dim[this->InternalCurrentPlane]-1;
    }
  
  outInfo->Set(vtkDataObject::ORIGIN(), ori, 3);
  outInfo->Set(vtkDataObject::SPACING(), outspa, 3);
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),wholeExtent,6);
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, 1);
  return 1; 
}


float vtkpxImageExtract::Get3DEchoVoxel(vtkImageData* img,int dim[3],int x,int y,int z,int comp,
					float angularspacing,float middlex)
{
  int flag=0;
  /*  if ( (x==50 && y==50 ) || ( x==(dim[0]-50) && y==(dim[1]-50)))
      flag=1;*/


  if (middlex<0.0) middlex=(dim[0])/2.0;
  float slice_y=Frange(z,0.0,(float)dim[1]-1);

  float px=float(x)-middlex;
  float py=float(y)-middlex;

  float radius=sqrt(px*px+py*py);
  float theta=atan2(py,px)*180.0/M_PI;


  if (theta< (000.0-0.5*angularspacing)) theta+=180.0;
  if (theta>=(180.0-0.5*angularspacing)) theta-=180.0;
  float slice_z=fabs(theta)/angularspacing;
  
  int slice_no=int(slice_z+0.5);

  if (py<0.0 && slice_no!=0)  radius=-radius;
  if (slice_no==0 && px<0.0)  radius=-radius;
  slice_no=Irange(slice_no,0,dim[2]-1);
  float slice_x=radius+middlex;

  /*if (flag)
    fprintf(stderr,"x=%d, y=%d midx=%.2f --> px,py=(%.2f,%.2f) ,radius=%.2f theta=%.2f\n\t slice_no=%d  --> %d,%d,%d\n",
    x,y,middlex,px,py,radius,theta,slice_no,int(slice_x+0.5),int(slice_y+0.5),slice_no);*/
	    


  if (slice_x>=0 && slice_x<dim[0]-5)
    return img->GetScalarComponentAsDouble(int(slice_x+0.5),int(slice_y+0.5),slice_no,comp);
  else
    return 0.0;
}


void vtkpxImageExtract::SimpleExecutePolar(vtkImageData* input, vtkImageData* output) 
{
  vtkDebugMacro(<< "Extracting Polar Slice");

  int dim[3];   input->GetDimensions(dim);
  double ori[3];  input->GetOrigin(ori);
  double sp[3]; input->GetSpacing(sp);

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  int dty=inp->GetDataType();

  output->SetSpacing(input->GetSpacing()[0],input->GetSpacing()[0],input->GetSpacing()[0]);
  output->SetScalarType(dty);
  output->SetNumberOfScalarComponents(1);
  output->SetOrigin(0.0,0.0,0.0);

  if (this->InternalCurrentPlane<0)
      this->InternalCurrentPlane=0;
  else if (this->InternalCurrentPlane>2)
      this->InternalCurrentPlane=2;


  if (this->SliceNo<0)
    {
      this->SliceNo=0;
    }
  else if (this->InternalCurrentPlane==0 && this->SliceNo>=dim[1])
    {
      this->SliceNo=dim[1]-1;
    }
  else if (this->InternalCurrentPlane>0 && this->SliceNo>=dim[2])
    {
      this->SliceNo=dim[2]-1;
    }

  if (this->Frame<0)
    this->Frame=0;
  if ( this->Frame>=input->GetNumberOfScalarComponents())
    this->Frame=input->GetNumberOfScalarComponents()-1;

  switch(this->InternalCurrentPlane)
      {
      case 2:
      case 1:
	{
	  output->SetDimensions(dim[0],dim[1],1);
	  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,0);
	  output->AllocateScalars();
	  vtkDataArray* out=output->GetPointData()->GetScalars();
	  int offset=dim[0]*dim[1]*this->SliceNo;
	  //fprintf(stderr,"\t\t\t ***** Image Extract this=%d Plane=%d Slice=%d\n",this,InternalCurrentPlane,SliceNo);
	  for (int j=0;j<dim[1];j++)
	    for (int i=0;i<dim[0];i++)
	      out->SetComponent(i+j*dim[0],0,inp->GetComponent(i+dim[0]*j+offset,this->Frame));
	}
	break;

      case 0:
	{
	  float middlex=this->PolarMiddle;
	  if (middlex<0)
	    middlex=float(dim[0])/2.0;
	  float angularspacing=180.0/float(dim[2]);
	  output->SetDimensions(dim[0],dim[0],1);
	  output->SetWholeExtent(0,dim[0]-1, 0,dim[0]-1, 0,0);
	  output->AllocateScalars();
	  vtkDataArray* out=output->GetPointData()->GetScalars();

	  //fprintf(stderr,"\t\t\t **** Image Extract this=%d Plane=%d Slice=%d\n",this,InternalCurrentPlane,SliceNo);

	  int iz=this->SliceNo;
	  for (int iy=0;iy<dim[0];iy++)
	    for (int ix=0;ix<dim[0];ix++)
	      out->SetComponent(ix+iy*dim[0],0,
				this->Get3DEchoVoxel(input,dim,ix,iy,iz,this->Frame,angularspacing,middlex));
	}
	break;
     }

  vtkErrorMacro("output" << output);
}


