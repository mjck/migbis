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
 * vtkbisTaskBasedSeedCorrelation.h
 *
 *  Created on: Jul 23, 2009
 *      Author: Isabella Murphy
 */

#ifndef VTKBISTASKBASEDSEEDCORRELATION_H_
#define VTKBISTASKBASEDSEEDCORRELATION_H_

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkpxMatrix.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkFloatArray.h"

class vtkbisTaskBasedSeedCorrelation : public vtkSimpleImageToImageFilter {
public:
	static vtkbisTaskBasedSeedCorrelation *New();
	vtkTypeMacro(vtkbisTaskBasedSeedCorrelation,vtkSimpleImageToImageFilter);

	// ROI Image
	vtkSetObjectMacro(ImageROI,vtkImageData);
	vtkGetObjectMacro(ImageROI,vtkImageData);

	//NumberOfRegions
	vtkGetMacro(NumberOfRegions,int);

	// Mask Image
	vtkSetObjectMacro(ImageMask,vtkImageData);
	vtkGetObjectMacro(ImageMask,vtkImageData);

	//Matrix
	vtkSetObjectMacro(Matrix,vtkpxMatrix);
	vtkGetObjectMacro(Matrix,vtkpxMatrix);

	//Task
	vtkGetMacro(Task,int);
	vtkSetClampMacro(Task,int,0,1);

	//AddRange
	vtkGetMacro(AddRegion,double);
	vtkSetClampMacro(AddRegion,double,0,100);

	//DoText
	vtkSetClampMacro(DoText,int,0,1);
	vtkGetMacro(DoText,int);

	// File name
	virtual void SetFileName(const char* filename);
	virtual const char* GetFileName();



protected:
	vtkbisTaskBasedSeedCorrelation();
	virtual ~vtkbisTaskBasedSeedCorrelation();
	virtual void CalculateROIMean(vtkFloatArray* inp,vtkFloatArray* roi,vtkDataArray* msk,vtkFloatArray* roimean,int nt,int nc,int startframe,int endframe);
	virtual void CalculateSeedCorrelation(vtkFloatArray* inp,vtkFloatArray*roimean,vtkDataArray* msk,int nt,int startframe,int endframe,vtkDataArray* out);
	int RequestInformation (vtkInformation * request,
				vtkInformationVector ** inputVector,
				vtkInformationVector * outputVector);
	virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);
private:
	vtkImageData* ImageROI;
	vtkImageData* ImageMask;
	vtkpxMatrix* Matrix;
	int NumberOfRegions;
	int Task;
	double AddRegion;
	int DoText;
	//BTX
	std::string FileName;
	//ETX
	vtkbisTaskBasedSeedCorrelation(const vtkbisTaskBasedSeedCorrelation& src){};
	vtkbisTaskBasedSeedCorrelation& operator=(const vtkbisTaskBasedSeedCorrelation& rhs){};
};

#endif /* VTKBISTASKBASEDSEEDCORRELATION_H_ */

