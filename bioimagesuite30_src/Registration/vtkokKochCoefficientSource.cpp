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

#include "vtkokKochCoefficientSource.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkImageData.h"
#include "vtkImageFFT.h"
#include "vtkImageRFFT.h"
#include "vtkImageMathematics.h"
#include "vtkImageShiftScale.h"
#include "vtkImagePermute.h"
#include "vtkDataArray.h"
#include "vtkExtractVOI.h"
#include "vtkImageAccumulate.h"
#include "vtkImageThreshold.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformation.h"
#include "pxisinf.h"

vtkCxxRevisionMacro(vtkokKochCoefficientSource, "$Revision: 1.00 $");
vtkStandardNewMacro(vtkokKochCoefficientSource);

//----------------------------------------------------------------------------
vtkokKochCoefficientSource::vtkokKochCoefficientSource()
{
  this->ScaleFactorOfDisplacement=1000.0;
  this->B0=1.0;
  this->OutputScalarType = VTK_DOUBLE;
  this->mode= 0;

  this->ImageFFT               = vtkImageData::New();
  this->ImageCoefficient       = vtkImageData::New();
  this->ImageMultiplied        = vtkImageData::New();
  this->ImageRFFT              = vtkImageData::New();
  this->InducedField           = vtkImageData::New();
  this->CoefficientMap         = vtkImageData::New();
  this->Padding=20;
}

//----------------------------------------------------------------------------
vtkokKochCoefficientSource::~vtkokKochCoefficientSource(){
    this->ImageFFT->Delete();
    this->ImageCoefficient->Delete();
    this->ImageMultiplied->Delete();
    this->ImageRFFT->Delete();
    this->InducedField->Delete();
    this->CoefficientMap->Delete();

  };

//----------------------------------------------------------------------------
void vtkokKochCoefficientSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "OutputScalarType: " << this->OutputScalarType << "\n";
}
//----------------------------------------------------------------------------
// Compute the "Coeffcient Image" Ck(kx,ky,kz) of
// B_dz(kx,ky,kz) = B0 * Ck(kx,ky,kz) * Chi(kx,ky,kz)
// where
// B_dz() : induced field caused by target susceptibility in fourier domain
// B0     : static magnetic field
// Chi()  : susceptibility in fourier domain
//
// In case the argument "mode" is set to nonzero value, small value will be 
// assigned to voxels whose values are zero according to the above formula.
// This computation
// Refer to (10) of "Rapid calculations of susceptibility-induced magnetic field
// perturbations for in vivo magnetic resonance", Koch et al,Phys. Med.BioL 
// 51(2006)6831-6402
//----------------------------------------------------------------------------
template <class IT,class OT>
void vtkokKochCoefficientSourceExecute(vtkokKochCoefficientSource *self,
				       vtkImageData *input,vtkImageData* output,
				       int mode,
				       IT *,OT* )
{
  double tmpx,tmpy,tmpz,denom;
  OT outVal;

  int extent[6]; input->GetExtent(extent);
  OT* ptr = (OT*)output->GetScalarPointerForExtent(extent);

  fprintf(stderr,"In Source Execute %d:%d %d:%d %d:%d\n",
	  extent[0],extent[1],extent[2],extent[3],extent[4],extent[5]);

  //center of the fourier space
  float c0 = 0.5*(extent[0]+extent[1]);
  float c1 = 0.5*(extent[2]+extent[3]);
  float c2 = 0.5*(extent[4]+extent[5]);

  double spacing[3];
  output->GetSpacing(spacing);

  int min0 = extent[0];
  int max0 = extent[1];

  vtkIdType inc0, inc1, inc2;

  output->GetContinuousIncrements(extent, inc0, inc1, inc2);
  inc0 = 2;

  printf("center = %f %f %f \n",c0,c1,c2);
  printf("extent = %d %d %d %d %d %d \n",extent[0],extent[1],extent[2],extent[3],extent[4],extent[5]);
  //printf("inc = %d %d %d \n",(int)inc0,inc1,inc2);
  cout << "inc = " << inc0 << " " << inc1 << " " << inc2 << endl;

  printf("spacing = %f %f %f \n",spacing[0],spacing[1],spacing[2]);
  double tmp0,tmp1,tmp2;

  tmp0 = spacing[0]*spacing[0];
  tmp1 = spacing[1]*spacing[1];
  tmp2 = spacing[2]*spacing[2];

  for (int idx2 = extent[4]; idx2 <= extent[5]; ++idx2)
    {
      tmpz = (idx2<c2)?(idx2*idx2*tmp2):((extent[5]-idx2)*(extent[5]-idx2)*tmp2);
      for (int idx1 = extent[2]; idx1 <= extent[3]; ++idx1)
	{
	  tmpy = (idx1<c1)?(idx1*idx1*tmp1):((extent[3]-idx1)*(extent[3]-idx1)*tmp1);
	  for (int idx0 = extent[0]; idx0 <= extent[1]; ++idx0)
	    {
	      tmpx = (idx0<c0)?(idx0*idx0*tmp0):((extent[1]-idx0)*(extent[1]-idx0)*tmp0);
	      denom = tmpx + tmpy + tmpz;
	      if (denom == 0 )
		{
		  outVal = (OT)0.0;
		}
	      else
		{
		  outVal = (OT)((1.0/3.0 - tmpz/(tmpx+tmpy+tmpz)));
		}
	      if (mode == 0 )
		{
		  *ptr = outVal;
		  ptr+=inc0;
		}
	      else 
		{ 
		  double epsilon=0.01;
		  if(outVal < epsilon && outVal > -epsilon)
		    outVal = OT(epsilon);
		  *ptr++ = outVal;
		  *ptr++ = outVal;
		}
	    }
	  ptr += inc1;
	}
      ptr += inc2;
    }
  fprintf(stderr,"Done Source Execute\n");
}




