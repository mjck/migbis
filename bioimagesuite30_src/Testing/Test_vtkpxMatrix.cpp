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

//---------------------------------------
// test class
//---------------------------------------
class test_vtkpxMatrix{
  public :
  int Add();
  int QRDecomposition();
  int Q1Q2RDecomposition();
  int Invert();            

  int SolveLeastSquares();//for SGELS of LAPACK


  int SolveLinearSystem();//for SGESV of LAPACK

  int SolveUpperSymmetricLinearSystem();//for SPOSV of LAPACK
  int SolveLowerSymmetricLinearSystem();//for SPOSV of LAPACK

  int InternalSolveSymmetricLinearSystem(int mode);

};

//---------------------------------------
// test vtkpxMatrix::Add
//---------------------------------------
int  test_vtkpxMatrix::Add()
{
  int ret;
  ret = 0;

  vtkpxMatrix *matrix_operator = (vtkpxMatrix *) vtkpxMatrix::New();

  vtkpxMatrix *matrix1       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix2       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix_result = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix_expect = (vtkpxMatrix *) vtkpxMatrix::New();

  //Set matrix1
  matrix1->Zeros(3,3);
  matrix1->SetDirectElement (0,0, 1.0);  matrix1->SetDirectElement (0,1,2.0);  matrix1->SetDirectElement (0,2,  3.0);
  matrix1->SetDirectElement (1,0, 4.0);  matrix1->SetDirectElement (1,1,5.0);  matrix1->SetDirectElement (1,2,  6.0);
  matrix1->SetDirectElement (2,0, 7.0);  matrix1->SetDirectElement (2,1,8.0);  matrix1->SetDirectElement (2,2,  9.0);

  //Set matrix2
  matrix2->Zeros(3,3);
  matrix2->SetDirectElement (0,0, 9.0);  matrix2->SetDirectElement (0,1,8.0);  matrix2->SetDirectElement (0,2,  7.0);
  matrix2->SetDirectElement (1,0, 6.0);  matrix2->SetDirectElement (1,1,5.0);  matrix2->SetDirectElement (1,2,  4.0);
  matrix2->SetDirectElement (2,0, 3.0);  matrix2->SetDirectElement (2,1,2.0);  matrix2->SetDirectElement (2,2,  1.0);

  //Initialize
  matrix_result->Zeros(3,3);

  //Add
  matrix_operator->Add(matrix1,matrix2,matrix_result);

  //Set expect data
  matrix_expect->Ones(3,3);
  matrix_expect->Scale(10.0);

  //Validate
  ret = matrix_operator->IsEqual(matrix_expect,matrix_result,tolerance);

  //Output
  std::cout << "Test_Add()-------------------------------------";
  std::cout << "[expect]\n";
  matrix_expect->InternalPrint();
  std::cout << "[result]\n";
  matrix_result->InternalPrint();

  matrix_operator->Delete();
  matrix1->Delete();
  matrix2->Delete();
  matrix_result->Delete();
  matrix_expect->Delete();

  return ret;
}

