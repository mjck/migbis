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

 see vtkpxElectrodeGridSource.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkAppendPolyData.h"
#include "vtkCellArray.h"
#include "vtkUnsignedShortArray.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkpxElectrodeGridSource.h"
#include "vtkpxElectrodeSource.h"
#include "vtkCollection.h"
#include "vtkUnsignedCharArray.h"
#include "vtkShortArray.h"
#include "vtkMath.h"
#include "vtkFloatArray.h"
#include "vtkThinPlateSplineTransform.h"
#include "pxutil.h"
//------------------------------------------------------------------------------
vtkpxElectrodeGridSource* vtkpxElectrodeGridSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxElectrodeGridSource");
  if(ret)
    {
      return (vtkpxElectrodeGridSource*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxElectrodeGridSource;
}

// Construct object with no children.
vtkpxElectrodeGridSource::vtkpxElectrodeGridSource()
{
  this->Dimensions[0]=0; this->Dimensions[1]=0;
  this->ElectrodeSpacing[0]=10.0;   this->ElectrodeSpacing[1]=10.0;
  this->ElectrodeType=0;
  this->Radius=2.0;
  this->Thickness=0.05;
  this->GridName=NULL; this->SetGridName("Grid");
  this->ElectrodeCollection=vtkCollection::New();
  this->Visible=1;
  this->EnableSynchronization=1;
  this->JustShowElectrodes=0;
  this->Color[0]=0.8;
  this->Color[1]=0.8;
  this->Color[2]=0.8;
  this->LabelSurface=vtkPolyData::New();
  this->ShowLabelsGap=1;
  this->ColorMode=0;
}

/* -------------------------------------------------------------------------*/
 
vtkpxElectrodeGridSource::~vtkpxElectrodeGridSource()
{
  this->ElectrodeCollection->Delete();
  if (this->GridName!=NULL)
    delete [] GridName;
  this->LabelSurface->Delete();
}

/* -------------------------------------------------------------------------*/
void vtkpxElectrodeGridSource::CopyAttributes(vtkpxElectrodeGridSource* other)
{
 int d[2]; other->GetDimensions(d);
 if (d[0]==this->Dimensions[0] && d[1]==this->Dimensions[1])
   {

     this->SetElectrodeSpacing(other->GetElectrodeSpacing());
     this->ElectrodeType=other->GetElectrodeType();
     this->Radius=other->GetRadius();
     this->Thickness=other->GetThickness();
     this->ElectrodeCollection->InitTraversal();
     this->SetGridName(other->GetGridName());
     this->SetColor(other->GetColor());

     int total=this->Dimensions[0]*this->Dimensions[1];
     for (int i=0;i<total;i++)
       this->GetElectrode(i)->CopyAttributes(other->GetElectrode(i));
   }

}

void vtkpxElectrodeGridSource::Copy(vtkpxElectrodeGridSource* other)
{
  int d[2]; other->GetDimensions(d);

  this->InitializeGrid(d[0],d[1]);
  this->SetElectrodeSpacing(other->GetElectrodeSpacing());
  this->ElectrodeType=other->GetElectrodeType();
  this->Radius=other->GetRadius();
  this->Thickness=other->GetThickness();
  this->ElectrodeCollection->InitTraversal();
  this->SetGridName(other->GetGridName());
  this->SetColor(other->GetColor());


  int total=this->Dimensions[0]*this->Dimensions[1];
  for (int i=0;i<total;i++)
    this->GetElectrode(i)->Copy(other->GetElectrode(i));

  this->SynchronizeElectrodeTypeAndDimensions();
  this->Modified();

}
/* -------------------------------------------------------------------------*/
int vtkpxElectrodeGridSource::GetElectrodeRealIndex(int index)
{
  int j=index/this->Dimensions[0];
  int i=(index-j*this->Dimensions[0]);
  //  fprintf(stderr,"index=%d --> (%d,%d) ",index,i,j);
  return this->GetElectrodeRealIndex(i,j);

}

int vtkpxElectrodeGridSource::GetElectrodeRealIndex(int i,int j)
{
  i=Irange(i,0,this->Dimensions[0]-1);
  j=Irange(j,0,this->Dimensions[1]-1);
  

  int index=i*this->Dimensions[1]+(this->Dimensions[1]-j);
  //  fprintf(stderr,"  (%d,%d) --> %d\n",i,j,index);
  return index;
  

}

vtkpxElectrodeSource* vtkpxElectrodeGridSource::GetElectrode(int i,int j)
{
  i=Irange(i,0,this->Dimensions[0]-1);
  j=Irange(j,0,this->Dimensions[1]-1);

  int index=j*this->Dimensions[0]+i;
  vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetItemAsObject(index));//
  //return this->GetElectrode(index);
  return electrode;
}

