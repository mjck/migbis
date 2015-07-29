

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
#include "vtkmpjCardiacCoordinates.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjCardiacCoordinates* vtkmpjCardiacCoordinates::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjCardiacCoordinates");
  if(ret)
    {
      return (vtkmpjCardiacCoordinates*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjCardiacCoordinates;
}

// ----------------------------------------------------------------------------
// Construct object with no children.
vtkmpjCardiacCoordinates::vtkmpjCardiacCoordinates()
{
  	this->Mask = NULL;
	this->ApexBaseDirection[0] = 0;
	this->ApexBaseDirection[1] = 0;
	this->ApexBaseDirection[2] = 1;
	this->OutputTensor = 0;
	this->Eigenvalues[0]=1.0;
    this->Eigenvalues[1]=0.5;
    this->Eigenvalues[2]=0.1;
}

// ----------------------------------------------------------------------------
vtkmpjCardiacCoordinates::~vtkmpjCardiacCoordinates()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjCardiacCoordinates::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }

  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  int dim[3];  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();
  
  if (this->OutputTensor == 1)
  	output->SetNumberOfScalarComponents(6);
  else
  	output->SetNumberOfScalarComponents(9);
  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------	
inline void vtkmpjCardiacCoordinates::ProjectOntoPlane(double v[3], double n[3], double r[3])
{
  double d = vtkMath::Dot(v, n);
  r[0] = v[0] - n[0] * d; 
  r[1] = v[1] - n[1] * d; // r = v - (v . n) n
  r[2] = v[2] - n[2] * d;
}

void vtkmpjCardiacCoordinates::FixCoordinates(vtkImageData *input, vtkImageData *output, int coord[3], int index)
{
	vtkDataArray *in=input->GetPointData()->GetScalars();
	vtkDataArray *out = output->GetPointData()->GetScalars();	

  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];

  // number of voxels in a row and slice
  int factor[3] = {1, dim[0], dim0Tdim1};
 
	// phi value and position index
	double l[3], r[3], c[3];
	double ll[3], rr[3], cc[3];
	double dot;
	
	l[0] = in->GetComponent(index, 0);
	l[1] = in->GetComponent(index, 1);
	l[2] = in->GetComponent(index, 2);
	
	r[0] = in->GetComponent(index, 3);
	r[1] = in->GetComponent(index, 4);
	r[2] = in->GetComponent(index, 5);
	
	c[0] = in->GetComponent(index, 6);
	c[1] = in->GetComponent(index, 7);
	c[2] = in->GetComponent(index, 8);
	
	for (int j=0; j<3; j++)  // axis  
  	for (int i=-1; i<2; i+=2) // direction 
    {		
			int neighbor=index + (int)i*factor[j];	

			if ((coord[j]+i >= 0) && (coord[j]+i < dim[j]) && (this->InsideMask(neighbor))) {	  
				
				ll[0] = in->GetComponent(neighbor, 0);
				ll[1] = in->GetComponent(neighbor, 1);
				ll[2] = in->GetComponent(neighbor, 2);
				
				rr[0] = in->GetComponent(neighbor, 3);
				rr[1] = in->GetComponent(neighbor, 4);
				rr[2] = in->GetComponent(neighbor, 5);
				
				cc[0] = in->GetComponent(neighbor, 6);
				cc[1] = in->GetComponent(neighbor, 7);
				cc[2] = in->GetComponent(neighbor, 8);
				
				dot = vtkMath::Dot(l, ll);
				if (dot < 0) { ll[0]=-ll[0]; ll[1]=-ll[1]; ll[2]=-ll[2]; }
				dot = vtkMath::Dot(r, rr);	
				if (dot < 0) { rr[0]=-rr[0]; rr[1]=-rr[1]; rr[2]=-rr[2]; }
				dot = vtkMath::Dot(c, cc);
				if (dot < 0) { cc[0]=-cc[0]; cc[1]=-cc[1]; cc[2]=-cc[2]; }
				
				out->SetComponent(neighbor, 0, ll[0]);
		  		out->SetComponent(neighbor, 1, ll[1]);
		  		out->SetComponent(neighbor, 2, ll[2]);

				out->SetComponent(neighbor, 3, rr[0]);
				out->SetComponent(neighbor, 4, rr[1]);
				out->SetComponent(neighbor, 5, rr[2]);

				out->SetComponent(neighbor, 6, cc[0]);
				out->SetComponent(neighbor, 7, cc[1]);
				out->SetComponent(neighbor, 8, cc[2]);				
 			}
    }
}

