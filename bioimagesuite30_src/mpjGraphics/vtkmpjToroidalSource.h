

#ifndef __vtkmpjToroidalSource_h
#define __vtkmpjToroidalSource_h

#include "vtkPolyDataSource.h"

#define VTK_MAX_SUPERQUADRIC_RESOLUTION 1024
#define VTK_MIN_SUPERQUADRIC_THICKNESS  1e-4
#define VTK_MIN_SUPERQUADRIC_ROUNDNESS  1e-24

class vtkmpjToroidalSource : public vtkPolyDataSource 
{
public:
  // Description:
  // Create a default superquadric with a radius of 0.5, non-toroidal,
  // spherical, and centered at the origin.
  static vtkmpjToroidalSource *New();

  vtkTypeMacro(vtkmpjToroidalSource,vtkPolyDataSource);
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
  // Set/Get Superquadric isotropic size.
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

protected:
  vtkmpjToroidalSource(int res=16);
  ~vtkmpjToroidalSource() {};

  void Execute();

  double Alpha;
  double Beta;
  double Gamma;

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

  vtkmpjToroidalSource(const vtkmpjToroidalSource&);  // Not implemented.
  void operator=(const vtkmpjToroidalSource&);  // Not implemented.
};

#endif

