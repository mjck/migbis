

//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
//BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkReinitializeLevelSetImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:38 $
  Version:   $Revision: 1.16 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _vtkmpjReinitializeLevelSetImageFilter_h
#define _vtkmpjReinitializeLevelSetImageFilter_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageSource.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkMath.h"
#include "math.h"

#include "vtkmpjFastMarchingImageFilter.h"
#include "vtkmpjLevelSetNeighborhoodExtractor.h"

/** \class ReinitializeLevelSetImageFilter
 *  \brief Reinitialize the level set to the signed distance function.
 *
 * ReinitializeLevelSetImageFilter reinitializes the input level set to
 * the approximated signed distance function from a particular 
 * level set. The output is a level set of the same type as the input.
 *
 * For some level set algorithms, it is useful to periodically
 * reinitialize the level set function to prevent numerical accuracy
 * problems in computing derivatives and curvature values where level
 * sets are densely bunched together.
 *
 * This class supports narrowbanding. If the input narrowband is provided,
 * the algorithm will only locate the level set within the input narrowband.
 * For the output, the reinitialize level set is only valid for a distance
 * of OutputNarrowBandwidth / 2 of either side of the level set of interest.
 *
 * Implementation of this class is based on Chapter 11 of
 * "Level Set Methods and Fast Marching Methods", J.A. Sethian,
 * Cambridge Press, Second edition, 1999.
 *
 * \ingroup LevelSetSegmentation 
 *
 */
class vtkmpjReinitializeLevelSetImageFilter: public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjReinitializeLevelSetImageFilter *New();
  vtkTypeMacro(vtkmpjReinitializeLevelSetImageFilter,vtkpxSimpleImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set/Get Set the value of the level set to be located. Default is 0.
  vtkSetMacro(LevelSetValue, double);
  vtkGetMacro(LevelSetValue, double);
  
  // Description:
  // Set/Get the narrowbanding flag. By default, narrowbanding is switched off
  vtkGetMacro(NarrowBanding, int);
  vtkSetMacro(NarrowBanding, int);
  vtkBooleanMacro(NarrowBanding, int);
  
  // Description:
  // Set/Get the input narrow bandwidth. Default is 12.
  vtkSetMacro(InputNarrowBandwidth, double);
  vtkGetMacro(InputNarrowBandwidth, double);
  
  // Description:
  // Set/Get the output narrow bandwidth. Default is 12.
  vtkSetMacro(OutputNarrowBandwidth, double);
  vtkGetMacro(OutputNarrowBandwidth, double);
  
  /** Set the bandwidth for both the input and output narrowband,
   * By default, both the input and output are set to 12. */
  void SetNarrowBandwidth( double value )
    {
      this->InputNarrowBandwidth=value;
      this->OutputNarrowBandwidth=value;
    }
  
  // Description:
  // Set the input narrowband.
  vtkSetObjectMacro(InputNarrowBand, vtkDataArray);
  
  // Description:
  // Get the input narrowband.
  vtkGetObjectMacro(OutputNarrowBand, vtkDataArray);
  
protected:
  vtkmpjReinitializeLevelSetImageFilter();
  virtual ~vtkmpjReinitializeLevelSetImageFilter();

  double LevelSetValue;

  vtkmpjLevelSetNeighborhoodExtractor *Locator;
  vtkmpjFastMarchingImageFilter *Marcher;
  
  vtkDoubleArray *InsidePoints;
  vtkDoubleArray *OutsidePoints;

  int NarrowBanding;
  double InputNarrowBandwidth;
  double OutputNarrowBandwidth;
  vtkDataArray *InputNarrowBand;
  vtkDataArray *OutputNarrowBand;
  
  void SetOutputNarrowBand( vtkDataArray *ptr )
    { OutputNarrowBand = ptr; }
  
  void ExecuteInformation();  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  
private:  
  vtkmpjReinitializeLevelSetImageFilter(const vtkmpjReinitializeLevelSetImageFilter&); //purposely not implemented
  void operator=(const vtkmpjReinitializeLevelSetImageFilter&); //purposely not implemented
  
  //void SortNodesUsed();
  void GenerateDataFull(vtkImageData *input, vtkImageData *output);
  void GenerateDataNarrowBand(vtkImageData *input, vtkImageData *output);

};

#endif
