

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

  vtkmpjLevelSetNeighborhoodExtractor.h
  
  Marcel Jackowski, Feb. 2005.
  
  This is a VTK translation of the itkLevelSetNeighborhoodExtractor from the 
  Insight Segmentation & Registration Toolkit.
  
  Notes: converted to take vtkDoubleArrays

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
  
  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notices for more information.
  
=========================================================================*/
#ifndef _vtkmpjLevelSetNeighborhoodExtractor_h
#define _vtkmpjLevelSetNeighborhoodExtractor_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageSource.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkImageAccumulate.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

/** \class LevelSetNeighborhoodExtractor
 * \brief Locate pixels of a particular level set.
 *
 * LevelSetNeighborhoodExtractor locates a particular level set in the input
 * level set. Specifically, the method Locate() fills two containers: one
 * containing pixels immediately inside the contour defined by the level set
 * and the other containing pixels immediately outside.  For each located
 * pixel, an estimated distance to the particular level set is also
 * calculated.
 *
 * The containers InsidePoints and OutsidePoints can then be used 
 * in FastMarchingImageFilter to produce a signed distance map from 
 * the specified level set.
 *
 * This class is templated over the image type representing
 * the level set.
 *
 * This class supports narrowbanding. If a input narrowband is
 * provided, the alogrithm will only search pixels within the
 * narrowband.
 *
 * Implemenation of this class is based on Chapter 11 of
 * "Level Set Methods and Fast Marching Methods", J.A. Sethian,
 * Cambridge Press, Second edition, 1999.
 *
 * \ingroup LevelSetSegmentation 
 * 
 */

class vtkmpjLevelSetNeighborhoodExtractor: public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjLevelSetNeighborhoodExtractor *New();
  vtkTypeMacro(vtkmpjLevelSetNeighborhoodExtractor,vtkpxSimpleImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set/Get Set the value of the level set to be located. Default is 0.
  vtkSetMacro(LevelSetValue, double);
  vtkGetMacro(LevelSetValue, double);

  // Description:
  // Set/Get the narrow band width. Default is 12.
  vtkSetMacro(NarrowBandwidth, double);
  vtkGetMacro(NarrowBandwidth, double);
  
  // Description:
  // Set/Get the narrowbanding flag. 
  vtkGetMacro(NarrowBanding, int);
  vtkSetMacro(NarrowBanding, int);
  vtkBooleanMacro(NarrowBanding, int);
  
  // Description:
  // Set/Get the input narrowband. 
  vtkSetObjectMacro(InputNarrowBand, vtkDoubleArray);
  vtkGetObjectMacro(InputNarrowBand, vtkDoubleArray);
  
  // Description:
  // Get the container of inside points.
  vtkGetObjectMacro(InsidePoints, vtkDoubleArray);

  // Description:
  // Get the container of outside points.
  vtkGetObjectMacro(OutsidePoints, vtkDoubleArray);

  // Description:
  // Set/Get Set value for infinity
  vtkSetMacro(LargeValue, double);
  vtkGetMacro(LargeValue, double);
  
  // Description:
  // Locate the level set. This method evokes the level set
  // location algorithm
  void Locate() { this->Update(); }

protected:
  vtkmpjLevelSetNeighborhoodExtractor();
  virtual ~vtkmpjLevelSetNeighborhoodExtractor();

  double LevelSetValue;

  vtkDoubleArray *InsidePoints;
  vtkDoubleArray *OutsidePoints;

  int NarrowBanding;
  double NarrowBandwidth;
  vtkDoubleArray *InputNarrowBand;

  double LargeValue;

  int LastPointIsInside;
 
  int NodesUsedIndex[3];
  double NodesUsedValue[3];

  int GetNodeIndexUsedInComputation(unsigned int idx) { return NodesUsedIndex[idx]; }
  double GetNodeValueUsedInComputation(unsigned int idx) { return NodesUsedValue[idx]; }

  virtual void Initialize();
  virtual double CalculateDistance( int index, int coord[3] );
  
  void ExecuteInformation();  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  
private:  
  vtkmpjLevelSetNeighborhoodExtractor(const vtkmpjLevelSetNeighborhoodExtractor&); //purposely not implemented
  void operator=(const vtkmpjLevelSetNeighborhoodExtractor&); //purposely not implemented
  
  void SortNodesUsed();
  void GenerateDataFull(vtkImageData *input, vtkImageData *output);
  void GenerateDataNarrowBand(vtkImageData *input, vtkImageData *output);

};

#endif
