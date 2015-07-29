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

#include "vtkDataSet.h"
#include "vtkFloatArray.h"
#include "vtkIdList.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkOldStyleCallbackCommand.h"
#include "vtkPointLocator.h"
#include "vtkTransform.h"
#include "vtkTriangleFilter.h"
#include "vtkpxMath.h"
#include "vtkpxMatrix.h"
#include "vtkpxRPMCorrespondenceFinder.h"
#include "vtkpxSparseMatrix.h"
#include "vtkpxUtil.h"
#include "vtkpxWeightedLandmarkTransform.h"
#include "pxutil.h"
//-----------------------------------------------------------------------------------

vtkpxRPMCorrespondenceFinder* vtkpxRPMCorrespondenceFinder::New()
{
  // First try to create the object from the vtkObjectFactory

  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxRPMCorrespondenceFinder");
  if(ret)
    {
    return (vtkpxRPMCorrespondenceFinder*)ret;
    }

  // If the factory was unable to create the object, then create it here.

  return new vtkpxRPMCorrespondenceFinder;
}

//----------------------------------------------------------------------------

vtkpxRPMCorrespondenceFinder::vtkpxRPMCorrespondenceFinder()
{
  M=NULL; M_Outliers_Row=NULL; M_Outliers_Column=NULL;   
  X=NULL; VX=NULL; Y=NULL; VY=NULL; LX=NULL; LY=NULL;                  
  Locator=NULL; PointsX=NULL;  PointsVY=NULL;
  Temperature=0.02;

  this->Source = NULL;
  this->Target = NULL;


  this->EnableFeedback=0;

  this->MaximumNumberOfLandmarks = 2000;

  this->MatchMode=1;
 
  this->UseLabels=1;
  this->UseAttributes=0;
  this->FastThreshold=2.0;
  this->ExtraDebug=0;

  this->InitialTransform=NULL;
  this->UseInitialTransform=0;

  this->UseWeightedLeastSquares=1;
  this->MatchMatrixType=-1;
  this->PreferentialSampling=0;
  this->AttributesWeight=0.1;
}

//----------------------------------------------------------------------------

vtkpxRPMCorrespondenceFinder::~vtkpxRPMCorrespondenceFinder()
{
  this->CleanUp();

  ReleaseSource();
  ReleaseTarget();
  this->SetInitialTransform(NULL);

}

void vtkpxRPMCorrespondenceFinder::CleanUp()
{
  cleanobject(M);    cleanobject(M_Outliers_Row);    cleanobject(M_Outliers_Column);
  cleanobject(X);    cleanobject(VX);    cleanobject(Y);    cleanobject(VY);
  cleanobject(LX);   cleanobject(LY);    cleanobject(Locator);
  cleanobject(PointsX);   
  cleanobject(PointsVY);


  M=NULL; M_Outliers_Row=NULL; M_Outliers_Column=NULL;   
  X=NULL; VX=NULL; Y=NULL; VY=NULL; LX=NULL; LY=NULL;                  
  Locator=NULL; PointsX=NULL;  PointsVY=NULL;
}

void vtkpxRPMCorrespondenceFinder::SetSource(vtkPolyData *source)
{
  if (this->Source == source)
      return;
  
  if (this->Source)
    this->ReleaseSource();
  
  if (source)
    source->Register(this);
  
  this->Source = source;
  this->Modified();
}

//----------------------------------------------------------------------------

void vtkpxRPMCorrespondenceFinder::ReleaseSource(void) 
{
  if (this->Source) 
    {
      this->Source->UnRegister(this);
      this->Source = NULL;
    }
}

//----------------------------------------------------------------------------

void vtkpxRPMCorrespondenceFinder::SetTarget(vtkPolyData *target)
{
  if (this->Target == target)
    return;
  
  if (this->Target)
    this->ReleaseTarget();
  
  if (target)
    target->Register(this);
  
  this->Target = target;
  this->Modified();
}

//----------------------------------------------------------------------------

void vtkpxRPMCorrespondenceFinder::ReleaseTarget(void) 
{
  if (this->Target) 
    {
      this->Target->UnRegister(this);
      this->Target = NULL;
    }
}

//----------------------------------------------------------------------------
void vtkpxRPMCorrespondenceFinder::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkProcessObject::PrintSelf(os,indent);

  if ( this->Source ) 
    {
      os << indent << "Source: " << this->Source << "\n";
    }
  else 
    {
      os << indent << "Source: (none)\n";
    }
  
  if ( this->Target ) 
    {
      os << indent << "Target: " << this->Target << "\n";
    }
  else 
    {
      os << indent << "Target: (none)\n";
    }
  
  os << indent << "MaximumNumberOfLandmarks: " << this->MaximumNumberOfLandmarks << "\n";
  os << indent << "MatchMode: " << this->MatchMode << "\n";
}


