

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
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkmpjRecursiveSeparableImageFilter.h"

//------------------------------------------------------------------------------
vtkStandardNewMacro(vtkmpjRecursiveSeparableImageFilter);


// Construct object with no children.
vtkmpjRecursiveSeparableImageFilter::vtkmpjRecursiveSeparableImageFilter()
{
  this->Direction = 0;
}

// ----------------------------------------------------------------------------
vtkmpjRecursiveSeparableImageFilter::~vtkmpjRecursiveSeparableImageFilter()
{
}

// --------------------------------------------------------------------------------------------
void vtkmpjRecursiveSeparableImageFilter::FilterDataArray(RealType *outs, const RealType *data, RealType *scratch, unsigned int ln)
{
  /**
   * Causal direction pass
   */
  
  // this value is assumed to exist from the border to infinity.
  const RealType outV1 = data[0];

  /**
   * Initialize borders
   */
  scratch[0] = RealType( m_N00 * outV1   + m_N11 * outV1   + m_N22 * outV1   + m_N33 * outV1 );
  scratch[1] = RealType( m_N00 * data[1] + m_N11 * outV1   + m_N22 * outV1   + m_N33 * outV1 );
  scratch[2] = RealType( m_N00 * data[2] + m_N11 * data[1] + m_N22 * outV1   + m_N33 * outV1 );
  scratch[3] = RealType( m_N00 * data[3] + m_N11 * data[2] + m_N22 * data[1] + m_N33 * outV1 );
  
  // note that the outV1 value is multiplied by the Boundary coefficients m_BNi
  scratch[0] -= RealType( m_BN1 * outV1 + m_BN2 * outV1 + m_BN3 * outV1  + m_BN4 * outV1 );
  scratch[1] -= RealType( m_D11 * scratch[0] + m_BN2 * outV1 + m_BN3 * outV1  + m_BN4 * outV1 );
  scratch[2] -= RealType( m_D11 * scratch[1] + m_D22 * scratch[0] + m_BN3 * outV1  + m_BN4 * outV1 );
  scratch[3] -= RealType( m_D11 * scratch[2] + m_D22 * scratch[1] + m_D33 * scratch[0]  + m_BN4 * outV1 );
  
  /**
   * Recursively filter the rest
   */
  for( unsigned int i=4; i<ln; i++ ) 
    {
      scratch[i]  = RealType( m_N00 * data[i] + m_N11 * data[i-1] + m_N22 * data[i-2] + m_N33 * data[i-3] );
      scratch[i] -= RealType( m_D11 * scratch[i-1] + m_D22 *   scratch[i-2] + m_D33 *   scratch[i-3] + m_D44 *   scratch[i-4] );
    }
  
  /**
   * Store the causal result
   */
  for( unsigned int i=0; i<ln; i++ ) 
    {
      outs[i] = RealType( m_K * scratch[i] );
    }
    
  /**
   * AntiCausal direction pass
   */
  
  // this value is assumed to exist from the border to infinity.
  const RealType outV2 = data[ln-1];
  
  /**
   * Initialize borders
   */
  scratch[ln-1] = RealType( m_M11 * outV2      + m_M22 * outV2      + m_M33 * outV2      + m_M44 * outV2);
  scratch[ln-2] = RealType( m_M11 * data[ln-1] + m_M22 * outV2      + m_M33 * outV2      + m_M44 * outV2); 
  scratch[ln-3] = RealType( m_M11 * data[ln-2] + m_M22 * data[ln-1] + m_M33 * outV2      + m_M44 * outV2); 
  scratch[ln-4] = RealType( m_M11 * data[ln-3] + m_M22 * data[ln-2] + m_M33 * data[ln-1] + m_M44 * outV2);
  
  // note that the outV2value is multiplied by the Boundary coefficients m_BMi
  scratch[ln-1] -= RealType( m_BM1 * outV2    + m_BM2 * outV2    + m_BM3 * outV2    + m_BM4 * outV2);
  scratch[ln-2] -= RealType( m_D11 * scratch[ln-1] + m_BM2 * outV2    + m_BM3 * outV2    + m_BM4 * outV2);
  scratch[ln-3] -= RealType( m_D11 * scratch[ln-2] + m_D22 * scratch[ln-1] + m_BM3 * outV2    + m_BM4 * outV2);
  scratch[ln-4] -= RealType( m_D11 * scratch[ln-3] + m_D22 * scratch[ln-2] + m_D33 * scratch[ln-1] + m_BM4 * outV2);
  
  /**
   * Recursively filter the rest
   */
  for( unsigned int i=ln-4; i>0; i-- ) 
    {
      scratch[i-1]  = RealType( m_M11 * data[i] + m_M22 * data[i+1] + m_M33 * data[i+2] + m_M44 * data[i+3] );
      scratch[i-1] -= RealType( m_D11 *   scratch[i] + m_D22 *  scratch[i+1] + m_D33 *   scratch[i+2] + m_D44 *   scratch[i+3] );
    }
  
  /**
   * Roll the antiCausal part into the output
   */
  for( unsigned int i=0; i<ln; i++ ) 
    {
      outs[i] += RealType( m_K * scratch[i] );
    }
  
}

