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



#include <stdlib.h>
#include <math.h>
#include "pxutil.h"
#include "vtkColorTransferFunction.h"
#include "vtkImageShiftScale.h"
#include "vtkLookupTable.h"
#include "vtkObjectFactory.h"
#include "vtkPiecewiseFunction.h"
#include "vtkImageData.h"
#include "vtkAbstractVolumeMapper.h"
#include "vtkVolumeMapper.h"
#include "vtkVolumeProperty.h"
#include "vtkVolumeRayCastMIPFunction.h"
//#include "vtkCroppedVolumeRayCastCompositeFunction.h"
//#include "vtkCroppedVolumeRayCastMapper.h"



#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkVolumeRayCastMapper.h"

#include "vtkFixedPointVolumeRayCastMapper.h"

#include "vtkCropVolumeRayCastMapper.h"

#include "vtkVolumeTextureMapper2D.h"
#include "vtkpxVolume.h"
#include "vtkImageReslice.h"
#include "vtkIdentityTransform.h"
#include "vtkImageContinuousDilate3D.h"
#include "vtkImageExtractComponents.h"
#include "vtkpxColorMapUtil.h"
#include "vtkImageBlend.h"
#include "vtkImageMapToRGBA.h"
#include "vtkpxUtil.h"
#include<vtkVolumeTextureMapper3D.h>
#include "vtkImageMapToColors.h"



using namespace bioimagesuite_legacy;


//------------------------------------------------------------------------------
vtkpxVolume* vtkpxVolume::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxVolume");
  if(ret)
    {
    return (vtkpxVolume*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxVolume;
}


// Creates a Volume with the following defaults: origin(0,0,0)
// position=(0,0,0) scale=1 visibility=1 pickable=1 dragable=
//------------------------------------------------------------------------------

vtkpxVolume::vtkpxVolume()
{
  this->VolumeRenderingMode= 2;
  this->ResliceMode=-1;
  this->TargetSampleDistance=1.0;
  this->InterpolationMode=1;
  this->Orientation=0;
  this->ShadingMode                 = 0;
  this->MinValue                   = 0.0;
  this->MaxValue                   = 255.0;
  this->MaxIntensity               = 255;
  this->probeLocation[0] = this->probeLocation[1] = this->probeLocation[2] = 0.0;

  this->CropShape = 0;
  this->CropSphereRadius = 50.0;
  this->gradientEnhancementMode = 0;

  this->ShiftScale                  = NULL;
  this->SecondShiftScale                  = NULL;

  this->cTFun                       = NULL;
  this->oTFun                       = NULL;

  this->oTFun = vtkPiecewiseFunction::New();
  this->oTFun->AddSegment(0, 0.0, 255, 1.0);

  this->dualTransferFunction=vtkPiecewiseFunction::New();
  this->dualTransferFunction->AddSegment(0, 0.0, 255, 1.0);

  // Create a transfer function mapping scalar value to color (grey)
  this->cTFun = vtkColorTransferFunction::New();
  this->cTFun->AddRGBPoint(0,0.0,0.0,0.0);
  this->cTFun->AddRGBPoint(255,1.0,1.0,1.0);

  this->gTFun = vtkPiecewiseFunction::New();
  this->gTFun->AddSegment(0, 0.0, 255, 1.0);

  this->gTFun2 = vtkPiecewiseFunction::New();
  this->gTFun2->AddSegment(0, 1.0, 255, 1.0);


  this->LookupTable=vtkLookupTable::New();
  vtkpxColorMapUtil::SetStepColorMap(this->LookupTable,0,255,0,255,256,1);

  this->SecondLookupTable=vtkLookupTable::New();
  vtkpxColorMapUtil::SetStepColorMap(this->SecondLookupTable,0,255,0,255,256,1);

  this->Property = vtkVolumeProperty::New();
  if (ShadingMode)
    this->Property->ShadeOn();
  else
    this->Property->ShadeOff();
  this->Property->SetInterpolationTypeToLinear();
  this->Property->SetColor(this->cTFun);
  this->Property->SetScalarOpacity(0,this->oTFun);

  this->ImageReslice=NULL;
  this->SecondImageReslice=NULL;
  this->ImageMapToColors=vtkImageMapToColors::New();
  this->ImageMapToColors->SetOutputFormatToRGBA();
  //this->ImageMapToColors->SetLookupTable(this->LookupTable());

  this->Frame=0;
  this->NumberOfFrames=1;
  this->ExtractFrame=NULL;
  this->SecondExtractFrame=NULL;

  for (int ia=0;ia<=2;ia++)
    {
      this->TargetDimensions[ia]=64;
      this->TargetOrigin[ia]=0.0;
      this->TargetSpacing[ia]=1.0;
    }
  this->InputImage=vtkImageData::New();
  this->SecondInputImage=NULL;
  this->DualInputMode=0;
  this->ImageBlend=NULL;
  this->ImageMapToRGBA1=NULL;
  this->ImageMapToRGBA2=NULL;
  this->RGBAMode=0;
  this->RGBAMode_changed = 0;
}
//------------------------------------------------------------------------------
// Destruct a volume
vtkpxVolume::~vtkpxVolume()
{
  if (this->ShiftScale )
    this->ShiftScale->Delete();

  if (this->SecondShiftScale )
    this->SecondShiftScale->Delete();

  if (this->oTFun )
    this->oTFun->Delete();

  if (this->cTFun )
    this->cTFun->Delete();

  if (this->dualTransferFunction )
    this->dualTransferFunction->Delete();

  if (this->ImageReslice )
    this->ImageReslice->Delete();

  if (this->SecondImageReslice )
    this->SecondImageReslice->Delete();

  if (this->ImageMapToRGBA1)
    this->ImageMapToRGBA1->Delete();

  if (this->ImageMapToRGBA2)
    this->ImageMapToRGBA2->Delete();

  if (this->ImageBlend)
    this->ImageBlend->Delete();

  if (this->ImageMapToColors)
    this->ImageMapToColors->Delete();
  if (this->ExtractFrame)
    this->ExtractFrame->Delete();

  if (this->SecondExtractFrame)
    this->SecondExtractFrame->Delete();


  if (this->InputImage)
    this->InputImage->Delete();

  if (this->SecondInputImage)
    this->SecondInputImage->Delete();

  if (this->LookupTable)
    this->LookupTable->Delete();

  if (this->SecondLookupTable)
    this->SecondLookupTable->Delete();

}
//------------------------------------------------------------------------------

void vtkpxVolume::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkProp3D::PrintSelf(os,indent);

  if( this->Property )
    {
      os << indent << "Property:\n";
      this->Property->PrintSelf(os,indent.GetNextIndent());
    }
  else
    {
      os << indent << "Property: (not defined)\n";
    }

  if( this->Mapper )
    {
      os << indent << "Mapper:\n";
      this->Mapper->PrintSelf(os,indent.GetNextIndent());
    }
  else
    {
      os << indent << "Mapper: (not defined)\n";
    }

  // make sure our bounds are up to date
  if ( this->Mapper )
    {
      this->GetBounds();
      os << indent << "Bounds: (" << this->Bounds[0] << ", "
	 << this->Bounds[1] << ") (" << this->Bounds[2] << ") ("
	 << this->Bounds[3] << ") (" << this->Bounds[4] << ") ("
	 << this->Bounds[5] << ")\n";
    }
  else
    {
      os << indent << "Bounds: (not defined)\n";
    }
}