vtkpxElectrodeSource* vtkpxElectrodeGridSource::GetElectrode(int index) 
{
  int size=this->Dimensions[0]*this->Dimensions[1];
  index=Irange(index,0,size-1);
  vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetItemAsObject(index));
  return electrode;
}
/* -------------------------------------------------------------------------*/
void vtkpxElectrodeGridSource::InitializeGrid(int dimx,int dimy)
{
  this->Dimensions[0]=Irange(dimx,1,22);
  this->Dimensions[1]=Irange(dimy,1,22);
  if (this->ElectrodeCollection->GetNumberOfItems()>0)
    this->ElectrodeCollection->RemoveAllItems();
  for (int j=0;j<Dimensions[1];j++)
    for (int i=0;i<Dimensions[0];i++)
      {
	vtkpxElectrodeSource* electrode=vtkpxElectrodeSource::New();
	double d[3];
	d[0]=i*this->ElectrodeSpacing[0];
	d[1]=j*this->ElectrodeSpacing[1];
	d[2]=0.0;
	electrode->SetPosition(d);
	this->ElectrodeCollection->AddItem(electrode);
	electrode->Delete();
      }

  this->Modified();
}
/* -------------------------------------------------------------------------*/
void vtkpxElectrodeGridSource::GridFlipX()
{
  vtkpxElectrodeSource* tmp_electrode=vtkpxElectrodeSource::New();
  for (int j=0;j<Dimensions[1];j++)
    for (int i=0;i<Dimensions[0]/2;i++)
      {
	int ia=(this->Dimensions[0]-i-1);
	vtkpxElectrodeSource* first=this->GetElectrode(i,j);
	vtkpxElectrodeSource* second=this->GetElectrode(ia,j);
	tmp_electrode->Copy(second);
	second->Copy(first);
	first->Copy(tmp_electrode);
      }
  
  tmp_electrode->Delete();
  this->Modified();
}

void vtkpxElectrodeGridSource::GridFlipY()
{
  vtkpxElectrodeSource* tmp_electrode=vtkpxElectrodeSource::New();

  for (int i=0;i<Dimensions[0];i++)
    for (int j=0;j<Dimensions[1]/2;j++)
      {
	int ja=(this->Dimensions[1]-j-1);
	vtkpxElectrodeSource* first=this->GetElectrode(i,j);
	vtkpxElectrodeSource* second=this->GetElectrode(i,ja);
	tmp_electrode->Copy(second);
	second->Copy(first);
	first->Copy(tmp_electrode);
      }
  
  this->Modified();
  tmp_electrode->Delete();
}

