

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


#include "vtkmpjLevelSetSignedDistance.h"

//------------------------------------------------------------------------------
vtkmpjLevelSetSignedDistance* vtkmpjLevelSetSignedDistance::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjLevelSetSignedDistance");
  if (ret) {
    return (vtkmpjLevelSetSignedDistance*)ret;
  }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjLevelSetSignedDistance;
}

// Construct object with no children.
vtkmpjLevelSetSignedDistance::vtkmpjLevelSetSignedDistance()
{
  this->LevelSetValue = 0.0;
  this->PropagationTime = 0;
  this->ExtensionImage = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjLevelSetSignedDistance::~vtkmpjLevelSetSignedDistance()
{
  if (this->ExtensionImage != NULL)
    this->ExtensionImage->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetSignedDistance::ExecuteInformation()
{
  vtkImageData *output = this->GetOutput();
  vtkImageData *input = this->GetInput();
  
  int dim[3]; input->GetDimensions(dim);

  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  int ext_nc = 0;

  if (this->ExtensionImage != NULL) {
    ext_nc = this->ExtensionImage->GetNumberOfScalarComponents();
  }
  
  output->SetNumberOfScalarComponents(ext_nc+1);  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetSignedDistance::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  //vtkDataArray *in = input->GetPointData()->GetScalars();
  //vtkDataArray *out = output->GetPointData()->GetScalars();
  vtkDataArray *ext = NULL;
  
  if (this->ExtensionImage != NULL) {
    ext = this->ExtensionImage->GetPointData()->GetScalars();
  }
  
  this->UpdateProgress(0.01);

  // step 1: extract level set 
  vtkmpjExtractLevelSet *xls = vtkmpjExtractLevelSet::New();
  xls->SetInput(input);
  xls->SetLevelSetValue(0.0);
  xls->Update();
  
  this->UpdateProgress(0.125);

  // step 2: get inner and outer level set mask 
  vtkImageThreshold *thresh1 = vtkImageThreshold::New();
  thresh1->SetInput(xls->GetOutput());
  thresh1->SetOutputScalarTypeToFloat();
  thresh1->ThresholdByLower(-1.0);
  thresh1->SetInValue(1.0);
  thresh1->SetOutValue(0.0);
  thresh1->Update();

  // step 3: march inwards and outwards
  vtkmpjFastMarchingExtension *fmm1;
  //  if (ext == NULL) {
  //fmm1 = vtkmpjFastMarchingMethod::New();
  //} else {
  fmm1 = vtkmpjFastMarchingExtension::New();
  fmm1->SetExtensionImage(this->ExtensionImage);
  //}
  fmm1->SetInput(thresh1->GetOutput());
  fmm1->SetKnownPoints(xls->GetInsidePoints());
  fmm1->Update();
  
  thresh1->Delete();

  this->UpdateProgress(0.25);

  vtkImageThreshold *thresh2 = vtkImageThreshold::New();
  thresh2->SetInput(xls->GetOutput());
  thresh2->SetOutputScalarTypeToFloat();
  thresh2->ThresholdByUpper(1.0);
  thresh2->SetInValue(1.0);
  thresh2->SetOutValue(0.0);
  thresh2->Update();

  vtkmpjFastMarchingExtension *fmm2;
  //  if (ext == NULL) {
  //  fmm2 = vtkmpjFastMarchingMethod::New();
  //} else {
  fmm2 = vtkmpjFastMarchingExtension::New();
  fmm2->SetExtensionImage(this->ExtensionImage);
  //}
  fmm2->SetInput(thresh2->GetOutput());
  fmm2->SetKnownPoints(xls->GetOutsidePoints());
  fmm2->Update();
  
  thresh2->Delete();
  
  xls->Delete();

  this->UpdateProgress(0.375);

  // step 4: threshold marching result to get rid of infinity values
  vtkImageThreshold *thresh3 = vtkImageThreshold::New();
  thresh3->SetInput(fmm1->GetOutput());
  thresh3->SetOutputScalarTypeToFloat();
  thresh3->ThresholdByLower(1000);
  thresh3->SetOutValue(0);
  thresh3->Update();
  
  fmm1->Delete();

  vtkImageThreshold *thresh4 = vtkImageThreshold::New();
  thresh4->SetInput(fmm2->GetOutput());
  thresh4->SetOutputScalarTypeToFloat();
  thresh4->ThresholdByLower(1000);
  thresh4->SetOutValue(0);
  thresh4->Update();

  fmm2->Delete();

  this->UpdateProgress(0.50);

  // step 5: combine inner and outer propagation results
  vtkmpjFrameExtract *frame0 = vtkmpjFrameExtract::New();
  frame0->SetInput(thresh3->GetOutput());
  frame0->SetStartFrame(0);
  frame0->SetEndFrame(0);
  frame0->Update();

  thresh3->Delete();

  this->UpdateProgress(0.625);

  vtkImageMathematics *math1 = vtkImageMathematics::New();
  math1->SetInput1(frame0->GetOutput());
  math1->SetOperationToMultiplyByK();
  math1->SetConstantK(-1.0);
  math1->Update();

  frame0->Delete();

  this->UpdateProgress(0.75);

  vtkmpjFrameExtract *frame1 = vtkmpjFrameExtract::New();
  frame1->SetInput(thresh3->GetOutput());
  frame1->SetStartFrame(1);
  frame1->SetEndFrame(1);
  frame1->Update();

  vtkImageAppendComponents *appnd = vtkImageAppendComponents::New();
  appnd->AddInput(math1->GetOutput());
  appnd->AddInput(frame1->GetOutput());
  appnd->Update();

  math1->Delete();
  frame1->Delete();

  this->UpdateProgress(0.875);

  vtkImageMathematics *math2 = vtkImageMathematics::New();
  math2->SetInput1(appnd->GetOutput());
  math2->SetInput2(thresh4->GetOutput());
  math2->SetOperationToAdd();
  math2->Update();

  appnd->Delete();
  thresh4->Delete();

  // step 5: copy output
  output->ShallowCopy(math2->GetOutput());

  math2->Delete();

  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
