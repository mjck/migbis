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

#ifndef __vtkbisObjectmapRegularization_h
#define __vtkbisObjectmapRegularization_h

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkDoubleArray.h"
#include "vtkShortArray.h"
#include "vtkIdList.h"

class vtkbisObjectmapRegularization : public vtkSimpleImageToImageFilter
{
public:
  static vtkbisObjectmapRegularization *New();
  vtkTypeMacro(vtkbisObjectmapRegularization,vtkSimpleImageToImageFilter);

  // Description:
  // Number Of Iterations (EM Iterations)
  vtkGetMacro(NumberOfIterations,int);
  vtkSetClampMacro(NumberOfIterations,int,0,20);

  // Description:
  // Weight of MRF Smoothness Term
  vtkGetMacro(Smoothness,double);
  vtkSetClampMacro(Smoothness,double,0,10000.0);


  // Description:
  // Max Percentage of Voxels changing needed for iteration to converge (0 == all voxels)
  vtkGetMacro(ConvergencePercentage,double);
  vtkSetClampMacro(ConvergencePercentage,double,0,0.4);

  // Description:
  // Debug Voxel ...
  vtkGetMacro(DebugVoxel,int);
  vtkSetMacro(DebugVoxel,int);



protected:

  vtkbisObjectmapRegularization();
  virtual ~vtkbisObjectmapRegularization();
  vtkbisObjectmapRegularization(const vtkbisObjectmapRegularization&) {};
  void operator=(const vtkbisObjectmapRegularization&) {};


  // Description:
  // Basic Stuff
  virtual void SimpleExecute(vtkImageData* in,vtkImageData* out);

  virtual int RequestInformation (vtkInformation * request,
				  vtkInformationVector ** inputVector,
				  vtkInformationVector * outputVector);

			      
  // M-Step
  // -----------------------------------

  virtual double ComputeTotalMinusLogProbability(double   intensity,
						 int      current_voxel,
						 double   current_label,
						 vtkDataArray* labels,
						 int incr[26],double wgth[26],
						 double smoothness);
  
  virtual int  ComputeMRFIncrementsAndWeights(vtkImageData* img,int incr[26],double wgt[26]);
  virtual int  ClassifyVoxel(int voxel_index,vtkDataArray* intensities,vtkDataArray* labels,vtkDataArray* outlabels,int incr[26],double wgth[26],
			     double smoothness,vtkIdList* nlist);
  /*  virtual double DoMaximizationStep(vtkImageData* intensity_image,
				    vtkImageData* label_image,
				    vtkImageData* output_label_image,
				    double smoothness,
				    int maxiter);*/



  // Descrtiption:
  // Key Variables
  double          Smoothness;
  double          ConvergencePercentage;
  int             NumberOfIterations;
  int             DebugVoxel;

};

#endif

