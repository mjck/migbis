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

 see vtkpxElectrodeMultiGridSource.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkAppendPolyData.h"
#include "vtkCellArray.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkpxElectrodeMultiGridSource.h"
#include "vtkpxElectrodeGridSource.h"
#include "vtkCollection.h"
#include "vtkUnsignedCharArray.h"
#include "vtkFloatArray.h"
#include "vtkSphereSource.h"
#include "vtkImageData.h"
#include "vtkMath.h"
#include "vtkShortArray.h"
#include "vtkLookupTable.h"
#include "sstream"
//------------------------------------------------------------------------------
vtkpxElectrodeMultiGridSource* vtkpxElectrodeMultiGridSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxElectrodeMultiGridSource");
  if(ret)
    {
      return (vtkpxElectrodeMultiGridSource*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxElectrodeMultiGridSource;
}

// Construct object with no children.
vtkpxElectrodeMultiGridSource::vtkpxElectrodeMultiGridSource()
{
  this->Description=NULL; this->SetDescription("patient");
  this->Comment=NULL; this->SetComment("no additional comment");
  this->ElectrodeGridCollection=vtkCollection::New();
  this->LabelSurface=vtkPolyData::New();
}

/* -------------------------------------------------------------------------*/
 
vtkpxElectrodeMultiGridSource::~vtkpxElectrodeMultiGridSource()
{
  this->ElectrodeGridCollection->Delete();
  if (this->Description!=NULL)
    delete [] Description;
  if (this->Comment!=NULL)
    delete [] Comment;
  this->LabelSurface->Delete();
}

/* -------------------------------------------------------------------------*/
void vtkpxElectrodeMultiGridSource::CopyAttributes(vtkpxElectrodeMultiGridSource* other)
{
  int d=this->ElectrodeGridCollection->GetNumberOfItems();
  int num=other->GetNumberOfGrids();

  if (d!=num)
    {
      vtkErrorMacro(<<"Cannot copy attributes .. different number of grids\n");
      return;
    }

  this->ElectrodeGridCollection->InitTraversal();

  for (int i=0;i<d;i++)
    {
      vtkpxElectrodeGridSource* electrodegrid=(vtkpxElectrodeGridSource*)(this->ElectrodeGridCollection->GetNextItemAsObject());
      electrodegrid->CopyAttributes(other->GetElectrodeGrid(i));
    }
  this->Modified();

}


void vtkpxElectrodeMultiGridSource::Copy(vtkpxElectrodeMultiGridSource* other)
{
  int num=other->GetNumberOfGrids();
  this->InitializeMultiGrid(num);

  this->SetComment(other->GetComment());
  this->SetDescription(other->GetDescription());

  int d=this->ElectrodeGridCollection->GetNumberOfItems();
  this->ElectrodeGridCollection->InitTraversal();

  for (int i=0;i<d;i++)
      {
	vtkpxElectrodeGridSource* electrodegrid=(vtkpxElectrodeGridSource*)(this->ElectrodeGridCollection->GetNextItemAsObject());
	electrodegrid->Copy(other->GetElectrodeGrid(i));
      }
  this->Modified();
}
/* -------------------------------------------------------------------------*/
int vtkpxElectrodeMultiGridSource::GetNumberOfGrids()
{
  return this->ElectrodeGridCollection->GetNumberOfItems();
}
/* -------------------------------------------------------------------------*/
vtkpxElectrodeGridSource* vtkpxElectrodeMultiGridSource::GetElectrodeGrid(int i)
{
  int d=this->ElectrodeGridCollection->GetNumberOfItems();
  i=Irange(i,0,d-1);
  vtkpxElectrodeGridSource* electrodegrid=(vtkpxElectrodeGridSource*)(this->ElectrodeGridCollection->GetItemAsObject(i));
  return electrodegrid;
}
/* -------------------------------------------------------------------------*/
void vtkpxElectrodeMultiGridSource::InitializeMultiGrid(int numgrids)
{
  numgrids=Irange(numgrids,0,40);

  if (this->ElectrodeGridCollection->GetNumberOfItems()>0)
    this->ElectrodeGridCollection->RemoveAllItems();
  for (int i=0;i<numgrids;i++)
      {
	vtkpxElectrodeGridSource* electrodegrid=vtkpxElectrodeGridSource::New();
	if (i==0)
	  electrodegrid->InitializeGrid(8,8);
	else
	  electrodegrid->InitializeGrid(1,8);
	this->ElectrodeGridCollection->AddItem(electrodegrid);
	electrodegrid->Delete();
      }

  this->Modified();
}
/* -------------------------------------------------------------------------*/
int vtkpxElectrodeMultiGridSource::Load(const char* fname)
{  
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  int ok=Load(fin);
  gzclose(fin);

  return ok;
}

int vtkpxElectrodeMultiGridSource::Load(gzFile fin)
{
  char line[255];
  gzgets(fin,line,100);
  if (gzfindstringinstring(line,"#vtkpxElectrodeMultiGridSource File")==0)
    return 0;

  int a=0;
  gzgets(fin,line,100);   gzgets(fin,line,100); a=strlen(line);  line[a-1]=(char)0;  this->SetDescription(line);
  gzgets(fin,line,100);   gzgets(fin,line,100); a=strlen(line);  line[a-1]=(char)0;  this->SetComment(line);


  int d;
  gzgets(fin,line,100); gzgets(fin,line,100); sscanf(line,"%d",&d);
  this->InitializeMultiGrid(d);
  d=this->ElectrodeGridCollection->GetNumberOfItems();

  this->ElectrodeGridCollection->InitTraversal();
  for (int i=0;i<d;i++)
      {
	gzgets(fin,line,100);
	gzgets(fin,line,100);
	gzgets(fin,line,100);
	vtkpxElectrodeGridSource* electrodegrid=(vtkpxElectrodeGridSource*)(this->ElectrodeGridCollection->GetNextItemAsObject());
	if (electrodegrid==NULL)
	  fprintf(stderr,"Bad Grid %d\n",i);
	electrodegrid->Load(fin);
      }
  
  return 1;
}
//----------------------------------------------------------------------------
int vtkpxElectrodeMultiGridSource::Save(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
    return 0;
  
  int ok=Save(fout);
  fclose(fout);
  return ok;
}

int vtkpxElectrodeMultiGridSource::Save(FILE* fout)
{
  fprintf(fout,"#vtkpxElectrodeMultiGridSource File\n");
  fprintf(fout,"#Description\n%s\n",Description);
  fprintf(fout,"#Comment\n%s\n",Comment);
  int num=this->GetNumberOfGrids();
  fprintf(fout,"#Number of Grids\n %d \n",num);
  this->ElectrodeGridCollection->InitTraversal();
  for (int i=0;i<num;i++)
    {
      fprintf(fout,"#- - - - - - - - - - - - - - - - - - -\n# Electrode Grid %d\n- - - - - - - - - - - - - - - - - - -\n",i);
      vtkpxElectrodeGridSource* electrodegrid=(vtkpxElectrodeGridSource*)(this->ElectrodeGridCollection->GetNextItemAsObject());
      electrodegrid->Save(fout);
    }
  
  return 1;
}
//----------------------------------------------------------------------------
void vtkpxElectrodeMultiGridSource::SetNumberOfValues(int vl)
{
  int num=this->GetNumberOfGrids();
  this->ElectrodeGridCollection->InitTraversal();
  for (int i=0;i<num;i++)
    {
      vtkpxElectrodeGridSource* electrodegrid=(vtkpxElectrodeGridSource*)(this->ElectrodeGridCollection->GetNextItemAsObject());
      if (electrodegrid!=NULL)
	electrodegrid->SetNumberOfValues(vl);
    }
  this->Modified();
}
//----------------------------------------------------------------------------
int vtkpxElectrodeMultiGridSource::GetNumberOfValues()
{
  int num=this->GetNumberOfGrids();
  this->ElectrodeGridCollection->InitTraversal();
  for (int i=0;i<num;i++)
    {
      vtkpxElectrodeGridSource* electrodegrid=(vtkpxElectrodeGridSource*)(this->ElectrodeGridCollection->GetNextItemAsObject());
      if (electrodegrid!=NULL)
	return electrodegrid->GetNumberOfValues();
    }
  return 0;
}
//----------------------------------------------------------------------------
void vtkpxElectrodeMultiGridSource::ShowLabelsGap(int all)
{
  int num=this->GetNumberOfGrids();
  this->ElectrodeGridCollection->InitTraversal();
  for (int i=0;i<num;i++)
    {
      vtkpxElectrodeGridSource* electrodegrid=(vtkpxElectrodeGridSource*)(this->ElectrodeGridCollection->GetNextItemAsObject());
      if (electrodegrid!=NULL)
	electrodegrid->SetShowLabelsGap(all);
    }
  this->Modified();
}

int vtkpxElectrodeMultiGridSource::ApplyTransformation(vtkAbstractTransform* tr,int forward)
{
  if (tr==NULL)
    return 0;
 
  int num=this->GetNumberOfGrids();
  this->ElectrodeGridCollection->InitTraversal();
  for (int i=0;i<num;i++)
    {
      vtkpxElectrodeGridSource* electrodegrid=(vtkpxElectrodeGridSource*)(this->ElectrodeGridCollection->GetNextItemAsObject());
      if (electrodegrid!=NULL)
	electrodegrid->ApplyTransformation(tr,forward);
    }
  this->Modified();
  return 1;
}



//----------------------------------------------------------------------------


void vtkpxElectrodeMultiGridSource::Execute()
{
  vtkPolyData* output = this->GetOutput();

  
  // First add all preset electrodes 
  vtkAppendPolyData* append=vtkAppendPolyData::New();

  int num=this->GetNumberOfGrids();
  this->ElectrodeGridCollection->InitTraversal();
  int num_grids_to_display=0;
  int ntotalp=0;

  for (int i=0;i<num;i++)
    {
      vtkpxElectrodeGridSource* electrodegrid=(vtkpxElectrodeGridSource*)(this->ElectrodeGridCollection->GetNextItemAsObject());
      if (electrodegrid->GetVisible()==1)
	{
	  electrodegrid->Update();
	  append->AddInput(electrodegrid->GetOutput());
	  ntotalp+=electrodegrid->GetLabelSurface()->GetNumberOfPoints();

	  ++num_grids_to_display;
	}
    }

  if (ntotalp>0)
    {
      vtkPoints* pt=vtkPoints::New();
      pt->SetNumberOfPoints(ntotalp);

      vtkShortArray* pl=vtkShortArray::New();
      pl->SetNumberOfComponents(1);
      pl->SetNumberOfTuples(ntotalp);
      
      int index=0;

      this->ElectrodeGridCollection->InitTraversal();
      for (int i=0;i<num;i++)
	{
	  vtkpxElectrodeGridSource* electrodegrid=(vtkpxElectrodeGridSource*)(this->ElectrodeGridCollection->GetNextItemAsObject());
	  if (electrodegrid->GetVisible()==1)
	    {
	      int n=electrodegrid->GetLabelSurface()->GetNumberOfPoints();
	      vtkPoints* t_p=electrodegrid->GetLabelSurface()->GetPoints();
	      vtkDataArray* t_d=electrodegrid->GetLabelSurface()->GetPointData()->GetScalars();
	      for (int j=0;j<n;j++)
		{
		  pt->SetPoint(index,t_p->GetPoint(j));
		  pl->SetComponent(index,0,t_d->GetComponent(j,0));
		  ++index;
		}
	    }
	}
      this->LabelSurface->SetPoints(pt);
      this->LabelSurface->GetPointData()->SetScalars(pl);
      pt->Delete();
      pl->Delete();
    }

  if (num_grids_to_display==0)
    {
      vtkSphereSource* sphere=vtkSphereSource::New();
      sphere->SetRadius(0.01);
      sphere->SetThetaResolution(3);
      sphere->SetPhiResolution(3);
      sphere->Update();
      output->ShallowCopy(sphere->GetOutput());
      this->LabelSurface->ShallowCopy(sphere->GetOutput());
      sphere->Delete();
    }
  else
    {
      append->Update();

      output->ShallowCopy(append->GetOutput());
    }
  append->Delete();

}

//----------------------------------------------------------------------------
void vtkpxElectrodeMultiGridSource::InsertNewGrid()
{
  vtkpxElectrodeGridSource* electrodegrid=vtkpxElectrodeGridSource::New();
  electrodegrid->InitializeGrid(1,8);
  char line[20];  sprintf(line,"grid_%d",this->GetNumberOfGrids()+1);
  electrodegrid->SetGridName(line);
  this->ElectrodeGridCollection->AddItem(electrodegrid);
  electrodegrid->Delete();
  this->Modified();
}
//----------------------------------------------------------------------------
void vtkpxElectrodeMultiGridSource::DeleteGrid(int index)
{
  if (this->GetNumberOfGrids()<2)
    return;

  if (index<0 || index>=this->GetNumberOfGrids())
    return;
  
  this->ElectrodeGridCollection->RemoveItem(index);

}
//----------------------------------------------------------------------------
vtkPolyData* vtkpxElectrodeMultiGridSource::ExportToPolyDataAsPoints(int Mode)
{
  return this->ExportToPolyDataAsPoints(Mode,-1.0);
}
//----------------------------------------------------------------------------
vtkPolyData* vtkpxElectrodeMultiGridSource::ExportToPolyDataAsPoints(int Mode,float radius)
{
  vtkPoints* points=vtkPoints::New();
  points->Allocate(100,20);

  vtkFloatArray* labels=vtkFloatArray::New();
  labels->SetNumberOfComponents(1);
  labels->Allocate(100,20);

  int num=this->GetNumberOfGrids();
  this->ElectrodeGridCollection->InitTraversal();
  for (int i=0;i<num;i++)
    {
      vtkpxElectrodeGridSource* electrodegrid=(vtkpxElectrodeGridSource*)(this->ElectrodeGridCollection->GetNextItemAsObject());
      if (electrodegrid->GetVisible()==1)
	{
	  int dim[2];
	  electrodegrid->GetDimensions(dim);
	  int tdim=dim[0]*dim[1];
	  for (int j=0;j<tdim;j++)
	    {
	      vtkpxElectrodeSource* electrode=electrodegrid->GetElectrode(j);
	      if (electrode->GetElectrodePresent()==1)
		{
		  double p[3]; electrode->GetPosition(p);
		  points->InsertNextPoint(p);
		  double m=(double)electrode->GetFunctionalInfo(Mode);
		  labels->InsertNextTuple1(m);
		}
	    }
	}
    }

  vtkPolyData* output=vtkPolyData::New();
  output->SetPoints(points);
  output->GetPointData()->SetScalars(labels);

  points->Delete();
  labels->Delete();
  
  return output;
}

//----------------------------------------------------------------------------
vtkImageData* vtkpxElectrodeMultiGridSource::CreateSinglePixelImage(vtkDataArray* ModeArray,vtkImageData* baseimage)
{
  if (baseimage==NULL || ModeArray==NULL)
    {
      vtkErrorMacro(<<"Bad BaseImage or Mode Array\n");
      return NULL;
    }

  int ncomp=ModeArray->GetNumberOfTuples();

  vtkImageData* output=vtkImageData::New();
  output->CopyStructure(baseimage);
  output->SetScalarTypeToUnsignedChar();
  output->SetNumberOfScalarComponents(ncomp);
  output->AllocateScalars();
  vtkDataArray* outarr=output->GetPointData()->GetScalars();


  double sp[3],ori[3];
  output->GetSpacing(sp);
  output->GetOrigin(ori);

  int dim[3]; output->GetDimensions(dim);

  for (int comp=0;comp<ncomp;comp++)
    {
      outarr->FillComponent(comp,0.0);
      int Mode=(int)ModeArray->GetComponent(comp,0);
      fprintf(stderr,"Component=%d Mode=%d\n",comp,Mode);
      vtkPolyData* poly=this->ExportToPolyDataAsPoints(Mode,1.0);
      vtkDataArray* inarr=poly->GetPointData()->GetScalars();

      vtkPoints* pts=poly->GetPoints();
      int np=pts->GetNumberOfPoints();
      for (int i=0;i<np;i++)
	{
	  double x[3]; pts->GetPoint(i,x);
	  int   px[3],goodpoint=1;
	  for (int ia=0;ia<=2;ia++)
	    {
	      px[ia]=int( (x[ia]-ori[ia])/sp[ia]+0.5);
	      if (px[ia]<0 || px[ia]>=dim[ia])
		goodpoint=0;
	    }
	  if (goodpoint == 1 && inarr->GetComponent(i,0)>0 )
	    output->SetScalarComponentFromDouble(px[0],px[1],px[2],comp,100);
	}
      poly->Delete();
    }
  return output;
}

//----------------------------------------------------------------------------
int vtkpxElectrodeMultiGridSource::PickFunction(vtkImageData* img)
{
  double origin[3],spacing[3];
  img->GetOrigin(origin);
  img->GetSpacing(spacing);
  int imgdim[3];
  img->GetDimensions(imgdim);

  int nc=img->GetNumberOfScalarComponents();

  fprintf(stderr,"Image Dimensions %d %d %d fr=%d\n",imgdim[0],imgdim[1],imgdim[2],nc);

  int d=this->ElectrodeGridCollection->GetNumberOfItems();
  for (int k=0;k<d;k++)
      {
	vtkpxElectrodeGridSource* electrodegrid=this->GetElectrodeGrid(k);
	int dim[2]; 	electrodegrid->GetDimensions(dim);
	//fprintf(stderr,"Reading grid %d (%d x %d ) name=%s\n",k,dim[0],dim[1],electrodegrid->GetGridName());
	for (int j=0;j<dim[1];j++)
	  for (int i=0;i<dim[0];i++)
	  {
	    vtkpxElectrodeSource* electrode=electrodegrid->GetElectrode(i,j);
	    if (electrode->GetElectrodePresent()==1)
	      {
		//int realindex=electrodegrid->GetElectrodeRealIndex(i,j);
		double p[3]; 
		electrode->GetPosition(p);
		int c[3];
		int allok=1;
		for (int ia=0;ia<=2;ia++)
		  {
		    c[ia]=int((p[ia]-origin[ia])/spacing[ia]);
		    if (c[ia]<0)
		      allok=0;
		    if (c[ia]>=imgdim[ia])
		      allok=0;
		  }
		
		//float val=0.0;


		if (allok)
		  {
		    vtkFloatArray* values=electrode->GetValues();
		    values->SetNumberOfComponents(1);
		    values->SetNumberOfTuples(nc);
		    for (int ia=0;ia<nc;ia++)
		      values->SetComponent(ia,0,img->GetScalarComponentAsDouble(c[0],c[1],c[2],ia));
		  }
	      }
	  }
	electrodegrid->Modified();
      }
  this->Modified();
  return 0;
}


int vtkpxElectrodeMultiGridSource::ExportValues(const char* fname,const char* description,const char* column,const char* columnnames)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
    return 0;



  fprintf(fout,"#%s\n",description);
  fprintf(fout,"#Electrode \t positionx \t positiony \t positionz \t %s \n",columnnames);
  int d=this->ElectrodeGridCollection->GetNumberOfItems();
  for (int k=0;k<d;k++)
    {
      vtkpxElectrodeGridSource* electrodegrid=this->GetElectrodeGrid(k);
      int dim[2]; electrodegrid->GetDimensions(dim);
      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {
	    vtkpxElectrodeSource* electrode=electrodegrid->GetElectrode(i,j);

	    if (electrode->GetElectrodePresent()==1)
	      {
		double pos[3]; electrode->GetPosition(pos);
		int realindex=electrodegrid->GetElectrodeRealIndex(i,j);
		std::stringstream s2;
		s2 << electrodegrid->GetGridName();
		s2 << realindex;
		s2 << "\t" << pos[0] << "\t" << pos[1] << "\t" << pos[2];
		std::string s= electrodegrid->GetGridName();	s+= "\t";
		s+=column; s+= "\t";
		s+=s2.str();
		//		fprintf(fout,"%s \t %s \t %s",electrodegrid->GetGridName(),line,column);
		fprintf(fout,"%s",s.c_str());
		vtkFloatArray* values=electrode->GetValues();
		for (int ia=0;ia<values->GetNumberOfTuples();ia++)
		  fprintf(fout,"\t %.3f",values->GetComponent(ia,0));
		fprintf(fout,"\n");
	      }
	  }
    }
  fclose(fout);
  return 1;
}


