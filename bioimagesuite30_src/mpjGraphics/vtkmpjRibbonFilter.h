

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


#ifndef __vtkmpjRibbonFilter_h
#define __vtkmpjRibbonFilter_h

#include "vtkPolyDataToPolyDataFilter.h"

#define VTK_VARY_RADIUS_OFF 0
#define VTK_VARY_RADIUS_BY_SCALAR 1
#define VTK_VARY_RADIUS_BY_VECTOR 2
#define VTK_VARY_RADIUS_BY_ABSOLUTE_SCALAR 3

#define VTK_TCOORDS_OFF                    0
#define VTK_TCOORDS_FROM_NORMALIZED_LENGTH 1
#define VTK_TCOORDS_FROM_LENGTH            2
#define VTK_TCOORDS_FROM_SCALARS           3

class vtkCellArray;
class vtkCellData;
class vtkDataArray;
class vtkFloatArray;
class vtkDoubleArray;
class vtkPointData;
class vtkPoints;

class vtkmpjRibbonFilter : public vtkPolyDataToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjRibbonFilter,vtkPolyDataToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct object with radius 0.5, radius variation turned off, the
  // number of sides set to 3, and radius factor of 10.
  static vtkmpjRibbonFilter *New();

  // Description:
  // Set the minimum tube radius (minimum because the tube radius may vary).
  vtkSetClampMacro(Radius,float,0.0,VTK_LARGE_FLOAT);
  vtkGetMacro(Radius,float);

  // Description:
  // Turn on/off the variation of tube radius with scalar value.
  vtkSetClampMacro(VaryRadius,int,
                   VTK_VARY_RADIUS_OFF,VTK_VARY_RADIUS_BY_ABSOLUTE_SCALAR);
  vtkGetMacro(VaryRadius,int);
  void SetVaryRadiusToVaryRadiusOff()
    {this->SetVaryRadius(VTK_VARY_RADIUS_OFF);};
  void SetVaryRadiusToVaryRadiusByScalar()
    {this->SetVaryRadius(VTK_VARY_RADIUS_BY_SCALAR);};
  void SetVaryRadiusToVaryRadiusByVector()
    {this->SetVaryRadius(VTK_VARY_RADIUS_BY_VECTOR);};
  void SetVaryRadiusToVaryRadiusByAbsoluteScalar()
    {this->SetVaryRadius(VTK_VARY_RADIUS_BY_ABSOLUTE_SCALAR);};
  const char *GetVaryRadiusAsString();

  // Description:
  // Set the number of sides for the tube. At a minimum, number of sides is 3.
  //vtkSetClampMacro(NumberOfSides,int,4,VTK_LARGE_INTEGER);
  //vtkGetMacro(NumberOfSides,int);

  // Description:
  // Set the maximum tube radius in terms of a multiple of the minimum radius.
  vtkSetMacro(RadiusFactor,float);
  vtkGetMacro(RadiusFactor,float);

  // Description:
  // Set the default normal to use if no normals are supplied, and the
  // DefaultNormalOn is set.
  vtkSetVector3Macro(DefaultNormal,float);
  vtkGetVectorMacro(DefaultNormal,float,3);

  // Description:
  // Set a boolean to control whether to use default normals.
  // DefaultNormalOn is set.
  vtkSetMacro(UseDefaultNormal,int);
  vtkGetMacro(UseDefaultNormal,int);
  vtkBooleanMacro(UseDefaultNormal,int);

  // Description:
  // Set a boolean to control whether tube sides should share vertices.
  // This creates independent strips, with constant normals so the
  // tube is always faceted in appearance.
  vtkSetMacro(SidesShareVertices, int);
  vtkGetMacro(SidesShareVertices, int);
  vtkBooleanMacro(SidesShareVertices, int);

  // Description:
  // Turn on/off whether to cap the ends with polygons.
  vtkSetMacro(Capping,int);
  vtkGetMacro(Capping,int);
  vtkBooleanMacro(Capping,int);

  // Description:
  // Control the striping of the tubes. If OnRatio is greater than 1,
  // then every nth tube side is turned on, beginning with the Offset
  // side.
  vtkSetClampMacro(OnRatio,int,1,VTK_LARGE_INTEGER);
  vtkGetMacro(OnRatio,int);

  // Description:
  // Control the striping of the tubes. The offset sets the
  // first tube side that is visible. Offset is generally used with
  // OnRatio to create nifty striping effects.
  vtkSetClampMacro(Offset,int,0,VTK_LARGE_INTEGER);
  vtkGetMacro(Offset,int);

  // Description:
  // Control whether and how texture coordinates are produced. This is
  // useful for striping the tube with length textures, etc. If you
  // use scalars to create the texture, the scalars are assumed to be
  // monotonically increasing (or decreasing).
  vtkSetClampMacro(GenerateTCoords,int,VTK_TCOORDS_OFF,
                   VTK_TCOORDS_FROM_SCALARS);
  vtkGetMacro(GenerateTCoords,int);
  void SetGenerateTCoordsToOff()
    {this->SetGenerateTCoords(VTK_TCOORDS_OFF);}
  void SetGenerateTCoordsToNormalizedLength()
    {this->SetGenerateTCoords(VTK_TCOORDS_FROM_NORMALIZED_LENGTH);}
  void SetGenerateTCoordsToUseLength()
    {this->SetGenerateTCoords(VTK_TCOORDS_FROM_LENGTH);}
  void SetGenerateTCoordsToUseScalars()
    {this->SetGenerateTCoords(VTK_TCOORDS_FROM_SCALARS);}
  const char *GetGenerateTCoordsAsString();

  // Description:
  // Control the conversion of units during the texture coordinates
  // calculation. The TextureLength indicates what length (whether 
  // calculated from scalars or length) is mapped to the [0,1)
  // texture space.
  vtkSetClampMacro(TextureLength,float,0.000001,VTK_LARGE_INTEGER);
  vtkGetMacro(TextureLength,float);

  // Description:
  // Set the eigenvector for normal
  vtkSetMacro(NormalEigenvector,int);
  vtkGetMacro(NormalEigenvector,int);

  // Description:
  // Set the eigenvector for binormal
  vtkSetMacro(BinormalEigenvector,int);
  vtkGetMacro(BinormalEigenvector,int);

  // Description:
  // Set the scale for eigenvalue 1
  vtkSetMacro(NormalScale,float);
  vtkGetMacro(NormalScale,float);

  // Description:
  // Set the scale for eigenvalue 2
  vtkSetMacro(BinormalScale,float);
  vtkGetMacro(BinormalScale,float);

