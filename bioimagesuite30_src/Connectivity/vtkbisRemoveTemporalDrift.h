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
 * vtkbisRemoveTemporalDrift.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: Isabella Murphy, Xilin Chen
 *
 * Use Legendre Polynomial or power function to create a LSQ matrix
 * to model the potential drift of fMRI data
 */

#ifndef __vtkbisRemoveTemporalDrift_h
#define __vtkbisRemoveTemporalDrift_h

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkpxMatrix.h"


class vtkbisRemoveTemporalDrift : public vtkSimpleImageToImageFilter
{
public:
  static vtkbisRemoveTemporalDrift *New();
  vtkTypeMacro(vtkbisRemoveTemporalDrift,vtkSimpleImageToImageFilter);

  // Mask Image
  vtkSetObjectMacro(ImageMask,vtkImageData);
  vtkGetObjectMacro(ImageMask,vtkImageData);


  // Number of Tasks (last N columns of regressor matrix, first few columns are drift)
  vtkSetMacro(DebugIndex,int);
  vtkGetMacro(DebugIndex,int);

  // Description:
  // Order of Polynomial
  vtkSetClampMacro(Order,int,0,6);
  vtkGetMacro(Order,int);

  // Description:
  // UseLegendrePolynomials
  vtkSetClampMacro(UseLegendrePolynomials,int,0,1);
  vtkGetMacro(UseLegendrePolynomials,int);


protected:

  vtkbisRemoveTemporalDrift();
  virtual ~vtkbisRemoveTemporalDrift();

  virtual int RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector,
  					  vtkInformationVector *outputVector);
  virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);
  virtual vtkpxMatrix* ComputeLSQMatrix(int order,int numframes);
  virtual void   EliminatePolynomial(vtkDataArray* inp,vtkDataArray* out,int index,int order,
				     int numframes,
				     vtkpxMatrix* LSQ,
				     vtkpxMatrix* temp1,vtkpxMatrix* temp2);

  //Polynomial(t,order) where t =time and order=order of polynomial (0=constant,1=linear etc.)
  virtual double Polynomial(double t, int order);

  //return a “central” representation for time frame
  virtual double ComputeTime(int frame,int numframes);

  int   Order;
  vtkImageData* ImageMask;
  int  DebugIndex;
  int UseLegendrePolynomials;

private:
	vtkbisRemoveTemporalDrift(const vtkbisRemoveTemporalDrift& src){};
	vtkbisRemoveTemporalDrift& operator=(const vtkbisRemoveTemporalDrift& rhs){};
};
#endif

