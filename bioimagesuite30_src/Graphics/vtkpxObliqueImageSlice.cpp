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
  Module:    $RCSfile: vtkpxObliqueImageSlice.cpp,v $
  Language:  C++
  Date:      $Date: 2004/03/19 14:14:48 $
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
#include "vtkpxObliqueImageSlice.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataWriter.h"
#include "vtkpxUtil.h"
#include "vtkImageMapToColors.h"
#include "vtkpxColorMapUtil.h"
#include "vtkOutlineFilter.h"
#include "vtkMath.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
//------------------------------------------------------------------------------
vtkpxObliqueImageSlice* vtkpxObliqueImageSlice::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxObliqueImageSlice");
  if(ret)
    {
    return (vtkpxObliqueImageSlice*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxObliqueImageSlice;
}
//------------------------------------------------------------------------------
// Construct object with no children.
vtkpxObliqueImageSlice::vtkpxObliqueImageSlice()
{
  this->ImageMapper=NULL;
  this->ImageComponentExtractor=NULL;
  this->ImageSliceOutline=NULL;    
  this->ImageSlice=NULL;         
  this->ImagePlane=NULL;         
  this->InputImage=NULL;

  this->ImageTexture=NULL; 
  this->BaseImageReslicer=NULL;
  this->BaseImageTransform=NULL;

  this->InitialPoints=vtkPoints::New();
  this->InitialPoints->SetNumberOfPoints(3);

      
  this->Frame=0;
  this->Resolution=256;
  this->NumberOfFrames=1;
  this->ShowNormalArrow=0;
  this->NormalArrowColor=0;

  this->Interpolation=1;      
  this->Opacity=1.0;

  this->ColorMap=NULL;           
  this->OwnsColorMap=1;   
  this->DisplayMode=3;

  this->ColorMap=vtkLookupTable::New();
  this->ColorMap->SetNumberOfTableValues(256);
  this->ColorMap->SetTableRange(0,255);
  this->ColorMode=0;

  for (int i=0;i<=255;i++)
    {
      double v=double(i)/double(255.0);
      this->ColorMap->SetTableValue(i,v,v,v,1.0);
    }

  this->Normal[0]=0.0;   this->Normal[1]=0.0;  this->Normal[2]=1.0; 
  this->Center[0]=0.0;   this->Center[1]=0.0;  this->Center[2]=0.0;
  this->Distance=0.0;
 
  this->AutoUpdate=1;
}
//------------------------------------------------------------------------------
vtkpxObliqueImageSlice::~vtkpxObliqueImageSlice()
{
  if (this->ImageComponentExtractor!=NULL)
    this->ImageComponentExtractor->Delete();

  if (this->InputImage)
    this->InputImage->Delete();

  if (this->ImagePlane)
    this->ImagePlane->Delete();

  if (this->ImageMapper)
    this->ImageMapper->Delete();
  
  if (this->ImageSliceOutline)
    this->ImageSliceOutline->Delete();
  
  if (this->ImageSlice)
    this->ImageSlice->Delete();
  

  if (this->BaseImageReslicer)
    this->BaseImageReslicer->Delete();

  if (this->BaseImageTransform)
    this->BaseImageTransform->Delete();

  if (this->ImageTexture)
    ImageTexture->Delete();

  this->InitialPoints->Delete();

  if (this->OwnsColorMap)
    {
      if (this->ColorMap)
	this->ColorMap->Delete();
    }
}

//------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::SetInput(vtkImageData* image)
{
  if (image==NULL)
    return;

  int dochange=1;
  
  if (this->InputImage==NULL)
    this->InputImage=vtkImageData::New();

  int dim1[3]; double sp1[3],ori1[3];
  image->GetDimensions(dim1); image->GetSpacing(sp1); image->GetOrigin(ori1);
  int dim2[3]; double sp2[3],ori2[3];
  this->InputImage->GetDimensions(dim2); this->InputImage->GetSpacing(sp2); this->InputImage->GetOrigin(ori2);
  
  int sum=0;
  for (int ia=0;ia<=2;ia++)
    {
      sum+=   abs(dim1[ia] -dim2[ia]);
      sum+=( fabs(sp1[ia]  -sp2[ia])>0.01);
      sum+=( fabs(ori1[ia] -ori2[ia])>0.01);
    }
  if (sum==0)
    dochange=0;

  this->InputImage->ShallowCopy(image);
  this->InputImage->Modified();

  BuildImageSlice();
  if (this->OwnsColorMap)
    vtkpxColorMapUtil::AutoUpdateColormapRange(this->ColorMap,image);
  
  int dim[3]; image->GetDimensions(dim);
  this->NumberOfFrames=image->GetNumberOfScalarComponents();
  this->Frame=0;
  
  // Create New Image Voi and Texture 
  this->ImageComponentExtractor->SetInput(this->InputImage);
  
  if (this->ColorMode)
    {
      this->ImageComponentExtractor->SetComponents(0,1,2);
    }
  else
    {
      this->ImageComponentExtractor->SetComponents(this->Frame);
    }
  
  this->ImageComponentExtractor->Modified();

  if (dochange)
    {
      double sp[3];   this->InputImage->GetSpacing(sp);
      double ori[3];  this->InputImage->GetOrigin(ori);
      
      double length2=0.0,length[3];
      for (int ka=0;ka<=2;ka++)
	{
	  length[ka]=sp[ka]*(dim[ka]-1);
	  length2+=pow(length[ka],2.0);
	}
      length2=sqrt(length2);
      
      for (int ka=0;ka<=2;ka++)
	{
	  if (ka==2)
	    {
	      OutlineBounds[ka*2]=0.0;
	      OutlineBounds[ka*2+1]=double(dim[ka]-1)*sp[ka]+ori[ka];
	    }
	  else
	    {
	      float half=0.5*(length2-length[ka]);
	      OutlineBounds[ka*2]=ori[ka]-half;
	      OutlineBounds[ka*2+1]=double(dim[ka]-1)*sp[ka]+ori[ka]+half;
	    }
	}
      

      double p1[3],p2[3],p3[3];
      
      this->ImagePlane->SetOrigin(OutlineBounds[0],OutlineBounds[2],0.5*(OutlineBounds[4]+OutlineBounds[5]));
      this->ImagePlane->GetOrigin(p1); 
      this->InitialPoints->SetPoint(0,p1);
      
      this->ImagePlane->SetPoint1(OutlineBounds[1],OutlineBounds[2],0.5*(OutlineBounds[4]+OutlineBounds[5]));
      this->ImagePlane->GetPoint1(p2); 
      this->InitialPoints->SetPoint(1,p2);
      
      this->ImagePlane->SetPoint2(OutlineBounds[0],OutlineBounds[3],0.5*(OutlineBounds[4]+OutlineBounds[5]));
      this->ImagePlane->GetPoint2(p3); 
      this->InitialPoints->SetPoint(2,p3);
      
      for (int ia=0;ia<=2;ia++)
	this->Center[ia]=0.5*(OutlineBounds[ia*2]+OutlineBounds[ia*2+1]);
      
      for (int ia=0;ia<=2;ia++)
	this->BaseImageOrigin[ia]=p1[ia];
      this->BaseImageSpacing[0]=(OutlineBounds[1]-OutlineBounds[0]-1.0)/double(this->Resolution);
      this->BaseImageSpacing[1]=(OutlineBounds[3]-OutlineBounds[2]-1.0)/double(this->Resolution);
      this->BaseImageSpacing[2]=1.0;
      
      this->BaseImageExtent[0]=0;      this->BaseImageExtent[2]=0;      this->BaseImageExtent[4]=0;
      this->BaseImageExtent[1]=this->Resolution-1;
      this->BaseImageExtent[3]=this->Resolution-1;
      this->BaseImageExtent[5]=0;
      
      this->Distance=0.0;
      this->Normal[0]=0.0;   this->Normal[1]=0.0;  this->Normal[2]=1.0;  
      this->ArrowSource->SetHeight(0.2*(OutlineBounds[5]-OutlineBounds[4]));
      this->ArrowSource->Modified();

    }
  UpdateSlice();
}
//------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::SetLookupTable(vtkLookupTable* table)
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
  /*  if (this->ImageMapper!=NULL)
      this->ImageMapper->SetLookupTable(this->ColorMap);*/
}
//------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::SetInterpolation(int on)
{
  this->Interpolation=on>0;
  if (this->ImageTexture)
      {
	if (this->Interpolation==1)
	  {
	    this->ImageTexture->InterpolateOn();
	    this->BaseImageReslicer->SetInterpolationModeToLinear();
	  }
	else
	  {
	    this->ImageTexture->InterpolateOff();
	    this->BaseImageReslicer->SetInterpolationModeToNearestNeighbor();
	  }
      }
}
//------------------------------------------------------------------------------

