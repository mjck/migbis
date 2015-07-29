

//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
//BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxMaximumImages.h,v $
  Language:  C++
  Date:      $Date: 2002/09/26 18:08:12 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
// .NAME vtkpxMaximumImages -- combines images to form either mean or median
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>

#ifndef __vtkmpjMaximumImages_h
#define __vtkmpjMaximumImages_h

#include "vtkpxSimpleImageToImageFilter.h"

class vtkmpjMaximumImages : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkmpjMaximumImages *New();
  vtkTypeMacro(vtkmpjMaximumImages,vtkpxSimpleImageToImageFilter);

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
  // Get one input to this filter
  vtkImageData *GetInput(int num);
  vtkImageData *GetInput();

protected:

  //BTX
  vtkmpjMaximumImages();
  virtual ~vtkmpjMaximumImages();

  vtkmpjMaximumImages(const vtkmpjMaximumImages&) {};
  void operator=(const vtkmpjMaximumImages&) {};

  virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);

  //ETX
};

#endif


