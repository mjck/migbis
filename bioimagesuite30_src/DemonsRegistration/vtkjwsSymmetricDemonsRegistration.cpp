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
  Module:    $RCSfile: vtkjwsDemonsRegistration.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papd@noodle.med.yale.edu
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
#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "vtkjwsSymmetricDemonsRegistration.h"
#include "vtkImageCast.h"
#include "itkImage.h"
#include "vtkIntArray.h"
#include "vtkImageReslice.h"
#include "vtkImageAppend.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "vtkITKUtility.h"
#include "vtkjwsSignedDanielssonDistanceMapImageFilter.h"
#include <string.h>
#include "wook.h"
#include "OtherFunctions.h"
using namespace jws_demons;
#include "firstExtendedSymmetricDemon.h"
#include "secondExtendedSymmetricDemon.h"
#include "thirdSymmetricDemon.h"

//---------------------------------------------------------------------------
// First a Helper Class
// -----------------------------------------------------------------------------
vtkjwsSymmetricDemonsRegistration* vtkjwsSymmetricDemonsRegistration::New()
{
  return new vtkjwsSymmetricDemonsRegistration;
}



vtkjwsSymmetricDemonsRegistration::vtkjwsSymmetricDemonsRegistration()
{
  //ldsa
  this->ReferenceDistanceMap=NULL;
  this->TargetDistanceMap=NULL;
  this->GridTransform=NULL;
}
// ----------------------------------------------------------------------------
vtkjwsSymmetricDemonsRegistration::~vtkjwsSymmetricDemonsRegistration()
{
  this->SetReferenceDistanceMap(NULL);
  this->SetTargetDistanceMap(NULL);
  if (this->GridTransform!=NULL)
    this->GridTransform->Delete();
}
// ----------------------------------------------------------------------------

//  half_mode = 1 if using Second Demons where images 2 x size of the displacement field
//  half_mode = 0 if image size = displacement field size

vtkImageData* vtkjwsSymmetricDemonsRegistration_ConvertTransformation(vtkImageData* ReferenceDistanceMap,int half_mode,
							     DeformationFieldType::Pointer DField)
{
  int dim[3]; ReferenceDistanceMap->GetDimensions(dim);
  if (half_mode)
    dim[0]=dim[0]/2;

  vtkImageData* dispfield=vtkImageData::New();
  dispfield->SetOrigin( ReferenceDistanceMap->GetOrigin()  );
  dispfield->SetSpacing( ReferenceDistanceMap->GetSpacing()  );
  dispfield->SetDimensions(dim);
  dispfield->SetNumberOfScalarComponents(3);
  dispfield->SetScalarTypeToDouble();
  dispfield->AllocateScalars();

  DeformationFieldType::IndexType pixelIndex;


  for (int k=0;k<dim[2];k++)
    {
      pixelIndex[2]=k;
      for (int j=0;j<dim[1];j++)
	{
	  pixelIndex[1]=j;
	  for (int i=0;i<dim[0];i++)
	    {
	      pixelIndex[0]=i;
	      VectorPixelType v= DField->GetPixel(pixelIndex);
	      dispfield->SetScalarComponentFromDouble(i,j,k,0,v[0]);
	      dispfield->SetScalarComponentFromDouble(i,j,k,1,v[1]);
	      dispfield->SetScalarComponentFromDouble(i,j,k,2,v[2]);
	    }
	}
    }

  return dispfield;
}


