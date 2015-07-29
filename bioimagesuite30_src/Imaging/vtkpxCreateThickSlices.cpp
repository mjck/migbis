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
  Module:    $RCSfile: vtkpxCreateThickSlices.cpp,v $
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
#include "vtkObjectFactory.h"
#include "vtkSource.h"
#include "vtkpxCreateThickSlices.h"
#include <vtkDataArray.h>
#include <vtkImageData.h>

//------------------------------------------------------------------------------
vtkpxCreateThickSlices* vtkpxCreateThickSlices::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxCreateThickSlices");
  if(ret)
    {
    return (vtkpxCreateThickSlices*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxCreateThickSlices;
}

// Construct object with no children.
vtkpxCreateThickSlices::vtkpxCreateThickSlices()
{
  this->InPlaneSample=1;
  this->SliceSample=1;
  this->SumOnly=0;

}
// ----------------------------------------------------------------------------
void vtkpxCreateThickSlices::ExecuteInformation()
{
  vtkImageData *input=this->GetImageDataInput(0);

  if (input==NULL)
      {
	vtkErrorMacro(<<"No Input Image !!");
	return;
      }

  vtkImageData *output=this->GetOutput();

  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  int wholeExtent[6];
  input->GetWholeExtent( wholeExtent );

  int dims[3];

  dims[0] = int ( (wholeExtent[1] - wholeExtent[0] +1 ) / this->InPlaneSample);
  dims[1] = int ( (wholeExtent[3] - wholeExtent[2] +1 ) / this->InPlaneSample);
  dims[2] = int ( (wholeExtent[5] - wholeExtent[4] +1 ) / this->SliceSample);

  wholeExtent[0] = 0;
  wholeExtent[1] = dims[0] - 1;
  wholeExtent[2] = 0;
  wholeExtent[3] = dims[1] - 1;
  wholeExtent[4] = 0;
  wholeExtent[5] = dims[2] - 1;
  output->SetWholeExtent( wholeExtent );
}

// ----------------------------------------------------------------------------

void vtkpxCreateThickSlices::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  
  // Create Output Array
  // -------------------
  int   dim[3]; input->GetDimensions(dim);
  double sp[3];  input->GetSpacing(sp);
  
  dim[0]=int(dim[0]/this->InPlaneSample);
  dim[1]=int(dim[1]/this->InPlaneSample);
  dim[2]=int(dim[2]/this->SliceSample);
  sp[0]=sp[0]*float(this->InPlaneSample);
  sp[1]=sp[1]*float(this->InPlaneSample);
  sp[2]=sp[2]*float(this->SliceSample);

    
  output->SetSpacing(sp);
  output->SetOrigin(input->GetOrigin());
  output->SetScalarType(input->GetScalarType());
  output->SetNumberOfScalarComponents(input->GetNumberOfScalarComponents());
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  output->SetUpdateExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  output->AllocateScalars();

  
  float progress=0.0;
  float dprog=1.0/float(output->GetNumberOfScalarComponents()*dim[2]);

  float factor=1.0/float(this->SliceSample*this->InPlaneSample*this->InPlaneSample);
  if (this->SumOnly==1)
    factor=1.0;

  for (int comp=0;comp<output->GetNumberOfScalarComponents();comp++)
    {
      for (int k=0;k<dim[2];k++)
	{
	  int kmin=k*this->SliceSample;
	  int kmax=kmin+this->SliceSample;
	  for (int j=0;j<dim[1];j++)
	    {
	      int jmin=j*this->InPlaneSample;
	      int jmax=jmin+this->InPlaneSample;
	      for (int i=0;i<dim[0];i++)
		{
		  int imin=i*this->InPlaneSample;
		  int imax=imin+this->InPlaneSample;
	      
		  float sum=0.0;
		  
		  for (int ka=kmin;ka<kmax;ka++)
		    for (int ja=jmin;ja<jmax;ja++)
		      for (int ia=imin;ia<imax;ia++)
			sum+=input->GetScalarComponentAsDouble(ia,ja,ka,comp);
		  
		  int index=k*dim[1]*dim[0]+j*dim[0]+i;
		  output->GetPointData()->GetScalars()->SetComponent(index,comp,sum*factor);
		}
	    }
	  progress+=dprog;
	  this->UpdateProgress(progress);
	  char line[100];
	  sprintf(line,"prog=%f k=%d\n",progress,k);
	  vtkDebugMacro(<<line);
	}
    }

}


