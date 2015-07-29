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
  Module:    $RCSfile: vtkpxBinaryImageMedian3D.cpp,v $
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
#include "vtkImageData.h"

#include "vtkpxBinaryImageMedian3D.h"
#include "vtkImageEllipsoidSource.h"
#include "vtkObjectFactory.h"



//------------------------------------------------------------------------------
vtkpxBinaryImageMedian3D* vtkpxBinaryImageMedian3D::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxBinaryImageMedian3D");
  if(ret)
    {
    return (vtkpxBinaryImageMedian3D*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxBinaryImageMedian3D;
}





//----------------------------------------------------------------------------
// Construct an instance of vtkpxBinaryImageMedian3D fitler.
// By default zero values are eroded.
vtkpxBinaryImageMedian3D::vtkpxBinaryImageMedian3D()
{
  this->HandleBoundaries = 1;
  this->KernelSize[0] = 1;
  this->KernelSize[1] = 1;
  this->KernelSize[2] = 1;

  this->Ellipse = vtkImageEllipsoidSource::New();
  // Setup the Ellipse to default size
  this->SetKernelSize(1, 1, 1);
}


//----------------------------------------------------------------------------
vtkpxBinaryImageMedian3D::~vtkpxBinaryImageMedian3D()
{
  if (this->Ellipse)
    {
    this->Ellipse->Delete();
    this->Ellipse = NULL;
    }
}


//----------------------------------------------------------------------------
void vtkpxBinaryImageMedian3D::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkImageSpatialFilter::PrintSelf(os,indent);
}

//----------------------------------------------------------------------------
// This method sets the size of the neighborhood.  It also sets the 
// default middle of the neighborhood and computes the eliptical foot print.
void vtkpxBinaryImageMedian3D::SetKernelSize(int size0, int size1, int size2)
{
  int modified = 0;
  
  if (this->KernelSize[0] != size0)
    {
    modified = 1;
    this->KernelSize[0] = size0;
    this->KernelMiddle[0] = size0 / 2;
    }
  if (this->KernelSize[1] != size1)
    {
    modified = 1;
    this->KernelSize[1] = size1;
    this->KernelMiddle[1] = size1 / 2;
    }
  if (this->KernelSize[2] != size2)
    {
    modified = 1;
    this->KernelSize[2] = size2;
    this->KernelMiddle[2] = size2 / 2;
    }

  if (modified)
    {
    this->Modified();
    this->Ellipse->SetWholeExtent(0, this->KernelSize[0]-1, 
				  0, this->KernelSize[1]-1, 
				  0, this->KernelSize[2]-1);
    this->Ellipse->SetCenter((float)(this->KernelSize[0]-1)*0.5,
			     (float)(this->KernelSize[1]-1)*0.5,
			     (float)(this->KernelSize[2]-1)*0.5);
    this->Ellipse->SetRadius((float)(this->KernelSize[0])*0.5,
			     (float)(this->KernelSize[1])*0.5,
			     (float)(this->KernelSize[2])*0.5);
    // make sure scalars have been allocated (needed if multithreaded is used)
    this->Ellipse->GetOutput()->SetUpdateExtent(0, this->KernelSize[0]-1, 
						0, this->KernelSize[1]-1, 
						0, this->KernelSize[2]-1);
    this->Ellipse->GetOutput()->Update();
    }
}