//---------------------------------------
// test vtkpxMatrix::QRDecomposition
//---------------------------------------
int test_vtkpxMatrix::QRDecomposition()
{
  int ret;

  ret = 0;

  vtkpxMatrix *matrix_operator = (vtkpxMatrix *) vtkpxMatrix::New();

  vtkpxMatrix *matrix1       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix_Q      = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix_R      = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix_Q_expect      = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix_R_expect      = (vtkpxMatrix *) vtkpxMatrix::New();

  //Set matrix1
  matrix1->Zeros(3,3);
  matrix1->SetDirectElement (0,0, 12.0);  matrix1->SetDirectElement (0,1,-51.0);  matrix1->SetDirectElement (0,2,  4.0);
  matrix1->SetDirectElement (1,0,  6.0);  matrix1->SetDirectElement (1,1,167.0);  matrix1->SetDirectElement (1,2,-69.0);
  matrix1->SetDirectElement (2,0, -4.0);  matrix1->SetDirectElement (2,1, 24.0);  matrix1->SetDirectElement (2,2,-41.0);

  //Initialize
  matrix_Q->Zeros(3,3);
  matrix_R->Zeros(3,3);

  //QRDecomposition
  matrix1->QRDecomposition(matrix_Q,matrix_R);

  //Set expect data calculated by Octave version 2.1.57 (i686-pc-linux-gnu)
  //Set matrix_Q_expect
  matrix_Q_expect->Zeros(3,3);
  matrix_Q_expect->SetDirectElement (0,0, -0.857143);  matrix_Q_expect->SetDirectElement (0,1, 0.394286);  matrix_Q_expect->SetDirectElement (0,2,  0.331429);
  matrix_Q_expect->SetDirectElement (1,0, -0.428571);  matrix_Q_expect->SetDirectElement (1,1,-0.902857);  matrix_Q_expect->SetDirectElement (1,2, -0.034286);
  matrix_Q_expect->SetDirectElement (2,0,  0.285714);  matrix_Q_expect->SetDirectElement (2,1,-0.171429);  matrix_Q_expect->SetDirectElement (2,2,  0.942857);

  //Set matrix_R_expect
  matrix_R_expect->Zeros(3,3);
  matrix_R_expect->SetDirectElement (0,0,-14.0);  matrix_R_expect->SetDirectElement (0,1, -21.0);  matrix_R_expect->SetDirectElement (0,2, 14.42857);
  matrix_R_expect->SetDirectElement (1,0,  0.0);  matrix_R_expect->SetDirectElement (1,1,-175.0);  matrix_R_expect->SetDirectElement (1,2, 70.90286);
  matrix_R_expect->SetDirectElement (2,0,  0.0);  matrix_R_expect->SetDirectElement (2,1,   0.0);  matrix_R_expect->SetDirectElement (2,2,-34.96571);
  
  //Output
  std::cout << "Test_QRDecomposition()-------------------------------------";
  std::cout << "[expect]\n";
  std::cout << "Q\n";
  matrix_Q_expect->InternalPrint();
  std::cout << "R\n";
  matrix_R_expect->InternalPrint();
  std::cout << "[result]\n";
  std::cout << "Q\n";
  matrix_Q->InternalPrint();
  std::cout << "R\n";
  matrix_R->InternalPrint();

  //Validate
  ret = matrix_operator->IsEqual(matrix_Q,matrix_Q_expect,tolerance); if(ret !=0 ){ return ret;}
  ret = matrix_operator->IsEqual(matrix_R,matrix_R_expect,tolerance); if(ret !=0 ){ return ret;}

  matrix1->Delete();
  matrix_Q->Delete();
  matrix_R->Delete();
  matrix_Q_expect->Delete();
  matrix_R_expect->Delete();
  matrix_operator->Delete();
  return ret;
}

