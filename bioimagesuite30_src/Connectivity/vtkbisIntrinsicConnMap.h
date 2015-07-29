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
 * vtkbisIntrinsicConnMap.h
 *
 *  Created on: Nov 3, 2008
 *      Author: Isabella Murphy, Xilin Shen
 *      given a set of time courses compute intrinsic connectivity mapping
 *      @Input: a 4D vtkImageData object (Brain image) with time courses
 *      @Output: a 3D vtkImageData object with intrinsic connectivity mapping
 */

#ifndef VTKBISINTRINSICCONNMAP_H_
#define VTKBISINTRINSICCONNMAP_H_

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include <string>
#include <iostream>

class vtkInformation;
class vtkInformationVector;

class vtkbisIntrinsicConnMap : public vtkSimpleImageToImageFilter{

public:
  static vtkbisIntrinsicConnMap *New();
  vtkTypeMacro(vtkbisIntrinsicConnMap,vtkSimpleImageToImageFilter);

  // Threshold
  vtkSetClampMacro(Threshold,float,0.0,1.0);
  vtkGetMacro(Threshold,float);

  // Mask Image
  vtkSetObjectMacro(ImageMask,vtkImageData);
  vtkGetObjectMacro(ImageMask,vtkImageData);

  //Second Output Image
  vtkSetObjectMacro(OutData2,vtkImageData);
  vtkGetObjectMacro(OutData2,vtkImageData);

  //Range
  vtkSetClampMacro(Range,int,1,3);
  vtkGetMacro(Range,int);

  //Abs
  vtkSetClampMacro(Abs,int,1,2);
  vtkGetMacro(Abs,int);

  //OutputRaw
  //vtkSetClampMacro(OutputRaw,int,0,1);
  //vtkGetMacro(OutputRaw,int);

  //lookN
  vtkSetClampMacro(Optimized,int,0,1);
  vtkGetMacro(Optimized,int);

  //Type
  vtkSetClampMacro(Type,int,1,9);
  vtkGetMacro(Type,int);

  // File name
  virtual void SetFileName(const char* filename);
  virtual const char* GetFileName();

  //Mean
  vtkSetMacro(Mean,double);
  vtkGetMacro(Mean,double);

  //Std
  vtkSetMacro(Std,double);
  vtkGetMacro(Std,double);

  //Mean of degree
  vtkSetMacro(DegreeMean,double);
  vtkGetMacro(DegreeMean,double);

  //Std of degree
  vtkSetMacro(DegreeStd,double);
  vtkGetMacro(DegreeStd,double);


protected:
  vtkbisIntrinsicConnMap();
  virtual ~vtkbisIntrinsicConnMap();
  virtual int RequestInformation (vtkInformation * request,
				  vtkInformationVector ** inputVector,
				  vtkInformationVector * outputVector) ;
  virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);
  virtual void OldExecute(vtkImageData* input,vtkImageData* output);

private:
  vtkbisIntrinsicConnMap(const vtkbisIntrinsicConnMap& src){};
  vtkbisIntrinsicConnMap& operator=(const vtkbisIntrinsicConnMap& rhs){};
  float Threshold;
  vtkImageData* ImageMask;
  vtkImageData* OutData2;
  int Range;
  int Abs;
  //int LookN;
  int Optimized;
  int Type;
  //int OutputRaw;
  //BTX
  std::string FileName;
  //ETX
  double Mean;
  double Std;
  double DegreeMean;
  double DegreeStd;

};

#endif /* VTKBISINTRINSICCONNMAP_H_ */



