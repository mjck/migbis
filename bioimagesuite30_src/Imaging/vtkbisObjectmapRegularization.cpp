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
#include "vtkbisObjectmapRegularization.h"
#include "vtkDoubleArray.h"
#include "vtkMath.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"

vtkbisObjectmapRegularization* vtkbisObjectmapRegularization::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisObjectmapRegularization");
  if(ret)
    {
      return (vtkbisObjectmapRegularization*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisObjectmapRegularization;
}

// Construct object with no children.
vtkbisObjectmapRegularization::vtkbisObjectmapRegularization()
{
  this->Smoothness=2.0;
  this->NumberOfIterations=20;
  this->ConvergencePercentage=0.2;
  this->DebugVoxel=-1;
  //  this->DebugVoxel=481439;
}

// ----------------------------------------------------------------------------
vtkbisObjectmapRegularization::~vtkbisObjectmapRegularization()
{

}
// ----------------------------------------------------------------------------
//
// M - Step
//
// ---------------------------------------------------------------------------
//
//   Total Minus Log Probability is sum of log likelihood and log MRF (MRF=Gibbs Model, log MRF=Energy Term)
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
double vtkbisObjectmapRegularization::ComputeTotalMinusLogProbability(double    intensity,
								      int    current_voxel,
								      double    trylabel,
								      vtkDataArray* labels,
								      int incr[26],double wgth[26],
								      double smoothness)
{
  double pmrf  =0.0;

  if (smoothness>0.001)
    {
      for (int i=0;i<=25;i++)
	{
	  if ( fabs(trylabel-labels->GetComponent(current_voxel+incr[i],0))>0.0001)
	    pmrf+=wgth[i];
	}
    }

  pmrf=pmrf*smoothness;

  if (current_voxel==this->DebugVoxel)
    fprintf(stderr,"MRF Term = %lf",pmrf);

  
  if ( fabs(trylabel-intensity)>0.0001)
    {
      pmrf+=1.0;
      if (current_voxel==this->DebugVoxel)
	fprintf(stderr," + Intensity 1.0 = %lf\n",pmrf);
    }
  else if (current_voxel==this->DebugVoxel)
    fprintf(stderr," + Intensity 0.0 = %lf\n",pmrf);
  

  return pmrf;
}
// --------------------------------------------------------------------------------------------
//
//  This is a greedy optimization scheme in which the label for each voxel
//  is updated separately to maxmize the probablity of this voxel having class=label
//
// --------------------------------------------------------------------------------------------
int  vtkbisObjectmapRegularization::ClassifyVoxel(int voxel_index,vtkDataArray* intensities,vtkDataArray* labels,vtkDataArray* outlabels,
						  int incr[26],double wgth[26],
						  double smoothness,vtkIdList* nlist)
{

  int current_label=(int)(labels->GetComponent(voxel_index,0)+0.00001);
  double v=intensities->GetComponent(voxel_index,0);

  if (voxel_index==this->DebugVoxel)
    fprintf(stderr,"Beginning Classify Voxel voxel_index=%d, current_label=%d, intensity=%.0f\n",voxel_index,current_label,v);



  // We need a different loop here to identify numclasses ......
  nlist->SetNumberOfIds(0);
  nlist->InsertNextId(current_label);
  for (int ia=0;ia<=25;ia++)
    {
      int l=(int)(intensities->GetComponent(voxel_index+incr[ia],0)+0.00001);
      nlist->InsertUniqueId(l);
      //      if (this->DebugVoxel)
      //	fprintf(stderr,"Inserting Unique Id  ia=%d  %d --> %d\n",ia,l,nlist->GetNumberOfIds());
    }

  double bestprob=0.0;
  int    bestclass=current_label;
  int numclasses=nlist->GetNumberOfIds();

  if (voxel_index==this->DebugVoxel)
    fprintf(stderr,"NumClasses=%d\n",numclasses);

  if (numclasses>1)
    {
      for (int i=0;i< numclasses;i++)
	{
	  int cl=nlist->GetId(i);
	  double clp=double(cl);
	  double prob=this->ComputeTotalMinusLogProbability(v,voxel_index,clp,labels,incr,wgth,smoothness);
	  if ( (prob-bestprob)<0.000001 || i==0)
	    {
	      bestprob=prob;
	      bestclass=cl;
	    }
	  if (voxel_index==this->DebugVoxel)
	    fprintf(stderr,"Examining Class %d prob=%lf (bestprob=%lf bestclass=%d)\n",
		    cl,prob,bestprob,bestclass);
	}

      if (bestclass!=current_label)
	{
	  outlabels->SetComponent(voxel_index,0,(double)(bestclass+0.00001));
	  //	  fprintf(stderr,"Changing voxel =%d\n",voxel_index);
	  return 1;
	}
    }
  
  outlabels->SetComponent(voxel_index,0,(double)(current_label+0.0001));

  return 0;
}
// ----------------------------------------------------------------------------------------------------
//  High Level Routines for Maximization
// ----------------------------------------------------------------------------------------------------
// Computes the distances in (voxel index or raster index) between a voxel and its neighbors incr[26]
// The weights are proportional to the inverse of the distance between each voxel and its neighbors
//      these are constant if the image has isotropic resolution.
// --------------------------------------------------------------------------------------------------
int  vtkbisObjectmapRegularization::ComputeMRFIncrementsAndWeights(vtkImageData* img,int incr[26],double wgt[26])
{
  int dim[3]; img->GetDimensions(dim);
  double sp[3];img->GetSpacing(sp);

  int slicesize=dim[0]*dim[1];
  int index=0;
  double d[3];

  for (int ic=-1;ic<=1;ic++)
    {
      d[2]=pow(sp[2]*ic,2.0);
      for (int ib=-1;ib<=1;ib++)
	{
	  d[1]=pow(sp[1]*ib,2.0);
	  for (int ia=-1;ia<=1;ia++)
	    {
	      d[0]=pow(sp[0]*ia,2.0);
	      if (!(ia==0 && ib==0 && ic==0))
		{
		  incr[index]=ia+ib*dim[0]+ic*slicesize;
		  wgt[index]=1.0/sqrt(d[0]+d[1]+d[2]);

		  /*		  fprintf(stderr,"Ia=%d ib=%d ic=%d \tincr[index]=%d \twgt[index]=%.5f \td[0]=%.3f d[1]=%.3f d[2]=%.3f\n",
		    ia,ib,ic,incr[index],wgt[index],d[0],d[1],d[2]);*/
		  ++index;
		}
	    }
	}
    }

  //  fprintf(stderr,"Done setting, number=%d\n",index-1);

  double sum=0.0;
  for (int l=0;l<=25;l++)
    sum+=wgt[l];

  for (int k=0;k<=25;k++)
    {
      wgt[k]/=sum;
      //      fprintf(stderr,"Index=%d  incr=%d weight=%.3f\n",k,incr[k],wgt[k]);
    }
  
    
  return 1;
}
// ----------------------------------------------------------------------------
/*double vtkbisObjectmapRegularization::DoMaximizationStep(vtkImageData* intensity_image,
							 vtkImageData* label_image,
							 vtkImageData* output_label_image,
							 double smoothness,
							 int maxiter)
{

  //  fprintf(stderr,"\n ******* Beginning Maximimization Step\n ------------------------------\n");

  vtkDataArray* intensities=intensity_image->GetPointData()->GetScalars();
  vtkDataArray* labels=label_image->GetPointData()->GetScalars();
  vtkDataArray* out_labels=output_label_image->GetPointData()->GetScalars();
  
  double r1[2],r2[2];
  intensities->GetRange(r1);
  labels->GetRange(r2);
  //  fprintf(stderr,"Input Range = %.0f:%.0f , Label Range=%.0f:%.0f\n",r1[0],r1[1],r2[0],r2[1]);

  vtkIdList* nlist=vtkIdList::New();
  nlist->SetNumberOfIds(26);

  int nt=intensities->GetNumberOfTuples();
  int dim[3]; intensity_image->GetDimensions(dim);
  int incr[26]; double weights[26]; this->ComputeMRFIncrementsAndWeights(intensity_image,incr,weights);
  int done=0,iter=0;
  int tenth=nt/11;

  // Only Classify voxels that are not on the image boundary. 
  // Check for 2D Images here
  int mink=1,maxk=dim[2]-1;
  if (dim[2]==1)
    {
      mink=0;
      maxk=1;
    }

  if (smoothness<0.001)
    maxiter=1;

  int slicesize=dim[0]*dim[1];

  double total=0.0;
  double changed=0.0;
  int    count=0;
      
  fprintf(stderr,"*** MRF Regularization (%.2f) :",smoothness);
  for (int k=mink;k<maxk;k++)
    {
      for (int j=1;j<dim[1]-1;j++)
	{
	  int vox=k*slicesize+j*dim[0]+1;
	  for (int i=1;i<dim[0]-1;i++)
	    {
	      if (count==tenth)
		{
		  fprintf(stderr,".");
		  count=0;
		}
	      
	      changed+=this->ClassifyVoxel(vox,intensities,labels,out_labels,incr,weights,smoothness,nlist);
	      ++vox;
	      ++total;
	      ++count;
	    }
	}
    }
         
  changed=100.0*changed/total;
  fprintf(stderr,"changed=%.4f\n",changed);
  nlist->Delete();
  return changed;
  }*/
// ----------------------------------------------------------------------------
//
//     Execute Segmentation 
//
// ----------------------------------------------------------------------------
int vtkbisObjectmapRegularization::RequestInformation (vtkInformation * request,
						       vtkInformationVector ** inputVector,
						       vtkInformationVector * outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_SHORT, 1);
  return 1;
}

