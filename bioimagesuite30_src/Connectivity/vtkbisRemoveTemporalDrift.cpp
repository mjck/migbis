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
 * vtkbisRemoveTemporalDrift.cpp
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
#include "vtkbisRemoveTemporalDrift.h"
#include "vtkpxAnalyzeImageWriter.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"

vtkbisRemoveTemporalDrift* vtkbisRemoveTemporalDrift::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisRemoveTemporalDrift");
  if(ret)
    {
      return (vtkbisRemoveTemporalDrift*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisRemoveTemporalDrift;
}

vtkbisRemoveTemporalDrift::vtkbisRemoveTemporalDrift()
{
  this->DebugIndex=-1;
  this->ImageMask=NULL;
  this->Order=3;
  this->UseLegendrePolynomials=1;
}

vtkbisRemoveTemporalDrift::~vtkbisRemoveTemporalDrift()
{
  this->SetImageMask(NULL);
}

/*-------------------------------------------------------------------------------------
 * force the output data type to float, need to change extend information to let
 * the pipeline know the difference
 * @Set: output data type = float
 */
int vtkbisRemoveTemporalDrift::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector,
					  vtkInformationVector *outputVector)
{
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
	return 1;
}
/*!---------------------------------------------------------------------------------------------------------------------------
*Use Legendre polynomial functions or power function to create a matrix.
*Call this method with outer loop = number of orders, inner loop = number of frame of the input image
*@param[in]t:the time point (frame) we are working at
*@param[in]order:order of polynomial (0=constant,1=linear etc.)
*@/return a double value
*/
double vtkbisRemoveTemporalDrift::Polynomial(double t, int order)
{
    if (order<0)
      order=0;
    else if (order>6)
      order=6;

    if (this->UseLegendrePolynomials)
      {
	switch(order)
	  {
	  case 0: // P_0(x)=1;
	    return 1.0;
	    break;
	  case 1: // P_1(x)=x;
	    return t;
	    break;
	  case 2: // P_2(x)=0.5*(3x^2-1)
	    return 1.5*t*t-0.5;
	    break;
	  case 3: // P_3(x) =0.5*(5x^3-3x)
	    return 2.5*t*t*t-1.5*t;
	    break;
	  case 4: // P_4(x) = 1/8*(35x^4-30x^2+3)
	    return 0.125*35*t*t*t*t-0.125*30*t*t+0.375;
	    break;
	  case 5: // P_5(x) = 1/8*(63*x^5-70*x^3+15x)
	    return 0.125*63*t*t*t*t*t-0.125*70*t*t*t+0.125*15*t;
	    break;
	  case 6: // P_6(x) = 1/16*(231x^6-315x^4+105x^2-5)
	    return (231*t*t*t*t*t*t-315*t*t*t*t+105*t*t-5)/16;
	    break;
	  }
      }


	return pow(t,order);

 }
