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
  Module:    $RCSfile: vtkpxLevelSetUtil.cpp,v $
  Language:  C++
  Date:      $Date: 2002/07/15 12:58:44 $
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

#include "vtkpxLevelSetUtil.h"
#include "vtkImageCast.h"
#include "vtkImageImport.h"
#include "vtkImageExport.h"

#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkFastMarchingImageFilter.h"
#include "itkSigmoidImageFilter.h"

#include  "itkMRIBiasFieldCorrectionFilter.h"
#include "itkMultivariateLegendrePolynomial.h"

#include "vtkPointData.h"
#include "vtkDataArray.h"

#include "vtkImageGaussianSmooth.h"
#include "vtkImageResample.h"
#include "vtkbisImageReslice.h"
#include "vtkImageShiftScale.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

vtkpxLevelSetUtil* vtkpxLevelSetUtil::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxLevelSetUtil");
  if(ret)
    {
      return (vtkpxLevelSetUtil*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxLevelSetUtil;
}


// --------------------------------------------------------------------------------------
//                      LevelSet Stuff
// --------------------------------------------------------------------------------------

vtkImageData* vtkpxLevelSetUtil::SigmoidFilter(vtkImageData* input,
					       float SigmoidAlpha,float SigmoidBeta,
					       float SigmoidMin,float SigmoidMax)
{
  if (input==NULL)
    return NULL;

  double range[2]; input->GetPointData()->GetScalars()->GetRange(range);

  SigmoidAlpha*=(range[1]-range[0]);
  SigmoidBeta*=0.5*(range[1]+range[0]);
  
  vtkImageCast* cast=vtkImageCast::New();
  cast->SetInput(input);
  cast->SetOutputScalarTypeToFloat();

  vtkImageExport *vtkExporter = vtkImageExport::New();
  vtkExporter->SetInput(cast->GetOutput());

  pxImportType::Pointer itkImporter =  pxImportType::New();
  ConnectPipelines(vtkExporter, itkImporter);
  itkImporter->Update();

  typedef   itk::SigmoidImageFilter<pxImageType,pxImageType >  SigmoidFilterType;
  SigmoidFilterType::Pointer sigmoid = SigmoidFilterType::New();
  sigmoid->SetOutputMinimum( SigmoidMin  );
  sigmoid->SetOutputMaximum( SigmoidMax  );
  sigmoid->SetInput( itkImporter->GetOutput() );
  sigmoid->SetAlpha(SigmoidAlpha);
  sigmoid->SetBeta(SigmoidBeta);
  sigmoid->Update();
  
  pxExportType::Pointer itkExporter = pxExportType::New();
  itkExporter->SetInput(sigmoid->GetOutput());
  itkExporter->Update();

  vtkImageImport* vtkImporter=vtkImageImport::New();
  ConnectPipelines(itkExporter, vtkImporter);
  vtkImporter->Update();

  vtkImageData* EdgeMap=vtkImageData::New();
  EdgeMap->DeepCopy(vtkImporter->GetOutput());
  vtkExporter->Delete();
  vtkImporter->Delete();
  cast->Delete();

  return EdgeMap;
}


vtkImageData* vtkpxLevelSetUtil::GenerateGradientMagnitudeSpeedImage(vtkImageData* input,int SmoothingIterations,float Conductance,
								     float GradientSigma,
								     float SigmoidAlpha,float SigmoidBeta,
								     float SigmoidMin,float SigmoidMax)
{
  if (input==NULL)
    return NULL;

  double range[2]; input->GetPointData()->GetScalars()->GetRange(range);
  SigmoidAlpha*=(range[1]-range[0]);
  SigmoidBeta*=0.5*(range[1]+range[0]);

  vtkImageCast* cast=vtkImageCast::New();
  cast->SetInput(input);
  cast->SetOutputScalarTypeToFloat();

  vtkImageExport *vtkExporter = vtkImageExport::New();
  vtkExporter->SetInput(cast->GetOutput());

  pxImportType::Pointer itkImporter =  pxImportType::New();
  ConnectPipelines(vtkExporter, itkImporter);
  itkImporter->Update();

  typedef   itk::GradientMagnitudeRecursiveGaussianImageFilter<pxImageType,pxImageType >  GradientFilterType;
  GradientFilterType::Pointer  gradientMagnitude = GradientFilterType::New();

  typedef   itk::GradientAnisotropicDiffusionImageFilter<pxImageType,pxImageType >  SmoothingFilterType;
  SmoothingFilterType::Pointer smoothing = SmoothingFilterType::New();
  
  if (SmoothingIterations>0)
    {
      smoothing->SetInput( itkImporter->GetOutput() );
      smoothing->SetTimeStep( 0.05 );
      smoothing->SetNumberOfIterations(SmoothingIterations);
      smoothing->SetConductanceParameter( Conductance );
      gradientMagnitude->SetInput( smoothing->GetOutput() );
    }
  else
    {
      gradientMagnitude->SetInput(itkImporter->GetOutput());
      gradientMagnitude->SetSigma( GradientSigma  );
    }

  typedef   itk::SigmoidImageFilter<pxImageType,pxImageType >  SigmoidFilterType;
  SigmoidFilterType::Pointer sigmoid = SigmoidFilterType::New();
  sigmoid->SetOutputMinimum( SigmoidMin  );
  sigmoid->SetOutputMaximum( SigmoidMax  );
  sigmoid->SetInput( gradientMagnitude->GetOutput() );
  sigmoid->SetAlpha(SigmoidAlpha);
  sigmoid->SetBeta(SigmoidBeta);
  sigmoid->Update();

  pxExportType::Pointer itkExporter = pxExportType::New();
  itkExporter->SetInput(sigmoid->GetOutput());
  itkExporter->Update();

  vtkImageImport* vtkImporter=vtkImageImport::New();
  ConnectPipelines(itkExporter, vtkImporter);
  vtkImporter->Update();

  vtkImageData* EdgeMap=vtkImageData::New();
  EdgeMap->DeepCopy(vtkImporter->GetOutput());

  vtkExporter->Delete();
  vtkImporter->Delete();
  cast->Delete();

  return EdgeMap;
}

// -------------------------------------------------------------------------------------------------------------------
template <int ImageDimension>
vtkImageData* vtkpxLevelSetUtil_GenerateInputLevelSet(vtkImageData* input,vtkPoints* points,float offset,float factor)
{
  typedef float PixelType;
  typedef itk::Image<PixelType,ImageDimension>    ImageType;

  typedef  itk::FastMarchingImageFilter<ImageType,ImageType>   FastMarchingFilterType;
  typename FastMarchingFilterType::NodeContainer::Pointer seeds;
  seeds= FastMarchingFilterType::NodeContainer::New();
  
  seeds->Initialize();

  typename ImageType::IndexType   seedPosition;
  for (int i=0 ; i< points->GetNumberOfPoints();i++)
    {
      double p[3]; points->GetPoint(i,p);

      std::cerr << "Seed Position " << i << "(";
      for (int ia=0;ia<ImageDimension;ia++)
	{
	  seedPosition[ia] = long(p[ia]/factor+0.5);
	  std::cerr << seedPosition[ia] << " ";
	}
      std::cerr << ")" << std::endl;

      typename FastMarchingFilterType::NodeType  node;
      node.SetValue( -fabs(offset*factor));
      node.SetIndex( seedPosition );
      seeds->InsertElement( i, node );
    }


  typename ImageType::SizeType size;
  int dim[3]; input->GetDimensions(dim);
  for (int i=0;i<ImageDimension;i++)
    size[i]=dim[i];

  typename FastMarchingFilterType::Pointer fastMarching = FastMarchingFilterType::New();
  fastMarching->SetTrialPoints(  seeds  );
  fastMarching->SetSpeedConstant( 1.0 );
  fastMarching->SetOutputSize(size);
  fastMarching->Update();

  typedef itk::VTKImageExport<ImageType> ExportType;
  typename ExportType::Pointer itkExporter = ExportType::New();
  itkExporter->SetInput(fastMarching->GetOutput());
  itkExporter->Update();

  vtkImageImport* vtkImporter=vtkImageImport::New();
  ConnectPipelines(itkExporter, vtkImporter);
  vtkImporter->Update();

  vtkImageData* InputLevelSet=vtkImageData::New();
  InputLevelSet->DeepCopy(vtkImporter->GetOutput());
  InputLevelSet->SetOrigin(input->GetOrigin());
  InputLevelSet->SetSpacing(input->GetSpacing());
  vtkImporter->Delete();

  return InputLevelSet;


}
// -------------------------------------------------------------------------------------------------------------------

vtkImageData* vtkpxLevelSetUtil::GenerateInputLevelSet(vtkImageData* input,vtkPoints* points,float offset,float factor)
{
  if (input==NULL || points==NULL)
    return NULL;

  if (points->GetNumberOfPoints()<1)
    return NULL;

  int dim[3]; input->GetDimensions(dim);

  if (dim[2]==1)
    return vtkpxLevelSetUtil_GenerateInputLevelSet<2>(input,points,offset,factor);

  return vtkpxLevelSetUtil_GenerateInputLevelSet<3>(input,points,offset,factor);
}

// -------------------------------------------------------------------------------------------------------------------
int vtkpxLevelSetUtil::ResliceDistanceMap(vtkImageData* destination,vtkImageData* source,vtkImageData* ref)
{
  int nt1=source->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nt2=ref->GetPointData()->GetScalars()->GetNumberOfTuples();
  if (nt1==nt2)
    {
      destination->ShallowCopy(source);
    }
  else
    {
      vtkImageShiftScale* sc=vtkImageShiftScale::New();
      sc->SetInput(source);
      sc->SetShift(0.0);
      sc->SetScale(ref->GetSpacing()[0]/source->GetSpacing()[0]);

      vtkbisImageReslice* resl=vtkbisImageReslice::New();
      resl->OptimizationOff();
      resl->SetInput(sc->GetOutput());
      resl->SetInformationInput(ref);
      resl->SetInterpolationMode(3);
      resl->Update();
      destination->ShallowCopy(resl->GetOutput());
      resl->Delete();
      sc->Delete();
    }
  return 1;
}
// -------------------------------------------------------------------------------------------------------------------
int vtkpxLevelSetUtil::ResampleAndSmoothImage(vtkImageData* destination,vtkImageData* source,float factor)
{

  if (source==NULL || destination==NULL)
      return 0;


  float blurfactor=0.4247;


  double sp[3];  source->GetSpacing(sp);
  int    dim[3];   source->GetDimensions(dim);
  double gp[3],resolution[3];
  for (int j=0;j<=2;j++)
    {
      gp[j]=factor*blurfactor;
      resolution[j]=sp[j]*factor;
    }

  if (dim[2]==1 || sp[2] > 2.0*sp[0])
    {
      gp[2]=0.0;
      resolution[2]=sp[2];
    }

  fprintf(stderr,"In Reample and smooth Image\n");

  vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
  smooth->SetInput(source);
  smooth->SetStandardDeviations(gp);
  smooth->Update();
  
  vtkImageResample* resamp=vtkImageResample::New();
  resamp->SetInput(smooth->GetOutput());
  resamp->InterpolateOn();
  resamp->SetDimensionality(3);

  for (int ib=0;ib<=2;ib++)
    resamp->SetAxisOutputSpacing(ib,resolution[ib]);
  resamp->Update();

  
  destination->ShallowCopy(resamp->GetOutput());
  destination->GetSpacing(resolution);

  fprintf(stderr,"Smoothed (%.2f,%.2f,%.2f)  (sp=%.2f,%.2f,%.2f) --> (%.2f,%.2f,%.2f)\n",
	  gp[0],gp[1],gp[2],sp[0],sp[1],sp[2],resolution[0],resolution[1],resolution[2]);

  resamp->Delete();
  smooth->Delete();

  return 1;
}

// -------------------------------------------------------------------------------------------------------------------

