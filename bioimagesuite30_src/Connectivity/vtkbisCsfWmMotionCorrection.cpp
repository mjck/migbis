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
 * vtkbisCsfWmMotionCorrection.cpp
 *
 *  Created on: May 29, 2009
 *      Author: Isabella Murphy, Xilin Shen
 *      calculate the mean of csh and white matter, construct a matrix [ csf_mean white matter_mean motion1 motion2 motion3 motion4 motion5 motion6 ]
 *      Define mask 0=background, 1=csf, 2=white matter, 3=grey matter
 */

#include <vtkbisCsfWmMotionCorrection.h>
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vnl/vnl_matrix.h"
#include "vnl/algo/vnl_matrix_inverse.h"



using namespace std;

// This creates the "New" Function
vtkStandardNewMacro(vtkbisCsfWmMotionCorrection);

vtkbisCsfWmMotionCorrection::vtkbisCsfWmMotionCorrection()
{
  this->ImageMask=NULL;
  this->MotionMatrix=vtkpxMatrix::New();
  this->LSQ=vtkpxMatrix::New();

}
vtkbisCsfWmMotionCorrection::~vtkbisCsfWmMotionCorrection()
{
  this->SetImageMask(NULL);
  this->MotionMatrix->Delete();
  this->LSQ->Delete();

}
void vtkbisCsfWmMotionCorrection::SetFileName(const char* filename)
{
  this->FileName=filename;
}

const char* vtkbisCsfWmMotionCorrection::GetFileName()
{
  return this->FileName.c_str();
}
//----------------------------------------------------------------------------------------------------------------------
//RequestInformation
//I. make sure output image data type is float
// ---------------------------------------------------------------------------------------------------------------------------
int vtkbisCsfWmMotionCorrection::RequestInformation (vtkInformation * request,
						vtkInformationVector ** inputVector,
						vtkInformationVector * outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);//set output datatype to float

  return 1;
}
// ---------------------------------------------------------------------------------------------------------------------------
/*
  Problem =  A x = B  where X is unknown

 First premultiply by A':  A'A x = A'B
 Solution x = (A'A)^{-1}.A'.B

  LSQ = (A'A)^{-1}.A'
*/
// ---------------------------------------------------------------------------------------------------------------------------

void vtkbisCsfWmMotionCorrection::ComputeLSQMatrix(vtkpxMatrix* A, vtkpxMatrix* LSQ)
{
  int sz[2]; A->GetSize(sz);
  vnl_matrix<double> M(sz[0],sz[1]);
  for (unsigned i=0; i<M.rows(); ++i)
    for (unsigned j=0; j<M.cols(); ++j)
      M(i,j)=A->GetDirectElement(i,j);


  vnl_matrix<double> Mt=M.transpose();
  vnl_matrix<double> MtM=Mt*M;
  vnl_matrix<double> IN=vnl_matrix_inverse<double>(MtM);
  vnl_matrix<double> F=IN*Mt;

  LSQ->Zeros(F.rows(), F.cols());
  for (unsigned i=0; i<F.rows(); ++i)
    for (unsigned j=0; j<F.cols(); ++j)
      LSQ->SetDirectElement(i,j,F(i,j));
}
/*
 * EliminateError: subtract error from original data
 *@param[in]inp: original input data
 *@param[in]index: which voxel we are working at
 *@param[in]numframes: how many frames
 *@param[in]LSQ: LSQ matrix calculated earlier
 *@param[in]A:8 column big matrix with csf mean, wm mean and 6 columns of motion information
 *@param[out]out: write to output image directly
 */
