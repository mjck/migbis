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
  Module:    $RCSfile: vtkpx2DImageSlice.cpp,v $
  Language:  C++
  Date:      $Date: 2004/02/03 15:28:43 $
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
#include "vtkpx2DImageSlice.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataMapper.h"
#include "vtkpxUtil.h"
#include "vtkImageMapToColors.h"
#include "vtkpxColorMapUtil.h"
#include "vtkImageToStructuredPoints.h"
//------------------------------------------------------------------------------
vtkpx2DImageSlice* vtkpx2DImageSlice::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpx2DImageSlice");
  if(ret)
    {
    return (vtkpx2DImageSlice*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpx2DImageSlice;
}
//------------------------------------------------------------------------------
// Construct object with no children.
vtkpx2DImageSlice::vtkpx2DImageSlice()
{
  this->ImageSlice=NULL;         
  this->ImagePlane=NULL;         
  this->ImageTexture=NULL; 
  this->CurrentImage=NULL;

  this->Level=-1;              
  this->Opacity=1.0;            
  this->Interpolation=0;      
  this->ColorMap=NULL;           
  this->OwnsColorMap=1;   

  this->CurrentPlane=2;

  this->ColorMap=vtkLookupTable::New();
  this->ColorMap->SetNumberOfTableValues(256);
  this->ColorMap->SetTableRange(0,255);

  this->PolarMode=0;
  this->PolarMiddle=-1.0;
  
  for (int i=0;i<=255;i++)
    {
      float v=float(i)/float(255.0);
      this->ColorMap->SetTableValue(i,v,v,v,1.0);
    }
  
  this->AutoUpdate=1;
}
//------------------------------------------------------------------------------
vtkpx2DImageSlice::~vtkpx2DImageSlice()
{
  if (this->CurrentImage)
    this->CurrentImage->Delete();

  if (this->ImagePlane)
    this->ImagePlane->Delete();
  
  if (this->ImageSlice)
    this->ImageSlice->Delete();
  
  if (this->ImageTexture)
    ImageTexture->Delete();
  
  if (this->OwnsColorMap)
    {
      if (this->ColorMap)
	this->ColorMap->Delete();
    }
}

//------------------------------------------------------------------------------
void vtkpx2DImageSlice::SetInput(vtkImageData* image)
{
  if (image==NULL)
    return;
  
  int dim[3]; image->GetDimensions(dim);

  if (dim[0]!=1 && dim[1]!=1 && dim[2]!=1)
    {
      //      fprintf(stderr,"3D Image Input Ignoring %d,%d,%d\n",dim[0],dim[1],dim[2]);
      return;
    }
	

  if (this->CurrentImage==NULL)
    this->CurrentImage=vtkImageData::New();

  this->CurrentImage->ShallowCopy(image);

  BuildImageSlice();
  if (this->OwnsColorMap)
    vtkpxColorMapUtil::AutoUpdateColormapRange(this->ColorMap,this->CurrentImage);
  
  UpdateSlice();
}
//------------------------------------------------------------------------------
void vtkpx2DImageSlice::SetLookupTable(vtkLookupTable* table)
{
  if (!table)
      return;

  if (table==this->ColorMap)
      return;

  if (this->ImageTexture)
      this->ImageTexture->SetLookupTable(table);

  if (this->OwnsColorMap)
      {
	this->ColorMap->Delete();
	this->OwnsColorMap=0;
      }

  this->ColorMap=table;  
  this->Update();
}
//------------------------------------------------------------------------------
void vtkpx2DImageSlice::SetInterpolation(int on)
{
  this->Interpolation=on>0;
  if (this->ImageTexture)
    {
      if (this->Interpolation==1)
	this->ImageTexture->InterpolateOn();
      else
	this->ImageTexture->InterpolateOff();
    }
}
//------------------------------------------------------------------------------
void vtkpx2DImageSlice::SetOpacity(float opacity)
{
  this->Opacity=Frange(opacity,0.0,1.0);
  if (this->ImageSlice)
    {
      this->ImageSlice->GetProperty()->SetOpacity(this->Opacity);
    }
}
//------------------------------------------------------------------------------
void vtkpx2DImageSlice::SetPolarMode(int mode)
{
  int p=(mode>0);
  if (p!=this->PolarMode)
    {
      this->PolarMode=p;
      if (this->ImageSlice && this->AutoUpdate)
	UpdateSlice();
    }
}
//------------------------------------------------------------------------------
void vtkpx2DImageSlice::SetPolarMiddle(float pm)
{
  if (pm!=this->PolarMiddle)
    {
      this->PolarMiddle=pm;
      if (this->PolarMode && this->ImageSlice)
	UpdateSlice();
    }
}
//------------------------------------------------------------------------------
void vtkpx2DImageSlice::SetLevel(int level)
{
  if (this->Level!=level)
    {
      this->Level=level;
      if (this->ImageSlice && this->AutoUpdate==1)
	UpdateSlice();
    }
}
//------------------------------------------------------------------------------
void vtkpx2DImageSlice::SetCurrentPlane(int currentplane)
{
  if (currentplane!=this->CurrentPlane)
    {
      this->CurrentPlane=currentplane;
      if (this->ImageSlice && this->AutoUpdate==1)
	UpdateSlice();
    }
}
//------------------------------------------------------------------------------
void vtkpx2DImageSlice::BuildImageSlice()
{
  if (!this->ImageSlice)
    {
      this->ImagePlane=vtkPlaneSource::New();
      this->ImagePlane->SetXResolution(1);
      this->ImagePlane->SetYResolution(1);
      
      // ImageSlice
      this->ImageSlice=vtkActor::New();
      this->ImageSlice->PickableOff();
      this->ImageTexture=vtkTexture::New();
      
      this->ImageTexture->SetInput(this->CurrentImage);
      if (this->Interpolation==0)
	this->ImageTexture->InterpolateOn();
      else
	this->ImageTexture->InterpolateOff();
      this->ImageTexture->SetLookupTable(this->ColorMap);
      this->ImageTexture->MapColorScalarsThroughLookupTableOn();
      this->ImageTexture->RepeatOff();
      
      vtkPolyDataMapper* imageMapper=vtkPolyDataMapper::New();
      imageMapper->SetInput(this->ImagePlane->GetOutput());
            
      this->ImageSlice->SetMapper(imageMapper);
      this->ImageSlice->GetProperty()->SetOpacity(this->Opacity);
      this->ImageSlice->GetProperty()->SetInterpolationToFlat();
      this->ImageSlice->SetTexture(this->ImageTexture);
      
      AddPart(this->ImageSlice);
      imageMapper->Delete();  
    }
}
//------------------------------------------------------------------------------  
void vtkpx2DImageSlice::UpdateTexture()
{
  if (!this->CurrentImage || !this->ImageSlice)
      return;

  this->ImageSlice->GetTexture()->Modified();
  this->ImageSlice->Modified();

}
//------------------------------------------------------------------------------  
void vtkpx2DImageSlice::UpdateSlice()
{
  if (!this->CurrentImage)
      return;

  if (this->PolarMode)
    {
      UpdateSlicePolar();
      return;
    }

  vtkImageData* img=this->CurrentImage;
  int range[3];  img->GetDimensions(range);
  double sp[3];   img->GetSpacing(sp);
  double ori[3];  img->GetOrigin(ori);

  this->CurrentPlane=Irange(this->CurrentPlane,0,2);
  
  if (this->Level==-1)
      this->Level=range[this->CurrentPlane]/2;
  
  this->Level=Irange(this->Level,0,range[this->CurrentPlane]-1);

  double d1[3];
  double d0[3];
  for (int i=0;i<=2;i++)
      {
	if (i!=this->CurrentPlane)
	    {
	      d1[i]=sp[i]*double(range[i]-0.5)+ori[i];
	      d0[i]=sp[i]*double(-0.5)+ori[i];
	    }
	else
	    {
	      d1[i]=sp[i]*double(this->Level)+ori[i];
	      d0[i]=d1[i];
	    }
      }

  switch (this->CurrentPlane)
    {
    case 0: // yz
      this->ImagePlane->SetOrigin(d1[0] , d0[1] , d0[2]);
      this->ImagePlane->SetPoint1(d1[0] , d1[1] , d0[2]);
      this->ImagePlane->SetPoint2(d1[0] , d0[1] , d1[2]);
      break;
      
    case 1: // xz
      this->ImagePlane->SetOrigin(d0[0] , d1[1] , d0[2]);
      this->ImagePlane->SetPoint1(d1[0] , d1[1] , d0[2]);
      this->ImagePlane->SetPoint2(d0[0] , d1[1] , d1[2]);
      break;
      
    case 2: // xy
      this->ImagePlane->SetOrigin(d0[0] , d0[1] , d1[2]);
      this->ImagePlane->SetPoint1(d1[0] , d0[1] , d1[2]);
      this->ImagePlane->SetPoint2(d0[0] , d1[1] , d1[2]);
      break;
    }
  this->ImagePlane->Update();
  
}

// ------------------------------------------------------------------------------------------
void vtkpx2DImageSlice::UpdateSlicePolar()
{
  vtkImageData* img=this->CurrentImage;
  int dim[3];  img->GetDimensions(dim);
  double sp[3];   img->GetSpacing(sp);
  double ori[3];  img->GetOrigin(ori);

  this->CurrentPlane=Irange(this->CurrentPlane,0,2);
  
  if (this->Level<0)
    {
      if (this->CurrentPlane == 0)
	this->Level=dim[1]/2;
      else
	this->Level=dim[2]/2;
    }

  if (this->CurrentPlane==0 && this->Level>=dim[1])
    {
      this->Level=dim[1]-1;
    }
  else if (this->CurrentPlane>0 && this->Level>=dim[2])
    {
      this->Level=dim[2]-1;
    }


  double r=0.5*double(dim[0])*sp[0];
  double ox=ori[0]+double(sp[0]*(dim[0]-1))*0.5;

  if (this->CurrentPlane>0)
    {
      double theta=180.0*double(this->Level)/double(dim[2]);
      theta*=M_PI/180.0;
      this->ImagePlane->SetOrigin(ox-r*cos(theta),ox-r*sin(theta),ori[1]-0.5*sp[0]);
      this->ImagePlane->SetPoint1(ox+r*cos(theta),ox+r*sin(theta),ori[1]-0.5*sp[0]);
      this->ImagePlane->SetPoint2(ox-r*cos(theta),ox-r*sin(theta),ori[1]+(dim[1]-0.5)*sp[0]);
    }
  else
    {
      double z=sp[0]*double(this->Level)+ori[1];
      this->ImagePlane->SetOrigin(ox-r,ox-r,z);
      this->ImagePlane->SetPoint1(ox+r,ox-r,z);
      this->ImagePlane->SetPoint2(ox-r,ox+r,z);
    }

  this->ImagePlane->Update();

}




