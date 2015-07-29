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
  Module:    $RCSfile: vtkpxBSplineRPMRegistration.cpp,v $
  Language:  C++
  Date:      $Date: 2005/10/25 00:45:36 $
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
#include "vtkpxGridTransform.h"
#include "vtkpxMath.h"
#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"
#include "vtkpxBSplineRPMRegistration.h"
#include "vtkpxMatrix.h"
#include "vtkpxUtil.h"
#include "vtkIdList.h"
#include "vtkCollection.h"
#include "vtkDoubleArray.h"
#include "vtkPolyData.h"
#include "vtkPointLocator.h"
#include "vtkLinearTransform.h"
//-----------------------------------------------------------------------------------

vtkpxBSplineRPMRegistration* vtkpxBSplineRPMRegistration::New()
{
  // First try to create the object from the vtkObjectFactory

  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxBSplineRPMRegistration");
  if(ret)
    {
      return (vtkpxBSplineRPMRegistration*)ret;
    }

  // If the factory was unable to create the object, then create it here.

  return new vtkpxBSplineRPMRegistration;
}

//----------------------------------------------------------------------------

vtkpxBSplineRPMRegistration::vtkpxBSplineRPMRegistration()
{
  this->ComboTransform=vtkpxComboTransform::New();
  this->ComboTransform->SetNonLinearFirst(1);
  this->GridTransform = this->ComboTransform->GetGridTransform();
  this->InitialControlPointSpacing=0.2;
  this->FinalControlPointSpacing=0.2;
  this->CurrentControlPointSpacing=-1.0;
  this->FinalWeights=vtkDoubleArray::New();
}

//----------------------------------------------------------------------------

vtkpxBSplineRPMRegistration::~vtkpxBSplineRPMRegistration()
{
  this->ComboTransform->Delete();
  this->GridTransform=NULL;
  this->FinalWeights->Delete();
}

//----------------------------------------------------------------------------
void vtkpxBSplineRPMRegistration::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkpxAbstractRPMRegistration::PrintSelf(os,indent);
  os << indent << "InitialControlPointSpacing:  " << this->InitialControlPointSpacing << "\n";
  os << indent << "FinalControlPointSpacing:  " << this->FinalControlPointSpacing << "\n";
  os << indent << "CurrentControlPointSpacing:  " << this->CurrentControlPointSpacing << "\n";
  os << indent << "Transformation: " << this->ComboTransform << "\n";
}

