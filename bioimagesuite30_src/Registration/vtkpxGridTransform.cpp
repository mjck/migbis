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





#include "pxutil.h"
#include "vtkDoubleArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkDataArray.h"
#include "vtkStructuredPoints.h"
#include "vtkpxGridTransform.h"
#include "vtkLinearTransform.h"
#include "vtkpxUtil.h"
#include "vtkpxMatrix.h"
#include "vtkpxWeightedLandmarkTransform.h"
#include "vtkCollection.h"
#include "vtkpxOptimizer.h"
#include "vtkbisImageReslice.h"
#include "pxisinf.h"
#include "bioimagesuite_settings.h"
#include "vtkTimerLog.h"

//-----------------------------------------------------------------------------------
vtkpxGridTransform* vtkpxGridTransform::New()
{
  // First try to create the object from the vtkObjectFactory

  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGridTransform");
  if(ret)
    {
    return (vtkpxGridTransform*)ret;
    }

  // If the factory was unable to create the object, then create it here.

  return new vtkpxGridTransform;
}

//----------------------------------------------------------------------------

vtkpxGridTransform::vtkpxGridTransform()
{
  for (int ia=0;ia<=2;ia++)
    {
      this->GridDimensions[ia]=0;
      this->GridOrigin[ia]=0.0;
      this->GridSpacing[ia]=1.0;
    }
  this->GridSliceDimensions=1;
  this->NumberOfControlPoints=1;
  this->Displacements=NULL;

  this->InverseTolerance = 0.1;
  this->InverseIterations = 10;
  this->ApproximationMode=0;
}

//----------------------------------------------------------------------------

vtkpxGridTransform::~vtkpxGridTransform()
{
}

//----------------------------------------------------------------------------
void vtkpxGridTransform::InitializeDisplacementGrid(double bounds[6],double dx, double dy, double dz)
{
  this->InitializeDisplacementGridBounds(bounds,dx,dy,dz);
}
void vtkpxGridTransform::InitializeDisplacementGrid(vtkImageData *image, double spacing,double offset,vtkAbstractTransform* tr)
{
  double bounds[6];  image->GetBounds(bounds);
  this->InitializeDisplacementGridTransform(bounds,spacing,offset,tr);
  fprintf(stderr,"Dimensions=%d,%d,%d Spacing=%.2f,%.2f,%.2f Origin=%.2f,%.2f,%.2f\n",
	  this->GridDimensions[0],	  this->GridDimensions[1],	  this->GridDimensions[2],
	  this->GridSpacing[0],	  this->GridSpacing[1],	  this->GridSpacing[2],
	  this->GridOrigin[0],	  this->GridOrigin[1],	  this->GridOrigin[2]);
 
}
//----------------------------------------------------------------------------
void vtkpxGridTransform::SetDisplacementGrid(vtkImageData *grid)
{
  if (grid==NULL)
    return;

  vtkpxBaseGridTransform::SetDisplacementGrid(grid);

  this->DisplacementGrid->GetDimensions(this->GridDimensions);
  this->DisplacementGrid->GetOrigin(this->GridOrigin);
  this->DisplacementGrid->GetSpacing(this->GridSpacing);

  this->Displacements=(vtkDoubleArray*)this->DisplacementGrid->GetPointData()->GetScalars();
  this->NumberOfControlPoints=1;
  this->NumberOfControlPoints=this->Displacements->GetNumberOfTuples();
  this->GridSliceDimensions=this->GridDimensions[1]*this->GridDimensions[0]; 
}
//----------------------------------------------------------------------------
// Index Manipulation
//----------------------------------------------------------------------------
inline int  vtkpxGridTransform::GetControlPointIndex(int i,int j,int k)
{
  i=Irange(i,0,this->GridDimensions[0]-1);
  j=Irange(j,0,this->GridDimensions[1]-1);
  k=Irange(k,0,this->GridDimensions[2]-1);

  return i+j*this->GridDimensions[0]+k*this->GridSliceDimensions;
}

inline void vtkpxGridTransform::GetControlPointIndices(int index,int ind[3])
{
  index=Irange(index,0,this->NumberOfControlPoints-1);
  ind[2]=index/this->GridSliceDimensions;
  int t=index%this->GridSliceDimensions;
  ind[1]=t/this->GridDimensions[0];
  ind[0]=t%this->GridDimensions[0];
}
//----------------------------------------------------------------------------
//  Get double* pointer to the displacement of control point index (0..numcontrolpoints-1)
//----------------------------------------------------------------------------
inline double* vtkpxGridTransform::GetControlPointPointer(int index)
{
  index=Irange(index,0,this->NumberOfControlPoints-1);
  return this->Displacements->GetPointer(index*3);
}
//-------------------------------------------------------------------------
// Get/Set Displacement of control point index 
//----------------------------------------------------------------------------
inline double vtkpxGridTransform::GetControlPointDisplacement(int index,int comp)
{
  comp=Irange(comp,0,2);
  double* ptr=this->GetControlPointPointer(index);
  return ptr[comp];
}
//-------------------------------------------------------------------------
inline void vtkpxGridTransform::GetControlPointDisplacement(int index,double val[3])
{
  double* ptr=this->GetControlPointPointer(index);
  for (int ia=0;ia<=2;ia++)
    val[ia]=ptr[ia];
}
//-------------------------------------------------------------------------
inline void vtkpxGridTransform::SetControlPointDisplacement(int index,int comp,double val)
{
  comp=Irange(comp,0,2);
  double* ptr=this->GetControlPointPointer(index);
  ptr[comp]=val;
}

inline void vtkpxGridTransform::SetControlPointDisplacement(int index,double val[3])
{
  double* ptr=this->GetControlPointPointer(index);
  for (int i=0;i<=2;i++)
    ptr[i]=val[i];
}
//----------------------------------------------------------------------------
// Move Control Point index
//----------------------------------------------------------------------------
inline void vtkpxGridTransform::ShiftControlPoint(int index,int comp,double val)
{
  comp=Irange(comp,0,2);
  double* ptr=this->GetControlPointPointer(index);
  if (ptr!=NULL)
    ptr[comp]+=val;
  else
    vtkErrorMacro(<<"NULL Pointer\n");
}
//-------------------------------------------------------------------------
inline void vtkpxGridTransform::ShiftControlPoint(int index,double val[3])
{
  double* ptr=this->GetControlPointPointer(index);
  for (int i=0;i<=2;i++)
    ptr[i]+=val[i];
}
//-------------------------------------------------------------------------
// Get the Location of control point index
//-------------------------------------------------------------------------
inline void vtkpxGridTransform::GetControlPointLocation(int index,double p[3])
{
  int ind[3];  this->GetControlPointIndices(index,ind);
  for (int ia=0;ia<=2;ia++)
    {
      p[ia]=this->GridOrigin[ia]+this->GridSpacing[ia]*double(ind[ia]);
    }
}
//----------------------------------------------------------------------------
inline double vtkpxGridTransform::GetControlPointLocation(int index,int comp)
{
  double p[3];
  this->GetControlPointLocation(index,p);
  comp=Irange(comp,0,2);
  return p[comp];
}
//----------------------------------------------------------------------------
// Get Internal Coordinates both lattice and arclengths
//----------------------------------------------------------------------------
void vtkpxGridTransform::GetCoordinatesInGrid(double p[3],int lat[3],double s[3])
{
  for (int ia=0;ia<=2;ia++)
    {
      double x=(p[ia]-this->GridOrigin[ia])/this->GridSpacing[ia]; 
      lat[ia]=int(floor(x+0.0001));
      s[ia]=x-lat[ia];
    }
}
//----------------------------------------------------------------------------
// Compute Derivative Matrix for Point x1,x2,x3 and store in vtkpxMatrix
//----------------------------------------------------------------------------
double vtkpxGridTransform::ComputeDerivativeMatrix(double x1,double x2,double x3,vtkpxMatrix* matr)
{
  double der[3][3];
  double x[3]; x[0]=x1; x[1]=x2; x[2]=x3;
  double det=this->ComputeDerivativeMatrix(x,der);
  matr->Zeros(3,3);
  for (int ia=0;ia<=2;ia++)
    for (int ib=0;ib<=2;ib++)
      matr->SetElement(ia,ib,der[ia][ib]);
  return det;
}

