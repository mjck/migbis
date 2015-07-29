#include "vtkmpjThresholdPolyData.h"
 
#include "vtkCell.h"
#include "vtkCellData.h"
#include "vtkIdList.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"

vtkStandardNewMacro(vtkmpjThresholdPolyData);

// Construct with lower threshold=0, upper threshold=1, and threshold 
// function=upper AllScalars=1.
vtkmpjThresholdPolyData::vtkmpjThresholdPolyData()
{
  this->LowerThreshold = 0.0;
  this->UpperThreshold = 1.0;
  this->AllScalars = 1;
  this->AttributeMode = VTK_ATTRIBUTE_MODE_USE_POINT_DATA;
  this->ThresholdFunction = &vtkmpjThresholdPolyData::Upper;
  this->InputScalarsSelection = NULL;
}

vtkmpjThresholdPolyData::~vtkmpjThresholdPolyData()
{
  // This frees the string
  this->SetInputScalarsSelection(NULL);
}

// Criterion is cells whose scalars are less or equal to lower threshold.
void vtkmpjThresholdPolyData::ThresholdByLower(float lower) 
{
  if ( this->LowerThreshold != lower || 
       this->ThresholdFunction != &vtkmpjThresholdPolyData::Lower)
    {
    this->LowerThreshold = lower; 
    this->ThresholdFunction = &vtkmpjThresholdPolyData::Lower;
    this->Modified();
    }
}
                           
// Criterion is cells whose scalars are greater or equal to upper threshold.
void vtkmpjThresholdPolyData::ThresholdByUpper(float upper)
{
  if ( this->UpperThreshold != upper ||
       this->ThresholdFunction != &vtkmpjThresholdPolyData::Upper)
    {
    this->UpperThreshold = upper; 
    this->ThresholdFunction = &vtkmpjThresholdPolyData::Upper;
    this->Modified();
    }
}
                           
// Criterion is cells whose scalars are between lower and upper thresholds.
void vtkmpjThresholdPolyData::ThresholdBetween(float lower, float upper)
{
  if ( this->LowerThreshold != lower || this->UpperThreshold != upper ||
       this->ThresholdFunction != &vtkmpjThresholdPolyData::Between)
    {
    this->LowerThreshold = lower; 
    this->UpperThreshold = upper;
    this->ThresholdFunction = &vtkmpjThresholdPolyData::Between;
    this->Modified();
    }
}
  