//------------------------------------------------------------------------------
void vtkpxVolume::SetSecondLookupTable(vtkLookupTable* table)
{
  if (!table)
    return;

  vtkpxColorMapUtil::CopyLookupTable(table,this->SecondLookupTable);
}

void vtkpxVolume::SetFromLookupTable(vtkLookupTable* table)
{
  if (!table)
    return;

  vtkpxColorMapUtil::CopyLookupTable(table,this->LookupTable);

  if (!this->oTFun)
    this->oTFun = vtkPiecewiseFunction::New();

  if (!this->cTFun)
    this->cTFun = vtkColorTransferFunction::New();

  this->oTFun->RemoveAllPoints();
  this->oTFun->ClampingOn();

  this->cTFun->RemoveAllPoints();
  this->cTFun->ClampingOn();

  double dat[2];  table->GetTableRange(dat);
  double diff=dat[1]-dat[0];
  if ( fabs(dat[1]-dat[0])<1e-7)
    {
      dat[1]=dat[0]+1e-7;
    }
  diff=dat[1]-dat[0]+1.0;
  double step=diff/float(table->GetNumberOfColors());

  //  fprintf(stderr,"Dat = %.2f:%.2f step=%f\n",dat[0],dat[1],step);

  for (int i=0;i<table->GetNumberOfColors();i++)
    {
      double val[4];
      double base=float(i)*step+dat[0];
      table->GetTableValue(i,val);
      this->cTFun->AddRGBPoint(base,val[0],val[1],val[2]);
      //      if (i>245 && i <250)
      //	fprintf(stderr,"%.2f (%.2f, %.2f,%.2f)\n",base,val[0],val[1],val[2]);
      this->oTFun->AddPoint(base,val[3]);
    }

  if (this->ImageMapToColors!=NULL)
    {
      this->ImageMapToColors->SetLookupTable(this->LookupTable);
      this->ImageMapToColors->Modified();
    }
}
//------------------------------------------------------------------------------
void vtkpxVolume::UpdateRange()
{
  int dim[3]; this->InputImage->GetDimensions(dim);
  double range[2];  this->InputImage->GetPointData()->GetScalars()->GetRange(range,this->Frame);

  this->MinValue=range[0];
  this->MaxValue=range[1];

  if (ShiftScale==NULL)
    return;

  if (fabs(this->MinValue-this->MaxValue)<1e-4)
    {
      ShiftScale->SetShift(0);
      ShiftScale->SetScale(1.0);
      this->MinValue=0.0;
      this->MaxValue=255.0;
    }
  else if (fabs(range[0]-range[1])<1.0)
    {
      float sc=255.0/(range[1]-range[0]);
      ShiftScale->SetShift(-range[0]);
      ShiftScale->SetScale(sc);
      this->MinValue=0.0;
      this->MaxValue=255.0;
    }
  else if (range[0]>=0.0 && range[1]<25.0)
    {
      ShiftScale->SetShift(0);
      ShiftScale->SetScale(10.0);
    }
  else if (range[0]>=0.0 && range[1]<256.0)
    {
      ShiftScale->SetShift(0);
      ShiftScale->SetScale(1.0);
    }
  else if (range[0]>=0.0 && range[1]<512.0)
    {
      ShiftScale->SetShift(0);
      ShiftScale->SetScale(0.5);
      this->MaxValue=0.5*this->MaxValue;
    }
  else
    {
      float sc=255.0/(range[1]-range[0]);
      ShiftScale->SetShift(-range[0]);
      ShiftScale->SetScale(sc);
      this->MinValue=0.0;
      this->MaxValue=255.0;
    }

  /*  fprintf(stderr,"Shift=%f, Scale=%f range=%f:%f\n",
      ShiftScale->GetShift(),ShiftScale->GetScale(),range[0],range[1]);*/

  this->MaxIntensity=(int)this->MaxValue;
  this->ShiftScale->Modified();


  if (this->SecondShiftScale!=NULL)
    {
      double range[2];  this->SecondInputImage->GetPointData()->GetScalars()->GetRange(range,this->Frame);

      if (fabs(range[1]-range[0])<1e-4)
	{
	  this->SecondShiftScale->SetShift(0);
	  this->SecondShiftScale->SetScale(1.0);
	}
      else if (range[0]>=0.0 && range[1]<256.0)
	{
	  this->SecondShiftScale->SetShift(0);
	  this->SecondShiftScale->SetScale(1.0);
	}
      else if (range[0]>=0.0 && range[1]<512.0)
	{
	  this->SecondShiftScale->SetShift(0);
	  this->SecondShiftScale->SetScale(0.5);
	}
      else
	{
	  float sc=255.0/(range[1]-range[0]);
	  this->SecondShiftScale->SetShift(-range[0]);
	  this->SecondShiftScale->SetScale(sc);
	}
      this->SecondShiftScale->Modified();
    }
}
//------------------------------------------------------------------------------
void vtkpxVolume::SetFrame(int fr)
{
  if (!this->ExtractFrame)
    return;

  int oldframe=this->Frame;
  this->Frame=Irange(fr,0,this->NumberOfFrames-1);
  if (oldframe!=this->Frame)
    {
      this->ExtractFrame->SetComponents(this->Frame);
      if (this->SecondExtractFrame!=NULL)
	{
	  int fr=this->SecondInputImage->GetNumberOfScalarComponents();
	  int sfr=this->Frame;
	  if (this->Frame>=fr)
	    sfr=fr-1;
	  this->SecondExtractFrame->SetComponents(sfr);
	  //	  fprintf(stderr,"Frame=%d, sfr=%d\n",this->Frame,sfr);
	}
      this->UpdateRange();
      this->Modified();
    }

}
//------------------------------------------------------------------------------
void vtkpxVolume::SetInput(vtkImageData* image)
{
  this->SetInput(image,0);
}
//------------------------------------------------------------------------------
void vtkpxVolume::SetInput(vtkImageData* image,int orientation)
{
  if (image==NULL)
    return;

  this->Orientation=Irange(orientation,0,3);

  if (this->InputImage!=NULL)
    this->InputImage->Delete();
  this->InputImage=vtkImageData::New();
  this->InputImage->ShallowCopy(image);

  int dim[3]; this->InputImage->GetDimensions(dim);

  this->NumberOfFrames=this->InputImage->GetNumberOfScalarComponents();
  this->Frame=0;

  if (this->ExtractFrame!=NULL)
    this->ExtractFrame->Delete();

  this->ExtractFrame=vtkImageExtractComponents::New();
  this->ExtractFrame->SetInput(this->InputImage);
  this->ExtractFrame->SetComponents(this->Frame);
  this->ExtractFrame->Modified();

  if (ShiftScale!=NULL)
    this->ShiftScale->Delete();

  ShiftScale=vtkImageShiftScale::New();
  ShiftScale->SetOutputScalarTypeToUnsignedChar();
  ShiftScale->ClampOverflowOn();
  ShiftScale->SetInput(this->ExtractFrame->GetOutput());

  if (ImageReslice!=NULL)
    this->ImageReslice->Delete();

  ImageReslice=vtkImageReslice::New();
  ImageReslice->SetInput(this->ShiftScale->GetOutput());

  this->UpdateRange();
  this->GetImageRange(cropping_range);
  this->SetResliceInterpolationModes(abs(this->ResliceMode),this->InterpolationMode);

  if (this->Mapper)
    {
      this->Mapper->Delete();
      this->Mapper=NULL;
    }
  this->Modified();

  this->DualInputMode=0;

  this->Update();
}
// -----------------------------------------------------------------------------------------------
void vtkpxVolume::SetSecondInput(vtkImageData* image)
{
  if (image==NULL || this->ImageReslice==NULL)
    return;

  int nc=image->GetNumberOfScalarComponents();
  int onc=this->InputImage->GetNumberOfScalarComponents();

  if (onc<nc)
    {
      this->DualInputMode=0;
      return;
    }

  if (this->SecondInputImage==NULL)
    this->SecondInputImage=vtkImageData::New();
  this->SecondInputImage->ShallowCopy(image);

  if (this->SecondExtractFrame!=NULL)
    this->SecondExtractFrame->Delete();

  int fr=this->Frame;
  if (fr>=nc)
    fr=nc-1;

  this->SecondExtractFrame=vtkImageExtractComponents::New();
  this->SecondExtractFrame->SetInput(this->SecondInputImage);
  this->SecondExtractFrame->SetComponents(fr);
  this->SecondExtractFrame->Modified();

  if (SecondShiftScale!=NULL)
    this->SecondShiftScale->Delete();

  SecondShiftScale=vtkImageShiftScale::New();
  SecondShiftScale->SetOutputScalarTypeToUnsignedChar();
  SecondShiftScale->ClampOverflowOn();
  SecondShiftScale->SetInput(this->SecondExtractFrame->GetOutput());

  if (SecondImageReslice!=NULL)
    this->SecondImageReslice->Delete();

  SecondImageReslice=vtkImageReslice::New();
  SecondImageReslice->SetInput(this->SecondShiftScale->GetOutput());

  this->UpdateRange();
  this->GetImageRange(cropping_range);
  this->SetResliceInterpolationModes(abs(this->ResliceMode),this->InterpolationMode,1);


  // ------------------ But there is more ---------------------------------------
  //        Blend Images
  // ----------------------------------------------------------------------------

  if (this->ImageMapToRGBA1!=NULL)
    this->ImageMapToRGBA1->Delete();

  this->ImageMapToRGBA1=vtkImageMapToRGBA::New();
  this->ImageMapToRGBA1->SetInput(this->ImageReslice->GetOutput());
  this->ImageMapToRGBA1->SetLookupTable(this->LookupTable);

  if (this->ImageMapToRGBA2!=NULL)
    this->ImageMapToRGBA2->Delete();

  this->ImageMapToRGBA2=vtkImageMapToRGBA::New();
  this->ImageMapToRGBA2->SetInput(this->SecondImageReslice->GetOutput());
  this->ImageMapToRGBA2->SetLookupTable(this->SecondLookupTable);

  if (this->ImageBlend!=NULL)
    this->ImageBlend->Delete();

  this->ImageBlend=vtkImageBlend::New();
  this->ImageBlend->AddInput(this->ImageMapToRGBA1->GetOutput());
  this->ImageBlend->AddInput(this->ImageMapToRGBA2->GetOutput());
  this->ImageBlend->SetBlendModeToNormal();
  this->DualInputMode=1;
  //  fprintf(stderr,"Done Setting Second Input, DualInputMode=%d\n",this->DualInputMode);
}
//------------------------------------------------------------------------------
void vtkpxVolume::SetMapper(vtkVolumeMapper *mapper)
{
  Update();
}
//------------------------------------------------------------------------------
void vtkpxVolume::SetProperty(vtkVolumeProperty *property)
{
  return;
}
//------------------------------------------------------------------------------
void vtkpxVolume::SetProbeLocation(double location[3]) // use another function name.
{

  for(int ia=0;ia<3;ia++)
    {
      probeLocation[ia] = (location[ia]-this->TargetOrigin[ia])/this->TargetSpacing[ia];

    }
  //printf("pl :%f %f %f\n",  probeLocation[0], probeLocation[1], probeLocation[2]);

  this->Modified();
  this->Update();
}
//------------------------------------------------------------------------------
void vtkpxVolume::SetIrregularCroppingMode(int inputCropMode)
{
  if (inputCropMode<-1)
    inputCropMode=-1;
  else if (inputCropMode>1)
    inputCropMode=1;

  if (cropMode==inputCropMode)
      return;

  cropMode = inputCropMode;

  //printf("crop mode is %d\n", inputCropMode);
  this->Update();
}
//------------------------------------------------------------------------------
void vtkpxVolume::SetGradientEnhancementMode(int inputgradientEnhancementMode)
{
  if (inputgradientEnhancementMode<-1)
    inputgradientEnhancementMode=-1;
  else if (inputgradientEnhancementMode>1)
    inputgradientEnhancementMode=1;

  if (gradientEnhancementMode==inputgradientEnhancementMode)
      return;

  gradientEnhancementMode = inputgradientEnhancementMode;

  this->Update();
}