double vtkpxGridTransform::ComputeDerivativeMatrix(double x[3],double der[3][3])
{
  double x2[3];
  this->InternalTransformDerivative(x,x2,der);
  return vtkMath::Determinant3x3(der);
}

//----------------------------------------------------------------------------
// Compute Derivative at location of control point index
//----------------------------------------------------------------------------
double vtkpxGridTransform::ComputeDerivativeMatrix(int index,vtkpxMatrix* matr)
{
  double der[3][3];
  double x[3]; this->GetControlPointLocation(index,x);
  double det=this->ComputeDerivativeMatrix(x,der);
  matr->Zeros(3,3);
  for (int ia=0;ia<=2;ia++)
    for (int ib=0;ib<=2;ib++)
      matr->SetElement(ia,ib,der[ia][ib]);
  return det;
}

double vtkpxGridTransform::ComputeDerivativeMatrix(int index,double der[3][3])
{
  double x[3]; this->GetControlPointLocation(index,x);
  return this->ComputeDerivativeMatrix(x,der);
}

//----------------------------------------------------------------------------
double vtkpxGridTransform::ComputeDerivative(int index,int comp1,int comp2)
{
  double x[3];
  this->GetControlPointLocation(index,x);
  return this->ComputeDerivative(x,comp1,comp2);
}

double vtkpxGridTransform::ComputeDerivative(double x1,double x2,double x3,int comp1,int comp2)
{
  double x[3];
  x[0]=x1; x[1]=x2; x[2]=x3;
  return this->ComputeDerivative(x,comp1,comp2);

}
//----------------------------------------------------------------------------
// Single Component of Derivative
//----------------------------------------------------------------------------
double vtkpxGridTransform::ComputeDerivative(double x[3],int comp1,int comp2)
{
  comp1=Irange(comp1,0,2);
  comp2=Irange(comp1,0,2);
  double x2[3],der[3][3];
  this->ForwardTransformDerivative(x,x2,der);
  return der[comp1][comp2];
}
//----------------------------------------------------------------------------
// Stuff Needed to fit transformation to set of points 
//----------------------------------------------------------------------------
void vtkpxGridTransform::FormWeightMatrix(vtkPoints* points,vtkpxMatrix* W)
{
  int nump=points->GetNumberOfPoints();

  if (nump<1)
    return;

  int numc=this->NumberOfControlPoints;
  if (numc<1)
    return;
  
  W->Zeros(nump,numc);

  for (int node = 0; node < nump; node++)
    {
      double p1[3],s[3];
      int  lat[3];
      points->GetPoint(node,p1);
      this->GetCoordinatesInGrid(p1,lat,s);
      
      for (int k = 0; k < 4; k++)
	{
	  int K = Irange(k + lat[2] - 1,0,this->GridDimensions[2]-1);
	  for (int j = 0; j < 4; j++)
	    {
	      int J = Irange(j + lat[1] - 1,0,this->GridDimensions[1]-1);
	      for (int i = 0; i < 4; i++)
		{
		  int I = Irange(i + lat[0] - 1,0,this->GridDimensions[0]-1);
		  int   cpoint=I+J*this->GridDimensions[0]+K*this->GridSliceDimensions;
		  double wgt= B(i, s[0]) * B(j, s[1]) * B(k, s[2]);
		  W->AddToElement(node,cpoint,wgt);
		}
	    }
	}
    }
}


double vtkpxGridTransform::TestTransformPoint(double p1[3],double p2[3])
{
  double p2a[3];
  fprintf(stderr,"****\n");
  this->TransformPoint(p1,p2a);
  fprintf(stderr,"****\n");

  double s[3];
  int  lat[3];

  double pp[3]; 
  for (int ia=0;ia<=2;ia++)
    pp[ia]=p1[ia];

  this->GetCoordinatesInGrid(pp,lat,s);
 
  fprintf(stderr,"p1 = (%.2f,%.2f,%.2f) lat=(%d,%d,%d), s=(%.2f,%.2f,%.2f)\n",
	  p1[0],p1[1],p1[2],lat[0],lat[1],lat[2],s[0],s[1],s[2]);

  /*  if (p1[2]>94.79 && p1[2]<94.81)
    {
      lat[2]-=1;
      s[2]+=1.0;
      fprintf(stderr,"\t\t\t p1 = (%.2f,%.2f,%.2f) lat=(%d,%d,%d), s=(%.2f,%.2f,%.2f)\n",
	      p1[0],p1[1],p1[2],lat[0],lat[1],lat[2],s[0],s[1],s[2]);
	      }*/

  {
    for (int ia=0;ia<=2;ia++)
      p2[ia]=p1[ia];
  }
     
  for (int k = 0; k < 4; k++)
    {
      int K = Irange(k + lat[2] - 1,0,this->GridDimensions[2]-1);
      for (int j = 0; j < 4; j++)
	{
	  int J = Irange(j + lat[1] - 1,0,this->GridDimensions[1]-1);
	  for (int i = 0; i < 4; i++)
	    {
	      int I = Irange(i + lat[0] - 1,0,this->GridDimensions[0]-1);
	      int   cpoint=I+J*this->GridDimensions[0]+K*this->GridSliceDimensions;
	      double wgt= B(i, s[0]) * B(j, s[1]) * B(k, s[2]);

	      double c[3],d[3];
	      this->GetControlPointLocation(cpoint,c);
	      this->GetControlPointDisplacement(cpoint,d);
	      for (int ia=0;ia<=2;ia++)
		p2[ia]+=wgt*d[ia];
	      if (wgt>0.005)
		fprintf(stderr,"\t\t (%d %d %d) wgt=%.2f c=(%.1f,%.1f,%.1f) \t p2=(%.1f,%.1f,%.1f) \n",I,J,K,wgt,c[0],c[1],c[2],p2[0],p2[1],p2[2]);

	    }
	}
    }

  double d=sqrt(vtkMath::Distance2BetweenPoints(p2a,p2));
  
  fprintf(stderr,"p1 (%.1f,%.1f,%.1f) ---> \t (%.1f,%.1f,%.1f)\n\t\t\t (%.1f,%.1f,%.1f)  d=%.2f\n",
	  p1[0],p1[1],p1[2],
	  p2a[0],p2a[1],p2a[2],
	  p2[0],p2[1],p2[2],d);
  return d;
}

double vtkpxGridTransform::TestTransformPoint(double x,double y,double z)
{
  double p1[3],p2[3];
  p1[0]=x;p1[1]=y;p1[2]=z;
  p2[0]=0;p2[1]=0;p2[2]=0;
  
  return this->TestTransformPoint(p1,p2);
}

//----------------------------------------------------------------------------
// Form Cached Collections between points and control points 
//----------------------------------------------------------------------------
int vtkpxGridTransform::FormCollectionStructure(vtkPoints* points,vtkCollection* ControlsCollection)
{
  return this->FormCollectionStructure(points,ControlsCollection,0.0);
}

