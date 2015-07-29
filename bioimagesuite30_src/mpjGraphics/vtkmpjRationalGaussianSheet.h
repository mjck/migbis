/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkmpjRationalGaussianSheet.h

  Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkmpjRationalGaussianSheet - generates a RaG surface from a set 
// of points
// .SECTION Description
//
// An important use of this filter is to fit an elastic surface to 
// irregularly spaced points.


#ifndef __vtkmpjRationalGaussianSheet_h
#define __vtkmpjRationalGaussianSheet_h

#include "vtkPolyDataAlgorithm.h"

class vtkIdList;
class vtkPoints;
class vtkPolyData;
class vtkDataArray;
class vtkFloatArray;
class vtkUnsignedCharArray;

class vtkmpjRationalGaussianSheet : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkmpjRationalGaussianSheet,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct object with SigmaValue=0.1, Resolution=[10, 10],
  // CloseSurface off.
  static vtkmpjRationalGaussianSheet *New();

  // Description:
  // Set/Get the factor that controls tearing of the surface.
  vtkSetClampMacro(SigmaValue,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(SigmaValue,double);

  // Description:
  // Indicate whether stats should be created
  vtkSetMacro(ComputeStatistics,bool);
  vtkGetMacro(ComputeStatistics,bool);

  // Description:
  // Indicate whether directional colormap should be created
  vtkSetMacro(DirectionalColormap,bool);
  vtkGetMacro(DirectionalColormap,bool);

  // Description:
  // These parameters are used to determine the sampling rate for the surface. 
  // Resolution[0] defines the resolution in the direction of the polylines; 
  // Resolution[1] defines the resolution across the polylines 
  // (i.e., direction orthogonal to Resolution[0]).
  vtkSetVector2Macro(Resolution,int);
  vtkGetVectorMacro(Resolution,int,2);

  // Description:
  // Set the eigenvector for normal
  vtkSetMacro(NormalEigenvector,int);
  vtkGetMacro(NormalEigenvector,int);

  // Description:
  // Set the scale for eigenvalue 1
  vtkSetMacro(NormalScale,double);
  vtkGetMacro(NormalScale,double);


  // Description:
  // Set the scale for eigenvalue 1
  vtkSetMacro(Scale,double);
  vtkGetMacro(Scale,double);

  // Description:
  // Turn on/off the variation of sheet thickness with scalar value.
  vtkGetMacro(VaryThickness,bool);
  vtkSetMacro(VaryThickness,bool);

  // Description:
  // Turn on/off the variation of sheet thickness with scalar value.
  vtkGetMacro(HighQuality,bool);
  vtkSetMacro(HighQuality,bool);

  // Description:
  // Turn on/off the variation of sheet thickness with scalar value.
  vtkGetMacro(InterpolateScalars,bool);
  vtkSetMacro(InterpolateScalars,bool);

protected:
  vtkmpjRationalGaussianSheet();
  ~vtkmpjRationalGaussianSheet();

  // Usual data generation method
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  double SigmaValue;
  int    Resolution[2];
  bool   ComputeStatistics;
  bool   DirectionalColormap;
  double Scale;
  int    NormalEigenvector;
  double NormalScale;
  bool   VaryThickness;
  bool   HighQuality;
  bool   InterpolateScalars;
  
private:
  vtkPoints *newPoints;
  vtkFloatArray *newParams, *newNormals, *newScalars; 
  vtkCellArray *newPolys; 
  vtkFloatArray *newFA, *newMD, *newDist, *newAngle, *newTorsion, *newRoi, *newOther;
  vtkFloatArray *minangle, *maxangle, *medangle, *meanangle, *varangle;
  vtkFloatArray *mintorsion, *maxtorsion, *medtorsion, *meantorsion, *vartorsion;
  vtkFloatArray *minother, *maxother, *medother, *meanother, *varother;
  vtkUnsignedCharArray *dec;
  
  //vtkIdList *Ids;
  //double     Weights[4];

  double Gaussian(double u, double v, double ui, double vi, double sigma);
  double g(double u, double v, double ui, double vi, double sigma, vtkDataArray *params, vtkIdType numPts);
  void ComputeNormal(vtkPoints *newPoints, int i, int j, double norm[3]);
  void ComputeSideNormals(vtkPoints *newPoints, int j, vtkIdType offset, double norm1[3], double norm2[3]);
  void ComputeUpsideNormals(vtkPoints *newPoints, int i, vtkIdType offset, double norm1[3], double norm2[3]);
  void DuplicateFields(vtkIdType index, int ComputeDEC);
  
private:
  vtkmpjRationalGaussianSheet(const vtkmpjRationalGaussianSheet&);  // Not implemented.
  void operator=(const vtkmpjRationalGaussianSheet&);  // Not implemented.
};

#endif
