//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
//BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
//BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
//BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
//BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
//BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
//BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//BIOIMAGESUITE_LICENSE  See also  http://www.gnu.org/licenses/gpl.html
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
//BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
//BIOIMAGESUITE_LICENSE 
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------




#include "vtkpxAppendAndLabelPolyData.h"

#include "vtkAlgorithmOutput.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataSetAttributes.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkLookupTable.h"

vtkCxxRevisionMacro(vtkpxAppendAndLabelPolyData, "$Revision: 1.103 $");
vtkStandardNewMacro(vtkpxAppendAndLabelPolyData);

//----------------------------------------------------------------------------
vtkpxAppendAndLabelPolyData::vtkpxAppendAndLabelPolyData()
{
  this->ParallelStreaming = 0;
  this->UserManagedInputs = 0;
}

//----------------------------------------------------------------------------
vtkpxAppendAndLabelPolyData::~vtkpxAppendAndLabelPolyData()
{
}

//----------------------------------------------------------------------------
// Add a dataset to the list of data to append.
void vtkpxAppendAndLabelPolyData::AddInput(vtkPolyData *ds)
{
  if (this->UserManagedInputs)
    {
    vtkErrorMacro(<<
      "AddInput is not supported if UserManagedInputs is true");
    return;
    }
  this->Superclass::AddInput(ds);
}

//----------------------------------------------------------------------------
// Remove a dataset from the list of data to append.
void vtkpxAppendAndLabelPolyData::RemoveInput(vtkPolyData *ds)
{
  if (this->UserManagedInputs)
    {
    vtkErrorMacro(<<
      "RemoveInput is not supported if UserManagedInputs is true");
    return;
    }

  vtkAlgorithmOutput *algOutput = 0;
  if (ds)
    {
    algOutput = ds->GetProducerPort();
    }

  this->RemoveInputConnection(0, algOutput);
}

//----------------------------------------------------------------------------
// make ProcessObject function visible
// should only be used when UserManagedInputs is true.
void vtkpxAppendAndLabelPolyData::SetNumberOfInputs(int num)
{
  if (!this->UserManagedInputs)
    {
    vtkErrorMacro(<<
      "SetNumberOfInputs is not supported if UserManagedInputs is false");
    return;
    }

  // Ask the superclass to set the number of connections.
  this->SetNumberOfInputConnections(0, num);
}

//----------------------------------------------------------------------------
// Set Nth input, should only be used when UserManagedInputs is true.
void vtkpxAppendAndLabelPolyData::SetInputByNumber(int num, vtkPolyData *input)
{
  if (!this->UserManagedInputs)
    {
    vtkErrorMacro(<<
      "SetInputByNumber is not supported if UserManagedInputs is false");
    return;
    }

  // Ask the superclass to connect the input.
  this->SetNthInputConnection(0, num, input? input->GetProducerPort() : 0);
}

