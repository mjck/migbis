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

#include "vtkbisImageTimeSeriesNormalize.h"

#include "vtkImageData.h"
#include "vtkImageProgressIterator.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "pxisinf.h"


vtkCxxRevisionMacro(vtkbisImageTimeSeriesNormalize, "$Revision: 1.54 $");
vtkStandardNewMacro(vtkbisImageTimeSeriesNormalize);

//----------------------------------------------------------------------------
vtkbisImageTimeSeriesNormalize::vtkbisImageTimeSeriesNormalize()
{
  //  this->NormalizeMean=1;
  //  this->NormalizeSigma=1;
}

//----------------------------------------------------------------------------
vtkbisImageTimeSeriesNormalize::~vtkbisImageTimeSeriesNormalize()
{
}

//----------------------------------------------------------------------------
void vtkbisImageTimeSeriesNormalize::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  //  os << indent << "NormalizeMean: " << this->NormalizeMean << "\n";
  //  os << indent << "NormalizeSigma: " << this->NormalizeSigma << "\n";

}

//----------------------------------------------------------------------------
int vtkbisImageTimeSeriesNormalize::RequestInformation(vtkInformation*,
                                           vtkInformationVector**,
                                           vtkInformationVector* outputVector)
{
  vtkInformation* outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
  return 1;
}

//----------------------------------------------------------------------------
// This function template implements the filter for any type of data.
// The last two arguments help the vtkTemplateMacro calls below
// instantiate the proper input and output types.
template <class IT, class OT>
void vtkbisImageTimeSeriesNormalizeExecute(vtkbisImageTimeSeriesNormalize* self,
                               vtkImageData* inData,
                               vtkImageData* outData,
                               int outExt[6], int id,
                               IT*, OT*)
{
  // Create iterators for the input and output extents assigned to
  // this thread.
  vtkImageIterator<IT> inIt(inData, outExt);
  vtkImageProgressIterator<OT> outIt(outData, outExt, self, id);

  // Get the shift and scale parameters values.
  //  int domean=self->GetNormalizeMean();
  //  int dosigma=self->GetNormalizeSigma();

  // Clamp pixel values within the range of the output type.

  int maxC = inData->GetNumberOfScalarComponents();

  double scalefactor=1.0;
  scalefactor=1.0/double(maxC);

  // Loop through output pixels.
  while (!outIt.IsAtEnd())
    {
      IT* inSI = inIt.BeginSpan();
      OT* outSI = outIt.BeginSpan();
      OT* outSIEnd = outIt.EndSpan();

      while (outSI != outSIEnd)
	{
	  IT* inSItemp=inSI;
	  double sum=0.0,sum2=0.0;
	  for (int idxC = 0; idxC < maxC; idxC++)
	    {
	      // Pixel operation
	      double v=(static_cast<double>(*inSItemp));
	      sum+=v;
	      sum2+=v*v;
	      ++inSItemp;
	    }
	  double mean=sum*scalefactor;
	  double sigma=sqrt(sum2*scalefactor-mean*mean);

	  if (sigma<0.00001)
	    sigma=1.0;

	  if (isnan(sigma) || isinf(sigma))
	    sigma=1.0;
	  if (isnan(mean) || isinf(mean))
	    mean=0.0;


	  for (int idxC = 0; idxC < maxC; idxC++)
	    {
	      // Pixel operation
	      double v=(static_cast<double>(*inSI));
	      //cout<<"oriV:"<<v;
	      v= (v-mean)/sigma;
	      *outSI = static_cast<OT>(v);
	      //cout<<"newV:"<<v<<"mean:"<<mean<<"sigma:"<<sigma<<" | ";
	      ++outSI;
	      ++inSI;
	    }
	}
      inIt.NextSpan();
      outIt.NextSpan();
    }
}

//----------------------------------------------------------------------------
template <class T>
void vtkbisImageTimeSeriesNormalizeExecute1(vtkbisImageTimeSeriesNormalize* self,
                                vtkImageData* inData,
                                vtkImageData* outData,
                                int outExt[6], int id, T*)
{
  switch (outData->GetScalarType())
    {
    vtkTemplateMacro(
      vtkbisImageTimeSeriesNormalizeExecute(self, inData,
                                outData, outExt, id,
                                static_cast<T*>(0),
                                static_cast<VTK_TT*>(0)));
    default:
      vtkErrorWithObjectMacro(
        self, "ThreadedRequestData: Unknown output ScalarType");
      return;
    }
}

//----------------------------------------------------------------------------
// This method is passed a input and output data, and executes the filter
// algorithm to fill the output from the input.
// It just executes a switch statement to call the correct function for
// the datas data types.
void vtkbisImageTimeSeriesNormalize::ThreadedRequestData(vtkInformation*,
                                             vtkInformationVector**,
                                             vtkInformationVector*,
                                             vtkImageData*** inData,
                                             vtkImageData** outData,
                                             int outExt[6],
                                             int threadId)
{
  vtkImageData* input = inData[0][0];
  vtkImageData* output = outData[0];
  switch(input->GetScalarType())
    {
    vtkTemplateMacro(
      vtkbisImageTimeSeriesNormalizeExecute1(this, input, output, outExt, threadId,
                                 static_cast<VTK_TT*>(0)));
    default:
      vtkErrorMacro("ThreadedRequestData: Unknown input ScalarType");
      return;
    }
}

