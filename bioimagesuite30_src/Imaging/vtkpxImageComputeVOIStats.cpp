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
  Module:    $RCSfile: vtkpxImageComputeVOIStats.cpp,v $
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
#include "vtkObjectFactory.h"
#include "vtkpxImageComputeVOIStats.h"
#include "vtkpxUtil.h"
#include "vtkFloatArray.h"
#include "vtkImageGaussianSmooth.h"

//------------------------------------------------------------------------------
vtkpxImageComputeVOIStats* vtkpxImageComputeVOIStats::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageComputeVOIStats");
  if(ret)
    {
    return (vtkpxImageComputeVOIStats*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageComputeVOIStats;
}

// Construct object with no children.
vtkpxImageComputeVOIStats::vtkpxImageComputeVOIStats()
{
  this->VOI=NULL;
  this->AbsoluteValue=0;
}
// ----------------------------------------------------------------------------
vtkpxImageComputeVOIStats::~vtkpxImageComputeVOIStats()
{
  this->SetVOI(NULL);
}
// ----------------------------------------------------------------------------
void vtkpxImageComputeVOIStats::SimpleExecute(vtkImageData* ,vtkImageData* output)
{

  if (this->GetImageDataInput(0)==NULL    || this->VOI==NULL )
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxImageComputeVOIStats");
      return;
    }

  int dim[3],dim2[3];
  this->GetImageDataInput(0)->GetDimensions(dim);
  this->VOI->GetDimensions(dim2);
  
  int s=0;
  for (int ia=0;ia<=2;ia++)
    s+=abs(dim[ia]-dim2[ia]);
  if (s!=0)
    {
      vtkErrorMacro(<<"Bad Input Dimensions");
      return;
    }
 
  double range[2]; this->VOI->GetPointData()->GetScalars()->GetRange(range);
  int numv=int(range[1]-range[0]);

  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* in1=this->GetImageDataInput(0)->GetPointData()->GetScalars();  
  vtkDataArray* in2=this->VOI->GetPointData()->GetScalars();

  int* numvox=new int[numv];
  float* sum=new float[numv];
  float* sum2=new float[numv];

  for (int i=0;i<numv;i++)
    {
      numvox[i]=0;
      sum[i]=0.0;
      sum2[i]=0.0;
    }

  int numscalars=in1->GetNumberOfTuples();
  this->UpdateProgress(0.1);
  
  for (int jj=0;jj<numscalars;jj++) 
    {
      float x=in1->GetComponent(jj,0);
      int y=int(in2->GetComponent(jj,0)-range[0])-1;

      if (this->AbsoluteValue)
	x=fabs(x);


      if (y>=0)
	{
	  ++numvox[y];
	  sum[y]+=x;
	  sum2[y]+=x*x;
	}
    }
  this->UpdateProgress(0.9);

  for (int region=0;region<numv;region++)
    {
      if (numvox[region]<1)
	{
	  out->SetComponent(region,0,0);
	  out->SetComponent(region,1,-1);
	}
      else
	{
	  float mean=sum[region]/float(numvox[region]);
	  float std =sqrt(sum2[region]/float(numvox[region]) - mean*mean);
	  
	  out->SetComponent(region,0,mean);
	  out->SetComponent(region,1,std);
	  
	  fprintf(stderr,"VOI int=%d,\t mean=%.2f\t std=%.2f\n",
		  int(region+range[0]+1),
		  mean,
		  std);
	}
    }

  delete [] sum2;
  delete [] sum;
  delete [] numvox;
  
  this->UpdateProgress(1);

}

// ----------------------------------------------------------------------------
void vtkpxImageComputeVOIStats::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  vtkImageData *output=this->GetOutput();
  output->SetSpacing(1,1,1);
  output->SetOrigin(0,0,0);
  output->SetWholeExtent(0,0,0,0,0,0);

  if (this->VOI==NULL)
    return;

  double range[2]; this->VOI->GetPointData()->GetScalars()->GetRange(range);
  int numv=int(range[1]-range[0]);
  
  output->SetScalarTypeToFloat();
  output->SetWholeExtent(0,numv-1,0,0,0,0);
  output->SetNumberOfScalarComponents(2);
}

