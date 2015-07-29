

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


#include "vtkmpjLevelSetNeighborhoodExtractor.h"

vtkStandardNewMacro(vtkmpjLevelSetNeighborhoodExtractor);

// Construct object 
vtkmpjLevelSetNeighborhoodExtractor::vtkmpjLevelSetNeighborhoodExtractor()
{
  this->LevelSetValue = 0.0;
  this->InsidePoints = NULL;
  this->OutsidePoints = NULL;
  this->LargeValue = VTK_LARGE_FLOAT;
  this->NarrowBanding = 0;
  this->NarrowBandwidth = 12.0;
  this->InputNarrowBand = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjLevelSetNeighborhoodExtractor::~vtkmpjLevelSetNeighborhoodExtractor()
{  
  if (this->InsidePoints != NULL)
    this->InsidePoints->Delete();

  if (this->OutsidePoints != NULL)
    this->OutsidePoints->Delete();
  
  if (this->InputNarrowBand != NULL)
    this->InputNarrowBand->Delete();  
}

//----------------------------------------------------------------------------
void vtkmpjLevelSetNeighborhoodExtractor::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);  
  os << indent << "Input level set: " << this->GetInput() << "\n";
  os << indent << "Level set value: " << this->LevelSetValue << "\n";
  os << indent << "Narrow bandwidth: " << this->NarrowBandwidth << "\n";
  os << indent << "Narrowbanding: " << this->NarrowBanding << "\n";
  os << indent << "Input narrow band: ";
  os << this->InputNarrowBand << "\n";  
}

//----------------------------------------------------------------------------
void vtkmpjLevelSetNeighborhoodExtractor::SortNodesUsed() 
{
  int tmpIndex[3];
  double tmpValue[3];
  
  if (this->NodesUsedValue[0] < this->NodesUsedValue[1]) {
    if (this->NodesUsedValue[0] < this->NodesUsedValue[2]) {
      if (this->NodesUsedValue[1] < this->NodesUsedValue[2]) {
	tmpValue[0] = this->NodesUsedValue[0]; tmpIndex[0] = this->NodesUsedIndex[0];  
	tmpValue[1] = this->NodesUsedValue[1]; tmpIndex[1] = this->NodesUsedIndex[1];  
	tmpValue[2] = this->NodesUsedValue[2]; tmpIndex[2] = this->NodesUsedIndex[2];  
      } else {
	tmpValue[0] = this->NodesUsedValue[0]; tmpIndex[0] = this->NodesUsedIndex[0];  
	tmpValue[1] = this->NodesUsedValue[2]; tmpIndex[1] = this->NodesUsedIndex[2];  
	tmpValue[2] = this->NodesUsedValue[1]; tmpIndex[2] = this->NodesUsedIndex[1];  
      }
    } else 
      {
	tmpValue[0] = this->NodesUsedValue[2]; tmpIndex[0] = this->NodesUsedIndex[2];  
	tmpValue[1] = this->NodesUsedValue[0]; tmpIndex[1] = this->NodesUsedIndex[0];  
	tmpValue[2] = this->NodesUsedValue[1]; tmpIndex[2] = this->NodesUsedIndex[1];  
      }
  } else {
    if (this->NodesUsedValue[1] < this->NodesUsedValue[2]) {
      if (this->NodesUsedValue[0] < this->NodesUsedValue[2]) {
	tmpValue[0] = this->NodesUsedValue[1]; tmpIndex[0] = this->NodesUsedIndex[1];  
	tmpValue[1] = this->NodesUsedValue[0]; tmpIndex[1] = this->NodesUsedIndex[0];  
	tmpValue[2] = this->NodesUsedValue[2]; tmpIndex[2] = this->NodesUsedIndex[2];  
      } else {
	tmpValue[0] = this->NodesUsedValue[1]; tmpIndex[0] = this->NodesUsedIndex[1];  
	tmpValue[1] = this->NodesUsedValue[2]; tmpIndex[1] = this->NodesUsedIndex[2];  
	tmpValue[2] = this->NodesUsedValue[0]; tmpIndex[2] = this->NodesUsedIndex[0];  
      }
    } else
      {
	tmpValue[0] = this->NodesUsedValue[2]; tmpIndex[0] = this->NodesUsedIndex[2];  
	tmpValue[1] = this->NodesUsedValue[1]; tmpIndex[1] = this->NodesUsedIndex[1];  
	tmpValue[2] = this->NodesUsedValue[0]; tmpIndex[2] = this->NodesUsedIndex[0];  
      }    
  }
  
  this->NodesUsedValue[0] = tmpValue[0];
  this->NodesUsedValue[1] = tmpValue[1];
  this->NodesUsedValue[2] = tmpValue[2];

  this->NodesUsedIndex[0] = tmpIndex[0];
  this->NodesUsedIndex[1] = tmpIndex[1];
  this->NodesUsedIndex[2] = tmpIndex[2];
}

