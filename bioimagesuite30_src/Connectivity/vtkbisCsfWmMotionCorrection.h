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
 * vtkbisCsfWmMotionCorrection.h
 *
 *  Created on: May 29, 2009
 *      Author: Isabella Murphy, Xilin Shen
 *      calculate the mean of csf and white matter, construct a matrix [ csf_mean white matter_mean motion1 motion2 motion3 motion4 motion5 motion6 ]
 */

#ifndef VTKBISCSFWMMOTIONCORRECTION_H_
#define VTKBISCSFWMMOTIONCORRECTION_H_

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkpxMatrix.h"
#include <string>

using namespace std;

class vtkbisCsfWmMotionCorrection : public vtkSimpleImageToImageFilter
{
public:
	vtkTypeMacro(vtkbisCsfWmMotionCorrection,vtkSimpleImageToImageFilter);
	static vtkbisCsfWmMotionCorrection *New();

	// Mask Image
	vtkSetObjectMacro(ImageMask,vtkImageData);
	vtkGetObjectMacro(ImageMask,vtkImageData);

	// Motion Matrix
	vtkSetObjectMacro(MotionMatrix,vtkpxMatrix);
	vtkGetObjectMacro(MotionMatrix,vtkpxMatrix);

	// LSQ
	vtkSetObjectMacro(LSQ,vtkpxMatrix);
	vtkGetObjectMacro(LSQ,vtkpxMatrix);

	//Filename
	void SetFileName(const char* filename);
	const char* GetFileName();

	//doMotion
	vtkSetClampMacro(doMotion,int,0,1);
	vtkGetMacro(doMotion,int);

	//doCsfWm
	vtkSetClampMacro(doCsfWm,int,0,1);
	vtkGetMacro(doCsfWm,int);

protected:
	vtkbisCsfWmMotionCorrection();
	virtual ~vtkbisCsfWmMotionCorrection();
	virtual int RequestInformation (vtkInformation * request,
							vtkInformationVector ** inputVector,
							vtkInformationVector * outputVector);
	virtual void MotionErrorCorr(int numoftuples,int numofframes,vtkDataArray* inp,vtkDataArray* out,vtkDataArray* msk);
	virtual void CSFWMErrorCorr(int numoftuples,int numofframes,vtkDataArray* inp,vtkDataArray* out,vtkDataArray* msk);
	virtual void EliminateError(vtkDataArray* inp,vtkDataArray* out,int index,int numframes,vtkpxMatrix* LSQ,vtkpxMatrix* A);
	virtual void ComputeLSQMatrix(vtkpxMatrix* A, vtkpxMatrix* LSQ);

	// Execute Function -- main function
	virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);
private:
	vtkbisCsfWmMotionCorrection(const vtkbisCsfWmMotionCorrection& src) {};
	vtkbisCsfWmMotionCorrection& operator=(const vtkbisCsfWmMotionCorrection& rhs) {};
	vtkImageData* ImageMask;
	vtkpxMatrix* MotionMatrix;
	vtkpxMatrix* LSQ;
	//BTX
	std::string FileName;
	//ETX
	int doMotion;
	int doCsfWm;

};

#endif /* VTKBISCSFWMMOTIONCORRECTION_H_ */