void vtkbisCsfWmMotionCorrection::EliminateError(vtkDataArray* inp,vtkDataArray* out,int index,
						    int numframes,
						    vtkpxMatrix* LSQ,
						    vtkpxMatrix* A)
{
/*	//Store original value of each frame of voxel "index" into B matrix
	for (int i=0;i<numframes;i++)
	  this->B->SetDirectElement(i,0,inp->GetComponent(index,i));

	//X = LSQ*B
	vtkpxMatrix::Multiply(LSQ,this->B,this->X);

	//AX = A*X
	vtkpxMatrix::Multiply(A,this->X,this->AX);

	//for every frame, subtract AX
	for (int ia=0;ia<numframes;ia++)
	{
		out->SetComponent(index,ia,this->B->GetDirectElement(ia,0)-this->AX->GetDirectElement(ia,0));
			//std::cout<<"NV:"<<out->GetComponent(index,ia)<<"="<<B->GetDirectElement(ia,0)<<"-"<<AX->GetDirectElement(ia,0)<<"|";
	}
*/

  vnl_matrix<double> mB(numframes,1);
  for(int i=0;i<numframes;i++)
    mB(i,0)=inp->GetComponent(index,i);


  int sz[2]; LSQ->GetSize(sz);
  vnl_matrix<double> mLSQ(sz[0],sz[1]);
  for (unsigned i=0; i<mLSQ.rows(); ++i)
    for (unsigned j=0; j<mLSQ.cols(); ++j)
      mLSQ(i,j)=LSQ->GetDirectElement(i,j);

  int szA[2]; A->GetSize(szA);
  vnl_matrix<double> mA(szA[0],szA[1]);
  for (unsigned i=0; i<mA.rows(); ++i)
    for (unsigned j=0; j<mA.cols(); ++j)
      mA(i,j)=A->GetDirectElement(i,j);

  vnl_matrix<double> mX=mLSQ*mB;
  vnl_matrix<double> mAX=mA*mX;
  //std::cout<<"I am in eliminateError"<<endl;
  //for (unsigned i=0; i<mAX.rows(); ++i)
  //for (unsigned j=0; j<mAX.cols(); ++j)
  //this->AX->SetDirectElement(i,j,mAX(i,j));
  for (int ia=0;ia<numframes;ia++)
    {
      out->SetComponent(index,ia,mB(ia,0)-mAX(ia,0));
      //std::cout<<"NV:"<<out->GetComponent(index,ia)<<"="<<B->GetDirectElement(ia,0)<<"-"<<AX->GetDirectElement(ia,0)<<"|";
    }

}
/*
 * MotionErrorCorr: correct motion error
 *@param[in]numoftuples: number of tuples in the image
 *@param[in]numofframes: number of frames in the image
 *@param[in]inp: input vtkDataArray
 *@param[in]msk: mask vtkDataArray
 *@param[out]out: write to output image directly
 */
void vtkbisCsfWmMotionCorrection::MotionErrorCorr(int numoftuples,int numofframes,vtkDataArray* inp,vtkDataArray* out,vtkDataArray* msk)
{
  int nt=numoftuples;
  int nc=numofframes;
  vtkpxMatrix* A = vtkpxMatrix::New();
  A->ImportFromMatlab(this->FileName.c_str(),"Q_ser");

  this->ComputeLSQMatrix(A,this->LSQ);

  int maxcount=int(nt/10);
  int count=0;
  double pog=0.0;
  for (int voxel=0;voxel<nt;voxel++)
    {
      int dooperation=1;
      if (msk!=NULL)
	{
	  if (msk->GetComponent(voxel,0) <1)
	    dooperation=0;
	}

      if (dooperation)
	{
	  this->EliminateError(inp,out,voxel,nc,LSQ,A);
	}

      ++count;
      if (count==maxcount)
	{
	  pog=double(voxel)/double(nt);
	  this->UpdateProgress(pog);
	  count=0;
	  //fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,double(numdone)/double(nt));
	}
    }
  A->Delete();
}
/*
 * CSFWMErrorCorr: correct CSF, WM error
 *@param[in]numoftuples: number of tuples in the image
 *@param[in]numofframes: number of frames in the image
 *@param[in]inp: input vtkDataArray
 *@param[in]msk: mask vtkDataArray
 *@param[out]out: write to output image directly
 */
