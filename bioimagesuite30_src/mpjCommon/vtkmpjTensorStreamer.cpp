/*=========================================================================

  Module:    vtkmpjTensorStreamer.cpp

 Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkmpjTensorStreamer.h"

#include "vtkCell.h"
#include "vtkDataSet.h"
#include "vtkDoubleArray.h"
#include "vtkExecutive.h"
#include "vtkGenericCell.h"
#include "vtkInformation.h"
#include "vtkmpjInterpolatedTensorField.h"
#include "vtkMath.h"
#include "vtkMultiThreader.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkRungeKutta2.h"
#include "vtkRungeKutta4.h"

vtkCxxSetObjectMacro(vtkmpjTensorStreamer,Integrator,vtkInitialValueProblemSolver);

#define VTK_MPJ_START_FROM_POSITION 0
#define VTK_MPJ_START_FROM_LOCATION 1

static const double VTK_EPSILON=1E-12;

struct vtkmpjTensorStreamerThreadStruct
{
  vtkmpjTensorStreamer *Filter;
  vtkDataSet *Tensor;
  vtkDataSet *Source;
  vtkDataSet *Mask;
  vtkDataSet *Scalars;
};

vtkmpjTensorStreamer::TensorStreamPoint::TensorStreamPoint()
{
  this->v[0] = v0;
  this->v[1] = v1;
  this->v[2] = v2;
}

vtkmpjTensorStreamer::TensorStreamArray::TensorStreamArray()
{
  this->MaxId = -1; 
  this->Array = new vtkmpjTensorStreamer::TensorStreamPoint[1000];
  this->Size = 1000;
  this->Extend = 5000;
  this->Direction = VTK_MPJ_INTEGRATE_FORWARD;
}

vtkmpjTensorStreamer::TensorStreamPoint *vtkmpjTensorStreamer::TensorStreamArray::Resize(vtkIdType sz)
{
  vtkmpjTensorStreamer::TensorStreamPoint *newArray;
  vtkIdType newSize;

  if (sz >= this->Size)
    {
    newSize = this->Size + 
      this->Extend*(((sz-this->Size)/this->Extend)+1);
    }
  else
    {
    newSize = sz;
    }

  newArray = new vtkmpjTensorStreamer::TensorStreamPoint[newSize];

  memcpy(newArray, this->Array,
         static_cast<size_t>(sz < this->Size ? sz : this->Size)
         * sizeof(vtkmpjTensorStreamer::TensorStreamPoint));

  this->Size = newSize;
  delete [] this->Array;
  this->Array = newArray;

  return this->Array;
}

// Construct object to start from position (0,0,0); integrate forward; terminal
// speed 0.0; vorticity computation off; integrations step length 0.2; and
// maximum propagation time 100.0.
vtkmpjTensorStreamer::vtkmpjTensorStreamer()
{
  this->StartFrom = VTK_MPJ_START_FROM_POSITION;

  this->StartCell = 0;
  this->StartSubId = 0;
  this->StartPCoords[0] = this->StartPCoords[1] = this->StartPCoords[2] = 0.5;
  this->StartPosition[0] = this->StartPosition[1] = this->StartPosition[2] = 0.0;
  this->Streamers = NULL;
 
  this->IntegrationDirection = VTK_MPJ_INTEGRATE_FORWARD;
  this->IntegrationStepLength = 0.2;
  
  //this->StepLength = 0.01;
  this->MaximumDistance = 100.0;
  this->MinimumDistance = 5;

  this->MaximumAngle = 90.0;
  this->MaximumTorsion = 90.0;
  this->MinimumFA = 0.1;
  this->MaximumFA = 1.0;
  this->MinimumMD = 0.0;
  this->MaximumMD = VTK_LARGE_FLOAT;

  this->IntegrationEigenvector = 0; // Major eigenvector
  this->NormalEigenvector = 1;      // Medium
  this->BinormalEigenvector = 2;    // Minor

  this->NumberOfStreamers = 0;
  this->Epsilon=VTK_EPSILON;

  this->FlipX = false;
  this->FlipY = false;
  this->FlipZ = false;
  this->FixEigenvectors = false;
  
  this->Threader = vtkMultiThreader::New();
  this->NumberOfThreads = this->Threader->GetNumberOfThreads();
  this->Integrator = vtkRungeKutta2::New();
  //this->SavePointInterval = 0.00001;

  this->ConstrainPropagationToMask = false;

  this->SetNumberOfInputPorts(4);
}

vtkmpjTensorStreamer::~vtkmpjTensorStreamer()
{
  if( this->Streamers )
    {
    delete [] this->Streamers;
    }

  if (this->Threader)
    {
    this->Threader->Delete();
    }
  this->SetIntegrator(0);
}

void vtkmpjTensorStreamer::SetSource(vtkDataSet *source)
{
  this->SetInput(1, source);
}

void vtkmpjTensorStreamer::SetMask(vtkDataSet *mask)
{
  this->SetInput(2, mask);
}

void vtkmpjTensorStreamer::SetScalars(vtkDataSet *scalars)
{
  this->SetInput(3, scalars);
}

vtkDataSet *vtkmpjTensorStreamer::GetSource()
{
  if (this->GetNumberOfInputConnections(1) < 1)
    {
    return NULL;
    }
  return vtkDataSet::SafeDownCast(
    this->GetExecutive()->GetInputData(1, 0));
}

vtkDataSet *vtkmpjTensorStreamer::GetMask()
{
  if (this->GetNumberOfInputConnections(2) < 1)
    {
    return NULL;
    }
  return vtkDataSet::SafeDownCast(
    this->GetExecutive()->GetInputData(2, 0));
}

vtkDataSet *vtkmpjTensorStreamer::GetScalars()
{
  if (this->GetNumberOfInputConnections(3) < 1)
    {
    return NULL;
    }
  return vtkDataSet::SafeDownCast(
    this->GetExecutive()->GetInputData(3, 0));
}

// Specify the start of the streamline in the cell coordinate system. That is,
// cellId and subId (if composite cell), and parametric coordinates.
void vtkmpjTensorStreamer::SetStartLocation(vtkIdType cellId, int subId,
                                   double pcoords[3])
{
  if ( cellId != this->StartCell || subId != this->StartSubId ||
       pcoords[0] !=  this->StartPCoords[0] || 
       pcoords[1] !=  this->StartPCoords[1] || 
       pcoords[2] !=  this->StartPCoords[2] )
    {
    this->Modified();
    this->StartFrom = VTK_MPJ_START_FROM_LOCATION;

    this->StartCell = cellId;
    this->StartSubId = subId;
    this->StartPCoords[0] = pcoords[0];
    this->StartPCoords[1] = pcoords[1];
    this->StartPCoords[2] = pcoords[2];
    }
}

// Specify the start of the streamline in the cell coordinate system. That is,
// cellId and subId (if composite cell), and parametric coordinates.
void vtkmpjTensorStreamer::SetStartLocation(vtkIdType cellId, int subId, double r,
                                   double s, double t)
{
  double pcoords[3];
  pcoords[0] = r;
  pcoords[1] = s;
  pcoords[2] = t;

  this->SetStartLocation(cellId, subId, pcoords);
}

// Get the starting location of the streamline in the cell coordinate system.
vtkIdType vtkmpjTensorStreamer::GetStartLocation(int& subId, double pcoords[3])
{
  subId = this->StartSubId;
  pcoords[0] = this->StartPCoords[0];
  pcoords[1] = this->StartPCoords[1];
  pcoords[2] = this->StartPCoords[2];
  return this->StartCell;
}

// Specify the start of the streamline in the global coordinate system. Search
// must be performed to find initial cell to start integration from.
void vtkmpjTensorStreamer::SetStartPosition(double x[3])
{
  if ( x[0] != this->StartPosition[0] || x[1] != this->StartPosition[1] || 
       x[2] != this->StartPosition[2] )
    {
    this->Modified();
    this->StartFrom = VTK_MPJ_START_FROM_POSITION;

    this->StartPosition[0] = x[0];
    this->StartPosition[1] = x[1];
    this->StartPosition[2] = x[2];
    }
}

// Specify the start of the streamline in the global coordinate system. Search
// must be performed to find initial cell to start integration from.
void vtkmpjTensorStreamer::SetStartPosition(double x, double y, double z)
{
  double pos[3];
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;

  this->SetStartPosition(pos);
}

// Get the start position in global x-y-z coordinates.
double *vtkmpjTensorStreamer::GetStartPosition()
{
  return this->StartPosition;
}

// Use the major eigenvector field as the vector field through which to 
// integrate. The major eigenvector is the eigenvector whose corresponding
// eigenvalue is closest to positive infinity.
void vtkmpjTensorStreamer::IntegrateMajorEigenvector()
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
void vtkmpjTensorStreamer::IntegrateMediumEigenvector()
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
void vtkmpjTensorStreamer::IntegrateMinorEigenvector()
{
  if ( this->IntegrationEigenvector != 2 )
    {
    this->Modified();
    this->IntegrationEigenvector = 2;
  }
}

VTK_THREAD_RETURN_TYPE vtkmpjTensorStreamer::ThreadedIntegrate( void *arg )
{
  vtkmpjTensorStreamer              *self;
  vtkmpjTensorStreamerThreadStruct  *str;
  int                      thread_count;
  int                      thread_id;
  vtkmpjTensorStreamer::TensorStreamArray *streamer;
  vtkmpjTensorStreamer::TensorStreamPoint *sNext = 0, *sPtr;
  vtkmpjTensorStreamer::TensorStreamPoint pt1, pt2;
  int                      i, iv, ix, iy;
  //, iz;
  vtkIdType                idxNext, ptId;
  double                   d, step, dir;
  double                   xNext[3];
  //, vel[3];
  //double                   *cellVel;
  //double                   derivs[9];
  double                   *w;
  //, pcoords[3]; 
  double                   coords[4];
  vtkDataSet               *tensor;
  vtkGenericCell           *cell;
  vtkPointData             *pdt;
  vtkDataArray             *inScalars=0;
  vtkDataArray             *inTensors=0;
  vtkDataArray             *inMask=0;
  vtkDoubleArray           *cellTensors=0;
  vtkDataArray             *cellScalars=0;
  vtkDataArray             *cellMask=0;
  //double tOffset, vort[3];
  double err;
  int counter=0;

  double angle, temp;
  //, torsion;
  //double w0, w1, w2;
  double *m[3], *v[3];
  //, ev[3];
  double m0[3], m1[3], m2[3];
  double v0[3], v1[3], v2[3], vec[3];
  //double ten[9];
  
  // set up working matrices
  v[0] = v0; v[1] = v1; v[2] = v2; 
  m[0] = m0; m[1] = m1; m[2] = m2; 

  vtkMultiThreader::ThreadInfo *info=
    static_cast<vtkMultiThreader::ThreadInfo *>(arg);
  
  thread_id = info->ThreadID;
  thread_count = info->NumberOfThreads;
  str = static_cast<vtkmpjTensorStreamerThreadStruct *>(info->UserData);
  self = str->Filter;

  iv = self->GetIntegrationEigenvector();
  ix = self->GetNormalEigenvector();
  iy = self->GetBinormalEigenvector();
   
  //fprintf(stderr,"thread id=%d has started!\n",thread_id);
  tensor    = str->Tensor;
  
  // tensor information
  pdt       = tensor->GetPointData();
  inTensors = pdt->GetScalars();
  //int numcomp = inTensors->GetNumberOfComponents();

  cell = vtkGenericCell::New();
  cellTensors = vtkDoubleArray::New();
  cellTensors->SetNumberOfComponents(inTensors->GetNumberOfComponents());
  cellTensors->Allocate(inTensors->GetNumberOfComponents()*VTK_CELL_SIZE);

  // mask information
  if (self->GetMask() != NULL) {
    inMask = str->Mask->GetPointData()->GetScalars();
    cellMask = vtkDataArray::CreateDataArray(inMask->GetDataType());
    cellMask->SetNumberOfComponents(1);
    cellMask->SetNumberOfTuples(VTK_CELL_SIZE);
  }

  // scalars information
  if (self->GetScalars() != NULL) {
    inScalars = str->Scalars->GetPointData()->GetScalars();
    cellScalars = inScalars->NewInstance();
    cellScalars->SetNumberOfComponents(inScalars->GetNumberOfComponents());
    cellScalars->Allocate(inScalars->GetNumberOfComponents()*VTK_CELL_SIZE);
  }
  
  //fprintf(stderr,"here9\n");

  w = new double[tensor->GetMaxCellSize()];

  // Set the function set to be integrated
  vtkmpjInterpolatedTensorField* func = vtkmpjInterpolatedTensorField::New();
  func->AddDataSet(tensor);
  func->SetFixEigenvectors(true);
  func->SetEigenvector(iv);
  func->SetNormalEigenvector(ix);
  func->SetBinormalEigenvector(iy);
  func->SetFlipX(self->GetFlipX());
  func->SetFlipY(self->GetFlipY());
  func->SetFlipZ(self->GetFlipZ());

  if (self->GetIntegrator() == 0)
  {
    vtkGenericWarningMacro("No integrator is specified.");
    return VTK_THREAD_RETURN_VALUE;
  }

  // Create a new integrator, the type is the same as Integrator
  vtkInitialValueProblemSolver* integrator = 
    self->GetIntegrator()->NewInstance();
  integrator->SetFunctionSet(func);

   //fprintf(stderr,"here10\n");

  // Used to avoid calling these function many times during
  // the integration
  double maxdist = self->GetMaximumDistance();
  
  double minFA = self->GetMinimumFA();
  double maxFA = self->GetMaximumFA();
  double minMD = self->GetMinimumMD();
  double maxMD = self->GetMaximumMD();
  double maxAngle = self->GetMaximumAngle();
  double maxTorsion = self->GetMaximumTorsion();

  //double savePointInterval = self->GetSavePointInterval();
  //fprintf(stderr,"here11\n");

  // For each streamer, integrate in appropriate direction
  // Do only the streamers that this thread should handle.
  for (ptId=0; ptId < self->GetNumberOfStreamers(); ptId++) 
  {
    if ( ptId % thread_count == thread_id )
      {
      // Get starting step
      streamer = self->GetStreamers() + ptId;
      sPtr = streamer->GetTensorStreamPoint(0);
      if ( sPtr->cellId < 0 )
        {
        continue;
        }

      //fprintf(stderr,"thread id=%d has started tracking...\n",thread_id);

      // Set the last cell id in the vtkInterpolatedVelocityField
      // object to speed up FindCell calls
      func->SetLastCellId(sPtr->cellId);

      // Set the last vector set in order to maintain consistency
      // during interpolation of tensor field
      func->SetLastVectors(sPtr->v, iv, ix, iy);
      //printf("last vectors(%d)=%lf,%lf,%lf\n",thread_id,sPtr->v[0][iv],sPtr->v[1][iv],sPtr->v[2][iv]);

      dir = streamer->Direction;

      // Copy the first point
      pt1 = *sPtr;
      pt2 = *sPtr;
      //tOffset = pt1.t;

      //printf("%x %x\n",sPtr->v[0],pt2.v[0]);

      //integrate until time has been exceeded
      while ( (sPtr->cellId >= 0) && 
              (sPtr->d < maxdist) &&
              (sPtr->fa >= minFA) && (sPtr->fa < maxFA) &&
              (sPtr->md >= minMD) && (sPtr->md < maxMD) &&
              (sPtr->angle <= maxAngle) && (sPtr->torsion <= maxTorsion) )
        {

        if ( counter++ % 1000 == 0 )
          {
          if (!thread_id)
            {
            self->UpdateProgress(
              static_cast<double>(ptId)
              /static_cast<double>(self->GetNumberOfStreamers()));
              //+sPtr->t/maxtime/static_cast<double>(self->GetNumberOfStreamers()));
            }
          if (self->GetAbortExecute())
            {
            break;
            }
          }

        // Set the integration step to be characteristic cell length
        // time IntegrationStepLength
        tensor->GetCell(sPtr->cellId, cell);
        step = dir*self->GetIntegrationStepLength() 
          * sqrt(static_cast<double>(cell->GetLength2()));

        //fprintf(stderr,"*");

        // Calculate the next step using the integrator provided

        // ATTENTION: Fix sPtr->v[iv]
        for(i=0; i<3; i++)
        {
          vec[i] = sPtr->v[i][iv];
        }

        if (integrator->ComputeNextStep(sPtr->x, vec, xNext, 0, step, 0, err)
            != 0)
          {
          break;
          }

        for(i=0; i<3; i++)
        {
          coords[i] = xNext[i];
        }

        // if same point skip
        //if (sPtr->x[0] == xNext[0] && sPtr->x[1] == xNext[1] && sPtr->x[2] == xNext[2])
          //break;

        idxNext = streamer->InsertNextTensorStreamPoint();
        sNext = streamer->GetTensorStreamPoint(idxNext);

        // Interpolate the tensor field at coords
        if ( !func->TensorValues(coords, sNext->w, sNext->v) )
        {
          break;
        }

        //printf("vector(%d)=%lf,%lf,%lf\n",thread_id,sNext->v[0][iv],sNext->v[1][iv],sNext->v[2][iv]);

        for (i=0; i<3; i++)
          sNext->x[i] = xNext[i];
        
        //InterpolateTensor(cell, cellTensors, w, m);
        //vtkMath::Jacobi(m, sNext->w, sNext->v);
        //self->FixVectors(sPtr->v, sNext->v, iv, ix, iy);

        // make them absolute eigenvalues
        for(i=0;i<3;i++) {
          if (std::isfinite(sNext->w[i])) 
          {
            if(sNext->w[i] < 0) sNext->w[i] = -sNext->w[i];
          } else
            sNext->w[i] = 0;
        }

        sNext->cellId = func->GetLastCellId();
        func->GetLastWeights(w);
        //func->GetLastLocalCoordinates(pcoords);
        tensor->GetCell(sNext->cellId, cell);
        
        // select weight
        int wi=0;
        double wmax=0;
        for (i=0;i<cell->GetNumberOfPoints();i++)
            if (w[i] > wmax) {
                wi = i;  wmax = w[i];
            }

        // Interpolate mask
        if (inMask) {
          inMask->GetTuples(cell->PointIds, cellMask);
          sNext->mask = cellMask->GetComponent(wi, 0);
          if (self->GetConstrainPropagationToMask() && sNext->mask == 0)
            break;
        } else sNext->mask = 1;

        // Interpolate scalars
        if (inScalars) {
          inScalars->GetTuples(cell->PointIds, cellScalars);
          for (sNext->s=0.0, i=0; i < cell->GetNumberOfPoints(); i++)
          {
            temp = cellScalars->GetComponent(i,0);
            if (std::isfinite(temp))
              sNext->s += temp * w[i];
          }
        } else sNext->s = 0;

        d = sqrt(static_cast<double>(
                   vtkMath::Distance2BetweenPoints(sPtr->x,sNext->x)));
        sNext->d = sPtr->d + d;

        // Interpolate angle
        double vold[3], vnew[3];
        for (i=0;i<3;i++) {
            vold[i] = sPtr->v[i][iv];
            vnew[i] = sNext->v[i][iv];
        }       
        vtkMath::Normalize(vold);
        vtkMath::Normalize(vnew);
        angle = vtkMath::Dot(vold, vnew);       
        if (angle > 1.0) angle = 1.0; 
        if (angle < -1.0) angle = -1.0;    
        sNext->angle = vtkMath::DegreesFromRadians(acos(angle));   

        // only in case of the first point
        if (sPtr->d == 0)
          sPtr->angle = sNext->angle;
        //sNext->angle = acos(angle) * rad2deg;

        // compute torsion    
        v0[0] = sNext->v[0][iv];
        v0[1] = sNext->v[1][iv];
        v0[2] = sNext->v[2][iv];
        
        v1[0] = sPtr->v[0][self->GetNormalEigenvector()];
        v1[1] = sPtr->v[1][self->GetNormalEigenvector()];
        v1[2] = sPtr->v[2][self->GetNormalEigenvector()];
        
        v2[0] = sPtr->v[0][self->GetBinormalEigenvector()];
        v2[1] = sPtr->v[1][self->GetBinormalEigenvector()];
        v2[2] = sPtr->v[2][self->GetBinormalEigenvector()];
        
        self->ProjectVectorOntoPlane(v1, v0, vec);
        v1[0] = sNext->v[0][self->GetNormalEigenvector()];
        v1[1] = sNext->v[1][self->GetNormalEigenvector()];
        v1[2] = sNext->v[2][self->GetNormalEigenvector()];

        angle = vtkMath::Dot(v1, vec);        
        if (angle > 1.0) angle = 1.0; 
        if (angle < -1.0) angle = -1.0;   
        sNext->torsion = vtkMath::DegreesFromRadians(acos(angle));   

        // only in case of the first point
        if (sPtr->d == 0)
          sPtr->torsion = sNext->torsion; 

        // calculate MD and FA for new point
        sNext->md = self->CalculateMD(sNext->w);
        sNext->fa = self->CalculateFA(sNext->w);
        //if (std::isnan(sNext->fa))
        //  sNext->fa = 0.0;
        
        //printf("dist=%f, mask=%f, fa=%f, md=%f, angle=%f, torsion=%f\n",
          //sNext->d,sNext->mask,sNext->fa,sNext->md,sNext->angle,sNext->torsion);  
        sPtr = sNext;

      } 
      
      // Clear the last cell to avoid starting a search from
      // the last point in the streamline
      func->ClearLastCellId();

      }
    }

  integrator->Delete();
  func->Delete();

  cell->Delete();
  cellTensors->Delete();
  if (cellMask) {
    cellMask->Delete();
  }
  if (cellScalars) {
    cellScalars->Delete();
  }
  delete[] w;

  //printf("my job %d is done!\n",thread_id);

  return VTK_THREAD_RETURN_VALUE;
}

inline double vtkmpjTensorStreamer::CalculateMD(double w[3])
{
  return (w[0] + w[1] + w[2]) / 3.0;
}

inline double vtkmpjTensorStreamer::CalculateFA(double w[3])
{
  double w0d = (w[0] - w[1]) * (w[0] - w[1]);
  double w1d = (w[1] - w[2]) * (w[1] - w[2]);
  double w2d = (w[0] - w[2]) * (w[0] - w[2]);
  double w0 = w[0] * w[0];
  double w1 = w[1] * w[1];
  double w2 = w[2] * w[2];
  return sqrt(1.0/2.0) * (sqrt(w0d + w1d + w2d) / sqrt(w0 + w1 + w2));
}

void vtkmpjTensorStreamer::Integrate(vtkDataSet *tensor, vtkDataSet *source, vtkDataSet *mask, vtkDataSet *scalars)
{
  vtkPointData *pdt = tensor->GetPointData();

  vtkDataArray *inScalars=0;
  vtkDataArray *inTensors=0;
  vtkDataArray *inMask=0;

  vtkIdType numSourcePts, idx, idxNext;
  vtkmpjTensorStreamer::TensorStreamPoint *sNext, *sPtr, *sLast;
  vtkIdType ptId, i;
  int j, offset;
  vtkCell *cell;
  double xNext[3];
  double tol2;
  double *w = new double[tensor->GetMaxCellSize()];

  double *m[3], *v[3];//
  double m0[3], m1[3], m2[3];
  double v0[3], v1[3], v2[3], temp;//, vec[3];

 // set up working matrices
  v[0] = v0; v[1] = v1; v[2] = v2; 
  m[0] = m0; m[1] = m1; m[2] = m2; 

  vtkDebugMacro(<<"Generating streamers");
  this->NumberOfStreamers = 0;

  //fprintf(stderr,"here1\n");

  // reexecuting - delete old stuff
  if( this->Streamers ) {
    delete [] this->Streamers;
  }
  this->Streamers = NULL;

  inTensors = pdt->GetScalars();
  if (!inTensors) {
     delete [] w;
     vtkErrorMacro(<<"No tensor data defined!");
     return;
  }

  vtkDoubleArray *cellTensors;
  int numcomp = inTensors->GetNumberOfComponents();
  cellTensors = vtkDoubleArray::New();
  cellTensors->SetNumberOfComponents(numcomp);
  cellTensors->Allocate(numcomp*VTK_CELL_SIZE);

  vtkDataArray *cellScalars=0;

  vtkDataArray *cellMask = NULL;
  if (this->GetMask() != NULL) {
    inMask = mask->GetPointData()->GetScalars();
    cellMask = vtkDataArray::CreateDataArray(inMask->GetDataType());
    cellMask->SetNumberOfComponents(1);
    cellMask->SetNumberOfTuples(VTK_CELL_SIZE);
  }

  if (this->GetScalars() != NULL) {
    inScalars = scalars->GetPointData()->GetScalars();
    cellScalars = inScalars->NewInstance();
    cellScalars->SetNumberOfComponents(inScalars->GetNumberOfComponents());
    cellScalars->Allocate(cellScalars->GetNumberOfComponents()*VTK_CELL_SIZE);
  }
  
  tol2 = tensor->GetLength()/1000.0; 
  tol2 = tol2*tol2;

  int iv = this->IntegrationEigenvector;
  int ix = this->NormalEigenvector;
  int iy = this->BinormalEigenvector;

  vtkDebugMacro(<<"Primary: " << iv << ", Normal: " << ix << ", Binormal: " << iy);

  //
  // Create starting points
  //
  this->NumberOfStreamers = numSourcePts = offset = 1;
  if ( source )
    {
    this->NumberOfStreamers = numSourcePts = source->GetNumberOfPoints();
    }
 
  if ( this->IntegrationDirection == VTK_MPJ_INTEGRATE_BOTH_DIRECTIONS )
    {
    offset = 2;
    this->NumberOfStreamers *= 2;
    }

  this->Streamers = new vtkmpjTensorStreamer::TensorStreamArray[this->NumberOfStreamers];

  if ( this->StartFrom == VTK_MPJ_START_FROM_POSITION && !source )
    {
    idx = this->Streamers[0].InsertNextTensorStreamPoint();
    sPtr = this->Streamers[0].GetTensorStreamPoint(idx);
    sPtr->subId = 0;
    for (i=0; i<3; i++)
      {
      sPtr->x[i] = this->StartPosition[i];
      }
    sPtr->cellId = tensor->FindCell(this->StartPosition, NULL, -1, 0.0, 
                                   sPtr->subId, sPtr->p, w);
    }

  else if ( this->StartFrom == VTK_MPJ_START_FROM_LOCATION && !source )
    {
    idx = this->Streamers[0].InsertNextTensorStreamPoint();
    sPtr = this->Streamers[0].GetTensorStreamPoint(idx);
    sPtr->subId = 0;
    cell =  tensor->GetCell(sPtr->cellId);
    cell->EvaluateLocation(sPtr->subId, sPtr->p, sPtr->x, w);
    }

  else //VTK_MPJ_START_FROM_SOURCE
    {
    for (ptId=0; ptId < numSourcePts; ptId++)
      {
      idx = this->Streamers[offset*ptId].InsertNextTensorStreamPoint();
      sPtr = this->Streamers[offset*ptId].GetTensorStreamPoint(idx);
      sPtr->subId = 0;
      source->GetPoint(ptId,sPtr->x);
      sPtr->cellId = tensor->FindCell(sPtr->x, NULL, -1, tol2,
                                     sPtr->subId, sPtr->p, w);
      }
    }

  //fprintf(stderr,"here2\n");

  // Finish initializing each streamer
  //
  for (idx=0, ptId=0; ptId < numSourcePts; ptId++)
    {
    this->Streamers[offset*ptId].Direction = 1.0;
    sPtr = this->Streamers[offset*ptId].GetTensorStreamPoint(idx);
    sPtr->d = 0.0;
    sPtr->s = 0.0;
    sPtr->angle = 0.0;
    sPtr->torsion = 0.0;
    
    if ( sPtr->cellId >= 0 ) //starting point in dataset
      {
      cell = tensor->GetCell(sPtr->cellId);
      cell->EvaluateLocation(sPtr->subId, sPtr->p, xNext, w);

      inTensors->GetTuples(cell->PointIds, cellTensors);

      this->InterpolateTensor(cell, cellTensors, w, m);   

      vtkMath::Jacobi(m, sPtr->w, sPtr->v);
      if ( this->FixEigenvectors )
        this->FixVectors(ptId == 0 ? NULL : sLast->v, sPtr->v, iv, ix, iy);

      //printf("tangent=%lf,%lf,%lf\n",sPtr->v[0][0],sPtr->v[1][0],sPtr->v[2][0]);

      // make them absolute eigenvalues
      for(i=0;i<3;i++) {
        if (std::isfinite(sPtr->w[i]))
        {
          if(sPtr->w[i] < 0) sPtr->w[i] = -sPtr->w[i];
        } else
          sPtr->w[i] = 0;
      }

      sPtr->md = this->CalculateMD(sPtr->w);
      sPtr->fa = this->CalculateFA(sPtr->w);
      //if (!std::isfinite(sPtr->fa))
      //  sPtr->fa = 0.0;
        
      //fprintf(stderr,"fa=%lf, md=%lf\n",sPtr->fa,sPtr->md);

      if (inScalars) {
        inScalars->GetTuples(cell->PointIds, cellScalars);
        for (sPtr->s=0, i=0; i < cell->GetNumberOfPoints(); i++)
        {
          temp = cellScalars->GetComponent(i,0);
          if (std::isfinite(temp))
            sPtr->s += temp * w[i];
        }
      } else sPtr->s = 0;

        // select weight
        int wi=0;
        float wmax=0;
        for (i=0;i<cell->GetNumberOfPoints();i++)
          if (w[i] > wmax) {
            wi = i;  wmax = w[i];
          }

        // compute mask value
        if (inMask) {     
          inMask->GetTuples(cell->PointIds, cellMask);
          sPtr->mask = cellMask->GetComponent(wi, 0);
        } else sPtr->mask = 1;

      }
    else
      {
      for (j=0; j<3; j++)
        {
        sPtr->p[j] = 0.0;
        for (i = 0; i<3; i++)
            sPtr->v[j][i] = 0.0;
        }
      }

    sLast = sPtr;

    if ( this->IntegrationDirection == VTK_MPJ_INTEGRATE_BOTH_DIRECTIONS )
      {
      this->Streamers[offset*ptId+1].Direction = -1.0;
      idxNext = this->Streamers[offset*ptId+1].InsertNextTensorStreamPoint();
      sNext = this->Streamers[offset*ptId+1].GetTensorStreamPoint(idxNext);
      sPtr = this->Streamers[offset*ptId].GetTensorStreamPoint(idx);
      *sNext = *sPtr;
      }
    else if ( this->IntegrationDirection == VTK_MPJ_INTEGRATE_BACKWARD )
      {
      this->Streamers[offset*ptId].Direction = -1.0;
      }


    } //for each streamer

  //fprintf(stderr,"here4, number of threads=%d\n",this->NumberOfThreads);

  // Some data access methods must be called once from a single thread before they
  // can safely be used. Call those now
  vtkGenericCell *gcell = vtkGenericCell::New();
  tensor->GetCell(0,gcell);
  gcell->Delete();
  
  // Set up and execute the thread
  this->Threader->SetNumberOfThreads( this->NumberOfThreads );
  vtkmpjTensorStreamerThreadStruct str;
  str.Filter = this;
  str.Tensor = tensor;
  str.Source = source;
  str.Scalars = scalars;
  str.Mask = mask;
  this->Threader->SetSingleMethod( vtkmpjTensorStreamer::ThreadedIntegrate, &str );
  this->Threader->SingleMethodExecute();

 //fprintf(stderr,"here5, number of threads=%d\n",this->NumberOfThreads);

  delete [] w;
  cellTensors->Delete();
  if (cellScalars)
    {
      cellScalars->Delete();
    }
  if (cellMask)
    {
      cellMask->Delete();
    } 

}

int vtkmpjTensorStreamer::FillInputPortInformation(int port, vtkInformation *info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");

  if (port >= 1) {
    info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), port);
  }
  
  return 1;
}

// Make sure coordinate systems are consistent
void vtkmpjTensorStreamer::FixVectors(double **prev, double **current, int iv, int ix, int iy)
{
  double p0[3], p1[3], p2[3];
  double v0[3], v1[3], v2[3];
  double temp[3];
  int i;

  int flip[3];
  flip[0] = this->FlipX ? -1 : 1;
  flip[1] = this->FlipY ? -1 : 1;
  flip[2] = this->FlipZ ? -1 : 1;

  for (i=0; i<3; i++) 
  {
    v0[i] = current[i][iv] * flip[i]; 
    v1[i] = current[i][ix] * flip[i];
    v2[i] = current[i][iy] * flip[i];
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

void vtkmpjTensorStreamer::ProjectVectorOntoPlane(double v[3], double n[3], double r[3])
{
  double d = vtkMath::Dot(v, n);
  r[0] = v[0] - n[0] * d; 
  r[1] = v[1] - n[1] * d; // r = v - (v . n) n
  r[2] = v[2] - n[2] * d;
}

void vtkmpjTensorStreamer::InterpolateTensor(vtkCell *cell, vtkDataArray *cellTensors, double *w, double *m[3])
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

void vtkmpjTensorStreamer::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  if ( this->StartFrom == VTK_MPJ_START_FROM_POSITION && !this->GetSource())
    {
    os << indent << "Starting Position: (" << this->StartPosition[0] << ","
       << this->StartPosition[1] << ", " << this->StartPosition[2] << ")\n";
    }
  else if ( this->StartFrom == VTK_MPJ_START_FROM_LOCATION && !this->GetSource())
    {
    os << indent << "Starting Location:\n\tCell: " << this->StartCell 
       << "\n\tSubId: " << this->StartSubId << "\n\tP.Coordinates: ("
       << this->StartPCoords[0] << ", " 
       << this->StartPCoords[1] << ", " 
       << this->StartPCoords[2] << ")\n";
    }
  else
    {
    os << indent << "Starting Source: "
       << static_cast<void *>(this->GetSource()) << "\n";
    }

 // os << indent << "Maximum Propagation Time: " 
 //    << this->MaximumPropagationTime << "\n";

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

  //os << indent << "Interval with which points are stored:" 
    // << this->SavePointInterval << endl;

  os << indent << "Integrator: " << this->Integrator << endl;

  os << indent << "Number Of Streamers: " << this->NumberOfStreamers << "\n";
  os << indent << "Number Of Threads: " << this->NumberOfThreads << "\n";
  os << indent << "Epsilon: " << this->Epsilon << "\n";
}