//---------------------------------------
// test vtkpxMatrix::Q1Q2RDecomposition
//---------------------------------------
int test_vtkpxMatrix::Q1Q2RDecomposition()
{
  int ret;

  ret = 0;
  std::cout <<  "Q1Q2RDecomposition start\n";

  vtkpxMatrix *matrix_operator = (vtkpxMatrix *) vtkpxMatrix::New();

  vtkpxMatrix *matrix1       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix_Q1      = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix_Q2      = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix_R      = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix_Q1_expect      = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix_Q2_expect      = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix_R_expect      = (vtkpxMatrix *) vtkpxMatrix::New();

  //Set matrix1
  matrix1->Zeros(5,3);
  matrix1->SetDirectElement (0,0, 12.0);  matrix1->SetDirectElement (0,1,-51.0);  matrix1->SetDirectElement (0,2,  4.0);
  matrix1->SetDirectElement (1,0,  6.0);  matrix1->SetDirectElement (1,1,167.0);  matrix1->SetDirectElement (1,2,-69.0);
  matrix1->SetDirectElement (2,0, -4.0);  matrix1->SetDirectElement (2,1, 24.0);  matrix1->SetDirectElement (2,2,-41.0);
  matrix1->SetDirectElement (3,0,  10);   matrix1->SetDirectElement (3,1,    1);  matrix1->SetDirectElement (3,2,  7.0);  
  matrix1->SetDirectElement (4,0,  -4);   matrix1->SetDirectElement (4,1,-21.0);  matrix1->SetDirectElement (4,2,  6.0);


  //Initialize
  matrix_Q1->Zeros(5,3);
  matrix_Q2->Zeros(5,2);
  matrix_R->Zeros(3,3);

  //Q1Q2RDecomposition
  matrix1->Q1Q2RDecomposition(matrix_Q1,matrix_Q2,matrix_R);


  //Set expect data calculated by Octave version 2.1.57 (i686-pc-linux-gnu)
  matrix_Q1_expect->Zeros(5,3);
  matrix_Q1_expect->SetDirectElement (0,0, -0.679366);  matrix_Q1_expect->SetDirectElement (0,1, 0.374264);  matrix_Q1_expect->SetDirectElement (0,2,-0.463994);
  matrix_Q1_expect->SetDirectElement (1,0, -0.339683);  matrix_Q1_expect->SetDirectElement (1,1,-0.905745);  matrix_Q1_expect->SetDirectElement (1,2,-0.028185);
  matrix_Q1_expect->SetDirectElement (2,0,  0.226455);  matrix_Q1_expect->SetDirectElement (2,1,-0.164496);  matrix_Q1_expect->SetDirectElement (2,2,-0.859179);
  matrix_Q1_expect->SetDirectElement (3,0, -0.566139);  matrix_Q1_expect->SetDirectElement (3,1, 0.064925);  matrix_Q1_expect->SetDirectElement (3,2, 0.202198);  
  matrix_Q1_expect->SetDirectElement (4,0,  0.226455);  matrix_Q1_expect->SetDirectElement (4,1, 0.090982);  matrix_Q1_expect->SetDirectElement (4,2,-0.069586);

  //Set matrix_Q2_expect
  matrix_Q2_expect->Zeros(5,2);
  matrix_Q2_expect->SetDirectElement (0,0, -0.408598);  matrix_Q2_expect->SetDirectElement (0,1, 0.127062); 
  matrix_Q2_expect->SetDirectElement (1,0, -0.177169);  matrix_Q2_expect->SetDirectElement (1,1, 0.179046); 
  matrix_Q2_expect->SetDirectElement (2,0,  0.406245);  matrix_Q2_expect->SetDirectElement (2,1,-0.135775); 
  matrix_Q2_expect->SetDirectElement (3,0,  0.793298);  matrix_Q2_expect->SetDirectElement (3,1, 0.071180); 
  matrix_Q2_expect->SetDirectElement (4,0,  0.085451);  matrix_Q2_expect->SetDirectElement (4,1, 0.963481); 

  //Set matrix_R_expect
  matrix_R_expect->Zeros(3,3);
  matrix_R_expect->SetDirectElement (0,0,-17.66352);  matrix_R_expect->SetDirectElement (0,1, -21.96617);  matrix_R_expect->SetDirectElement (0,2,  8.83176);
  matrix_R_expect->SetDirectElement (1,0,      0.0);  matrix_R_expect->SetDirectElement (1,1,-176.14053);  matrix_R_expect->SetDirectElement (1,2, 71.73817);
  matrix_R_expect->SetDirectElement (2,0,      0.0);  matrix_R_expect->SetDirectElement (2,1,       0.0);  matrix_R_expect->SetDirectElement (2,2, 36.31301);
  

  //Output
  std::cout << "Test_Q1Q2RDecomposition()-------------------------------------\n";
  std::cout << "[expect]\n";
  std::cout << "Q1\n";
  matrix_Q1_expect->InternalPrint();
  std::cout << "Q2\n";
  matrix_Q2_expect->InternalPrint();
  std::cout << "R\n";
  matrix_R_expect->InternalPrint();

  std::cout << "[result]\n";
  std::cout << "Q1\n";
  matrix_Q1->InternalPrint();
  std::cout << "Q2\n";
  matrix_Q2->InternalPrint();
  std::cout << "R\n";
  matrix_R->InternalPrint();

  //Validate
  ret = matrix_operator->IsEqual(matrix_Q1,matrix_Q1_expect,tolerance); if(ret !=0 ){ return ret;}
  ret = matrix_operator->IsEqual(matrix_Q2,matrix_Q2_expect,tolerance); if(ret !=0 ){ return ret;}
  ret = matrix_operator->IsEqual(matrix_R,matrix_R_expect,tolerance); if(ret !=0 ){ return ret;}


  matrix_operator->Delete();


  matrix1->Delete();
  matrix_Q1->Delete();
  matrix_Q2->Delete();
  matrix_R->Delete();
  matrix_Q1_expect->Delete();
  matrix_Q2_expect->Delete();
  matrix_R_expect->Delete();

  return ret;
}

