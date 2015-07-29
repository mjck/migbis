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
  Module:    $RCSfile: vtknrImageCluster.h,v $
  Language:  C++
  Date:      $Date: 2005/08/10 18:06:52 $
  Version:   $Revision: 1.1 $


Copyright (c) 2005 Nallakkandi Rajeevan, Ph.D.  n.rajeevan@yale.edu
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

// vtknrImageCluster is a class to extract all the clusters in an image and
// threshold clusters based on its size.  This class also provides cluster
// probability distribution function that can be used in the calculation of
// false positive probability in fMRI and PET image analysis.
//                                             -- Rajeevan  August 15, 2005
//

/******************************************************************************/

#include "vtknrImageCluster.h"
#include "vtkObjectFactory.h"
#include "vtkImageMask.h"
#include "vtkImageThreshold.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkImageDataGeometryFilter.h"
#include "vtkThresholdPoints.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkImageAccumulate.h"
#include "vtkIntArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkpxUtil.h"
#include <vector>
#include <stack>
using namespace std;

/******************************************************************************/

vtknrImageCluster* vtknrImageCluster::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtknrImageCluster");
  if(ret)
    {
    return (vtknrImageCluster*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtknrImageCluster;
}

/******************************************************************************/

vtknrImageCluster::vtknrImageCluster()
{
   this->MaxClusterSize       = 0;
   this->IntensityThreshold   = 0.0;
   this->ClusterNumberImage   = NULL;
   this->Clusters=NULL;
   this->ClusterHistogram=NULL;
   this->OneConnected=1;
   //  fprintf(stderr,"in vtknrImage Cluster :: Constructor\n");

}

/******************************************************************************/

vtknrImageCluster::~vtknrImageCluster()
{

  this->SetClusters(NULL);
  this->SetClusterHistogram(NULL);

  if (this->ClusterNumberImage!=NULL)
     this->ClusterNumberImage->Delete();
}

/******************************************************************************/
int vtknrImageCluster::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector, 
					  vtkInformationVector *outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, 1);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_INT, -1);
  return 1;
}

/******************************************************************************/
vtkImageData* vtknrImageCluster::ColorInClusterCentroids(vtkImageData* clustermap)
{
  if (clustermap==NULL)
    {
      fprintf(stderr,"Bad Input Clustermap\n");
      return NULL;
    }

  vtkDataArray* inp=clustermap->GetPointData()->GetScalars();
  double range[2]; inp->GetRange(range);
  if (range[0]!=0 || range[1]>100000)
    {
      fprintf(stderr,"Bad Range of input clustermap %f:%f\n",range[0],range[1]);
      return NULL;
    }

  vtkImageData* output=vtkImageData::New();
  output->CopyStructure(clustermap);
  output->SetScalarTypeToShort();
  output->AllocateScalars();
  output->GetPointData()->GetScalars()->FillComponent(0,0.0);

  double spa[3]; clustermap->GetSpacing(spa);
  double ori[3]; clustermap->GetOrigin(ori);
 
  vtkImageDataGeometryFilter* filt=vtkImageDataGeometryFilter::New();
  filt->SetInput(clustermap);
  filt->Update();

  vtkDataSet* dataset=(vtkDataSet*)filt->GetOutput();

  int maxrange=(int)range[1];

  for (int index=1;index<maxrange;index++)
    {
      float minv=float(index)-0.5;
      float maxv=minv+1.0;

      vtkThresholdPoints* thrPoints=vtkThresholdPoints::New();
      thrPoints->SetInput(filt->GetOutput());
      thrPoints->ThresholdBetween(minv,maxv);
      thrPoints->Update();
      vtkPoints* pts=thrPoints->GetOutput()->GetPoints();

      int np=pts->GetNumberOfPoints();
      if (np>0)
	{
	  double sum[3]={0.0,0.0,0.0};
	  for (int ia=0;ia<np;ia++)
	    {
	      double x[3]; pts->GetPoint(ia,x);
	      for (int ib=0;ib<=2;ib++)
		sum[ib]+=x[ib];
	    }
	  
	  int vox[3];
	  for (int ib=0;ib<=2;ib++)
	    vox[ib]= int( (sum[ib]/double(np)-ori[ib])/spa[ib]);

	  //	  fprintf(stderr,"Setting index=%d np=%d      (%d,%d,%d) to %f\n",index,np,
	  //		  vox[0],vox[1],vox[2],double(index));
	  output->SetScalarComponentFromDouble(vox[0],vox[1],vox[2],0,double(index));
		  
	}
      thrPoints->Delete();
    }
  filt->Delete();
  return output;

}

