

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


#ifndef __vtkmpjDiffusionGlyph_h
#define __vtkmpjDiffusionGlyph_h

#include "vtkDataSetToPolyDataFilter.h"
#include "vtkCell.h"
#include "vtkCellArray.h"
#include "vtkDataSet.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkTransform.h"
#include "vtkDoubleArray.h"

//BTX
enum
  {
    COLOR_BY_SCALARS,
  };
//ETX

class vtkmpjDiffusionGlyph : public vtkDataSetToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjDiffusionGlyph,vtkDataSetToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkmpjDiffusionGlyph *New();

  // Description:
  // Specify the geometry to copy to each point.
  void SetSource(vtkPolyData *source);
  vtkPolyData *GetSource();

  // Description:
  // Specify scale factor to scale object by. (Scale factor always affects
  // output even if scaling is off.)
  //vtkSetMacro(ScaleFactor,float);
  //vtkGetMacro(ScaleFactor,float);

  // Description:
  // Turn on/off coloring of glyph with input scalar data or
  // eigenvalues. If false, or input scalar data not present, then the
  // scalars from the source object are passed through the filter.
  //vtkSetMacro(ColorGlyphs,int);
  // vtkGetMacro(ColorGlyphs,int);
  //vtkBooleanMacro(ColorGlyphs,int);

  // Description:
  // Set the color mode to be used for the glyphs.  This can be set to
  // use the input scalars (default) or to use the eigenvalues at the
  // point.  If ThreeGlyphs is set and the eigenvalues are chosen for
  // coloring then each glyph is colored by the corresponding
  // eigenvalue and if not set the color corresponding to the largest
  // eigenvalue is chosen.  The recognized values are:
  // COLOR_BY_SCALARS = 0 (default)
  // COLOR_BY_EIGENVALUES = 1
  //vtkSetClampMacro(ColorMode, int, COLOR_BY_SCALARS, COLOR_BY_SCALARS);
  //vtkGetMacro(ColorMode, int);
  //void SetColorModeToScalars()
  //{this->SetColorMode(COLOR_BY_SCALARS);};

  // Description:
  // Turn on/off scalar clamping. If scalar clamping is on, the ivar
  // MaxScaleFactor is used to control the maximum scale factor. (This is
  // useful to prevent uncontrolled scaling near singularities.)
  //vtkSetMacro(ClampScaling,int);
  // vtkGetMacro(ClampScaling,int);
  //vtkBooleanMacro(ClampScaling,int);

  // Description:
  // Set/Get the maximum allowable scale factor. This value is compared to the
  // combination of the scale factor times the eigenvalue. If less, the scale
  // factor is reset to the MaxScaleFactor. The boolean ClampScaling has to 
  // be "on" for this to work.
  //vtkSetMacro(MaxScaleFactor,float);
  //vtkGetMacro(MaxScaleFactor,float);
  
  // Description:
  // Set/Get the b-Value
  vtkSetMacro(NumberOfDirections,int);
  vtkGetMacro(NumberOfDirections,int);

  // Description:
  // Sets/Gets the array of gradient directions
  vtkSetObjectMacro(Gradients, vtkDoubleArray);
  vtkGetObjectMacro(Gradients, vtkDoubleArray);

  // Description
  // Sets/Gets the array of diffusion coefficients - ADCs
  vtkSetStringMacro(DiffusionName);
  vtkGetStringMacro(DiffusionName);

protected:
  char *DiffusionName;
  int NumberOfDirections;
  vtkDoubleArray *Gradients;

  float ScaleFactor; // Scale factor to use to scale geometry
  int ColorGlyphs; // Boolean controls coloring with input scalar data
  int ColorMode; // The coloring mode to use for the glyphs.
  int ClampScaling; // Boolean controls whether scaling is clamped.
  float MaxScaleFactor; // Maximum scale factor (ScaleFactor*eigenvalue)

   
  void InterpolateV(int number, vtkIdType param[20][20], int *param_count,
		    vtkPoints *newPts, vtkCellArray *newCells,
		    vtkFloatArray *newScalars);
  void InterpolateH(int number, vtkIdType param[20][20], int *param_count,
		    vtkPoints *newPts, vtkCellArray *newCells,
		    vtkFloatArray *newScalars);

  vtkmpjDiffusionGlyph();
  ~vtkmpjDiffusionGlyph();

  void Execute();

private:
  vtkmpjDiffusionGlyph(const vtkmpjDiffusionGlyph&);  // Not implemented.
  void operator=(const vtkmpjDiffusionGlyph&);  // Not implemented.
};

#endif
