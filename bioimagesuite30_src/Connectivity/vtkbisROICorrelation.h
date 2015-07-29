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
 * vtkbisROICorrelation.h
 *
 *  Created on: Sep 30, 2008
 *      Author: Isabella Murphy, Xilin Lin
 *      @Input: a 4D vtkImageData object (Brain image) with average time courses and a 4D ROI average time courses image
 *      @Output: a 4D vtkImageData object with correlation of the two input images
 */

#ifndef VTKBISROICORRELATION_H_
#define VTKBISROICORRELATION_H_

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"

class vtkInformation;
class vtkInformationVector;

class vtkbisROICorrelation : public vtkSimpleImageToImageFilter{
public:
	static vtkbisROICorrelation *New();
	vtkTypeMacro(vtkbisROICorrelation,vtkSimpleImageToImageFilter);

	// ROI Image
	vtkSetObjectMacro(AverageROI,vtkImageData);
	vtkGetObjectMacro(AverageROI,vtkImageData);

	// Mask Image
	vtkSetObjectMacro(ImageMask,vtkImageData);
	vtkGetObjectMacro(ImageMask,vtkImageData);

  // Raw Correlation vs Z-value
  vtkSetClampMacro(OutputRaw,int,0,1);
  vtkGetMacro(OutputRaw,int);


protected:
	vtkbisROICorrelation();
	virtual ~vtkbisROICorrelation();
	virtual int RequestInformation (vtkInformation * request,
			vtkInformationVector ** inputVector,
			vtkInformationVector * outputVector) ;

	virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);

	vtkImageData* AverageROI;
	vtkImageData* ImageMask;
  int OutputRaw;

private:
	vtkbisROICorrelation(const vtkbisROICorrelation& src){};
	vtkbisROICorrelation& operator=(const vtkbisROICorrelation& rhs){};
};

#endif /* VTKBISROICORRELATION_H_ */