//----------------------------------------------------------------------------
void vtkmpjLevelSetNeighborhoodExtractor::Initialize()
{
  this->InsidePoints = vtkDoubleArray::New();
  this->InsidePoints->SetNumberOfComponents(4);
  
  this->OutsidePoints = vtkDoubleArray::New();
  this->OutsidePoints->SetNumberOfComponents(4);
}

//----------------------------------------------------------------------------
void vtkmpjLevelSetNeighborhoodExtractor::GenerateDataFull(vtkImageData *input, vtkImageData *output)
{  
  int dim[3]; input->GetDimensions(dim);
  int dim0xdim1 = dim[0]*dim[1];
  unsigned long index;
  int coord[3];

  vtkDataArray *out=output->GetPointData()->GetScalars();

  double distance;
  
  unsigned long totalPixels = 
    input->GetPointData()->GetScalars()->GetNumberOfTuples(); 
  
  unsigned long updateVisits = totalPixels / 10;
  if ( updateVisits < 1) { updateVisits = 1; }
  
  unsigned long i;
  for ( index = 0; index < totalPixels; index++ )
    {
      // update progress
      if ( !(index % updateVisits) )
	{
	  this->UpdateProgress( (float) index/ (float) totalPixels );
	}
      
      // decompose index
      coord[2] = index / dim0xdim1; 
      int r = index % dim0xdim1;
      coord[1] = r / dim[0];
      coord[0] = r % dim[0];
      
      distance = this->CalculateDistance( index, coord );
      
      // write out distance to the levelset
      out->SetComponent(index, 0, distance);      
    }
}

//----------------------------------------------------------------------------
void vtkmpjLevelSetNeighborhoodExtractor::GenerateDataNarrowBand(vtkImageData *input, vtkImageData *output)
{
  if ( !this->InputNarrowBand ) {
    vtkErrorMacro(<< "InputNarrowBand has not been set" );
    return;
  }
  
  if (this->InputNarrowBand->GetNumberOfComponents() < 4) {
    vtkErrorMacro(<< "InputNarrowBand should contain tuples with 4 components " );
    return;
  }

  int dim[3]; input->GetDimensions(dim);
  int dim0xdim1 = dim[0]*dim[1];
  int index=0, coord[3];
  
  vtkDataArray *out=output->GetPointData()->GetScalars();

  double maxValue = this->NarrowBandwidth / 2.0;
  double nodeValue;
  double distance;

  int nc = this->InputNarrowBand->GetNumberOfComponents();
  unsigned long totalPixels  = this->InputNarrowBand->GetNumberOfTuples();
  unsigned long updateVisits = totalPixels / 10;
  if ( updateVisits < 1) { updateVisits = 1; }

  unsigned int i;
  for ( i = 0; i < totalPixels; i++ )
    {
      
      // update progress
      if ( !(i % updateVisits) )
	{
	  this->UpdateProgress( (float) i/ (float) totalPixels );
	}
      
      coord[0] = (int)this->InputNarrowBand->GetComponent(index, 0);
      coord[1] = (int)this->InputNarrowBand->GetComponent(index, 1);
      coord[2] = (int)this->InputNarrowBand->GetComponent(index, 2);
      
      if (nc > 3)
	nodeValue = (double)this->InputNarrowBand->GetComponent(index, 3);
      else nodeValue = 0.0;
      
      if ( fabs(nodeValue) <= maxValue )
	{
	  index = dim0xdim1*coord[2] + dim[0]*coord[1] + coord[0];
	 
	  distance = this->CalculateDistance( index, coord );

	  // write out distance to the levelset
	  out->SetComponent(index, 0, distance);
	}
    }
}

