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
  Module:    $RCSfile: vtkpxUtil.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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

#include "vtkAbstractTransform.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkIdentityTransform.h"
#include "vtkImageData.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkbisImageResample.h"
#include "vtkbisImageHeader.h"
#include "vtkbisImageReslice.h"
#include "vtkMath.h"
#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"
#include "vtkStructuredPoints.h"
#include "vtkTransform.h"
#include "vtkpxAnalyzeImageWriter.h"
#include "vtkpxImageExtract.h"
#include "vtkpxUtil.h"
#include "vtkpxImageExtractVOI.h"
#include "vtkImageAccumulate.h"
#include "vtkImageResample.h"
#include "vtkImageExtractComponents.h"
#include "vtkPointData.h"


// ---------------------------

vtkpxUtil* vtkpxUtil::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxUtil");
  if(ret)
      {
	return (vtkpxUtil*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxUtil;
}


/*=========================================================================


         C R E A T E  / C O P Y    I M A G E S

=========================================================================*/

int vtkpxUtil::CreateImage(vtkImageData* img,vtkImageData* Image)
{
  if (Image==NULL)
    return 0;

  int dim[3];  Image->GetDimensions(dim);
  double sp[3];  Image->GetSpacing(sp);
  double ori[3];  Image->GetOrigin(ori);
  int c=Image->GetNumberOfScalarComponents();
  int t=Image->GetScalarType();

  return vtkpxUtil::CreateImage(img,dim[0],dim[1],dim[2],c,t,
				sp[0],sp[1],sp[2],ori[0],ori[1],ori[2]);
  
}

int vtkpxUtil::CreateFloatImage(vtkImageData* img,
						 int width,int height,int depth,int numcomp,
						 float sp1,float sp2,float sp3,
						 float or1,float or2,float or3)
{
  return vtkpxUtil::CreateImage(img,width,height,depth,numcomp,VTK_FLOAT,
				sp1,sp2,sp3,or1,or2,or3);
}

int vtkpxUtil::CreateShortImage(vtkImageData* img,
						 int width,int height,int depth,int numcomp,
						 float sp1,float sp2,float sp3,
						 float or1,float or2,float or3)
{
  return vtkpxUtil::CreateImage(img,width,height,depth,numcomp,VTK_SHORT,
				sp1,sp2,sp3,or1,or2,or3);

}


int vtkpxUtil::CreateImage(vtkImageData* img,
			   int width,int height,int depth,int numcomp,
			   int scalar_type,
			   float sp1,float sp2,float sp3,
					    float or1,float or2,float or3)
{
  if (img==NULL)
    return 0;

  img->SetSpacing(sp1,sp2,sp3);
  img->SetOrigin(or1,or2,or3);
  img->SetScalarType(scalar_type);
  img->SetNumberOfScalarComponents(numcomp);
  img->SetDimensions(width,height,depth);
  img->SetWholeExtent(0,width-1,0,height-1,0,depth-1);
  img->SetExtent(0,width-1,0,height-1,0,depth-1);
  img->AllocateScalars();
  return 1;
}

/*=========================================================================


         B A S I C   I M A G E    U T I L I T I E S


=========================================================================*/

int vtkpxUtil::SaveAnalyze(const char* filename,vtkImageData* saveThisImage,int compression)
{
  if (saveThisImage==NULL)
      return 0;

  vtkpxAnalyzeImageWriter* ana=vtkpxAnalyzeImageWriter::New();

  ana->SetInput(saveThisImage);
  ana->SetCompression(compression);
  ana->Save(filename);
  ana->Delete();

  return 1;
}
//------------------------------------------------------------------------------
int vtkpxUtil::ReorderImage(vtkImageData* destination,vtkImageData* source)
{

  // Frames And Slices are output correct i.e. the correct output number of frames and slices

  if (source==NULL || destination==NULL)
      return 0;

  int dim[3];
  source->GetDimensions(dim);

  int slices=source->GetNumberOfScalarComponents();
  int frames=dim[2];


  destination->CopyStructure(source);
  destination->SetScalarType(source->GetScalarType());
  destination->SetNumberOfScalarComponents(frames);
  destination->SetDimensions(dim[0],dim[1],slices);
  destination->AllocateScalars();

  int numpixels=dim[0]*dim[1];
  vtkDataArray* destScalars=destination->GetPointData()->GetScalars();
  vtkDataArray* sourceScalars=source->GetPointData()->GetScalars();
  
  /*  fprintf(stdout,"Num Scalars output=%d, input=%d\n",
	  destScalars->GetNumberOfTuples(),
	  sourceScalars->GetNumberOfTuples());*/

  for (int outputslice=0;outputslice<slices;outputslice++)
    {
      int  index_out=outputslice*numpixels;
      for (int outputframe=0;outputframe<frames;outputframe++)
	{
	  int inputslice=outputframe;
	  int inputframe=outputslice;
	  int  index_in =inputslice*numpixels;
	  //  fprintf(stderr,"Mapping input slice=%d, frame=%d (index_in=%d) to output slice=%d,frame=%d (index_out=%d)\n",inputslice,inputframe,index_in,outputslice,outputframe,index_out);
	  for (int k=0;k<numpixels;k++)
	    destScalars->SetComponent(index_out+k,outputframe,sourceScalars->GetComponent(index_in+k,inputframe));
      }
    }
  return 1;
}

// -------------------------------------------------------------------------------------
//   Multi-scale pyramid creation
// -------------------------------------------------------------------------------------
int vtkpxUtil::ResampleAndSmoothImage(vtkImageData* destination,
				      vtkImageData* source,
				      float gaussian,
				      float resolution)
{
  return vtkpxUtil::ResampleAndSmoothImage(destination,source,gaussian,resolution,0.0);
}


int vtkpxUtil::ResampleAndSmoothImage(vtkImageData* destination,
				      vtkImageData* source,
				      float gaussian,
				      float resolution,
				      float padvalue)
{

  if (source==NULL || destination==NULL)
      return 0;

  if (source->GetPointData()==NULL)
    return 0;

  double sp[3];  source->GetSpacing(sp);
  double ori[3]; source->GetOrigin(ori);
  int dim[3];   source->GetDimensions(dim);


  int ext[6];
  for (int i=0;i<3;i++)
    {
      ext[i*2]=0;
      ext[i*2+1] = int(0.5+(dim[i]*sp[i])/resolution)-1;
    }

  // Step 1 Smooth 
  double gp[3];
  for (int ia=0;ia<=2;ia++)
    gp[ia]=(gaussian/sp[ia]);

  vtkbisImageReslice* res=vtkbisImageReslice::New();
  res->OptimizationOff();
  if (gaussian > 0.1 )
    {
      vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
      smooth->SetInput(source);
      smooth->SetStandardDeviations(gp);
      res->SetInput(smooth->GetOutput());
      smooth->Delete();
    }
  else
    res->SetInput(source);
  
  res->SetOutputOrigin(source->GetOrigin());
  res->SetOutputSpacing(resolution,resolution,resolution);
  res->SetInterpolationMode(1);
  res->SetOutputExtent(ext);
  res->SetBackgroundLevel(padvalue);
  res->Update();

  destination->ShallowCopy(res->GetOutput());
  res->Delete();

  return 1;
}


// -------------------------------------------------------------------------------------
//   Compute Image Difference
// -------------------------------------------------------------------------------------

float vtkpxUtil::ComputeAbsoluteImageDifference(vtkImageData* image1,
						vtkImageData* image2,
						int ignoreon,float ignore)
{
  int dim1[3]; image1->GetDimensions(dim1);
  int dim2[3]; image1->GetDimensions(dim2);

  int diff=0;
  for (int ia=0;ia<=2;ia++)
    diff+=abs(dim1[ia]-dim2[ia]);

  if (diff!=0)
    return -1.0;

  int totalpixels=dim1[0]*dim1[1]*dim1[2];
  float sum=0.0;
  int used=0;
  for (int px=0;px<totalpixels;px++)
    {
      float p1=image1->GetPointData()->GetScalars()->GetComponent(px,0);
      float p2=image2->GetPointData()->GetScalars()->GetComponent(px,0);

      if (ignoreon==0 || ( p1!=ignore && p2!=ignore)) 
	{
	  sum+=fabs(p1-p2);
	  ++used;
	}
    }

  if (used>0)
    sum/=float(used);
  return sum;
}


/*=========================================================================


         C O D E   T O    R E O R I E N T    I M A G E S


=========================================================================*/


//------------------------------------------------------------------------------
//  ReOrient Image Stuff 
//------------------------------------------------------------------------------

int vtkpxUtil::ReOrientMatrix(vtkImageData* source,int inputorient,int outputorient,vtkMatrix4x4* tmat,int centered)
{

  if (tmat==NULL || source==NULL)
    return 0;
  
  int   dim[3];
  double sp[3];
  double ori[3];
  source->GetDimensions(dim);
  source->GetSpacing(sp);
  source->GetOrigin(ori);

  return vtkpxUtil::ReOrientMatrix(ori,sp,dim,inputorient,outputorient,tmat,centered);
}

// ----------------------------------------------------------------------------------------------------------

int vtkpxUtil::ReOrientMatrix(double ori[3],double sp[3],int dim[3],
			      int inputorient,int outputorient,vtkMatrix4x4* tmat,
			      int centered)
{
  if (tmat==NULL)
    return 0;
  
  // Xenios -- to do stuff
  // This should be generalized for all vectors etc.
  // In fact the easiest way would be to simply generate matrices for each orientation
  // and then multiply them
  inputorient=vtkbisImageHeader::MapOrientationToOldStyle(inputorient);
  outputorient=vtkbisImageHeader::MapOrientationToOldStyle(outputorient);



  /*  fprintf(stderr,"Inputs ori=%f,%f,%f sp=%f,%f,%f dim=%d,%d,%d orient=%d,%d\n",
	  ori[0],ori[1],ori[2],
	  sp[0],sp[1],sp[2],
	  dim[0],dim[1],dim[2],inputorient,outputorient);*/

  vtkMatrix4x4* mat=vtkMatrix4x4::New();
  mat->Identity();
  
  if (inputorient==outputorient)
    return 1;

  if (inputorient==1 && outputorient==0)
    {
      // Coronal->Axial  
      mat->SetElement(1, 1, 0); mat->SetElement( 1, 2, -1);
      mat->SetElement(2, 1, 1); mat->SetElement( 2, 2,  0);
    }
   
  if (inputorient==0 && outputorient==1)
    {
      // Axial->Coronal  
      mat->SetElement(1, 1,  0); mat->SetElement(1, 2, 1);
      mat->SetElement(2, 1, -1); mat->SetElement(2, 2, 0);
    }
	
  if (inputorient==1 && outputorient==2)
    {
      // Coronal->Sagittal  
      mat->SetElement(0, 0, 0) ;  mat->SetElement(1, 0, 0 );  mat->SetElement(2, 0, 1);
      mat->SetElement(1, 1, -1);
      mat->SetElement(0, 2, -1 ); mat->SetElement(1, 2, 0 ); mat->SetElement(2, 2, 0);
    }
  
  if (inputorient==2 && outputorient==1)
    {
      // Sagittal->Coronal  
      mat->SetElement(0, 0, 0) ;  mat->SetElement(0, 1, 0 );  mat->SetElement(0, 2, 1);
      mat->SetElement(1, 1, -1);
      mat->SetElement(2, 0, -1 ); mat->SetElement(2, 1, 0 ); mat->SetElement(2, 2, 0);
    }

  
  if (inputorient==0 && outputorient==2)
    {
      // Axial->Sagittal  
      mat->SetElement(0, 0,  0) ;  mat->SetElement(1, 0, 1) ;  mat->SetElement(2, 0, 0);
      mat->SetElement(0, 1,  0) ;  mat->SetElement(1, 1, 0) ;  mat->SetElement(2, 1, 1);
      mat->SetElement(0, 2, -1) ;  mat->SetElement(1, 2, 0) ;  mat->SetElement(2, 2, 0);
    }

  if (inputorient==2 && outputorient==0)
    {
      // Sagittal->Axial  
      mat->SetElement(0, 0,  0) ;  mat->SetElement(0, 1, 1) ;  mat->SetElement(0, 2, 0);
      mat->SetElement(1, 0,  0) ;  mat->SetElement(1, 1, 0) ;  mat->SetElement(1, 2, 1);
      mat->SetElement(2, 0, -1) ;  mat->SetElement(2, 1, 0) ;  mat->SetElement(2, 2, 0);
    }


  //  mat->Print(cerr);

  if (centered==0)
    {
      double sp_out[3]; int dim_out[3];
      vtkpxUtil::ReOrientDimensions(sp,dim,inputorient,outputorient,sp_out,dim_out);
      float shift_y[3],shift_x[3];
      for (int i=0;i<=2;i++)
	{
	  shift_y[i]= 0.5*sp[i]*float(dim[i]-1);
	  shift_x[i]=-0.5*sp_out[i]*float(dim_out[i]-1);
	}
      
      vtkTransform* tr=vtkTransform::New();
      tr->Identity();
      tr->PostMultiply();
      tr->Translate(shift_x);
      tr->Concatenate(mat);
      tr->Translate(shift_y);
      tr->GetMatrix(tmat);
      tr->Delete();
    }
  else
    tmat->DeepCopy(mat);

  mat->Delete();
  

  return 1;
}  
//------------------------------------------------------------------------------
int vtkpxUtil::ReOrientDimensions(double sp_in[3],int dim_in[3],int inputorient,int outputorient,
				  double sp_out[3],int dim_out[3])
{

  inputorient=vtkbisImageHeader::MapOrientationToOldStyle(inputorient);
  outputorient=vtkbisImageHeader::MapOrientationToOldStyle(outputorient);

  if (inputorient==outputorient)
    {
      for (int ia=0;ia<=2;ia++)
	{
	  dim_out[ia]=dim_in[ia];
	  sp_out[ia]=sp_in[ia];
	}
    }

  if (inputorient==1 && outputorient==0)
    {
      // Coronal->Axial  
      dim_out[0]=dim_in[0]; dim_out[1]=dim_in[2]; dim_out[2]=dim_in[1];
      sp_out[0]=sp_in[0];   sp_out[1]=sp_in[2];   sp_out[2]=sp_in[1];
    }
   
  if (inputorient==0 && outputorient==1)
    {
      // Axial->Coronal  
      dim_out[0]=dim_in[0]; dim_out[1]=dim_in[2]; dim_out[2]=dim_in[1];
      sp_out[0]=sp_in[0];   sp_out[1]=sp_in[2];   sp_out[2]=sp_in[1];
    }
	
  if (inputorient==1 && outputorient==2)
    {
      // Coronal->Sagittal  
      dim_out[0]=dim_in[2]; dim_out[1]=dim_in[1]; dim_out[2]=dim_in[0];
      sp_out[0]=sp_in[2];   sp_out[1]=sp_in[1];   sp_out[2]=sp_in[0];
    }
  
  if (inputorient==2 && outputorient==1)
    {
      // Sagittal->Coronal  
      dim_out[0]=dim_in[2]; dim_out[1]=dim_in[1]; dim_out[2]=dim_in[0];
      sp_out[0]=sp_in[2];   sp_out[1]=sp_in[1];   sp_out[2]=sp_in[0];
    }

  
  if (inputorient==0 && outputorient==2)
    {
      // Axial->Sagittal  
      dim_out[0]=dim_in[1]; dim_out[1]=dim_in[2]; dim_out[2]=dim_in[0];
      sp_out[0]=sp_in[1];   sp_out[1]=sp_in[2];   sp_out[2]=sp_in[0];
    }

  if (inputorient==2 && outputorient==0)
    {
      // Sagittal->Axial  
      dim_out[0]=dim_in[2]; dim_out[1]=dim_in[0]; dim_out[2]=dim_in[1];
      sp_out[0]=sp_in[2];   sp_out[1]=sp_in[0];   sp_out[2]=sp_in[1];
    }

  return 1;
}


int vtkpxUtil::ReOrientImage(vtkImageData* destination,vtkImageData* source,int inputorient,int outputorient)
{
  if (source==NULL || destination==NULL)
      return 0;

  // This is unncecessary, i.e. taken care of in the individual functions
  inputorient=vtkbisImageHeader::MapOrientationToOldStyle(inputorient);
  outputorient=vtkbisImageHeader::MapOrientationToOldStyle(outputorient);

  if (inputorient==outputorient)
    {
      destination->ShallowCopy(source);
      return 1;
    }
  
 
  vtkMatrix4x4* tmat=vtkMatrix4x4::New();
  int ok2=vtkpxUtil::ReOrientMatrix(source,inputorient,outputorient,tmat);
  if (ok2==0)
    return ok2;

  vtkTransform* trans=vtkTransform::New();
  trans->SetMatrix(tmat);
		
  double sp[3],spout[3];
  int dim[3],dimout[3];

  source->GetSpacing(sp);
  source->GetDimensions(dim);
  vtkpxUtil::ReOrientDimensions(sp,dim,inputorient,outputorient,spout,dimout);

  vtkbisImageReslice* reslice=vtkbisImageReslice::New();

  reslice->SetInput(source);
  reslice->OptimizationOff();
  reslice->SetOutputOrigin(0.0,0.0,0.0);
  reslice->SetOutputExtent(0,dimout[0]-1,0,dimout[1]-1,0,dimout[2]-1);
  reslice->SetOutputSpacing(spout[0],spout[1],spout[2]);
  reslice->SetInterpolationMode(0);
  reslice->SetResliceTransform(trans);
  reslice->SetBackgroundLevel(0);
  reslice->Update();

  destination->ShallowCopy(reslice->GetOutput());
  reslice->Delete();
  trans->Delete();
  tmat->Delete();
  return 1;
}

// --------------------------------------------------------------------------------
// Reslice image source to have same dimensions as image ref
// Store output in destination using transform, interp and background
// --------------------------------------------------------------------------------
int vtkpxUtil::ResliceImage(vtkImageData* destination,
			    vtkImageData* source,
			    vtkImageData* ref,
			    vtkAbstractTransform* transform,
			    int interp,float background)
{
  if (destination==NULL || source==NULL || ref==NULL || transform==NULL)
    return 0;

  if (interp==1)
    {
      double range[2]; 
      source->GetPointData()->GetScalars()->GetRange(range);
      if (range[1]-range[0]<16.0)
	{
	  interp=3;
	  fprintf(stderr,"Switching to Cubic .... low range of input\n");
	}
    }

  vtkbisImageReslice* resl=vtkbisImageReslice::New();
  resl->SetInput(source);
  resl->SetInformationInput(ref);
  resl->SetResliceTransform(transform);
  resl->SetInterpolationMode(interp);
  resl->SetBackgroundLevel(background);
  resl->OptimizationOff();
  resl->Update();

  destination->ShallowCopy(resl->GetOutput());
  resl->Delete();
  return 1;
}

// --------------------------------------------------------------------------------
// Equi Reslice force two images to have same dimensions using linear interpolation
// --------------------------------------------------------------------------------


int vtkpxUtil::EquiReslice(vtkImageData* output,
			   vtkImageData* input,
			   vtkImageData* reference)
{
  if (input==NULL || output == NULL || reference==NULL)
    return 0;

  vtkIdentityTransform* tr=vtkIdentityTransform::New();

  int ok=vtkpxUtil::ResliceImage(output,input,reference,tr,1,0.0);
  tr->Delete();
  return ok;


}


// ------------------------------------------------------------------------------
int vtkpxUtil::ExtractFrames(vtkImageData* destination,vtkImageData* source,int begin,int end,int inc)
{
  // Frames And Slices are output correct i.e. the correct output number of frames and slices

  if (source==NULL || destination==NULL)
      return 0;

  int dim[3];
  source->GetDimensions(dim);
  int frames=source->GetNumberOfScalarComponents();

  

  begin=Irange(begin,0,frames-1);
  end=Irange(end,begin,frames-1);
  inc=Irange(inc,1,frames);

  if (begin==0 && end==(frames-1) && inc==1)
    {
      destination->ShallowCopy(source);
      return 1;
    }

  int totalframes=0;
  for (int test=begin;test<=end;test+=inc)
    ++totalframes;

  destination->SetScalarType(source->GetScalarType());
  destination->SetNumberOfScalarComponents(totalframes);
  destination->SetDimensions(dim);
  destination->SetWholeExtent(source->GetWholeExtent());
  destination->SetExtent(source->GetExtent());
  destination->SetSpacing(source->GetSpacing());
  destination->SetOrigin(source->GetOrigin());
  destination->AllocateScalars();


  vtkDataArray* destScalars=destination->GetPointData()->GetScalars();
  vtkDataArray* sourceScalars=source->GetPointData()->GetScalars();
  
  int inframe=begin;
  for (int fr=0;fr<totalframes;fr++)
    {
      destScalars->CopyComponent(fr,sourceScalars,inframe);
      inframe+=inc;
    }
  return 1;

}

// ------------------------------------------------------------------------------
int vtkpxUtil::ComputeImageNormalizationFactors(vtkImageData* ref,
						vtkImageData* scaled,
						vtkFloatArray* params,
						float threshold)
  
{
 
  int nump=ref->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nump2=scaled->GetPointData()->GetScalars()->GetNumberOfTuples();

  if (nump!=nump2)
    return 0;

  params->SetNumberOfComponents(1);
  params->SetNumberOfTuples(2);


  float sx=0.0,sy=0.0,sxy=0.0,sxx=0.0;
  int ndata=0;
  
  vtkDataArray* pix1=ref->GetPointData()->GetScalars();
  vtkDataArray* pix2=scaled->GetPointData()->GetScalars();


  for (int j=0;j<nump;j++) 
    {
      float x=pix1->GetComponent(j,0);
      float y=pix2->GetComponent(j,0);
      
      if (x>=threshold)
	{
	  sx += x;
	  sy += y;
	  sxy += x*y;
	  sxx += x*x;
	  ++ndata;
	}
    }

  if (ndata==0)
    return 0;

  //  fprintf(stderr,"sx=%f , sy=%f sxy=%f sxx=%f ndata=%d\n",sx/float(ndata),sy/float(ndata),sxy,sxx,ndata);
  
  float del=float(ndata)*sxx-sx*sx;
  float a=(sxx*sy-sx*sxy)/del;
  float b=(float(ndata)*sxy-sx*sy)/del;

  params->SetComponent(0,0,a);
  params->SetComponent(1,0,b);
  return 1;
}
						

// ---------------------------------------------------------------
float vtkpxUtil::Fmin(float a,float b)
{
  if (a<b)
      return a;
  else
      return b;
}

float vtkpxUtil::Fmax(float a,float b)
{
  if (a>b)
      return a;
  else
      return b;
}

int vtkpxUtil::Imin(int a,int b)
{
  if (a<b)
      return a;
  else
      return b;
}

int vtkpxUtil::Imax(int a,int b)
{
  if (a>b)
      return a;
  else
      return b;
}
// -------------------------------------------------------------
float vtkpxUtil::Frange(float a,float minv,float maxv)
{
  if (a<minv) a=minv;
  if (a>maxv) a=maxv;
  return a;
}
// -------------------------------------------------------------
int  vtkpxUtil::Irange(int   a,int   minv,int   maxv)
{
  if (a<minv) a=minv;
  if (a>maxv) a=maxv;
  return a;
}
// -------------------------------------------------------------

int vtkpxUtil::AutoCropImage(vtkImageData* img,int offset)
{

  if (img==NULL)
    {
      fprintf(stderr,"Bad Image in AutoCrop\n");
      return 0;
    }

  if (offset<2)
    offset=2;

  int dim[3]; img->GetDimensions(dim);
  int bounds[3][2];
  for (int ia=0;ia<=2;ia++)
    {
      bounds[ia][0]=dim[ia]-1;
      bounds[ia][1]=0;
    }

  fprintf(stderr,"Starting Bounds x: %d:%d y: %d:%d z: %d:%d\n",
	  bounds[0][0],bounds[0][1],bounds[1][0],bounds[1][1],bounds[2][0],bounds[2][1]);
  
  for (int k=0;k<dim[2];k+=2)
    for (int j=0;j<dim[1];j+=2)
      for (int i=0;i<dim[0];i+=2)
	{
	  double v=img->GetScalarComponentAsDouble(i,j,k,0);
	  if (v>0)
	    {
	      bounds[0][0]=Imin(bounds[0][0],i);
	      bounds[0][1]=Imax(bounds[0][1],i);
	      bounds[1][0]=Imin(bounds[1][0],j);
	      bounds[1][1]=Imax(bounds[1][1],j);
	      bounds[2][0]=Imin(bounds[2][0],k);
	      bounds[2][1]=Imax(bounds[2][1],k);
	    }
	}
  
  fprintf(stderr,"Bounds x: %d:%d y: %d:%d z: %d:%d\n",
      bounds[0][0],bounds[0][1],bounds[1][0],bounds[1][1],bounds[2][0],bounds[2][1]);

  for (int ia=0;ia<=2;ia++)
    {
      bounds[ia][0]=Imax(0,bounds[ia][0]-offset);
      bounds[ia][1]=Imin(dim[ia]-1,bounds[ia][1]+offset);
    }


  fprintf(stderr,"Offset (%d) Bounds x: %d:%d y: %d:%d z: %d:%d\n",offset,
	  bounds[0][0],bounds[0][1],bounds[1][0],bounds[1][1],bounds[2][0],bounds[2][1]);

  //  fprintf(stderr,"Old Dimensions =%d %d %d\n",img->GetDimensions()[0],img->GetDimensions()[1],img->GetDimensions()[2]);

  vtkpxImageExtractVOI* voi=  vtkpxImageExtractVOI::New();
  //  voi->DebugOn();
  voi->SetVOI(bounds[0][0],bounds[0][1],bounds[1][0],bounds[1][1],bounds[2][0],bounds[2][1]);
  //  voi->DebugOff();
  voi->SetInput(img);
  voi->Update();

  img->ShallowCopy(voi->GetOutput());
  //  fprintf(stderr,"New Dimensions =%d %d %d\n",img->GetDimensions()[0],img->GetDimensions()[1],img->GetDimensions()[2]);
  voi->Delete();
  return 1;
}


// ----------------------------------------------------------------------------------------------------
void vtkpxUtil::ImageRobustRange(vtkImageData* image,float perlow,float perhigh,vtkFloatArray* params,
				 float dim_target)
{
  if (params==NULL)
    return;

  float tlow,thigh;
  ImageRobustRange(image,perlow,perhigh,tlow,thigh,dim_target);
  params->SetNumberOfTuples(2);
  params->SetComponent(0,0,tlow);
  params->SetComponent(1,0,thigh);
}

void vtkpxUtil::ImageRobustRange(vtkImageData* image,float perlow,float perhigh,float& tlow,float& thigh,
				 float dim_target)
{
  dim_target=Frange(dim_target,16.0,256.0);

  if (image==NULL)
    {
      tlow=0.0;
      thigh=1.0;
      return;
    }

  if (perlow<0.0 || perlow>0.99)
    perlow=0.0;
  
  if (perhigh<perlow)
    perhigh=perlow+0.01;
  if (perhigh>1.0)
    perhigh=1.0;

  double in_range[2];
  image->GetPointData()->GetScalars()->GetRange(in_range);
  if ((in_range[1]-in_range[0])<20.0)
    {
      tlow=in_range[0];
      thigh=in_range[1];
      double diff=in_range[1]-in_range[0];

      if (diff>0.0)
	{
	  tlow=in_range[0]+perlow*diff;
	  thigh=in_range[0]+perhigh*diff;
	}
      return;
    }
  

  double sp[3]; image->GetSpacing(sp);
  int dim[3];image->GetDimensions(dim);

  for (int ia=0;ia<=2;ia++)
    {
      if (dim[ia]>32)
	sp[ia]*=double(dim[ia])/dim_target;
    }

  vtkImageResample* resl=vtkImageResample::New();
  //  resl->DebugOn();

  if (image->GetNumberOfScalarComponents()>1)
    {
      vtkImageExtractComponents* ext=vtkImageExtractComponents::New();
      ext->SetInput(image);
      ext->SetComponents(0);
      ext->Update();
      resl->SetInput(ext->GetOutput());
      ext->Delete();
    }
  else
    {
      resl->SetInput(image);
    }

  resl->InterpolateOff();
  resl->SetAxisOutputSpacing(0,sp[0]);
  resl->SetAxisOutputSpacing(1,sp[1]);
  resl->SetAxisOutputSpacing(2,sp[2]);
  resl->Update();
  
  vtkDataArray* tmp=resl->GetOutput()->GetPointData()->GetScalars();
  double range[2];
  tmp->GetRange(range);
  int bins=512;

  if (range[1]-range[0] < bins)
    bins=int(range[1]-range[0])+1;

  float spacing = (range[1]-range[0])/float(bins-1);


  vtkImageAccumulate* accumulate=vtkImageAccumulate::New();

  accumulate->SetInput(resl->GetOutput());
  accumulate->SetComponentExtent(0,bins-1,0,0,0,0);
  accumulate->SetComponentOrigin(range[0],0,0);
  accumulate->SetComponentSpacing(spacing,1,1);
  accumulate->Update();

  vtkDataArray* data=accumulate->GetOutput()->GetPointData()->GetScalars();
  vtkFloatArray* cum=vtkFloatArray::New();
  cum->SetNumberOfTuples(bins);

  cum->SetComponent(0,0,data->GetComponent(0,0));
  for (int ia=1;ia<bins;ia++)
    cum->SetComponent(ia,0,data->GetComponent(ia,0)+cum->GetComponent(ia-1,0));

  float total=cum->GetComponent(bins-1,0);

  int foundperlow=0,foundperhigh=0;

  for (int ib=1;ib<bins;ib++)
    {
      float v=cum->GetComponent(ib,0);
      if (foundperlow==0)
	{
	  if (v/total > perlow)
	    {
	      foundperlow=1;
	      tlow=range[0]+ib*spacing;
	    }
	}

      if (foundperhigh==0)
	{
	  if (v/total > perhigh)
	    {
	      foundperhigh=1;
	      thigh=range[0]+ib*spacing;
	      ib=bins;
	    }
	}
    }

  //    fprintf(stderr,"low=%.2f (%f) high=%.2f (%f)\n", perlow,tlow,perhigh,thigh);

  accumulate->Delete();
  resl->Delete();

  //   fprintf(stderr,"Done\n");
}

double vtkpxUtil::ComputeImageProductSum(vtkImageData* img1,vtkImageData* img2)
{
  if (img1==NULL || img2==NULL)
    return 0.0;

  vtkDataArray* dat1=img1->GetPointData()->GetScalars();
  vtkDataArray* dat2=img2->GetPointData()->GetScalars();
  if (dat1->GetNumberOfTuples()!=dat2->GetNumberOfTuples())
    return 0.0;

  int nt=dat1->GetNumberOfTuples();

  double sum=0.0;
  
  for (int ia=0;ia<nt;ia++)
    sum+=dat1->GetComponent(ia,0)*dat2->GetComponent(ia,0);

  return sum;

}

double vtkpxUtil::ComputeImageSum(vtkImageData* img1,double threshold)
{
  if (img1==NULL)
    return 0.0;

  double range[2]; img1->GetPointData()->GetScalars()->GetRange(range);
  if (threshold<0.0)
    threshold=0.0;
  else if (threshold>1.0)
    threshold=1.0;

  double thr=range[0]+threshold*range[1];
  vtkDataArray* dat1=img1->GetPointData()->GetScalars();
  int nt=dat1->GetNumberOfTuples();

  double sum=0.0;
  for (int ia=0;ia<nt;ia++)
    {
      double v=dat1->GetComponent(ia,0);
      if (v>thr)
	sum+=v;
    }
  return sum;
}