void vtkpxObliqueImageSlice::SetOpacity(double opacity)
{
  this->Opacity=Frange(opacity,0.0,1.0);
  if (this->ImageSlice)
    this->ImageSlice->GetProperty()->SetOpacity(this->Opacity);
}
//------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::SetResolution(int res)
{
  int r=Irange(res,2,1024);

  if (this->Resolution==r)
    return;

  this->Resolution=r;

  if (this->ImagePlane)
    {
      this->BaseImageSpacing[0]=(OutlineBounds[1]-OutlineBounds[0])/double(this->Resolution);
      this->BaseImageSpacing[1]=(OutlineBounds[3]-OutlineBounds[2])/double(this->Resolution);
      this->BaseImageSpacing[2]=1.0;

      this->BaseImageExtent[0]=0;      this->BaseImageExtent[2]=0;      this->BaseImageExtent[4]=0;
      this->BaseImageExtent[1]=this->Resolution-1;
      this->BaseImageExtent[3]=this->Resolution-1;
      this->BaseImageExtent[5]=0;

      this->BaseImageReslicer->SetOutputOrigin(this->BaseImageOrigin);
      this->BaseImageReslicer->SetOutputSpacing(this->BaseImageSpacing);
      this->BaseImageReslicer->SetOutputExtent(this->BaseImageExtent);


      if (this->ImageSlice && this->AutoUpdate==1)
	{
	  this->BaseImageReslicer->Modified();
	  UpdateSlice();
	}
    }
}
//------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::SetFrame(int frame)
{
  if (this->ColorMode==0)
    {
      frame=Irange(frame,0,this->NumberOfFrames-1);
      this->Frame=frame;
      
      if (this->ImageComponentExtractor!=NULL)
	{
	  this->ImageComponentExtractor->SetComponents(this->Frame);
	  this->ImageComponentExtractor->Update();
	}
      if (this->ImageSlice && this->AutoUpdate==1)
	UpdateSlice();
    }
}

