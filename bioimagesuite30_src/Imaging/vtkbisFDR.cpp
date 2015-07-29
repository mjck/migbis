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
 * vtkbisFDR.cpp
 *
 *  Created on: Apr 17, 2009
 *      Author: Isabella Murphy, Xilin Shen
 *      @Input:3D image, all voxel values have transformed to p-value.
 *      @Output:3D image, 1 or 0
 */

#include <vtkbisFDR.h>
#include "vtkObjectFactory.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkSortDataArray.h"
#include "vtkTimerLog.h"
#include "vtkImageCast.h"
#include "vtkDoubleArray.h"
#include "math.h"
#include "vtkMath.h"
#include "vtkIntArray.h"


using namespace std;

vtkbisFDR* vtkbisFDR::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisFDR");
  if(ret)
  {
      return (vtkbisFDR*)ret;
  }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisFDR;
}
vtkbisFDR::vtkbisFDR() {
  this->Q=0;
  this->MaxPvalue=0.0;
  this->ImageMask = NULL;

}
vtkbisFDR::~vtkbisFDR()
{
  this->SetImageMask(NULL);
}
/*-------------------------------------------------------------------------------------
 * force the output data type to float, need to change extend information to let
 * the pipeline know the difference
 * @Set: output data type = float, number of components 1. First components set all the voxels with p(i)<=iq/Vc(V) to 1
 * and p(i)>iq/Vc(V) to 0.
 */

int vtkbisFDR::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector,
					  vtkInformationVector *outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_UNSIGNED_CHAR, 1);
  return 1;
}

// -------------------------------------------------------------------------------------

void vtkbisFDR::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (this->ImageMask!=NULL)
  {
      int dim[3]; this->ImageMask->GetDimensions(dim);
      int dim2[3]; input->GetDimensions(dim2);
      int sum=0;
      for (int ia=0;ia<=2;ia++)
    	  sum+=abs(dim[ia]-dim2[ia]);
      if (sum>0)
      {
    	  fprintf(stderr,"Bad Mask Input to vtkbisFDR SimpleExecute, input image and mask image have different dimension.\n");
    	  return;
      }
  }

  // --------------------------------------------

  double r = 0.5772;
  vtkTimerLog *timer = vtkTimerLog::New();

  vtkDataArray*   out=output->GetPointData()->GetScalars();
  // Cleanup first -- fill output with 1s
  out->FillComponent(0,0.0);

  vtkDataArray* msk=NULL;
  if (this->ImageMask!=NULL)
    msk=this->ImageMask->GetPointData()->GetScalars();

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  int nt=inp->GetNumberOfTuples();

  vtkDoubleArray* values=vtkDoubleArray::New();
  values->SetNumberOfComponents(2);
  values->Allocate(nt/2,nt/5);
  //  values->FillComponent(0,0.0);
  //  values->FillComponent(1,0.0);

  double val[2];

  // ----------------------------------------------------

  int maxcount=int(nt/10),count=0;
  double pog=0.0;
  this->UpdateProgress(0.01);// for the use interface to show progress

  for (int i = 0; i < nt; i++)
  {
      int usevoxel=1;
      if (msk!=NULL)
      {
    	  if (msk->GetComponent(i,0)<1)
    		  usevoxel=0;
      }
      if (usevoxel)
      {
    	  val[0]=double(i);
    	  val[1]=inp->GetComponent(i,0);
    	  values->InsertNextTuple2(val[0],val[1]);
      }
  }

  //sort the values array base on component 1
  timer->StartTimer();
  vtkSortDataArray::SortArrayByComponent(values,1);
  timer->StopTimer();
  cout << "Time to sort array: " << timer->GetElapsedTime() << " sec" <<","<< "voxels in image= " << nt << ", voxels in sorting array= " << values->GetNumberOfTuples() << "," << endl;
  nt=values->GetNumberOfTuples();
  double counter=double(nt);
  for (int i = 0; i < 11; i++)
    {
	  cout<<values->GetComponent(i, 1)<<"|";
    }

  for (int i = 0; i < nt-1; i++)
  {
      if (values->GetComponent(i, 1) > values->GetComponent(i+1, 1))
      {
    	  cout<<i<<":"<<values->GetComponent(i, 1)<<">"<<values->GetComponent(i+1, 1)<<"Array not properly sorted!" << endl;
    	  break;
      }
  }
  cout << "Array consistency check finished\n" << endl;
  this->UpdateProgress(0.5);

  //if p(i)<= i*q/(V*c(V)) = i*q/(V*(ln(V)+r)) = i*q/(V*(ln(V)+0.5772)), v(i)=1
  //       double constant=this->Q/(double)(double(counter)*(log(double(counter))+r));
  //       int i = 0;

  fprintf(stderr,"Q=%f, counter=%f r=%f\n",this->Q,counter,r);
  //double constant=this->Q/(counter*(log(counter)+r));
  double constant=this->Q/counter;
  fprintf(stderr,"Constant=%f\n",constant);

  double maxpvalue=values->GetComponent(0,1);
  fprintf(stderr,"Maxpvalue =%f\n",maxpvalue);
  //de
int counter2=0;
//de
  for (int i=0;i<nt;i++)
  {
      int voxelindex=(int)values->GetComponent(i,0);
      double v=values->GetComponent(i,1);
      double thr=double(i+1)*constant;

      if( v <= thr)
      {
    	  out->SetComponent(voxelindex,0,1);
    	     	  if (v>maxpvalue)
    		  maxpvalue=v;
    	     	  //de
    	     	  ++counter2;
    	     	  //de
      }
      ++count;
      if (count==maxcount)
      {
    	  pog=double(i)/double(nt);
    	  this->UpdateProgress(pog);
    	  count=0;
      }
  }
  cout<<"final count:"<<counter2<<"\n";
  this->MaxPvalue=maxpvalue;
  fprintf(stderr,"Final Maxpvalue =%f\n",maxpvalue);


  timer->Delete();
  values->Delete();
  this->UpdateProgress(1.0);
}

