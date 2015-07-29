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

// Renamed vtkpxSimpleImageToImageFilter

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkInformation.h"

vtkCxxRevisionMacro(vtkpxSimpleImageToImageFilter, "$Revision: 1.11 $");

//----------------------------------------------------------------------------
vtkpxSimpleImageToImageFilter::vtkpxSimpleImageToImageFilter()
{
  this->NumberOfRequiredInputs = 1;
  this->SetNumberOfInputPorts(1);
}

//----------------------------------------------------------------------------
vtkpxSimpleImageToImageFilter::~vtkpxSimpleImageToImageFilter()
{
}

//----------------------------------------------------------------------------
void vtkpxSimpleImageToImageFilter::SetInput(vtkImageData *input)
{
  this->vtkProcessObject::SetNthInput(0, input);
}

//----------------------------------------------------------------------------
vtkImageData *vtkpxSimpleImageToImageFilter::GetInput()
{
  if (this->NumberOfInputs < 1)
    {
    return NULL;
    }
  
  return (vtkImageData *)(this->Inputs[0]);
}

void vtkpxSimpleImageToImageFilter::ExecuteInformation()
{
  vtkImageData *input = this->GetInput();
  vtkImageData *output = this->GetOutput();

  // Make sure the Input has been set.
  if ( input == NULL)
    {
    vtkErrorMacro(<< "ExecuteInformation: Input is not set.");
    return;
    }

  // Start with some defaults.
  output->CopyTypeSpecificInformation( input );
}


void vtkpxSimpleImageToImageFilter::ComputeInputUpdateExtent( int inExt[6], 
                                                            int vtkNotUsed(outExt)[6] )
{
  vtkImageData *input = this->GetInput();
  // Make sure the Input has been set.
  if ( input == NULL)
    {
    vtkErrorMacro(<< "ComputeInputUpdateExtent: Input is not set.");
    return;
    }
  int* wholeExtent = input->GetWholeExtent();
  memcpy(inExt,wholeExtent,sizeof(int)*6);
}

void vtkpxSimpleImageToImageFilter::ExecuteData(vtkDataObject *vtkNotUsed(out))
{

  vtkDebugMacro("Executing.");
  vtkImageData* output = this->GetOutput();
  vtkImageData* input = this->GetInput();

  if (!input)
    {
    vtkErrorMacro("No input is specified!");
    return;
    }

  // Too many filters have floating point exceptions to execute
  // with empty input/ no request.
  if (this->UpdateExtentIsEmpty(output))
    {
    return;
    }

  // Set the extent of the output and allocate memory.
  output->SetExtent(output->GetWholeExtent());
  output->AllocateScalars();

  this->SimpleExecute(input, output);
}

//----------------------------------------------------------------------------
void vtkpxSimpleImageToImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

int vtkpxSimpleImageToImageFilter::FillInputPortInformation(int port,vtkInformation* info)
{
  if(!this->Superclass::FillInputPortInformation(port, info))
    {
      return 0;
    }
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkImageData");
  return 1;
}


// ----------------------------------------------------------------------------
void vtkpxSimpleImageToImageFilter::AddInput(vtkImageData *input)
{
  this->vtkProcessObject::AddInput(input);
}

//----------------------------------------------------------------------------
void vtkpxSimpleImageToImageFilter::SetInput(int idx, vtkImageData *input)
{
  this->vtkProcessObject::SetNthInput(idx, input);
}

//----------------------------------------------------------------------------
vtkImageData *vtkpxSimpleImageToImageFilter::GetInput(int idx)
{
  if (this->NumberOfInputs <= idx)
    {
      return NULL;
    }
  
  return (vtkImageData*)(this->Inputs[idx]);
}
// ----------------------------------------------------------------------------