void vtkpxObliqueImageSlice::SetColorMode(int cm)
{
  if (this->ColorMode!=cm)
    {
      this->ColorMode=(cm>0);
      if (this->ImageComponentExtractor!=NULL)
	{
	  if (this->ColorMode)
	    this->ImageComponentExtractor->SetComponents(0,1,2);
	  else
	    this->ImageComponentExtractor->SetComponents(this->Frame);
	}
      if (this->ImageSlice && this->AutoUpdate==1)
	UpdateSlice();
    }
}

//------------------------------------------------------------------------------


void vtkpxObliqueImageSlice::UpdateTexture()
{
  if (!this->InputImage || !this->ImageSlice)
      return;

  this->ImageSlice->GetTexture()->Modified();
  this->ImageSlice->Modified();
}


//------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::SetDisplayMode(int md)
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
      
    }
}
//------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::BuildImageSlice()
{
  if (this->ImageSliceOutline!=NULL)
    return;

  // Everything goes through this one 
  this->ImageComponentExtractor=vtkImageExtractComponents::New();
  this->ImageComponentExtractor->SetComponents(0);

  this->ImagePlane=vtkPlaneSource::New();
  this->ImagePlane->SetXResolution(2);//this->Resolution);
  this->ImagePlane->SetYResolution(2);//this->Resolution);
  this->ImagePlane->SetOrigin(-1.0,-1.0,0.0);
  this->ImagePlane->SetPoint1(1.0,-1.0,0.0);
  this->ImagePlane->SetPoint2(-1.0,1.0,0.0);
  



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Outline
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Mapper 
  vtkPolyDataMapper* mappd=vtkPolyDataMapper::New();
  mappd->SetInput(this->ImagePlane->GetOutput());

  // Actor
  this->ImageSliceOutline=vtkActor::New();
  this->ImageSliceOutline->PickableOff();
  this->ImageSliceOutline->SetVisibility(0);
  this->ImageSliceOutline->SetMapper(mappd);
  this->ImageSliceOutline->GetProperty()->SetColor(0.5, 0.5, 0.0);
  this->ImageSliceOutline->GetProperty()->SetRepresentationToWireframe();
  this->ImageSliceOutline->GetProperty()->SetAmbient(1.0);
  this->ImageSliceOutline->GetProperty()->SetDiffuse(0.0);
  this->ImageSliceOutline->GetProperty()->SetSpecular(0.0);

  mappd->Delete();   
  
  this->AddPart(ImageSliceOutline);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Slice 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  this->BaseImageTransform=vtkTransform::New();

  this->BaseImageReslicer=vtkbisImageReslice::New();
  this->BaseImageReslicer->OptimizationOff();
  this->BaseImageReslicer->SetOutputOrigin(this->BaseImageOrigin);
  this->BaseImageReslicer->SetOutputSpacing(this->BaseImageSpacing);
  this->BaseImageReslicer->SetOutputExtent(this->BaseImageExtent);
  this->BaseImageReslicer->SetInput(this->ImageComponentExtractor->GetOutput());
  this->BaseImageReslicer->SetResliceTransform(this->BaseImageTransform);
  this->BaseImageReslicer->SetInterpolationModeToLinear();
  
  this->ImageTexture=vtkTexture::New();
  this->ImageTexture->SetInput(this->BaseImageReslicer->GetOutput());

  if (this->Interpolation==0)
    this->ImageTexture->InterpolateOn();
  else
    this->ImageTexture->InterpolateOff();

  this->ImageTexture->SetLookupTable(this->ColorMap);
  this->ImageTexture->MapColorScalarsThroughLookupTableOn();
  this->ImageTexture->RepeatOff();
      
  this->ImageMapper=vtkPolyDataMapper::New();
  this->ImageMapper->SetInput(this->ImagePlane->GetOutput());

  this->ImageSlice=vtkActor::New();
  this->ImageSlice->SetMapper(this->ImageMapper);
  this->ImageSlice->GetProperty()->SetOpacity(1.0);
  this->ImageSlice->GetProperty()->SetInterpolationToFlat();
  this->ImageSlice->SetTexture(this->ImageTexture);
  
  AddPart(this->ImageSlice);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Arrow 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  this->ArrowSource=vtkConeSource::New();
  this->ArrowSource->SetHeight(20.0);
  this->ArrowSource->SetRadius(5.0);
  this->ArrowSource->SetDirection(1,0,0);
  this->ArrowSource->SetCenter(0,0,0);
  this->ArrowSource->CappingOn();

  vtkPolyDataMapper* map=vtkPolyDataMapper::New();
  map->SetInput(this->ArrowSource->GetOutput());

  this->NormalArrow=vtkActor::New();
  this->NormalArrow->SetMapper(map); map->Delete();
  this->NormalArrow->SetVisibility(this->ShowNormalArrow);
  if (this->NormalArrowColor==0)
    this->NormalArrow->GetProperty()->SetColor(1,0,0);
  else
    this->NormalArrow->GetProperty()->SetColor(0,1,0);
  AddPart(this->NormalArrow);

  
  this->SetDisplayMode(this->DisplayMode);

}
//------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::SetShowNormalArrow(int a)
{
  this->ShowNormalArrow=(a>0);
  this->NormalArrow->SetVisibility(this->ShowNormalArrow);
  if (this->NormalArrowColor==0)
    this->NormalArrow->GetProperty()->SetColor(1,0,0);
  else
    this->NormalArrow->GetProperty()->SetColor(0,1,0);

  this->NormalArrow->Modified();
}

