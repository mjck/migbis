

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


#include "vtkmpjStreamer.h"

// #include "vtkCell.h"
// #include "vtkDataSet.h"
// #include "vtkFloatArray.h"
// #include "vtkGenericCell.h"
// #include "vtkInterpolatedVelocityField.h"
// #include "vtkMath.h"
// #include "vtkMultiThreader.h"
// #include "vtkObjectFactory.h"
// #include "vtkPointData.h"
// #include "vtkRungeKutta2.h"

//vtkCxxRevisionMacro(vtkmpjStreamer, "$Revision: 1.83 $");
vtkCxxSetObjectMacro(vtkmpjStreamer,Integrator,vtkInitialValueProblemSolver);
//vtkStandardNewMacro(vtkmpjStreamer);

#define VTK_START_FROM_POSITION 0
#define VTK_START_FROM_LOCATION 1

vtkmpjStreamer::mpjStreamArray::mpjStreamArray()
{
  this->MaxId = -1; 
  this->Array = new vtkmpjStreamer::mpjStreamPoint[1000];
  this->Size = 1000;
  this->Extend = 5000;
  this->Direction = VTK_INTEGRATE_FORWARD;
}

vtkmpjStreamer::mpjStreamPoint *vtkmpjStreamer::mpjStreamArray::Resize(vtkIdType sz)
{
  vtkmpjStreamer::mpjStreamPoint *newArray;
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

  newArray = new vtkmpjStreamer::mpjStreamPoint[newSize];

  memcpy(newArray, this->Array,
         (sz < this->Size ? sz : this->Size) * sizeof(vtkmpjStreamer::mpjStreamPoint));

  this->Size = newSize;
  delete [] this->Array;
  this->Array = newArray;

  return this->Array;
}

// Construct object to start from position (0,0,0); integrate forward; terminal
// speed 0.0; vorticity computation off; integrations step length 0.2; and
// maximum propagation time 100.0.
vtkmpjStreamer::vtkmpjStreamer()
{
  this->StartFrom = VTK_START_FROM_POSITION;

  this->StartCell = 0;
  this->StartSubId = 0;
  this->StartPCoords[0] = this->StartPCoords[1] = this->StartPCoords[2] = 0.5;
  this->StartPosition[0] = this->StartPosition[1] = this->StartPosition[2] = 0.0;
  this->Streamers = NULL;
  this->MaximumPropagationTime = 100.0;
  this->IntegrationDirection = VTK_INTEGRATE_FORWARD;
  this->IntegrationStepLength = 0.2;
  this->Vorticity = 0;
  this->TerminalSpeed = 0.0;
  this->SpeedScalars = 0;
  this->OrientationScalars = 0;
  this->NumberOfStreamers = 0;
  this->Threader = vtkMultiThreader::New();
  this->NumberOfThreads = this->Threader->GetNumberOfThreads();
  this->Integrator = vtkRungeKutta2::New();
  this->SavePointInterval = 0.00001;
}

vtkmpjStreamer::~vtkmpjStreamer()
{
  delete [] this->Streamers;

  //  fprintf(stderr,"This far \n");
  //  this->SetSource(0);
  //  fprintf(stderr,"This far 2\n");
  if (this->Threader)
    {
      //  fprintf(stderr,"This far 3\n");
      this->Threader->Delete();
    }
  //  fprintf(stderr,"This far 4\n");
  this->SetIntegrator(0);
}

void vtkmpjStreamer::SetSource(vtkDataSet *source)
{
  this->vtkProcessObject::SetNthInput(1, source);
}

vtkDataSet *vtkmpjStreamer::GetSource()
{
  if (this->NumberOfInputs < 2)
    {
    return NULL;
    }
  return (vtkDataSet *)(this->Inputs[1]);
}

