

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


#ifndef __vtkmpjTensorGlyphs_h
#define __vtkmpjTensorGlyphs_h

#include "vtkPolyDataSource.h"
#include "vtkImageData.h"
#include "vtkDataSetToPolyDataFilter.h"

class vtkmpjTensorGlyphs : public vtkDataSetToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjTensorGlyphs,vtkDataSetToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description
  // Construct object with scaling on and scale factor 1.0. Eigenvalues are 
  // extracted, glyphs are colored with input scalar data.
  static vtkmpjTensorGlyphs *New();

  // Description:
  // Specify the geometry to copy to each point.
  void SetSource(vtkPolyData *source);
  vtkPolyData *GetSource();

  // Description:
  // Set/Get the number of subdivisions / tessellations in the u parametric
  // direction. Note that the number of tessellant points in the u 
  // direction is the UResolution + 1.
  vtkSetMacro(UResolution,int); 
  vtkGetMacro(UResolution,int);

  // Description:
  // Set/Get the number of subdivisions / tessellations in the v parametric
  // direction. Note that the number of tessellant points in the v 
  // direction is the VResolution + 1.
  vtkSetMacro(VResolution,int);
  vtkGetMacro(VResolution,int);

  // Description:
  // Turn on/off scaling of glyph with eigenvalues.
  vtkSetMacro(Scaling,int);
  vtkGetMacro(Scaling,int);
  vtkBooleanMacro(Scaling,int);

  // Description:
  // Specify scale factor to scale object by. (Scale factor always affects
  // output even if scaling is off.)
  vtkSetMacro(ScaleFactor,float);
  vtkGetMacro(ScaleFactor,float);

  // Description:
  // Set/Get threshold value for classification
  vtkSetMacro(EigenvalueThreshold,float);
  vtkGetMacro(EigenvalueThreshold,float);

  // Description:
  // Turn on/off scalar clamping. If scalar clamping is on, the ivar
  // MaxScaleFactor is used to control the maximum scale factor. (This is
  // useful to prevent uncontrolled scaling near singularities.)
  vtkSetMacro(ClampScaling,int);
  vtkGetMacro(ClampScaling,int);
  vtkBooleanMacro(ClampScaling,int);

  vtkSetMacro(Inequality,int);
  vtkGetMacro(Inequality,int);
  vtkBooleanMacro(Inequality,int);

  vtkSetMacro(SwapPowers,int);
  vtkGetMacro(SwapPowers,int);
  vtkBooleanMacro(SwapPowers,int);

  // Description:
  // Set/Get the maximum allowable scale factor. This value is compared to the
  // combination of the scale factor times the eigenvalue. If less, the scale
  // factor is reset to the MaxScaleFactor. The boolean ClampScaling has to 
  // be "on" for this to work.
  vtkSetMacro(MaxScaleFactor,float);
  vtkGetMacro(MaxScaleFactor,float);

   // Description:
  // Set/Get the shape of the torus ring.  Default = 1.
  vtkSetMacro(N1,double);
  vtkGetMacro(N1,double);

  // Description:
  // Set/Get the shape of the cross section of the ring. Default = 1.
  vtkSetMacro(N2,double);
  vtkGetMacro(N2,double);

  // Description:
  // Set/Get the ring smoothness
  vtkSetMacro(Smoothness, double);
  vtkGetMacro(Smoothness, double);

  // Description:
  // Set/Get cross-section smoothness
  vtkSetMacro(Smoothness2, double);
  vtkGetMacro(Smoothness2, double);

  // Description:
  // Enable/Disable constant ring smoothness
  vtkSetMacro(UseConstantSmoothness,int);
  vtkGetMacro(UseConstantSmoothness,int);
  vtkBooleanMacro(UseConstantSmoothness,int);

  // Description:
  // Enable/Disable constant cross-section smoothness
  vtkSetMacro(UseConstantSmoothness2,int);
  vtkGetMacro(UseConstantSmoothness2,int);
  vtkBooleanMacro(UseConstantSmoothness2,int);

  // Description:
  // Set/Get based-map ring smoothness
  vtkGetObjectMacro(SmoothnessImage, vtkImageData);
  vtkSetObjectMacro(SmoothnessImage, vtkImageData);

  // Description:
  // Set/Get map-based cross-section smoothness
  vtkGetObjectMacro(SmoothnessImage2, vtkImageData);
  vtkSetObjectMacro(SmoothnessImage2, vtkImageData);

  // Description:
  // Set/Get the edge smoothness. Default = 0.5.
  vtkSetMacro(Tubeness, double);
  vtkGetMacro(Tubeness, double);

  //BTX
  enum GLYPH_TYPE { 
	  GLYPH_SOURCE = 0, 
	  GLYPH_ELLIPSOID, 
	  GLYPH_SUPERQUADRIC, 
	  GLYPH_TOROID, 
	  GLYPH_TOROID_EXT, 
	  GLYPH_SUPERTOROID, 
	  GLYPH_SUPERTOROID_EXT,
	  GLYPH_SUPERTOROID_EXT2,
	  GLYPH_SUPERTOROID_EXT3,
	  GLYPH_SUPERTOROID_EXT4 };
  //ETX

  // Description:
  // Get/Set the glyph type to used. The options are:
  // GLYPH_SOURCE, GLYPH_TOROID, GLYPH_SUPERQUADRIC
  vtkSetClampMacro(GlyphType, int, GLYPH_SOURCE, GLYPH_SUPERTOROID_EXT4);
  vtkGetMacro(GlyphType, int);
  void SetGlyphTypeToSource( void ) {this->SetGlyphType(GLYPH_SOURCE);}
  void SetGlyphTypeToEllipsoid( void ) {this->SetGlyphType(GLYPH_ELLIPSOID);}
  void SetGlyphTypeToSuperQuadric( void ) {this->SetGlyphType(GLYPH_SUPERQUADRIC);}
  void SetGlyphTypeToToroid( void ) {this->SetGlyphType(GLYPH_TOROID);}
  void SetGlyphTypeToToroidExtended ( void ) { this->SetGlyphType(GLYPH_TOROID_EXT);}
  void SetGlyphTypeToSuperToroid ( void ) { this->SetGlyphType(GLYPH_SUPERTOROID);}
  void SetGlyphTypeToSuperToroidExtended ( void ) { this->SetGlyphType(GLYPH_SUPERTOROID_EXT); }
  void SetGlyphTypeToSuperToroidExtended2 ( void ) { this->SetGlyphType(GLYPH_SUPERTOROID_EXT2); }
  void SetGlyphTypeToSuperToroidExtended3 ( void ) { this->SetGlyphType(GLYPH_SUPERTOROID_EXT3); }
  void SetGlyphTypeToSuperToroidExtended4 ( void ) { this->SetGlyphType(GLYPH_SUPERTOROID_EXT4); }