//---------------------------------------
// test vtkpxMatrix::Invert
//---------------------------------------
int test_vtkpxMatrix::Invert()
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

  matrix1->Invert();

  //Set expect
  matrix1_expect->Zeros(3,3);
  matrix1_expect->SetDirectElement (0,0, -0.0426982);  matrix1_expect->SetDirectElement (0,1,0.0604134);  matrix1_expect->SetDirectElement (0,2,-0.1058369);
  matrix1_expect->SetDirectElement (1,0, -0.2157014);  matrix1_expect->SetDirectElement (1,1,0.0144144);  matrix1_expect->SetDirectElement (1,2,-0.0453024);
  matrix1_expect->SetDirectElement (2,0, -0.1220986);  matrix1_expect->SetDirectElement (2,1,0.0025437);  matrix1_expect->SetDirectElement (2,2,-0.2600954);


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

//---------------------------------------
// test vtkpxMatrix::SolveLeastSquares
//---------------------------------------
int test_vtkpxMatrix::SolveLeastSquares()
{
  int ret;
  ret = 0;
  vtkpxMatrix *matrix_operator = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix1       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix2       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix3       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix3_expect= (vtkpxMatrix *) vtkpxMatrix::New();

  //Overdetermined
  //Set matrix1
  matrix1->Zeros(5,3);
  matrix1->SetDirectElement (0,0, 12.0);  matrix1->SetDirectElement (0,1,-51.0);  matrix1->SetDirectElement (0,2,  4.0);
  matrix1->SetDirectElement (1,0,  6.0);  matrix1->SetDirectElement (1,1,167.0);  matrix1->SetDirectElement (1,2,-69.0);
  matrix1->SetDirectElement (2,0, -4.0);  matrix1->SetDirectElement (2,1, 24.0);  matrix1->SetDirectElement (2,2,-41.0);
  matrix1->SetDirectElement (3,0,  10);   matrix1->SetDirectElement (3,1,    1);  matrix1->SetDirectElement (3,2,  7.0);  
  matrix1->SetDirectElement (4,0,  -4);   matrix1->SetDirectElement (4,1,-21.0);  matrix1->SetDirectElement (4,2,  6.0);
  //Set matrix2
  matrix2->Zeros(5,1);
  matrix2->SetDirectElement(0,0,2.0);
  matrix2->SetDirectElement(1,0,2.0);
  matrix2->SetDirectElement(2,0,2.0);
  matrix2->SetDirectElement(3,0,2.0);
  matrix2->SetDirectElement(4,0,2.0);
  //Initialize matrix3
  matrix3->Zeros(3,1);

  //
  matrix1->SolveLeastSquares(matrix2,matrix3);

  //Set expect
  //Calculated by pinv(matrix1)*matrix2 of Octave
  matrix3_expect->Zeros(3,1);
  matrix3_expect->SetDirectElement(0,0,0.121015);
  matrix3_expect->SetDirectElement(1,0,-0.021206);
  matrix3_expect->SetDirectElement(2,0,-0.067125);

  //Validate
  ret = matrix_operator->IsEqual(matrix3_expect,matrix3,tolerance);

  std::cout << "test_SolveLeastSquares()-------------------------------------------\n";
  std::cout << "Overdetermined\n";
  std::cout << "[expect]\n";
  matrix3_expect->InternalPrint();
  std::cout << "[result]\n";
  matrix3->InternalPrint();


  matrix1->Delete();
  matrix2->Delete();
  matrix3->Delete();
  matrix3_expect->Delete();

  //Welldetermined

 matrix1       = (vtkpxMatrix *) vtkpxMatrix::New();
 matrix2       = (vtkpxMatrix *) vtkpxMatrix::New();
 matrix3       = (vtkpxMatrix *) vtkpxMatrix::New();
 matrix3_expect= (vtkpxMatrix *) vtkpxMatrix::New();

  //Set matrix1
  matrix1->Zeros(3,3);
  matrix1->SetDirectElement (0,0, 12.0);  matrix1->SetDirectElement (0,1,-51.0);  matrix1->SetDirectElement (0,2,  4.0);
  matrix1->SetDirectElement (1,0,  6.0);  matrix1->SetDirectElement (1,1,167.0);  matrix1->SetDirectElement (1,2,-69.0);
  matrix1->SetDirectElement (2,0, -4.0);  matrix1->SetDirectElement (2,1, 24.0);  matrix1->SetDirectElement (2,2,-41.0);
  //Set matrix2
  matrix2->Zeros(3,1);
  matrix2->SetDirectElement(0,0,2.0);
  matrix2->SetDirectElement(1,0,2.0);
  matrix2->SetDirectElement(2,0,2.0);
  //Initialize matrix3
  matrix3->Zeros(3,1);

  //
  matrix1->SolveLeastSquares(matrix2,matrix3);

  //Set expect
  //Calculated by simple "division(=\)" operation of Octave
  matrix3_expect->Zeros(3,1);
  matrix3_expect->SetDirectElement(0,0,0.101207);
  matrix3_expect->SetDirectElement(1,0,-0.020965);
  matrix3_expect->SetDirectElement(2,0,-0.070927);

  //Validate
  ret = matrix_operator->IsEqual(matrix3_expect,matrix3,tolerance);


  std::cout << "Welldetermined\n";
  std::cout << "[expect]\n";
  matrix3_expect->InternalPrint();
  std::cout << "[result]\n";
  matrix3->InternalPrint();

  matrix1->Delete();
  matrix2->Delete();
  matrix3->Delete();
  matrix3_expect->Delete();
  
  //Underdetermined
  matrix1       = (vtkpxMatrix *) vtkpxMatrix::New();
  matrix2       = (vtkpxMatrix *) vtkpxMatrix::New();
  matrix3       = (vtkpxMatrix *) vtkpxMatrix::New();
  matrix3_expect= (vtkpxMatrix *) vtkpxMatrix::New();

  //Set matrix1
  matrix1->Zeros(5,5);
  matrix1->SetDirectElement (0,0, 12.0);  matrix1->SetDirectElement (0,1,-51.0);  matrix1->SetDirectElement (0,2,  4.0);  
  matrix1->SetDirectElement (0,3,  7.0);matrix1->SetDirectElement   (0,4,-3.0);
  matrix1->SetDirectElement (1,0,  6.0);  matrix1->SetDirectElement (1,1,167.0);  matrix1->SetDirectElement (1,2,-69.0);
  matrix1->SetDirectElement (1,3,  10.0);matrix1->SetDirectElement  (1,4,-1.5);
  matrix1->SetDirectElement (2,0, -4.0);  matrix1->SetDirectElement (2,1, 24.0);  matrix1->SetDirectElement (2,2,-41.0);
  matrix1->SetDirectElement (2,3,  2.0);matrix1->SetDirectElement   (2,4,-3.0);

  //Set matrix2
  matrix2->Zeros(5,1);
  matrix2->SetDirectElement(0,0,2.0);
  matrix2->SetDirectElement(1,0,2.0);
  matrix2->SetDirectElement(2,0,2.0);
  matrix2->SetDirectElement(3,0,0.0);
  matrix2->SetDirectElement(4,0,0.0);
  //Initialize matrix3
  matrix3->Zeros(5,1);

  //
   matrix1->SolveLeastSquares(matrix2,matrix3);

  //Set expect
  matrix3_expect->Zeros(5,1);
  matrix3_expect->SetDirectElement(0,0,0.101207);
  matrix3_expect->SetDirectElement(1,0,-0.020965);
  matrix3_expect->SetDirectElement(2,0,-0.070927);
  matrix3_expect->SetDirectElement(3,0,0);
  matrix3_expect->SetDirectElement(4,0,0);

  std::cout << "Underdetermined\n";
  std::cout << "[expect]\n";
  matrix3_expect->InternalPrint();
  std::cout << "[result]\n";
  matrix3->InternalPrint();

  matrix1->Delete();
  matrix2->Delete();
  matrix3->Delete();
  matrix3_expect->Delete();

  return ret;
}

