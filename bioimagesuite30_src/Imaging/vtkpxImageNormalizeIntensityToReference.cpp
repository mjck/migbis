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
  Module:    $RCSfile: vtkpxImageNormalizeIntensityToReference.cpp,v $
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
#include "vtkpxImageNormalizeIntensityToReference.h"
#include "vtkpxUtil.h"
#include "vtkFloatArray.h"
#include "vtkpxUtil.h"
#include "vtkpxMatrix.h"
//------------------------------------------------------------------------------
vtkpxImageNormalizeIntensityToReference* vtkpxImageNormalizeIntensityToReference::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageNormalizeIntensityToReference");
  if(ret)
    {
    return (vtkpxImageNormalizeIntensityToReference*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageNormalizeIntensityToReference;
}

// Construct object with no children.
vtkpxImageNormalizeIntensityToReference::vtkpxImageNormalizeIntensityToReference()
{
  this->Threshold=0.02;
  this->Reference=NULL;
  this->Order=1;
}
// ----------------------------------------------------------------------------
vtkpxImageNormalizeIntensityToReference::~vtkpxImageNormalizeIntensityToReference()
{
  this->SetReference(NULL);
}
// ----------------------------------------------------------------------------
void vtkpxImageNormalizeIntensityToReference::SimpleExecute(vtkImageData* ,vtkImageData* )
{

  if (this->GetImageDataInput(0)==NULL    || this->Reference==NULL )
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxImageNormalizeIntensityToReference");
      return;
    }


  int dim[3],dim2[3];
  this->GetImageDataInput(0)->GetDimensions(dim);
  this->GetReference()->GetDimensions(dim2);
  
  int ssum=0;
  for (int ia=0;ia<=2;ia++)
    ssum+=abs(dim[ia]-dim2[ia]);

  ssum+=abs(this->GetImageDataInput(0)->GetNumberOfScalarComponents()-this->GetReference()->GetNumberOfScalarComponents());
  if (ssum!=0)
    {
      vtkErrorMacro(<<"Bad Input Dimensions");
      return;
    }
    
  vtkImageData* output=this->GetOutput();
  output->SetSpacing(this->GetImageDataInput(0)->GetSpacing());
  output->SetOrigin(this->GetImageDataInput(0)->GetOrigin());
  output->SetScalarType(VTK_FLOAT);
  output->SetNumberOfScalarComponents(this->GetImageDataInput(0)->GetNumberOfScalarComponents());
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  output->AllocateScalars();


  float minr,maxr;
  vtkpxUtil::ImageRobustRange(this->GetImageDataInput(0),this->Threshold,1-this->Threshold,minr,maxr);
  fprintf(stderr,"Using range %.2f:%.2f (Thr=%f)\n",minr,maxr,this->Threshold);
  
  vtkDataArray* in1=this->GetImageDataInput(0)->GetPointData()->GetScalars();  
  vtkDataArray* in2=this->GetReference()->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();

  int nt=in1->GetNumberOfTuples();
  
  float sx[5],sy[3];
  for (int iq=0;iq<5;iq++)
  {
	sx[iq]=0;
	if (iq<3)
		sy[iq]=0;
  }

  { for (int i=0;i<nt;i++)
    {
      float x=in1->GetComponent(i,0);
      if (x>minr && x<maxr && x!=-1)
	{
	  float y=in2->GetComponent(i,0);
	  float x2=x*x;
	  sx[0]+=1;
	  sx[1]+=x;
	  sx[2]+=x2;
	  sx[3]+=x2*x;
	  sx[4]+=x2*x2;
	  sy[0]+=y;
	  sy[1]+=x*y;
	  sy[2]+=x2*y;
	}
    }
  }

  vtkpxMatrix* A=vtkpxMatrix::New();
  vtkpxMatrix* B=vtkpxMatrix::New();
  vtkpxMatrix* X=vtkpxMatrix::New();

  A->Zeros(this->Order+1,this->Order+1);
  B->Zeros(this->Order+1,1);
  X->Zeros(this->Order+1,1);

  for (int i=0;i<this->Order+1;i++)
    {
      B->SetElement(i,0,sy[i]);
      for (int j=0;j<this->Order+1;j++)
	A->SetElement(i,j,sx[i+j]);
    }
  
  A->Print("A=");
  B->Print("B=");
  A->Invert();
  vtkpxMatrix::Multiply(A,B,X);

  X->Print("X");

  float f[3];
  f[2]=0.0;
  {  for (int i=0;i<this->Order+1;i++)
	  f[i]=X->GetElement(i,0);}

  
  {for (int i=0;i<nt;i++)
    for (int c=0;c<output->GetNumberOfScalarComponents();c++)
      {
	float x=in1->GetComponent(i,c);
	float y=f[0]+f[1]*x+f[2]*x*x;
	out->SetComponent(i,c,y);
  }}
  this->UpdateProgress(1.0);
}