//------------------------------------------------------------------------------
void vtkpxVolume::SetCropValues(int Control_shapeIndex,
								double Control_SphereRadius,
								double Control_CubeLengthX,
								double Control_CubeLengthY,
								double Control_CubeLengthZ,
								double Control_EllipseRadius_X,
								double Control_EllipseRadius_Y,
								double Control_EllipseRadius_Z,
								double Control_CylinderRadius,
								double Control_CylinderHeight,
								double normal_vector[3])
{


	CropShape = Control_shapeIndex;

	CropSphereRadius = Control_SphereRadius/this->TargetSpacing[0];
	CropCubeLengthX = Control_CubeLengthX/this->TargetSpacing[0];
	CropCubeLengthY = Control_CubeLengthY/this->TargetSpacing[1];
	CropCubeLengthZ = Control_CubeLengthZ/this->TargetSpacing[2];
	CropEllipseRadiusX = Control_EllipseRadius_X;
	CropEllipseRadiusY = Control_EllipseRadius_Y;
	CropEllipseRadiusZ = Control_EllipseRadius_Z;


	CropCylinderRadius = Control_CylinderRadius/this->TargetSpacing[0];
	CropCylinderHeight = Control_CylinderHeight;

	direction_vector[0] = normal_vector[0];
	direction_vector[1] = normal_vector[1];
	direction_vector[2] = normal_vector[2];

    this->Update();
	this->Modified();
}
//------------------------------------------------------------------------------
void vtkpxVolume::SetVolumeRenderingMode(int m)
{
  // Alark: Note If Changing number of modes add here

  if (m<0 || m > 5)
    m=0;

  if (m==5)
    m=4;
  
  if (m==this->VolumeRenderingMode)
    return;
  this->VolumeRenderingMode=m;
  //  fprintf(stderr,"Setting Volume Rendering Mode=%d\n",this->VolumeRenderingMode);
  this->Modified();
}
//------------------------------------------------------------------------------
void vtkpxVolume::SetShadingMode(int shad)
{
  shad=shad>0;
  if (shad==ShadingMode)
    return;
  else
    ShadingMode=shad;
  this->Modified();
}
//------------------------------------------------------------------------------
int vtkpxVolume::RenderTranslucentGeometry( vtkViewport *vp )
{
  if (!this->Mapper)
    this->Update();
  // ALARK LOOK AT THIS
  //return vtkVolume::RenderTranslucentGeometry(vp);
  return 1;
}
//------------------------------------------------------------------------------
void vtkpxVolume::GetImageRange(int bounds[6])
{
  int dim[3]; this->InputImage->GetDimensions(dim);
  for (int i=0;i<=2;i++)
    {
      bounds[i*2]=0;
      bounds[i*2+1]=dim[i]-1;
    }
}
//------------------------------------------------------------------------------
void vtkpxVolume::GetCroppingRange(int bounds[6])
{
  for (int ia=0;ia<=5;ia++)
      bounds[ia]=cropping_range[ia];
}
//------------------------------------------------------------------------------