void vtkpxObliqueImageSlice::UpdateSlice()
{
  if (!this->BaseImageTransform)
    return;

  double f1[3]; this->ImagePlane->GetCenter(f1);
  double n1[3]; this->ImagePlane->GetNormal(n1);
  for (int ia=0;ia<=2;ia++)
    f1[ia]=f1[ia]+0.5*n1[ia]*this->ArrowSource->GetHeight();
  
  this->ArrowSource->SetCenter(f1);
  this->ArrowSource->SetDirection(n1);
  this->ArrowSource->Update();

  this->BaseImageReslicer->SetOutputOrigin(this->BaseImageOrigin);
  this->BaseImageReslicer->SetOutputSpacing(this->BaseImageSpacing);
  this->BaseImageReslicer->SetOutputExtent(this->BaseImageExtent);
  this->BaseImageReslicer->Modified();
  this->BaseImageReslicer->Update();

  
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


  this->ImagePlane->Modified();
  this->ImageSliceOutline->Modified();
  this->ImageSliceOutline->GetMapper()->Modified();
  this->ImageTexture->Modified();
  this->Modified();
  //  fprintf(stderr,"Oblique Slice --> Image Slice Updated %.2f,%.2f,%.2f\n",n1[0],n1[1],n1[2]);

}
// ------------------------------------------------------------------------------------------
vtkImageData* vtkpxObliqueImageSlice::GetObliqueImage()
{
  if (this->BaseImageReslicer==NULL)
    return NULL;

  return this->BaseImageReslicer->GetOutput();
}
// ------------------------------------------------------------------------------------------
vtkTransform* vtkpxObliqueImageSlice::GetObliqueTransform()
{
  if (this->BaseImageTransform==NULL)
    return NULL;

  return this->BaseImageTransform;
}
// ------------------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::GetImageRange(float bounds[6])
{
  if (this->ImageComponentExtractor==NULL)
    {
      for (int i=0;i<=2;i++)
	{
	  bounds[i*2]=0;
	  bounds[i*2+1]=1;
	}
      return;
    }

  vtkImageData* img=(this->ImageComponentExtractor->GetOutput());

  int dim[3]; img->GetDimensions(dim);
  double spa[3]; img->GetSpacing(spa);
  double ori[3]; img->GetOrigin(ori);
  for (int i=0;i<=2;i++)
    {
      bounds[i*2]=ori[i];
      bounds[i*2+1]=ori[i]+spa[i]*double(dim[i]-1);
    }
}

