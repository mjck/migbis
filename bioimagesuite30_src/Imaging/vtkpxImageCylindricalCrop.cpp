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





#include "vtkpxImageCylindricalCrop.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"

vtkpxImageCylindricalCrop* vtkpxImageCylindricalCrop::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageCylindricalCrop");
  if(ret)
    {
      return (vtkpxImageCylindricalCrop*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageCylindricalCrop;
}


// Construct object to extract all of the input data.
vtkpxImageCylindricalCrop::vtkpxImageCylindricalCrop()
{

  this->Center[0]=-1000;
  this->Center[1]=-1000;
  this->Center[2]=-1000;
  this->Radius=-1;
}

// ---------------------------------------------------------------------------
void vtkpxImageCylindricalCrop::SimpleExecute(vtkImageData* input, vtkImageData* output) 
{
  int dim[3]; input->GetDimensions(dim);

  for (int ia=0;ia<=2;ia++)
    {
      if (this->Center[ia]==-1000)
	this->Center[ia]=dim[ia]/2;
    }
  if (this->Radius<0)
    this->Radius=dim[0]/2;
    
  int numc=input->GetNumberOfScalarComponents();

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();

  
  for (int c=0;c<numc;c++)
    out->FillComponent(c,0.0);


  /*  fprintf(stderr,"Cropping Center (%d,%d,%d) Radius=%d\n",
	  this->Center[0],
	  this->Center[1],
	  this->Center[2],
	  this->Radius);*/

  int index=0;
  this->UpdateProgress(0.01);

  double thr=double(this->Radius*this->Radius);

  for (int k=0;k<dim[2];k++)
    {
      for (int j=0;j<dim[1];j++)
	{
	  double dj=pow(double(j-this->Center[1]),2.0);
	  for (int i=0;i<dim[0];i++)
	    {
	      double di=pow(double(i-this->Center[0]),2.0);
	      double d=di+dj;
	      
	      if (d<thr)
		{
		  for (int c=0;c<numc;c++)
		    out->SetComponent(index,c,inp->GetComponent(index,c));
		}

	      ++index;
	    }
	}
      this->UpdateProgress(float(k+1)/float(dim[2]));
    }
}
  