//----------------------------------------------------------------------------
void vtkpxRPMCorrespondenceFinder::Initialize(vtkAbstractTransform* OffsetTransform)
{
  this->CleanUp();
  fprintf(stderr,"\n\n\n\n --------------------------------\n External Initialization \n ----------------------- \n");

  // Sample Points 
  X=vtkpxMatrix::New(); LX=vtkpxMatrix::New();
  Y=vtkpxMatrix::New(); LY=vtkpxMatrix::New();
  this->SamplePoints(X,Y,LX,LY,this->MaximumNumberOfLandmarks,this->PreferentialSampling);

  int sz[2],nump_x,nump_y;
  X->GetSize(sz);   nump_x=sz[0];
  Y->GetSize(sz);   nump_y=sz[0];
  vtkDebugMacro(<< "Initialization: Using " << nump_x << " , " << nump_y << "Landmarks UseLabels="<< this->UseLabels << "or Attributes " << this->UseAttributes << "\n");

  // Initialize Match Matrices 
  this->M=this->AllocateMatchMatrix();
  M_Outliers_Row=vtkpxMatrix::New();
  M_Outliers_Column=vtkpxMatrix::New();
  this->InitMatchMatrices(M,M_Outliers_Row,M_Outliers_Column,nump_x,nump_y);
  
  // Initialize Transformed X and Y stuff 
  VX=vtkpxMatrix::New();   VX->Copy(X);
  VY=vtkpxMatrix::New();   VY->Copy(X);


  int useinit=(this->UseInitialTransform>0);
  if (useinit ==1 && this->InitialTransform==NULL)
    useinit=0;

  int useoffset=0;
  if (OffsetTransform!=NULL)
    useoffset=1;

  if ( useoffset > 0 || useinit > 0 )
    {
      float ox[3],x[3],tx[3];
      for (int i=0;i<nump_x;i++)
	{
	  for (int ia=0;ia<3;ia++)
	    ox[ia]=VX->GetDirectElement(i,ia);
	
	  if (useoffset)
	    {
	      OffsetTransform->TransformPoint(ox,ox);
	      for (int ib=0;ib<3;ib++)
		X->SetDirectElement(i,ib,ox[ib]);
	    }

	  if (useinit > 0 ) 
	    {
	      this->InitialTransform->TransformPoint(x,tx);
	      for (int ib=0;ib<3;ib++)
		VX->SetDirectElement(i,ib,tx[ib]);
	    }
	  else 
	    {
	      for (int ib=0;ib<3;ib++)
		VX->SetDirectElement(i,ib,ox[ib]);
	    }
	}
      
      M_Outliers_Column->Fill(1.0);
    }

  this->Locator=this->CreateLocator(this->Y);
  this->Collection=vtkCollection::New();
  
  int np=this->X->GetSize()[0];
  
  this->PointsX=vtkPoints::New();
  this->PointsX->SetNumberOfPoints(np);
  this->PointsVY=vtkPoints::New();
  this->PointsVY->SetNumberOfPoints(np);
  
  
  for (int i=0;i<np;i++)
    {
      float x[3];
      for (int ia=0;ia<=2;ia++)
	x[ia]=this->X->GetDirectElement(i,ia);
      
      this->PointsX->SetPoint(i,x);
      this->PointsVY->SetPoint(i,x);
    }
}

//----------------------------------------------------------------------------
void vtkpxRPMCorrespondenceFinder::WarpAndUpdateCorrespondence(vtkAbstractTransform* warpTransform,
							       float temperature)

{
  this->TransformPoints(warpTransform,this->X,this->VX);
  
  this->SetTemperature(temperature);
  
  // Somewhere here do annealing etc

  this->UpdateCorrespondence(this->M,
			     this->M_Outliers_Row,
			     this->M_Outliers_Column,
			     this->VX,
			     this->Y,
			     this->VY,
			     this->Temperature,
			     this->Locator,
			     this->LX,
			     this->LY);

  /*  this->X->Print("X");
  this->Y->Print("Y");
  this->VX->Print("VX");
  this->VY->Print("VY");
  this->M->Print("M");
  this->M_Outliers_Column->Print("OC");
  this->M_Outliers_Row->Print("OR");
  */

  this->UpdateVYPoints();

  /*  if (this->M_Outliers_Column!=NULL)
    {
      int np=this->M_Outliers_Column->GetSize()[0];
      float sum=0.0,sum2=0.0,vmin=1.0e+30,vmax=-1.0e+30;
      int numlow=0,nummed=0,numhigh=0;
      for (int i=0;i<np;i++)
	{
	  float v=M_Outliers_Column->GetElement(i,0);
	  vmin=Fmin(v,vmin);
	  vmax=Fmax(v,vmax);
	  if (v<0.75)
	    ++numlow;
	  else if (v>1.25)
	    ++numhigh;
	  else
	    ++nummed;
	  sum+=v;
	  sum2+=v*v;
	}

      float mean=sum/float(np);
      float std= sqrt(sum2/float(np) - mean*mean);
      fprintf(stderr,"Outlier Range=%.2f:%.2f mean=%.2f std=%.2f (low=%.2f,med=%.2f high=%.2f)\n",
	      vmin,vmax,mean,std,
	      100.0*float(numlow)/float(np),
	      100.0*float(nummed)/float(np),
	      100.0*float(numhigh)/float(np));
	      }*/
}
//----------------------------------------------------------------------------
void vtkpxRPMCorrespondenceFinder::UpdateVYPoints()
{
  int np=this->VY->GetSize()[0];

  for (int i=0;i<np;i++)
    {
      float y[3];
      for (int ia=0;ia<=2;ia++)
	y[ia]=this->VY->GetDirectElement(i,ia);
      

      this->PointsVY->SetPoint(i,y);
    }
}
//----------------------------------------------------------------------------

