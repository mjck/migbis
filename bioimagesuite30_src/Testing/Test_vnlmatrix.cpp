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

//
// .Description
//  Test program for vtkpxMatrix Class
// .SETCTION Autor 
//  Hirohito Okuda <hirohito.okuda@yale.edu>
//

#include "bioimagesuite_settings.h"
#include "vtkpxMatrix.h"
#include "test.h"
#include "vnl/vnl_matrix.h"
#include "vnl/algo/vnl_matrix_inverse.h"

//---------------------------------------
// test class
//---------------------------------------
class test_vnlMatrix{
  public :
  int Invert();            
};


//---------------------------------------
// test vtkpxMatrix::Invert
//---------------------------------------
int test_vnlMatrix::Invert()
{
  int ret;

  ret = 0;

  vtkpxMatrix *matrix_operator = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix1            = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix1_expect      = (vtkpxMatrix *) vtkpxMatrix::New();


  //Set matrix1
  matrix1->Zeros(3,3);
  matrix1->SetDirectElement (0,0, 1.20);  matrix1->SetDirectElement (0,1,-5.1);  matrix1->SetDirectElement (0,2, 0.4);
  matrix1->SetDirectElement (1,0, 16.7);  matrix1->SetDirectElement (1,1, 0.6);  matrix1->SetDirectElement (1,2,-6.9);
  matrix1->SetDirectElement (2,0, -0.4);  matrix1->SetDirectElement (2,1, 2.4);  matrix1->SetDirectElement (2,2,-4.1);

  //Set expect
  matrix1_expect->Zeros(3,3);
  matrix1_expect->SetDirectElement (0,0, -0.0426982);  matrix1_expect->SetDirectElement (0,1,0.0604134);  matrix1_expect->SetDirectElement (0,2,-0.1058369);
  matrix1_expect->SetDirectElement (1,0, -0.2157014);  matrix1_expect->SetDirectElement (1,1,0.0144144);  matrix1_expect->SetDirectElement (1,2,-0.0453024);
  matrix1_expect->SetDirectElement (2,0, -0.1220986);  matrix1_expect->SetDirectElement (2,1,0.0025437);  matrix1_expect->SetDirectElement (2,2,-0.2600954);


  int sz[2]; matrix1->GetSize(sz);
  vnl_matrix<double> M(sz[0],sz[1]);
  for (unsigned i=0; i<M.rows(); ++i)
    for (unsigned j=0; j<M.cols(); ++j)
      M(i,j)=matrix1->GetDirectElement(i,j);

  vnl_matrix<double> IN=vnl_matrix_inverse<double>(M);

  for (unsigned i=0; i< sz[0]; ++i)
    for (unsigned j=0; j< sz[1]; ++j)
	matrix1->SetDirectElement(i,j,IN(i,j));

  //Output
  std::cout << "Test_Inverse()-------------------------------------\n";
  std::cout << "[expect]\n";
  matrix1_expect->InternalPrint();
  std::cout << "\n";
  std::cout << "[result]\n";
  matrix1->InternalPrint();

  //Validate
  ret = matrix_operator->IsEqual(matrix1,matrix1_expect,tolerance); if(ret !=0 ){ return ret;}

  matrix_operator->Delete();
  matrix1->Delete();
  matrix1_expect->Delete();

  return ret;
}


//--------------------------------------------
// main part for testing vtkpxMatrix class
//--------------------------------------------
int main(int argc,char* argv[])
{
  int ret;
  ret = 0;

  test_vnlMatrix test;

  ret = test.Invert();             if (ret!=0) {std::cout << "est_Invert() returns error "; return ret;}

  if (ret==0)
    std::cout << "All Tests Passed\n";
  
  return ret;
}


