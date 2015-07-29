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

#include "vtkdsSpectUtil.h"
#include "vtkObjectFactory.h"
#include <stdio.h>
#include "vtkImageData.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkpxMath.h"
//#include "vtkSortDataArray.h"

vtkStandardNewMacro(vtkdsSpectUtil);

vtkFloatArray* vtkdsSpectUtil::ClusterImage(vtkImageData* zscoreImage,vtkImageData* clusterId,vtkImageData* clusterSize,float En,float EM)
{	
  
  if (clusterId==NULL || zscoreImage==NULL || clusterSize==NULL)
    {
      fprintf(stderr,"BAD INPUTS IMAGES!!!!!!!\n"); 
      return NULL;
    }

  if (clusterId->GetNumberOfPoints()   != clusterSize->GetNumberOfPoints() ||
	  clusterId->GetNumberOfPoints()   != zscoreImage->GetNumberOfPoints() ||
	  zscoreImage->GetNumberOfPoints() != clusterSize->GetNumberOfPoints())
    {
      fprintf(stderr,"BAD INPUTS IMAGES they have different size !!!!!!!\n"); 
      return NULL;
    }

  double range[2] ;clusterId->GetPointData()->GetScalars()->GetRange(range);
  int numbersOfCluster=int(range[1]-range[0])+1;

  fprintf(stderr,"\n %i total number of cluster\n",numbersOfCluster);

  vtkFloatArray* clusterInfo=vtkFloatArray::New();
  
  // numvoxels clusterprob maxzscore x y z 
  clusterInfo->SetNumberOfComponents(7);//columns
  clusterInfo->SetNumberOfTuples(numbersOfCluster);//rows
  
  for (int i=0;i<6;i++)
  clusterInfo->FillComponent(i,0.0);

  int clustersAboveMin = 0;
  int dim[3]; clusterId->GetDimensions(dim);

  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	  {	
	    int id=(int)clusterId->GetScalarComponentAsDouble(i,j,k,0);
	    int kSize=(int)clusterSize->GetScalarComponentAsDouble(i,j,k,0);
	    double zscore=zscoreImage->GetScalarComponentAsDouble(i,j,k,0);
	    
	    if (clusterInfo->GetComponent(id-1,0) == 0 && fabs(zscore)>0)
	      {
		float clusterPvalue = exp(-pow((1.3378/En*kSize/(17.2/2*16.6/2*18.3/2)),(2.0/3.0)));//1.3293
		float correctPvalue = 1 - exp(-EM*clusterPvalue);
		clusterInfo->SetComponent(id-1,0,kSize);
		clusterInfo->SetComponent(id-1,1,clusterPvalue);
		clusterInfo->SetComponent(id-1,2,correctPvalue);
		clustersAboveMin++;
		//		fprintf(stderr,"\n %i %f %f\n",kSize,clusterPvalue,correctPvalue);
	      }
	    if (fabs(zscore)>fabs(clusterInfo->GetComponent(id-1,3)))
	      {
		clusterInfo->SetComponent(id-1,3,(float)zscore);
		clusterInfo->SetComponent(id-1,4,i);
		clusterInfo->SetComponent(id-1,5,j);
		clusterInfo->SetComponent(id-1,6,k);
	      }
	  }
  
  fprintf(stderr,"\n %i clusters above minimum threshold\n",clustersAboveMin);
  /*
    vtkSortDataArray sort=vtkSortDataArray::New();
    sort->ArrayByComponent(clusterInfo,0);
    sort->Delete():
  */
  vtkFloatArray* output=vtkFloatArray::New();
  output->SetNumberOfComponents(7);
  output->SetNumberOfTuples(clustersAboveMin);
  int tupleCount = 0;
  
  for (int index=0;index<numbersOfCluster;index++) 
    {
      if (clusterInfo->GetComponent(index,0) != 0)
	{
	  //fprintf(stderr,"\n");
	  for (int index2=0;index2<7;index2++) 
	    {
	      output->SetComponent(tupleCount,index2,clusterInfo->GetComponent(index,index2));
	      //fprintf(stderr,"%f\n",output->GetComponent(tupleCount,index2));
	    }
	  //fprintf(stderr,"\n");
	  tupleCount++;
	}
    }
  clusterInfo->Delete();
  return output;
}


