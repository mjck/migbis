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





#include "vtkpxImageNormalizeIntensity.h"
#include "vtkObjectFactory.h"
#include "vtkpxUtil.h"
#include "vtkImageToImageFilter.h"

//------------------------------------------------------------------------------
vtkpxImageNormalizeIntensity* vtkpxImageNormalizeIntensity::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageNormalizeIntensity");
  if(ret)
    {
    return (vtkpxImageNormalizeIntensity*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageNormalizeIntensity;
}

//----------------------------------------------------------------------------
// Constructor sets default values
vtkpxImageNormalizeIntensity::vtkpxImageNormalizeIntensity()
{
  this->InputLevel1=-1;
  this->InputLevel2=0;
  this->InputLevel3=1;
  this->InputLevel4=-1;

  this->OutputLevel0=-1;
  this->OutputLevel1=-1;
  this->OutputLevel2=0;
  this->OutputLevel3=1;
  this->OutputLevel4=-1;
}

//----------------------------------------------------------------------------
// This templated function executes the filter for any type of data.
template <class IT, class OT>
static void vtkpxImageNormalizeIntensityExecute(vtkpxImageNormalizeIntensity *self,
				      vtkImageData *inData, IT *inPtr,
				      vtkImageData *outData, OT *outPtr,
				      int outExt[6], int id)
{

  float InputLevel1=self->GetInputLevel1();
  float InputLevel2=self->GetInputLevel2();
  float InputLevel3=self->GetInputLevel3();
  float InputLevel4=self->GetInputLevel4();
  
  float OutputLevel0=self->GetOutputLevel0();
  float OutputLevel1=self->GetOutputLevel1();
  float OutputLevel2=self->GetOutputLevel2();
  float OutputLevel3=self->GetOutputLevel3();
  float OutputLevel4=self->GetOutputLevel4();


  // for preventing overflow
  float outTypeMin = outData->GetScalarTypeMin();
  float outTypeMax = outData->GetScalarTypeMax();

  float inTypeMin = inData->GetScalarTypeMin();
  float inTypeMax = inData->GetScalarTypeMax();

  // Set Outer Levels if set to default to correspond to typemin,typemax
  if (InputLevel1==-1)
    InputLevel1=inTypeMin;
  if (InputLevel4==-1)
    InputLevel4=inTypeMax;

  if (OutputLevel0<outTypeMin || OutputLevel0>outTypeMax)
    OutputLevel0=outTypeMin;

  if (OutputLevel1<OutputLevel0)
    OutputLevel1=OutputLevel0;
  else
    OutputLevel1=Frange(OutputLevel1,outTypeMin,outTypeMax);

  if (OutputLevel4==-1)
    OutputLevel4=outTypeMax;
  else
    OutputLevel4=Frange(OutputLevel4,outTypeMin,outTypeMax);
  
  OutputLevel2=Frange(OutputLevel2,outTypeMin,outTypeMax);
  OutputLevel3=Frange(OutputLevel3,outTypeMin,outTypeMax);


  // Eliminate Division by Zero Errors
  if (InputLevel1<=0)
    InputLevel1=0;

  if (InputLevel1>=InputLevel2)
    InputLevel2=InputLevel1+1.0;
  if (InputLevel2>=InputLevel3)
    InputLevel3=InputLevel2+1;
  if (InputLevel3>=InputLevel4)
    InputLevel4=InputLevel3+1;

  float scale1=(OutputLevel2-OutputLevel1)/(InputLevel2-InputLevel1);
  float scale2=(OutputLevel3-OutputLevel2)/(InputLevel3-InputLevel2);
  float scale3=(OutputLevel4-OutputLevel3)/(InputLevel4-InputLevel3);

  /*  fprintf(stderr, "Processing Levels Inputs  (%f,%f,%f,%f)\n Processing Levels Outputs (%f,%f,%f,%f,%f)\n Processing Levels Scales  (%f,%f,%f)\n\n",
	  InputLevel1,InputLevel2,InputLevel3,InputLevel4,
	  OutputLevel0,OutputLevel1,OutputLevel2,OutputLevel3,OutputLevel4,
	  scale1,scale2,scale3);*/

  int idxR, idxY, idxZ;
  int maxY, maxZ;
  vtkIdType inIncX, inIncY, inIncZ;
  vtkIdType outIncX, outIncY, outIncZ;
  int rowLength;
  unsigned long count = 0;
  unsigned long target;


  // find the region to loop over
  rowLength = (outExt[1] - outExt[0]+1)*inData->GetNumberOfScalarComponents();
  maxY = outExt[3] - outExt[2]; 
  maxZ = outExt[5] - outExt[4];
  target = (unsigned long)((maxZ+1)*(maxY+1)/50.0);
  target++;
  
  // Get increments to march through data 
  inData->GetContinuousIncrements(outExt, inIncX, inIncY, inIncZ);
  outData->GetContinuousIncrements(outExt, outIncX, outIncY, outIncZ);

  // Loop through ouput pixels
  for (idxZ = 0; idxZ <= maxZ; idxZ++)
    {
    for (idxY = 0; !self->AbortExecute && idxY <= maxY; idxY++)
      {
	if (!id) 
	  {
	    if (!(count%target))
	      {
		self->UpdateProgress(count/(50.0*target));
	      }
	    count++;
	  }
	
	for (idxR = 0; idxR < rowLength; idxR++)
	  {
	  // Pixel operation
	    float val = ((float)(*inPtr)); 
	    
	    if (val<InputLevel1)
	      {
		val=OutputLevel0;
	      }
	    else if (val<InputLevel2)
	      {
		val=(val-InputLevel1)*scale1+OutputLevel1;
	      }
	    else if (val<InputLevel3)
	      {
		val=(val-InputLevel2)*scale2+OutputLevel2;
	      }
	    else if (val<InputLevel4)
	      {
		val=(val-InputLevel3)*scale3+OutputLevel3;
	      }
	    else
	      {
		val=OutputLevel4;
	      }
	    
	    *outPtr = (OT)(val);
	    outPtr++;
	    inPtr++;
	  }
	
	outPtr += outIncY;
	inPtr += inIncY;
      }
    outPtr += outIncZ;
    inPtr += inIncZ;
    }
}


//----------------------------------------------------------------------------
// This method is passed a input and output data, and executes the filter
// algorithm to fill the output from the input.
// It just executes a switch statement to call the correct function for
// the datas data types.
void vtkpxImageNormalizeIntensity::ThreadedExecute(vtkImageData *inData, 
						   vtkImageData *outData,
						   int outExt[6], int id)
{
  void *inPtr = inData->GetScalarPointerForExtent(outExt);
  void *outPtr = outData->GetScalarPointerForExtent(outExt);
  
  switch (inData->GetScalarType())
    { 
      vtkTemplateMacro7(vtkpxImageNormalizeIntensityExecute,this, 
			inData, (VTK_TT *)(inPtr), 
			outData,(VTK_TT *)(outPtr), 
			outExt, id);

    default:
      vtkErrorMacro(<< "Execute: Unknown ScalarType");
      return;
    }
}



void vtkpxImageNormalizeIntensity::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkImageToImageFilter::PrintSelf(os,indent);

  os << indent << "InputLevels: "<<this->InputLevel1<<this->InputLevel2<<this->InputLevel3<<this->InputLevel4<<"\n";
  os << indent << "OutputLevels: "<<this->OutputLevel1<<this->OutputLevel2<<this->OutputLevel3<<this->OutputLevel4<<"\n";
}

void vtkpxImageNormalizeIntensity::SetInputToNull(vtkImageToImageFilter* source)
{
  source->SetInput(NULL);
}

