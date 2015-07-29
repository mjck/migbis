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
 * vtkbisIpsilateralConnMap.h
 *
 *  Created on: Nov 13, 2008
 *      Author: Isabella Murphy, Xilin Shen
 *      given a brain time series image, calculate intrinsic connectivity within each half
 *      (left and right) of the brain
 *      @Input: a 4D vtkImageData object (Brain image) with time courses.
 *      @Output: a 3D vtkImageData object with intrinsic ipsilateral connectivity contrast.
 */
#ifndef VTKBISIPSILATERALINTRINSICCONNCONTRAST_H_
#define VTKBISIPSILATERALINTRINSICCONNCONTRAST_H_

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include <string>
#include <iostream>

class vtkInformation;
class vtkInformationVector;

class vtkbisIpsilateralConnMap : public vtkSimpleImageToImageFilter{
public:
	static vtkbisIpsilateralConnMap *New();
	vtkTypeMacro(vtkbisIpsilateralConnMap,vtkSimpleImageToImageFilter);

	// Threshold
	vtkSetClampMacro(Threshold,float,0.0,1.0);
	vtkGetMacro(Threshold,float);

	// Mask Image
	vtkSetObjectMacro(ImageMask,vtkImageData);
	vtkGetObjectMacro(ImageMask,vtkImageData);

	//Range
	vtkSetClampMacro(Range,int,1,3);
	vtkGetMacro(Range,int);

	//Abs
	vtkSetClampMacro(Abs,int,1,2);
	vtkGetMacro(Abs,int);

	//counter
	vtkSetObjectMacro(OutData2,vtkImageData);
	vtkGetObjectMacro(OutData2,vtkImageData);

	//OutpuRaw
//	vtkSetClampMacro(OutputRaw,int,0,1);
//	vtkGetMacro(OutputRaw,int);

	//Type
	vtkSetClampMacro(Type,int,1,9);
	vtkGetMacro(Type,int);

	//DoContralateral
	vtkSetClampMacro(DoContralateral,int,0,1);
	vtkGetMacro(DoContralateral,int);

	virtual float CalculateGoodSum (float sum,int& counter);
	//virtual void OverThreshold (double sum,int& counter);

	// File name
	virtual void SetFileName(const char* filename);
	virtual const char* GetFileName();

	vtkSetMacro(LMean,double);
	vtkGetMacro(LMean,double);
	vtkSetMacro(LStd,double);
	vtkGetMacro(LStd,double);
	vtkSetMacro(RMean,double);
	vtkGetMacro(RMean,double);
	vtkSetMacro(RStd,double);
	vtkGetMacro(RStd,double);
	vtkSetMacro(LDegreeMean,double);
	vtkGetMacro(LDegreeMean,double);
	vtkSetMacro(LDegreeStd,double);
	vtkGetMacro(LDegreeStd,double);
	vtkSetMacro(RDegreeMean,double);
	vtkGetMacro(RDegreeMean,double);
	vtkSetMacro(RDegreeStd,double);
	vtkGetMacro(RDegreeStd,double);
	vtkSetMacro(LSize,int);
	vtkGetMacro(LSize,int);
	vtkSetMacro(RSize,int);
	vtkGetMacro(RSize,int);
protected:
	vtkbisIpsilateralConnMap();
	virtual ~vtkbisIpsilateralConnMap();
	virtual int RequestInformation (vtkInformation * request,
				vtkInformationVector ** inputVector,
				vtkInformationVector * outputVector) ;
	virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);
	virtual void OldExecute(vtkImageData* input ,vtkImageData* output);

private:
	vtkbisIpsilateralConnMap(const vtkbisIpsilateralConnMap& src){};
	vtkbisIpsilateralConnMap& operator=(const vtkbisIpsilateralConnMap& rhs){};
	float Threshold;
	vtkImageData* ImageMask;
	int Range;
	int Abs;
	vtkImageData* OutData2;
	//int LookN;
	int Optimize;
	int Type;
	//int OutputRaw;
	int DoContralateral;
	//BTX
	std::string FileName;
	//ETX
	double LMean;
	double LStd;
	double RMean;
	double RStd;
	double LDegreeMean;
	double LDegreeStd;
	double RDegreeMean;
	double RDegreeStd;
	int LSize;
	int RSize;

};

#endif /* VTKBISIPSILATERALINTRINSICCONNCONTRAST_H_ */










