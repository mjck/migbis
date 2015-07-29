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

// ------------------------------------------------------------------------
//
// This file derives from original work
// in the vtkCISG Toolkit
// that was released under the terms of 
// the GNU General Public License (either version 2,
// of the License, or any later version.)
// 
// The code has been modified for inclusion in BioImage Suite
//
// It may consist of work derived from more than one source
// file in the vtkCISG Toolkit. The original copyright notice
// was:
//
//  Authors   : Thomas Hartkens <thomas@hartkens.de>
//              Daniel Rueckert <d.rueckert@doc.ic.ac.uk>
//	        Julia Schnabel  <julia.schnabel@kcl.ac.uk>
//	        under the supervision of Dave Hawkes and Derek Hill
//  Web       : http://www.image-registration.com
//  Copyright : King's College London
//              Div. of Radiological Sciences, 
//              Computational Imaging Science Group, 1997 - 2000
//	        http://www-ipg.umds.ac.uk/cisg
//
// ------------------------------------------------------------------------


// vtk includes
#include "pxutil.h"
#include "vtkImageData.h"
#include "vtkImageCast.h"
#include "vtkImageExtractComponents.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkbisImageResample.h"
#include "vtkbisImageReslice.h"
#include "vtkObjectFactory.h"
#include "vtkpxRegistration.h"
#include "vtkMath.h"
#include "vtkpxMath.h"
#include "vtkLandmarkTransform.h"
#include "vtkpxUtil.h"
#include "vtkTransform.h"
#include "vtkImageShiftScale.h"
#include "vtkPoints.h"
#include "vtkpxComboTransform.h"
#include "vtkpxUtil.h"
#include "vtkpxImageNormalizeIntensityToReference.h"
#include "vtkpxLinearTransform.h"
#include "vtkpxLinearRegistration.h"
#include "vtkImageGaussianSource.h"
#include "vtkPolyData.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkTriangleFilter.h"
#include "vtkFloatArray.h"
//#include "vtkIdType.h"
#include "vtkCellDataToPointData.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkInformation.h"