/*! ---------------------------------------------------------------------------------------------------------------------------
* a function that return a “central” representation for time frame: col-(numframes-1)/2.
* has to change range to -1 to 1, there for divid the result with
* (numframes-1)/2
*@param[in]col: the column(frame) we are working on
*@param[in]numframe: total number of frames
*@/return centralized representation for time frame (range from -1 to 1)
*/
double vtkbisRemoveTemporalDrift::ComputeTime(int col,int numframes)
{
	if ((numframes-1)<0.00001)
		return ((double)col)-((double)numframes-1)*0.5;
	else
		return (((double)col)-((double)numframes-1)*0.5)/(((double)numframes-1)*0.5);
}
/*!---------------------------------------------------------------------------------------------------------------------------
*  Problem : A x = B  where X is unknown
*  First pre-multiply by A':  A'A x = A'B
*  Solution x = (A'A)^{-1}.A'.B
*  LSQ = (A'A)^{-1}.A'
*@param[in]order: order of LSQ
*@param[in]numframes: number of frames of the input image
*@/return LSQ matrix
*/
vtkpxMatrix* vtkbisRemoveTemporalDrift::ComputeLSQMatrix(int order,int numframes)
{
	double v;
  vtkpxMatrix* At=vtkpxMatrix::New();
  At->Zeros(order+1,numframes);

  for (int row=0;row<=order;row++)
    {
      for (int col=0;col<numframes;col++)
		  {
			  double t=this->ComputeTime(col,numframes);
			  v=Polynomial(t,row);
			  At->SetElement(row,col,v);
		  }
    }
  //At->Print();

  vtkpxMatrix* A=vtkpxMatrix::New();
  A->Copy(At);
  A->Transpose();

  vtkpxMatrix* AtA=vtkpxMatrix::New();
  vtkpxMatrix::Multiply(At,A,AtA);
  AtA->Invert();

  vtkpxMatrix* LSQ=vtkpxMatrix::New();
  vtkpxMatrix::Multiply(AtA,At,LSQ);

  AtA->Delete();
  At->Delete();
  A->Delete();
  return LSQ;

}
// ---------------------------------------------------------------------------------------------------------------------------
/*void vtkbisRemoveTemporalDrift::StoreVoxelIndexIntoColumn(vtkDataArray* dat,int index,vtkpxMatrix* colvect,int nc)
{
  for (int i=0;i<nc;i++)
    colvect->SetDirectElement(i,0,dat->GetComponent(index,i));
}*/
//----------------------------------------------------------------------------------------------------------------------------
void vtkbisRemoveTemporalDrift::EliminatePolynomial(vtkDataArray* inp,vtkDataArray* out,int index,int order,
						    int numframes,
						    vtkpxMatrix* LSQ,
						    vtkpxMatrix* temp1,vtkpxMatrix* temp2)
{
	double t,v,test;int p;
  //Store value of each frame of voxel x into temp1 vector
  for (int i=0;i<numframes;i++)
	  temp1->SetDirectElement(i,0,inp->GetComponent(index,i));

  //temp2 = LSQ*temp1
  vtkpxMatrix::Multiply(LSQ,temp1,temp2);
  //  temp2->Print("Params");

  //for every frame, subtract LSQ*temp1*polynomial(a very small number)
  for (int ia=0;ia<numframes;ia++)
    {
      t=this->ComputeTime(ia,numframes);
      v=inp->GetComponent(index,ia);
      //std::cout<<"OV:"<<v<<endl;
      test = 0;
      for (p=0;p<=order;p++)
      {
    	  //std::cout<<"t2:"<<temp2->GetDirectElement(p,0)<<"p:"<<Polynomial(t,p)<<endl;
    	  test+=temp2->GetDirectElement(p,0)*Polynomial(t,p);
    	  //std::cout<<"test:"<<test<<endl;
      }
      v =v-test;
    	  out->SetComponent(index,ia,v);
    	  //std::cout<<"NV:"<<v<<endl;
    }
}
// ---------------------------------------------------------------------------------------------------------------------------
//   Main Function
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisRemoveTemporalDrift::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{

  fprintf(stderr,"Beginning RemoveTemporalDrift use legendre=%d order=%d\n",
		  this->UseLegendrePolynomials,this->Order);


  if (this->ImageMask!=NULL)
    {
      int dim[3]; this->ImageMask->GetDimensions(dim);
      int dim2[3]; input->GetDimensions(dim2);
      int sum=0;
      for (int ia=0;ia<=2;ia++)
    	  sum+=abs(dim[ia]-dim2[ia]);
      if (sum>0)
      {
    	  fprintf(stderr,"Bad Mask Input to vtkbisRemoveTemporalDrift SimpleExecute\n");
    	  return;
      }
    }

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  int nt=inp->GetNumberOfTuples();
  int nc=inp->GetNumberOfComponents();

  vtkDataArray* out=output->GetPointData()->GetScalars();
  for (int ia=0;ia<nc;ia++)
    out->FillComponent(ia,0.0);

  vtkDataArray* msk=NULL;
  if (this->ImageMask!=NULL)
    msk=this->ImageMask->GetPointData()->GetScalars();

  int maxcount=int(nt/10);
  int count=0;
  double pog=0.0;

  vtkpxMatrix* LSQ=this->ComputeLSQMatrix(this->Order,nc);
  //LSQ->Print();
  vtkpxMatrix* temp1=vtkpxMatrix::New();
  temp1->Zeros(nc,1);

  vtkpxMatrix* temp2=vtkpxMatrix::New();
  temp2->Zeros(this->Order+1,1);


  for (int voxel=0;voxel<nt;voxel++)
  {
	  int doglm=1;
      if (msk!=NULL)
      {
    	  if (msk->GetComponent(voxel,0)<1)
    		  doglm=0;
      }

      if (doglm)
      {
    	  //std::cout<<"[";
    	  this->EliminatePolynomial(inp,out,voxel,this->Order,nc,LSQ,temp1,temp2);
    	  //std::cout<<"]"<<endl;
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

  LSQ->Delete();
  temp1->Delete();
  temp2->Delete();
  this->UpdateProgress(1.0);
}


