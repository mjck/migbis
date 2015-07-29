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


#include "vtkObjectFactory.h"
#include "vtkSource.h"
#include "vtkxqImageHessian.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageGradient.h"
#include "vtkImageExtractComponents.h"

//#include "vtkpxAppendComponents.h"
#include "vtkImageShiftScale.h"
#include "vtkImageMathematics.h"
#include "vtkImageCast.h"
#include "vtkMath.h"
#include "vtkImageReslice.h"
#include "vtkImageFFT.h"
#include "vtkImageRFFT.h"
#include "bioimagesuite_settings.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"


//------------------------------------------------------------------------------
vtkxqImageHessian* vtkxqImageHessian::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkxqImageHessian");
  if(ret)
    {
      return (vtkxqImageHessian*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkxqImageHessian;
}

// Construct object with no children.
vtkxqImageHessian::vtkxqImageHessian()
{
  this->Sigma=1.0;
  this->Mode=0;
}

// -------------------------------------------------------------------------------------------------------------------
int vtkxqImageHessian::RequestInformation(vtkInformation *vtkNotUsed(request),  vtkInformationVector **inputVector, 
						     vtkInformationVector *outputVector)

{  
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, 6);
  return 1; 
}
// -------------------------------------------------------------------------------------------------------------------
void vtkxqImageHessian::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs");
      return;
    }

  if (input->GetNumberOfScalarComponents()!=1)
    {
      vtkErrorMacro(<<"Only Single Component Images are valid inputs\n");
      return;
    }

  double sp[3]; input->GetSpacing(sp);
  double sig[3];
  for (int ia=0;ia<=2;ia++)
    sig[ia]=this->Sigma*sp[0]/sp[ia];      
  this->SetFilterBankToHessian(sig);
  this->UpdateProgress(0.05);


  this->vtkxqImageFourierConvolution::SimpleExecute(input,output);
}