// ------------------------------------------------------------------------------------------
int vtknrImageCluster::RenumberClusterImage(vtkImageData* img,int sizethreshold)
{
  if (img==NULL)
    return 0;


  vtkDataArray* arr=img->GetPointData()->GetScalars();
  double r[2];   
  arr->GetRange(r,0);

  int maxn=int(r[1]);

  //  fprintf(stderr,"Renumbering Cluster Image maxn=%d\n",maxn);


  vtkImageAccumulate* accumulate=vtkImageAccumulate::New();
  accumulate->SetInput(img);
  accumulate->SetComponentExtent(0,maxn+1,0,0,0,0);
  accumulate->SetComponentOrigin(0,0,0);
  accumulate->SetComponentSpacing(1,1,1);
  accumulate->Update();

  vtkDataArray* data=accumulate->GetOutput()->GetPointData()->GetScalars();
  vtkIntArray*  cum=vtkIntArray::New();
  cum->SetNumberOfComponents(1);
  cum->SetNumberOfTuples(data->GetNumberOfTuples());
  cum->FillComponent(0,0.0);
  
  int numclusters=0;
  for (int i=1;i<data->GetNumberOfTuples();i++)
    {
      double v=data->GetComponent(i,0);
      //      fprintf(stderr,"Data i=%d v=%f\n",i,v);
      if (v>sizethreshold)
	{
	  ++numclusters;
	  cum->SetComponent(i,0,numclusters);
	  //	  fprintf(stderr,"Mapping %d to %d\n",i,numclusters);
	}
    }





  if (numclusters>0)
    {
      int nt=arr->GetNumberOfTuples();
      for (int i=0;i<nt;i++)
	{
	  double v=arr->GetComponent(i,0);
	  if (v>0)
	    {
	      int index=int(v);
	      double val=cum->GetComponent(0,index);
	      arr->SetComponent(i,0,val);
	    }
	}
    }

  cum->Delete();
  accumulate->Delete();

  return numclusters;
}
// -----------------------------------------------------------------------------------------

