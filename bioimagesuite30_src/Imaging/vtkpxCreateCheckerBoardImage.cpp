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
  Module:    $RCSfile: vtkpxCreateCheckerBoardImage.cpp,v $
  Language:  C++
  Date:      $Date: 2002/09/26 18:22:26 $
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
#include "vtkpxCreateCheckerBoardImage.h"
#include "vtkObjectFactory.h"
#include "vtkpxUtil.h"

//------------------------------------------------------------------------------
vtkpxCreateCheckerBoardImage* vtkpxCreateCheckerBoardImage::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxCreateCheckerBoardImage");
  if(ret)
    {
    return (vtkpxCreateCheckerBoardImage*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxCreateCheckerBoardImage;
}

// Construct object with no children.
vtkpxCreateCheckerBoardImage::vtkpxCreateCheckerBoardImage()
{
  this->NormalizeImages=1;
  this->Input2=NULL;
  this->SquareSize=8;
}

vtkpxCreateCheckerBoardImage::~vtkpxCreateCheckerBoardImage()
{
  this->SetInput2(NULL);
}

//------------------------------------------------------------------------------
void vtkpxCreateCheckerBoardImage::ComputeStepBounds(vtkImageData* image,double range[2],int mode)
{
  vtkDataArray* scal=image->GetPointData()->GetScalars();
  scal->GetRange(range);
  
  if (mode==0)
    return;

  int n=scal->GetNumberOfTuples(),inc=1;
  if (n>133)
    inc=int(n/133);

  double mean=0.0,mean2=0.0;
  int num=0;
  for (int i=0;i<n;i+=inc)
    {
      double v=scal->GetComponent(i,0);
      if (v!=0.0)
	{
	  mean+=v;
	  mean2+=v*v;
	  ++num;
	}
    }

  if (num==0)
    return;

  mean/=double(num);
  mean2/=double(num);
  double std=sqrt(mean2-mean*mean);

  range[0]=Frange(mean-3.0*std,range[0],range[1]-1.0);
  range[1]=Frange(mean+3.0*std,range[0]+1.0,range[1]);
}
//------------------------------------------------------------------------------
double vtkpxCreateCheckerBoardImage::ComputeAnatomicalVoxelValue(double vox,double range0,double range1,
								double min,double max)
{
  
  if (vox<=range0)
    return min;
  if (vox>=range1)
    return max;

  return min+(max-min)/(range1-range0)*(vox-range0);

}
//------------------------------------------------------------------------------

void vtkpxCreateCheckerBoardImage::SimpleExecute(vtkImageData* i1,vtkImageData* o1)
{
  vtkImageData *input[2];
  input[0]=this->GetImageDataInput(0);
  input[1]=this->GetInput2();
  vtkImageData *output=this->GetOutput();

  if (input[0]==NULL || input[1]==NULL)
      {
	vtkErrorMacro(<<"No input images!");
	return;
      }


  int dim[3];     input[0]->GetDimensions(dim);
  int dim2[3];    input[1]->GetDimensions(dim2);


  int diff=0;
  int ia;
  for (ia=0;ia<=2;ia++)
    diff+=abs(dim[ia]-dim2[ia]);
  if (diff>0)
    {
      vtkErrorMacro(<<"Images Have Different Dimensions\n");
      return;
    }

  // On To Merge

  this->UpdateProgress(0.05);

  double range[2][2];

  for (ia=0;ia<=1;ia++)
    {
      double r[2];
      this->ComputeStepBounds(input[ia],r,this->NormalizeImages);
      range[ia][0]=r[0];
      range[ia][1]=r[1];
      if (range[ia][0]==range[ia][1])
	range[ia][1]+=1.0;
    }

  int numpixels=dim[0]*dim[1];  
  
  output->SetSpacing(input[0]->GetSpacing());
  output->SetOrigin(input[0]->GetOrigin());
  output->SetScalarType(input[0]->GetScalarType());
  output->SetNumberOfScalarComponents(1);
  output->SetDimensions(input[0]->GetDimensions());
  output->SetWholeExtent(input[0]->GetWholeExtent());
  output->AllocateScalars();
  vtkDataArray* outScalars=output->GetPointData()->GetScalars();
  
  int odd[3],count[3];
  
  odd[0]=0;  count[0]=0;
  for (int sl=0;sl<dim[2];sl++)
    {
      int offset=sl*numpixels;
      odd[1]=0;  count[1]=0;
      
      for (int iy=0;iy<dim[1];iy++)
	{
	  int offset2=offset+iy*dim[0];
	  odd[2]=0;  count[2]=0;
	  
	  for (int ix=0;ix<dim[0];ix++)
	    {
	      
	      int sum=odd[0]+odd[1]+odd[2];
	      int imgno=1;
	      if (sum == 0 || sum== 2)
		imgno=0;
	      
	      double vc=input[imgno]->GetPointData()->GetScalars()->GetComponent(ix+offset2,0);
	      int c=(int)this->ComputeAnatomicalVoxelValue(vc,range[imgno][0],range[imgno][1],0,255);
	      outScalars->SetComponent(ix+offset2,0,double(c));
	      
	      ++count[2];
	      if (count[2]==this->SquareSize)
		{
		  count[2]=0;
		  odd[2]=1-odd[2];
		}
	    }
	  
	  ++count[1];
	  if (count[1]==this->SquareSize)
	    {
	      count[1]=0;
	      odd[1]=1-odd[1];
	    }
	}
      
      ++count[0];
      if (count[0]==this->SquareSize)
	{
	  count[0]=0;
	  odd[0]=1-odd[0];
	}
      
      
      double pog=double(sl)/double(dim[2]);
      this->UpdateProgress(pog);	
    }

  this->UpdateProgress(1.0);
}
//------------------------------------------------------------------------------