//----------------------------------------------------------------------------
// This method is much too long, and has to be broken up!
// Append data sets into single polygonal data set.
int vtkpxAppendAndLabelPolyData::RequestData(vtkInformation *vtkNotUsed(request),
                                   vtkInformationVector **inputVector,
                                   vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the ouptut
  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  int idx;
  vtkPolyData *ds;
  vtkPoints  *inPts;
  vtkPoints *newPts;
  vtkCellArray *inVerts, *newVerts;
  vtkCellArray *inLines, *newLines;
  vtkCellArray *inPolys, *newPolys;
  vtkIdType sizePolys, numPolys;
  vtkCellArray *inStrips, *newStrips;
  vtkIdType numPts, numCells;
  vtkPointData *inPD = NULL;
  vtkCellData *inCD = NULL;
  vtkPointData *outputPD = output->GetPointData();
  vtkCellData *outputCD = output->GetCellData();
  vtkDataArray *newPtScalars = vtkDataArray::CreateDataArray(VTK_SHORT);
  vtkDataArray *newPtVectors = NULL;
  vtkDataArray *newPtNormals = NULL;
  vtkDataArray *newPtTCoords = NULL;
  vtkDataArray *newPtTensors = NULL;
  int i;
  vtkIdType *pts = 0;
  vtkIdType *pPolys;
  vtkIdType npts = 0;
  vtkIdType ptId, cellId;
  
  vtkDebugMacro(<<"Appending polydata");

  // loop over all data sets, checking to see what point data is available.
  numPts = 0;
  numCells = 0;
  sizePolys = numPolys = 0;

  int countPD=0;
  int countCD=0;

  vtkIdType numVerts = 0, numLines = 0, numStrips = 0;

  int numInputs = this->GetNumberOfInputConnections(0);
  vtkInformation *inInfo;

  // These Field lists are very picky.  Count the number of non empty inputs
  // so we can initialize them properly.
  for (idx = 0; idx < numInputs; ++idx)
    {
    inInfo = inputVector[0]->GetInformationObject(idx);
    ds = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (ds != NULL)
      {
      if ( ds->GetNumberOfPoints() > 0)
        {
        ++countPD;
        }
      if (ds->GetNumberOfCells() > 0 )  
        {
        ++countCD;
        } // for a data set that has cells
      } // for a non NULL input
    } // for each input

  // These are used to determine which fields are available for appending
  vtkDataSetAttributes::FieldList ptList(countPD);
  vtkDataSetAttributes::FieldList cellList(countCD);  
  
  countPD = countCD = 0;
  for (idx = 0; idx < numInputs; ++idx)
    {
    inInfo = inputVector[0]->GetInformationObject(idx);
    ds = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (ds != NULL)
      {
      // Skip points and cells if there are no points.  Empty inputs may have no arrays.
      if ( ds->GetNumberOfPoints() > 0)
        {
        numPts += ds->GetNumberOfPoints();
        // Take intersection of available point data fields.
        inPD = ds->GetPointData();
        if ( countPD == 0 )
          {
          ptList.InitializeFieldList(inPD);
          }
        else
          {
          ptList.IntersectFieldList(inPD);
          }
        ++countPD;
        } // for a data set that has points
        
      // Although we cannot have cells without points ... let's not nest.
      if (ds->GetNumberOfCells() > 0 )  
        {
        // keep track of the size of the poly cell array
        if (ds->GetPolys())
          {
          sizePolys += ds->GetPolys()->GetNumberOfConnectivityEntries();
          }
        numCells += ds->GetNumberOfCells();
        // Count the cells of each type.
        // This is used to ensure that cell data is copied at the correct
        // locations in the output.
        numVerts += ds->GetNumberOfVerts();
        numLines += ds->GetNumberOfLines();
        numPolys += ds->GetNumberOfPolys();
        numStrips += ds->GetNumberOfStrips();
        
        inCD = ds->GetCellData();
        if ( countCD == 0 )
          {
          cellList.InitializeFieldList(inCD);
          }
        else
          {
          cellList.IntersectFieldList(inCD);
          }
        ++countCD;
        } // for a data set that has cells
      } // for a non NULL input
    } // for each input

  if ( numPts < 1 || numCells < 1 )
    {
    vtkDebugMacro(<<"No data to append!");
    return 1;
    }
  this->UpdateProgress(0.10);

  // AllocateScalars
  if (newPtScalars)
    {
      newPtScalars->SetNumberOfComponents(1);
      newPtScalars->SetNumberOfTuples(numPts);
    }

  // Allocate Scalar Lookup Table 
  vtkLookupTable*cmap =vtkLookupTable::New();
  cmap->SetNumberOfTableValues(numInputs);
  cmap->SetTableRange(0,numInputs);
  float numc=float(cmap->GetNumberOfColors());
  for (i=0;i<cmap->GetNumberOfColors();i++)
      {
	float v=float(i+1)/float(numc);
	cmap->SetTableValue(i,v,v,1-v,1.0);
      }
  newPtScalars->SetLookupTable(cmap);
  cmap->Delete();


  // Examine the points and check if they're the same type. If not,
  // use highest (double probably), otherwise the type of the first
  // array (float no doubt). Depends on defs in vtkSetGet.h - Warning.
  int ttype, firstType=1, AllSame=1;
  int pointtype = 0;

  // Keep track of types for fast point append
  for (idx = 0; idx < numInputs; ++idx)
    {
    inInfo = inputVector[0]->GetInformationObject(idx);
    ds = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (ds != NULL && ds->GetNumberOfPoints()>0)
      {
      if ( firstType )
        {
        firstType = 0;
        pointtype = ds->GetPoints()->GetData()->GetDataType();
        }
      ttype = ds->GetPoints()->GetData()->GetDataType();
      
      if ( ttype != pointtype )
        {
        AllSame = 0;
        vtkDebugMacro(<<"Different point data types");
        }
      pointtype = pointtype > ttype ? pointtype : ttype;
      }
    }

  // Allocate geometry/topology
  newPts = vtkPoints::New(pointtype);
  newPts->SetNumberOfPoints(numPts);

  newVerts = vtkCellArray::New();
  newVerts->Allocate(numCells*4);

  newLines = vtkCellArray::New();
  newLines->Allocate(numCells*4);

  newStrips = vtkCellArray::New();
  newStrips->Allocate(numCells*4);

  newPolys = vtkCellArray::New();
  pPolys = newPolys->WritePointer(numPolys, sizePolys);

  if (!pPolys && sizePolys > 0)
    {
    vtkErrorMacro(<<"Memory allocation failed in append filter");
    return 0;
    }


  // loop over all input sets
  vtkIdType ptOffset = 0;
  vtkIdType vertOffset = 0;
  vtkIdType linesOffset = 0;
  vtkIdType polysOffset = 0;
  vtkIdType stripsOffset = 0;
  countPD = countCD = 0;
  for (idx = 0; idx < numInputs; ++idx)
    {
    this->UpdateProgress(0.2 + 0.8*idx/numInputs);
    inInfo = inputVector[0]->GetInformationObject(idx);
    ds = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    // this check is not necessary, but I'll put it in anyway
    if (ds != NULL)
      {
      numPts = ds->GetNumberOfPoints();
      numCells = ds->GetNumberOfCells();
      if ( numPts <= 0 && numCells <= 0 )
        {
        continue; //no input, just skip
        }

      inPD = ds->GetPointData();
      inCD = ds->GetCellData();

      inPts = ds->GetPoints();
      inVerts = ds->GetVerts();
      inLines = ds->GetLines();
      inPolys = ds->GetPolys();
      inStrips = ds->GetStrips();

      if (ds->GetNumberOfPoints() > 0)
        {
        // copy points directly
        if (AllSame)
          {
          this->AppendData(newPts->GetData(),
                           inPts->GetData(), ptOffset);
          }
        else
          {
          this->AppendDifferentPoints(newPts->GetData(),
                                      inPts->GetData(), ptOffset);
          }
        // copy scalars directly
	if (newPtScalars)
	  {
	    for (i=0;i<ds->GetNumberOfPoints();i++)
	      newPtScalars->SetComponent(i+ptOffset,0,idx);
	  }
	
        // copy normals directly
        if (newPtNormals)
          {
	    this->AppendData(newPtNormals, inPD->GetNormals(), ptOffset);
          }
        // copy vectors directly
        if (newPtVectors)
          {
	    this->AppendData(newPtVectors, inPD->GetVectors(), ptOffset);
          }
        // copy tcoords directly
        if (newPtTCoords)
          {
	    this->AppendData(newPtTCoords, inPD->GetTCoords() , ptOffset);
          }
        // copy tensors directly
        if (newPtTensors)
          {
	    this->AppendData(newPtTensors, inPD->GetTensors(), ptOffset);
          }
        // append the remainder of the field data
        for (ptId=0; ptId < numPts; ptId++)
          {
	    outputPD->CopyData(ptList,inPD,countPD,ptId,ptId+ptOffset);
          }
        ++countPD;
        }


      if (ds->GetNumberOfCells() > 0)
        {
        // These are the cellIDs at which each of the cell types start.
        vtkIdType linesIndex = ds->GetNumberOfVerts();
        vtkIdType polysIndex = linesIndex + ds->GetNumberOfLines();
        vtkIdType stripsIndex = polysIndex + ds->GetNumberOfPolys();

        // cell data could be made efficient like the point data,
        // but I will wait on that.
        // copy cell data
        for (cellId=0; cellId < numCells; cellId++)
          {
          vtkIdType outCellId = 0;
          if (cellId < linesIndex)
            {
            outCellId = vertOffset;
            vertOffset++;
            }
          else if (cellId < polysIndex)
            {
            // outCellId = number of lines we already added + total number of 
            // verts expected in the output.
            outCellId = linesOffset + numVerts;
            linesOffset++;
            }
          else if (cellId < stripsIndex)
            {
            // outCellId = number of polys we already added + total number of 
            // verts and lines expected in the output.
            outCellId = polysOffset + numLines + numVerts;
            polysOffset++;
            }
          else
            {
            // outCellId = number of tstrips we already added + total number of 
            // polys, verts and lines expected in the output.
            outCellId = stripsOffset + numPolys + numLines + numVerts;
            stripsOffset++;
            }
          outputCD->CopyData(cellList,inCD,countCD,cellId,outCellId);
          }
        ++countCD;
        
        // copy the cells
        pPolys = this->AppendCells(pPolys, inPolys, ptOffset);
        
        // These other cell arrays could be made efficient like polys ...
        for (inVerts->InitTraversal(); inVerts->GetNextCell(npts,pts); )
          {
          newVerts->InsertNextCell(npts);
          for (i=0; i < npts; i++)
            {
            newVerts->InsertCellPoint(pts[i]+ptOffset);
            }
          }
        
        for (inLines->InitTraversal(); inLines->GetNextCell(npts,pts); )
          {
          newLines->InsertNextCell(npts);
          for (i=0; i < npts; i++)
            {
            newLines->InsertCellPoint(pts[i]+ptOffset);
            }
          }

        for (inStrips->InitTraversal(); inStrips->GetNextCell(npts,pts); )
          {
          newStrips->InsertNextCell(npts);
          for (i=0; i < npts; i++)
            {
            newStrips->InsertCellPoint(pts[i]+ptOffset);
            }
          }
        }
      ptOffset += numPts;
      }
    }
  
  // Update ourselves and release memory
  //
  output->SetPoints(newPts);
  newPts->Delete();

  if (newPtScalars)
    {
    output->GetPointData()->SetScalars(newPtScalars);
    newPtScalars->Delete();
    }
  if (newPtNormals)
    {
    output->GetPointData()->SetNormals(newPtNormals);
    newPtNormals->Delete();
    }
  if (newPtVectors)
    {
    output->GetPointData()->SetVectors(newPtVectors);
    newPtVectors->Delete();
    }
  if (newPtTCoords)
    {
    output->GetPointData()->SetTCoords(newPtTCoords);
    newPtTCoords->Delete();
    }
  if (newPtTensors)
    {
    output->GetPointData()->SetTensors(newPtTensors);
    newPtTensors->Delete();
    }
  
  if ( newVerts->GetNumberOfCells() > 0 )
    {
    output->SetVerts(newVerts);
    }
  newVerts->Delete();

  if ( newLines->GetNumberOfCells() > 0 )
    {
    output->SetLines(newLines);
    }
  newLines->Delete();

  if ( newPolys->GetNumberOfCells() > 0 )
    {
    output->SetPolys(newPolys);
    }
  newPolys->Delete();

  if ( newStrips->GetNumberOfCells() > 0 )
    {
    output->SetStrips(newStrips);
    }
  newStrips->Delete();

  // When all optimizations are complete, this squeeze will be unecessary.
  // (But it does not seem to cost much.)
  output->Squeeze();

  return 1;
}