int vtkpxGridTransform::FormCollectionStructure(vtkPoints* points,vtkCollection* ControlsCollection,double thr)
{
  if (points==NULL || ControlsCollection==NULL)
    return 0;
  
  thr=Frange(thr,0.0,0.2);

  ControlsCollection->RemoveAllItems();
  int np=points->GetNumberOfPoints();
  int nc=this->NumberOfControlPoints;

  vtkDoubleArray** arrlist=new vtkDoubleArray*[nc];

  for (int cp=0;cp<nc;cp++)
    {
      vtkDoubleArray* fl=vtkDoubleArray::New();
      fl->SetNumberOfComponents(2);
      fl->Allocate(10,10);
      ControlsCollection->AddItem(fl);
      arrlist[cp]=fl;
    }

  for (int node=0;node<np;node++)
    {
      double p1[3],s[3];
      int  lat[3];
      points->GetPoint(node,p1);
      this->GetCoordinatesInGrid(p1,lat,s);
      //      double sum=0.0;

      for (int k = 0; k < 4; k++)
	{
	  int K = Irange(k + lat[2] - 1,0,this->GridDimensions[2]-1);
	  for (int j = 0; j < 4; j++)
	    {
	      int J = Irange(j + lat[1] - 1,0,this->GridDimensions[1]-1);
	      for (int i = 0; i < 4; i++)
		{
		  int I = Irange(i + lat[0] - 1,0,this->GridDimensions[0]-1);
		  int   cpoint=I+J*this->GridDimensions[0]+K*this->GridSliceDimensions;
		  double wgt= B(i, s[0]) * B(j, s[1]) * B(k, s[2]);
		  //sum+=wgt;
		  if (wgt>=thr)
		    {
		      //vtkDoubleArray* fl=(vtkDoubleArray*)ControlsCollection->GetItemAsObject(cpoint);
		      vtkDoubleArray* fl=arrlist[cpoint];
		      int numt=fl->GetNumberOfTuples();
		      int found=0,l=0;
		      
		      while (l<numt && found==0)
			{
			  int cand=(int)fl->GetComponent(l,0);
			  if (cand==node)
			    {
			      fl->SetComponent(l,1,fl->GetComponent(l,1)+wgt);
			      found=1;
			    }
			  l++;
			}
		      
		      if (found==0)
			fl->InsertNextTuple2(double(node),wgt);
		    }
		}
	    }
	}
      //      fprintf(stderr,"node=%d sum=%f\n",node,sum);
    }

  delete [] arrlist;


  int sum=0;
  ControlsCollection->InitTraversal();
  for (int p=0;p<nc;p++)
    {
      vtkDoubleArray* fl=(vtkDoubleArray*)ControlsCollection->GetNextItemAsObject();
      sum+=fl->GetNumberOfTuples();
    }
  int m=int(sum/nc);
  //fprintf(stderr,"Mean Number of Points=%d\n",m);
  return m;

}
//----------------------------------------------------------------------------
int vtkpxGridTransform::InitialApproximateTransformation(vtkAbstractTransform* tr)
{
  if (tr==NULL)
    return 0;

  double tp[3],tq[3];
  tr->Update();
  vtkImageData *grid=this->DisplacementGrid;
  for (int k=0;k<this->GridDimensions[2];k++)
    {
      tp[2]=this->GridOrigin[2]+this->GridSpacing[2]*double(k);
      for (int j=0;j<this->GridDimensions[1];j++)
	{
	  tp[1]=this->GridOrigin[1]+this->GridSpacing[1]*double(j);
	  for (int i=0;i<this->GridDimensions[0];i++)
	    {
	      tp[0]=this->GridOrigin[0]+this->GridSpacing[0]*double(i);
	      tr->InternalTransformPoint(tp,tq);
	      double *v=(double*)grid->GetScalarPointer(i,j,k);
	      for (int ia=0;ia<=2;ia++)
		v[ia]=tq[ia]-tp[ia];
	    }
	}
    }

  this->Update();
  return 1;
}
//-------------------------------------------------------------------------
void vtkpxGridTransform::GenerateTestPoints(vtkPoints* points,double incr)
{
  int nc=this->NumberOfControlPoints;

  incr=Frange(incr,0.2,3.0);

  //  fprintf(stderr,"Incr=%f\n",incr);
  double max=nc/(incr*incr*incr)+10;
  //  fprintf(stderr,"Max=%f\n",max);
  points->Allocate((int)max,20);
  double p[3];

  //  int index=0;
  for (double k=0;k<this->GridDimensions[2];k+=incr)
    {
      p[2]=this->GridOrigin[2]+this->GridSpacing[2]*(k);
      for (double j=0;j<this->GridDimensions[1];j+=incr)
	{
	  p[1]=this->GridOrigin[1]+this->GridSpacing[1]*(j);
	  for (double i=0;i<this->GridDimensions[0];i+=incr)
	    {
	      p[0]=this->GridOrigin[0]+this->GridSpacing[0]*(i);
	      points->InsertNextPoint(p);
	      //	      if (index==50)
		//		fprintf(stderr,"Point index=%d x= %f,%f,%f \n",index,p[0],p[1],p[2]);
	      //	      ++index;
	    }
	}
    }

}
//-------------------------------------------------------------------------
void vtkpxGridTransform::FillMatrices(vtkAbstractTransform* tr,vtkpxMatrix* A,vtkpxMatrix* BB)
{
  int nc=this->NumberOfControlPoints;
  vtkPoints* points=vtkPoints::New();
  points->SetNumberOfPoints(nc);
  A->Zeros(nc,nc);
  BB->Zeros(nc,3);
  double x[3],tx[3];
  tr->Update();
  for (int index=0;index<nc;index++)
    {
      this->GetControlPointLocation(index,x);
      points->SetPoint(index,(double*)x);
      tr->InternalTransformPoint((double*)x,tx);
      for (int ib=0;ib<=2;ib++)
	tx[ib]-=x[ib];
      BB->SetRow(index,3,(double*)tx);
    }

  this->FormWeightMatrix(points,A);
  points->Delete();
}
//-------------------------------------------------------------------------
double vtkpxGridTransform::TestApproximation(vtkAbstractTransform* tr,vtkPoints* points)
{
  double error=0.0;
  int nc=points->GetNumberOfPoints();
  if (nc<1)
    return -1.0;

  tr->Update();
  this->Update();

  double x[3],t1[3],t2[3];
  for (int i=0;i<nc;i++)
    {
      points->GetPoint(i,x);
      tr->InternalTransformPoint(x,t1);
      this->InternalTransformPoint(x,t2);
      error+=sqrt(vtkMath::Distance2BetweenPoints(t1,t2));
    }

  return error/double(nc);
}
//-------------------------------------------------------------------------
double vtkpxGridTransform::TestInverseApproximation(vtkAbstractTransform* tr,vtkPoints* points)
{
  double error=0.0;
  int nc=points->GetNumberOfPoints();
  if (nc<1)
    return -1.0;

  tr->Update();
  this->Update();

  double x[3],t1[3],t2[3];
  for (int i=0;i<nc;i++)
    {
      points->GetPoint(i,x);
      tr->InternalTransformPoint(x,t1);
      this->InternalTransformPoint(t1,t2);
      double d=sqrt(vtkMath::Distance2BetweenPoints(x,t2));
      /*fprintf(stderr,
	      "i=%d\t x=(%5.2f %5.2f %5.2f) -->\t t1=(%5.2f %5.2f %5.2f) -->\t (%5.2f %5.2f %5.2f) \t dist=%5.2f\n",
	      i,x[0],x[1],x[2],t1[0],t1[1],t1[2],t2[0],t2[1],t2[2],d);*/
      error+=d;
    }
  return error/double(nc);
}

//-------------------------------------------------------------------------
double vtkpxGridTransform::TestApproximation(vtkPoints* points,vtkPoints* points2)
{
  double error=0.0;
  int nc=points->GetNumberOfPoints();
  
  if (nc<1)
    return -1.0;

  this->Update();

  double x[3],tx[3],x2[3];
  for (int i=0;i<nc;i++)
    {
      points->GetPoint(i,x);
      points2->GetPoint(i,tx);
      this->InternalTransformPoint(x,x2);
      error+=sqrt(vtkMath::Distance2BetweenPoints(x2,tx));
    }
  return error/double(nc);
}
//-------------------------------------------------------------------------
double vtkpxGridTransform::TestWeightedApproximation(vtkPoints* points,vtkPoints* points2,
						     vtkpxMatrix* lsqweights,
						     double& sum2error)
{
  double error=0.0;
  sum2error=0.0;
  int nc=points->GetNumberOfPoints();
  
  int useweights=0;
  if (lsqweights!=NULL)
    {
      if (lsqweights->GetSize()[0]>=points->GetNumberOfPoints())
	useweights=1;
    }


  if (nc<1)
    {
      fprintf(stderr,"*** nc<1");
      return 0.0;
    }

  this->Update();
  double x[3],x2[3],tx[3];
  for (int i=0;i<nc;i++)
    {
      points->GetPoint(i,x);
      points2->GetPoint(i,tx);
      this->InternalTransformPoint(x,x2);
      //this->TransformPoint(x,x2);
      // This is the key **** point **********
      double d=vtkMath::Distance2BetweenPoints(x2,tx);
      if (isinf(d))
	{
	  fprintf(stderr,"-- i=%d %.2f %.2f %.2f,  %.2f %.2f %.2f\n",i,
		  x[0],x[1],x[2],
		  x2[0],x2[1],x2[2]);
	  d=0.0;
	}
      double w=1.0;
      if (useweights)
	{
	  w=lsqweights->GetDirectElement(i,0);
	  if (isinf(w))
	    {
	      w=1.0;
	      fprintf(stderr,"Weight=inf i=%d\n",i);
	    }
	}
      error+=w*sqrt(d);
      sum2error+=w*d;
    }
  sum2error/=double(nc);
  return error/double(nc);
}
//-------------------------------------------------------------------------
int vtkpxGridTransform::ApproximateTransformation(vtkAbstractTransform* tr)
{
  return this->ApproximateTransformation(tr,0.05,20,1,0.0005,0.0);
}

