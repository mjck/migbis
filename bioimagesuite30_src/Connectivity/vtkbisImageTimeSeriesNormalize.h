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

// .NAME vtkbisImageTimeSeriesNormalize - takes a time series and normalizes it to have zero mean and unit standard deviation

#ifndef __vtkbisImageTimeSeriesNormalize_h
#define __vtkbisImageTimeSeriesNormalize_h


#include "vtkThreadedImageAlgorithm.h"

class vtkbisImageTimeSeriesNormalize : public vtkThreadedImageAlgorithm
{
public:
  static vtkbisImageTimeSeriesNormalize *New();
  vtkTypeRevisionMacro(vtkbisImageTimeSeriesNormalize,vtkThreadedImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  //NumOfFrames
  //vtkGetMacro(NumOfFrames,int);
  //vtkSetMacro(NumOfFrames,int);

  // Description:
  // Normalize Mean and Sigma
  /*  vtkSetClampMacro(NormalizeMean,int,0,1);
  vtkGetMacro(NormalizeMean,int);
  vtkBooleanMacro(NormalizeMean,int)

  vtkSetClampMacro(NormalizeSigma,int,0,1);
  vtkGetMacro(NormalizeSigma,int);
  vtkBooleanMacro(NormalizeSigma,int)*/


protected:
  vtkbisImageTimeSeriesNormalize();
  ~vtkbisImageTimeSeriesNormalize();

  //  int NormalizeMean;
  //  int NormalizeSigma;


  virtual int RequestInformation(vtkInformation*,
                                 vtkInformationVector**,
                                 vtkInformationVector*);

  virtual void ThreadedRequestData(vtkInformation*,
                                   vtkInformationVector**,
                                   vtkInformationVector*,
                                   vtkImageData*** inData,
                                   vtkImageData** outData,
                                   int outExt[6],
                                   int threadId);
private:
  vtkbisImageTimeSeriesNormalize(const vtkbisImageTimeSeriesNormalize&);  // Not implemented.
  void operator=(const vtkbisImageTimeSeriesNormalize&);  // Not implemented.
  //int NumOfFrames;
};

#endif

