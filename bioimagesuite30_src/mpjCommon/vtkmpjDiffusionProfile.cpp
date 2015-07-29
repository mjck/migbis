

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


#include "vtkmpjDiffusionProfile.h"
#include "vtkPointSet.h"
#include "vtkUnsignedCharArray.h"
#include "vtkDoubleArray.h"
#include "vtkUnstructuredGrid.h"

vtkStandardNewMacro(vtkmpjDiffusionProfile);

// Construct object with scaling on and scale factor 1.0. 
vtkmpjDiffusionProfile::vtkmpjDiffusionProfile()
{
  this->ScaleFactor = 1.0;
  this->ColorMode = COLOR_BY_SCALARS;
  this->ShapeMode = SHAPE_SPHERE;
  this->ClampScaling = 0;
  this->MaxScaleFactor = 100;
  this->ArrayName = NULL;
}

vtkmpjDiffusionProfile::~vtkmpjDiffusionProfile()
{
  if (this->ArrayName)
    delete []ArrayName;
  
  this->SetSource(NULL);
}

void vtkmpjDiffusionProfile::Execute()
{
  vtkDataSet *input = this->GetInput();
  vtkPolyData *output = this->GetOutput();
  
  vtkPointData *pd = input->GetPointData();
  vtkPointData *outPD = output->GetPointData();
  vtkDataArray *inScalars = pd->GetScalars();
  vtkDataArray *inArray = inScalars;

  vtkIdType ptIncr, subIncr, cellId;
  vtkCellArray *sourceCells, *cells;
  vtkCell *cell;
  vtkIdList *cellPts;
  vtkIdType npts;

  // make sure source geometry has been specified.
  if (this->GetSource() == NULL)
    {
      vtkErrorMacro("No source.");
      return;
    }

  //printf("input=%d, pd=%d, inscalars=%d, inAdcs=%d\n",input,pd,inScalars,inADCs);
  if (this->ArrayName != NULL) {
    inArray = pd->GetArray(this->ArrayName);
  }

  vtkIdType numPts = input->GetNumberOfPoints();  
  vtkIdType numComp = inArray->GetNumberOfComponents();
  
  vtkPoints *newPts = vtkPoints::New();  
  vtkCellArray *newCells = vtkCellArray::New();
  
  // get source geometry information
  vtkPoints *sourcePts = this->GetSource()->GetPoints();
  vtkIdType numSourcePts = sourcePts->GetNumberOfPoints();
  vtkIdType numSourceCells = this->GetSource()->GetNumberOfCells();
  
  newPts = vtkPoints::New();
  newPts->Allocate(numPts*numSourcePts);

  // Setting up for calls to PolyData::InsertNextCell()
  if ( (sourceCells=this->GetSource()->GetPolys())->GetNumberOfCells() > 0 )
    {
      cells = vtkCellArray::New();
      cells->Allocate(numPts*sourceCells->GetSize());
      output->SetPolys(cells);
      cells->Delete();
    }
  
  //newScalars = vtkFloatArray::New();
  //newScalars = vtkUnsignedCharArray::New();

  printf("ColorByOrientation=%d\n",this->ColorMode);
  
  vtkDataArray *newScalars;

  if (this->ColorMode == COLOR_BY_ORIENTATION) {
    newScalars = vtkUnsignedCharArray::New();
    newScalars->SetNumberOfComponents(3);
  } else {
    newScalars = vtkDoubleArray::New();
    newScalars->SetNumberOfComponents(1);
  }
  
  printf("numPts=%d,%d\n",numPts,numSourcePts);
  printf("result from alloc=%d\n",newScalars->Allocate(numPts*numSourcePts));
  
  double max_val = -VTK_LARGE_FLOAT;
  double min_val = VTK_LARGE_FLOAT;
  double *norm_val = new double[numComp];
  int numNewPts=0;
  int index=0;
  
  double *scalar=NULL;
  double *values=NULL;

  vtkIdType *pts;
  pts = new vtkIdType[this->GetSource()->GetMaxCellSize()];
  
  printf("traverse points\n");

  // Traverse all points and copy topology
  for (vtkIdType inPtId=0; inPtId < numPts; inPtId++)    
    {	  
      ptIncr = inPtId * numSourcePts;
      
      for (cellId=0; cellId < numSourceCells; cellId++)
	{
	  cell = this->GetSource()->GetCell(cellId);
	  
	  cellPts = cell->GetPointIds();
	  npts = cellPts->GetNumberOfIds();
	  
	  // This variable may be removed, but that
	  // will not improve readability
	  subIncr = ptIncr + numSourcePts;
	  
	  for (int i=0; i < npts; i++)
	    {
	      pts[i] = cellPts->GetId(i) + subIncr;
	    }
	  
	  output->InsertNextCell(cell->GetCellType(),npts,pts);
	}
    }

  printf("creating scalars\n");
  
  for (vtkIdType inPtId=0; inPtId < numPts; inPtId++)
    {
      ptIncr = inPtId * numSourcePts;
      
      // populate vector
      scalar = inScalars->GetTuple(inPtId);       
      values = inArray->GetTuple(inPtId); 
      
      // determine minimum/maximum value from profile
      max_val = -VTK_LARGE_FLOAT;
      min_val =  VTK_LARGE_FLOAT;
      for (int i=0; i<numComp; i++) {
	norm_val[i] = values[i];
	if (values[i] > max_val) {
	  max_val = values[i];
	}	
	if (values[i] < min_val) {
	  min_val = values[i];
	}	
      }
      
      // normalize numbers 
      for (int i=0; i<numComp; i++) {
	norm_val[i] = (norm_val[i] - min_val) / (max_val - min_val);
      }       
      
      // translate Source to Input point
      double *x = input->GetPoint(inPtId);

      index = numNewPts;
      
      double np[3];
      
      // for each of the 32 directions insert position
      for (int i=0; i<numComp; i++) {
	
	double *p = sourcePts->GetPoint(i);

	if (this->ShapeMode == SHAPE_DEFORMATION) {
	  np[0] = p[0] * norm_val[i]*this->ScaleFactor + x[0];
	  np[1] = p[1] * norm_val[i]*this->ScaleFactor + x[1];	  
	  np[2] = p[2] * norm_val[i]*this->ScaleFactor + x[2];
	} else {
	  np[0] = p[0] * this->ScaleFactor + x[0];
	  np[1] = p[1] * this->ScaleFactor + x[1];	  
	  np[2] = p[2] * this->ScaleFactor + x[2];
	}
	
	// insert point
	newPts->InsertNextPoint(np);

	// insert scalar
	if (this->ColorMode == COLOR_BY_DIFFUSIVITIES) {
	  newScalars->InsertNextTuple(&norm_val[i]); 	
	} else if (this->ColorMode == COLOR_BY_ORIENTATION) {
	  // TO DO:
	  float rgb[3]={fabs(p[0])*255, fabs(p[1])*255, fabs(p[2])*255};
	  newScalars->InsertNextTuple(rgb);
	} else {
	  newScalars->InsertNextTuple(&scalar[0]);
	} 

	//printf("numNewScalars=%d,numNewPts=%d\n",numPts*numSourcePts,numNewPts);

	numNewPts++;
	
      }

    }
  
  printf ("out of loop\n");

  delete [] norm_val;
  delete [] pts;

  output->SetPoints(newPts);
  newPts->Delete();

  printf("before setting scalars\n");

  outPD->SetScalars(newScalars);
  newScalars->Delete();

  printf("before setting point\n");

 

  printf("before the end\n");
  
}

void vtkmpjDiffusionProfile::SetSource(vtkPolyData *source)
{
  this->vtkProcessObject::SetNthInput(1, source);
}
 
vtkPolyData *vtkmpjDiffusionProfile::GetSource()
{
  if (this->NumberOfInputs < 2)
    {
    return NULL;
    }
  return (vtkPolyData *)(this->Inputs[1]);
   
}

void vtkmpjDiffusionProfile::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  
  os << indent << "Scale Factor: " << this->ScaleFactor << "\n";
  os << indent << "Color Mode: " << this->ColorMode << endl;
  os << indent << "Clamp Scaling: " << (this->ClampScaling ? "On\n" : "Off\n");
  os << indent << "Max Scale Factor: " << this->MaxScaleFactor << "\n";
}
