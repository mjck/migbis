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

/*
 * vtkbisConnectivityUtility.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: Isabella Murphy
 *      collection of static utility methods
 *      trunk3
 */
#include <vtkbisConnectivityUtility.h>
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkbisImageTimeSeriesNormalize.h"
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <math.h>
#include <float.h>
#include "vtkbisMedianTemporalSmoothing.h"
#include "vtkpxImageRobustHistogram.h"
#include "pxisinf.h"

using namespace std;


// ---------------------------

vtkbisConnectivityUtility* vtkbisConnectivityUtility::New()
{
	// First try to create the object from the vtkObjectFactory
	vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisConnectivityUtility");
	if(ret)
    {
		return (vtkbisConnectivityUtility*)ret;
    }
	// If the factory was unable to create the object, then create it here.
	return new vtkbisConnectivityUtility;
}

double vtkbisConnectivityUtility::ComputeRatio(vtkImageData* input, vtkImageData * mask, double threshold)
{
	int dim[3];input->GetDimensions(dim);
	int dimMask[3];
	int tsum = 0;
	if (mask!=NULL)
	{
		mask->GetDimensions(dimMask);
		for (int ia=0;ia<=2;ia++)
				tsum += abs(dim[ia]-dimMask[ia]);
		if (tsum>0)
		{
			fprintf(stderr,"Bad Mask Input to vtkbisIpsilateralConnMap SimpleExecute\n");
			return -1;
		}
	}

	vtkDataArray* temp=input->GetPointData()->GetScalars();
	int nt=temp->GetNumberOfTuples();//number of voxels
	int nc=temp->GetNumberOfComponents();//number of frames (time series)

	vtkDataArray* msk = NULL;
	if (mask!=NULL)
		msk=mask->GetPointData()->GetScalars();

	double position[3],spacing[3];
	int lcounter=0;int rcounter=0;
	input->GetSpacing(spacing);
	//sort voxel to left or right brain vector
	for(int t= 0; t < nt; t++)
	{

		int DoGlm=1;
		if (mask!=NULL)
		{
			if(msk->GetComponent(t,0) < 1)
					DoGlm=0;
		}
		if(DoGlm)
		{
			input->GetPoint(t,position);
			if( position[0]/spacing[0] > (dim[0]-1)/2 )
			{
				//std::cout<<"position:"<<position[0]<<"spacing:"<<spacing[0]<<endl;
				//left brain
				if(temp->GetComponent(t,0) > threshold)
					++lcounter;
				//std::cout<<"t:"<<t<<"lv:"<<temp->GetComponent(t,0)<<"lcounter:"<<lcounter<<endl;
			}
			else
			{
				//right brain
				//std::cout<<"position:"<<position[0]<<"spacing:"<<spacing[0]<<endl;
				if(temp->GetComponent(t,0) > threshold)
					++rcounter;
				//std::cout<<"t:"<<t<<"rv:"<<temp->GetComponent(t,0)<<"rcounter:"<<rcounter<<endl;
			}
		}
	}
	if (lcounter+rcounter > 0.000001)
	{
		std::cout<<"lcounter:"<<lcounter<<"rcounter"<<rcounter<<"ratio:"<<(double)(lcounter-rcounter)/(lcounter+rcounter)*100<<endl;
		return (double)(lcounter-rcounter)/(lcounter+rcounter)*100;
	}
	else
	{
		fprintf(stderr,"No voxel within threshold\n");
		return -1;
	}
}

