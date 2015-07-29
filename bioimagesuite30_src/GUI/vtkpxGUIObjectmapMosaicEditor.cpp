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
  Module:    $RCSfile: vtkpxGUIObjectmapMosaicEditor.cpp,v $
  Language:  C++
  Date:      $Date: 2002/12/18 16:11:14 $
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

#include "vtkpxGUIObjectmapMosaicEditor.h"
#include "vtkpxGUIObjectmapMosaicViewer.h"
#include "vtkObjectFactory.h"


vtkpxGUIObjectmapMosaicEditor* vtkpxGUIObjectmapMosaicEditor::New()
{
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIObjectmapMosaicEditor");
  if(ret)
      {
	return (vtkpxGUIObjectmapMosaicEditor*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIObjectmapMosaicEditor;
}

// Construct object with no children.
vtkpxGUIObjectmapMosaicEditor::vtkpxGUIObjectmapMosaicEditor()
{
  this->Viewer=NULL;
}

/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapMosaicEditor::UpdateFromViewer(int mode)
{
  if (this->Viewer==NULL)
    return;
  if (mode==0)
    this->SetImagesAndLookupTable(this->Viewer->GetImage(),
				  this->Viewer->GetObjectMapImage(),NULL,
				  this->Viewer->GetCurrentImageIsColor());
  else
    this->SetImagesAndLookupTable(this->Viewer->GetImage(),
				  this->Viewer->GetObjectMapImage(),
				  this->Viewer->GetObjectMapLookupTable(),
				  this->Viewer->GetCurrentImageIsColor());

}

void vtkpxGUIObjectmapMosaicEditor::UpdateViewerDisplay()
{
  if (this->Viewer==NULL)
    return;
 
  this->Viewer->UpdateDisplay();

}

void vtkpxGUIObjectmapMosaicEditor::UpdateViewerObjectmapColormap()
{
   if (this->Viewer==NULL)
    return;
 
   this->Viewer->SetObjectLookupTable(this->ObjectmapLookupTable,0);
}
 
void vtkpxGUIObjectmapMosaicEditor::UpdateViewerObjectmap()
{
   if (this->Viewer==NULL)
    return;
 
   this->Viewer->QuickUpdateObjectmap();
}

void vtkpxGUIObjectmapMosaicEditor::SetObjectmapViewer(vtkpxGUIObjectmapMosaicViewer* viewer)
{
  this->Viewer=viewer;
}



