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




/*=========================================================================*/
#include "vtkpxMidlineSymmetricRegistration.h"
#include "vtkpxRegistration.h"
#include "vtkbisImageReslice.h"
#include "vtkImageFlip.h"
#include "vtkpxUtil.h"
#include "vtkObjectFactory.h"


vtkpxMidlineSymmetricRegistration* vtkpxMidlineSymmetricRegistration::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxMidlineSymmetricRegistration");
  if(ret)
    {
      return (vtkpxMidlineSymmetricRegistration*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxMidlineSymmetricRegistration;
}

vtkpxMidlineSymmetricRegistration::vtkpxMidlineSymmetricRegistration() 
{
  this->MidlineSymmetricMode=1;
}

int vtkpxMidlineSymmetricRegistration::InitializeAll() 
{
  this->TranslationY=0;
  this->TranslationZ=0;
  this->RotationX=0;
  return vtkpxLinearRegistration::InitializeAll();
}

double vtkpxMidlineSymmetricRegistration::Evaluate()
{
  this->LastSmoothness=0.0;
  static int count=0;
  vtkImageData* ref=this->SampledReferenceImage;
  vtkImageData* targ=this->SampledTransformImage;
  vtkTransform* trans=this->LinearTransform;
  vtkpxJointHistogram* histo=this->Histogram;
  int similaritymeasure=this->SimilarityMeasure;
  int interp=this->InterpolationMode;
  int reset=1;

  // The magic is here ... create a composite image and add it up ...
  // ----------------------------------------------------------------

  if (ref==NULL || targ==NULL || trans==NULL || histo==NULL)
    return 0.0;

  if (interp<0)
    interp=-interp;

  if (interp<0 || interp>1)
    interp=3;
  
  vtkbisImageReslice* resl=vtkbisImageReslice::New(); resl->OptimizationOff();
  resl->SetInput(targ);
  resl->SetInformationInput(ref);
  resl->SetResliceTransform(trans);
  resl->SetInterpolationMode(interp);
  resl->Update();
  
  vtkImageFlip* flip=vtkImageFlip::New();
  flip->SetInput(resl->GetOutput());
  flip->SetFilteredAxis(0);
  flip->PreserveImageExtentOn();
  flip->Update();

  /*  if (count==1)
    {
      vtkpxUtil::SaveAnalyze("flip.nii.gz",flip->GetOutput(),9);
      vtkpxUtil::SaveAnalyze("resl.nii.gz",resl->GetOutput(),9);
    }
    ++count;*/


  histo->FillHistogram(flip->GetOutput(),resl->GetOutput(),reset);
  float val=histo->Similarity(similaritymeasure);
  
  resl->Delete();
  this->LastSimilarity=val;
  return this->LastSimilarity;

}

