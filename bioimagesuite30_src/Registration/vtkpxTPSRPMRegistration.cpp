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
  Module:    $RCSfile: vtkpxTPSRPMRegistration.cpp,v $
  Language:  C++
  Date:      $Date: 2002/03/12 16:49:59 $
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

#include "pxutil.h"
#include "vtkOldStyleCallbackCommand.h"
#include "vtkDataSet.h"
#include "vtkLandmarkTransform.h"
#include "vtkMath.h"
#include "vtkpxMath.h"
#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"
#include "vtkpxTPSRPMRegistration.h"
#include "vtkpxMatrix.h"
#include "vtkpxUtil.h"
#include "vtkIdList.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkCollection.h"
#include "vtkFloatArray.h"
//-----------------------------------------------------------------------------------

vtkpxTPSRPMRegistration* vtkpxTPSRPMRegistration::New()
{
  // First try to create the object from the vtkObjectFactory

  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxTPSRPMRegistration");
  if(ret)
    {
    return (vtkpxTPSRPMRegistration*)ret;
    }

  // If the factory was unable to create the object, then create it here.

  return new vtkpxTPSRPMRegistration;
}

//----------------------------------------------------------------------------

vtkpxTPSRPMRegistration::vtkpxTPSRPMRegistration()
{
  this->ThinPlateSplineTransform = vtkThinPlateSplineTransform::New();
  for (int i=0;i<12;i++)
    temp_mat[i]=vtkpxMatrix::New();

}

//----------------------------------------------------------------------------

vtkpxTPSRPMRegistration::~vtkpxTPSRPMRegistration()
{
  this->ThinPlateSplineTransform->Delete();
  for (int i=0;i<12;i++)
    temp_mat[i]->Delete();
}

//----------------------------------------------------------------------------
void vtkpxTPSRPMRegistration::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkpxAbstractRPMRegistration::PrintSelf(os,indent);
  os << indent << "Transformation: " << this->ThinPlateSplineTransform << "\n";
}

//----------------------------------------------------------------------------
vtkAbstractTransform* vtkpxTPSRPMRegistration::GetCurrentTransformation()
{
  return this->ThinPlateSplineTransform;
}
//----------------------------------------------------------------------------
// Transformation Stuff
//----------------------------------------------------------------------------
void vtkpxTPSRPMRegistration::WarpControlPoints(vtkpxMatrix* X,vtkpxMatrix* VX,vtkpxMatrix* C,vtkpxMatrix* D)
{
  this->Gen_TPS_K(X,temp_mat[10]); 
  vtkpxMatrix::Multiply(X,D,VX);
  vtkpxMatrix::Multiply(temp_mat[10],C,temp_mat[11]);
  vtkpxMatrix::Add(VX,temp_mat[11],VX);
  return;
}


// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
// Helper Routines for Thin-Plate Spline 
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
void vtkpxTPSRPMRegistration::Gen_TPS_K(vtkpxMatrix* X,vtkpxMatrix* K)
{
  int sx[2];
  X->GetSize(sx);
  K->Zeros(sx[0],sx[0]);

  float p1[3],p2[3];
  for (int k=0;k<=2;k++)
    {
      p1[k]=0.0;
      p2[k]=0.0;
    }


  for (int row=0;row<sx[0];row++)
    {
      for (int ia=0;ia<3;ia++)
	p1[ia]=X->GetDirectElement(row,ia);

      for (int column=row+1;column<sx[0];column++)
	{
	  if (row!=column)
	    {
	      for (int ib=0;ib<3;ib++)
		p2[ib]=X->GetDirectElement(column,ib);
	      
	      float dist2= vtkMath::Distance2BetweenPoints(p1, p2);
	      K->SetElement(row,column,-sqrt(dist2));
	      K->SetElement(column,row,K->GetDirectElement(row,column));
	    }
	}
    }
}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - -  
void vtkpxTPSRPMRegistration::Gen_TPS_CD_Regularized(float lambda1,float lambda2,
					       vtkpxMatrix* q1,vtkpxMatrix* q2,
					       vtkpxMatrix* R,vtkpxMatrix* K,vtkpxMatrix* Y,
					       vtkpxMatrix* C,vtkpxMatrix* D)
{
  int sy[2];
  Y->GetSize(sy);
  int n=sy[0];
  int M=sy[1];

  this->temp_mat[5]->Copy(q2);
  this->temp_mat[5]->Transpose();
  vtkpxMatrix::Multiply(this->temp_mat[5],Y,this->temp_mat[6]);
  
  // this->temp_mat[0]=q2'*K*q2
  // Speed this up 
  vtkpxMatrix::MultiplyTripleProduct(q2,K,this->temp_mat[0]);
  // this->temp_mat[1]=lambda1*eye(n-M)
  this->temp_mat[1]->Eye(n-M);
  this->temp_mat[1]->Scale(lambda1);
  
  // this->temp_mat[0]=this->temp_mat[0]+this->temp_mat[1];
  vtkpxMatrix::Add(this->temp_mat[0],this->temp_mat[1],this->temp_mat[0]);
  
  // Invert this->temp_mat[0]
  this->temp_mat[0]->Invert();
  
  // C= q2 * inv (this->temp_mat[0]) * this->temp_mat[6]
  vtkpxMatrix::Multiply3(q2,this->temp_mat[0],this->temp_mat[6],C);
  

  // Then Calculate D 
  //   A = inv(R'*R + lamda2 * eye(length(R),length(R))) * ( R'*q1'*(y-K*q2*gamma) - R'*R);
  //   d = A + eye(dim+1,dim+1);
  

  this->temp_mat[3]->Copy(R);
  this->temp_mat[3]->Transpose();
  
  // this->temp_mat[1]= R'*R
  vtkpxMatrix::Multiply(this->temp_mat[3],R,this->temp_mat[4]);
  
  // this->temp_mat[0] = eye(length(R,R))
  int lr[2]; 
  R->GetSize(lr);
  this->temp_mat[0]->Eye(lr[0]);
  this->temp_mat[0]->Scale(lambda2);

  // Find R'*R + lambda2*eye(length(R)
  vtkpxMatrix::Add(this->temp_mat[0],this->temp_mat[4],this->temp_mat[0]);

  // Invert this->temp_mat[0]
  this->temp_mat[0]->Invert();

  // Work on ( R'*q1'*(y-K*C) - R'*R);    (q2*gamma=C)
  
  // this->temp_mat[1]=y-KC 
  vtkpxMatrix::Multiply(K,C,this->temp_mat[1]);
  this->temp_mat[1]->Scale(-1.0);
  vtkpxMatrix::Add(Y,this->temp_mat[1],this->temp_mat[1]);

  // Find this->temp_mat[2]=R'*q1'*(y-KC)-R'R
  q1->Transpose();
  vtkpxMatrix::Multiply3(this->temp_mat[3],q1,this->temp_mat[1],this->temp_mat[2]);
  this->temp_mat[4]->Scale(-1.0);
  vtkpxMatrix::Add(this->temp_mat[2],this->temp_mat[4],this->temp_mat[2]);
  
  // Find this->temp_mat[1] = A  = inv(R'*R + lamda2 * eye(length(R),length(R))) * ( R'*q1'*(y-K*q2*gamma) - R'*R)
  vtkpxMatrix::Multiply(this->temp_mat[0],this->temp_mat[2],this->temp_mat[1]);
  
  // Find D = A + eye(4,4);
  D->Eye(4);
  vtkpxMatrix::Add(D,this->temp_mat[1],D);


}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - -  
void vtkpxTPSRPMRegistration::GenerateTransformation(vtkpxMatrix* X,vtkpxMatrix* VY,
						     float lambda1,float lambda2,vtkpxMatrix* C,vtkpxMatrix* D)
{
  X->Q1Q2RDecomposition(temp_mat[7],temp_mat[8],temp_mat[9]);  // q1,q2,R
  this->Gen_TPS_K(X,temp_mat[10]);                             // K
  this->Gen_TPS_CD_Regularized(lambda1,lambda2,temp_mat[7],temp_mat[8],temp_mat[9],temp_mat[10],VY,C,D);
}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - -  
void vtkpxTPSRPMRegistration::CreateFinalTransformation(vtkpxMatrix* X,vtkpxMatrix* VX)
  