//----------------------------------------------------------------------------
//  Compute induced displacement field from susceptibility map
//  Input  : this->Input 
//  Coefficient Source == KochCoefficient
//  Output : this->Output
//----------------------------------------------------------------------------

vtkImageData* vtkokKochCoefficientSource::EstimateInductionField(vtkImageData* SusceptibilityField,
								 vtkImageData* KochCoefficient)
{
   //Compute fourier transform Chi(kx,ky,kz) of susceptibility map 
   vtkDebugMacro(<<"#FFT");
   vtkImageFFT *fftfilter = vtkImageFFT::New();
   fftfilter->SetDimensionality(3);
   fftfilter->SetInput(SusceptibilityField);
   fftfilter->Update();
   this->ImageFFT->ShallowCopy(fftfilter->GetOutput());
   this->ImageFFT->SetSpacing(KochCoefficient->GetSpacing());

   // Compute the "Coeffcient Image" Ck(kx,ky,kz)

   vtkDebugMacro(<<"#Image Coefficient");
   this->ImageCoefficient->ShallowCopy(KochCoefficient);

   //Compute Ck(kx,ky,kz) * Chi(kx,ky,kz)

   vtkDebugMacro(<<"#Multiply Coefficient");
   this->ImageMultiplied->ShallowCopy(this->ImageCoefficient);
   vtkImageMathematics *math = vtkImageMathematics::New();
   math->SetInput1(this->ImageCoefficient);
   math->SetInput2(this->ImageFFT);
   math->SetOperationToComplexMultiply();
   math->Update();
   this->ImageMultiplied->ShallowCopy(math->GetOutput());

   //Compute Inverse transform of Ck(kx,ky,kz) * Chi(kx,ky,kz)
   //which equals B_dz(x,y,z)*B0^(-1)
   //where B_dz(x,y,z) is the induced field and B0 is the static
   //magnetic field

   vtkDebugMacro(<<"#Inverse fft");
   vtkImageRFFT* rfft = vtkImageRFFT::New();
   rfft->SetDimensionality(3);
   rfft->SetInput(this->ImageMultiplied);
   rfft->Update();
   this->ImageRFFT->ShallowCopy(rfft->GetOutput());
   this->ImageRFFT->SetSpacing(SusceptibilityField->GetSpacing());

   //Compute induced field B_dz(x,y,z)
   vtkDebugMacro(<<"#Induced Field");
   //ImageRFFT:[ppm  ]
   //c:[Hz]=[T]*[Hz/T]
   double c = 1.0*this->B0;
   printf("c = %f\n",c);
   vtkImageShiftScale* scale = vtkImageShiftScale::New();
   scale->SetScale(c);
   scale->SetInput(this->ImageRFFT);
   scale->Update();
   this->InducedField->ShallowCopy(scale->GetOutput());

   //Convert unit from induced field to displacement field
   vtkDebugMacro(<<"#Displacement Field");
   vtkDataArray* data1 = this->ImageRFFT->GetPointData()->GetScalars();

   
   double mean=0.0;
   if(this->MeanDisplacementToZero)
     {
       vtkDebugMacro(<<"Computing Mean ");
       vtkDataArray* arr=SusceptibilityField->GetPointData()->GetScalars();
       int nt=arr->GetNumberOfTuples();
       int nv=0.0;

       for (int i=0;i<nt;i++)
	 {
	   double v=arr->GetComponent(i,0);
	   if (fabs(v+9.0)<0.01)
	     {
	       nv++;
	       mean+=data1->GetComponent(i,0);
	     }
	 }
       if(nv>0) mean/=nv;
       fprintf(stderr,"mean = %f\n",mean);
   } 

   //shift image so that the average of VOI becomes zero.

   vtkImageData* dispfield=vtkImageData::New();
   dispfield->ShallowCopy(this->ImageRFFT);
   dispfield->SetScalarType(this->OutputScalarType);
   dispfield->AllocateScalars();


   vtkDataArray* arr=dispfield->GetPointData()->GetScalars();
   int nt=arr->GetNumberOfTuples();
   for (int i=0;i<nt;i++)
     {
       double v=data1->GetComponent(i,0);
       arr->SetComponent(i,0,v-mean);
     }
   fftfilter->Delete();
   math->Delete();
   rfft->Delete();
   scale->Delete();


   return dispfield;

}
//----------------------------------------------------------------------------
vtkImageData* vtkokKochCoefficientSource::PadImage(vtkImageData* img,int padsize)
{
  vtkImageReslice* resl=vtkImageReslice::New();
  resl->SetInput(img);

  int dim[3]; img->GetDimensions(dim);
  int paddim[3];
  for (int i=0;i<=2;i++)
    paddim[i]=2*padsize+dim[i];


  double ori[3];  img->GetOrigin(ori);
  double spa[3];  img->GetSpacing(spa);
  double neworigin[3];
  if (this->Debug)
    fprintf(stderr,"\t Padding = (%d,%d,%d) ori=(%.1f,%.1f,%.1f) spa=(%.1f,%.1f,%.1f)\n",
	    dim[0],	  dim[1],	  dim[2],
	    ori[0],	  ori[1],	  ori[2],
	    spa[0],	  spa[1],	  spa[2]);

  
  for (int ia=0;ia<=2;ia++)
    {
      int offset=(paddim[ia]-dim[ia])/2;
      neworigin[ia]=ori[ia];//-spa[ia]*double(offset);
    }
  

  resl->SetOutputExtent(0,paddim[0]-1,0,paddim[1]-1,0,paddim[2]-1);
  resl->SetOutputOrigin(neworigin);
  resl->SetOutputSpacing(spa);

  resl->SetInterpolationMode(0);
  resl->SetBackgroundLevel(0.0);
  resl->Update();
  
  vtkImageData* out=vtkImageData::New();
  out->ShallowCopy(resl->GetOutput());
  resl->Delete();

  out->GetDimensions(dim);
  out->GetOrigin(ori);
  out->GetSpacing(spa);

  if (this->Debug)
    fprintf(stderr,"\t Padding Done = (%d,%d,%d) ori=(%.1f,%.1f,%.1f) spa=(%.1f,%.1f,%.1f)\n",
	    dim[0],	  dim[1],	  dim[2],
	    ori[0],	  ori[1],	  ori[2],
	    spa[0],	  spa[1],	  spa[2]);
  
  return out;
}

