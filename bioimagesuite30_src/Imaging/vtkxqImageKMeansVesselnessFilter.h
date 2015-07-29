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





#ifndef __vtkxqImageKMeansVesselnessFilter_h
#define __vtkxqImageKMeansVesselnessFilter_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"

class vtkxqImageKMeansVesselnessFilter : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkxqImageKMeansVesselnessFilter *New();
  vtkTypeMacro(vtkxqImageKMeansVesselnessFilter,vtkpxSimpleImageToImageFilter);

  // Description:
  // Generate  Image
  vtkSetClampMacro(ComputeAuxOutput,int,0,1);
  vtkGetMacro(ComputeAuxOutput,int);
  
  // Description
  // Sigma Filtering For Cluster Size
  vtkSetMacro(Sigma,double);
  vtkGetMacro(Sigma,double);

  // Description:
  // Beta value for entropy computation, ignored otherwise
  vtkSetMacro(Beta,double);
  vtkGetMacro(Beta,double);

  // Description:
  // Alpha value for entropy computation, raise input image to Alpha prior to clustering
  vtkSetMacro(Alpha,double);
  vtkGetMacro(Alpha,double);


  // Description:
  // ODF Image if stored
  // xq: Brightness mask
  vtkGetObjectMacro(AuxOutput,vtkImageData);

  // Description:
  // ODF Image if stored
  // xq: oriented mean
  vtkSetObjectMacro(SecondInput,vtkImageData);
  vtkGetObjectMacro(SecondInput,vtkImageData);

protected:

  vtkxqImageKMeansVesselnessFilter();
  virtual ~vtkxqImageKMeansVesselnessFilter();
  vtkxqImageKMeansVesselnessFilter(const vtkxqImageKMeansVesselnessFilter&) {};
  void operator=(const vtkxqImageKMeansVesselnessFilter&) {};

  // Description:
  // Basic Input Outputs
  virtual void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData* inp,vtkImageData* out);

  // Description:
  // Data
  double        Sigma;
  double        Beta;
  double        Alpha;
  vtkImageData* AuxOutput;
  vtkImageData* SecondInput;
  int           ComputeAuxOutput;

  // Description:
  virtual int    ComputeClustering(int np,double* data,double* data2,double mean[2],double sigma[2],double meanint[2],double sigmaint[2],int debug=0);
  virtual void ComputeMeasure(vtkDataArray* inp,vtkDataArray* inp2,vtkDataArray* out,vtkDataArray* aux);

};

#endif

