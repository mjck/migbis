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
  Module:    $RCSfile: vtkpxImageFeatureExtract.cpp,v $
  Language:  C++
  Date:      $Date: 2002/09/26 18:14:21 $
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

#include "vtkFloatArray.h"
#include "vtkImageExtractComponents.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageGradient.h"
#include "vtkImageMathematics.h"
#include "vtkImageNonMaximumSuppression.h"
#include "vtkImageShiftScale.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkpxImageFeatureExtract.h"
#include "vtkpxUtil.h"
#include "vtkDataArray.h"
vtkpxImageFeatureExtract* vtkpxImageFeatureExtract::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageFeatureExtract");
  if(ret)
      {
	return (vtkpxImageFeatureExtract*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageFeatureExtract;
}


// Construct object to extract all of the input data.
vtkpxImageFeatureExtract::vtkpxImageFeatureExtract()
{
  this->CornerThreshold=0.5;
  this->DetectionThreshold=1.0;
  this->Mode=0;
  this->NonMaximumSuppression=0;
  this->SmoothSigma=1.0;
  this->DataDimensions=3;
}


void vtkpxImageFeatureExtract::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkpxSimpleImageToImageFilter::PrintSelf(os,indent);
  os << indent << "  DataDimensions " << this->DataDimensions << "\n";
  os << indent << "  Mode " << this->Mode << "\n";
  os << indent << "  SmoothSigma " << this->SmoothSigma << "\n";
  os << indent << "  CornerThreshold " << this->CornerThreshold << "\n";
  os << indent << "  DetectionThreshold " << this->DetectionThreshold << "\n";
  os << indent << "  NonMaximumSuppression " << this->NonMaximumSuppression << "\n";
}



float vtkpxImageFeatureExtract::ComputeFeatureValue(float Cg[3][3])
{
  float detCg=0.0,traceCg=0.0;
  int ia;
  // Dim=2,3 
  switch (this->DataDimensions)
    {
    case 2:
      traceCg=Cg[0][0]+Cg[1][1];
      detCg=Cg[0][0]*Cg[1][1]-Cg[0][1]*Cg[1][0];
      break;
    case 3:
      traceCg=0.0;
      for (ia=0;ia<=2;ia++)
	traceCg+=Cg[ia][ia];
      detCg=vtkMath::Determinant3x3(Cg);
      break;
    }

  if (traceCg<this->DetectionThreshold)
    return 0.0;

  float Op=fabs(detCg/traceCg);

  if (this->Mode==0)
    return sqrt(traceCg);

  if (this->Mode==1)
    return sqrt(fabs(detCg));

  if (this->Mode==2)
    {
      if (Op<this->DetectionThreshold)
	return 0.0;
      else
	return Op;
    }

  float fdim=float(this->DataDimensions);
  float ifdim=1.0/fdim;
  float OpR=fabs(detCg/pow(ifdim*traceCg,fdim));

  if (OpR<CornerThreshold || Op<this->DetectionThreshold)
    return 0.0;
  else 
    return  Op;
  
}