int vtkpxGridTransform::ApproximateTransformation(vtkAbstractTransform* tr,
						  double step,
						  int numiter,
						  int numlevels,double threshold,double lambda)
{
  this->InitialApproximateTransformation(tr);

  if (tr->IsA("vtkLinearTransform")==1)
    return 1;
    
  //  timer->StartTimer();
  vtkCollection* collection=vtkCollection::New();
  vtkPoints* points=vtkPoints::New();
  this->GenerateTestPoints(points,0.5);
  
  int np=points->GetNumberOfPoints();



  vtkPoints* points2=vtkPoints::New();
  points2->SetNumberOfPoints(np);
  tr->Update();
  for (int ia=0;ia<np;ia++)
    {
      double x[3],tx[3];
      points->GetPoint(ia,x);
      tr->InternalTransformPoint(x,tx);
      points2->SetPoint(ia,tx);
    }
  fprintf(stderr,"\t GridTransform::Approximate  Transform %d points (error=%.4f)\n",np  , this->TestApproximation(points,points2));
  this->FormCollectionStructure(points,collection,0.01);

  vtkTimerLog* timer=vtkTimerLog::New();
  timer->StartTimer();
  this->ApproximatePoints(points,points2,collection,step,numiter,numlevels,threshold,lambda,0);
  timer->StopTimer();  fprintf(stderr,"\t GridTransform::Approximate  Finished Approximation %d TestPoints = %.5fs\n",np,timer->GetElapsedTime());
  points->Delete();
  points2->Delete();
  collection->Delete();
  timer->Delete();
  return 1;
}
//------------------------------------------------------------------------- 
// Compute Approximation to Set of Displacements
//-------------------------------------------------------------------------
double vtkpxGridTransform::ErrorAtControlPoint(int cp,vtkPoints* points,vtkPoints* points2,
					      vtkDoubleArray* indices,double lambda,vtkpxMatrix* lsqweight,
					      vtkpxGridTransform* priorTransform,vtkDoubleArray* priorWeights)
{
  int np=indices->GetNumberOfTuples();
  double nump=double(points->GetNumberOfPoints());
  double x[3],tx[3],sum=0.0;
  this->Update();
  if (np>0)
    {
      for (int ki=0;ki<np;ki++)
	{
	  int i=(int)indices->GetComponent(ki,0);
	  double w=indices->GetComponent(ki,1);
	  points->GetPoint(i,x);
	  points2->GetPoint(i,tx);
	  this->InternalTransformPoint(x,x);
	  double dist= (vtkMath::Distance2BetweenPoints(x, tx));

	  double w2=1.0;
	  if (lsqweight!=NULL)
	    w2=lsqweight->GetDirectElement(i,0);
	  sum+=w*w2*dist;
	}
    }

  if (priorTransform==NULL)
    return (sum/nump)+lambda*this->ComputeBendingEnergyAroundControlPoint(cp);

  return (sum/nump)+lambda*this->ComputeWeightedDistance(OptimizationStructure.priorTransform,cp,OptimizationStructure.priorWeights);
}
//------------------------------------------------------------------------- 
double vtkpxGridTransform::GradientAtControlPoint(int cp,double grad[3],
						 vtkPoints* points,vtkPoints* points2,
						 vtkDoubleArray* indices,double step,
						 double lambda,vtkpxMatrix* lsqweight,
						 vtkpxGridTransform* priorTransform,
						 vtkDoubleArray* priorWeights)
{
  for (int dim=0;dim<=2;dim++)
    {
      grad[dim]=0.0;
      this->ShiftControlPoint(cp,dim,-step);
      double s1 = this->ErrorAtControlPoint(cp,points,points2,indices,lambda,lsqweight,priorTransform,priorWeights);
      this->ShiftControlPoint(cp,dim,+2.0*step);
      double s2 = this->ErrorAtControlPoint(cp,points,points2,indices,lambda,lsqweight,priorTransform,priorWeights);
      this->ShiftControlPoint(cp,dim,-step);
      double s0=this->ErrorAtControlPoint(cp,points,points2,indices,lambda,lsqweight,priorTransform,priorWeights);

      /*if (cp==99)
	fprintf(stderr,"s0=%.5f s1=%.5f s2=%.5f dim=%d\n",s0,s1,s2,dim);*/

      if (s0 < s1 && s0 < s2)
	{
	  grad[dim]=0.0;
	}
      else 
	{
	  grad[dim]=s2-s1;
	}
    }

  return ( pow(grad[0],2.0)+pow(grad[1],2.0)+pow(grad[2],2.0));
}


// ---------------------------------------------------------------------------------------
// Key Procedures for approximation of points via optimization
// ---------------------------------------------------------------------------------------
void vtkpxGridTransform::ShiftControlPoints(vtkDoubleArray* grad,double step)
{
  int nc=this->NumberOfControlPoints;
  for (int j=0;j<=2;j++)
    for (int i = 0; i < nc; i++)
      this->ShiftControlPoint(i,j,step*grad->GetComponent(i,j));
}
// ---------------------------------------------------------------------------------------
void vtkpxGridTransform::ScaleGradients(vtkDoubleArray* grad,double scale)
{
  int nt=grad->GetNumberOfTuples();
  int nc=grad->GetNumberOfComponents();
  for (int t=0;t<nt;t++)
    for (int c=0;c<nc;c++)
      grad->SetComponent(t,c,grad->GetComponent(t,c) * scale);

}
// ---------------------------------------------------------------------------------------
void vtkpxGridTransform::InitializeApproximatePoints(vtkPoints* points,
						     vtkPoints* points2,
						     vtkpxMatrix* weights)

