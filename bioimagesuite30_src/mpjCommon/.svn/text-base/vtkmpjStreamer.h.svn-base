

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



#ifndef __vtkmpjStreamer_h
#define __vtkmpjStreamer_h

#include "vtkDataSetToPolyDataFilter.h"

#include "vtkCell.h"
#include "vtkDataSet.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"
#include "vtkGenericCell.h"
#include "vtkInterpolatedVelocityField.h"
#include "vtkMath.h"
#include "vtkMultiThreader.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkRungeKutta2.h"

class vtkInitialValueProblemSolver;
class vtkMultiThreader;

#define VTK_INTEGRATE_FORWARD 0
#define VTK_INTEGRATE_BACKWARD 1
#define VTK_INTEGRATE_BOTH_DIRECTIONS 2

class vtkmpjStreamer : public vtkDataSetToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjStreamer,vtkDataSetToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Specify the start of the streamline in the cell coordinate system. That
  // is, cellId and subId (if composite cell), and parametric coordinates.
  void SetStartLocation(vtkIdType cellId, int subId, float pcoords[3]);

  // Description:
  // Specify the start of the streamline in the cell coordinate system. That
  // is, cellId and subId (if composite cell), and parametric coordinates.
  void SetStartLocation(vtkIdType cellId, int subId, float r, float s,
                        float t);

  // Description:
  // Get the starting location of the streamline in the cell coordinate system.
  vtkIdType GetStartLocation(int& subId, float pcoords[3]);

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
  // Specify the source object used to generate starting points.
  void SetSource(vtkDataSet *source);
  vtkDataSet *GetSource();

  // Description:
  // Specify the maximum length of the Streamer expressed in elapsed time.
  vtkSetClampMacro(MaximumPropagationTime,float,0.0,VTK_LARGE_FLOAT);
  vtkGetMacro(MaximumPropagationTime,float);

  // Description:
  // Specify the direction in which to integrate the Streamer.
  vtkSetClampMacro(IntegrationDirection,int,
                   VTK_INTEGRATE_FORWARD,VTK_INTEGRATE_BOTH_DIRECTIONS);
  vtkGetMacro(IntegrationDirection,int);
  void SetIntegrationDirectionToForward()
    {this->SetIntegrationDirection(VTK_INTEGRATE_FORWARD);};
  void SetIntegrationDirectionToBackward()
    {this->SetIntegrationDirection(VTK_INTEGRATE_BACKWARD);};
  void SetIntegrationDirectionToIntegrateBothDirections()
    {this->SetIntegrationDirection(VTK_INTEGRATE_BOTH_DIRECTIONS);};
  const char *GetIntegrationDirectionAsString();

  // Description:
  // Specify a nominal integration step size (expressed as a fraction of
  // the size of each cell). This value can be larger than 1.
  vtkSetClampMacro(IntegrationStepLength,float,0.0000001,VTK_LARGE_FLOAT);
  vtkGetMacro(IntegrationStepLength,float);

  // Description:
  // Turn on/off the creation of scalar data from velocity magnitude. If off,
  // and input dataset has scalars, input dataset scalars are used.
  vtkSetMacro(SpeedScalars,int);
  vtkGetMacro(SpeedScalars,int);
  vtkBooleanMacro(SpeedScalars,int);

  // Description:
  // Turn on/off the creation of scalar data from vorticity information. 
  // The scalar information is currently the orientation value "theta"
  // used in rotating stream tubes. If off, and input dataset has scalars,
  // then input dataset scalars are used, unless SpeedScalars is also on.
  // SpeedScalars takes precedence over OrientationScalars.
  vtkSetMacro(OrientationScalars, int);
  vtkGetMacro(OrientationScalars, int);
  vtkBooleanMacro(OrientationScalars, int);

  // Description:
  // Set/get terminal speed (i.e., speed is velocity magnitude).  Terminal 
  // speed is speed at which streamer will terminate propagation.
  vtkSetClampMacro(TerminalSpeed,float,0.0,VTK_LARGE_FLOAT);
  vtkGetMacro(TerminalSpeed,float);

  // Description:
  // Turn on/off the computation of vorticity. Vorticity is an indication of
  // the rotation of the flow. In combination with vtkStreamLine and 
  // vtkTubeFilter can be used to create rotated tubes. 
  // If vorticity is turned on, in the output, the velocity vectors 
  // are replaced by vorticity vectors.
  vtkSetMacro(Vorticity,int);
  vtkGetMacro(Vorticity,int);
  vtkBooleanMacro(Vorticity,int);

  vtkSetMacro( NumberOfThreads, int );
  vtkGetMacro( NumberOfThreads, int );

  vtkSetMacro( SavePointInterval, float );
  vtkGetMacro( SavePointInterval, float );

  // Description:
  // Set/get the integrator type to be used in the stream line
  // calculation. The object passed is not actually used but
  // is cloned with NewInstance by each thread/process in the
  // process of integration (prototype pattern). The default is 
  // 2nd order Runge Kutta.
  void SetIntegrator(vtkInitialValueProblemSolver *);
  vtkGetObjectMacro ( Integrator, vtkInitialValueProblemSolver );