void vtkpxRPMCorrespondenceFinder::MapPointsICP(vtkpxMatrix* VX,vtkpxMatrix* Y,
						vtkpxMatrix* VY,vtkPointLocator* locator)
					
{
  int size[2];
  VX->GetSize(size);

  if (this->ExtraDebug) 
    fprintf(stderr,"Forming ICP Map and Warp %d ...",size[0]);


  double x[3];
  for (int k=0;k<=2;k++)
    x[k]=0.0;

  // Find Closest point and use binary correspondece!
  for (int i=0;i<size[0];i++)
    {
      VX->GetRow(i,3,x);
      int id=locator->FindClosestPoint(x);
      Y->GetRow(id,3,x);
      VY->SetRow(i,3,x);
    }

  if (this->ExtraDebug) 
    fprintf(stderr,"Done\n");
}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
void vtkpxRPMCorrespondenceFinder::MapPointsICPPlus(vtkpxMatrix* VX,vtkpxMatrix* Y,vtkpxMatrix* VY,
						    float T,vtkPointLocator* locator)
					       
{
  int size[2];
  VX->GetSize(size);

  //float minvalue=0.1/float(size[1]);

  if (this->ExtraDebug) 
    fprintf(stderr,"Forming Fast Distance and Map %d ...",size[0]);

  int total=0;



  double p1[3],p2[3],p1h[3];
  this->ZeroArray(3,p1);
  this->ZeroArray(3,p2);
      
  vtkIdList* idlist=vtkIdList::New();

  float T2=2.0*T*T;

  for (int row=0;row<size[0];row++)
    {
      float sum=0.0;
      VX->GetRow(row,3,p1);
      this->ZeroArray(3,p1h);
           
      locator->FindPointsWithinRadius(T*this->FastThreshold,p1,idlist);
      int np=idlist->GetNumberOfIds();

      if (np<5)
	{
	  locator->FindClosestNPoints(5,p1,idlist);
	  np=idlist->GetNumberOfIds();
	}

      total+=np;

      for (int j=0;j<np;j++)
	{
	  int column=idlist->GetId(j);
	  if (column>=0)
	    {
	      Y->GetRow(column,3,p2);
	      float dist= vtkMath::Distance2BetweenPoints(p1, p2);
	      float m=exp(-dist/T2);//+(vtkpxMath::GaussianRandom()*0.001/float(size[0]));
	      /*if (m<minvalue)
		m=minvalue;*/
	      for (int ic=0;ic<3;ic++)
		p1h[ic]+=m*p2[ic];
	      sum+=m;
	    }
	}
      for (int ic=0;ic<3;ic++)
	VY->SetDirectElement(row,ic,p1h[ic]/sum);

    }
  if (this->ExtraDebug) 
    fprintf(stderr,"%d done \n",total/size[0]);
}

// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
void vtkpxRPMCorrespondenceFinder::FormDistanceMatrix(vtkpxAbstractMatrix* M,vtkpxMatrix* VX,vtkpxMatrix* Y,float T,
							 vtkpxMatrix* LX,vtkpxMatrix* LY)
{
  int size[2];
  M->GetSize(size);
  M->Zero();

  //  float minvalue=0.00000001;




  int dolabels=0;
  int numattributes=1;
  float* labelx=NULL;
  float* labely=NULL;

  if (LX!=NULL && LY!=NULL)
    {
      if (this->UseLabels>0)
	{
	  dolabels=1;
	}
      else if (this->UseAttributes>0)
	{
	  dolabels=2;
	  numattributes=LX->GetSize()[1];
	}
    }
  else
    {
      numattributes=0;
    }

  if (this->ExtraDebug) 
    fprintf(stderr,"Forming Distance Matrix %dx%d ... uselabels=%d  (numattr=%d)",size[0],size[1],dolabels,numattributes);
  

  if (dolabels>0)
    {
      labelx=new float[numattributes];
      labely=new float[numattributes];
    }

  double p1[3],p2[3];
  for (int k=0;k<=2;k++)
    {
      p1[k]=0.0;
      p2[k]=0.0;
    }

  float T2=2.0*T*T;

  for (int row=0;row<size[0];row++)
    {
      for (int ia=0;ia<3;ia++)
	p1[ia]=VX->GetDirectElement(row,ia);

      if (dolabels==1)
	{
	  labelx[0]=(int)LX->GetDirectElement(row,0);
	}
      else if (dolabels==2)
	{
	  for (int j=0;j<numattributes;j++)
	    labelx[j]=LX->GetDirectElement(row,j);
	}

      for (int column=0;column<size[1];column++)
	{
	  float adddist=0.0;
	  if (dolabels==1)
	    {
	      labely[0]=(int)LY->GetDirectElement(column,0);
	    }
	  else if (dolabels==2)
	    {
	      for (int j=0;j<numattributes;j++)
		{
		  labely[j]=LY->GetDirectElement(column,j);
		  adddist+=pow(labelx[j]-labely[j],float(2.0));
		}
	      adddist=this->AttributesWeight * adddist;
	    }
	  
	  if (dolabels!=1 || labelx[0]==labely[0])
	    {
	      p2[2]=0.0;
	      for (int ib=0;ib<3;ib++)
		p2[ib]=Y->GetDirectElement(column,ib);
	      
	      float dist= vtkMath::Distance2BetweenPoints(p1, p2) + adddist;
	      float m=exp(-dist/T2);
	      if (m>0.0000)
		M->SetDirectElement(row,column,m);
	      else
		M->SetDirectElement(row,column,0.00000);
	    }
	}
    }

  if (labelx!=NULL)
    delete [] labelx;
  
  if (labely!=NULL)
    delete [] labely;

  if (this->ExtraDebug) 
    fprintf(stderr,"done \n");
}

// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
void vtkpxRPMCorrespondenceFinder::FormDistanceMatrixFast(vtkpxAbstractMatrix* M,vtkpxMatrix* VX,vtkpxMatrix* Y,
							  float T,vtkPointLocator* locator,
							  vtkpxMatrix* LX,vtkpxMatrix* LY)
{
  int size[2];
  M->GetSize(size);
  M->Zero();

  int total=0;
  //  float minvalue=0.00000001;

  float trueThreshold=T*this->FastThreshold;
  if (trueThreshold<0.15)
    trueThreshold=0.15;



  int dolabels=0;
  int numattributes=1;

  float* labelx=NULL;
  float* labely=NULL;


  if (LX!=NULL && LY!=NULL)
    {
      if (this->UseLabels>0)
	{
	  dolabels=1;
	}
      else if (this->UseAttributes>0)
	{
	  dolabels=2;
	  numattributes=LX->GetSize()[1];
	}
    }



  if (dolabels>0)
    {
      labelx=new float[numattributes+5];
      labely=new float[numattributes+5];
    }
  else
    {
      numattributes=0;
    }
	

  double p1[3],p2[3];
  for (int k=0;k<=2;k++)
    {
      p1[k]=0.0;
      p2[k]=0.0;
    }
      

  if (this->ExtraDebug) 
    fprintf(stderr,"Forming Fast Distance Matrix %dx%d %.3f (%.3f) ... labels=%d (numattr=%d)\n",size[0],size[1],
	    trueThreshold,T*this->FastThreshold,dolabels,numattributes);
  
  vtkIdList* idlist=vtkIdList::New();
  //int count1=0,count2=0;
  float T2=2.0*T*T;
  for (int row=0;row<size[0];row++)
    {
      for (int ia=0;ia<3;ia++)
	p1[ia]=VX->GetDirectElement(row,ia);
      
      if (dolabels==1)
	{
	  labelx[0]=(int)LX->GetDirectElement(row,0);
	}
      else if (dolabels==2)
	{
	  for (int j=0;j<numattributes;j++)
	    labelx[j]=LX->GetDirectElement(row,j);
	}
      locator->FindPointsWithinRadius(trueThreshold,p1,idlist);
      int np=idlist->GetNumberOfIds();

      if (np<3)
	{
	  locator->FindClosestNPoints(3,p1,idlist);
	  np=idlist->GetNumberOfIds();

	}
      
      total+=np;
      double sum=0.0;

      for (int j=0;j<np;j++)
	{
	  int column=idlist->GetId(j);
	  float adddist=0.0;

	  if (dolabels==1)
	    {
	      labely[0]=(int)LY->GetDirectElement(column,0);
	    }
	  else if (dolabels==2)
	    {
	      for (int j=0;j<numattributes;j++)
		{
		  labely[j]=LY->GetDirectElement(column,j);
		  adddist+=pow(labelx[j]-labely[j],float(2.0));
		}
	      adddist=this->AttributesWeight * adddist;
	    }

	  if (dolabels!=1 || labelx[0]==labely[0])
	    {
	      p2[2]=0.0;
	      for (int ib=0;ib<3;ib++)
		p2[ib]=Y->GetDirectElement(column,ib);
	      
	      float dist= vtkMath::Distance2BetweenPoints(p1, p2) + adddist;
	      float m=exp(-dist/T2);//+(vtkpxMath::GaussianRandom()*0.001/float(size[0]));
	      sum+=m;
	      if (m>0.0)
		M->SetDirectElement(row,column,m);
	      else
		M->SetDirectElement(row,column,0.0);
	    }
	}

      if (sum<0.0001)
	{
	  int id=locator->FindClosestPoint(p1);
	  //	  fprintf(stderr,"Adding 0.001 at %d %d\n",row,id);
	  M->SetDirectElement(row,id,0.0001);
	}
    }


  if (labelx!=NULL)
    delete [] labelx;
  if (labely!=NULL)
    delete [] labely;

  if (this->ExtraDebug) 
    fprintf(stderr,"%d done \n",total/size[0]);
}


// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
vtkPoints* vtkpxRPMCorrespondenceFinder::CreatePointsFromMatrix(vtkpxMatrix* Y,int sample)
{
  int size[2];
  Y->GetSize(size);
  
  sample=Irange(sample,1,10);

  int nump=size[0]/sample;

  vtkPoints* pt=vtkPoints::New();
  pt->SetNumberOfPoints(nump);

  int i;

  for (i=0;i<nump;i++)
    {
      int index=i*sample;
      pt->SetPoint(i,Y->GetDirectElement(index,0),Y->GetDirectElement(index,1),Y->GetDirectElement(index,2));
    }

  
  return pt;

}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
vtkpxMatrix* vtkpxRPMCorrespondenceFinder::CreateMatrixFromPoints(vtkPoints* pts)
{
  int nump=pts->GetNumberOfPoints();

  vtkpxMatrix* Y=vtkpxMatrix::New();
  Y->Zeros(nump,4);
  double p[3];

  for (int i=0;i<nump;i++)
    {
      pts->GetPoint(i,p);
      for (int ia=0;ia<=2;ia++)
	Y->SetDirectElement(i,ia,p[ia]);
      Y->SetElement(i,3,1);
    }

  return Y;

}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
vtkPointLocator* vtkpxRPMCorrespondenceFinder::CreateLocator(vtkpxMatrix* Y,int pointsperbucket)
{
  vtkPointLocator* locator=vtkPointLocator::New();
  locator->SetNumberOfPointsPerBucket(1);
  vtkPoints* pt=this->CreatePointsFromMatrix(Y);
  double bounds[6];
  pt->GetBounds(bounds);

  vtkPolyData* poly=vtkPolyData::New();
  poly->SetPoints(pt);

  locator->SetDataSet(poly);
  locator->BuildLocator();
  pt->Delete();
  poly->Delete();

  int dat[3];
  locator->GetDivisions(dat);
  if (this->ExtraDebug) 
    fprintf(stderr,"Locator number of buckets = %d (%d,%d,%d)\n",locator->GetNumberOfPointsPerBucket(),dat[0],dat[1],dat[2]);
	  

  return locator;
}

void vtkpxRPMCorrespondenceFinder::ZeroArray(int num,float *x)
{
  for (int i=0;i<num;i++)
    x[i]=0.0;
}

void vtkpxRPMCorrespondenceFinder::ZeroArray(int num,double *x)
{
  for (int i=0;i<num;i++)
    x[i]=0.0;
}