void vtkpxElectrodeGridSource::TransposeGrid()
{
  if (this->Dimensions[0]!=1 && this->Dimensions[1]!=1)
    {
      int total=Dimensions[0]*Dimensions[1];
      vtkCollection* tmpCollection=vtkCollection::New();

      this->ElectrodeCollection->InitTraversal();

      for (int i=0;i<Dimensions[0];i++)
	for (int j=0;j<Dimensions[1];j++)
	{
	  vtkpxElectrodeSource* orig=this->GetElectrode(i,j);
	  vtkpxElectrodeSource* tmp_electrode=vtkpxElectrodeSource::New();
	  tmp_electrode->Copy(orig);
	  tmpCollection->AddItem(tmp_electrode);
	}

      
      this->InitializeGrid(this->Dimensions[1],this->Dimensions[0]);
      this->ElectrodeCollection->InitTraversal();
      tmpCollection->InitTraversal();

      for (int i=0;i<total;i++)
	{
	  vtkpxElectrodeSource* target=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
	  vtkpxElectrodeSource* source=(vtkpxElectrodeSource*)(tmpCollection->GetNextItemAsObject());
	  target->Copy(source);
	}

      tmpCollection->RemoveAllItems();
      tmpCollection->Delete();
    }
  else
    {
      int tmp=this->Dimensions[1];
      this->Dimensions[1]=this->Dimensions[0];
      this->Dimensions[0]=tmp;
    }
  this->Modified();
}
/* -------------------------------------------------------------------------*/
int vtkpxElectrodeGridSource::Load(const char* fname)
{  
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  int ok=Load(fin);
  gzclose(fin);

  return ok;
}

int vtkpxElectrodeGridSource::Load(gzFile fin)
{
  char line[255];
  gzgets(fin,line,100);
  int mode=0;

  if (gzfindstringinstring(line,"#vtkpxElectrodeGridSource File")==0)
    return 0;

  if (gzfindstringinstring(line,"#vtkpxElectrodeGridSource File v2")==1)
    mode=1;


  gzgets(fin,line,250);   gzgets(fin,line,250); 

  int a=strlen(line);  line[a-1]=(char)0;
  this->SetGridName(line);

  int d[2];
  gzgets(fin,line,250); gzgets(fin,line,250); sscanf(line,"%d %d",&d[0],&d[1]);

  this->InitializeGrid(d[0],d[1]);
  gzgets(fin,line,250); gzgets(fin,line,250); sscanf(line,"%f %f",&this->ElectrodeSpacing[0],&this->ElectrodeSpacing[1]);
  gzgets(fin,line,250); gzgets(fin,line,250); sscanf(line,"%d",&this->ElectrodeType);
  gzgets(fin,line,250); gzgets(fin,line,250); sscanf(line,"%f",&this->Radius);
  gzgets(fin,line,250); gzgets(fin,line,250); sscanf(line,"%f",&this->Thickness);
  if (mode==1)
    {
      gzgets(fin,line,250); 
      gzgets(fin,line,250); 
      sscanf(line,"%f %f %f",&this->Color[0],&this->Color[1],&this->Color[2]);
    }
  else
    {
      this->Color[0]=0.8;
      this->Color[1]=0.8;
      this->Color[2]=0.8;
    }

  this->ElectrodeCollection->InitTraversal();
  for (int j=0;j<this->Dimensions[1];j++)
    for (int i=0;i<this->Dimensions[0];i++)
      {
	gzgets(fin,line,250); gzgets(fin,line,250);gzgets(fin,line,250);
	vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
	electrode->Load(fin);
      }

  this->SynchronizeElectrodeTypeAndDimensions();
  return 1;
}
//----------------------------------------------------------------------------
int vtkpxElectrodeGridSource::Save(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
    return 0;
  
  int ok=Save(fout);
  fclose(fout);
  return ok;
}