void vtkbisConnectivityUtility::ImagetoText(vtkImageData* input, const char* filename)
{
	vtkDataArray*  inp=input->GetPointData()->GetScalars();
	int nt = inp->GetNumberOfTuples();
	int nc = inp->GetNumberOfComponents();

	if(nt > 400 || nc > 500)
	{
		fprintf(stderr, "Too much data to export.");
		return;
	}
	std::ofstream myfile(filename);//output file stream
	if(!myfile.is_open())
	{
		cout<< "Unable to open file"<<filename<<"/n";
		return;
	}
	//loop through the output image file
	int counter=0;
	for (int ia = 0; ia<nc; ia++)
	{
		for (int ib=0;ib<nt;ib++)
		{
			myfile<<inp->GetComponent(counter,0)<< "\t";
	    	++counter;
	    }
	    myfile<<"\n";
	}
	myfile.close();
}
void vtkbisConnectivityUtility::TexttoImage(const char* filename,vtkImageData* out)
{
	std::ifstream infile(filename, ios::in);

	if(!infile.is_open())
	{
		cerr<< "Unable to open file"<<filename<<"\n";
		return;
	}

	std::vector<float> data;
	float value=0.0;

	while (!infile.eof())
	{
		infile>>value;
		if(!infile.eof())
			data.push_back(value);
	}
	infile.close();
	int size = data.size();

	if( (size)%2 > 0 )
	{
		cerr<<"not a matrix, error.\n";
		return;
	}
	int dim = (int)sqrt((float)size);
	out->SetOrigin(0,0,0);
	out->SetSpacing(1,1,1);
	out->SetDimensions(dim,dim,1);
	out->SetScalarTypeToFloat();
	out->AllocateScalars();
	vtkDataArray* output = out->GetPointData()->GetScalars();
	output->FillComponent(0,0.0);

	for (int i=0;i<size;i++)
	{
		output->SetComponent(i,0,data[i]);
	}

}

int vtkbisConnectivityUtility::ExtractImageBlocks(vtkImageData* input, vtkImageData* result, vtkIntArray* frames)
{

  if (input==NULL || result==NULL  || frames==NULL)
    {
      fprintf(stderr,"Bad Inputs to Extract Image Blocks\n");
      return 0;
    }

  vtkDataArray* temp = input->GetPointData()->GetScalars();
  int nframes=temp->GetNumberOfComponents();

  vtkIntArray* markers=vtkIntArray::New();
  markers->SetNumberOfComponents(2);

  //  fprintf(stderr,"\n\n");

  //output
  int arraysize = frames->GetNumberOfTuples();
  int totalframe = 0;
  for (int i=0; i<arraysize;i++)
  {
      int startframe= (int) frames->GetComponent(i,0);
      int endframe  = (int) frames->GetComponent(i,1);
      if (startframe<=endframe && endframe>=0)
      {
    	  totalframe+=(endframe-startframe)+1;
    	  double p[2];
    	  p[0]=startframe;
    	  p[1]=endframe;
    	  markers->InsertNextTuple2(p[0],p[1]);
      }
  }

  result->CopyStructure(input);
  result->SetScalarType(input->GetScalarType());
  result->SetNumberOfScalarComponents(totalframe);
  result->AllocateScalars();

  //  fprintf(stderr,"Data Type inp=%s, out=%s\n",input->GetScalarTypeAsString(), result->GetScalarTypeAsString());

  vtkDataArray* out=result->GetPointData()->GetScalars();
  for(int ia=0;ia<totalframe;ia++)
    out->FillComponent(ia,0);

  //annealing blocks together
  int comp=0;
  arraysize=markers->GetNumberOfTuples();

  //  fprintf(stderr,"\n\n");

  for (int j=0;j<arraysize;j++)
  {
      int bframe=(int)markers->GetComponent(j,0);
      int eframe=(int)markers->GetComponent(j,1);
      if (bframe>=0 && bframe<nframes && eframe>=0 && eframe<nframes)
      {
    	  fprintf(stdout,"Using Block %d:%d \n", bframe+1,eframe+1,nframes);
    	  for(int k=bframe;k<=eframe;k++)
	    {
	      out->CopyComponent(comp,temp,k);
	      ++comp;
	    }
      }
      else
	{
    	  fprintf(stderr,"Block %d:%d is not valid, number of frames=%d\n",
		  bframe+1,eframe+1,nframes);
	}
  }

  markers->Delete();
  return totalframe;
}