//----------------------------------------------------------------------------
int vtkokKochCoefficientSource::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector, 
						    vtkInformationVector *outputVector)
{
  vtkInformation *inInfo =  inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  int wholeExtent[6]; inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExtent);
  double sp[3];  inInfo->Get(vtkDataObject::SPACING(), sp);
  double origin[3];  inInfo->Get(vtkDataObject::ORIGIN(),  origin);

  outInfo->Set(vtkDataObject::ORIGIN(),  origin, 3);
  outInfo->Set(vtkDataObject::SPACING(),sp , 3);
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),wholeExtent,6);
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), this->OutputScalarType, 1);

  return 1;
}
//----------------------------------------------------------------------------
template <class T>
void vtkokKochCoefficientSourceExecute1(vtkokKochCoefficientSource *self,
					vtkImageData *inData,
					vtkImageData *outData,int mode,
					T *)
{
  fprintf(stderr,"In Simple Execute 1\n");
  switch (outData->GetScalarType())
    {
      vtkTemplateMacro6(vtkokKochCoefficientSourceExecute, self, inData,
			outData,mode,
			static_cast<T *>(0), static_cast<VTK_TT *>(0));
    default:
      vtkGenericWarningMacro("Execute: Unknown input ScalarType");
      return;
    }
}

