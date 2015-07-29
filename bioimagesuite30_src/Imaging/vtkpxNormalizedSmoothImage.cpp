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
  Module:    $RCSfile: vtkpxNormalizedSmoothImage.cpp,v $
  Language:  C++
  Date:      $Date: 2002/09/26 18:09:25 $
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
#include "vtkpxNormalizedSmoothImage.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkDataArray.h"
//------------------------------------------------------------------------------
vtkpxNormalizedSmoothImage* vtkpxNormalizedSmoothImage::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxNormalizedSmoothImage");
  if(ret)
    {
      return (vtkpxNormalizedSmoothImage*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxNormalizedSmoothImage;
}

// Construct object with no children.
vtkpxNormalizedSmoothImage::vtkpxNormalizedSmoothImage()
{
  this->Sigma=3.0;
  this->NormalizerInput=NULL;
  this->NormalizeScale=1000.0;

}
// ----------------------------------------------------------------------------
vtkpxNormalizedSmoothImage::~vtkpxNormalizedSmoothImage()
{
  this->SetNormalizerInput(NULL);
}
// --------------------------------------------------------------------------------------------
void vtkpxNormalizedSmoothImage::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }

  //  int nimpc=input->GetNumberOfScalarComponents();


  this->UpdateProgress(0.01);

  vtkImageGaussianSmooth* sm=vtkImageGaussianSmooth::New();
  sm->SetInput(this->GetImageDataInput(0));
  double sp[3],sigma[3]; input->GetSpacing(sp);
  for (int i=0;i<=2;i++)
    sigma[i]=this->Sigma/sp[i];
  sm->SetStandardDeviations(sigma);
  sm->Update();

  this->UpdateProgress(0.7);

  vtkDataArray* norm=sm->GetOutput()->GetPointData()->GetScalars();
  int offset=1;

  if (this->NormalizerInput!=NULL)
    {
      norm=this->NormalizerInput->GetPointData()->GetScalars();
      offset=0;
    }


  fprintf(stderr,"Offset = %d\n",offset);
  
  vtkDataArray* inp=sm->GetOutput()->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();
  if (offset==1)
    out->CopyComponent(0,inp,0);
  
  int nt1=inp->GetNumberOfTuples();
  int nt2=norm->GetNumberOfTuples();
  if (nt1!=nt2)
    {
      vtkErrorMacro(<<"Cannot Normalize Bad Input Dimensions");
    }
  else
    {
      int nt=out->GetNumberOfTuples();
      int nc=out->GetNumberOfComponents();
      
      for (int i=0;i<nt;i++)
	{
	  float divisor=norm->GetComponent(i,0);
	  for (int j=offset;j<nc;j++)
	    {
	      double v=inp->GetComponent(i,j);
	      if (fabs(divisor)>0.001)
		out->SetComponent(i,j,this->NormalizeScale*v/divisor);
	      else
		out->SetComponent(i,j,0.0);
	    }


	}
    }

  sm->Delete();
  this->UpdateProgress(1.0);
}


