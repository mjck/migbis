/*=========================================================================

  Program:   vtkmpjTensorStreamer.h

  Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkTensorStreamer - abstract object implements integration of massless particle through tensor fields
// .SECTION Description

// .SECTION See Also
// vtkmpjTensorStreamer vtkStreamLine vtkDashedStreamLine vtkTensorStreamPoints

#ifndef __vtkmpjTensorStreamer_h
#define __vtkmpjTensorStreamer_h

#include "vtkPolyDataAlgorithm.h"

class vtkInitialValueProblemSolver;
class vtkMultiThreader;

#define VTK_MPJ_INTEGRATE_FORWARD 0
#define VTK_MPJ_INTEGRATE_BACKWARD 1
#define VTK_MPJ_INTEGRATE_BOTH_DIRECTIONS 2

class vtkmpjTensorStreamer : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkmpjTensorStreamer,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Specify the start of the streamline in the cell coordinate system. That
  // is, cellId and subId (if composite cell), and parametric coordinates.
  void SetStartLocation(vtkIdType cellId, int subId, double pcoords[3]);

  // Description:
  // Specify the start of the streamline in the cell coordinate system. That
  // is, cellId and subId (if composite cell), and parametric coordinates.
  void SetStartLocation(vtkIdType cellId, int subId, double r, double s,
                        double t);

  // Description:
  // Get the starting location of the streamline in the cell coordinate system.
  vtkIdType GetStartLocation(int& subId, double pcoords[3]);

  // Description:
  // Specify the start of the streamline in the global coordinate
  // system. Search must be performed to find initial cell to start
  // integration from.
  void SetStartPosition(double x[3]);

  // Description:
  // Specify the start of the streamline in the global coordinate
  // system. Search must be performed to find initial cell to start
  // integration from.
  void SetStartPosition(double x, double y, double z);

  // Description:
  // Get the start position in global x-y-z coordinates.
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
  vtkSetClampMacro(MaximumDistance,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(MaximumDistance,double);
  
  // Description:
  // Set / get the minimum length of the hyperstreamline expressed as absolute
  // distance (i.e., arc length) value.
  vtkSetClampMacro(MinimumDistance,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(MinimumDistance,double);

  // Description:
  // Set / Get the maximum angle of streamline.
  vtkSetClampMacro(MaximumAngle,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(MaximumAngle,double);

  // Description:
  // Set / Get the maximum torsion of streamline.
  vtkSetClampMacro(MaximumTorsion,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(MaximumTorsion,double);

  
  // Description:
  // Specify the source object used to generate starting points.
  void SetSource(vtkDataSet *source);
  vtkDataSet *GetSource();

  // Description:
  // Specify the map image.
  void SetScalars(vtkDataSet *scalars);
  vtkDataSet *GetScalars();

   // Description:
  // Specify the source object used to generate starting points.
  void SetMask(vtkDataSet *mask);
  vtkDataSet *GetMask();

  // Description:
  // Specify the direction in which to integrate the Streamer.
  vtkSetClampMacro(IntegrationDirection,int,
                   VTK_MPJ_INTEGRATE_FORWARD,VTK_MPJ_INTEGRATE_BOTH_DIRECTIONS);
  vtkGetMacro(IntegrationDirection,int);
  void SetIntegrationDirectionToForward()
    {this->SetIntegrationDirection(VTK_MPJ_INTEGRATE_FORWARD);};
  void SetIntegrationDirectionToBackward()
    {this->SetIntegrationDirection(VTK_MPJ_INTEGRATE_BACKWARD);};
  void SetIntegrationDirectionToIntegrateBothDirections()
    {this->SetIntegrationDirection(VTK_MPJ_INTEGRATE_BOTH_DIRECTIONS);};
  const char *GetIntegrationDirectionAsString();

  // Description:
  // Specify a nominal integration step size (expressed as a fraction of
  // the size of each cell). This value can be larger than 1.
  vtkSetClampMacro(IntegrationStepLength,double,0.0000001,VTK_DOUBLE_MAX);
  vtkGetMacro(IntegrationStepLength,double);

// Description:
  // Set/Get Min FA
  vtkSetClampMacro(MinimumFA,double,0.0,2.0);
  vtkGetMacro(MinimumFA,double);
  
  // Description:
  // Set/Get Max FA
  vtkSetClampMacro(MaximumFA,float,0.0,2.0);
  vtkGetMacro(MaximumFA,double);
  
  // Description:
  // Set/Get Min MD
  vtkSetClampMacro(MinimumMD,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(MinimumMD,double);
  
  // Description:
  // Set/Get Max MD
  vtkSetClampMacro(MaximumMD,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(MaximumMD,double);

  // Description:
  // Set/Get the flipping of the x-coordinate sign
  vtkSetMacro(FlipX, bool);
  vtkGetMacro(FlipX, bool);

  // Description:
  // Set/Get the flipping of the y-coordinate sign
  vtkSetMacro(FlipY, bool);
  vtkGetMacro(FlipY, bool);

  // Description:
  // Set/Get the flipping of the z-coordinate sign
  vtkSetMacro(FlipZ, bool);
  vtkGetMacro(FlipZ, bool);

  // Description:
  // Set/Get the flipping of the z-coordinate sign
  vtkSetMacro(FixEigenvectors, bool);
  vtkGetMacro(FixEigenvectors, bool);

  // Description:
  // Set/Get whether to constrain propagation only to mask locations
  vtkSetMacro(ConstrainPropagationToMask, bool);
  vtkGetMacro(ConstrainPropagationToMask, bool);

  // Description:
  // Specify normal direction to the hyperstreamline for use with ribbon visualization
  vtkSetClampMacro(IntegrationEigenvector, int, 0, 3);
  vtkGetMacro(IntegrationEigenvector,int);

  // Description:
  // Specify normal direction to the hyperstreamline for use with ribbon visualization
  vtkSetClampMacro(NormalEigenvector, int, 0, 3);
  vtkGetMacro(NormalEigenvector,int);

  // Description:
  // Specifythe binormal direction to the hyperstreamline for use with ribbon visualization
  vtkSetClampMacro(BinormalEigenvector, int, 0, 3);
  vtkGetMacro(BinormalEigenvector,int);

  vtkSetMacro( NumberOfThreads, int );
  vtkGetMacro( NumberOfThreads, int );

  //vtkSetMacro( SavePointInterval, double );
  //vtkGetMacro( SavePointInterval, double );

  // Description:
  // Set/get the integrator type to be used in the stream line
  // calculation. The object passed is not actually used but
  // is cloned with NewInstance by each thread/process in the
  // process of integration (prototype pattern). The default is 
  // 2nd order Runge Kutta.
  void SetIntegrator(vtkInitialValueProblemSolver *);
  vtkGetObjectMacro ( Integrator, vtkInitialValueProblemSolver );

  // Description:
  // A positive value, as small as possible for numerical comparison.
  // The initial value is 1E-12.
  vtkSetMacro(Epsilon,double);
  vtkGetMacro(Epsilon,double);
  
protected:
  // Description:
  // Construct object to start from position (0,0,0); integrate forward;
  // terminal speed 0.0; vorticity computation off; integrations step length
  // 0.2; and maximum propagation time 100.0.
  vtkmpjTensorStreamer();
  ~vtkmpjTensorStreamer();

  // Integrate data
  void Integrate(vtkDataSet *tensor, vtkDataSet *source, vtkDataSet *mask, vtkDataSet *map);

  double CalculateMD(double w[3]);
  double CalculateFA(double w[3]);
  void ProjectVectorOntoPlane(double v[3], double n[3], double r[3]);
  void InterpolateTensor(vtkCell *cell, vtkDataArray *cellTensors, double *w, double *m[3]);
  void FixVectors(double **prev, double **current, int iv, int ix, int iy);

  // Controls where streamlines start from (either position or location).
  int StartFrom;

  // Starting from cell location
  vtkIdType StartCell;
  int StartSubId;
  double StartPCoords[3];

  // starting from global x-y-z position
  double StartPosition[3];

  //
  // Special classes for manipulating data
  //
  //BTX - begin tcl exclude
  //
  class TensorStreamPoint {
  public:
    TensorStreamPoint();
    double    x[3];    // position 
    vtkIdType cellId;  // cell
    int       subId;   // cell sub id
    double    p[3];    // parametric coords in cell 
    double    w[3];    // eigenvalues sorted in decresing magnitude
    double    *v[3];   // pointers to sorted eigenvectors
    double    v0[3];   // eigenvector 0
    double    v1[3];   // eigenvector 1
    double    v2[3];   // eigenvector 2
    double    s;       // scalar value 
    double    d;       // distance travelled so far
    double    mask;    // mask value 
    double    angle;   // angle between eigenvectors
    double    torsion; // torsion angle
    double    fa;      // fractional anisotropy
    double    md;      // mean diffusivity
  };

  class TensorStreamArray;
  friend class TensorStreamArray;
  class TensorStreamArray { //;prevent man page generation
  public:
    TensorStreamArray();
    ~TensorStreamArray()
      {
        if (this->Array)
          {
          delete [] this->Array;
          }
      };
    vtkIdType GetNumberOfPoints() {return this->MaxId + 1;};
    TensorStreamPoint *GetTensorStreamPoint(vtkIdType i) {return this->Array + i;};
    vtkIdType InsertNextTensorStreamPoint() 
      {
        if ( ++this->MaxId >= this->Size )
          {
          this->Resize(this->MaxId);
          }
        return this->MaxId; //return offset from array
      }
    TensorStreamPoint *Resize(vtkIdType sz); //reallocates data
    void Reset() {this->MaxId = -1;};

    TensorStreamPoint *Array;     // pointer to data
    vtkIdType MaxId;        // maximum index inserted thus far
    vtkIdType Size;         // allocated size of data
    vtkIdType Extend;       // grow array by this amount
    double Direction;       // integration direction
  };
  //ETX
  //

  //array of streamers
  TensorStreamArray *Streamers;
  vtkIdType NumberOfStreamers;

  // max length of hyperstreamline in absolute distance
  double MaximumDistance;
  
  // min length of hyperstreamline
  double MinimumDistance;

  // minimum and maximum anisotropy
  double MinimumFA;
  double MaximumFA;

  // minimum and maximum mean diffusivity
  double MinimumMD;
  double MaximumMD;
  
  // maximum streamline angles
  double MaximumAngle;
  double MaximumTorsion;

  // which eigenvector to use as integration vector field
  int IntegrationEigenvector;

  // integration direction
  int IntegrationDirection;

  // the length (fraction of cell size) of integration steps
  double IntegrationStepLength;

  // which eigenvector to use as a normal and binormal to integration vector field
  int NormalEigenvector;
  int BinormalEigenvector;
  int ComputeNormals;

  bool FixEigenvectors;
  bool ConstrainPropagationToMask;

  // eigenvector flips
  bool FlipX, FlipY, FlipZ;

  // Prototype showing the integrator type to be set by the user.
  vtkInitialValueProblemSolver* Integrator;

  // A positive value, as small as possible for numerical comparison.
  // The initial value is 1E-12.
  double Epsilon;
  
  // Interval with which the stream points will be stored.
  // Useful in reducing the memory footprint. Since the initial
  // value is small, by default, it will store all/most points.
  //double SavePointInterval;

  static VTK_THREAD_RETURN_TYPE ThreadedIntegrate( void *arg );

  // Description:
  // These methods were added to allow access to these variables from the
  // threads. 
  vtkGetMacro( NumberOfStreamers, vtkIdType );
  TensorStreamArray *GetStreamers() { return this->Streamers; };

  void InitializeThreadedIntegrate();
  vtkMultiThreader           *Threader;
  int                        NumberOfThreads;

  virtual int FillInputPortInformation(int port, vtkInformation *info);

private:
  vtkmpjTensorStreamer(const vtkmpjTensorStreamer&);  // Not implemented.
  void operator=(const vtkmpjTensorStreamer&);  // Not implemented.
};

// Description:
// Return the integration direction as a character string.
inline const char *vtkmpjTensorStreamer::GetIntegrationDirectionAsString()
{
  if ( this->IntegrationDirection == VTK_MPJ_INTEGRATE_FORWARD ) 
    {
    return "IntegrateForward";
    }
  else if ( this->IntegrationDirection == VTK_MPJ_INTEGRATE_BACKWARD ) 
    {
    return "IntegrateBackward";
    }
  else 
    {
    return "IntegrateBothDirections";
    }
}

#endif