void vtkokKochCoefficientSource::SimpleExecute(vtkImageData* input,vtkImageData *output)
{
  fprintf(stderr,"In Simple Execute\n");



  // Pad Input by a number of voxels
  // 
  vtkImageData* paddedinput=this->PadImage(input,this->Padding);


  double FOV[3];
  int wholeExtent[6]; paddedinput->GetWholeExtent(wholeExtent);
  FOV[0] = (wholeExtent[1] - wholeExtent[0] +1);
  FOV[1] = (wholeExtent[3] - wholeExtent[2] +1);
  FOV[2] = (wholeExtent[5] - wholeExtent[4] +1);

  double sp[3];     sp[0]=2*M_PI/FOV[0];  sp[1]=2*M_PI/FOV[1];  sp[2]=2*M_PI/FOV[2];
  double origin[3]; paddedinput->GetOrigin(origin);
  int    dim[3];    paddedinput->GetDimensions(dim);


  this->CoefficientMap->CopyStructure(paddedinput);
  this->CoefficientMap->SetOrigin(origin);
  this->CoefficientMap->SetDimensions(dim);
  this->CoefficientMap->SetNumberOfScalarComponents(2);
  this->CoefficientMap->SetScalarType(this->OutputScalarType);
  this->CoefficientMap->AllocateScalars();

  switch (paddedinput->GetScalarType())
    {
      vtkTemplateMacro5(vtkokKochCoefficientSourceExecute1,
			this,paddedinput,this->CoefficientMap,this->mode,
			static_cast<VTK_TT *>(0));
    default:
      vtkErrorMacro("Execute: Unknown output ScalarType");
    }

  vtkDebugMacro(<<"vtkokKochCoefficientSourceExecute done..");

  vtkImageData* tmpout=this->EstimateInductionField(paddedinput,this->CoefficientMap);

  vtkImageReslice* resl=vtkImageReslice::New();
  resl->SetInformationInput(input);
  resl->SetInput(tmpout);
  resl->SetInterpolationMode(0);
  resl->SetBackgroundLevel(0.0);
  resl->Update();
  
  output->ShallowCopy(resl->GetOutput());
  resl->Delete();
  // Unpad Output .....
}


