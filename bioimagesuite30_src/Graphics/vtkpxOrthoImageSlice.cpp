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
  Module:    $RCSfile: vtkpxOrthoImageSlice.cpp,v $
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
#include "vtkpxOrthoImageSlice.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataMapper.h"
#include "vtkpxColorMapUtil.h"


//------------------------------------------------------------------------------
vtkpxOrthoImageSlice* vtkpxOrthoImageSlice::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxOrthoImageSlice");
  if(ret)
    {
    return (vtkpxOrthoImageSlice*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxOrthoImageSlice;
}
//------------------------------------------------------------------------------
// Construct object with no children.
vtkpxOrthoImageSlice::vtkpxOrthoImageSlice()
{
  for (int ia=0;ia<=2;ia++)
    {
      this->ImageSlice[ia]=NULL;
      this->Levels[ia]=-1;
    }
  this->Frame=-1;

  this->PolarMode=0;
  this->PolarMiddle=-1.0;
 
  this->AutoOpacity=1;
  this->Opacity=1.0;            
  this->Interpolation=0;      

  this->ColorMap=NULL;           
  this->ColorMode=0;
  this->OwnsColorMap=1;   
  
  this->InputImage=NULL;

  this->ColorMap=vtkLookupTable::New();
  this->ColorMap->SetNumberOfTableValues(256);
  this->ColorMap->SetTableRange(0,255);
  this->DisplayMode=2;

  
  for (int i=0;i<=255;i++)
    {
      float v=float(i)/float(255.0);
      this->ColorMap->SetTableValue(i,v,v,v,1.0);
    }

  this->AutoUpdate=1;
}
//------------------------------------------------------------------------------
vtkpxOrthoImageSlice::~vtkpxOrthoImageSlice()
{
  for (int i=0;i<=2;i++)
  if (this->ImageSlice[i])
    this->ImageSlice[i]->Delete();
  
  if (this->OwnsColorMap)
    {
      if (this->ColorMap)
	this->ColorMap->Delete();
    }

  if (this->InputImage)
    this->InputImage->Delete();

}
//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::SetInput(vtkImageData* image)
{
  if (image==NULL)
    return;

  if (this->InputImage==NULL)
    this->InputImage=vtkImageData::New();

  this->InputImage->ShallowCopy(image);
  BuildSlices();
  if (this->OwnsColorMap)
    vtkpxColorMapUtil::AutoUpdateColormapRange(this->ColorMap,this->InputImage,this->AutoOpacity);
  for (int i=0;i<=2;i++)
    {
      this->ImageSlice[i]->SetInput(this->InputImage);
      this->ImageSlice[i]->SetPolarMode(this->PolarMode);
      this->Levels[i]=this->ImageSlice[i]->GetLevel();
    }
  this->Frame=this->ImageSlice[0]->GetFrame();
}
//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::SetLookupTable(vtkLookupTable* table)
{
  if (!table)
      return;

  if (table==this->ColorMap)
      return;

  if (this->OwnsColorMap)
      {
	this->ColorMap->Delete();
	this->OwnsColorMap=0;
      }

  this->ColorMap=table;  
  for (int i=0;i<=2;i++)
    {
      if (this->ImageSlice[i])
	this->ImageSlice[i]->SetLookupTable(this->ColorMap);
    }
}
//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::SetDisplayMode(int md)
{
  this->DisplayMode=md;
  int hide=0;

  if (this->DisplayMode<0)
    {
      hide=1;
      this->DisplayMode=abs(this->DisplayMode);
    }
  
  for (int i=0;i<=2;i++)
    {
      if (this->ImageSlice[i])
	{
	  if (hide==0 || i==2)
	    this->ImageSlice[i]->SetDisplayMode(this->DisplayMode);
	  else
	    this->ImageSlice[i]->SetDisplayMode(0);
	}
    }
}
//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::SetInterpolation(int on)
{
  this->Interpolation=on>0;
  for (int i=0;i<=2;i++)
    {
      if (this->ImageSlice[i])
	this->ImageSlice[i]->SetInterpolation(this->Interpolation);
    }

}
//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::SetPolarMode(int mode)
{
  this->PolarMode=(mode>0);
  for (int i=0;i<=2;i++)
    {
      if (this->ImageSlice[i])
	this->ImageSlice[i]->SetPolarMode(this->PolarMode);
    }

}

void vtkpxOrthoImageSlice::SetColorMode(int mode)
{
  this->ColorMode=(mode>0);
  for (int i=0;i<=2;i++)
    {
      if (this->ImageSlice[i])
	this->ImageSlice[i]->SetColorMode(this->ColorMode);
    }

}
//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::SetPolarMiddle(float middle)
{
  this->PolarMiddle=middle;
  for (int i=0;i<=2;i++)
    {
      if (this->ImageSlice[i])
	this->ImageSlice[i]->SetPolarMiddle(this->PolarMiddle);
    }
}

//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::SetAutoOpacity(int mode)
{
    this->AutoOpacity=(mode>0);
}

//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::SetOpacity(float opacity)
{
  this->Opacity=opacity;
  if (this->Opacity<0.0)
    this->Opacity=0.0;
  else if (this->Opacity>1.0)
    this->Opacity=1.0;

  for (int i=0;i<=2;i++)
    {
      if (this->ImageSlice[i])
	this->ImageSlice[i]->SetOpacity(this->Opacity);
    }

}
//------------------------------------------------------------------------------ 
void vtkpxOrthoImageSlice::SetLevelsFrame(int level1,int level2,int level3,int frame)
{
  int a=this->AutoUpdate;
  if (a)
    this->SetAutoUpdate(0);
  this->SetFrame(frame);
  if (a)
    this->SetAutoUpdate(1);
  this->SetLevels(level1,level2,level3);
}
//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::SetFrame(int frame)
{
  for (int i=0;i<=2;i++)
    {
      if (this->ImageSlice[i])
	this->ImageSlice[i]->SetFrame(frame);
    }
  if (this->ImageSlice[0])
    this->Frame=this->ImageSlice[0]->GetFrame();
}
//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::SetLevels(int level1,int level2,int level3)
{
  this->Levels[0]=level1;
  this->Levels[1]=level2;
  this->Levels[2]=level3;
  
  for (int i=0;i<=2;i++)
    {
      if (this->ImageSlice[i])
	this->ImageSlice[i]->SetLevel(this->Levels[i]);
    }

}
//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::BuildSlices()
{
  for (int i=0;i<=2;i++)
    {
      if (!this->ImageSlice[i])
	{
	  this->ImageSlice[i]=vtkpxImageSlice::New();
	  this->ImageSlice[i]->SetAutoUpdate(0);
	  this->ImageSlice[i]->SetPolarMode(this->PolarMode);
	  this->ImageSlice[i]->SetColorMode(this->ColorMode);
	  this->ImageSlice[i]->SetCurrentPlane(i);
	  this->ImageSlice[i]->SetLookupTable(this->ColorMap);
	  this->ImageSlice[i]->SetAutoUpdate(this->AutoUpdate);
	  this->AddPart(ImageSlice[i]);
	}
    }
}
//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::Update()
{
  for (int i=0;i<=2;i++)
    if (this->ImageSlice[i]!=NULL)
      this->ImageSlice[i]->Update();
}
//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::SetAutoUpdate(int a)
{
  this->AutoUpdate=(a>0);
  for (int i=0;i<=2;i++)
    if (this->ImageSlice[i]!=NULL)
      this->ImageSlice[i]->SetAutoUpdate(this->AutoUpdate);
}
//------------------------------------------------------------------------------
void vtkpxOrthoImageSlice::UpdateTexture()
{
  for (int i=0;i<=2;i++)
    if (this->ImageSlice[i]!=NULL)
      {
	this->ImageSlice[i]->UpdateTexture();
	this->ImageSlice[i]->Update();
      }
}

