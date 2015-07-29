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





#include "vtkpxGUICallback.h"
#include "vtkObject.h"

vtkpxGUICallback* vtkpxGUICallback::New()
{ 
  return new vtkpxGUICallback; 
}

vtkpxGUICallback::vtkpxGUICallback()
{
  this->component=NULL;
  this->filter=NULL;

}

void vtkpxGUICallback::AttachFilter(vtkObject* in_filter,vtkpxGUIComponent* in_component,const char* in_label)
{
  this->filter=in_filter;
  this->component=in_component;
  strncpy(this->label,in_label,50);
}

void vtkpxGUICallback::Execute(vtkObject *caller,unsigned long event, void* vtkNotUsed(v))
{
  if (this->filter==NULL || this->component==NULL)
    return;

  if (caller)
    {
      if (event == vtkCommand::ProgressEvent)
	{
	  if (this->filter->IsA("vtkProcessObject")==1)
	    {
	      this->component->ExecuteProgressMethod(this->label,((vtkProcessObject*)this->filter)->GetProgress());
	    }
	  else
	    this->component->ExecuteProgressMethod(this->label,0.5);
	}
      else if (event == vtkCommand::StartEvent)
	this->component->ExecuteProgressMethod(this->label,0.0);
      else if (event == vtkCommand::EndEvent)
	this->component->ExecuteProgressMethod(this->label,1.0);
    }
}