void vtkbisCsfWmMotionCorrection::CSFWMErrorCorr(int numoftuples,int numofframes,vtkDataArray* inp,vtkDataArray* out,vtkDataArray* msk)
{
  int nt=numoftuples;
  int nc=numofframes;

  double* csfMean = new double[nc];
  double* wmMean = new double[nc];
  for(int ia=0;ia<nc;ia++)
    {
      csfMean[ia]=0.0;
      wmMean[ia]=0.0;
    }

  double csfsum,wmsum;int csfcount,wmcount;
  for (int i=0;i<nc;i++)
    {
      csfsum=0.0;wmsum=0.0;
      csfcount=0;wmcount = 0;
      for (int j=0;j<nt;j++)
	{
	  if(msk->GetComponent(j,0) == 1)
	    {
	      csfsum+=inp->GetComponent(j,i);
	      ++csfcount;
	    }
	  if(msk->GetComponent(j,0) == 2)
	    {
	      wmsum+=inp->GetComponent(j,i);
	      ++wmcount;
	    }
	}
      csfMean[i]=(double)(csfsum/csfcount);
      wmMean[i]=(double)(wmsum/wmcount);
    }

  //take out the error from csf and wm
  vtkpxMatrix* csfwmA = vtkpxMatrix::New();
  csfwmA->Zeros(nc,2);
  for(int r=0;r<nc;r++)
    {
      int c=0;
      csfwmA->SetDirectElement(r,c,csfMean[r]);
      ++c;
      csfwmA->SetDirectElement(r,c,wmMean[r]);
    }

  this->ComputeLSQMatrix(csfwmA, this->LSQ);
  //std::string fname = "/mnt/cluster1/isabella/Desktop/james.mat";
  //std::string matrixname = "James";
  //csfwmALSQ->ExportToMatlab(fname.c_str(),matrixname.c_str());
  //csfwmALSQ->Print();


  for (int voxel=0;voxel<nt;voxel++)
    {
      int dooperation=1;
      if (msk!=NULL)
	{
	  if (msk->GetComponent(voxel,0) <1)
	    dooperation=0;
	}

      if (dooperation)
	{
	  this->EliminateError(inp,out,voxel,nc,this->LSQ,csfwmA);
	}
    }

  delete [] csfMean;
  delete [] wmMean;
  csfwmA->Delete();

}
void vtkbisCsfWmMotionCorrection::SimpleExecute(vtkImageData* input, vtkImageData* output)
{

  fprintf(stderr,"Beginning vtkbisCsfWmMotionCorrection\n");

  if (this->ImageMask!=NULL)
    {
      int dim[3]; this->ImageMask->GetDimensions(dim);
      int dim2[3]; input->GetDimensions(dim2);
      int sum=0;
      for (int ia=0;ia<=2;ia++)
	sum+=abs(dim[ia]-dim2[ia]);
      if (sum>0)
	{
	  fprintf(stderr,"Bad Mask Input to vtkbisCsfWmMotionCorrection SimpleExecute\n");
	  return;
	}
    }

  vtkDataArray*   inp=input->GetPointData()->GetScalars();
  int nt=inp->GetNumberOfTuples();
  int nc=inp->GetNumberOfComponents();

  vtkDataArray* msk=NULL;
  if (this->ImageMask!=NULL)
    msk=this->ImageMask->GetPointData()->GetScalars();

  vtkDataArray*   out=output->GetPointData()->GetScalars();
  for (int ia=0;ia<nc;ia++)
    out->FillComponent(ia,0.0);

  if(this->doMotion==1 && this->doCsfWm ==1)
    {
      this->CSFWMErrorCorr(nt,nc,inp,out,msk);
      this->MotionErrorCorr(nt,nc,out,out,msk);
    }
  else if (this->doMotion==1)
    {
      this->MotionErrorCorr(nt,nc,inp,out,msk);
    }
  else if (this->doCsfWm == 1)
    {
      this->CSFWMErrorCorr(nt,nc,inp,out,msk);
    }
}

