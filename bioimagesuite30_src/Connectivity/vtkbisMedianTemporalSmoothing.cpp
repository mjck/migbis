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
 * vtkbisMedianTemporalSmoothing.cpp
 *
 *  Created on: Aug 20, 2008
 *      Author: Isabella Murphy,Xilin Shen
 *      @Input:4D image
 *      @Output:Smoothed image using median. Kernel size = 2*radius+1.
 */
#include "vtkbisMedianTemporalSmoothing.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkpxMath.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkbisConnectivityUtility.h"
#include "vtkIntArray.h"


// This creates the "New" Function
vtkStandardNewMacro(vtkbisMedianTemporalSmoothing);

vtkbisMedianTemporalSmoothing::vtkbisMedianTemporalSmoothing()
{
  this->Radius=1;
  this->ImageMask=NULL;
  //this->Result=NULL;
}

vtkbisMedianTemporalSmoothing::~vtkbisMedianTemporalSmoothing()
{
  this->SetImageMask(NULL);
  //if (this->Result!=NULL)
  //       this->SetResult(NULL);
}
/*-------------------------------------------------------------------------------------
 * force the output data type to double, need to change extend information to let
 * the pipeline know the difference
 * @Set: output data type = double
 */
int vtkbisMedianTemporalSmoothing::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector,
					  vtkInformationVector *outputVector)
{
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
	return 1;
}
// ---------------------------------------------------------------------------
void vtkbisMedianTemporalSmoothing::SmoothVoxel(vtkDataArray* inpdata,vtkDataArray* outdata,int voxel,
					       int numframes,int radius)
{

  int length=2*radius+1;double imagevalue;int temparrindex;double median;

  //create a temporary array to hold the frames that is going
  //to pass into GetMeidan method to calculate median
  // 30 is a large number > max number needed
  float temparr[30];

  for (int f=0;f<numframes;f++)
    {
      for (int t=-radius;t<=radius;t++)
	{
    	  int frame=f+t;
    	  if (frame<0)
    		  frame=0;
    	  else if (frame>=numframes)
    		  frame=numframes-1;

    	  imagevalue=inpdata->GetComponent(voxel,frame);
    	  temparrindex=t+radius;
    	  temparr[temparrindex]=imagevalue;
	}
      median=vtkpxMath::GetMedian(length, temparr);
      outdata->SetComponent(voxel,f,median);
    }
}

// ---------------------------------------------------------------------------
void vtkbisMedianTemporalSmoothing::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
	fprintf(stderr,"Beginning vtkbisMedianTemporalSmoothing use radius=%d\n",
				  this->Radius);

	if (this->ImageMask!=NULL)
	{
		int dim[3]; this->ImageMask->GetDimensions(dim);
	    int dim2[3]; input->GetDimensions(dim2);
	    int sum=0;
	    for (int ia=0;ia<=2;ia++)
	     sum+=abs(dim[ia]-dim2[ia]);
	    if (sum>0)
	    {
	     fprintf(stderr,"Bad Mask Input to vtkbisMedianTemporalSmoothing SimpleExecute\n");
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


  int length=2*this->Radius+1;

  vtkDataArray* msk=NULL;
    if (this->ImageMask!=NULL)
      msk=this->ImageMask->GetPointData()->GetScalars();


  //fprintf(stderr,"The ratio is: %.2f",vtkbisConnectivityUtility::ComputeRatio(input, this->ImageMask, this->Threshold) );
//Test some functions has nothing to do with this class
  //vtkbisConnectivityUtility::ImagetoText( input, "test.txt");

  //vtkbisConnectivityUtility::TexttoImage("test.txt",result);
 /*   if (this->Result!=NULL)
        this->Result->Delete();
    Result = vtkImageData::New();

    vtkIntArray* frames = vtkIntArray::New();
    frames->SetNumberOfTuples(3);
    frames->SetNumberOfComponents(2);
    for (int ia=0;ia<2;ia++)
    {
    	frames->FillComponent(ia,0);
    }

    frames->SetComponent(0,0,1);
    frames->SetComponent(0,1,2);
    frames->SetComponent(1,0,4);
    frames->SetComponent(1,1,5);
    frames->SetComponent(2,0,7);
    frames->SetComponent(2,1,8);
    vtkbisConnectivityUtility::ExtractImageBlocks(input,Result,frames);
    frames->Delete();
*/
  int maxcount=int(nt/10);
  int count=0;
  double pog=0.0;

  this->UpdateProgress(0.01);

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
		  this->SmoothVoxel(inp,out,voxel,nc,this->Radius);
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
}