//----------------------------------------------------------------------------
int vtkpxAppendAndLabelPolyData::RequestUpdateExtent(vtkInformation *vtkNotUsed(request),
                                           vtkInformationVector **inputVector,
                                           vtkInformationVector *outputVector)
{
  // get the output info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  int piece, numPieces, ghostLevel;
  int idx;

  piece = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER());
  numPieces = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES());
  ghostLevel = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS());
  
  // make sure piece is valid
  if (piece < 0 || piece >= numPieces)
    {
    return 0;
    }

  int numInputs = this->GetNumberOfInputConnections(0);
  if (this->ParallelStreaming)
    {
    piece = piece * numInputs;
    numPieces = numPieces * numInputs;
    }
 
  vtkInformation *inInfo;
  // just copy the Update extent as default behavior.
  for (idx = 0; idx < numInputs; ++idx)
    {
    inInfo = inputVector[0]->GetInformationObject(idx);
    if (inInfo)
      {
      if (this->ParallelStreaming)
        {
        inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER(),
                    piece + idx);
        inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES(),
                    numPieces);
        inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS(),
                    ghostLevel);
        }
      else
        {
        inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER(),
                    piece);
        inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES(),
                    numPieces);
        inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS(),
                    ghostLevel);
        }
      }
    }
  
  return 1;
}