// ----------------------------------------------------------------------------
void vtkmpjCardiacCoordinates::ComputeCoordinates(vtkImageData *input, int coord[3], int index, 
												double l[3], double r[3], double c[3])
{
  vtkDataArray *in=input->GetPointData()->GetScalars();
  
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  
  // number of voxels in a row and slice
  int factor[3] = {1, dim[0], dim0Tdim1};

	// radial vector is the input
	r[0] = in->GetComponent(index, 0);
	r[1] = in->GetComponent(index, 1);
	r[2] = in->GetComponent(index, 2);
	(void)vtkMath::Normalize(r);
	   
	// compute longitudinal vector
	this->ProjectOntoPlane(this->ApexBaseDirection, r, l);
	(void)vtkMath::Normalize(l);	
	
	// compute circumferential
	(void)vtkMath::Cross(l, r, c);
	(void)vtkMath::Normalize(c);
}

// ----------------------------------------------------------------------------
int vtkmpjCardiacCoordinates::InsideMask(int index)
{
  return (this->mask != NULL)?(this->mask->GetComponent(index, 0) > 0) : 1;
}
// ----------------------------------------------------------------------------
void vtkmpjCardiacCoordinates::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  vtkDataArray *out = output->GetPointData()->GetScalars();
  vtkDataArray *in = input->GetPointData()->GetScalars();
  int inmask;	

  // Get pointer to mask array
  this->mask = (this->Mask != NULL) ? this->Mask->GetPointData()->GetScalars() : NULL;

  double D[3][3]={{0,0,0},{0,0,0},{0,0,0}};  /* matrix of eigenvalues in mask */
  double V[3][3];                            /* matrix of eigenvectors in mask */
  double v1[3], v2[3], v3[3];
  double R1[3][3], R2[3][3];
  double T[3][3];  

  double Dxx;
  double Dyy;
  double Dzz;
  double Dxy;
  double Dxz;
  double Dyz;

  int index = 0;
  int coord[3];
  double endoepi[3], apexbase[3], antpost[3];

  D[0][0] = this->Eigenvalues[0];
  D[1][1] = this->Eigenvalues[1];
  D[2][2] = this->Eigenvalues[2];

  for (int k=0;k<dim[2];k++) {    
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
			{
				inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;

			  if (inmask) {
				
	  			coord[0]=i; coord[1]=j; coord[2]=k;
	  		
					this->ComputeCoordinates(input, coord, index, apexbase, endoepi, antpost);
	  		
	  				if (this->OutputTensor == 0) {

						out->SetComponent(index, 0, -endoepi[0]);  
		  				out->SetComponent(index, 1, -endoepi[1]);  /* ENDO TO EPI vector: defines the CIRC plane */
		  				out->SetComponent(index, 2, -endoepi[2]);

						out->SetComponent(index, 3, apexbase[0]);
				  		out->SetComponent(index, 4, apexbase[1]);   /* APEX TO BASE vector: defines the RADIAL plane */
				  		out->SetComponent(index, 5, apexbase[2]);
				
						out->SetComponent(index, 6, -antpost[0]);
						out->SetComponent(index, 7, -antpost[1]);   /* ANT TO POST vector: defines the LONG plane */
						out->SetComponent(index, 8, -antpost[2]);

					} else {

						V[0][0] = -endoepi[0];
						V[1][0] = -endoepi[1];
						V[2][0] = -endoepi[2];

						V[0][1] = apexbase[0];
						V[1][1] = apexbase[1];
						V[2][1] = apexbase[2];

						V[0][2] = -antpost[0];
						V[1][2] = -antpost[1];
						V[2][2] = -antpost[2];

						/* Tensor = V * D * V' */
            			(void) vtkMath::Transpose3x3(V,R1);
            			(void) vtkMath::Multiply3x3(D,R1,R2);
            			(void) vtkMath::Multiply3x3(V,R2,T);

            			Dxx = T[0][0];  
            			Dyy = T[1][1];  
            			Dzz = T[2][2]; 
            			Dxy = T[1][0]; 
            			Dxz = T[2][0]; 
            			Dyz = T[2][1]; 

            			out->SetComponent(index, 0, Dxx);
            			out->SetComponent(index, 1, Dxy);
			            out->SetComponent(index, 2, Dxz);
			            out->SetComponent(index, 3, Dyy);
			            out->SetComponent(index, 4, Dyz);
			            out->SetComponent(index, 5, Dzz);       

					}
					
				} else {
					
					for(int l=0;l<(this->OutputTensor==1?6:9);l++)
						out->SetComponent(index, l, 0.0);
				
				}
	  		
				++index;	  
			}  	  

    	double pog=double(k)/double(dim[2]);
    	this->UpdateProgress(pog);
	}
	
	//printf("fixing vectors\n");
	//for (int k=0;k<dim[2];k++) {    
    //for (int j=0;j<dim[1];j++)
      //for (int i=0;i<dim[0];i++)
			//{
				//inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;

			  //if (inmask) {
				
	  			//coord[0]=i; coord[1]=j; coord[2]=k;
	
			    //this->FixCoordinates(input, output, coord, index);	
	      //}
	
			//}
							
	//}
	
  this->UpdateProgress(1.0);
}

