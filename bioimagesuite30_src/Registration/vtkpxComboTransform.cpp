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





#include "vtkObjectFactory.h"
#include "vtkpxComboTransform.h"
#include "vtkCollection.h"
#include "vtkTransformToGrid.h"
#include "vtkStructuredPointsReader.h"
#include "vtkStructuredPoints.h"
#include "vtkImageCast.h"
//----------------------------------------------------------------------------
vtkpxComboTransform *vtkpxComboTransform::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxComboTransform");
  if(ret)
    {
      return (vtkpxComboTransform*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxComboTransform;
}

//----------------------------------------------------------------------------
vtkpxComboTransform::vtkpxComboTransform()
{
  this->LinearTransform=NULL;
  this->NonLinearFirst=1;
  this->GridTransformationsCollection=vtkCollection::New();
  this->InitializeTransformations();
}

//----------------------------------------------------------------------------
vtkpxComboTransform::~vtkpxComboTransform()
{
  this->SetLinearTransform(NULL);
  this->GridTransformationsCollection->RemoveAllItems();
  this->GridTransformationsCollection->Delete();
}
//----------------------------------------------------------------------------
int vtkpxComboTransform::Save(const char* fname)
{
  return this->Save(fname,0);
}

int vtkpxComboTransform::Save(const char* fname,int compression)
{
  if (compression==0)
    {
      FILE* fout=fopen(fname,"w");
      if (!fout)
	return 0;
      
      int ok=Save(fout);
      fclose(fout);
      return ok;
    } 

  gzFile fout=gzsuffixopen(fname,"wb",compression);
  if (!fout)
    return 0;
  int ok=Save(fout);
  gzclose(fout);
  return ok;
  
}


int vtkpxComboTransform::Save(FILE* fout)
{
  if (this->LinearTransform==NULL || this->GridTransformationsCollection==NULL)
    return 0;

  int nt=this->GridTransformationsCollection->GetNumberOfItems();

  if (nt==1)
    {
      fprintf(fout,"#vtkpxNewComboTransform File\n");
    }
  else
    {
      fprintf(fout,"#vtkpxMultiComboTransform File\n");
      fprintf(fout,"#Number of Non Linear Transformations\n%d\n",
	      this->GridTransformationsCollection->GetNumberOfItems());
    }
  fprintf(fout,"#NonLinearFirst\n%d\n",this->NonLinearFirst);
  fprintf(fout,"#Linear Component\n");
  for(int i=0;i<=3;i++)
    {
      for (int j=0;j<=3;j++)
	fprintf(fout,"%7.3f ",this->LinearTransform->GetMatrix()->GetElement(i,j));
      fprintf(fout,"\n");
    }
  int ok=1;
  for (int i=0;i<nt;i++)
    {
      vtkpxGridTransform* obj=(vtkpxGridTransform*)this->GridTransformationsCollection->GetItemAsObject(i);  
      ok=ok*obj->Save(fout);
    }
  return ok;
}

int vtkpxComboTransform::Save(gzFile fout)
{
  if (this->LinearTransform==NULL || this->GridTransformationsCollection==NULL)
    return 0;

  int nt=this->GridTransformationsCollection->GetNumberOfItems();

  if (nt==1)
    {
      gzprintf(fout,"#vtkpxNewComboTransform File\n");
    }
  else
    {
      gzprintf(fout,"#vtkMultiComboTransform File\n");
      gzprintf(fout,"#Number of Non Linear Transformations\n%d\n",
	       this->GridTransformationsCollection->GetNumberOfItems());
    }
  gzprintf(fout,"#NonLinearFirst\n%d\n",this->NonLinearFirst);
  gzprintf(fout,"#Linear Component\n");
  for(int i=0;i<=3;i++)
    {
      for (int j=0;j<=3;j++)
	gzprintf(fout,"%7.3f ",this->LinearTransform->GetMatrix()->GetElement(i,j));
      gzprintf(fout,"\n");
    }

  int ok=1;
  for (int i=0;i<nt;i++)
    {
      vtkpxGridTransform* obj=(vtkpxGridTransform*)this->GridTransformationsCollection->GetItemAsObject(i);  
      ok=ok*obj->Save(fout);
    }
  return ok;
}

//----------------------------------------------------------------------------
int vtkpxComboTransform::Load(const char* fname)
{  
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  int ok=Load(fin);
  gzclose(fin);

  if (ok==-1)
    {
      // Load this as .vtk file
      ok=0;
      vtkStructuredPointsReader* rd=vtkStructuredPointsReader::New();
      rd->SetFileName(fname);
      rd->Update();

      int nc=rd->GetOutput()->GetNumberOfScalarComponents();
      if (nc==3)
	{
	  vtkImageCast* cst=vtkImageCast::New();
	  cst->SetInput(rd->GetOutput());
	  cst->SetOutputScalarTypeToDouble();
	  cst->Update();
	  this->ClearGridTransformationsCollection();
	  this->LinearTransform->Identity();
	  this->SetNonLinearFirst(1);
	  vtkpxGridTransform* gr=vtkpxGridTransform::New();
	  gr->SetDisplacementGrid(cst->GetOutput());
	  gr->SetInterpolationModeToLinear();
	  this->AddGridTransform(gr);
	  gr->Delete();
	  cst->Delete();

	  //	  fprintf(stderr,"Transformation Loaded from %s (vtk format)\n",fname);
	  ok=1;
	}
      rd->Delete();
    }
      

  return ok;
}

int vtkpxComboTransform::Load(gzFile fin)
{
  this->InitializeTransformations();
  char pxtk_buffer[255];
  gzgets(fin,pxtk_buffer,100);
  //  fprintf(stderr,"Header = %s\n",pxtk_buffer);
  int isvtk=gzfindstringinstring(pxtk_buffer,"# vtk DataFile Version 3.0");
  if (isvtk)
    return -1;

  int iscombo=gzfindstringinstring(pxtk_buffer,"#vtkpxComboTransform File");
  int isnewcombo=gzfindstringinstring(pxtk_buffer,"#vtkpxNewComboTransform File");
  int ismulticombo=gzfindstringinstring(pxtk_buffer,"#vtkpxMultiComboTransform File");
  int isgrid=gzfindstringinstring(pxtk_buffer,"#vtkpxBaseGridTransform2 File");



  //  fprintf(stderr,"iscombo=%d isnewcombo=%d ismulticombo=%d isgrid=%d\n",
  //	  iscombo,isnewcombo,ismulticombo,isgrid);

  if (iscombo==0 && isnewcombo==0 && ismulticombo==0 && isgrid==0)
    return 0;


  this->ClearGridTransformationsCollection();

  if (isgrid==1)
    {
      this->LinearTransform->Identity();
      this->SetNonLinearFirst(1);
    }

  int numgrid=1;

  if (iscombo || isgrid)
    {
      this->SetNonLinearFirst(0);
    }
  else if (isnewcombo)
    {
      gzgets(fin,pxtk_buffer,100);
      gzgets(fin,pxtk_buffer,200);
      int a;      sscanf(pxtk_buffer,"%d",&a);
      this->SetNonLinearFirst(a);
    } 
  else if (ismulticombo)
    {
      gzgets(fin,pxtk_buffer,100);
      gzgets(fin,pxtk_buffer,200);
      sscanf(pxtk_buffer,"%d",&numgrid);
      gzgets(fin,pxtk_buffer,100);
      gzgets(fin,pxtk_buffer,200);
      int a;      sscanf(pxtk_buffer,"%d",&a);
      this->SetNonLinearFirst(a);
    }
  
  // Now Load Identity
  if (!isgrid)
    {
      gzgets(fin,pxtk_buffer,100);
      vtkMatrix4x4* mat=vtkMatrix4x4::New();
      mat->Identity();
      
      float m1,m2,m3,m4;
      for (int i=0;i<=3;i++)
	{
	  gzgets(fin,pxtk_buffer,200);
	  sscanf(pxtk_buffer,"%f %f %f %f",&m1,&m2,&m3,&m4);
	  mat->SetElement(i,0,m1);
	  mat->SetElement(i,1,m2);
	  mat->SetElement(i,2,m3);
	  mat->SetElement(i,3,m4);
	}
      
      this->LinearTransform->Identity();
      this->LinearTransform->PostMultiply();
      this->LinearTransform->Concatenate(mat);
      mat->Delete();
    } 
  
  int ok=1;
  if (!isgrid)
    {
      for (int i=0;i<numgrid;i++)
	{
	  vtkpxGridTransform* gr=vtkpxGridTransform::New();
	  int ok2=gr->Load(fin);
	  if (ok2)
	    {
	      this->AddGridTransform(gr);
	    }
	  else
	    {
	      fprintf(stderr,"Failed to read Grid Transform %d of %d\n",i+1,numgrid);
	    }
	  gr->Delete();
	  ok=ok*ok2;
	}
    }
  else
    {
      numgrid=1;
      vtkpxGridTransform* gr=vtkpxGridTransform::New();
      ok=gr->Load(fin,1,"#vtkpxBaseGridTransform2 File");
      if (ok)
	this->AddGridTransform(gr);
      else
	fprintf(stderr,"Failed to Load Single Grid Transform\n");

      gr->Delete();
    }
  return ok;
  
}
//----------------------------------------------------------------------------
void vtkpxComboTransform::UpdateStack()
{
  this->Identity();
  this->PostMultiply();
  if (this->NonLinearFirst==0)
      this->Concatenate(this->LinearTransform);

  int nt=this->GridTransformationsCollection->GetNumberOfItems();
  for (int i=nt-1;i>=0;i=i-1)
    {
      vtkpxGridTransform* obj=(vtkpxGridTransform*)this->GridTransformationsCollection->GetItemAsObject(i);  
      this->Concatenate(obj);
    }
  
  if (this->NonLinearFirst==1)
    this->Concatenate(this->LinearTransform);
  this->Modified();
}

// --------------------------------------------------------------------------------

void vtkpxComboTransform::SetNonLinearFirst(int s)
{
  int old=this->NonLinearFirst;
  this->NonLinearFirst=(s>0);
  //  if (old!=this->NonLinearFirst)
  this->UpdateStack();
}

void vtkpxComboTransform::ChangeLinearTransform(vtkTransform* LinearTransformIn)
{
  this->SetLinearTransform(LinearTransformIn);
  this->UpdateStack();
}


int vtkpxComboTransform::GetNumberOfGridTransformations()
{
  return this->GridTransformationsCollection->GetNumberOfItems();
}

// --------------------------------------------------------------------------------
vtkpxGridTransform* vtkpxComboTransform::GetGridTransform()
{
  return this->GetGridTransformByIndex(-1);
}

vtkpxGridTransform* vtkpxComboTransform::GetGridTransformByIndex(int index)
{
  int nt=this->GridTransformationsCollection->GetNumberOfItems();
  if (index<0 || index>=nt)
    index=nt-1;
  vtkpxGridTransform* obj=(vtkpxGridTransform*)this->GridTransformationsCollection->GetItemAsObject(index);  
  return obj;
}

void vtkpxComboTransform::ChangeGridTransform(vtkpxGridTransform* GridTransform)
{
  return this->ChangeGridTransformByIndex(GridTransform,-1);
}
 
void vtkpxComboTransform::ChangeGridTransformByIndex(vtkpxGridTransform* GridTransform,int index)
{
  int nt=this->GridTransformationsCollection->GetNumberOfItems();
  if (index<0 || index>=nt)
    index=nt-1;
  
  this->GridTransformationsCollection->ReplaceItem(index,GridTransform);
  this->UpdateStack();
}


void vtkpxComboTransform::SetGridTransform(vtkpxGridTransform* GridTransform)
{
  int nt=this->GridTransformationsCollection->GetNumberOfItems();
  if (nt>0)
    {
      this->ChangeGridTransformByIndex(GridTransform,-1);
      return;
    }

  this->GridTransformationsCollection->AddItem(GridTransform);
  this->UpdateStack();
}


int vtkpxComboTransform::ClearGridTransformationsCollection()
{
  this->GridTransformationsCollection->RemoveAllItems();
  this->UpdateStack();
  return 0;
}

int vtkpxComboTransform::AddGridTransform(vtkpxGridTransform* GridTransform)
{
  this->GridTransformationsCollection->AddItem(GridTransform);
  this->UpdateStack();
  return 0;
}


void vtkpxComboTransform::SetTransformations(vtkTransform* LinearTransformIn,vtkpxGridTransform* GridTransformIn)
{
  this->SetLinearTransform(LinearTransformIn);
  this->SetGridTransform(GridTransformIn);
  this->UpdateStack();
}


void vtkpxComboTransform::InitializeTransformations()
{
  if (this->LinearTransform==NULL || this->GetNumberOfGridTransformations()==0 )
    {
      vtkTransform* tr=vtkTransform::New();
      vtkpxGridTransform* gr=vtkpxGridTransform::New();
      this->SetTransformations(tr,gr);
      tr->Delete();
      gr->Delete();
    }
}

// Copy will be a pain
// --------------------
void vtkpxComboTransform::InternalDeepCopy(vtkAbstractTransform *gtrans)
{
  vtkGeneralTransform::InternalDeepCopy(gtrans);
  vtkpxComboTransform *transform =(vtkpxComboTransform *)gtrans;
  
  vtkTransform* tr=vtkTransform::New();
  tr->Identity();
  tr->Concatenate(transform->GetLinearTransform());
  
  this->SetNonLinearFirst(transform->GetNonLinearFirst());
  this->SetLinearTransform(tr);
  tr->Delete();

  int nt=transform->GetNumberOfGridTransformations();
  this->ClearGridTransformationsCollection();

  for (int i=0;i<nt;i++)
    {
      vtkpxGridTransform* newgridxform=vtkpxGridTransform::New();
      newgridxform->DeepCopy(transform->GetGridTransformByIndex(i));
      this->AddGridTransform(newgridxform);
      newgridxform->Delete();
    }
}

void vtkpxComboTransform::Copy(vtkpxComboTransform *ComboTransform)
{
  this->InternalDeepCopy(ComboTransform);
}
// --------------------
void vtkpxComboTransform::Inverse()
{
  fprintf(stderr,"Inverting Transform");
  this->GetLinearTransform()->Inverse();

  int nt=this->GridTransformationsCollection->GetNumberOfItems();
  if (nt==1)
    {
      this->GetGridTransform()->Inverse();
    }
  else
    {
      vtkCollection* temp=vtkCollection::New();
      for (int i=0;i<nt;i++)
	temp->AddItem(this->GridTransformationsCollection->GetItemAsObject(i));

      this->GridTransformationsCollection->RemoveAllItems();

      for (int i=nt-1;i>=0;i=i-1)
	{
	  vtkpxGridTransform* tr=(vtkpxGridTransform*)temp->GetItemAsObject(i);
	  tr->Inverse();
      	  this->GridTransformationsCollection->AddItem(tr);
	}
      temp->Delete();
    }
  this->SetNonLinearFirst(1-this->NonLinearFirst);
}




int vtkpxComboTransform::CollapseTransormationsCollection()
{
  int nt=this->GridTransformationsCollection->GetNumberOfItems();

  // No point in this if we only have zero or 1 transformation !!!!
  if (nt<2)
    return 1;

  double spacing[3],origin[3],outer[3];

  vtkGeneralTransform* gen=vtkGeneralTransform::New();
  gen->PostMultiply();


  for (int i=0;i<nt;i++)
    {
      vtkpxGridTransform* tr=(vtkpxGridTransform*)this->GridTransformationsCollection->GetItemAsObject(i);  
      vtkImageData* img=tr->GetDisplacementGrid();
      double spa[3];img->GetSpacing();
      double ori[3];img->GetOrigin();
      int    dim[3];img->GetDimensions(dim);

      for (int ia=0;ia<=2;ia++)
	{
	  if (i==0 || spa[ia]<spacing[ia])
	    spacing[ia]=spa[ia];
	  if (i==0 || ori[ia]<origin[ia])
	    origin[ia]=ori[ia];

	  double maxv=ori[ia]+spacing[ia]*double(dim[ia]-1);
	  if (i==0 || outer[ia]<maxv)
	    outer[ia]=maxv;
	}
      gen->Concatenate(tr);
    }

  int extent[6];
  for (int ia=0;ia<=2;ia++)
    {
      extent[ia*2]=0;
      extent[ia*2+1]=int(outer[ia]/spacing[ia])+1;
    }

  
  vtkTransformToGrid* tgrid=vtkTransformToGrid::New();
  tgrid->SetGridExtent(extent);
  tgrid->SetGridOrigin(origin);
  tgrid->SetGridSpacing(spacing);
  tgrid->SetInput(gen);
  tgrid->Update();

  vtkpxGridTransform* newgr=vtkpxGridTransform::New();
  newgr->SetInterpolationModeToLinear();
  newgr->SetDisplacementGrid(tgrid->GetOutput());
	
  this->ClearGridTransformationsCollection();
  this->AddGridTransform(newgr);

  tgrid->Delete();
  newgr->Delete();
  gen->Delete();

  return 1;


}




