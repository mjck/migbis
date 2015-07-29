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
  Module:    $RCSfile: vtkpxImagePolarToEuclidean.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:02:52 $
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
#include <math.h>
#include "vtkpxImagePolarToEuclidean.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkpxMath.h"


//------------------------------------------------------------------------------
vtkpxImagePolarToEuclidean* vtkpxImagePolarToEuclidean::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImagePolarToEuclidean");
  if(ret)
    {
    return (vtkpxImagePolarToEuclidean*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImagePolarToEuclidean;
}






//----------------------------------------------------------------------------
vtkpxImagePolarToEuclidean::vtkpxImagePolarToEuclidean()
{
  this->Inverse=0;
}

//----------------------------------------------------------------------------
// This templated function executes the filter for any type of data.
template <class T>
static void vtkpxImagePolarToEuclideanExecute(vtkpxImagePolarToEuclidean *self,
					      vtkImageData *inData, T *inPtr,
					      vtkImageData *outData, T *outPtr,
					      int outExt[6], int id)
{
  int idxX, idxY, idxZ;
  int maxC, maxX, maxY, maxZ;
  vtkIdType inIncX, inIncY, inIncZ;
  vtkIdType outIncX, outIncY, outIncZ;
  unsigned long count = 0;
  unsigned long target;
  float X, Y, Theta, R;
  int   inverse=self->GetInverse();

  // find the region to loop over
  maxC = inData->GetNumberOfScalarComponents();
  maxX = outExt[1] - outExt[0]; 
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

	  /*if ( !( (idxY + outExt[2]) & 1 ))
	    fprintf(stderr,"Even Number %d\n",idxY);*/

	  for (idxX = 0; idxX <= maxX; idxX++)
	    {
	      if (inverse==0)
		{
		  // Pixel operation
		  Theta = (float)(*inPtr);
		  R = (float)(inPtr[1]);
		  
		  X=R*sin(Theta);
		  Y=R*cos(Theta);
		  
		  *outPtr = (T)(X);
		  outPtr[1] = (T)(Y);
		}
	      else
		{
		  X=(float)(*inPtr);
		  Y=(float)(inPtr[1]);
		  vtkpxMath::EuclideanToPolar(X,Y,R,Theta);
		  *outPtr= (T)(Theta);
		  outPtr[1]=(T)(R);
		}
	      inPtr += maxC;
	      outPtr += maxC;
	    }
	  outPtr += outIncY;
	  inPtr += inIncY;
	}
      outPtr += outIncZ;
      inPtr += inIncZ;
    }
}

//----------------------------------------------------------------------------
void vtkpxImagePolarToEuclidean::ThreadedExecute(vtkImageData *inData, 
						 vtkImageData *outData,
						 int outExt[6], int id)
{
  void *inPtr = inData->GetScalarPointerForExtent(outExt);
  void *outPtr = outData->GetScalarPointerForExtent(outExt);
  
  vtkDebugMacro(<< "Execute: inData = " << inData 
		<< ", outData = " << outData);
  
  // this filter expects that input is the same type as output.
  if (inData->GetScalarType() != outData->GetScalarType())
    {
      vtkErrorMacro(<< "Execute: input ScalarType, " << inData->GetScalarType()
      << ", must match out ScalarType " << outData->GetScalarType());
      return;
    }
  
  // input must have at least two components
  if (inData->GetNumberOfScalarComponents() < 2)
    {
      vtkErrorMacro(<< "Execute: input does not have at least two components");
      return;
    }
  
  switch (inData->GetScalarType())
    {
      vtkTemplateMacro7(vtkpxImagePolarToEuclideanExecute, this, 
			inData, (VTK_TT *)(inPtr), 
			outData, (VTK_TT *)(outPtr), outExt, id);
    default:
      vtkErrorMacro(<< "Execute: Unknown ScalarType");
      return;
    }
}

void vtkpxImagePolarToEuclidean::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkImageToImageFilter::PrintSelf(os,indent);
  os << indent << "Value of Pi: " << this->ValueOfPi << "\n";
  os << indent << "Inverse    : " << this->Inverse   << "\n";
}