protected:
  // Description:
  // Construct object to start from position (0,0,0); integrate forward;
  // terminal speed 0.0; vorticity computation off; integrations step length
  // 0.2; and maximum propagation time 100.0.
  vtkmpjStreamer();
  virtual ~vtkmpjStreamer();

  // Integrate data
  void Integrate();

  // Special method for computing streamer vorticity
  void ComputeVorticity();

  // Controls where streamlines start from (either position or location).
  int StartFrom;

  // Starting from cell location
  vtkIdType StartCell;
  int StartSubId;
  float StartPCoords[3];

  // starting from global x-y-z position
  double StartPosition[3];

  //
  // Special classes for manipulating data
  //
  //BTX - begin tcl exclude
  //
  class mpjStreamPoint {
  public:
    double   x[3];    // position 
    vtkIdType     cellId;  // cell
    int     subId;   // cell sub id
    double   p[3];    // parametric coords in cell 
    double   v[3];    // velocity 
    float   speed;   // velocity norm 
    float   s;       // scalar value 
    float   t;       // time travelled so far 
    float   d;       // distance travelled so far 
    float   omega;   // stream vorticity, if computed
    float   theta;    // rotation angle, if vorticity is computed
  };

  class mpjStreamArray;
  friend class mpjStreamArray;
  class mpjStreamArray { //;prevent man page generation
  public:
    mpjStreamArray();
    ~mpjStreamArray()    {     if (this->Array)
	{
          delete [] this->Array;
	}
    };
    vtkIdType GetNumberOfPoints() {return this->MaxId + 1;};
    mpjStreamPoint *GetmpjStreamPoint(vtkIdType i) {return this->Array + i;};
    vtkIdType InsertNextmpjStreamPoint() 
    {
      if ( ++this->MaxId >= this->Size )
	{
          this->Resize(this->MaxId);
	}
      return this->MaxId; //return offset from array
    }
    mpjStreamPoint *Resize(vtkIdType sz); //reallocates data
    void Reset() {this->MaxId = -1;};

    mpjStreamPoint *Array;  // pointer to data
    vtkIdType MaxId;        // maximum index inserted thus far
    vtkIdType Size;         // allocated size of data
    vtkIdType Extend;       // grow array by this amount
    float Direction;        // integration direction
  };
  //ETX
  //

  //array of streamers
  mpjStreamArray *Streamers;
  vtkIdType NumberOfStreamers;

  // length of Streamer is generated by time, or by MaximumSteps
  float MaximumPropagationTime;

  // integration direction
  int IntegrationDirection;

  // the length (fraction of cell size) of integration steps
  float IntegrationStepLength;

  // boolean controls whether vorticity is computed
  int Vorticity;

  // terminal propagation speed
  float TerminalSpeed;

  // boolean controls whether data scalars or velocity magnitude are used
  int SpeedScalars;

  // boolean controls whether data scalars or vorticity orientation are used
  int OrientationScalars;

  // Prototype showing the integrator type to be set by the user.
  vtkInitialValueProblemSolver* Integrator;

  // Interval with which the stream points will be stored.
  // Useful in reducing the memory footprint. Since the initial
  // value is small, by default, it will store all/most points.
  float SavePointInterval;

  static  VTK_THREAD_RETURN_TYPE ThreadedIntegrate( void *arg );

  // Description:
  // These methods were added to allow access to these variables from the
  // threads. 
  vtkGetMacro( NumberOfStreamers, int );
  mpjStreamArray *GetStreamers() { return this->Streamers; };

  void InitializeThreadedIntegrate();
  vtkMultiThreader           *Threader;
  int                        NumberOfThreads;

private:
  vtkmpjStreamer(const vtkmpjStreamer&);  // Not implemented.
  void operator=(const vtkmpjStreamer&);  // Not implemented.
};

// Description:
// Return the integration direction as a character string.
inline const char *vtkmpjStreamer::GetIntegrationDirectionAsString(void)
{
  if ( this->IntegrationDirection == VTK_INTEGRATE_FORWARD ) 
    {
    return "IntegrateForward";
    }
  else if ( this->IntegrationDirection == VTK_INTEGRATE_BACKWARD ) 
    {
    return "IntegrateBackward";
    }
  else 
    {
    return "IntegrateBothDirections";
    }
}

#endif


