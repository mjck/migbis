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





#include "vtkpxEnclosedPipeline.h"
#include "vtkObjectFactory.h"
//------------------------------------------------------------------------------
vtkpxEnclosedPipeline* vtkpxEnclosedPipeline::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxEnclosedPipeline");
  if(ret)
    {
    return (vtkpxEnclosedPipeline*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxEnclosedPipeline;
}

// Construct object with no children.
vtkpxEnclosedPipeline::vtkpxEnclosedPipeline()
{
}

vtkpxEnclosedPipeline::~vtkpxEnclosedPipeline()
{
}

/*
// Add a part to the list of Parts.
void vtkpxEnclosedPipeline::AddPart(vtkActor *actor)
{
  vtkAssembly::AddPart(actor);
}

// Remove a part from the list of parts,
void vtkpxEnclosedPipeline::RemovePart(vtkActor *actor)
{
  vtkAssembly::RemovePart(actor);
}

void vtkpxEnclosedPipeline::InitPartTraversal()
{
  vtkAssembly::InitPartTraversal();
}

// Return the next part in the hierarchy of assembly Parts.  This method 
// returns a properly transformed and updated actor.
vtkActor *vtkpxEnclosedPipeline::GetNextPart()

{
  return vtkAssembly::GetNextPart();
}

int vtkpxEnclosedPipeline::GetNumberOfParts()
{
  return vtkAssembly::GetNumberOfParts();
}

void vtkpxEnclosedPipeline::SetProperty (vtkProperty *lut)
{
  vtkAssembly::SetProperty(lut);
}

void vtkpxEnclosedPipeline::SetBackfaceProperty (vtkProperty *lut)
{
 vtkAssembly::SetBackfaceProperty(lut);
}

void vtkpxEnclosedPipeline::SetTexture ( vtkTexture *t)
{
 vtkAssembly::SetTexture(t);
}

void vtkpxEnclosedPipeline::SetMapper ( vtkMapper *m)
{
  vtkAssembly::SetMapper(m);
}

void vtkpxEnclosedPipeline::SetProperty (vtkProperty& lut)
{
  vtkAssembly::SetProperty(lut);
}

void vtkpxEnclosedPipeline::SetBackfaceProperty (vtkProperty& lut)
{
  vtkAssembly::SetBackfaceProperty(lut);
}
*/

