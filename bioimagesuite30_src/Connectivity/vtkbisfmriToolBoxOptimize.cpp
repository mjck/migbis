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
 * vtkbisfmriToolBox.cpp
 *
 *  Created on: May 21, 2010
 *      Author: Isabella Murphy,Xilin Shen
 *      This class compute complex network measures
 *      1. Adjacency matrix (brain network)
 *      2. Degrees
 *      3. Reachability matrix
 *      4. Shortest path length matrix
 *      5. Number of triangles
 *      6. Characteristic path length
 *      7. individual characteristic path length
 *      8. Global efficiency
 *      9. individual global efficiency
 *      10. Clustering coefficient
 *      11. Transicity
 *      12. Local efficiency
 *      13. individual local efficiency
 *      14. Closeness Centrality
 *      15. Betweeness centrality
 *      @[in]:input,vtkImageData,seedToseed correlation result from vtkbisTimeSeriesCorrelation
 *      @[in]:this->functionalImage,vtkImageData
 *      @[in]:this->roiMap,vtkImageData
 *      @[in]:this->threshold, to threshold seedToseed correlation result to generate adjacency matrix for the rest of calculation
 *      @[out]:output,_shortestpathlength.nii.gz
 *      @[out]:this->OutData2,_networkMeasure.nii.gz
 *      @[out]:_adjMatrixForAlarkViewer.txt.Text file for Alark Viewer to display adjacency matrix.
 *      @[out]:_networkMeasures.txt. Text file for Alark Viewer to display complex network measures.
 *      @[out]:_roiCorrelationForAlarkViewer.txt. Text file for Alark Viewer to display seedToseed correlation result from vtkbisTimeSereisCorrelation.
 */
#include "vtkbisImageTimeSeriesNormalize.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include <vtkbisfmriToolBoxOptimize.h>
#include "vtkObjectFactory.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include <vector>
#include "vtkbisROIMean.h"
#include "vtkPoints.h"
#include "vtkIdList.h"
#include "vtkbisDijkstraGraphGeodesicPath.h"
#include "vtkpxMatrix.h"
#include "vtkPolyDataWriter.h"
#include <time.h>
#include "pxisinf.h"
#include <fstream>
#include <cstdlib>
#include "vnl/vnl_sparse_matrix.h"


using namespace std;