// ------------------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::UpdateImagePlane(vtkCamera* camera,int mode)
{
  double c[3]; camera->GetPosition(c);
  double f[3]; camera->GetFocalPoint(f);
  double d[2]; camera->GetClippingRange(d);

  double mid;

  if (mode==0)
    mid=0.5*(d[0]+d[1]);
  else
    mid=0.98*d[0]+0.02*d[1];


  double n[3],magn=0.0;
  for (int i=0;i<=2;i++)
    {
      n[i]=f[i]-c[i];
      magn+=n[i]*n[i];
    }
  magn=sqrt(magn);

  for (int j=0;j<=2;j++)
    {
      n[j]/=magn;
      f[j]=c[j]+mid*n[j];
    }

  double f1[3]; this->ImagePlane->GetCenter(f1);
  double n1[3]; this->ImagePlane->GetNormal(n1);

  double dist=0.0;
  for (int ia=0;ia<=2;ia++)
    dist+=fabs(f1[ia]-f[ia])+10.0*fabs(n1[ia]-n[ia]);

				  
  if (dist>1e-3)
    this->UpdateImagePlane(f[0],f[1],f[2],n[0],n[1],n[2]);

  

}

//------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::Reset()
{
  if (this->ImageComponentExtractor==NULL)
    return;

  this->ImageComponentExtractor->Modified();
  this->ImageComponentExtractor->Update();

  vtkImageData* image=this->ImageComponentExtractor->GetOutput();

  double sp[3];   image->GetSpacing(sp);
  double ori[3];  image->GetOrigin(ori);
  int  dim[3];   image->GetDimensions(dim);

  for (int ka=0;ka<=2;ka++)
    {
      OutlineBounds[ka*2]=0.0;
      OutlineBounds[ka*2+1]=(dim[ka]-1)*sp[ka]+ori[ka];
    }
 
  this->ImagePlane->SetOrigin(OutlineBounds[0],OutlineBounds[2],0.5*(OutlineBounds[4]+OutlineBounds[5]));
  this->ImagePlane->SetPoint1(OutlineBounds[1],OutlineBounds[2],0.5*(OutlineBounds[4]+OutlineBounds[5]));
  this->ImagePlane->SetPoint2(OutlineBounds[0],OutlineBounds[3],0.5*(OutlineBounds[4]+OutlineBounds[5]));

  for (int ia=0;ia<=2;ia++)
    this->Center[ia]=0.5*(OutlineBounds[ia*2]+OutlineBounds[ia*2+1]);
  this->Distance=0.0;
  this->Normal[0]=0.0;   this->Normal[1]=0.0;  this->Normal[2]=1.0; 

  UpdateSlice();
}
// ------------------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::UpdateImagePlane(double dist,double n1,double n2,double n3)
{
  double magn=0.0;
  this->Normal[0] = n1;  this->Normal[1] = n2; this->Normal[2]=n3;

  for (int j=0;j<=2;j++)
    magn+=this->Normal[j]*this->Normal[j];


  magn=sqrt(magn);
  if (magn==0.0)
    {
      this->Normal[0]=0; 
      this->Normal[1]=0; 
      this->Normal[2]=1.0;
    } 
  else
    {
      for (int j=0;j<=2;j++)
	this->Normal[j]=this->Normal[j]/magn;
    }

  this->Distance=dist;

  for (int ia=0;ia<=2;ia++)
    this->Center[ia]=0.5*(OutlineBounds[ia*2]+OutlineBounds[ia*2+1])+this->Distance*this->Normal[ia];
  


  this->InternalUpdatePlane();
}

