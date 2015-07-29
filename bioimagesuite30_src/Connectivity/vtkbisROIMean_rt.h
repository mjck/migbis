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
 * vtkbisROIMean_rt.h
 *
 *  Created on: Sep 26, 2008
 *      Author: Isabella Murphy; Xilin Lin
 *      calculate the mean time series of each ROI region
 */

#ifndef VTKBISROIMEAN_H_
#define VTKBISROIMEAN_H_

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include <string>
#include <iostream>

class vtkInformation;
class vtkPoints;
class vtkInformationVector;


class vtkbisROIMean_rt : public vtkSimpleImageToImageFilter {

public:

  static vtkbisROIMean_rt *New();
  vtkTypeMacro(vtkbisROIMean_rt,vtkSimpleImageToImageFilter);

  // ROI Image
  vtkSetObjectMacro(ImageROI,vtkImageData);
  vtkGetObjectMacro(ImageROI,vtkImageData);

  // ROI Centroids
  vtkGetObjectMacro(Centroids,vtkPoints);

  // File name
  virtual void SetFileName(const char* filename);
  virtual const char* GetFileName();

  //DoText
  vtkSetClampMacro(DoText,int,0,1);
  vtkGetMacro(DoText,int);

  //MinThreshold
  vtkSetClampMacro(MinThreshold,int,-1,10000);
  vtkGetMacro(MinThreshold,int);

  //AddRegion
  vtkSetClampMacro(AddRegion,int,0,1);
  vtkGetMacro(AddRegion,int);

  //NumberOfRegions
  vtkGetMacro(NumberOfRegions,int);


  // Mask Image
  vtkSetObjectMacro(ImageMask,vtkImageData);
  vtkGetObjectMacro(ImageMask,vtkImageData);

protected:
  vtkbisROIMean_rt();
  virtual ~vtkbisROIMean_rt();
  virtual int RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector,
				 vtkInformationVector *outputVector);

  virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);

  vtkImageData* ImageROI;
  int NumberOfRegions;
  vtkPoints*    Centroids;
  int MinThreshold;
  //BTX
  std::string FileName;
  //ETX

  int DoText;
  int AddRegion;
  vtkImageData* ImageMask;


private:
  vtkbisROIMean_rt(const vtkbisROIMean_rt& src){};
  vtkbisROIMean_rt& operator=(const vtkbisROIMean_rt& rhs){};
};

#endif /* VTKBISROIMEAN_H_ */

