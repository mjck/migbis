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





#include "vtkpxImageReplaceSlice.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include <stdio.h>

vtkpxImageReplaceSlice* vtkpxImageReplaceSlice::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageReplaceSlice");
  if(ret)
    {
      return (vtkpxImageReplaceSlice*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageReplaceSlice;
}


// Construct object to extract all of the input data.
vtkpxImageReplaceSlice::vtkpxImageReplaceSlice()
{
  this->InternalCurrentPlane=2; 
  this->SliceNo=0;
  this->Frame=0;
  this->InsertedSlice=NULL;
  this->DeepCopyMode=1;
}

// ---------------------------------------------------------------------------

void vtkpxImageReplaceSlice::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkpxSimpleImageToImageFilter::PrintSelf(os,indent);

  os << indent << "  InternalCurrentPlane " << this->InternalCurrentPlane << "\n";
  os << indent << "  SliceNo " << this->SliceNo << "\n";
  os << indent << "  Frame " << this->Frame << "\n";
  os << indent << "  DeepCopyMode " << this->DeepCopyMode << "\n";

  this->SetInsertedSlice(NULL);
}
// ---------------------------------------------------------------------------

void vtkpxImageReplaceSlice::SetCurrentPlane(int a)
{
  this->SetInternalCurrentPlane(a);
}

int  vtkpxImageReplaceSlice::GetCurrentPlane()
{
  return this->InternalCurrentPlane;
}

// ---------------------------------------------------------------------------
void vtkpxImageReplaceSlice::SimpleExecute(vtkImageData* input, vtkImageData* output) 
{
  if (input==NULL || this->InsertedSlice==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxImageReplaceSlice\n");
      return;
    }

  // --------------------- Check Dimensions ------------------------------------
  if (this->InternalCurrentPlane<0)
      this->InternalCurrentPlane=0;
  else if (this->InternalCurrentPlane>2)
      this->InternalCurrentPlane=2;


  int dim[3];   input->GetDimensions(dim);
  int dim2[3];  this->InsertedSlice->GetDimensions(dim2);
  int targetdim[3];

  


  targetdim[2]=  1;
  switch (this->InternalCurrentPlane)
    {
    case 2:
      targetdim[0] = dim[0];
      targetdim[1] = dim[1];
      break;
    case 1:
      targetdim[0] = dim[0];
      targetdim[1] = dim[2];
      break;
    case 0:
      targetdim[0] = dim[1];
      targetdim[1] = dim[2];
      break;
    }
  
  int nc=this->InsertedSlice->GetNumberOfScalarComponents();

  if (targetdim[0]!=dim2[0] || targetdim[1]!=dim2[1] || targetdim[2]!=dim2[2])
    {
      vtkErrorMacro(<<"Cannot Insert Slice, Bad Dimensions\n");
      return;
    }


  if (this->SliceNo<0)
    this->SliceNo=0;
  else if (this->SliceNo>=dim[this->InternalCurrentPlane])
    this->SliceNo=dim[this->InternalCurrentPlane]-1;

  if (this->Frame<0)
    this->Frame=0;
  if ( this->Frame>=input->GetNumberOfScalarComponents())
    this->Frame=input->GetNumberOfScalarComponents()-1;

  vtkDebugMacro(<< "Inserting Slice");

  if (this->DeepCopyMode)
    output->DeepCopy(input);
  else
    output->GetPointData()->SetScalars(input->GetPointData()->GetScalars());

  vtkDataArray* slarr=this->InsertedSlice->GetPointData()->GetScalars();
  int index=0;

  switch(this->InternalCurrentPlane)
      {
      case 2:
	{
	  int offset=dim[0]*dim[1]*this->SliceNo;
	  for (int j=0;j<dim[1];j++)
	    for (int i=0;i<dim[0];i++)
	      {
		output->SetScalarComponentFromDouble(i,j,this->SliceNo,this->Frame,
						     slarr->GetComponent(index,0));
		++index;
	      }
	}
	break;

      case 1:
	{
	  for (int k=0;k<dim[2];k++)
	    for (int i=0;i<dim[0];i++)
	      {
		output->SetScalarComponentFromDouble(i,this->SliceNo,k,this->Frame,
						     slarr->GetComponent(index,0));
		++index;
	      }
	}
	break;
	
      case 0:
	{
	  for (int k=0;k<dim[2];k++)
	    for (int j=0;j<dim[1];j++)
	      {
		output->SetScalarComponentFromDouble(this->SliceNo,j,k,this->Frame,
						     slarr->GetComponent(index,0));
		++index;
	      }
	}
	break;
      }
  
  output->GetPointData()->GetScalars()->Modified();
  double* r=output->GetPointData()->GetScalars()->GetRange(0);
}