int vtkpxElectrodeGridSource::Save(FILE* fout)
{
  this->SynchronizeElectrodeTypeAndDimensions();
  fprintf(fout,"#vtkpxElectrodeGridSource File v2\n");
  fprintf(fout,"#Description\n%s\n",GridName);
  fprintf(fout,"#Dimensions\n %d %d\n",this->Dimensions[0],this->Dimensions[1]);
  fprintf(fout,"#Electrode Spacing\n %.4f %.4f\n",this->ElectrodeSpacing[0],this->ElectrodeSpacing[1]);
  fprintf(fout,"#Electrode Type\n%d\n",this->ElectrodeType);
  fprintf(fout,"#Radius \n%f\n",this->Radius);
  fprintf(fout,"#Thickeness\n%f\n",this->Thickness);
  fprintf(fout,"#Color\n%.3f %.3f %.3f\n",this->Color[0],this->Color[1],this->Color[2]);

  this->ElectrodeCollection->InitTraversal();
  for (int j=0;j<this->Dimensions[1];j++)
    for (int i=0;i<this->Dimensions[0];i++)
      {
	fprintf(fout,"#- - - - - - - - - - - - - - - - - - -\n# Electrode %d %d\n- - - - - - - - - - - - - - - - - - -\n",i,j);
	vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
	electrode->Save(fout);
      }

  return 1;
}

//----------------------------------------------------------------------------
int vtkpxElectrodeGridSource::ApplyTransformation(vtkAbstractTransform* tr,int forward)
{
  if (tr==NULL)
    return 0;
 
  this->ElectrodeCollection->InitTraversal();
  for (int j=0;j<this->Dimensions[1];j++)
    for (int i=0;i<this->Dimensions[0];i++)
      {
	vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
	if (electrode!=NULL)
	  electrode->ApplyTransformation(tr,forward);
      }
  this->Modified();
  return 1;
}
//----------------------------------------------------------------------------
void vtkpxElectrodeGridSource::SetNumberOfValues(int vl)
{
  this->ElectrodeCollection->InitTraversal();
  for (int j=0;j<this->Dimensions[1];j++)
    for (int i=0;i<this->Dimensions[0];i++)
      {
	vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
	if (electrode!=NULL)
	  electrode->SetNumberOfValues(vl);
      }
}
//----------------------------------------------------------------------------
int vtkpxElectrodeGridSource::GetNumberOfValues()
{
  this->ElectrodeCollection->InitTraversal();
  vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
  if (electrode!=NULL)
    return electrode->GetNumberOfValues();
  return 0;
}
//----------------------------------------------------------------------------
void vtkpxElectrodeGridSource::SynchronizeElectrodeTypeAndDimensions()
{
  this->ElectrodeCollection->InitTraversal();
  for (int j=0;j<this->Dimensions[1];j++)
    for (int i=0;i<this->Dimensions[0];i++)
      {
	vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
	if (electrode->GetElectrodeType()!=this->ElectrodeType)
	  electrode->SetElectrodeType(this->ElectrodeType);
	if (electrode->GetRadius()!=this->Radius)
	  electrode->SetRadius(this->Radius);
	if (electrode->GetThickness()!=this->Thickness)
	  electrode->SetThickness(this->Thickness);
      }

}
//----------------------------------------------------------------------------
int vtkpxElectrodeGridSource::CheckWhetherToShowLabelName(int i,int j)
{
  if (this->ShowLabelsGap==0 || this->Dimensions[0]*this->Dimensions[1]==1)
    return 1;

  j=(this->Dimensions[1]-j-1);

  int sp=this->ShowLabelsGap+1;

  if (this->Dimensions[1]<3)
    {
      if (i % sp == 0 )
	return 1;
      return 0;
    }

  else if (this->Dimensions[0]<3)
    {
      if (j % sp == 0 )
	return 1;
      return 0;
    }
  
  if (this->ShowLabelsGap==1)
    {
      if (  (j % 2) ==  (i % 2)) 
	   return 1;
      else
	return 0;
    }

  if ( ( j % this->ShowLabelsGap) == 0 && (i %this->ShowLabelsGap) ==0 )
    return 1;
  
  return 0;
}

