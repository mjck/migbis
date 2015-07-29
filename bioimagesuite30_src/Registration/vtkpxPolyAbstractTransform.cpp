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
  Module:    $RCSfile: vtkpxPolyAbstractTransform.cpp,v $
  Language:  C++
  Date:      $Date: 2003/04/11 20:00:25 $
  Version:   $Revision: 1.2 $


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
#include "vtkpxPolyAbstractTransform.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkpxAnalyzeImageWriter.h"
#include "vtkpxAnalyzeImageSource.h"
#include "vtkpxUtil.h"
#include "vtkpxTransformationUtil.h"
#include "vtkpxComboTransform.h"
#include "vtkpxBaseGridTransform.h"
#include "vtkpxSimilarityTransform.h"
//----------------------------------------------------------------------------
vtkpxPolyAbstractTransform* vtkpxPolyAbstractTransform::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxPolyAbstractTransform");
  if(ret)
    {
      return (vtkpxPolyAbstractTransform*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxPolyAbstractTransform;
}

//------------------------------------------------------------------------
vtkpxPolyAbstractTransform::vtkpxPolyAbstractTransform()
{
  this->RegionLabel=NULL;
  this->TransformationCollection=vtkCollection::New();
  this->ThinPlateSplineTransform=NULL;
}

//------------------------------------------------------------------------
vtkpxPolyAbstractTransform::~vtkpxPolyAbstractTransform()
{
  if (this->RegionLabel)
    this->SetRegionLabel(NULL);

  if (this->TransformationCollection)
    this->TransformationCollection->Delete();

  if (this->ThinPlateSplineTransform!=NULL)
    this->ThinPlateSplineTransform->Delete();
}

//------------------------------------------------------------------------
int vtkpxPolyAbstractTransform::IsValid()
{
  if (this->RegionLabel==NULL || this->TransformationCollection->GetNumberOfItems()==0)
    return 0;

  int nc=this->RegionLabel->GetNumberOfScalarComponents();
  if (nc!=1 && nc!=4)
    return 0;

  return 1;
}
//------------------------------------------------------------------------
unsigned long vtkpxPolyAbstractTransform::GetMTime()
{
  if (this->IsValid()==0)
    return 0;

  unsigned long result = this->RegionLabel->GetMTime();

  int np=this->TransformationCollection->GetNumberOfItems();
  for (int i=0;i<np;i++)
    {
      vtkObject* obj=this->TransformationCollection->GetItemAsObject(i);  
      unsigned long mtime = obj->GetMTime(); 
      if (mtime > result)
	result = mtime;
    }

  return result;
}
//------------------------------------------------------------------------
void vtkpxPolyAbstractTransform::PrintSelf(ostream& os, vtkIndent indent)
{
  if (this->IsValid()==0)
    {
      os << indent << " Not Initialized \n";
      return;
    }

  vtkAbstractTransform::PrintSelf(os,indent);
  
  os << indent << "RegionLabel: " << this->RegionLabel << "\n";
  this->RegionLabel->PrintSelf(os,indent.GetNextIndent());

  os << indent << "TransformationCollection: " << this->TransformationCollection << "\n";
  this->TransformationCollection->PrintSelf(os,indent.GetNextIndent());
 
  os << indent << "ThinPlateSplineTransform: " << this->ThinPlateSplineTransform << "\n";

}

//----------------------------------------------------------------------------
vtkAbstractTransform *vtkpxPolyAbstractTransform::MakeTransform()
{
  return vtkpxPolyAbstractTransform::New(); 
}

//----------------------------------------------------------------------------
void vtkpxPolyAbstractTransform::InternalDeepCopy(vtkAbstractTransform *transform)
{
  vtkpxPolyAbstractTransform *t = (vtkpxPolyAbstractTransform *)transform;
  this->SetRegionLabel(t->GetRegionLabel());
  
  this->TransformationCollection->RemoveAllItems();

  vtkCollection* tcol=t->GetTransformationCollection();
  for (int i=0;i<tcol->GetNumberOfItems();i++)
    {
      vtkObject* obj=tcol->GetItemAsObject(i);
      this->TransformationCollection->AddItem(obj);
    }

  if (t->GetThinPlateSplineTransform()==NULL)
    {
      if (this->ThinPlateSplineTransform!=NULL)
	this->ThinPlateSplineTransform->Delete();
      this->ThinPlateSplineTransform=NULL;
    }
  else
    {
      if (this->ThinPlateSplineTransform==NULL)
	this->ThinPlateSplineTransform=vtkThinPlateSplineTransform::New();
      this->ThinPlateSplineTransform->DeepCopy(t->GetThinPlateSplineTransform());
    }
  this->Modified();
}
//------------------------------------------------------------------------
void vtkpxPolyAbstractTransform::InternalUpdate()
{
  return;
}
//------------------------------------------------------------------------
void vtkpxPolyAbstractTransform::InternalTransformPoint(const double point[3], 
							double output[3])
{
  vtkAbstractTransform* tr0=(vtkAbstractTransform*)(this->TransformationCollection->GetItemAsObject(0));

  if (tr0==NULL || this->RegionLabel==NULL)
    {
      vtkErrorMacro(<<"Bad tr0\n");
      return;
    }

      
  if (this->TransformationCollection->GetNumberOfItems()==1)
    {
      tr0->InternalTransformPoint(point,output);
      return;
    }
 
  int p=this->RegionLabel->FindPoint(point[0],point[1],point[2]);
  if (p==-1)
    {
      if (this->ThinPlateSplineTransform!=NULL)
	{
	  //	  fprintf(stderr,"+");
	  this->ThinPlateSplineTransform->TransformPoint(point,output);
	}
      else
	tr0->TransformPoint(point,output);
      return;
    }

  int l=(int)this->RegionLabel->GetPointData()->GetScalars()->GetComponent(p,0);
  if (l==-1)
    {
      if (this->ThinPlateSplineTransform!=NULL)
	{
	  //	  fprintf(stderr,"+");
	  this->ThinPlateSplineTransform->TransformPoint(point,output);
	}
      else
	tr0->TransformPoint(point,output);
      return;
    }

  vtkAbstractTransform* tr=(vtkAbstractTransform*)(this->TransformationCollection->GetItemAsObject(l));
  tr->InternalTransformPoint(point,output);


}
// ---------------------------------------------------------------------------------------
void vtkpxPolyAbstractTransform::InternalTransformPoint(const float point[3], 
							float output[3])
{
  double x[3]= { point[0],point[1],point[2]};
  double y[3];
  this->InternalTransformPoint(x,y);
  for (int ia=0;ia<=2;ia++)
    output[ia]=y[ia];
}

//------------------------------------------------------------------------
void vtkpxPolyAbstractTransform::InternalTransformDerivative(const double point[3],
							 double output[3],
							 double derivative[3][3])
{

  vtkAbstractTransform* tr0=(vtkAbstractTransform*)(this->TransformationCollection->GetItemAsObject(0));
  if (this->TransformationCollection->GetNumberOfItems()==1)
    {
      tr0->InternalTransformDerivative(point,output,derivative);
      return;
    }

  int p=this->RegionLabel->FindPoint(point[0],point[1],point[2]);
  if (p==-1)
    {
      if (this->ThinPlateSplineTransform!=NULL)
	{
	  //	  fprintf(stderr,"*");
	  this->ThinPlateSplineTransform->InternalTransformDerivative(point,output,derivative);
	}
      else
	tr0->InternalTransformDerivative(point,output,derivative);
      return;
    }

  int l=(int)this->RegionLabel->GetPointData()->GetScalars()->GetComponent(p,0);
  if (l==-1)
    {
      if (this->ThinPlateSplineTransform!=NULL)
	{
	  //	  fprintf(stderr,"*");
	  this->ThinPlateSplineTransform->InternalTransformDerivative(point,output,derivative);
	}
      else
	tr0->InternalTransformDerivative(point,output,derivative);
      return;
    }


  vtkAbstractTransform* tr=(vtkAbstractTransform*)(this->TransformationCollection->GetItemAsObject(l));

  if (this->RegionLabel->GetNumberOfScalarComponents()==1)
    {
      tr->InternalTransformDerivative(point,output,derivative);
    }
  else
    {

      int l2=(int)this->RegionLabel->GetPointData()->GetScalars()->GetComponent(p,2);
      vtkAbstractTransform* tr2=(vtkAbstractTransform*)(this->TransformationCollection->GetItemAsObject(l2));

      double w1=this->RegionLabel->GetPointData()->GetScalars()->GetComponent(p,1);
      double w2=this->RegionLabel->GetPointData()->GetScalars()->GetComponent(p,3);
      
      double wsum=w1+w2; if (wsum<0.001) wsum=1.0;
      w1/=wsum; w2/=wsum;

      double p1[3],p2[3],d1[3][3],d2[3][3];
      tr->InternalTransformDerivative(point,p1,d1);
      tr2->InternalTransformDerivative(point,p2,d2);
      
      for (int ia=0;ia<=2;ia++)
	{
	  output[ia]=(w1*p1[ia]+w2*p2[ia]);
	  for (int ib=0;ib<=2;ib++)
	    derivative[ia][ib]=w1*d1[ia][ib]+w2*d2[ia][ib];
	}
    }
}
// ---------------------------------------------------------------------------------------
void vtkpxPolyAbstractTransform::InternalTransformDerivative(const float point[3],
							 float output[3],
							 float derivative[3][3])
{
  double x[3]= { point[0],point[1],point[2]};
  double y[3];
  double d[3][3];
  this->InternalTransformDerivative(x,y,d);
  for (int ia=0;ia<=2;ia++)
    {
      output[ia]=y[ia];
      for (int ib=0;ib<=2;ib++)
	derivative[ia][ib]=d[ia][ib];
    }
}
// ---------------------------------------------------------------------------------------
int vtkpxPolyAbstractTransform::Load(const char* fname)
{
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;

  char pxtk_buffer[255];
  gzgets(fin,pxtk_buffer,100);
  if (gzfindstringinstring(pxtk_buffer,"#vtkpxPolyAbstractTransform")==0)
    {
      gzclose(fin);
      return 0;
    }

  gzgets(fin,pxtk_buffer,100);
  gzgets(fin,pxtk_buffer,200);
  int numtransforms;      
  sscanf(pxtk_buffer,"%d",&numtransforms);
  if (numtransforms<1)
    {
      gzclose(fin);
      return 0;
    }

  //  fprintf(stderr,"Numtransforms=%d\n",numtransforms);
  
  this->TransformationCollection->RemoveAllItems();

  for (int i=0;i<numtransforms;i++)
    {
      gzgets(fin,pxtk_buffer,200);
      gzgets(fin,pxtk_buffer,200);
      //      fprintf(stderr,"Header: %s\n",pxtk_buffer);
      gzgets(fin,pxtk_buffer,200);
      //      fprintf(stderr,"Type : %s\n",pxtk_buffer);

      if (gzfindstringinstring(pxtk_buffer,"Combo Transform")==1)
	{
	  vtkpxComboTransform* tr=vtkpxComboTransform::New();
	  tr->Load(fin);
	  this->TransformationCollection->AddItem(tr);
	  tr->Delete();
	}
      else if (gzfindstringinstring(pxtk_buffer,"Grid Transform")==1)
	{
	  vtkpxGridTransform* tr=vtkpxGridTransform::New();
	  tr->Load(fin);
	  this->TransformationCollection->AddItem(tr);
	  tr->Delete();
	}
      else if (gzfindstringinstring(pxtk_buffer,"Similarity Transform")==1)
	{
	  vtkpxSimilarityTransform* tr=vtkpxSimilarityTransform::New();
	  tr->Load(fin);
	  this->TransformationCollection->AddItem(tr);
	  tr->Delete();
	}
      else if (gzfindstringinstring(pxtk_buffer,"Linear Transform")==1)
	{
	  vtkTransform* tr=vtkTransform::New();
	  vtkMatrix4x4* mat_in=vtkMatrix4x4::New();
	  mat_in->Identity();
	  float mat[3];
	  for (int ia=0;ia<=3;ia++)
	    {
	      gzgets(fin,pxtk_buffer,200);
	      sscanf(pxtk_buffer,"%f %f %f %f",&mat[0],&mat[1],&mat[2],&mat[3]);
	      for (int j=0;j<=3;j++)
		mat_in->SetElement(ia,j,mat[j]);
	    }
	  tr->Concatenate(mat_in);
	  mat_in->Delete();
	  this->TransformationCollection->AddItem(tr);
	  tr->Delete();
	}
    }

  if (this->ThinPlateSplineTransform==NULL)
    this->ThinPlateSplineTransform=vtkThinPlateSplineTransform::New();
     
  int ok=vtkpxTransformationUtil::LoadThinPlateSplineTransform(this->ThinPlateSplineTransform,fin);
  if (ok==0)
    {
      this->ThinPlateSplineTransform->Delete();
      this->ThinPlateSplineTransform=NULL;
    }
        

  gzclose(fin);

  vtkpxAnalyzeImageSource* w=vtkpxAnalyzeImageSource::New();
  w->Load(fname);

  this->SetRegionLabel(w->GetOutput());
  w->Delete();
  
  return 1;
}

void vtkpxPolyAbstractTransform::Identity()
{
  int np=this->TransformationCollection->GetNumberOfItems();
  for (int i=0;i<np;i++)
    {
      vtkObject* obj=this->TransformationCollection->GetItemAsObject(i);  
      if (obj->IsA("vtkAbstractTransform"))
	((vtkAbstractTransform*)obj)->Identity();
    }
}

int vtkpxPolyAbstractTransform::Save(const char* fname)
{
  if (this->IsValid()==0)
    return 0;

  vtkpxAnalyzeImageWriter* w=vtkpxAnalyzeImageWriter::New();
  w->SetInput(this->RegionLabel);
  w->Save(fname);
  
  int np=this->TransformationCollection->GetNumberOfItems();


  FILE* fout=fopen(fname,"w");
  fprintf(fout,"#vtkpxPolyAbstractTransform\n");
  fprintf(fout,"#Number Of Transformations\n");
  fprintf(fout,"%d\n",np);

  for (int i=0;i<np;i++)
    {
      fprintf(fout,"#*****************************\n#Transformation %d\n",i+1);
      vtkObject* obj=this->TransformationCollection->GetItemAsObject(i);  
      if (obj->IsA("vtkpxComboTransform"))
	{
	  fprintf(fout,"# Combo Transform\n");
	  ((vtkpxComboTransform*)(obj))->Save(fout);
	}
      else if (obj->IsA("vtkpxBaseGridTransform"))
	{
	  fprintf(fout,"# Grid Transform\n");
	  ((vtkpxBaseGridTransform*)(obj))->Save(fout);
	}
      else if (obj->IsA("vtkpxSimilarityTransform"))
	{
	  fprintf(fout,"# Similarity Transform\n");
	  ((vtkpxSimilarityTransform*)(obj))->Save(fout);
	}
      else if (obj->IsA("vtkLinearTransform"))
	{
	  vtkLinearTransform* tr=(vtkLinearTransform*)obj;
	  fprintf(fout,"# Linear Transform\n");
	  for(int row=0;row<=3;row++)
	    {
	      for (int j=0;j<=3;j++)
		fprintf(fout,"%7.3f ",tr->GetMatrix()->GetElement(row,j));
	      fprintf(fout,"\n");
	    }
	}
    }

  if (this->ThinPlateSplineTransform==NULL)
    fprintf(fout,"No Thin Plate Spline Transform\n");
  else
    vtkpxTransformationUtil::SaveThinPlateSplineTransform(this->ThinPlateSplineTransform,fout);
    
  fclose(fout);
  return 1;
}


void vtkpxPolyAbstractTransform::Inverse()
{
  vtkErrorMacro(<<"Inverse Not Implemented Yet\n");
}