// Specify the start of the streamline in the cell coordinate system. That is,
// cellId and subId (if composite cell), and parametric coordinates.
void vtkmpjStreamer::SetStartLocation(vtkIdType cellId, int subId,
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

// Specify the start of the streamline in the cell coordinate system. That is,
// cellId and subId (if composite cell), and parametric coordinates.
void vtkmpjStreamer::SetStartLocation(vtkIdType cellId, int subId, float r,
                                   float s, float t)
{
  float pcoords[3];
  pcoords[0] = r;
  pcoords[1] = s;
  pcoords[2] = t;

  this->SetStartLocation(cellId, subId, pcoords);
}

// Get the starting location of the streamline in the cell coordinate system.
vtkIdType vtkmpjStreamer::GetStartLocation(int& subId, float pcoords[3])
{
  subId = this->StartSubId;
  pcoords[0] = this->StartPCoords[0];
  pcoords[1] = this->StartPCoords[1];
  pcoords[2] = this->StartPCoords[2];
  return this->StartCell;
}

// Specify the start of the streamline in the global coordinate system. Search
// must be performed to find initial cell to start integration from.
void vtkmpjStreamer::SetStartPosition(double x[3])
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

// Specify the start of the streamline in the global coordinate system. Search
// must be performed to find initial cell to start integration from.
void vtkmpjStreamer::SetStartPosition(double x, double y, double z)
{
  double pos[3];
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;

  this->SetStartPosition(pos);
}

// Get the start position in global x-y-z coordinates.
double *vtkmpjStreamer::GetStartPosition()
{
  return this->StartPosition;
}

static const float VTK_EPSILON=1E-12;

VTK_THREAD_RETURN_TYPE vtkmpjStreamer::ThreadedIntegrate( void *arg )
{
  vtkmpjStreamer              *self;
  int                      thread_count;
  int                      thread_id;
  vtkmpjStreamer::mpjStreamArray           *streamer;
  vtkmpjStreamer::mpjStreamPoint *sNext = 0, *sPtr;
  vtkmpjStreamer::mpjStreamPoint pt1, pt2;
  int                      i;
  vtkIdType                idxNext, ptId;
  double                    d, step, dir;
  double                    xNext[3], vel[3];
  double *cellVel;
  double derivs[9];
  double                    *w, pcoords[3];
  double                    coords[4];
  vtkDataSet               *input;
  vtkGenericCell           *cell;
  vtkPointData             *pd;
  vtkDataArray             *inScalars;
  vtkDataArray             *inVectors;
  vtkDoubleArray            *cellVectors;
  vtkDataArray             *cellScalars=0;
  double tOffset, vort[3];
  double err;
  int nSavePts = 0, counter=0;

  thread_id = ((vtkMultiThreader::ThreadInfo *)(arg))->ThreadID;
  thread_count = ((vtkMultiThreader::ThreadInfo *)(arg))->NumberOfThreads;
  self = (vtkmpjStreamer *)(((vtkMultiThreader::ThreadInfo *)(arg))->UserData);

  input     = self->GetInput();
  pd        = input->GetPointData();
  inScalars = pd->GetScalars();
  inVectors = pd->GetVectors();

  cell = vtkGenericCell::New();
  cellVectors = vtkDoubleArray::New();
  cellVectors->SetNumberOfComponents(3);
  cellVectors->Allocate(3*VTK_CELL_SIZE);
  if (inScalars)
    {
    cellScalars = inScalars->NewInstance();
    cellScalars->SetNumberOfComponents(inScalars->GetNumberOfComponents());
    cellScalars->Allocate(inScalars->GetNumberOfComponents()*VTK_CELL_SIZE);
    }

  w = new double[input->GetMaxCellSize()];

  // Set the function set to be integrated
  vtkInterpolatedVelocityField* func = vtkInterpolatedVelocityField::New();
  func->AddDataSet(input);

  if (self->GetIntegrator() == 0)
    {
    vtkGenericWarningMacro("No integrator is specified.");
    return VTK_THREAD_RETURN_VALUE;
    }

  // Create a new integrator, the type is the same as Integrator
  vtkInitialValueProblemSolver* integrator = 
    self->GetIntegrator()->NewInstance();
  integrator->SetFunctionSet(func);

  // Used to avoid calling these function many times during
  // the integration
  float termspeed = self->GetTerminalSpeed();
  float maxtime = self->GetMaximumPropagationTime();
  float savePointInterval = self->GetSavePointInterval();

  // For each streamer, integrate in appropriate direction
  // Do only the streamers that this thread should handle.
  for (ptId=0; ptId < self->GetNumberOfStreamers(); ptId++)
    {
    if ( ptId % thread_count == thread_id )
      {
      // Get starting step
      streamer = self->GetStreamers() + ptId;
      sPtr = streamer->GetmpjStreamPoint(0);
      if ( sPtr->cellId < 0 )
        {
        continue;
        }
      // Set the last cell id in the vtkInterpolatedVelocityField
      // object to speed up FindCell calls
      func->SetLastCellId(sPtr->cellId);

      dir = streamer->Direction;

      // Copy the first point
      pt1 = *sPtr;
      pt2 = *sPtr;
      tOffset = pt1.t;

      //integrate until time has been exceeded
      while ( pt1.cellId >= 0 && pt1.speed > termspeed && pt1.t <  maxtime )
        {
	  
	  //printf("pt1.cellId=%d, pt1.speed=%f, pt1.time=%f\n",pt1.cellId,pt1.speed,pt1.t);
	  
	  if ( counter++ % 1000 == 0 )
	    {
	      if (!thread_id)
		{
		  self->UpdateProgress((float)ptId/self->GetNumberOfStreamers()
				       +pt1.t/maxtime/self->GetNumberOfStreamers());
		}
	      if (self->GetAbortExecute())
		{
		  break;
		}
	    }
	  
	  // Set the integration step to be characteristic cell length
	  // time IntegrationStepLength
	  input->GetCell(pt1.cellId, cell);
	  step = dir*self->GetIntegrationStepLength() * sqrt((double)cell->GetLength2());///pt1.speed;

        // Calculate the next step using the integrator provided
        if (integrator->ComputeNextStep(pt1.x, pt1.v, xNext, 0, step, 0, err)
            != 0)
          {
	    //printf("out of integration\n");
	    break;
          }

        for(i=0; i<3; i++)
          {
          coords[i] = xNext[i];
          }

        // Interpolate the velocity field at coords
        if ( !func->FunctionValues(coords, vel) )
          {
	    //printf("out of interpolation with coords=%f,%f,%f,%f\n",coords[0],coords[1],coords[2],coords[3]);
	    break;
          }

        for(i=0; i<3; i++)
          {
          pt2.v[i] = vel[i];
          }

        for (i=0; i<3; i++)
          {
          pt2.x[i] = xNext[i];
          }
        
        pt2.cellId = func->GetLastCellId();
        func->GetLastWeights(w);
        func->GetLastLocalCoordinates(pcoords);
        input->GetCell(pt2.cellId, cell);
        
        if ( inScalars )
          {
          // Interpolate scalars
          inScalars->GetTuples(cell->PointIds, cellScalars);
          for (pt2.s=0.0, i=0; i < cell->GetNumberOfPoints(); i++)
            {
            pt2.s += cellScalars->GetComponent(i,0) * w[i];
            }
          }

        pt2.speed = vtkMath::Norm(pt2.v);
	//printf("pt2.v=%f, %f, %f\n",pt2.v[0],pt2.v[1],pt2.v[2]);
	(void)vtkMath::Normalize(pt2.v);

        d = sqrt((double)vtkMath::Distance2BetweenPoints(pt1.x,pt2.x));
        pt2.d = pt1.d + d;

	//printf("d=%f, total=%f\n",d,pt2.d);
        // If at stagnation region, stop the integration
        if ( d <= 1E-3 || (pt1.speed + pt2.speed) < VTK_EPSILON )
          {
	    //printf("stagnation point: d=%f, pt1.speed=%f, pt2.speed=%f\n",d,pt1.speed,pt2.speed);
	    pt2.t = pt1.t;
	    break;
          }
        pt2.t = pt1.t + (2.0 * d / (pt1.speed + pt2.speed));
	//printf("d=%f, pt1.speed=%f, pt2.speed=%f\n",d,pt1.speed,pt2.speed);
        if (self->GetVorticity() && inVectors)
          {
	    // compute vorticity
          inVectors->GetTuples(cell->PointIds, cellVectors);
          cellVel = cellVectors->GetPointer(0);
          cell->Derivatives(0, pcoords, cellVel, 3, derivs);
          vort[0] = derivs[7] - derivs[5];
          vort[1] = derivs[2] - derivs[6];
          vort[2] = derivs[3] - derivs[1];
          // rotation
          pt2.omega = vtkMath::Dot(vort, pt2.v);
          pt2.omega /= pt2.speed;
          pt2.theta += (pt1.omega+pt2.omega)/2 * (pt2.t - pt1.t);
          }
                
        // Store only points which have a point to be displayed
        // between them
        if (tOffset >= pt1.t && tOffset <= pt2.t)
          {
          // Do not store if same as the last point.
          // To avoid storing some points twice.
          if ( !sNext || sNext->x[0] != pt1.x[0] || sNext->x[1] != pt1.x[1]
               || sNext->x[2] != pt1.x[2] )
            {
            idxNext = streamer->InsertNextmpjStreamPoint();
            sNext = streamer->GetmpjStreamPoint(idxNext);
            *sNext = pt1;
            nSavePts++;
            }
          idxNext = streamer->InsertNextmpjStreamPoint();
          sNext = streamer->GetmpjStreamPoint(idxNext);
          *sNext = pt2;
          nSavePts++;
          }
        if (tOffset < pt2.t)
          {
          tOffset += ((int)(( pt2.t - tOffset) / savePointInterval) + 1)
            * savePointInterval;
          }
        pt1 = pt2;
	//printf("pt1.cellId=%d, pt1.speed=%f, pt1.time=%f\n",pt1.cellId,pt1.speed,pt1.t);
        } 
      //printf("out\n");
      // Store the last point anyway.
      if ( !sNext || sNext->x[0] != pt2.x[0] || sNext->x[1] != pt2.x[1]
           || sNext->x[2] != pt2.x[2] )
        {
        idxNext = streamer->InsertNextmpjStreamPoint();
        sNext = streamer->GetmpjStreamPoint(idxNext);
        *sNext = pt2;
        nSavePts++;
        }
      // Clear the last cell to avoid starting a search from
      // the last point in the streamline
      func->ClearLastCellId();
      }

    }

  integrator->Delete();
  func->Delete();

  cell->Delete();
  cellVectors->Delete();
  if (cellScalars)
    {
    cellScalars->Delete();
    }
  delete[] w;

  return VTK_THREAD_RETURN_VALUE;
}

void vtkmpjStreamer::Integrate()
{
  vtkDataSet *input = this->GetInput();
  vtkDataSet *source = this->GetSource();
  vtkPointData *pd=input->GetPointData();
  vtkDataArray *inScalars;
  vtkDataArray *inVectors;
  vtkIdType numSourcePts, idx, idxNext;
  vtkmpjStreamer::mpjStreamPoint *sNext, *sPtr;
  vtkIdType ptId, i;
  int j, offset;
  vtkCell *cell;
  double *v; 
  double *cellVel;
  double derivs[9], xNext[3], vort[3];
  double tol2;
  double *w=new double[input->GetMaxCellSize()];
  vtkDoubleArray *cellVectors;
  vtkDataArray *cellScalars=0;

  vtkDebugMacro(<<"Generating streamers");
  this->NumberOfStreamers = 0;

// reexecuting - delete old stuff
  delete [] this->Streamers;
  this->Streamers = NULL;

  if ( ! (inVectors=pd->GetVectors()) )
    {
    delete [] w;
    vtkErrorMacro(<<"No vector data defined!");
    return;
    }

  cellVectors = vtkDoubleArray::New();
  cellVectors->SetNumberOfComponents(3);
  cellVectors->Allocate(3*VTK_CELL_SIZE);

  inScalars = pd->GetScalars();

  if (inScalars)
    {
    cellScalars = inScalars->NewInstance();
    cellScalars->SetNumberOfComponents(inScalars->GetNumberOfComponents());
    cellScalars->Allocate(cellScalars->GetNumberOfComponents()*VTK_CELL_SIZE);
    }
  
  tol2 = input->GetLength()/1000; 
  tol2 = tol2*tol2;

  //
  // Create starting points
  //
  this->NumberOfStreamers = numSourcePts = offset = 1;
  if ( this->GetSource() )
    {
    this->NumberOfStreamers = numSourcePts = source->GetNumberOfPoints();
    }
 
  if ( this->IntegrationDirection == VTK_INTEGRATE_BOTH_DIRECTIONS )
    {
    offset = 2;
    this->NumberOfStreamers *= 2;
    }

  this->Streamers = new vtkmpjStreamer::mpjStreamArray[this->NumberOfStreamers];

  if ( this->StartFrom == VTK_START_FROM_POSITION && !this->GetSource() )
    {
    idx = this->Streamers[0].InsertNextmpjStreamPoint();
    sPtr = this->Streamers[0].GetmpjStreamPoint(idx);
    sPtr->subId = 0;
    for (i=0; i<3; i++)
      {
      sPtr->x[i] = this->StartPosition[i];
      }
    sPtr->cellId = input->FindCell(this->StartPosition, NULL, -1, 0.0, 
                                   sPtr->subId, sPtr->p, w);
    }

  else if ( this->StartFrom == VTK_START_FROM_LOCATION && !this->GetSource() )
    {
    idx = this->Streamers[0].InsertNextmpjStreamPoint();
    sPtr = this->Streamers[0].GetmpjStreamPoint(idx);
    sPtr->subId = 0;
    cell =  input->GetCell(sPtr->cellId);
    cell->EvaluateLocation(sPtr->subId, sPtr->p, sPtr->x, w);
    }

  else //VTK_START_FROM_SOURCE
    {
    for (ptId=0; ptId < numSourcePts; ptId++)
      {
      idx = this->Streamers[offset*ptId].InsertNextmpjStreamPoint();
      sPtr = this->Streamers[offset*ptId].GetmpjStreamPoint(idx);
      sPtr->subId = 0;
      source->GetPoint(ptId,sPtr->x);
      sPtr->cellId = input->FindCell(sPtr->x, NULL, -1, tol2,
                                     sPtr->subId, sPtr->p, w);
      }
    }

  // Finish initializing each streamer
  //
  for (idx=0, ptId=0; ptId < numSourcePts; ptId++)
    {
    this->Streamers[offset*ptId].Direction = 1.0;
    sPtr = this->Streamers[offset*ptId].GetmpjStreamPoint(idx);
    sPtr->d = 0.0;
    sPtr->t = 0.0;
    sPtr->s = 0.0;
    sPtr->theta = 0.0;
    sPtr->omega = 0.0;
    
    if ( sPtr->cellId >= 0 ) //starting point in dataset
      {
      cell = input->GetCell(sPtr->cellId);
      cell->EvaluateLocation(sPtr->subId, sPtr->p, xNext, w);

      inVectors->GetTuples(cell->PointIds, cellVectors);
      sPtr->v[0]  = sPtr->v[1] = sPtr->v[2] = 0.0;
      for (i=0; i < cell->GetNumberOfPoints(); i++)
        {
        v =  cellVectors->GetTuple(i);
        for (j=0; j<3; j++)
          {
          sPtr->v[j] += v[j] * w[i];
          }
        }
    
      sPtr->speed = vtkMath::Norm(sPtr->v);
      //printf("sPtr->speed=%f\n",sPtr->speed);

      (void)vtkMath::Normalize(sPtr->v);

      if (this->GetVorticity() && inVectors)
        {
          // compute vorticity
        inVectors->GetTuples(cell->PointIds, cellVectors);
        cellVel = cellVectors->GetPointer(0);
        cell->Derivatives(0, sPtr->p, cellVel, 3, derivs);
        vort[0] = derivs[7] - derivs[5];
        vort[1] = derivs[2] - derivs[6];
        vort[2] = derivs[3] - derivs[1];
        // rotation
        sPtr->omega = vtkMath::Dot(vort, sPtr->v);
        sPtr->omega /= sPtr->speed;
        sPtr->theta = 0;
        }

      if ( inScalars ) 
        {
        inScalars->GetTuples(cell->PointIds, cellScalars);
        for (sPtr->s=0, i=0; i < cell->GetNumberOfPoints(); i++)
          {
          sPtr->s += cellScalars->GetComponent(i,0) * w[i];
          }
        }
      }
    else
      {
      for (j=0; j<3; j++)
        {
        sPtr->p[j] = 0.0;
        sPtr->v[j] = 0.0;
        }
      sPtr->speed = 0;
      }

    if ( this->IntegrationDirection == VTK_INTEGRATE_BOTH_DIRECTIONS )
      {
      this->Streamers[offset*ptId+1].Direction = -1.0;
      idxNext = this->Streamers[offset*ptId+1].InsertNextmpjStreamPoint();
      sNext = this->Streamers[offset*ptId+1].GetmpjStreamPoint(idxNext);
      sPtr = this->Streamers[offset*ptId].GetmpjStreamPoint(idx);
      *sNext = *sPtr;
      }
    else if ( this->IntegrationDirection == VTK_INTEGRATE_BACKWARD )
      {
      this->Streamers[offset*ptId].Direction = -1.0;
      }


    } //for each streamer

  // Some data access methods must be called once from a single thread before they
  // can safely be used. Call those now
  vtkGenericCell *gcell = vtkGenericCell::New();
  input->GetCell(0,gcell);
  gcell->Delete();
  
  // Set up and execute the thread
  this->Threader->SetNumberOfThreads( this->NumberOfThreads );
  this->Threader->SetSingleMethod( vtkmpjStreamer::ThreadedIntegrate, (void *)this );
  this->Threader->SingleMethodExecute();

  //
  // Now create appropriate representation
  //
  if ( this->OrientationScalars && !this->SpeedScalars)
    {
    for (ptId=0; ptId < this->NumberOfStreamers; ptId++)
      {
      for ( sPtr=this->Streamers[ptId].GetmpjStreamPoint(0), i=0; 
            i < this->Streamers[ptId].GetNumberOfPoints() && sPtr->cellId >= 0; 
            i++, sPtr=this->Streamers[ptId].GetmpjStreamPoint(i) )
        {
        sPtr->s = sPtr->theta;
        }
      }
    }

  if ( this->SpeedScalars )
    {
    for (ptId=0; ptId < this->NumberOfStreamers; ptId++)
      {
      for ( sPtr=this->Streamers[ptId].GetmpjStreamPoint(0), i=0; 
            i < this->Streamers[ptId].GetNumberOfPoints() && sPtr->cellId >= 0; 
            i++, sPtr=this->Streamers[ptId].GetmpjStreamPoint(i) )
        {
        sPtr->s = sPtr->speed;
        }
      }
    }
  delete [] w;
  cellVectors->Delete();
  if (cellScalars)
    {
    cellScalars->Delete();
    }
}

void vtkmpjStreamer::ComputeVorticity()
{
}

void vtkmpjStreamer::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  if ( this->StartFrom == VTK_START_FROM_POSITION && !this->GetSource())
    {
    os << indent << "Starting Position: (" << this->StartPosition[0] << ","
       << this->StartPosition[1] << ", " << this->StartPosition[2] << ")\n";
    }
  else if ( this->StartFrom == VTK_START_FROM_LOCATION && !this->GetSource())
    {
    os << indent << "Starting Location:\n\tCell: " << this->StartCell 
       << "\n\tSubId: " << this->StartSubId << "\n\tP.Coordinates: ("
       << this->StartPCoords[0] << ", " 
       << this->StartPCoords[1] << ", " 
       << this->StartPCoords[2] << ")\n";
    }
  else
    {
    os << indent << "Starting Source: " << (void *)this->GetSource() << "\n";
    }

  os << indent << "Maximum Propagation Time: " 
     << this->MaximumPropagationTime << "\n";

  if ( this->IntegrationDirection == VTK_INTEGRATE_FORWARD )
    {
    os << indent << "Integration Direction: FORWARD\n";
    }
  else if ( this->IntegrationDirection == VTK_INTEGRATE_BACKWARD )
    {
    os << indent << "Integration Direction: BACKWARD\n";
    }
  else
    {
    os << indent << "Integration Direction: FORWARD & BACKWARD\n";
    }

  os << indent << "Integration Step Length: " << this->IntegrationStepLength << "\n";

  os << indent << "Vorticity: " << (this->Vorticity ? "On\n" : "Off\n");

  os << indent << "Terminal Speed: " << this->TerminalSpeed << "\n";

  os << indent << "Speed Scalars: " << (this->SpeedScalars ? "On\n" : "Off\n");

  os << indent << "Orientation Scalars: " << (this->OrientationScalars ? "On\n" : "Off\n");

  os << indent << "Interval with which points are stored:" 
     << this->SavePointInterval << endl;

  os << indent << "Integrator: " << this->Integrator << endl;

  os << indent << "Number Of Streamers: " << this->NumberOfStreamers << "\n";
  os << indent << "Number Of Threads: " << this->NumberOfThreads << "\n";
}