vtkpxRegistration* vtkpxRegistration::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxRegistration");
  if(ret)
    {
      return (vtkpxRegistration*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxRegistration;
}


vtkpxRegistration::vtkpxRegistration()
{
}

vtkpxRegistration::~vtkpxRegistration()
{
}


//----------------------------------------------------------------------------
int vtkpxRegistration::ExtractComponentAndCastImageToShort(vtkImageData* destination,vtkImageData* source,
							   int frame,int doscale)
{
  if (destination==NULL || source==NULL)
    return 0;

  double range[2];

  vtkImageData* newsource=NULL;
  int donewsource=1;

  if (source->GetNumberOfScalarComponents()>1)
    {
      int fr=Irange(frame,0,source->GetNumberOfScalarComponents()-1);
      vtkImageExtractComponents* extr=vtkImageExtractComponents::New();
      
      extr->SetInput(source);
      extr->SetComponents(fr);
      fprintf(stderr," ************ Extracting frame %d\n",fr);
      extr->Update();
      newsource=vtkImageData::New();
      newsource->ShallowCopy(extr->GetOutput());
      donewsource=1;
      extr->Delete();
    }
  else
    {
      newsource=source;
      donewsource=0;
    }
  
  newsource->GetPointData()->GetScalars()->GetRange(range);
  //  fprintf(stderr,"extracting component %d scale=%d range=%.2f:%.2f\n",frame,doscale,range[0],range[1]);
  
  if (  (range[1]-range[0]<32.0) ||  source->GetScalarType() != VTK_SHORT || range[1]>32767.0 || range[2]<-32767.0)
    {
      double scale=1.0,shift=0.0;
      vtkImageCast* cast2=vtkImageCast::New();

      if ((range[1]-range[0]<32.0) && doscale==1)
	{
	  scale=100.0;
	}
      else if ((range[1]>32760.0 || range[0]<-32767.0))
	{
	  shift=-range[0];
	  scale=30000.0/(range[1]-range[0]);
	}
	
      //      fprintf(stderr,"Here shift=%.5f, scale=%.5f\n",shift,scale);

      if (scale!=1.0 || shift!=0.0)
	{
	  fprintf(stderr,"\t Scaling Intensity=%.4f,%.4f\n",scale,shift);
	  vtkImageShiftScale* cast=vtkImageShiftScale::New();
	  cast->SetInput(newsource);
	  cast->SetShift(shift);
	  cast->SetScale(scale);
	  cast->SetOutputScalarTypeToFloat();
	  cast->Update();
	  cast2->SetInput(cast->GetOutput());
	  cast->Delete();
	}
      else
	{
	  //  fprintf(stderr,"Simply Casting\n");
	  cast2->SetInput(source);
	}
      cast2->SetOutputScalarTypeToShort();
      cast2->Update();
      destination->ShallowCopy(cast2->GetOutput());
      cast2->Delete();
    }
  else
    {
      destination->ShallowCopy(newsource);
    }
  
  if (donewsource!=0)
    newsource->Delete();

  destination->GetPointData()->GetScalars()->GetRange(range);
  

  if (destination->GetScalarType() !=VTK_SHORT) 
    {
      cerr << "Trouble in casting ::: vtkpxRegistration supports SHORT images only! \n";
    }
  return 1;
}

//----------------------------------------------------------------------------
//-------------------------------------------------------------------------
int vtkpxRegistration::CalculateNumberOfBinsAndAdjustImage(vtkImageData *image, int maxbin,
							   short& minv,short& maxv,int logmode,
							   int posmode,int autonorm)
{
  if (image->GetScalarType() != VTK_SHORT && image->GetScalarType() != VTK_UNSIGNED_CHAR) 
    {
      cerr << "vtkpxRegistration supports SHORT or UNSIGNED CHAR images only! \n";
      return 0;
    }

  

  vtkpxRegistration::CalculateRange(image,minv,maxv,autonorm);
  return vtkpxRegistration::AdjustImage(image,maxbin,minv,maxv,logmode,posmode,autonorm);

}
//------------------------------------------------------------------------- 
int vtkpxRegistration::CalculateNumberOfBinsAndAdjustImages(vtkImageData *image1,vtkImageData* image2, 
							    int maxbin,
							    short& minv,short& maxv,
							    int logmode,int posmode,int autonorm)
{
  int ok=0;

  if ( (image1->GetScalarType() != image2->GetScalarType() ) ||
       (image1->GetScalarType() != VTK_UNSIGNED_CHAR && image2->GetScalarType() != VTK_SHORT ) )
    {
      cerr << "vtkpxRegistration supports SHORT or UNSIGNED_CHAR images only! \n";
      return 0;
    }

  short min_val1,min_val2,max_val1,max_val2;
  vtkpxRegistration::CalculateRange(image1,min_val1,max_val1,autonorm);
  vtkpxRegistration::CalculateRange(image2,min_val2,max_val2,autonorm);

  minv=Imin(min_val1,min_val2);
  maxv=Imax(max_val1,max_val2);

  vtkpxRegistration::AdjustImage(image1,maxbin,minv,maxv,
				 logmode,posmode,autonorm);

  vtkpxRegistration::AdjustImage(image2,maxbin,minv,maxv,
				 logmode,posmode,autonorm);

  return 1;

}


int vtkpxRegistration::CalculateRange(vtkImageData* image, short& minv,short& maxv,int autonorm)
{
  if (autonorm)
    {
      float perlow=0.01,perhigh=0.99,tlow,thigh;
      vtkpxUtil::ImageRobustRange(image,perlow,perhigh,tlow,thigh,256);
      minv=int(tlow+0.5);
      maxv=int(thigh+0.5);
      return 1;
    }

  double r[2];
  image->GetPointData()->GetScalars()->GetRange(r,0);

  if (image->GetNumberOfScalarComponents()>0)
    {
      double t[2];
      int nf=image->GetNumberOfScalarComponents();
      for (int i=1;i<nf;i++)
	{
	  image->GetPointData()->GetScalars()->GetRange(t,i);
	  if (t[0]<r[0])
	    r[0]=t[0];
	  if (t[1]>r[1])
	    r[1]=t[1];
	}
    }
     
  minv=int(r[0]);
  maxv=int(r[1]);
      
  return 1;
}


int vtkpxRegistration::AdjustImage(vtkImageData* image,int maxbin,
				   short minv,short maxv,
				   int logmode,int posmode,int autonorm)
{
  //  vtkpxRegistration::PrintImageInfo(image,"In Adjust Image");
  // --------------------------------------------------------------------------
  // Not Logarithm Mode
  // --------------------------------------------------------------------------
  
  if (!logmode)
    {
      if (posmode && maxv>2 && minv<=0)
	{
	  minv=1;
	}

      int range = int(maxv - minv + 1);
      int nbins = range;
      int width = 1;
      
      // Calculate number of bins to use
      if (maxbin > 0)
	{
	  while ((int)(ceil(range / (double)width)) > maxbin)
	    width++;
	  nbins = (int)(ceil(range/(double)width));    
	}
      

      float scale=float(range)/float(nbins);
      int  ns=image->GetPointData()->GetScalars()->GetNumberOfTuples()*image->GetPointData()->GetScalars()->GetNumberOfComponents();

      if (image->GetScalarType()==VTK_SHORT)
	{
	  short *img = (short*) image->GetPointData()->GetScalars()->GetVoidPointer(0);
	  
	  for (int i=0;i<ns;i++)
	    {
	      float a=float(*img);
	      int b=int((a-minv)/scale+0.5);
	      if (b<0)
		b=0;
	      else if (b>=nbins)
		b=nbins-1;
	      *img=b;
	      ++img;
	    }
	}
      else
	{
	  unsigned char *img = (unsigned char*) image->GetPointData()->GetScalars()->GetVoidPointer(0);
	  
	  for (int i=0;i<ns;i++)
	    {
	      float a=float(*img);
	      int b=int((a-minv)/scale+0.5);
	      if (b<0)
		b=0;
	      else if (b>=nbins)
		b=nbins-1;
	      *img=b;
	      ++img;
	    }

	}

      //      vtkpxRegistration::PrintImageInfo(image,"Done Adjust Image");
      return nbins;
    }

  // --------------------------------------------------------------------------
  // Logarithm Mode
  // --------------------------------------------------------------------------

  float logrange=log(float(maxv-minv+1));
  float scale=1.0;
  if (logrange>0.01)
    scale = float(maxbin-1)/logrange;
      
  minv=0;
  maxv=(maxbin-1);

  int  ns=image->GetPointData()->GetScalars()->GetNumberOfTuples()*image->GetPointData()->GetScalars()->GetNumberOfComponents();

  if (image->GetScalarType()==VTK_SHORT)
    {
      short *img = (short*) image->GetPointData()->GetScalars()->GetVoidPointer(0);
      float truemin=2000,truemax=-2000;
      
      for (int i=0;i<ns;i++)
	{
	  float inval=float(*img);
	  float outval = log(inval-minv+1)*scale;
	  *img=short(outval);
	  truemin=Fmin(truemin,outval);
	  truemax=Fmax(truemax,outval);
	  ++img;
	}
    }
  else
    {
      unsigned char* img = (unsigned char*) image->GetPointData()->GetScalars()->GetVoidPointer(0);
      float truemin=2000,truemax=-2000;
      
      for (int i=0;i<ns;i++)
	{
	  float inval=float(*img);
	  float outval = log(inval-minv+1)*scale;
	  *img=short(outval);
	  truemin=Fmin(truemin,outval);
	  truemax=Fmax(truemax,outval);
	  ++img;
	}
    }


  return maxbin;

}


int vtkpxRegistration::ScaleWeightImage(vtkImageData* weightimage,vtkImageData* sampledimage,double maxvalue)
{
  if (weightimage==NULL || sampledimage==NULL)
    return 0;

  vtkImageExtractComponents* extr=vtkImageExtractComponents::New();
  extr->SetInput(weightimage);
  extr->SetComponents(0);
  extr->Update();
  
  double range[2]; extr->GetOutput()->GetPointData()->GetScalars()->GetRange(range);
  
  if (maxvalue<10.0)
    maxvalue=10.0;
  if (maxvalue>100.0)
    maxvalue=100.0;

  if (range[1]<1.0)
    range[1]=1.0;

  double scalefactor=maxvalue/range[1];

  vtkImageShiftScale* cast=vtkImageShiftScale::New();
  cast->SetInput(extr->GetOutput());
  extr->Delete();
  cast->SetShift(0.0);
  cast->SetScale(scalefactor);
  cast->SetOutputScalarType(sampledimage->GetScalarType());
  cast->Update();

  weightimage->ShallowCopy(cast->GetOutput());
  cast->Delete();
  return 1;


}
//-------------------------------------------------------------------------
int vtkpxRegistration::ResampleAndSmoothImage(vtkImageData* destination,
					      vtkImageData* source,
					      double baseresolution[3],
					      float sigma,
					      float res,int sameslices)
{

  if (source==NULL || destination==NULL)
      return 0;

  double sp[3];  source->GetSpacing(sp);
  double ori[3]; source->GetOrigin(ori);
  int dim[3];   source->GetDimensions(dim);



  double gaussian[3],resolution[3];
  for (int j=0;j<=2;j++)
    {
      gaussian[j]=sigma*baseresolution[j];
      resolution[j]=baseresolution[j]*fabs(res);
    }

  // Step 1 Smooth 
  double gp[3],sum=0.0;
  for (int ia=0;ia<=2;ia++)
    {
      gp[ia]=(gaussian[ia]/sp[ia]);
      if (gp[ia]>2.0) 
	gp[ia]=2.0;
      sum+=gp[ia];
    }

  if (sameslices)
    {
      gp[2]=0.0;
      resolution[2]=sp[2];
    }

  vtkbisImageResample* resamp=vtkbisImageResample::New();

  int largeimage=0;
 #ifdef _WIN32
  if (dim[0]>256 || dim[1]>256 || dim[2]>256)
    {
      largeimage=1;
      fprintf(stderr,"\t\t Large Image (windows) not smoothing\n");
    }
#endif
  
  if (sum>0.5 && largeimage==0)
    {
      vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
      smooth->SetInput(source);
      smooth->SetStandardDeviations(gp);
      smooth->Update();
      resamp->SetInput(smooth->GetOutput());
      smooth->Delete();
    }
  else
    {
      resamp->SetInput(source);
    }

  // Speed things up this is a hack anyway bspline will take over for <1
  if (res<1.0)
    resamp->InterpolateOff();
  else
    resamp->InterpolateOn();
  resamp->SetDimensionality(3);
  for (int ib=0;ib<=2;ib++)
    resamp->SetAxisOutputSpacing(ib,resolution[ib]);
  resamp->Update();

  destination->ShallowCopy(resamp->GetOutput());
  resamp->Delete();

  return 1;
}

//-------------------------------------------------------------------------
int vtkpxRegistration::ResliceImage(vtkImageData* destination,vtkImageData* source,vtkImageData* ref,
				    vtkAbstractTransform* transform,
				    int interp)
{
  if (destination==NULL || source==NULL || ref==NULL || transform==NULL)
    return 0;

  if (interp<0)
    interp=-interp;

  if (interp>=0)
    {
      if (interp<0 || interp>1)
	interp=3;
      vtkbisImageReslice* resl=vtkbisImageReslice::New(); resl->OptimizationOff();
      
      resl->SetInput(source);
      resl->SetInformationInput(ref);
      resl->SetResliceTransform(transform);
      resl->SetInterpolationMode(interp);
      resl->SetBackgroundLevel(-10.0);
      resl->Update();
      destination->ShallowCopy(resl->GetOutput());
      resl->Delete();
    }

  return 1;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
int vtkpxRegistration::MultiplyAndOverWriteImage(vtkImageData* inout,vtkImageData* scale)
{
  
  int np1= inout->GetPointData()->GetScalars()->GetNumberOfTuples();
  int np2= scale->GetPointData()->GetScalars()->GetNumberOfTuples();

  if (np1!=np2)
    return 0;
  
  if (inout->GetScalarType()==VTK_SHORT && scale->GetScalarType()==VTK_SHORT)
    {
      short *img1 = (short*) inout->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *img2 = (short*) scale->GetPointData()->GetScalars()->GetVoidPointer(0);
      for (int i=0;i<np1;i++)
	{
	  *img1 = ( *img1) * (*img2);
	  ++img1;
	  ++img2;
	}
    }
  else
    {
      vtkDataArray* in1=inout->GetPointData()->GetScalars();
      vtkDataArray* in2=scale->GetPointData()->GetScalars();
      for (int i=0;i<np1;i++)
	in1->SetComponent(i,0,in1->GetComponent(i,0)*in2->GetComponent(i,0));
    }
  return 1;
}
//-------------------------------------------------------------------------
// Jacobian Stuff for visualization
//-------------------------------------------------------------------------
int vtkpxRegistration::ComputeJacobianImage(vtkImageData* ref,vtkImageData* jacobian,
					    vtkAbstractTransform* tr)
{
  return vtkpxRegistration::ComputeJacobianImage(ref,jacobian,tr,1.0);
}

//-------------------------------------------------------------------------
int vtkpxRegistration::ComputeSimpleJacobianImage(vtkImageData* ref,vtkImageData* jacobian,vtkAbstractTransform* tr,
						  float scale,float offset)
{
  return vtkpxRegistration::ComputeSimpleJacobianImage(ref,jacobian,tr,scale,offset,0.0);
}

int vtkpxRegistration::ComputeSimpleJacobianImage(vtkImageData* ref,vtkImageData* jacobian,vtkAbstractTransform* tr,
						  float scale,float offset,float threshold)
{
 if (ref==NULL || jacobian==NULL || tr==NULL)
    return 0;

  tr->Update();

  int dim[3];  ref->GetDimensions(dim);
  int dim2[3]; jacobian->GetDimensions(dim2);

  double r[2]; ref->GetPointData()->GetScalars()->GetRange(r);

  if (threshold<0.0)
    threshold=0.0;
  else if (threshold>1.0)
    threshold=1.0;
  double cutoff=r[0]+threshold*(r[1]-r[0]);
  

  if (dim[0]!=dim2[0] || dim[1]!=dim2[1] || dim[2]!=dim2[2] || 
      jacobian->GetScalarType()!=VTK_FLOAT || 
      jacobian->GetNumberOfScalarComponents()!=1)
    {
      jacobian->CopyStructure(ref);
      jacobian->SetNumberOfScalarComponents(1);
      jacobian->SetScalarTypeToFloat();
      jacobian->AllocateScalars();
    }

  double sp[3];  ref->GetSpacing(sp);
  double ori[3]; ref->GetOrigin(ori);
  double der[3][3],x[3],x2[3];
  
  vtkDataArray* dat=jacobian->GetPointData()->GetScalars();
  int index=0;

  for (int k=0;k<dim[2];k++)
    {
      x[2]=k*sp[2]+ori[2];
      for (int j=0;j<dim[1];j++)
	{
	  x[1]=j*sp[1]+ori[1];
	  for (int i=0;i<dim[0];i++)
	    {
	      x[0]=i*sp[0]+ori[0];

	      double v=ref->GetScalarComponentAsDouble(i,j,k,0);
	      if (v>=cutoff)
		{
		  tr->InternalTransformDerivative(x,x2,der);
		  double det=vtkMath::Determinant3x3(der);
		  dat->SetComponent(index,0,(det-offset)*scale);
		}
	      else
		{
		  dat->SetComponent(index,0,0.0);
		}
	      ++index;
	    }
	}
    }
  fprintf(stderr,"\t\t Offset = %.2f scale=%.2f cutoff=%.4f\n",offset,scale,cutoff);
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxRegistration::ComputeJacobianImage(vtkImageData* ref,vtkImageData* jacobian,
					    vtkAbstractTransform* tr,float scale,float affine)
{
  return vtkpxRegistration::ComputeJacobianImage(ref,jacobian,tr,scale,affine,0.0);
}

int vtkpxRegistration::ComputeJacobianImage(vtkImageData* ref,vtkImageData* jacobian,
					    vtkAbstractTransform* tr,float scale,float affine,float threshold)
{
  if (ref==NULL || jacobian==NULL || tr==NULL)
    return 0;

  tr->Update();

  int dim[3];  ref->GetDimensions(dim);
  int dim2[3]; jacobian->GetDimensions(dim2);


  vtkpxGridTransform* grid=NULL;
  vtkTransform* linear=NULL;
  int numcomponents=2;
  int minpass=0;

  if ( tr->IsA("vtkpxComboTransform")==1)
    {
      linear=((vtkpxComboTransform*)tr)->GetLinearTransform();
      float sum=0.0;
      for (int i=0;i<=3;i++)
	for (int j=0;j<=3;j++)
	  {
	    float a=linear->GetMatrix()->GetElement(i,j);
	    float b=float(i==j);
	    sum+=fabs(a-b);
	  }

      grid=((vtkpxComboTransform*)tr)->GetGridTransform();
	  
      if (sum>0.0)
	{
	  numcomponents=18;
	  minpass=0;
	}
      else
	{
	  numcomponents=9;
	  minpass=1;
	}
    }

  if (dim[0]!=dim2[0] || dim[1]!=dim2[1] || dim[2]!=dim2[2] || 
      jacobian->GetScalarType()!=VTK_FLOAT || 
      jacobian->GetNumberOfScalarComponents()!=numcomponents)
    {
      jacobian->CopyStructure(ref);
      jacobian->SetNumberOfScalarComponents(numcomponents);
      jacobian->SetScalarTypeToFloat();
      jacobian->AllocateScalars();
    }

  double sp[3];  ref->GetSpacing(sp);
  double ori[3]; ref->GetOrigin(ori);
  double der[3][3];
  double x[3],x2[3],x3[3];
  
  vtkDataArray* dat=jacobian->GetPointData()->GetScalars();
  int index=0;

  for (int k=0;k<dim[2];k++)
    {
      x[2]=k*sp[2]+ori[2];
      for (int j=0;j<dim[1];j++)
	{
	  x[1]=j*sp[1]+ori[1];
	  for (int i=0;i<dim[0];i++)
	    {
	      x[0]=i*sp[0]+ori[0];

	      if (grid==NULL)
		{
		  tr->InternalTransformDerivative(x,x2,der);
		  double det=vtkMath::Determinant3x3(der);
		  double val=det/fabs(affine);
		  dat->SetComponent(index,0,(det-1.0)*scale);
		  dat->SetComponent(index,1,(val-1.0)*scale);
		}
	      else
		{
		  tr->InternalTransformDerivative(x,x2,der);
		  int ind=0;
		  for (int pass=minpass;pass<=1;pass++)
		    {
		      for (int ia=0;ia<=2;ia++)
			for (int ib=0;ib<=2;ib++)
			  {
			    dat->SetComponent(index,ind,der[ia][ib]);
			    ++ind;
			  }
		      
		      if (pass<1)
			{
			  linear->TransformPoint(x,x2);
			  grid->InternalTransformDerivative(x2,x3,der);
			}
		    }
		}
	      ++index;
	    }
	}
    }
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxRegistration::ComputeCompleteJacobianImage(vtkImageData* ref,vtkImageData* jacobian,vtkpxComboTransform* tr,
						    int nonlinearonly)
{
  if (ref==NULL || jacobian==NULL || tr==NULL)
    return 0;

  int dim[3];  ref->GetDimensions(dim);
  int dim2[3]; jacobian->GetDimensions(dim2);

  tr->Update();

  vtkpxGridTransform* grid=tr->GetGridTransform();
  vtkTransform* linear=tr->GetLinearTransform();
  int nlfirst=tr->GetNonLinearFirst();
  int numcomponents=6;
  
  if (dim[0]!=dim2[0] || dim[1]!=dim2[1] || dim[2]!=dim2[2] || 
      jacobian->GetScalarType()!=VTK_FLOAT || 
      jacobian->GetNumberOfScalarComponents()!=numcomponents)
    {
      jacobian->CopyStructure(ref);
      jacobian->SetNumberOfScalarComponents(numcomponents);
      jacobian->SetScalarTypeToFloat();
      jacobian->AllocateScalars();
    }

  double sp[3];  ref->GetSpacing(sp);
  double ori[3]; ref->GetOrigin(ori);
  double der[3][3];
  double x[3],x2[3],x3[3];
  
  vtkDataArray* dat=jacobian->GetPointData()->GetScalars();
  int index=0;

  for (int k=0;k<dim[2];k++)
    {
      x[2]=k*sp[2]+ori[2];
      for (int j=0;j<dim[1];j++)
	{
	  x[1]=j*sp[1]+ori[1];
	  for (int i=0;i<dim[0];i++)
	    {
	      x[0]=i*sp[0]+ori[0];

	      if (nonlinearonly==0)
		{
		  tr->InternalTransformDerivative(x,x2,der);
		}
	      else
		{
		  if (nlfirst==1)
		    {
		      grid->InternalTransformDerivative(x,x2,der);
		    }
		  else
		    {
		      linear->TransformPoint(x,x2);
		      grid->InternalTransformDerivative(x2,x3,der);
		    }
		}
	      dat->SetComponent(index,0,der[0][0]);
	      dat->SetComponent(index,1,der[0][1]);
	      dat->SetComponent(index,2,der[0][2]);
	      dat->SetComponent(index,3,der[1][1]);
	      dat->SetComponent(index,4,der[1][2]);
	      dat->SetComponent(index,5,der[2][2]);
	      ++index;
	    }
	}
      
    }
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxRegistration::ComputePrincipalStrains(vtkImageData* ref,vtkImageData* jacobian,
					       vtkAbstractTransform* tr,float threshold)
{
  if (ref==NULL || jacobian==NULL || tr==NULL)
    return 0;

  tr->Update();

  double range[2]; ref->GetPointData()->GetScalars()->GetRange(range);

  if (threshold<0.0)
    threshold=range[0]-10.0;
  else
    threshold=Frange(threshold,0.0,0.5)*(range[1]-range[0])+range[0];


  int dim[3];  ref->GetDimensions(dim);
  int dim2[3]; jacobian->GetDimensions(dim2);


  if (dim[0]!=dim2[0] || dim[1]!=dim2[1] || dim[2]!=dim2[2] || 
      jacobian->GetScalarType()!=VTK_FLOAT || 
      jacobian->GetNumberOfScalarComponents()!=3)
    {
      jacobian->CopyStructure(ref);
      jacobian->SetNumberOfScalarComponents(3);
      jacobian->SetScalarTypeToFloat();
      jacobian->AllocateScalars();
    }

  double sp[3];  ref->GetSpacing(sp);
  double ori[3]; ref->GetOrigin(ori);
  double der[3][3], x[3],x2[3];
  
  vtkDataArray* dat=jacobian->GetPointData()->GetScalars();
  int index=0;

  dat->FillComponent(0,0.0);
  dat->FillComponent(1,0.0);
  dat->FillComponent(2,0.0);

  fprintf(stderr,"Computing Strains: ");
  for (int k=0;k<dim[2];k++)
    {
      x[2]=k*sp[2]+ori[2];
      for (int j=0;j<dim[1];j++)
	{
	  x[1]=j*sp[1]+ori[1];
	  for (int i=0;i<dim[0];i++)
	    {
	      x[0]=i*sp[0]+ori[0];
	      double v=ref->GetScalarComponentAsDouble(i,j,k,0);
	      if (v>=threshold)
		{
		  tr->InternalTransformDerivative(x,x2,der);
		  for (int ia=0;ia<=2;ia++)
		    {
		      der[ia][ia]-=1.0;
		      for (int ib=ia+1;ib<=2;ib++)
			{
			  der[ia][ib]=0.5*(der[ia][ib]+der[ib][ia]);
			  der[ib][ia]=der[ia][ib];
			}
		    }
		  
		  //float det=vtkMath::Determinant3x3(der);
		  //  if (det>1e-4)
		  //		    {
		      double w[3],V[3][3];
		      vtkMath::Diagonalize3x3(der,w,V);
		      float w2[3]; for (int ia=0;ia<=2;ia++) w2[ia]=w[ia];
		      vtkpxMath::Heapsort(3,&w2[0]);
		      
		      dat->SetComponent(index,0,(w2[2])*100.0);
		      dat->SetComponent(index,1,(w2[1])*100.0);
		      dat->SetComponent(index,2,(w2[0])*100.0);
		      /*}
		  else
		    {
		      dat->SetComponent(index,0,0.01);
		      dat->SetComponent(index,1,0.01);
		      dat->SetComponent(index,2,0.01);
		      }*/
		}
	      else
		{
		  dat->SetComponent(index,0,-0.01);
		  dat->SetComponent(index,1,-0.01);
		  dat->SetComponent(index,2,-0.01);
		}
	      ++index;
	    }
	}
      if ( 10*int(k/10)==k)
	fprintf(stderr,"%d/%d ",k+1,dim[2]);
    }
  fprintf(stderr,"\n");
  return 1;
}
//-------------------------------------------------------------------------
// Description:
// Compute Average Jacobian for image
//-------------------------------------------------------------------------
float vtkpxRegistration::ComputeAverageJacobian(vtkImageData* ref,vtkAbstractTransform* tr)
{
  return vtkpxRegistration::ComputeAverageJacobian(ref,tr,0.05);
}

float vtkpxRegistration::ComputeAverageJacobian(vtkImageData* ref,vtkAbstractTransform* tr,float srate)
{
  if (ref==NULL || tr==NULL)
    {
      fprintf(stderr,"Bad Inputs to Compute Average Jacobian\n");
      return 1.0;
    }

  if ( tr->IsA("vtkHomogeneousTransform")==1) 
    return ((vtkHomogeneousTransform*)tr)->GetMatrix()->Determinant();

  if ( tr->IsA("vtkpxComboTransform")==1)
    {
      vtkTransform* linear=((vtkpxComboTransform*)tr)->GetLinearTransform();
      float sum=0.0;
      for (int i=0;i<=3;i++)
	for (int j=0;j<=3;j++)
	  {
	    float a=linear->GetMatrix()->GetElement(i,j);
	    float b=float(i==j);
	    sum+=fabs(a-b);
	  }
      
      if (sum>0.0)
	{
	  return linear->GetMatrix()->Determinant();
	}
    }
  fprintf(stderr,"Computing determinant from landmark transform\n");
  srate=Frange(srate,0.01,0.5);
  float offset=srate*0.5;

  double ori[3],spa[3];
  int   dim[3];

  ref->GetDimensions(dim);
  ref->GetSpacing(spa);
  ref->GetOrigin(ori);

  // Use Middle 60% of images
  float wd[3];
  for (int ia=0;ia<=2;ia++)
    wd[ia]=float(dim[ia])*spa[ia];


  int np=int(1.0/srate);
  int nptot=np*np*np;

  vtkPoints* p1=vtkPoints::New();
  vtkPoints* p2=vtkPoints::New();
  p1->SetNumberOfPoints(nptot);
  p2->SetNumberOfPoints(nptot);
  
    
  float x[3],tx[3];
  int index=0;
  for (int k=0;k<np;k++)
    {
      x[2]=(offset+float(k)*srate)*wd[2]+ori[2];
      for (int j=0;j<np;j++)
	{
	  x[1]=(offset+float(j)*srate)*wd[1]+ori[1];
	  for (int i=0;i<np;i++)
	    {
	      x[0]=(offset+float(i)*srate)*wd[0]+ori[0];
	      tr->TransformPoint(x,tx);
	      p1->SetPoint(index,x);
	      p2->SetPoint(index,tx);
	      ++index;
	    }
	}
    }

  vtkLandmarkTransform* land=vtkLandmarkTransform::New();
  land->SetSourceLandmarks(p1);
  land->SetTargetLandmarks(p2);
  land->SetModeToAffine();
  land->Modified();
  land->Update();
  float det=land->GetMatrix()->Determinant();

  p1->Delete();
  p2->Delete();
  land->Delete();


  return det;
}

//-------------------------------------------------------------------------
int vtkpxRegistration::ReOrientImage(vtkImageData* destination,vtkImageData* source,int inputorient,int outputorient)
{
  if (source==NULL || destination==NULL)
      return 0;


  return vtkpxUtil::ReOrientImage(destination,source,inputorient,outputorient);

  /*  inputorient=Irange(inputorient,0,2);
  outputorient=Irange(outputorient,0,2);

  if (inputorient==outputorient)
    {
      destination->DeepCopy(source);
      return 0;
    }
  
  vtkMatrix4x4* tmat=vtkMatrix4x4::New();
  int ok2=vtkpxUtil::ReOrientMatrix(source,inputorient,outputorient,tmat);
  if (ok2==0)
    return ok2;

  vtkTransform* trans=vtkTransform::New();
  trans->SetMatrix(tmat);
		
  float sp[3],spout[3];
  int dim[3],dimout[3];

  source->GetSpacing(sp);
  source->GetDimensions(dim);
  vtkpxUtil::ReOrientDimensions(sp,dim,inputorient,outputorient,spout,dimout);

  vtkbisImageReslice* reslice=vtkbisImageReslice::New();    resl->OptimizationOff();
  reslice->SetInput(source);
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
  return 1;*/
}


//-------------------------------------------------------------------------
void vtkpxRegistration::CenterImageOrigin(vtkImageData* img)
{
  double sp[3];   img->GetSpacing(sp);
  int dm[3]; img->GetDimensions(dm);
  double ori[3];

  for (int ia=0;ia<=2;ia++)
    ori[ia]=-0.5*(sp[ia]*float(dm[ia]));

  img->SetOrigin(ori);
}
//-------------------------------------------------------------------------
float vtkpxRegistration::ComputeSimpleSimilarityBetweenImages(vtkImageData* img1,vtkImageData* img2,
							      vtkAbstractTransform* tr)
{
  return vtkpxRegistration::ComputeSimilarityBetweenImages(img1,img2,tr,img1->GetSpacing(),0.0,
							   5,1,64);
}



float vtkpxRegistration::ComputeSimilarityBetweenImages(vtkImageData* img1,vtkImageData* img2,
							vtkAbstractTransform* tr,
							double resolution[3],float sigma,
							int metric,int interpolation,int numbins)
{
  vtkImageData* ref=vtkImageData::New();
  vtkImageData* trn=vtkImageData::New();
  
  vtkpxRegistration::ResampleAndSmoothImage(ref,img1,
					    resolution,sigma,1.0);
  
  vtkpxRegistration::ResampleAndSmoothImage(trn,img2,
					    resolution,sigma,1.0);
  
  short minr=0,maxr=1,mint=0,maxt=1;
  int num_ref=CalculateNumberOfBinsAndAdjustImage(ref,numbins,minr,maxr,0);
  int num_tar=CalculateNumberOfBinsAndAdjustImage(trn,numbins,mint,maxt,0);


  vtkpxJointHistogram* Histogram=vtkpxJointHistogram::New();
  Histogram->Allocate(num_ref,num_tar);

  vtkbisImageReslice* resl=vtkbisImageReslice::New();   resl->OptimizationOff();
  
  resl->SetInput(trn);
  resl->SetInformationInput(ref);
  resl->SetResliceTransform(tr);
  resl->SetInterpolationMode(interpolation);
  resl->Update();

  Histogram->FillHistogram(ref,resl->GetOutput(),1);
  float val=0.0;

  switch (metric) 
    {
    case 1:
      val= -Histogram->JointEntropy();
      break;
    case 2:
    case 3:
      val= +Histogram->CrossCorrelation();
      break;
    case 4:
      val= +Histogram->MutualInformation();
      break;
    case 5:
      val= +Histogram->NormalizedMutualInformation();
      break;
    case 6:
      val= -Histogram->SumsOfSquaredDifferences() / 
	(double)Histogram->GetNumberOfSamples();
      break;
    case 7:
      val= +Histogram->CorrelationRatioXY();
      break;
    case 8:
      val= +Histogram->CorrelationRatioYX();
      break;
    case 9:
      val= Histogram->ComputeProduct();
      break;
    case 10:
      val= Histogram->ComputeEntropyOfDifference();
      break;

    default:
      val= +Histogram->NormalizedMutualInformation();
      break;
    }


  resl->Delete();
  Histogram->Delete();
  ref->Delete();
  trn->Delete();
  return val;
}


void vtkpxRegistration::ComputeAllSimilarityMetricsBetweenImages(vtkImageData* img1,vtkImageData* img2,
								 vtkAbstractTransform* tr,vtkFloatArray* results)
{
  vtkpxRegistration::ComputeAllSimilarityMetricsBetweenImages(img1,img2,tr,results,64);
}

void vtkpxRegistration::ComputeAllSimilarityMetricsBetweenImages(vtkImageData* img1,vtkImageData* img2,
								 vtkAbstractTransform* tr,vtkFloatArray* results,int numbins)
{

  if (results==NULL)
    return;

  results->SetNumberOfComponents(1);
  results->SetNumberOfTuples(10);

  double resolution[3];
  img1->GetSpacing(resolution);
  
  float sigma=0.0;
  
  int interpolation=1;
  if (numbins<2)
    numbins=2;
  else if (numbins>1024)
    numbins=1024;
 

  vtkImageData* ref=vtkImageData::New();
  vtkImageData* trn=vtkImageData::New();
  
  vtkpxRegistration::ResampleAndSmoothImage(ref,img1,
					    resolution,sigma,1.0);
  
  vtkpxRegistration::ResampleAndSmoothImage(trn,img2,
					    resolution,sigma,1.0);
  
  short minr=0,maxr=1,mint=0,maxt=1;
  int num_ref=CalculateNumberOfBinsAndAdjustImage(ref,numbins,minr,maxr,0);
  int num_tar=CalculateNumberOfBinsAndAdjustImage(trn,numbins,mint,maxt,0);


  vtkpxJointHistogram* Histogram=vtkpxJointHistogram::New();
  Histogram->Allocate(num_ref,num_tar);

  vtkbisImageReslice* resl=vtkbisImageReslice::New(); resl->OptimizationOff();
  
  resl->SetInput(trn);
  resl->SetInformationInput(ref);
  resl->SetResliceTransform(tr);
  resl->SetInterpolationMode(interpolation);
  resl->Update();

  Histogram->FillHistogram(ref,resl->GetOutput(),1);


  for (int metric=1;metric<=10;metric++)
    {
      float val=0.0;
      switch (metric) 
	{
	case 1:
	  val= -Histogram->JointEntropy();
	  break;
	case 2:
	case 3:
	  val= +Histogram->CrossCorrelation();
	  break;
	case 4:
	  val= +Histogram->MutualInformation();
	  break;
	case 5:
	  val= +Histogram->NormalizedMutualInformation();
	  break;
	case 6:
	  val= -Histogram->SumsOfSquaredDifferences() / 
	    (double)Histogram->GetNumberOfSamples();
	  break;
	case 7:
	  val= +Histogram->CorrelationRatioXY();
	  break;
	case 8:
	  val= +Histogram->CorrelationRatioYX();
	  break;
	case 9:
	  val= Histogram->ComputeProduct();
	  break;
	case 10:
	  val= Histogram->ComputeEntropyOfDifference();
	  break;
	}
      results->SetComponent(metric-1,0,val);
    }

  resl->Delete();
  Histogram->Delete();
  ref->Delete();
  trn->Delete();
  return;
}


//-------------------------------------------------------------------------
float vtkpxRegistration::HistogramEvaluate(vtkImageData* ref,vtkImageData* targ,vtkAbstractTransform* trans,
					   vtkpxJointHistogram* histo,int similaritymeasure,
					   int interp,int reset)
{
  if (ref==NULL || targ==NULL || trans==NULL || histo==NULL)
    return 0.0;

  if (interp<0)
    interp=-interp;

  if (interp<0 || interp>1)
    interp=3;
  
  vtkbisImageReslice* resl=vtkbisImageReslice::New(); resl->OptimizationOff();
  
  resl->SetInput(targ);
  resl->SetInformationInput(ref);
  resl->SetResliceTransform(trans);
  resl->SetInterpolationMode(interp);
  resl->Update();
  
  histo->FillHistogram(ref,resl->GetOutput(),reset);
  float val=histo->Similarity(similaritymeasure);
  
  resl->Delete();
  return val;
}
//-------------------------------------------------------------------------
float vtkpxRegistration::WeightedHistogramEvaluate(vtkImageData* ref,vtkImageData* wgt,vtkImageData* targ,vtkImageData* wgt2,
						   vtkAbstractTransform* trans,
						   vtkpxJointHistogram* histo,int similaritymeasure,
						   int interp,int reset)
{
  if (ref==NULL || targ==NULL || trans==NULL || histo==NULL)
    return 0.0;
  
  if (interp<0 || interp>1)
    interp=3;
  
  vtkbisImageReslice* resl=vtkbisImageReslice::New(); resl->OptimizationOff();
  resl->SetInput(targ);
  resl->SetInformationInput(ref);
  resl->SetResliceTransform(trans);
  resl->SetInterpolationMode(interp);
  resl->Update();

  vtkImageData* reslweight=NULL;

  if (wgt2!=NULL)
    {
      vtkbisImageReslice* resl2=vtkbisImageReslice::New();
      resl2->OptimizationOff();
      resl2->SetInput(wgt2);
      resl2->SetInformationInput(ref);
      resl2->SetResliceTransform(trans);
      resl2->SetInterpolationMode(interp);
      resl2->Update();
      histo->DualWeightedFillHistogram(ref,resl->GetOutput(),wgt,resl2->GetOutput(),reset);
      resl2->Delete();
    }
  else
    {
      histo->WeightedFillHistogram(ref,resl->GetOutput(),wgt,reset);
    }
  float val=histo->Similarity(similaritymeasure);
  
  resl->Delete();
  return val;
}
//-------------------------------------------------------------------------

int vtkpxRegistration::SeamImages(vtkImageData* ref,vtkImageData* targ,vtkLinearTransform* tr,vtkImageData* output,int intensitymap)
{
  {
    double r1[2],r2[2];
    ref->GetPointData()->GetScalars()->GetRange(r1);
    targ->GetPointData()->GetScalars()->GetRange(r2);
    fprintf(stderr,"Input Ranges=%.1f:%.1f and %.1f:%.1f\n",r1[0],r1[1],r2[0],r2[1]);
  }

  double bounds[6],bounds2[6];
  ref->GetBounds(bounds);
  targ->GetBounds(bounds2);

  double p1[3],p2[3],pc1[3],pc2[3];
  int ia=0;

  for (ia=0;ia<=2;ia++)
    {
      p1[ia]=bounds2[ia*2];
      p2[ia]=bounds2[ia*2+1];
      pc1[ia]=0.5*(bounds[ia*2]+bounds[ia*2+1]);
      pc2[ia]=0.5*(p2[ia]+p1[ia]);
      
    }

  tr->GetLinearInverse()->TransformPoint(p1,p1);
  tr->GetLinearInverse()->TransformPoint(p2,p2);
  tr->GetLinearInverse()->TransformPoint(pc2,pc2);
  
  fprintf(stderr,"Reference Bounds=%.4f:%.4f %.4f:%.4f %.4f:%.4f\n",
	  bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

  for (ia=0;ia<=2;ia++)
    {
      if (bounds[ia*2]>p1[ia])
	bounds[ia*2]=p1[ia];
      if (bounds[ia*2]>p2[ia])
	bounds[ia*2]=p2[ia];

      if (bounds[ia*2+1]<p1[ia])
	bounds[ia*2+1]=p1[ia];
      if (bounds[ia*2+1]<p2[ia])
	bounds[ia*2+1]=p2[ia];
    }
  
  fprintf(stderr,"Outlining Bounds=%.4f:%.4f %.4f:%.4f %.4f:%.4f\n",
	  bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

  
  double ori[3],spa[3]; ref->GetOrigin(ori); ref->GetSpacing(spa);
  double outorigin[3];
  int   outdim[3];
  
  for (ia=0;ia<=2;ia++)
    {
      if (bounds[2*ia]<ori[ia])
	{
	  int v=int(fabs(bounds[2*ia]-ori[ia])/spa[ia]+0.99);
	  outorigin[ia]=ori[ia]-v*spa[ia];
	}
      else
	{
	  outorigin[ia]=ori[ia];
	}

      outdim[ia]=int(fabs(bounds[2*ia+1]-outorigin[ia])/spa[ia]+0.99);
    }

  fprintf(stderr,"Outputs ... Origin=(%.4f %.4f %.4f) Spacing=(%.4f,%.4f,%.4f) Dimensions(%d,%d,%d)\n",
	  outorigin[0],	  outorigin[1],	  outorigin[2],
	  spa[0],	  spa[1],	  spa[2],
	  outdim[0],	  outdim[1],	  outdim[2]);

  
  vtkbisImageReslice* resl[2];

  for (int img=0;img<=1;img++)
    {
      resl[img]=vtkbisImageReslice::New(); resl[img]->OptimizationOff();
      resl[img]->SetOutputSpacing(spa);
      resl[img]->SetOutputOrigin(outorigin);
      resl[img]->SetOutputExtent(0,outdim[0]-1,0,outdim[1]-1,0,outdim[2]-1);
      if (img==0)
	{
	  resl[img]->SetInput(ref);
	  resl[img]->SetInterpolationMode(0);
	  resl[img]->SetBackgroundLevel(-1);
	}
      else
	{
	  resl[img]->SetInput(targ);
	  resl[img]->SetResliceTransform(tr);
	  resl[img]->SetInterpolationMode(1);
	  resl[img]->SetBackgroundLevel(-1);
	}
      resl[img]->Update();
    }

  vtkpxImageNormalizeIntensityToReference* norm=vtkpxImageNormalizeIntensityToReference::New();
  norm->SetInput(resl[1]->GetOutput());
  norm->SetReference(resl[0]->GetOutput());
  norm->SetThreshold(0.03);
  norm->SetOrder(1);
  norm->Update();

  output->ShallowCopy(resl[0]->GetOutput());

  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* dat=norm->GetOutput()->GetPointData()->GetScalars();
  vtkDataArray* dat2=resl[1]->GetOutput()->GetPointData()->GetScalars();

  double r1[2],r2[2];
  out->GetRange(r1);
  dat->GetRange(r2);
  fprintf(stderr,"Range=%.1f:%.1f and %.1f:%.1f\n",r1[0],r1[1],r2[0],r2[1]);

  int index=0;
  double tp[3];
  for (int k=0;k<outdim[2];k++)
    {
      tp[2]=outorigin[2]+float(k)*spa[2];
      for (int j=0;j<outdim[1];j++)
	{
	  tp[1]=outorigin[1]+float(j)*spa[1];
	  for (int i=0;i<outdim[0];i++)
	    {
	      tp[0]=outorigin[0]+float(i)*spa[0];
	      float y2=dat2->GetComponent(index,0);
	      if (y2!=-1)
		{
		  float d1=vtkMath::Distance2BetweenPoints(tp,pc1);
		  float d2=vtkMath::Distance2BetweenPoints(tp,pc2);
		  
		  if (d2<d1)
		    {
		      for (int c=0;c<output->GetNumberOfScalarComponents();c++)
			out->SetComponent(index,c,dat->GetComponent(index,c));
		    }
		}
	      ++index;
	    }
	}
    }



  resl[0]->Delete();
  resl[1]->Delete();
  norm->Delete();

  return 1;
}


int vtkpxRegistration::AutoReorientImages(vtkImageData* orig_ref,int ref_orient,
					  vtkImageData* targ,int trg_orient,
					  vtkImageData* output)
{

  vtkImageData* ref=vtkImageData::New();
  
  if (ref_orient==trg_orient)
    ref->ShallowCopy(orig_ref);
  else
    vtkpxUtil::ReOrientImage(ref,orig_ref,ref_orient,trg_orient);

  vtkpxLinearRegistration* areg=vtkpxLinearRegistration::New();
  areg->SetTransformModeToAffine();
  areg->SetSimilarityMeasureToNormalizedMutualInformation();
  areg->SetReferenceImage(ref);
  areg->SetTransformImage(targ);
  areg->SetReferenceOrientation(0);
  areg->SetTransformOrientation(0);
  areg->SetResolution(2.0);
  areg->SetNumberOfLevels(3);
  areg->SetNumberOfSteps(4);
  areg->SetStepSize(2.0);
  areg->SetOptimizationMethodToHybrid();
  areg->SetNumberOfIterations(25);
  areg->SetPositiveIntensitiesOnly(0);
  areg->SetLogarithmMode(0);
  areg->SetJointlyNormalizeIntensities(0);
  areg->SetAutoNormalizeIntensities(1);
  areg->SetInterpolationMode(1);
  areg->Run();
    
  vtkAbstractTransform* tr=areg->GetTransformation();

  double bounds2[6];
  targ->GetBounds(bounds2);

  vtkpxLinearTransform* lin=vtkpxLinearTransform::New();
  lin->ExtractParameters(tr,9,ref,0,targ,0);
  lin->Put(6,100);
  lin->Put(7,100);
  lin->Put(8,100);

  vtkTransform* tr2=vtkTransform::New();
  lin->GenerateUpdateTransform(tr2,ref,0,targ,0);
  lin->Delete();
  
  float p1[3],p2[3];
  int ia=0;

  for (ia=0;ia<=2;ia++)
    {
      p1[ia]=bounds2[ia*2];
      p2[ia]=bounds2[ia*2+1];
    }

  tr2->GetLinearInverse()->TransformPoint(p1,p1);
  tr2->GetLinearInverse()->TransformPoint(p2,p2);
  

  double bounds[6],spa[3],outorigin[3];
  int   outdim[3];

  targ->GetSpacing(spa);  
  float outspacing=spa[0];
  if (outspacing>spa[1])
    outspacing=spa[1];
  
  if (outspacing>spa[2])
    outspacing=spa[2];
  
 
  for (ia=0;ia<=2;ia++)
    {
      bounds[ia*2]  =Fmin(p1[0],p2[0])-3.0*outspacing;
      bounds[ia*2+1]=Fmax(p1[0],p2[0])+3.0*outspacing;
      outorigin[ia]=bounds[ia*2];
      outdim[ia]=int( (bounds[ia*2+1]-bounds[ia*2])/outspacing + 0.5);
    }

  fprintf(stderr,"Outlining Bounds=%.4f:%.4f %.4f:%.4f %.4f:%.4f\n",
	  bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);
  
  
  
  fprintf(stderr,"Outputs ... Origin=(%.4f %.4f %.4f) Spacing=(%.4f,%.4f,%.4f) Dimensions(%d,%d,%d)\n",
	  outorigin[0],	  outorigin[1],	  outorigin[2],
	  outspacing,	  outspacing,	  outspacing,
	  outdim[0],	  outdim[1],	  outdim[2]);

  
  vtkbisImageReslice* resl=vtkbisImageReslice::New(); resl->OptimizationOff();
  resl->SetOutputSpacing(outspacing,outspacing,outspacing);
  resl->SetOutputOrigin(outorigin);
  resl->SetOutputExtent(0,outdim[0]-1,0,outdim[1]-1,0,outdim[2]-1);
  resl->SetInput(targ);
  resl->SetResliceTransform(tr2);
  resl->SetInterpolationMode(1);
  resl->SetBackgroundLevel(0);
  resl->Update();
  
  output->ShallowCopy(resl->GetOutput());
  
  resl->Delete();
  ref->Delete();
  areg->Delete();
  return 1;
}
//-------------------------------------------------------------------------
// Vasicek Estimator
//
//
double vtkpxRegistration::ComputeEntropy(unsigned long n,short* data,double drift,short ignore)
{
  if (n<2)
    return 0.0;

  if (n<21)
    {
      fprintf(stderr,"Inp = ");
      for (unsigned long j=0;j<n;j++)
	fprintf(stderr,"%d ",data[j]);
      fprintf(stderr,"\n");
    }

  if (fabs(drift)>0.000001)
    {
      for (unsigned long j=0;j<n;j++)
	data[j]=short(double(data[j])/(1.0+drift*j));
    }
  
  if (n<21)
    {
      fprintf(stderr,"Scale = ");
      for (unsigned long j=0;j<n;j++)
	fprintf(stderr,"%d ",data[j]);
    }

  vtkpxMath::Heapsort_Short(n,data);

  if (n<21)
    {
      fprintf(stderr,"\nSort = ");
      for (unsigned long j=0;j<n;j++)
	fprintf(stderr,"%d ",data[j]);
      fprintf(stderr,"\n");
    }
  
  //  fprintf(stderr,"data[0]=%d ignore=%d\n",data[0],ignore);
  if (data[0]==ignore)
    return -1.0;

  double h=0.0;
  unsigned long m=(int)floor(sqrt(double(n)));
  if (m<1)
    m=1;
  else if (m>=n)
    m=n-1;
  double factor=double(n)/double(m);
  for (int i=0;i<n-m;i++)
    h+=log(1.0+factor*(data[i+m]-data[i]));
  return h/factor;
}
//-------------------------------------------------------------------------
double vtkpxRegistration::ComputeWeightedTotalGroupEntropy(vtkImageData* img,vtkImageData* weights,double drift,short ignore)
{
  vtkDataArray* arr=img->GetPointData()->GetScalars();
  int nt=arr->GetNumberOfTuples();
  int nc=arr->GetNumberOfComponents();

  short* data=new short[nc];
  
  vtkDataArray* wgt=NULL;
  if (weights!=NULL)
    wgt=weights->GetPointData()->GetScalars();


  double entropy=0.0;
  double sum=0.0;
  //  int bad=0;
  for (int i=0;i<nt;i++)
    {
      for (int j=0;j<nc;j++)
	data[j]=(short)arr->GetComponent(i,j);

      double w=1.0;
      if (wgt!=NULL)
	w=wgt->GetComponent(i,0);

      double ent=vtkpxRegistration::ComputeEntropy(nc,data,drift);
      if (ent>0.0)
	{
	  entropy+=w*ent;
	  sum+=w;
	}
      /*  else
	  bad+=1;*/
    }
  //  fprintf(stderr,"Bad=%.2f ent=%.3f\n",double(bad)/double(nt)*100.0,entropy/sum);
  return entropy/sum;
}
//-------------------------------------------------------------------------
double vtkpxRegistration::ComputeTotalGroupEntropy(vtkImageData* img,double drift,short ignore)
{
  return vtkpxRegistration::ComputeWeightedTotalGroupEntropy(img,NULL,drift,ignore);
}
//-------------------------------------------------------------------------
vtkImageData* vtkpxRegistration::ComputeVoxelwiseEntropyImage(vtkImageData* img,double drift,short ignore)
{
  vtkImageData* output=vtkImageData::New();
  output->CopyStructure(img);
  output->SetNumberOfScalarComponents(3);
  output->SetScalarTypeToFloat();
  output->AllocateScalars();

  vtkDataArray* out=output->GetPointData()->GetScalars();
  out->FillComponent(0,-1.0);
  out->FillComponent(1,-1.0);
  out->FillComponent(2,-1.0);

  vtkDataArray* arr=img->GetPointData()->GetScalars();
  int nt=arr->GetNumberOfTuples();
  int nc=arr->GetNumberOfComponents();

  short* data=new short[nc];
  
  double entropy=0.0;
  double sum=0.0;
  int bad=0;
  for (int i=0;i<nt;i++)
    {
      for (int j=0;j<nc;j++)
	data[j]=(short)arr->GetComponent(i,j);

  
      double ent=vtkpxRegistration::ComputeEntropy(nc,data,drift,ignore);
      double med=double(data[nc/2]);
      out->SetComponent(i,1,med);
      if (ent>0.0)
	{
	  out->SetComponent(i,0,ent);
	  entropy+=ent;
	  sum+=1.0;
	}
      else
	bad+=1;
    }

  double r1[2]; out->GetRange(r1,0);
  double r2[2]; out->GetRange(r2,1);
  double scale1=200.0;
  double scale2=1000.0/r2[1];

  fprintf(stderr,"Range entr = %.2f :%.2f sc=%.2f\n",r1[0],r1[1],scale1);
  fprintf(stderr,"Range grsc = %.2f :%.2f sc=%.2f\n",r2[0],r2[1],scale2);
  for (int i=0;i<nt;i++)
    {
      double a=out->GetComponent(i,0);
      if (a>0.0)
	{
	  double b=out->GetComponent(i,1);
	  out->SetComponent(i,0,scale1*a);
	  out->SetComponent(i,1,b*scale2);
	  out->SetComponent(i,2,pow(r1[1]-a,2.0)*b);
	}
    }

  double r3[2]; out->GetRange(r3,2);
  double scale3=1000.0/r3[1];
  fprintf(stderr,"Range prod = %.2f :%.2f sc=%.2f\n",r3[0],r3[1],scale3);
  for (int i=0;i<nt;i++)
    {
      double a=out->GetComponent(i,2);
      if (a>0.0)
	out->SetComponent(i,2,scale3*a);
    }



  fprintf(stderr,"Bad=%.2f ent=%.3f\n",double(bad)/double(nt)*100.0,entropy/sum);
  return output;
}

// --------------------------------------------------------------------------------------------------------------------
//
//
//  Area Expansion Methods for Surface Deformation Morphometry
//
//
// --------------------------------------------------------------------------------------------------------------------


float vtkpxRegistration::ComputeAreaExpansionSurface(vtkPolyData* surface,vtkAbstractTransform* tr,float scale,float offset)
{
  if (surface==NULL || tr==NULL)
    return -1.0;

  vtkTriangleFilter* triFilter=vtkTriangleFilter::New();
  triFilter->SetInput(surface);
  triFilter->Update();

  vtkTransformPolyDataFilter* filt= vtkTransformPolyDataFilter::New();
  filt->SetInput(triFilter->GetOutput());
  filt->SetTransform(tr);
  filt->Update();

  vtkPolyData* tmpsurface=vtkPolyData::New();
  tmpsurface->DeepCopy(triFilter->GetOutput());
  triFilter->Delete();
  
  surface->ShallowCopy(tmpsurface);
  tmpsurface->Delete();

  vtkCellArray* triangles=surface->GetPolys();
  vtkPoints*    p1=surface->GetPoints();
  vtkPoints*    p2=filt->GetOutput()->GetPoints();

  int nt=surface->GetNumberOfPolys();
  int np=p1->GetNumberOfPoints();

  //  fprintf(stderr,"NumberOf Triangles=%d, points=%d\n",nt,np);


  vtkFloatArray* areas=vtkFloatArray::New();
  areas->SetNumberOfComponents(1);
  areas->SetNumberOfTuples(nt);
  areas->FillComponent(0,0.0);
  float sumarea[2] = { 0.000001,0.000001};
  triangles->InitTraversal();
  vtkIdType  iad[10];
  vtkIdType* id=&iad[0];

  for (int ia=0;ia<nt;ia++)
    {
      vtkIdType ncell;

      triangles->GetNextCell(ncell,id);
      if (ncell!=3)
	{
	  fprintf(stderr,"Error Triangle but npoints!=3 (%d)\n",ncell);
	}

      for (int i=0;i<=2;i++)
	{
	  if (id[i]>=np)
	    {
	      fprintf(stderr,"Bad Point %d (Triangle %d) max=%d\n",
		      id[i],ia,np);
	    }
	}
      
      double v0[3],v1[3],v2[3],q0[3],q1[3],q2[3];
      p1->GetPoint(id[0],v0);      p1->GetPoint(id[1],v1);      p1->GetPoint(id[2],v2);
      p2->GetPoint(id[0],q0);      p2->GetPoint(id[1],q1);      p2->GetPoint(id[2],q2);

      if (ia==nt/2)
	{
	  fprintf(stderr,"Triangle1= [ %.2f %.2f %.2f ; %.2f %.2f %.2f ; %.2f %.2f %.2f ]\n",
		  v0[0],v0[1],v0[2],
		  v1[0],v1[1],v1[2],		  
		  v2[0],v2[1],v2[2]);
	  fprintf(stderr,"Triangle2= [ %.2f %.2f %.2f ; %.2f %.2f %.2f ; %.2f %.2f %.2f ]\n",
		  q0[0],q0[1],q0[2],
		  q1[0],q1[1],q1[2],		  
		  q2[0],q2[1],q2[2]);
	}

      for (int ib=0;ib<=2;ib++)
	{
	  v1[ib]=v1[ib]-v0[ib];	  v2[ib]=v2[ib]-v0[ib];
	  q1[ib]=q1[ib]-q0[ib];	  q2[ib]=q2[ib]-q0[ib];
	}

      vtkMath::Cross(v1,v2,v0);       vtkMath::Cross(q1,q2,q0); 
      double v=0.5*vtkMath::Norm(v0);
      double q=0.5*vtkMath::Norm(q0);

      if (ia==nt/2)
	fprintf(stderr,"Areas = %.4f %.4f\n",v,q);

      sumarea[0]+=v;
      sumarea[1]+=q;
      double ratio=1.0;
      if (q>0.0)
	ratio=v/q;
      areas->SetComponent(ia,0,(ratio-offset)*scale);
    }
  
  surface->GetCellData()->SetScalars(areas);
  areas->Delete();
  filt->Delete();
  
  vtkCellDataToPointData* pt=vtkCellDataToPointData::New();
  pt->SetInput(surface);
  pt->Update();

  surface->ShallowCopy(pt->GetOutput());
  pt->Delete();

  //  delete [] id;
    
  return  sumarea[1]/sumarea[0];

}
// -----------------------------------------------------------------------------------------------------------
int vtkpxRegistration::SmoothAreaExpansionSurface(vtkPolyData* surface,int iterations,float lambda)
{
  if (surface==NULL)
    return 0;
  
  if (surface->GetPointData()->GetScalars()==NULL)
  return 0;

  PXIntegerList* neighbours=vtkpxSurfaceUtil::GenerateNeighbors(surface,0);

  vtkDataArray* data=surface->GetPointData()->GetScalars();
  int nt=data->GetNumberOfTuples();
  vtkFloatArray* temp=vtkFloatArray::New();
  temp->SetNumberOfComponents(1);
  temp->SetNumberOfTuples(nt);
  
  fprintf(stderr,"Smoothing point data , iterations (lambda=%5.3f,iter=%d)\n ",lambda,iterations);

  for (int iter=1;iter<=iterations;iter++)
    {
      temp->CopyComponent(0,data,0);
      for(int i=0;i<nt;i++)
	{
	  double oldx=temp->GetComponent(i,0),newx=0.0;
	
	  /*	  if (i==nt/2)
	    fprintf(stderr,"i=%d oldx=%.2f\n",i,oldx);*/
	  
	  neighbours[i].Rewind();
	  int n=(neighbours[i].getnumnodes());
	  double scalefactor=0.0001;
	  if (n>0)
	    scalefactor=1.0/double(n);
	  for(int j=0;j<n;j++)
	    {
	      int p=neighbours[i].getcurrent();
	      double v=temp->GetComponent(p,0);
	      newx+=v*scalefactor;
	      neighbours[i].Next();

	      /*	      if (i==nt/2)
		fprintf(stderr,"\tj=%d \t p=%d \t v=%.2f\t newx=%f\n",j,p,v,newx);*/
	    }
	
	  double final=oldx;
	  if (n>0)
	    final=(1.0-lambda)*oldx+lambda*newx;
	  /*	  if (i==nt/2)
	    fprintf(stderr,"\tnewx=%f\t oldx=%f \t final=%f\n\n\n",oldx,newx,final);*/
	  
	  data->SetComponent(i,0,final);
	}
	
    }
  temp->Delete();
  delete [] neighbours;

  return 1;
}

// -----------------------------------------------------------------------------------------------------------

vtkImageData* vtkpxRegistration::ComputeAreaExpansionImage(vtkPolyData* surface)
{
  if (surface==NULL)
    return NULL;

  vtkDataArray* dat=surface->GetPointData()->GetScalars();
  if (dat==NULL)
    return NULL;

  int nt=dat->GetNumberOfTuples();
  if (nt==0)
    return NULL;

  double v=sqrt(double(nt));
  int width=int(v+1);
  int height=int(v+1);

  fprintf(stderr,"Input cell data=%d output image = %d x %d = %d\n",
	  nt,width,height,width*height);

  vtkImageData* img=vtkImageData::New();
  img->SetDimensions(width,height,1);
  img->SetScalarTypeToFloat();
  img->AllocateScalars();

  vtkDataArray* out=img->GetPointData()->GetScalars();
  out->FillComponent(0,0.0);
  for (int ia=0;ia<nt;ia++)
    out->SetComponent(ia,0,dat->GetComponent(ia,0));

  //  fprintf(stderr,"Done Copying Data\n");
  return img;
		
}

int vtkpxRegistration::ConvertAreaExpansionImageToSurface(vtkImageData* img, vtkPolyData* surface)
{
  if (surface==NULL || img==NULL)
    return -1;

  vtkTriangleFilter* triFilter=vtkTriangleFilter::New();
  triFilter->SetInput(surface);
  triFilter->Update();

  vtkCellArray* triangles=triFilter->GetOutput()->GetPolys();
  int nt=triFilter->GetOutput()->GetNumberOfPoints();
  vtkDataArray* dat=img->GetPointData()->GetScalars();
  int np=dat->GetNumberOfTuples();

  if (np<nt)
    {
      fprintf(stderr,"Bad Image\n");
      triFilter->Delete();
      return -1;
    }

  vtkFloatArray* areas=vtkFloatArray::New();
  areas->SetNumberOfComponents(1);
  areas->SetNumberOfTuples(nt);
  areas->FillComponent(0,0.0);
  for (int ia=0;ia<nt;ia++)
    areas->SetComponent(ia,0,dat->GetComponent(ia,0));

  surface->ShallowCopy(triFilter->GetOutput());
  surface->GetPointData()->SetScalars(areas);
  
  areas->Delete();
  triFilter->Delete();
  return 1;
}

void vtkpxRegistration::PrintImageInfo(vtkImageData* tmp,const char* comment)
{
  double sp1[3],or1[3];
  int   dim1[3];

  if (tmp==NULL)
    {
      fprintf(stderr,"\n +++++++++++ Image is NULL %s\n",comment);
      return;
    }

  tmp->GetSpacing(sp1);
  tmp->GetDimensions(dim1);
  tmp->GetOrigin(or1);

  

  fprintf(stderr,"\n ************* Image (%s) dim=(%dx%dx%d) spa=(%.2f,%.2f,%.2f) ori=(%.2f,%.2f,%.2f) \t\t",comment,dim1[0],dim1[1],dim1[2],sp1[0],sp1[1],sp1[2],or1[0],or1[1],or1[2]);

  for (int i=0;i<=1;i++)
    {
      vtkInformation* info;
      if (i==0)
	info=tmp->GetInformation();
      else
	info=tmp->GetPipelineInformation();
      
      if (info!=NULL)
	{
	  if (info->Has(vtkDataObject::SPACING()))
	    {
	      double sp[3];  info->Get(vtkDataObject::SPACING(), sp);
	      fprintf(stderr," PIPELINE i=%d (%.2f,%.2f,%.2f)\n\n\n\n",i,sp[0],sp[1],sp[2]);
	    }
	}
    }


  fprintf(stderr,"\n");
}


void vtkpxRegistration::FixPipelineSpacingAndOrigin(vtkImageData* img)
{
  double Origin[3]; img->GetOrigin(Origin);
  double Spacing[3]; img->GetSpacing(Spacing);

  if (img->GetInformation()->Has(vtkDataObject::SPACING()))
    img->GetInformation()->Set(vtkDataObject::SPACING(),Spacing[0],Spacing[1],Spacing[2]);

  if (img->GetPipelineInformation()->Has(vtkDataObject::SPACING()))
    img->GetPipelineInformation()->Set(vtkDataObject::SPACING(),Spacing[0],Spacing[1],Spacing[2]);


  if (img->GetInformation()->Has(vtkDataObject::ORIGIN()))
    img->GetInformation()->Set(vtkDataObject::ORIGIN(),Origin[0],Origin[1],Origin[2]);

  if (img->GetPipelineInformation()->Has(vtkDataObject::ORIGIN()))
    img->GetPipelineInformation()->Set(vtkDataObject::ORIGIN(),Origin[0],Origin[1],Origin[2]);

}