//----------------------------------------------------------------------------
vtkPolyData *vtkpxAppendAndLabelPolyData::GetInput(int idx)
{
  return vtkPolyData::SafeDownCast(
    this->GetExecutive()->GetInputData(0, idx));
}

//----------------------------------------------------------------------------
void vtkpxAppendAndLabelPolyData::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << "ParallelStreaming:" << (this->ParallelStreaming?"On":"Off") << endl;
  os << "UserManagedInputs:" << (this->UserManagedInputs?"On":"Off") << endl;
}

//----------------------------------------------------------------------------
template <class T>
size_t vtkpxAppendAndLabelPolyDataGetTypeSize(T*)
{
  return sizeof(T);
}

//----------------------------------------------------------------------------
void vtkpxAppendAndLabelPolyData::AppendData(vtkDataArray *dest, vtkDataArray *src,
                                   vtkIdType offset)
{
  void *pSrc, *pDest;
  vtkIdType length;

  // sanity checks
  if (src->GetDataType() != dest->GetDataType())
    {
    vtkErrorMacro("Data type mismatch.");
    return;
    }
  if (src->GetNumberOfComponents() != dest->GetNumberOfComponents())
    {
    vtkErrorMacro("NumberOfComponents mismatch.");
    return;
    }
  if (src->GetNumberOfTuples() + offset > dest->GetNumberOfTuples())
    {
    vtkErrorMacro("Destination not big enough");
    return;
    }
  
  // convert from tuples to components.
  offset *= src->GetNumberOfComponents();
  length = src->GetMaxId() + 1;

  switch (src->GetDataType())
    {
    vtkTemplateMacro(
      length *= vtkpxAppendAndLabelPolyDataGetTypeSize(static_cast<VTK_TT*>(0))
      );
    default:
      vtkErrorMacro("Unknown data type " << src->GetDataType());
    }

  pSrc  = src->GetVoidPointer(0);
  pDest = dest->GetVoidPointer(offset);

  memcpy(pDest, pSrc, length);
}

