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





#include "vtkpxImageRobustHistogram.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkpxUtil.h"
#include <stdio.h>
#include "pxutil.h"
#include "pxisinf.h"
#include <math.h>

vtkpxImageRobustHistogram* vtkpxImageRobustHistogram::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageRobustHistogram");
  if(ret)
    {
      return (vtkpxImageRobustHistogram*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageRobustHistogram;
}


// Construct object to extract all of the input data.
vtkpxImageRobustHistogram::vtkpxImageRobustHistogram()
{
  this->NumberOfBins=128;
  this->Interpolation=0;
  this->LowerBound=0.0;
  this->UpperBound=1.0;
  this->LowerThreshold=0.0;
  this->UpperThreshold=0.0;
  this->Entropy=0.0;
  this->Initialized=0;
  this->RobustMode=0;
  this->SaturationMode=0;
  this->LastOutput=NULL;
  this->WeightImage=NULL;
  this->IgnoreMaskedVoxels=1;
}

vtkpxImageRobustHistogram::~vtkpxImageRobustHistogram()
{
  if (this->LastOutput!=NULL)
    this->LastOutput->Delete();
  this->SetWeightImage(NULL);
}
// ---------------------------------------------------------------------------
void vtkpxImageRobustHistogram::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkpxSimpleImageToImageFilter::PrintSelf(os,indent);

  os << indent << "  WeightImage " << this->WeightImage << "\n";
  os << indent << "  NumberOfBins " << this->NumberOfBins << "\n";
  os << indent << "  Interpolation " << this->Interpolation << "\n";
  os << indent << "  RobustMode " << this->RobustMode << "\n";
  os << indent << "  SaturationMode " << this->SaturationMode << "\n";
  os << indent << "  Bounds " << this->LowerBound << ":" << this->UpperBound << "\n";
  os << indent << "  Thresholds " << this->LowerThreshold << ":" << this->UpperThreshold << "\n";
  os << indent << "  Entropy " << this->Entropy << "\n";
  os << indent << "  Initialized " << this->Initialized << "\n";
}
// ---------------------------------------------------------------------------
void vtkpxImageRobustHistogram::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();

  if (input==NULL)
    return;

  double sp[3]; sp[1]=1.0; sp[2]=1.0;

  this->UpdateThresholds();
  sp[0]=(this->UpperThreshold-this->LowerThreshold)/double(this->NumberOfBins);

  int wholeExtent[6];
  int dims[3];    dims[1]=1; dims[2]=1; dims[0]=this->NumberOfBins;

  wholeExtent[0]=0;
  wholeExtent[1]=dims[0]-1;
  wholeExtent[2]=0;
  wholeExtent[3]=0;
  wholeExtent[4]=0;
  wholeExtent[5]=0;

  output->SetSpacing(sp[0],sp[1],sp[2]);
  output->SetOrigin(this->LowerThreshold,0.0,0.0);
  output->SetWholeExtent( wholeExtent );
  output->SetUpdateExtent( wholeExtent );
  output->SetScalarTypeToDouble();
  output->SetNumberOfScalarComponents(1);

}
// ---------------------------------------------------------------------------
void vtkpxImageRobustHistogram::BackupHistogram()
{
  if (this->Initialized==0)
    return;

  if (this->LastOutput==NULL)
    this->LastOutput=vtkImageData::New();
  this->LastOutput->DeepCopy(this->GetOutput());
}
// ---------------------------------------------------------------------------
void vtkpxImageRobustHistogram::SetPlainHistogram()
{
  this->RobustMode=0;
  this->Interpolation=0;
  this->SaturationMode=0;
}
// ---------------------------------------------------------------------------
void vtkpxImageRobustHistogram::UpdateThresholds()
{
  vtkImageData* input=this->GetInput();
  if (input==NULL)
    return;

  //  fprintf(stderr,"Frame=%d\n",this->Frame);

  double range[2]; input->GetPointData()->GetScalars()->GetRange(range,this->Frame);
  float minv=range[0];
  float maxv=range[1];

  if (this->RobustMode==1)
  {
      vtkpxUtil::ImageRobustRange(input,this->LowerBound,this->UpperBound,minv,maxv);

      if (this->LowerBound>0.0)
	this->LowerThreshold=minv;
      else
	this->LowerThreshold=range[0];

      if (this->UpperBound<1.0)
	this->UpperThreshold=maxv;
      else
	this->UpperThreshold=range[1];
  }
  else if (this->UpperThreshold==this->LowerThreshold)
  {

      this->UpperThreshold=maxv;
      this->LowerThreshold=minv;
  }
    //std::cout<<"frame:"<<this->Frame<<"max:"<<this->UpperThreshold<<" min:"<<this->LowerThreshold<<endl;
}


