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
  Module:    $RCSfile: vtkpxAppendComponents.cpp,v $
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
#include "vtkpxAppendComponents.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
//------------------------------------------------------------------------------
vtkpxAppendComponents* vtkpxAppendComponents::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxAppendComponents");
  if(ret)
    {
    return (vtkpxAppendComponents*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxAppendComponents;
}

// Construct object with no children.
vtkpxAppendComponents::vtkpxAppendComponents()
{

}
// ----------------------------------------------------------------------------
vtkpxAppendComponents::~vtkpxAppendComponents()
{

}
// ----------------------------------------------------------------------------
void vtkpxAppendComponents::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();

  int dim_0[3];
  this->GetImageDataInput(0)->GetDimensions(dim_0);
  int numtotal=this->GetImageDataInput(0)->GetNumberOfScalarComponents();

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

	    if (sum!=0)
	      {
		vtkErrorMacro(<<"Multiple Images have different Dimensions !!");
		return;
	      }

	    numtotal+=this->GetImageDataInput(i)->GetNumberOfScalarComponents();
	  }
      }

  output->SetNumberOfScalarComponents(numtotal);
}

void vtkpxAppendComponents::SimpleExecute(vtkImageData* ,vtkImageData* output)
{
  if (this->GetNumberOfInputPorts()==0 || this->GetImageDataInput(0)==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }

  int dim_0[3];
  this->GetImageDataInput(0)->GetDimensions(dim_0);
  int num_comp=this->GetImageDataInput(0)->GetNumberOfScalarComponents();

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
	    if (sum!=0)
	      {
		vtkErrorMacro(<<"Multiple Images have different Dimensions !!");
		return;
	      }
    	    num_comp+=this->GetImageDataInput(i)->GetNumberOfScalarComponents();
	  }
      }


  // Create Output Array
  // -------------------
  output->CopyStructure(this->GetImageDataInput(0));
  output->SetNumberOfScalarComponents(num_comp);
  output->AllocateScalars();
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  int numscalars=out->GetNumberOfTuples();

  this->UpdateProgress(0.01);
  float pog=0.00;

  int component=0;
  for (int i=0;i<this->GetNumberOfInputPorts();i++)
    {
      vtkDataArray* inp=this->GetImageDataInput(i)->GetPointData()->GetScalars();
      for (int c=0;c<inp->GetNumberOfComponents();c++)
	{
	  out->CopyComponent(component,inp,c);
	  ++component;
	}
      this->UpdateProgress(float(component)/float(num_comp));
    }
    
  this->UpdateProgress(1.0);
}


