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
  Module:    $RCSfile: vtkpxImageIDFilter.h,v $
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

#include "vtkpxImageIDFilter.h"
#include "vtkObjectFactory.h"
#include "vtkpxUtil.h"
#include "vtkImageToImageFilter.h"
#include "vtkIdList.h"
//------------------------------------------------------------------------------
vtkpxImageIDFilter* vtkpxImageIDFilter::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageIDFilter");
  if(ret)
    {
    return (vtkpxImageIDFilter*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageIDFilter;
}

//----------------------------------------------------------------------------
// Constructor sets default values
vtkpxImageIDFilter::vtkpxImageIDFilter()
{
  this->OutputScalarType = VTK_UNSIGNED_CHAR;
  this->IdList=NULL;
  this->InValue=1.0;
  this->OutValue=0.0;
}

vtkpxImageIDFilter::~vtkpxImageIDFilter()
{
  this->SetIdList(NULL);
}

//----------------------------------------------------------------------------
void vtkpxImageIDFilter::ExecuteInformation(vtkImageData *inData, 
					    vtkImageData *outData)
{
  this->vtkImageToImageFilter::ExecuteInformation( inData, outData );

  if (this->OutputScalarType != -1)
    outData->SetScalarType(this->OutputScalarType);
}

//----------------------------------------------------------------------------
// This templated function executes the filter for any type of data.
template <class IT, class OT>
static void vtkpxImageIDFilterExecute(vtkpxImageIDFilter *self,
				      vtkImageData *inData, IT *inPtr,
				      vtkImageData *outData, OT *outPtr,
				      int outExt[6], int id)
{

  vtkIdList* IdList=self->GetIdList();
  float InValue=self->GetInValue();
  float OutValue=self->GetOutValue();

  int idxR, idxY, idxZ;
  int maxY, maxZ;
  vtkIdType inIncX, inIncY, inIncZ;
  vtkIdType outIncX, outIncY, outIncZ;
  int rowLength;
  unsigned long count = 0;
  unsigned long target;


  // find the region to loop over
  rowLength = (outExt[1] - outExt[0]+1)*inData->GetNumberOfScalarComponents();
  maxY = outExt[3] - outExt[2]; 
  maxZ = outExt[5] - outExt[4];
  target = (unsigned long)((maxZ+1)*(maxY+1)/50.0);
  target++;
  
  // Get increments to march through data 
  inData->GetContinuousIncrements(outExt, inIncX, inIncY, inIncZ);
  outData->GetContinuousIncrements(outExt, outIncX, outIncY, outIncZ);
  
  // Loop through ouput pixels
  for (idxZ = 0; idxZ <= maxZ; idxZ++)
    {
    for (idxY = 0; !self->AbortExecute && idxY <= maxY; idxY++)
      {
	if (!id) 
	  {
	    if (!(count%target))
	      {
		self->UpdateProgress(count/(50.0*target));
	      }
	    count++;
	  }
	
	for (idxR = 0; idxR < rowLength; idxR++)
	  {
	  // Pixel operation
	    vtkIdType val = ((vtkIdType)(*inPtr)); 
	    if (IdList->IsId(val)>=0)
	      *outPtr = (OT)(InValue);
	    else
	      *outPtr = (OT)(OutValue);
	    outPtr++;
	    inPtr++;
	  }
	
	outPtr += outIncY;
	inPtr += inIncY;
      }
    outPtr += outIncZ;
    inPtr += inIncZ;
    }
}
//----------------------------------------------------------------------------
// This method is passed a input and output data, and executes the filter
// algorithm to fill the output from the input.
// It just executes a switch statement to call the correct function for
// the datas data types.
void vtkpxImageIDFilter::ThreadedExecute(vtkImageData *inData, 
					vtkImageData *outData,
					int outExt[6], int id)
{
  void *inPtr = inData->GetScalarPointerForExtent(outExt);
  void *outPtr = outData->GetScalarPointerForExtent(outExt);
  
  vtkIdList* Map=this->GetIdList();
  if (Map->GetNumberOfIds()<1)
    {
      vtkErrorMacro(<<"Bad ID List number of ids="<<Map->GetNumberOfIds()<<"<1");
      return;
    }



  switch (inData->GetScalarType())
    {
      vtkTemplateMacro7(vtkpxImageIDFilterExecute,this, 
			inData, (VTK_TT *)(inPtr), 
			outData,(VTK_TT *)(outPtr), 
			outExt, id);

    default:
      vtkErrorMacro(<< "Execute: Unknown ScalarType");
      return;
    }
}



void vtkpxImageIDFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkImageToImageFilter::PrintSelf(os,indent);
  os << indent << "IdList: "<<this->IdList;
  os << indent << "InValue: "<<this->InValue;
  os << indent << "OutValue: "<<this->OutValue;
  os << indent << "Output Scalar Type: " << this->OutputScalarType << "\n";
}