// ----------------------------------------------------------------------------
int vtkjwsSymmetricDemonsRegistration::RunExtendedDemons(int Mode)
{
  if (Mode!=2)
    Mode=1;


  if (this->ReferenceDistanceMap==NULL || this->TargetDistanceMap==NULL)
    {
      vtkErrorMacro(<<"NULL Inputs to vtkjwsSymmetricDemonsRegistration");
      return 0;
    }

  
  // Resize Images to have equal and appropriate size
  int newdimensions[3]; 
  this->ReferenceDistanceMap->GetDimensions(newdimensions);

  // Step 1 is take vtkImageData and convert to ITK
  vtkImageCast* cast_ref=vtkImageCast::New();
  cast_ref->SetInput(this->ReferenceDistanceMap);
  cast_ref->SetOutputScalarTypeToFloat();

  vtkImageCast* cast_targ=vtkImageCast::New();
  cast_targ->SetInput(this->TargetDistanceMap);
  cast_targ->SetOutputScalarTypeToFloat();
  

  // Create the appropriate input class (itk Image is OutputImageType which is 
  // unsigned short -- see wook.h 
  typedef itk::VTKImageImport<FixedImageType> ImportType;
  
  // Reference
  vtkImageExport *vtkInputExporterRef = vtkImageExport::New();
  vtkInputExporterRef->SetInput(cast_ref->GetOutput());
  ImportType::Pointer itkInputImporterRef =  ImportType::New();
  ConnectPipelines(vtkInputExporterRef, itkInputImporterRef);
  itkInputImporterRef->Update();

  vtkImageExport *vtkInputExporterTarg = vtkImageExport::New();
  vtkInputExporterTarg->SetInput(cast_targ->GetOutput());
  ImportType::Pointer itkInputImporterTarg =  ImportType::New();
  ConnectPipelines(vtkInputExporterTarg, itkInputImporterTarg);
  itkInputImporterTarg->Update();
  
  // Step 2 is run ITK Algorithm

  // 2.a  Create Same Types etc.
  OutputImageType::RegionType region; 
  InputImageType::RegionType::IndexType indexStart;  
  InputImageType::RegionType::SizeType  ImageSize;
  ImageSize[0]=newdimensions[0];
  ImageSize[1]=newdimensions[1];
  ImageSize[2]=newdimensions[2];
  
  if (Mode==2) ImageSize[0]=newdimensions[0] / 2;
  
  indexStart[0]=0;   indexStart[1]=0;  indexStart[2]=0;
  region.SetSize( ImageSize );
  region.SetIndex(indexStart ); 	
  cout<< "size of DField="<< region.GetSize()<<endl;
   
  // 2c Run Registration

  DeformationFieldType::Pointer DField = DeformationFieldType::New();     
  DField->SetRegions( region  );
  DField->SetOrigin(  itkInputImporterRef->GetOutput()->GetOrigin()  );
  DField->SetSpacing( itkInputImporterRef->GetOutput()->GetSpacing() );
  DField->Allocate();


  if (this->GridTransform!=NULL)
    this->GridTransform->Delete();
  this->GridTransform=vtkpxGridTransform::New();
  this->GridTransform->SetInterpolationModeToLinear();
  
  vtkImageData* dispfield=NULL;

  if (Mode==1)
    {
      string RegistrationResult = "FirstRegResultHalf";
      FIRST_SEDEMON::firstExtendedDemonRegistration(itkInputImporterRef->GetOutput(), 
						   itkInputImporterTarg->GetOutput(),
						   DField, RegistrationResult);	
      dispfield=vtkjwsSymmetricDemonsRegistration_ConvertTransformation(ReferenceDistanceMap,
							       0,DField);
    } 
  else if (Mode==2)
    {
      string RegistrationResult = "SecondRegResultDouble";
      SECOND_SEDEMON::ExtendedDemonRegistrationMain(itkInputImporterRef->GetOutput(), 
						   itkInputImporterTarg->GetOutput(),
						   DField, RegistrationResult);	
      dispfield=vtkjwsSymmetricDemonsRegistration_ConvertTransformation(ReferenceDistanceMap,
							       1,DField);

    }
  
  // Step 3:
  // Push it out to VTK
  this->GridTransform->SetDisplacementGrid(dispfield);
  dispfield->Delete();

  cast_ref->Delete();
  cast_targ->Delete();
  vtkInputExporterRef->Delete();
  vtkInputExporterTarg->Delete();

  return 1;
}

// ---------------------------------------------------------------------------------------------