void vtkpxVolume::GetProbeLocation(double location[3])
{
  for (int ia=0;ia<3;ia++)
      location[ia]=probeLocation[ia];
}

//------------------------------------------------------------------------------

void vtkpxVolume::ResetCroppingRange()
{
  GetImageRange(cropping_range);
  vtkVolumeMapper* map=(vtkVolumeMapper*)this->GetMapper();
  if (map!=NULL)
    map->CroppingOff();
}

// ---------------------------------------------------------------------------
void vtkpxVolume::SetResliceMode(int re)
{
  SetResliceInterpolationModes(re,this->InterpolationMode);
}

void vtkpxVolume::SetInterpolationMode(int interp)
{
  SetResliceInterpolationModes(this->ResliceMode,interp);
}


void vtkpxVolume::SetRGBAMode(int m)
{
   int q=(m>0);
   if (q==this->RGBAMode)
   {
	   this->RGBAMode_changed = 0;
	   return;
   }

   printf("RGBA mode is %d\n", m);
   this->RGBAMode=m;
   this->RGBAMode_changed = 1;
   this->Update();
}

void vtkpxVolume::SetDualInputMode(int dm)
{
  dm=(dm>0);
  if (this->DualInputMode==dm)
    return;

  if (dm==1 && this->SecondImageReslice==NULL)
    return;

  this->DualInputMode=dm;
  //  fprintf(stderr,"Setting Dual Input Mode = %d\n",dm);
  if (this->Mapper)
    {
      this->Mapper->Delete();
      this->Mapper=NULL;
    }
  this->Modified();
}

