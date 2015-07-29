

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


#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkmpjSmoothingRecursiveGaussianImageFilter.h"

//------------------------------------------------------------------------------
vtkStandardNewMacro(vtkmpjSmoothingRecursiveGaussianImageFilter);

// Construct object with no children.
vtkmpjSmoothingRecursiveGaussianImageFilter::vtkmpjSmoothingRecursiveGaussianImageFilter()
{
  this->NormalizeAcrossScale = 0;

  this->m_FirstSmoothingFilter = vtkmpjRecursiveGaussianImageFilter::New();
  this->m_FirstSmoothingFilter->SetOrder ( 0 );
  this->m_FirstSmoothingFilter->SetDirection( 0 );
  this->m_FirstSmoothingFilter->SetNormalizeAcrossScale( this->NormalizeAcrossScale );
  this->m_FirstSmoothingFilter->ReleaseDataFlagOn();
  //  this->m_FirstSmoothingFilter->DebugOn();
  
  for( unsigned int i = 0; i < 2; i++ )
    {
      this->m_SmoothingFilters[ i ] = vtkmpjRecursiveGaussianImageFilter::New();
      this->m_SmoothingFilters[ i ]->SetOrder( 0 );
      this->m_SmoothingFilters[ i ]->SetNormalizeAcrossScale( this->NormalizeAcrossScale );
      this->m_SmoothingFilters[ i ]->SetDirection( i+1 );
      this->m_SmoothingFilters[ i ]->ReleaseDataFlagOn();
    }
  
  this->m_SmoothingFilters[0]->SetInput( this->m_FirstSmoothingFilter->GetOutput() );
  for( unsigned int i = 1; i < 2; i++ )
    {
      this->m_SmoothingFilters[ i ]->SetInput( this->m_SmoothingFilters[i-1]->GetOutput() );
    }
    
  this->SetSigma( 1.0 );
}

vtkmpjSmoothingRecursiveGaussianImageFilter::~vtkmpjSmoothingRecursiveGaussianImageFilter() 
{
  if (this->m_FirstSmoothingFilter)
    this->m_FirstSmoothingFilter->Delete();
  
  for( unsigned int i = 0; i < 2; i++ )
    {
      if (this->m_SmoothingFilters[ i ])
	this->m_SmoothingFilters[ i ]->Delete();
    }
}

void vtkmpjSmoothingRecursiveGaussianImageFilter::SetSigma( float sigma )
{  
  for( unsigned int i = 0; i < 2; i++ )
    {
      this->m_SmoothingFilters[ i ]->SetSigma( sigma );
    }
  
  this->m_FirstSmoothingFilter->SetSigma( sigma );
  
  this->Modified();  
}

void  vtkmpjSmoothingRecursiveGaussianImageFilter::SetNormalizeAcrossScale( int normalize )
{
  
  this->NormalizeAcrossScale = normalize;
  
  for( unsigned int i = 0; i < 2; i++ )
    {
      this->m_SmoothingFilters[ i ]->SetNormalizeAcrossScale( normalize );
    }

  this->m_FirstSmoothingFilter->SetNormalizeAcrossScale( normalize );

  this->Modified();
}

// --------------------------------------------------------------------------------------------
void vtkmpjSmoothingRecursiveGaussianImageFilter::SimpleExecute(vtkImageData *input, vtkImageData *output)
{  
  int dim[3];  input->GetDimensions(dim);

  vtkDebugMacro(<< "SmoothingRecursiveGaussianImageFilter generating data ");
  
  for( unsigned int d=0; d < 3; d++)
    {
    if( dim[d] < 4 )
      {
	vtkErrorMacro("The number of pixels along dimension " << d << " is less than 4. This filter requires a minimum of four pixels along the dimension to be processed.");
      }
    }
  
  this->m_FirstSmoothingFilter->SetInput( input );
  this->m_SmoothingFilters[1]->Update();
  
  output->DeepCopy(this->m_SmoothingFilters[1]->GetOutput());
}
