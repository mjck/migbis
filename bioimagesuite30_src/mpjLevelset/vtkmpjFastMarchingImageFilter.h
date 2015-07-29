

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

  vtkmpjFastMarchingImageFilter.h
  
  Marcel Jackowski, March 2005.
  
  This is a VTK translation of the itkFastMarchingImageFilter from the 
  Insight Segmentation & Registration Toolkit.
  
  Notes: A vtkDataArray object is used to store points coordinates and values.
         SetOutputSize has been translated into SetOutputExtent.

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
  
  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notices for more information.
  
=========================================================================*/
#ifndef __vtkmpjFastMarchingImageFilter_h
#define __vtkmpjFastMarchingImageFilter_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageSource.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkImageAccumulate.h"
#include "vtkDataArray.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

#include <queue>

/** \class FastMarchingImageFilter
 * \brief Solve an Eikonal equation using Fast Marching
 *
 * Fast marching solves an Eikonal equation where the speed is always
 * non-negative and depends on the position only. Starting from an 
 * initial position on the front, fast marching systematically moves the
 * front forward one grid point at a time.
 *
 * Updates are preformed using an entropy satisfy scheme where only
 * "upwind" neighborhoods are used. This implementation of Fast Marching
 * uses a std::priority_queue to locate the next proper grid position to
 * update. 
 * 
 * Fast Marching sweeps through N grid points in (N log N) steps to obtain
 * the arrival time value as the front propagates through the grid.
 *
 * Implementation of this class is based on Chapter 8 of
 * "Level Set Methods and Fast Marching Methods", J.A. Sethian,
 * Cambridge Press, Second edition, 1999.
 *
 * This class is templated over the level set image type and the speed
 * image type. The initial front is specified by two containers: one
 * containing the known points and one containing the trial
 * points. The speed function can be specified as a speed image or a
 * speed constant. The speed image is set using the method
 * SetInput(). If the speed image is NULL, a constant speed function
 * is used and is specified using method the SetSpeedConstant().
 *
 * If the speed function is constant and of value one, fast marching results
 * in an approximate distance function from the initial alive points.
 * FastMarchingImageFilter is used in the ReinitializeLevelSetImageFilter
 * object to create a signed distance function from the zero level set.
 *
 * The algorithm can be terminated early by setting an appropriate stopping
 * value. The algorithm terminates when the current arrival time being
 * processed is greater than the stopping value.
 *
 * There are two ways to specify the output image information 
 * ( Extent, Spacing ): (a) it is copied directly from
 * the input speed image or (b) it is specified by the user. Default values
 * are used if the user does not specify all the information.
 *
 * The output information is computed as follows.
 * If the speed image is NULL or if the OverrideOutputInformation is set to
 * true, the output information is set from user specified parameters. These 
 * parameters can be specified using methods SetOutputExtent() and SetOutputSpacing()
 * Else if the speed image is not NULL, the output information
 * is copied from the input speed image.
 *
 * Possible Improvements:
 * In the current implemenation, std::priority_queue only allows 
 * taking nodes out from the front and putting nodes in from the back.
 * To update a value already on the heap, a new node is added to the heap.
 * The defunct old node is left on the heap. When it is removed from the
 * top, it will be recognized as invalid and not used.
 * Future implementations can implement the heap in a different way
 * allowing the values to be updated. This will generally require 
 * some sift-up and sift-down functions and  
 * an image of back-pointers going from the image to heap in order
 * to locate the node which is to be updated.
 *
 * \sa LevelSetTypeDefault
 * \ingroup LevelSetSegmentation 
 */
class vtkmpjFastMarchingImageFilter : public vtkpxSimpleImageToImageFilter
{
 public: 

  /** Enum of Fast Marching algorithm point types. FarPoints represent far
   * away points; TrialPoints represent points within a narrowband of the
   * propagating front; and AlivePoints represent points which have already
   * been processed. */
  //BTX
  enum LabelType { FarPoint, AlivePoint, TrialPoint };
  //ETX
  