//----------------------------------------------------------------------------
double vtkmpjLevelSetNeighborhoodExtractor::CalculateDistance( int index, int coord[3] )
{  
  vtkImageData *input = this->GetInput();
  vtkDataArray *in=input->GetPointData()->GetScalars();

  int dim[3]; input->GetDimensions(dim);
  int dim0xdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0xdim1};

  this->LastPointIsInside = 0;

  double centerValue = (double) in->GetComponent(index, 0);
  centerValue -= this->LevelSetValue;

  double centerNodeValue;
  
  int centerNodeIndex = index;
  int centerNodeCoord[3] = {coord[0], coord[1], coord[2]};
  
  if( centerValue == 0.0 )
    { 
      centerNodeValue = 0.0;
      this->InsidePoints->InsertNextTuple4( centerNodeCoord[0], 
					    centerNodeCoord[1], 
					    centerNodeCoord[2],
					    centerNodeValue);
      this->LastPointIsInside = 1;
      return 0.0;
    }
  
  int inside = ( centerValue <= 0.0 );

  int neighIndex = index;
  double neighValue;
  
  int neighNodeIndex;
  double neighNodeValue;
  
  double distance;

  // In each dimension, find the distance to the zero set
  // by linear interpolating along the grid line.
  for( unsigned int j = 0; j < 3; j++ )
    {
      neighNodeValue = this->LargeValue;

      for( int s = -1; s < 2; s = s + 2 )
	{
	  neighIndex = index + s*factor[j];
	  
	  if ( (coord[j] + s) < 0 || (coord[j] + s) >= dim[j] ) 
	    {
	      continue;
	    }
	  
	  neighValue = (double) in->GetComponent( neighIndex, 0 );
	  neighValue -= this->LevelSetValue;
	  
	  if( ( neighValue > 0 && inside ) ||
	      ( neighValue < 0 && !inside ) )
	    {
	      distance = centerValue / ( centerValue - neighValue );
	      
	      if( neighNodeValue > distance )
		{
		  neighNodeValue = distance;
		  neighNodeIndex = neighIndex;
		}
	    }
	  
	} // end one dim loop
      
      // put the minimum distance neighbor onto the heap
      this->NodesUsedIndex[j] = neighNodeIndex;
      this->NodesUsedValue[j] = neighNodeValue;
      
      // reset neighIndex
      
    } // end dimension loop
  
    
  // sort the neighbors according to distance
  this->SortNodesUsed();
  //  std::sort( m_NodesUsed.begin(), m_NodesUsed.end() );
  
  // The final distance is given by the minimum distance to the plane
  // crossing formed by the zero set crossing points.
  distance = 0.0;
  for( unsigned int j = 0; j < 3; j++ )
    {
      neighNodeIndex = this->NodesUsedIndex[j];
      neighNodeValue = this->NodesUsedValue[j];

      if( neighNodeValue >= this->LargeValue )
	{ 
	  break;
	}
      
      distance += (double) 1.0 / ( neighNodeValue * neighNodeValue );
    }
  
  if( distance == 0.0 )
    {
      return this->LargeValue;
    }
  
  distance = sqrt( (double) 1.0 / distance );
  centerNodeValue = distance;

  if( inside )
    {
      this->InsidePoints->InsertNextTuple4( centerNodeCoord[0],
					    centerNodeCoord[1],
					    centerNodeCoord[2],
					    centerNodeValue );
      this->LastPointIsInside = 1;
    }
  else
    {
      this->OutsidePoints->InsertNextTuple4( centerNodeCoord[0],
					     centerNodeCoord[1],
					     centerNodeCoord[2],
					     centerNodeValue );      
      this->LastPointIsInside = 0;
    }
  
  return distance;
  
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetNeighborhoodExtractor::ExecuteInformation()
{
  vtkImageData *output = this->GetOutput();  
  
  vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  output->SetScalarType(VTK_FLOAT);
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetNeighborhoodExtractor::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if( this->GetInput() == NULL ) 
    { 
      vtkErrorMacro( << "Input level set is NULL" );
      return;
    }
  
  this->Initialize();
  
  if( this->NarrowBanding )
    {
      this->GenerateDataNarrowBand(input, output);
    }
  else
    {
      this->GenerateDataFull(input, output);
    }
  
  vtkDebugMacro(<< "No. inside points: " << InsidePoints->GetNumberOfTuples());
  vtkDebugMacro(<< "No. outside points: " << OutsidePoints->GetNumberOfTuples());
}

