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
  Module:    $RCSfile: vtkpxSurfaceUtil.h,v $
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
#include <math.h>

#include "vtkpxImageGaussianNoiseSource.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"

//------------------------------------------------------------------------------
vtkpxImageGaussianNoiseSource* vtkpxImageGaussianNoiseSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageGaussianNoiseSource");
  if(ret)
    {
    return (vtkpxImageGaussianNoiseSource*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageGaussianNoiseSource;
}




//----------------------------------------------------------------------------
vtkpxImageGaussianNoiseSource::vtkpxImageGaussianNoiseSource()
{
  this->Magnitude = 2.0;
  this->NumberOfComponents= 1;
  this->WholeExtent[0]=0;   this->WholeExtent[1]=255;
  this->WholeExtent[2]=0;   this->WholeExtent[3]=255;
  this->WholeExtent[4]=0;   this->WholeExtent[5]=255;
}


//----------------------------------------------------------------------------
void vtkpxImageGaussianNoiseSource::SetWholeExtent(int xMin, int xMax, 
						   int yMin, int yMax,
						   int zMin, int zMax)
{
  int modified = 0;
  
  if (this->WholeExtent[0] != xMin)
    {
    modified = 1;
    this->WholeExtent[0] = xMin ;
    }
  if (this->WholeExtent[1] != xMax)
    {
    modified = 1;
    this->WholeExtent[1] = xMax ;
    }
  if (this->WholeExtent[2] != yMin)
    {
    modified = 1;
    this->WholeExtent[2] = yMin ;
    }
  if (this->WholeExtent[3] != yMax)
    {
    modified = 1;
    this->WholeExtent[3] = yMax ;
    }
  if (this->WholeExtent[4] != zMin)
    {
    modified = 1;
    this->WholeExtent[4] = zMin ;
    }
  if (this->WholeExtent[5] != zMax)
    {
    modified = 1;
    this->WholeExtent[5] = zMax ;
    }
  if (modified)
    {
    this->Modified();
    }
}

//----------------------------------------------------------------------------
float vtkpxImageGaussianNoiseSource::GaussianRandom()
{
  static int iset=0;
  static float gset;
  float fac,rsq,v1,v2;

  if  (iset == 0) 
    {
      do 
	{
	  v1=2.0*vtkMath::Random()-1.0;
	  v2=2.0*vtkMath::Random()-1.0;
	  rsq=v1*v1+v2*v2;
	} 
      while (rsq >= 1.0 || rsq == 0.0);
      
      fac=sqrt(-2.0*log(rsq)/rsq);
      gset=v1*fac;
      iset=1;
      return v2*fac;
    } 
  else
    {
      iset=0;
      return gset;
    }
}
//----------------------------------------------------------------------------
void vtkpxImageGaussianNoiseSource::ExecuteInformation()
{
  vtkImageData *output = this->GetOutput();
  
  output->SetWholeExtent(this->WholeExtent);
  output->SetScalarType(VTK_FLOAT);
  output->SetNumberOfScalarComponents(this->NumberOfComponents);
}

void vtkpxImageGaussianNoiseSource::ExecuteData(vtkDataObject *output)
{
  vtkImageData *data = this->AllocateOutputData(output);

  if (data->GetScalarType() != VTK_FLOAT)
    vtkErrorMacro("Execute: This source only outputs floats");
  
  int *outExt = data->GetExtent();
  
  // find the region to loop over

  int rowLength = (outExt[1] - outExt[0]+1)*data->GetNumberOfScalarComponents();
  int maxY = outExt[3] - outExt[2]; 
  int maxZ = outExt[5] - outExt[4];
  
  // Get increments to march through data 

  vtkIdType outIncX, outIncY, outIncZ;
  data->GetContinuousIncrements(outExt, outIncX, outIncY, outIncZ);
  float*  outPtr = (float *) data->GetScalarPointer(outExt[0],outExt[2],outExt[4]);
  
  unsigned long target = (unsigned long)((maxZ+1)*(maxY+1)/50.0);
  unsigned long count = 0;
  target++;

  for (int idxZ = 0; idxZ <= maxZ; idxZ++)
    {
      for (int idxY = 0; !this->AbortExecute && idxY <= maxY; idxY++)
	{
	  if (!(count%target))
	    this->UpdateProgress(count/(50.0*target));
	  
	  count++;
	  for (int idxR = 0; idxR < rowLength; idxR++)
	    {
	      *outPtr = this->Magnitude * vtkpxImageGaussianNoiseSource::GaussianRandom();
	      outPtr++;
	    }
	  outPtr += outIncY;
	}
      outPtr += outIncZ;
    }
}

void vtkpxImageGaussianNoiseSource::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkImageSource::PrintSelf(os,indent);
  os << indent << "NumberOfComponents: " << this->NumberOfComponents << "\n";
  os << indent << "Magnitude: " << this->Magnitude << "\n";
}



