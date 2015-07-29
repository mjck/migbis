

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


#include "vtkmpjFastMarchingImageFilter.h"

vtkStandardNewMacro(vtkmpjFastMarchingImageFilter);

// Construct object 
vtkmpjFastMarchingImageFilter::vtkmpjFastMarchingImageFilter()
{  
  
  this->OutputExtent[0] = this->OutputExtent[2] = this->OutputExtent[4] = 0;
  this->OutputExtent[1] = this->OutputExtent[3] = this->OutputExtent[5] = 63;
  this->OutputSpacing[0] = this->OutputSpacing[1] = this->OutputSpacing[2] = 1.0;

  this->OverrideOutputInformation = 0;
  
  this->AlivePoints = NULL;
  this->TrialPoints = NULL;
  
  this->CollectPoints = 0;
  this->ProcessedPoints = NULL;

  this->SpeedConstant = 1.0;
  this->InverseSpeed = -1.0;
  
  this->LargeValue = VTK_LARGE_FLOAT / 2.0;
  this->StoppingValue = this->LargeValue;

  this->NormalizationFactor = 1.0;

  this->LabelImage = NULL;

  this->NumberOfRequiredInputs = 0;
}

// ----------------------------------------------------------------------------
vtkmpjFastMarchingImageFilter::~vtkmpjFastMarchingImageFilter()
{
  if (this->ProcessedPoints != NULL)
    this->ProcessedPoints->Delete();
  
  if (this->AlivePoints != NULL)
    this->AlivePoints->Delete();
  
  if (this->TrialPoints != NULL)
    this->TrialPoints->Delete();
  
  if (this->LabelImage != NULL)
    this->LabelImage->Delete();  
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingImageFilter::SetOutputExtent(int minX, int maxX,
						    int minY, int maxY,
						    int minZ, int maxZ)
{
  this->OutputExtent[0] = minX;
  this->OutputExtent[1] = maxX;
  this->OutputExtent[2] = minY;
  this->OutputExtent[3] = maxY;
  this->OutputExtent[4] = minZ;
  this->OutputExtent[5] = maxZ;
  this->Modified();
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingImageFilter::Initialize(vtkImageData *output)
{
  int dim[3]; output->GetDimensions(dim);
  vtkDataArray *out=output->GetPointData()->GetScalars();
  
  // set all output values to infinity
  out->FillComponent(0, this->LargeValue);
  
  // create label image
  this->LabelImage = vtkImageData::New();
  this->LabelImage->SetSpacing(output->GetSpacing());
  this->LabelImage->SetOrigin(output->GetOrigin());
  this->LabelImage->SetDimensions(output->GetDimensions());
  this->LabelImage->SetScalarType(VTK_SHORT);
  this->LabelImage->SetNumberOfScalarComponents(1);
  this->LabelImage->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  this->LabelImage->AllocateScalars();
  
  // set all points type to FarPoint
  vtkDataArray *label = this->LabelImage->GetPointData()->GetScalars();
  label->FillComponent(0, FarPoint);


  int index, coord[3];
  int dim0xdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0xdim1};
   
  // process input alive points
  if ( this->AlivePoints ) {
    
    int total = this->AlivePoints->GetNumberOfTuples();
    int nc = this->AlivePoints->GetNumberOfComponents();
    double value = 0.0;
    
    for(int i=0; i<total; i++) {
      
      // get coordinates
      coord[0] = (int)this->AlivePoints->GetComponent(i, 0);
      coord[1] = (int)this->AlivePoints->GetComponent(i, 1);
      coord[2] = (int)this->AlivePoints->GetComponent(i, 2);
      index = dim0xdim1*coord[2] + dim[0]*coord[1] + coord[0];
      
      // get output value
      if (nc > 3)
	value = (double)this->AlivePoints->GetComponent(i, 3);
      
      // make this an alive point
      label->SetComponent(index, 0, AlivePoint);
      
      // initialize output value
      out->SetComponent(index, 0, value);  
      
    }
    
  }
  
  // make sure the heap is empty
  while ( !this->TrialHeap.empty() )
    {
      this->TrialHeap.pop();
    }
    
  // process the input trial points
  if ( this->TrialPoints )
    {
      int total = this->TrialPoints->GetNumberOfTuples();
      int nc = this->TrialPoints->GetNumberOfComponents();
      double value = 0.0;
      
      for(int i=0; i<total; i++) {
	
	// get point coordinates
	coord[0] = (int)this->TrialPoints->GetComponent(i, 0);
	coord[1] = (int)this->TrialPoints->GetComponent(i, 1);
	coord[2] = (int)this->TrialPoints->GetComponent(i, 2);
	index = dim0xdim1*coord[2] + dim[0]*coord[1] + coord[0];

	// get output value
	if (nc > 3)
	  value = (double)this->TrialPoints->GetComponent(i, 3);
	
	// make this a trial point
	label->SetComponent(index, 0, TrialPoint);
	
	// initialize output value
	out->SetComponent(index, 0, value);
	
	// insert point into queue
	this->TrialHeap.push ( new NodeType(value, index) );
      }
      
    }

  vtkDebugMacro(<< "reached end of Initialize()");
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingImageFilter::GenerateData(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; output->GetDimensions(dim);
  int dim0xdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0xdim1};
  int coord[3];

  vtkDebugMacro(<< "in GenerateData()");

  vtkDataArray *in=NULL;
  vtkDataArray *out=output->GetPointData()->GetScalars();
  vtkDataArray *label;

  vtkDebugMacro(<< "calling Initialize()");

  this->Initialize( output );

  if (input != NULL) {
    in = input->GetPointData()->GetScalars();
  }

  label = this->LabelImage->GetPointData()->GetScalars();
  
  if (this->CollectPoints) {
    this->ProcessedPoints = vtkDoubleArray::New();
    this->ProcessedPoints->SetNumberOfComponents(4);
    this->ProcessedPoints->Allocate(500);
  }
  
  int index;
  double value;
  double currentValue;
  double oldValue = -this->LargeValue;

  double oldProgress = 0;
  unsigned long NumPoints = 0;
  unsigned long InvalidPoints = 0;
  
  this->UpdateProgress( 0.0 ); // Send first progress event

  // process points on the heap
  while ( !this->TrialHeap.empty () ) {
    
    // get the node with the smallest value
    NodeType *node = this->TrialHeap.top();
    this->TrialHeap.pop();

    index = node->index;
    value = node->value;

    if ( this->GetDebug() ) { NumPoints++; }
    
    // does this node contain the current value ?
    currentValue = (double) out->GetComponent(index, 0);

    if ( (float(value)) != currentValue )
      {
	if ( this->GetDebug() ) { InvalidPoints++; }
	printf("value=%f, currentValue=%f\n",value,currentValue);
	continue;
      } 
    
    // is this node already alive ?
    if ( label->GetComponent(index, 0) != TrialPoint ) 
      {
	if ( this->GetDebug() ) { InvalidPoints++; }
	continue;
      }    
    
    if ( currentValue > this->StoppingValue )
      {
	vtkDebugMacro(<< "stopping value reached");
	break;
      }
    
    if ( this->GetDebug() && currentValue < oldValue) 
      {
	vtkDebugMacro(<< "error value decrease at:" << index );
      }
    
    oldValue = currentValue;

    // decompose index
    coord[2] = index / dim0xdim1;
    int r = index % dim0xdim1;
    coord[1] = r / dim[0];
    coord[0] = r % dim[0];

    if ( this->CollectPoints )
      {	
	this->ProcessedPoints->InsertNextTuple4((double)coord[0], 
						(double)coord[1], 
						(double)coord[2],
						(double)value);
      }

    // set this node as alive
    label->SetComponent( index, 0, AlivePoint );
    
    // update its neighbors
    this->UpdateNeighbors( index, coord, input, output );

    // Send events every certain number of points.
    const double newProgress = currentValue / this->StoppingValue;
    if( newProgress - oldProgress > 0.01 )  // update every 1%
      {
	this->UpdateProgress( newProgress );
	oldProgress = newProgress;
	if ( this->GetAbortExecute() )
	  {
	    break;
	  }
      }
    
  }
  
  vtkDebugMacro(<< "No. points processed: " << NumPoints);
  vtkDebugMacro(<< "No. invalid points: " << InvalidPoints);
  
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingImageFilter::UpdateNeighbors( int index, int coord[3], vtkImageData *input, vtkImageData *output )
{
  vtkDataArray *label=this->LabelImage->GetPointData()->GetScalars();

  int dim[3]; output->GetDimensions(dim);
  int dim0xdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0xdim1};
  int neighCoord[3] = {coord[0], coord[1], coord[2]};
  int neighIndex = index;
  
  for ( int j = 0; j < 3; j++ ) // for each dimension
    { 

      // update left neighbor
      if ( (coord[j] - 1) >= 0 )
	{
	  neighIndex = index - factor[j];
	  neighCoord[j] = coord[j] - 1;	  
	}
      
      if ( label->GetComponent( neighIndex, 0 ) != AlivePoint )
	{
	  this->UpdateValue( neighIndex, neighCoord, input, output );
	}
      
      // update right neighbor
      if ( (coord[j] + 1) < dim[j] ) 
	{
	  neighIndex = index + factor[j];	  
	  neighCoord[j] = coord[j] + 1;
	}
      
      if ( label->GetComponent( neighIndex, 0 ) != AlivePoint )
	{
	  this->UpdateValue( neighIndex, neighCoord, input, output );
	}

      //reset neighCoord
      neighCoord[j] = coord[j];
      neighIndex = index;
      
    }
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingImageFilter::UpdateValue( int index, int coord[3], vtkImageData *input, vtkImageData *output )
{
  vtkDataArray *in=NULL;
  vtkDataArray *out = output->GetPointData()->GetScalars();
  vtkDataArray *label=this->LabelImage->GetPointData()->GetScalars();
    
  if (input != NULL) 
    in = input->GetPointData()->GetScalars();

  int dim[3]; output->GetDimensions(dim);
  int dim0xdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0xdim1};
  int neighIndex = index;
  int neighCoord[3] = {coord[0], coord[1], coord[2]}; 
  
  double neighValue, minValue, values[3];
  int minIndex;

  for ( int j = 0; j < 3; j++ ) // for each dimension
    {
      minValue = this->LargeValue;
      
      // find smallest valued neighbor in this dimension
      for( int s = -1; s < 2; s = s + 2 )
	{
	  neighIndex = index + s*factor[j];
	  
	  if ( (coord[j] + s) < 0 || (coord[j] + s) > dim[j] ) 
	    {
	      continue;
	    }
	  
	  if (label->GetComponent( neighIndex, 0 ) == AlivePoint )
	    {
	      neighValue = out->GetComponent( neighIndex, 0 );
	      
	      if( minValue > neighValue )
		{
		  minValue = neighValue;
		  minIndex = neighIndex;
		}
	    }	  	  
	}
      
      // store minimum value in each dimension
      values[j] = minValue;           
    }
  
  // sort the local list
  this->SortThreeValues(values); 
  
  // solve quadratic equation
  double aa, bb, cc;
  double solution = this->LargeValue;
  
  aa = 0.0;
  bb = 0.0;
  if ( input != NULL )
    {
      cc = (double) in->GetComponent(index, 0) / this->NormalizationFactor;
      cc = -1.0 * ( 1.0 / cc ) * ( 1.0 / cc);
    }
  else 
    {
      cc = this->InverseSpeed;
    }
 
  double discrim;
  
  for ( int j = 0; j < 3; j++ ) // for each dimension
    {
      if ( solution >= values[j] )
	{
	  aa += 1.0;
	  bb += values[j];
	  cc += values[j] * values[j];
	  
	  discrim = bb * bb - aa * cc;
	  if ( discrim < 0.0 )
	    {
	      // Discriminant of quadratic eqn. is negative
	      vtkErrorMacro(<<"Discriminant of quadratic equation is negative");
	      solution = this->LargeValue;
	    }
	  
	  solution = ( sqrt( discrim ) + bb ) / aa;
	}
      else
	{
	  break;
	}      
    }

  if ( solution < this->LargeValue )
    {
      // write solution to m_OutputLevelSet
      out->SetComponent(index, 0, solution);  
      
      // insert point into trial heap
      label->SetComponent(index, 0, TrialPoint);
      
      // insert point into queue
      this->TrialHeap.push ( new NodeType(solution, index) );
    }

  return solution;
  
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingImageFilter::SortThreeValues(double value[3]) 
{
  double tmp[3];

  if (value[0] < value[1]) {
    if (value[0] < value[2]) {
      if (value[1] < value[2]) {
	tmp[0] = value[0]; tmp[1] = value[1]; tmp[2] = value[2]; 
      } else {
	tmp[0] = value[0]; tmp[1] = value[2]; tmp[2] = value[1]; 
      }
    } else 
      {
	tmp[0] = value[2]; tmp[1] = value[0]; tmp[2] = value[1];
      }
  } else {
    if (value[1] < value[2]) {
      if (value[0] < value[2]) {
	tmp[0] = value[1]; tmp[1] = value[0]; tmp[2] = value[2];
      } else {
	tmp[0] = value[1]; tmp[1] = value[2]; tmp[2] = value[0];
      }
    } else
      {
	tmp[0] = value[2]; tmp[1] = value[1]; tmp[2] = value[0];
      }    
  }

  value[0] = tmp[0];
  value[1] = tmp[1];
  value[2] = tmp[2];
}


// ----------------------------------------------------------------------------
void vtkmpjFastMarchingImageFilter::SetSpeedConstant( double value )      
{
  this->SpeedConstant = value;
  this->InverseSpeed = -1.0 * ( 1.0 / this->SpeedConstant ) * ( 1.0 / this->SpeedConstant );
  this->Modified();
}

//----------------------------------------------------------------------------
void vtkmpjFastMarchingImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  
  os << indent << "Alive points: " << this->AlivePoints << "\n";
  os << indent << "Trial points: " << this->TrialPoints << "\n";
  os << indent << "Speed constant: " << this->SpeedConstant << "\n";
  os << indent << "Stopping value: " << this->StoppingValue << "\n";
  os << indent << "Large Value: " << this->LargeValue << "\n";
  os << indent << "Normalization Factor: " << this->NormalizationFactor << "\n";
  os << indent << "Collect points: " << this->CollectPoints << "\n";
  os << indent << "OverrideOutputInformation: " << this->OverrideOutputInformation << "\n";
  os << indent << "OutputExtent: " << 
    this->OutputExtent[0] << "," << 
    this->OutputExtent[1] << "," <<
    this->OutputExtent[2] << "," <<
    this->OutputExtent[3] << "," <<
    this->OutputExtent[4] << "," <<
    this->OutputExtent[5] << "\n";
  os << indent << "OutputSpacing: " <<
    this->OutputSpacing[0] << "," << 
    this->OutputSpacing[1] << "," << 
    this->OutputSpacing[2] << "\n";
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingImageFilter::ExecuteInformation()
{
  vtkImageData *input = this->GetInput();
  vtkImageData *output = this->GetOutput();
   
  if (input == NULL || this->OverrideOutputInformation) {
    output->SetNumberOfScalarComponents(1);  
    output->SetScalarType(VTK_FLOAT);
    output->SetSpacing(this->OutputSpacing[0], 
		       this->OutputSpacing[1], 
		       this->OutputSpacing[2]);
    output->SetWholeExtent(this->OutputExtent[0],this->OutputExtent[1],
			   this->OutputExtent[2],this->OutputExtent[3],
			   this->OutputExtent[4],this->OutputExtent[5]);
  } else {    
    output->CopyTypeSpecificInformation(input);
    output->SetScalarType(VTK_FLOAT);
  }
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingImageFilter::ComputeInputUpdateExtent( int inExt[6], 
							      int vtkNotUsed(outExt)[6] )
{
  vtkImageData *input = this->GetInput();
  // Make sure the Input has been set.
  if ( input == NULL )
    {
      memcpy(inExt,this->OutputExtent,sizeof(int)*6);
    } else {      
      int* wholeExtent = input->GetWholeExtent();
      memcpy(inExt,wholeExtent,sizeof(int)*6);
    }
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingImageFilter::ExecuteData(vtkDataObject *vtkNotUsed(out))
{  
  vtkDebugMacro("Executing.");
  vtkImageData* output = this->GetOutput();
  vtkImageData* input = this->GetInput();
  
  // Set the extent of the output and allocate memory.
  output->SetExtent(output->GetWholeExtent());
  output->AllocateScalars();

  vtkDebugMacro("Calling SimpleExecute.");
  this->SimpleExecute(input, output);
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingImageFilter::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  this->GenerateData(input, output);
}

// ----------------------------------------------------------------------------