int vtknrImageCluster::ComputeClusters(vtkImageData* input,vtkImageData* output)
{
  this->Clusters->SetNumberOfTuples(0);
  this->Clusters->FillComponent(0,0.0);
  // Get increments to march through data 


   int dim[3];
   input->GetDimensions(dim);
   //fprintf(stderr,"Dimensions = %d,%d,%d\n",dim[0],dim[1],dim[2]);

   double sp[3];
   input->GetSpacing(sp);

   int shift[27][3];
   int maxshift=0;

   int index=0;
   int maxc=2;
   if (dim[2]==1)
     maxc=0;

   if (this->OneConnected==0)
     {
       for (int ic=0;ic<=maxc;ic++)
	 {
	   int k=ic;
	   if (k>1)
	     k=-1;
	   for (int ia=-1;ia<=1;ia++)
	     for (int ib=-1;ib<=1;ib++)
	       {
		 shift[index][0]=ia;
		 shift[index][1]=ib;
		 shift[index][2]=k;
		 ++index;
	       }
	 }
       maxshift=27;
     }
   else
     {
       for (int i=0;i<=6;i++)
	 for (int j=0;j<=2;j++)
	   shift[i][j]=0;
       
       shift[1][0]=-1;        shift[2][0]=1;
       shift[3][1]=-1;        shift[4][1]=1;
       shift[5][2]=-1;        shift[6][2]=1;
       maxshift=7;
     }

   
   int LimitSize  = dim[0]*dim[1]*dim[2];
   fprintf(stdout,"MaxShift=%d, OneConnected=%d LimitSize=%d, %d\n",maxshift,OneConnected,LimitSize);

   int tmp=0;
   this->Clusters->InsertNextTuple1(0);
   
   // Loop through ouput pixels
   int CP[4];
   int ClusterNumber=1;
   double VoxelVisited=-1.0;
   
   output->GetPointData()->GetScalars()->FillComponent(0,0.0);
   

  
   double voxelValue=0.0;
   vtkDataArray* inparray=input->GetPointData()->GetScalars();
   vtkDataArray* outarray=output->GetPointData()->GetScalars();
   int slicesize=dim[1]*dim[0];
   
   int voxelindex=-1;

   for (int idxZ = 0; idxZ < dim[2]; idxZ++)
     for (int idxY = 0; idxY < dim[1]; idxY++)
       for (int idxX = 0; idxX < dim[0]; idxX++)
	 {
	   ++voxelindex;
	   int nClusterVoxels=0;
	   
	   // Only Use First Component
	   double value = inparray->GetComponent(voxelindex,0);
	   double otval = outarray->GetComponent(voxelindex,0);
	   double voxelsign=1.0;
	   if (value<0.0)
	     voxelsign=-1.0;
	   value=fabs(value);
	   
	   if(value >= this->IntensityThreshold && otval == 0.0 )
	     {
	       std::stack<std::vector<int> > voxelStack;
	       //	       fprintf(stderr,"Beginning to Cluster (%d,%d,%d)\n",idxX,idxY,idxZ);
	       outarray->SetComponent(voxelindex,0,double(ClusterNumber));
	       std::vector<int> voxel;
	       voxel.push_back(0);
	       voxel.push_back(idxX);
	       voxel.push_back(idxY);
	       voxel.push_back(idxZ);
	       voxelStack.push(voxel);

	       //	       fprintf(stderr,"\t\t\t Adding %d %d %d to stack\n",idxX,idxY,idxZ);
	       //output->SetScalarComponentFromDouble(idxX,idxY,idxZ,0, double(ClusterNumber));
	       if(nClusterVoxels < LimitSize - 1 )
		 nClusterVoxels++;
	       
	       // ----------------------------------------------------------------------------------------
	       while(!voxelStack.empty())
		 {
		   //		   fprintf(stderr,"\t\t Beginning Recursion\n");
		   CP[0] = voxelStack.top()[0];
		   CP[1] = voxelStack.top()[1];
		   CP[2] = voxelStack.top()[2];
		   CP[3] = voxelStack.top()[3];
		   voxelStack.pop();

		   for (int nb=1;nb<maxshift;nb++)
		     {
		       int i1=CP[1]+shift[nb][0];
		       int i2=CP[2]+shift[nb][1];
		       int i3=CP[3]+shift[nb][2];
		 
		       int inindex=i3*slicesize+i2*dim[0]+i1;
      
		       if (i1>=0 && i1<dim[0] && i2>=0 && i2<dim[1] && i3>=0 && i3<dim[2])
			 {
			   double ot=outarray->GetComponent(inindex,0);
			   if ( fabs(ot) < 0.00001) 
			     {
			       double it=voxelsign*inparray->GetComponent(inindex,0);
			       if (it >= this->IntensityThreshold)
				 {
				   outarray->SetComponent(inindex,0,double (ClusterNumber));
				   std::vector<int> voxel;                       
				   voxel.push_back(0);
				   voxel.push_back(i1);
				   voxel.push_back(i2);
				   voxel.push_back(i3);
				   voxelStack.push(voxel);
				   
				   if(nClusterVoxels < LimitSize - 1)
				     nClusterVoxels++;
				 }
			       else
				 {
				   outarray->SetComponent(inindex,0,(VoxelVisited));
				 }
			     }
			 }
		     } // End of Inner Loop
		 }
	       if(nClusterVoxels > 0)
		 {
		   this->Clusters->InsertNextTuple1(nClusterVoxels);
		   ClusterNumber++;
		 }
	     }
	   else if (otval==0.0)
	     output->SetScalarComponentFromDouble(idxX,idxY,idxZ,0,(VoxelVisited));
	 }
   fprintf(stdout,"Max Clusters=%d\n",ClusterNumber);
   
   //vtkDataArray* out=output->GetPointData()->GetScalars();
   return 1;
}


