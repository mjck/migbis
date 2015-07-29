#include "vtkmpjThresholdPolyDataByDistance.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyLine.h"

vtkStandardNewMacro(vtkmpjThresholdPolyDataByDistance);

// Construct object 
vtkmpjThresholdPolyDataByDistance::vtkmpjThresholdPolyDataByDistance()
{
  this->Origin[0] = 0;
  this->Origin[1] = 0;
  this->Origin[2] = 0;
  this->MinimumDistance = 1.0;
}

void vtkmpjThresholdPolyDataByDistance::Execute()
{
  vtkPolyData *input = this->GetInput();
  vtkPolyData *output = this->GetOutput();
  vtkPointData *pd=input->GetPointData();
  vtkCellData *cd=input->GetCellData();

  vtkPointData *outPD=output->GetPointData();
  vtkCellData *outCD=output->GetCellData();
  vtkCellArray *inLines = NULL;

  vtkPoints *inPts;
  vtkIdType numPts = 0;
  vtkIdType numLines;
    
  vtkIdType npts=0, *pts=NULL;
  int id, abort=0;
  vtkIdType inCellId;
  double *x, dist;
  
  vtkDataArray *Scalars = pd->GetScalars();
  vtkDataArray *cellScalars = cd->GetScalars();

  // Check input and initialize
  //  
  if ( !(inPts=input->GetPoints()) || 
       (numPts = inPts->GetNumberOfPoints()) < 1 ||
       !(inLines = input->GetLines()) || 
       (numLines = inLines->GetNumberOfCells()) < 1 )
    {
      return;
    }
  
  // Create output data
  vtkPoints *newPts = vtkPoints::New();
  vtkCellArray *newCells = vtkCellArray::New();
  vtkFloatArray *newScalars = vtkFloatArray::New();
  vtkFloatArray *newCellScalars = vtkFloatArray::New();

  for (inCellId=0, inLines->InitTraversal(); 
       inLines->GetNextCell(npts,pts) && !abort; inCellId++)
    {
      this->UpdateProgress((float)inCellId/numLines);
      abort = this->GetAbortExecute();
      
      if (npts < 2)
	{
	  vtkWarningMacro(<< "Less than two points in line!");
	  continue; //skip filtering this line
	}

     float mindist = VTK_LARGE_FLOAT;
      
      // traverse polyline
      for(int i = 0; i <npts; i++) {

	x = inPts->GetPoint(pts[i]);

	/* compute distance */
	dist = vtkMath::Distance2BetweenPoints(x,this->Origin);
	if (dist < mindist) {
	  mindist = dist;
	}
	
      }

      //      printf("mindist=%f\n",mindist);

      // if dist between threshold, it's OK to copy
      if (mindist <= this->MinimumDistance) {
	
	// allocate next cell
	newCells->InsertNextCell(npts);
	newCellScalars->InsertNextTuple(cellScalars->GetTuple(inCellId));
        	
	// create new polyline
	for(int i=0; i<npts; i++) {

	  id = newPts->InsertNextPoint(inPts->GetPoint(pts[i]));
	  
	  // insert next point id
	  newCells->InsertCellPoint(id);

	  // insert point scalars
	  newScalars->InsertNextTuple(Scalars->GetTuple(pts[i]));
	}
	
      } 

    }

  output->GetPointData()->SetScalars(newScalars);
  newScalars->Delete();

  output->GetCellData()->SetScalars(newCellScalars);
  newCellScalars->Delete();

  output->SetPoints(newPts);
  newPts->Delete();

  output->SetLines(newCells);
  newCells->Delete();

  output->Squeeze();

}

