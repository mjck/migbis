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
  Module:    $RCSfile: vtkpxGUI4DRenderer.cpp,v $
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
#include "vtkpxGUI4DRenderer.h"
#include "vtkObjectFactory.h"
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkLight.h>
#include <vtkLightCollection.h>
#include "pxutil.h"

vtkpxGUI4DRenderer* vtkpxGUI4DRenderer::New()
{
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUI4DRenderer");
  if(ret)
      {
	return (vtkpxGUI4DRenderer*)ret;
      }
  return new vtkpxGUI4DRenderer;
}

// Construct object with no children.
vtkpxGUI4DRenderer::vtkpxGUI4DRenderer()
{
  this->RendererCollection=NULL;
  this->LightCollection=NULL;
  this->CurrentFrame=0;
  this->NumberOfFrames=0;
  this->ActualNumberOfFrames=0;
}


vtkpxGUI4DRenderer::~vtkpxGUI4DRenderer()
{
  // To be done later 
}

// -------------------------------------------------------------------

char* vtkpxGUI4DRenderer::Initialize(const char* widgetname,vtkRenderer* ren,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();

  vtkpxGUIRenderer::Initialize(widgetname,ren,inside);
  
  this->NumberOfFrames=1;
  this->ActualNumberOfFrames=1;
  this->CurrentFrame=0;

  this->RendererCollection=vtkRendererCollection::New();
  this->RendererCollection->AddItem(this->renderer);

  this->LightCollection=vtkLightCollection::New();
  this->LightCollection->AddItem(main_light);


  return GetWidgetName();
}
/* -------------------------------------------------------------------------*/
vtkRenderer* vtkpxGUI4DRenderer::GetRendererForFrame(int frame)
{
 if (this->RendererCollection==NULL)
    return NULL;
 
 frame=Irange(frame,0,this->NumberOfFrames-1);
 return (vtkRenderer*)this->RendererCollection->GetItemAsObject(frame);
}
/* -------------------------------------------------------------------------*/
vtkRendererCollection* vtkpxGUI4DRenderer::GetRendererCollection()
{
  return this->RendererCollection;
}


/* -------------------------------------------------------------------------*/
int vtkpxGUI4DRenderer::SetCurrentFrame(int frame)
{
  if (this->NumberOfFrames==0)
    return 0;

  frame=Irange(frame,0,this->NumberOfFrames-1);
  if (frame==this->CurrentFrame)
    return frame;

  return this->SwitchFrame(frame,this->CurrentFrame,0);

}
/* -------------------------------------------------------------------------*/
int vtkpxGUI4DRenderer::FastSetCurrentFrame(int frame)
{
  if (this->NumberOfFrames==0)
    return 0;

  frame=Irange(frame,0,this->NumberOfFrames-1);
  if (frame==this->CurrentFrame)
    return frame;

  return this->SwitchFrame(frame,this->CurrentFrame,1);
}

/* -------------------------------------------------------------------------*/
int vtkpxGUI4DRenderer::GetCurrentFrame()
{
  return this->CurrentFrame;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUI4DRenderer:: SetNumberOfFrames(int numframes)
{
  if (this->NumberOfFrames==0)
    return 0;

  if (numframes==this->NumberOfFrames)
    return numframes;

  numframes=Irange(numframes,1,999);

  if (numframes<=this->ActualNumberOfFrames)
    {
      if (this->CurrentFrame>=this->NumberOfFrames)
	this->SwitchFrame(0,this->CurrentFrame);
    }
  else
    {
      int diff=numframes-this->ActualNumberOfFrames;
      for (int i=0;i<diff;i++)
	this->AddFrame();
    }

  this->NumberOfFrames=numframes;
  return this->NumberOfFrames;
      
}
/* -------------------------------------------------------------------------*/
int vtkpxGUI4DRenderer:: GetNumberOfFrames()
{
  return this->NumberOfFrames;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUI4DRenderer::PrepareForCineMode()
{
  int f=this->CurrentFrame;
  int ip=f;
  
  for (int i=0;i<this->NumberOfFrames;i++)
    {
      if (i!=f)
	ip=this->SwitchFrame(i,ip);
    }
  ip=this->SwitchFrame(f,ip);
  return 1;
  
}

/* -------------------------------------------------------------------------*/
int vtkpxGUI4DRenderer::SwitchFrame(int newframe,int oldframe,int fast)
{
  //  fprintf(stderr,"Switchframe from %d to %d fast=%d\n",oldframe,newframe,fast);

  vtkRenderer* newRenderer=(vtkRenderer*)this->RendererCollection->GetItemAsObject(newframe);
  vtkRenderer* oldRenderer=(vtkRenderer*)this->RendererCollection->GetItemAsObject(oldframe);
  this->CopyCamera(newRenderer->GetActiveCamera(),oldRenderer->GetActiveCamera());
  vtkLight* newLight=(vtkLight*)this->LightCollection->GetItemAsObject(newframe);

  //float vp[4],cl[3];
  if  (fast==0)
    {
      vtkLight* oldLight=(vtkLight*)this->LightCollection->GetItemAsObject(oldframe);
      newRenderer->SetViewport(oldRenderer->GetViewport());
      newRenderer->SetBackground(oldRenderer->GetBackground());
      newLight->DeepCopy(oldLight);
    }
  
  vtkRenderWindow* renWin=oldRenderer->GetRenderWindow();
  
  if (renWin!=NULL)
    {
      vtkRendererCollection* renWinCol=oldRenderer->GetRenderWindow()->GetRenderers();
      renWinCol->RemoveItem(oldRenderer);
      renWin->AddRenderer(newRenderer);
      this->renderer=newRenderer;
      this->UpdateClippingPlanes();
    }

  this->main_light=newLight;

  this->CurrentFrame=newframe;
  return newframe;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUI4DRenderer::AddFrame()
{
  if (this->RendererCollection==NULL || this->LightCollection==NULL)
    return 0;

  vtkRenderer* ren=vtkRenderer::New();
  this->RendererCollection->AddItem(ren);
 
  vtkLight* light=vtkLight::New();
  this->LightCollection->AddItem(light);

  ++this->ActualNumberOfFrames;
  //  fprintf(stderr,"GUIRenderer:: Adding Frame Now have %d \n",this->ActualNumberOfFrames);


  return 1; 
}
/* -------------------------------------------------------------------------*/

