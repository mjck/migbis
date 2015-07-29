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







#include "vtkpxMatrix.h"
#include "vtkObjectFactory.h"
#include "vtkpxUtil.h"
#include "bio_f2c.h"
#include "clapack.h"
#include "pxutil.h"
#include "vtkpxMath.h"
#include "vnl/vnl_vector.h"
#include "vnl/vnl_vector_fixed.h"
#include "vnl/vnl_matrix.h"
#include "vnl/vnl_matrix_fixed.h"
#include "vnl/vnl_matlab_filewrite.h"
#include <vnl/vnl_matlab_read.h>
#include <vnl/vnl_matlab_write.h>
#include <vnl/vnl_matlab_print.h>
#include <vcl_cstring.h>
#include <vcl_fstream.h>
#include "bis_znzlib.h"
#include <pxutil.h>
// -----------------------------------------------------------------------------------

vtkpxMatrix* vtkpxMatrix::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxMatrix");
  if(ret)
      {
	return (vtkpxMatrix*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxMatrix;
}

// -----------------------------------------------------------------------------------
vtkpxMatrix::vtkpxMatrix()
{
  this->Size[0]=0;
  this->Size[1]=0;
  this->TrueSize=0;
  this->TotalSize=0;
  this->Matrix=NULL;
}
// -----------------------------------------------------------------------------------
vtkpxMatrix::~vtkpxMatrix()
{
  if (this->Matrix!=NULL)
    delete [] this->Matrix;
}
// -----------------------------------------------------------------------------------
// Some in-line stuff

void vtkpxMatrix::Zeros(int rows,int cols)
{
  Allocate(rows,cols);
  Fill(0.0);
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::Ones(int rows,int cols)
{
  Allocate(rows,cols); Fill(1.0);
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::Eye(int rowcol)
{
  Allocate(rowcol,rowcol) ; Identity();
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::Allocate(int rows,int columns)
{
  if (rows==0 || columns==0)
    return;

  if (this->Matrix!=NULL)
    {
      if (rows*columns<=this->TrueSize)
	{
	  this->Size[0]=rows;
	  this->Size[1]=columns;
	  this->TotalSize=this->Size[0]*this->Size[1];
	  return;
	}

      delete [] this->Matrix;
    }

  this->Size[0]=Irange(rows,1,100000000);
  this->Size[1]=Irange(columns,1,100000000);
  this->TotalSize=this->Size[0]*this->Size[1];
  this->TrueSize=this->TotalSize;
  this->Matrix=new float[this->TrueSize];
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::Squeeze()
{
  if (this->Matrix==NULL)
    return;

  if (this->TotalSize==this->TrueSize)
    return;

  this->TrueSize=this->Size[0]*this->Size[1];
  float* tmp=this->Matrix;

  this->Matrix=new float[this->TrueSize];
  for (int i=0;i<this->TrueSize;i++)
    this->Matrix[i]=tmp[i];

  delete [] tmp;
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::Identity()
{
  if (this->Matrix==NULL || (this->Size[0]!=this->Size[1]))
    return;

  this->Zero();
  for (int i=0;i<this->Size[0];i++)
    this->SetDirectElement(i,i,1.0);
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::Zero()
{
  Fill(0);
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::Fill(float v)
{
  if (this->Matrix==NULL)
    return;

  for (int i=0;i<this->TotalSize;i++)
    this->Matrix[i]=v;
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::Transpose()
{
  if (this->Matrix==NULL)  return;

  // Only need to do transposing if matrix is not a row or col vector,
  if (this->Size[0]==1 || this->Size[1]==1)
    {
      int t=this->Size[0];
      this->Size[0]=this->Size[1];
      this->Size[1]=t;
      return;
    }

  // Square Matrix
  if (this->Size[1]==this->Size[0])
    {
      for (int i=0;i<this->Size[0];i++)
	for (int j=i+1;j<this->Size[1];j++)
	  {
	    float a=this->GetDirectElement(i,j);
	    this->SetDirectElement(i,j,this->GetDirectElement(j,i));
	    this->SetDirectElement(j,i,a);
	  }
      return;
    }

  // Copy Matrix out first
  vtkpxMatrix* temp=vtkpxMatrix::New();
  temp->Copy(this);

  int r=Size[0];
  int c=Size[1];
  this->Allocate(c,r);

  for (int i=0;i<this->Size[0];i++)
    for (int j=0;j<this->Size[1];j++)
      this->SetDirectElement(i,j,temp->GetDirectElement(j,i));

  temp->Delete();
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::Print(const char* message)
{
  fprintf(stdout,"\n %s =\n",message);
  this->InternalPrint();
  fprintf(stdout,"\n");
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::InternalPrint()
{
  if (this->Matrix==NULL)  return;

  for(int i = 0; i < this->Size[0]; i++)
    {
      fprintf(stdout,"[ ");
      for(int j = 0; j < this->Size[1]; j++)
	fprintf(stdout,"%7.3f ", this->GetDirectElement(i,j));
      fprintf(stdout,"]\n");
    }

}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::PrintRange(const char* message,int r1,int r2,int c1,int c2)
{
  fprintf(stdout,"\n %s (%d:%d,%d:%d) =\n",message,r1,r2,c1,c2);
  this->PrintRange(r1,r2,c1,c2);
  fprintf(stdout,"\n");
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::PrintRange(int r1,int r2,int c1,int c2)
{
  if (this->Matrix==NULL)  return;


  if (r1==-1)
    r1=0;
  else
    r1=Irange(r1,0,this->Size[0]-1);

  if (r2==-1)
    r2=this->Size[0]-1;
  else
    r2=Irange(r2,r1,this->Size[0]-1);

  if (c1==-1)
    c1=0;
  else
    c1=Irange(c1,0,this->Size[1]-1);

  if (c2==-1)
    c2=this->Size[1]-1;
  else
    c2=Irange(c2,c1,this->Size[1]-1);



  for(int i = r1; i <= r2; i++)
    {
      fprintf(stdout,"[ ");
      for(int j = c1; j <= c2; j++)
	fprintf(stdout,"%7.3f ", this->GetDirectElement(i,j));
      fprintf(stdout,"]\n");
    }

}
// -----------------------------------------------------------------------------------
int vtkpxMatrix::Load(const char* fname)
{
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  int ok=Load(fin);
  gzclose(fin);
  return ok;
}


void vtkpxMatrix::ReplaceWithSpace(char* line,const char* t)
{
  if (line==NULL)
    return;

  if (strlen(line)<1)
    return;

  //fprintf(stderr,"Starting line=***%s***\n",line);
  
  int done=0;
  std::string newpath=line;



  if (newpath.length()> strlen(t)+1) 
    {
      while ( done==0 && newpath.length() > (strlen(t)+1 ))
	{
	  int f=newpath.rfind(t);
	  if (f==std::string::npos)
	    {
	      done=1;
	    }
	  else if (f+strlen(t) >= newpath.length())
	    {
	      done =1;
	    }
	  else
	    {
	      newpath.replace(f,strlen(t)," ");
	    }
	}
      strcpy(line,newpath.c_str());
    }
}


int vtkpxMatrix::Load(gzFile fin)
{
  char line[100];

  gzgets(fin,line,100);   gzgets(fin,line,100);   gzgets(fin,line,100);

  if (gzfindstringinstring(line,"type: matrix")==0)
    {
      gzclose(fin);
      return 0;
    }

  int np1,np2;
  gzgets(fin,line,100);  sscanf(line,"# rows: %d",&np1);
  gzgets(fin,line,100);  sscanf(line,"# columns: %d",&np2);

  if (np1<0 || np1>1000000 || np2<0 || np2>1000000)
    {
      gzclose(fin);
      return 0;
    }
  this->Allocate(np1,np2);
  char* newline=new char[np2*15];
  float a=0.0;

  for (int i=0;i<np1;i++)
    {
      gzgets(fin,newline,np2*15);
      vtkpxMatrix::ReplaceWithSpace(newline,"  ");
      vtkpxMatrix::ReplaceWithSpace(newline,"\t");
      vtkpxMatrix::ReplaceWithSpace(newline,",");
      char* parseline=strtok(newline," ");
      for (int j=0;j<np2;j++)
	{
	  sscanf(parseline,"%f",&a);
	  parseline=strtok(NULL," ");
	  this->SetDirectElement(i,j,a);
	}
    }

  delete [] newline;


  return 1;

}
// -----------------------------------------------------------------------------------
int vtkpxMatrix::Save(const char* fname)
{
  char* matname=eatsuffix(fname);
  char* matname2=anaeatpath(matname);
  return this->Save(fname,matname2);
  delete []  matname;

}
// -----------------------------------------------------------------------------------
int vtkpxMatrix::Save(const char* fname,const char* varname,int compression)
{
  if (compression==0)
    {
      FILE* fout=fopen(fname,"w");
      if (fout==NULL)
	return 0;
      int ok=Save(fout,varname);
      fclose(fout);
      return ok;
    }

  gzFile fout=gzsuffixopen(fname,"wb",compression);
  if (!fout)
    return 0;
  int ok=Save(fout,varname);
  gzclose(fout);
  return ok;
}
// -----------------------------------------------------------------------------------
int vtkpxMatrix::Save(gzFile fout,const char* varname)
{
  gzprintf(fout,"#vtkpxMatrix File\n");
  gzprintf(fout,"# name: %s\n# type: matrix\n",varname);
  gzprintf(fout,"# rows: %d\n# columns: %d\n",this->Size[0],this->Size[1]);

  for(int i = 0; i < this->Size[0]; i++)
    {
      for(int j = 0; j < this->Size[1]; j++)
	gzprintf(fout,"%f ", this->GetDirectElement(i,j));
      gzprintf(fout,"\n");
    }
  return 1;
}
// -----------------------------------------------------------------------------------
int vtkpxMatrix::Save(FILE*  fout,const char* varname)
{
  fprintf(fout,"#vtkpxMatrix File\n");
  fprintf(fout,"# name: %s\n# type: matrix\n",varname);
  fprintf(fout,"# rows: %d\n# columns: %d\n",this->Size[0],this->Size[1]);

  for(int i = 0; i < this->Size[0]; i++)
    {
      for(int j = 0; j < this->Size[1]; j++)
	fprintf(fout,"%f ", this->GetDirectElement(i,j));
      fprintf(fout,"\n");
    }
  return 1;
}

int vtkpxMatrix::ExportAFNI(char*  fname,const char* formatstring)
{
  FILE* fout=fopen(fname,"w");
  if (fout==NULL)
    return 0;

  for(int i = 0; i < this->Size[0]; i++)
    {
      for(int j = 0; j < this->Size[1]; j++)
	fprintf(fout,formatstring, this->GetDirectElement(i,j));
      fprintf(fout,"\n");
    }
  fclose(fout);
  return 1;
}

// -----------------------------------------------------------------------------------
void vtkpxMatrix::Print(const char* message,const char* format)
{
  if (this->Matrix==NULL || message==NULL || format==NULL)  return;

  fprintf(stdout,"\n %s =\n",message);

  for(int i = 0; i < this->Size[0]; i++)
    {
      fprintf(stdout,"[ ");
      for(int j = 0; j < this->Size[1]; j++)
	fprintf(stdout,format, this->GetDirectElement(i,j));
      fprintf(stdout,"]\n");
    }
  fprintf(stdout,"\n");

}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::Copy(vtkpxMatrix* oldmat)
{
  int oldsize[2];
  oldmat->GetSize(oldsize);
  this->Allocate(oldsize[0],oldsize[1]);

  float* o_pointer=oldmat->GetMatrixPointer();

  for (int i=0;i<this->TotalSize;i++)
    Matrix[i]=o_pointer[i];
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::Copy(vtkpxMatrix* oldmat,int x1,int x2,int y1,int y2)
{
  int oldsize[2];
  oldmat->GetSize(oldsize);

  if (x1==-1)     x1=0;
  if (x2==-1)     x2=oldsize[0]-1;
  if (y1==-1)     y1=0;
  if (y2==-1)     y2=oldsize[1]-1;

  x1=Irange(x1,0,oldsize[0]-1);
  x2=Irange(x2,0,oldsize[0]-1);
  y1=Irange(y1,0,oldsize[1]-1);
  y2=Irange(y2,0,oldsize[1]-1);

  this->Allocate(x2-x1+1,y2-y1+1);

  int sz[2]; this->GetSize(sz);

  /*  fprintf(stdout,"Copying Matrix -- New matrix Size = %d x %d vs old %d x %d\n",
      sz[0],sz[1],oldsize[0],oldsize[1]);*/

  for (int i=0;i<this->Size[0];i++)
    for (int j=0;j<this->Size[1];j++)
      this->SetDirectElement(i,j,oldmat->GetDirectElement(i+x1,j+y1));

}

int vtkpxMatrix::CopyInto(vtkpxMatrix* oldmat,int x1,int x2,int y1,int y2,int newx,int newy)
{
 int oldsize[2];
  oldmat->GetSize(oldsize);

  if (x1==-1)     x1=0;
  if (x2==-1)     x2=oldsize[0]-1;
  if (y1==-1)     y1=0;
  if (y2==-1)     y2=oldsize[1]-1;

  x1=Irange(x1,0,oldsize[0]-1);
  x2=Irange(x2,0,oldsize[0]-1);
  y1=Irange(y1,0,oldsize[1]-1);
  y2=Irange(y2,0,oldsize[1]-1);


  int sz[2]; this->GetSize(sz);
  if ( (x2-x1+newx) > sz[0] || (y2-y1+newy) > sz[1])
    {
      fprintf(stderr,"Error CopyIntoMatrix ... x1:x2=%d:%d newx=%d y1:y2=%d:%d newy=%d sz=%d,%d oldsz=%d,%d\n",
	      x1,x2,newx,y1,y2,newy,sz[0],sz[1],oldsize[0],oldsize[1]);
      return 0;
    }
  else
      fprintf(stderr,"OK CopyIntoMatrix ... x1:x2=%d:%d newx=%d y1:y2=%d:%d newy=%d sz=%d,%d, oldsz=%d,%d\n",
	      x1,x2,newx,y1,y2,newy,sz[0],sz[1],oldsize[0],oldsize[1]);

  //  this->Allocate(x2-x1+1,y2-y1+1);

  /*  fprintf(stdout,"Copying Matrix -- New matrix Size = %d x %d vs old %d x %d\n",
      sz[0],sz[1],oldsize[0],oldsize[1]);*/

  for (int i=x1;i<=x2;i++)
    for (int j=y1;j<=y2;j++)
      this->SetDirectElement(newx+i-x1,
			     newy+j-y1,
			     oldmat->GetDirectElement(i,j));
      //    this->SetDirectElement(i,j,oldmat->GetDirectElement(i+x1,j+y1));
  return 1;

}


// -----------------------------------------------------------------------------------
void vtkpxMatrix::SetMatrix(double** mat,int x,int y)
{
  Copy(mat,x,y) ;
}

void vtkpxMatrix::Copy(double** mat,int x,int y)
{
  Copy(mat,0,x-1,0,y-1);
}

void vtkpxMatrix::Copy(double** mat,int x1,int x2,int y1,int y2)
{
  this->Allocate(x2-x1+1,y2-y1+1);
  for (int i=0;i<this->Size[0];i++)
    for (int j=0;j<this->Size[1];j++)
      this->SetDirectElement(i,j,mat[i+x1][j+y1]);
}
// -----------------------------------------------------------------------------------
int  vtkpxMatrix::GetRow(int row,int maxval,float* values)
{
  if (Matrix==NULL)
    return 0;
  row=Irange(row,0,this->Size[0]-1);

  int minn=Imin(this->Size[1],maxval);
  for (int i=0;i<minn;i++)
    values[i]=this->GetDirectElement(row,i);
  return minn;
}

int  vtkpxMatrix::GetColumn(int column,int maxval,float* values)
{
  if (Matrix==NULL)
    return 0;
  column=Irange(column,0,this->Size[1]-1);

  int minn=Imin(this->Size[0],maxval);
  for (int i=0;i<minn;i++)
    values[i]=this->GetDirectElement(i,column);
  return minn;
}
// -----------------------------------------------------------------------------------
int  vtkpxMatrix::SetRow(int row,int maxval,float* values)
{
  if (Matrix==NULL)
    return 0;
  row=Irange(row,0,this->Size[0]-1);

  int minn=Imin(this->Size[1],maxval);
  for (int i=0;i<minn;i++)
    this->SetDirectElement(row,i,values[i]);
  return minn;
}

int  vtkpxMatrix::SetColumn(int column,int maxval,float* values)
{
  if (Matrix==NULL)
    return 0;
  column=Irange(column,0,this->Size[1]-1);

  int minn=Imin(this->Size[0],maxval);
  for (int i=0;i<minn;i++)
    this->SetDirectElement(i,column,values[i]);
  return minn;
}
// -----------------------------------------------------------------------------------
int  vtkpxMatrix::GetRow(int row,int maxval,double* values)
{
  if (Matrix==NULL)
    return 0;
  row=Irange(row,0,this->Size[0]-1);

  int minn=Imin(this->Size[1],maxval);
  for (int i=0;i<minn;i++)
    values[i]=this->GetDirectElement(row,i);
  return minn;
}

int  vtkpxMatrix::GetColumn(int column,int maxval,double* values)
{
  if (Matrix==NULL)
    return 0;
  column=Irange(column,0,this->Size[1]-1);

  int minn=Imin(this->Size[0],maxval);
  for (int i=0;i<minn;i++)
    values[i]=this->GetDirectElement(i,column);
  return minn;
}
// -----------------------------------------------------------------------------------
int  vtkpxMatrix::SetRow(int row,int maxval,double* values)
{
  if (Matrix==NULL)
    return 0;
  row=Irange(row,0,this->Size[0]-1);

  int minn=Imin(this->Size[1],maxval);
  for (int i=0;i<minn;i++)
    this->SetDirectElement(row,i,values[i]);
  return minn;
}

int  vtkpxMatrix::SetColumn(int column,int maxval,double* values)
{
  if (Matrix==NULL)
    return 0;
  column=Irange(column,0,this->Size[1]-1);

  int minn=Imin(this->Size[0],maxval);
  for (int i=0;i<minn;i++)
    this->SetDirectElement(i,column,values[i]);
  return minn;
}
// -----------------------------------------------------------------------------------
double**  vtkpxMatrix::GetNewMatrixPointer()
{
  if (Matrix==NULL)
    return NULL;

  double** a=vtkpxMatrix::NewMatrix(this->Size[0],this->Size[1]);
  for (int i=0;i<this->Size[0];i++)
    for (int j=0;j<this->Size[1];j++)
      a[i][j]=this->GetDirectElement(i,j);
  return a;
}
//  =====================================================================
//  Real Matrix Operations !!
//  =====================================================================
void vtkpxMatrix::Scale(float scale)
{
  if (Matrix==NULL)
    return;


  for (int i=0;i<this->TotalSize;i++)
    Matrix[i]*=scale;
}

void vtkpxMatrix::ScaleAdd(float multi,float offset)
{
  if (Matrix==NULL)
    return;


  for (int i=0;i<this->TotalSize;i++)
    Matrix[i]=Matrix[i]*multi+offset;
}

// -----------------------------------------------------------------------------------
void vtkpxMatrix::Add(vtkpxMatrix* a,vtkpxMatrix* b,vtkpxMatrix* c)
{
  int s1[2],s2[2];
  a->GetSize(s1);
  b->GetSize(s2);

  if (s1[1]!=s2[1] || s1[0]!=s2[0])
    {
      fprintf(stderr,"Cannot add matrices bad size a=%dx%d, b=%dx%d!\n",s1[0],s1[1],s2[0],s2[1]);
      return;
    }

  c->Allocate(s1[0],s1[1]);

  float* A=a->GetMatrixPointer();
  float* B=b->GetMatrixPointer();
  float* C=c->GetMatrixPointer();

  int sz=c->GetTotalSize();

  for(int i=0;i<sz;i++)
    {
      *C=*A+*B;
      ++C;
      ++B;
      ++A;
    }
  //    c_pointer[i]=a_pointer[i]+b_pointer[i];

}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::Add(float wa,vtkpxMatrix* a,float wb,vtkpxMatrix* b,vtkpxMatrix* c)
{

  int s1[2],s2[2];
  a->GetSize(s1);
  b->GetSize(s2);

  if (s1[1]!=s2[1] || s1[0]!=s2[0])
    {
      fprintf(stderr,"Cannot add matrices bad size %f*a=%dx%d, %f*b=%dx%d!\n",wa,s1[0],s1[1],wb,s2[0],s2[1]);
      return;
    }

  c->Allocate(s1[0],s1[1]);

  float* A=a->GetMatrixPointer();
  float* B=b->GetMatrixPointer();
  float* C=c->GetMatrixPointer();

  int sz=c->GetTotalSize();

  for(int i=0;i<sz;i++)
    {
      *C = wa*(*A) + wb*(*B);
      ++C;
      ++A;
      ++B;
    }

  //    c_pointer[i]=wa*a_pointer[i]+wb*b_pointer[i];

}

int vtkpxMatrix::IsEqual(vtkpxMatrix* a, vtkpxMatrix* b,double tolerance)
{

  int ret;
  int s1[2],s2[2];

  ret = 0;
  a->GetSize(s1);
  b->GetSize(s2);

  if (s1[1]!=s2[1] || s1[0]!=s2[0])
    {
      fprintf(stderr,"Size of matrices are different a=%dx%d, b=%dx%d!\n",s1[0],s1[1],s2[0],s2[1]);
      ret = 1;
      return ret;
    }

  float* A=a->GetMatrixPointer();
  float* B=b->GetMatrixPointer();

  int sz=a->GetTotalSize();

  for(int i=0;i<sz;i++)
    {
      if(((*A) - (*B) > tolerance ) || ((*A) - (*B) < -tolerance ))
	{
	  ret = 1;
	  return ret;
	}
      ++A;
      ++B;
    }

  return ret;
}

// -----------------------------------------------------------------------------------
void vtkpxMatrix::Multiply(vtkpxMatrix* a,vtkpxMatrix* b,vtkpxMatrix* c)
{
  int s1[2],s2[2],s3[2];
  a->GetSize(s1);
  b->GetSize(s2);

  if (s1[1]!=s2[0])
    {
      fprintf(stderr,"Cannot multiply matrices bad size! a=%dx%d, b=%dx%d\n",s1[0],s1[1],s2[0],s2[1]);
      return;
    }

  c->Allocate(s1[0],s2[1]);
  c->GetSize(s3);

  float* a_pointer=a->GetMatrixPointer();
  float* b_pointer=b->GetMatrixPointer();
  float* c_pointer=c->GetMatrixPointer();

  float* C=&c_pointer[0];

  for(int col = 0; col < s3[1]; col++)
    {
      for(int row = 0; row < s3[0]; row++)
	{
	  float* B=&b_pointer[col*s2[0]];
	  float* A=&a_pointer[row];
	  *C=0;
	  for(int i = 0; i < s1[1]; i++)
	    {
	      (*C)+=(*A)*(*B);
	      ++B;
	      A+=s1[0];
	    }
	  ++C;
	}
    }
}
// -----------------------------------------------------------------------------------
void vtkpxMatrix::MultiplyTripleProduct(vtkpxMatrix* a,vtkpxMatrix* b,vtkpxMatrix* c)
{
  int s1[2],s2[2],s3[2];
  a->GetSize(s1);
  b->GetSize(s2);

  if (s1[0]!=s2[0] || s2[0]!=s2[1])
    {
      fprintf(stderr,"Cannot triple-multiply matrices bad sizes a=%dx%d, b=%dx%d!\n",s1[0],s1[1],s2[0],s2[1]);
      return;
    }

  int n=s2[0];

  c->Allocate(s1[1],s1[1]);
  c->GetSize(s3);


  float* a_pointer=a->GetMatrixPointer();
  float* b_pointer=b->GetMatrixPointer();
  float* c_pointer=c->GetMatrixPointer();

  float* C =&c_pointer[0];

  for(int j = 0; j < s3[1]; j++)
    {
      for(int i = 0; i < s3[0]; i++)
	{
	  (*C)=0;
	  /*for(int k = 0; k < n ; k++)
	    for (int l=0; l <  n ; l++ )
	    (*C) = (*C) + a_pointer[i*s1[0]+k]*b_pointer[l*s2[0]+k]*a_pointer[j*s1[0]+l];*/
	  float *AT=&a_pointer[i*s1[0]];
	  for(int k = 0; k < n ; k++)
	    {
	      float* B =&b_pointer[k];
	      float* A =&a_pointer[j*s1[0]];
	      for (int l=0; l <  n ; l++ )
		{
		  (*C) = (*C) + (*AT)*(*B)*(*A);//a_pointer[j*s1[0]+l];
		  B+=s2[0];
		  ++A;
		}
	      ++AT;
	    }

	  ++C;
	}
    }
}

// -----------------------------------------------------------------------------------
void  vtkpxMatrix::Multiply3(vtkpxMatrix* a,vtkpxMatrix* b,vtkpxMatrix* c,vtkpxMatrix* result)
{
  int s1[2],s2[2],s3[2],s4[2];
  a->GetSize(s1);
  b->GetSize(s2);
  c->GetSize(s3);

  if (s1[1]!=s2[0] || s2[1]!=s3[0])
    {
      fprintf(stderr,"Cannot multiply3 matrices bad sizes a=%dx%d, b=%dx%d c=%dx%d!\n",s1[0],s1[1],s2[0],s2[1],s3[0],s3[1]);
      return;
    }

  result->Allocate(s1[0],s3[1]);
  result->GetSize(s4);

  float* a_pointer=a->GetMatrixPointer();
  float* b_pointer=b->GetMatrixPointer();
  float* c_pointer=c->GetMatrixPointer();

  float* R=result->GetMatrixPointer();

  for(int j = 0; j < s4[1]; j++)
    for(int i = 0; i < s4[0]; i++)
      {
	*R=0.0;
	float* A=&a_pointer[i];
	for(int k = 0; k < s1[1] ; k++)
	  {
	    float *B=&b_pointer[k];
	    float *C=&c_pointer[j*s3[0]];
	    for (int l=0; l <  s2[1] ; l++ )
	      {
		(*R)+=(*A)*(*B)*(*C);//c_pointer[j*s3[0]+l];
		B+=s2[0];
		++C;
	      }
	    A+=s1[0];
	  }
	R++;
      }
}
// -----------------------------------------------------------------------------------
float vtkpxMatrix::Max()
{
  if (Matrix==NULL)
    return 0.0;
  float max=this->Matrix[0];
  for (int i=0;i<this->TotalSize;i++)
    {
      float v=Matrix[i];
      if (max<v)
	max=v;
    }
  return max;
}
// -----------------------------------------------------------------------------------
float vtkpxMatrix::MaxColumn(int col)
{
  if (Matrix==NULL)
    return 0.0;

  if (col<0 || col>=Size[1])
    return 0.0;

  float max=GetDirectElement(0,col);
  for (int i=1;i<this->Size[0];i++)
    {
      float v=this->GetDirectElement(i,col);
      if (max<v)
	max=v;
    }
  return max;
}
// -----------------------------------------------------------------------------------
float vtkpxMatrix::MaxRow(int row)
{
  if (Matrix==NULL)
    return 0.0;

  if (row<0 || row>=Size[0])
    return 0.0;

  float max=GetDirectElement(row,0);
  for (int i=1;i<this->Size[1];i++)
    {
      float v=this->GetDirectElement(row,i);
      if (max<v)
	max=v;
    }
  return max;
}

// -----------------------------------------------------------------------------------
float vtkpxMatrix::Sum()
{
  if (Matrix==NULL)
    return 0.0;

  float sum=0.0;
  for (int i=0;i<this->TotalSize;i++)
    sum+=this->Matrix[i];
  return sum;
}
// -----------------------------------------------------------------------------------
float vtkpxMatrix::Median()
{
  if (Matrix==NULL)
    return 0.0;

  return vtkpxMath::GetMedian(this->TotalSize,this->Matrix);
}
// -----------------------------------------------------------------------------------
float vtkpxMatrix::SumSquares()
{
  if (Matrix==NULL)
    return 0.0;

  float sum=0.0;
  for (int i=0;i<this->TotalSize;i++)
    sum+=pow(this->Matrix[i],float(2.0));
  return sum;
}
// -----------------------------------------------------------------------------------
float vtkpxMatrix::SumMagnitude()
{
  if (Matrix==NULL)
    return 0.0;

  float sum=0.0;
  for (int i=0;i<this->TotalSize;i++)
    sum+=fabs(this->Matrix[i]);
  return sum;
}
// -----------------------------------------------------------------------------------
vtkpxMatrix* vtkpxMatrix::RowSums()
{
  if (Matrix==NULL)
    return NULL;

  vtkpxMatrix* temp=vtkpxMatrix::New();
  temp->Allocate(this->Size[0],1);

  for (int i=0;i<this->Size[0];i++)
    {
      float sum=0.0;
      for (int j=0;j<this->Size[1];j++)
	sum+=this->GetDirectElement(i,j);
      temp->SetDirectElement(i,0,sum);
    }
  return temp;
}
// -----------------------------------------------------------------------------------
vtkpxMatrix* vtkpxMatrix::ColumnSums()
{
  if (Matrix==NULL)
    return NULL;

  vtkpxMatrix* temp=vtkpxMatrix::New();
  temp->Allocate(1,this->Size[1]);

  for (int j=0;j<this->Size[1];j++)
    {
      float sum=0.0;
      for (int i=0;i<this->Size[0];i++)
	sum+=this->GetDirectElement(i,j);
      temp->SetDirectElement(0,j,sum);
    }
  return temp;
}

// -----------------------------------------------------------------------------------
//   Uses Lapack Stuff from here
// -----------------------------------------------------------------------------------
int vtkpxMatrix::Invert()
{
  if (Matrix==NULL)
    return 0;

  if (this->Size[0]!=this->Size[1])
    {
      vtkErrorMacro(<<"Cannot Invert non-square matrix");
      return 0;
    }

  if (this->Size[0]==1)
    {
      // This is stupid, 1x1 matrix but you never know !!
      float a=this->GetDirectElement(0,0);
      if (a!=0.0)
	this->SetDirectElement(0,0,1.0/a);
      return 1;
    }

  /*  =====================================================================
   *  SUBROUTINE DGETRF( M, N, A, LDA, IPIV, INFO )
   *
   *  DGETRF computes an LU factorization of a general M-by-N matrix A
   *  using partial pivoting with row interchanges.
   *
   *  The factorization has the form
   *     A = P * L * U
   *  where P is a permutation matrix, L is lower triangular with unit
   *  diagonal elements (lower trapezoidal if m > n), and U is upper
   *  triangular (upper trapezoidal if m < n).
   *
   *  M       (input) INTEGER
   *          The number of rows of the matrix A.  M >= 0.
   *
   *  N       (input) INTEGER
   *          The number of columns of the matrix A.  N >= 0.
   *
   *  A       (input/output) FLOAT PRECISION array, dimension (LDA,N)
   *          On entry, the M-by-N matrix to be factored.
   *          On exit, the factors L and U from the factorization
   *          A = P*L*U; the unit diagonal elements of L are not stored.
   *
   *  LDA     (input) INTEGER
   *          The leading dimension of the array A.  LDA >= max(1,M).
   *
   *  IPIV    (output) INTEGER array, dimension (min(M,N))
   *          The pivot indices; for 1 <= i <= min(M,N), row i of the
   *          matrix was interchanged with row IPIV(i).
   *
   *  INFO    (output) INTEGER
   *          = 0:  successful exit
   *          < 0:  if INFO = -i, the i-th argument had an illegal value
   *          > 0:  if INFO = i, U(i,i) is exactly zero. The factorization
   *                has been completed, but the factor U is exactly
   *                singular, and division by zero will occur if it is used
   *                to solve a system of equations.
   *
   *  ===================================================================== */

  integer M=this->Size[0];
  integer N=this->Size[1];

  // Copy Matrix (transposed) into Array A
  real* A=new real[M*N];

  float* mat=&Matrix[0];
  int i,j;
  for (j=0;j<N;j++)
    for (i=0;i<M;i++)
      {
	A[j*M+i]=*mat;//this->GetDirectElement(i,j);
	++mat;
      }

  integer LDA=M;

  integer K=this->Size[0];
  if (K > this->Size[1])
    K=this->Size[1];

  integer* IPIV=new integer[K];

  integer INFO =1;

  // Do LU Factorization First
  sgetrf_(&M, &N, &A[0], &LDA, &IPIV[0], &INFO);

  if (INFO!=0)
    {
      vtkErrorMacro(<<"Error INFO="<<INFO);
      return 0;
    }

  /*  =====================================================================
   *  SUBROUTINE DGETRI( N, A, LDA, IPIV, WORK, LWORK, INFO )
   *
   *  DGETRI computes the inverse of a matrix using the LU factorization
   *  computed by DGETRF.
   *
   *  This method inverts U and then computes inv(A) by solving the system
   *  inv(A)*L = inv(U) for inv(A).
   *
   *  N       (input) INTEGER
   *          The order of the matrix A.  N >= 0.
   *
   *  A       (input/output) FLOAT PRECISION array, dimension (LDA,N)
   *          On entry, the factors L and U from the factorization
   *          A = P*L*U as computed by DGETRF.
   *          On exit, if INFO = 0, the inverse of the original matrix A.
   *
   *  LDA     (input) INTEGER
   *          The leading dimension of the array A.  LDA >= max(1,N).
   *
   *  IPIV    (input) INTEGER array, dimension (N)
   *          The pivot indices from DGETRF; for 1<=i<=N, row i of the
   *          matrix was interchanged with row IPIV(i).
   *
   *  WORK    (workspace/output) FLOAT PRECISION array, dimension (LWORK)
   *          On exit, if INFO=0, then WORK(1) returns the optimal LWORK.
   *
   *  LWORK   (input) INTEGER
   *          The dimension of the array WORK.  LWORK >= max(1,N).
   *          For optimal performance LWORK >= N*NB, where NB is
   *          the optimal blocksize returned by ILAENV.
   *
   *          If LWORK = -1, then a workspace query is assumed; the routine
   *          only calculates the optimal size of the WORK array, returns
   *          this value as the first entry of the WORK array, and no error
   *          message related to LWORK is issued by XERBLA.
   *
   *  INFO    (output) INTEGER
   *          = 0:  successful exit
   *          < 0:  if INFO = -i, the i-th argument had an illegal value
   *          > 0:  if INFO = i, U(i,i) is exactly zero; the matrix is
   *                singular and its inverse could not be computed.
   *
   *  ===================================================================== */

  integer LWORK=10*N;
  real* WORK=new real[LWORK];

  sgetri_( &N , &A[0] , &LDA, &IPIV[0], &WORK[0], &LWORK, &INFO);

  vtkDebugMacro(<<"Work [0]="<<WORK[0]);

  if (INFO!=0)
    {
      vtkErrorMacro(<<"Error INFO="<<(int)INFO);
      return 0;
    }

  mat=&Matrix[0];
  for (j=0;j<N;j++)
    for (i=0;i<N;i++)
      {
	*mat=A[j*M+i];
	++mat;
      }


  delete [] WORK;
  delete [] IPIV;
  delete [] A;

  return 1;

}
// -----------------------------------------------------------------------------------
int vtkpxMatrix::QRDecomposition(vtkpxMatrix* Q,vtkpxMatrix* R)
{
  if (Matrix==NULL)
    return 0;

  /*  =====================================================================
      int sgeqrf_(integer *m, integer *n, real *a, integer *
                  lda, real *tau, real *work, integer *lwork, integer *info)

		  -- LAPACK routine (version 2.0) --
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
       Courant Institute, Argonne National Lab, and Rice University
       September 30, 1994


    Purpose
    =======

    DGEQRF computes a QR factorization of a real M-by-N matrix A:
    A = Q * R.

    Arguments
    =========

    M       (input) INTEGER
            The number of rows of the matrix A.  M >= 0.

    N       (input) INTEGER
            The number of columns of the matrix A.  N >= 0.

    A       (input/output) FLOAT PRECISION array, dimension (LDA,N)
            On entry, the M-by-N matrix A.
            On exit, the elements on and above the diagonal of the array

            contain the min(M,N)-by-N upper trapezoidal matrix R (R is
            upper triangular if m >= n); the elements below the diagonal,

            with the array TAU, represent the orthogonal matrix Q as a
            product of min(m,n) elementary reflectors (see Further
            Details).

    LDA     (input) INTEGER
            The leading dimension of the array A.  LDA >= max(1,M).

    TAU     (output) FLOAT PRECISION array, dimension (min(M,N))
            The scalar factors of the elementary reflectors (see Further

            Details).

    WORK    (workspace/output) FLOAT PRECISION array, dimension (LWORK)

            On exit, if INFO = 0, WORK(1) returns the optimal LWORK.

    LWORK   (input) INTEGER
            The dimension of the array WORK.  LWORK >= max(1,N).
            For optimum performance LWORK >= N*NB, where NB is
            the optimal blocksize.

    INFO    (output) INTEGER
            = 0:  successful exit
            < 0:  if INFO = -i, the i-th argument had an illegal value

    Further Details
    ===============

    The matrix Q is represented as a product of elementary reflectors

       Q = H(1) H(2) . . . H(k), where k = min(m,n).

    Each H(i) has the form

       H(i) = I - tau * v * v'

    where tau is a real scalar, and v is a real vector with
    v(1:i-1) = 0 and v(i) = 1; v(i+1:m) is stored on exit in A(i+1:m,i),

    and tau in TAU(i).

    ===================================================================== */

  int i,j;

  integer M=this->Size[0];
  integer N=this->Size[1];


  // Copy Matrix (transposed) into Array A

  integer MK=Imax(M,N);
  integer K=Imin(M,N);
  integer MK2=MK*MK;

  integer LDA=MK;

  real* A=new real[MK2];
  // To get the full QR ( as opposed to the economy QR A needs to be made square and padded with zeros!!!)

  for (i=0;i<MK2;i++)
    A[i]=0.0;

  for (i=0;i<N;i++)
    for (j=0;j<M;j++)
      A[i*MK+j]=this->GetDirectElement(j,i);


  real* TAU=new real[MK];

  integer LWORK=MK*10;
  real* WORK=new real[LWORK];

  integer INFO=0;

  sgeqrf_(&MK, &MK, &A[0], &LDA, &TAU[0], &WORK[0], &LWORK, &INFO);
  vtkDebugMacro(<<"Work [0]="<<WORK[0]);

  if (INFO!=0)
    {
      vtkErrorMacro(<<"Error INFO="<<(int)INFO);
      return 0;
    }

  // Copy R Matrix Out First
  R->Zeros(MK,N);

  for (i=0;i<K;i++)
    for (j=i;j<N;j++)
      R->SetDirectElement(i,j,A[j*MK+i]);

  // Next Get Q and then we are out of here
  /*  =====================================================================
      int SORGQR_(integer *m, integer *n, integer *k, real *
                 a, integer *lda, real *tau, real *work, integer *lwork,
                 integer *info)


      -- LAPACK routine (version 2.0) --
      Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
      Courant Institute, Argonne National Lab, and Rice University
      September 30, 1994


    Purpose
    =======

    DORGQR generates an M-by-N real matrix Q with orthonormal columns,
    which is defined as the first N columns of a product of K elementary

    reflectors of order M

          Q  =  H(1) H(2) . . . H(k)

    as returned by DGEQRF.

    Arguments
    =========

    M       (input) INTEGER
            The number of rows of the matrix Q. M >= 0.

    N       (input) INTEGER
            The number of columns of the matrix Q. M >= N >= 0.

    K       (input) INTEGER
            The number of elementary reflectors whose product defines the

            matrix Q. N >= K >= 0.

    A       (input/output) FLOAT PRECISION array, dimension (LDA,N)
            On entry, the i-th column must contain the vector which
            defines the elementary reflector H(i), for i = 1,2,...,k, as

            returned by DGEQRF in the first k columns of its array
            argument A.
            On exit, the M-by-N matrix Q.

    LDA     (input) INTEGER
            The first dimension of the array A. LDA >= max(1,M).

    TAU     (input) FLOAT PRECISION array, dimension (K)
            TAU(i) must contain the scalar factor of the elementary
            reflector H(i), as returned by DGEQRF.

    WORK    (workspace/output) FLOAT PRECISION array, dimension (LWORK)

            On exit, if INFO = 0, WORK(1) returns the optimal LWORK.

    LWORK   (input) INTEGER
            The dimension of the array WORK. LWORK >= max(1,N).
            For optimum performance LWORK >= N*NB, where NB is the
            optimal blocksize.

    INFO    (output) INTEGER
            = 0:  successful exit
            < 0:  if INFO = -i, the i-th argument has an illegal value

   *  ===================================================================== */

  sorgqr_( &MK, &MK, &MK, A, &LDA, TAU, WORK, &LWORK, &INFO );

  if (INFO!=0)
    {
      vtkErrorMacro(<<"Error INFO="<<INFO);
      return 0;
    }

  // Copy Q Matrix
  Q->Zeros(MK,MK);

  for (i=0;i<MK;i++)
    for (j=0;j<MK;j++)
      Q->SetDirectElement(i,j,A[j*MK+i]);

  delete [] WORK;
  delete [] TAU;
  delete [] A;

  return 1;
}

// -----------------------------------------------------------------------------------
int vtkpxMatrix::Q1Q2RDecomposition(vtkpxMatrix* Q1,vtkpxMatrix* Q2,vtkpxMatrix* R)
{
  if (Matrix==NULL)
    return 0;

  int i,j;

  integer M=this->Size[0];
  integer N=this->Size[1];

  if (N>M)
    {
      fprintf(stderr,"More columns than rows, no Q1Q2R possible\n");
      return 0;
    }


  // Copy Matrix (transposed) into Array A
  integer MK=Imax(M,N);
  integer K=Imin(M,N);
  integer MK2=MK*MK;
  integer LDA=MK;

  real* A=new real[MK2];
  // To get the full QR ( as opposed to the economy QR A needs to be made square and padded with zeros!!!)

  for (i=0;i<MK2;i++)
    A[i]=0.0;

  for (i=0;i<N;i++)
    for (j=0;j<M;j++)
      A[i*MK+j]=this->GetDirectElement(j,i);


  real* TAU=new real[MK];

  integer LWORK=MK*10;
  real* WORK=new real[LWORK];

  integer INFO=0;

  sgeqrf_(&MK, &MK, &A[0], &LDA, &TAU[0], &WORK[0], &LWORK, &INFO);
  vtkDebugMacro(<<"Work [0]="<<WORK[0]);

  if (INFO!=0)
    {
      vtkErrorMacro(<<"Error INFO="<<(int)INFO);
      return 0;
    }

  // Copy R Matrix Out First
  R->Zeros(K,N);

  for (i=0;i<K;i++)
    for (j=i;j<N;j++)
      R->SetDirectElement(i,j,A[j*MK+i]);

  // Next Get Q and then we are out of here
  sorgqr_( &MK, &MK, &MK, A, &LDA, TAU, WORK, &LWORK, &INFO );

  if (INFO!=0)
    {
      vtkErrorMacro(<<"Error INFO="<<INFO);
      return 0;
    }

  // Copy Q Matrix
  Q1->Zeros(M,N);

  for (i=0;i<M;i++)
    for (j=0;j<N;j++)
      Q1->SetDirectElement(i,j,A[j*MK+i]);

  Q2->Zeros(M,M-N);
  for (i=0;i<M;i++)
    for (j=N;j<M;j++)
      Q2->SetDirectElement(i,j-N,A[j*MK+i]);

  delete [] WORK;
  delete [] TAU;
  delete [] A;

  return 1;
}
// -----------------------------------------------------------------------------------
int vtkpxMatrix::SolveWeightedLeastSquares(vtkpxMatrix* W,vtkpxMatrix* B,vtkpxMatrix* X)
{
  if (this->Matrix==NULL || W==NULL || B==NULL || X==NULL)
    {
      vtkErrorMacro(<<"Invalid Inputs");
      return 0;
    }

  int w_size[2],b_size[2],x_size[2];

  W->GetSize(w_size);
  B->GetSize(b_size);
  X->GetSize(x_size);

  if (w_size[0] != w_size[1] || w_size[1] != this->Size[0])
    {
      vtkErrorMacro(<<"Bad Size for Matrix W");
      return 0;
    }

  if (w_size[1] != b_size[0])
    {
      vtkErrorMacro(<<"Bad Size for Vector B");
      return 0;
    }

  if (x_size[0] != this->Size[1])
    {
      vtkErrorMacro(<<"Bad Size for Vector X");
      return 0;
    }

  vtkpxMatrix* WA=vtkpxMatrix::New();
  vtkpxMatrix::Multiply(W,this,WA);

  vtkpxMatrix* WB=vtkpxMatrix::New();
  vtkpxMatrix::Multiply(W,B,WB);

  int ok=WA->SolveLeastSquares(WB,X);

  WA->Delete();
  WB->Delete();

  return ok;
}
// -----------------------------------------------------------------------------------
int vtkpxMatrix::SolveLeastSquares(vtkpxMatrix* Borig,vtkpxMatrix* X)
{

  if (this->Matrix==NULL ||  Borig==NULL || X==NULL)
    {
      vtkErrorMacro(<<"Invalid Inputs");
      return 0;
    }


  /*
      SUBROUTINE SGELS( TRANS, M, N, NRHS, A, LDA, B, LDB, WORK, LWORK,
     $                  INFO )
*
*  -- LAPACK driver routine (version 3.0) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 30, 1999

*  Purpose
*  =======
*
*  SGELS solves overdetermined or underdetermined real linear systems
*  involving an M-by-N matrix A, or its transpose, using a QR or LQ
*  factorization of A.  It is assumed that A has full rank.
*
*  The following options are provided:
*
*  1. If TRANS = 'N' and m >= n:  find the least squares solution of
*     an overdetermined system, i.e., solve the least squares problem
*                  minimize || B - A*X ||.
*
*  2. If TRANS = 'N' and m < n:  find the minimum norm solution of
*     an underdetermined system A * X = B.
*
*  3. If TRANS = 'T' and m >= n:  find the minimum norm solution of
*     an undetermined system A**T * X = B.
*
*  4. If TRANS = 'T' and m < n:  find the least squares solution of
*     an overdetermined system, i.e., solve the least squares problem
*                  minimize || B - A**T * X ||.
*
*  Several right hand side vectors b and solution vectors x can be
*  handled in a single call; they are stored as the columns of the
*  M-by-NRHS right hand side matrix B and the N-by-NRHS solution
*  matrix X.
*
*  Arguments
*  =========
*
*  TRANS   (input) CHARACTER
*          = 'N': the linear system involves A;
*          = 'T': the linear system involves A**T.
*
*  M       (input) INTEGER
*          The number of rows of the matrix A.  M >= 0.
*
*  N       (input) INTEGER
*          The number of columns of the matrix A.  N >= 0.
*
*  NRHS    (input) INTEGER
*          The number of right hand sides, i.e., the number of
*          columns of the matrices B and X. NRHS >=0.
*
*  A       (input/output) REAL array, dimension (LDA,N)
*          On entry, the M-by-N matrix A.
*          On exit,
*            if M >= N, A is overwritten by details of its QR
*                       factorization as returned by SGEQRF;
*            if M <  N, A is overwritten by details of its LQ
*                       factorization as returned by SGELQF.
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A.  LDA >= max(1,M).
*
*  B       (input/output) REAL array, dimension (LDB,NRHS)
*          On entry, the matrix B of right hand side vectors, stored
*          columnwise; B is M-by-NRHS if TRANS = 'N', or N-by-NRHS
*          if TRANS = 'T'.
*          On exit, B is overwritten by the solution vectors, stored
*          columnwise:
*          if TRANS = 'N' and m >= n, rows 1 to n of B contain the least
*          squares solution vectors; the residual sum of squares for the
*          solution in each column is given by the sum of squares of
*          elements N+1 to M in that column;
*          if TRANS = 'N' and m < n, rows 1 to N of B contain the
*          minimum norm solution vectors;
*          if TRANS = 'T' and m >= n, rows 1 to M of B contain the
*          minimum norm solution vectors;
*          if TRANS = 'T' and m < n, rows 1 to M of B contain the
*          least squares solution vectors; the residual sum of squares
*          for the solution in each column is given by the sum of
*          squares of elements M+1 to N in that column.
*
*  LDB     (input) INTEGER
*          The leading dimension of the array B. LDB >= MAX(1,M,N).
*
*  WORK    (workspace/output) REAL array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK.
*          LWORK >= max( 1, MN + max( MN, NRHS ) ).
*          For optimal performance,
*          LWORK >= max( 1, MN + max( MN, NRHS )*NB ).
*          where MN = min(M,N) and NB is the optimum block size.
*
*          If LWORK = -1, then a workspace query is assumed; the routine
*          only calculates the optimal size of the WORK array, returns
*          this value as the first entry of the WORK array, and no error
*          message related to LWORK is issued by XERBLA.
*
*  INFO    (output) INTEGER
*          = 0:  successful exit
*          < 0:  if INFO = -i, the i-th argument had an illegal value
*
*  =====================================================================
*/


  // First Matrix A = M*N
  // --------------------
  integer M=this->Size[0];
  integer N=this->Size[1];

  // Copy Matrix (transposed) into Array A
  real* A=new real[M*N];
  float* mat=&Matrix[0];
  int i,j;
  for (j=0;j<N;j++)
    for (i=0;i<M;i++)
      {
	A[j*M+i]=*mat;//this->GetDirectElement(i,j);
	++mat;
      }

  integer LDA=M;

  // Then Matrix B = M*NRHS
  // ----------------------
  int b_size[2];
  Borig->GetSize(b_size);
  integer NRHS=b_size[1];

  // Copy Matrix (transposed) into Array A
  real* B=new real[M*NRHS];
  float* matB=Borig->GetMatrixPointer();
  for (j=0;j<NRHS;j++)
    for (i=0;i<M;i++)
      {
	B[j*M+i]=*matB;//this->GetDirectElement(i,j);
	++matB;
      }

  integer LDB=M;


  // Then Other Stuff

  integer MX=M;
  if (NRHS>M)
    MX=NRHS;

  integer LWORK=N+3*MX;
  real* WORK=new real[3*M];

  integer INFO=-1;

  char line[2];
  strcpy(line,"N");
  sgels_(&line[0],&M,&N,&NRHS,A,&LDA,B,&LDB,
	 WORK,&LWORK,&INFO);

  if (INFO==0)
    {
      int xsize[2];
      X->GetSize(xsize);

      for (i=0;i<xsize[0];i++)
	for (j=0;j<xsize[1];j++)
	  X->SetElement(i,j,B[j*M+i]);
    }
  else
    {
      vtkErrorMacro(<<"Error in Solving Least Squares Problem!");
      INFO=1;
    }

  delete [] A;
  delete [] B;
  delete [] WORK;

  return (1-INFO);
}
// -----------------------------------------------------------------------------------
int vtkpxMatrix::SolveLinearSystem(vtkpxMatrix* B,vtkpxMatrix* X)
{
  return this->InternalSolveLinearSystem(B,X,0);
}

int vtkpxMatrix::SolveSymmetricLinearSystem(vtkpxMatrix* B,vtkpxMatrix* X)
{
  return this->InternalSolveLinearSystem(B,X,1);
}

int vtkpxMatrix::SolveUpperSymmetricLinearSystem(vtkpxMatrix* B,vtkpxMatrix* X)
{
  return this->InternalSolveLinearSystem(B,X,1);
}

int vtkpxMatrix::SolveLowerSymmetricLinearSystem(vtkpxMatrix* B,vtkpxMatrix* X)
{
  return this->InternalSolveLinearSystem(B,X,-1);
}

int vtkpxMatrix::InternalSolveLinearSystem(vtkpxMatrix* Borig,vtkpxMatrix* X,int symmetric)
{
  if (this->Matrix==NULL ||  Borig==NULL || X==NULL)
    {
      vtkErrorMacro(<<"Invalid Inputs");
      return 0;
    }

  integer M=this->Size[0];
  integer N=this->Size[1];

  // Copy Matrix (transposed) into Array A
  real* A=new real[M*N];
  float* mat=&Matrix[0];
  int i,j;
  for (j=0;j<N;j++)
    for (i=0;i<M;i++)
      {
	A[j*M+i]=*mat;//this->GetDirectElement(i,j);
	++mat;
      }
  integer LDA=M;

  int b_size[2];
  Borig->GetSize(b_size);
  integer NRHS=b_size[1];

  // Copy Matrix (transposed) into Array A
  real* B=new real[M*NRHS];
  float* matB=Borig->GetMatrixPointer();
  for (j=0;j<NRHS;j++)
    for (i=0;i<M;i++)
      {
	B[j*M+i]=*matB;//this->GetDirectElement(i,j);
	++matB;
      }
  integer LDB=M;
  integer INFO=0;

  if (symmetric==0)
    {
      /*      SUBROUTINE SGESV( N, NRHS, A, LDA, IPIV, B, LDB, INFO )
       *
       *  -- LAPACK driver routine (version 3.0) --
       *     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
       *     Courant Institute, Argonne National Lab, and Rice University
       *     March 31, 1993
       *
       *
       *  Purpose
       *  =======
       *
       *  SGESV computes the solution to a real system of linear equations
       *     A * X = B,
       *  where A is an N-by-N matrix and X and B are N-by-NRHS matrices.
       *
       *  The LU decomposition with partial pivoting and row interchanges is
       *  used to factor A as
       *     A = P * L * U,
       *  where P is a permutation matrix, L is unit lower triangular, and U is
       *  upper triangular.  The factored form of A is then used to solve the
       *  system of equations A * X = B.
       *
       *  Arguments
       *  =========
       *
       *  N       (input) INTEGER
       *          The number of linear equations, i.e., the order of the
       *          matrix A.  N >= 0.
       *
       *  NRHS    (input) INTEGER
       *          The number of right hand sides, i.e., the number of columns
       *          of the matrix B.  NRHS >= 0.
       *
       *  A       (input/output) REAL array, dimension (LDA,N)
       *          On entry, the N-by-N coefficient matrix A.
       *          On exit, the factors L and U from the factorization
       *          A = P*L*U; the unit diagonal elements of L are not stored.
       *
       *  LDA     (input) INTEGER
       *          The leading dimension of the array A.  LDA >= max(1,N).
       *
       *  IPIV    (output) INTEGER array, dimension (N)
       *          The pivot indices that define the permutation matrix P;
       *          row i of the matrix was interchanged with row IPIV(i).
       *
       *  B       (input/output) REAL array, dimension (LDB,NRHS)
       *          On entry, the N-by-NRHS matrix of right hand side matrix B.
       *          On exit, if INFO = 0, the N-by-NRHS solution matrix X.
       *
       *  LDB     (input) INTEGER
       *          The leading dimension of the array B.  LDB >= max(1,N).
       *
       *  INFO    (output) INTEGER
       *          = 0:  successful exit
       *          < 0:  if INFO = -i, the i-th argument had an illegal value
       *          > 0:  if INFO = i, U(i,i) is exactly zero.  The factorization
       *                has been completed, but the factor U is exactly
       *                singular, so the solution could not be computed.
       *
       *  =====================================================================*/
      integer* IPIV=new integer[N];
      for (int i=0;i<N;i++)
	IPIV[i]=i+1;

      sgesv_(&N,&NRHS,A,&LDA,IPIV,B,&LDB,&INFO);

      delete [] IPIV;
    } else {

      /* SUBROUTINE SPOSV( UPLO, N, NRHS, A, LDA, B, LDB, INFO )
       *
       *  -- LAPACK driver routine (version 3.0) --
       *     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
       *     Courant Institute, Argonne National Lab, and Rice University
       *     March 31, 1993
       *
       *
       *  Purpose
       *  =======
       *
       *  SPOSV computes the solution to a real system of linear equations
       *     A * X = B,
       *  where A is an N-by-N symmetric positive definite matrix and X and B
       *  are N-by-NRHS matrices.
       *
       *  The Cholesky decomposition is used to factor A as
       *     A = U**T* U,  if UPLO = 'U', or
       *     A = L * L**T,  if UPLO = 'L',
       *  where U is an upper triangular matrix and L is a lower triangular
       *  matrix.  The factored form of A is then used to solve the system of
       *  equations A * X = B.
       *
       *  Arguments
       *  =========
       *
       *  UPLO    (input) CHARACTER*1
       *          = 'U':  Upper triangle of A is stored;
       *          = 'L':  Lower triangle of A is stored.
       *
       *  N       (input) INTEGER
       *          The number of linear equations, i.e., the order of the
       *          matrix A.  N >= 0.
       *
       *  NRHS    (input) INTEGER
       *          The number of right hand sides, i.e., the number of columns
       *          of the matrix B.  NRHS >= 0.
       *
       *  A       (input/output) REAL array, dimension (LDA,N)
       *          On entry, the symmetric matrix A.  If UPLO = 'U', the leading
       *          N-by-N upper triangular part of A contains the upper
       *          triangular part of the matrix A, and the strictly lower
       *          triangular part of A is not referenced.  If UPLO = 'L', the
       *          leading N-by-N lower triangular part of A contains the lower
       *          triangular part of the matrix A, and the strictly upper
       *          triangular part of A is not referenced.
       *
       *          On exit, if INFO = 0, the factor U or L from the Cholesky
       *          factorization A = U**T*U or A = L*L**T.
       *
       *  LDA     (input) INTEGER
       *          The leading dimension of the array A.  LDA >= max(1,N).
       *
       *  B       (input/output) REAL array, dimension (LDB,NRHS)
       *          On entry, the N-by-NRHS right hand side matrix B.
       *          On exit, if INFO = 0, the N-by-NRHS solution matrix X.
       *
       *  LDB     (input) INTEGER
       *          The leading dimension of the array B.  LDB >= max(1,N).
       *
       *  INFO    (output) INTEGER
       *          = 0:  successful exit
       *          < 0:  if INFO = -i, the i-th argument had an illegal value
       *          > 0:  if INFO = i, the leading minor of order i of A is not
       *                positive definite, so the factorization could not be
       *                completed, and the solution has not been computed.
       *
       *  =====================================================================*/

      char line[2];
      if (symmetric==1)
	strcpy(line,"U");
      else
	strcpy(line,"L");
      sposv_(&line[0],&N,&NRHS,A,&LDA,B,&LDB,&INFO);
    }


    if (INFO==0)
    {
      int xsize[2];
      X->GetSize(xsize);

      for (i=0;i<xsize[0];i++)
	for (j=0;j<xsize[1];j++)
	  X->SetElement(i,j,B[j*M+i]);
    }
  else
    {
      vtkErrorMacro(<<"Error in Solving Linear System Problem (symmetric="<<symmetric<<")!");
      INFO=1;
    }

  delete [] A;
  delete [] B;

  return (1-INFO);


}
//------------------------------------------------------------------------
int vtkpxMatrix::SOR(vtkpxMatrix* B,vtkpxMatrix* X,float thr,float omega)
{
  if (X==NULL || B==NULL)
    return 0;

  int b_size[2];   B->GetSize(b_size);

  if (this->Size[0] != this->Size[1])
    {
      vtkErrorMacro(<<"Bad Size for Solving Linear System (Not Square)");
      return 0;
    }

  if (this->Size[1] != b_size[0])
    {
      vtkErrorMacro(<<"Bad Size for Vector B");
      return 0;
    }

  int x_size[2]; X->GetSize(x_size);
  if (x_size[0] !=b_size[0] || x_size[1] !=b_size[1])
    X->Zeros(b_size[0],b_size[1]);

  const int MAXITER=100;
  int maxiter=MAXITER*this->Size[0];
  int totaliter=0;

  int ok=1;
  for (int i=0;i<this->Size[0];i++)
    {
      float v=this->GetDirectElement(i,i);
      if (fabs(v)<0.0001)
	{
	  ok=0;
	  i=this->Size[0];
	}
    }

  if (ok==0)
    {
      vtkErrorMacro(<<"Zero diagonal element ... bad SOR\n");
      return 0;
    }



  float *newx=new float[this->Size[0]];
  float *oldx=new float[this->Size[0]];

  float* bpoint=B->GetMatrixPointer();

  for (int master_column=0;master_column<b_size[1];master_column++)
    {
      int row,col,iter=0;

      for (int ia=0;ia<this->Size[0];ia++)
	{
	  oldx[ia]=X->GetDirectElement(ia,master_column);
	  newx[ia]=oldx[ia];
	}



      float res=thr+1.0;

      while ( res>thr && iter < maxiter)
	{
	  iter++;
	  for (row=0;row<this->Size[0];row++)
	    {
	      float r=0.0;

	      //     form gaus-seidel best estimate;
	      //     for elements before leading diagonal;
	      for (col=0;col<row;col++)
		r+=this->Matrix[col*Size[0]+row]*newx[col];

	      //  for elements after leading diagonal;

	      for (col=row+1;col<this->Size[0];col++)
		r+=this->Matrix[col*Size[0]+row]*oldx[col];

	      // form sor (gaus seidel omega=1);

	      newx[row]=omega*((bpoint[row+master_column*b_size[0]]-r)/this->Matrix[row*Size[0]+row]);
	      newx[row]=newx[row]+(1-omega)*oldx[row];
	    }

	  //     calculate ||x(k+1)-x(k)||(inf) moddif and ||x(k)||(inf)=modx;
	  //     also do oldx=x to prepare for next iteration;

	  float moddif=0.0,modx=0.0;

	  for (row=0;row<this->Size[0];row ++)
	    {
	      if (fabs(oldx[row]-newx[row]) > moddif)
		moddif=fabs(oldx[row]-newx[row]);
	      if (fabs(newx[row]) > modx)
		modx=fabs(newx[row]);
	      oldx[row]=newx[row];
	    }
	  res=moddif/modx;
	  fprintf(stderr,"Iteration %d/%d col=%d/%d thr=%.4f < %.4f\n",
		  iter,maxiter,master_column,b_size[1],res,thr);
	}

      totaliter+=iter;

      for (int ib=0;ib<this->Size[0];ib++)
	X->SetDirectElement(ib,master_column,oldx[ib]);
    }
  delete [] newx;
  delete [] oldx;
  return totaliter;
}

//------------------------------------------------------------------------
// Static Code which can be accessed directly
//------------------------------------------------------------------------
double** vtkpxMatrix::NewMatrix(int x, int y)
{
  double** m = new double*[x];
  for(int i = 0; i < x; i++)
    {
      m[i] = new double[y];
    }
  return m;
}

void vtkpxMatrix::DeleteMatrix(double** m, int x, int vtkNotUsed(y))
{
  for(int i = 0; i < x; i++)
    {
    delete [] m[i]; // OK, we don't actually need y
    }
  delete [] m;
}

void vtkpxMatrix::FillMatrixWithZeros(double** m, int x, int y)
{
  int i,j;
  for(i = 0; i < x; i++)
    {
    for(j = 0; j < y; j++)
      {
      m[i][j] = 0.0;
      }
    }
}

void vtkpxMatrix::MatrixMultiply(double** a, double** b, double** c,
					     int ar, int ac, int br, int bc)
{
  if(ac != br)
    {
    return;	// ac must equal br otherwise we can't proceed
    }

  // c must have size ar*bc (we assume this)
  const int cr = ar;
  const int cc = bc;
  int row,col,i;
  for(row = 0; row < cr; row++)
    {
    for(col = 0; col < cc; col++)
      {
      c[row][col] = 0.0;
      for(i = 0; i < ac; i++)
        {
        c[row][col] += a[row][i]*b[i][col];
        }
      }
    }
}

void vtkpxMatrix::PrintMatrix(double **m, int x, int y)
{
  int i,j;
  for(i = 0; i < x; i++)
    {
    for(j = 0; j < y; j++)
      {
      cerr << m[i][j] << ((j != y-1) ? " " : "\n");
      }
    }
}
void vtkpxMatrix::InPlaceTransposeMatrix(double** m,int x, int y)
{
  for (int i=0;i<x;i++)
    for (int j=0;j<y;j++)
      {
	double a=m[i][j];
	m[i][j]=m[j][i];
	m[j][i]=a;
      }
}

double** vtkpxMatrix::TransposeMatrix(double** src,int x ,int y)
{
  double** dest=vtkpxMatrix::NewMatrix(x,y);
  for (int i=0;i<x;i++)
    for (int j=0;j<y;j++)
      dest[i][j]=src[j][i];
  return dest;
}

void vtkpxMatrix::CopyMatrix(double** dest,double** src,int x ,int y)
{
  for (int i=0;i<x;i++)
    for (int j=0;j<y;j++)
      dest[i][j]=src[i][j];
}

double** vtkpxMatrix::CopyMatrix(double** src,int x ,int y)
{
  double** dest=vtkpxMatrix::NewMatrix(x,y);
  for (int i=0;i<x;i++)
    for (int j=0;j<y;j++)
      dest[i][j]=src[i][j];
  return dest;
}

double** vtkpxMatrix::SubMatrix(double** src,int x1,int x2,int y1,int y2)
{
  int x=x2-x1+1;
  int y=y2-y1+1;
  double** dest=vtkpxMatrix::NewMatrix(x,y);
  for (int i=0;i<x;i++)
    for (int j=0;j<y;j++)
      dest[i][j]=src[i+x1][j+y1];
  return dest;
}

//------------------------------------------------------------------------
// RPM Code
//------------------------------------------------------------------------
void vtkpxMatrix::MapPoints(vtkpxMatrix* Y,vtkpxMatrix* VY)
{
  int size[2];
  this->GetSize(size);

  if (Y->GetSize()[0] != size[1] || Y->GetSize()[1]!=4)
    {
      vtkErrorMacro(<<"Bad Matrices to Map Points");
      return;
    }

  if (VY->GetSize()[0]!=size[0] || VY->GetSize()[1]!=4)
    VY->Zeros(size[0],4);

  if (this->Debug)
    fprintf(stderr,"Mapping Points %dx%d ...",size[0],size[1]);

  for (int row=0;row<size[0];row++)
    {
      float sum=0.0;
      for (int column=0;column<size[1];column++)
	sum+=this->GetDirectElement(row,column);

      if (sum<0.0001)
	sum=0.0001;

      for (int comp=0;comp<3;comp++)
	{
	  float value=0.0;
	  for (int column=0;column<size[1];column++)
	    value+=this->GetDirectElement(row,column)*Y->GetDirectElement(column,comp);
	  VY->SetDirectElement(row,comp,value/sum);
	}
      VY->SetDirectElement(row,3,1.0);
    }
  if (this->Debug)
    fprintf(stderr,"done\n");
}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - -
void vtkpxMatrix::NormalizeMixtureMatrix(vtkpxMatrix* M_Outliers_Row)
{
  int size[2];
  this->GetSize(size);
  if (this->Debug)
    fprintf(stderr,"Normalizing Distance Matrix %dx%d ...",size[0],size[1]);
  int row,column;

  // Normalize Accross Outlier Row
  for (column=0;column<size[1];column++)
    {
      float sum=0.0;
      for (row=0;row<size[0];row++)
	sum+=this->GetDirectElement(row,column);
      sum+=M_Outliers_Row->GetElement(0,column);

      if (sum<0.0001)
	sum=0.0001;

      for (row=0;row<size[0];row++)
	this->SetDirectElement(row,column,this->GetDirectElement(row,column)/sum);
    }
  if (this->Debug)
    fprintf(stderr,"done\n");
}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - -
void vtkpxMatrix::NormalizeRPMMatrix(vtkpxMatrix* M_Outliers_Row,vtkpxMatrix* M_Outliers_Column)
{
  int size[2];
  this->GetSize(size);

  //float norm_threshold = 0.05;
  int   norm_maxit     = 10;

  int row,column;
  float sumx,sumy;

  int norm_it = 0;
  while (norm_it < norm_maxit)
    {
      // --- Row normalization --------------------------------------------
      for (row=0;row<size[0];row++)
	{
	  sumx=0.0;
	  for (column=0;column<size[1];column++)
	    sumx+=this->GetDirectElement(row,column);
	  sumx+=M_Outliers_Column->GetElement(row,0);

	  if (sumx<0.0001)
	    sumx=0.0001;

	  for (column=0;column<size[1];column++)
	    this->SetDirectElement(row,column,this->GetDirectElement(row,column)/sumx);
	  M_Outliers_Column->SetElement(row,0,M_Outliers_Column->GetElement(row,0)/sumx);
	}

      /*sx = sum(m')' + m_outlier_col;
	m  = m ./ (sx * ones(0,ymax));
	m_outlier_col = m_outlier_col ./sx;*/

      // --- Column normalization -----------------------------------------
      for (column=0;column<size[1];column++)
	{
	  sumy=0.0;
	  for (row=0;row<size[0];row++)
	    sumy+=this->GetDirectElement(row,column);
	  sumy+=M_Outliers_Row->GetElement(0,column);

	  if (sumy<0.0001)
	    sumy+=0.0001;

	  for (row=0;row<size[0];row++)
	    this->SetDirectElement(row,column,this->GetDirectElement(row,column)/sumy);
	  M_Outliers_Row->SetElement(0,column,M_Outliers_Row->GetElement(0,column)/sumy);
	}

      /*sy = sum(m) + m_outlier_row;
	m  = m ./ (ones(xmax,0)*sy);
	m_outlier_row = m_outlier_row ./sy;*/

      // time to quit?
      norm_it = norm_it + 1;
    }

}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - -
int vtkpxMatrix::Eigenvalues(vtkpxMatrix* D)
{
  return this->Diagonalize(D,NULL);
}
int vtkpxMatrix::Diagonalize(vtkpxMatrix* D,vtkpxMatrix* U)
{
  if (Matrix==NULL || D==NULL)
    return 0;

  if (this->Size[0]!=this->Size[1])
    {
      vtkErrorMacro(<<"Cannot Invert non-square matrix");
      return 0;
    }

  /*
    SUBROUTINE SSYEV( JOBZ, UPLO, N, A, LDA, W, WORK, LWORK, INFO )
    *
    *  -- LAPACK driver routine (version 3.0) --
    *     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
    *     Courant Institute, Argonne National Lab, and Rice University
    *     June 30, 1999
    *
    *     .. Scalar Arguments ..
    CHARACTER          JOBZ, UPLO
    INTEGER            INFO, LDA, LWORK, N
    *     ..
    *     .. Array Arguments ..
    REAL               A( LDA, * ), W( * ), WORK( * )
    *     ..
    *
    *  Purpose
    *  =======
    *
    *  SSYEV computes all eigenvalues and, optionally, eigenvectors of a
    *  real symmetric matrix A.
    *
    *  Arguments
    *  =========
    *
    *  JOBZ    (input) CHARACTER*1
    *          = 'N':  Compute eigenvalues only;
    *          = 'V':  Compute eigenvalues and eigenvectors.
    *
    *  UPLO    (input) CHARACTER*1
    *          = 'U':  Upper triangle of A is stored;
    *          = 'L':  Lower triangle of A is stored.
    *
    *  N       (input) INTEGER
    *          The order of the matrix A.  N >= 0.
    *
    *  A       (input/output) REAL array, dimension (LDA, N)
    *          On entry, the symmetric matrix A.  If UPLO = 'U', the
    *          leading N-by-N upper triangular part of A contains the
    *          upper triangular part of the matrix A.  If UPLO = 'L',
    *          the leading N-by-N lower triangular part of A contains
    *          the lower triangular part of the matrix A.
    *          On exit, if JOBZ = 'V', then if INFO = 0, A contains the
    *          orthonormal eigenvectors of the matrix A.
    *          If JOBZ = 'N', then on exit the lower triangle (if UPLO='L')
    *          or the upper triangle (if UPLO='U') of A, including the
    *          diagonal, is destroyed.
    *
    *  LDA     (input) INTEGER
    *          The leading dimension of the array A.  LDA >= max(1,N).
    *
    *  W       (output) REAL array, dimension (N)
    *          If INFO = 0, the eigenvalues in ascending order.
    *
    *  WORK    (workspace/output) REAL array, dimension (LWORK)
    *          On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
    *
    *  LWORK   (input) INTEGER
    *          The length of the array WORK.  LWORK >= max(1,3*N-1).
    *          For optimal efficiency, LWORK >= (NB+2)*N,
    *          where NB is the blocksize for SSYTRD returned by ILAENV.
    *
    *          If LWORK = -1, then a workspace query is assumed; the routine
    *          only calculates the optimal size of the WORK array, returns
    *          this value as the first entry of the WORK array, and no error
    *          message related to LWORK is issued by XERBLA.
    *
    *  INFO    (output) INTEGER
    *          = 0:  successful exit
    *          < 0:  if INFO = -i, the i-th argument had an illegal value
    *          > 0:  if INFO = i, the algorithm failed to converge; i
    *                off-diagonal elements of an intermediate tridiagonal
    *                form did not converge to zero.
    */


  integer N=this->Size[0];

  // Copy Matrix (transposed) into Array A
  real* A=new real[N*N];

  float* mat=&Matrix[0];
  int i,j;
  for (j=0;j<N;j++)
    for (i=0;i<N;i++)
      {
	A[j*N+i]=*mat;//this->GetDirectElement(i,j);
	++mat;
      }

  integer LDA=N;
  float *W=new real[N];

  integer LWORK=5*N;
  float *WORK=new float[LWORK];

  integer INFO =0;

  char md[2],mv[2];
  if (U!=NULL)
    strcpy(md,"V");
  else
    strcpy(md,"N");
  strcpy(mv,"U");

  // Call Eigenvalue routine
  ssyev_(&md[0],&mv[0],&N,&A[0],&LDA,&W[0],&WORK[0],&LWORK, &INFO);

  if (INFO!=0)
    {
      vtkErrorMacro(<<"Error INFO="<<INFO);
      return 0;
    }

  D->Zeros(N,1);
  for (i=0;i<N;i++)
    D->SetDirectElement((N-1)-i,0,W[i]);

  if (U!=NULL)
    {
      U->Zeros(N,N);

      for (j=0;j<N;j++)
	for (i=0;i<N;i++)
	  U->SetDirectElement(i,(N-1)-j,A[j*N+i]);
    }

  delete [] WORK;
  delete [] W;
  delete [] A;

  return 1;

}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - -
int vtkpxMatrix::BuildSmoothness(int ndim,int width,int height,int depth,float weight)
{
  int sz=width*height*depth;
  this->Eye(sz*ndim);

   int dim12=height*depth;
   for (int k=0;k<depth;k++)
     {
       int kp=Icyclic(k+1,depth);
       int km=Icyclic(k-1,depth);

       for (int j=0;j<height;j++)
	 {
	   int jp=Icyclic(j+1,height);
	   int jm=Icyclic(j-1,height);
	   for (int i=0;i<width;i++)
	     {
	       int ip=Icyclic(i+1,width);
	       int im=Icyclic(i-1,width);

	       int ind1=i+j*width+k*dim12,l=0,ind2=0;

	       if (width>1)
		 {
		   ind2=ip+j*width+k*dim12;
		   for (l=0;l<ndim;l++)
		     this->SetElement(ind1*ndim+l,ind2*ndim+l,0.5);

		   ind2=im+j*width+k*dim12;
		   for (l=0;l<ndim;l++)
		     this->SetElement(ind1*ndim+l,ind2*ndim+l,0.5);
		 }

	       if (height>1)
		 {
		   ind2=i+jp*width+k*dim12;
		   for (l=0;l<ndim;l++)
		     this->SetElement(ind1*ndim+l,ind2*ndim+l,0.5);

		   ind2=i+jm*width+k*dim12;
		   for (l=0;l<ndim;l++)
		     this->SetElement(ind1*ndim+l,ind2*ndim+l,0.5);
		 }

	       if (depth>1)
		 {
		   ind2=i+j*width+kp*dim12;
		   for (l=0;l<ndim;l++)
		     this->SetElement(ind1*ndim+l,ind2*ndim+l,0.5);

		   ind2=i+j*width+km*dim12;
		   for (l=0;l<ndim;l++)
		     this->SetElement(ind1*ndim+l,ind2*ndim+l,0.5);
		 }
	     }
	 }
     }
   return 1;
}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - -
int vtkpxMatrix::DoPCA(vtkpxMatrix* D,vtkpxMatrix* U,vtkpxMatrix* M)
{
  if (Matrix==NULL || D==NULL || U==NULL || M==NULL)
    return 0;

  if (this->Size[1]<2 || this->Size[0]<2)
    {
      vtkErrorMacro(<<"Cannot Do PCA matrix is too small");
      return 0;
    }

  // First Compute Mean Vector and subtract it from the picture
  M->Zeros(this->Size[0],1);
  float scalefactor=1.0/float(this->Size[1]);
  for (int ia=0;ia<this->Size[0];ia++)
    {
      float sum=0.0;
      for (int ib=0;ib<this->Size[1];ib++)
	sum+=this->GetDirectElement(ia,ib);
      sum*=scalefactor;
      M->SetDirectElement(ia,0,sum);
      sum=-1.0*sum;
      for (int ic=0;ic<this->Size[1];ic++)
	this->AddToElement(ia,ic,sum);
    }

  // Next Compute S=(1/n) D*D^t  where D=this and temp=D^t
  vtkpxMatrix* temp=vtkpxMatrix::New();
  vtkpxMatrix* S=vtkpxMatrix::New();
  temp->Copy(this);
  temp->Transpose();

  if (this->Size[1] < this->Size[2])
    vtkpxMatrix::Multiply(temp,this,S);
  else
    vtkpxMatrix::Multiply(this,temp,S);
  S->Scale(scalefactor);


  // Next Diagonalize S to get eigenvalues D and "eigenvectors" in temp
  if (this->Size[1] < this->Size[2])
    {
      S->Diagonalize(D,temp);
      // Next Generate real eigenvectors  U = D*temp
      vtkpxMatrix::Multiply(this,temp,U);
    }
  else
    {
      S->Diagonalize(D,U);
    }

  // Now normalize eigenvectors to have size 1 and scale eigenvalues appropriately
  for (int j=0;j<U->Size[1];j++)
    {
      float sum=0.0;
      for (int i=0;i<U->Size[0];i++)
	{
	  float v=U->GetDirectElement(i,j);
	  sum+=v*v;
	}
      float magn=sqrt(sum);

      if (magn>1e-4)
	{
	  for (int ib=0;ib<U->Size[0];ib++)
	    {
	      float v=U->GetDirectElement(ib,j);
	      U->SetDirectElement(ib,j,v/magn);
	    }
	}
      else
	{
	  D->SetElement(j,0,0.0);
	  for (int ib=0;ib<U->Size[0];ib++)
	    U->SetDirectElement(ib,j,0.0);

	}
    }

  S->Delete();
  temp->Delete();
  return 1;

}

// ----------------------------------------------------------------------------------------------------
int vtkpxMatrix::ExportToMatlab(const char* fname,const char* matrixname)
{
  vnl_matrix<float> M(this->Size[0],this->Size[1]);
  for (unsigned i=0; i<M.rows(); ++i)
    for (unsigned j=0; j<M.cols(); ++j)
      M(i,j) = this->GetDirectElement(i,j);

  //  vcl_cout << M << vcl_endl;
  //  vnl_matlab_print(vcl_cout, M, "M") << vcl_endl;

  vcl_ofstream f(fname);
  if (vnl_matlab_write(f, (float const * const *)M.data_array(), M.rows(), M.cols(), matrixname))
    {
      f.close();
      return 1;
    }
  return 0;
}
// ---------------------------------------------------------------------------------------
int vtkpxMatrix::ImportFromMatlab(const char* fname,const char* matrixname)
{
  return this->ImportFromMatlab2(fname,matrixname,0);
}
// ---------------------------------------------------------------------------------------
int vtkpxMatrix::ImportFromMatlab2(const char* fname,const char* matrixname,int debug)
{
  debug=(debug>0);
  vcl_ifstream f(fname);

  try
    {
      vnl_matlab_readhdr  pReader2(f);
      if (pReader2.is_complex())
	{
	  f.close();
	  	  if (debug)
	    std::cout << " header is complex, matlab v6 \n";
	  return this->ImportFromMatlabV6(fname,matrixname,debug);
	}
      f.close();

    }
  catch (std::bad_alloc)
    {
      std::cout << "Exception caught onto matlab v6\n";
      return this->ImportFromMatlabV6(fname,matrixname,debug);
    }

  int count=0;
  vnl_matlab_readhdr  pReader(f);
  while (count < 20 )
    {
      if (debug)
	{
	  std::cout << "\n Reading " << fname << "\n";
	  std::cout << "p.Name = " << pReader.name() << "Size=" << pReader.rows() << "x" <<  pReader.cols() << "\n";
	}

      if (vcl_strcmp(pReader.name(), matrixname)==0 || strlen(matrixname) == 0)
	{
	  if (debug)
	    std::cout << "Found matrix " << matrixname << "\n";
	  vnl_matrix<double> M(pReader.rows(), pReader.cols());
	  pReader.read_data(M.data_array());

	  this->Zeros(pReader.rows(), pReader.cols());
	  for (unsigned i=0; i<M.rows(); ++i)
	    for (unsigned j=0; j<M.cols(); ++j)
	      this->SetDirectElement(i,j,M(i,j));
	  f.close();
	  return 1;
	}

      ++count;
    }
  f.close();
  std::cerr << " matrix " << matrixname << "not found \n";
  return 0;
}
// ------------------------------------------------------------------------------------------------------
int vtkpxMatrix::ImportFromMatlabV6(const char* fname,const char* matrixname,int debug)
{
  //  debug=1;
  znzFile fp = znzopen(fname,"rb",1);
  char buffer[2000];
  char name[2000];

  znzread(buffer,1,116,fp);
  if (debug)
    std::cout << "Description = " << buffer << "\n";

  znzread(buffer,1,8,fp);
  int ok=1;
  for (int ia=0;ia<=7;ia++)
    {
      int v=int(buffer[ia]);
      if (!(v==0 || v==32))
	{
	  std::cerr << "We have a problem with ia=" << ia << "\n";
	  ok=0;
	  ia=9;
	}
    }

  if (ok==0)
    {
      znzclose(fp);
      return 0;
    }
  znzread(buffer,1,4,fp);
  if (debug)
    std::cout << "All Offset bytes are either zero or space OK!\n";


  int done=0;
  int totalread=128;
  int count=0;

  while (done==0)
    {
      vxl_int_32 dtype[2];
      int n=znzread(dtype,sizeof(vxl_int_32),2,fp);
      totalread+=8;
      if (debug)
	std::cout << "n = " << n << " totalread= " << totalread << "\n\n-----------------------------------------------------\n";
      if (n<1)
	{
	  done=1;
	}
      else
	{
	  int swap=0;
	  ++count;
	  if (dtype[0]<0 || dtype[0]>1024)
	    {
	      dtype[0]=pxanaswapint(dtype[0]);
	      dtype[1]=pxanaswapint(dtype[1]);
	      swap=1;
	    }
	  if (debug)
	    std::cout << "\n\n\n dtype = " << dtype[0] << " , numbytes = " << dtype[1] << "\n";
	  if (dtype[0]!=14)
	    {
	      if (debug)
		{
		  std::cout << "Not a matrix skipping ahead " << dtype[1] << " bytes\n";
		  std::cout << "Seaking ahead  " << dtype[1] << " bytes\n";
		}
	      int toread=dtype[1];


	      // Add padding
	      int tmp=8*int(toread/8);
	      if (tmp<toread)
		{
		  toread=8+tmp;
		  //		  std::cout  << "Adding padding from " << dtype[1] << " to " << toread << "\n";
		}

	      while (toread>0)
		{
		  int n=toread;
		  if (toread>256) n=256;
		  totalread+=znzread(buffer,1,n,fp);
		  toread-=n;
		}
	      if (debug)
		std::cout << "Total read = " << totalread << "\n";
	    }
	  else
	    {
	      if (debug)
		std::cout << "Beginning to read matrix: \n";
	      int bytes_read=znzread(buffer,1,16,fp);
	      int cl=(int)buffer[8];
	      if (debug)
		fprintf(stderr,"in cl=%d\n",cl);
	      //	      if (swap)
	      //		cl=pxanaswapint(cl);

	      vxl_int_32 flags[6],length[1];
	      bytes_read+=sizeof(vxl_int_32)*znzread(flags,sizeof(vxl_int_32),5,fp);
	      if (swap)
		{
		  length[0]=pxanaswapint(length[0]);
		  for (int ic=0;ic<5;ic++)
		    flags[ic]=pxanaswapint(flags[ic]);
		}

	      if (debug)
		{
		  fprintf(stderr,"Flags[0]=%d, flags[1]=%d\n",flags[0],flags[1]);
		  fprintf(stderr,"Flags[4]=%d, flags[5]=%d\n",flags[4],flags[5]);
		}

	      // Check for use of small element format
	      //
	      int test=int(flags[4] / 65536 );
	      if (debug)
		std::cout << "***************** Is Small Element " << test << "\n";
	      if (test==0)
		{
		  bytes_read+=sizeof(vxl_int_32)*znzread(length,sizeof(vxl_int_32),1,fp);
		  //		  if (swap)
		  //		    length[0]=pxanaswapint(length[0]);
		  flags[5]=length[0];
		  if (swap)
		    flags[5]=pxanaswapint(flags[5]);
		  bytes_read+=znzread(name,1,flags[5],fp);
		  name[flags[5]]=(char)0;
		  int rem=8-(flags[5]-int(flags[5]/8)*8);
		  if (debug)
		    std::cout << "Remainder " << rem << "\n";
		  bytes_read+=znzread(buffer,1,rem,fp);
		}
	      else
		{
		  int nb=(flags[4] && 65535);
		  if (debug)
		    std::cout << "Small Element nb=" << nb << " test= " << int(test/65536) << "\n";
		  bytes_read+=znzread(name,1,4,fp);
		  name[nb]=(char)0;
		  flags[5]=0;
		}

	      vxl_int_32 nfl[2];
	      bytes_read+=sizeof(vxl_int_32)*znzread(nfl,sizeof(vxl_int_32),2,fp);
	      if (swap)
		{
		  nfl[0]=pxanaswapint(nfl[0]);
		  nfl[1]=pxanaswapint(nfl[1]);
		}
	      if (debug)
		std::cout << "Final Flags = " << nfl[0] << "," << nfl[1] << "\n";


	      int numrows=flags[2];
	      int numcols=flags[3];
	      if (debug)
		{
		  std::cout << "Dimensions = " << numrows << "x" << numcols << " cl = " << cl << "length = " << flags[5] << "\n";
		  std::cout << "Name = " << name << "(bytes read= " << bytes_read << ")\n";
		}
	      int toread=dtype[1]-bytes_read;
	      if (debug)
		std::cout << "To read (1) = " << toread << "\n";
	      int canread=0;

	      if (vcl_strcmp(name, matrixname)==0 || strlen(matrixname) == 0)
		{
		  if ( (cl==7 || cl==6))
		    {
		      int numbytesneeded=4*numrows*numcols;
		      if (cl==6)
			numbytesneeded*=2;

		      if (debug)
			fprintf(stderr,"Need (%d) < (%d) to read matrix \n",numbytesneeded , toread);


		      if (numbytesneeded<=toread)
			{
			  double* rd=NULL;
			  float*  rf=NULL;

			  if (cl==6)
			    rd=new double[numrows];
			  else
			    rf=new float[numrows];


			  if (debug)
			    fprintf(stderr,"Beginning to read matrix (%d) < (%d) \n",numbytesneeded , toread);
			  this->Zeros(numrows,numcols);
			  for (int ib=0;ib<numcols;ib++)
			    {
			      if (cl==6)
				{
				  bytes_read+=sizeof(double)*znzread(rd,sizeof(double),numrows,fp);
				  for (int ia=0;ia<numrows;ia++)
				    if (swap)
				      this->SetDirectElement(ia,ib,pxanaswapdouble(rd[ia]));
				    else
				      this->SetDirectElement(ia,ib,rd[ia]);
				}
			      else
				{
				  bytes_read+=sizeof(float)*znzread(rf,sizeof(float),numrows,fp);
				  for (int ia=0;ia<numrows;ia++)
				    if (swap)
				      this->SetDirectElement(ia,ib,pxanaswapfloat(rf[ia]));
				    else
				      this->SetDirectElement(ia,ib,rf[ia]);
				}
			    }
			  znzclose(fp);
			  if (rd!=NULL) delete [] rd;
			  if (rf!=NULL) delete [] rf;
			  //			  this->Print(name);
			  return 1;
			  toread=dtype[1]-bytes_read;
			  if (debug)
			    std::cout << "To read part2 = " << toread << "(" << numbytesneeded << ")\n";
			}
		      else
			{
			  std::cerr << "\t\t\t can't read data not enough bytes\n";
			}
		    }
		  std::cout << "Final To read (2) = " << toread << "\n";
		}
	      else
		{
		  if (debug)
		    std::cout << "Not the matrix we are looking for\n";
		}

	      while (toread>0)
		{
		  int n=toread;
		  if (toread>256) n=256;
		  bytes_read+=znzread(buffer,1,n,fp);
		  toread-=n;
		}

	      totalread+=bytes_read;
	    }
	}
    }

  znzclose(fp);

  return 0;


}
// ------------------------------------------------------------------------
void vtkpxMatrix::NormalizeColumns(int begin,int end)
{
  int size[2];  this->GetSize(size);
  if (begin<0 || begin>=size[1])
    begin=0;
  if (end<0 || end >=size[1])
    end=size[1]-1;
  if (begin>end)
    {
      int tmp=end;
      end=begin;
      begin=end;
    }

  for (int col=begin;col<=end;col++)
    {
      double sum=0.0;
      for (int row=0;row<size[0];row++)
	sum+=pow(this->GetDirectElement(row,col),(float)2.0);
      sum=sqrt(sum);
      if (sum>0.0000001)
	{
	  for (int row=0;row<size[0];row++)
	    this->SetDirectElement(row,col,this->GetDirectElement(row,col)/sum);
	}
    }
}

