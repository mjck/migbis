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
  Module:    $RCSfile: vtkpxImageComponentsToFrames.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/28 16:17:31 $
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
#include "vtkpxImageComponentsToFrames.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "math.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

//------------------------------------------------------------------------------
vtkpxImageComponentsToFrames* vtkpxImageComponentsToFrames::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageComponentsToFrames");
  if(ret)
    {
    return (vtkpxImageComponentsToFrames*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageComponentsToFrames;
}

// Construct object with no children.
vtkpxImageComponentsToFrames::vtkpxImageComponentsToFrames()
{
  this->Inverse=0;
  this->SlicesPerFrame=16;
}
// ----------------------------------------------------------------------------
vtkpxImageComponentsToFrames::~vtkpxImageComponentsToFrames()
{
}
// ----------------------------------------------------------------------------
void vtkpxImageComponentsToFrames::ExecuteInformation()
{
  vtkImageData* input=vtkImageData::SafeDownCast(this->GetImageDataInput(0));

  if (input==NULL)
    {
	vtkErrorMacro(<<"No Input Specified!!");
	return;
      }
  vtkImageData *output=this->GetOutput();
  vtkpxSimpleImageToImageFilter::ExecuteInformation();


  int dim[3];  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();

  if (this->Inverse==1)
    {
      int numfr=dim[2]/this->SlicesPerFrame;
      if (numfr<1)
	numfr=1;
      output->SetNumberOfScalarComponents(numfr);
      output->SetDimensions(dim[0],dim[1],this->SlicesPerFrame);
      output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,this->SlicesPerFrame-1);
      output->SetExtent(0,dim[0]-1,0,dim[1]-1,0,this->SlicesPerFrame-1);
      output->SetUpdateExtent(0,dim[0]-1,0,dim[1]-1,0,this->SlicesPerFrame-1);
    }
  else
    {
      output->SetNumberOfScalarComponents(1);
      output->SetDimensions(dim[0],dim[1],dim[2]*nc);
      output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]*nc-1);
      output->SetExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]*nc-1);
      output->SetUpdateExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]*nc-1);
    }

}
// ----------------------------------------------------------------------------
void vtkpxImageComponentsToFrames::SimpleExecute(vtkImageData* input, vtkImageData* output) 
{
  vtkpxImageComponentsToFrames::ExecuteInformation();

  vtkDataArray* in =input->GetPointData()->GetScalars();
  int dim[3];  input->GetDimensions(dim);

  if (this->Inverse==0)
    {
      int nc=input->GetNumberOfScalarComponents();
      /*      output->CopyStructure(input);
      output->SetNumberOfScalarComponents(1);
      output->SetDimensions(dim[0],dim[1],dim[2]*nc);
      output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]*nc-1);
      output->AllocateScalars();*/


      vtkDataArray* out=output->GetPointData()->GetScalars();

      for (int comp=0;comp<nc;comp++)
	{
	  int offset=comp*dim[0]*dim[1]*dim[2];
	  int index=0;
	  for (int k=0;k<dim[2];k++)
	    for (int j=0;j<dim[1];j++)
	      for (int i=0;i<dim[0];i++)
		{
		  out->SetComponent(offset+index,0,in->GetComponent(index,comp));
		  ++index;
		}
	}
    }
  else
    {
      int numfr=dim[2]/this->SlicesPerFrame;
      if (numfr<1)
	numfr=1;
      /*      output->CopyStructure(input);
      output->SetNumberOfScalarComponents(numfr);
      output->SetDimensions(dim[0],dim[1],this->SlicesPerFrame);
      output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,this->SlicesPerFrame-1);
      output->AllocateScalars();*/

      int d[3]; output->GetDimensions(d); fprintf(stderr,"input (%d,%d,%d) %d --> (%d,%d,%d) %d\n",
						  dim[0],dim[1],dim[2],1,d[0],d[1],d[2],numfr);

      vtkDataArray* out=output->GetPointData()->GetScalars();
      
      dim[2]=this->SlicesPerFrame;

      for (int comp=0;comp<numfr;comp++)
	{
	  int offset=comp*dim[0]*dim[1]*dim[2];
	  int index=0;
	  for (int k=0;k<dim[2];k++)
	    for (int j=0;j<dim[1];j++)
	      for (int i=0;i<dim[0];i++)
		{
		  out->SetComponent(index,comp,in->GetComponent(index+offset,0));
		  ++index;
		}
	}
    }
  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------