//----------------------------------------------------------------------------
// This templated function executes the filter on any region,
// whether it needs boundary checking or not.
// If the filter needs to be faster, the function could be duplicated
// for strictly center (no boundary ) processing.
template <class T>
static void vtkpxBinaryImageMedian3DExecute(vtkpxBinaryImageMedian3D *self,
					     vtkImageData *mask,
					     vtkImageData *inData, T *inPtr, 
					     vtkImageData *outData, 
					     int *outExt, T *outPtr, int id)
{
  int *kernelMiddle, *kernelSize;
  // For looping though output (and input) pixels.
  int outMin0, outMax0, outMin1, outMax1, outMin2, outMax2;
  int outIdx0, outIdx1, outIdx2;
  vtkIdType inInc0, inInc1, inInc2;
  vtkIdType outInc0, outInc1, outInc2;
  T *inPtr0, *inPtr1, *inPtr2;
  T *outPtr0, *outPtr1, *outPtr2;
  int numComps, outIdxC;
  // For looping through hood pixels
  int hoodMin0, hoodMax0, hoodMin1, hoodMax1, hoodMin2, hoodMax2;
  int hoodIdx0, hoodIdx1, hoodIdx2;
  T *hoodPtr0, *hoodPtr1, *hoodPtr2;
  // For looping through the mask.
  unsigned char *maskPtr, *maskPtr0, *maskPtr1, *maskPtr2;
  vtkIdType maskInc0, maskInc1, maskInc2;
  // The extent of the whole input image
  int inImageMin0, inImageMin1, inImageMin2;
  int inImageMax0, inImageMax1, inImageMax2;
  // to compute the range
  int numPlusPixels;
  int numMinusPixels;
  unsigned long count = 0;
  unsigned long target;

  // Get information to march through data
  inData->GetIncrements(inInc0, inInc1, inInc2); 
  self->GetInput()->GetWholeExtent(inImageMin0, inImageMax0, inImageMin1,
				   inImageMax1, inImageMin2, inImageMax2);
  outData->GetIncrements(outInc0, outInc1, outInc2); 
  outMin0 = outExt[0];   outMax0 = outExt[1];
  outMin1 = outExt[2];   outMax1 = outExt[3];
  outMin2 = outExt[4];   outMax2 = outExt[5];
  numComps = outData->GetNumberOfScalarComponents();
  
  
  // Get ivars of this object (easier than making friends)
  kernelSize = self->GetKernelSize();
  kernelMiddle = self->GetKernelMiddle();
  hoodMin0 = - kernelMiddle[0];
  hoodMin1 = - kernelMiddle[1];
  hoodMin2 = - kernelMiddle[2];
  hoodMax0 = hoodMin0 + kernelSize[0] - 1;
  hoodMax1 = hoodMin1 + kernelSize[1] - 1;
  hoodMax2 = hoodMin2 + kernelSize[2] - 1;

  // Setup mask info
  maskPtr = (unsigned char *)(mask->GetScalarPointer());
  mask->GetIncrements(maskInc0, maskInc1, maskInc2);
  
  // in and out should be marching through corresponding pixels.
  inPtr = (T *)(inData->GetScalarPointer(outMin0, outMin1, outMin2));

  target = (unsigned long)(numComps*(outMax2-outMin2+1)*
			   (outMax1-outMin1+1)/50.0);
  target++;
  
  // loop through components
  for (outIdxC = 0; outIdxC < numComps; ++outIdxC)
    {
      // loop through pixels of output
      outPtr2 = outPtr;
      inPtr2 = inPtr;
      for (outIdx2 = outMin2; outIdx2 <= outMax2; ++outIdx2)
	{
	  outPtr1 = outPtr2;
	  inPtr1 = inPtr2;
	  for (outIdx1 = outMin1; 
	       !self->AbortExecute && outIdx1 <= outMax1; ++outIdx1)
	    {
	      if (!id) 
		{
		  if (!(count%target))
		    {
		      self->UpdateProgress(count/(50.0*target));
		    }
		  count++;
		}
	      outPtr0 = outPtr1;
	      inPtr0 = inPtr1;
	      for (outIdx0 = outMin0; outIdx0 <= outMax0; ++outIdx0)
		{
		  
		  // Find min
		  numMinusPixels=0;
		  numPlusPixels=0;
		  
		  // loop through neighborhood pixels
		  // as sort of a hack to handle boundaries, 
		  // input pointer will be marching through data that does not exist.
		  hoodPtr2 = inPtr0 - kernelMiddle[0] * inInc0 
		    - kernelMiddle[1] * inInc1 - kernelMiddle[2] * inInc2;
		  maskPtr2 = maskPtr;
		  for (hoodIdx2 = hoodMin2; hoodIdx2 <= hoodMax2; ++hoodIdx2)
		    {
		      hoodPtr1 = hoodPtr2;
		      maskPtr1 = maskPtr2;
		      for (hoodIdx1 = hoodMin1; hoodIdx1 <= hoodMax1; ++hoodIdx1)
			{
			  hoodPtr0 = hoodPtr1;
			  maskPtr0 = maskPtr1;
			  for (hoodIdx0 = hoodMin0; hoodIdx0 <= hoodMax0; ++hoodIdx0)
			    {
			      // A quick but rather expensive way to handle boundaries
			      if ( outIdx0 + hoodIdx0 >= inImageMin0 &&
				   outIdx0 + hoodIdx0 <= inImageMax0 &&
				   outIdx1 + hoodIdx1 >= inImageMin1 &&
				   outIdx1 + hoodIdx1 <= inImageMax1 &&
				   outIdx2 + hoodIdx2 >= inImageMin2 &&
				   outIdx2 + hoodIdx2 <= inImageMax2)
				{
				  if (*maskPtr0)
				    {
				      if (*hoodPtr0 > 0.0)
					numPlusPixels++;
				      else
					numMinusPixels++;
				    }
				}
			      
			      hoodPtr0 += inInc0;
			      maskPtr0 += maskInc0;
			    }
			  hoodPtr1 += inInc1;
			  maskPtr1 += maskInc1;
			}
		      hoodPtr2 += inInc2;
		      maskPtr2 += maskInc2;
		    }
		  if ( numPlusPixels > numMinusPixels)
		    *outPtr0 = 1;
		  else
		    *outPtr0 = 0;
		  //		  fprintf(stderr,"NumPlus=%d, numMinus=%d\n",numPlusPixels,numMinusPixels);
		  
		  inPtr0 += inInc0;
		  outPtr0 += outInc0;
		}
	      inPtr1 += inInc1;
	      outPtr1 += outInc1;
	    }
	  inPtr2 += inInc2;
	  outPtr2 += outInc2;
	}
      ++inPtr;
      ++outPtr;
    }
}


		

