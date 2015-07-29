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
#include "vtkxqImageKMeansVesselnessFilter.h"
#include "vtkImageReslice.h"
#include "vtkImageExtractComponents.h"
#include "vtkImageFFT.h"
#include "vtkImageRFFT.h"
//#include "vtkpxImageFloatFFT.h"
//#include "vtkpxImageFloatRFFT.h"
#include "vtkImageCast.h"
#include "vtkImageMagnitude.h"
#include "vtkImageMathematics.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkImageShiftScale.h"
#include "vtkMath.h"
#include "pxisinf.h"

//------------------------------------------------------------------------------
vtkxqImageKMeansVesselnessFilter* vtkxqImageKMeansVesselnessFilter::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkxqImageKMeansVesselnessFilter");
  if(ret)
    {
    return (vtkxqImageKMeansVesselnessFilter*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkxqImageKMeansVesselnessFilter;
}

// Construct object with no children.
vtkxqImageKMeansVesselnessFilter::vtkxqImageKMeansVesselnessFilter()
{
  this->Sigma=0.2;
  this->Beta=5.0;
  this->Alpha=1.0;
  this->ComputeAuxOutput=1;
  this->AuxOutput=NULL;
  this->SecondInput=NULL;
}
// ----------------------------------------------------------------------------
vtkxqImageKMeansVesselnessFilter::~vtkxqImageKMeansVesselnessFilter()
{
  this->SetSecondInput(NULL);
  if (this->AuxOutput!=NULL)
    this->AuxOutput->Delete();


}
// ----------------------------------------------------------------------------
void vtkxqImageKMeansVesselnessFilter::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData* input=this->GetInput();
  vtkImageData* output=this->GetOutput();

  if (input==NULL)
    {
      vtkErrorMacro(<<"No Input Set");
      return;
    }
  output->SetScalarTypeToFloat();
  output->SetNumberOfScalarComponents(1);
}
// -------------------------------------------------------------------------------------------------------------
void vtkxqImageKMeansVesselnessFilter::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Input to vtkxqImageKMeansVesselnessFilter");
      return;
    }
  
  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* inp2=NULL;

  if (this->SecondInput!=NULL)
    inp2=this->SecondInput->GetPointData()->GetScalars();

  if (this->AuxOutput!=NULL)
    {
      this->AuxOutput->Delete();
      this->AuxOutput=NULL;
    }


  vtkDataArray* auxout=NULL;
  if (this->ComputeAuxOutput)
    {
      this->AuxOutput=vtkImageData::New();
      this->AuxOutput->CopyStructure(input);
      this->AuxOutput->SetScalarTypeToFloat();
      this->AuxOutput->SetNumberOfScalarComponents(10);
      this->AuxOutput->AllocateScalars();
      auxout=this->AuxOutput->GetPointData()->GetScalars();
      for (int ia=0;ia<auxout->GetNumberOfComponents();ia++)
	auxout->FillComponent(ia,-1.0);
    }

  output->GetPointData()->GetScalars()->FillComponent(0,-1.0);
  this->ComputeMeasure(inp,inp2,out,auxout);
  this->UpdateProgress(1.0);
}
// -------------------------------------------------------------------------------------------------------------
int vtkxqImageKMeansVesselnessFilter::ComputeClustering(int np,double* data,double* data2,double mean[2],double sigma[2],double meanint[2],double sigmaint[2],int debug)
{
  // Inititalize
  double minv=data[0],maxv=data[0];
  for (int i=1;i<np;i++)
    {
      if (data[i]<minv)
	minv=data[i];
      else if (data[i]>maxv)
	maxv=data[i];
    }
	
  double threshold=0.5*(maxv+minv);


  if (debug)
    {
      fprintf(stderr,"\n\n -------------------------------------------------------------\n min=%f , max=%f\n",minv,maxv);
      for (int i=0;i<np;i++)
	{
	  fprintf(stderr,"Data Element %d \t %f",i,data[i]);
	  if (data2!=NULL)
	    fprintf(stderr,"\t %f",data2[i]);
	  fprintf(stderr,"\n");
	}
      fprintf(stderr,"Beginning Iteration %d\t min=%f \t max=%f \t thr=%f\n",0,minv,maxv,threshold);
    }

  double dthreshold=fabs(threshold*0.05);
  double lastchange=dthreshold+1.0;
  int    iter=0,maxiter=20;
  int num[2];

  while (iter<maxiter && lastchange > dthreshold)
    {
      for (int cl=0;cl<=1;cl++)
	{
	  num[cl]=0;
	  mean[cl]=0.0;
	}
      
      for (int i=0;i<np;i++)
	{
	  int cl=1;
	  if (data[i] < threshold)
	    cl=0;
	  num[cl]++;
	  mean[cl]+=data[i];
	}
      
      if (num[0]==0 && num[1]==0)  //?
	{
	  for (int ia=0;ia<=1;ia++)
	    {
	      mean[ia]=0.0; sigma[ia]=1.0;
	      meanint[ia]=0.0; sigmaint[ia]=1.0;
	    }
	  return 0;
	}
      
      for (int cl=0;cl<=1;cl++)
	mean[cl]/=double(num[cl]);

      double oldthr=threshold;
      threshold=0.5*(mean[0]+mean[1]);
      lastchange=fabs(threshold-oldthr);
      ++iter;

      if (debug)
	fprintf(stderr,"End Of Iteration %d\t mean[0]=%f \t mean[1]=%f \t threshold=%f oldthr=%f lastchange=%f, dthreshold=%f\n",iter,mean[0],mean[1],threshold,oldthr,lastchange,dthreshold);
    }

  

  // By now we are all converged so compute output sums
  for (int cl=0;cl<=1;cl++)
    {
      num[cl]=0;
      mean[cl]=0.0;
      sigma[cl]=0.0;
      meanint[cl]=0.0;
      sigmaint[cl]=0.0;
    }
    
  for (int i=0;i<np;i++)
    {
      int cl=1;
      if (data[i] < threshold)
	cl=0;
      num[cl]+=1;
      mean[cl]+=data[i];
      sigma[cl]+=pow(data[i],2.0);

      if (data2!=NULL)
	{
	  meanint[cl]+=data2[i];
	  sigmaint[cl]+=pow(data2[i],2.0);
	}
    }

  // Compute Sigma, Meanint and sigmaint
  for (int cl=0;cl<=1;cl++)
    {
      if (num[cl]!=0)
	{
	  mean[cl]/=double(num[cl]);
	  sigma[cl]/=double(num[cl]);
	  sigma[cl]=sqrt(sigma[cl]-(mean[cl]*mean[cl]));

	  if (debug)
	    {
	      fprintf(stderr, "c1: %d sigma-c1 %f\n", cl, sigma[cl]);
	      fprintf(stderr,"\t\t Class %d (deviation)\t num=%d\t mean=%f \t sigma=%f\n",cl,num[cl],mean[cl],sigma[cl]);
	    }
	  
	  if (data2!=NULL)
	    {
	      meanint[cl]/=double(num[cl]);
	      sigmaint[cl]/=double(num[cl]);
	      sigmaint[cl]=sqrt(sigmaint[cl]-meanint[cl]*meanint[cl]);
	      if (debug)
		fprintf(stderr,"\t\t Class %d (intensity)\t num=%d\t %f \t %f\n",cl,num[cl],meanint[cl],sigmaint[cl]);
	    }
	}
      else
	{
	  fprintf(stderr,"*");
	  mean[cl]=0.0;
	  sigma[cl]=0.0;
	  if (data2!=NULL)
	    {
	      meanint[cl]=0.0;
	      sigmaint[cl]=0.0;
	    }
	}
    }

  if (debug)
    fprintf(stderr," -------------------------------------------------------------\n\n");


  return num[0];
}
// -------------------------------------------------------------------------------------------------------------
void vtkxqImageKMeansVesselnessFilter::ComputeMeasure(vtkDataArray* inp,vtkDataArray* inp2,vtkDataArray* out,vtkDataArray* auxout)
{
  int nc=inp->GetNumberOfComponents();
  int nt=inp->GetNumberOfTuples();

  fprintf(stderr,"\t nc=%d  nt=%d Beta=%.2f \n",nc, nt, this->Beta);
  double* dt=new double[nc];
  double* dt2=NULL;
  double mean[2],sigma[2],meanint[2],sigmaint[2];

  if (inp2!=NULL)
    dt2=new double[nc];

  int tenth=nt/10;
  int count=0;

  double clustersigma2=2.0*pow((this->Sigma)*double(nc),2.0);  


  double range[2]; inp->GetRange(range);
  double threshold=range[0];//0.95*range[0]+0.05*range[1];

  fprintf(stderr,"Threshold=%.2f \n",threshold);




  for (int voxel=0;voxel<nt;voxel++)
    {
      int debug=0;
      if (voxel==16820)
	debug=1;

      double meanraw=0.0;
      double minraw=inp->GetComponent(voxel,0);
      for (int comp=0;comp<nc;comp++)
	{
	  double v=inp->GetComponent(voxel,comp);
	  meanraw+=v;
	  if (v<minraw)
	    minraw=v;
	}
      minraw=0.0;
      
      meanraw=meanraw/double(nc);
      if (meanraw>threshold)
	{
	  // Create Arrays (dt,dt2)
	  for (int comp=0;comp<nc;comp++)
	    {
	      dt[comp]=inp->GetComponent(voxel,comp);
	      if (inp2!=NULL)
		dt2[comp]=inp2->GetComponent(voxel,comp);
	    }
	  
	  // Do Clustering and extract values
	  int numc1=this->ComputeClustering(nc,dt,dt2,mean,sigma,meanint,sigmaint,debug);
	  int numc2=nc-numc1;
	  
	  mean[0]=-mean[0];
	  mean[1]=-mean[1];

	  double meandiff = fabs(mean[0]-mean[1]);
	  double x2=pow(double(numc1),2.0);

	  double sizeweight=exp(-x2/clustersigma2);
	  double vess= pow(meanint[0],this->Alpha)*sizeweight;
	  double vess2=pow(meanint[0],this->Alpha)*pow(double(numc2),this->Beta);
	  if (isnan(vess))
	    vess=-1.0;
	  if (isnan(vess2))
	    vess2=-1.0;

	  // Compute main value vesselness
	  out->SetComponent(voxel,0,vess2);

	  // Store aux values
	  if (auxout!=NULL)
	    {

	      if (isnan(mean[0]) || isnan(mean[1]) || isnan(meanint[0]) || isnan(meanint[1]))
		{
		  mean[0]=0.0;
		  mean[1]=0.0;
		  meanint[0]=0.0;
		  meanint[1]=0.0;
		}


	      // Cluster vesselness
	      // Frame 1
	      auxout->SetComponent(voxel,0,vess2);
	      
	      // Alternative vesselness
	      // Frame 2
	      auxout->SetComponent(voxel,1,vess);

	      // Numbers in each cluster
	      // Frames 3 & 4
	      auxout->SetComponent(voxel,2,numc1);
	      auxout->SetComponent(voxel,3,numc2);
	      
	      // Means of Vesslness in each cluster
	      // Frames 5 & 6
	      auxout->SetComponent(voxel,4,mean[0]);
	      auxout->SetComponent(voxel,5,mean[1]);

	      // Means of Intensity in each cluster
	      // Frames 7 & 8
	      auxout->SetComponent(voxel,6,meanint[0]);
	      auxout->SetComponent(voxel,7,meanint[1]);

	      // Components of final vesselness
	      // Frame 9
	      // Frame 10
	      auxout->SetComponent(voxel,8,pow(meanint[0],this->Alpha));
	      auxout->SetComponent(voxel,9,pow(double(numc2),this->Beta));
	    }
	  

	}

      ++count;
      if (count==tenth)
	{
	  this->UpdateProgress(this->GetProgress()+0.1);
	  count=0;
	}
    }

  if (dt2!=NULL)
    delete [] dt2;
  delete [] dt;

  if (auxout!=NULL)
    {
      for (int ia=0;ia<auxout->GetNumberOfComponents();ia++)
	{
	  if (ia!=3 && ia!=2)
	    {
	      double r[2]; auxout->GetRange(r,ia);
	      if (fabs(r[1]-r[0])<0.000001)
		r[1]+=0.01;
	      double shift=r[0];
	      double scale=100.0/(r[1]-r[0]);
	      fprintf(stderr,"ia=%d r=%f:%f shift=%f scale=%f\n",ia,r[0],r[1],shift,scale);
	      for (int voxel=0;voxel<nt;voxel++)
		{
		  double v=auxout->GetComponent(voxel,ia);
		  auxout->SetComponent(voxel,ia,scale*(v-shift));
		}
	    }
	}
      
      auxout->Modified();
      
      for (int ia=0;ia<auxout->GetNumberOfComponents();ia++)
	{
	  double r[2]; auxout->GetRange(r,ia);
	  fprintf(stderr,"Out Range ia=%d r=%f:%f \n",ia,r[0],r[1]);
	}
    }
}