// --------------------------------------------------------------------------------------------
void vtkmpjRecursiveSeparableImageFilter::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int dim[3];  input->GetDimensions(dim);
  float sp[3]; input->GetSpacing(sp);
  
  int dim0Tdim1 = dim[0]*dim[1];
  
  vtkDataArray *in=input->GetPointData()->GetScalars();
  vtkDataArray *out=output->GetPointData()->GetScalars();

  if( this->Direction > 3 )
    {
      vtkErrorMacro("Direction selected for filtering is greater than 3");
    }
  
  this->SetUp( sp[this->Direction] );
  
  const unsigned int ln = dim[ this->Direction ];

  if( ln < 4 )
    {
      vtkErrorMacro("The number of pixels along direction " << this->Direction << " is less than 4. This filter requires a minimum of four pixels along the dimension to be processed.");
    }
  
  RealType *inps = 0;
  RealType *outs = 0;
  RealType *scratch = 0;
  
  try 
    {
      inps = new RealType[ ln ];
    }
  catch( std::bad_alloc & ) 
    {
      vtkErrorMacro("Problem allocating memory for internal computations");
    }
  
  try 
    {
      outs = new RealType[ ln ];
    }
  catch( std::bad_alloc & ) 
    {
      delete [] inps;
      vtkErrorMacro("Problem allocating memory for internal computations");
    }
  
  try 
    {
      scratch = new RealType[ ln ];
    }
  catch( std::bad_alloc &) 
    {
      delete [] inps;
      delete [] outs;
      vtkErrorMacro("Problem allocating memory for internal computations");
    }

  switch(this->Direction) {
    
  case 0:
    
    for(int k=0; k<dim[2]; k++) 
      for(int i=0; i<dim[1]; i++) {
	
	for(int j=0; j<dim[0]; j++) {
	  int index = k*dim0Tdim1 + i*dim[0] + j;     	  
	  inps[j] = in->GetComponent(index, 0);	  
	}
	
	this->FilterDataArray( outs, inps, scratch, ln );
	
	for(int j=0; j<dim[0]; j++) {
	  int index = k*dim0Tdim1 + i*dim[0] + j;     	  
	  out->SetComponent(index, 0, outs[j]);	  
	}
	
      }
    
    break;
    
  case 1:        
    
    for(int k=0; k<dim[2]; k++) 
      for(int j=0; j<dim[0]; j++) {
	
	for(int i=0; i<dim[1]; i++) {
	  int index = k*dim0Tdim1 + i*dim[0] + j;     	  
	  inps[i] = in->GetComponent(index, 0);	  
	}
	
	this->FilterDataArray( outs, inps, scratch, ln );
	
	for(int i=0; i<dim[1]; i++) {
	  int index = k*dim0Tdim1 + i*dim[0] + j;     	  
	  out->SetComponent(index, 0, outs[i]);	  
	}	
	
      }
    
    break;
    
  case 2:
    
    for(int j=0; j<dim[0]; j++)
      for(int i=0; i<dim[1]; i++) {
	
	for(int k=0; k<dim[2]; k++) {
	  int index = k*dim0Tdim1 + i*dim[0] + j;     	  
	  inps[k] = in->GetComponent(index, 0);	  
	}
	
	this->FilterDataArray( outs, inps, scratch, ln );
	
	for(int k=0; k<dim[2]; k++) {
	  int index = k*dim0Tdim1 + i*dim[0] + j;     	  
	  out->SetComponent(index, 0, outs[k]);	  
	}       	
	
      }
    
    break;

  };
  
  delete [] outs;
  delete [] inps;
  delete [] scratch;

}

