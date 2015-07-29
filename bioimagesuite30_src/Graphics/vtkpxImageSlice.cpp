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
  Module:    $RCSfile: vtkpxImageSlice.cpp,v $
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
#include "vtkpxImageSlice.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataMapper.h"
#include "vtkpxUtil.h"
#include "vtkImageMapToColors.h"
#include "vtkpxColorMapUtil.h"
#include "vtkImageToStructuredPoints.h"

using namespace bioimagesuite_legacy;
//------------------------------------------------------------------------------
vtkpxImageSlice* vtkpxImageSlice::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageSlice");
  if(ret)
    {
    return (vtkpxImageSlice*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageSlice;
}
//------------------------------------------------------------------------------
// Construct object with no children.
vtkpxImageSlice::vtkpxImageSlice()
{
  this->ImageSliceHighlight=NULL;
  this->ImageSliceOutline=NULL;    
  this->ImageSlice=NULL;         
  this->ImagePlane=NULL;         
  this->ImageHighlightPlane=NULL;         
  this->ImageVOI=NULL;           
  this->ImageTexture=NULL; 
  this->CurrentImage=NULL;
  this->Level=-1;              
  this->Frame=0;
  this->NumberOfFrames=1;

  this->Opacity=1.0;            
  this->Interpolation=1;      
  this->ColorMap=NULL;           
  this->OwnsColorMap=1;   

  this->CurrentPlane=2;
  this->DisplayMode=2;
  this->ShowHighlight=0;

  this->ColorMap=vtkLookupTable::New();
  this->ColorMap->SetNumberOfTableValues(256);
  this->ColorMap->SetTableRange(0,255);
  this->ColorMode=0;

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
vtkpxImageSlice::~vtkpxImageSlice()
{
  if (this->CurrentImage)
    this->CurrentImage->Delete();

  if (this->ImagePlane)
    this->ImagePlane->Delete();

  if (this->ImageHighlightPlane)
    this->ImageHighlightPlane->Delete();
  
  if (this->ImageSliceOutline)
    this->ImageSliceOutline->Delete();
  
  if (this->ImageSlice)
    this->ImageSlice->Delete();
  
  if (this->ImageVOI)
    ImageVOI->Delete();
  
  if (this->ImageTexture)
    ImageTexture->Delete();
  
  if (this->ImageHighlightCube)
    this->ImageHighlightCube->Delete();
  
  if (this->ImageSliceHighlight)
    ImageSliceHighlight->Delete();
  
  if (this->OwnsColorMap)
    {
      if (this->ColorMap)
	this->ColorMap->Delete();
    }
}

//------------------------------------------------------------------------------
void vtkpxImageSlice::SetInput(vtkImageData* image)
{
  if (image==NULL)
    return;
  
  if (this->CurrentImage!=NULL)
    this->CurrentImage->Delete();
  this->CurrentImage=vtkImageData::New();
  this->CurrentImage->ShallowCopy(image);
  BuildImageSlice();
  if (this->OwnsColorMap)
    vtkpxColorMapUtil::AutoUpdateColormapRange(this->ColorMap,this->CurrentImage);
  
  int dim[3]; this->CurrentImage->GetDimensions(dim);
  this->NumberOfFrames=this->CurrentImage->GetNumberOfScalarComponents();
  this->Frame=0;
  
  /*  if (this->CurrentPlane==1)
    {
      double spa[3]; this->CurrentImage->GetSpacing(spa);
      fprintf(stderr,"Input Spacing =%.2f %.2f %.2f\n",spa[0],spa[1],spa[2]);
      }*/

  // Create New Image Voi and Texture 
  this->ImageVOI->SetInput(this->CurrentImage);
  this->ImageVOI->Modified();

  double sp[3];   this->CurrentImage->GetSpacing(sp);
  double ori[3];  this->CurrentImage->GetOrigin(ori);

  /*  int uext[6],wext[6]; 
  image->GetUpdateExtent(uext);
  image->GetWholeExtent(wext);
  fprintf(stderr,"Whole Extent =%d %d %d %d %d %d\n",wext[0],wext[1],wext[2],wext[3],wext[4],wext[5]);
  fprintf(stderr,"Updat Extent =%d %d %d %d %d %d\n",uext[0],uext[1],uext[2],uext[3],uext[4],uext[5]);*/
 


  for (int ka=0;ka<=2;ka++)
    {
      OutlineBounds[ka*2]=0.0;
      OutlineBounds[ka*2+1]=dim[ka]*sp[ka]+ori[ka];
    }
  
  UpdateSlice();
}
//------------------------------------------------------------------------------
void vtkpxImageSlice::SetLookupTable(vtkLookupTable* table)
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
void vtkpxImageSlice::SetInterpolation(int on)
{
  int old=this->Interpolation;
  this->Interpolation=(on>0);

  if (this->Interpolation==old)
    return;

  if (!this->ImageTexture)
    return;

  if (this->Interpolation==1)
    {
      this->ImageTexture->InterpolateOn();
      this->ImageVOI->ForcePowerOfTwoOff();
    }
  else
    {
      this->ImageTexture->InterpolateOff();
      this->ImageVOI->ForcePowerOfTwoOn();
    }

this->ImageVOI->Modified();
  this->UpdateSlice();

}
//------------------------------------------------------------------------------
void vtkpxImageSlice::SetOpacity(float opacity)
{
  this->Opacity=Frange(opacity,0.0,1.0);
  if (this->ImageSlice)
    {
      this->ImageSlice->GetProperty()->SetOpacity(this->Opacity);
    }
}
//------------------------------------------------------------------------------
void vtkpxImageSlice::SetFrame(int frame)
{
  if (this->ColorMode==0)
    {
      frame=Irange(frame,0,this->NumberOfFrames-1);
      this->Frame=frame;
      if (this->ImageSlice && this->AutoUpdate==1)
	UpdateSlice();
    }
}
//------------------------------------------------------------------------------
void vtkpxImageSlice::SetColorMode(int cm)
{
  if (this->ColorMode!=cm)
    {
      this->ColorMode=(cm>0);
      if (this->ImageSlice && this->AutoUpdate==1)
	UpdateSlice();
    }
}
//------------------------------------------------------------------------------
void vtkpxImageSlice::SetPolarMode(int mode)
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
void vtkpxImageSlice::SetPolarMiddle(float pm)
{
  if (pm!=this->PolarMiddle)
    {
      this->PolarMiddle=pm;
      if (this->PolarMode && this->ImageSlice)
	UpdateSlice();
    }
}
//------------------------------------------------------------------------------
void vtkpxImageSlice::SetLevel(int level)
{
  if (this->Level!=level)
    {
      this->Level=level;
      if (this->ImageSlice && this->AutoUpdate==1)
	UpdateSlice();
    }
}
//------------------------------------------------------------------------------
void vtkpxImageSlice::SetCurrentPlane(int currentplane)
{
  if (currentplane!=this->CurrentPlane)
    {
      this->CurrentPlane=currentplane;
      if (this->ImageSlice && this->AutoUpdate==1)
	UpdateSlice();
    }
}
//------------------------------------------------------------------------------
void vtkpxImageSlice::SetDisplayMode(int md)
{
  this->DisplayMode=md;
  if (this->ImageSlice)
    {
      this->SetVisibility(1);
      
      if (this->DisplayMode==2 || this->DisplayMode==3)
	this->ImageSlice->SetVisibility(1);
      else
	this->ImageSlice->SetVisibility(0);
      
      
      if (this->DisplayMode==1 || this->DisplayMode==3)
	this->ImageSliceOutline->SetVisibility(1);
      else
	this->ImageSliceOutline->SetVisibility(0);
      
      /*fprintf(stderr,"DIsplayMode=%d ,outline=%d, image=%d\n",
	this->DisplayMode,
	this->ImageSliceOutline->GetVisibility(),
	this->ImageSlice->GetVisibility());*/
    }
}
//------------------------------------------------------------------------------
void vtkpxImageSlice::BuildImageSlice()
{
  //  fprintf(stderr,"In Build Image Slice %d, color mode=%d\n",(int)this,this->ColorMode);

  if (!this->ImageSliceOutline)
    {
      // ImageOutline
      this->ImageSliceOutline=vtkActor::New();
      this->ImageSliceOutline->PickableOff();
      this->ImageSliceOutline->SetVisibility(0);
      
      this->ImagePlane=vtkPlaneSource::New();
      this->ImagePlane->SetXResolution(1);
      this->ImagePlane->SetYResolution(1);
      
      this->ImageHighlightPlane=vtkPlaneSource::New();
      this->ImageHighlightPlane->SetXResolution(1);
      this->ImageHighlightPlane->SetYResolution(1);
      

      vtkPolyDataMapper* mappd=vtkPolyDataMapper::New();
      mappd->SetInput(this->ImageHighlightPlane->GetOutput());
      
      this->ImageSliceOutline->SetMapper(mappd);
      this->ImageSliceOutline->GetProperty()->SetColor(0.5, 0.5, 0.5);
      this->ImageSliceOutline->GetProperty()->SetRepresentationToWireframe();
      this->ImageSliceOutline->GetProperty()->SetAmbient(1.0);
      this->ImageSliceOutline->GetProperty()->SetDiffuse(0.0);
      this->ImageSliceOutline->GetProperty()->SetSpecular(0.0);
      this->ImageSliceOutline->GetProperty()->SetRepresentationToWireframe();
      
      mappd->Delete();   
      
      this->AddPart(ImageSliceOutline);
      
      // ImageSlice
      this->ImageSlice=vtkActor::New();
      this->ImageSlice->PickableOff();
      this->ImageTexture=vtkTexture::New();
      this->ImageVOI=vtkpxImageExtract::New();
      this->ImageVOI->LimitTextureSizeOff();
      if (!this->Interpolation)
	{
	  this->ImageVOI->ForcePowerOfTwoOn();
	  this->ImageVOI->SetMaximumTextureSize(512);
	}

      if (this->ColorMode)
	this->ImageVOI->ExtractAllFramesOn();
      else
	this->ImageVOI->ExtractAllFramesOff();
      
      this->ImageVOI->SetInput((vtkImageData*)NULL);
      
      //setCoordinates(-1,-1);

      this->ImageTexture->SetInput(this->ImageVOI->GetOutput());
      if (this->Interpolation==1)
	this->ImageTexture->InterpolateOn();
      else
	this->ImageTexture->InterpolateOff();

      if (this->ColorMode)
	{
	  this->ImageTexture->SetLookupTable(NULL);
	  this->ImageTexture->MapColorScalarsThroughLookupTableOff();
	}
      else
	{
	  this->ImageTexture->SetLookupTable(this->ColorMap);
	  this->ImageTexture->MapColorScalarsThroughLookupTableOn();
	}
      this->ImageTexture->RepeatOff();

      
      vtkPolyDataMapper* imageMapper=vtkPolyDataMapper::New();
      imageMapper->SetInput(this->ImagePlane->GetOutput());
            
      this->ImageSlice->SetMapper(imageMapper);
      this->ImageSlice->GetProperty()->SetOpacity(this->Opacity);
      this->ImageSlice->GetProperty()->SetInterpolationToFlat();
      this->ImageSlice->SetTexture(this->ImageTexture);
      
      AddPart(this->ImageSlice);
      imageMapper->Delete();  
      
      this->SetDisplayMode(this->DisplayMode);
    }

}
//------------------------------------------------------------------------------
void vtkpxImageSlice::UpdateSlice()
{
  if (!this->ImageVOI)
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

  //  fprintf(stderr,"Updating Slice, level=%d, plane=%d interpolation=%d\n",this->Level,this->CurrentPlane,this->Interpolation);


  int newrange[3];

  if (this->Interpolation == 0 )
    {
      for (int ia=0;ia<=2;ia++)
	{
	  int d=2;
	  while (d<range[ia])
	    d*=2;
	  newrange[ia]=d;
	}
    }
  else
    {
      for (int ia=0;ia<=2;ia++)
	newrange[ia]=range[ia];
    }

  double d1[3];
  double d0[3];
  double d2[3];
  double d3[3];

  for (int i=0;i<=2;i++)
    {
      if (i!=this->CurrentPlane)
	{
	  d3[i]=sp[i]*double(range[i]-0.5)+ori[i];
	  d1[i]=sp[i]*double(newrange[i]-0.5)+ori[i];
	  d0[i]=sp[i]*double(-0.5)+ori[i];
	}
      else
	{
	  d1[i]=sp[i]*double(this->Level)+ori[i];
	  d0[i]=d1[i];
	  d3[i]=d1[i];
	}
      d2[i]=d0[i];
      //      fprintf(stderr,"d1[%d]=%f, d3[%d]=%f\n",i,d1[i],i,d3[i]);
    }

  this->ImageVOI->SetPolarMode(0);
  
  switch (this->CurrentPlane)
    {
    case 0: // yz
      this->ImagePlane->SetOrigin(d1[0] , d0[1] , d0[2]);
      this->ImagePlane->SetPoint1(d1[0] , d1[1] , d0[2]);
      this->ImagePlane->SetPoint2(d1[0] , d0[1] , d1[2]);

      this->ImageHighlightPlane->SetOrigin(d3[0] , d2[1] , d2[2]);
      this->ImageHighlightPlane->SetPoint1(d3[0] , d3[1] , d2[2]);
      this->ImageHighlightPlane->SetPoint2(d3[0] , d2[1] , d3[2]);
      
      this->ImageVOI->SetCurrentPlane(0);
      this->ImageVOI->SetSliceNo(this->Level);
      this->ImageVOI->SetFrame(this->Frame);

      break;
      
    case 1: // xz
      this->ImagePlane->SetOrigin(d0[0] , d1[1] , d0[2]);
      this->ImagePlane->SetPoint1(d1[0] , d1[1] , d0[2]);
      this->ImagePlane->SetPoint2(d0[0] , d1[1] , d1[2]);

      this->ImageHighlightPlane->SetOrigin(d2[0] , d3[1] , d2[2]);
      this->ImageHighlightPlane->SetPoint1(d3[0] , d3[1] , d2[2]);
      this->ImageHighlightPlane->SetPoint2(d2[0] , d3[1] , d3[2]);


      this->ImageVOI->SetCurrentPlane(1);
      this->ImageVOI->SetSliceNo(this->Level);
      this->ImageVOI->SetFrame(this->Frame);
      break;
      
    case 2: // xy
      this->ImagePlane->SetOrigin(d0[0] , d0[1] , d1[2]);
      this->ImagePlane->SetPoint1(d1[0] , d0[1] , d1[2]);
      this->ImagePlane->SetPoint2(d0[0] , d1[1] , d1[2]);

      this->ImageHighlightPlane->SetOrigin(d2[0] , d2[1] , d3[2]);
      this->ImageHighlightPlane->SetPoint1(d3[0] , d2[1] , d3[2]);
      this->ImageHighlightPlane->SetPoint2(d2[0] , d3[1] , d3[2]);

      this->ImageVOI->SetCurrentPlane(2);
      this->ImageVOI->SetSliceNo(this->Level);
      this->ImageVOI->SetFrame(this->Frame);
      break;
    }

  if (this->ColorMode)
    {
      this->ImageVOI->ExtractAllFramesOn();
      this->ImageTexture->MapColorScalarsThroughLookupTableOff();
    }
  else
    {
      this->ImageVOI->ExtractAllFramesOff();
      this->ImageTexture->MapColorScalarsThroughLookupTableOn();
    }
  this->ImageVOI->Modified();
  
  //  this->ImageVOI->Update();

  /*  if (this->ImageSliceHighlight)
      this->SetHighlightBounds(-1,-1, -1,-1, -1,-1);*/

  /*  fprintf(stderr,"Cartesian Image Plane (%d), origin=(%.2f,%.2f,%.2f) p1=(%.2f,%.2f,%.2f) p2=(%.2f,%.2f,%.2f)\n",
	  this->CurrentPlane,
	  this->ImagePlane->GetOrigin()[0],
	  this->ImagePlane->GetOrigin()[1],
	  this->ImagePlane->GetOrigin()[2],
	  this->ImagePlane->GetPoint1()[0],
	  this->ImagePlane->GetPoint1()[1],
	  this->ImagePlane->GetPoint1()[2],
	  this->ImagePlane->GetPoint2()[0],
	  this->ImagePlane->GetPoint2()[1],
	  this->ImagePlane->GetPoint2()[2]);*/

  //  fprintf(stderr,"Cartesian Image Plane / Level this=%d (%d,%d)\n",(int)this,this->CurrentPlane,this->Level);
  //this->ImageSliceOutline->SetVisibility(1);				 
}

// ------------------------------------------------------------------------------------------

void vtkpxImageSlice::SetDisplayHighlight(int hg)
{
  this->ShowHighlight=hg>0;
  if (this->ShowHighlight==0)
      {
	if (this->ImageSliceHighlight)
	    ImageSliceHighlight->SetVisibility(0);
      }
  else
      {
	if (!this->ImageSliceHighlight)
	    this->SetHighlightBounds(-1,-1, -1,-1, -1,-1);

	if (this->ImageSliceHighlight)
	    ImageSliceHighlight->SetVisibility(1);
      }
}
//------------------------------------------------------------------------------
void vtkpxImageSlice::SetHighlightBounds(int b1,int b2,int b3,int b4,int b5,int b6)
{
  if (!this->ImageVOI)
      return;

  if (!this->ImageSliceHighlight)
      {
	
	this->ImageHighlightCube=vtkCubeSource::New();
	vtkPolyDataMapper* mappd=vtkPolyDataMapper::New();
	mappd->SetInput(this->ImageHighlightCube->GetOutput());

	this->ImageSliceHighlight=vtkActor::New();
	this->ImageSliceHighlight->PickableOff();
	this->ImageSliceHighlight->SetMapper(mappd);
	this->ImageSliceHighlight->GetProperty()->SetColor(1.0,0.0,0.0);
	this->ImageSliceHighlight->GetProperty()->SetRepresentationToWireframe();
	this->ImageSliceHighlight->GetProperty()->SetLineWidth(1.0);
	this->ImageSliceHighlight->GetProperty()->SetAmbient(1.0);
	this->ImageSliceHighlight->GetProperty()->SetDiffuse(0.0);
	this->ImageSliceHighlight->GetProperty()->SetSpecular(0.0);
	
	mappd->Delete();   
	this->AddPart(ImageSliceHighlight);
      }

  vtkImageData* img=this->CurrentImage;

  int   range[3];   img->GetDimensions(range);
  double sp[3];     img->GetSpacing(sp);
  double ori[3];    img->GetOrigin(ori);
 
 
  if (b1>=0)      OutlineBounds[0]=b1*sp[0]+ori[0];   
  if (b2>=0)      OutlineBounds[1]=b2*sp[0]+ori[0];
  if (b3>=0)      OutlineBounds[2]=b3*sp[1]+ori[1];  
  if (b4>=0)      OutlineBounds[3]=b4*sp[1]+ori[1]; 
  if (b5>=0)      OutlineBounds[4]=b5*sp[2]+ori[2];  
  if (b6>=0)      OutlineBounds[5]=b6*sp[2]+ori[2];


  // Sort the Levels
  for (int ka=0;ka<=2;ka++)
      for (int kb=0;kb<=1;kb++)
	  OutlineBounds[ka*2+kb]=Frange(OutlineBounds[ka*2+kb],ori[ka],(range[ka]-1)*sp[ka]+ori[ka]);


  double level=this->Level*sp[this->CurrentPlane]+ori[this->CurrentPlane];

  switch (this->CurrentPlane)
    {
    case 0: // saggital
      this->ImageHighlightCube->SetBounds(level-0.1,level+0.1,
					  OutlineBounds[2],OutlineBounds[3],
					  OutlineBounds[4],OutlineBounds[5]);
      break;
      
    case 1: // coronal
      this->ImageHighlightCube->SetBounds(OutlineBounds[0],OutlineBounds[1],
					  level-0.1,level+0.1,
					  OutlineBounds[4],OutlineBounds[5]);
      break;
      
    case 2: // axial
      this->ImageHighlightCube->SetBounds(OutlineBounds[0],OutlineBounds[1],
					  OutlineBounds[2],OutlineBounds[3],
					  level-0.1,level+0.1);
      break;
    }

}

// ------------------------------------------------------------------------------------------
void vtkpxImageSlice::UpdateSlicePolar()
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

  this->ImageVOI->SetPolarMode(1);
  this->ImageVOI->SetPolarMiddle(this->PolarMiddle);

  if (this->CurrentPlane>0)
    {
      double theta=180.0*double(this->Level)/double(dim[2]);
      //fprintf(stderr,"\n\t this=%d,Current Plane = %d  theta=%.2f voi=%d\n",this,CurrentPlane,theta,this->ImageVOI);
      // ------------------------
      // -- Convert To Radians --
      // ------------------------
      theta*=M_PI/180.0;

      this->ImagePlane->SetOrigin(ox-r*cos(theta),ox-r*sin(theta),ori[1]-0.5*sp[0]);
      this->ImagePlane->SetPoint1(ox+r*cos(theta),ox+r*sin(theta),ori[1]-0.5*sp[0]);
      this->ImagePlane->SetPoint2(ox-r*cos(theta),ox-r*sin(theta),ori[1]+(dim[1]-0.5)*sp[0]);

      this->ImageVOI->SetCurrentPlane(this->CurrentPlane);
      this->ImageVOI->SetSliceNo(this->Level);
      this->ImageVOI->SetFrame(this->Frame);
      this->ImageSliceOutline->SetVisibility(0);
    }
  else
    {
      double z=sp[0]*double(this->Level)+ori[1];
      //fprintf(stderr,"\n\t this=%d Current Plane = %d  z=%.2f ImageVOI=%d\n",this,CurrentPlane,z,this->ImageVOI);
      this->ImagePlane->SetOrigin(ox-r,ox-r,z);
      this->ImagePlane->SetPoint1(ox+r,ox-r,z);
      this->ImagePlane->SetPoint2(ox-r,ox+r,z);
      this->ImageVOI->SetCurrentPlane(0);
      this->ImageVOI->SetSliceNo(this->Level);
      this->ImageVOI->SetFrame(this->Frame);
      this->ImageSliceOutline->SetVisibility(1);
    }

  //  fprintf(stderr,"Polar Image Plane / Level this=%d (%d,%d)\n",(int)this,this->CurrentPlane,this->Level);
  this->SetDisplayHighlight(0);

}


void vtkpxImageSlice::UpdateTexture()
{
  if (!this->CurrentImage || !this->ImageSlice)
      return;

  this->ImageSlice->GetTexture()->Modified();
  this->ImageSlice->Modified();

}


