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
 * vtkbisConnectivityUtility.h
 *
 *  Created on: Feb 19, 2009
 *      Author: Isabella Murphy
 *      trunk3
 */

#ifndef VTKBISCONNECTIVITYUTILITY_H_
#define VTKBISCONNECTIVITYUTILITY_H_

#include "vtkObject.h"
#include "vtkImageData.h"
#include "vtkIntArray.h"
#include "vtkpxMatrix.h"
#include "vtkpxImageRobustHistogram.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"


class vtkbisConnectivityUtility : public vtkObject{
public:
  vtkTypeMacro(vtkbisConnectivityUtility,vtkObject);
  static vtkbisConnectivityUtility *New();

  //static functions
  static double ComputeRatio(vtkImageData* input, vtkImageData* mask, double threshold);
  static void ImagetoText(vtkImageData* input, const char *filename);
  static void TexttoImage(const char* filename,vtkImageData* out);
  static int TexttoMarix6Cols(const char* filename, vtkpxMatrix* outmatrix);
  static int SplitBlocks(vtkpxMatrix* matrix , int column, int numrows, vtkIntArray* output);
  static void VasicekExact(double* v,int len,int m);

  // Descripition:
  // Input is input
  // Output is result
  // Frames is a two-component array where each tuple is a block with limits in component0:component1 (inclusive)
  static int ExtractImageBlocks(vtkImageData* input, vtkImageData* result, vtkIntArray* frames);

  // Eliminate frames from image
  static int DiscardImageBlocks(vtkImageData* input, vtkImageData* result, vtkIntArray* frames);

  // Other stuff
  static void Ztransform(vtkImageData* zmap, vtkImageData* output);
  static void ROCCurve(vtkImageData* mask, vtkImageData* groundtruth, vtkImageData* segmentedimage, int steps, vtkDoubleArray* output,const char* Filename);

};

#endif /* VTKBISCONNECTIVITYUTILITY_H_ */

