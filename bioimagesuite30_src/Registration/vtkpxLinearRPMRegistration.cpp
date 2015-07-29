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
  Module:    $RCSfile: vtkpxRPMTransform.cpp,v $
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
#include "vtkpxWeightedLandmarkTransform.h"
#include "vtkTransform.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkpxLinearRPMRegistration.h"
#include "vtkpxMatrix.h"
#include "vtkpxUtil.h"
#include "vtkIdList.h"
#include "vtkFloatArray.h"
#include "vtkPointLocator.h"
//-----------------------------------------------------------------------------------

vtkpxLinearRPMRegistration* vtkpxLinearRPMRegistration::New()
{
  // First try to create the object from the vtkObjectFactory

  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxLinearRPMRegistration");
  if(ret)
    {
    return (vtkpxLinearRPMRegistration*)ret;
    }

  // If the factory was unable to create the object, then create it here.

  return new vtkpxLinearRPMRegistration;
}

//----------------------------------------------------------------------------

vtkpxLinearRPMRegistration::vtkpxLinearRPMRegistration()
{
  this->ForwardTransform = vtkTransform::New();
  this->ForwardTransform->Identity();
  this->TransformMode=2;
}

//----------------------------------------------------------------------------

vtkpxLinearRPMRegistration::~vtkpxLinearRPMRegistration()
{
  this->ForwardTransform->Delete();
}

//----------------------------------------------------------------------------
void vtkpxLinearRPMRegistration::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkpxAbstractRPMRegistration::PrintSelf(os,indent);
  os << indent << "TransformMode:  " << this->TransformMode << "\n";
  os << indent << "Transformation: " << this->ForwardTransform << "\n";
}

//----------------------------------------------------------------------------
vtkAbstractTransform* vtkpxLinearRPMRegistration::GetCurrentTransformation()
{
  return this->ForwardTransform;
}
//----------------------------------------------------------------------------
// Transformation Stuff
//----------------------------------------------------------------------------
void vtkpxLinearRPMRegistration::GenerateTransformation(vtkTransform* transform,
							vtkpxMatrix* x,vtkpxMatrix* vy,int mode,vtkpxMatrix* weights,
							vtkpxMatrix* labels)
{
  vtkPoints* ptx=this->CreatePointsFromMatrix(x);
  vtkPoints* pty=this->CreatePointsFromMatrix(vy);

  vtkpxWeightedLandmarkTransform* tr=vtkpxWeightedLandmarkTransform::New();
  tr->SetSourceLandmarks(ptx);
  tr->SetTargetLandmarks(pty);
  tr->SetWeights(weights);
  if (mode==0)
    tr->SetModeToRigidBody();
  else if (mode==1)
    tr->SetModeToSimilarity(); 
  else
    tr->SetModeToAffine();

  tr->Modified();
  tr->Update();
    
  transform->Identity();
  transform->PostMultiply();
  transform->Concatenate(tr);

  tr->Delete();
  ptx->Delete();
  pty->Delete();
}

//----------------------------------------------------------------------------
// Description:
// Annealing Procedure
//----------------------------------------------------------------------------
void vtkpxLinearRPMRegistration::Anneal()
{
  int perT_maxit=5;
  float anneal_rate=this->AnnealRate;

  float T,T_final;
  // Initialize Temperature Stuff 
  T      =Frange(this->InitialTemperature,0.1,500);
  T_final=Frange(this->FinalTemperature,0.01,T);
  

  float totaliter=fabs(log(T/T_final)/log(anneal_rate))+1.0;

  fprintf(stderr,"T=%f, T_final=%f totaliter=%.0f anneal_rate=%.3f (orig=%.2f:%.2f)\n",T,T_final,totaliter,anneal_rate,
	  this->InitialTemperature,this->FinalTemperature);

  this->Initialize(NULL);

  // Centroid Initialization:
  double sumx[3] = { 0.0,0.0,0.0};
  double sumy[3] = { 0.0,0.0,0.0};
  int sz[2],nump_x,nump_y;
  VX->GetSize(sz);   nump_x=sz[0];
  Y->GetSize(sz);   nump_y=sz[0];
  for (int i=0;i<nump_x;i++)
    for (int ia=0;ia<3;ia++)
      sumx[ia]+=VX->GetDirectElement(i,ia);
  
  for (int i=0;i<nump_y;i++)
    for (int ia=0;ia<3;ia++)
      sumy[ia]+=Y->GetDirectElement(i,ia);

  double shift[3];
  for (int ia=0;ia<3;ia++)
    {
      sumx[ia]/=double(nump_x);
      sumy[ia]/=double(nump_y);
      shift[ia]=sumy[ia]-sumx[ia];
    }

  fprintf(stderr,"\t\t Linear Initialization ::: Initial Shifts = (%.1f,%.1f,%.1f)\n",shift[0],shift[1],shift[2]);

  for (int i=0;i<nump_x;i++)
    for (int ia=0;ia<3;ia++)
      VX->SetDirectElement(i,ia,VX->GetDirectElement(i,ia)+shift[ia]);

  // Annealing Procedure 
  int flag_stop=0;
  int iter=1;

  perT_maxit=1;

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
					 VX,Y,VY,T,this->Locator,LX,LY);
	      this->GenerateTransformation(this->ForwardTransform,X,VY,this->TransformMode,M_Outliers_Column,LX);
	      this->TransformPoints(this->ForwardTransform,X,VX);
	    }
	}

      vtkDebugMacro(<<"\n----------------------------------------------------------\nIteration " << iter << "/" << totaliter << " T = " << T << ", abort= " << this->AbortExecute << "\n");


      this->UpdateProgress(float(iter)/totaliter);
      iter++;
      T=T*anneal_rate;
      if (T<T_final || this->AbortExecute>0)
	flag_stop=1;
    }
  
  this->InvokeEvent(vtkCommand::EndEvent,NULL);
  this->UpdateVYPoints();

  
}
//----------------------------------------------------------------------------