void vtkpxImageRobustHistogram::AddDataArray(vtkDataArray* data,float factor,int frame,vtkDataArray* weights)
{
  if (this->Initialized==0)
    return;

  vtkDataArray* out=this->GetOutput()->GetPointData()->GetScalars();
  int nt=data->GetNumberOfTuples();

  double maxbin=double(this->NumberOfBins-1);
  double maxrange=this->UpperThreshold-this->LowerThreshold;
  double numbins=maxbin+1.0;

  for (int i=0;i<nt;i++)
  {
      double v=data->GetComponent(i,frame);
      if (!isnan(v))
      {
    	  double bin=-1.0;
    	  double wgt=1.0;
    	  if (weights!=NULL)
    		  wgt=weights->GetComponent(i,0);

    	  if (wgt==0)
    	  {
    		  if (this->IgnoreMaskedVoxels==0)
    			  out->SetComponent(0,0,out->GetComponent(0,0)+1.0);
    	  }
    	  else
    	  {
    		  if (this->SaturationMode==0)
    		  {
    			  if (v<this->LowerThreshold || v>this->UpperThreshold)
    				  bin=-1;
    			  else
    				  bin=maxbin*(v-this->LowerThreshold)/maxrange;
					  //bin=this->NumberOfBins*(v-this->LowerThreshold)/maxrange;
					  //		  std::cout<<"v:"<<v<<"bin:"<<bin;
    		  }
    		  else
    		  {
    			  if (v<=this->LowerThreshold)
    				  bin=0.0;
    			  else if (v>=this->UpperThreshold)
    				  bin=maxbin;
    			  else
    				  bin=maxbin*(v-this->LowerThreshold)/maxrange;
    		  }

    		  if (bin!=-1 && !isnan(bin) && !isinf(bin))
    		  {
    			  if (this->Interpolation==0)
    			  {
    				  int b=int(bin+0.5);
    				  out->SetComponent(b,0,out->GetComponent(b,0)+factor*wgt);
    				  //  std::cout<<"b:"<<b<<"number:"<<out->GetComponent(b,0)<<endl;
    			  }
    			  else
    			  {
    				  int b0=(int)(bin);
    				  int b1=(int)(bin+1.0);

    				  if (b1==this->NumberOfBins)
    					  b1=b0;

    				  double w1=bin-double(b0);
    				  double w0=1.0-w1;

    				  out->SetComponent(b0,0,out->GetComponent(b0,0)+w0*factor*wgt);
    				  out->SetComponent(b1,0,out->GetComponent(b1,0)+w1*factor*wgt);
    			  }
    		  }
	    }
	}
  }
}
// ---------------------------------------------------------------------------
vtkDataArray* vtkpxImageRobustHistogram::GetWeightArray(vtkImageData* image,vtkImageData* weightimage)
{
  if (weightimage==NULL || image==NULL)
    return NULL;

  vtkDataArray* weights=weightimage->GetPointData()->GetScalars();
  if (weights->GetNumberOfTuples()!=image->GetPointData()->GetScalars()->GetNumberOfTuples())
    return NULL;

  return weights;
}
// ---------------------------------------------------------------------------
int vtkpxImageRobustHistogram::RemoveImage(vtkImageData* newimage,vtkImageData* weights)
{
  if (newimage==NULL)
    return 0;

  if (this->Initialized==0)
    return 0;

  this->AddDataArray(newimage->GetPointData()->GetScalars(),-1.0,this->Frame,this->GetWeightArray(newimage,weights));
  return 1;
}
// ---------------------------------------------------------------------------
int vtkpxImageRobustHistogram::UndoLastChange()
{
  if (this->Initialized==0 || this->LastOutput==NULL)
    return 0;

  vtkImageData* output=this->GetOutput();
  output->DeepCopy(this->LastOutput);
  return 1;
}
// ---------------------------------------------------------------------------
double vtkpxImageRobustHistogram::ComputeEntropy()
{
  vtkDataArray* arr=this->GetOutput()->GetPointData()->GetScalars();

  int nt=arr->GetNumberOfTuples();
  double sum=0.0;
  for (int i=0;i<nt;i++)
    sum+=arr->GetComponent(i,0);

  this->Entropy=0.0;

  if (sum>1.0 && nt>1)
    {
      for (int i=0;i<nt;i++)
	{
	  double p=arr->GetComponent(i,0)/sum;
	  if (p>0.0)
	    this->Entropy+= (-p*log(p));
	}
      this->Entropy/=log(double(nt));
    }

  return this->Entropy;
}
// ---------------------------------------------------------------------------
int vtkpxImageRobustHistogram::AddImage(vtkImageData* newimage,vtkImageData* weights)
{
  if (newimage==NULL)
    return 0;

  if (this->GetInput()==NULL)
    {
      this->SetInput(newimage);
      this->Update();
    }
  else
    {
      this->Update();
      this->AddDataArray(newimage->GetPointData()->GetScalars(),1.0,this->Frame,this->GetWeightArray(newimage,weights));
    }
  return 1;
}
// ---------------------------------------------------------------------------
void vtkpxImageRobustHistogram::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
  if (input==NULL)
    return;

  int nc=input->GetNumberOfScalarComponents();


  if (this->Frame<0)
    this->Frame=0;
  if (this->Frame>=nc)
    this->Frame=nc-1;


  vtkDataArray* data=output->GetPointData()->GetScalars();
  vtkDataArray* weights=this->GetWeightArray(input,this->WeightImage);
  if (weights==NULL)
    this->SetWeightImage(NULL);

  data->FillComponent(0,0.0);
  this->Initialized=1;
  this->AddDataArray(input->GetPointData()->GetScalars(),1.0,this->Frame,weights);


}
// ---------------------------------------------------------------------------
int vtkpxImageRobustHistogram::SaveGnuplot(char* fname)
{
  if (this->Initialized==0)
    return 0;

  FILE* fout=fopen(fname,"w");
  if (!fout)
    return 0;

  vtkImageData* output=this->GetOutput();
  vtkDataArray* data=output->GetPointData()->GetScalars();
  int nt=data->GetNumberOfTuples();
  double  sp[3]; output->GetSpacing(sp);
  double ori[3]; output->GetOrigin(ori);

  fprintf(fout,"#Bin Minimum\t Bin Maximum\t  Value\t Cumulative \n");

  double sum=0.0;
  for (int i=0;i<nt;i++)
    {
      //      double x=double(i+0.5)*sp[0]+ori[0];
      double x0=double(i)*sp[0]+ori[0];
      double x1=double(i+1.0)*sp[0]+ori[0];
      double y=data->GetComponent(i,0);
      sum+=y;
      fprintf(fout,"%f \t %f \t %f \t %f  \n",x0,x1,y,sum);

    }

  fclose(fout);
  return 1;
}

