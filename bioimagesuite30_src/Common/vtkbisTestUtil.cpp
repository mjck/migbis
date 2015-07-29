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




#include "vtkObjectFactory.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkMath.h"
#include "vtkbisTestUtil.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkCellArray.h"
#include "vtkPointLocator.h"

vtkbisTestUtil* vtkbisTestUtil::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisTestUtil");
  if(ret)
      {
	return (vtkbisTestUtil*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisTestUtil;
}

vtkbisTestUtil::vtkbisTestUtil() 
{ 
  this->ClearStatus();
}

const char* vtkbisTestUtil::ReturnStatus() 
{
  return this->Status.c_str();
}

void vtkbisTestUtil::ClearStatus()
{
  this->Status="";
}

// ----------------------------------------------------------------------------------------------------
//
//  Data Arrays
//
// ----------------------------------------------------------------------------------------------------

int vtkbisTestUtil::CompareDataArrays(vtkDataArray* data1,vtkDataArray* data2)
{
  return vtkbisTestUtil::CompareDataArrays(data1,data2,0.001);

}
int vtkbisTestUtil::CompareDataArrays(vtkDataArray* data1,vtkDataArray* data2,double threshold)
{

  if (data1==NULL && data2==NULL)
    return 1;

  if (data1==NULL || data2==NULL)
    return 0;


  if (data1->GetDataType()!=data2->GetDataType())
    {
      this->Status+="**** DataArray:Warning Differences in data type";
      //      return 0;
    }

  if (data1->GetNumberOfTuples()!=data2->GetNumberOfTuples())
    {
      this->Status+="**** DataArray:Error Differences in number of tuples";
      return 0;
    }

  if (data1->GetNumberOfComponents()!=data2->GetNumberOfComponents())
    {
      this->Status+="**** DataArray:Error Difference in number of components";
      return 0;
    }

  int nt=data1->GetNumberOfTuples();
  int nc=data1->GetNumberOfComponents();

  double maxd=0.0;
  for (int ia=0;ia<nt;ia++)
    for (int ib=0;ib<nc;ib++)
      {
	//	fprintf(stdout,"Point %d Comp %d %f vs %f\n",ia,ib,data1->GetComponent(ia,ib),data2->GetComponent(ia,ib));
	double v=fabs(data1->GetComponent(ia,ib)-data2->GetComponent(ia,ib));
	if (v>maxd)
	  maxd=v;
      }

  if (nt>0)
    {
      sprintf(this->Buffer,"Comparison max=%.5f, thr=%.5f ",maxd,threshold);
      this->Status+=this->Buffer;
    }

  if (maxd>threshold)
    return 0;

  return 1;
}

// ----------------------------------------------------------------------------------------------------

int vtkbisTestUtil::CorrelateDataArrays(vtkDataArray* data1,vtkDataArray* data2)
{
  return vtkbisTestUtil::CorrelateDataArrays(data1,data2,0.001);

}
int vtkbisTestUtil::CorrelateDataArrays(vtkDataArray* data1,vtkDataArray* data2,double threshold)
{

  if (data1==NULL && data2==NULL)
    return 1;

  if (data1==NULL || data2==NULL)
    return 0;


  if (data1->GetDataType()!=data2->GetDataType())
    {
      this->Status+="**** CorrDataArray:Warning Differences in data type";
    }

  if (data1->GetNumberOfTuples()!=data2->GetNumberOfTuples())
    {
      this->Status+="**** CorrDataArray:Error Differences in number of tuples";
      return 0;
    }

  if (data1->GetNumberOfComponents()!=data2->GetNumberOfComponents())
    {
      this->Status+="**** CorrDataArray:Error Differences in number of components";
      return 0;
    }

  int nt=data1->GetNumberOfTuples();
  int nc=data1->GetNumberOfComponents();

  double mean[2],var[2],sum[2],sum2[2],sumprod=0.0;
  sum[0]=0.0; sum[1]=0.0; sum2[0]=0.0; sum2[1]=0.0;
  int numscalars=0;
  for (int i=0;i<nt;i++)
    for (int j=0;j<nc;j++)
      {
	double v0=data1->GetComponent(i,j);
	sum[0]+=v0;
	sum2[0]+=v0*v0;
	
	double v1=data2->GetComponent(i,j);
	sum[1]+=v1;
	sum2[1]+=v1*v1;
	sumprod+=v0*v1;
	++numscalars;
      }
  
  for (int j=0;j<=1;j++)
    {
      mean[j]=sum[j]/double(numscalars);
      var[j] =sum2[j]/double(numscalars)-mean[j]*mean[j];
      if (var[j]<0.00001)
	var[j]=0.00001;
    }

  double covar=pow(sumprod/double(numscalars)-mean[0]*mean[1],double(2.0));
  double covar2=covar/(var[0]*var[1]);

  sprintf(this->Buffer,"cc=%.5f, thr=%.5f",covar2,threshold);
  this->Status+=this->Buffer;

  if (covar2<threshold)
    return 0;

  return 1;
}

// ----------------------------------------------------------------------------------------------------
// Images
// ----------------------------------------------------------------------------------------------------
int vtkbisTestUtil::CompareImages(vtkImageData* img1,vtkImageData* img2) 
{
  return vtkbisTestUtil::CompareImages(img1,img2,0.001);
}
// ----------------------------------------------------------------------------------------------------
int vtkbisTestUtil::CorrelateImages(vtkImageData* img1,vtkImageData* img2) 
{
  return vtkbisTestUtil::CorrelateImages(img1,img2,0.9);
}
// ----------------------------------------------------------------------------------------------------
int vtkbisTestUtil::CompareImages(vtkImageData* img1,vtkImageData* img2,double threshold)
{

  if (img1==NULL || img2==NULL)
    return 0;

  int dim1[3]; img1->GetDimensions(dim1);
  int dim2[3]; img2->GetDimensions(dim2);
  double spa1[3]; img1->GetSpacing(spa1);
  double spa2[3]; img2->GetSpacing(spa2);

  double sum=0.0;
  for (int ia=0;ia<=2;ia++)
    {
      sum+= double(abs(dim1[ia]-dim2[ia]));
      sum+= fabs(spa1[ia]-spa2[ia]);
    }

  if (sum>0.01*threshold)
    {
      this->Status+="**** Image:Error Differences in dimensions/spacing:";
      for (int ia=0;ia<=2;ia++)
	{
	  sprintf(this->Buffer,"ia=%d dim (%d vs %d) spa=(%f vs %f)\t",ia,dim1[ia],dim2[ia],spa1[ia],spa2[ia]);
	  this->Status+=this->Buffer;
	}

      return 0;
    }

  vtkDataArray* data1=img1->GetPointData()->GetScalars();
  vtkDataArray* data2=img2->GetPointData()->GetScalars();

  return vtkbisTestUtil::CompareDataArrays(data1,data2,threshold);

}


// ----------------------------------------------------------------------------------------------------
int vtkbisTestUtil::CorrelateImages(vtkImageData* img1,vtkImageData* img2,double threshold)
{

  if (img1==NULL || img2==NULL)
    return 0;


  int dim1[3]; img1->GetDimensions(dim1);
  int dim2[3]; img2->GetDimensions(dim2);
  double spa1[3]; img1->GetSpacing(spa1);
  double spa2[3]; img2->GetSpacing(spa2);

  double sum=0.0;
  for (int ia=0;ia<=2;ia++)
    {
      sum+= double(abs(dim1[ia]-dim2[ia]));
      sum+= fabs(spa1[ia]-spa2[ia]);
    }

  if (sum>0.01*threshold)
    {
      this->Status+="**** Image:Error Differences in dimensions/spacing:";
      for (int ia=0;ia<=2;ia++)
	{
	  sprintf(this->Buffer,"ia=%d dim (%d vs %d) spa=(%f vs %f)\t",ia,dim1[ia],dim2[ia],spa1[ia],spa2[ia]);
	  this->Status+=this->Buffer;
	}
    }

  vtkDataArray* data1=img1->GetPointData()->GetScalars();
  vtkDataArray* data2=img2->GetPointData()->GetScalars();

  return vtkbisTestUtil::CorrelateDataArrays(data1,data2,threshold);

}


// ----------------------------------------------------------------------------------------------------
int vtkbisTestUtil::CompareSurfaces(vtkPolyData* sur1,vtkPolyData* sur2)
{
  return vtkbisTestUtil::CompareSurfaces(sur1,sur2,0.001);
}
 
int vtkbisTestUtil::CompareSurfaces(vtkPolyData* sur1,vtkPolyData* sur2,double threshold)
{

  if (sur1==NULL || sur2==NULL)
    return 0;

  this->Status+="Points:";
  int ok=vtkbisTestUtil::CompareDataArrays(sur1->GetPoints()->GetData(),sur2->GetPoints()->GetData());
  if (ok==0)    return 0;

  
  this->Status+="Verts: ";
  ok=vtkbisTestUtil::CompareDataArrays(sur1->GetVerts()->GetData(),sur2->GetVerts()->GetData());
  if (ok==0)    return 0;
  
  this->Status+="Lines: ";
  ok=vtkbisTestUtil::CompareDataArrays(sur1->GetLines()->GetData(),sur2->GetLines()->GetData());
  if (ok==0)    return 0;

  this->Status+="Polys: ";
  ok=vtkbisTestUtil::CompareDataArrays(sur1->GetPolys()->GetData(),sur2->GetPolys()->GetData());
  if (ok==0)    return 0;

  this->Status+="Strips: ";
  ok=vtkbisTestUtil::CompareDataArrays(sur1->GetStrips()->GetData(),sur2->GetStrips()->GetData());
  if (ok==0)    return 0;

  // CellData and PointData
  // GetNumberOfArrays!
  // GetArray!
  for (int ia=0;ia<=1;ia++)
    {
      vtkFieldData* f1=sur1->GetPointData();
      vtkFieldData* f2=sur2->GetPointData();
      if (ia==1)
	{
	  f1=sur1->GetFieldData();
	  f2=sur2->GetFieldData();
	}
  
      if (f1==NULL && f2==NULL)
	{
	  // OK no issue

	}
      else if (f1==NULL || f2==NULL)
	{
	  return 0;
	}
      else
	{
	  int n=f1->GetNumberOfArrays();
	  for (int ib=0;ib<n;ib++)
	    {
	      ok=vtkbisTestUtil::CompareDataArrays(f1->GetArray(ib),f2->GetArray(ib));
	      if (ok==0)
		return 0;
	    }
	}
    }
  return ok;
}
// ----------------------------------------------------------------------------------------------------
int vtkbisTestUtil::ComparePoints(vtkPoints* pt1,vtkPoints* pt2)
{
  return vtkbisTestUtil::ComparePoints(pt1,pt2,0.001);
}
 
int vtkbisTestUtil::ComparePoints(vtkPoints* pt1,vtkPoints* pt2,double threshold)
{

  if (pt1==NULL || pt2==NULL)
    return 0;

  return vtkbisTestUtil::CompareDataArrays(pt1->GetData(),pt2->GetData(),threshold);
}

// -----------------------------------------------------------------------------------------------------------
int vtkbisTestUtil::CompareSurfacesByCorrespondence(vtkPolyData* surface1,vtkPolyData* surface2,double threshold)
{


  vtkPointLocator* locator=vtkPointLocator::New();
  locator->SetNumberOfPointsPerBucket(1);
  locator->SetDataSet(surface2);
  locator->BuildLocator();

  int np=surface1->GetNumberOfPoints();
  double maxd=0.0;
  double dthr=threshold*threshold;
  for (int i=0;i<np;i++)
    {
      double x[3]; surface1->GetPoint(i,x);
      int id=locator->FindClosestPoint(x);
      double y[3]; surface2->GetPoint(id,y);
      
      double d=vtkMath::Distance2BetweenPoints(x, y);
      if (d>maxd)
	maxd=d;
    }
  locator->Delete();
  maxd=sqrt(maxd);

  sprintf(this->Buffer,"+++++ SurfaceICP Comparison max=%.5f, thr=%.5f",maxd,threshold);
  this->Status+=this->Buffer;
  if (maxd>threshold)
    return 0;
  return 1;

}

