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

 see vtkpxElectrodeSource.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */
#include "vtkAppendPolyData.h"
#include "vtkDiskSource.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkOutlineFilter.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkpxCrossHairSource.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkUnsignedCharArray.h"
#include "vtkpxElectrodeSource.h"
#include "vtkGridTransform.h"
#include "vtkTransformToGrid.h"
#include "vtkFloatArray.h"
#include "pxutil.h"
//------------------------------------------------------------------------------
vtkpxElectrodeSource* vtkpxElectrodeSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxElectrodeSource");
  if(ret)
    {
      return (vtkpxElectrodeSource*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxElectrodeSource;
}

// Construct object with no children.
vtkpxElectrodeSource::vtkpxElectrodeSource()
{
  this->ColorName=new char[40];
  this->Values=vtkFloatArray::New();
  this->Reset();
  this->CurrentValueIndex=-1;
}
/* -------------------------------------------------------------------------*/
vtkpxElectrodeSource::~vtkpxElectrodeSource()
{
  delete [] this->ColorName;
  this->Values->Delete();
}
/* -------------------------------------------------------------------------*/
void vtkpxElectrodeSource::Reset()
{
  this->MotorFunction=0;
  this->SensoryFunction=0;
  this->VisualFunction=0;
  this->LanguageFunction=0;
  this->AuditoryFunction=0;
  this->User1Function=0;
  this->User2Function=0;

  this->SeizureOnset=0;
  this->SpikesPresent=0;
  
  this->Position[0]=0;this->Position[1]=0;this->Position[2]=0;
  this->SetPreviousPosition(this->Position);
  this->Normal[0]=1.0;this->Normal[1]=0.0;  this->Normal[2]=0.0;
						
  this->ElectrodeType=0;
  this->ElectrodePresent=1;

  this->Radius=5.0;
  this->Thickness=0.05;
  this->Color[0]=1.0; 
  this->Color[1]=1.0; 
  this->Color[2]=1.0;
  this->StoreColorInColorName();
  this->Values->SetNumberOfTuples(1);
  this->Values->SetComponent(0,0,0.0);
  this->ColorMode=1;
}

/* -------------------------------------------------------------------------*/
int  vtkpxElectrodeSource::GetNumberOfValues()
{
  return this->Values->GetNumberOfTuples();
}
    
void vtkpxElectrodeSource::SetNumberOfValues(int vl)
{
  vl=Irange(vl,1,1000);
  int nt=this->GetNumberOfValues();
  if (nt!=vl)
    {
      this->Values->SetNumberOfComponents(1);
      this->Values->SetNumberOfTuples(vl);
      this->Values->FillComponent(0,0.0);
    }
} 
/* -------------------------------------------------------------------------*/
void vtkpxElectrodeSource::Copy(vtkpxElectrodeSource* other)
{
  this->SetPosition(other->GetPosition());
  this->SetPreviousPosition(other->GetPreviousPosition());
  this->SetNormal(other->GetNormal());
  this->CopyAttributes(other);
}

void vtkpxElectrodeSource::CopyAttributes(vtkpxElectrodeSource* other)
{
  this->MotorFunction=other->GetMotorFunction();
  this->SensoryFunction=other->GetSensoryFunction();
  this->VisualFunction=other->GetVisualFunction();
  this->LanguageFunction=other->GetLanguageFunction();
  this->AuditoryFunction=other->GetAuditoryFunction();
  this->User1Function=other->GetUser1Function();
  this->User2Function=other->GetUser2Function();

  this->SeizureOnset=other->GetSeizureOnset();
  this->SpikesPresent=other->GetSpikesPresent();
  
						
  this->ElectrodeType=other->GetElectrodeType();
  this->ElectrodePresent=other->GetElectrodePresent();

  this->Radius=other->GetRadius();
  this->Thickness=other->GetThickness();
  this->StoreColorInColorName();

  this->Values->DeepCopy(other->GetValues());
  this->CurrentValueIndex=other->GetCurrentValueIndex();

  this->Modified();
}
/* -------------------------------------------------------------------------*/
int vtkpxElectrodeSource::ComputeColor()
{
  int score=0;

  for (int i=0;i<=2;i++)
    this->InternalColor[i]=this->Color[i];

  if (this->ElectrodePresent==0)
    {
      this->InternalColor[0]=0;      this->InternalColor[1]=0;      this->InternalColor[2]=0;
      this->Color[0]=0;      this->Color[1]=0;      this->Color[2]=0;
      return 0;
    }
  
  if (this->LanguageFunction==1)
    score=3;
  else if (this->MotorFunction==1)
    score=1;
  else if (this->VisualFunction==1)
    score=8;
  else if (this->SensoryFunction==1)
    score=5;
  else if (this->AuditoryFunction==1)
    score=6;
  else if (this->User1Function==1)
    score=2;
  else if (this->User2Function==1)
    score=4;


  

  switch (score)
    {
    case 1: // Red
      this->InternalColor[0]=1.0; this->InternalColor[1]=0.2; this->InternalColor[2]=0.1;
      break;
    case 2: // Orange
      this->InternalColor[0]=1.0; this->InternalColor[1]=0.62; this->InternalColor[2]=0.0;
      break;
    case 3: // Yellow
      this->InternalColor[0]=1.0; this->InternalColor[1]=1.0; this->InternalColor[2]=0.1;
      break;
    case 4: // Green
      this->InternalColor[0]=0.0; this->InternalColor[1]=1.0; this->InternalColor[2]=0.0;
      break;
    case 5: // Blue
      this->InternalColor[0]=0.13; this->InternalColor[1]=0.5; this->InternalColor[2]=0.78;
      break;
    case 6: // Indigo
      this->InternalColor[0]=0.6; this->InternalColor[1]=0.6; this->InternalColor[2]=0.6;
      break;
    case 7: // Cyan
      this->InternalColor[0]=0.5; this->InternalColor[1]=0.9; this->InternalColor[2]=0.75;
      break;
    case 8: // Brown
      this->InternalColor[0]=0.65; this->InternalColor[1]=0.56; this->InternalColor[2]=0.16;
      break;
    }


  if (this->ColorMode==0)
    return -score;

  for (int i=0;i<=2;i++)
    this->Color[i]=this->InternalColor[i];

  return score;
}
/* -------------------------------------------------------------------------*/
void vtkpxElectrodeSource::Execute()
{
  vtkPolyData* inner_shape=this->GetOutput();

  int showcube=0;

  if (this->CurrentValueIndex>=0 && this->CurrentValueIndex<this->Values->GetNumberOfTuples())
    {
      if (this->Values->GetComponent(this->CurrentValueIndex,0)>0.0)
	showcube=2;
    }
  else if (this->SpikesPresent==1 || this->SeizureOnset==1)
    showcube=1;

  if (showcube>0)
    {
      inner_shape=vtkPolyData::New();
    } 

  int colormode=this->ComputeColor();
  double scale=1.0;
  if (colormode!=0)
    scale=1.2;
  else
    scale=0.8;

  if (this->ElectrodeType==0)
    {
      vtkpxCrossHairSource* sphere=vtkpxCrossHairSource::New();
      sphere->SetPosition(this->Position);
      sphere->SetLength(this->Radius*scale);
      sphere->SetThickness(0.5);
      sphere->SetShowSphere(1);
      sphere->Update();
      inner_shape->ShallowCopy(sphere->GetOutput());
      sphere->Delete();
    }
  else
    {
      vtkDiskSource* disk=vtkDiskSource::New();
      disk->SetInnerRadius(this->Radius*0.1);
      disk->SetOuterRadius(this->Radius*scale);
      disk->SetRadialResolution(2);
      disk->SetCircumferentialResolution(8);

      vtkTransform* tr=vtkTransform::New();
      tr->PostMultiply();
      // need to add orientation stuff ... do this later 
      tr->Translate(this->Position);

      vtkTransformPolyDataFilter* trF=vtkTransformPolyDataFilter::New();
      trF->SetInput(disk->GetOutput());
      trF->SetTransform(tr);
      trF->Update();

      inner_shape->ShallowCopy(trF->GetOutput());
      disk->Delete();
      tr->Delete();
      trF->Delete();
    }

  // Color inner with functional info



  unsigned char d[3],d2[3];
  for (int ia=0;ia<=2;ia++)
    {
      d[ia]=(unsigned char)(this->Color[ia]*255.0);
      d2[ia]=(unsigned char)(this->InternalColor[ia]*255.0);
    }


  vtkUnsignedCharArray* color_array=vtkUnsignedCharArray::New();
  color_array->SetNumberOfComponents(3);
  color_array->SetNumberOfTuples(inner_shape->GetNumberOfPoints());

  for (int ka=0;ka<color_array->GetNumberOfTuples();ka+=2)
    {
      color_array->SetTuple3(ka,d[0],d[1],d[2]);
      color_array->SetTuple3(ka+1,d2[0],d2[1],d2[2]);
    }
  
  inner_shape->GetPointData()->SetScalars(color_array);
  color_array->Delete();
  

  if (showcube==0)
    return;

  

  // other wise put color_box around it
  vtkOutlineFilter* filt=vtkOutlineFilter::New();
  filt->SetInput(inner_shape);
  filt->Update();

  vtkPolyData* outer_shape= vtkPolyData::New();
  outer_shape->CopyStructure(filt->GetOutput());
  filt->Delete();

  d[0]=255;    d[1]=0;    d[2]=0;
  
  if (this->SeizureOnset==0)
    d[1]=255;
  
  if (this->CurrentValueIndex>=0 && this->CurrentValueIndex<this->Values->GetNumberOfTuples())
    {
      if (this->Values->GetComponent(this->CurrentValueIndex,0)>0.0)
	d[2]=255;
    }

  vtkUnsignedCharArray* color_array2=vtkUnsignedCharArray::New();
  color_array2->SetNumberOfComponents(3);
  color_array2->SetNumberOfTuples(outer_shape->GetNumberOfPoints());


  //  fprintf(stderr,"showcube=%d SeizureOnset=%d SpikesPresent=%d\n",showcube,this->SeizureOnset,this->SpikesPresent);

  for (int k=0;k<color_array2->GetNumberOfTuples();k++)
    {
      color_array2->SetTuple3(k,d[0],d[1],d[2]);
      if ( showcube==1 && this->SeizureOnset==1 && this->SpikesPresent==1)
	{
	  d[1]=255-d[1];
	}
    }
	
  
  outer_shape->GetPointData()->SetScalars(color_array2);
  color_array2->Delete();

  vtkAppendPolyData* append=vtkAppendPolyData::New();
  append->AddInput(inner_shape);
  append->AddInput(outer_shape);

  append->Update();

  this->GetOutput()->ShallowCopy(append->GetOutput());
  append->Delete();
  inner_shape->Delete();
  outer_shape->Delete();

}
//----------------------------------------------------------------------------
int vtkpxElectrodeSource::Load(const char* fname)
{  
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  int ok=Load(fin);
  gzclose(fin);

  return ok;
}

int vtkpxElectrodeSource::Load(gzFile fin)
{
  char line[255];
  gzgets(fin,line,100);
  int mode=0;
  if (gzfindstringinstring(line,"#vtkpxElectrodeSource File")==0)
    {
        if (gzfindstringinstring(line,"#vtkpxElectrodeSource2 File")==0)
	  return 0;
	else
	  mode=1;
    }
  else
    {
      mode=0;
    }
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%lf %lf %lf",&this->Position[0],&this->Position[1],&this->Position[2]);
  this->SetPreviousPosition(this->Position);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%lf %lf %lf",&this->Normal[0],&this->Normal[1],&this->Normal[2]);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&this->MotorFunction);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&this->VisualFunction);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&this->SensoryFunction);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&this->LanguageFunction);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&this->AuditoryFunction);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&this->User1Function);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&this->User2Function);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&this->SeizureOnset);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&this->SpikesPresent);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&this->ElectrodePresent);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&this->ElectrodeType);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%f",&this->Radius);
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%f",&this->Thickness);
  if (mode==1)
    {
      int tmp;
      gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&tmp);
      this->Values->SetNumberOfComponents(1);
      this->Values->SetNumberOfTuples(tmp);
      float dtmp;
      for (int j=0;j<this->Values->GetNumberOfTuples();j++)
	{
	  gzgets(fin,line,100); sscanf(line,"%f",&dtmp);
	  this->Values->SetComponent(j,0,dtmp);
	}
    }

  //
  this->Color[0]=1.0;   this->Color[1]=1.0;   this->Color[2]=1.0;
  this->CurrentValueIndex=-1;
  return 1;
}
//----------------------------------------------------------------------------
int vtkpxElectrodeSource::Save(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
    return 0;
  
  int ok=Save(fout);
  fclose(fout);
  return ok;
}

