

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
 

#ifndef __vtkmpjEllipsoidSource_h
#define __vtkmpjEllipsoidSource_h

#include "vtkPolyDataSource.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkTriangleFilter.h"
#include "vtkPolyDataNormals.h"

class vtkmpjEllipsoidSource : public vtkPolyDataSource 
{
public:
  vtkTypeMacro(vtkmpjEllipsoidSource,vtkPolyDataSource);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct an ellipsoid with the following parameters:
  // MinimumU = 0, MaximumU = 2*Pi, 
  // MinimumV = 0, MaximumV = Pi, 
  // JoinU = 1, JoinV = 0, 
  // TwistU = 0, TwistV = 0, 
  // ClockwiseOrdering = 1,
  // DerivativesAvailable = 1, 
  // XRadius = 1, YRadius = 1,
  // ZRadius = 1, a sphere in this case.
  // UResolution = 10, VResolution = 10.
  static vtkmpjEllipsoidSource *New();

  // Description:
  // Set/Get the scaling factor for the x-axis. Default = 1.
  vtkSetMacro(XRadius,double);
  vtkGetMacro(XRadius,double);

  // Description:
  // Set/Get the scaling factor for the y-axis. Default = 1.
  vtkSetMacro(YRadius,double);
  vtkGetMacro(YRadius,double);

  // Description:
  // Set/Get the scaling factor for the z-axis. Default = 1.
  vtkSetMacro(ZRadius,double);
  vtkGetMacro(ZRadius,double);

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
  // Set/Get the generation of texture coordinates. This is off by
  // default.
  // Note that this is only applicable to parametric surfaces 
  // whose parametric dimension is 2.
  // Note that texturing may fail in some cases.
  vtkBooleanMacro(GenerateTextureCoordinates,int);
  vtkSetMacro(GenerateTextureCoordinates,int);
  vtkGetMacro(GenerateTextureCoordinates,int);

  // Description:
  // Set/Get the minimum u-value.
  vtkSetMacro(MinimumU,double);
  vtkGetMacro(MinimumU,double);

  // Description:
  // Set/Get the maximum u-value.
  vtkSetMacro(MaximumU,double);
  vtkGetMacro(MaximumU,double);

  // Description:
  // Set/Get the minimum v-value.
  vtkSetMacro(MinimumV,double);
  vtkGetMacro(MinimumV,double);

  // Description:
  // Set/Get the maximum v-value.
  vtkSetMacro(MaximumV,double);
  vtkGetMacro(MaximumV,double);

  // Description:
  // Set/Get the flag which joins the first triangle strip to the last one.
  vtkSetMacro(JoinU,int);
  vtkGetMacro(JoinU,int);
  vtkBooleanMacro(JoinU,int);

  // Description:
  // Set/Get the flag which joins the the ends of the triangle strips.
  vtkSetMacro(JoinV,int);
  vtkGetMacro(JoinV,int);
  vtkBooleanMacro(JoinV,int);

  // Description:
  // Set/Get the flag which joins the first triangle strip to 
  // the last one with a twist.
  // JoinU must also be set if this is set. Used when building
  // some non-orientable surfaces.
  vtkSetMacro(TwistU,int);
  vtkGetMacro(TwistU,int);
  vtkBooleanMacro(TwistU,int);

  // Description:
  // Set/Get the flag which joins the ends of the 
  // triangle strips with a twist.
  // JoinV must also be set if this is set. Used when building
  // some non-orientable surfaces.
  vtkSetMacro(TwistV,int);
  vtkGetMacro(TwistV,int);
  vtkBooleanMacro(TwistV,int);

  // Description:
  // Set/Get the flag which determines the ordering of the the 
  // vertices forming the triangle strips. The ordering of the 
  // points being inserted into the triangle strip is important 
  // because it determines the direction of the normals for the 
  // lighting. If set, the ordering is clockwise, otherwise the
  // ordering is anti-clockwise. Default is true (i.e. clockwise
  // ordering).
  vtkSetMacro(ClockwiseOrdering,int);
  vtkGetMacro(ClockwiseOrdering,int);
  vtkBooleanMacro(ClockwiseOrdering,int);

