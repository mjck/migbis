#include "vtkmpjFrameExtract.h"

#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"

#include <math.h>

vtkStandardNewMacro(vtkmpjFrameExtract);

//----------------------------------------------------------------------------
vtkmpjFrameExtract::vtkmpjFrameExtract()
{
  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(1);

  this->StartFrame = 0;
  this->EndFrame = 0;
}

//----------------------------------------------------------------------------
void vtkmpjFrameExtract::SetFrameInterval(int start, int end)
{
  if (this->StartFrame != start && this->EndFrame != end)
    {
      this->StartFrame = start;
      this->EndFrame = end;
      this->Modified();
    }
}

//----------------------------------------------------------------------------
void vtkmpjFrameExtract::SetFrameNumber(int frame)
{
  if (this->StartFrame != frame && this->EndFrame != frame)
    {
      this->StartFrame = this->EndFrame = frame;
      this->Modified();
    }
}   

//----------------------------------------------------------------------------
// This method tells the superclass that only one component will remain.
int vtkmpjFrameExtract::RequestInformation (
  vtkInformation       * vtkNotUsed( request ),
  vtkInformationVector ** vtkNotUsed( inputVector ), 
  vtkInformationVector * outputVector)
{
  vtkDataObject::SetPointDataActiveScalarInfo(
    outputVector->GetInformationObject(0), -1, this->EndFrame - this->StartFrame + 1);
  return 1;
}

//----------------------------------------------------------------------------
template <class T>
void vtkmpjFrameExtractExecute(vtkmpjFrameExtract *self,
                                      vtkImageData *inData, T *inPtr,
                                      vtkImageData *outData, T *outPtr,
                                      int outExt[6], int id)
{
  int idxR, idxY, idxZ;
  int maxX, maxY, maxZ;
  vtkIdType inIncX, inIncY, inIncZ;
  vtkIdType outIncX, outIncY, outIncZ;
  int cnt, inCnt;
  int start, end, offset;
  unsigned long count = 0;
  unsigned long target;
  
  // find the region to loop over
  maxX = outExt[1] - outExt[0];
  maxY = outExt[3] - outExt[2]; 
  maxZ = outExt[5] - outExt[4];
  target = static_cast<unsigned long>((maxZ+1)*(maxY+1)/50.0);
  target++;
  
  // Get increments to march through data 
  inData->GetContinuousIncrements(outExt, inIncX, inIncY, inIncZ);
  outData->GetContinuousIncrements(outExt, outIncX, outIncY, outIncZ);

  cnt = outData->GetNumberOfScalarComponents();
  inCnt = inData->GetNumberOfScalarComponents();
  
  // Loop through output pixels
  start = self->GetStartFrame();
  end = self->GetEndFrame();

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
	  	  
	  for (idxR = 0; idxR <= maxX; idxR++)
	    {
	      // Pixel operation
	      offset = start; // starting component
	      while (offset <= end)
		{
		  *outPtr = *(inPtr + offset);
		  outPtr++;
		  offset++;
		}
	      inPtr += inCnt;
	    }
      
	  outPtr += outIncY;
	  inPtr += inIncY;
	}
      outPtr += outIncZ;
      inPtr += inIncZ;
    }
}


//----------------------------------------------------------------------------
// This method is passed input and output datas, and executes the
// ExtractComponents function on each line.  
void vtkmpjFrameExtract::ThreadedExecute (vtkImageData *inData, 
                                                vtkImageData *outData,
                                                int outExt[6], int id)
{
  int max, idx;
  void *inPtr = inData->GetScalarPointerForExtent(outExt);
  void *outPtr = outData->GetScalarPointerForExtent(outExt);
  
  // this filter expects that input is the same type as output.
  if (inData->GetScalarType() != outData->GetScalarType())
    {
    vtkErrorMacro(<< "Execute: input ScalarType, " 
                  << inData->GetScalarType()
                  << ", must match out ScalarType " 
                  << outData->GetScalarType());
    return;
    }
  
  // make sure we can get all of the components.
  max = inData->GetNumberOfScalarComponents();
  for (idx = this->StartFrame; idx <= this->EndFrame; ++idx)
    {
    if (idx >= max || idx < 0)
      {
      vtkErrorMacro("Execute: Component " << idx
                    << " is not in input.");
      return;
      }
    }
  
  // choose which templated function to call.
  switch (inData->GetScalarType())
    {
    vtkTemplateMacro(
      vtkmpjFrameExtractExecute(this, inData, 
                                       static_cast<VTK_TT *>(inPtr), outData, 
                                       static_cast<VTK_TT *>(outPtr),
                                       outExt, id));
    default:
      vtkErrorMacro(<< "Execute: Unknown ScalarType");
      return;
    }
}

void vtkmpjFrameExtract::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "StartFrame: " << this->StartFrame << endl;
  os << indent << "EndFrame: " << this->EndFrame << endl;
}