int vtkpxElectrodeSource::Save(FILE* fout)
{
  fprintf(fout,"#vtkpxElectrodeSource2 File\n");
  fprintf(fout,"#Position\n %.4f %.4f %.4f\n",this->Position[0],this->Position[1],this->Position[2]);
  fprintf(fout,"#Normal\n %.4f %.4f %.4f\n",this->Normal[0],this->Normal[1],this->Normal[2]);
  fprintf(fout,"#Motor Function\n%d\n",this->MotorFunction);
  fprintf(fout,"#Sensory Function\n%d\n",this->SensoryFunction);
  fprintf(fout,"#Visual Function\n%d\n",this->VisualFunction);
  fprintf(fout,"#Language Function\n%d\n",this->LanguageFunction);
  fprintf(fout,"#Auditory Function\n%d\n",this->AuditoryFunction);
  fprintf(fout,"#User1 Function\n%d\n",this->User1Function);
  fprintf(fout,"#User2 Function\n%d\n",this->User2Function);
  fprintf(fout,"#Seizure Onset\n%d\n",this->SeizureOnset);
  fprintf(fout,"#Spikes Present\n%d\n",this->SpikesPresent);
  fprintf(fout,"#Electrode Present\n%d\n",this->ElectrodePresent);
  fprintf(fout,"#Electrode Type\n%d\n",this->ElectrodeType);
  fprintf(fout,"#Radius \n%f\n",this->Radius);
  fprintf(fout,"#Thickeness\n%f\n",this->Thickness);
  fprintf(fout,"#Values\n%d\n",this->Values->GetNumberOfTuples());
  for (int j=0;j<this->Values->GetNumberOfTuples();j++)
    fprintf(fout,"%f\n",this->Values->GetComponent(j,0));
  return 1;
}