vtkbisfmriToolBoxOptimize* vtkbisfmriToolBoxOptimize::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisfmriToolBoxOptimize");
  if(ret)
    {
      return (vtkbisfmriToolBoxOptimize*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisfmriToolBoxOptimize;
}

vtkbisfmriToolBoxOptimize::vtkbisfmriToolBoxOptimize()
{
  this->FunctionalImage=NULL;
  this->roiMap=NULL;
  this->OutData2=NULL;
  this->Threshold=0.0;
}

//----------------------------------------------------------------------------
vtkbisfmriToolBoxOptimize::~vtkbisfmriToolBoxOptimize()
{
  this->SetFunctionalImage(NULL);
  this->SetroiMap(NULL);
  if (this->OutData2!=NULL)
    this->OutData2->Delete();
}

void vtkbisfmriToolBoxOptimize::SetFilename(const char* filename)
{
  this->Filename=filename;
}

const char* vtkbisfmriToolBoxOptimize::GetFilename()
{
  return this->Filename.c_str();
}
void vtkbisfmriToolBoxOptimize::SetFilename2(const char* filename2)
{
  this->Filename2=filename2;
}

const char* vtkbisfmriToolBoxOptimize::GetFilename2()
{
  return this->Filename2.c_str();
}
void vtkbisfmriToolBoxOptimize::SetFilename3(const char* filename3)
{
  this->Filename3=filename3;
}

const char* vtkbisfmriToolBoxOptimize::GetFilename3()
{
  return this->Filename3.c_str();
}
//----------------------------------------------------------------------------
int vtkbisfmriToolBoxOptimize::RequestInformation(vtkInformation*,
                                           vtkInformationVector**,
                                           vtkInformationVector* outputVector)
{
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
  return 1;
}
/*
 * ComputeBrainNetwork
 * @[in]:roiCorrelation. Output of vtkbisTimeSeriesCorrelation, an ROI*ROI*1*1 image.
 * For example, correlation of average time course 0 to  (say 0-10) is stored in output
 * vtkImagedata voxel 0-10 component 0;correlation of average time course 1 to all other
 * average time course (say 0-10) is stored in output vtkImageData voxel 11-20 component
 * 0 and so on.
 * @[in]:threshold. If value of (ROI1,ROI2)>threshold, (ROI1,ROI2)is stored in the network
 * @[in]:binary. If 0 then output weighted network; if 1 then output binary netowrk
 * @[out]:2Darray. A matrix describe the connection between ROIs
 */
vtkFloatArray* vtkbisfmriToolBoxOptimize::ComputeBrainNetwork(vtkImageData* roiCorrelation, float threshold, int binary)
{
	//get number of regions
	vtkDataArray* inp=roiCorrelation->GetPointData()->GetScalars();
	int nt = (int)sqrt(double(inp->GetNumberOfTuples()));

	//prepare output
	vtkFloatArray* output=vtkFloatArray::New();
	output->SetNumberOfComponents(nt);
	output->SetNumberOfTuples(nt);
	for(int ia=0;ia<nt;ia++)
		output->FillComponent(ia,0.0);

	//scan through the roiCorrelation result, fill up the brain network base on the threshold
	int counter=0;
	for (int ia=0;ia<nt;ia++)
    {
		for (int ib=0;ib<nt;ib++)
		{
			double value = inp->GetComponent(counter,0);
			if(value > threshold && ia!=ib)
			{
				if(binary > 0)
				{
					output->SetComponent(ia,ib,1.0);
				}
				else
				{
					output->SetComponent(ia,ib,value);
				}
			}
			++counter;
		}
    }

	return output;
}
/*
 * In an undirected graph, the degree is the number of connections for individual nodes.
 * @[in]:brainNetwork. the output of ComputeBrainNetwork
 * @[out]:output. Output a vtkIntArray and a text file
 */
vtkIntArray* vtkbisfmriToolBoxOptimize::ComputeDegreesUnd(vtkFloatArray* brainNetwork)
{
	int nt = brainNetwork->GetNumberOfComponents();
	vtkIntArray* output= vtkIntArray::New();
	output->SetNumberOfComponents(1);
	output->SetNumberOfTuples(nt);
	output->FillComponent(0,0.0);

	for (int i=0;i<nt;i++)
    {
		int sum = 0;
		for (int j=0;j<nt;j++)
		{
			if(brainNetwork->GetComponent(i,j)==1)
				++sum;
		}
		output->SetComponent(i,0,(double)sum);
    }
	return output;
}

float vtkbisfmriToolBoxOptimize::ComputeNumberOfTriangles(vtkFloatArray* brainNetwork,int tuple)
{
	int nt=brainNetwork->GetNumberOfTuples();
	float counter=0;
	for(int j=0;j<nt;j++)
	{
		for(int h=0;h<nt;h++)
		{
			if((brainNetwork->GetComponent(tuple,j)==1) && (brainNetwork->GetComponent(tuple,h)==1) && (brainNetwork->GetComponent(j,h)==1))
			{
				++counter;
			}
		}
	}
	counter/=2;
	return counter;
}
/*
void vtkbisfmriToolBoxOptimize::transpose(vnl_sparse_matrix<float> & A, vnl_sparse_matrix<float> & A_transpose)
{
    //vnl_sparse_matrix<double> A(30,30);
	//vnl_sparse_matrix<double> A_transpose(30,30);
	int N = A.rows();

	// For each row I
	for (int i = 0; i < N; i++)
	{
		vcl_vector< vnl_sparse_matrix_pair<float> > rowVector = A.get_row(i);
		int numRowElems = rowVector.size();

		for (int elem=0; elem<numRowElems; elem++)
		{
			vnl_sparse_matrix_pair<float> elemPair = rowVector[elem];
			int j = elemPair.first;
			float value = elemPair.second;

			A_transpose(j,i) = value;
		}
	}
}
*/
/*
 * Translate from matlab code betweeness_bin.m
 */
vtkpxMatrix* vtkbisfmriToolBoxOptimize::ComputeBetweenessCentrality(vtkFloatArray* brainNetwork)
{
	time_t start,end;
	time(&start);
	unsigned int nt=brainNetwork->GetNumberOfTuples();
	std::cout<<"Start computing BC remaked."<<endl;

	vtkpxMatrix* BC=vtkpxMatrix::New();
	//vtkpxMatrix* G=vtkpxMatrix::New();
	//vtkpxMatrix* Gt=vtkpxMatrix::New();
	//vtkpxMatrix* NPd=vtkpxMatrix::New();
	//vtkpxMatrix* tempNPd=vtkpxMatrix::New();
	//vtkpxMatrix* NSPd=vtkpxMatrix::New();
	//vtkpxMatrix* NSP=vtkpxMatrix::New();
	//vtkpxMatrix* tempNSP=vtkpxMatrix::New();
	//vtkpxMatrix* L=vtkpxMatrix::New();
	vtkpxMatrix* DP=vtkpxMatrix::New();
	//vtkpxMatrix* tempDP=vtkpxMatrix::New();
	//vtkpxMatrix* DPd1=vtkpxMatrix::New();
	//vtkpxMatrix* tempDPd1=vtkpxMatrix::New();
	//vtkpxMatrix* temptempDPd1=vtkpxMatrix::New();


	//G->Zeros(nt,nt);
	vnl_sparse_matrix<float> vnlG(nt,nt);
	for(int row = 0;row<nt;row++)
	{
		for(int col=0;col<nt;col++)
		{
			//G->SetElement(row,col,brainNetwork->GetComponent(row,col));
			float val = brainNetwork->GetComponent(row,col);
			if(val!=0)
			{
				vnlG(row,col)=val;

			}
		}
	}

	//NPd->Copy(G);
	vnl_sparse_matrix<float> vnlNPd(vnlG);

	std::cout<<"NPd->Copy(G) done "<<endl;


	//NSPd->Copy(NPd);
	vnl_sparse_matrix<float> vnlNSPd(vnlNPd);
	std::cout<<"NSPd->Copy(NPd) done "<<endl;


	//NSP->Copy(NSPd);
	vnl_sparse_matrix<float> vnlNSP(vnlNSPd);
	std::cout<<"NSP->Copy(NSPd) done "<<endl;

	//L->Copy(NSPd);
	vnl_sparse_matrix<float> vnlL(vnlNSPd);
	std::cout<<"L->Copy(NSPd) done "<<endl;

	//NSP(I)=1;L(I)1;
	for(int i=0;i<nt;i++)
	{
		//NSP->SetElement(d,d,1);
		vnlNSP(i,i)=1;
		//L->SetElement(d,d,1);
		vnlL(i,i)=1;
	}

	int NSPdhasOne=1;
	int d=1;//path length

	while(NSPdhasOne==1 )
	{
		NSPdhasOne=0;
		++d;
		std::cout<<"in while loop "<<d-1<<" time\n";

		//NPd=NPd*G
		//vtkpxMatrix::Multiply(NPd,G,tempNPd);
		//NPd->Copy(tempNPd);
		vnl_sparse_matrix<float> vnltempNPd(nt,nt);
        vnlNPd.mult(vnlG, vnltempNPd);
        vnlNPd=vnltempNPd;


		//NSPd=NPd.*(L==0)
		for(int row = 0;row<nt;row++)
		{
			for(int col=0;col<nt;col++)
			{
				//if(L->GetElement(row,col)==0)
				if(vnlL(row,col) == 0)
				{
					//NSPd->SetElement(row,col,NPd->GetElement(row,col));
					vnlNSPd(row,col)=vnlNPd(row,col);

				}else{
					//NSPd->SetElement(row,col,0);
					vnlNSPd(row,col)=0;
				}
			}
		}

		//NSP=NSP+NSPd
		//vtkpxMatrix::Add(NSP,NSPd,tempNSP);
		//NSP->Copy(tempNSP);
		vnl_sparse_matrix<float> vnltempNSP(nt,nt);
		vnlNSP.add(vnlNSPd,vnltempNSP);
		vnlNSP=(vnltempNSP);

		//L=L+d.*(NSPd~=0)
		for(int row = 0;row<nt;row++)
		{
			for(int col=0;col<nt;col++)
			{
				//if(NSPd->GetElement(row,col)!=0)
				if(vnlNSPd(row,col)!=0)
				{
					//L->SetElement(row,col,d);
					vnlL(row,col)=d;
					NSPdhasOne=1;
					//can not jump out of the double for look until all the rows and column of L has been updated
				}
			}
		}
	}//end of while loop

	//L(~L)=-1; L(I)=0; NSP(~NSP)=1
	for(int row = 0;row<nt;row++)
	{
		for(int col=0;col<nt;col++)
		{
			//if(L->GetElement(row,col)==0)
			if(vnlL(row,col)==0)
			{
				//L->SetElement(row,col,-1);
				vnlL(row,col)=-1;
			}
			//if(NSP->GetElement(row,col)==0)
			if(vnlNSP(row,col)==0)
			{
				//NSP->SetElement(row,col,1);
				vnlNSP(row,col)=1;
			}
		}
	}
	for(int i=0;i<nt;i++)
	{
		//L->SetElement(l,l,0);
		vnlL(i,i)=0;
	}


	//Gt=G.'
	//Gt->Copy(G);
	//Gt->Transpose();
	/**
	 * This block of code is to do transpose
	 */
	vnl_sparse_matrix<float> vnlGt(nt,nt);
	int N = vnlG.rows();

	// For each row I
	for (int i = 0; i < N; i++)
	{
		vcl_vector< vnl_sparse_matrix_pair<float> > rowVector = vnlG.get_row(i);
		int numRowElems = rowVector.size();

		for (int elem=0; elem<numRowElems; elem++)
		{
			vnl_sparse_matrix_pair<float> elemPair = rowVector[elem];
			int j = elemPair.first;
			float value = elemPair.second;

			vnlGt(j,i) = value;
		}
	}


	/*for(d=diam:-1:2)
	 *DPd1=((L==d).*(1+DP)/NSP)*Gt).*((L==d-1).*NSP)
	 *DP=DP+DPd1
	 */
	//DP->Zeros(nt,nt);
	vnl_sparse_matrix<float> vnlDP(nt,nt);
	//DPd1->Zeros(nt,nt);
	vnl_sparse_matrix<float> vnlDPd1(nt,nt);
	//tempDPd1->Zeros(nt,nt);
	vnl_sparse_matrix<float> vnltempDPd1(nt,nt);
	int diam=d-1;
	int counter2=0;
	for(int t=diam;t>=2;t--)
	{
		std::cout<<"In t loop "<<++counter2;
		for(int row = 0;row<nt;row++)
		{
			for(int col=0;col<nt;col++)
			{
				//if(L->GetElement(row,col)==t)
				if(vnlL(row,col)==t)
				{
					//tempDPd1->SetElement(row,col,((DP->GetElement(row,col)+1)/NSP->GetElement(row,col)));
					vnltempDPd1(row,col)=(vnlDP(row,col)+1)/vnlNSP(row,col);
				}
			}
		}
		//vtkpxMatrix::Multiply(tempDPd1,Gt,temptempDPd1);
		vnl_sparse_matrix<float> vnltemptempDPd1(nt,nt);
		vnltempDPd1.mult(vnlGt,vnltemptempDPd1);

		//DPd1->Fill(0);
		for(int row = 0;row<nt;row++)
		{
			for(int col=0;col<nt;col++)
			{
				//std::cout<<"L:"<<vnlL(row,col);
				//if(L->GetElement(row,col)==(t-1))
				if(vnlL(row,col)==t-1)
				{
					//DPd1->SetElement(row,col,temptempDPd1->GetElement(row,col)*NSP->GetElement(row,col));
					vnlDPd1(row,col)=vnltemptempDPd1(row,col)*vnlNSP(row,col);
				}
			}
		}
		//vtkpxMatrix::Add(DP,DPd1,tempDP);
		//DP->Copy(tempDP);
		vnl_sparse_matrix<float> vnltempDP(nt,nt);
		vnlDP.add(vnlDPd1,vnltempDP);
		vnlDP=vnltempDP;

	}

	//BC=sum(DP,1)
	//copy vnlDP to vtkpxMatrix DP and then do column sum using vtkpxMatrix's ColumnSums method
	DP->Zeros(nt,nt);
	int numRows = vnlDP.rows();

	// For each row I
	for (int i = 0; i < numRows; i++)
	{
		vcl_vector< vnl_sparse_matrix_pair<float> > rowVector = vnlDP.get_row(i);
		int numRowElems = rowVector.size();

		for (int elem=0; elem<numRowElems; elem++)
		{
			vnl_sparse_matrix_pair<float> elemPair = rowVector[elem];
			int j = elemPair.first;
			float value = elemPair.second;

			DP->SetElement(i,j,value);
			}
		}
	BC=DP->ColumnSums();


	time(&end);
	  std::cout<<endl<<"\nelapsed time for calculating Betweeness Centrality in fmritoolboxoptimized "<<difftime(end, start)<<" seconds"<<endl;

	//G->Delete();
	//Gt->Delete();
	//NPd->Delete();
	//tempNPd->Delete();
	//NSPd->Delete();
	//NSP->Delete();
	//tempNSP->Delete();
	//L->Delete();
	DP->Delete();
	//tempDP->Delete();
	//DPd1->Delete();
	//tempDPd1->Delete();
	//temptempDPd1->Delete();
	return BC;
}

/*
 * translate from matalb code efficiency.m
 */
vtkFloatArray* vtkbisfmriToolBoxOptimize::ComputeLocalEfficiency(vtkFloatArray* brainNetwork, vtkIntArray* degrees)
{
	time_t start,end;
	time(&start);
	std::cout<<"Start computing Local Efficiency."<<endl;

	int nt=brainNetwork->GetNumberOfTuples();

		vtkFloatArray* localE = vtkFloatArray::New();
		localE->SetNumberOfComponents(1);
		localE->SetNumberOfTuples(nt);
		localE->FillComponent(0,0.0);
		//vtkpxMatrix* g=vtkpxMatrix::New();
		vnl_sparse_matrix<float> vnlg;
		//vtkpxMatrix* D=vtkpxMatrix::New();
		vnl_sparse_matrix<float> vnlD;
		//vtkpxMatrix* tempD=vtkpxMatrix::New();
		vnl_sparse_matrix<float> vnltempD;
		//vtkpxMatrix* nPATH=vtkpxMatrix::New();
		vnl_sparse_matrix<float> vnlnPATH;
		//vtkpxMatrix* tempPATH=vtkpxMatrix::New();
		vnl_sparse_matrix<float> vnltempPATH;
		//vtkpxMatrix* L=vtkpxMatrix::New();
		vnl_sparse_matrix<float> vnlL;

/*
		int maxDegree=0;
		for (int i=0;i<nt;i++)
	    {
			if(degrees->GetComponent(i,0)>maxDegree)
			{
				maxDegree=degrees->GetComponent(i,0);
			}
	    }

		g->Zeros(maxDegree,maxDegree);
		L->Zeros(maxDegree,maxDegree);
		*/
		for(int c=0;c<nt;c++)
	    {
			int ki=(int)degrees->GetComponent(c,0);
			if(ki>=2)
			{
				//1.extract small matrix g
				vector<int> v(ki,0);
				int index=0;
				for(int t=0;t<nt;t++)
				{
					if(brainNetwork->GetComponent(t,c)>0)
					{
						v[index]=t;
						++index;
					}
				}

				//g->Zeros(ki,ki);
				//g->Fill(0.0);
				//vnl_sparse_matrix<float> vnlg(ki,ki);
				vnlg.set_size(ki,ki);
				for(int row = 0;row<ki;row++)
				{
					for(int col=0;col<ki;col++)
					{
						//g->SetElement(row,col,brainNetwork->GetComponent(v[row],v[col]));
						float val = brainNetwork->GetComponent(v[row],v[col]);
						if(val!=0)
						{
							vnlg(row,col)=val;
						}
					}
				}

				//2.make identical matrix D
				//D->Eye(ki);
				//vnl_sparse_matrix<float> vnlD(ki,ki);
				vnlD.set_size(ki,ki);
				for(int row = 0;row<ki;row++)
				{
					for(int col=0;col<ki;col++)
					{
						if(row==col)
						{
							vnlD(row,col)=1;
						}
					}
				}
			/*	for(int row = 0;row<ki;row++)
				{
					for(int col=0;col<ki;col++)
					{
						std::cout<<vnlD(row,col);
					}
				}
				*/

				//3.initialize number of steps
				int n=1;

				//4.nPATH=g
				//nPATH->Copy(g);
				//vnl_sparse_matrix<float> vnlnPATH(ki,ki);
				vnlnPATH.resize(ki,ki);
				vnlnPATH=(vnlg);

				//5.L=(nPATH~=0)
				int LhasOne=0;

				//L->Zeros(ki,ki);
				//L->Fill(0.0);
				//vnl_sparse_matrix<float> vnlL(ki,ki);
				vnlL.set_size(ki,ki);
				for(int row=0;row<ki;row++)
				{
					for(int col=0;col<ki;col++)
					{
						//if(nPATH->GetElement(row,col)>0)
						if(vnlnPATH(row,col)>0)
						{
							//L->SetElement(row,col,1);
							vnlL(row,col)=1;
							LhasOne=1;
						}
					}
				}
				while(LhasOne>0)
				{
					//6.D=D+n.*L
					//vtkpxMatrix::Add(D,L,tempD);
					//D->Copy(tempD);
					vnltempD.set_size(ki,ki);
					vnlD.add(vnlL,vnltempD);
					vnlD=(vnltempD);

					//7. increment number of steps
					++n;

					//8.nPATH=nPATH*g
					//vtkpxMatrix::Multiply(nPATH,g,tempPATH);
					//nPATH->Copy(tempPATH);
					vnltempPATH.set_size(ki,ki);
					vnlnPATH.mult(vnlg,vnltempPATH);
					vnlnPATH=(vnltempPATH);

					//9.L=(nAPTH~=0) and D==0
					LhasOne=0;
					//L->Fill(0.0);
					vnlL.set_size(ki,ki);
					for(int row=0;row<ki;row++)
					{
						for(int col=0;col<ki;col++)
						{
							//if((nPATH->GetElement(row,col)!=0) && (D->GetElement(row,col)==0))
							if(vnlnPATH(row,col)!=0 && vnlD(row,col)==0)
							{
								//L->SetElement(row,col,n);
								vnlL(row,col)=n;
								LhasOne=1;
							}
						}
					}
				}//end of while loop

				//10.D=1/D
				for(int row=0;row<ki;row++)
				{
					for(int col=0;col<ki;col++)
					{
						//double d=D->GetElement(row,col);
						double d=vnlD(row,col);

						if(d>0)
						{
							//D->SetElement(row,col,(double)1/d);
							vnlD(row,col)=1/d;
						}else{
							//D->SetElement(row,col,0);
							vnlD(row,col)=0;
						}
					}
				}

				//11.remove 1 from diagnal line
				for(int d =0;d<ki;d++)
				{
					//D->SetElement(d,d,0);
					vnlD(d,d)=0;
				}

				//12.sum D
				//float sum=D->Sum();
				float sum=0;
				for(int row=0;row<ki;row++)
				{
					for(int col=0;col<ki;col++)
					{
						sum+=vnlD(row,col);
					}
				}

				//13.e=sum/ki(ki-1)
				int bot=ki*(ki-1);
				localE->SetComponent(c,0,(double)sum/bot);
			}
	    }
		time(&end);
			  std::cout<<endl<<"\nelapsed time for calculating Local Efficiency in fmritoolboxoptimized "<<difftime(end, start)<<" seconds"<<endl;
		//g->Delete();
		//D->Delete();
		//tempD->Delete();
		//nPATH->Delete();
		//tempPATH->Delete();
		//L->Delete();
		return localE;
}

/*
 * Output format:
 * component
 * 0 index
 * 1 x
 * 2 y
 * 3 z
 * 4 label
 * 5 individual char path length
 * 6 individual global efficient
 * 7 closeness centrality
 * 8 # of triangles
 * 9 degrees
 * (below only store one value on tuple 0)
 * 10 char path length
 * 11 global efficiency
 * 12 clustering coefficient
 * 13 transicity
 */
vtkFloatArray* vtkbisfmriToolBoxOptimize::ComputeNetworkMeasures(vtkFloatArray* brainNetwork,vtkImageData* brain, vtkImageData* ROI, vtkIntArray* degrees,vtkDataArray* out)
{
  time_t start,end;
  time(&start);
  std::cout<<"Start computing Network Measures."<<endl;

  int nt = brainNetwork->GetNumberOfTuples();
  //std::cout<<"nt:!!!!!!!!!!!"<<nt<<endl;

  int numItem=14;

  vtkFloatArray* output=vtkFloatArray::New();
  output->SetNumberOfComponents(numItem);
  output->SetNumberOfTuples(nt);
  for(int ia=0;ia<numItem;ia++)
    output->FillComponent(ia,0.0);

  //compute xyz coordinates of each ROI center
  vtkbisROIMean* calCentroids = vtkbisROIMean::New();
  calCentroids->SetInput(brain);
  calCentroids->SetImageROI(ROI);
  calCentroids->SetDoText(0);
  calCentroids->Update();

  //vtkbisROIMean output has 1 extra point at end with xyz=-1,-1,-1
  vtkPoints* centroids = vtkPoints::New();
  centroids->Allocate(100,10);
  //centroids->ShallowCopy(calCentroids->GetCentroids());
  //calCentroids->Delete();

  vtkIdType pt[2];
  vtkPolyData* surface =vtkPolyData::New();
  vtkCellArray* s_lines=vtkCellArray::New();
  s_lines->Allocate(100,10);

  //Now, create the real connections
  for(int i=0;i<nt;i++)//for each point(ROI)
    {
      double x[3];
      //centroids->GetPoint(i,x);
      calCentroids->GetCentroids()->GetPoint(i,x);
      output->SetComponent(i,0,i);
      output->SetComponent(i,1,x[0]);
      output->SetComponent(i,2,x[1]);
      output->SetComponent(i,3,x[2]);
      output->SetComponent(i,4,i+1);

      centroids->InsertNextPoint(x);
      for(int j=i+1;j<nt;j++)
	{
	  if(brainNetwork->GetComponent(i,j)>0)
	    {
	      pt[0]=i;
	      pt[1]=j;
	      s_lines->InsertNextCell(2,pt);
	    }
	}
    }

  surface->SetPoints(centroids);
  centroids->Delete();
  surface->SetLines(s_lines);
  s_lines->Delete();
/*
  vtkPolyDataWriter* w=vtkPolyDataWriter::New();
  w->SetInput(surface);
  w->SetFileName("test.vtk");
  w->Write();
  w->Delete();
*/
  vtkIntArray* pathlength = vtkIntArray::New();
  pathlength->SetNumberOfComponents(nt);
  pathlength->SetNumberOfTuples(nt);

  vtkIntArray* reachable =  vtkIntArray::New();
  reachable->SetNumberOfComponents(nt);
  reachable->SetNumberOfTuples(nt);

  for(int ia=0;ia<nt;ia++)
    {
      pathlength->FillComponent(ia,0.0);
      reachable->FillComponent(ia,0.0);
    }

  for(int ib=0;ib<nt;ib++)
    reachable->SetComponent(ib,ib,1.0);


  //find shortest path using vtkDijkstraGraphGeodesicPath
  //std::map<int, vector<int> > pathmatrix;
  vtkbisDijkstraGraphGeodesicPath* shpath = vtkbisDijkstraGraphGeodesicPath::New();


  //int idcounter=1;
  for (int k=0;k<nt;k++)
    {
      for(int l=k+1;l<nt;l++)
	{
	  shpath->SetInput(surface);
	  shpath->SetStartVertex(k);
	  shpath->SetEndVertex(l);
	  shpath->Update();

	  vtkIdList* list = shpath->GetIdList();

	  //if there is a path between k and l, store it.
	  if(list->GetId(1)!= -1)
	    {
	      int numid = list->GetNumberOfIds();
	      pathlength->SetComponent(k,l,double(numid-1.0));
	      reachable->SetComponent(k,l,1.0);
	      reachable->SetComponent(l,k,1.0);
	      //std::cout<<"["<<l<<","<<k<<"] \t";
	      //myfile<<"["<<l<<","<<k<<"] \t";
	      //vector<int> path(numid,0);
	      //for (int m=0;m<numid;m++)
	      //{
	      //path[m]=list->GetId(m);
	      //std::cout<<list->GetId(m)<<"\t";
	      //myfile<<list->GetId(m)<<"\t";
	      //}
	      //pathmatrix[idcounter]=path;
	      //idTable->SetComponent(k,l,idcounter);
	      //idcounter++;

	      //std::cout<<"\n";
	      //myfile<<"\n";
	    }
	}
    }

  /*1.output reachability matrix
   *2.output shortest path length matrix
   *3.calculate characteristic path length (cpl)
   *4.calculate global efficiency(ge)
   *5.calculate closeness centrality
   */
  /*	//std::cout<<"Reachability Matrix:\n";
	//myfile<<"Reachability Matrix:\n";
	for(int r=0;r<nt;r++)
	{
	for(int c=0;c<nt;c++)
	{
	std::cout<<reachable->GetComponent(r,c)<<"\t";
	myfile<<reachable->GetComponent(r,c)<<"\t";
	}
	//std::cout<<"\n";
	//myfile<<"\n";
	}
  */
  double cpl =0;
  double ge=0.0;
  //  vector<double> centrality(0,nt);

  //std::cout<<"Path Length Matrix:\n";
  //myfile<<"Path Length Matrix:\n";
  int colument=0;
  int offset=0;
  for(int n=0;n<nt;n++)
    {
      double sumdi=0.0;
      //initialize rownt=1. Because a node is always connect to itself, but our brain network set plength to itself to 0. By setting rownt=1 to start with, we add 1 back.
      int rownt=1;
      double thiscpl=0.0;
      double thisge=0.0;
      for(int p=0;p<nt;p++)
	{
	  int plength;
	  if(n<p)
	    {
	      plength=(int)pathlength->GetComponent(n,p);
	    }else{
	    plength=(int)pathlength->GetComponent(p,n);
	  }
	  if(plength>0)
	    {
	      thisge+=(double)1/plength;
	      ++rownt;
	      sumdi+=plength;
	      out->InsertTuple1(offset,plength);

	    }
	  ++offset;
	  //std::cout<<plength<<"\t";
	  //myfile<<plength<<"\t";


	}
      output->SetComponent(n,7,(double)(rownt-1)/sumdi);
      //centrality[n]=(double)(rownt-1)/sumdi;

      //if rownt >1 means this node connect to another node other then itself, then count this node for CPL and GE
      if(rownt>1)
	{
	  thisge/=(rownt-1);
	  output->SetComponent(n,6,thisge);
	  ge+=thisge;
	  thiscpl=(double)sumdi/(rownt-1);
	  output->SetComponent(n,5,thiscpl);
	  cpl+=thiscpl;
	  ++colument;
	}
      //std::cout<<"\n";
      //myfile<<"\n";
    }

  cpl/=colument;
  ge/=colument;
  std::cout<<"Characteristic Path Length: "<<cpl<<" \nGlobal Efficiency: "<<ge;
  //myfile<<"Characteristic Path Length: "<<cpl<<" \nGlobal Efficiency: "<<ge<<" \nCloseness Centrality: ";
  output->SetComponent(0,10,cpl);
  output->SetComponent(0,11,ge);
  /*	for(int ib=0;ib<nt;ib++)
    {
    std::cout<<centrality[ib]<<"\t";
    myfile<<centrality[ib]<<"\t";

    }
  */
  //calculate number of triangles
  vector<float> triangle(nt,0.0);
  //std::cout<<"\nNumber of Triangles: ";
  //myfile<<"\nNumber of Triangles: ";
  for(int i=0;i<nt;i++)
    {
      triangle[i]=vtkbisfmriToolBoxOptimize::ComputeNumberOfTriangles(brainNetwork,i);
      //std::cout<<triangle[i]<<"\t";
      //myfile<<triangle[i]<<"\t";
      output->SetComponent(i,8,ComputeNumberOfTriangles(brainNetwork,i));
    }

  //compute clustering coefficient
  float ci=0.0;int counter=0;
  float ttop=0.0;float tbot=0.0;
  for(int i=0;i<nt;i++)
    {
      int ki=(int)degrees->GetComponent(i,0);
      output->SetComponent(i,9,ki);
      if(ki>=2)
	{
	  ttop+=2*triangle[i];
	  tbot+=ki*(ki-1);
	  ci+=(float)((2*triangle[i])/(ki*(ki-1)));
	  ++counter;
	}
    }
  float cc=(float)ci/counter;
  float trans=(float)ttop/tbot;

  std::cout<<"\nClustering Coefficient: "<<cc;
  std::cout<<"\nTransitivity: "<<trans;

  output->SetComponent(0,12,cc);
  output->SetComponent(0,13,trans);

  //myfile<<"\nClustering Coefficient: "<<cc<<"\nTransitivity: "<<trans;

  time(&end);
  std::cout<<endl<<"\nelapsed time for calculating Network Measurements in fmritoolboxoptimized "<<difftime(end, start)<<" seconds"<<endl;

  surface->Delete();
  pathlength->Delete();

  //idTable->Delete();

  //shpath->Delete();
  //fprintf(stderr,"OK about to return output size %d x %d\n",output->GetNumberOfTuples(),output->GetNumberOfComponents());
  return output;
  //myfile.close();

}

/*
 * otherData Input format:
 * component
 * 0 index
 * 1 x
 * 2 y
 * 3 z
 * 4 label
 * 5 individual char path length
 * 6 individual global efficient
 * 7 closeness centrality
 * 8 # of triangles
 * 9 degrees
 * (below only store one value on tuple 0)
 * 10 char path lenth
 * 11 global efficiency
 * 12 clustering coefficient
 * 13 transicity
 */
void vtkbisfmriToolBoxOptimize::GenerateOutputForAlarkViewer(vtkFloatArray* localE, vtkpxMatrix * BC, vtkFloatArray* otherData,const char* Filename2, vtkDataArray* out2)
{
  int nt=otherData->GetNumberOfTuples();
  int nc=10;
  int out2nc=12;

  std::ofstream alarkfile(Filename2);

  if(!alarkfile.is_open())
    {
      cout<< "Unable to open file"<<Filename2<<"\n";
      return;
    }
  else
    {
      std::cout << "Opening " << Filename2 << "\n";
    }

  alarkfile<<"#Connectivity file\n#Points\n"<<nt<<"\n#index x y z label ind.CharPathLength ind.GlobalEffi. ClosenessCentrality #ofTriangles Degrees LocalEffi BetweenessCentrality\n";

  int index=0;
  for (int i=0;i<nt;i++)
    {
      //      fprintf(stderr,"Row %d/%d\n",i,nt);
      for(int j=0;j<nc;j++)
	{
	  alarkfile<<otherData->GetComponent(i,j)<<"\t";
	  out2->SetComponent(index,0,otherData->GetComponent(i,j));
	  ++index;
	}
      alarkfile<<localE->GetComponent(i,0)<<"\t"<<BC->GetDirectElement(i,0)<<"\n";
      out2->SetComponent(index,0,localE->GetComponent(i,0));
      ++index;
      out2->SetComponent(index,0,BC->GetDirectElement(i,0));
      ++index;
    }
  alarkfile<<"#Other information\n#CharPathLength "<<otherData->GetComponent(0,10)<<"\n#Global Effi "<<otherData->GetComponent(0,11)<<"\n#Clustering Coef "<<otherData->GetComponent(0,12)<<"\n#Transicity "<<otherData->GetComponent(0,13);

  alarkfile.close();
  //  fprintf(stderr,"This Far\n");
}

void vtkbisfmriToolBoxOptimize::GenerateAdjMatrixOutputForAlarkViewer(vtkFloatArray* brainNetwork, vtkFloatArray* otherData, const char* Filename)
{
	std::ofstream alarkfile(Filename);

		if(!alarkfile.is_open())
		{
			cout<< "Unable to open file"<<Filename<<"\n";
			return;
		}
		else
		{
			std::cout << "Opening " << Filename << "\n";
		}

		int nt=brainNetwork->GetNumberOfTuples();
		int nc=5;

		alarkfile<<"#Connectivity file\n#Points\n"<<nt<<"\n#index x y z label\n";

		for (int i=0;i<nt;i++)
		{
			for(int j=0;j<nc;j++)
			{
				alarkfile<<otherData->GetComponent(i,j)<<"\t";
			}
			alarkfile<<"\n";
		}

		alarkfile<<"\n#Connections and strengths\n"<<((1+nt-1)*(nt-1))/2<<"\n";
		for(int i=0;i<nt;i++)
		{
			for(int j=i+1;j<nt;j++)
			{
				alarkfile<<i<<"\t"<<j<<"\t"<<brainNetwork->GetComponent(i,j)<<"\n";
			}
		}

		alarkfile.close();
}
void vtkbisfmriToolBoxOptimize::GenerateROICorrelationOutputForAlarkViewer(vtkImageData* roiCorrelation, vtkFloatArray* otherData, const char* Filename3)
{
	std::ofstream alarkfile(Filename3);

	if(!alarkfile.is_open())
	{
		cout<< "Unable to open file"<<Filename3<<"\n";
		return;
	}else
	{
		std::cout << "Opening " << Filename3 << "\n";
	}

	//get number of regions
	vtkDataArray* inp=roiCorrelation->GetPointData()->GetScalars();
	int nt = (int)sqrt(double(inp->GetNumberOfTuples()));
	int nc=5;

	alarkfile<<"#Connectivity file\n#Points\n"<<nt<<"\n#index x y z label\n";

	for (int i=0;i<nt;i++)
	{
		for(int j=0;j<nc;j++)
		{
			alarkfile<<otherData->GetComponent(i,j)<<"\t";
		}
		alarkfile<<"\n";
	}

	alarkfile<<"\n#Connections and strengths\n"<<((1+nt-1)*(nt-1))/2<<"\n";

	//prepare temp structure to whole roi correlation data
	vtkFloatArray* temp=vtkFloatArray::New();
	temp->SetNumberOfComponents(nt);
	temp->SetNumberOfTuples(nt);
	for(int ia=0;ia<nt;ia++)
		temp->FillComponent(ia,0.0);

	int index=0;
	for(int it=0;it<nt;it++)
	{
		for(int jt=0;jt<nt;jt++)
		{
			temp->SetComponent(it,jt,inp->GetComponent(index,0));
			++index;
		}
	}
	for(int i=0;i<nt;i++)
	{
		for(int j=i+1;j<nt;j++)
		{
			alarkfile<<i<<"\t"<<j<<"\t"<<temp->GetComponent(i,j)<<"\n";

		}
	}
	alarkfile.close();
}
/*
 * @[in]:input:ROI Correlation from vtkbisTimeSeriesCorrelation
 * @[in]:this->FunctionalImage
 * @[in]:this->ROI Map
 * @[out]:output:(pair-wise) shortest path length
 * @[out]:OutData2:network measures
 */
void vtkbisfmriToolBoxOptimize::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
	fprintf(stderr,"Beginning vtkbisfmriToolBoxOptimize\n");
	if(input == NULL)
	{
		cerr<<"No ROI Correlation Image input. Computation terminated.\n";
		return;
	}else if(this->FunctionalImage ==NULL)
	{
		cerr<<"No Functional Image input. Computation terminated.\n";
		return;
	}else if(this->roiMap == NULL)
	{
		cerr<<"No ROI Map input. Computation terminated.\n";
		return;
	}

	int dim[3];int dim2[3];
	this->FunctionalImage->GetDimensions(dim);
	this->roiMap->GetDimensions(dim2);
	int sum=0;int sum1=0;
	for(int ia=0;ia<=2;ia++)
	{
		sum+=abs(dim2[ia]-dim[ia]);
	}
	if(sum>0)
	{
		cerr<<"ROI Map and Functional image does not have the same dimensions. Computation terminated.'n";
		return;
	}
	int numItem=12;
std::cout<<"going to compute brain network."<<endl;
	vtkFloatArray* brainNetwork=vtkbisfmriToolBoxOptimize::ComputeBrainNetwork(input, this->Threshold, 1);
std::cout<<"Done computing brain network."<<endl;
	int nc=brainNetwork->GetNumberOfComponents();
	vtkDataArray* out=output->GetPointData()->GetScalars();
	out->FillComponent(0,0.0);
std::cout<<"going to compute degrees."<<endl;
	vtkIntArray* degrees=vtkbisfmriToolBoxOptimize::ComputeDegreesUnd(brainNetwork);
	std::cout<<"Done computing degrees."<<endl;
	vtkpxMatrix* BC=vtkbisfmriToolBoxOptimize::ComputeBetweenessCentrality(brainNetwork);
	vtkFloatArray* localE=vtkbisfmriToolBoxOptimize::ComputeLocalEfficiency(brainNetwork,degrees);
	vtkFloatArray* otherData=vtkbisfmriToolBoxOptimize::ComputeNetworkMeasures(brainNetwork,this->FunctionalImage,this->roiMap,degrees,out);

	//	fprintf(stderr,"This far\n");

	vtkbisfmriToolBoxOptimize::GenerateAdjMatrixOutputForAlarkViewer(brainNetwork, otherData, this->GetFilename());

	//output network measures as an nii.gz file
	int dim3[3];dim3[0]=nc;dim3[1]=numItem;dim3[2]=1;
	//	int ext[6] = { 0, nc-1, 0, numItem-1, 0, 1 };
	this->OutData2=vtkImageData::New();
	this->OutData2->SetSpacing(output->GetSpacing());
	this->OutData2->SetOrigin(output->GetOrigin());
	this->OutData2->SetScalarTypeToFloat();
	this->OutData2->SetNumberOfScalarComponents(1);
	this->OutData2->SetDimensions(dim3);
	//	this->OutData2->SetWholeExtent(ext);
	this->OutData2->AllocateScalars();//allocation space on memory

	vtkDataArray* out2=this->OutData2->GetPointData()->GetScalars();
	out2->FillComponent(0,0.0);

	vtkbisfmriToolBoxOptimize::GenerateOutputForAlarkViewer(localE, BC, otherData,this->GetFilename2(), out2);
	vtkbisfmriToolBoxOptimize::GenerateROICorrelationOutputForAlarkViewer(input, otherData, this->GetFilename3());
}

