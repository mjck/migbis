

#ifndef __vtkmpjSuperToroidalSource_h
#define __vtkmpjSuperToroidalSource_h

#include "vtkPolyDataSource.h"

#define VTK_MAX_SUPERQUADRIC_RESOLUTION 1024
#define VTK_MIN_SUPERQUADRIC_THICKNESS  1e-4
#define VTK_MIN_SUPERQUADRIC_ROUNDNESS  1e-24

class vtkmpjSuperToroidalSource : public vtkPolyDataSource 
{
public:
  // Description:
  // Create a default supertoroid with a radius of 0.5, 
  // spherical, and centered at the origin.
  static vtkmpjSuperToroidalSource *New();

  vtkTypeMacro(vtkmpjSuperToroidalSource,vtkPolyDataSource);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the center of the superquadric. Default is 0,0,0.
  vtkSetVector3Macro(Center,double);
  vtkGetVectorMacro(Center,double,3);

  // Description:
  // Set the scale factors of the superquadric. Default is 1,1,1.
  vtkSetVector3Macro(Scale,double);
  vtkGetVectorMacro(Scale,double,3);

  // Description:
  // Set the number of points in the longitude direction.
  vtkGetMacro(ThetaResolution,int);
  void SetThetaResolution(int i);

  // Description:
  // Set the number of points in the latitude direction.
  vtkGetMacro(PhiResolution,int);
  void SetPhiResolution(int i);

  // Description:
  // Set/Get SuperToroid isotropic size.
  vtkSetMacro(Size,double);
  vtkGetMacro(Size,double);

  // Description:
  // Set/Get the alpha value, the ring radius. 
  // torus.  The default value is 1.0.
  vtkSetMacro(Alpha,double);
  vtkGetMacro(Alpha,double);

  // Description:
  // Set/Get the beta value, the cross section of the ring.  The default value
  // is 0.5.
  vtkSetMacro(Beta,double);
  vtkGetMacro(Beta,double);

  // Description:
  // Set/Get the gamma value, the height of cross section of the ring.  The default value
  // is 0.5.
  vtkSetMacro(Gamma,double);
  vtkGetMacro(Gamma,double);
  
  // Description:
  // Set/Get the shape of the torus ring.  Default = 1.
  vtkSetMacro(N1,double);
  vtkGetMacro(N1,double);

  // Description:
  // Set/Get the shape of the cross section of the ring. Default = 1.
  vtkSetMacro(N2,double);
  vtkGetMacro(N2,double);

protected:
  vtkmpjSuperToroidalSource(int res=16);
  ~vtkmpjSuperToroidalSource() {};

  void Execute();

  double Alpha;
  double Beta;
  double Gamma;

  double N1;
  double N2;

  double Size;   
  double Center[3];
  double Scale[3];
  int ThetaResolution;
  int PhiResolution;

private:

	void Evaluate(double theta, double phi,  
                  double dtheta, double dphi,                   
                  double dims[3],                    
                  double xyz[3],     
                  double nrm[3]);      

  vtkmpjSuperToroidalSource(const vtkmpjSuperToroidalSource&);  // Not implemented.
  void operator=(const vtkmpjSuperToroidalSource&);  // Not implemented.

  // Description:
  // Calculate sign(x)*(abs(x)^n).
  double Power ( double x, double n );
};

#endif

