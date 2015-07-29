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


/*!
 * vtkbisTimeSeriesCorrelation.h
 *
 *  Created on: Sep 29, 2008
 *      Author: Isabella Murphy; Xilin Lin
 *      given a set of average time courses compute a correlation matrix among them
 *      also called matrix correlation
 */

#ifndef _VTKBISTIMESERIESCORRELATION_H
#define _VTKBISTIMESERIESCORRELATION_H

#include "vtkSimpleImageToImageFilter.h"
#include <string>
#include <iostream>
#include "vtkFloatArray.h"

class vtkImageData;
class vtkInformation;
class vtkInformationVector;

class vtkbisTimeSeriesCorrelation: public vtkSimpleImageToImageFilter {
public:
  static vtkbisTimeSeriesCorrelation *New();
  vtkTypeMacro(vtkbisTimeSeriesCorrelation,vtkSimpleImageToImageFilter);

  //filename
  virtual void SetFileName(const char* filename);
  virtual const char* GetFileName();
  // File name2
  virtual void SetFileName2(const char* filename2);
  virtual const char* GetFileName2();

  //DoText
  vtkSetClampMacro(DoText,int,0,1);
  vtkGetMacro(DoText,int);
  //OutputRaw
  vtkSetClampMacro(OutputRaw,int,0,1);
  vtkGetMacro(OutputRaw,int);

  // Compute Correlation Coefficient
  //

  static double ComputeCorrelationCoefficient(vtkDataArray* a1,int index1,vtkDataArray* a2,int index2);
  static double RhoToZConversion(double rho);

  // This function does no checking!
  static double ComputePreNormalizedCorrelationCoefficient(vtkDataArray* a1,int index1,vtkDataArray* a2,int index2,double factor,int nc);


protected:
  vtkbisTimeSeriesCorrelation();
  virtual ~vtkbisTimeSeriesCorrelation(){};

  virtual int RequestInformation (vtkInformation * request,
				  vtkInformationVector ** inputVector,
				  vtkInformationVector * outputVector);
  virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);



private:
  vtkbisTimeSeriesCorrelation(const vtkbisTimeSeriesCorrelation& src){};
  vtkbisTimeSeriesCorrelation& operator=(const vtkbisTimeSeriesCorrelation& rhs){};
  int DoText;
  int OutputRaw;

  //BTX
  std::string FileName;
  std::string FileName2;
  //ETX

};

#endif /* VTKBISTIMESERIESCORRELATION_H_ */

