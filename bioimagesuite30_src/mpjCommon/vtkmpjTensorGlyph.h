

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


#ifndef __vtkmpjTensorGlyph_h
#define __vtkmpjTensorGlyph_h

#include "vtkDataSetToPolyDataFilter.h"

class vtkmpjTensorGlyph : public vtkDataSetToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjTensorGlyph,vtkDataSetToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description
  // Construct object with scaling on and scale factor 1.0. Eigenvalues are 
  // extracted, glyphs are colored with input scalar data, and logarithmic
  // scaling is turned off.
  static vtkmpjTensorGlyph *New();

  // Description:
  // Specify the geometry to copy to each point.
  void SetSource(vtkPolyData *source);
  vtkPolyData *GetSource();

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
  // Turn on/off drawing three glyphs
  vtkSetMacro(ThreeGlyphs,int);
  vtkGetMacro(ThreeGlyphs,int);
  vtkBooleanMacro(ThreeGlyphs,int);

  // Description:
  // Turn on/off drawing a mirror of each glyph
  vtkSetMacro(Symmetric,int);
  vtkGetMacro(Symmetric,int);
  vtkBooleanMacro(Symmetric,int);

  // Description:
  // Set/Get the distance, along x, from the origin to the end of the 
  // source glyph. It is used to draw the symmetric glyphs.
  vtkSetMacro(Length,float);
  vtkGetMacro(Length,float);

  // Description:
  // Turn on/off extraction of eigenvalues from tensor.
  vtkSetMacro(ExtractEigenvalues,int);
  vtkBooleanMacro(ExtractEigenvalues,int);
  vtkGetMacro(ExtractEigenvalues,int);

  // Description:
  // Turn on/off coloring of glyph with input scalar data or
  // eigenvalues. If false, or input scalar data not present, then the
  // scalars from the source object are passed through the filter.
  vtkSetMacro(ColorGlyphs,int);
  vtkGetMacro(ColorGlyphs,int);
  vtkBooleanMacro(ColorGlyphs,int);

//BTX
  enum
  {
      COLOR_BY_SCALARS,
      COLOR_BY_EIGENVALUES
  };
//ETX

  // Description:
  // Set the color mode to be used for the glyphs.  This can be set to
  // use the input scalars (default) or to use the eigenvalues at the
  // point.  If ThreeGlyphs is set and the eigenvalues are chosen for
  // coloring then each glyph is colored by the corresponding
  // eigenvalue and if not set the color corresponding to the largest
  // eigenvalue is chosen.  The recognized values are:
  // COLOR_BY_SCALARS = 0 (default)
  // COLOR_BY_EIGENVALUES = 1
  vtkSetClampMacro(ColorMode, int, COLOR_BY_SCALARS, COLOR_BY_EIGENVALUES);
  vtkGetMacro(ColorMode, int);
  void SetColorModeToScalars()
    {this->SetColorMode(COLOR_BY_SCALARS);};
  void SetColorModeToEigenvalues()
    {this->SetColorMode(COLOR_BY_EIGENVALUES);};  

  // Description:
  // Turn on/off scalar clamping. If scalar clamping is on, the ivar
  // MaxScaleFactor is used to control the maximum scale factor. (This is
  // useful to prevent uncontrolled scaling near singularities.)
  vtkSetMacro(ClampScaling,int);
  vtkGetMacro(ClampScaling,int);
  vtkBooleanMacro(ClampScaling,int);

  // Description:
  // Set/Get the maximum allowable scale factor. This value is compared to the
  // combination of the scale factor times the eigenvalue. If less, the scale
  // factor is reset to the MaxScaleFactor. The boolean ClampScaling has to 
  // be "on" for this to work.
  vtkSetMacro(MaxScaleFactor,float);
  vtkGetMacro(MaxScaleFactor,float);

 // Description: whether to flip the sign of the x-coordinate 
  vtkSetMacro(FlipX,int);
  vtkGetMacro(FlipX,int);
  vtkBooleanMacro(FlipX,int);
  
  // Description: whether to flip the sign of the y-coordinate
  vtkSetMacro(FlipY,int);
  vtkGetMacro(FlipY,int);
  vtkBooleanMacro(FlipY,int);
  
  // Description: whether to flip the sign of the z-coordinate
  vtkSetMacro(FlipZ,int);
  vtkGetMacro(FlipZ,int);
  vtkBooleanMacro(FlipZ,int);

protected:
  vtkmpjTensorGlyph();
  ~vtkmpjTensorGlyph();

  void Execute();

  int Scaling; // Determine whether scaling of geometry is performed
  float ScaleFactor; // Scale factor to use to scale geometry
  int ExtractEigenvalues; // Boolean controls eigenfunction extraction
  int ColorGlyphs; // Boolean controls coloring with input scalar data
  int ColorMode; // The coloring mode to use for the glyphs.
  int ClampScaling; // Boolean controls whether scaling is clamped.
  float MaxScaleFactor; // Maximum scale factor (ScaleFactor*eigenvalue)
  int ThreeGlyphs; // Boolean controls drawing 1 or 3 glyphs
  int Symmetric; // Boolean controls drawing a "mirror" of each glyph
  float Length; // Distance, in x, from the origin to the end of the glyph
  int FlipX;
  int FlipY;
  int FlipZ;
private:
  vtkmpjTensorGlyph(const vtkmpjTensorGlyph&);  // Not implemented.
  void operator=(const vtkmpjTensorGlyph&);  // Not implemented.
};

#endif