void vtkpxImageFeatureExtract::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  vtkDebugMacro(<< "Extracting Features");

  // Step 1: Smooth Image 
  int ia,ib;
  float Cg[3][3];

  // Step 1: Smooth Image  
  vtkImageGaussianSmooth* presmooth=vtkImageGaussianSmooth::New();
  presmooth->SetInput(input);
  presmooth->SetDimensionality(this->DataDimensions);
  presmooth->SetStandardDeviations(this->SmoothSigma,this->SmoothSigma,this->SmoothSigma);
  presmooth->Update();
  vtkDebugMacro(<< "1. Presmooth Done");

  // Step 2: Calculate Gradients  
  vtkImageGradient* gradFilter=vtkImageGradient::New();
  gradFilter->SetInput(presmooth->GetOutput());
  gradFilter->SetDimensionality(this->DataDimensions);
  gradFilter->HandleBoundariesOn();
  gradFilter->Update();
  presmooth->Delete();

  vtkDebugMacro(<< "2. Gradients Done");

  // Step 3: Extract Components of Smoothed  Gradients  
  vtkImageExtractComponents* compFilter[3];
  for (ia=0;ia<this->DataDimensions;ia++)
    {
      compFilter[ia]=vtkImageExtractComponents::New();

      compFilter[ia]->SetInput(gradFilter->GetOutput());
      compFilter[ia]->SetComponents(ia);
      compFilter[ia]->Update();
      vtkDebugMacro(<< "3." << ia+1  << " Extrating Component ("<<ia+1<<") Done");
    }
  

  // Step 4: Compute 3x3 Matrix C
  vtkImageMathematics* compCg[3][3];

  for (ia=0;ia<this->DataDimensions;ia++)
    for (ib=ia;ib<this->DataDimensions;ib++)
      {
	compCg[ia][ib]=vtkImageMathematics::New();

	compCg[ia][ib]->SetOperationToMultiply();
	compCg[ia][ib]->SetInput1(compFilter[ia]->GetOutput());
	compCg[ia][ib]->SetInput2(compFilter[ib]->GetOutput());
	compCg[ia][ib]->Update();
	vtkDebugMacro(<< "4." << ia+1 <<"."<<ib+1<< " Computing Product ("<<ia+1<<"x"<<ib+1<<") Done");
      }
  for (ia=0;ia<this->DataDimensions;ia++)
    compFilter[ia]->Delete();

  vtkImageGaussianSmooth* matrixCg[3][3];
  for (ia=0;ia<this->DataDimensions;ia++)
    for (ib=ia;ib<this->DataDimensions;ib++)
      {
	matrixCg[ia][ib]=vtkImageGaussianSmooth::New();
	matrixCg[ia][ib]->SetInput(compCg[ia][ib]->GetOutput());
	matrixCg[ia][ib]->SetStandardDeviations(this->SmoothSigma,this->SmoothSigma,this->SmoothSigma);
	matrixCg[ia][ib]->SetDimensionality(this->DataDimensions);
	//	matrixCg[ia][ib]->BypassOn();
	matrixCg[ia][ib]->Update();
	vtkDebugMacro(<< "5." << ia+1 <<"."<<ib+1<< " Computing Smooth Product ("<<ia+1<<"x"<<ib+1<<") Done");
      }
 
  for (ia=0;ia<this->DataDimensions;ia++)
    for (ib=ia;ib<this->DataDimensions;ib++)
      compCg[ia][ib]->Delete();

  // Step 6: For Each voxel get appropriate components and store in matrix C 
  // Then compute Feature Values 

  // First Allocate Output 

  int dty=matrixCg[0][0]->GetOutput()->GetPointData()->GetScalars()->GetDataType();
  int dim[3];
  matrixCg[0][0]->GetOutput()->GetDimensions(dim);
  
  output->SetDimensions(matrixCg[0][0]->GetOutput()->GetDimensions());
  output->SetWholeExtent(matrixCg[0][0]->GetOutput()->GetWholeExtent());
  output->SetUpdateExtent(matrixCg[0][0]->GetOutput()->GetWholeExtent());
  output->SetSpacing(matrixCg[0][0]->GetOutput()->GetSpacing());
  output->SetOrigin(matrixCg[0][0]->GetOutput()->GetOrigin());
  output->SetNumberOfScalarComponents(1);  
  output->SetScalarType(dty);
  output->AllocateScalars();


  // First make Cg = identity 
  for (ia=0;ia<=2;ia++)
    for (ib=0;ib<=2;ib++)
      Cg[ia][ib]=float(ia==ib);
  
  int totalsize=dim[0]*dim[1]*dim[2];

  vtkDebugMacro(<< "6. Pointers in place totalsize="<<totalsize);

  for (int pixel=0;pixel<totalsize;pixel++)
    {
      for (ia=0;ia<this->DataDimensions;ia++)
	{
	  Cg[ia][ia]=matrixCg[ia][ia]->GetOutput()->GetPointData()->GetScalars()->GetComponent(pixel,0);
	  for (ib=ia+1;ib<this->DataDimensions;ib++)
	    {
	      Cg[ia][ib]=matrixCg[ia][ib]->GetOutput()->GetPointData()->GetScalars()->GetComponent(pixel,0);
	      Cg[ib][ia]=Cg[ia][ib];
	    }
	}
      float v=ComputeFeatureValue(Cg);
      output->GetPointData()->GetScalars()->SetComponent(pixel,0,v);
    }

  for (ia=0;ia<this->DataDimensions;ia++)
    for (ib=ia;ib<this->DataDimensions;ib++)
      matrixCg[ia][ib]->Delete();

  vtkImageData* std=vtkImageData::New();
  std->ShallowCopy(output);

  if (this->NonMaximumSuppression)
    {
      vtkImageNonMaximumSuppression* nonMax=vtkImageNonMaximumSuppression::New();
      nonMax->SetVectorInput(gradFilter->GetOutput());
      nonMax->SetMagnitudeInput(std);
      nonMax->SetDimensionality(this->DataDimensions);
      nonMax->Update();
      std->ShallowCopy(nonMax->GetOutput());
      nonMax->Delete();
      vtkDebugMacro(<< "6.5 NonMaximum Suppression Done");
    }

  output->GetDimensions(dim);  
  double range[2];
  output->GetPointData()->GetScalars()->GetRange(range);
  float sc2=255.0/(range[1]-range[0]);

  vtkImageShiftScale *shiftscale=vtkImageShiftScale::New();
  shiftscale->SetInput(std);
  shiftscale->SetShift(-range[0]);
  shiftscale->SetScale(sc2);
  shiftscale->SetOutputScalarTypeToUnsignedChar();
  shiftscale->Update();
  output->ShallowCopy(shiftscale->GetOutput());
  output->GetPointData()->GetScalars()->GetRange(range);

  vtkDebugMacro(<< "7. Scaled Output to lie in 0:255");
  
  shiftscale->Delete();
  std->Delete();
  gradFilter->Delete();

}



