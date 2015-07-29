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

// .NAME vtkpxSimpleImageToImageFilter - Generic image filter with one input.
// .SECTION Description
// vtkpxSimpleImageToImageFilter is a filter which aims to avoid much
// of the complexity associated with vtkImageToImageFilter (i.e.
// support for pieces, multi-threaded operation). If you need to write
// a simple image-image filter which operates on the whole input, use
// this as the superclass. The subclass has to provide only an execute
// method which takes input and output as arguments. Memory allocation
// is handled in vtkpxSimpleImageToImageFilter. Also, you are guaranteed to
// have a valid input in the Execute(input, output) method. By default, 
// this filter
// requests it's input's whole extent and copies the input's information
// (spacing, whole extent etc...) to the output. If the output's setup
// is different (for example, if it performs some sort of sub-sampling),
// ExecuteInformation has to be overwritten. As an example of how this
// can be done, you can look at vtkImageShrink3D::ExecuteInformation.
// For a complete example which uses templates to support generic data
// types, see  vtkpxSimpleFilterExample.

// .SECTION See also
// vtkImageToImageFilter vtkpxSimpleImageFilterExample

#ifndef __vtkpxSimpleImageToImageFilter_h
#define __vtkpxSimpleImageToImageFilter_h

#include "vtkImageSource.h"

class  vtkpxSimpleImageToImageFilter : public vtkImageSource
{
public:
  vtkTypeRevisionMacro(vtkpxSimpleImageToImageFilter,vtkImageSource);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the Input of a filter. 
  virtual void SetInput(vtkImageData *input);
  vtkImageData *GetInput();

  // Multiple Input Stuff
  // --------------------
  // Description:
  // Set an Input of this filter. 
  virtual void SetInput(int num, vtkImageData *input);

  // Description:
  // Adds an input to the first null position in the input list.
  // Expands the list memory if necessary
  virtual void AddInput(vtkImageData *input);
  
  // Description:
  // Get one input to this filter.
  vtkImageData *GetInput(int num);

  
  // Description:
  // More Compatibility Hacks
  vtkImageData* GetImageDataInput(int n) { return this->GetInput(n);}
  int GetNumberOfInputPorts() { return this->GetNumberOfInputs();}


protected:
  vtkpxSimpleImageToImageFilter();
  ~vtkpxSimpleImageToImageFilter();

  // These are called by the superclass.
  // You might have to override ExecuteInformation
  virtual void ExecuteInformation();
  virtual void ComputeInputUpdateExtent(int inExt[6], int outExt[6]);

  // You don't have to touch this unless you have a good reason.
  virtual void ExecuteData(vtkDataObject *output);
  // In the simplest case, this is the only method you need to define.
  virtual void SimpleExecute(vtkImageData* input, vtkImageData* output) = 0;

  // This is needed for VTK5
  virtual int FillInputPortInformation(int, vtkInformation*);

private:
  vtkpxSimpleImageToImageFilter(const vtkpxSimpleImageToImageFilter&);  // Not implemented.
  void operator=(const vtkpxSimpleImageToImageFilter&);  // Not implemented.
};

#endif