void vtkbisObjectmapRegularization::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if ( input==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs (one of them is NULL\n");
      return;
    }

  int np1=input->GetNumberOfPoints();
  double range[2]; input->GetPointData()->GetScalars()->GetRange(range);
 
  this->UpdateProgress(double(0.2)/double(this->NumberOfIterations+1));


  output->GetPointData()->GetScalars()->CopyComponent(0,input->GetPointData()->GetScalars(),0);
  //classification->GetPointData()->GetScalars()->FillComponent(0,0.0);

  vtkImageData* tmp=vtkImageData::New();
  tmp->CopyStructure(output);
  tmp->SetNumberOfScalarComponents(1);
  tmp->AllocateScalars();
  tmp->GetPointData()->GetScalars()->FillComponent(0,0);
  

  vtkIdList* nlist=vtkIdList::New();
  nlist->SetNumberOfIds(26);

  vtkDataArray* intensities=input->GetPointData()->GetScalars();
  int nt=intensities->GetNumberOfTuples();
  int dim[3]; input->GetDimensions(dim);
  int incr[26]; double weights[26]; this->ComputeMRFIncrementsAndWeights(input,incr,weights);
  int done=0,iter=0;
  int tenth=nt/11;

  // Only Classify voxels that are not on the image boundary. 
  // Check for 2D Images here
  int mink=1,maxk=dim[2]-1;
  if (dim[2]==1)
    {
      mink=0;
      maxk=1;
    }

  int slicesize=dim[0]*dim[1];
  int iterations=1;
  fprintf(stderr,"\n -----------------------------------\n Beginning Objectmap Regularization\n");
  while(iterations<=this->NumberOfIterations)
    {

      vtkDataArray* labels=tmp->GetPointData()->GetScalars();
      vtkDataArray* out_labels=output->GetPointData()->GetScalars();
  
      fprintf(stderr,"\n + + + + + +  Iteration %d (%.3f): ",iterations, this->Smoothness);
      labels->CopyComponent(0,out_labels,0);
      double changed=0.0;
      double total=0.0;
      int    count=0;
      
      for (int k=mink;k<maxk;k++)
	for (int j=1;j<dim[1]-1;j++)
	  {
	    int vox=k*slicesize+j*dim[0]+1;
	    for (int i=1;i<dim[0]-1;i++)
	      {
		if (count==tenth)
		  {
		    fprintf(stderr,".");
		    count=0;
		  }
		
		changed+=double(this->ClassifyVoxel(vox,intensities,labels,out_labels,incr,weights,this->Smoothness,nlist));
		++vox;
		++total;
		++count;
	      }
	  }
      changed=100.0*changed/total;
      fprintf(stderr,"changed=%.6f\n",changed);
      if (changed<this->ConvergencePercentage)
	iterations=this->NumberOfIterations+1;
      ++iterations;
    }


  nlist->Delete();
  tmp->Delete();
  this->UpdateProgress(1.0);
}