//----------------------------------------------------------------------------
// Higher Level Procedures 
//----------------------------------------------------------------------------
void vtkpxRPMCorrespondenceFinder::UpdateCorrespondence(vtkpxAbstractMatrix* m,
							vtkpxMatrix* m_outliers_row,vtkpxMatrix* m_outliers_column,
							vtkpxMatrix* vx,vtkpxMatrix* y,vtkpxMatrix* vy,
							float T,vtkPointLocator* locator,
							vtkpxMatrix* lx,vtkpxMatrix* ly)
{
  if (this->ExtraDebug)
    m->DebugOn();

  char line[30];

  this->SetOutliers(m_outliers_row,m_outliers_column,T);


  switch (MatchMode)
    {
    case 0:
      this->MapPointsICP(vx,y,vy, locator);
      sprintf(line,"ICP");
      break;
    case 1:
      this->FormDistanceMatrix(m,vx,y,T,lx,ly);
      sprintf(line,"Dense");
      m->NormalizeMixtureMatrix(m_outliers_row);
      m->MapPoints(y,vy);
      break;
    case 2:
      this->FormDistanceMatrix(m,vx,y,T,lx,ly);
      sprintf(line,"Dense");
      m->NormalizeRPMMatrix(m_outliers_row,m_outliers_column);
      m->MapPoints(y,vy);
      break;
    case 3:
      this->FormDistanceMatrixFast(m,vx,y,T,locator,lx,ly);
      sprintf(line,"Fast");
      m->NormalizeMixtureMatrix(m_outliers_row);
      m->MapPoints(y,vy);
      break;
    case 4:
      this->FormDistanceMatrixFast(m,vx,y,T,locator,lx,ly);
      sprintf(line,"Fast");
      m->NormalizeRPMMatrix(m_outliers_row,m_outliers_column);
      m->MapPoints(y,vy);
      break;
    case 5:
      this->MapPointsICPPlus(vx,y,vy,T, locator);
      sprintf(line,"ICPP");
      break;
    }

  vtkDebugMacro(<<line << " Match Matrix Formed : Density = " << m->Density());

  this->TransformOutlierColumnToWeights(m_outliers_column,this->MatchMode,this->UseWeightedLeastSquares);

  if (m_outliers_column!=NULL && ( this->MatchMode==2 || this->MatchMode==4) )
    {
      int dolabels=0;
      if (lx!=NULL && this->UseLabels>0)
	dolabels=1;

      float lab[50][2],sumlab[50];
      int numlab[50];
      for (int cnt=0;cnt<50;cnt++)
	{
	  lab[cnt][0]=1000.0;
	  lab[cnt][1]=0.0;
	  sumlab[cnt]=0.0;
	  numlab[cnt]=0;
	}
      
      float range[2];
      float v=m_outliers_column->GetDirectElement(0,0);
      range[0]=range[1]=v;
      int np=m_outliers_column->GetSize()[0];
      for (int i=0;i<np;i++)
	{
	  float v=m_outliers_column->GetElement(i,0);
	  range[0]=Fmin(range[0],v);
	  range[1]=Fmax(range[1],v);
	  if (dolabels)
	    {
	      int lv=(int)lx->GetElement(i,0);
	      if (lv<50 && lv>=0)
		{		      
		  sumlab[lv]+=v;
		  numlab[lv]+=1;
		  lab[lv][0]=Fmin(v,lab[lv][0]);
		  lab[lv][1]=Fmax(v,lab[lv][1]);
		}
	    }
	}
      fprintf(stderr,"Outlier Range=%.2f %.2f\n",range[0],range[1]);
      {   
	for (int cnt=0;cnt<50;cnt++)
	  {
	    if (lab[cnt][0]!=1000.0 || lab[cnt][1]!=0.0)
	      {
		fprintf(stderr,"Label %d range = %.2f:%.2f  num=%d mean=%.2f\n",cnt,
			lab[cnt][0],lab[cnt][1],
			numlab[cnt],sumlab[cnt]/float(numlab[cnt]));
	      }
	  }
      }
      fprintf(stderr,"\n");
    }
  
  if (this->ExtraDebug)
    m->DebugOff();

}
//----------------------------------------------------------------------------
// Description:
// Annealing Helper Procedures
//----------------------------------------------------------------------------
void vtkpxRPMCorrespondenceFinder::SamplePoints(vtkpxMatrix* X,vtkpxMatrix* Y,vtkpxMatrix* LX,vtkpxMatrix* LY,int numlandmarks,
						     int preferentialsampling)
{
  if (numlandmarks==-1)
    numlandmarks=this->MaximumNumberOfLandmarks;

  if (this->ExtraDebug)
    fprintf(stderr,"Sample Only numlandmarks=%d %d Preferential=%d, Use Attributes=%d UseLabels=%d\n",numlandmarks,this->MaximumNumberOfLandmarks,
	    preferentialsampling,this->UseAttributes,this->UseLabels);

  if (this->UseAttributes==1)
    this->UseLabels=0;

  int stepx = 1,stepy=1;
  int nump_x,nump_y;

  int numattributes=0;

  if ( (this->UseLabels>0 || this->UseAttributes>0) && LX!=NULL && LY!=NULL)
    {
      if (Source->GetPointData()!=NULL && Target->GetPointData()!=NULL)
	{
	  if (Source->GetPointData()->GetScalars()!=NULL  && Target->GetPointData()->GetScalars()!=NULL)
	    {
	      if ( (Source->GetPointData()->GetScalars()->GetNumberOfTuples()==Source->GetNumberOfPoints()) &&
		   (Target->GetPointData()->GetScalars()->GetNumberOfTuples()==Target->GetNumberOfPoints()) )
		numattributes=1;
	      else
		this->UseLabels=0;
	      
	      if (numattributes>0 && this->UseAttributes==1)
		{
		  numattributes=Source->GetPointData()->GetScalars()->GetNumberOfComponents();
		  if (numattributes>Target->GetPointData()->GetScalars()->GetNumberOfComponents())
		    numattributes=Target->GetPointData()->GetScalars()->GetNumberOfComponents();
		}
	    }
	}
      else
	{
	  this->UseLabels=0;
	}
    }
  else
    {
      this->UseLabels=0;
    }

      

  int maxlan=Imax(Source->GetNumberOfPoints(),Target->GetNumberOfPoints());

  if (maxlan > numlandmarks)
    {
      stepx = this->Source->GetNumberOfPoints() / numlandmarks;
      if (stepx<1)
	stepx=1;
      nump_x=this->Source->GetNumberOfPoints()/stepx;

      stepy = this->Target->GetNumberOfPoints() / numlandmarks;
      if (stepy<1)
	stepy=1;
      nump_y=this->Target->GetNumberOfPoints()/stepy;
    }
  else 
    {
      stepx=1;
      stepy=1;
      nump_x=this->Source->GetNumberOfPoints();
      nump_y=this->Target->GetNumberOfPoints();
    }


  X->Zeros(nump_x,4);
  Y->Zeros(nump_y,4);

  if (numattributes>0)
    {
      LX->Zeros(nump_x,numattributes);
      LY->Zeros(nump_y,numattributes);
    }
  else
    this->UseLabels=0;

  int done=0;

  if (numattributes==1  && this->UseLabels==1 && preferentialsampling==1)
    {
      done=DoPreferentialSampling(this->Source,X,LX,nump_x);
      if (done>0)
	done=DoPreferentialSampling(this->Target,Y,LY,nump_y);
    }

  if (done==0)
    {
      if (this->ExtraDebug)
	fprintf(stderr,"Doing Straight Sampling\n");
      DoStraightSampling(this->Source,X,LX,nump_x,stepx,(numattributes>0));
      DoStraightSampling(this->Target,Y,LY,nump_y,stepy,(numattributes>0));
    }
}

// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
// Sampling Stats
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 

void vtkpxRPMCorrespondenceFinder::DoStraightSampling(vtkPolyData* surface,vtkpxMatrix* X,vtkpxMatrix* LX,int nump,int step,int dolabels)
{
  double tpt[3];
  int numattributes=LX->GetSize()[1];

  for (int i=0;i<nump;i++)
    {
      surface->GetPoint(i*step,tpt);

      for (int ia=0;ia<3;ia++)
	X->SetElement(i,ia,tpt[ia]);
      X->SetElement(i,3,1.0);
      if (dolabels)
	{
	  for (int j=0;j<numattributes;j++)
	    LX->SetElement(i,j,surface->GetPointData()->GetScalars()->GetComponent(i*step,j));
	}
    }
}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
int vtkpxRPMCorrespondenceFinder::DoPreferentialSampling(vtkPolyData* surface,vtkpxMatrix* X,vtkpxMatrix* LX,int nump)
{
  if (this->ExtraDebug)
    fprintf(stderr,"In Do Preferential Sampling nump=%d\n",nump);


  vtkDataArray* in_labels=surface->GetPointData()->GetScalars();
  int numlab=in_labels->GetNumberOfTuples();
  double range[2]; in_labels->GetRange(range);

  int upper=0,flag=0;
  while (upper<numlab && flag==0)
    {
      if ( fabs(in_labels->GetComponent(upper,0)-range[0])<0.01)
	++upper;
      else
	flag=1;
    }

  if (flag==0)
    {
      if (this->ExtraDebug)
	fprintf(stderr,"No Second Labels Found in Preferential Sampling\n");
      return 0;
    }


  int numupper=numlab-upper;
  int numlower=nump-numupper;


  if (numlower<0)
    {
      if (this->ExtraDebug)
	fprintf(stderr,"Not enough points found for Preferential Sampling upper=%d numlab=%d numlower=%d numupper=%d nump=%d\n",
		upper,numlab,numlower,numupper,nump);
      return 0;
    }

  
  int step=int(upper/numlower);
  if (step<1)
    step=1;
  numlower=upper/step;

  int newnump=numlower+numupper;
  if (newnump!=nump)
    {
      nump=newnump;
      X->Zeros(nump,4);
      LX->Zeros(nump,1);
    }

  double tpt[3];
  int numcomp=LX->GetSize()[1];
  for (int i=0;i<numlower;i++)
    {
      surface->GetPoint(i*step,tpt);
      for (int ia=0;ia<3;ia++)
	X->SetElement(i,ia,tpt[ia]);
      X->SetElement(i,3,1.0);
      for (int j=0;j<numcomp;j++)
	LX->SetElement(i,j,surface->GetPointData()->GetScalars()->GetComponent(i*step,j));
    }
  for (int j=numlower;j<nump;j++)
    {
      surface->GetPoint(upper+(j-numlower),tpt);
      for (int ia=0;ia<3;ia++)
	X->SetElement(j,ia,tpt[ia]);
      X->SetElement(j,3,1.0);
      for (int jj=0;jj<numcomp;jj++)
	LX->SetElement(j,jj,surface->GetPointData()->GetScalars()->GetComponent(upper+(j-numlower),jj));
    }

  if (this->ExtraDebug)
    fprintf(stderr,"Preferential Sampling : Input: out=%d tot=%d per=%.1f \t  Output: out=%d tot=%d per=%.1f\n",
	    upper,numlab,float(100.0*upper)/float(numlab),
	    numlower,nump,float(100.0*numlower)/float(nump));

  return 1;
}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
void vtkpxRPMCorrespondenceFinder::InitMatchMatrices(vtkpxAbstractMatrix* M,vtkpxMatrix* M_Outliers_Row,
						     vtkpxMatrix* M_Outliers_Column,
						     int nump_x,int nump_y)
{
  M->Zeros(nump_x,nump_y);
  M_Outliers_Row->Zeros(1,nump_y);
  M_Outliers_Column->Zeros(nump_x,1);
}
// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
float vtkpxRPMCorrespondenceFinder::SetOutliers(vtkpxMatrix* M_Outliers_Row,vtkpxMatrix* M_Outliers_Column,float T)
{
  int size[2];  M_Outliers_Column->GetSize(size);
  float moutlier=0.01;
  M_Outliers_Row->Fill(moutlier);
  if (this->MatchMode==2 || this->MatchMode==4 || this->MatchMode==7)
    M_Outliers_Column->Fill(moutlier);
  if (this->ExtraDebug)
    fprintf(stderr,"Moutlier=%f (Temperature=%.5f)\n",moutlier,T);
  
  return moutlier;
}

