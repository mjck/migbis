

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


#include "vtkmpjReinitializeLevelSetImageFilter.h"

vtkStandardNewMacro(vtkmpjReinitializeLevelSetImageFilter);

// Construct object 
vtkmpjReinitializeLevelSetImageFilter::vtkmpjReinitializeLevelSetImageFilter()
{
  this->LevelSetValue = 0.0;

  this->Locator = vtkmpjLevelSetNeighborhoodExtractor::New();
  this->Marcher = vtkmpjFastMarchingImageFilter::New();
  
  this->NarrowBanding = 0;
  this->InputNarrowBandwidth = 12.0;
  this->OutputNarrowBandwidth = 12.0;
  
  this->InputNarrowBand = NULL;
  this->OutputNarrowBand = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjReinitializeLevelSetImageFilter::~vtkmpjReinitializeLevelSetImageFilter()
{  
  if (this->Locator != NULL)
    this->Locator->Delete();
  
  if (this->Marcher != NULL)
    this->Marcher->Delete();
  
  if (this->InputNarrowBand != NULL)
    this->InputNarrowBand->Delete();  
  
  if (this->OutputNarrowBand != NULL)
    this->OutputNarrowBand->Delete();      
}

//----------------------------------------------------------------------------
void vtkmpjReinitializeLevelSetImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);  
  os << indent << "Level set value: " << this->LevelSetValue << "\n";
  os << indent << "Narrowbanding: " << this->NarrowBanding << "\n";
  os << indent << "Input narrow bandwidth: " << this->InputNarrowBandwidth;
  os << "\n";
  os << indent << "Output narrow bandwidth: " << this->OutputNarrowBandwidth;
  os << "\n";
  os << indent << "Input narrow band: " << this->InputNarrowBand;
  os << "\n";
  os << indent << "Output narrow band: " << this->OutputNarrowBand;
  os << "\n";
}

//----------------------------------------------------------------------------
void vtkmpjReinitializeLevelSetImageFilter::GenerateDataFull(vtkImageData *input, vtkImageData *output)
{  
  int dim[3]; input->GetDimensions(dim);

  vtkDataArray *in=input->GetPointData()->GetScalars();
  vtkDataArray *out=output->GetPointData()->GetScalars();

  unsigned long numtuples = out->GetNumberOfTuples();

  this->UpdateProgress( 0.0 );

  // locate the level set
  this->Locator->SetInputLevelSet( input );
  this->Locator->SetLevelSetValue( this->LevelSetValue );
  this->Locator->Locate();
  
  this->UpdateProgress( 0.33 );
  
  // march outward
  this->Marcher->SetOutputExtent( 0, dim[0]-1, 0, dim[1]-1, 0, dim[2]-1 );
  this->Marcher->SetTrialPoints( this->Locator->GetOutsidePoints() );
  this->Marcher->Update();

  double value;
  vtkDataArray *tmpLevelSet=this->Marcher->GetOutput()->GetPointData()->GetScalars();
  
  for (unsigned long index = 0; index < numtuples; index++) {    
    value = (double) in->GetComponent(index, 0);    
    if ( value - this->LevelSetValue > 0 ) {      
      out->SetComponent(index, 0, tmpLevelSet->GetComponent(index, 0));      
    }    
  }
  
  this->UpdateProgress( 0.66 );
  
  // march inward
  this->Marcher->SetTrialPoints( this->Locator->GetInsidePoints() );
  this->Marcher->Update();

  tmpLevelSet=this->Marcher->GetOutput()->GetPointData()->GetScalars();

  for (unsigned long index = 0; index < numtuples; index++) {    
    value = (double) in->GetComponent(index, 0);    
    if ( value - this->LevelSetValue <= 0 ) {
      value = (double) tmpLevelSet->GetComponent(index, 0);
      out->SetComponent(index, 0, -1.0 * value);
    }    
  }

  this->UpdateProgress( 1.0 );
}

