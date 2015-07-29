

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
#include "vtkmpjLatticeAnisotropy.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjLatticeAnisotropy* vtkmpjLatticeAnisotropy::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjLatticeAnisotropy");
  if(ret)
    {
      return (vtkmpjLatticeAnisotropy*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjLatticeAnisotropy;
}

// Construct object with no children.
vtkmpjLatticeAnisotropy::vtkmpjLatticeAnisotropy()
{
  this->KernelSize[0] = 3;
  this->KernelSize[1] = 3;
  this->KernelSize[2] = 3;
  this->Mask = NULL;
  this->Eigenvalues = NULL;
  this->MaskOutValue = 0;
  this->Method=VTK_MPJ_LATTICEANISO_BASSER;
}

// ----------------------------------------------------------------------------
vtkmpjLatticeAnisotropy::~vtkmpjLatticeAnisotropy()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
  if (this->Eigenvalues != NULL)
    this->Eigenvalues->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjLatticeAnisotropy::ExecuteInformation()
{
  vtkImageData *eigvec=this->GetEigenvectors();
  vtkImageData *eigval=this->GetEigenvalues();
  vtkImageData *output=this->GetOutput();
  
  if (eigvec==NULL || eigval==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  int dim[3];  eigvec->GetDimensions(dim);
  int ncv=eigvec->GetNumberOfScalarComponents();
  int ncd=eigval->GetNumberOfScalarComponents();
  
  if (ncv < 9 || ncd < 3) {
    vtkErrorMacro(<< "Three eigenvectors and three eigenvalues are required as input!");
    return;
  }

  if (this->Method == VTK_MPJ_LATTICEANISO_BOTH)
    output->SetNumberOfScalarComponents(2);
  else output->SetNumberOfScalarComponents(1);  

  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

float vtkmpjLatticeAnisotropy::TensorProduct(float ru[3], float nu[3], float rv[3][3], float nv[3][3])
{
  float tp=0.0;
  for(int s=0; s<3; s++)
    for(int t=0; t<3; t++) {
      float dp = rv[0][s]*nv[0][t] + rv[1][s]*nv[1][t] + rv[2][s]*nv[2][t];
      tp = tp + ru[s]*nu[t] * dp*dp;
    }
  return tp;
}

// ----------------------------------------------------------------------------
void vtkmpjLatticeAnisotropy::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  vtkImageData *eigvec=input;
  vtkImageData *eigval=this->GetEigenvalues();
  vtkDataArray* invec =eigvec->GetPointData()->GetScalars();
  vtkDataArray *inval =eigval->GetPointData()->GetScalars();
 
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* mask =NULL;
  
  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
  
  int mink = this->KernelSize[2] / 2;
  int minj = this->KernelSize[1] / 2;
  int mini = this->KernelSize[0] / 2;
  int total_ij = dim[0]*dim[1];
  int index = 0;

  /*  printf("mini=%d,minj=%d,mink=%d\n",mini,minj,mink);*/
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  if ((mask != NULL)?(mask->GetComponent(index, 0) > 0):1)
	    {
	      float rv[3][3];  /* reference eigenvectors */
	      rv[0][0] = invec->GetComponent(index, 0);
	      rv[1][0] = invec->GetComponent(index, 1);
	      rv[2][0] = invec->GetComponent(index, 2);		      	      
	      rv[0][1] = invec->GetComponent(index, 3);
	      rv[1][1] = invec->GetComponent(index, 4);
	      rv[2][1] = invec->GetComponent(index, 5);	      
	      rv[0][2] = invec->GetComponent(index, 6);
	      rv[1][2] = invec->GetComponent(index, 7);
	      rv[2][2] = invec->GetComponent(index, 8);
	      
	      float ru[3];   /* reference eigenvalues */
	      ru[0]=inval->GetComponent(index, 0);
	      ru[1]=inval->GetComponent(index, 1);
	      ru[2]=inval->GetComponent(index, 2);

	      float tracer=ru[0]+ru[1]+ru[2];
	      float tpr = TensorProduct(ru, ru, rv, rv);
	      int count = 0;
	      float sumLI=0;
	      float sumAdd=0.0;
	      float sumdist=0;
	      
	      /* calculate neghborhood coherence */
	      for (int nk=-mink; nk<=mink; nk++) {
		int total_k = (k + nk) * total_ij;
		
		for (int nj=-minj; nj<=minj; nj++) {
		  int total_jk = total_k + (j + nj)*dim[0];
		  
		  for (int ni=-mini; ni<=mini; ni++) {

		    /* skip center voxel */
		    if (nk==0 && nj==0 && ni==0) continue;
		    
		    /* determine neighbor's address in image array */
		    int nindex = total_jk + (i + ni);
		    
		    /* handle boundaries */
		    if ((k+nk)>=0 && (j+nj)>=0 && (i+ni)>=0 &&
			(k+nk)<dim[2] && (j+nj)<dim[1] && (i+ni)<dim[0]) {
		      
		      /* if neighbor is in mask */
		      if ((mask != NULL)?(mask->GetComponent(nindex, 0) > 0):1) {
					
			float dist = 1.0 / sqrt(float(nk*nk + nj*nj + ni*ni));
			//			fprintf(stderr,"dist=%f\n",dist);

			float nv[3][3];  /* neighbor eigenvectors */
			
			nv[0][0] = invec->GetComponent(nindex, 0);
			nv[1][0] = invec->GetComponent(nindex, 1);
			nv[2][0] = invec->GetComponent(nindex, 2);		      
			
			nv[0][1] = invec->GetComponent(nindex, 3);
			nv[1][1] = invec->GetComponent(nindex, 4);
			nv[2][1] = invec->GetComponent(nindex, 5);

			nv[0][2] = invec->GetComponent(nindex, 6);
			nv[1][2] = invec->GetComponent(nindex, 7);
			nv[2][2] = invec->GetComponent(nindex, 8);
			
			float nu[3];    /* neighbor eigenvalues */
			nu[0] = inval->GetComponent(nindex, 0);
			nu[1] = inval->GetComponent(nindex, 1);
			nu[2] = inval->GetComponent(nindex, 2);
			
			float tracen = nu[0]+nu[1]+nu[2];
			float tpn = TensorProduct(nu, nu, nv, nv);
			float tp = TensorProduct(ru, nu, rv, nv);
			float atp = fabs(tp - (1.0/3.0)*tracer*tracen);  /* aniso tensor product */
			float a = sqrt(atp) / sqrt(tp);
			float b = atp / (sqrt(tpr)*sqrt(tpn));
			float LI = sqrt(3.0/8.0)*a + 3.0/4.0*b;
			float Add = atp / tp;

			//printf("atp=%f, tp=%f, tpn=%f, tpr=%f, a=%f, b=%f\n",atp,tp,tpn,tpr,a,b);
			sumLI = sumLI + dist*LI;
			sumAdd = sumAdd + dist*Add;

			sumdist = sumdist + dist;
 
			++count;
		      } // within boundaries
		    } // if in mask
		  } //for ni
		} // for nj
	      } // for nk
	      	      
	      if ( count > 0 ) {
		switch(this->Method) {
		case VTK_MPJ_LATTICEANISO_BASSER:
		  out->SetComponent(index, 0, sumLI/sumdist); 
		  break;
		case VTK_MPJ_LATTICEANISO_SKARE:
		  out->SetComponent(index, 0, sumAdd/sumdist);
		  break;
		default:
		  out->SetComponent(index, 0, sumLI/sumdist); 
		  out->SetComponent(index, 1, sumAdd/sumdist); 
		}
	      } else {
		out->SetComponent(index, 0, this->MaskOutValue);
	      }
	      
	    } else {
	      out->SetComponent(index, 0, this->MaskOutValue);
	    }
	  
	  ++index;
	 
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
  }
  
  this->UpdateProgress(1.0);

}
// ----------------------------------------------------------------------------
