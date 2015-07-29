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
  Module:    $RCSfile: vtkpxCreateSyntheticFmri.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/23 21:21:45 $
  Version:   $Revision: 1.2 $


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
#include "vtkpxCreateSyntheticFmri.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkbisImageReslice.h"
#include "vtkpxUtil.h"
#include "vtkAbstractTransform.h"
#include "vtkTransform.h"
#include "vtkGeneralTransform.h"
#include "math.h"
#include "vtkPlaneSource.h"
#include "vtkAppendPolyData.h"
#include "vtkPolyData.h"
#include "vtkbisImageReslice.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageExtractComponents.h"
//------------------------------------------------------------------------------
vtkpxCreateSyntheticFmri* vtkpxCreateSyntheticFmri::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxCreateSyntheticFmri");
  if(ret)
    {
    return (vtkpxCreateSyntheticFmri*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxCreateSyntheticFmri;
}

// Construct object with no children.
vtkpxCreateSyntheticFmri::vtkpxCreateSyntheticFmri()
{
  this->ScaleFactor=0.1;
  this->NumberOfFrames=2;
  this->BlockPeriod=1;
  this->ActivationMap=NULL;
}
// ----------------------------------------------------------------------------
vtkpxCreateSyntheticFmri::~vtkpxCreateSyntheticFmri()
{
  if (this->ActivationMap!=NULL)
    {
      this->ActivationMap->UnRegister(this);
      this->ActivationMap=NULL;
    }

}
// --------------------------------------------------------------------------------------
// New Code 
// --------------------------------------------------------------------------------------

void vtkpxCreateSyntheticFmri::ExecuteInformation()
{
  vtkImageData *input=this->GetImageDataInput(0);
  if (input==NULL || this->ActivationMap == NULL )
    {
      vtkErrorMacro(<<"No Input or no Activation Data !!");
      return;
    }

  vtkImageData *output=this->GetOutput();
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  output->SetNumberOfScalarComponents(this->NumberOfFrames);

}

// ----------------------------------------------------------------------------
void vtkpxCreateSyntheticFmri::SimpleExecute(vtkImageData*,vtkImageData*)
{

  if (this->GetImageDataInput(0)==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }

  if (this->ActivationMap==NULL)
    {
      vtkErrorMacro(<<"No Activation Map\n Exiting\n");
      return;
    }


  vtkImageData* input=this->GetImageDataInput(0);

  // Check Dimensions to make sure all phase images have the same dimensions
  // -----------------------------------------------------------------------
  int dim[3];
  input->GetDimensions(dim);

  int dim_1[3];
  this->ActivationMap->GetDimensions(dim_1);
  int sum=0;
  for (int kk=0;kk<=2;kk++)
    sum+=abs(dim_1[kk]-dim[kk]);
  if (sum!=0)
    {
      vtkErrorMacro(<<"Activation Map and Input Image have different Dimensions !!");
      return;
    }
  
  
  vtkDebugMacro(<< "Creating Output\n" <<  "Dimensions " << dim[0] <<","<< dim[1] <<","<< dim[2] << "nf=" << this->NumberOfFrames << "\n");
  
  vtkImageData* output=this->GetOutput();
  output->SetSpacing(input->GetSpacing());
  output->SetOrigin(input->GetOrigin());
  output->SetScalarType(input->GetScalarType());
  output->SetDimensions(input->GetDimensions());
  output->SetWholeExtent(input->GetWholeExtent());
  output->SetNumberOfScalarComponents(this->NumberOfFrames);
  output->AllocateScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();


  int slicesize=dim[0]*dim[1];
  int activation=0,count=0;
  static int count2=0;
  int tenth=this->NumberOfFrames*dim[0]*dim[1]*dim[2]/10;
  this->UpdateProgress(0.0);
  float pog=0.0;

  for (int f=0;f<this->NumberOfFrames;f++)
    {
      for (int k=0;k<dim[2];k++)
	for (int i=0;i<dim[0];i++)
	  for (int j=0;j<dim[1];j++)
	    {
	      int pixel=k*slicesize + j*dim[0] + i;
	      
	      float oldintensity=input->GetScalarComponentAsDouble(i,j,k,0);
	      float tmapvalue=this->ActivationMap->GetScalarComponentAsDouble(i,j,k,0)*this->ScaleFactor;
	      float newintensity=(1.0+float(activation)*tmapvalue)*oldintensity;
	      if (newintensity<0.0)
		newintensity=0.0;
	      out->SetComponent(pixel,f,newintensity);

	      ++count2;
	      if (count2==tenth)
		{
		  pog+=0.1;
		  this->UpdateProgress(pog);
		  count2=0;
		}

	    }
      ++count;
      if (count==this->BlockPeriod)
	{
	  count=0;
	  activation=1-activation;
	}
    }
}

// --------------------------------------------------------------------------------------
//   Tmap Comparison Stuff 
// --------------------------------------------------------------------------------------

int vtkpxCreateSyntheticFmri::DoROCAnalysis(vtkImageData* input,
					    vtkImageData* roi_img,
					    vtkFloatArray* tvaluesin,
					    vtkFloatArray* tripletsout,
					    float ignore)

{
  if (input==NULL || roi_img==NULL || tvaluesin==NULL || tripletsout==NULL )
    return 0;

  int numtests=tvaluesin->GetNumberOfTuples();
  tripletsout->SetNumberOfComponents(4);
  tripletsout->SetNumberOfTuples(numtests);

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* roi=roi_img->GetPointData()->GetScalars();

  int np1=inp->GetNumberOfTuples();
  int np2=roi->GetNumberOfTuples();

  if (np1!=np2)
    {
      fprintf(stderr,"Different Number Of Scalars\n");
      return 0;
    }

  for (int ia=0;ia<4;ia++)
    tripletsout->FillComponent(ia,0.0);

  
  for (int test=0;test<numtests;test++)
    {
      float threshold=tvaluesin->GetComponent(test,0);
      int   numtrue=0,totalnumtrue=0,numfalse=0,totalnumfalse=0;

      for (int pixel=0;pixel<np1;pixel++)
	{
	  float act=inp->GetComponent(pixel,0);
	  float flag=roi->GetComponent(pixel,0);
	  
	  if (act != ignore ) 
	    {
	      if (flag>0)
		++totalnumtrue;
	      else
		++totalnumfalse;
	      
	      if (fabs(act)>threshold)
		{
		  if (flag>0)
		    ++numtrue;
		  else
		    ++numfalse;
		}
	    } 
	}
      
      float truepos=100.0*float(numtrue)/float(totalnumtrue);
      float falsepos=100.0*float(numfalse)/float(totalnumfalse);
      float fraction=100.0*float(totalnumtrue)/float(totalnumtrue+totalnumfalse);
      tripletsout->SetComponent(test,0,threshold);
      tripletsout->SetComponent(test,1,truepos);
      tripletsout->SetComponent(test,2,falsepos);
      tripletsout->SetComponent(test,3,fraction);
      //fprintf(stderr,"Np1=%d totalnumtrue=%d totalnumfalse=%d numtrue=%d numfalse=%d\n",
      //np1,totalnumtrue,totalnumfalse,numtrue,numfalse);
    }

  
  return 1;
  
}

// --------------------------------------------------------------------------------------
//   Smart Reslice Stuff 
// --------------------------------------------------------------------------------------
int vtkpxCreateSyntheticFmri::ComputeResliceTransformation(vtkTransform* trans,
							   vtkImageData* inputImage,
							   float offsetx,float offsety,float offsetz,
							   int   dimx,int dimy,int dimz,
							   float fovx,float fovy,float slicespacing,
							   float anglex,float angley,float anglez)
{

  if (trans==NULL || inputImage==NULL)
    return 0;


  double sp[3],ori[3];
  int dim[3];

  inputImage->GetSpacing(sp);
  inputImage->GetOrigin(ori);
  inputImage->GetDimensions(dim);

  double cent[3];
  for (int ia=0;ia<=2;ia++)
    cent[ia]=ori[ia]+float(dim[ia])*sp[ia]*0.5;

  fprintf(stderr,"Center = %.1f %.1f %.1f\n",cent[0],cent[1],cent[2]);
  fprintf(stderr,"Angles = %.1f %.1f %.1f\n",anglex,angley,anglez);
  fprintf(stderr,"offsets = %.1f %.1f %.1f\n",offsetx,offsety,offsetz);

  trans->PostMultiply();
  trans->Identity();

  trans->Translate(-cent[0],-cent[1],-cent[2]);
  trans->RotateX(anglex);
  trans->RotateY(angley);
  trans->RotateZ(anglez);
  trans->Translate(offsetx,offsety,offsetz);
  trans->Translate(cent[0],cent[1],cent[2]);
  
  for(int i=0;i<=3;i++)
    {
      for (int j=0;j<=3;j++)
	fprintf(stderr,"%6.2f ",trans->GetMatrix()->GetElement(i,j));
      fprintf(stderr,"\n");
    }


  return 1;
}
// --------------------------------------------------------------------------------------
int vtkpxCreateSyntheticFmri::ResliceImageUsingOffsets(vtkImageData* inputImage,
						       vtkImageData* outputImage,
						       vtkTransform* trans,
						       int dimx,int dimy,int dimz,
						       float fovx,float fovy,float slicespacing,
						       float slicethickness,
						       int interpolation_mode)
{
  if (trans==NULL || inputImage==NULL)
    return 0;


  double sp[3],ori[3];
  int dim[3];

  inputImage->GetSpacing(sp);
  inputImage->GetOrigin(ori);
  inputImage->GetDimensions(dim);

  double cent[3];
  for (int ia=0;ia<=2;ia++)
    cent[ia]=ori[ia]+float(dim[ia])*sp[ia]*0.5;

  float cx=fovx*10.0*0.5;
  float xsp=fovx*10.0/float(dimx);

  float cy=10.0*fovy*0.5;
  float ysp=10.0*fovy/float(dimy);
  
  float zl= 0.5* float(dimz-1) * slicespacing;

  double p[3];
  p[0]=cent[0]-cx;
  p[1]=cent[1]-cy;
  p[2]=cent[2]-zl;

  double sigma[3],radius[3],ratio[3];
  ratio[0]=(xsp/sp[0]);
  ratio[1]=(ysp/sp[1]);
  ratio[2]=(slicethickness/sp[2]);

  for (int i=0;i<=2;i++)
    {
      sigma[i] =10.0;
      radius[i]=0.5*(ratio[i]/sigma[i]);
    }


  vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
  smooth->SetInput(inputImage);
  smooth->SetStandardDeviations(sigma);
  smooth->SetRadiusFactors(radius);


  vtkbisImageReslice* resl=vtkbisImageReslice::New(); resl->OptimizationOff();
  resl->SetInput(smooth->GetOutput());
  resl->SetInterpolationMode(interpolation_mode);
  resl->SetResliceTransform(trans);
  resl->SetOutputSpacing(xsp,ysp,slicespacing);
  resl->SetOutputOrigin(p);
  resl->SetOutputExtent(0,dimx-1,0,dimy-1,0,dimz-1);
  resl->Update();
  outputImage->ShallowCopy(resl->GetOutput());
  resl->Delete();

  return 1;
}

// --------------------------------------------------------------------------------------
int vtkpxCreateSyntheticFmri::CreateReferenceImageGeometry(vtkImageData* inputImage,
							   vtkImageData* outputReference,
							   int dimx,int dimy,int dimz,
							   float fovx,float fovy,float slicespacing)
{
  if (inputImage==NULL)
    return 0;


  double sp[3],ori[3];
  int dim[3];

  inputImage->GetSpacing(sp);
  inputImage->GetOrigin(ori);
  inputImage->GetDimensions(dim);

  double cent[3];
  for (int ia=0;ia<=2;ia++)
    cent[ia]=ori[ia]+float(dim[ia])*sp[ia]*0.5;

  fprintf(stderr,"Offset Center = %.1f %.1f %.1f\n",cent[0],cent[1],cent[2]);

  float cx=fovx*10.0*0.5;
  float xsp=fovx*10.0/float(dimx);

  float cy=10.0*fovy*0.5;
  float ysp=10.0*fovy/float(dimy);
  
  float zl= 0.5* float(dimz-1) * slicespacing;

  double p[3];
  p[0]=cent[0]-cx;
  p[1]=cent[1]-cy;
  p[2]=cent[2]-zl;


  outputReference->Initialize();
  outputReference->SetOrigin(p);
  outputReference->SetSpacing(xsp,ysp,slicespacing);
  outputReference->SetDimensions(dimx,dimy,dimz);
  outputReference->SetWholeExtent(0,dimx-1,0,dimy-1,0,dimz-1);
  outputReference->SetScalarType(VTK_CHAR);
  outputReference->SetNumberOfScalarComponents(1);
  outputReference->AllocateScalars();


  return 1;
}
// --------------------------------------------------------------------------------------
int vtkpxCreateSyntheticFmri::CreatePolyData(vtkTransform* trans,
					     vtkImageData* inputImage,
					     vtkPolyData* polydata,
					     int xr,int yr,
					     int   dimx,int dimy,int dimz,
					     float fovx,float fovy,float slicespacing)
{

  if (trans==NULL || inputImage==NULL || polydata == NULL)
    return 0;

  double  sp[3],ori[3];
  int dim[3];

  inputImage->GetSpacing(sp);
  inputImage->GetOrigin(ori);
  inputImage->GetDimensions(dim);

  double cent[3];
  for (int ia=0;ia<=2;ia++)
    cent[ia]=ori[ia]+float(dim[ia])*sp[ia]*0.5;

  float width = fovx*10.0;
  float height= fovy*10.0;

  float cx=width*0.5;
  float xsp=width/dimx;
  float cy=height*0.5;
  float ysp=height/dimy;
  float zl= 0.5* float(dimz-1) * slicespacing;


  vtkAppendPolyData* append=vtkAppendPolyData::New();
  
  for (int sl=0;sl<dimz;sl++)
    {
      vtkPlaneSource* pl=vtkPlaneSource::New();
      pl->SetResolution(xr,yr);
      pl->SetOrigin(cent[0]-cx    ,cent[1]-cy    ,cent[2]-zl + sl*slicespacing);
      pl->SetPoint1(cent[0]+cx-xsp,cent[1]-cy    ,cent[2]-zl + sl*slicespacing);
      pl->SetPoint2(cent[0]-cx    ,cent[1]+cy-ysp,cent[2]-zl + sl*slicespacing);
      pl->Update();
      append->AddInput(pl->GetOutput());
      pl->Delete();
    }
  append->Update();
  polydata->DeepCopy(append->GetOutput());
  append->Delete();
  return 1;
}
  

