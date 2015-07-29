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
  Module:    $RCSfile: vtkpxWeightedSmoothImage.cpp,v $
  Language:  C++
  Date:      $Date: 2002/05/01 13:49:39 $
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
#include "vtkpxWeightedSmoothImage.h"
#include "vtkDataArray.h"
#include "math.h"
#include "vtkpxUtil.h"
#include "vtkGridTransform.h"
#include "vtkFloatArray.h"
//------------------------------------------------------------------------------
vtkpxWeightedSmoothImage* vtkpxWeightedSmoothImage::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxWeightedSmoothImage");
  if(ret)
    {
    return (vtkpxWeightedSmoothImage*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxWeightedSmoothImage;
}

// Construct object with no children.
vtkpxWeightedSmoothImage::vtkpxWeightedSmoothImage()
{
  this->Smoothness=0.5;
  this->Dimensionality=2;
  this->Iterations=5;
}
// ----------------------------------------------------------------------------
vtkpxWeightedSmoothImage::~vtkpxWeightedSmoothImage()
{
}
// ----------------------------------------------------------------------------
int vtkpxWeightedSmoothImage::CheckInputs()
{
 if (this->GetImageDataInput(0)==NULL)
    {
      vtkErrorMacro(<<"No Input Field Map Supplied\n");
      return 0;
    }

 int dim1[3];
 this->GetImageDataInput(0)->GetDimensions(dim1);

 int c=this->GetImageDataInput(0)->GetNumberOfScalarComponents();

 if (dim1[0]<4 || dim1[1]<4 || dim1[2]<1 || c<2)
   {
     vtkErrorMacro(<<"Image Dimensions are too small ( 16x16x1 minimum 2 components (1=intensity,2=weights))");
     return 0;
   }


 return 1;
}
// ----------------------------------------------------------------------------
void vtkpxWeightedSmoothImage::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (this->CheckInputs()==0)
    return;

  output->DeepCopy(input);

  if (this->Iterations==0)
    return;

  const int searchdir[6][3]= { 
    { 1 , 0 , 0 } ,     { -1 ,  0 ,  0 } ,
    { 0 , 1 , 0 } ,     {  0 , -1 ,  0 } ,
    { 0 , 0 , 1 } ,     {  0 ,  0 , -1 } };


  vtkDataArray* outscal=output->GetPointData()->GetScalars();
  vtkDataArray* temp=vtkDataArray::CreateDataArray(outscal->GetDataType());
  temp->SetNumberOfComponents(2);

  int dim[3];
  output->GetDimensions(dim);

  int numsearch=this->Dimensionality*2;
  int iter=0;

  float msmooth=1.0-this->Smoothness;

  while ( iter < this->Iterations ) 
    {
      temp->DeepCopy(outscal);

      int index=0;
      for (int k=0;k<dim[2];k++)
	for (int j=0;j<dim[1];j++)
	  for (int i=0;i<dim[0];i++)
	    {
	      int debug=0;

	      if (i==dim[0]/2 && j==dim[1]/2)
		debug=1;

	      float tw1=0.0;
	      float tc1=0.0;
	      for (int s=0;s<numsearch;s++)
		{
		  int newi=Irange(i+searchdir[s][0],0,dim[0]-1);
		  int newj=Irange(j+searchdir[s][1],0,dim[1]-1);
		  int newk=Irange(k+searchdir[s][2],0,dim[2]-1);

		  float c1=output->GetScalarComponentAsDouble(newi,newj,newk,0);
		  float w1=output->GetScalarComponentAsDouble(newi,newj,newk,1);

		  tc1+=w1*c1;
		  tw1+=w1;

		  if (debug)
		    fprintf(stderr,"\tScanning %d,%d,%d = %.1f,%.1f totals = %.1f,%.1f\n",
			    newi,newj,newk,c1,w1,tc1,tw1);
		}

	      if (tw1>=0.01)
		tc1=tc1/tw1;
	      else
		tw1=0.0;
	      tw1=tw1*this->Smoothness/float(numsearch);
	      if (debug)
		fprintf(stderr,"\t\t mn = %.1f,%.1f\n",tc1,tw1);

	      
	      float c=output->GetScalarComponentAsDouble(i,j,k,0);
	      float w=output->GetScalarComponentAsDouble(i,j,k,1)*msmooth;

	      float sumconf=w+tw1;
	      float vout=c;

	      if (sumconf>0.0001)
		vout = ( w*c+ tw1*tc1)/(w+tw1);

	      if (debug)
		fprintf(stderr,"Point =%d,%d,%d \t\tc,w=%.1f,%.1f \n\t vout = %.2f\n\n",
			i,j,k,c,w,vout);

	      temp->SetComponent(index,0,vout);
	      ++index;
	    }
    
      fprintf(stderr,"Smoothing Iteration = %d \n",iter);
      ++iter;
      outscal->DeepCopy(temp);
    }
  temp->Delete();
  return;
}


