

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


#include "vtkmpjTensorStreamOld.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkFloatArray.h"
#include "pxisinf.h"

#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

//------------------------------------------------------------------------------
vtkmpjTensorStreamOld* vtkmpjTensorStreamOld::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjTensorStreamOld");
  if(ret)
    {
    return (vtkmpjTensorStreamOld*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjTensorStreamOld;
}

//
// Special classes for manipulating data
//BTX
class vtkmpjTensorPoint { //;prevent man page generation
public:
  vtkmpjTensorPoint(); // method sets up storage
  vtkmpjTensorPoint &operator=(const vtkmpjTensorPoint& hp); //for resizing
  
  double X[3];        // position 
  vtkIdType CellId;  // cell
  int SubId;         // cell sub id
  //  vtkIdType PointId; // approx point id
  double P[3];        // parametric coords in cell 
  double W[3];        // eigenvalues (sorted in decreasing value)
  double *V[3];       // pointers to eigenvectors (also sorted)
  double V0[3];       // storage for eigenvectors
  double V1[3];
  double V2[3];
  float S;            // input scalar
  float Angle;        // angle between eigenvectors
  float Torsion;      // torsion angle
  float FA;           // fractional anisotropy 
  float MD;           // mean diffusivity
  float M;            // mask value
  float D;            // distance travelled so far 
};
//ETX

class vtkmpjTensorArray { //;prevent man page generation
public:
  vtkmpjTensorArray();
  ~vtkmpjTensorArray()
  {
    if (this->Array)
      {
        delete [] this->Array;
      }
  };
  vtkIdType GetNumberOfPoints() {return this->MaxId + 1;};
  vtkmpjTensorPoint *GetmpjTensorPoint(vtkIdType i) {return this->Array + i;};
  vtkmpjTensorPoint *InsertNextmpjTensorPoint() 
  {
    if ( ++this->MaxId >= this->Size )
      {
	this->Resize(this->MaxId);
      }
    return this->Array + this->MaxId;
  }
  vtkmpjTensorPoint *Resize(vtkIdType sz); //reallocates data
  void Reset() {this->MaxId = -1;};
  
  vtkmpjTensorPoint *Array;  // pointer to data
  vtkIdType MaxId;          // maximum index inserted thus far
  vtkIdType Size;          // allocated size of data
  vtkIdType Extend;       // grow array by this amount
  float Direction;       // integration direction
};

#define VTK_START_FROM_POSITION 0
#define VTK_START_FROM_LOCATION 1

vtkmpjTensorPoint::vtkmpjTensorPoint()
{
  this->V[0] = this->V0;
  this->V[1] = this->V1;
  this->V[2] = this->V2;
}

vtkmpjTensorPoint& vtkmpjTensorPoint::operator=(const vtkmpjTensorPoint& hp)
{
  int i, j;

  for (i=0; i<3; i++) 
    {
      this->X[i] = hp.X[i];
      this->P[i] = hp.P[i];
      this->W[i] = hp.W[i];
      for (j=0; j<3; j++)
	{
	  this->V[j][i] = hp.V[j][i];
	}
    }
  
  this->S = hp.S;
  this->CellId = hp.CellId;
  //  this->PointId = hp.PointId;
  this->SubId = hp.SubId;
  
  this->D = hp.D;  
  this->Angle = hp.Angle;
  this->Torsion = hp.Torsion;
  this->MD = hp.MD;
  this->FA = hp.FA;
  this->M = hp.M;
  
  return *this;
}

vtkmpjTensorArray::vtkmpjTensorArray()
{
  this->MaxId = -1; 
  this->Array = new vtkmpjTensorPoint[1000];
  this->Size = 1000;
  this->Extend = 5000;
  this->Direction = VTK_MPJ_INTEGRATE_FORWARD;
}

vtkmpjTensorPoint *vtkmpjTensorArray::Resize(vtkIdType sz)
{
  vtkmpjTensorPoint *newArray;
  vtkIdType newSize, i;

  if (sz >= this->Size)
    {
    newSize = this->Size + 
      this->Extend*(((sz-this->Size)/this->Extend)+1);
    }
  else
    {
      newSize = sz;
    }
  
  newArray = new vtkmpjTensorPoint[newSize];

  for (i=0; i<sz; i++)
    {
    newArray[i] = this->Array[i];
    }

  this->Size = newSize;
  delete [] this->Array;
  this->Array = newArray;

  return this->Array;
}

// Construct object with initial starting position (0,0,0); 
vtkmpjTensorStreamOld::vtkmpjTensorStreamOld()
{
  this->StartFrom = VTK_START_FROM_POSITION;
  this->StartPosition[0] = this->StartPosition[1] = this->StartPosition[2] = 0.0;  this->StartCell = 0;  
  this->StartSubId = 0;
  this->StartPCoords[0] = this->StartPCoords[1] = this->StartPCoords[2] = 0.5;

  this->Streamers = NULL;

  this->MaximumDistance = 100.0;
  this->MinimumDistance = 5;
  this->IntegrationStepLength = 0.2;
  this->StepLength = 0.01;
  this->IntegrationDirection = VTK_MPJ_INTEGRATE_FORWARD;

  this->MaximumAngle = 90.0;
  this->MaximumTorsion = 90.0;
  this->MinimumFA = 0.1;
  this->MaximumFA = 1.0;
  this->MinimumMD = 0.0;
  this->MaximumMD = VTK_LARGE_FLOAT;
  
  this->FlipX = 0;
  this->FlipY = 0;
  this->FlipZ = 0;

  this->IntegrationEigenvector = 0; // Major eigenvector
  this->NormalEigenvector = 1;     // Medium
  this->BinormalEigenvector = 2;     // Minor

  this->IntegrationOrder = VTK_MPJ_RUNGE_KUTTA2;

  this->StatisticsMap = 0;

  this->DirectionalColormap = 0;
  this->DirectionalColorScheme = 0;
  this->DirectionalColorSaturation = 0.5;
  this->DirectionalColorScaling = 1.0;
  this->DirectionalColorModulate = 0;
  this->DirectionalColorAlphaBlending = 0;

  this->Tensor = NULL;
  this->Source = NULL;
  this->Mask = NULL;
}

vtkmpjTensorStreamOld::~vtkmpjTensorStreamOld()
{
  if ( this->Streamers )
    {
      delete [] this->Streamers;
    }
  
  // destroy source as well as inputs  
  if (this->Tensor != NULL)
    this->Tensor->Delete();
  
  if (this->Mask != NULL)
    this->Mask->Delete();
  
  if (this->Source != NULL)
    this->Source->Delete();
}

// ----------------------------------------------------------------------------
vtkDataSet *vtkmpjTensorStreamOld::GetInput()
{
  return (vtkDataSet *)(this->Inputs[0]);
}

// Specify the start of the hyperstreamline in the cell coordinate system. 
// That is, cellId and subId (if composite cell), and parametric coordinates.
void vtkmpjTensorStreamOld::SetStartLocation(vtkIdType cellId, int subId,
                                          float pcoords[3])
{
  if ( cellId != this->StartCell || subId != this->StartSubId ||
       pcoords[0] !=  this->StartPCoords[0] || 
       pcoords[1] !=  this->StartPCoords[1] || 
       pcoords[2] !=  this->StartPCoords[2] )
    {
    this->Modified();
    this->StartFrom = VTK_START_FROM_LOCATION;

    this->StartCell = cellId;
    this->StartSubId = subId;
    this->StartPCoords[0] = pcoords[0];
    this->StartPCoords[1] = pcoords[1];
    this->StartPCoords[2] = pcoords[2];
    }
}

// Specify the start of the hyperstreamline in the cell coordinate system. 
// That is, cellId and subId (if composite cell), and parametric coordinates.
void vtkmpjTensorStreamOld::SetStartLocation(vtkIdType cellId, int subId,
                                          float r, float s, float t)
{
  float pcoords[3];
  pcoords[0] = r;
  pcoords[1] = s;
  pcoords[2] = t;

  this->SetStartLocation(cellId, subId, pcoords);
}

// Get the starting location of the hyperstreamline in the cell coordinate
// system. Returns the cell that the starting point is in.
vtkIdType vtkmpjTensorStreamOld::GetStartLocation(int& subId, float pcoords[3])
{
  subId = this->StartSubId;
  pcoords[0] = this->StartPCoords[0];
  pcoords[1] = this->StartPCoords[1];
  pcoords[2] = this->StartPCoords[2];
  return this->StartCell;
}

// Specify the start of the hyperstreamline in the global coordinate system. 
// Starting from position implies that a search must be performed to find 
// initial cell to start integration from.
void vtkmpjTensorStreamOld::SetStartPosition(double x[3])
{
  if ( x[0] != this->StartPosition[0] || x[1] != this->StartPosition[1] || 
  x[2] != this->StartPosition[2] )
    {
    this->Modified();
    this->StartFrom = VTK_START_FROM_POSITION;

    this->StartPosition[0] = x[0];
    this->StartPosition[1] = x[1];
    this->StartPosition[2] = x[2];
    }
}

// Specify the start of the hyperstreamline in the global coordinate system. 
// Starting from position implies that a search must be performed to find 
// initial cell to start integration from.
void vtkmpjTensorStreamOld::SetStartPosition(double x, double y, double z)
{
  double pos[3];
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;

  this->SetStartPosition(pos);
}

// Get the start position of the hyperstreamline in global x-y-z coordinates.
double *vtkmpjTensorStreamOld::GetStartPosition()
{
  return this->StartPosition;
}

// Use the major eigenvector field as the vector field through which to 
// integrate. The major eigenvector is the eigenvector whose corresponding
// eigenvalue is closest to positive infinity.
void vtkmpjTensorStreamOld::IntegrateMajorEigenvector()
{
  if ( this->IntegrationEigenvector != 0 )
    {
    this->Modified();
    this->IntegrationEigenvector = 0;
  }
}

// Use the major eigenvector field as the vector field through which to 
// integrate. The major eigenvector is the eigenvector whose corresponding
// eigenvalue is between the major and minor eigenvalues.
void vtkmpjTensorStreamOld::IntegrateMediumEigenvector()
{
  if ( this->IntegrationEigenvector != 1 )
    {
    this->Modified();
    this->IntegrationEigenvector = 1;
  }
}

// Use the major eigenvector field as the vector field through which to 
// integrate. The major eigenvector is the eigenvector whose corresponding
// eigenvalue is closest to negative infinity.
void vtkmpjTensorStreamOld::IntegrateMinorEigenvector()
{
  if ( this->IntegrationEigenvector != 2 )
    {
    this->Modified();
    this->IntegrationEigenvector = 2;
  }
}

// Make sure coordinate systems are consistent
void vtkmpjTensorStreamOld::FixVectors(double **prev, double **current, int iv, int ix, int iy)
{
  double p0[3], p1[3], p2[3];
  double v0[3], v1[3], v2[3];
  double temp[3];
  int i;

  float flip[3];
  flip[0] = this->FlipX ? -1.0 : 1.0;
  flip[1] = this->FlipY ? -1.0 : 1.0;
  flip[2] = this->FlipZ ? -1.0 : 1.0;

  for (i=0; i<3; i++)
    {
      v0[i] = (current[i][iv] *= flip[i]); 
      v1[i] = (current[i][ix] *= flip[i]);
      v2[i] = (current[i][iy] *= flip[i]);
    }
  
  if ( prev == NULL ) //make sure coord system is right handed
    {
      vtkMath::Cross(v0,v1,temp);
      if ( vtkMath::Dot(v2,temp) < 0.0 )
	{
	  for (i=0; i<3; i++)
	    {
	      current[i][iy] *= -1.0;
	    }
	}
    }
  
  else //make sure vectors consistent from one point to the next
    {
      for (i=0; i<3; i++)
	{
	  p0[i] = prev[i][iv];
	  p1[i] = prev[i][ix];
	  p2[i] = prev[i][iy];
	}
      if ( vtkMath::Dot(p0,v0) < 0.0 )
	{
	  for (i=0; i<3; i++)
	    {
	      current[i][iv] *= -1.0;
	    }
	}
      if ( vtkMath::Dot(p1,v1) < 0.0 )
	{
	  for (i=0; i<3; i++)
	    {
	      current[i][ix] *= -1.0;
	    }
	}
      if ( vtkMath::Dot(p2,v2) < 0.0 )
	{
	  for (i=0; i<3; i++)
	    {
	      current[i][iy] *= -1.0;
	    }
	}
    }
}

void vtkmpjTensorStreamOld::Execute()
{
  vtkDataSet *input = this->GetInput();
  vtkDataSet *source = this->Source;

  vtkPointData *pdt=this->Tensor->GetPointData();

  //  float *tensor;
  vtkmpjTensorPoint *sNext, *sPtr;
  int i, ptId, subId, offset, index, iv, ix, iy;
  //int j, k;
  vtkIdType numSourcePts, idx;
  vtkCell *cell;
  double ev[3], xNext[3];
  float k1[3], k2[3], k3[3], k4[3];
  //  float vort[3], derivs[9];
  float d, step, dir, tol2;
  double p[3];
  double *w;
  double dist2;
  float angle, torsion;
  float w0,w1,w2;
  double sqrt1over2 = sqrt(1.0/2.0);
  double closestPoint[3];
  double *m[3], *v[3];
  double m0[3], m1[3], m2[3];
  double v0[3], v1[3], v2[3], vec[3];

  vtkDataArray *inMask = NULL;
  vtkDataArray *cellMask = NULL;

  if (this->Mask != NULL) {
    inMask = this->Mask->GetPointData()->GetScalars();
    cellMask = vtkDataArray::CreateDataArray(inMask->GetDataType());
    cellMask->SetNumberOfComponents(1);
    cellMask->SetNumberOfTuples(VTK_CELL_SIZE);
  }
  
  vtkDataArray *inTensors;
  vtkDataArray *inScalars;
  vtkDataArray *cellTensors;
  vtkDataArray *cellScalars;
  
  // set up working matrices
  v[0] = v0; v[1] = v1; v[2] = v2; 
  m[0] = m0; m[1] = m1; m[2] = m2; 
  float rad2deg = 180.0 / vtkMath::Pi();
  
  this->NumberOfStreamers = 0;
  w = new double[input->GetMaxCellSize()];
  
  inTensors = pdt->GetScalars();
  cellTensors = vtkDataArray::CreateDataArray(inTensors->GetDataType());
  
  //    vtkPointData *pd = ((vtkDataSet *)this->Inputs[n])->GetPointData();
  inScalars = this->GetInput()->GetPointData()->GetScalars();
  cellScalars = vtkDataArray::CreateDataArray(inScalars->GetDataType());
  cellScalars->SetNumberOfComponents(inScalars->GetNumberOfComponents());
  cellScalars->SetNumberOfTuples(VTK_CELL_SIZE);
  
  int numTComp;
  numTComp = inTensors->GetNumberOfComponents();
  cellTensors->SetNumberOfComponents(numTComp);
  cellTensors->SetNumberOfTuples(VTK_CELL_SIZE);
  
  tol2 = input->GetLength() / 1000.0;
  tol2 = tol2 * tol2;

  iv = this->IntegrationEigenvector;
  ix = this->NormalEigenvector;
  iy = this->BinormalEigenvector;
  //ix = (iv + 1) % 3;
  //iy = (iv + 2) % 3;

  //printf("tracking: %d\n",iv);
  //printf("normal: %d\n",ix);
  //printf("binormal: %d\n",iy);
  //
  // Create starting points
  //
  this->NumberOfStreamers = numSourcePts = offset = 1;
  
  if ( this->Source )
    {
      this->NumberOfStreamers = numSourcePts = this->Source->GetNumberOfPoints();
    }
  
  if ( this->IntegrationDirection == VTK_MPJ_INTEGRATE_BOTH_DIRECTIONS )
    {
      offset = 2;
      this->NumberOfStreamers *= 2;
    }
  
  this->Streamers = new vtkmpjTensorArray[this->NumberOfStreamers];
  
  if ( this->StartFrom == VTK_START_FROM_POSITION && !this->Source )
    {
      sPtr = this->Streamers[0].InsertNextmpjTensorPoint();
      for (i=0; i<3; i++)
	{
	  sPtr->X[i] = this->StartPosition[i];
	}

      sPtr->CellId = input->FindCell(this->StartPosition, NULL, -1, 0.0, 
				     sPtr->SubId, sPtr->P, w);
    }
  else if ( this->StartFrom == VTK_START_FROM_LOCATION && !this->Source )
    {
      sPtr = this->Streamers[0].InsertNextmpjTensorPoint();
      cell =  input->GetCell(sPtr->CellId);
      cell->EvaluateLocation(sPtr->SubId, sPtr->P, sPtr->X, w);
    }
  
  else //VTK_START_FROM_SOURCE
    {      
      for (ptId=0; ptId < numSourcePts; ptId++)
	{
	  sPtr = this->Streamers[offset*ptId].InsertNextmpjTensorPoint();
	  source->GetPoint(ptId,sPtr->X);	  
	  sPtr->CellId = input->FindCell(sPtr->X, NULL, -1, tol2, 
					 sPtr->SubId, sPtr->P, w);
	}
    }
  
  //
  // Finish initializing each hyperstreamline
  //
  for (idx=0, ptId=0; ptId < numSourcePts; ptId++)
    {
      this->Streamers[offset*ptId].Direction = 1.0;
      sPtr = this->Streamers[offset*ptId].GetmpjTensorPoint(idx);
      
      sPtr->S = 0.0;
      sPtr->D = 0.0;      
      sPtr->Angle = 0.0;
      sPtr->Torsion = 0.0;
      
      if ( sPtr->CellId >= 0 ) //starting point in dataset
	{
	  cell = input->GetCell(sPtr->CellId);
	  cell->EvaluateLocation(sPtr->SubId, sPtr->P, xNext, w);
	  
	  inTensors->GetTuples(cell->PointIds, cellTensors);	  
	  
	  InterpolateTensor(cell, cellTensors, w, m);	  
	  vtkMath::Jacobi(m, sPtr->W, sPtr->V);
	  FixVectors(NULL, sPtr->V, iv, ix, iy);

   // printf("tangent_old=%lf,%lf,%lf\n",sPtr->V[0][0],sPtr->V[1][0],sPtr->V[2][0]);

	  // make them absolute eigenvalues
	  for(i=0;i<3;i++)
	    if(sPtr->W[i] < 0) sPtr->W[i] = -sPtr->W[i];
	  
	  // compute MD and FA
	  sPtr->MD = (sPtr->W[0] + sPtr->W[1] + sPtr->W[2]) / 3.0;
	  /*w0 = sPtr->W[0] - sPtr->MD;
	  w1 = sPtr->W[1] - sPtr->MD;
	  w2 = sPtr->W[2] - sPtr->MD;
	  sPtr->FA = sqrt(3*(w0*w0 + w1*w1 + w2*w2)) /
	    sqrt(2*((sPtr->W[0])*(sPtr->W[0]) + 
		    (sPtr->W[1])*(sPtr->W[1]) + 
		    (sPtr->W[2])*(sPtr->W[2])));*/

	  /* Make sure we use a normalized FA */
	  w0 = sPtr->W[0]-sPtr->W[1];
	  w1 = sPtr->W[1]-sPtr->W[2];
	  w2 = sPtr->W[0]-sPtr->W[2];
	  sPtr->FA = sqrt1over2 * 
	    (sqrt(w0*w0 + w1*w1 + w2*w2) / 
	    sqrt((sPtr->W[0])*(sPtr->W[0]) + 
		 (sPtr->W[1])*(sPtr->W[1]) + 
		 (sPtr->W[2])*(sPtr->W[2])));
	  if (isnan(sPtr->FA)) {
	    sPtr->FA = 0.0;
	  }
	  
    //printf("fa,md_old=%lf,%lf\n",sPtr->FA,sPtr->MD);

	  // compute scalar
	  inScalars->GetTuples(cell->PointIds, cellScalars);

	  for (sPtr->S=0, i=0; i < cell->GetNumberOfPoints(); i++)
	    {
	      sPtr->S += cellScalars->GetComponent(i,0) * w[i];
	    }
	  
	  // select weight
	  int wi=0;
	  float wmax=0;
	  for (i=0;i<cell->GetNumberOfPoints();i++)
	    if (w[i] > wmax) {
	      wi = i;  wmax = w[i];
	    }
	  
	  // compute point id
	  //	  sPtr->PointId = cell->GetPointId(wi);

	  // compute mask value
	  if (inMask != NULL) {	    
	    inMask->GetTuples(cell->PointIds, cellMask);
	    sPtr->M = cellMask->GetComponent(wi, 0);
	  } else sPtr->M = 0;
	  
	} else {
	  // zero it out
	}
      
      if ( this->IntegrationDirection == VTK_MPJ_INTEGRATE_BOTH_DIRECTIONS )
	{
	  this->Streamers[offset*ptId+1].Direction = -1.0;
	  sNext = this->Streamers[offset*ptId+1].InsertNextmpjTensorPoint();
	  *sNext = *sPtr;
	}
      else if ( this->IntegrationDirection == VTK_MPJ_INTEGRATE_BACKWARD )
	{
	  this->Streamers[offset*ptId].Direction = -1.0;
	}
      
    } //for hyperstreamline in dataset
  
  // Progress computation
  int count = 0;
  float pog = 0.0;
  int tenth= (int)(this->NumberOfStreamers / 10.0);
  
  this->UpdateProgress(0.01);
  
  //
  // For each hyperstreamline, integrate in appropriate direction (using RK2).
  //
  for (ptId=0; ptId < this->NumberOfStreamers; ptId++)
    {
      
      // progress computation
      count++;
      if (count==tenth)
        {
          pog+=0.1;
          this->UpdateProgress(pog);
          count=0;
        }

      //get starting step
      sPtr = this->Streamers[ptId].GetmpjTensorPoint(0);
      if ( sPtr->CellId < 0 )
	{
	  continue;
	}
      
      dir = this->Streamers[ptId].Direction;
      cell = input->GetCell(sPtr->CellId);
      cell->EvaluateLocation(sPtr->SubId, sPtr->P, xNext, w);
      step = this->IntegrationStepLength * sqrt((double)cell->GetLength2());
      inTensors->GetTuples(cell->PointIds, cellTensors);
      inScalars->GetTuples(cell->PointIds, cellScalars);

      //printf("before integrating %d, %lf %lf %lf %lf\n",sPtr->CellId,sPtr->D,sPtr->FA,sPtr->MD,sPtr->Angle);
      //printf("W: %lf %lf %lf\n",sPtr->W[0],sPtr->W[1],sPtr->W[2]);
      

      //integrate until criteria has been met
      while ( ( sPtr->CellId >= 0 ) && 
	      ( sPtr->D < this->MaximumDistance ) &&
	      ( sPtr->FA >= this->MinimumFA ) && ( sPtr->FA < this->MaximumFA ) &&
	      ( sPtr->MD >= this->MinimumMD ) && ( sPtr->MD < this->MaximumMD ) &&
	      ( sPtr->Angle <= this->MaximumAngle ) && ( sPtr->Torsion <= this->MaximumTorsion )) 

	{	  

	  //printf("step in integrating %d, %lf %lf %lf %lf\n",sPtr->CellId,sPtr->D,sPtr->FA,sPtr->MD,sPtr->Angle);
	  //printf("W: %lf %lf %lf\n",sPtr->W[0],sPtr->W[1],sPtr->W[2]);

	  // evaluate k2
	  for (i=0; i<3; i++) {
	    k1[i] = dir * step * sPtr->V[i][iv];
	    xNext[i] = sPtr->X[i] + 0.5 * k1[i];
	  }

	  cell->EvaluatePosition(xNext, closestPoint, subId, p, dist2, w);
	  
	  InterpolateTensor(cell, cellTensors, w, m);
	  vtkMath::Jacobi(m, ev, v);
	  FixVectors(sPtr->V, v, iv, ix, iy);
	  
	  // evaluate k3
	  for (i=0; i<3; i++) {
	    k2[i] = dir * step * v[i][iv];
	    //xNext[i] = sPtr->X[i] + 0.5 * k2[i];
      xNext[i] = sPtr->X[i] + k2[i];
	  }
	  
	  if ( this->IntegrationOrder == VTK_MPJ_RUNGE_KUTTA4 ) {	    
	    cell->EvaluatePosition(xNext, closestPoint, subId, p, dist2, w);
	    InterpolateTensor(cell, cellTensors, w, m);
	    vtkMath::Jacobi(m, ev, v);
	    FixVectors(sPtr->V, v, iv, ix, iy);
	    
	    // evaluate k4
	    for (i=0; i<3; i++) {
	      k3[i] = dir * step * v[i][iv];
	      xNext[i] = sPtr->X[i] + 0.5 * k3[i];
	    }
	    
	    cell->EvaluatePosition(xNext, closestPoint, subId, p, dist2, w);
	    InterpolateTensor(cell, cellTensors, w, m);
	    vtkMath::Jacobi(m, ev, v);
	    FixVectors(sPtr->V, v, iv, ix, iy);
	    
	    for (i=0; i<3; i++) {
	      k4[i] = dir * step * v[i][iv];
	      
	      //now compute final position
	      xNext[i] = sPtr->X[i] + k1[i]/6.0 + k2[i]/3.0 + k3[i]/3.0 + k4[i]/6.0;
	    }	    
	  }
	  
	  // get a hold of the current index
	  index = this->Streamers[ptId].GetNumberOfPoints();	  
	  
	  // insert next point
	  sNext = this->Streamers[ptId].InsertNextmpjTensorPoint();
	  
	  // make sure sPtr is still valid after possible Resizing
	  sPtr = this->Streamers[ptId].GetmpjTensorPoint(index-1);

	  if ( cell->EvaluatePosition(xNext, closestPoint, sNext->SubId, 
				      sNext->P, dist2, w) )
	    { 
	      //integration still in cell
	      for (i=0; i<3; i++)
		{
		  sNext->X[i] = closestPoint[i];
		}
	      sNext->CellId = sPtr->CellId;
	      sNext->SubId = sPtr->SubId;
	    }
	  else
	    { //integration has passed out of cell
	      sNext->CellId = input->FindCell(xNext, cell, sPtr->CellId, tol2, 
					      sNext->SubId, sNext->P, w);
	      if ( sNext->CellId >= 0 ) //make sure not out of dataset
		{
		  for (i=0; i<3; i++)
		    {
		      sNext->X[i] = xNext[i];
		    }
		  cell = input->GetCell(sNext->CellId);
		  inTensors->GetTuples(cell->PointIds, cellTensors);
		  inScalars->GetTuples(cell->PointIds, cellScalars);
		  step = this->IntegrationStepLength * sqrt((double)cell->GetLength2());
		} else {
		}
	    }
	  
	  if ( sNext->CellId >= 0 )
	    {
	      cell->EvaluateLocation(sNext->SubId, sNext->P, xNext, w);
	      InterpolateTensor(cell, cellTensors, w, m);
	      vtkMath::Jacobi(m, sNext->W, sNext->V);
	      FixVectors(sPtr->V, sNext->V, iv, ix, iy);
       // printf("vector=%lf,%lf,%lf\n",sNext->V[0][0],sNext->V[1][0],sNext->V[2][0]);


	      // make them absolute eigenvalues
	      for(i=0;i<3;i++)
		if(sNext->W[i] < 0) sNext->W[i] = -sNext->W[i];
	      
	      // interpolate scalars
	      inScalars->GetTuples(cell->PointIds, cellScalars);
	      for (sNext->S=0, i=0; i < cell->GetNumberOfPoints(); i++)
		{
		  sNext->S += cellScalars->GetComponent(i,0) * w[i];
		}
	      
	      // select weight
	      int wi=0;
	      float wmax=0;
	      for (i=0;i<cell->GetNumberOfPoints();i++)
		if (w[i] > wmax) {
		  wi = i;  wmax = w[i];
		}

	      // compute point id
	      //	      sPtr->PointId = cell->GetPointId(wi);

	      // interpolate mask value
	      if (inMask != NULL) {
		inMask->GetTuples(cell->PointIds, cellMask);
		sNext->M = cellMask->GetComponent(wi, 0);
	      }
	      
	      d = sqrt((double)vtkMath::Distance2BetweenPoints(sPtr->X,sNext->X));
	      sNext->D = sPtr->D + d; 
	      
	      float v_old[3], v_new[3];
	      
	      for (i=0;i<3;i++) {
		v_old[i] = sPtr->V[i][iv];
		v_new[i] = sNext->V[i][iv];
	      }	      
	      vtkMath::Normalize(v_old);
	      vtkMath::Normalize(v_new);
	      angle = vtkMath::Dot(v_old, v_new);	      
	      if (angle > 1.0) angle = 1.0; 
	      if (angle < -1.0) angle = -1.0;	      
	      sNext->Angle = acos(angle) * rad2deg;
	      
	      // compute torsion		
	      v0[0] = sNext->V[0][iv];
	      v0[1] = sNext->V[1][iv];
	      v0[2] = sNext->V[2][iv];
	      
	      v1[0] = sPtr->V[0][this->NormalEigenvector];
	      v1[1] = sPtr->V[1][this->NormalEigenvector];
	      v1[2] = sPtr->V[2][this->NormalEigenvector];
	      
	      v2[0] = sPtr->V[0][this->BinormalEigenvector];
	      v2[1] = sPtr->V[1][this->BinormalEigenvector];
	      v2[2] = sPtr->V[2][this->BinormalEigenvector];
	      
	      ProjectVectorOntoPlane(v1, v0, vec);
	      v1[0] = sNext->V[0][this->NormalEigenvector];
	      v1[1] = sNext->V[1][this->NormalEigenvector];
	      v1[2] = sNext->V[2][this->NormalEigenvector];

	      angle = vtkMath::Dot(v1, vec);	      
	      if (angle > 1.0) angle = 1.0; 
	      if (angle < -1.0) angle = -1.0;	      
	      sNext->Torsion = acos(angle) * rad2deg;


	      // compute MD and FA
	      sNext->MD = (sNext->W[0] + sNext->W[1] + sNext->W[2]) / 3.0;
	      /*w0 = sNext->W[0] - sNext->MD;
	      w1 = sNext->W[1] - sNext->MD;
	      w2 = sNext->W[2] - sNext->MD;
	      sNext->FA = sqrt(3*(w0*w0 + w1*w1 + w2*w2)) /
		sqrt(2*((sNext->W[0])*(sNext->W[0]) + 
			(sNext->W[1])*(sNext->W[1]) + 
			(sNext->W[2])*(sNext->W[2])));*/
	      
	      /* Make sure we use a normalized FA */
	      w0 = sNext->W[0]-sNext->W[1];
	      w1 = sNext->W[1]-sNext->W[2];
	      w2 = sNext->W[0]-sNext->W[2];
	      sNext->FA = sqrt1over2 * 
		(sqrt(w0*w0 + w1*w1 + w2*w2) / 
		 sqrt((sNext->W[0])*(sNext->W[0]) + 
		      (sNext->W[1])*(sNext->W[1]) + 
		      (sNext->W[2])*(sNext->W[2])));
	      if (isnan(sNext->FA)) {
		sNext->FA = 0.0;
		//printf("w0=%f,w1=%f,w1=%f,W0=%f,W1=%f,W2=%f\n",w0,w1,w2,sNext->W[0],sNext->W[1],sNext->W[2]);
	      }

           // printf("dist=%f, mask=%f, fa=%f, md=%f, angle=%f, torsion=%f\n",sNext->D,sNext->M,sNext->FA,sNext->MD,sNext->Angle,sNext->Torsion);  

	    } else {
	      
	      /* copy information from last point */
	      *sNext = *sPtr;
	      sNext->CellId = -1;
	    }
	  
	  sPtr = sNext;
	  
	  /* printf("cellid=%d\n",sPtr->CellId);
	     printf("S1[%d]=%f\n",this->TerminalScalarComponent1,sPtr->S[this->TerminalScalarComponent1]);
	     printf("S2[%d]=%f\n",this->TerminalScalarComponent2,sPtr->S[this->TerminalScalarComponent2]);
	     printf("TS1=%f\n",this->TerminalScalar1);
	     printf("TS2=%f\n",this->TerminalScalar2);
	     printf("D=%f, max=%f \n",sPtr->D,this->MaximumDistance);
	     printf("Angle=%f max=%f\n",sPtr->Angle, this->MaximumAngle);*/

	  
	} //for elapsed time
      
    } //for each hyperstreamline

  this->BuildPolylines();
  
  delete [] w;
  cellTensors->Delete();
  
  cellScalars->Delete();    
}

void vtkmpjTensorStreamOld::BuildPolylines()
{
  vtkPolyData *output = this->GetOutput();
  vtkPointData *outPD = output->GetPointData();
  
  vtkPoints *newPts = vtkPoints::New();
  vtkCellArray *newCells = vtkCellArray::New();
  vtkFloatArray *newScalars = vtkFloatArray::New();

  vtkFloatArray *fa = vtkFloatArray::New();
  fa->SetName("fractani");
  fa->SetNumberOfComponents(1);

  vtkFloatArray *md = vtkFloatArray::New();
  md->SetName("meandiff");
  md->SetNumberOfComponents(1);

  vtkFloatArray *dist = vtkFloatArray::New();
  dist->SetName("distance");
  dist->SetNumberOfComponents(1);

  vtkFloatArray *angle = vtkFloatArray::New();
  angle->SetName("angle");
  angle->SetNumberOfComponents(1);

  vtkFloatArray *torsion = vtkFloatArray::New();
  torsion->SetName("torsion");
  torsion->SetNumberOfComponents(1);

  //vtkFloatArray *torsion2 = vtkFloatArray::New();
  //torsion2->SetName("torsion2");
  //torsion2->SetNumberOfComponents(1);

  vtkFloatArray *roi = vtkFloatArray::New();
  roi->SetName("roi");
  roi->SetNumberOfComponents(1);
  
  vtkFloatArray *other = vtkFloatArray::New();
  other->SetName("other");
  other->SetNumberOfComponents(1);



  vtkFloatArray *minangle = vtkFloatArray::New();
  minangle->SetName("minangle");
  minangle->SetNumberOfComponents(1);
  
  vtkFloatArray *maxangle = vtkFloatArray::New();
  maxangle->SetName("maxangle");
  maxangle->SetNumberOfComponents(1);

  vtkFloatArray *medangle = vtkFloatArray::New();
  medangle->SetName("medangle");
  medangle->SetNumberOfComponents(1);

  vtkFloatArray *meanangle = vtkFloatArray::New();
  meanangle->SetName("meanangle");
  meanangle->SetNumberOfComponents(1);

  // 11/30/2010: added variance - mjack
  vtkFloatArray *varangle = vtkFloatArray::New();
  varangle->SetName("varangle");
  varangle->SetNumberOfComponents(1);


  // torsion stats - 08/02/2012 - mjack
  vtkFloatArray *mintorsion = vtkFloatArray::New();
  mintorsion->SetName("mintorsion");
  mintorsion->SetNumberOfComponents(1);
  
  vtkFloatArray *maxtorsion = vtkFloatArray::New();
  maxtorsion->SetName("maxtorsion");
  maxtorsion->SetNumberOfComponents(1);

  vtkFloatArray *medtorsion = vtkFloatArray::New();
  medtorsion->SetName("medtorsion");
  medtorsion->SetNumberOfComponents(1);

  vtkFloatArray *meantorsion = vtkFloatArray::New();
  meantorsion->SetName("meantorsion");
  meantorsion->SetNumberOfComponents(1);

  vtkFloatArray *vartorsion = vtkFloatArray::New();
  vartorsion->SetName("vartorsion");
  vartorsion->SetNumberOfComponents(1);



  vtkFloatArray *minother = vtkFloatArray::New();
  minother->SetName("minother");
  minother->SetNumberOfComponents(1);
  
  vtkFloatArray *maxother = vtkFloatArray::New();
  maxother->SetName("maxother");
  maxother->SetNumberOfComponents(1);

  vtkFloatArray *medother = vtkFloatArray::New();
  medother->SetName("medother");
  medother->SetNumberOfComponents(1);

  vtkFloatArray *meanother = vtkFloatArray::New();
  meanother->SetName("meanother");
  meanother->SetNumberOfComponents(1);

  // 11/30/2010: added variance - mjack
  vtkFloatArray *varother = vtkFloatArray::New();
  varother->SetName("varother");
  varother->SetNumberOfComponents(1);

  
  // 05/22/2012 added normals and vectors - mjack
  vtkFloatArray *normals = vtkFloatArray::New();
  normals->SetNumberOfComponents(3);

  vtkFloatArray *binormals = vtkFloatArray::New();
  binormals->SetName("binormal");
  binormals->SetNumberOfComponents(3);
  
  vtkFloatArray *vectors = vtkFloatArray::New();
  vectors->SetNumberOfComponents(3);


  vtkUnsignedCharArray *dec = vtkUnsignedCharArray::New();
  dec->SetName("rgb");
  dec->SetNumberOfComponents(this->DirectionalColorAlphaBlending ? 4 : 3);

  int ptId, id, iv, ix, iy, niv;
  vtkmpjTensorPoint *sPtr, *sPtr2, *sPrev, *sNext;
  double v[3], vec[3], pv[3];
  double nrm[3], nrm2[3], pnrm[3], pnrm2[3];
  float rgb[4];
  double p0[3], p1[3], p2[3];
  double v0[3], v1[3], v2[3];
  double rad2deg = 180.0 / vtkMath::Pi();

  //iv = this->IntegrationEigenvector;
  //niv = this->NormalEigenvector - 1;

  // Normal cannot be the same eigenvector
  /*if (iv == niv || niv < 0 || niv > 3) {
    ix = (iv + 1) % 3;
    iy = (iv + 2) % 3;
  } else {
    ix = niv;
    iy = (niv + 1) % 3;
    if (iy == iv)
      iy = (iy + 1) % 3;
  }

  printf("Tracking eigenvector: e%d\n", iv+1);
  printf("Normal eigenvector: e%d\n", ix+1);*/

  newScalars->SetNumberOfComponents(3);
  
  for (ptId=0; ptId < this->NumberOfStreamers; ptId+=2)
    {     
      int numPts = this->Streamers[ptId].GetNumberOfPoints();
      int numPts2 = this->Streamers[ptId+1].GetNumberOfPoints();
      
      //sPtr = this->Streamers[ptId].GetmpjTensorPoint(0);	
      //if (ptId < this->NumberOfStreamers-1) {
      //sNext = this->Streamers[ptId+1].GetmpjTensorPoint(0);	
      //printf("pos1=%f,%f,%f\n",sPtr->X[0],sPtr->X[1],sPtr->X[2]);
      //	printf("pos2=%f,%f,%f\n",sNext->X[0],sNext->X[1],sNext->X[2]);
      //}

      int numTotal = numPts + numPts2;
      //printf("numTotal=%d\n",numTotal);

      if ( numTotal >= 2 ) {
	
	// only keep fibers with minimum distance from seed
	sPtr = this->Streamers[ptId].GetmpjTensorPoint(numPts-1);
	sPtr2 = this->Streamers[ptId+1].GetmpjTensorPoint(numPts2-1);
	
	if ( (sPtr->D + sPtr2->D ) >= this->MinimumDistance ) {
	  
	  // allocate next cell
	  // newCells->InsertNextCell(numTotal-1);
	  
	  // Modified by mjack: 22/02/2010
	  // Be able to display mean and median values of fiber angle
	  vector<double> angles;
	  vector<double> torsions;  // newest addition
	  vector<double> others;
	  double sumangle = 0;
	  double sumother = 0;
	  double sumtorsion = 0;
	  double sumangle2 = 0;
	  double sumtorsion2 = 0;
	  double sumother2 = 0;

	  int total1 = 0;
	  int total2 = 0;
	  for(int i=numPts-1; i>=0; i--) {
	    sPtr = this->Streamers[ptId].GetmpjTensorPoint(i);

	    if (( sPtr->FA >= this->MinimumFA ) && ( sPtr->FA < this->MaximumFA ) &&
		( sPtr->MD >= this->MinimumMD ) && ( sPtr->MD < this->MaximumMD ) &&
		( sPtr->Angle <= this->MaximumAngle )) {

	      angles.push_back(sPtr->Angle);
	      torsions.push_back(sPtr->Torsion);
	      others.push_back(sPtr->S);

	      sumangle += sPtr->Angle;
	      sumangle2 += sPtr->Angle * sPtr->Angle;  // new by mjack at 11/30/2010

	      sumtorsion += sPtr->Torsion;
	      sumtorsion2 += sPtr->Torsion * sPtr->Torsion;  // new by mjack at 08/02/2012
	      
	      sumother += sPtr->S;
	      sumother2 += sPtr->S * sPtr->S;  // new by mjack at 11/30/2010

	      total1++;

	    }
	  }
	  
	  // Now for the second segment of the fiber
	  for(int i=1; i<numPts2; i++) {
	    sPtr = this->Streamers[ptId+1].GetmpjTensorPoint(i);

	    if (( sPtr->FA >= this->MinimumFA ) && ( sPtr->FA < this->MaximumFA ) &&
		( sPtr->MD >= this->MinimumMD ) && ( sPtr->MD < this->MaximumMD ) &&
		( sPtr->Angle <= this->MaximumAngle )) {
	      
	      angles.push_back(sPtr->Angle);
	      torsions.push_back(sPtr->Torsion);
	      others.push_back(sPtr->S);
	      //printf("angle=%f\n",sPtr->Angle);

	      sumangle += sPtr->Angle;
	      sumangle2 += sPtr->Angle*sPtr->Angle; // new by mjack at 11/30/2010
	      
	      sumtorsion += sPtr->Torsion;
	      sumtorsion2 += sPtr->Torsion * sPtr->Torsion;  // new by mjack at 08/02/2012

	      sumother += sPtr->S;
	      sumother2 += sPtr->S*sPtr->S;  // new by mjack at 11/30/2010

	      total2++;
	    }
	  }

	  //printf("total1=%d, total2=%d\n", total1, total2);

	  // sort them
	  sort(angles.begin(), angles.end());
	  sort(torsions.begin(), torsions.end());
	  sort(others.begin(), others.end());

	  // calculate stats	  
	  int total = angles.size();
	  double min_angle = angles[0];
	  double max_angle = angles[total-1];
	  double med_angle = angles[total / 2];
	  double mean_angle = sumangle / (double)total;
	  double var_angle = (sumangle2 - (((double)total)*mean_angle*mean_angle)) / ((double) total-1);

	  double min_torsion = torsions[0];
	  double max_torsion = torsions[total-1];
	  double med_torsion = torsions[total / 2];
	  double mean_torsion = sumtorsion / (double)total;
	  double var_torsion = (sumtorsion2 - (((double)total)*mean_torsion*mean_torsion)) / ((double) total-1);

	  // calculate stats
	  double min_other = others[0];
	  double max_other = others[total-1];
	  double med_other = others[total / 2];
	  double mean_other = sumother / (double)total;
	  double var_other = (sumother2 - (((double)total)*mean_other*mean_other)) / ((double) total-1);
	  
	  //printf("stats=%f %f - %f %f\n",min_angle,max_angle,med_angle,mean_angle);
	  // double mean_angle = sum_angle / total;

	  // allocate next cell
	  newCells->InsertNextCell(angles.size());

	  // first segment of the fiber
	  //	  for(int i=0; i<numPts; i++) {
	  for (int i=numPts-1; i>=0; i--) {
	   
	    sPtr = this->Streamers[ptId].GetmpjTensorPoint(i);	
	    sPrev = i < numPts-1 ? this->Streamers[ptId].GetmpjTensorPoint(i+1) : sPtr;

	    if (( sPtr->FA >= this->MinimumFA ) && ( sPtr->FA < this->MaximumFA ) &&
		( sPtr->MD >= this->MinimumMD ) && ( sPtr->MD < this->MaximumMD ) &&
		( sPtr->Angle <= this->MaximumAngle )) {

	      // insert next point
	      id = newPts->InsertNextPoint(sPtr->X);
	      
	      // insert next cell
	      newCells->InsertCellPoint(id);
	      
	      // insert scalars(s)
	      if (this->ComputeNormals)
		newScalars->InsertNextTuple3(sPtr->W[0], sPtr->W[1], sPtr->W[2]);
	      else
		newScalars->InsertNextTuple3(1.0, 1.0, 1.0);

	      // insert fields
	      fa->InsertNextTuple(&sPtr->FA);
	      md->InsertNextTuple(&sPtr->MD);
	      dist->InsertNextTuple(&sPtr->D);
	      angle->InsertNextTuple(&sPtr->Angle);
	      torsion->InsertNextTuple(&sPtr->Torsion);
	      roi->InsertNextTuple(&sPtr->M);
	      other->InsertNextTuple(&sPtr->S);
	      
	      if (this->StatisticsMap) {
		
		minangle->InsertNextTuple(&min_angle);
		maxangle->InsertNextTuple(&max_angle);
		medangle->InsertNextTuple(&med_angle);
		meanangle->InsertNextTuple(&mean_angle);
		varangle->InsertNextTuple(&var_angle);

		mintorsion->InsertNextTuple(&min_torsion);
		maxtorsion->InsertNextTuple(&max_torsion);
		medtorsion->InsertNextTuple(&med_torsion);
		meantorsion->InsertNextTuple(&mean_torsion);
		vartorsion->InsertNextTuple(&var_torsion);

		minother->InsertNextTuple(&min_other);
		maxother->InsertNextTuple(&max_other);
		medother->InsertNextTuple(&med_other);
		meanother->InsertNextTuple(&mean_other);
		varother->InsertNextTuple(&var_other);

	      }
	      
	      // insert vectors
	      iv = this->IntegrationEigenvector;
	      vec[0] = sPtr->V[0][iv];
	      vec[1] = sPtr->V[1][iv];
	      vec[2] = sPtr->V[2][iv];
	      vectors->InsertNextTuple(vec);

	      // insert normals and binormals
	      if (this->ComputeNormals) {
		ix = this->NormalEigenvector;
		nrm[0] = sPtr->V[0][ix];
		nrm[1] = sPtr->V[1][ix];
		nrm[2] = sPtr->V[2][ix];
		normals->InsertNextTuple(nrm);

		ix = this->BinormalEigenvector;
		nrm[0] = sPtr->V[0][ix];
		nrm[1] = sPtr->V[1][ix];
		nrm[2] = sPtr->V[2][ix];
		binormals->InsertNextTuple(nrm);
	      }

	      // insert color coding
	      if (this->DirectionalColormap) {
		
		v[0] = sPtr->V[0][iv]; 
		v[1] = sPtr->V[1][iv]; 
		v[2] = sPtr->V[2][iv];
		
		switch(this->DirectionalColorScheme) {
		case 1: 
		  this->RotationalSymmetryScheme(sPtr->FA, v, rgb);
		  break;
		case 2:
		  this->NoSymmetryScheme(sPtr->FA, v, rgb);
		  break;
		case 3:
		  this->MirrorSymmetryScheme(sPtr->FA, v, rgb);
		  break;
		default:		
		  this->AbsoluteValueScheme(sPtr->FA, v, rgb);
		}	
		
		if (this->DirectionalColorAlphaBlending)
		  rgb[3] = sPtr->FA*255;
		
		dec->InsertNextTuple(rgb);
	      }
	    }
	  }

	  // second segment of the fiber
	  for(int i=1; i<numPts2; i++) {

	    sPtr = this->Streamers[ptId+1].GetmpjTensorPoint(i);
	    //sPrev = i > 1 ? this->Streamers[ptId+1].GetmpjTensorPoint(i-1) : sPtr;
	    sPrev = this->Streamers[ptId+1].GetmpjTensorPoint(i-1);

	    if (( sPtr->FA >= this->MinimumFA ) && ( sPtr->FA < this->MaximumFA ) &&
		( sPtr->MD >= this->MinimumMD ) && ( sPtr->MD < this->MaximumMD ) &&
		( sPtr->Angle <= this->MaximumAngle ) ) {

	      // insert next point
	      id = newPts->InsertNextPoint(sPtr->X);
	      
	      // insert next cell
	      newCells->InsertCellPoint(id);
	      
	      // insert scalars(s)
	      //newScalars->InsertNextTuple(&sPtr->W);   
	      if (this->ComputeNormals)
		newScalars->InsertNextTuple3(sPtr->W[0], sPtr->W[1], sPtr->W[2]);
	      else
		newScalars->InsertNextTuple3(1.0, 1.0, 1.0);		

	      // insert fields
	      fa->InsertNextTuple(&sPtr->FA);
	      md->InsertNextTuple(&sPtr->MD);
	      dist->InsertNextTuple(&sPtr->D);
	      angle->InsertNextTuple(&sPtr->Angle);
	      torsion->InsertNextTuple(&sPtr->Torsion);
	      roi->InsertNextTuple(&sPtr->M);	   
	      other->InsertNextTuple(&sPtr->S);	 
	      
	      if (this->StatisticsMap) {
		
		minangle->InsertNextTuple(&min_angle);
		maxangle->InsertNextTuple(&max_angle);
		medangle->InsertNextTuple(&med_angle);
		meanangle->InsertNextTuple(&mean_angle);
		varangle->InsertNextTuple(&var_angle);

		mintorsion->InsertNextTuple(&min_torsion);
		maxtorsion->InsertNextTuple(&max_torsion);
		medtorsion->InsertNextTuple(&med_torsion);
		meantorsion->InsertNextTuple(&mean_torsion);
		vartorsion->InsertNextTuple(&var_torsion);

		minother->InsertNextTuple(&min_other);
		maxother->InsertNextTuple(&max_other);
		medother->InsertNextTuple(&med_other);
		meanother->InsertNextTuple(&mean_other);
		varother->InsertNextTuple(&var_other);
		
	      }
	      
	      // insert vector
	      iv = this->IntegrationEigenvector;
	      vec[0] = sPtr->V[0][iv];
	      vec[1] = sPtr->V[1][iv];
	      vec[2] = sPtr->V[2][iv];
	      vectors->InsertNextTuple(vec);
	      
	      // insert normals
	      if (this->ComputeNormals) {
		ix = this->NormalEigenvector;
		nrm[0] = sPtr->V[0][ix];
		nrm[1] = sPtr->V[1][ix];
		nrm[2] = sPtr->V[2][ix];
		normals->InsertNextTuple(nrm);

		ix = this->BinormalEigenvector;
		nrm[0] = sPtr->V[0][ix];
		nrm[1] = sPtr->V[1][ix];
		nrm[2] = sPtr->V[2][ix];
		binormals->InsertNextTuple(nrm);
	      }

	      // insert color coding
	      if (this->DirectionalColormap) {
		
		v[0] = sPtr->V[0][iv]; 
		v[1] = sPtr->V[1][iv]; 
		v[2] = sPtr->V[2][iv];
		
		switch(this->DirectionalColorScheme) {
		case 1: 
		  this->RotationalSymmetryScheme(sPtr->FA, v, rgb);
		  break;
		case 2:
		  this->NoSymmetryScheme(sPtr->FA, v, rgb);
		  break;
		case 3:
		  this->MirrorSymmetryScheme(sPtr->FA, v, rgb);
		  break;
		default:		
		  this->AbsoluteValueScheme(sPtr->FA, v, rgb);
		}
		
		if (this->DirectionalColorAlphaBlending) 
		  rgb[3] = sPtr->FA*255;
		
		dec->InsertNextTuple(rgb);
	      }
	    }
	  }
	  
	}
      }
    }
    
  outPD->SetScalars(newScalars);
  newScalars->Delete();
  
  outPD->AddArray(fa);
  fa->Delete();

  outPD->AddArray(md);
  md->Delete();

  outPD->AddArray(dist);
  dist->Delete();

  outPD->AddArray(angle);
  angle->Delete();

  outPD->AddArray(roi);
  roi->Delete();
  
  outPD->AddArray(other);
  other->Delete();

  // insert stats
  if (this->StatisticsMap) {

    outPD->AddArray(minangle);
    minangle->Delete();
    
    outPD->AddArray(maxangle);
    maxangle->Delete();

    outPD->AddArray(medangle);
    medangle->Delete();

    outPD->AddArray(meanangle);
    meanangle->Delete();

    outPD->AddArray(varangle);
    varangle->Delete();


    outPD->AddArray(mintorsion);
    mintorsion->Delete();
    
    outPD->AddArray(maxtorsion);
    maxtorsion->Delete();

    outPD->AddArray(medtorsion);
    medtorsion->Delete();

    outPD->AddArray(meantorsion);
    meantorsion->Delete();

    outPD->AddArray(vartorsion);
    vartorsion->Delete();


    outPD->AddArray(minother);
    minother->Delete();
    
    outPD->AddArray(maxother);
    maxother->Delete();

    outPD->AddArray(medother);
    medother->Delete();

    outPD->AddArray(meanother);
    meanother->Delete();

    outPD->AddArray(varother);
    varother->Delete();
  }

  // insert color coding
  if (this->DirectionalColormap) {
    outPD->AddArray(dec);
  }
  dec->Delete();

  // insert tracing vectors
  outPD->SetVectors(vectors);

  // insert normals and torsion values
  if (this->ComputeNormals > 0) {
    outPD->SetNormals(normals);
    outPD->AddArray(binormals);
  }
  normals->Delete();
  binormals->Delete();

  outPD->AddArray(torsion);
  torsion->Delete();

  output->SetPoints(newPts);
  newPts->Delete();

  output->SetLines(newCells);
  newCells->Delete();
}

void vtkmpjTensorStreamOld::ProjectVectorOntoPlane(double v[3], double n[3], double r[3])
{
  double d = vtkMath::Dot(v, n);
  r[0] = v[0] - n[0] * d; 
  r[1] = v[1] - n[1] * d; // r = v - (v . n) n
  r[2] = v[2] - n[2] * d;
}

void vtkmpjTensorStreamOld::InterpolateTensor(vtkCell *cell, vtkDataArray *cellTensors, double *w, double *m[3])
{
  int i, j, k;
  double *tensor;
    
  // reset tensor
  for (j=0; j<3; j++)
    for (i=0; i<3; i++)
      {
	m[i][j] = 0.0;
      }
  
  int numcomp=cellTensors->GetNumberOfComponents();

  // interpolate tensor
  for (k=0; k < cell->GetNumberOfPoints(); k++)
    {
      tensor = cellTensors->GetTuple(k);
      
      if (numcomp == 6) {

	m[0][0] += tensor[0] * w[k];
	m[1][0] += tensor[1] * w[k]; 
	m[0][1] += tensor[1] * w[k];
	m[2][0] += tensor[2] * w[k]; 
	m[0][2] += tensor[2] * w[k];		
	m[1][1] += tensor[3] * w[k]; 
	m[2][1] += tensor[4] * w[k]; 
	m[1][2] += tensor[4] * w[k];
	m[2][2] += tensor[5] * w[k];			

	//printf("%f %f %f %f %f %f -- ",m[0][0],m[1][0],m[2][0],m[1][1],m[2][1],m[2][2]);	

      } else {
	
	m[0][0] += tensor[0] * w[k];
	m[1][0] += tensor[1] * w[k]; 
	m[0][1] += tensor[1] * w[k];
	m[2][0] += tensor[2] * w[k]; 
	m[0][2] += tensor[2] * w[k];	
	m[1][1] += tensor[4] * w[k]; 
	m[2][1] += tensor[5] * w[k]; 
	m[1][2] += tensor[5] * w[k];
	m[2][2] += tensor[8] * w[k];			

	//printf("%f %f %f %f %f %f -- ",m[0][0],m[1][0],m[2][0],m[1][1],m[2][1],m[2][2]);	
      } 	
    }
}

void vtkmpjTensorStreamOld::AbsoluteValueScheme(float FA, double v[3], float nrgb[3])
{
  float mgn;
  float rgb[3];
  float scale = this->DirectionalColorScaling;

  rgb[0] = rgb[1] = rgb[2] = 0;
  
  rgb[0] = fabs(v[0]);
  rgb[1] = fabs(v[1]);
  rgb[2] = fabs(v[2]);
  
  /* compute color */
  mgn = (this->DirectionalColorModulate ? FA : 1.0);

  rgb[0] *= mgn;
  rgb[1] *= mgn;
  rgb[2] *= mgn;
  
  /* RGBs have to be in [0,255] */
  nrgb[0] = rgb[0]*255*scale;
  nrgb[1] = rgb[1]*255*scale;
  nrgb[2] = rgb[2]*255*scale;
  
  nrgb[0] = nrgb[0] < 256 ? nrgb[0] : 255;
  nrgb[1] = nrgb[1] < 256 ? nrgb[1] : 255;
  nrgb[2] = nrgb[2] < 256 ? nrgb[2] : 255;
  
}

void vtkmpjTensorStreamOld::RotationalSymmetryScheme(float FA, double v[3], float nrgb[3])
{

  float rgb[3], hsv[3];
  float theta, phi;
  float vlenxy, vlen;
  float pi = vtkMath::Pi();
  float ps = this->DirectionalColorSaturation;
  float scale = this->DirectionalColorScaling;

  rgb[0] = rgb[1] = rgb[2] = 0;
  
  nrgb[0] = v[0];
  nrgb[1] = v[1];
  nrgb[2] = v[2];
  
  /* normalize vector */
  vlenxy = vtkMath::Norm2D(v);

  /* compute hue */
  if (v[0] > 0)
    phi = asin(v[1] / vlenxy);
  else
    phi = pi - asin(v[1] / vlenxy);
  hsv[0] = fmod((phi+2*pi+pi/2)*2,2*pi) / (2*pi);
  if (hsv[0] > 1.0) hsv[0] = 1.0;
  
  /* compute saturation */
  vlen = vtkMath::Norm(v);
  theta = asin(vlenxy / vlen);
  if (theta > 1) theta = 1.0;
  hsv[1] = sin(ps*theta) / sin(ps*pi/2);
  
  /* compute value */
  hsv[2] = (this->DirectionalColorModulate ? FA : 1.0);
  
  vtkMath::HSVToRGB(hsv, rgb);

  /* RGBs have to be in [0,255] */
  nrgb[0] = rgb[0]*255+scale*nrgb[0];
  nrgb[1] = rgb[1]*255+scale*nrgb[1];
  nrgb[2] = rgb[2]*255+scale*nrgb[2];
  
  nrgb[0] = nrgb[0] < 256 ? nrgb[0] : 255;
  nrgb[1] = nrgb[1] < 256 ? nrgb[1] : 255;
  nrgb[2] = nrgb[2] < 256 ? nrgb[2] : 255;
}


void vtkmpjTensorStreamOld::NoSymmetryScheme(float FA, double v[3], float nrgb[3])
{
  float rgb[3], hsv[3];
  float theta, phi;
  float vlenxy, vlen;
  float pi = vtkMath::Pi();
  float ps = this->DirectionalColorSaturation;
  float scale = this->DirectionalColorScaling;
  
  rgb[0] = rgb[1] = rgb[2] = 0;
  
  nrgb[0] = v[0];
  nrgb[1] = v[1];
  nrgb[2] = v[2];

  /* normalize vector */
  vlenxy = vtkMath::Norm2D(v);
  
  /* compute hue */
  if (v[0] > 0)
    phi = asin(v[1] / vlenxy);
  else
    phi = pi - asin(v[1] / vlenxy);
  hsv[0] = fmod(phi+2*pi,2*pi) / (2*pi);
  if (hsv[0] > 1.0) hsv[0] = 1.0;
  
  /* compute saturation */
  vlen = vtkMath::Norm(v);
  theta = asin(vlenxy / vlen);
  if (theta > 1) theta = 1.0;
  hsv[1] = sin(ps*theta) / sin(ps*pi/2);
    
  /* compute value */
  hsv[2] = (this->DirectionalColorModulate ? FA : 1.0);
  
  vtkMath::HSVToRGB(hsv, rgb);
  
  /* RGBs have to be in [0,255] */
  nrgb[0] = rgb[0]*255+scale*nrgb[0];
  nrgb[1] = rgb[1]*255+scale*nrgb[1];
  nrgb[2] = rgb[2]*255+scale*nrgb[2];
  
  nrgb[0] = nrgb[0] < 256 ? nrgb[0] : 255;
  nrgb[1] = nrgb[1] < 256 ? nrgb[1] : 255;
  nrgb[2] = nrgb[2] < 256 ? nrgb[2] : 255;
  
}


void vtkmpjTensorStreamOld::MirrorSymmetryScheme(float FA, double v[3], float nrgb[3])
{
  float rgb[3], hsv[3];
  float theta, phi;
  float vlenxy, vlen;
  float pi = vtkMath::Pi();
  float ps = this->DirectionalColorSaturation;
  float scale = this->DirectionalColorScaling;

  rgb[0] = rgb[1] = rgb[2] = 0;
  
  nrgb[0] = v[0];
  nrgb[1] = v[1];
  nrgb[2] = v[2];
  
  /* normalize vector */
  vlenxy = vtkMath::Norm2D(v);
  
  /* compute hue */
  if (v[0] > 0)
    phi = asin(v[1] / vlenxy);
  else
    phi = pi + asin(v[1] / vlenxy);
  hsv[0] = 2 * fmod(phi + pi, pi) / (2*pi);
  if (hsv[0] > 1.0) hsv[0] = 1.0;
  
  /* compute saturation */
  vlen = vtkMath::Norm(v);
  theta = asin(vlenxy / vlen);
  if (theta > 1) theta = 1.0;
  hsv[1] = sin(ps*theta) / sin(ps*pi/2);
  
  /* compute value */
  hsv[2] = (this->DirectionalColorModulate ? FA : 1.0);
  
  vtkMath::HSVToRGB(hsv, rgb);
  
  /* RGBs have to be in [0,255] */
  nrgb[0] = rgb[0]*255+scale*nrgb[0];
  nrgb[1] = rgb[1]*255+scale*nrgb[1];
  nrgb[2] = rgb[2]*255+scale*nrgb[2];
  
  nrgb[0] = nrgb[0] < 256 ? nrgb[0] : 255;
  nrgb[1] = nrgb[1] < 256 ? nrgb[1] : 255;
  nrgb[2] = nrgb[2] < 256 ? nrgb[2] : 255;
}

void vtkmpjTensorStreamOld::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkDataSetToPolyDataFilter::PrintSelf(os,indent);

  if ( this->StartFrom == VTK_START_FROM_POSITION )
    {
    os << indent << "Starting Position: (" << this->StartPosition[0] << ","
       << this->StartPosition[1] << ", " << this->StartPosition[2] << ")\n";
    }
  else 
    {
    os << indent << "Starting Location:\n\tCell: " << this->StartCell 
       << "\n\tSubId: " << this->StartSubId << "\n\tP.Coordinates: ("
       << this->StartPCoords[0] << ", " 
       << this->StartPCoords[1] << ", " 
       << this->StartPCoords[2] << ")\n";
    }

  os << indent << "Maximum Propagation Distance: " 
     << this->MaximumDistance << "\n";

  if ( this->IntegrationDirection == VTK_MPJ_INTEGRATE_FORWARD )
    {
    os << indent << "Integration Direction: FORWARD\n";
    }
  else if ( this->IntegrationDirection == VTK_MPJ_INTEGRATE_BACKWARD )
    {
    os << indent << "Integration Direction: BACKWARD\n";
    }
  else
    {
    os << indent << "Integration Direction: FORWARD & BACKWARD\n";
    }

  os << indent << "Integration Step Length: " << this->IntegrationStepLength << "\n";
  os << indent << "Step Length: " << this->StepLength << "\n";

  if ( this->IntegrationEigenvector == 0 )
    {
    os << indent << "Integrate Along Major Eigenvector\n";
    }
  else if ( this->IntegrationEigenvector == 1 )
    {
    os << indent << "Integrate Along Medium Eigenvector\n";
    }
  else
    {
    os << indent << "Integrate Along Minor Eigenvector\n";
    }
}