/******************************************************************************/
int vtknrImageCluster::IntegrateHistogram()
{
  double range[2]; this->Clusters->GetRange(range);

  int nt=this->ClusterHistogram->GetNumberOfTuples();
  int nnt=int(range[1])+1;
  if (nt < nnt)
    this->ClusterHistogram->SetNumberOfTuples(nnt);
  this->ClusterHistogram->FillComponent(0,0.0);

  int nclust=this->Clusters->GetNumberOfTuples();
  for (int i=0;i<nclust;i++)
    {
      int v=(int)(this->Clusters->GetComponent(i,0));
      //      if (i<10)
      //      fprintf(stderr,"Cluster = %d vol=%d\n",i,v);
      this->ClusterHistogram->SetComponent(v,0,this->ClusterHistogram->GetComponent(v,0)+1);
    }

  return nnt;
}

int vtknrImageCluster::ComputeOutputImage(vtkImageData* input,vtkImageData* output)
{
  vtkDataArray* inp=input->GetPointData()->GetScalars(); 
  vtkDataArray* out=output->GetPointData()->GetScalars(); 

  int nt=inp->GetNumberOfTuples();
  int nv=out->GetNumberOfTuples();

  //  fprintf(stderr,"nt=%d, nv=%d\n",nt,nv);

  out->FillComponent(0,0.0);
  for (int vox=0;vox<nt;vox++)
    {
      int val=(int)inp->GetComponent(vox,0);
      if (val>=1)
	{
	  double vol=(this->Clusters->GetComponent(val,0));
	  out->SetComponent(vox,0,vol);
	}
    }
  return 1;
}
/******************************************************************************/
void vtknrImageCluster::SimpleExecute(vtkImageData *inData, vtkImageData *outData)
{
  //  fprintf(stderr,"This Far\n\n\n\n\n");
  if (inData==NULL)
    {
      fprintf(stderr,"No Input Specified\n");
      return;
    }

  //  fprintf(stderr,"This Far\n");

  if (this->Clusters==NULL)
    {
      vtkIntArray* cl=vtkIntArray::New();
      cl->SetNumberOfComponents(1);
      cl->Allocate(100,100);
      this->SetClusters(cl);
      cl->Delete();
    }

  if (this->ClusterHistogram==NULL)
    {
      vtkIntArray* cl=vtkIntArray::New();
      cl->SetNumberOfComponents(1);
      cl->Allocate(100,100);
      this->SetClusterHistogram(cl);
      cl->Delete();
    }

  if (this->ClusterNumberImage!=NULL)
    this->ClusterNumberImage->Delete();

  this->ClusterNumberImage=vtkImageData::New();
  this->ClusterNumberImage->CopyStructure(outData);
  this->ClusterNumberImage->AllocateScalars();
  
  //  fprintf(stderr,"\n\nClusterRadius=%.2f\n\n\n",this->ClusterRadius);
  this->ComputeClusters(inData,this->ClusterNumberImage);

  //  vtkpxUtil::SaveAnalyze("00cnumber.hdr",this->ClusterNumberImage,9);

  this->IntegrateHistogram();
  this->ComputeOutputImage(this->ClusterNumberImage,outData);



}