{
  vtkPoints* p1_a=this->CreatePointsFromMatrix(X);
  vtkPoints* p2_a=this->CreatePointsFromMatrix(VX);


  this->ThinPlateSplineTransform->SetSourceLandmarks(p1_a);
  this->ThinPlateSplineTransform->SetTargetLandmarks(p2_a);
  this->ThinPlateSplineTransform->SetBasisToR();
  this->ThinPlateSplineTransform->Update();

  p1_a->Delete();
  p2_a->Delete();


}
//----------------------------------------------------------------------------
void vtkpxTPSRPMRegistration::Anneal()
{
  int perT_maxit=5;
  //int relax_maxit=1;
  float anneal_rate=this->AnnealRate;
  float lambda1_init=this->InitialSmoothnessFactor;
  float lambda2_init=0.01*lambda1_init;

  // Initialize Temperature Stuff 
  float T      =Frange(this->InitialTemperature,0.01,0.5);
  float T_final=Frange(this->FinalTemperature,0.00001,T);
  float totaliter=fabs(log(T/T_final)/log(anneal_rate))+1.0;

  vtkpxMatrix* X=vtkpxMatrix::New();
  vtkpxMatrix* Y=vtkpxMatrix::New();
  vtkpxMatrix* LX=vtkpxMatrix::New();
  vtkpxMatrix* LY=vtkpxMatrix::New();

  this->SamplePoints(X,Y,LX,LY,this->MaximumNumberOfLandmarks);


  int sz[2],nump_x,nump_y;
  X->GetSize(sz);   nump_x=sz[0];
  Y->GetSize(sz);   nump_y=sz[0];
  vtkDebugMacro(<< "Anneal: Using " << nump_x << " , " << nump_y << "Landmarks UseLabels="<< this->UseLabels << "\n");

  vtkpxAbstractMatrix* M=this->AllocateMatchMatrix(this->MatchMatrixType);
  vtkpxMatrix* M_Outliers_Row=vtkpxMatrix::New();
  vtkpxMatrix* M_Outliers_Column=vtkpxMatrix::New();
  this->InitMatchMatrices(M,M_Outliers_Row,M_Outliers_Column,nump_x,nump_y);
  
  vtkPointLocator* locator=this->CreateLocator(Y);

  // Initialize Transformed X and Y stuff 
  vtkpxMatrix* VX=vtkpxMatrix::New();   VX->Copy(X);


  if (this->InitialTransform!=NULL && this->UseInitialTransform>0 )
    {
      fprintf(stderr,"Initializing using current transform!\n\n");
      float x[3],tx[3];
      for (int i=0;i<nump_x;i++)
	{
	  for (int ia=0;ia<3;ia++)
	    x[ia]=VX->GetDirectElement(i,ia);

	  this->InitialTransform->TransformPoint(x,tx);
	  for (int ib=0;ib<3;ib++)
	    VX->SetDirectElement(i,ib,tx[ib]);
	}
    }
  
  // THis is a BUG in Haili's Code, VY has same size as X!!! 
  vtkpxMatrix* VY=vtkpxMatrix::New();  VY->Copy(X);

  vtkpxMatrix* C=vtkpxMatrix::New();
  vtkpxMatrix* D=vtkpxMatrix::New();
  C->Zeros(nump_x,4);
  D->Zeros(4,4);
  
  // Annealing Procedure 
  int flag_stop=0;
  int iter=1;

  perT_maxit=1;

  while (flag_stop == 0 )
    {
      float lambda1=lambda1_init*float(nump_x)*T;
      float lambda2=lambda2_init*float(nump_x)*T;

      for (int i=1;i<=perT_maxit;i++)
	{
	  if (this->AbortExecute>0)
	    {
	      i=perT_maxit+1;
	    }
	  else
	    {
	      this->SetOutliers(M_Outliers_Row,M_Outliers_Column,T);
	      this->UpdateCorrespondence(M,M_Outliers_Row,M_Outliers_Column,
					 VX,Y,VY,T,locator,LX,LY);
	      this->GenerateTransformation(X,VY,
					   lambda1,lambda2,C,D);
	      this->WarpControlPoints(X,VX,C,D);
	    }
	}

      vtkDebugMacro(<<"\n----------------------------------------------------------\nIteration " << iter << "/" << totaliter << " T = " << T << " Lambda1=" << lambda1 << "Lambda2 = "<< lambda2 << ", abort= " << this->AbortExecute << "\n");

      if (this->EnableFeedback)
	this->CreateFinalTransformation(X,VX);


      this->UpdateProgress(float(iter)/totaliter);
      iter++;
      T=T*anneal_rate;
      if (T<T_final || this->AbortExecute>0)
	flag_stop=1;
    }
  
  vtkDebugMacro(<<"Creating Final Transformation");
  this->CreateFinalTransformation(X,VX);
  this->InvokeEvent(vtkCommand::EndEvent,NULL);

  // Cleanup Stuff 
  if (locator!=NULL)    locator->Delete();
  if (C!=NULL)          C->Delete();
  if (D!=NULL)          D->Delete();
  VX->Delete();         VY->Delete();
  M->Delete();          M_Outliers_Row->Delete();
  X->Delete();          M_Outliers_Column->Delete();
  Y->Delete();          
  LX->Delete();         LY->Delete();
}

