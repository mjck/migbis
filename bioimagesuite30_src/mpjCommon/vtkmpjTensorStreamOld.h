

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


#ifndef __vtkmpjTensorStreamOld_h
#define __vtkmpjTensorStreamOld_h

#include "vtkDataSetToPolyDataFilter.h"
#include "vtkPolyData.h"
#include "vtkDataSet.h"
#include "vtkPointData.h"
#include "vtkCellArray.h"
#include "vtkImageData.h"

#define VTK_MPJ_RUNGE_KUTTA2 0
#define VTK_MPJ_RUNGE_KUTTA4 1

#define VTK_MPJ_INTEGRATE_FORWARD 0
#define VTK_MPJ_INTEGRATE_BACKWARD 1
#define VTK_MPJ_INTEGRATE_BOTH_DIRECTIONS 2

class vtkmpjTensorArray;

class vtkmpjTensorStreamOld : public vtkDataSetToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjTensorStreamOld,vtkDataSetToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct object with initial starting position (0,0,0); integration
  // step length 0.2; step length 0.01; forward integration; terminal
  // eigenvalue 0.0; number of sides 6; radius 0.5; and logarithmic scaling
  // off.
  static vtkmpjTensorStreamOld *New();
  
  // Description:
  // Get one input to this filter
  vtkDataSet *GetInput();

  // Description:
  // Specify the start of the hyperstreamline in the cell coordinate system. 
  // That is, cellId and subId (if composite cell), and parametric coordinates.
  void SetStartLocation(vtkIdType cellId, int subId, float pcoords[3]);
 
  // Description:
  // Specify the start of the hyperstreamline in the cell coordinate system. 
  // That is, cellId and subId (if composite cell), and parametric coordinates.
  void SetStartLocation(vtkIdType cellId, int subId, float r, float s,
                        float t);

  // Description:
  // Get the starting location of the hyperstreamline in the cell coordinate
  // system. Returns the cell that the starting point is in.
  vtkIdType GetStartLocation(int& subId, float pcoords[3]);

  // Description:
  // Specify the start of the hyperstreamline in the global coordinate system. 
  // Starting from position implies that a search must be performed to find 
  // initial cell to start integration from.
  void SetStartPosition(double x[3]);

  // Description:
  // Specify the start of the hyperstreamline in the global coordinate system. 
  // Starting from position implies that a search must be performed to find 
  // initial cell to start integration from.
  void SetStartPosition(double x, double y, double z);

  // Description:
  // Get the start position of the hyperstreamline in global x-y-z coordinates.
  double *GetStartPosition();

  // Description:
  // Use the major eigenvector field as the vector field through which to 
  // integrate. The major eigenvector is the eigenvector whose corresponding
  // eigenvalue is closest to positive infinity.
  void IntegrateMajorEigenvector();

  // Description:
  // Use the major eigenvector field as the vector field through which to 
  // integrate. The major eigenvector is the eigenvector whose corresponding
  // eigenvalue is between the major and minor eigenvalues.
  void IntegrateMediumEigenvector();

  // Description:
  // Use the major eigenvector field as the vector field through which to 
  // integrate. The major eigenvector is the eigenvector whose corresponding
  // eigenvalue is closest to negative infinity.
  void IntegrateMinorEigenvector();

  // Description:
  // Set / get the maximum length of the hyperstreamline expressed as absolute
  // distance (i.e., arc length) value.
  vtkSetClampMacro(MaximumDistance,float,0.0,VTK_LARGE_FLOAT);
  vtkGetMacro(MaximumDistance,float);
  
  // Description:
  // Set / get the minimum length of the hyperstreamline expressed as absolute
  // distance (i.e., arc length) value.
  vtkSetClampMacro(MinimumDistance,float,0.0,VTK_LARGE_FLOAT);
  vtkGetMacro(MinimumDistance,float);

  // Description:
  // Set / Get the maximum angle of streamline.
  vtkSetClampMacro(MaximumAngle,float,0.0,VTK_LARGE_FLOAT);
  vtkGetMacro(MaximumAngle,float);

  // Description:
  // Set / Get the maximum torsion of streamline.
  vtkSetClampMacro(MaximumTorsion,float,0.0,VTK_LARGE_FLOAT);
  vtkGetMacro(MaximumTorsion,float);

  // Description:
  // Set/Get Integration order
  vtkSetClampMacro(IntegrationOrder,int,0,1);
  vtkGetMacro(IntegrationOrder,int);
  
  // Description:
  // Set / get a nominal integration step size (expressed as a fraction of
  // the size of each cell).
  vtkSetClampMacro(IntegrationStepLength,float,0.001,0.5);
  vtkGetMacro(IntegrationStepLength,float);

  // Description:
  // Set / get the length of a tube segment composing the hyperstreamline. The
  // length is specified as a fraction of the diagonal length of the input
  // bounding box.
  vtkSetClampMacro(StepLength,float,0.000001,1.0);
  vtkGetMacro(StepLength,float);
  
  // Description:
  // Set/Get Min FA
  vtkSetClampMacro(MinimumFA,float,0.0,2.0);
  vtkGetMacro(MinimumFA,float);
  
  // Description:
  // Set/Get Max FA
  vtkSetClampMacro(MaximumFA,float,0.0,2.0);
  vtkGetMacro(MaximumFA,float);
  
  // Description:
  // Set/Get Min MD
  vtkSetClampMacro(MinimumMD,float,0.0,VTK_LARGE_FLOAT);
  vtkGetMacro(MinimumMD,float);
  
  // Description:
  // Set/Get Max MD
  vtkSetClampMacro(MaximumMD,float,0.0,VTK_LARGE_FLOAT);
  vtkGetMacro(MaximumMD,float);

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
 
  // Description: enable/disable directional color coding
  vtkSetMacro(StatisticsMap, int);
  vtkGetMacro(StatisticsMap, int);
  vtkBooleanMacro(StatisticsMap, int);

  // Description: enable/disable directional color coding
  vtkSetMacro(DirectionalColormap, int);
  vtkGetMacro(DirectionalColormap, int);
  vtkBooleanMacro(DirectionalColormap, int);
  
  // Description: determine type of color scheme to use
  vtkSetMacro(DirectionalColorScheme, int);
  vtkGetMacro(DirectionalColorScheme, int);

  // Description: color saturation
  vtkSetMacro(DirectionalColorSaturation, float);
  vtkGetMacro(DirectionalColorSaturation, float);
  
  // Description: intensity scaling
  vtkSetMacro(DirectionalColorScaling, float);
  vtkGetMacro(DirectionalColorScaling, float);

  // Description: magnitude component
  vtkSetMacro(DirectionalColorModulate, int);
  vtkGetMacro(DirectionalColorModulate, int);
   	      
  // Description: enable/disable fiber transparency
  vtkSetMacro(DirectionalColorAlphaBlending, int);
  vtkGetMacro(DirectionalColorAlphaBlending, int);
  
  // Description: enable/disable normal calculation
  vtkSetMacro(ComputeNormals, int);
  vtkGetMacro(ComputeNormals, int);
  vtkBooleanMacro(ComputeNormals, int);

  // Description:
  // Specify normal direction to the hyperstreamline for use with ribbon visualization
  vtkSetClampMacro(NormalEigenvector, int, 0, 3);
  vtkGetMacro(NormalEigenvector,int);

  // Description:
  // Specifythe binormal direction to the hyperstreamline for use with ribbon visualization
  vtkSetClampMacro(BinormalEigenvector, int, 0, 3);
  vtkGetMacro(BinormalEigenvector,int);

  // Description:
  // Specify the direction in which to integrate the hyperstreamline.
  vtkSetClampMacro(IntegrationDirection,int,
		   VTK_MPJ_INTEGRATE_FORWARD,VTK_MPJ_INTEGRATE_BOTH_DIRECTIONS);
  vtkGetMacro(IntegrationDirection,int);

  void SetIntegrationDirectionToForward()
    {this->SetIntegrationDirection(VTK_MPJ_INTEGRATE_FORWARD);};
  void SetIntegrationDirectionToBackward()
    {this->SetIntegrationDirection(VTK_MPJ_INTEGRATE_BACKWARD);};
  void SetIntegrationDirectionToIntegrateBothDirections()
    {this->SetIntegrationDirection(VTK_MPJ_INTEGRATE_BOTH_DIRECTIONS);};
  
  void SetIntegrationOrderToRungeKutta2() {this->SetIntegrationOrder(VTK_MPJ_RUNGE_KUTTA2);};
  void SetIntegrationOrderToRungeKutta4() {this->SetIntegrationOrder(VTK_MPJ_RUNGE_KUTTA4);};
  
  // Description:
  // Set/Get Tensor
  vtkSetObjectMacro(Tensor, vtkImageData);
  vtkGetObjectMacro(Tensor, vtkImageData);

  // Description:
  // Set/Get Mask
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
  // Description:
  // Set/Get Source points
  vtkGetObjectMacro(Source, vtkDataSet);
  vtkSetObjectMacro(Source, vtkDataSet);
  