int vtkpxElectrodeSource::ApplyTransformation(vtkAbstractTransform* tr,int forward)
{
  if (tr==NULL)
    return 0;
  
  double p[3],pq[3];
  if (forward)
    {
      tr->TransformPoint(this->Position,p);
    }
  else
    {
      vtkTransformToGrid* tgrid=vtkTransformToGrid::New();
      tgrid->SetGridExtent(0,2,0,2,0,2);
      tgrid->SetGridOrigin(this->Position[0]-1.0,this->Position[1]-1.0,this->Position[2]-1.0);
      tgrid->SetGridSpacing(1,1,1);
      tgrid->SetInput(tr);
      tgrid->Update();

      vtkGridTransform* trg=vtkGridTransform::New();
      trg->SetDisplacementGrid(tgrid->GetOutput());
      trg->SetInterpolationModeToLinear();
      trg->Inverse();
      
      trg->TransformPoint(this->Position,p);
      tr->TransformPoint(p,pq);

      trg->Delete();
      tgrid->Delete();
    }

  /*  fprintf(stderr,"forward=%d Pos=(%.1f %.1f %.1f) --> (%.1f %.1f %.1f) --> (%.1f %.1f %.1f) \n",
	  forward,
	  this->Position[0],
	  this->Position[1],
	  this->Position[2],
	  p[0],p[1],p[2],
	  pq[0],pq[1],pq[2]);*/
  this->SetPosition(p);
  this->Modified();
  return 1;
}
//----------------------------------------------------------------------------
void vtkpxElectrodeSource::StoreColorInColorName()
{
  this->ComputeColor();
  
  int c[3];
  c[0]=int(Frange(this->InternalColor[0],0,1.0)*255.0);
  c[1]=int(Frange(this->InternalColor[1],0,1.0)*255.0);
  c[2]=int(Frange(this->InternalColor[2],0,1.0)*255.0);

  strcpy(this->ColorName,"#");
  for (int k=0;k<=2;k++)
    {
      for (int byte=0;byte<=1;byte++)
	{
	  int val=0;
	  switch(byte)
	    {
	    case 0:
	      val=c[k]/16;
	      break;
	    case 1:
	      val=c[k]-16*int(c[k]/16);
	      break;
	    }
	  
	  if (val<10)
	    {
	      sprintf(this->ColorName,"%s%1d",this->ColorName,val);
	    }
	  else
	    {
	      switch (val)
		{
		case 10:
		  strcat(this->ColorName,"A");
		  break;
		case 11:
		  strcat(this->ColorName,"B");
		  break;
		case 12:
		  strcat(this->ColorName,"C");
		  break;
		case 13:
		  strcat(this->ColorName,"D");
		  break;
		case 14:
		  strcat(this->ColorName,"E");
		  break;
		case 15:
		  strcat(this->ColorName,"F");
		  break;
		}
	    }
	}
    }

}
//----------------------------------------------------------------------------
int vtkpxElectrodeSource::GetFunctionalInfo(int Mode)
{
  // Description:
  // Get Functional Info
  // Mode Settings
  /*
    Location Alone = 0;
    Epilepsy Map   = 1  (4 gray levels, 0 no electrode, 1 electrode, 2=spikes present 3=seizure onset)
    Any Functional = 2;
    Motor Function = 3
    Sensory Function = 4
    Visual Function = 5
    Language Function = 6;
    Auditory Function = 7;
    User 1 Function = 8;
    User 2 Function = 9;

    Values = 20 ---> 20 + numberoftuples -1
  */

  if (Mode>=20)
    {
      int nt=Irange(Mode-20,0,this->GetNumberOfValues()-1);
      return (int)this->Values->GetComponent(nt,0);
    }

  switch (Mode)
    {
    case 1:
      if (this->SeizureOnset)
	return 2;
      if (this->SpikesPresent)
	return 1;
      else
	return 0;
      break;
      
    case 2:
      if (this->MotorFunction ||   this->SensoryFunction || this->VisualFunction ||
	  this->LanguageFunction || this->AuditoryFunction || this->User1Function || this->User2Function )
	return 1;
      else
	return 0;
      break;

    case 3:
      return this->MotorFunction;
      break;

    case 4:
      return this->SensoryFunction;
      break;
    case 5:
      return this->VisualFunction;
      break;
    case 6:
      return this->LanguageFunction;
      break;
    case 7:
      return this->AuditoryFunction;
      break;
    case 8:
      return this->User1Function;
      break;
    case 9:
      return this->User2Function;
      break;
    case 0:
      return 1;
      break;
    }      
     
  return 0;

}


