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
 * vtkbisRemoveSliceMean.h
 *
 *  Created on: Aug 28, 2008
 *      Author: Isabella Murphy, Xilin Chen
 *
 * Subtract slice mean of each frame for each voxel
 */

#ifndef VTKBISREMOVESLICEMEAN_H_
#define VTKBISREMOVESLICEMEAN_H_

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"

class vtkbisRemoveSliceMean : public vtkSimpleImageToImageFilter{
public:
	static vtkbisRemoveSliceMean *New();
	vtkTypeMacro(vtkbisRemoveSliceMean,vtkSimpleImageToImageFilter);

	// Mask Image
	vtkSetObjectMacro(ImageMask,vtkImageData);
	vtkGetObjectMacro(ImageMask,vtkImageData);


protected:
	vtkbisRemoveSliceMean();
	virtual ~vtkbisRemoveSliceMean();

	virtual int RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector,
						  vtkInformationVector *outputVector);
	virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);
	virtual void RemoveMean(vtkDataArray* inp,vtkDataArray* out,double* outarr,int voxel,int numc);
	virtual void CalculateSliceMean(vtkDataArray* inpdata,vtkDataArray* mask,
			double* outarr,int numvoxels,int slice,int numtimepoints);

private:
	vtkbisRemoveSliceMean(const vtkbisRemoveSliceMean& src){};
	vtkbisRemoveSliceMean& operator=(const vtkbisRemoveSliceMean& rhs){};

	vtkImageData* ImageMask;

};

#endif /* VTKBISREMOVESLICEMEAN_H_ */