// ---------------------------------------------------------------------------

void vtkpxVolume::SetResliceInterpolationModes(int re,int interp,int force)
{
  re=Irange(abs(re),0,9);
  interp=Irange(interp,0,2);
  if (interp==2)
    interp=3;
  int doreslice=0;
  if (force==0)
    {
      if (this->ResliceMode!=re)
	{
	  this->ResliceMode=re;
	  doreslice=1;
	}

      if (this->InterpolationMode!=interp)
	{
	  this->InterpolationMode=interp;
	  doreslice=1;
	}
    }
  else
    doreslice=1;

  if (this->ImageReslice==NULL  || this->ShiftScale==NULL || doreslice==0)
    return;

  int dim[3];  double sp[3],ori[3];
  this->InputImage->GetDimensions(dim);
  this->InputImage->GetSpacing(sp);
  this->InputImage->GetOrigin(ori);


  int ia;

  switch (this->ResliceMode)
    {
    case 0:
      for (ia=0;ia<=2;ia++)
	TargetDimensions[ia]=dim[ia];
      break;
    case 1:
      for (ia=0;ia<=2;ia++)
	TargetDimensions[ia]=dim[ia]/2;
      break;
    case 2:
      for (ia=0;ia<=2;ia++)
	TargetDimensions[ia]=dim[ia]/4;
      break;
    case 3:
      TargetDimensions[0]=64;
      TargetDimensions[1]=64;
      TargetDimensions[2]=64;
      break;
    case 4:
      TargetDimensions[0]=128;
      TargetDimensions[1]=128;
      TargetDimensions[2]=64;
      break;
    case 5:
      TargetDimensions[0]=128;
      TargetDimensions[1]=128;
      TargetDimensions[2]=128;
      break;
    case 6:
      TargetDimensions[0]=256;
      TargetDimensions[1]=256;
      TargetDimensions[2]=128;
      break;
    case 7:
      TargetDimensions[0]=256;
      TargetDimensions[1]=256;
      TargetDimensions[2]=256;
      break;
    case 8:
      TargetDimensions[0]=512;
      TargetDimensions[1]=512;
      TargetDimensions[2]=256;
      break;
    case 9:
      TargetDimensions[0]=512;
      TargetDimensions[1]=512;
      TargetDimensions[2]=512;
      break;
    default:
      TargetDimensions[0]=64;
      TargetDimensions[1]=64;
      TargetDimensions[2]=64;
      break;
    }

  /*  fprintf(stderr,"Using Reslice Mode = %d Dims=%d x %d x %d\n",this->ResliceMode,this->TargetDimensions[0],
	  this->TargetDimensions[1],
	  this->TargetDimensions[2]);*/

  for (ia=0;ia<=2;ia++)
    if (TargetDimensions[ia]<2)
      TargetDimensions[ia]=2;

  if (this->Mapper!=NULL)
    {
      for (int ia=0;ia<=2;ia++)
	{
	  dim[ia]=cropping_range[ia*2+1]-cropping_range[ia*2];
	  ori[ia]=ori[ia]+sp[ia]*cropping_range[ia*2];
	}
    }

  //  fprintf(stderr,"\n\nInput Dimensions = %d,%d,%d\n",dim[0],dim[1],dim[2]);
  //  fprintf(stderr,"Input Spacing = %.2f,%.2f,%.2f\n",sp[0],sp[1],sp[2]);

  this->TargetSampleDistance=0.0;
  for (ia=0;ia<=2;ia++)
    {
      if (TargetDimensions[ia]>dim[ia])
	TargetDimensions[ia]=dim[ia];

      if (TargetDimensions[ia]<4)
	{
	  this->InterpolationMode=0;
	  TargetDimensions[ia]=4;
	}
      this->TargetSpacing[ia]=sp[ia]*double(dim[ia])/double(TargetDimensions[ia]);
      if (ia!=0)
	this->TargetSampleDistance=Fmin(this->TargetSampleDistance,this->TargetSpacing[ia]);
      else
	this->TargetSampleDistance=this->TargetSpacing[ia];
      this->TargetOrigin[ia]=ori[ia];
    }

  //fprintf(stderr,"Output Dimensions = %d,%d,%d\n",TargetDimensions[0],TargetDimensions[1],TargetDimensions[2]);
  //  fprintf(stderr,"Output Spacing = %.2f,%.2f,%.2f\n\n",TargetSpacing[0],TargetSpacing[1],TargetSpacing[2]);
  //  fprintf(stderr,"Output Origin = %.2f,%.2f,%.2f\n\n",TargetOrigin[0],TargetOrigin[1],TargetOrigin[2]);
  //  fprintf(stderr,"Recomputed Target Sample Distance=%.2f\n",this->TargetSampleDistance);

  ImageReslice->SetOutputOrigin(this->TargetOrigin);
  ImageReslice->AutoCropOutputOff();

  ImageReslice->SetOutputSpacing(this->TargetSpacing);
  ImageReslice->SetOutputExtent(0,TargetDimensions[0]-1,0,TargetDimensions[1]-1,0,TargetDimensions[2]-1);
  ImageReslice->SetInterpolationMode(this->InterpolationMode);
  ImageReslice->OptimizationOff();
  ImageReslice->Modified();

  if (this->SecondImageReslice!=NULL)
    {
      SecondImageReslice->SetOutputOrigin(this->TargetOrigin);
      SecondImageReslice->SetOutputSpacing(this->TargetSpacing);
      SecondImageReslice->SetOutputExtent(0,TargetDimensions[0]-1,0,TargetDimensions[1]-1,0,TargetDimensions[2]-1);
      SecondImageReslice->SetInterpolationMode(0);
      SecondImageReslice->Modified();
    }

}