int vtkjwsSymmetricDemonsRegistration::RunNormalDemons()
{


  if (this->ReferenceDistanceMap==NULL || this->TargetDistanceMap==NULL)
    {
      vtkErrorMacro(<<"NULL Inputs to vtkjwsSymmetricDemonsRegistration");
      return 0;
    }

  
  // Resize Images to have equal and appropriate size
  int newdimensions[3]; 
  this->ReferenceDistanceMap->GetDimensions(newdimensions);

  // Step 1 is take vtkImageData and convert to ITK
  vtkImageCast* cast_ref=vtkImageCast::New();
  cast_ref->SetInput(this->ReferenceDistanceMap);
  cast_ref->SetOutputScalarTypeToShort();

  vtkImageCast* cast_targ=vtkImageCast::New();
  cast_targ->SetInput(this->TargetDistanceMap);
  cast_targ->SetOutputScalarTypeToShort();
  

  // Create the appropriate input class (itk Image is OutputImageType which is 
  // unsigned short -- see wook.h 
  typedef itk::VTKImageImport<GrayImageType> ImportType;
  
  // Reference
  vtkImageExport *vtkInputExporterRef = vtkImageExport::New();
  vtkInputExporterRef->SetInput(cast_ref->GetOutput());
  ImportType::Pointer itkInputImporterRef =  ImportType::New();
  ConnectPipelines(vtkInputExporterRef, itkInputImporterRef);
  itkInputImporterRef->Update();

  vtkImageExport *vtkInputExporterTarg = vtkImageExport::New();
  vtkInputExporterTarg->SetInput(cast_targ->GetOutput());
  ImportType::Pointer itkInputImporterTarg =  ImportType::New();
  ConnectPipelines(vtkInputExporterTarg, itkInputImporterTarg);
  itkInputImporterTarg->Update();
  
  // Step 2 is run ITK Algorithm

  OutputImageType::RegionType region; 
  InputImageType::RegionType::IndexType indexStart;  
  InputImageType::RegionType::SizeType  ImageSize;
  ImageSize[0]=newdimensions[0];
  ImageSize[1]=newdimensions[1];
  ImageSize[2]=newdimensions[2];
  indexStart[0]=0;   indexStart[1]=0;  indexStart[2]=0;
  region.SetSize( ImageSize );
  region.SetIndex(indexStart ); 	

  DeformationFieldType::Pointer DField = DeformationFieldType::New();     
  DField->SetRegions( region  );
  DField->SetOrigin(  itkInputImporterRef->GetOutput()->GetOrigin()  );
  DField->SetSpacing( itkInputImporterRef->GetOutput()->GetSpacing() );
  DField->Allocate();


  if (this->GridTransform!=NULL)
    this->GridTransform->Delete();
  this->GridTransform=vtkpxGridTransform::New();
  this->GridTransform->SetInterpolationModeToLinear();
  

  // Fixed Third Demons to Return displacement field

  THIRD_SEDEMON::DemonRegistrationMain(itkInputImporterRef->GetOutput(), 
				      itkInputImporterTarg->GetOutput(),
				      DField, "");     
  vtkImageData* dispfield=
    vtkjwsSymmetricDemonsRegistration_ConvertTransformation(ReferenceDistanceMap,0,DField);
  
  // Step 3:
  // Push it out to VTK
  this->GridTransform->SetDisplacementGrid(dispfield);
  dispfield->Delete();

  cast_ref->Delete();
  cast_targ->Delete();
  vtkInputExporterRef->Delete();
  vtkInputExporterTarg->Delete();

  return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------------
vtkIntArray* vtkjwsSymmetricDemonsRegistration::DecideImageSize(vtkImageData* ref,vtkImageData* targ)
{
  int d[3];
  int ok=DecideImageSize(ref,targ,d);
  if (ok==0)
    return NULL;

  vtkIntArray* arr=vtkIntArray::New();
  arr->SetNumberOfComponents(3);
  arr->SetNumberOfTuples(1);
  arr->SetComponent(0,0,d[0]);
  arr->SetComponent(0,1,d[1]);
  arr->SetComponent(0,2,d[2]);
  
  return arr;
}

// --------------------------------------------------------------------------------------------------------------------------------------

int vtkjwsSymmetricDemonsRegistration::DecideImageSize(vtkImageData* ref,vtkImageData* targ,int outdim[3])
{
  if (ref==NULL || targ==NULL)
    return 0;

  int Image1size[3]; ref->GetDimensions(Image1size);
  int Image2size[3]; targ->GetDimensions(Image2size);
  
  int sizeX1, sizeY1, sizeZ1;
  int sizeX2, sizeY2, sizeZ2;
  sizeX1 =Image1size[0];     sizeY1 = Image1size[1];    sizeZ1 = Image1size[2];
  sizeX2 =Image2size[0];     sizeY2 = Image2size[1];    sizeZ2 = Image2size[2];
  cout<<"Input Fixed image sizeX="<< sizeX1<<" sizeY="<<sizeY1<<" sizeZ="<<sizeZ1<<endl;
  cout<<"Input Moving image sizeX="<< sizeX2<<" sizeY="<<sizeY2 <<" sizeZ="<<sizeZ2<<endl;
  
  int tempX, tempY, tempZ;
  
  InputImageType::RegionType::SizeType InternalImageSize;
  if(sizeX1 >= sizeX2) tempX = sizeX1;
  else tempX = sizeX2;
  
  if(sizeY1 >= sizeY2) tempY = sizeY1;
  else tempY = sizeY2;
  
  if(sizeZ1 >= sizeZ2) tempZ = sizeZ1;
  else tempZ = sizeZ2;
  
  int adjustX, adjustY, adjustZ;
  
  //adjustX =  ((tempX+2)>>2)<<2;	adjustY=  ((tempY+2)>>2)<<2;	adjustZ =  ((tempZ+2)>>2)<<2;
  adjustX =  ((tempX+3)>>2)<<2;	adjustY=  ((tempY+3)>>2)<<2;	adjustZ =  ((tempZ+3)>>2)<<2;
  cout<<"Adjusted image sizeX="<<adjustX<<" sizeY="<<adjustY <<" sizeZ="<<adjustZ<<endl;

  
  outdim[0] = adjustX;
  outdim[1] = adjustY;
  outdim[2] = adjustZ;
  
  return 1;
}

vtkImageData* vtkjwsSymmetricDemonsRegistration::PadImage(vtkImageData* inp,int x,int y, int z )
{
  int d[3];
  d[0]=x;
  d[1]=y;
  d[2]=z;
  return PadImage(inp,d);
}

vtkImageData* vtkjwsSymmetricDemonsRegistration::PadImage(vtkImageData* inp,int outdim[3])
{

  vtkImageReslice* pad=vtkImageReslice::New();
  pad->SetInput(inp);
  pad->SetOutputSpacing(inp->GetSpacing());
  pad->SetOutputOrigin(inp->GetOrigin());
  pad->SetInterpolationMode(0);
  pad->SetOutputExtent(0,outdim[0]-1,0,outdim[1]-1,0,outdim[2]-1);
  pad->Update();


  vtkImageData* out=vtkImageData::New();
  out->ShallowCopy(pad->GetOutput());
  pad->Delete();

  return out;
}



vtkImageData*  vtkjwsSymmetricDemonsRegistration::ComputeDistanceMap(vtkImageData* input,double lower_threshold,double upper_threshold)
{
  if (input==NULL)
    {
      fprintf(stderr,"Bad Input to ComputeDistanceMap\n");
      return NULL;
    }


  // This is in the Levelset directory
  vtkjwsSignedDanielssonDistanceMapImageFilter* filt=vtkjwsSignedDanielssonDistanceMapImageFilter::New();
  filt->SetLowerThreshold(lower_threshold);
  filt->SetUpperThreshold(upper_threshold);
  filt->SetInValue(3056);
  filt->SetOutValue(0);
  filt->SetRadius(1);
  filt->SetInput(input);
  filt->Update();

  vtkImageData* dmap=vtkImageData::New();
  dmap->ShallowCopy(filt->GetOutput());
  filt->Delete();
  return dmap;
}


vtkImageData*  vtkjwsSymmetricDemonsRegistration::MergeDistanceMaps(vtkImageData* dmap1,vtkImageData* dmap2)
{
  if (dmap1==NULL || dmap2 == NULL)
    {
      fprintf(stderr,"Bad Input to Merge Distance Maps\n");
      return NULL;
    }


  vtkImageAppend* filt=vtkImageAppend::New();
  filt->AddInput(dmap1);
  filt->AddInput(dmap2);
  filt->SetAppendAxis(0);
  filt->Update();

  vtkImageData* dmap=vtkImageData::New();
  dmap->ShallowCopy(filt->GetOutput());
  filt->Delete();
  return dmap;
}