//---------------------------------------
// test vtkpxMatrix::SolveLinearSystem
//---------------------------------------
int test_vtkpxMatrix::SolveLinearSystem()
{
  int ret;
  ret = 0;

  vtkpxMatrix *matrix_operator = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix1       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix2       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix3       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix3_expect= (vtkpxMatrix *) vtkpxMatrix::New();

  matrix1->Zeros(3,3);
  matrix1->SetDirectElement (0,0, 12.0);  matrix1->SetDirectElement (0,1,-51.0);  matrix1->SetDirectElement (0,2,  4.0);
  matrix1->SetDirectElement (1,0,  6.0);  matrix1->SetDirectElement (1,1,167.0);  matrix1->SetDirectElement (1,2,-69.0);
  matrix1->SetDirectElement (2,0, -4.0);  matrix1->SetDirectElement (2,1, 24.0);  matrix1->SetDirectElement (2,2,-41.0);
  //Set matrix2
  matrix2->Zeros(3,1);
  matrix2->SetDirectElement(0,0,2.0);
  matrix2->SetDirectElement(1,0,2.0);
  matrix2->SetDirectElement(2,0,2.0);
  //Initialize matrix3
  matrix3->Zeros(3,1);

  //
  matrix1->SolveLinearSystem(matrix2,matrix3);

  //Set expect
  //Calculated by simple "division(=\)" operation of Octave
  matrix3_expect->Zeros(3,1);
  matrix3_expect->SetDirectElement(0,0,0.101207);
  matrix3_expect->SetDirectElement(1,0,-0.020965);
  matrix3_expect->SetDirectElement(2,0,-0.070927);

  //Validate
  ret = matrix_operator->IsEqual(matrix3_expect,matrix3,tolerance);


  std::cout << "test_SolveLinearSystem---------------------------\n";
  std::cout << "[expect]\n";
  matrix3_expect->InternalPrint();
  std::cout << "[result]\n";
  matrix3->InternalPrint();

  matrix1->Delete();
  matrix2->Delete();
  matrix3->Delete();
  matrix3_expect->Delete();
  matrix_operator->Delete();

  return ret;
}

