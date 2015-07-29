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
  Module:    $RCSfile: vtkpxImageVariableSmoothing.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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
#include "vtkpxImageVariableSmoothing.h"
#include "vtkpxUtil.h"
#include "vtkFloatArray.h"
#include "vtkImageGaussianSmooth.h"
//------------------------------------------------------------------------------
vtkpxImageVariableSmoothing* vtkpxImageVariableSmoothing::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageVariableSmoothing");
  if(ret)
    {
    return (vtkpxImageVariableSmoothing*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageVariableSmoothing;
}

// Construct object with no children.
vtkpxImageVariableSmoothing::vtkpxImageVariableSmoothing()
{
  this->LowerThreshold=2.0;
  this->LowerSigma=1.0;
  this->UpperThreshold=5.0;
  this->UpperSigma=3.0;
  this->DistanceMap=NULL;
  this->RatioMap=NULL;
}
// ----------------------------------------------------------------------------
vtkpxImageVariableSmoothing::~vtkpxImageVariableSmoothing()
{
  this->SetDistanceMap(NULL);
  if (this->RatioMap!=NULL)
    this->RatioMap->Delete();
}
// ----------------------------------------------------------------------------
void vtkpxImageVariableSmoothing::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL    || this->DistanceMap==NULL )
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxImageVariableSmoothing");
      return;
    }

  int dim[3],dim2[3];
  input->GetDimensions(dim);
  this->DistanceMap->GetDimensions(dim2);
  
  int sum=0;
  for (int ia=0;ia<=2;ia++)
    sum+=abs(dim[ia]-dim2[ia]);
  if (sum!=0)
    {
      vtkErrorMacro(<<"Bad Input/Distance Map  Dimensions");
      return;
    }
    
  vtkImageGaussianSmooth* smooth[2];
  vtkDataArray* sm_img[2];

  double sp[3]; input->GetSpacing(sp);

  for (int ia=0;ia<=1;ia++)
    {
      smooth[ia]=vtkImageGaussianSmooth::New();
      smooth[ia]->SetInput(input);
      float sigma=this->LowerSigma;
      if (ia==1)
	sigma=this->UpperSigma;
      smooth[ia]->SetStandardDeviations(sigma,sigma*sp[0]/sp[1],sigma*sp[0]/sp[2]);
      smooth[ia]->Update();
      sm_img[ia]=smooth[ia]->GetOutput()->GetPointData()->GetScalars();
      this->UpdateProgress(0.2*double(ia+1));
    }

  vtkDataArray* out=output->GetPointData()->GetScalars();

  if (this->RatioMap==NULL)
    this->RatioMap=vtkImageData::New();
  this->RatioMap->CopyStructure(this->DistanceMap);
  this->RatioMap->AllocateScalars();

  vtkDataArray* dist=this->DistanceMap->GetPointData()->GetScalars();
  vtkDataArray* rmap=this->RatioMap->GetPointData()->GetScalars();
  int np=out->GetNumberOfTuples();
  int nc=out->GetNumberOfComponents();

  // A Hack but ...
  if (this->UpperThreshold<=this->LowerThreshold)
    this->UpperThreshold=this->LowerThreshold+1.0;
  float diff=this->UpperThreshold-this->LowerThreshold;

  for (int i=0;i<np;i++)
    {
      float d=fabs(dist->GetComponent(i,0));
      for (int j=0;j<nc;j++)
	{
	  if (d<=this->LowerThreshold)
	    {
	      out->SetComponent(i,j,sm_img[0]->GetComponent(i,j));
	      rmap->SetComponent(i,j,0.0);
	    }
	  else if (d>=this->UpperThreshold)
	    {
	      out->SetComponent(i,j,sm_img[1]->GetComponent(i,j));
	      rmap->SetComponent(i,j,100.0);
	    }
	  else
	    {
	      float ratio=(d-this->LowerThreshold)/diff;
	      out->SetComponent(i,j,
				(1.0-ratio)*sm_img[0]->GetComponent(i,j)+
				ratio*sm_img[1]->GetComponent(i,j));
	      rmap->SetComponent(i,j,100.0*ratio);
	    }
	}
    }

  smooth[0]->Delete();
  smooth[1]->Delete();

  this->UpdateProgress(1.0);
}