int vtkpxElectrodeMultiGridSource::SetElectrodeColorsFromFunction(vtkLookupTable* cmap)
{
  if (cmap==NULL)
    return 0;

  int d=this->ElectrodeGridCollection->GetNumberOfItems();
  for (int k=0;k<d;k++)
    {
      vtkpxElectrodeGridSource* electrodegrid=this->GetElectrodeGrid(k);
      electrodegrid->SetColorMode(2);
      int dim[2]; 	electrodegrid->GetDimensions(dim);
      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {
	    vtkpxElectrodeSource* electrode=electrodegrid->GetElectrode(i,j);
	    if (electrode->GetElectrodePresent()==1)
	      {
		double v=electrode->GetValues()->GetComponent(0,0);
		double c[3];
		cmap->GetColor(v,c);
		float c1[3]= { c[0],c[1],c[2]};
		electrode->SetColor(c1);
	      }
	  }
      electrodegrid->Modified();
    }
  return 1;


}
//----------------------------------------------------------------------------
void vtkpxElectrodeMultiGridSource::SetCurrentValueIndex(int a)
{
  int d=this->ElectrodeGridCollection->GetNumberOfItems();
  for (int k=0;k<d;k++)
    {
      vtkpxElectrodeGridSource* electrodegrid=this->GetElectrodeGrid(k);
      electrodegrid->SetCurrentValueIndex(a);
    }
  this->Modified();
}

int  vtkpxElectrodeMultiGridSource::GetCurrentValueIndex()
{
  vtkpxElectrodeGridSource* electrodegrid=this->GetElectrodeGrid(0);
  if (electrodegrid==NULL)
    return -1;
  
  return electrodegrid->GetCurrentValueIndex();
}

