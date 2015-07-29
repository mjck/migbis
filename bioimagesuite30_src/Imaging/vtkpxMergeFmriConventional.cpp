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
  Module:    $RCSfile: vtkpxMergeFmriConventional.cpp,v $
  Language:  C++
  Date:      $Date: 2004/02/03 16:36:54 $
  Version:   $Revision: 1.2 $


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
#include "vtkpxMergeFmriConventional.h"
#include "vtkSource.h"
#include "vtkObjectFactory.h"
#include "vtkpxUtil.h"
#include "vtkMatrix4x4.h"
#include "vtkTransform.h"
#include "vtkAbstractTransform.h"
#include "vtkbisImageReslice.h"
#include "vtkImageData.h"
#include "vtkImageMedian3D.h"
#include "vtkImageFlip.h"
#include "vtkGeneralTransform.h"
#include "vtkIdentityTransform.h"
#include "vtkAbstractTransform.h"
#include "vtkLongArray.h"
#include "vtkDoubleArray.h"
//------------------------------------------------------------------------------
vtkpxMergeFmriConventional* vtkpxMergeFmriConventional::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxMergeFmriConventional");
  if(ret)
    {
    return (vtkpxMergeFmriConventional*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxMergeFmriConventional;
}

// Construct object with no children.
vtkpxMergeFmriConventional::vtkpxMergeFmriConventional()
{

  this->NormalizeAnatomical=1;
  this->MinimumThreshold=0;
  this->MaximumThreshold=1;
  this->ThresholdMode=1;
  this->IntensityThreshold=5;

  this->fmriInput=NULL;
  this->Transform1=NULL;
  this->Transform2=NULL;
  this->OutputMode=0;

  this->SingleFrameMode=1;
  this->CurrentFrame=0;

  this->MedianFiltering=0;
  this->FlipFmri=0;
  
  for (int i=0;i<3;i++)
    for (int j=0;j<2;j++)
      {
	this->SourceBounds[i*2+j]=j;
	this->TargetBounds[i*2+j]=j;
      }
  this->PositiveLaterality=0.0;
  this->NegativeLaterality=0.0;
}

vtkpxMergeFmriConventional::~vtkpxMergeFmriConventional()
{

  if (this->fmriInput!=NULL)
    {
      this->fmriInput->UnRegister(this);
      this->fmriInput=NULL;
    }

  if (this->Transform1!=NULL)
    {
      this->Transform1->UnRegister(this);
      this->Transform1=NULL;
    }

  if (this->Transform2!=NULL)
    {
      this->Transform2->UnRegister(this);
      this->Transform2=NULL;
    }

 
}

void vtkpxMergeFmriConventional::ThresholdPositiveOnly()
{
  this->SetThresholdMode(1);
}

void vtkpxMergeFmriConventional::ThresholdNegativeOnly()
{
  this->SetThresholdMode(2);
}

void vtkpxMergeFmriConventional::ThresholdBoth()
{
  this->SetThresholdMode(3);
}

void vtkpxMergeFmriConventional::ThresholdNone()
{
  this->SetThresholdMode(0);
}

//------------------------------------------------------------------------------
void vtkpxMergeFmriConventional::SetTransformation1(vtkAbstractTransform* tr)
{
  if (this->Transform1!=NULL)
    this->Transform1->UnRegister(this);

  this->Transform1=tr;

  if (this->Transform1!=NULL)
    this->Transform1->Register(this);

}

vtkAbstractTransform* vtkpxMergeFmriConventional::GetTransformation1()
{
  return this->Transform1;
}

void vtkpxMergeFmriConventional::SetTransformation2(vtkAbstractTransform* tr)
{
  if (this->Transform2!=NULL)
    this->Transform2->UnRegister(this);

  this->Transform2=tr;

  if (this->Transform2!=NULL)
    this->Transform2->Register(this);

}

vtkAbstractTransform* vtkpxMergeFmriConventional::GetTransformation2()
{
  return this->Transform2;
}

//------------------------------------------------------------------------------

void vtkpxMergeFmriConventional::SetConventional(vtkImageData* img)
{
  this->SetInput(img);
}

void vtkpxMergeFmriConventional::SetFmriInput(vtkImageData* fmri)
{
  this->SetfmriInput(fmri);

  if (this->fmriInput==NULL)
    return;


  if (this->MinimumThreshold==0 && this->MaximumThreshold==1)
    {
      double range[2];
      this->fmriInput->GetPointData()->GetScalars()->GetRange(range);

      if (range[0]<0.0 && range[1]>0.0)
	{
	  this->MinimumThreshold=range[1]*0.5;
	  this->MaximumThreshold=range[1];
	}
      else
	{
	  if (range[1]>0.0)
	    {
	      this->MinimumThreshold=(range[0]);
	      this->MaximumThreshold=(range[1]);
	    }
	  else
	    {
	      this->MinimumThreshold=fabs(range[1]);
	      this->MaximumThreshold=fabs(range[0]);
	    }
	}
    }

}
//------------------------------------------------------------------------------
vtkImageData* vtkpxMergeFmriConventional::DealWithTransformStuff()
{
  vtkGeneralTransform* combotransform=NULL;

  if (this->Transform1!=NULL || this->Transform2 !=NULL )
    {
      combotransform=vtkGeneralTransform::New();
      combotransform->PostMultiply();
      if (this->Transform1!=NULL)
	combotransform->Concatenate(this->Transform1);
      if (this->Transform2!=NULL)
	combotransform->Concatenate(this->Transform2);
    }

  vtkImageData* fmri=EquiResample(fmriInput,this->GetInput(),combotransform,0,0);//this->FlipFmri);

  if (combotransform!=NULL)
    combotransform->Delete();

  return fmri;
}
//------------------------------------------------------------------------------
void vtkpxMergeFmriConventional::ComputeStepBounds(vtkImageData* image,double range[2],int mode)
{
  vtkDataArray* scal=image->GetPointData()->GetScalars();
  scal->GetRange(range);
  
  if (mode==0)
    return;

  int n=scal->GetNumberOfTuples(),inc=1;
  if (n>1033)
    inc=int(n/1033);

  float mean=0.0,mean2=0.0;
  int num=0;
  for (int i=0;i<n;i+=inc)
    {
      float v=scal->GetComponent(i,0);
      if (v!=0.0)
	{
	  mean+=v;
	  mean2+=v*v;
	  ++num;
	}
    }

  if (num==0)
    return;

  mean/=float(num);
  mean2/=float(num);
  float std=sqrt(mean2-mean*mean);

  range[0]=Frange(mean-3.0*std,range[0],range[1]-1.0);
  range[1]=Frange(mean+3.0*std,range[0]+1.0,range[1]);
}
//------------------------------------------------------------------------------ 
float vtkpxMergeFmriConventional::ComputeAnatomicalVoxelValue(float vox,double range[2],float min,float max)
{
  
  if (vox<=range[0])
    return min;
  if (vox>=range[1])
    return max;

  return min+(max-min)/(range[1]-range[0])*(vox-range[0]);

}
//------------------------------------------------------------------------------
vtkImageData* vtkpxMergeFmriConventional::EquiResample(vtkImageData* source,
							      vtkImageData* target,
							      vtkAbstractTransform* xform,
							      int median,int flipx)
{

  if (source==NULL || target==NULL)
    {
      fprintf(stderr,"Source or Target=NULL\n");
      return NULL;
    }

  vtkbisImageReslice* reslice=vtkbisImageReslice::New(); reslice->OptimizationOff();
  reslice->SetInput(source);
  reslice->SetInterpolationModeToLinear();
  reslice->SetBackgroundLevel(1.0) ;
  reslice->SetOutputSpacing(target->GetSpacing());
  reslice->SetOutputOrigin(target->GetOrigin());
  reslice->SetOutputExtent(target->GetExtent());
  if (xform!=NULL)
    {
      reslice->SetResliceTransform(xform);
    }
  else
    {
      vtkIdentityTransform* tmp=vtkIdentityTransform::New();
      reslice->SetResliceTransform(tmp);
      tmp->Delete();
    }
  reslice->Update();

  vtkImageData* tmp=vtkImageData::New();
  if (flipx)
    {
      vtkImageFlip* imageFlip=vtkImageFlip::New();
      imageFlip->SetInput(reslice->GetOutput());
      imageFlip->SetFilteredAxis(0);
      imageFlip->SetPreserveImageExtent(1);
      imageFlip->Update();
      tmp->ShallowCopy(imageFlip->GetOutput());
      imageFlip->Delete();
    }
  else
    tmp->ShallowCopy(reslice->GetOutput());
  reslice->Delete();

  vtkImageData* output2=vtkImageData::New();
  if (median<1)
    {
      vtkImageMedian3D* imageMedian=vtkImageMedian3D::New();
      imageMedian->SetInput(tmp);
      imageMedian->Update();
      output2->ShallowCopy(imageMedian->GetOutput());
      imageMedian->Delete();
    }
  else
    output2->ShallowCopy(tmp);

  tmp->Delete();

  return output2;
}
//------------------------------------------------------------------------------
vtkTransform* vtkpxMergeFmriConventional::GetBoundsTransform(vtkImageData* conventional,
							     vtkImageData* functional,
							     int SourceBounds[6],int TargetBounds[6])
							
{
  vtkTransform* Transform=vtkTransform::New();
  Transform->Identity();

  if (conventional==NULL || functional == NULL)
    return Transform;
  
  double spf[3],spc[3];

  // Functional To Conventional 
  functional->GetSpacing(spf);
  conventional->GetSpacing(spc);

  double t1[3],t2[3],sc[3];
  for (int ia=0;ia<=2;ia++)
    {
      t1[ia]=float(SourceBounds[ia*2])*spf[ia];
      t2[ia]=-float(TargetBounds[ia*2])*spc[ia];
      sc[ia]=1.0;
      float bottom=SourceBounds[ia*2+1]*spf[ia];
      float top=TargetBounds[ia*2+1]*spc[ia];
      if (bottom!=0.0)
	sc[ia]=top/bottom;
    }
      
  
  Transform->PostMultiply();
  Transform->Translate(t2);
  Transform->Scale(sc);
  Transform->Translate(t1);

  /*{
    int i,j; 
    for(i = 0; i < 4; i++) 
      {
	for(j = 0; j < 4; j++) 
	  {
	    cerr << Transform->GetMatrix()->GetElement(i,j) << ((j != 3) ? " " : "\n");
	  }
      }
      }*/

  return Transform;
}
//------------------------------------------------------------------------------

vtkTransform* vtkpxMergeFmriConventional::GetBoundsTransform()
{
  return this->GetBoundsTransform(this->GetInput(),this->fmriInput,
				  this->SourceBounds,this->TargetBounds);
}

//---------------------------------------------------------------------------------------------
//  
//                        Simple Execute 
//

//---------------------------------------------------------------------------------------------
void vtkpxMergeFmriConventional::SimpleExecute(vtkImageData* conventional,vtkImageData* output)
{
  if (this->fmriInput==NULL)
      {
	vtkErrorMacro(<<"No fmri images!");
	return;
      }

  int dim[3];     conventional->GetDimensions(dim);
  int dim2[3];    this->fmriInput->GetDimensions(dim2);

  vtkImageData* fmri;
  int doresample=0;

  int diff=0;
  for (int ia=0;ia<=2;ia++)
    diff+=abs(dim[ia]-dim2[ia]);
  if (diff>0)
    {
      doresample=1;
      fmri=DealWithTransformStuff();
    }
  else
    {
      fmri=this->fmriInput;
      doresample=0;
    }


  // -------------------------
  // Next Allocate Output Data
  // -------------------------
  int numcomponents=1;
  int usecomponent=0;
  if (this->SingleFrameMode==0)
    numcomponents=fmri->GetNumberOfScalarComponents();
  else
    usecomponent=Irange(this->CurrentFrame,0,fmri->GetNumberOfScalarComponents()-1);
  
  output->SetSpacing(conventional->GetSpacing());
  output->SetOrigin(conventional->GetOrigin());
  output->SetScalarType(VTK_UNSIGNED_CHAR);
  output->SetNumberOfScalarComponents(numcomponents);
  output->SetDimensions(conventional->GetDimensions());
  output->SetWholeExtent(conventional->GetWholeExtent());
  output->AllocateScalars();
  vtkDataArray* outScalars=output->GetPointData()->GetScalars();

  this->UpdateProgress(0.05);


  // -------------------------
  // Set Color Mapping Parameters
  // -------------------------
  float fmri_numcol=3.0, fnum_col=55.0;
  int   mid_fmri=60,min_fmri=56,max_fmri=63;
  if (this->OutputMode==1)
    {
      fnum_col=191.0;
      fmri_numcol=31.0;
      min_fmri=192;
      max_fmri=255;
      mid_fmri=224;
    }
  else if (this->OutputMode==2)
    {
      fnum_col=237.0;
      fmri_numcol=8.0;
      min_fmri=238;
      max_fmri=255;
      mid_fmri=247;
    }

  // -------------------------------------
  // Normalize Anatomical Stuff
  // -------------------------------------
  double crange[2];  this->ComputeStepBounds(conventional,crange,this->NormalizeAnatomical);

  // Scale factor for conv  0..55
  if (crange[0]==crange[1])
    crange[1]+=1.0;
  float convscale=fnum_col/(crange[1]-crange[0]);


  // ------------------------------------------
  // Do fMRI Stuff  -- once component at a time
  // ------------------------------------------
  this->PositiveLaterality=0.0;
  this->NegativeLaterality=0.0;
  double poscount[2],negcount[2];
  for (int ia=0;ia<=1;ia++)
    {
      poscount[ia]=0.0; negcount[ia]=0.0;
    }
     

  for (int outputcomponent=0;outputcomponent<numcomponents;outputcomponent++)
    {

      int inputcomponent=outputcomponent;
      if (this->SingleFrameMode)
	inputcomponent=usecomponent;

      double fmrirange[2];
      fmri->GetPointData()->GetScalars()->GetRange(fmrirange,inputcomponent);

      // Scale fmri 
      if (this->MaximumThreshold==this->MinimumThreshold)
	this->MaximumThreshold+=1.0;
      float fmriscale=fmri_numcol/(this->MaximumThreshold-this->MinimumThreshold);
      
      outScalars->FillComponent(outputcomponent,0.0);

      vtkDebugMacro(<< "Current Component = " << inputcomponent << " ---> " << outputcomponent << "\n");
      vtkDebugMacro(<< "Ranges: conv " << crange[0] <<","<< crange[1] <<",  fmri:"<< fmrirange[0]<<","<<fmrirange[1]);
      vtkDebugMacro(<< "Thresholds : min : " << this->MinimumThreshold << " max: " <<  this->MaximumThreshold  << " Intensity : " << this->IntensityThreshold );
      vtkDebugMacro(<< "Convscale = " << convscale << ", ThresholdMode " << this->ThresholdMode << " FlipFmri=" << this->FlipFmri << "\n");
      
      // Lopp Through all voxels 
      // -----------------------
      
      int numpixels=dim[0]*dim[1];
      

      int midpoint=dim[0]/2;

      for (int sl=0;sl<dim[2];sl++)
	{
	  int offset=sl*numpixels;
	  for (int iy=0;iy<dim[1];iy++)
	    {
	      int offset2=offset+iy*dim[0];
	      for (int ix=0;ix<dim[0];ix++)
		{
		  float vc=conventional->GetPointData()->GetScalars()->GetComponent(ix+offset2,0);
		  int c=(int)this->ComputeAnatomicalVoxelValue(vc,crange,0,fnum_col);
		  
		  float fm;
		  if (!this->FlipFmri)
		    fm=fmri->GetPointData()->GetScalars()->GetComponent(ix+offset2,inputcomponent);
		  else
		    fm=fmri->GetScalarComponentAsDouble(dim[0]-ix-1,iy,sl,inputcomponent);
	      
		  if (c>=this->IntensityThreshold)
		    {
		      if (fm>0.0 && 
			  ( this->ThresholdMode==1 || this->ThresholdMode==3) && 
			  fm>=this->MinimumThreshold )
			{
			  float fm2=(fm-this->MinimumThreshold)*fmriscale;
			  c=Irange(int(fm2+mid_fmri+0.5),mid_fmri,max_fmri);
			  if (ix<midpoint)
			    poscount[0]++;
			  else
			    poscount[1]++;
			}
		      else if (fm<0.0 && 
			       ( this->ThresholdMode==2 || this->ThresholdMode==3) && 
			       fabs(fm)>=this->MinimumThreshold )  
			{
			  float fm2=(fabs(fm)-this->MinimumThreshold)*fmriscale;
			  c=Irange(int(0.5+mid_fmri-fm2),min_fmri,mid_fmri-1);
			  if (ix<midpoint)
			    negcount[0]++;
			  else
			    negcount[1]++;
			}
		      
		    }
		  outScalars->SetComponent(ix+offset2,outputcomponent,float(c));
		  //numtotal++;
		}
	    }
	  
	  float pog=float(sl)/float(dim[2]*numcomponents)+float(outputcomponent+1)/float(numcomponents);
	  this->UpdateProgress(pog);	
	}
    }
  
  if ((poscount[0]+poscount[1])>0.0)
    {
      this->PositiveLaterality=(poscount[1]-poscount[0])/(poscount[0]+poscount[1]);
    }
  
  if ((negcount[0]+negcount[1])>0.0)
    {
      this->NegativeLaterality=(negcount[1]-negcount[0])/(negcount[0]+negcount[1]);
    }
    



  this->UpdateProgress(1.0);

  if (doresample==1)
    {
      //      fprintf(stderr,"Deleting fmri\n");
      fmri->Delete();
    }

}
//------------------------------------------------------------------------------
/*

8 components

0  index
2  number of voxels
3  sum                --> Mean
4  sum**2             --> SIGMA
5  number of voxels > 0
6  vol
7  vol in mm^3
8  max value
9  max location x
10 max location y
11 max location z
12 mean location x
13 mean location x
14 mean location x




*/
// -----------------------------------------------------------------------------------



int vtkpxMergeFmriConventional::ComputeROIStatistics(vtkImageData* input,vtkImageData* roi,
						     vtkAbstractTransform* tr,vtkFloatArray *inout_stats)
{
  if (input==NULL || roi==NULL || tr==NULL || inout_stats==NULL)
    {
      fprintf(stderr,"Compute ROI Inout_Stats bad inputs\n");
      return 0;
    }
	      
  int nt1=input->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nt2=roi->GetPointData()->GetScalars()->GetNumberOfTuples();

  vtkImageData* tmp=vtkImageData::New();

  if (nt1!=nt2)
    {
      vtkbisImageReslice* reslice=vtkbisImageReslice::New(); reslice->OptimizationOff();
      reslice->SetInterpolationMode(0);
      reslice->SetBackgroundLevel(0);
      reslice->SetInput(roi);
      reslice->SetInformationInput(input);
      reslice->SetResliceTransform(tr);
      reslice->Update();
      tmp->ShallowCopy(reslice->GetOutput());
      reslice->Delete();
    }
  else
    {
      //      fprintf(stderr,"Identical Dimensions NOT Reslicing\n");
      tmp->ShallowCopy(roi);
    }


  double sp[3];  input->GetSpacing(sp);
  double voxel_size=sp[0]*sp[1]*sp[2];

  vtkDataArray* data_in=input->GetPointData()->GetScalars();
  vtkDataArray* roi_array=tmp->GetPointData()->GetScalars();
  
  int nvox=data_in->GetNumberOfTuples();
  int nc=data_in->GetNumberOfComponents();

  //  fprintf(stderr,"Num vox =%d numframes=%d\n",nvox,nc);

  double range[2];
  roi_array->GetRange(range);
  int r0=int(range[0]);
  int r1=int(range[1]);
  int numlevels=r1-r0+1;

  //  fprintf(stderr,"Range of roi=%.1f:%.1f or %d:%d numlevels=%d\n",range[0],range[1],r0,r1,numlevels);

  
  int isinteger=0;
  /*if (input->GetScalarType()==VTK_SHORT ||
      input->GetScalarType()==VTK_CHAR ||
      input->GetScalarType()==VTK_UNSIGNED_SHORT ||
      input->GetScalarType()==VTK_UNSIGNED_CHAR ||
      input->GetScalarType()==VTK_INT ||
      input->GetScalarType()==VTK_UNSIGNED_INT  ||
      input->GetScalarType()==VTK_LONG ||
      input->GetScalarType()==VTK_UNSIGNED_LONG )
    {
      isinteger=1;
      fprintf(stderr,"Input Data is Integer\n");
      }*/
  vtkDataArray* stats=NULL;
  if (isinteger==0)
    stats=vtkDoubleArray::New();
  else
    stats=vtkLongArray::New();
  
  stats->SetNumberOfComponents(15);
  stats->SetNumberOfTuples(nc*numlevels);
  inout_stats->SetNumberOfComponents(15);
  inout_stats->SetNumberOfTuples(nc*numlevels);

  for (int j=0;j<15;j++)
    stats->FillComponent(j,0.0);

  for (int k=0;k<nc;k++)
    for (int j=0;j<numlevels;j++)
      {
	stats->SetComponent(k*numlevels+j,0,k);
	stats->SetComponent(k*numlevels+j,1,r0+j);
      }


  
  int voxel=0;
  int dim[3]; input->GetDimensions(dim);
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  int index=(int)roi_array->GetComponent(voxel,0);
	  for (int frame=0;frame<nc;frame++)
	    {
	      float v=data_in->GetComponent(voxel,frame);
	      
	      int outindex=frame*numlevels+index;
	      stats->SetComponent(outindex,2,stats->GetComponent(outindex,2)+1);
	      stats->SetComponent(outindex,3,stats->GetComponent(outindex,3)+v);
	      stats->SetComponent(outindex,4,stats->GetComponent(outindex,4)+v*v);
	      if (v>0.0)
		stats->SetComponent(outindex,5,stats->GetComponent(outindex,5)+1);

	      if (fabs(v)> fabs(stats->GetComponent(outindex,8)))
		{
		  stats->SetComponent(outindex,8,v);
		  stats->SetComponent(outindex,9,double(i));
		  stats->SetComponent(outindex,10,double(j));
		  stats->SetComponent(outindex,11,double(k));
		}

	      stats->SetComponent(outindex,12,stats->GetComponent(outindex,12)+double(i));
	      stats->SetComponent(outindex,13,stats->GetComponent(outindex,13)+double(j));
	      stats->SetComponent(outindex,14,stats->GetComponent(outindex,14)+double(k));
	    }
	  ++voxel;
	}


  for (int ka=0;ka<nc;ka++)
    for (int ja=0;ja<numlevels;ja++)
      {
	int outindex=ka*numlevels+ja;
	double nt=stats->GetComponent(outindex,2);
	double vol=nt*voxel_size;
	double mean=0.0,sigma=1.0, centroid[3];
	if (nt>=1.0)
	  {
	    double correction=sqrt(nt)/sqrt(nt-1.0);
	    double factor=1.0/(nt);
	    
	    double sum=stats->GetComponent(outindex,3);
	    double sum2=stats->GetComponent(outindex,4);
	    if (isinteger)
	      {
		sum=double(int(sum+0.5));
		sum2=double(int(sum2+0.5));
	      }

	    mean = sum*factor;
	    double diff=sum2*factor-mean*mean;
	    if (fabs(diff)<1.0e-7)
	      sigma=0.0;
	    else if (diff<0.0)
	      sigma=-1.0;
	    else
	      sigma  = sqrt(sum2*factor-mean*mean)*correction;

	    //	    fprintf(stderr,"ka=%d ja=%d,diff=%f sigma=%f\n",ka,ja,diff,sigma);

	    centroid[0]=factor*stats->GetComponent(outindex,12);
	    centroid[1]=factor*stats->GetComponent(outindex,13);
	    centroid[2]=factor*stats->GetComponent(outindex,14);

	  }
	stats->SetComponent(outindex,3,mean);
	stats->SetComponent(outindex,4,sigma);
	stats->SetComponent(outindex,12,centroid[0]);
	stats->SetComponent(outindex,13,centroid[1]);
	stats->SetComponent(outindex,14,centroid[2]);
	stats->SetComponent(outindex,6,vol);
	stats->SetComponent(outindex,7,stats->GetComponent(outindex,5)*voxel_size);
      }

  for (int c=0;c<15;c++)
    inout_stats->CopyComponent(c,stats,c);
  stats->Delete();
  tmp->Delete();
  return numlevels*nc;
}
//------------------------------------------------------------------------------
int vtkpxMergeFmriConventional::ModifyMask(vtkImageData* input,int x,int y,int z)
{
  
  int dim[3];  input->GetDimensions(dim); 
  vtkDataArray* data_in=input->GetPointData()->GetScalars();

  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  int index=i+j*dim[0]+k*dim[1]*dim[0];
	  float v=data_in->GetComponent(index,0);
	  if (v>0)
	    {
	      if (i<x && j < y )
		{
		  if (k<65)
		    data_in->SetComponent(index,0,2);
		  else
		    data_in->SetComponent(index,0,3);
		}
	      else
		data_in->SetComponent(index,0,1);
	    }

	  }
	return 1;
}
//------------------------------------------------------------------------------
/*int vtkpxMergeFmriConventional::ComputeROIStatisticsComplex(vtkImageData* input,vtkImageData* roi,vtkFloatArray *stats)
{
  return vtkpxMergeFmriConventional::ComputeROIStatisticsComplex(input,roi,NULL,stats);
  }*/