  static vtkmpjFastMarchingImageFilter *New();
  vtkTypeMacro(vtkmpjFastMarchingImageFilter,vtkpxSimpleImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set/Get the container of Alive Points representing the initial front
  vtkSetObjectMacro(AlivePoints, vtkDataArray);
  vtkGetObjectMacro(AlivePoints, vtkDataArray);

  // Description:
  // Set/Get the container of Trial Points representing the initial front
  vtkSetObjectMacro(TrialPoints, vtkDataArray);
  vtkGetObjectMacro(TrialPoints, vtkDataArray);
  
  // Description:
  // Get label image
  vtkGetObjectMacro(LabelImage, vtkImageData);
  
  // Description:
  // Set the Speed Constant. If the Speed Image is NULL,
  // the SpeedConstant value is used for the whole level set.
  // By default, the SpeedConstant is set to 1.0.
  void SetSpeedConstant( double value );
  
  // Description:
  // Get the Speed Constant
  vtkGetMacro(SpeedConstant, double);
  
  // Set/Get the Normalization Factor for the Speed Image.
  // The values in the Speed Image is divided by this
  // factor. This allows the use of images with
  // integer pixel types to represent the speed. 
  vtkGetMacro(NormalizationFactor, double);
  vtkSetMacro(NormalizationFactor, double);

  // Description:
  // Set/Get the Fast Marching algorithm Stopping Value. The Fast Marching
  // algorithm is terminated when the value of the smallest trial point
  // is greater than the stopping value.
  vtkGetMacro(StoppingValue, double);
  vtkSetMacro(StoppingValue, double);

  // Description:
  // Set/Get the Collect Points flag. Instrument the algorithm to collect
  // a container of all nodes which it has visited. Useful for
  // creating Narrowbands for level set algorithms that supports 
  // narrow banding. 
  vtkGetMacro(CollectPoints, int);
  vtkSetMacro(CollectPoints, int);
  vtkBooleanMacro(CollectPoints, int);
  
  // Description:
  // Get the container of Processed Points. If the CollectPoints flag
  // is set, the algorithm collects a container of all processed nodes.
  // This is useful for defining creating Narrowbands for level
  // set algorithms that supports narrow banding.
  vtkGetObjectMacro(ProcessedPoints, vtkDoubleArray);

  // Description:
  // The output extents and spacing is computed as follows.
  // If the speed image is NULL or if the OverrideOutputInformation is true, 
  // the output information is set from user specified parameters. These 
  // parameters can be specified using methods SetOutputExtent(), SetOutputSpacing().
  // Else if the speed image is not NULL, the output information
  // is copied from the input speed image. 
  void SetOutputExtent(int minX, int maxX, int minY, int maxY, int minZ, int maxZ);
  void SetOutputSpacing(double si, double sj, double sk) {
    OutputSpacing[0]=si; OutputSpacing[1]=sj; OutputSpacing[2]=sk;
  }
  vtkGetVectorMacro(OutputSpacing, double, 3);
  vtkSetVectorMacro(OutputSpacing, double, 3);
  
  vtkGetMacro(OverrideOutputInformation, int);
  vtkSetMacro(OverrideOutputInformation, int);
  vtkBooleanMacro(OverrideOutputInformation, int);
  
  // Description:
  // Set/Get value for infinity
  vtkGetMacro(LargeValue, double);
  vtkSetMacro(LargeValue, double);

 protected:

  vtkDataArray *AlivePoints;
  vtkDataArray *TrialPoints;
  
  vtkImageData *LabelImage;
  
  double SpeedConstant;
  double InverseSpeed;
  double StoppingValue;
  
  int CollectPoints;
  vtkDoubleArray *ProcessedPoints;
  
  int OutputExtent[6];
  double OutputSpacing[3];
  int OverrideOutputInformation;

  double NormalizationFactor;

  double LargeValue;

  //BTX
  /** Trial points are stored in a min-heap. This allow efficient access
   * to the trial point with minimum value which is the next grid point
   * the algorithm processes. */
  struct NodeType {
    double value;
    int index;
    NodeType(double value, int index) { this->index=index; this->value=value; };
  }; 
  
  struct NodeTypeCmp {
    bool operator()(const NodeType* x, const NodeType* y) 
	 const { return x->value > y->value; }
  };
  
  typedef std::priority_queue<NodeType*,std::vector<NodeType *>,NodeTypeCmp> HeapType;
  //ETX

  HeapType TrialHeap;

  vtkmpjFastMarchingImageFilter();
  virtual ~vtkmpjFastMarchingImageFilter();
  
  void ExecuteInformation();  
  virtual void ComputeInputUpdateExtent(int inExt[6], int outExt[6]);
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  virtual void ExecuteData(vtkDataObject *output);

  virtual void Initialize(vtkImageData *output);  
  virtual void GenerateData(vtkImageData *input, vtkImageData *output);
  virtual void UpdateNeighbors(int index, int coord[3], vtkImageData *input, vtkImageData *output);
  virtual double UpdateValue(int index, int coord[3], vtkImageData *input, vtkImageData *output);

 private:
  void SortThreeValues(double value[3]);
    
  vtkmpjFastMarchingImageFilter(const vtkmpjFastMarchingImageFilter&);// Not implemented
  void operator=(const vtkmpjFastMarchingImageFilter&); // Not Implemented
};

#endif