{
  vtkpxWeightedLandmarkTransform* tr=vtkpxWeightedLandmarkTransform::New();
  tr->SetSourceLandmarks(points);
  tr->SetTargetLandmarks(points2);
  tr->SetWeights(weights);
  tr->SetModeToAffine();
  tr->Modified();
  tr->Update();

  this->InitialApproximateTransformation(tr);
  double e=this->TestApproximation(points,points2);
  OptimizationStructure.bend=0.0;
  OptimizationStructure.error=0.0;
  vtkpxGridTransform::OptimizerFeedback(this,"Initial Approximation Error ",e);
  tr->Delete();
}
// ---------------------------------------------------------------------------------------
void vtkpxGridTransform::ApproximatePoints(vtkPoints* points,
					   vtkPoints* points2,
					   vtkCollection* ControlsCollection,
					   double step,
					   int numiter,int numlevels,
					   double in_threshold,
					   double lambda,int initialize,
					   vtkpxMatrix* controlweights,
					   vtkpxGridTransform* priorTransform0,
					   vtkDoubleArray* priorWeights0)
{
  // Test for Parameters
  // --------------------

  if (ControlsCollection==NULL || points2==NULL)
    {
      vtkErrorMacro(<<"Collection or Points = NULL");
      return;
    }

  int nc=this->NumberOfControlPoints;
  if (ControlsCollection->GetNumberOfItems()!=nc)
    {
      vtkErrorMacro(<<"Bad Controls Collection\n");
      return ;
    }


  // Initial Step
  // -------------

  if (initialize)
    {
      vtkDebugMacro(<<"Step 1  Optimization\n");
      this->InitializeApproximatePoints(points,points2,controlweights);
      if (numlevels==0)
	{
	  vtkDebugMacro(<<"No Further Optimization\n");
	  return;
	}
    }

  vtkpxOptimizer* optimizer=vtkpxOptimizer::New();
  vtkDoubleArray* position=vtkDoubleArray::New();
  vtkDoubleArray* temp=vtkDoubleArray::New();
  
  this->GetCurrentParameters(position);
  optimizer->SetMasterObject(this);
  optimizer->SetEvaluateFunctionMethod(&vtkpxGridTransform::CalculateFunction);
  optimizer->SetEvaluateGradientMethod(&vtkpxGridTransform::CalculateGradient);
  optimizer->SetFeedbackMethod(&vtkpxGridTransform::OptimizerFeedback);
  
  double v=0.0;
  numlevels=Irange(numlevels,1,3);

  vtkpxGridTransform* priorTransform=priorTransform0;
  vtkDoubleArray* priorWeights=priorWeights0;

  // Make Sure that all is fine before using prior Stuff 
  if (priorTransform!=NULL)
    {
      int np=this->GetNumberOfControlPoints();
      int np1=priorTransform->GetNumberOfControlPoints();
      if (np!=np1)
	{
	  priorTransform=NULL;
	}
      else
	{
	  if (priorWeights!=NULL)
	    {
	      int np2=priorWeights->GetNumberOfTuples();
	      if (np!=np2)
		priorWeights=NULL;
	    }
	}
    }
  else
    {
      priorWeights=NULL;
      priorTransform=NULL;
    }

  fprintf(stderr,"\n Approximating  %d points lambda=%f step=%.2f\n",points->GetNumberOfPoints(),lambda,step);
  for (int level=1;level<=numlevels;level++)
    {
      this->OptimizationStructure.SetValues(ControlsCollection,points,points2,step,lambda,controlweights,
					    priorTransform,priorWeights);
      
      switch (this->ApproximationMode)
	{
	case 1:
	  v=optimizer->ComputeGradientDescent(position,numiter,0.01*in_threshold);
	  break;
	  
	default:
	  v=optimizer->ComputeConjugateGradient(position,numiter,0.01*in_threshold);
	  break;
	}
      step*=0.5;
      in_threshold*=0.5;
    }
  optimizer->Delete();
  position->Delete();
  temp->Delete();
  return;
}
// ---------------------------------------------------------------------------------------
void vtkpxGridTransform::CombineWithLinearTransform(vtkLinearTransform* tr,int premultiply)
{
  int nc=this->NumberOfControlPoints;

  vtkLinearTransform* inverse=tr->GetLinearInverse();
  

  double x[3],tx[3],dx[3];
  for (int cpoint=0;cpoint<nc;cpoint++)
    {
      this->GetControlPointLocation(cpoint,x);
      this->GetControlPointDisplacement(cpoint,dx);
      
      if (premultiply)
	{
	  inverse->InternalTransformPoint(x,tx);
	  for (int ia=0;ia<=2;ia++)
	    dx[ia]+=x[ia]-tx[ia];
	}
      else
	{
	  for (int ia=0;ia<=2;ia++)
	    dx[ia]+=x[ia];
	  tr->InternalTransformPoint(dx,tx);
	  for (int ib=0;ib<=2;ib++)
	    dx[ib]=tx[ib]-x[ib];
	}
      this->SetControlPointDisplacement(cpoint,dx);
    }
}

// -----------------------------------------------------------------------------------------
//                                     Perform Inversion
// -----------------------------------------------------------------------------------------

void vtkpxGridTransform::Inverse()
{
  if (this->InterpolationMode!=VTK_GRID_BSPLINE)
    {
      vtkGridTransform::Inverse();
      return;
    }

  this->InverseFlag=0;

  int nc=this->NumberOfControlPoints;
  if (nc<1)
    return;
  
  vtkPoints* points=vtkPoints::New();
  vtkPoints* points2=vtkPoints::New();


  double factor=2.0;
  double ffact=1.0/factor;

  double p[3],tp[3];
  this->Update();
  for (int k=0;k<this->GridDimensions[2]*factor;k++)
    {
      p[2]=this->GridOrigin[2]+this->GridSpacing[2]*double(k)*ffact;
      for (int j=0;j<this->GridDimensions[1]*factor;j++)
	{
	  p[1]=this->GridOrigin[1]+this->GridSpacing[1]*double(j*ffact);
	  for (int i=0;i<this->GridDimensions[0]*factor;i++)
	    {
	      p[0]=this->GridOrigin[0]+this->GridSpacing[0]*double(i*ffact);
	      this->InternalTransformPoint(p,tp);
	      points->InsertNextPoint(tp);
	      points2->InsertNextPoint(p);
	    }
	}
    }

  double bounds[6];
  points->GetBounds(bounds);

  double newsp[3];
  double offset=0.05;

  for (int ia=0;ia<=2;ia++)
    {
      double d=bounds[2*ia+1]-bounds[2*ia];
      double w=(1.0+2.0*offset)*d;
      bounds[2*ia]=bounds[2*ia]-offset*d;
      bounds[2*ia+1]=bounds[2*ia]+w;
      newsp[ia]=0.667*this->GridSpacing[ia];
    }


  fprintf(stderr,"#Original\n");
  fprintf(stderr,"#Origin\n %.4f %.4f %.4f\n",this->GridOrigin[0],this->GridOrigin[1],this->GridOrigin[2]);
  fprintf(stderr,"#Spacing\n %.4f %.4f %.4f\n",this->GridSpacing[0],this->GridSpacing[1],this->GridSpacing[2]);
  fprintf(stderr,"#Dimensions\n %d %d %d\n",this->GridDimensions[0],this->GridDimensions[1],this->GridDimensions[2]);


  this->InitializeDisplacementGrid(bounds,newsp[0],newsp[1],newsp[2]);

  
  fprintf(stderr,"#Inverse\n");
  fprintf(stderr,"#Origin\n %.4f %.4f %.4f\n",this->GridOrigin[0],this->GridOrigin[1],this->GridOrigin[2]);
  fprintf(stderr,"#Spacing\n %.4f %.4f %.4f\n",this->GridSpacing[0],this->GridSpacing[1],this->GridSpacing[2]);
  fprintf(stderr,"#Dimensions\n %d %d %d\n",this->GridDimensions[0],this->GridDimensions[1],this->GridDimensions[2]);


  vtkCollection* collection=vtkCollection::New();
  this->FormCollectionStructure(points,collection,0.01);
  this->ApproximatePoints(points,points2,
			  collection,
			  this->InverseTolerance*2,
			  this->InverseIterations,
			  3,
			  this->InverseTolerance,
			  0.0,1);
  

  points->Delete();
  points2->Delete();
}

// -----------------------------------------------------------------------------------------
// New Stuff for Optimization
// -----------------------------------------------------------------------------------------
double vtkpxGridTransform::CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output)
{
  return ((vtkpxGridTransform*)self)->OptimizerGradient(position,output);
}

double vtkpxGridTransform::CalculateFunction(vtkObject* self,vtkDoubleArray* position)
{
  return ((vtkpxGridTransform*)self)->OptimizerValue(position);
}

void vtkpxGridTransform::OptimizerFeedback(vtkObject* slf,const char* line,double val)
{
  vtkpxGridTransform* self=(vtkpxGridTransform*)slf;
  fprintf(stderr,"%s (%.5f) [ dist=%.5f tbend=%.5f]\n",line,val,
	  self->OptimizationStructure.error,
	  self->OptimizationStructure.bend);
}
// -----------------------------------------------------------------------------------------
// Get/Set Parameters, also used by NonLinearRegistration 
// -----------------------------------------------------------------------------------------
void  vtkpxGridTransform::SetCurrentParameters(vtkDoubleArray* param)
{
  int numt=this->GetNumberOfControlPoints();
  int numc=3;
  int num=param->GetNumberOfTuples();

  if (num!=numt*numc)
    {
      vtkErrorMacro(<<"Bad Data for Optimization");
      return;
    }

  int index=0;

  for (int i=0;i<numt;i++)
    {
      double p[3];
      for (int j=0;j<numc;j++)
	{
	  p[j]=param->GetComponent(index,0);
	  ++index;
	}
      
      this->SetControlPointDisplacement(i,p);
    }
}