// ---------------------------------------------------------------------------

void vtkpxVolume::SetCroppingRange(int bounds[6])
{
  if (this->ShiftScale==NULL)
    return;

  vtkVolumeMapper* map=(vtkVolumeMapper*)this->GetMapper();

  if (map==NULL || this->InputImage==NULL)
    return;


  for (int ia=0;ia<=5;ia++)
    cropping_range[ia]=bounds[ia];

  for (int ind1=0;ind1<=4;ind1+=2)
    {
      int ind2=ind1+1;
      if (cropping_range[ind1]==cropping_range[ind2])
	{
	  if (cropping_range[ind1]>0)
	    cropping_range[ind1]-=1;
	  else
	    cropping_range[ind2]+=1;
	}
      else if (cropping_range[ind1]>cropping_range[ind2])
	{
	  cropping_range[ind1]=bounds[ind2];
	  cropping_range[ind2]=bounds[ind1];
	}
    }

  map->CroppingOff();
  /*  fprintf(stderr,"Cropping Range =%d:%d, %d:%d, %d:%d\n",
	  cropping_range[0],
	  cropping_range[1],
	  cropping_range[2],
	  cropping_range[3],
	  cropping_range[4],
	  cropping_range[5]);*/
  this->SetResliceInterpolationModes(abs(this->ResliceMode),this->InterpolationMode,1);
}

