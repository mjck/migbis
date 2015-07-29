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





/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxImageSincInterpolation.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:02:52 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkObjectFactory.h"
#include "vtkSource.h"
#include "vtkpxImageSincInterpolation.h"

#include <vtkDataArray.h>
#include <vtkImageData.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageFFT.h>
#include <vtkImageFourierCenter.h>
#include <vtkImageRFFT.h>
#include <vtkImageConstantPad.h>
#include <vtkImageMagnitude.h>
#include <vtkImageShiftScale.h>
//------------------------------------------------------------------------------
vtkpxImageSincInterpolation* vtkpxImageSincInterpolation::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageSincInterpolation");
  if(ret)
    {
    return (vtkpxImageSincInterpolation*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageSincInterpolation;
}

// Construct object with no children.
vtkpxImageSincInterpolation::vtkpxImageSincInterpolation()
{
  this->Factor=1;
}
// ----------------------------------------------------------------------------
void vtkpxImageSincInterpolation::ExecuteInformation()
{
  vtkImageData *input=this->GetImageDataInput(0);

  if (input==NULL)
      {
	vtkErrorMacro(<<"No Input Image !!");
	return;
      }

  vtkImageData *output=this->GetOutput();

  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  int wholeExtent[6];
  input->GetWholeExtent( wholeExtent );

  int dims[3];

  dims[0] =  (wholeExtent[1] - wholeExtent[0] );
  dims[1] =  (wholeExtent[3] - wholeExtent[2] );
  dims[2] =  (wholeExtent[5] - wholeExtent[4] );

  for (int ia=0;ia<=2;ia++)
    {
      int a=int(log(float(dims[ia]))/log(2.0)+0.99);
      int d=int(pow(2.0,a)*this->Factor);
      //int d=dims[ia]*this->Factor;
      dims[ia]=d;
    }

  wholeExtent[0] = 0;
  wholeExtent[1] = dims[0] - 1;
  wholeExtent[2] = 0;
  wholeExtent[3] = dims[1] - 1;
  wholeExtent[4] = 0;
  wholeExtent[5] = dims[2] - 1;
  output->SetWholeExtent( wholeExtent );
}

// ----------------------------------------------------------------------------

void vtkpxImageSincInterpolation::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  
  // Create Output Array
  // -------------------
  int   dim[3]; input->GetDimensions(dim);
  int   pdim[3];
  int   newdim[3],newextent[6];
  double sp[3];  input->GetSpacing(sp);
  double ori[3];  input->GetOrigin(ori);
  
  for (int ia=0;ia<=2;ia++)
    {
      int a=int(log(float(dim[ia]))/log(2.0)+0.99);
      pdim[ia]=int(pow(2.0,a));
      //pdim[ia]=dim[ia];
      newdim[ia]=int(pdim[ia]*this->Factor);
      newextent[2*ia]=(pdim[ia]-newdim[ia])/2;
      newextent[2*ia+1]=(pdim[ia]+newdim[ia])/2-1;
      sp[ia]=float(dim[ia])/float(newdim[ia])*sp[ia];
    }

  output->SetSpacing(sp);
  output->SetOrigin(input->GetOrigin());
  output->SetScalarType(input->GetScalarType());
  output->SetNumberOfScalarComponents(input->GetNumberOfScalarComponents());
  output->SetDimensions(newdim[0],newdim[1],newdim[2]);
  output->SetWholeExtent(0,newdim[0]-1,0,newdim[1]-1,0,newdim[2]-1);
  output->SetUpdateExtent(0,newdim[0]-1,0,newdim[1]-1,0,newdim[2]-1);
  output->AllocateScalars();

  float progress=0.0;
  float dprog=1.0/float(output->GetNumberOfScalarComponents());

  vtkDataArray* out_array=output->GetPointData()->GetScalars();

  this->UpdateProgress(0.0);

  for (int comp=0;comp<output->GetNumberOfScalarComponents();comp++)
    {
      vtkImageExtractComponents* compExtract=vtkImageExtractComponents::New();
      compExtract->SetInput(input);
      compExtract->SetComponents(comp);
      compExtract->Update();

      vtkImageShiftScale* sc=vtkImageShiftScale::New();
      sc->SetInput(compExtract->GetOutput());
      compExtract->Delete();
      sc->SetOutputScalarTypeToFloat();
      sc->SetShift(0.0);
      sc->SetScale(this->Factor*this->Factor*this->Factor);
      sc->Update();

      vtkImageFFT* fft=vtkImageFFT::New();
      fft->SetInput(sc->GetOutput());
      sc->Delete();
      fft->SetDimensionality(3);
      fft->Update();

      vtkImageFourierCenter* cent=vtkImageFourierCenter::New();
      cent->SetInput(fft->GetOutput());
      fft->Delete();

      vtkImageConstantPad* padFilter=vtkImageConstantPad::New();
      padFilter->SetInput(cent->GetOutput());
      cent->Delete();
      padFilter->SetConstant(0);
      padFilter->SetOutputWholeExtent(newextent);
      padFilter->Update();
      
      vtkImageRFFT* rfft=vtkImageRFFT::New();
      rfft->SetInput(padFilter->GetOutput());
      padFilter->Delete();
      rfft->SetDimensionality(3);
      rfft->Update();

      vtkImageMagnitude* magn=vtkImageMagnitude::New();
      magn->SetInput(rfft->GetOutput());
      rfft->Delete();
      magn->Update();

      vtkDataArray* in_array=magn->GetOutput()->GetPointData()->GetScalars();
      out_array->CopyComponent(comp,in_array,0);

      progress+=dprog;
      this->UpdateProgress(progress);
      
      magn->Delete();
    }
}