// ---------------------------------------------------------------------------------------------------------
int vtkbisConnectivityUtility::DiscardImageBlocks(vtkImageData* input, vtkImageData* result, vtkIntArray* frames)
{

  if (input==NULL || result==NULL  || frames==NULL)
    {
      fprintf(stderr,"Bad Inputs to Discard Image Blocks\n");
      return 0;
    }

  vtkDataArray* temp = input->GetPointData()->GetScalars();
  int nframes=temp->GetNumberOfComponents();

  vtkIntArray* markers=vtkIntArray::New();
  markers->SetNumberOfComponents(2);

  //  fprintf(stderr,"\n\n");

  //output
  int arraysize = frames->GetNumberOfTuples();
  int totalframe = 0, startframe=0,endframe=0;
  for (int i=0; i<=arraysize;i++)
  {
      if (i<arraysize)
    	  endframe   = (int) frames->GetComponent(i,0);
      else
    	  endframe=nframes;

      endframe=endframe-1;

       //     fprintf(stderr,"i=%d startframe=%d, endframe=%d\n",i,startframe,endframe);

      if (startframe<=endframe && endframe>=0)
      {
    	  totalframe+=(endframe-startframe)+1;
    	  double p[2];
    	  p[0]=startframe;
    	  p[1]=endframe;
    	  markers->InsertNextTuple2(p[0],p[1]);
    	//  	  fprintf(stderr,"Creating Pair %.0f:%.0f \t\t totalframe=%d\n",p[0],p[1],totalframe);
      }

      if (i<arraysize)
    	  startframe = (int) frames->GetComponent(i,1)+1;

      //      fprintf(stderr,"done i=%d startframe=%d, endframe=%d\n",i,startframe,endframe);
  }

  result->CopyStructure(input);
  result->SetScalarType(input->GetScalarType());
  result->SetNumberOfScalarComponents(totalframe);
  result->AllocateScalars();

  //  fprintf(stderr,"Data Type inp=%s, out=%s\n",input->GetScalarTypeAsString(), result->GetScalarTypeAsString());

  vtkDataArray* out=result->GetPointData()->GetScalars();
  for(int ia=0;ia<totalframe;ia++)
    out->FillComponent(ia,0);

  //annealing blocks together
  int comp=0;
  arraysize=markers->GetNumberOfTuples();

  //  fprintf(stderr,"\n\n");

  for (int j=0;j<arraysize;j++)
  {
      int bframe=(int)markers->GetComponent(j,0);
      int eframe=(int)markers->GetComponent(j,1);
      if (bframe>=0 && bframe<nframes && eframe>=0 && eframe<nframes)
      {
    	  fprintf(stdout,"Using Block %d:%d \n", bframe+1,eframe+1,nframes);
    	  for(int k=bframe;k<=eframe;k++)
    	  {
    		  out->CopyComponent(comp,temp,k);
    		  ++comp;
    	  }
      }
      else
      {
    	  fprintf(stderr,"Block %d:%d is not valid, number of frames=%d\n",
    			  bframe+1,eframe+1,nframes);
      }
  }

  markers->Delete();
  return totalframe;
}
/*!---------------------------------------------------------------------------------------------------------------------------
*  Histogram fitting from Michele
*@param[in]zmap: input 3D image data type has to be z number (after calculating correlation, do fisher r-to-z transform to obtain this z number)
*@param[out]output:output image
*/
// ---------------------------------------------------------------------------------------------------------