// ------------------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::UpdateImagePlane(double x1,double y1,double z1,
					      double n1,double n2,double n3)
{
  this->Normal[0] = n1;  this->Normal[1] = n2; this->Normal[2]=n3;
  this->Center[0]=  x1;  this->Center[1]=  y1; this->Center[2]=z1;

  this->Distance=sqrt(pow( this->Center[0] - 0.5*(OutlineBounds[0]+OutlineBounds[1]),2.0)+
		      pow( this->Center[1] - 0.5*(OutlineBounds[2]+OutlineBounds[3]),2.0)+
		      pow( this->Center[2] - 0.5*(OutlineBounds[4]+OutlineBounds[5]),2.0));
		      


  double magn=0.0;
  for (int j=0;j<=2;j++)
    magn+=this->Normal[j]*this->Normal[j];

  magn=sqrt(magn);
  if (magn==0.0)
    {
      this->Normal[0]=0; 
      this->Normal[1]=0; 
      this->Normal[2]=1.0;
    } 
  else
    {
      for (int j=0;j<=2;j++)
	this->Normal[j]=this->Normal[j]/magn;

    }
  
  this->InternalUpdatePlane();
}
// ------------------------------------------------------------------------------------------
void vtkpxObliqueImageSlice::InternalUpdatePlane()
{
  if (!this->ImagePlane)
    return;

  double thisn[3];   thisn[0]=0; thisn[1]=0; thisn[2]=1.0;
  
  // Compute rotation vector using a transformation matrix.
  // Note that if normals are parallel then the rotation is either
  // 0 or 180 degrees.
  //double dp = vtkMath::Dot(this->Normal,thisn);
  double dp=this->Normal[2];
  
  double p0[3],p1[3],p2[3];
  this->InitialPoints->GetPoint(0,p0);
  this->InitialPoints->GetPoint(1,p1);
  this->InitialPoints->GetPoint(2,p2);
  double theta=0.0,rotVector[3],center[3];

  for (int ia=0;ia<=2;ia++)
    {
      rotVector[ia]=p1[ia]-p0[ia];
      center[ia]=0.5*(OutlineBounds[ia*2]+OutlineBounds[ia*2+1]);
    }
      
 
  if ( dp >= 1.0 )
    {
      theta=0.0;
    }
  else if ( dp <= -1.0 )
    {
      theta = 180.0;
    }
  else
    {
      vtkMath::Cross(thisn,(double*)this->Normal,rotVector);
#if ((VTK_MAJOR_VERSION >= 5)&&(VTK_MINOR_VERSION >= 4))
      theta = vtkMath::DegreesFromRadians(acos((double)dp));
#else
      theta = acos((double)dp) / vtkMath::DoubleDegreesToRadians();
#endif

    }

  // create rotation matrix
  this->BaseImageTransform->Identity();
  this->BaseImageTransform->PostMultiply();
  this->BaseImageTransform->Translate(-center[0],-center[1],-center[2]);
  this->BaseImageTransform->RotateWXYZ(theta,rotVector[0],rotVector[1],rotVector[2]);
  this->BaseImageTransform->Translate(this->Center[0],this->Center[1],this->Center[2]);
  this->BaseImageTransform->TransformPoint(p0,p0); this->ImagePlane->SetOrigin(p0);
  this->BaseImageTransform->TransformPoint(p1,p1); this->ImagePlane->SetPoint1(p1);
  this->BaseImageTransform->TransformPoint(p2,p2); this->ImagePlane->SetPoint2(p2);
  this->ImagePlane->Modified();

  this->UpdateSlice(); 
}
// ------------------------------------------------------------------------------------------

