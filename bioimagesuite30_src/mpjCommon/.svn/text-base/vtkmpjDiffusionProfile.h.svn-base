

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


#ifndef __vtkmpjDiffusionProfile_h
#define __vtkmpjDiffusionProfile_h

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
    COLOR_BY_DIFFUSIVITIES,
    COLOR_BY_ORIENTATION
  };
//ETX

enum
  {
    SHAPE_SPHERE,
    SHAPE_DEFORMATION
  };

class vtkmpjDiffusionProfile : public vtkDataSetToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjDiffusionProfile,vtkDataSetToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkmpjDiffusionProfile *New();

  // Description:
  // Specify the geometry to copy to each point.
  void SetSource(vtkPolyData *source);
  vtkPolyData *GetSource();

  // Description:
  // Specify scale factor to scale object by. (Scale factor always affects
  // output even if scaling is off.)
  vtkSetMacro(ScaleFactor,float);
  vtkGetMacro(ScaleFactor,float);

  // Description:
  // Set the color mode to be used for the glyphs.  This can be set to
  // use the input scalars (default) or to use the apparent diffusion
  // coefficients for each direction.
  // The recognized values are:
  // COLOR_BY_SCALARS = 0 (default)
  // COLOR_BY_DIFFUSIVITIES = 1
  // COLOR_BY_ORIENTATION = 2
  vtkSetClampMacro(ColorMode, int, COLOR_BY_SCALARS, COLOR_BY_ORIENTATION);
  vtkGetMacro(ColorMode, int);
  void SetColorModeToScalars() {this->SetColorMode(COLOR_BY_SCALARS);};
  void SetColorModeToDiffusivities() { this->SetColorMode(COLOR_BY_DIFFUSIVITIES);};
  void SetColorModeToOrientation() { this->SetColorMode(COLOR_BY_ORIENTATION);};
  
  vtkSetClampMacro(ShapeMode, int, SHAPE_SPHERE, SHAPE_DEFORMATION);
  vtkGetMacro(ShapeMode, int);
  void SetShapeModeToSphere() {this->SetColorMode(SHAPE_SPHERE);};
  void SetShapeModeToDeformation() { this->SetColorMode(SHAPE_DEFORMATION);};
    
  // Description:
  // Turn on/off scalar clamping. If scalar clamping is on, the ivar
  // MaxScaleFactor is used to control the maximum scale factor. (This is
  // useful to prevent uncontrolled scaling near singularities.)
  vtkSetMacro(ClampScaling,int);
  vtkGetMacro(ClampScaling,int);
  vtkBooleanMacro(ClampScaling,int);
  
  // Description:
  // Set/Get the maximum allowable scale factor. 
  // The boolean ClampScaling has to 
  // be "on" for this to work.
  vtkSetMacro(MaxScaleFactor,float);
  vtkGetMacro(MaxScaleFactor,float);

  // Description
  // Sets/Gets the array of data
  vtkSetStringMacro(ArrayName);
  vtkGetStringMacro(ArrayName);

protected:
  char *ArrayName;
  
  float ScaleFactor; // Scale factor to use to scale geometry
  int ColorMode; // The coloring mode to use for the glyphs.
  int ShapeMode; // Shape mode to use when drawing the profile
  int ClampScaling; // Boolean controls whether scaling is clamped.
  float MaxScaleFactor; // Maximum scale factor (ScaleFactor*eigenvalue)
   
  vtkmpjDiffusionProfile();
  ~vtkmpjDiffusionProfile();

  void Execute();

private:
  vtkmpjDiffusionProfile(const vtkmpjDiffusionProfile&);  // Not implemented.
  void operator=(const vtkmpjDiffusionProfile&);  // Not implemented.
};

#endif