//---------------------------------------
// Helper functions
//---------------------------------------
int test_vtkpxMatrix::InternalSolveSymmetricLinearSystem(int mode)
{
  int ret;
  ret = 0;
  vtkpxMatrix *matrix_operator = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix1       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix2       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix3       = (vtkpxMatrix *) vtkpxMatrix::New();
  vtkpxMatrix *matrix3_expect= (vtkpxMatrix *) vtkpxMatrix::New();

  matrix1->Zeros(3,3);
  matrix1->SetDirectElement (0,0,  3.0);  matrix1->SetDirectElement (0,1,2.0);  matrix1->SetDirectElement (0,2,1.0);
  matrix1->SetDirectElement (1,0,  2.0);  matrix1->SetDirectElement (1,1,2.0);  matrix1->SetDirectElement (1,2,1.0);
  matrix1->SetDirectElement (2,0,  1.0);  matrix1->SetDirectElement (2,1,1.0);  matrix1->SetDirectElement (2,2,1.0);
  //Set matrix2
  matrix2->Zeros(3,1);
  matrix2->SetDirectElement(0,0,2.4);
  matrix2->SetDirectElement(1,0,0.2);
  matrix2->SetDirectElement(2,0,1.2);
  //Initialize matrix3
  matrix3->Zeros(3,1);

  //
  char *tmp;
  switch (mode){
  case 0 :  matrix1->SolveLowerSymmetricLinearSystem(matrix2,matrix3);tmp = "Lower";break;
  case 1 :  matrix1->SolveUpperSymmetricLinearSystem(matrix2,matrix3);tmp = "Upper";break;
  default :  break;
  }

  //Set expect
  //Calculated by simple "division(=\)" operation of Octave
  matrix3_expect->Zeros(3,1);
  matrix3_expect->SetDirectElement(0,0,2.2);
  matrix3_expect->SetDirectElement(1,0,-3.2);
  matrix3_expect->SetDirectElement(2,0,2.2);

  //Validate
  //  ret = matrix_operator->IsEqual(matrix3_expect,matrix3,tolerance);

  std::cout << "test_" << tmp << "SymmetricLinearSystem---------------------------\n";
  std::cout << "[expect]\n";
  matrix3_expect->InternalPrint();
  std::cout << "[result]\n";
  matrix3->InternalPrint();

  matrix1->Delete();
  matrix2->Delete();
  matrix3->Delete();
  matrix3_expect->Delete();

  return ret;
}