//------------------------------------------------------------------------------
 /*int vtkpxMergeFmriConventional::ComputeROIStatisticsComplex(vtkImageData* input,vtkImageData* roi,
							    vtkFloatArray* ranges,vtkFloatArray *stats)
{
  if (input==NULL || roi==NULL ||  stats==NULL)
    {
      fprintf(stderr,"Compute ROI Stats bad inputs\n");
      return 0;
    }

  int numranges=0;
  if (ranges!=NULL)
    {
      if (ranges->GetNumberOfComponents()==2)
	numranges=ranges->GetNumberOfTuples();
    }

	      
  double sp[3];  input->GetSpacing(sp);
  double voxel_size=sp[0]*sp[1]*sp[2];

  vtkDataArray* data_in=input->GetPointData()->GetScalars();
  vtkDataArray* roi_array=roi->GetPointData()->GetScalars();
  
  int nvox =data_in->GetNumberOfTuples();
  int nc=data_in->GetNumberOfComponents();
  int nmask=roi_array->GetNumberOfTuples();
  
  if (nvox!=nmask)
    {
      fprintf(stderr,"Cannont Compute ROI Stats, different size images %d vs %d\n",nvox,nmask);
      return 0;
    }


  fprintf(stderr,"Computing ROI Statistics Num vox =%d numframes=%d number of ranges = %d\n",nvox,nc,numranges);

  double range[2];
  roi_array->GetRange(range);
  int r0=int(range[0]);
  int r1=int(range[1]);
  int numlevels=r1-r0+1;

  fprintf(stderr,"Range of roi=%.1f:%.1f or %d:%d numlevels=%d\n",range[0],range[1],r0,r1,numlevels);

  stats->SetNumberOfComponents(2+3*(numranges+1));
  int numcomp=stats->GetNumberOfComponents();
  stats->SetNumberOfTuples(nc*numlevels);
  for (int j=0;j<numcomp;j++)
    stats->FillComponent(j,0);
  
  for (int k=0;k<nc;k++)
    for (int j=0;j<numlevels;j++)
      {
	stats->SetComponent(k*numlevels+j,0,k);
	stats->SetComponent(k*numlevels+j,1,r0+j);
      }
  
  float* minv=new float[numranges+1];
  float* maxv=new float[numranges+1];
  if (numranges>0)
    {
      for (int i=0;i<numranges;i++)
	{
	  minv[i]=ranges->GetComponent(i,0);
	  maxv[i]=ranges->GetComponent(i,1);
	}
    }
  
  for (int voxel=0;voxel<nvox;voxel++)
    {
      int index=(int)roi_array->GetComponent(voxel,0);
      for (int frame=0;frame<nc;frame++)
	{
	  float v=data_in->GetComponent(voxel,frame);
	  int outindex=frame*numlevels+index;
	  stats->SetComponent(outindex,2,stats->GetComponent(outindex,2)+1);
	  stats->SetComponent(outindex,3,stats->GetComponent(outindex,3)+v);
	  stats->SetComponent(outindex,4,stats->GetComponent(outindex,4)+v*v);
	  int offset=5;
	  for (int j=0;j<numranges;j++)
	    {
	      if (v>=minv[j] && v< maxv[j])
		{
		  stats->SetComponent(outindex,offset, stats->GetComponent(outindex, offset)+1);
		  stats->SetComponent(outindex,offset+1,stats->GetComponent(outindex,offset+1)+v);
		  stats->SetComponent(outindex,offset+2,stats->GetComponent(outindex,offset+2)+v*v);
		}
	      offset+=3;
	    }
	}
    }

  for (int ka=0;ka<nc;ka++)
    for (int ja=0;ja<numlevels;ja++)
      {
	int outindex=ka*numlevels+ja;

	int offset=2;
	for (int ia=0;ia<=numranges;ia++)
	  {
	    double nt=stats->GetComponent(outindex,offset);
	    double mean=0.0,sigma=1.0;
	    if (nt>0)
	      {
		double correction=sqrt(nt)/sqrt(nt-1.0);
		double factor=1.0/(nt);
		
		double sum=stats->GetComponent(outindex,offset+1);
		double sum2=stats->GetComponent(outindex,offset+2);
		
		mean = sum*factor;
		double diff=sum2*factor-mean*mean;
		if (fabs(diff)<1.0e-7)
		  sigma=0.0;
		else if (diff<0.0)
		  sigma=-1.0;
		else
		  sigma  = sqrt(sum2*factor-mean*mean)*correction;
	      }
	    stats->SetComponent(outindex,offset+1,mean);
	    stats->SetComponent(outindex,offset+2,sigma);
	    offset+=3;
	  }
      }

  delete [] minv;
  delete [] maxv;
  return numlevels*nc;
  }*/
//------------------------------------------------------------------------------