void  vtkpxGridTransform::GetCurrentParameters(vtkDoubleArray* param)
{
  int numt=this->GetNumberOfControlPoints();
  int numc=3;
  int num=numt*numc;
  
  if (param->GetNumberOfComponents()!=1 || param->GetNumberOfTuples()!=num)
    {
      param->SetNumberOfComponents(1);
      param->SetNumberOfTuples(num);
    }


  int index=0;
  for (int i=0;i<numt;i++)
    {
      double p[3];
      this->GetControlPointDisplacement(i,p);
      for (int j=0;j<=2;j++)
	{
	  param->SetComponent(index,0,p[j]);
	  ++index;
	}
    }
}
// -----------------------------------------------------------------------------------------
// Gradient and Value Stuff 
// -----------------------------------------------------------------------------------------
double vtkpxGridTransform::OptimizerValue(vtkDoubleArray* params)
{
  this->SetCurrentParameters(params);
  double sqerr=0.0;
  this->OptimizationStructure.error=this->TestWeightedApproximation(this->OptimizationStructure.points,
								    this->OptimizationStructure.points2,
								    OptimizationStructure.weights,
								    sqerr);
  
  this->OptimizationStructure.bend =0.0;
  OptimizationStructure.ControlsCollection->InitTraversal();
  int nc=this->NumberOfControlPoints;

  
  if (OptimizationStructure.priorTransform==NULL)
    {
      double sum=0.0;
      for (int cpoint=0;cpoint<nc;cpoint++)
	sum+=this->ComputeBendingEnergyAtControlPoint(cpoint);
      OptimizationStructure.bend=sum;
    }
  else
    {
      fprintf(stderr,"+");
      OptimizationStructure.bend=this->ComputeWeightedDistance(OptimizationStructure.priorTransform,-1,OptimizationStructure.priorWeights);
    }
     
  /*  fprintf(stderr,"Optimizer Value: error=%.2f\t sqerror=%.2f\t bend=%.2f\t total=%.2f\n",
	  this->OptimizationStructure.error,
	  sqerr,
	  OptimizationStructure.bend,
	  sqerr+OptimizationStructure.bend*OptimizationStructure.lambda);*/
  
  return sqerr+OptimizationStructure.bend*OptimizationStructure.lambda;
}
// -----------------------------------------------------------------------------------------
double vtkpxGridTransform::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)
{
  if (grad->GetNumberOfComponents()!=1 || grad->GetNumberOfTuples()!=this->NumberOfControlPoints*3)
    {
      grad->SetNumberOfComponents(1);
      grad->SetNumberOfTuples(this->NumberOfControlPoints*3);
    }

  this->SetCurrentParameters(params);

  double sum=0.0;
  OptimizationStructure.ControlsCollection->InitTraversal();
  int nc=this->NumberOfControlPoints;
  double g[3];



  for (int cpoint=0;cpoint<nc;cpoint++)
    {      
      vtkDoubleArray* indices=(vtkDoubleArray*)OptimizationStructure.ControlsCollection->GetNextItemAsObject();
      
      double v=this->GradientAtControlPoint(cpoint,g,
					   OptimizationStructure.points,
					   OptimizationStructure.points2,
					   indices,
					   OptimizationStructure.step,
					   OptimizationStructure.lambda,
					   OptimizationStructure.weights,
					   OptimizationStructure.priorTransform,
					   OptimizationStructure.priorWeights);

      sum+=v;
      grad->SetComponent(cpoint*3,0,g[0]);
      grad->SetComponent(cpoint*3+1,0,g[1]);
      grad->SetComponent(cpoint*3+2,0,g[2]);
    }
	  
  sum=sqrt(sum);

  if (sum>0.0)
    {
      double scale=OptimizationStructure.step/sum;
      //      fprintf(stderr,"\tGradient Norm=%f scale=%.5f\n",sum,scale);
      this->ScaleGradients(grad,scale);
    }
  return sum;
}

// -----------------------------------------------------------------------------------------
void vtkpxGridTransform::InitializeDisplacementGrid(vtkpxGridTransform* other)
{
  vtkImageData* oldgrid=other->GetDisplacementGrid();
  vtkImageData *grid = vtkImageData::New();
  grid->SetScalarType(VTK_DOUBLE);
  grid->SetNumberOfScalarComponents(3);
  grid->SetSpacing(oldgrid->GetSpacing());
  grid->SetOrigin(oldgrid->GetOrigin());
  grid->SetExtent(oldgrid->GetExtent());

  grid->SetUpdateExtent(grid->GetWholeExtent());
  grid->AllocateScalars();
  for (int i=0;i<=2;i++)
    grid->GetPointData()->GetScalars()->FillComponent(i,0.0);
  this->SetDisplacementGrid(grid);
}
// -----------------------------------------------------------------------------------------
void vtkpxGridTransform::InverseApproximateTransformation(vtkpxGridTransform* output,int initialize)
{
  if (initialize)
    output->InitializeDisplacementGrid(this);

  vtkCollection* collection=vtkCollection::New();
  vtkPoints* points=vtkPoints::New();

  output->GenerateTestPoints(points,0.5);
  int np=points->GetNumberOfPoints();

  vtkPoints* points2=vtkPoints::New();
  points2->SetNumberOfPoints(np);

  for (int ia=0;ia<np;ia++)
    {
      double x[3],tx[3];
      points->GetPoint(ia,x);
      this->InverseTransformPoint(x,tx);
      points2->SetPoint(ia,tx);
    }


  output->FormCollectionStructure(points,collection,0.01);
  output->ApproximatePoints(points,points2,collection,
			    this->InverseTolerance*2,
			    this->InverseIterations,
			    3,
			    this->InverseTolerance,
			    0.0,1);
  points->Delete();
  points2->Delete();
  collection->Delete();
  return;
}


// -----------------------------------------------------------------------------------------
double vtkpxGridTransform::ComputeTotalBendingEnergy()
{
  double sum=0.0;
  int nc=this->NumberOfControlPoints;
  if (nc==0)
    return 0.0;

  for (int i=0;i<nc;i++)
    sum+=this->ComputeBendingEnergyAtControlPoint(i);


  return sum;
}
// ---------------------------------------------------------------------------------------
double vtkpxGridTransform::ComputeBendingEnergyAroundControlPoint(int cpoint)
{
  int ind[3];
  this->GetControlPointIndices(cpoint,ind);

  int ip=ind[0]+1; if (ip>=this->GridDimensions[0]-1) ip=this->GridDimensions[0]-1;
  int jp=ind[1]+1; if (jp>=this->GridDimensions[1]-1) jp=this->GridDimensions[1]-1;
  int kp=ind[2]+1; if (kp>=this->GridDimensions[2]-1) kp=this->GridDimensions[2]-1;

  int im=ind[0]-1; if (im<0) im=0;
  int jm=ind[1]-1; if (jm<0) jm=0;
  int km=ind[2]-1; if (km<0) km=0;

  double sum=0.0;
  for (int i=im;i<=ip;i++)
    for (int j=jm;j<=jp;j++)
      for (int k=km;k<=kp;k++)
	sum+=this->ComputeBendingEnergyAtControlPoint(i,j,k);

  return sum;
}

double vtkpxGridTransform::ComputeBendingEnergyAtControlPoint(int cpoint)
{
  int ind[3];  this->GetControlPointIndices(cpoint,ind);
  return this->ComputeBendingEnergyAtControlPoint(ind[0],ind[1],ind[2]);
}


