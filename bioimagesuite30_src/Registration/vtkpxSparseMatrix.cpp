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
  Module:    $RCSfile: vtkpxSparseMatrix.cpp,v $
  Language:  C++
  Date:      $Date: 2002/10/29 19:34:42 $
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

#include "vtkpxSparseMatrix.h"
#include "vtkObjectFactory.h"
#include "vtkpxUtil.h"
#include "pxutil.h"

// -----------------------------------------------------------------------------------

vtkpxSparseMatrix* vtkpxSparseMatrix::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxSparseMatrix");
  if(ret)
      {
	return (vtkpxSparseMatrix*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxSparseMatrix;
}

// -----------------------------------------------------------------------------------
vtkpxSparseMatrix::vtkpxSparseMatrix()
{
  this->Size[0]=0;
  this->Size[1]=0;
}
// -----------------------------------------------------------------------------------
vtkpxSparseMatrix::~vtkpxSparseMatrix()
{
  this->Matrix.cleanup();
}
// -----------------------------------------------------------------------------------
// Some in-line stuff 

void vtkpxSparseMatrix::Zeros(int rows,int cols) 
{
  Allocate(rows,cols); 
  Fill(0.0);
}
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::Ones(int rows,int cols)  
{ 
  Allocate(rows,cols); 
  Fill(1.0); 
}
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::Eye(int rowcol)
{
  Allocate(rowcol,rowcol) ; 
  Identity(); 
}
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::Allocate(int rows,int columns)
{
  this->Matrix.cleanup();
  this->Size[0]=rows;
  this->Size[1]=columns;

}
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::Identity()
{
  if (this->Size[0]!=this->Size[1])
    return;

  this->Zero();
  for (int i=0;i<this->Size[0];i++)
    this->SetDirectElement(i,i,1.0);
}
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::Zero()
{ 
  this->Matrix.cleanup();
}
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::Fill(float v)
{
  for (int i=0;i<this->Size[0];i++)
    for (int j=0;j<this->Size[1];j++)
      this->SetDirectElement(i,j,v);
}
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::Print(const char* message)
{
  fprintf(stdout,"\n %s =\n",message);
  this->Matrix.setindices(this->Size);
  this->Matrix.print(0);
  fprintf(stdout,"\n");
}

// -----------------------------------------------------------------------------------
float vtkpxSparseMatrix::Density()
{
  int total=this->Size[0]*this->Size[1];
  if (total<1)
    return 0.0;
  
  return float(this->Matrix.totalsize())/float(total);
}

// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::InternalPrint()
{
  if (this->Size[0]>0 && this->Size[1]>0)
    {
      this->Matrix.setindices(this->Size);
      this->Matrix.print(0);
    }
}

void vtkpxSparseMatrix::PrintBW()
{
  if (this->Size[0]>0 && this->Size[1]>0)
    {
      this->Matrix.setindices(this->Size);
      this->Matrix.printbw(0);
    }
}
// -----------------------------------------------------------------------------------
float vtkpxSparseMatrix::GetElement(int i,int j)
{
  if (this->Size[0]>0 && this->Size[1]>0)
    {
      i=Irange(i,0,this->Size[0]-1);
      j=Irange(j,0,this->Size[1]-1);
      return (this->Matrix)[i][j];
    }
  return 0.0;
}
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::SetElement(int i,int j,float v)
{
  if (this->Size[0]>0 && this->Size[1]>0)
    {
      i=Irange(i,0,this->Size[0]-1);
      j=Irange(j,0,this->Size[1]-1);
      (this->Matrix)[i][j]=v;
    }
}
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::AddToElement(int i,int j,float v)
{
  if (this->Size[0]>0 && this->Size[1]>0)
    {
      i=Irange(i,0,this->Size[0]-1);
      j=Irange(j,0,this->Size[1]-1);
      float v1=(this->Matrix)[i][j];
      (this->Matrix)[i][j]=v1+v;
    }
}
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::Add(vtkpxSparseMatrix* a,vtkpxSparseMatrix* b,vtkpxSparseMatrix* c)
{
  vtkpxSparseMatrix::Add(1.0,a,1.0,b,c);
}

void vtkpxSparseMatrix::Scale(float s)
{
  this->Matrix.scale(s);

}
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::Add(float wa,vtkpxSparseMatrix* a,float wb,vtkpxSparseMatrix* b,vtkpxSparseMatrix* c)
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

  /*  dvSparseMatrix<float> A=*(a->GetMatrixPointer());
  dvSparseMatrix<float> B=*(b->GetMatrixPointer());
  dvSparseMatrix<float> C=*(c->GetMatrixPointer());*/
  
  dvSparseMatrix A=*(a->GetMatrixPointer());
  dvSparseMatrix B=*(b->GetMatrixPointer());
  dvSparseMatrix C=*(c->GetMatrixPointer());
  
  for(int i=0;i<s1[0];i++)
    for(int j=0;j<s2[0];j++)
	A[i][j]=wa*B[i][j]+wb*B[i][j];

}

// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::Multiply(vtkpxSparseMatrix* a,vtkpxMatrix* b,vtkpxMatrix* c)
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

  //  dvSparseMatrix<float> A=*(a->GetMatrixPointer());
  dvSparseMatrix A=*(a->GetMatrixPointer());
  float* b_pointer=b->GetMatrixPointer();
  float* c_pointer=c->GetMatrixPointer();
  float* C=&c_pointer[0];

  for(int col = 0; col < s3[1]; col++) 
    {
      for(int row = 0; row < s3[0]; row++) 
	{
	  float* B=&b_pointer[col*s2[0]];
	  *C=0;
	  for(int i = 0; i < s1[1]; i++)
	    {
	      (*C)+=A[row][i]*(*B);
	      ++B;
	    }
	  ++C;
	}
    }
}
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::MultiplyThisWith(vtkpxMatrix* b,vtkpxMatrix* c)
{
  int s1[2];
  this->GetSize(s1);

  
  int s2[2],s3[2];
  b->GetSize(s2);
    
  if (s1[1]!=s2[0])
    {
      fprintf(stderr,"Cannot multiply matrices bad size! a=%dx%d, b=%dx%d\n",s1[0],s1[1],s2[0],s2[1]);
      return;
    }
    
  c->Allocate(s1[0],s2[1]);
  c->GetSize(s3);


  float* sum=new float[s3[1]];
  for (int j=0;j<s3[1];j++)
    sum[j]=0.0;

  std::map<unsigned long,dvSparseRow>::iterator iter = this->Matrix.rows_.begin();    
  
  

  while (iter!=this->Matrix.rows_.end())
    {
      std::map<unsigned long,float>::iterator i = (*iter).second.columns_.begin();    
      while (i != (*iter).second.columns_.end())                    
	{
	  for(int col = 0; col < s3[1]; col++) 
	    sum[col]+=(*i).second*b->GetDirectElement((*i).first,col);
	  ++i;
	}
      for(int col = 0; col < s3[1]; col++) 
	c->SetDirectElement((*iter).first,col,sum[col]);
      ++iter;
    }

  delete [] sum;
}

// -----------------------------------------------------------------------------------
// RPM Code
// -----------------------------------------------------------------------------------
void vtkpxSparseMatrix::MapPoints(vtkpxMatrix* Y,vtkpxMatrix* VY)
{
  int size[2];  this->GetSize(size);

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
      float sum=this->Matrix[row].getsum();
      if (sum<0.0001)
	sum=0.0001;
      float scale=1.0/sum;
      this->Matrix[row].multiplywith3vector(row,Y,VY,scale);
      VY->SetDirectElement(row,3,1.0);
    }
  if (this->Debug) 
    fprintf(stderr,"done\n");
}


// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
void vtkpxSparseMatrix::NormalizeMixtureMatrix(vtkpxMatrix* M_Outliers_Row)
{
  int size[2];
  this->GetSize(size);
  if (this->Debug) 
    fprintf(stderr,"Normalizing Distance Matrix %dx%d ...",size[0],size[1]);

  float* sum=new float[size[1]];
  for (int i=0;i<size[1];i++)
    sum[i]=0.0;

  for (int row=0;row<size[0];row++)
    this->Matrix[row].addtosum(sum);

  for (int column=0;column<size[1];column++)
    {
      sum[column]+=M_Outliers_Row->GetElement(0,column);
      if (sum[column]<0.0001)
	sum[column]=0.0001;
      sum[column]=1.0/sum[column];
    }
  
  for (int r=0;r<size[0];r++)
    this->Matrix[r].scalebyarray(sum);

  delete [] sum;

  if (this->Debug) 
    fprintf(stderr,"done\n");
}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
void vtkpxSparseMatrix::NormalizeRPMMatrix(vtkpxMatrix* M_Outliers_Row,vtkpxMatrix* M_Outliers_Column)
{
  int size[2];
  this->GetSize(size);

  if (this->Debug) 
    fprintf(stderr,"RPM Normalizing Distance Matrix %dx%d ...",size[0],size[1]);


  int   norm_maxit     = 10;
  int   row,column;
  float* sumcols=new float[size[1]];


  /*  this->Print("Original");
      M_Outliers_Row->Print("Original Row");
      M_Outliers_Column->Print("Original Column");*/

  int norm_it = 1;
  while (norm_it <= norm_maxit)
    {
      //fprintf(stderr,"\n --------------- Iteration %d --------------------\n",norm_it);
      // --- Row normalization --------------------------------------------
      for (row=0;row<size[0];row++)
	{
	  float sumrow=this->Matrix[row].getsum();
	  sumrow+=M_Outliers_Column->GetElement(row,0);

	  if (sumrow<0.0001)
	    sumrow=0.0001;
	  sumrow=1.0/sumrow;
	  
	  this->Matrix[row].scale(sumrow);
	  M_Outliers_Column->SetElement(row,0,M_Outliers_Column->GetElement(row,0)*sumrow);
	}

      /*this->Print("Normalized");
	M_Outliers_Column->Print("Column");*/

      
      // --- Column normalization -----------------------------------------
      for (column=0;column<size[1];column++)
	sumcols[column]=0.0;
      
      for (row=0;row<size[0];row++)
	this->Matrix[row].addtosum(sumcols);
            
      for (column=0;column<size[1];column++)
	{
	  sumcols[column]+=M_Outliers_Row->GetElement(0,column);
	  if (sumcols[column]<0.0001)
	    sumcols[column]=0.0001;
	  sumcols[column]=1.0/sumcols[column];
	  M_Outliers_Row->SetElement(0,column,M_Outliers_Row->GetElement(0,column)*sumcols[column]);
	}
  
      for (row=0;row<size[0];row++)
	this->Matrix[row].scalebyarray(sumcols);
    

      /*this->Print("Normalized");
	M_Outliers_Row->Print("Row");*/

      norm_it = norm_it + 1;
    }

  delete [] sumcols;

  if (this->Debug) 
    fprintf(stderr,"done\n");
}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 