vtkFloatArray* vtkdsSpectUtil::ClusterImage(vtkImageData* tscoreimage,vtkImageData* clusterId,vtkImageData* clusterSize,float En,float EM,float tscore,int ext)
{	

  if (clusterId==NULL || tscoreimage==NULL || clusterSize==NULL)
    {
      fprintf(stderr,"BAD INPUTS IMAGES!!!!!!!\n"); 
      return NULL;
    }
  
  if (clusterId->GetNumberOfPoints()   != clusterSize->GetNumberOfPoints() ||
	  clusterId->GetNumberOfPoints()   != tscoreimage->GetNumberOfPoints() ||
	  tscoreimage->GetNumberOfPoints() != clusterSize->GetNumberOfPoints())
    {
      fprintf(stderr,"BAD INPUTS IMAGES they have different size !!!!!!!\n"); 
      return NULL;
    }

  double range[2] ;clusterId->GetPointData()->GetScalars()->GetRange(range);
  int numbersOfCluster=int(range[1]-range[0])+1;

  fprintf(stderr,"\n %i total number of cluster\n",numbersOfCluster);

  vtkFloatArray* clusterInfo=vtkFloatArray::New();
  
  // numvoxels clusterprob maxzscore x y z 
  clusterInfo->SetNumberOfComponents(7);//columns
  clusterInfo->SetNumberOfTuples(numbersOfCluster);//rows
  
  for (int i=0;i<6;i++)
  clusterInfo->FillComponent(i,0.0);

  int clustersAboveMin = 0;
  int dim[3]; clusterId->GetDimensions(dim);

  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	  {	
	    int id=(int)clusterId->GetScalarComponentAsDouble(i,j,k,0);
	    int kSize=(int)clusterSize->GetScalarComponentAsDouble(i,j,k,0);
	    double tval=tscoreimage->GetScalarComponentAsDouble(i,j,k,0);
	    if (tval>=tscore && kSize>=ext && id>0)
	      {
		if (clusterInfo->GetComponent(id-1,0) == 0)
		  {
		    float clusterPvalue = exp(-pow((1.3378/En*kSize/(17.2/2*16.6/2*18.3/2)),(2.0/3.0)));//1.3293
		    float correctPvalue = 1 - exp(-EM*clusterPvalue);
		    clusterInfo->SetComponent(id-1,0,kSize);
		    clusterInfo->SetComponent(id-1,1,clusterPvalue);
		    clusterInfo->SetComponent(id-1,2,correctPvalue);
		    clustersAboveMin++;
		    fprintf(stderr,"\n %d %f %f\n",kSize,clusterPvalue,correctPvalue);
		  }
		if (tval>clusterInfo->GetComponent(id-1,3))
		  {
		    clusterInfo->SetComponent(id-1,3,tval);
		    clusterInfo->SetComponent(id-1,4,i);
		    clusterInfo->SetComponent(id-1,5,j);
		    clusterInfo->SetComponent(id-1,6,k);
		  }
	      }
	  }
  
  fprintf(stderr,"\n %i clusters above minimum threshold\n",clustersAboveMin);
  /*
  vtkSortDataArray sort=vtkSortDataArray::New();
  sort->ArrayByComponent(clusterInfo,0);
  sort->Delete():
  */
  vtkFloatArray* output=vtkFloatArray::New();
  output->SetNumberOfComponents(7);
  output->SetNumberOfTuples(clustersAboveMin);
  int tupleCount = 0;
  
  for (int index=0;index<numbersOfCluster;index++) 
    {
      if (clusterInfo->GetComponent(index,0) != 0)
	{
	  //fprintf(stderr,"\n");
	  for (int index2=0;index2<7;index2++) 
	    {
	      output->SetComponent(tupleCount,index2,clusterInfo->GetComponent(index,index2));
	      //fprintf(stderr,"%f\n",output->GetComponent(tupleCount,index2));
	    }
	  //fprintf(stderr,"\n");
	  tupleCount++;
	}
    }
  clusterInfo->Delete();
  return output;
}

