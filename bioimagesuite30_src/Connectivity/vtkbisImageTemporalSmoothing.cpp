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
 *
 * vtkbisImageTemporalSmoothing.cpp
 *
 *  Created on: Aug 20, 2008
 *      Author: Isabella Murphy,Xilin Shen
 *
 * Gaussian smoothing method
 * User can choose the value of sigma. The length of the kernel will be 1+2*radius.
 * Radius = 2sigma+0.5
 */

#include "vtkbisImageTemporalSmoothing.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "pxisinf.h"

// This creates the "New" Function
vtkStandardNewMacro(vtkbisImageTemporalSmoothing);

// Construct object to extract all of the input data.
vtkbisImageTemporalSmoothing::vtkbisImageTemporalSmoothing()
{
  this->Sigma=1.0;
  this->ImageMask=NULL;
}
vtkbisImageTemporalSmoothing::~vtkbisImageTemporalSmoothing()
{
  this->SetImageMask(NULL);
}

/*-------------------------------------------------------------------------------------
 * force the output data type to double, need to change extend information to let
 * the pipeline know the difference
 * @Set: output data type = double
 */
int vtkbisImageTemporalSmoothing::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector,
					  vtkInformationVector *outputVector)
{
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
	return 1;
}

vtkFloatArray* vtkbisImageTemporalSmoothing::CreateKernel(double sigma)
{

  int radius=int(2.0*sigma+0.5);
  int length=1+2*radius;
  int midpoint=radius;

  /*
    To create a pointer ordinarily
    vtkFloatArray* kernel=new vtkFloatArray();

    BUT

    constructor is protected! so you can't do this.

    Instead use the New method which is a static member of the class

  */
  vtkFloatArray* kernel=vtkFloatArray::New();
  kernel->SetNumberOfComponents(1);
  kernel->SetNumberOfTuples(length);

  double sum=0.0;
  double g=0.0;

  for (int i=0;i<length;i++)
    {
      double x=double(i-midpoint);
      if(sigma<0.00001)
      {
    	  g=1;
      }else{
    	  g=exp(-x*x/(2.0*sigma*sigma));
      }
      sum+=g;
      kernel->SetComponent(i,0,g);
    }
  if (sum<0.00001)
	  sum=1.0;
  if (isnan(sum) || isinf(sum))
	  sum=1.0;

  // Normalize
  for (int i=0;i<length;i++)
    {
      double g=kernel->GetComponent(i,0);
      if (isnan(g) || isinf(g))
      		  g=0.0;
      double gnorm=g/sum;
      kernel->SetComponent(i,0,gnorm);
            //fprintf(stderr,"Kernel Value %d = %.4f\n",i,kernel->GetComponent(i,0));
    }
  return kernel;
}

// ---------------------------------------------------------------------------
void vtkbisImageTemporalSmoothing::SmoothVoxel(vtkDataArray* inpdata,vtkDataArray* outdata,vtkFloatArray* kernel,int index,int numframes,int radius)
{
	double sum;int imgindex;int kernelindex;double imagevalue;double kerneldata;

  for (int f=0;f<numframes;f++)
    {
      sum=0.0;
      for (int t=-radius;t<=radius;t++)
	  {
    	  imgindex=f+t;
    	  if (imgindex<0)
    		  imgindex=0;
    	  else if (imgindex>=numframes)
    		  imgindex=numframes-1;

    	  kernelindex=t+radius;

    	  imagevalue=inpdata->GetComponent(index,imgindex);
    	  kerneldata=kernel->GetComponent(kernelindex,0);
    	  sum+=imagevalue*kerneldata;
	  }
		  outdata->SetComponent(index,f,sum);
    }
}
// ---------------------------------------------------------------------------
void vtkbisImageTemporalSmoothing::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
	fprintf(stderr,"Beginning ImageTemporalSmoothing use sigma=%.2f\n",
			  this->Sigma);

  if (this->ImageMask!=NULL)
  {
	  int dim[3]; this->ImageMask->GetDimensions(dim);
      int dim2[3]; input->GetDimensions(dim2);
      int sum=0;
      for (int ia=0;ia<=2;ia++)
      	sum+=abs(dim[ia]-dim2[ia]);
      if (sum>0)
      {
      	fprintf(stderr,"Bad Mask Input to vtkbisImageTemporalSmoothing SimpleExecute\n");
      	return;
      }
  }

  vtkDataArray*   inp=input->GetPointData()->GetScalars();
  int nt=inp->GetNumberOfTuples();
  int nc=input->GetNumberOfScalarComponents();

  vtkDataArray*   out=output->GetPointData()->GetScalars();
  // Cleanup first -- fill output with zeros
  for (int c=0;c<nc;c++)
    out->FillComponent(c,0.0);

  vtkFloatArray* kernel=this->CreateKernel(this->Sigma);
  int radius=int(2.0*this->Sigma+0.5);
  /*  for (int i=0;i<kernel->GetNumberOfTuples();i++)
      fprintf(stdout,"kernel %d = %f\n",i,kernel->GetComponent(i,0));*/

  vtkDataArray* msk=NULL;
  if (this->ImageMask!=NULL)
    msk=this->ImageMask->GetPointData()->GetScalars();

  int maxcount=int(nt/10);
  int count=0;
  double pog=0.0;

  this->UpdateProgress(0.01);
  int index=0;
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
		  this->SmoothVoxel(inp,out,kernel,voxel,nc,radius);
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
  this->UpdateProgress(1.0);
  kernel->Delete();
}


