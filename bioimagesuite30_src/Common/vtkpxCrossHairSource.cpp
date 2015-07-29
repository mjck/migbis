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





/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

 see vtkpxCrossHairSource.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */
#include "vtkAppendPolyData.h"
#include "vtkCubeSource.h"
#include "vtkSphereSource.h"
#include "vtkObjectFactory.h"
#include "vtkPolyData.h"
#include "vtkpxCrossHairSource.h"

//------------------------------------------------------------------------------
vtkpxCrossHairSource* vtkpxCrossHairSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxCrossHairSource");
  if(ret)
    {
      return (vtkpxCrossHairSource*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxCrossHairSource;
}

// Construct object with no children.
vtkpxCrossHairSource::vtkpxCrossHairSource()
{
  this->Position[0]=0;this->Position[1]=0;this->Position[2]=0;
  this->Thickness=0.5;
  this->Length=2.0;
  this->ShowSphere=1;
}
/* -------------------------------------------------------------------------*/
 
vtkpxCrossHairSource::~vtkpxCrossHairSource()
{
  
}
/* -------------------------------------------------------------------------*/
void vtkpxCrossHairSource::Execute()
{
  vtkAppendPolyData* append=vtkAppendPolyData::New();
  
  for (int axis=0;axis<=2;axis++)
    {
      vtkCubeSource* cube=vtkCubeSource::New();
      switch (axis)
	{
	case 0: // X-axis
	  cube->SetBounds(this->Position[0]-this->Length,
			  this->Position[0]+this->Length,
			  this->Position[1]-this->Thickness,
			  this->Position[1]+this->Thickness,
			  this->Position[2]-this->Thickness,
			  this->Position[2]+this->Thickness);
	  break;
	case 1:
	  cube->SetBounds(this->Position[0]-this->Thickness,
			  this->Position[0]+this->Thickness,
			  this->Position[1]-this->Length,
			  this->Position[1]+this->Length,
			  this->Position[2]-this->Thickness,
			  this->Position[2]+this->Thickness);
	  break;
	case 2:
	  cube->SetBounds(this->Position[0]-this->Thickness,
			  this->Position[0]+this->Thickness,
			  this->Position[1]-this->Thickness,
			  this->Position[1]+this->Thickness,
			  this->Position[2]-this->Length,
			  this->Position[2]+this->Length);
	  break;
	}
      cube->Update();
      append->AddInput(cube->GetOutput());
      cube->Delete();
    }

  if (this->ShowSphere)
    {
      vtkSphereSource* sphere=vtkSphereSource::New();
      sphere->SetCenter(this->Position);
      sphere->SetRadius(this->Length);
      sphere->Update();
      append->AddInput(sphere->GetOutput());
      sphere->Delete();
    }

  append->Update();
  this->GetOutput()->ShallowCopy(append->GetOutput());
  append->Delete();
}