float vtkdsSpectUtil::imageMean(vtkImageData* image,float min)
{
  vtkDataArray* in=image->GetPointData()->GetScalars();
  
  int numscalars=in->GetNumberOfTuples();
  
  float sum=0.0;
  float count=0;
  for (int pixel=0;pixel<numscalars;pixel++)
    {
      float v1=in->GetComponent(pixel,0);
      if (v1>min)
	{	
	  sum+=v1;
	  count++;
	}	
    }
  sum/=count;
  return sum;
}

void vtkdsSpectUtil::HAI(vtkImageData* image)
{
  if (image==NULL)
    {
      fprintf(stderr,"BAD INPUTS IMAGES!!!!!!!\n"); 
      return;
    }
  
  int volume(0);int leftHemi(0);
  int dim[3]; image->GetDimensions(dim);
  
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  if (fabs(image->GetScalarComponentAsDouble(i,j,k,0))>0 && i!=45)
	    {
	      volume++; 
	      if (i>45)
		leftHemi++;
	    }
	}
  float HAI=2.0*leftHemi/volume-1.0;
  fprintf(stderr,"leftHemi=%d,righthemi=%d",leftHemi,volume-leftHemi);
  fprintf(stderr,"\nHAI = %f\n", HAI); 
}

vtkFloatArray* vtkdsSpectUtil::reselCount(vtkImageData* image)
{
  if (image==NULL)
    {
      fprintf(stderr,"BAD INPUTS IMAGES!!!!!!!\n"); 
      return NULL;
    }
  
  int volume(0);int cubes(0);int xyFaces(0);int yzFaces(0);int xzFaces(0);int xEdges(0);int yEdges(0);int zEdges(0);
  int dim[3]; image->GetDimensions(dim);
  
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  
	  if (fabs(image->GetScalarComponentAsDouble(i,j,k,0))>0)
	    {
	      volume++; 
	      if (fabs(image->GetScalarComponentAsDouble(i+1,j,k,0))>0)
		{
		  xEdges++;
		  if (fabs(image->GetScalarComponentAsDouble(i,j+1,k,0))>0 && fabs(image->GetScalarComponentAsDouble(i+1,j+1,k,0))>0)
		    {
		      xyFaces++;
		      if (fabs(image->GetScalarComponentAsDouble(i,j,k+1,0))>0   && fabs(image->GetScalarComponentAsDouble(i+1,j,k+1,0))>0 &&
			  fabs(image->GetScalarComponentAsDouble(i,j+1,k+1,0))>0 && fabs(image->GetScalarComponentAsDouble(i+1,j+1,k+1,0))>0)
			{ 
			  cubes++;
			}
		    }
		}
	      if (fabs(image->GetScalarComponentAsDouble(i,j+1,k,0))>0)
		{
		  yEdges++;
		  if (fabs(image->GetScalarComponentAsDouble(i,j,k+1,0))>0 && fabs(image->GetScalarComponentAsDouble(i,j+1,k+1,0))>0)
		    {
		      yzFaces++;
		    }
		}
	      if (fabs(image->GetScalarComponentAsDouble(i,j,k+1,0))>0)
		{
		  zEdges++;
		  if (fabs(image->GetScalarComponentAsDouble(i+1,j,k,0))>0 && fabs(image->GetScalarComponentAsDouble(i+1,j,k+1,0))>0)
		    {
		      xzFaces++;
		    }
		}
	    }
	} 
  vtkFloatArray* output=vtkFloatArray::New();
  output->SetNumberOfComponents(8);
  output->SetNumberOfTuples(1);
  output->SetComponent(0,0,volume);
  output->SetComponent(0,1,cubes);
  output->SetComponent(0,2,xyFaces);
  output->SetComponent(0,3,yzFaces);
  output->SetComponent(0,4,xzFaces);
  output->SetComponent(0,5,xEdges);
  output->SetComponent(0,6,yEdges);
  output->SetComponent(0,7,zEdges);
  //for (int i=0;i<8;i++)
  //fprintf(stderr,"%f",output->GetComponent(0,i));
  return output;
}