//----------------------------------------------------------------------------
void vtkmpjReinitializeLevelSetImageFilter::GenerateDataNarrowBand(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0xdim1 = dim[0]*dim[1];
  int coord[3];

  vtkDataArray *in=input->GetPointData()->GetScalars();
  vtkDataArray *out=output->GetPointData()->GetScalars();

  unsigned long numtuples = out->GetNumberOfTuples();

  double posInfinity = VTK_DOUBLE_MAX;
  double negInfinity = VTK_DOUBLE_MIN;

  // set all internal pixels to minus infinity and 
  // all external pixels to positive infinity
  double value;

  for (unsigned long index = 0; index < numtuples; index++) {    
    value = in->GetComponent(index, 0);
    if ( value - this->LevelSetValue <= 0 ) {
      out->SetComponent(index, 0, negInfinity);
    } else {
      out->SetComponent(index, 0, posInfinity);
    }
  }
  
  // create a new output narrowband container
  if (this->OutputNarrowBand != NULL) {
    this->OutputNarrowBand->Delete();
  }
  this->OutputNarrowBand = vtkDoubleArray::New();
  this->OutputNarrowBand->SetNumberOfComponents(4);
  this->OutputNarrowBand->Allocate(100);
  
  this->UpdateProgress( 0.0 );
  
  // locate the level set
  this->Locator->SetInputLevelSet( input );
  this->Locator->SetLevelSetValue( this->LevelSetValue );

  if( this->NarrowBanding && this->InputNarrowBand )
    {
      this->Locator->NarrowBandingOn();
      this->Locator->SetNarrowBandwidth( this->InputNarrowBandwidth );
      this->Locator->SetInputNarrowBand( this->InputNarrowBand );
    }
  else
    { 
      this->Locator->NarrowBandingOff();
    }
  
  this->Locator->Locate();
  
  this->UpdateProgress( 0.33 );

  // march outward
  double stoppingValue = ( this->OutputNarrowBandwidth / 2.0 ) + 2.0;
  this->Marcher->SetStoppingValue( stoppingValue );
  this->Marcher->CollectPointsOn();
  this->Marcher->SetTrialPoints( this->Locator->GetOutsidePoints() );
  this->Marcher->Update();
  
  vtkDataArray *tmpLevelSet=this->Marcher->GetOutput()->GetPointData()->GetScalars();
  vtkDoubleArray *procPoints = this->Marcher->GetProcessedPoints();
  
  double inPixel;
  unsigned long pointIndex;

  unsigned long numpoints = procPoints->GetNumberOfTuples();

  for (unsigned long index = 0; index < numpoints; index++) {    
    
    coord[0] = (int)procPoints->GetComponent(index, 0);
    coord[1] = (int)procPoints->GetComponent(index, 1);
    coord[2] = (int)procPoints->GetComponent(index, 2);    
    pointIndex = dim0xdim1*coord[2] + dim[0]*coord[1] + coord[0];
    
    inPixel = value = in->GetComponent(pointIndex, 0);
    
    if ( value - this->LevelSetValue > 0 ) 
      {
	inPixel = tmpLevelSet->GetComponent(pointIndex, 0);	
	out->SetComponent(pointIndex, 0, inPixel);	
	this->OutputNarrowBand->InsertNextTuple4(coord[0],
						 coord[1],
						 coord[2],
						 procPoints->GetComponent(index, 3));
      }
    
  } // end for loop
  
  this->UpdateProgress( 0.66 );
 
  // march inward
  this->Marcher->SetTrialPoints( this->Locator->GetInsidePoints() );
  this->Marcher->Update();

  tmpLevelSet=this->Marcher->GetOutput()->GetPointData()->GetScalars();
  procPoints = this->Marcher->GetProcessedPoints();
  numpoints = procPoints->GetNumberOfTuples();

  for (unsigned long index = 0; index < numpoints; index++) {    
    
    coord[0] = (int)procPoints->GetComponent(index, 0);
    coord[1] = (int)procPoints->GetComponent(index, 1);
    coord[2] = (int)procPoints->GetComponent(index, 2);    
    pointIndex = dim0xdim1*coord[2] + dim[0]*coord[1] + coord[0];
    
    inPixel = value = in->GetComponent(pointIndex, 0);
    
    if( value - this->LevelSetValue <= 0 )
      {
	inPixel = value = tmpLevelSet->GetComponent(pointIndex, 0);
	inPixel = -1.0 * value;
	out->SetComponent(pointIndex, 0, inPixel);
	this->OutputNarrowBand->InsertNextTuple4(coord[0],
						 coord[1],
						 coord[2],
						 -1.0 * procPoints->GetComponent(index, 3));
      }
    
  } // end for loop

  this->UpdateProgress( 1.0 );
  
}

// ----------------------------------------------------------------------------
void vtkmpjReinitializeLevelSetImageFilter::ExecuteInformation()
{
  vtkImageData *output = this->GetOutput();  
  
  vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  output->SetScalarType(VTK_FLOAT);
}

// ----------------------------------------------------------------------------
void vtkmpjReinitializeLevelSetImageFilter::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if( this->GetInput() == NULL ) 
    { 
      vtkErrorMacro( << "Input level set is NULL" );
      return;
    }
  
  if( this->NarrowBanding )
    {
      this->GenerateDataNarrowBand(input, output);
    }
  else
    {
      this->GenerateDataFull(input, output);
    }
}

