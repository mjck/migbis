/*=========================================================================

  Program:   vtkmpjPolyDataToImageFilter.h

  Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkmpjPolyDataToImageFilter - Create an image from polydata surface
// .SECTION Description
// vtkmpjPolyDataToImageFilter produces images with pixel values determined 
// by polydata scalar values


#ifndef __vtkmpjPolyDataToImageFilter_h
#define __vtkmpjPolyDataToImageFilter_h

#include "vtkImageAlgorithm.h"

class vtkPolyData;
class vtkPoints;

class vtkmpjPolyDataToImageFilter : public vtkImageAlgorithm
{
public:
  static vtkmpjPolyDataToImageFilter *New();
  vtkTypeMacro(vtkmpjPolyDataToImageFilter,vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  virtual void SetPolyData(vtkPolyData *polydata);
  vtkPolyData *GetPolyData();

  vtkGetMacro(Epsilon, double);
  vtkSetMacro(Epsilon, double);

protected:
  vtkmpjPolyDataToImageFilter();
  ~vtkmpjPolyDataToImageFilter() {};

  double Epsilon;

  //virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  //virtual int FillInputPortInformation(int, vtkInformation*);

  virtual int FillInputPortInformation(int, vtkInformation*);
  virtual int RequestInformation (vtkInformation *, vtkInformationVector**, vtkInformationVector *);
  virtual int RequestData(vtkInformation *, vtkInformationVector**, vtkInformationVector *);
private:
  vtkmpjPolyDataToImageFilter(const vtkmpjPolyDataToImageFilter&);  // Not implemented.
  void operator=(const vtkmpjPolyDataToImageFilter&);  // Not implemented.

  static int BarycentricCoords(double x[3], double x1[3], double x2[3], double x3[3], double bcoords[3]);
  static void DrawLine(vtkImageData *output1, vtkImageData *output2, double a[3], double b[3], double c[3], 
    double x1[3], double x2[3], double m1, double m2, double m3);
  static void ConvertCoord(vtkImageData *image, double x[3], double c[3]);
};


#endif