double vtkdsSpectUtil::reselCount(vtkImageData* image, float eulerDensity1, float eulerDensity2, float eulerDensity3, float eulerDensity4)
{
  float eulerDensity[4];
  eulerDensity[0]=eulerDensity1;
  eulerDensity[1]=eulerDensity2;
  eulerDensity[2]=eulerDensity3;
  eulerDensity[3]=eulerDensity4;
  return reselCount(image,eulerDensity);
}

double vtkdsSpectUtil::reselCount(vtkImageData* image, float eulerDensity[4] )
{
  if (image==NULL)
    {
      fprintf(stderr,"BAD INPUTS IMAGES!!!!!!!\n"); 
      return 0.0;
    }
  
  float volume(0);float cubes(0);float xyFaces(0);float yzFaces(0);float xzFaces(0);float xEdges(0);float yEdges(0);float zEdges(0);
  int dim[3]; image->GetDimensions(dim);
  
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  if (fabs(image->GetScalarComponentAsDouble(i,j,k,0))>0)
	    {
	      volume++; 
	      if (fabs(image->GetScalarComponentAsDouble(i+1,j,k,0))>0)
		{
		  xEdges++;
		  if (fabs(image->GetScalarComponentAsDouble(i,j+1,k,0))>0 && fabs(image->GetScalarComponentAsDouble(i+1,j+1,k,0))>0)
		    {
		      xyFaces++;
		      if (fabs(image->GetScalarComponentAsDouble(i,j,k+1,0))>0   && fabs(image->GetScalarComponentAsDouble(i+1,j,k+1,0))>0 &&
			  fabs(image->GetScalarComponentAsDouble(i,j+1,k+1,0))>0 && fabs(image->GetScalarComponentAsDouble(i+1,j+1,k+1,0))>0)
			{ 
			  cubes++;
			}
		    }
		}
	      if (fabs(image->GetScalarComponentAsDouble(i,j+1,k,0))>0)
		{
		  yEdges++;
		  if (fabs(image->GetScalarComponentAsDouble(i,j,k+1,0))>0 && fabs(image->GetScalarComponentAsDouble(i,j+1,k+1,0))>0)
		    {
		      yzFaces++;
		    }
		}
	      if (fabs(image->GetScalarComponentAsDouble(i,j,k+1,0))>0)
		{
		  zEdges++;
		  if (fabs(image->GetScalarComponentAsDouble(i+1,j,k,0))>0 && fabs(image->GetScalarComponentAsDouble(i+1,j,k+1,0))>0)
		    {
		      xzFaces++;
		    }
		}
	    }
	} 

  float xDim=0.11627907; float yDim=0.120481928; float zDim=0.109289617;

  float reselCount[4];
  reselCount[0]=(volume-xEdges-yEdges-zEdges+xyFaces+xzFaces+yzFaces-cubes);
  reselCount[1]=(xEdges-xyFaces-xzFaces+cubes)*xDim+(yEdges-xyFaces-yzFaces+cubes)*yDim+(zEdges-yzFaces-xzFaces+cubes)*zDim;
  reselCount[2]=(xyFaces-cubes)*xDim*yDim+(xzFaces-cubes)*xDim*zDim+(yzFaces-cubes)*zDim*yDim;
  reselCount[3]=cubes*xDim*yDim*zDim;
  
  float expectedNumOfCluster=0.0;
  for (int kk=0;kk<4;kk++)  
    expectedNumOfCluster+=eulerDensity[kk]*reselCount[kk];

  return expectedNumOfCluster;
}


