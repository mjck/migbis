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
  Module:    $RCSfile: vtkpxMaximumImages.cpp,v $
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
#include "vtkSource.h"
#include "vtkpxMaximumImages.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
//------------------------------------------------------------------------------
vtkpxMaximumImages* vtkpxMaximumImages::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxMaximumImages");
  if(ret)
    {
    return (vtkpxMaximumImages*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxMaximumImages;
}

// Construct object with no children.
vtkpxMaximumImages::vtkpxMaximumImages()
{
  this->OutputIndex=0;
  this->DoMinimum=0;
}
// ----------------------------------------------------------------------------
vtkpxMaximumImages::~vtkpxMaximumImages()
{
}
// ----------------------------------------------------------------------------
void vtkpxMaximumImages::SimpleExecute(vtkImageData* ,vtkImageData* output)
{

  if (this->GetNumberOfInputPorts()==0 || this->GetImageDataInput(0)==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }

  int dim_0[3];
  this->GetImageDataInput(0)->GetDimensions(dim_0);
  int num_comp=this->GetImageDataInput(0)->GetNumberOfScalarComponents();

  fprintf(stderr, "Number of inputs: %d\n", this->GetNumberOfInputPorts());

  for (int i=1;i<this->GetNumberOfInputPorts();i++)
      {
	if (this->GetImageDataInput(i)==NULL)
	  {
	    vtkErrorMacro(<<"No Input Image Data " << i << " !!");
	    return;
	  }
	else
	  {
	    int dim_1[3];
	    this->GetImageDataInput(i)->GetDimensions(dim_1);
	    int sum=0;
	    for (int kk=0;kk<=2;kk++)
	      sum+=abs(dim_1[kk]-dim_0[kk]);
	    sum+=abs(num_comp-this->GetImageDataInput(i)->GetNumberOfScalarComponents());
	    if (sum!=0)
	      {
		vtkErrorMacro(<<"Multiple Images have different Dimensions !!");
		return;
	      }
	  }
      }


  // Create Output Array
  // -------------------
  int   dim[3];  this->GetImageDataInput(0)->GetDimensions(dim);
  vtkDebugMacro(<< "Maximum Value from Images\n" <<  "Dimensions " << dim[0] <<","<< dim[1] <<","<< dim[2] << "Components " << num_comp << "\n");

  output->CopyStructure(this->GetImageDataInput(0));
  output->SetNumberOfScalarComponents(this->GetImageDataInput(0)->GetNumberOfScalarComponents());
  output->AllocateScalars();

  vtkDataArray* out=output->GetPointData()->GetScalars();
  int numscalars=out->GetNumberOfTuples();

  this->UpdateProgress(0.01);
  float pog=0.00;

  int tenth=(numscalars*num_comp)/10;
  int count=0;

  //  int domean=1;
  for (int component=0;component<num_comp;component++)
    {
      for (int n=0;n<numscalars;n++)
	{
	  float max=this->GetImageDataInput(0)->GetPointData()->GetScalars()->GetComponent(n,component);
	  int bestindex=0;
	  for (int series=1;series<this->GetNumberOfInputPorts();series++)
	    {
	      float v=this->GetImageDataInput(series)->GetPointData()->GetScalars()->GetComponent(n,component);
	      if ( this->DoMinimum)
		{
		  if (v<max)
		    {
		      max=v;
		      bestindex=series;
		    }
		} 
	      else if (v>max)
		{
		  max=v;
		  bestindex=series;
		}
	    }

	  if (this->OutputIndex)
	    out->SetComponent(n,component,bestindex+1);
	  else
	    out->SetComponent(n,component,max);
	}
      
      count++;
      if (count==tenth)
	{
	  pog+=0.1;
	  this->UpdateProgress(pog);
	  count=0;
	}
    }
  this->UpdateProgress(1.0);
}