  // Description:
  // Set/Get the flag which determines whether derivatives are available 
  // from the parametric function (i.e., whether the Evaluate() method 
  // returns valid derivatives).
  vtkSetMacro(DerivativesAvailable,int);
  vtkGetMacro(DerivativesAvailable,int);
  vtkBooleanMacro(DerivativesAvailable,int);

//BTX
  // Description:
  // Enumerate the supported scalar generation modes.
  // <pre>
  // SCALAR_NONE, (default) scalars are not generated.
  // SCALAR_U, the scalar is set to the u-value. 
  // SCALAR_V, the scalar is set to the v-value.
  // SCALAR_U0, the scalar is set to 1 if u = (u_max - u_min)/2 = u_avg, 0 otherwise.
  // SCALAR_V0, the scalar is set to 1 if v = (v_max - v_min)/2 = v_avg, 0 otherwise.
  // SCALAR_U0V0, the scalar is 
  //   set to 1 if u == u_avg, 2 if v == v_avg, 3 if u = u_avg && v = v_avg, 0 otherwise.
  // SCALAR_MODULUS, the scalar is set to (sqrt(u*u+v*v)), this is measured relative to (u_avg,v_avg).
  // SCALAR_PHASE, the scalar is set to (atan2(v,u)) (in degrees, 0 to 360), this is measured relative to (u_avg,v_avg).
  // SCALAR_QUADRANT, the scalar is set to 1, 2, 3 or 4 
  //   depending upon the quadrant of the point (u,v).
  // SCALAR_X, the scalar is set to the x-value. 
  // SCALAR_Y, the scalar is set to the y-value. 
  // SCALAR_Z, the scalar is set to the z-value. 
  // SCALAR_DISTANCE, the scalar is set to (sqrt(x*x+y*y+z*z)). I.e. distance from the origin.
  // SCALAR_USER_DEFINED, the scalar is set to the value returned from EvaluateScalar().
  // </pre>
  enum SCALAR_MODE { SCALAR_NONE = 0, 
    SCALAR_U, SCALAR_V, 
    SCALAR_U0, SCALAR_V0, SCALAR_U0V0,
    SCALAR_MODULUS, SCALAR_PHASE, SCALAR_QUADRANT,
    SCALAR_X, SCALAR_Y, SCALAR_Z, SCALAR_DISTANCE,
    SCALAR_FUNCTION_DEFINED };
  //ETX

