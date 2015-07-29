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



/*!
 * vtkbisRemoveGlobalTemporalDrift.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: Isabella Murphy, Xilin Shen
 *
 * Use Legendre Polynomial or power to the order to create a LSQ matrix
 * to model the potential drift of fMRI data
 */
#include "vtkObjectFactory.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkbisRemoveGlobalTemporalDrift.h"
#include "vtkbisTimeSeriesNormalize.h"
#include "vtkFloatArray.h"

vtkbisRemoveGlobalTemporalDrift* vtkbisRemoveGlobalTemporalDrift::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisRemoveGlobalTemporalDrift");
  if(ret)
    {
      return (vtkbisRemoveGlobalTemporalDrift*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisRemoveGlobalTemporalDrift;
}

// -------------------------------------------------------------------------------------------------
vtkbisRemoveGlobalTemporalDrift::vtkbisRemoveGlobalTemporalDrift()
{
  this->OutputNormalized=1;
  this->NormalizedThreshold=1;
  this->Threshold=0.05;
}

void vtkbisRemoveGlobalTemporalDrift::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{

  fprintf(stderr,"Beginning RemoveGlobalTemporalDrift use legendre=%d order=%d\n",
		  this->UseLegendrePolynomials,this->Order);


  vtkDataArray* msk=NULL;
  if (this->ImageMask!=NULL)
    {
      int dim[3]; this->ImageMask->GetDimensions(dim);
      int dim2[3]; input->GetDimensions(dim2);
      int sum=0;
      for (int ia=0;ia<=2;ia++)
	sum+=abs(dim[ia]-dim2[ia]);
      if (sum>0)
	{
    	  fprintf(stderr,"Bad Mask Input to vtkbisRemoveGlobalTemporalDrift SimpleExecute\n");
    	  return;
	}
      msk=this->ImageMask->GetPointData()->GetScalars();
    }


  double thr=this->Threshold;
  if (this->NormalizedThreshold>0)
    {
      double range[2]; input->GetPointData()->GetScalars()->GetRange(range);
      thr=this->Threshold*(range[1]-range[0])+range[0];
    }


  vtkImageData* normalized=vtkImageData::New();
  normalized->CopyStructure(output);
  normalized->SetNumberOfScalarComponents(output->GetNumberOfScalarComponents());
  normalized->AllocateScalars();

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkFloatArray* stats=vtkFloatArray::New();
  stats->SetNumberOfComponents(2);
  stats->SetNumberOfTuples(inp->GetNumberOfTuples());
  int numframes=inp->GetNumberOfComponents();
  int nt=inp->GetNumberOfTuples();
  double scalefactor=1.0/double(numframes);

  vtkDataArray* norminp=normalized->GetPointData()->GetScalars();

  for (int voxel=0;voxel<nt;voxel++)
    {
      double sum=0.0,sum2=0.0;
      for (int i=0;i<numframes;i++)
	{
	  double v=inp->GetComponent(voxel,i);
	  sum+=v;
	  sum2+=v*v;
	}
      double mean=sum*scalefactor;
      double sigma=sqrt(sum2*scalefactor-mean*mean);
      stats->SetComponent(voxel,0,mean);
      stats->SetComponent(voxel,1,sigma);
      if (sigma<0.00001)
	sigma=0.00001;
      for (int i=0;i<numframes;i++)
	norminp->SetComponent(voxel,i,(inp->GetComponent(voxel,i)-mean)*sigma);
    }
  
  double numgood=0.0;


  double* sumarray=new double[numframes];
  for (int i=0;i<numframes;i++)
    sumarray[i]=0.0;

  double range[2];
  inp->GetRange(range);

  for (int voxel=0;voxel<nt;voxel++)
    {
      int usevoxel=1;
      if (msk!=NULL)
	{
    	  if (msk->GetComponent(voxel,0)<1)
	    usevoxel=0;
	}
      else 
	{
	  double v=inp->GetComponent(voxel,0);
	  if (v<thr)
	    usevoxel=0;
	}
      
      if (usevoxel)
	{
	  numgood+=1.0;
	  for (int i=0;i<numframes;i++)
	    sumarray[i]+=norminp->GetComponent(voxel,i);
	}
    }
  if (numgood==0.0)
    numgood=1.0;

  for (int i=0;i<numframes;i++)
    sumarray[i]/=numgood;

  fprintf(stderr,"So far, numgood=%f\n",numgood);
  
  this->UpdateProgress(0.1);


  vtkpxMatrix* LSQ=this->ComputeLSQMatrix(this->Order,numframes);
  vtkpxMatrix* temp2=vtkpxMatrix::New();
  temp2->Zeros(this->Order+1,1);

  vtkpxMatrix* temp1=vtkpxMatrix::New();
  temp1->Zeros(numframes,1);
  for (int i=0;i<numframes;i++)
    temp1->SetDirectElement(i,0,sumarray[i]);

  vtkpxMatrix::Multiply(LSQ,temp1,temp2);  
  this->UpdateProgress(0.2);

  fprintf(stderr,"Global Polynomial Fit: ");
  for (int p=0;p<=this->Order;p++)
    fprintf(stderr,"p[%d]=%.6f ",p,temp2->GetDirectElement(p,0));
  fprintf(stderr,"\n");

  vtkDataArray* out=output->GetPointData()->GetScalars();
  for (int voxel=0;voxel<nt;voxel++)
    {
      for (int ia=0;ia<numframes;ia++)
	{
	  double t=this->ComputeTime(ia,numframes);
	  double v=norminp->GetComponent(voxel,ia);
	  for (int p=1;p<=this->Order;p++)
	    v=v-temp2->GetDirectElement(p,0)*Polynomial(t,p);
	  if (this->OutputNormalized==0)
	    v=v*stats->GetComponent(voxel,1)+stats->GetComponent(voxel,0);
	  out->SetComponent(voxel,ia,v);
	}
    }
  
  
  this->UpdateProgress(0.99);
  delete [] sumarray;
  LSQ->Delete();
  temp1->Delete();
  temp2->Delete();
  stats->Delete();
  normalized->Delete();
  this->UpdateProgress(1.0);
}