void vtkpxElectrodeGridSource::Execute()
{
  vtkPolyData* output = this->GetOutput();

  if (this->EnableSynchronization)
    this->SynchronizeElectrodeTypeAndDimensions();

  // First add all preset electrodes 
  vtkAppendPolyData* append=vtkAppendPolyData::New();
  vtkPoints    *c_points    = vtkPoints::New();
  c_points->SetNumberOfPoints(this->Dimensions[0]*this->Dimensions[1]);


  vtkPoints      *c_labeledpoints  = vtkPoints::New();
  vtkShortArray* c_labelnumbers    = vtkShortArray::New();
  c_labelnumbers->SetNumberOfComponents(1);


  int j=0,index=0;
  //  fprintf(stderr,"Grid Color Mode=%d\n",this->ColorMode);
  this->ElectrodeCollection->InitTraversal();
  for (j=0;j<this->Dimensions[1];j++)
    {
      for (int i=0;i<this->Dimensions[0];i++)
	{
	  vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
	  if (electrode->GetElectrodePresent()==1)
	    {
	      if (this->ColorMode==0)
		{
		  electrode->SetColor(this->Color);
		  electrode->SetColorModeToManual();
		}
	      else if (this->ColorMode==1)
		electrode->SetColorModeToNormal();
	      else
		electrode->SetColorModeToManual();

	      electrode->Update();
	      append->AddInput(electrode->GetOutput());
	    }
	  c_points->SetPoint(index,electrode->GetPosition());
	  
	  if (electrode->GetElectrodePresent()==1)
	    {
	      if (this->CheckWhetherToShowLabelName(i,j)==1)
		{
		  c_labeledpoints->InsertNextPoint(electrode->GetPosition());
		  c_labelnumbers->InsertNextTuple1(float(this->GetElectrodeRealIndex(i,j)));
		}
	    }
	  
	  ++index;
	}
    }

  this->LabelSurface->SetPoints(c_labeledpoints);
  this->LabelSurface->GetPointData()->SetScalars(c_labelnumbers);
  c_labelnumbers->Delete();
  c_labeledpoints->Delete();


  vtkCellArray *c_lines     = vtkCellArray::New();
  c_lines->Allocate(this->Dimensions[0]+this->Dimensions[1],10);
  
  
  vtkIdType pt[2];
  for (j=0;j<this->Dimensions[1];j++)
    for (int i=0;i<this->Dimensions[0];i++)
      {
	vtkpxElectrodeSource* electrode=this->GetElectrode(i,j);
	pt[0]=j*this->Dimensions[0]+i;
	if (electrode->GetElectrodePresent())
	  {
	    if (i<this->Dimensions[0]-1)
	      {
		vtkpxElectrodeSource* electrodex=this->GetElectrode(i+1,j);
		if (electrodex->GetElectrodePresent())
		  {
		    pt[1]=pt[0]+1;
		    c_lines->InsertNextCell(2,pt);
		  }
	      }
	    if (j<this->Dimensions[1]-1)
	      {
		vtkpxElectrodeSource* electrodey=this->GetElectrode(i,j+1);
		if (electrodey->GetElectrodePresent())
		  {
		    pt[1]=pt[0]+this->Dimensions[0];
		    c_lines->InsertNextCell(2,pt);
		  }
	      }
	  }
      }

  
  unsigned char d[3];
  for (int ia=0;ia<=2;ia++)
    d[ia]=int(255*this->Color[ia]);

  if (this->JustShowElectrodes==0)
    {
      vtkUnsignedCharArray* color_array=vtkUnsignedCharArray::New();
      color_array->SetNumberOfComponents(3);
      color_array->SetNumberOfTuples(this->Dimensions[0]*this->Dimensions[1]);
      for (int k=0;k<color_array->GetNumberOfTuples();k++)
	color_array->SetTuple3(k,d[0],d[1],d[2]);
      
      
      vtkPolyData* lines=vtkPolyData::New();  
      lines->SetPoints(c_points);
      lines->SetLines(c_lines);
      
      lines->GetPointData()->SetScalars(color_array);
      color_array->Delete();
      c_lines->Delete();
      
      append->AddInput(lines);
      append->Update();
      lines->Delete();
    }
        
  append->Update();
  output->ShallowCopy(append->GetOutput());
  append->Delete();
  c_points->Delete();

}