//----------------------------------------------------------------------------
void vtkpxAppendAndLabelPolyData::AppendDifferentPoints(vtkDataArray *dest,
                                              vtkDataArray *src,
                                              vtkIdType offset)
{
  float  *fSrc;
  double *dSrc, *dDest;
  vtkIdType p;

  if (src->GetNumberOfTuples() + offset > dest->GetNumberOfTuples())
    {
    vtkErrorMacro("Destination not big enough");
    return;
    }

  vtkIdType vals = src->GetMaxId()+1;
  switch (dest->GetDataType())
    {
    //
    // Dest is FLOAT - if sources are not all same type, dest ought to
    // be double. (assuming float and double are the only choices)
    //
    case VTK_FLOAT:
        vtkErrorMacro("Dest type should be double? "
            << dest->GetDataType());
        break;
    //
    // Dest is DOUBLE - sources may be mixed float/double combinations
    //

    case VTK_DOUBLE:
      dDest = static_cast<double*>(
        dest->GetVoidPointer(offset*src->GetNumberOfComponents()));
      //
      switch (src->GetDataType())
        {
        case VTK_FLOAT:
          fSrc = static_cast<float*>(src->GetVoidPointer(0));
          for (p=0; p<vals; p++)
            {
            dDest[p] = static_cast<double>(fSrc[p]);
            }
          break;
        case VTK_DOUBLE:
          dSrc = static_cast<double*>(src->GetVoidPointer(0));
          memcpy(dDest, dSrc, vals*sizeof(double));
          break;
        default:
          vtkErrorMacro("Unknown data type " << dest->GetDataType());
        }
      break;
      //
    default:
      vtkErrorMacro("Unknown data type " << dest->GetDataType());
    }

}


//----------------------------------------------------------------------------
// returns the next pointer in dest
vtkIdType *vtkpxAppendAndLabelPolyData::AppendCells(vtkIdType *pDest, vtkCellArray *src,
                                          vtkIdType offset)
{
  vtkIdType *pSrc, *end, *pNum;

  if (src == NULL)
    {
    return pDest;
    }

  pSrc = src->GetPointer();
  end = pSrc + src->GetNumberOfConnectivityEntries();
  pNum = pSrc;
  
  while (pSrc < end)
    {
    if (pSrc == pNum)
      {
      // move cell pointer to next cell
      pNum += 1+*pSrc;
      // copy the number of cells
      *pDest++ = *pSrc++;
      }
    else
      {
      // offset the point index
      *pDest++ = offset + *pSrc++;
      }
    }
  
  return pDest;
}

//----------------------------------------------------------------------------
int vtkpxAppendAndLabelPolyData::FillInputPortInformation(int port, vtkInformation *info)
{
  if (!this->Superclass::FillInputPortInformation(port, info))
    {
    return 0;
    }
  info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 1);
  return 1;
}