protected:
  vtkmpjTensorGlyphs();
  ~vtkmpjTensorGlyphs();

  void Execute();

  int Scaling; // Determine whether scaling of geometry is performed
  float ScaleFactor; // Scale factor to use to scale geometry
  int ClampScaling; // Boolean controls whether scaling is clamped.
  float MaxScaleFactor; // Maximum scale factor (ScaleFactor*eigenvalue)
  float EigenvalueThreshold;  // Eigenvalue threshold
  int GlyphType;    // Glyph type
  int UResolution;  // Glyph resolution in u
  int VResolution;  // Glyph resolution in v
  int Inequality;
  int SwapPowers;
  int UseConstantSmoothness; // booleans for constant or map-based smoothness 
  int UseConstantSmoothness2;

  vtkImageData *SmoothnessImage;
  vtkImageData *SmoothnessImage2;

  double N1;
  double N2;
  double Smoothness;  // Edge smoothness for supertoroid and superquadrics
  double Smoothness2;
  double Tubeness;  // Tubeness

private:

   vtkPolyData *CreateToroidGlyph(double alpha, double beta, double gamma);
   vtkPolyData *CreateToroidExtGlyph(double XRadius, double YRadius, double ZRadius, double alpha, double beta, double gamma);
   vtkPolyData *CreateSuperToroidGlyph(double alpha, double beta, double gamma, double N1, double N2);
   vtkPolyData *CreateNewSuperToroidExtGlyph(double alpha, double beta, double gamma, 
	   double XRadius, double YRadius, double ZRadius, double N1, double N2, double Xi, int swap);
   vtkPolyData *CreateSuperToroidExtGlyph(double alpha, double beta, double gamma, 
	   double XRadius, double YRadius, double ZRadius, double N1, double N2, double Xi, int swap);
   vtkPolyData *CreateEllipsoidGlyph(double XRadius, double YRadius, double ZRadius);
   vtkPolyData *CreateSuperQuadricGlyph(double XRadius, double YRadius, double ZRadius, double Alpha, double Beta, int swap);
   int ClassifyEigenvalues(double w[3], double thresh);

   void SortEigenvectorsByMagnitude(double w[3], double *v[3]);

	vtkmpjTensorGlyphs(const vtkmpjTensorGlyphs&);  // Not implemented.
  void operator=(const vtkmpjTensorGlyphs&);  // Not implemented.
};

#endif