//----------------------------------------------------------------------------
int vtkpxElectrodeGridSource::FindClosestElectrodeIndex(float x,float y,float z)
{
  float q[3]; q[0]=x; q[1]=y; q[2]=z;
  return FindClosestElectrodeIndex(x,y,z);
}
//----------------------------------------------------------------------------
int vtkpxElectrodeGridSource::FindClosestElectrodeIndex(double q[3])
{
  int closest=-1;
  float d=pow(float(this->Radius*5.0),float(2.0));
  double p[3];
  float maxd2=pow(float(this->Radius),float(2.0));
  int index=0;
  this->ElectrodeCollection->InitTraversal();
  for (int j=0;j<this->Dimensions[1];j++)
    for (int i=0;i<this->Dimensions[0];i++)
      {
	vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
	if (electrode->GetElectrodePresent()>0)
	  {
	    electrode->GetPosition(p);
	    float d2=vtkMath::Distance2BetweenPoints(p,q);
	    if (d2<maxd2 && d2<d )
	      {
		d=d2;
		closest=index;
	      }
	  }
      	++index;
      }
  return closest;
}
//----------------------------------------------------------------------------
int vtkpxElectrodeGridSource::HandleClickedPoint(float x,float y,float z,int state)
{
  static int last_electrode=-1;
  int dm=this->Dimensions[0]*this->Dimensions[1];
  if (dm<1)
    return 0;

  double q[3]; q[0]=x; q[1]=y; q[2]=z;

  int found=-1;

  if (state==0)
    {
      found=this->FindClosestElectrodeIndex(q);
      if (found>-1)
	{
	  vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetItemAsObject(found));
	  this->EnableSynchronization=0;
	  electrode->SetRadius(2.0*this->GetRadius());
	  last_electrode=found;
	}
    }
  else if (state==1 && last_electrode>=0)
    {
      vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetItemAsObject(last_electrode));
      electrode->SetPosition(q);
      found=last_electrode;
    }
  else if (state==2 && last_electrode>=0)
    {
      vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetItemAsObject(last_electrode));
      electrode->SetPosition(q);
      this->EnableSynchronization=1;
      found=last_electrode;
      last_electrode=-1;
    }
  
  if (found==-1)
    return -1;


  this->Modified();
  return found;
}

//----------------------------------------------------------------------------
void vtkpxElectrodeGridSource::GetNeighborDistances(int index,vtkFloatArray* array)
{
  index=Irange(index,0,this->Dimensions[0]*this->Dimensions[1]-1);
  vtkpxElectrodeSource* this_electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetItemAsObject(index));
  double p[3];
  this_electrode->GetPosition(p);
  this->GetNeighborDistances(index,array,p[0],p[1],p[2]);
}

void vtkpxElectrodeGridSource::GetNeighborDistances(int index,vtkFloatArray* array,float px,float py,float pz)
{
  if (array==NULL)
    return;

  if (array->GetNumberOfComponents()!=1)
    array->SetNumberOfComponents(1);

  if (array->GetNumberOfTuples()!=4)
    array->SetNumberOfTuples(4);

  index=Irange(index,0,this->Dimensions[0]*this->Dimensions[1]-1);
  
  double p[3]; p[0]=px; p[1]=py; p[2]=pz;
  double q[3];
 
  int index2=index;
  for (int i=0;i<=3;i++)
    {
      switch (i) 
	{
	case 0:
	  index2=index-1;
	  break;
	case 1:
	  index2=index+1;
	  break;
	case 2:
	  index2=index-Dimensions[0];
	  break;
	case 3:
	  index2=index+Dimensions[0];
	  break;
	}

      if (index2<0 || index2>=this->Dimensions[0]*this->Dimensions[1])
	{
	  array->SetComponent(i,0,-1);
	}
      else 
	{
	  vtkpxElectrodeSource* other_electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetItemAsObject(index2));
	  if (other_electrode->GetElectrodePresent())
	    {
	      other_electrode->GetPosition(q);
	      array->SetComponent(i,0,sqrt(vtkMath::Distance2BetweenPoints(p,q)));
	    }
	  else
	    {
	      array->SetComponent(i,0,-1);
	    }
	}
    }
}