//----------------------------------------------------------------------------
vtkAbstractTransform* vtkpxBSplineRPMRegistration::GetCurrentTransformation()
{
  return this->ComboTransform;
}
//----------------------------------------------------------------------------
// Transformation Stuff
//----------------------------------------------------------------------------
void vtkpxBSplineRPMRegistration::GenerateTransformation(vtkpxGridTransform* gridxform,
							 vtkpxMatrix* points1,vtkpxMatrix* points2,
							 vtkpxMatrix* labels,
							 float controlpointspacing,
							 double bounds[6],
							 float lambda,
							 vtkCollection* W,vtkpxMatrix* OutlierWeights,
							 int finalmode)
{
  if (points1->GetSize()[0]!=points2->GetSize()[0])
    {
      vtkErrorMacro(<<"Unequal Number of points specified cannot fit bspline transform!");
      return;
    }

  float gap=fabs(this->InitialControlPointSpacing-this->FinalControlPointSpacing)*0.1;

  fprintf(stderr,"Controlpointspacing=%f current=%f\n",controlpointspacing,this->CurrentControlPointSpacing);
  int newgrid=0;
  if (fabs(controlpointspacing-this->CurrentControlPointSpacing)>gap)
    { 
      gridxform->InitializeDisplacementGridTransform(bounds,controlpointspacing,0.05,NULL);
      fprintf(stderr,"Tranformation Spacing = %.2f %.2f %.2f, Origin = %.2f %.2f %.2f, Dimensions=%d %d %d\n",
	      gridxform->GetGridSpacing()[0],
	      gridxform->GetGridSpacing()[1],
	      gridxform->GetGridSpacing()[2],
	      gridxform->GetGridOrigin()[0],
	      gridxform->GetGridOrigin()[1],
	      gridxform->GetGridOrigin()[2],
	      gridxform->GetGridDimensions()[0],
	      gridxform->GetGridDimensions()[1],
	      gridxform->GetGridDimensions()[2]);
      this->CurrentControlPointSpacing=controlpointspacing;
      newgrid=1;
    }

  int np=points1->GetSize()[0];

  int useoutliers=0;
  int dolabels=0;
  if (labels!=NULL && this->UseLabels>0)
    dolabels=1;


  if (OutlierWeights!=NULL)
    {
      if (OutlierWeights->GetSize()[0]==np)
	{
	  useoutliers=1;
	}
    }

  /*  if (initial_xform!=NULL)
    {
      if (initial_xform->IsA("vtkpxPolySimilarityTransform")==1)
	this->ComboTransform->InitializeFromPolySimilarityTransform((vtkpxPolySimilarityTransform*)initial_xform);
	}*/

  //  this->ComboTransform->Save("approx1.grd");

  vtkPoints* p1=vtkPoints::New();
  p1->SetNumberOfPoints(np);
  vtkPoints* p2=vtkPoints::New();
  p2->SetNumberOfPoints(np);
  
  double sum=0.0;
  for (int i=0;i<np;i++)
    {
      float x[3],y[3];
      for (int ia=0;ia<=2;ia++)
	{
	  x[ia]=points1->GetDirectElement(i,ia);
	  y[ia]=points2->GetDirectElement(i,ia);
	}

      p1->SetPoint(i,x);
      p2->SetPoint(i,y);
      float dsum=0.0;
      for (int j=0;j<=2;j++)
	{
	  float v=y[j]-x[j];
	  dsum+=pow(v,float(2.0));
	}
      sum+=sqrt(dsum);
    }
  sum/=np;

  int numc=gridxform->GetNumberOfControlPoints();

  if (W->GetNumberOfItems()!=numc)
    {
      fprintf(stderr,"Forming Collection Structure\n");
      gridxform->FormCollectionStructure(p1,W,0.001);
      fprintf(stderr,"Done\n");
    }

  float threshold=sum*0.01;
  fprintf(stderr,"Lambda=%10.7f average component=%10.5f approximating mode=%d threshold=%.5f\n",
	  lambda,sum,finalmode,
	  threshold*0.01);
  gridxform->SetApproximationMode(0);
  
  //  sum=20.0;
  sum*=2.5;

  switch (finalmode)
    {
    case 1:
      gridxform->ApproximatePoints(p1,p2,W,
				   sum*0.05,
				   20,1,
				   threshold,0.001,
				   newgrid,OutlierWeights);
      break;
    case 2:
      gridxform->ApproximatePoints(p1,p2,W,
				   sum*0.05,
				   20,3,
				   threshold,0.001,
				   newgrid,OutlierWeights);
      break;
    default:
      gridxform->ApproximatePoints(p1,p2,W,
				   sum*0.05,
				   40,1,
				   threshold,lambda,
				   newgrid,OutlierWeights);
      break;
    }
  double e2=0.0;
  float e=gridxform->TestWeightedApproximation(p1,p2,OutlierWeights,e2);
  fprintf(stderr,"Weighted Displacement approximation=%8.6f\n---------------------------------\n",e);
  p2->Delete();
  p1->Delete();



  return;

}
//----------------------------------------------------------------------------
//
//          Initial Transformation Stuff
//
//----------------------------------------------------------------------------
void vtkpxBSplineRPMRegistration::InitializeUsingInitialComboTransform(vtkpxMatrix* X,vtkpxMatrix* Y,
								  vtkpxMatrix* VX,
								  vtkpxComboTransform* usethiscombotransformation)
{
  vtkTransform* tr=vtkTransform::New();
  tr->GetMatrix()->DeepCopy(usethiscombotransformation->GetLinearTransform()->GetMatrix());
  this->ComboTransform->GetLinearTransform()->GetMatrix()->DeepCopy(tr->GetMatrix());
  this->ComboTransform->SetNonLinearFirst(usethiscombotransformation->GetNonLinearFirst());

  if (this->ComboTransform->GetNonLinearFirst()==0)
    {
      int nump_x=X->GetSize()[0];
      float x[3],tx[3],tx2[3];
      for (int i=0;i<nump_x;i++)
	{
	  for (int ia=0;ia<3;ia++)
	    x[ia]=X->GetDirectElement(i,ia);
	  usethiscombotransformation->GetLinearTransform()->TransformPoint(x,tx);
	  usethiscombotransformation->GetGridTransform()->TransformPoint(tx,tx2);
	  
	  for (int ib=0;ib<3;ib++)
	    {
	      X->SetDirectElement(i,ib,tx[ib]);
	      VX->SetDirectElement(i,ib,tx2[ib]);
	    }
	}
    }
  else
    {
      fprintf(stderr,"In Non Linear First = 1\n\n\n");
      int nump_x=X->GetSize()[0];
      float x[3],tx[3];
      for (int i=0;i<nump_x;i++)
	{
	  for (int ia=0;ia<3;ia++)
	    x[ia]=X->GetDirectElement(i,ia);
	  usethiscombotransformation->GetGridTransform()->TransformPoint(x,tx);
	  for (int ib=0;ib<3;ib++)
	    VX->SetDirectElement(i,ib,tx[ib]);
	}
      
      float y[3],ty[3];
      int nump_y=Y->GetSize()[0];
      for (int j=0;j<nump_y;j++)
	{
	  for (int ia=0;ia<3;ia++)
	    y[ia]=Y->GetDirectElement(j,ia);
	  usethiscombotransformation->GetLinearTransform()->GetLinearInverse()->TransformPoint(y,ty);
	  for (int ib=0;ib<3;ib++)
	      Y->SetDirectElement(j,ib,tx[ib]);
	}
    }

  

}
//----------------------------------------------------------------------------
void vtkpxBSplineRPMRegistration::InitializeUsingInitialTransform(vtkpxMatrix* X,vtkpxMatrix* Y,
								  vtkpxMatrix* VX,
								  vtkAbstractTransform* usethistransformation)
{
  
  /*  if (usethistransformation->IsA("vtkpxComboTransform"))
    {
      this->InitializeUsingInitialComboTransform(X,Y,VX,(vtkpxComboTransform*)usethistransformation);
      return;
      }*/

  this->ComboTransform->SetNonLinearFirst(1);
  if (usethistransformation->IsA("vtkLinearTransform")==1)
    {
      vtkLinearTransform* tr=(vtkLinearTransform*)usethistransformation;
      
      float y[3],ty[3];
      int nump_y=Y->GetSize()[0];
      for (int i=0;i<nump_y;i++)
	{
	  for (int ia=0;ia<3;ia++)
	    y[ia]=Y->GetDirectElement(i,ia);
	  
	  tr->GetLinearInverse()->TransformPoint(y,ty);
	  for (int ib=0;ib<3;ib++)
	    Y->SetDirectElement(i,ib,ty[ib]);
	}
      fprintf(stderr,"Used Linear Initial Transform inside vtkpxBSplineRPMRegistration");
      this->ComboTransform->GetLinearTransform()->GetMatrix()->DeepCopy(((vtkLinearTransform*)usethistransformation)->GetMatrix());
      this->ComboTransform->SetNonLinearFirst(1);
    }
  else
    {
      fprintf(stderr,"Using Non-Linear Initial Transform inside vtkpxBSplineRPMRegistration %s %d\n",
	      usethistransformation->GetClassName(),usethistransformation->IsA("vtkTransform"));
      float x[3],tx[3];
      int nump_x=X->GetSize()[0];
      for (int i=0;i<nump_x;i++)
	{
	  for (int ia=0;ia<3;ia++)
	    x[ia]=X->GetDirectElement(i,ia);
	  
	  usethistransformation->TransformPoint(x,tx);

	  for (int ib=0;ib<3;ib++)
	    VX->SetDirectElement(i,ib,tx[ib]);
	}
    }
}
//----------------------------------------------------------------------------
// Description:
// Annealing Procedure
//----------------------------------------------------------------------------
void vtkpxBSplineRPMRegistration::Anneal()
{
  fprintf(stderr,"\n\n\n\n --------------------------------\n Beginning Anneal \n ---------------------------- \n");

  int perT_maxit=2;
  //int relax_maxit=1;
  float anneal_rate=this->AnnealRate;
 
  // Initialize Temperature Stuff 
  float T      =Frange(this->InitialTemperature,0.1,500);
  float T_final=Frange(this->FinalTemperature,0.01,T);

  int   totaliter=int(fabs(log(T/T_final)/log(anneal_rate))+1.0);
  fprintf(stderr,"T=%f, T_final=%f totaliter=%d anneal_rate=%.3f\n",T,T_final,totaliter,anneal_rate);


  vtkpxMatrix* X=vtkpxMatrix::New();
  vtkpxMatrix* Y=vtkpxMatrix::New();
  vtkpxMatrix* LX=vtkpxMatrix::New();
  vtkpxMatrix* LY=vtkpxMatrix::New();

  double bounds[6]; this->Source->GetPoints()->GetBounds(bounds);
  
  if (this->MaximumNumberOfLandmarks<-1)
    {
      this->MaximumNumberOfLandmarks=abs(this->MaximumNumberOfLandmarks);
      this->PreferentialSampling=1;
    }

  if (this->UseAttributes==1)
    {
      this->PreferentialSampling=0;
      this->UseLabels=0;
    }


  this->SamplePoints(X,Y,LX,LY,this->MaximumNumberOfLandmarks,this->PreferentialSampling);


  int sz[2],nump_x,nump_y;
  X->GetSize(sz);   nump_x=sz[0];
  Y->GetSize(sz);   nump_y=sz[0];
  vtkDebugMacro(<< "Anneal: Using " << nump_x << " , " << nump_y << "Landmarks UseLabels="<< this->UseLabels << " PrefSampling=" << this->PreferentialSampling << "\n");

  vtkpxAbstractMatrix* M=this->AllocateMatchMatrix(this->MatchMatrixType);
  vtkpxMatrix* M_Outliers_Row=vtkpxMatrix::New();
  vtkpxMatrix* M_Outliers_Column=vtkpxMatrix::New();
  this->InitMatchMatrices(M,M_Outliers_Row,M_Outliers_Column,nump_x,nump_y);
  

  // Initialize Transformed X and Y stuff 
  vtkpxMatrix* VX=vtkpxMatrix::New();   VX->Copy(X);
  vtkpxMatrix* VY=vtkpxMatrix::New();   VY->Copy(X);

  vtkCollection* collection=vtkCollection::New();

  if (this->FinalSmoothnessFactor>this->InitialSmoothnessFactor)
    {
      float a=this->FinalSmoothnessFactor;
      this->FinalSmoothnessFactor=this->InitialSmoothnessFactor;
      this->InitialSmoothnessFactor=a;
    }

  float bsmooth=this->InitialSmoothnessFactor;
  float bsmooth_final=this->FinalSmoothnessFactor;
  float bsmooth_annealrate=exp(-fabs(log(bsmooth/bsmooth_final)/float(totaliter-1.0)));
  fprintf(stderr,"Bsmooth = %f Bsmoothfinal=%f totaliter=%d annealrate=%f\n",bsmooth,bsmooth_final,totaliter,bsmooth_annealrate);

  float cps=Frange(this->InitialControlPointSpacing,0.1,500);
  float cps_final=Frange(this->FinalControlPointSpacing,0.01,cps);

  this->InitialControlPointSpacing=cps;
  this->FinalControlPointSpacing=cps_final;

  float cps_annealrate=exp(-fabs(log(cps/cps_final)/float(totaliter-1.0)));
  float controlpointspacing=cps;

  // Annealing Procedure 
  int flag_stop=0;
  int iter=1;


  if (this->InitialTransform!=NULL && this->UseInitialTransform>0 )
    {
      this->InitializeUsingInitialTransform(X,Y,VX,this->InitialTransform);
      fprintf(stderr,"Initialized Using Initial Transform Done\n");

      /*      if (this->InitialTransform->IsA("vtkLinearTransform")==0)
	{
	  M_Outliers_Column->Fill(1.0);
	  fprintf(stderr,"Fitting Initial Transformation\n\n\n ****");
	  this->GenerateTransformation(this->GridTransform,X,VX,LX,
				       controlpointspacing,bounds,2.0,
				       collection,NULL,0);
				       }*/
    }
  else
    fprintf(stderr,"Not Using Initial Transform inside vtkpxBSplineRPMRegistration");

  vtkPointLocator* locator=this->CreateLocator(Y);


  while (flag_stop == 0 )
    {
      for (int i=1;i<=perT_maxit;i++)
	{
	  if (this->AbortExecute>0)
	    {
	      i=perT_maxit+1;
	    }
	  else
	    {
	      this->UpdateCorrespondence(M,M_Outliers_Row,M_Outliers_Column,
					 VX,Y,VY,T,locator,LX,LY);
	      this->GenerateTransformation(this->GridTransform,X,VY,LX,
					   controlpointspacing,bounds,bsmooth,
					   collection,M_Outliers_Column);
	      this->TransformPoints(this->GridTransform,X,VX);
	    }
	}

      vtkImageData* grid=this->GridTransform->GetDisplacementGrid();
      vtkDebugMacro(<<"\n\n----------------------------------------------------------\nIteration " << iter << "/" << totaliter << " T = " << T << "Cps= " << controlpointspacing << ", abort= " << this->AbortExecute << "\n\t" << "Grid Dimensions"<< grid->GetDimensions()[0] << " x " <<  grid->GetDimensions()[1] << " x " << grid->GetDimensions()[2] << " Num DOFs = " << this->GridTransform->GetNumberOfControlPoints() << " Bsmooth= " << bsmooth);
          
      this->UpdateProgress(float(iter)/totaliter);
      iter++;
      T=T*anneal_rate;
      controlpointspacing*=cps_annealrate;
      bsmooth*=bsmooth_annealrate;
      if (T<T_final || this->AbortExecute>0)
	flag_stop=1;
    }
  
  if (this->PreferentialSampling==1 &&   M_Outliers_Column!=NULL && ( this->MatchMode==2 || this->MatchMode==4) )
    {
      int dolabels=0;
      if (LX!=NULL && LY!=NULL && this->UseLabels>0)
	dolabels=1;
      if (dolabels==1)
	{
	  int nt=0;
	  for (int i=0;i<nump_x;i++)
	    {
	      if (LX->GetDirectElement(i,0)==1)
		{
		  M_Outliers_Column->SetDirectElement(i,0,0.0);
		  ++nt;
		}
	    }
	  fprintf(stderr," %d / %d outlier weights set to 0 \n",nt,nump_x);
	  float s=float(M_Outliers_Column->GetSize()[0])/M_Outliers_Column->Sum();
	  M_Outliers_Column->Scale(s);
	}
    }

  this->GridTransform->FormOutputWeights(collection,M_Outliers_Column,this->FinalWeights);

  vtkDebugMacro(<<"Creating Final Transformation");
  this->InvokeEvent(vtkCommand::EndEvent,NULL);

  // Cleanup Stuff 
  if (locator!=NULL)    locator->Delete();
  VX->Delete();         VY->Delete();
  M->Delete();          M_Outliers_Row->Delete();
  X->Delete();          M_Outliers_Column->Delete();
  Y->Delete();          
  LX->Delete();         LY->Delete();
  collection->Delete();
  this->CurrentControlPointSpacing=-1.0;

}
//----------------------------------------------------------------------------

