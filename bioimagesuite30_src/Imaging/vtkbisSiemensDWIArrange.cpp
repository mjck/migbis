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





#include "vtkbisSiemensDWIArrange.h"
#include "vtkImageExtractComponents.h"
#include "vtkbisImageReslice.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include <stdio.h>
#include "pxutil.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "nifti1_io.h"


vtkbisSiemensDWIArrange* vtkbisSiemensDWIArrange::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisSiemensDWIArrange");
  if(ret)
    {
      return (vtkbisSiemensDWIArrange*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisSiemensDWIArrange;
}


// Construct object to extract all of the input data.
vtkbisSiemensDWIArrange::vtkbisSiemensDWIArrange()
{
  this->NumberOfSlices=1;
}

// ---------------------------------------------------------------------------

void vtkbisSiemensDWIArrange::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkSimpleImageToImageFilter::PrintSelf(os,indent);
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
int vtkbisSiemensDWIArrange::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector, 
					  vtkInformationVector *outputVector)
{
  vtkInformation *inInfo =  inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  int wholeExtent[6]; inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExtent);
  double sp[3];  inInfo->Get(vtkDataObject::SPACING(), sp);
  double origin[3];  inInfo->Get(vtkDataObject::ORIGIN(),  origin);

  int totalslices=wholeExtent[5]-wholeExtent[4]+1;
  int numframes=int(totalslices)/int(this->NumberOfSlices);
  
  wholeExtent[4]=0;
  wholeExtent[5]=this->NumberOfSlices-1;


  origin[0]=0.0; origin[1]=0.0; origin[2]=0.0;

  outInfo->Set(vtkDataObject::ORIGIN(), origin, 3);
  outInfo->Set(vtkDataObject::SPACING(), sp, 3);
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),wholeExtent,6);
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, numframes);
  return 1;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void vtkbisSiemensDWIArrange::SimpleExecute(vtkImageData* input, vtkImageData* output) 
{
  
  int indim[3]; input->GetDimensions(indim);
  int odim[3]; output->GetDimensions(odim);
  int ncout=output->GetNumberOfScalarComponents();
  int ncin=input->GetNumberOfScalarComponents();

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();

  /*  fprintf(stderr,"odim=%dx%dx%d,%d idim=%dx%dx%d,%d\n",
	  odim[0],odim[1],odim[2],ncout,
	  indim[0],indim[1],indim[2],ncin);*/
  
  for (int ia=0;ia<ncout;ia++)
    out->FillComponent(0,0.0);

  int slizesize=odim[1]*odim[0];
  int inindex=0;
  for (int newz=0;newz<odim[2];newz++)
    for (int newt=0;newt<ncout;newt++)
      {
	int outindex=newz*slizesize;
	for (int pixel=0;pixel<slizesize;pixel++)
	  {
	    out->SetComponent(outindex,newt,inp->GetComponent(inindex,0));
	    ++inindex;
	    ++outindex;
	  }
      }
}


