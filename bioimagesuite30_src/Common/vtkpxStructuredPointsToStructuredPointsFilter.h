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





// .NAME vtkpxStructuredPointsToStructuredPointsFilter - abstract filter class
// .SECTION Description
// vtkpxStructuredPointsToStructuredPointsFilter is an abstract filter class 
// whose subclasses take on input structured points and generate
// structured points on output.

// .SECTION See Also
// vtkExtractVOI vtkImageDifference vtkSweptSurface
// vtkTransformStructuredPoints

#ifndef __vtkpxStructuredPointsToStructuredPointsFilter_h
#define __vtkpxStructuredPointsToStructuredPointsFilter_h

#include "vtkStructuredPointsToStructuredPointsFilter.h"
#include "vtkStructuredPoints.h"



class vtkpxStructuredPointsToStructuredPointsFilter : public vtkStructuredPointsToStructuredPointsFilter
{
public:
  vtkTypeMacro(vtkpxStructuredPointsToStructuredPointsFilter,vtkStructuredPointsToStructuredPointsFilter);


  // Description:
  // Construct object to extract all of the input data.
  static vtkpxStructuredPointsToStructuredPointsFilter *New();

  // Description:
  // Set / get the input data or filter.
  void SetInput(vtkStructuredPoints *input);
  void SetInput(vtkImageData *input);
  vtkStructuredPoints *GetInput();

  
protected:
  vtkpxStructuredPointsToStructuredPointsFilter() {this->NumberOfRequiredInputs = 1;};
  ~vtkpxStructuredPointsToStructuredPointsFilter() {};


  // Since input[0] and output are of same type, we can create this
  // method that defaults to just copying information.
  void ExecuteInformation();
  void ComputeInputUpdateExtents(vtkDataObject *output);


private:
  vtkpxStructuredPointsToStructuredPointsFilter(const vtkpxStructuredPointsToStructuredPointsFilter&);  
  void operator=(const vtkpxStructuredPointsToStructuredPointsFilter&);  

};

#endif