protected:
  vtkDataSet *Source;
  vtkImageData *Tensor;
  vtkImageData *Mask;
   
  vtkmpjTensorStreamOld();
  ~vtkmpjTensorStreamOld();

  // Integrate data
  void Execute();
  void BuildPolylines();
  void ProjectVectorOntoPlane(double v[3], double n[3], double r[3]);
  void InterpolateTensor(vtkCell *cell, vtkDataArray *cellTensors, double *w, double *m[3]);
  void FixVectors(double **prev, double **current, int iv, int ix, int iy);

  // Directional colormap support functions
  void AbsoluteValueScheme(float FA, double v[3], float nrgb[3]);
  void RotationalSymmetryScheme(float FA, double v[3], float nrgb[3]);
  void NoSymmetryScheme(float FA, double v[3], float nrgb[3]);
  void MirrorSymmetryScheme(float FA, double v[3], float nrgb[3]);

  // Flag indicates where streamlines start from (either position or location)
  int StartFrom;
  
  // Starting from cell location
  vtkIdType StartCell;
  int StartSubId;

  float StartPCoords[3];

  // starting from global x-y-z position
  double StartPosition[3];

  //array of hyperstreamlines
  vtkmpjTensorArray *Streamers;
  int NumberOfStreamers;
  
  // max length of hyperstreamline in absolute distance
  float MaximumDistance;
  
  // min length of hyperstreamline
  float MinimumDistance;
  
  // integration direction
  int IntegrationDirection;

  // stats
  int StatisticsMap;

  // colorcoding
  int DirectionalColormap;
  int DirectionalColorScheme;
  float DirectionalColorSaturation;
  float DirectionalColorScaling;
  int DirectionalColorModulate;
  int DirectionalColorAlphaBlending;

  // the length (fraction of cell size) of integration steps
  float IntegrationStepLength;
  
  // Runge-Kutta integration order
  int IntegrationOrder;

  // the length of the tube segments composing the hyperstreamline
  float StepLength;

  // minimum and maximum anisotropy
  float MinimumFA;
  float MaximumFA;

  // minimum and maximum mean diffusivity
  float MinimumMD;
  float MaximumMD;
  
  // maximum streamline angles
  float MaximumAngle;
  float MaximumTorsion;
  
  // which eigenvector to use as integration vector field
  int IntegrationEigenvector;

  // which eigenvector to use as a normal and binormal to integration vector field
  int NormalEigenvector;
  int BinormalEigenvector;
  int ComputeNormals;

  int FlipX;
  int FlipY;
  int FlipZ;

 private:
  vtkmpjTensorStreamOld(const vtkmpjTensorStreamOld&);  // Not implemented.
  void operator=(const vtkmpjTensorStreamOld&);  // Not implemented.
};

#endif