// ---------------------------------------------------------------------------
int vtkpxImageRobustHistogram::GetObjectmapStatistics(vtkImageData* img,vtkDoubleArray* array)
{

  if (img==NULL || array==NULL)
    {
      fprintf(stderr,"Bad Inputs to GetObjectmapStatistics \n");
      return 0;
    }
  img->GetPointData()->GetScalars()->Modified();
  double* nonused_dummy = img->GetPointData()->GetScalars()->GetRange(0);  
  double range[2]; img->GetPointData()->GetScalars()->GetRange(range);

  int minv=int(range[0]);
  int maxv=int(range[1]);


  int nt=maxv-minv+1;

  array->SetNumberOfComponents(4);
  array->SetNumberOfTuples(nt);

  for (int ia=0;ia<=3;ia++)
    array->FillComponent(ia,0.0);

  int    dim[3]; img->GetDimensions(dim);

  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  int v=(int)img->GetScalarComponentAsDouble(i,j,k,0)-minv;
	  if (v>=0 && v<nt)
	    {
	      array->SetComponent(v,0,array->GetComponent(v,0)+i);
	      array->SetComponent(v,1,array->GetComponent(v,1)+j);
	      array->SetComponent(v,2,array->GetComponent(v,2)+k);
	      array->SetComponent(v,3,array->GetComponent(v,3)+1);
	    }
	  else
	    {
	      fprintf(stderr,"Bad Voxel %d,%d,%d = %d\n",i,j,k,v);
	    }
	}



  for (int i=0;i<nt;i++)
    {
      double nvoxel=array->GetComponent(i,3);
      if (nvoxel>0.0)
	{
	  for (int j=0;j<=2;j++)
	    array->SetComponent(i,j,array->GetComponent(i,j)/nvoxel);
	}
    }

  return nt;
}