vtkImageData* vtkpxObliqueImageSlice::GetMaskImage(vtkImageData* input,int above,int binary)
{
  if (input==NULL)
    return NULL;

  float abv=-1.0;
  if (above>0)
    abv=1.0;
  binary=(binary>0);

  //  fprintf(stderr,"Above = %.2f binary=%d\n",abv,binary);

  double c[3]; ImagePlane->GetCenter(c);
  //  fprintf(stderr,"C=%.2f,%.2f,%.2f\n",c[0],c[1],c[2]);
  double n[3]; ImagePlane->GetNormal(n);

  vtkImageData* img=vtkImageData::New();
  img->CopyStructure(input);
  img->SetNumberOfScalarComponents(input->GetNumberOfScalarComponents());
  if (binary)
    {
      img->SetNumberOfScalarComponents(1);
      img->SetScalarTypeToChar();
    }
  img->AllocateScalars();

  int np=img->GetNumberOfPoints();
  int nc=img->GetNumberOfScalarComponents();
  vtkDataArray* data=img->GetPointData()->GetScalars();
  vtkDataArray* inp=input->GetPointData()->GetScalars();
  for (int i=0;i<nc;i++)
    data->FillComponent(i,0.0);

  for (int i=0;i<np;i++)
    {
      double x[3];
      img->GetPoint(i,x);
      double sum=0.0;
      for (int ia=0;ia<=2;ia++)
	sum+= (x[ia]-c[ia])*n[ia];

      if (sum*abv>0)
	{
	  if (binary)
	    {
	      data->SetComponent(i,0,1);
	    }
	  else
	    {
	      for (int c=0;c<nc;c++)
		data->SetComponent(i,c,inp->GetComponent(i,c));
	    }
	}
    }
  return img;
}