double vtkpxGridTransform::ComputeBendingEnergyAtControlPoint(int i,int j,int k)
{
  vtkImageData *grid=this->DisplacementGrid;
  double *gridPtr1,*gridPtr2,*gridPtr4,*gridPtr3;

  //double sp=(this->GridSpacing[0]+this->GridSpacing[1]+this->GridSpacing[2])/3.0;
  double scale=0.01*(1.0/(double(this->NumberOfControlPoints)));
  
  double sum=0.0;

  int ip=i+1; if (ip>=this->GridDimensions[0]-1) ip=this->GridDimensions[0]-1;
  int jp=j+1; if (jp>=this->GridDimensions[1]-1) jp=this->GridDimensions[1]-1;
  int kp=k+1; if (kp>=this->GridDimensions[2]-1) kp=this->GridDimensions[2]-1;

  int im=i-1; if (im<0) im=0;
  int jm=j-1; if (jm<0) jm=0;
  int km=k-1; if (km<0) km=0;


  gridPtr1 = (double *)grid->GetScalarPointer(ip,j,k);
  gridPtr2 = (double *)grid->GetScalarPointer(i,j,k);
  gridPtr3 = (double *)grid->GetScalarPointer(im,j,k);
  sum += pow(*(gridPtr1)  - 2 * *(gridPtr2)   + *(gridPtr3)   , 2.0);
  sum += pow(*(gridPtr1+1)- 2 * *(gridPtr2+1) + *(gridPtr3+1) , 2.0);
  sum += pow(*(gridPtr1+2)- 2 * *(gridPtr2+2) + *(gridPtr3+2) , 2.0);
  
  gridPtr1 = (double *)grid->GetScalarPointer(i,jp,k);
  gridPtr2 = (double *)grid->GetScalarPointer(i,j,k);
  gridPtr3 = (double *)grid->GetScalarPointer(i,jm,k);
  sum += pow(*(gridPtr1)  - 2 * *(gridPtr2)   + *(gridPtr3)   , 2.0);
  sum += pow(*(gridPtr1+1)- 2 * *(gridPtr2+1) + *(gridPtr3+1) , 2.0);
  sum += pow(*(gridPtr1+2)- 2 * *(gridPtr2+2) + *(gridPtr3+2) , 2.0);
  
  gridPtr1 = (double *)grid->GetScalarPointer(i,j,kp);
  gridPtr2 = (double *)grid->GetScalarPointer(i,j,k);
  gridPtr3 = (double *)grid->GetScalarPointer(i,j,km);
  sum += pow(*(gridPtr1)  - 2 * *(gridPtr2)   + *(gridPtr3)   , 2.0);
  sum += pow(*(gridPtr1+1)- 2 * *(gridPtr2+1) + *(gridPtr3+1) , 2.0);
  sum += pow(*(gridPtr1+2)- 2 * *(gridPtr2+2) + *(gridPtr3+2) , 2.0);
  
  gridPtr1 = (double *)grid->GetScalarPointer(ip,jp,k);
  gridPtr2 = (double *)grid->GetScalarPointer(im,jp,k);
  gridPtr3 = (double *)grid->GetScalarPointer(ip,jm,k);
  gridPtr4 = (double *)grid->GetScalarPointer(im,jm,k);
  sum += pow((*(gridPtr1  )-*(gridPtr2  )-*(gridPtr3  )+*(gridPtr4  ))/4.0, 2.0)*2;
  sum += pow((*(gridPtr1+1)-*(gridPtr2+1)-*(gridPtr3+1)+*(gridPtr4+1))/4.0, 2.0)*2;
  sum += pow((*(gridPtr1+2)-*(gridPtr2+2)-*(gridPtr3+2)+*(gridPtr4+2))/4.0, 2.0)*2;
  
  gridPtr1 = (double *)grid->GetScalarPointer(ip,j,kp);
  gridPtr2 = (double *)grid->GetScalarPointer(im,j,kp);
  gridPtr3 = (double *)grid->GetScalarPointer(ip,j,km);
  gridPtr4 = (double *)grid->GetScalarPointer(im,j,km);
  sum += pow((*(gridPtr1  )-*(gridPtr2  )-*(gridPtr3  )+*(gridPtr4  ))/4.0, 2.0)*2;
  sum += pow((*(gridPtr1+1)-*(gridPtr2+1)-*(gridPtr3+1)+*(gridPtr4+1))/4.0, 2.0)*2;
  sum += pow((*(gridPtr1+2)-*(gridPtr2+2)-*(gridPtr3+2)+*(gridPtr4+2))/4.0, 2.0)*2;
  
  gridPtr1 = (double *)grid->GetScalarPointer(i,jp,kp);
  gridPtr2 = (double *)grid->GetScalarPointer(i,jm,kp);
  gridPtr3 = (double *)grid->GetScalarPointer(i,jp,km);
  gridPtr4 = (double *)grid->GetScalarPointer(i,jm,km);
  sum += pow((*(gridPtr1  )-*(gridPtr2  )-*(gridPtr3  )+*(gridPtr4  ))/4.0, 2.0)*2;
  sum += pow((*(gridPtr1+1)-*(gridPtr2+1)-*(gridPtr3+1)+*(gridPtr4+1))/4.0, 2.0)*2;
  sum += pow((*(gridPtr1+2)-*(gridPtr2+2)-*(gridPtr3+2)+*(gridPtr4+2))/4.0, 2.0)*2;
  
  return (sum)*scale;

}
//-------------------------------------------------------------------------
double vtkpxGridTransform::ComputeJacobianConstraint(float gamma)
{

  fprintf(stdout,"\n\n\n\n-------------------------\n\n\n\nDoing jacobian constraint\n\n\n\n-------------------------\n\n\n\n");
	double sum=0.0;
	int nc=this->NumberOfControlPoints;
	if (nc==0)
		return 0.0;

	for (int i=0;i<nc;i++)
	{
		int ind[3];
		float p[3],p2[3],der[3][3];
		this->GetControlPointIndices(i,ind);
		p[0]=ind[0];p[1]=ind[1];p[2]=ind[2];
		this->InternalTransformDerivative(p,p2,der);

		double v1=vtkMath::Determinant3x3(der);

		if ( v1<=gamma )
			sum+=(gamma*gamma)/(v1*v1)-2;
		else
			sum+=0.0;
	}
	return sum;
}
// ---------------------------------------------------------------------------------------
double vtkpxGridTransform::ComputeTotalStrainEnergy(double v)
{
  double sum=0.0;
  int nc=this->NumberOfControlPoints;
  if (nc==0)
    return 0.0;

  for (int i=0;i<nc;i++)
    sum+=this->ComputeStrainEnergyAtControlPoint(i,v);
  return sum;

}
// ---------------------------------------------------------------------------------------
double vtkpxGridTransform::ComputeStrainEnergyAtControlPoint(int cpoint,double v)
{
  /*  double p1[3];
      this->GetControlPointLocation(cpoint,p1);
      return this->ComputeStrainEnergyAtPoint(p1,v);*/

  int ind[3];  this->GetControlPointIndices(cpoint,ind);
  vtkImageData *grid=this->DisplacementGrid;
  double *gridPtr1,*gridPtr2,*gridPtr3;

  //double sp=(this->GridSpacing[0]+this->GridSpacing[1]+this->GridSpacing[2])/3.0;
  double scale=0.01*(1.0/(double(this->NumberOfControlPoints)));

  int i=ind[0];
  int j=ind[1];
  int k=ind[2];
  

  int ip=i+1; if (ip>=this->GridDimensions[0]-1) ip=this->GridDimensions[0]-2;
  int jp=j+1; if (jp>=this->GridDimensions[1]-1) jp=this->GridDimensions[1]-2;
  int kp=k+1; if (kp>=this->GridDimensions[2]-1) kp=this->GridDimensions[2]-2;
  
  int im=i-1; if (im<1) im=1;
  int jm=j-1; if (jm<1) jm=1;
  int km=k-1; if (km<1) km=1;

  double det[3][3];
  
  gridPtr1 = (double *)grid->GetScalarPointer(ip,j,k);
  gridPtr2 = (double *)grid->GetScalarPointer(i,j,k);
  gridPtr3 = (double *)grid->GetScalarPointer(im,j,k);
   
  // du/dx
  det[0][0]= *(gridPtr1)  - 2 * *(gridPtr2)   + *(gridPtr3);
  // dv/dx
  det[1][0]= *(gridPtr1+1)- 2 * *(gridPtr2+1) + *(gridPtr3+1);
  // dw/dx
  det[2][0]= *(gridPtr1+2)- 2 * *(gridPtr2+2) + *(gridPtr3+2);
  
  
  gridPtr1 = (double *)grid->GetScalarPointer(i,jp,k);
  gridPtr2 = (double *)grid->GetScalarPointer(i,j,k);
  gridPtr3 = (double *)grid->GetScalarPointer(i,jm,k);
  
  // du/dy
  det[0][1]= *(gridPtr1)  - 2 * *(gridPtr2)   + *(gridPtr3)  ;
  // dv/dy
  det[1][1]= *(gridPtr1+1)- 2 * *(gridPtr2+1) + *(gridPtr3+1);
  // dw/dy
  det[2][1]= *(gridPtr1+2)- 2 * *(gridPtr2+2) + *(gridPtr3+2);
  
  
  gridPtr1 = (double *)grid->GetScalarPointer(i,j,kp);
  gridPtr2 = (double *)grid->GetScalarPointer(i,j,k);
  gridPtr3 = (double *)grid->GetScalarPointer(i,j,km);
  
  // du/dz
  det[0][2]= *(gridPtr1)  - 2 * *(gridPtr2)   + *(gridPtr3)  ;
  // dv/dz
  det[1][2]= *(gridPtr1+1)- 2 * *(gridPtr2+1) + *(gridPtr3+1);
  // dw/dz
  det[2][2]= *(gridPtr1+2)- 2 * *(gridPtr2+2) + *(gridPtr3+2);
  
  
  double shear[3];
  shear[0]=0.5*(det[0][1]+det[1][0]);
  shear[1]=0.5*(det[0][2]+det[2][0]);
  shear[2]=0.5*(det[1][2]+det[2][1]);
  
  double sum=0.0;
  sum+= det[0][0]*((1.0-v)*det[0][0]+v*(det[1][1]+det[2][2]));
  sum+= det[1][1]*((1.0-v)*det[1][1]+v*(det[0][0]+det[2][2]));
  sum+= det[2][2]*((1.0-v)*det[2][2]+v*(det[0][0]+det[1][1]));
  for (i=0;i<=2;i++)
    sum+=(0.5-v)*pow(shear[i],2.0);
  
  return scale*sum/( (1.0+v)*(1.0-2.0*v));
}
// --------------------------------------------------------------------------------------- 
double vtkpxGridTransform::ComputeStrainEnergyAtPoint(double p1[3],double v)
{
  if (v<0.0)
    v=0.0;
  else if (v>=0.5)
    v=0.49;

  // Linear Elastic Stuff 
  double det[3][3],p2[3];
  this->InternalTransformDerivative(p1,p2,det);

  for (int ia=0;ia<=2;ia++)
    det[ia][ia]-=1.0;

  double shear[3];
  shear[0]=0.5*(det[0][1]+det[1][0]);
  shear[1]=0.5*(det[0][2]+det[2][0]);
  shear[2]=0.5*(det[1][2]+det[2][1]);

  double sum=0.0;
  sum+= det[0][0]*((1.0-v)*det[0][0]+v*(det[1][1]+det[2][2]));
  sum+= det[1][1]*((1.0-v)*det[1][1]+v*(det[0][0]+det[2][2]));
  sum+= det[2][2]*((1.0-v)*det[2][2]+v*(det[0][0]+det[1][1]));
  for (int i=0;i<=2;i++)
    sum+=(0.5-v)*pow(shear[i],2.0);
  
  return sum/( (1.0+v)*(1.0-2.0*v));
}
// ---------------------------------------------------------------------------------------
double vtkpxGridTransform::ComputeWeightedDistance(vtkpxGridTransform* other,int cpindex,vtkDoubleArray* weights0)
{
  if (other==NULL)
    {
      vtkErrorMacro(<<"Really Bad Prior\n");
      return 0;
    }

  int np=other->GetNumberOfControlPoints();
  if (np!=this->GetNumberOfControlPoints())
    {
      vtkErrorMacro(<<"Bad Prior\n");
      return 0;
    }
  
  vtkDoubleArray* weights=weights0;

  double weight=1.0;
  if (weights0!=NULL)
    {
      if (weights->GetNumberOfTuples()!=np)
	weights=NULL;
    }
  int mincpindex=0;
  int maxcpindex=np-1;
  
  if (cpindex>=0)
    {
      mincpindex=Irange(cpindex,0,maxcpindex);
      maxcpindex=mincpindex;
    }
  
  double v=0;
  double scale=1.0/double(np);
  for (int cp=mincpindex;cp<=maxcpindex;cp++)
    {
      double d1[3],d2[3];
      other->GetControlPointDisplacement(cp,d1);
      this->GetControlPointDisplacement(cp,d2);
      double d=(vtkMath::Distance2BetweenPoints(d1,d2));
      if (weights!=NULL)
	weight=weights->GetComponent(cp,0);
      v+=d*weight;
    }
  return v*scale;
}
// ---------------------------------------------------------------------------------------
int vtkpxGridTransform::FormOutputWeights(vtkCollection* collection,vtkpxMatrix* PointWeights,vtkDoubleArray* FinalWeights)
{
  if (collection==NULL || PointWeights==NULL || FinalWeights==NULL)
    return 0;
 
  int numc=collection->GetNumberOfItems();
  if (FinalWeights->GetNumberOfTuples()!=numc)
    FinalWeights->SetNumberOfTuples(numc);

  collection->InitTraversal();
  double sum=1.0;
  for (int p=0;p<=1;p++)
    {
      for (int i=0;i<numc;i++)
	{
	  if (p==0)

	    {
	      vtkDoubleArray* indices=(vtkDoubleArray*)collection->GetNextItemAsObject();
	      int n=indices->GetNumberOfTuples();
	      double total=0.0;
	      for (int j=0;j<n;j++)
		{
		  int point=(int)indices->GetComponent(j,0);
		  double wgt=indices->GetComponent(j,1);
		  double wpt=PointWeights->GetDirectElement(point,0);
		  total+=wgt*wpt;
		}
	      FinalWeights->SetComponent(i,0,total);
	      sum+=total;
	    }
	  else
	    {
	      FinalWeights->SetComponent(i,0,FinalWeights->GetComponent(i,0)/sum);
	    }
	}

      if(p==0)
	sum/=double(numc);

    }
  return 1;
}