protected:
  vtkmpjRibbonFilter();
  ~vtkmpjRibbonFilter() {}

  // Usual data generation method
  void Execute();

  float Radius; //minimum radius of tube
  int VaryRadius; //controls radius variation
  int NumberOfSides; //number of sides to create tube
  float RadiusFactor; //maxium allowablew radius
  float DefaultNormal[3];
  int UseDefaultNormal;
  int SidesShareVertices;
  int Capping; //control whether tubes are capped
  int OnRatio; //control the generation of the sides of the tube
  int Offset;  //control the generation of the sides
  int GenerateTCoords; //control texture coordinate generation
  float TextureLength; //this length is mapped to [0,1) texture space

  int NormalEigenvector; // Eigenvalue/vector to dimension tube
  int BinormalEigenvector; // Eigenvalue/vector to dimension tube
  float NormalScale;
  float BinormalScale;
  
  // Helper methods
  int GeneratePoints(vtkIdType offset, vtkIdType npts, vtkIdType *pts,
                     vtkPoints *inPts, vtkPoints *newPts, 
                     vtkPointData *pd, vtkPointData *outPD,
                     vtkFloatArray *newNormals, vtkDataArray *inScalars,
                     double range[2], vtkDataArray *inVectors, double maxNorm, 
                     vtkDataArray *inNormals);
  void GenerateStrips(vtkIdType offset, vtkIdType npts, vtkIdType *pts, 
                      vtkIdType inCellId, vtkCellData *cd, vtkCellData *outCD,
                      vtkCellArray *newStrips);
  void GenerateTextureCoords(vtkIdType offset, vtkIdType npts, vtkIdType *pts, 
                             vtkPoints *inPts, vtkDataArray *inScalars,
                            vtkFloatArray *newTCoords);
  vtkIdType ComputeOffset(vtkIdType offset,vtkIdType npts);
  
  // Helper data members
  float Theta;

private:
  vtkmpjRibbonFilter(const vtkmpjRibbonFilter&);  // Not implemented.
  void operator=(const vtkmpjRibbonFilter&);  // Not implemented.
};

#endif