void vtkmpjThresholdPolyData::Execute()
{
  vtkIdType cellId, newCellId;
  vtkIdList *cellPts, *pointMap;
  vtkIdList *newCellPts;
  vtkCell *cell;
  vtkPoints *newPoints;
  int i, ptId, newId, numPts;
  int numCellPts;
  double *x;
  vtkDataSet *input = this->GetInput();
  
  if (!input)
    {
    vtkErrorMacro(<<"No input, Can't Execute");
    }
  vtkPolyData *output = this->GetOutput();
  vtkPointData *pd=input->GetPointData(), *outPD=output->GetPointData();
  vtkCellData *cd=input->GetCellData(), *outCD=output->GetCellData();
  vtkDataArray *pointScalars;
  vtkDataArray *cellScalars;
  int keepCell, usePointScalars;

  vtkDebugMacro(<< "Executing threshold filter");
  
  // I am explicitly checking to avoid a warning if the array is not found.
  if (this->InputScalarsSelection)
    {
    pointScalars=pd->GetArray(this->InputScalarsSelection);
    cellScalars=cd->GetArray(this->InputScalarsSelection);
    }
  else
    {
    pointScalars=pd->GetScalars();
    cellScalars=cd->GetScalars();
    }

  outPD->CopyAllocate(pd);
  outCD->CopyAllocate(cd);

  if ( !(pointScalars || cellScalars) )
    {
    vtkErrorMacro(<<"No scalar data to threshold");
    return;
    }

  numPts = input->GetNumberOfPoints();
  output->Allocate(input->GetNumberOfCells());
  newPoints = vtkPoints::New();
  newPoints->Allocate(numPts);

  pointMap = vtkIdList::New(); //maps old point ids into new
  pointMap->SetNumberOfIds(numPts);
  for (i=0; i < numPts; i++)
    {
    pointMap->SetId(i,-1);
    }

  // Determine which scalar data to use for thresholding
  if ( this->AttributeMode == VTK_ATTRIBUTE_MODE_DEFAULT )
    {
    if ( pointScalars != NULL)
      {
      usePointScalars = 1;
      }
    else
      {
      usePointScalars = 0;
      }
    }
  else if ( this->AttributeMode == VTK_ATTRIBUTE_MODE_USE_POINT_DATA )
    {
    usePointScalars = 1;
    }
  else
    {
    usePointScalars = 0;
    }

  // Check on scalar consistency
  if ( usePointScalars && !pointScalars )
    {
    vtkErrorMacro(<<"Can't use point scalars because there are none");
    return;
    }
  else if ( !usePointScalars && !cellScalars )
    {
    vtkErrorMacro(<<"Can't use cell scalars because there are none");
    return;
    }

  newCellPts = vtkIdList::New();     

  // Check that the scalars of each cell satisfy the threshold criterion
  for (cellId=0; cellId < input->GetNumberOfCells(); cellId++)
    {
    cell = input->GetCell(cellId);
    cellPts = cell->GetPointIds();
    numCellPts = cell->GetNumberOfPoints();
    
    if ( usePointScalars )
      {
      if (this->AllScalars)
        {
        keepCell = 1;
        for ( i=0; keepCell && (i < numCellPts); i++)
          {
          ptId = cellPts->GetId(i);
          keepCell = 
            (this->*(this->ThresholdFunction))(pointScalars->GetComponent(ptId,0));
          }
        }
      else
        {
        keepCell = 0;
        for ( i=0; (!keepCell) && (i < numCellPts); i++)
          {
          ptId = cellPts->GetId(i);
          keepCell = 
            (this->*(this->ThresholdFunction))(pointScalars->GetComponent(ptId,0));
          }
        }
      }
    else //use cell scalars
      {
      keepCell = (this->*(this->ThresholdFunction))(cellScalars->GetComponent(cellId,0));
      }
    
    if ( keepCell ) // satisfied thresholding
      {
      for (i=0; i < numCellPts; i++)
        {
        ptId = cellPts->GetId(i);
        if ( (newId = pointMap->GetId(ptId)) < 0 )
          {
          x = input->GetPoint(ptId);
          newId = newPoints->InsertNextPoint(x);
          pointMap->SetId(ptId,newId);
          outPD->CopyData(pd,ptId,newId);
          }
        newCellPts->InsertId(i,newId);
        }
      newCellId = output->InsertNextCell(cell->GetCellType(),newCellPts);
      outCD->CopyData(cd,cellId,newCellId);
      newCellPts->Reset();
      } // satisfied thresholding
    } // for all cells

  vtkDebugMacro(<< "Extracted " << output->GetNumberOfCells() 
                << " number of cells.");

  // now clean up / update ourselves
  pointMap->Delete();
  newCellPts->Delete();
  
  output->SetPoints(newPoints);
  newPoints->Delete();

  output->Squeeze();
}

// Return the method for manipulating scalar data as a string.
const char *vtkmpjThresholdPolyData::GetAttributeModeAsString(void)
{
  if ( this->AttributeMode == VTK_ATTRIBUTE_MODE_DEFAULT )
    {
    return "Default";
    }
  else if ( this->AttributeMode == VTK_ATTRIBUTE_MODE_USE_POINT_DATA )
    {
    return "UsePointData";
    }
  else 
    {
    return "UseCellData";
    }
}

void vtkmpjThresholdPolyData::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Attribute Mode: " << this->GetAttributeModeAsString() << endl;
  if (this->InputScalarsSelection)
    {
    os << indent << "InputScalarsSelection: " << this->InputScalarsSelection;
    } 

  os << indent << "All Scalars: " << this->AllScalars << "\n";
  if ( this->ThresholdFunction == &vtkmpjThresholdPolyData::Upper )
    {
    os << indent << "Threshold By Upper\n";
    }

  else if ( this->ThresholdFunction == &vtkmpjThresholdPolyData::Lower )
    {
    os << indent << "Threshold By Lower\n";
    }

  else if ( this->ThresholdFunction == &vtkmpjThresholdPolyData::Between )
    {
    os << indent << "Threshold Between\n";
    }

  os << indent << "Lower Threshold: " << this->LowerThreshold << "\n";
  os << indent << "Upper Threshold: " << this->UpperThreshold << "\n";
}