//---------------------------------------
// Helper functions
//---------------------------------------

int test_vtkpxMatrix::SolveUpperSymmetricLinearSystem()
{
  int ret;
  ret = 0;
  ret = this->InternalSolveSymmetricLinearSystem(0);
  return ret;
}


//---------------------------------------
// Helper functions
//---------------------------------------
int test_vtkpxMatrix::SolveLowerSymmetricLinearSystem()
{
  int ret;
  ret = 0;
  ret = this->InternalSolveSymmetricLinearSystem(1);
  return ret;
}

//--------------------------------------------
// main part for testing vtkpxMatrix class
//--------------------------------------------
int main(int argc,char* argv[])
{
  int ret;
  ret = 0;

  test_vtkpxMatrix test;

  ret = test.Add();                if (ret!=0) {std::cout << "test_Add() returns error "; return ret;}
  ret = test.QRDecomposition();    if (ret!=0) {std::cout << "test_QRDecomposition() returns error "; return ret;}
  ret = test.Q1Q2RDecomposition(); if (ret!=0) {std::cout << "test_Q1R2Decomposition() returns error "; return ret;}
  ret = test.Invert();             if (ret!=0) {std::cout << "est_Invert() returns error "; return ret;}
  ret = test.SolveLeastSquares();                           if (ret!=0) {std::cout << "test_SolveLeastSquares() returns error "; return ret;}
  ret = test.SolveLinearSystem();                           if (ret!=0) {std::cout << "test_SolveLinearSystem() returns error "; return ret;}
  ret = test.SolveUpperSymmetricLinearSystem();             if (ret!=0) {std::cout << "test_SolveUpperSymmetricLinearSystem() returns error "; return ret;}
  ret = test.SolveLowerSymmetricLinearSystem();             if (ret!=0) {std::cout << "test_SolveLowerSymmetricLinearSystem() returns error "; return ret;}

  if (ret==0)
    std::cout << "All Tests Passed\n";

  return ret;
}