void vtkbisConnectivityUtility::Ztransform(vtkImageData* zmap, vtkImageData* output)
{

	vtkDataArray* inp2=zmap->GetPointData()->GetScalars();
	int nt2=inp2->GetNumberOfTuples();//number of voxels
	int nc2=inp2->GetNumberOfComponents();

	output->SetDimensions(zmap->GetDimensions());
	output->SetSpacing(zmap->GetSpacing());
	output->SetOrigin(zmap->GetOrigin());
	output->SetNumberOfScalarComponents(zmap->GetNumberOfScalarComponents());
	output->SetScalarTypeToDouble();
	output->AllocateScalars();//allocation space on memery
	for (int ia=0;ia<nc2;ia++)
		output->GetPointData()->GetScalars()->FillComponent(ia,0.0);
	vtkDataArray* out=output->GetPointData()->GetScalars();


	vtkbisMedianTemporalSmoothing* smooth=vtkbisMedianTemporalSmoothing::New();
	vector<float> mean;
	vector<float> sigma;



	for(int i = 0; i < nc2;i++)
	{
		vtkpxImageRobustHistogram* histo = vtkpxImageRobustHistogram::New();
		histo->SetInput(zmap);
		histo->SetPlainHistogram();
		histo->SetFrame(i);
		histo->SetNumberOfBins(1024);
		histo->Update();
		/*if(i==0)
			histo->SaveGnuplot("test0.txt");
		else if(i==1)
			histo->SaveGnuplot("test1.txt");
		else if(i==2)
			histo->SaveGnuplot("test2.txt");
		else if(i==3)
			histo->SaveGnuplot("test3.txt");*/

		smooth->SetRadius(3);
		smooth->SetInput(histo->GetOutput());
		smooth->Update();
		histo->Delete();

		vtkDataArray* inp=smooth->GetOutput()->GetPointData()->GetScalars();
		int nt=inp->GetNumberOfTuples();//number of voxels
		//for(int his=0;his<nt;his++)
			//std::cout<<inp->GetComponent(his,0)<<"|";
		double range[2];
		inp->GetRange(range);
		int max = (int)(range[1]);
		float half = float(max)*0.5;
		int maxbucket =0;
		int leftbucket = 0;
		int j=0;int k=0;
		for(int l=0; l<nt;l++)
		{
			if(inp->GetComponent(l,0) == max)
			{
				maxbucket = l;
				//std::cout<<"# in maxbucket:"<<inp->GetComponent(l,0)<<endl;
			}

		}
		for(j=0;j<maxbucket;j++)
		{
			if(inp->GetComponent(j,0) <= half)
			{
				leftbucket = j;
			}
		}
		int rightbucket = maxbucket+1;
		for (k = maxbucket+1;k<nt;k++)
		{
			if(inp->GetComponent(k,0) >= half)
				rightbucket = k;
		}
		//compute sigma
		double  sp[3]; smooth->GetOutput()->GetSpacing(sp);
		double ori[3]; smooth->GetOutput()->GetOrigin(ori);
		double x10=double(leftbucket)*sp[0]+ori[0];
		double x11=double(leftbucket+1.0)*sp[0]+ori[0];
		double x20=double(rightbucket)*sp[0]+ori[0];
		double x21=double(rightbucket+1.0)*sp[0]+ori[0];
		double x1 = (x11-x10)*0.5+x10;
		double x2 = (x21-x20)*0.5+x20;
		float diff = fabs(float(x2-x1));
		float dsigma = diff/(2.0*sqrt(2.0*log(2.0)));
		if (dsigma<0.00001)
				dsigma=1.0;
		if (isnan(dsigma) || isinf(dsigma))
				dsigma=1.0;
		sigma.push_back(dsigma);
		//std::cout<<"difference:"<<diff<<"denominator:"<<2*sqrt(2*log(2))<<"sigma1:"<<sigma<<endl;
		//compute mean
		float sum = 0;
		int counter = 0;

		for(int m =0; m<nt2;m++)
		{
			if(inp2->GetComponent(m,0) > x1 && inp2->GetComponent(m,0) < x2)
			{
				sum+=inp2->GetComponent(m,0);
				//std::cout<<";"<<inp2->GetComponent(l,0);
				++counter;
			}
		}
		if(counter < 1)
			counter = 1;
		float dmean = sum/counter;
		if (isnan(dmean) || isinf(dmean))
				dmean=0.0;
		mean.push_back(dmean);
		//do z-mean/sigma on every value in zmap


		std::cout<<"max:"<<max<<"maxbucket:"<<maxbucket<<"leftbucket:"<<leftbucket<<"rightbucket:"<<rightbucket<<"x1:"<<x1<<"x2:"<<x2<<"sigma:"<<dsigma<<"mean:"<<dmean<<endl;

	}



	for(int m=0;m<nt2;m++)
	{
		for(int n=0;n<nc2;n++)
			out->SetComponent(m,n,(inp2->GetComponent(m,n)-mean[n])/sigma[n]);
	}

	smooth->Delete();

}
/*!---------------------------------------------------------------------------------------------------------------------------
*  Text file to matrix. This is written to read fmri motion file.
*@param[in]filename: input text file with a 2D matrix with 6 columns
*@param[out]output:output vtkpxMatrix object
*/
// ---------------------------------------------------------------------------------------------------------
int vtkbisConnectivityUtility::TexttoMarix6Cols(const char* filename, vtkpxMatrix* outmatrix)
{
	const int cols=6;

	std::ifstream infile(filename, ios::in);

	if(!infile.is_open())
	{
		cerr<< "Unable to open file"<<filename<<"\n";
		return 0;
	}

	std::vector<float> data;
	float value=0.0;

	while (!infile.eof())
	{
		infile>>value;
		if(!infile.eof())
			data.push_back(value);
	}
	infile.close();
	int size = data.size();

	if( (size)%cols > 0 )
	{
		cerr<<"not a matrix, error.\n";
		return 0;
	}
	int rows=0;
	rows = size/6;

	outmatrix->Zeros(rows,cols);
	int i=0;
	for(int r=0;r<rows;r++)
	{
		for(int c=0;c<cols;c++)
		{
			outmatrix->SetDirectElement(r,c,data[i]);
			++i;
		}
	}
	//outmatrix->Print();
	return rows;


}
void vtkbisConnectivityUtility::ROCCurve(vtkImageData* mask, vtkImageData* groundtruth, vtkImageData* segmentedimage, int steps, vtkDoubleArray* output, const char* Filename)
{
	if (groundtruth == NULL)
	{
		cout<<"bad input to ROC Curve, no ground truth image."<<endl;
		return;
	}
	if (segmentedimage == NULL)
	{
		cout<<"bad input to ROC Curve, no segmented image."<<endl;
	    return;
	}
	int dim1[3]; groundtruth->GetDimensions(dim1);
	int dim2[3]; segmentedimage->GetDimensions(dim2);
	int sum=0;
	for (int ia=0;ia<=2;ia++)
		sum+=abs(dim1[ia]-dim2[ia]);
	if (sum>0)
	{
		fprintf(stderr,"Error: ground truth and segmented image do not have the same dimension. \n");
		return;
	}
	if (mask!=NULL)
	{
		int dim3[3]; mask->GetDimensions(dim3);
	    int sum=0;
	    for (int ia=0;ia<=2;ia++)
	    	sum+=abs(dim3[ia]-dim2[ia]);
	    if (sum>0)
		{
	    	fprintf(stderr,"Bad Mask Input to ROC Curve \n");
		    return;
		}
	}

	vtkDataArray* msk=NULL;
	if (mask!=NULL)
	msk=mask->GetPointData()->GetScalars();
	vtkDataArray* grd=groundtruth->GetPointData()->GetScalars();
	vtkDataArray* seg=segmentedimage->GetPointData()->GetScalars();
	int nt=seg->GetNumberOfTuples();//number of voxels
	output->SetNumberOfComponents(3);
	output->SetNumberOfTuples(steps+1);


	double range[2];
	seg->GetRange(range);
	double min=range[0];
	double max=range[1];
	double addvalue=(max-min)/steps;
	std::cout<<"region min:"<<range[0]<<"region max:"<<range[1]<<endl;

	int step=0;
	for(double t=min;t<=max;t=t+addvalue)
	{
		int TP=0;
		int FP=0;
		int TN=0;
		int FN=0;
		for(int i=0;i<nt;i++)
		{
			int doAlg=1;
			if (msk!=NULL)
			{
				if (msk->GetComponent(i,0)<1)
					doAlg=0;
			}
			if (doAlg)
			{
				if(seg->GetComponent(i,0)>=t)
				{
					if(grd->GetComponent(i,0)>0)
					{
						++TP;
					}else{
						++FP;
					}
				}else{
					if(grd->GetComponent(i,0)>0)
					{
						++FN;
					}else{
						++TN;
					}
				}
			}
		}
		double sensitivity = (double)TP/(TP+FN);
		if (TP<0.00001 || (TP+FN)<1)
			sensitivity=0.0;

		double oneminusspecificity = (double)FP/(TN+FP);
		if (FP<0.00001 || (TN+FP)<1)
			oneminusspecificity=0.0;
		output->SetComponent(step,0,t);
		output->SetComponent(step,1,sensitivity);
		output->SetComponent(step,2,oneminusspecificity);
		cout<<"TP:"<<TP<<"TN"<<TN<<"FP:"<<FP<<"FN:"<<FN<<"Sen:"<<sensitivity<<"1-Spc:"<<oneminusspecificity<<"\n";
		++step;

	}
	/*for(int i=0;i<steps;i++)
		{
		cout<<output->GetComponent(i,0)<<"\t"<<output->GetComponent(i,1)<<"\t"<<output->GetComponent(i,2)<<"\n";

		}*/
	std::ofstream myfile(Filename);

	if(!myfile.is_open())
	{
	     cout<< "Unable to open file"<<Filename<<"/n";
	     return;
	}
	myfile<<"Step \t TP \t FP \n ";
	for(int i=0;i<steps;i++)
	{

		myfile<<output->GetComponent(i,0)<<"\t"<<output->GetComponent(i,1)<<"\t"<<output->GetComponent(i,2)<<"\n";
	}

	myfile.close();
}
/*!---------------------------------------------------------------------------------------------------------------------------
*  Take matrix from validateblockdefinition with task and blocks information, return a 1 D array of blocks of one task
*@param[in]matrix: the matrix with task information. For example, if frame 2 is doing task 3, on (2,3) will be 1
*@param[in]column: which column (task) we want to extract block information now
*@param[in]numrows: total number of rows of the matrix(frames)
*@param[out]output:output a 1D array with list of blocks, its beginning and ending frame.
*For example: 025688 means block one from frame 0 to frame 2;block 2 from frame 5 to frame 6;block 3 from frame 8 to frame 8.
*/
// ---------------------------------------------------------------------------------------------------------
int vtkbisConnectivityUtility::SplitBlocks(vtkpxMatrix* matrix , int column, int numrows, vtkIntArray* output)
{
	int pre=0;int numblocks=0;
	for (int r=0;r<numrows;r++)
	{

		if ( matrix->GetDirectElement(r,column) != pre)
		{
			if(pre==0)
			{
				pre=1;
				output->InsertNextTuple1(r);
				++numblocks;
			}else{
				pre=0;
				output->InsertNextTuple1(r-1);

			}
		}
	}
	if (output->GetNumberOfTuples()%2 != 0)
	{
		output->InsertNextTuple1(numrows-1);
	}
	std::cout<<"Beginning and ending frame of each block:"<<endl;
	for(int j=0;j<output->GetNumberOfTuples();j++)
	{
		std::cout<<output->GetTuple1(j)<<endl;
	}

	return numblocks;
}
/*
 * function h=vasicekExact(v,m)
>
> len=length(v);
>
> orderStats=sort(v);
>
>
>
> % Note that the intervals overlap for this estimator.
>
> intvals=orderStats(m+1:len)-orderStats(1:len-m);
>
> hvec=log(intvals);
>
> h=sum(hvec);
>
> %h=h/(len-m)+log((len+1)/m);     % Simpler version. See Learned-Miller
> et al.
>
> h=h/(len-m)+log(len+1)-psi(m);   % With partial bias correction? See
> Beirlant et al
 */
void vtkbisConnectivityUtility::VasicekExact(double* v,int len,int m)
{
	std::sort(v,v+len);


}