//----------------------------------------------------------------------------
// This method contains the first switch statement that calls the correct
// templated function for the input and output Data types.
// It hanldes image boundaries, so the image does not shrink.
void vtkpxBinaryImageMedian3D::ThreadedExecute(vtkImageData *inData, 
						vtkImageData *outData, 
						int outExt[6], int id)
{
  void *inPtr = inData->GetScalarPointerForExtent(outExt);
  void *outPtr = outData->GetScalarPointerForExtent(outExt);
  vtkImageData *mask;

  // Error checking on mask
  this->Ellipse->GetOutput()->Update();
  mask = this->Ellipse->GetOutput();
  if (mask->GetScalarType() != VTK_UNSIGNED_CHAR)
    {
    vtkErrorMacro(<< "Execute: mask has wrong scalar type");
    return;
    }

  // this filter expects the output type to be same as input
  if (outData->GetScalarType() != inData->GetScalarType())
    {
    vtkErrorMacro(<< "Execute: output ScalarType, " 
      << vtkImageScalarTypeNameMacro(outData->GetScalarType())
      << " must match input scalar type");
    return;
    }
  
  switch (inData->GetScalarType())
    {
    vtkTemplateMacro8(vtkpxBinaryImageMedian3DExecute, this, mask, 
                      inData, (VTK_TT *)(inPtr), outData, outExt, 
                      (VTK_TT *)(outPtr),id);
    default:
      vtkErrorMacro(<< "Execute: Unknown ScalarType");
      return;
    }
}