// ---------------------------------------------------------------------------------------
// Optimization Structure
// ---------------------------------------------------------------------------------------

vtkpxGridTransformOptimizationStructure::vtkpxGridTransformOptimizationStructure() {
  this->SetValues(NULL,NULL,NULL,0,1.0,NULL);
}

void vtkpxGridTransformOptimizationStructure::SetValues(vtkCollection* ControlsCollection0,
							vtkPoints* points0,
							vtkPoints* points2_0,
							double step0,
							double lambda0,
							vtkpxMatrix* weights0,		 
							vtkpxGridTransform* prior0,
							vtkDoubleArray* priorweights0)
{
  this->ControlsCollection=ControlsCollection0;
  this->points=points0;
  this->points2=points2_0;
  this->step=step0;
  this->lambda=lambda0;
  this->error=0.0;
  this->bend=0.0;
  this->weights=weights0;
  this->priorTransform=prior0;
  this->priorWeights=priorweights0;
}

// ---------------------------------------------------------------------------------------
int vtkpxGridTransform::SmoothCollection(vtkCollection* TransformationCollection,int periodic,double sigma)
{
  if (TransformationCollection==NULL)
    return 0;

  int nt=  TransformationCollection->GetNumberOfItems();
  
  TransformationCollection->InitTraversal();
  // First Make sure there all vtkpxGridTransforms, then get number of control points ....
  int numberofcontrolpoints=0;

  double** arr=new double*[nt];
  int flag=0;

  for (int i=0;i<nt;i++)
    {
      vtkObject* tr= TransformationCollection->GetNextItemAsObject();
      if (tr->IsA("vtkpxGridTransform")==1)
	{
	  if (i==0)
	    {
	      numberofcontrolpoints=((vtkpxGridTransform*)tr)->GetNumberOfControlPoints();
	    }
	  else
	    {
	      int nc=((vtkpxGridTransform*)tr)->GetNumberOfControlPoints();
	      if (numberofcontrolpoints!=nc)
		{
		  fprintf(stderr,"Bad Grid Transform nc=%d vs %d\n",nc,numberofcontrolpoints);
		  flag=1;
		  i=nt+1;
		}
	    }
	  if (flag==0)
	    arr[i]=((vtkpxGridTransform*)tr)->GetControlPointPointer(0);
	}
    }

  if (flag==0)
    {
      delete [] arr;
      return 0;
    }


  double weights[5] = { 0.05,0.25,0.4,0.25,0.04 };
  double* values=new double[nt];
  int    index[5];

  int numberofvalues=numberofcontrolpoints*3;
  for (int i=0;i<numberofvalues;i++)
    {
      // Copy into small array first 
      for (int j=0;j<nt;j++)
	values[j]=arr[j][i];
      
      for (int j=0;j<nt;j++)
	{
	  for (int ia=0;ia<=4;ia++)
	    {
	      index[ia]=j+(ia-2);
	      if (periodic)
		{
		  if (index[ia]<0)
		    index[ia]+=nt;
		  else if (index[ia]>=nt)
		    index[ia]-=nt;
		}
	      if (index[ia]<0)
		index[ia]=0;
	      else if (index[ia]>=nt)
		index[ia]=nt-1;
	    }
	  double newv=0.0;
	  for (int ia=0;ia<=4;ia++)
	    newv+=weights[ia]*values[index[ia]];
	
	  arr[j][i]=newv;
	}
    }
  
  delete [] values;
  delete [] arr;

  return 1;
}
// ---------------------------------------------------------------------------------------

