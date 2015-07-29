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
  Module:    $RCSfile: vtkpxImageHistogramEqualization.cpp,v $
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
#include "vtkpxImageHistogramEqualization.h"
#include "vtkpxUtil.h"
#include "vtkIntArray.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageAccumulate.h"
#include "vtkImageExtractComponents.h"
//------------------------------------------------------------------------------
vtkpxImageHistogramEqualization* vtkpxImageHistogramEqualization::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageHistogramEqualization");
  if(ret)
    {
    return (vtkpxImageHistogramEqualization*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageHistogramEqualization;
}

// Construct object with no children.
vtkpxImageHistogramEqualization::vtkpxImageHistogramEqualization()
{
  this->LowerThreshold=-1.0e+9;
  this->UpperThreshold= 1.0e+9;
  this->NumberOfBins=256;
}
// ----------------------------------------------------------------------------
void vtkpxImageHistogramEqualization::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{

  if (this->GetImageDataInput(0)==NULL)
    {
      vtkErrorMacro(<<"Bad Input to vtkpxImageHistogramEqualization");
      return;
    }

  // Compute Histogram with N-bins from Threshold to max
  // Compute Cumulative Histogram
  // Normalize Intensity

  int dim[3];
  this->GetImageDataInput(0)->GetDimensions(dim);

  int numcomp=input->GetNumberOfScalarComponents();
  int numvox=dim[0]*dim[1]*dim[2];

  vtkDataArray* inarray=input->GetPointData()->GetScalars();
  vtkDataArray* outarray=output->GetPointData()->GetScalars();


  vtkImageExtractComponents* comp=vtkImageExtractComponents::New();
  comp->SetInput(input);
  
 
  vtkIntArray* hist=vtkIntArray::New();
  hist->SetNumberOfComponents(1);


  vtkImageAccumulate* accum=vtkImageAccumulate::New(); 
  accum->SetInput(comp->GetOutput());

  this->UpdateProgress(0.01);


  
  for (int component=0;component<numcomp;component++)
    {
      comp->SetComponents(component);
      comp->Update();
      int numbins=this->NumberOfBins;


      double r[2]; comp->GetOutput()->GetPointData()->GetScalars()->GetRange(r);

      float pmin=Fmax(this->LowerThreshold,r[0]);
      float pmax=Fmin(this->UpperThreshold,r[1]);
      if (pmax<pmin)
	{
	  float t=pmax;
	  pmax=pmin;
	  pmin=t;
	}

      float prange=pmax-pmin;
      if (numbins>prange)
	numbins=(int)prange;
      float pspacing=prange/float(numbins);
      
      // Computes the Histogram
      accum->SetComponentExtent(0,numbins-1,0,0,0,0);
      accum->SetComponentOrigin(pmin,0,0);
      accum->SetComponentSpacing(pspacing,1,1);
      accum->Update();
     

      // Compute Cumulative Histogram
      if (hist->GetNumberOfTuples()!=numbins)
	hist->SetNumberOfTuples(numbins);
      hist->CopyComponent(0,accum->GetOutput()->GetPointData()->GetScalars(),0);

      for (int ia=1;ia<numbins;ia++)
	hist->SetComponent(ia,0,hist->GetComponent(ia,0)+hist->GetComponent(ia-1,0));
      float totalv=hist->GetComponent(numbins-1,0);

      // Re-normalize intensity 

      for (int i=0;i<numvox;i++)
	{
	  float v=inarray->GetComponent(i,component);
	  if (v<=pmin)
	    {
	      v=0;
	    }
	  else if (v>=pmax)
	    {
	      v=numbins-1;
	    }
	  else
	    {
	      int index=int(0.5+float(numbins-1)*(v-pmin)/prange);
	      float numv=hist->GetComponent(index,0);
	      v=numv/totalv*float(numbins-1);
	    }
	  outarray->SetComponent(i,component,v);
	}
      this->UpdateProgress(float(component+1)/float(numcomp));      
    }
	      
  hist->Delete();
  comp->Delete();
  accum->Delete();

  this->UpdateProgress(1.0);
}

