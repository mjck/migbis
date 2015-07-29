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
  Module:    $RCSfile: vtkpxImageAttenuationCorrection.cpp,v $
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
#include "vtkpxImageAttenuationCorrection.h"
#include "vtkpxUtil.h"
#include "vtkIntArray.h"
#include "vtkImageGaussianSmooth.h"


//------------------------------------------------------------------------------
vtkpxImageAttenuationCorrection* vtkpxImageAttenuationCorrection::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageAttenuationCorrection");
  if(ret)
    {
    return (vtkpxImageAttenuationCorrection*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageAttenuationCorrection;
}

// Construct object with no children.
vtkpxImageAttenuationCorrection::vtkpxImageAttenuationCorrection()
{
  this->Threshold=1.0;
  this->Sigma=2.0;
  this->Factor=0.999;
  this->AttenuationMap=NULL;
}

vtkpxImageAttenuationCorrection::~vtkpxImageAttenuationCorrection()
{
  if (this->AttenuationMap!=NULL)
    this->AttenuationMap->Delete();
}
// ----------------------------------------------------------------------------
void vtkpxImageAttenuationCorrection::SimpleExecute(vtkImageData* input,vtkImageData* output )
{

  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Input to vtkpxImageAttenuationCorrection");
      return;
    }

  double sp[3];    input->GetSpacing(sp);

  double gp[3];
  for (int ia=0;ia<=2;ia++)
    gp[ia]=2.0*sp[0]/sp[2];

  vtkImageGaussianSmooth* sm=vtkImageGaussianSmooth::New();
  sm->SetInput(input);
  sm->SetStandardDeviations(gp);
  sm->Update();

  fprintf(stderr,"Smoothing Done\n");

  this->AttenuationMap=vtkImageData::New();
  this->AttenuationMap->CopyStructure(input);
  this->AttenuationMap->SetScalarTypeToFloat();
  this->AttenuationMap->SetNumberOfScalarComponents(1);
  this->AttenuationMap->AllocateScalars();

  vtkImageData* tmp=sm->GetOutput();
  int dim[3];   tmp->GetDimensions(dim);
  vtkDataArray* tmp_arr=tmp->GetPointData()->GetScalars();
  vtkDataArray* out_arr=output->GetPointData()->GetScalars();
  vtkDataArray* in_arr=input->GetPointData()->GetScalars();
  vtkDataArray* at_arr=this->AttenuationMap->GetPointData()->GetScalars();

  int slicesize=dim[0]*dim[1];
  double* factor=new double[slicesize];
  fprintf(stderr,"Correcting : ");
  for (int slice=0;slice<dim[2];slice++)
    {
      fprintf(stderr,"%d ",slice);
      int offset=slice*slicesize;
      for (int pixel=0;pixel<slicesize;pixel++)
	{
	  int index=offset+pixel;
	  if (slice==0)
	    {
	      factor[pixel]=1.0;
	    }
	  else
	    {
	      if ( tmp_arr->GetComponent(index-slicesize,0)>this->Threshold)
		factor[pixel]*=this->Factor;
	    }
	  out_arr->SetComponent(index,0,in_arr->GetComponent(index,0)/factor[pixel]);
	  at_arr->SetComponent(index,0,factor[pixel]*100.0);
	}
    }

  delete [] factor;
  fprintf(stderr,"\n");
  sm->Delete();

}