// ----------------------------------------------------------------------------------
//   Update and Helper Functions
// ----------------------------------------------------------------------------------

int vtkpxVolume::Update_IsModeTheSame()
{
  if (this->Mapper==NULL )
    return 0;

  // Alark: Note If Changing number of modes add here

  //printf("volume rendering mode is %d\n", this->VolumeRenderingMode);

  switch (this->VolumeRenderingMode)
    {
    case 0:
		return this->Mapper->IsA("vtkFixedPointVolumeRayCastMapper");
      break;

    case 1:
      return this->Mapper->IsA("vtkVolumeRayCastMapper");
      break;

    case 2:
      return this->Mapper->IsA("vtkVolumeTextureMapper2D");
      break;
    case 3:
      return this->Mapper->IsA("vtkVolumeTextureMapper3D");
      break;
	
    case 4:
      return this->Mapper->IsA("vtkCropVolumeRayCastMapper");
      break;

    case 5:
      return 0;
      break;

    case 6:
      return this->Mapper->IsA("vtkFixedPointVolumeRayCastMapper");
      break;
    }

  return 0;
}
//------------------------------------------------------------------------------
int vtkpxVolume::Update_UpdateMapper()
{

  // Either Bad Mapper
  if ( this->Mapper == NULL)
    {
      vtkErrorMacro(<<"No Mapper Set\n");
      return 0;
    }


  if (this->Mapper->IsA("vtkVolumeTextureMapper2D"))
    {
      ((vtkVolumeTextureMapper2D*)this->Mapper)->SetTargetTextureSize(this->TargetDimensions[0],
								      this->TargetDimensions[1]);
      ((vtkVolumeTextureMapper2D*)this->Mapper)->CroppingOff();
    }
  else if (this->Mapper->IsA("vtkVolumeTextureMapper3D"))
    {
      ((vtkVolumeTextureMapper3D*)this->Mapper)->SetSampleDistance(this->TargetSampleDistance);
      ((vtkVolumeTextureMapper3D*)this->Mapper)->CroppingOff();
    }
  else if (this->Mapper->IsA("vtkFixedPointVolumeRayCastMapper"))
    {
      ((vtkFixedPointVolumeRayCastMapper*)this->Mapper)->AutoAdjustSampleDistancesOn();
      ((vtkFixedPointVolumeRayCastMapper*)this->Mapper)->CroppingOff();
    }
  else if (this->Mapper->IsA("vtkVolumeRayCastMapper"))
    {
      vtkVolumeRayCastMapper* map=((vtkVolumeRayCastMapper*)this->Mapper);
      map->CroppingOff();
      map->SetSampleDistance(this->TargetSampleDistance);
      vtkVolumeRayCastFunction* funct=map->GetVolumeRayCastFunction();

      int ismip=0,isray=0;
      if (funct!=NULL)
	{
	  ismip=funct->IsA("vtkVolumeRayCastMIPFunction");
	  isray=funct->IsA("vtkVolumeRayCastCompositeFunction");
	}

      if (this->VolumeRenderingMode==1 && ismip==0)
	{
	  vtkVolumeRayCastMIPFunction* mipFunction=vtkVolumeRayCastMIPFunction::New();
	  mipFunction->SetMaximizeMethodToScalarValue();
	  map->SetVolumeRayCastFunction(mipFunction);
	  mipFunction->Delete();
	}
      else if (this->VolumeRenderingMode==0 && isray==0)
	{
	 vtkVolumeRayCastCompositeFunction *compositeFunction =
	   vtkVolumeRayCastCompositeFunction::New();
	 map->SetVolumeRayCastFunction(compositeFunction);
	 compositeFunction->Delete();
	}
    }
  else
    {
      vtkCropVolumeRayCastMapper *map = ((vtkCropVolumeRayCastMapper*)this->Mapper);
      //map->SetSampleDistance(this->TargetSampleDistance/4.0);
      map->CroppingOff();
      
      map->AutoAdjustSampleDistancesOn();
      map->SetImageSampleDistance(.5);
      map->SetSampleDistance(.25);
      
      //map->SelectIrregularShape(this->CropShape);
      map->SetProbeShape(this->CropShape);
      
      // Get probe location for irregular cropping
      double ptr_location[3];
      this->GetProbeLocation(ptr_location);
      
      
      map->SetFixedSphereCenter(ptr_location);
      map->SetFunctionalCropOn(this->cropMode);
      
      switch(this->CropShape)
	{
	case 0:
	  map->SetFixedSphereRadius(this->CropSphereRadius);
	  break;
	  
	case 1:
	  map->SetFixedCubeLength(this->CropCubeLengthX, this->CropCubeLengthY, this->CropCubeLengthZ);
	  break;
	case 2:
	  map->SetFixedEllipseRadius(this->CropEllipseRadiusX, this->CropEllipseRadiusY, this->CropEllipseRadiusZ);
	  //map->SetSphereSize(this->CropEllipseRadius);
	  break;
	  
	case 3:
	  map->SetIrregularCropCylinderDimensions(this->CropCylinderRadius, this->CropCylinderHeight, this->direction_vector);
	  break;
	  
	default:
	  //printf("pxvolume shape selection default\n");
	  map->SetFixedSphereRadius(this->CropSphereRadius);
	  break;
	}
    }


  int canwedorgba=0;
  //printf("volume mapper name is %s\n", this->Mapper->GetClassName());

  if ( this->Mapper->IsA("vtkVolumeTextureMapper3D") || this->Mapper->IsA("vtkFixedPointVolumeRayCastMapper") || this->Mapper->IsA("vtkCropVolumeRayCastMapper"))
    canwedorgba=1;


  //  fprintf(stderr,"In Update Mapper, candorgba=%d RGBAMOde=%d DualInputMode=%d\n",canwedorgba,this->RGBAMode,this->DualInputMode);

  int doingrgba=0;

  if (this->DualInputMode==1 && canwedorgba==1)
    {
      this->ImageBlend->Update();
      int d[3]; this->ImageBlend->GetOutput()->GetDimensions(d);
      int nc=this->ImageBlend->GetOutput()->GetNumberOfScalarComponents();
      /*      fprintf(stderr,"Dual Input Mode is ON (common) %s (%d x %d x %d , %d)\n",this->Mapper->GetClassName(),
	      d[0],d[1],d[2],nc);*/
      this->Mapper->SetInput(this->ImageBlend->GetOutput());
      doingrgba=1;

    }
  else if (this->RGBAMode==1 && canwedorgba==1)
    {
      this->ImageMapToColors->SetLookupTable(this->LookupTable);
      this->ImageMapToColors->SetInput(ImageReslice->GetOutput());
      this->ImageMapToColors->Update();
      int d[3]; this->ImageMapToColors->GetOutput()->GetDimensions(d);
      int nc=this->ImageMapToColors->GetOutput()->GetNumberOfScalarComponents();
      /*      fprintf(stderr,"RGBA Mode is ON (common) %s (%d x %d x %d , %d)\n",this->Mapper->GetClassName(),
	      d[0],d[1],d[2],nc);*/
      this->Mapper->SetInput(this->ImageMapToColors->GetOutput());
      doingrgba=1;
    }
  else
    {
      //      fprintf(stderr,"RGBA Mode is Off %s\n",this->Mapper->GetClassName());
      this->Mapper->SetInput(ImageReslice->GetOutput());
      ImageReslice->Update();
      int d[3]; this->ImageReslice->GetOutput()->GetDimensions(d);
      int nc=this->ImageReslice->GetOutput()->GetNumberOfScalarComponents();
      //fprintf(stderr,"RGBA Mode is OFF %s (%d x %d x %d , %d)\n",this->Mapper->GetClassName(), d[0],d[1],d[2],nc);
    }

  if (ShadingMode)
    this->Property->ShadeOn();
  else
    this->Property->ShadeOff();

  this->Property->SetInterpolationTypeToLinear();

  if(gradientEnhancementMode==1)
    {
      //printf("in gradient opacity\n");
      this->Property->SetGradientOpacity(0, this->gTFun);
    }
  else
    {
      this->Property->SetGradientOpacity(0, this->gTFun2);
      //this->Property->SetDisableGradientOpacity(0, 1);
    }



  this->Property->SetColor(this->cTFun);
  this->Property->SetScalarOpacity(0,this->oTFun);

  if (doingrgba==1)
    {
      this->Property->IndependentComponentsOff();
    }

  this->Mapper->Update();

  return 1;
}
//------------------------------------------------------------------------------
void vtkpxVolume::Update()
{
  if (ImageReslice==NULL)
      return;

  // Texture Mode
  // 1 = Texture Mapping
  // 0  = Ray Casting Normal
  // -1 = Ray Casting MIP




  int proceed=this->Update_IsModeTheSame();
  //fprintf(stderr,"\nIn Update, DualInputMode=%d proceed=%d RGBAMode_changed %d\n",this->DualInputMode,proceed, this->RGBAMode_changed);


	 if(this->RGBAMode_changed == 1)
	 {
		if (proceed==1)
		{
		  this->Update_UpdateMapper();
		  return;
		}
		this->RGBAMode_changed = 0;
	 }

  // Alark: Note If Changing number of modes add here
  if (this->VolumeRenderingMode<0)
    this->VolumeRenderingMode=0;
  else if (this->VolumeRenderingMode>5)
    this->VolumeRenderingMode=5;

  // Create the Mapper Here

  if (this->Mapper)
    this->Mapper->Delete();

    //fprintf(stderr,"Creating a new Mapper mode = %d\n",this->VolumeRenderingMode);

  switch (this->VolumeRenderingMode)
    {
    case 1: // MIP
      this->Mapper=vtkVolumeRayCastMapper::New();
      break;
    case 2: // Text2D
      this->Mapper=vtkVolumeTextureMapper2D::New();
      break;
    case 3: // Text3D
      this->Mapper=vtkVolumeTextureMapper3D::New();
      break;
    case 4: // Cropped
      //this->Mapper=vtkCroppedVolumeRayCastMapper::New();
      this->Mapper = vtkCropVolumeRayCastMapper::New();
      break;
    default: // RayCast or Default
      this->Mapper=vtkFixedPointVolumeRayCastMapper::New();
      break;
    }


  if (this->Mapper!=NULL)
    {
      this->Update_UpdateMapper();
    }
  else
    {
      vtkErrorMacro(<<"Bad Update \n");
    }
}
// ---------------------------------------------------------------------------

