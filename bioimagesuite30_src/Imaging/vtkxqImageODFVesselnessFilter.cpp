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
#include "vtkxqImageODFVesselnessFilter.h"
#include "vtkImageReslice.h"
#include "vtkImageExtractComponents.h"
#include "vtkImageFFT.h"
#include "vtkImageRFFT.h"
//#include "vtkpxImageFloatFFT.h"
//#include "vtkpxImageFloatRFFT.h"
#include "vtkImageCast.h"
#include "vtkImageMagnitude.h"
#include "vtkImageMathematics.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkImageShiftScale.h"
#include "vtkMath.h"
//------------------------------------------------------------------------------
vtkxqImageODFVesselnessFilter* vtkxqImageODFVesselnessFilter::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkxqImageODFVesselnessFilter");
  if(ret)
    {
    return (vtkxqImageODFVesselnessFilter*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkxqImageODFVesselnessFilter;
}

// Construct object with no children.
vtkxqImageODFVesselnessFilter::vtkxqImageODFVesselnessFilter()
{
  this->Tau=1.0;
  this->Beta=0.01;
  this->OutputODF=0;
  this->ODFImage=NULL;
}
// ----------------------------------------------------------------------------
vtkxqImageODFVesselnessFilter::~vtkxqImageODFVesselnessFilter()
{
  if (this->ODFImage!=NULL)
    this->ODFImage->Delete();
}
// ----------------------------------------------------------------------------
void vtkxqImageODFVesselnessFilter::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData* input=this->GetInput();
  vtkImageData* output=this->GetOutput();

  if (input==NULL)
    {
      vtkErrorMacro(<<"No Input Set");
      return;
    }
  output->SetNumberOfScalarComponents(1);
}
// -------------------------------------------------------------------------------------------------------------


void vtkxqImageODFVesselnessFilter::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Input to vtkxqImageODFVesselnessFilter");
      return;
    }
  
  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();


  if (this->ODFImage!=NULL)
    {
      this->ODFImage->Delete();
      this->ODFImage=NULL;
    }

  vtkDataArray* odf=NULL;
  if (this->OutputODF)
    {
      this->ODFImage=vtkImageData::New();
      this->ODFImage->CopyStructure(input);
      this->ODFImage->SetScalarTypeToFloat();
      this->ODFImage->AllocateScalars();
      odf=this->ODFImage->GetPointData()->GetScalars();
    }

  this->ComputeEntropyMeasure(inp,out,odf);
    
  this->UpdateProgress(1.0);
}

void vtkxqImageODFVesselnessFilter::ComputeEntropyMeasure(vtkDataArray* inp,vtkDataArray* out,vtkDataArray* odf)
{
  int nc=inp->GetNumberOfComponents();
  int nt=inp->GetNumberOfTuples();
  double* p=new double[nc];

  int tenth=nt/10;
  int count=0;

  for (int voxel=0;voxel<nt;voxel++)
    {
      double sum=0.0;
      for (int comp=0;comp<nc;comp++)
	{
	  double v=inp->GetComponent(voxel,comp);
	  p[comp]=exp(-this->Beta*v);
	  sum+=p[comp];
	}

      double entropy=0.0;
      for (int comp=0;comp<nc;comp++)
	{
	  if (sum>1e-6)
	    p[comp]/=sum;
	  else
	    p[comp]=0.0;

	  if (odf!=NULL)
	    odf->SetComponent(voxel,comp,p[comp]);
	  if (p[comp]>1e-6)
	    entropy+= p[comp]*log(p[comp]);
	}

      double vess=exp(this->Tau*entropy);
      //      fprintf(stderr,"\n%f____%f\n",entropy,vess);
      out->SetComponent(voxel,0,vess);
      ++count;
      if (count==tenth)
	{
	  this->UpdateProgress(this->GetProgress()+0.1);
	  count=0;
	}
    }
  delete [] p;
}