  // Description:
  // Get/Set the mode used for the scalar data.  The options are:
  // SCALAR_NONE, (default) scalars are not generated.
  // SCALAR_U, the scalar is set to the u-value. 
  // SCALAR_V, the scalar is set to the v-value.
  // SCALAR_U0, the scalar is set to 1 if u = (u_max - u_min)/2 = u_avg, 0 otherwise.
  // SCALAR_V0, the scalar is set to 1 if v = (v_max - v_min)/2 = v_avg, 0 otherwise.
  // SCALAR_U0V0, the scalar is 
  //   set to 1 if u == u_avg, 2 if v == v_avg, 3 if u = u_avg && v = v_avg, 0 otherwise.
  // SCALAR_MODULUS, the scalar is set to (sqrt(u*u+v*v)), this is measured relative to (u_avg,v_avg).
  // SCALAR_PHASE, the scalar is set to (atan2(v,u)) (in degrees, 0 to 360), this is measured relative to (u_avg,v_avg).
  // SCALAR_QUADRANT, the scalar is set to 1, 2, 3 or 4 
  //   depending upon the quadrant of the point (u,v).
  // SCALAR_X, the scalar is set to the x-value. 
  // SCALAR_Y, the scalar is set to the y-value. 
  // SCALAR_Z, the scalar is set to the z-value. 
  // SCALAR_DISTANCE, the scalar is set to (sqrt(x*x+y*y+z*z)). I.e. distance from the origin.
  // SCALAR_FUNCTION_DEFINED, the scalar is set to the value returned from EvaluateScalar().
  vtkSetClampMacro(ScalarMode, int, SCALAR_NONE, SCALAR_FUNCTION_DEFINED);
  vtkGetMacro(ScalarMode, int);
  void SetScalarModeToNone( void ) {this->SetScalarMode(SCALAR_NONE);}
  void SetScalarModeToU( void ) {this->SetScalarMode(SCALAR_U);}
  void SetScalarModeToV( void ) {this->SetScalarMode(SCALAR_V);}
  void SetScalarModeToU0( void ) {this->SetScalarMode(SCALAR_U0);}
  void SetScalarModeToV0( void ) {this->SetScalarMode(SCALAR_V0);}
  void SetScalarModeToU0V0( void ) {this->SetScalarMode(SCALAR_U0V0);}
  void SetScalarModeToModulus( void ) {this->SetScalarMode(SCALAR_MODULUS);}
  void SetScalarModeToPhase( void ) {this->SetScalarMode(SCALAR_PHASE);}
  void SetScalarModeToQuadrant( void ) {this->SetScalarMode(SCALAR_QUADRANT);} 
  void SetScalarModeToX( void ) {this->SetScalarMode(SCALAR_X);}
  void SetScalarModeToY( void ) {this->SetScalarMode(SCALAR_Y);}
  void SetScalarModeToZ( void ) {this->SetScalarMode(SCALAR_Z);}
  void SetScalarModeToDistance( void ) {this->SetScalarMode(SCALAR_DISTANCE);}
  void SetScalarModeToFunctionDefined( void ) {this->SetScalarMode(SCALAR_FUNCTION_DEFINED);}

protected:
  vtkmpjEllipsoidSource();
  ~vtkmpjEllipsoidSource() {};

  void Execute();
  
  // Variables
  double XRadius;
  double YRadius;
  double ZRadius;
  
  int UResolution;
  int VResolution;
  int GenerateTextureCoordinates;
  int ScalarMode;
  
  // Variables
  double MinimumU;
  double MaximumU;
  double MinimumV;
  double MaximumV;
  double MinimumW;
  double MaximumW;

  int JoinU;
  int JoinV;
  int JoinW;

  int TwistU;
  int TwistV;
  int TwistW;

  int ClockwiseOrdering;
  int DerivativesAvailable;

 private:    
  
  // Description:
  // Generate triangle strips from an ordered set of points.
  //
  // Given a parametrization f(u,v)->(x,y,z), this function generates 
  // a vtkCellAarray of point IDs over the range MinimumU <= u < MaximumU 
  // and MinimumV <= v < MaximumV.
  //
  // Before using this function, ensure that: UResolution,
  // VResolution, MinimumU, MaximumU, MinimumV, MaximumV, JoinU, JoinV,
  // TwistU, TwistV, ordering are set appropriately for the parametric function.
  //
  void MakeTriangleStrips ( vtkCellArray * strips, int PtsU, int PtsV );
  
  // Description:
  // A torus.
  //
  // This function performs the mapping \f$f(u,v) \rightarrow (x,y,x)\f$, returning it
  // as Pt. It also returns the partial derivatives Du and Dv.
  // \f$Pt = (x, y, z), Du = (dx/du, dy/du, dz/du), Dv = (dx/dv, dy/dv, dz/dv)\f$ .
  // Then the normal is \f$N = Du X Dv\f$ .
  void Evaluate(double uvw[3], double Pt[3], double Duvw[9]);

  vtkmpjEllipsoidSource(const vtkmpjEllipsoidSource&);  // Not implemented.
  void operator=(const vtkmpjEllipsoidSource&);  // Not implemented.
};

#endif