vtkpxAbstractMatrix* vtkpxRPMCorrespondenceFinder::AllocateMatchMatrix(int dense)
{
  if (dense==1 || (dense==-1 &&  (this->MatchMode==1 || this->MatchMode==2)))
    {
      return vtkpxMatrix::New();
    }

  return vtkpxSparseMatrix::New();
}

void vtkpxRPMCorrespondenceFinder::TransformOutlierColumnToWeights(vtkpxMatrix* M_Outliers_Column,int Mode,int UseWLS)
{
  if ( UseWLS>0 && (Mode==2 || Mode==4 || Mode==6 || Mode==7) )
    {
      M_Outliers_Column->ScaleAdd(-1.0,1.0);

      float med=M_Outliers_Column->Median();
      if (med<0.001)
	{
	  float s=float(M_Outliers_Column->GetSize()[0])/M_Outliers_Column->Sum();
	  M_Outliers_Column->Scale(s);
	}
      else
	{
	  fprintf(stderr,"Median Scaling median=%.3f\n",med);
	  M_Outliers_Column->Scale(1.0/med);
	}
    }
  else
    {
      M_Outliers_Column->Fill(1.0);
    }
  
}

// - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - -  - - - - - - - - 
void vtkpxRPMCorrespondenceFinder::TransformPoints(vtkAbstractTransform* xform,vtkpxMatrix* X,vtkpxMatrix* VX)
{
  int size[2];
  X->GetSize(size);
  
  double tx[3],ty[3];
  for (int k=0;k<=2;k++)
    tx[k]=0.0;
  
  for (int i=0;i<size[0];i++)
    {
      for (int ia=0;ia<3;ia++)
	tx[ia]=X->GetDirectElement(i,ia);
      
      xform->TransformPoint(tx,ty);
      for (int ib=0;ib<3;ib++)
	VX->SetDirectElement(i,ib,ty[ib]);
    }
  
}
//----------------------------------------------------------------------------
float vtkpxRPMCorrespondenceFinder::ComputeDistance()
{
 return this->ComputeDistance("");
}

float vtkpxRPMCorrespondenceFinder::ComputeDistance(const char* label)
{
  int dooutliers=0;
  if  (this->M_Outliers_Column!=NULL && ( this->MatchMode==2 || this->MatchMode==4) )
    dooutliers=1;

  int dolabels=0;
  if (this->LX!=NULL && this->UseLabels>0)
    dolabels=1;

  char line[150];
  if (label!=NULL)
    strncpy(line,label,150);
  else
    strcpy(line,"");
  
  vtkPointLocator* locator=vtkPointLocator::New();
  locator->SetNumberOfPointsPerBucket(1);
  locator->SetDataSet(this->Target);
  locator->BuildLocator();
  
  float sumlab[50],numlab[50];
  float sum=0,totaln=0.0;
  
  for (int cnt=0;cnt<50;cnt++)
    {
      sumlab[cnt]=0.0;
      numlab[cnt]=0.0;
    }
  
  int np=this->VX->GetSize()[0];
  for (int i=0;i<np;i++)
    {
      double p4[5],p5[3];
      this->VX->GetRow(i,3,&p4[0]);
      int id=locator->FindClosestPoint(p4);
      this->Target->GetPoints()->GetPoint(id,p5);
      
      float w=1.0;
      if (dooutliers==1)
	w=M_Outliers_Column->GetDirectElement(i,0);
      float d=sqrt(vtkMath::Distance2BetweenPoints(p4,p5));

      sum+=d*w;
      totaln+=w;
      if (dolabels)
	{
	  int lv=(int)this->LX->GetElement(i,0);
	  if (lv<50 && lv>=0)
	    {		      
	      sumlab[lv]+=d*w;
	      numlab[lv]+=w;
	    }
	  
	}
    }

  {  for (int cnt=0;cnt<50;cnt++)
    {
      if (numlab[cnt]>0.01)
	{
	  fprintf(stdout,"EX \t %s \t Label %d  \t %8.5f \t %8.2f \n",line,cnt,
		  sumlab[cnt]/(numlab[cnt]),numlab[cnt]);
	}
  }}
  fprintf(stdout,"EX \t %s \t Overall \t %8.5f \t %8.2f \n",line,sum/(totaln),totaln);
  fprintf(stdout,"\n");

  this->AverageDistance = sum/(totaln);
  fprintf(stdout,"Transformation = %s ",line);
  fprintf(stdout,"Average distance between corresponding  points = %8.5f ",this->AverageDistance);
  fprintf(stdout,"Number of points used to compute distance  = %8.2f \n", totaln);

  locator->Delete();
  return sum/totaln;

      
}

