/* 
  -- vtkmpjFastMarchingMethod.h --
   
  Implements Sethian's Fast Marching Method on 3D uniform lattice domains.

  Solves the Eikonal equation |grad(T)| = h / F, where T is the
  arrival time of the front, F is the speed and h is the grid
  spacing.

  Marcel Jackowski, May 2003.
*/


#ifndef __vtkmpjFastMarchingMethod_h
#define __vtkmpjFastMarchingMethod_h

#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkmpjPriorityQueue.h"
#include "vtkImageAccumulate.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkPolynomialSolversUnivariate.h"
#include "vtkMath.h"
#include "math.h"
#include "vtkpxSimpleImageToImageFilter.h"

#define VTK_MPJ_FROZEN       -1     
#define VTK_MPJ_NARROW_BAND   0
#define VTK_MPJ_UNVISITED    +1

class vtkmpjFastMarchingMethod : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjFastMarchingMethod *New();
  vtkTypeMacro(vtkmpjFastMarchingMethod,vtkpxSimpleImageToImageFilter);
  
  // Description: Sets speed image
  void SetSpeedImage(vtkImageData *speed);

  // Description:
  // Set/Get boundary conditions for the propagation
  vtkSetObjectMacro(KnownPoints, vtkFloatArray);
  vtkGetObjectMacro(KnownPoints, vtkFloatArray);

  // Description:
  // Turn on/off values for known points
  vtkGetMacro(KnownValues,int);
  vtkSetClampMacro(KnownValues,int,0,1);
  vtkBooleanMacro(KnownValues,int);

  // Description:
  // Set/Get maximum propagation time
  vtkGetMacro(PropagationTime,float);
  vtkSetMacro(PropagationTime,float);

  // Description:
  // Set/Get infinity value
  vtkGetMacro(Infinity,float);
  vtkSetMacro(Infinity,float);

  // Description:
  // Set/Get grid spacing
  vtkGetVectorMacro(Spacing,double,3);
  vtkSetVectorMacro(Spacing,double,3);
  
  // Description:
  // Set/Get grid accuracy of the solution
  vtkGetMacro(SecondOrder,int);
  vtkSetMacro(SecondOrder,int);
  vtkBooleanMacro(SecondOrder,int);

  // Front label image. Points interior to the front have value -1, points 
  // on the front boundary have value 0, and points outside have value +1.
  vtkGetObjectMacro(LabelImage, vtkImageData);
  
  // Return an array containing front point vertices and their values
  vtkGetObjectMacro(FrontPoints, vtkFloatArray);
  
  // Return the filter's elapsed time
  vtkGetMacro(ElapsedTime, double);


 protected:
  double Spacing[3];
  float PropagationTime;
  double Infinity;
  double ElapsedTime;
  int KnownValues;  
  int SecondOrder;
  vtkImageData *Speed;
  vtkImageData *LabelImage;
  vtkFloatArray *KnownPoints;
  vtkFloatArray *FrontPoints;

  vtkDataArray *label;
  vtkmpjPriorityQueue *queue;     

  vtkmpjFastMarchingMethod();
  virtual ~vtkmpjFastMarchingMethod();
  
  void ExecuteInformation();  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  virtual void InitializeFront(vtkImageData *input, vtkImageData *output);
  virtual void PropagateFront(vtkImageData *input, vtkImageData *output);
  virtual void SaveFrontPoints(vtkImageData *input);  
  virtual double ComputeSpeed(vtkImageData *input, vtkImageData *output, int coord[3], int index);
  virtual double ComputeFirstOrder(vtkImageData *input, vtkImageData *output, int coord[3], int index);
  virtual double ComputeSecondOrder(vtkImageData *input, vtkImageData *output, int coord[3], int index);
  
  int GetNeighborIndex(int j);
  void SortTerms(double term[3], double spc[3]);
  double Solve2(double a, double b, double spc[3], double rhs);
  double SolveN(int number, double term[3], double spc[3], double rhs);
  double RegularSolve(double coeff[3]);
  double SolveQuadratic(int number, double term[3], double spc[3], double rhs);
  
  double ComputeTime(vtkImageData *input, vtkImageData *output,
		     int coord[3], int index);  
 private:
  //BTX
  float pog;
  int count, tenth; 
  int neighbor_index[3];
  //ETX
  
  vtkmpjFastMarchingMethod(const vtkmpjFastMarchingMethod&);// Not implemented
  void operator=(const vtkmpjFastMarchingMethod&); // Not Implemented
};

#endif