// ---------------------------------------------------------------------------------------
int vtkpxElectrodeGridSource::IsElectrodeModified(int index)
{
  int j=index/this->Dimensions[0];
  int i=(index-j*this->Dimensions[0]);

  double d[3],x[3];
  d[0]=i*this->ElectrodeSpacing[0];
  d[1]=j*this->ElectrodeSpacing[1];
  d[2]=0.0;
  this->GetElectrode(index)->GetPosition(x);
  if (vtkMath::Distance2BetweenPoints(x,d)<0.01)
    return 0;
  return 1;
}

// ---------------------------------------------------------------------------------------
int vtkpxElectrodeGridSource::GetNumberOfModifiedElectrodes()
{
  int index=0;
  int sum=0;
  for (int j=0;j<this->Dimensions[1];j++)
    for (int i=0;i<this->Dimensions[0];i++)
      {
	sum+=IsElectrodeModified(index);
	++index;
      }
  return sum;
}

// ---------------------------------------------------------------------------------------
int vtkpxElectrodeGridSource::AutoWarp()
{
  int nummod=this->GetNumberOfModifiedElectrodes();
  if (nummod<3)
    return 0;

  vtkPoints* points1=vtkPoints::New();
  vtkPoints* points2=vtkPoints::New();
  points1->SetNumberOfPoints(nummod);
  points2->SetNumberOfPoints(nummod);

  int index=0;
  double d[3],x[3];
  this->ElectrodeCollection->InitTraversal();
  for (int j=0;j<this->Dimensions[1];j++)
    for (int i=0;i<this->Dimensions[0];i++)
      {

	d[0]=i*this->ElectrodeSpacing[0];
	d[1]=j*this->ElectrodeSpacing[1];
	d[2]=0.0;
	vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
	electrode->GetPosition(x);
	if (vtkMath::Distance2BetweenPoints(x,d)>0.01)
	  {
	    points1->SetPoint(index,d);
	    points2->SetPoint(index,x);
	    ++index;
	  }
      }

  vtkThinPlateSplineTransform* spl=vtkThinPlateSplineTransform::New();
  spl->SetSourceLandmarks(points1);
  spl->SetTargetLandmarks(points2);
  spl->SetSigma(2.0);
  spl->SetBasisToR();
  points1->Delete();
  points2->Delete();
  spl->Update();
  
  this->ElectrodeCollection->InitTraversal();

  for (int j=0;j<this->Dimensions[1];j++)
    for (int i=0;i<this->Dimensions[0];i++)
      {
	d[0]=i*this->ElectrodeSpacing[0];
	d[1]=j*this->ElectrodeSpacing[1];
	d[2]=0.0;
	spl->TransformPoint(d,x);
	vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
	electrode->SetPreviousPosition(electrode->GetPosition());
	electrode->SetPosition(x);
	electrode->Modified();
      }
  this->Modified();
  spl->Delete();
  return 1;
}

//----------------------------------------------------------------------------
void vtkpxElectrodeGridSource::SetCurrentValueIndex(int a)
{
  this->ElectrodeCollection->InitTraversal();
  for (int j=0;j<this->Dimensions[1];j++)
    for (int i=0;i<this->Dimensions[0];i++)
      {
	vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
	electrode->SetCurrentValueIndex(a);
	electrode->Modified();
      }
  this->Modified();
}

int  vtkpxElectrodeGridSource::GetCurrentValueIndex()
{
 this->ElectrodeCollection->InitTraversal();
 vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(this->ElectrodeCollection->GetNextItemAsObject());
  if (electrode==NULL)
    return -1;
  
  return electrode->GetCurrentValueIndex();
}

