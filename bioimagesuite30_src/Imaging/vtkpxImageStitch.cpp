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
  Module:    $RCSfile: vtkpxImageStitch.cpp,v $
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
#include "vtkpxImageStitch.h"
#include "vtkpxUtil.h"
#include "vtkFloatArray.h"
#include "vtkImageGaussianSmooth.h"
//------------------------------------------------------------------------------
vtkpxImageStitch* vtkpxImageStitch::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageStitch");
  if(ret)
    {
    return (vtkpxImageStitch*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageStitch;
}

// Construct object with no children.
vtkpxImageStitch::vtkpxImageStitch()
{
  this->Axis=0;
  this->SeamCoordinate=-1;
  this->Input2=NULL;
  this->FlipOrientation=0;
  //  this->Transformation=NULL;
  //  this->AutoAdjustImageSize=0;
}
// ----------------------------------------------------------------------------
vtkpxImageStitch::~vtkpxImageStitch()
{
  this->SetInput2(NULL);
  //  this->SetTransformation(NULL);
}
// ----------------------------------------------------------------------------
void vtkpxImageStitch::SimpleExecute(vtkImageData* input,vtkImageData* output)
{

  if (input==NULL    || this->Input2==NULL )
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxImageStitch");
      return;
    }
   

  int dim[3],dim2[3];
  input->GetDimensions(dim);
  this->Input2->GetDimensions(dim2);
  
  int ssum=0;
  for (int ia=0;ia<=2;ia++)
    ssum+=abs(dim[ia]-dim2[ia]);

  ssum+=abs(input->GetNumberOfScalarComponents()-this->Input2->GetNumberOfScalarComponents());
  if (ssum!=0)
    {
      vtkErrorMacro(<<"Bad Input Dimensions");
      return;
    }
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* inp[2];
  inp[0]= input->GetPointData()->GetScalars();
  inp[1]= Input2->GetPointData()->GetScalars();

  this->UpdateProgress(0.01);
  
  float pog=0.00;
  float tenth=out->GetNumberOfTuples()/10.0;
  int count=0;
  int pixel=0;
  int currentimage=0;
  int nc=out->GetNumberOfComponents();

  if (this->SeamCoordinate<0)
    this->SeamCoordinate=dim[this->Axis]/2;

  //  fprintf(stderr,"Beginning Stitching Axis=%d Coordinate=%d\n",this->Axis,this->SeamCoordinate);

  for (int k=0;k<dim[2];k++)
    {
      if (this->Axis==2)
	{
	  if (k<this->SeamCoordinate)
	    currentimage=0;
	  else
	    currentimage=1;
	  if (this->FlipOrientation)
	    currentimage=1-currentimage;
	}

      for (int j=0;j<dim[1];j++)
	{
	  if (this->Axis==1)
	    {
	      if (j<this->SeamCoordinate)
		currentimage=0;
	      else
		currentimage=1;
	      if (this->FlipOrientation)
		currentimage=1-currentimage;
	    }

	  for (int i=0;i<dim[0];i++)
	    {
	      if (this->Axis==0)
		{
		  if (i<this->SeamCoordinate)
		    currentimage=0;
		  else
		    currentimage=1;
		  if (this->FlipOrientation)
		    currentimage=1-currentimage;
		}
	      
	      for (int c=0;c<nc;c++)
		out->SetComponent(pixel,c,inp[currentimage]->GetComponent(pixel,c));
	      ++pixel;
	      ++count;
	      if (count==tenth)
		{
		  pog+=0.05;
		  this->UpdateProgress(pog);
		  count=0;
		}
	    }
	}
    }

  this->UpdateProgress(1.0);

}

