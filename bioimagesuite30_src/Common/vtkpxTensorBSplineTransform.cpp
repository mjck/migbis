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


#include "vtkpxTensorBSplineTransform.h"
#include "vtkMath.h"
#include "vtkImageData.h"
#include "vtkDoubleArray.h"
#include "vtkObjectFactory.h"
#include "vtkCollection.h"
#include "pxisinf.h"

//----------------------------------------------------------------------------
vtkpxTensorBSplineTransform* vtkpxTensorBSplineTransform::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxTensorBSplineTransform");
  if(ret)
    {
      return (vtkpxTensorBSplineTransform*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxTensorBSplineTransform;
}

//------------------------------------------------------------------------
vtkpxTensorBSplineTransform::vtkpxTensorBSplineTransform()
{
  for (int ia=0;ia<=2;ia++)
    {
      this->Dimensions[ia]=0;
      this->Origin[ia]=0.0;
      this->Spacing[ia]=1.0;
      this->OffsetDimensions[ia]=1;
    }

  this->WholeImageCache=NULL;
  this->DetailImageCache=NULL;
  this->Displacements=NULL;
  this->CachedPointsInfo=NULL;
  this->CachedPoints=NULL;
  this->ActiveControlPointList=NULL;
  this->ControlPointStatus=NULL;
  this->NumberOfActiveComponents=3;
  this->CacheMode=0;
}
//------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::Copy(vtkpxTensorBSplineTransform* other)
{
  if (other==NULL)
    {
      vtkErrorMacro(<<"Copy Transform given NULL argument");
      return;
    }
  other->GetDimensions(this->Dimensions);
  other->GetOffsetDimensions(this->OffsetDimensions);
  other->GetSpacing(this->Spacing);
  other->GetOrigin(this->Origin);
  this->NumberOfActiveComponents=other->GetNumberOfActiveComponents();
  this->CacheMode=other->GetCacheMode();
  this->NumberOfControlPoints=other->GetNumberOfControlPoints();

  if (other->GetDisplacements()!=NULL)
    {
      if (this->Displacements==NULL)
	this->Displacements=vtkDoubleArray::New();
      this->Displacements->DeepCopy(other->GetDisplacements());
    }
  else
    {
      if (this->Displacements!=NULL)
	this->Displacements->Delete();
      this->Displacements=NULL;
    }

  if (other->GetControlPointStatus()!=NULL)
    {
      if (this->ActiveControlPointList==NULL)
	this->ActiveControlPointList=vtkIntArray::New();
      if (this->ControlPointStatus==NULL)
	this->ControlPointStatus=vtkCharArray::New();

      this->ControlPointStatus->DeepCopy(other->GetControlPointStatus());
      this->ActiveControlPointList->DeepCopy(other->GetActiveControlPointList());
      
    }
  else
    {
      if (this->ActiveControlPointList!=NULL)
	this->ActiveControlPointList->Delete();
      if (this->ControlPointStatus==NULL)
	this->ControlPointStatus->Delete();
      
      this->ActiveControlPointList=NULL;
      this->ControlPointStatus=NULL;
    }
  
  if (other->GetWholeImageCache()!=NULL)
    {
      if (this->WholeImageCache==NULL)
	this->WholeImageCache=vtkImageData::New();
      this->WholeImageCache->DeepCopy(other->GetWholeImageCache());
    }
  else
    {
      if (this->WholeImageCache!=NULL)
	this->WholeImageCache->Delete();
      this->WholeImageCache=NULL;
    }


  if (other->GetDetailImageCache()!=NULL)
    {
      if (this->DetailImageCache==NULL)
	this->DetailImageCache=vtkImageData::New();
      this->DetailImageCache->DeepCopy(other->GetDetailImageCache());
    }
  else
    {
      if (this->DetailImageCache!=NULL)
	this->DetailImageCache->Delete();
      this->DetailImageCache=NULL;
    }

  if (other->GetCachedPointsInfo()!=NULL)
    {
      if (this->CachedPointsInfo==NULL)
	this->CachedPointsInfo=vtkFloatArray::New();
      this->CachedPointsInfo->DeepCopy(other->GetCachedPointsInfo());
    }
  else
    {
      if (this->CachedPointsInfo!=NULL)
	this->CachedPointsInfo->Delete();
      this->CachedPointsInfo=NULL;
    }

  if (other->GetCachedPoints()!=NULL)
    {
      if (this->CachedPoints==NULL)
	this->CachedPoints=vtkPolyData::New();
      this->CachedPoints->DeepCopy(other->GetCachedPoints());
    }
  else
    {
      if (this->CachedPoints!=NULL)
	this->CachedPoints->Delete();
      this->CachedPoints=NULL;
    }
}
//------------------------------------------------------------------------
vtkpxTensorBSplineTransform::~vtkpxTensorBSplineTransform()
{
  if (this->WholeImageCache!=NULL)
    this->WholeImageCache->Delete();

  if (this->DetailImageCache!=NULL)
    this->DetailImageCache->Delete();

  if (this->Displacements!=NULL)
    this->Displacements->Delete();

  if (this->CachedPointsInfo!=NULL)
    this->CachedPointsInfo->Delete();

  if (this->CachedPoints!=NULL)
    this->CachedPoints->Delete();

  if (this->ActiveControlPointList!=NULL)
    this->ActiveControlPointList->Delete();

  if (this->ControlPointStatus!=NULL)
    this->ControlPointStatus=NULL;
}

//------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkWarpTransform::PrintSelf(os,indent);
  
}

//----------------------------------------------------------------------------
vtkAbstractTransform *vtkpxTensorBSplineTransform::MakeTransform()
{
  return vtkpxTensorBSplineTransform::New(); 
}

//----------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::InternalDeepCopy(vtkAbstractTransform *transform)
{
  vtkpxTensorBSplineTransform *t = (vtkpxTensorBSplineTransform *)transform;

  this->SetInverseTolerance(t->InverseTolerance);
  this->SetInverseIterations(t->InverseIterations);
  //  this->SetLandmarks(t->Landmarks);

  if (this->InverseFlag != t->InverseFlag)
    {
      this->InverseFlag = t->InverseFlag;
      this->Modified();
    }
}
//------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::InternalUpdate()
{
  return;
}
//------------------------------------------------------------------------
template<class T>
static inline void vtkpxTensorBSplineForwardTransformPoint(vtkpxTensorBSplineTransform *self,const T point[3], T output[3])
{
  double s[3];
  int  lat[3];

  double pp[3]; 
  for (int ia=0;ia<=2;ia++)
    {
      pp[ia]=point[ia];
      output[ia]=point[ia];
    }
  
  self->GetCoordinatesInGrid(pp,lat,s);

  int GridDimensions[3]; self->GetDimensions(GridDimensions);
  int GridSliceDimensions=GridDimensions[0]*GridDimensions[1];
  double ori[3]; self->GetOrigin(ori);
  double spa[3]; self->GetSpacing(spa);
  

  for (int k = 0; k < 4; k++)
    {
      int K = Irange(k + lat[2] - 1,0,GridDimensions[2]-1);
      for (int j = 0; j < 4; j++)
	{
	  int J = Irange(j + lat[1] - 1,0,GridDimensions[1]-1);
	  for (int i = 0; i < 4; i++)
	    {
	      int I = Irange(i + lat[0] - 1,0,GridDimensions[0]-1);
	      int   cpoint=I+J*GridDimensions[0]+K*GridSliceDimensions;
	      double wgt= vtkpxTensorBSplineTransform::B(i, s[0]) * vtkpxTensorBSplineTransform::B(j, s[1]) * vtkpxTensorBSplineTransform::B(k, s[2]);
	      double d[3]; self->GetControlPointDisplacement(cpoint,d);
	      for (int ia=0;ia<=2;ia++)
		output[ia]+=wgt*d[ia];
	    }
	}
    }

}
//------------------------------------------------------------------------
template<class T>
static inline void vtkpxTensorBSplineTransformDerivative(vtkpxTensorBSplineTransform *self,const T point[3], T output[3],T derivative[3][3])
{
  
  double s[3];
  int  lat[3];
  
  double pp[3]; 
  for (int ia=0;ia<=2;ia++)
    {
      pp[ia]=point[ia];
      output[ia]=0.0;
      for (int ib=0;ib<=2;ib++)
	derivative[ia][ib]=0.0;
    }
  
  self->GetCoordinatesInGrid(pp,lat,s);

  int GridDimensions[3]; self->GetDimensions(GridDimensions);
  int GridSliceDimensions=GridDimensions[0]*GridDimensions[1];
  
 
  for (int k = 0; k < 4; k++)
    {
      int K = Irange(k + lat[2] - 1,0,GridDimensions[2]-1);
      for (int j = 0; j < 4; j++)
	{
	  int J = Irange(j + lat[1] - 1,0,GridDimensions[1]-1);
	  for (int i = 0; i < 4; i++)
	    {
	      int I = Irange(i + lat[0] - 1,0,GridDimensions[0]-1);
	      int   cpoint=I+J*GridDimensions[0]+K*GridSliceDimensions;
	      double wgt= vtkpxTensorBSplineTransform::B(i, s[0]) * vtkpxTensorBSplineTransform::B(j, s[1]) * vtkpxTensorBSplineTransform::B(k, s[2]);
	      double w[3];
	      w[0]=vtkpxTensorBSplineTransform::dB(i, s[0]) * vtkpxTensorBSplineTransform::B(j, s[1]) * vtkpxTensorBSplineTransform::B(k, s[2]);
	      w[1]=vtkpxTensorBSplineTransform::B(i, s[0]) * vtkpxTensorBSplineTransform::dB(j, s[1]) * vtkpxTensorBSplineTransform::B(k, s[2]);
	      w[2]=vtkpxTensorBSplineTransform::B(i, s[0]) * vtkpxTensorBSplineTransform::B(j, s[1]) * vtkpxTensorBSplineTransform::dB(k, s[2]);

	      double d[3];
	      self->GetControlPointDisplacement(cpoint,d);
	      for (int ia=0;ia<=2;ia++)
		{
		  output[ia]+=wgt*d[ia];
		  for (int ib=0;ib<=2;ib++)
		    derivative[ia][ib]=w[ib]*d[ia];
		}
	    }
	}
    }
}
//------------------------------------------------------------------------

void vtkpxTensorBSplineTransform::ForwardTransformPoint(const double point[3],double output[3])
{
  if (this->InverseFlag)
    {
      vtkWarpTransform::InverseTransformPoint(point,output);
      return;
    }

  vtkpxTensorBSplineForwardTransformPoint(this,point,output);

}

void vtkpxTensorBSplineTransform::ForwardTransformPoint(const float point[3],float output[3])
{
  if (this->InverseFlag)
    {
      vtkWarpTransform::InverseTransformPoint(point,output);
      return;
    }

  vtkpxTensorBSplineForwardTransformPoint(this,point,output);

}

//------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::ForwardTransformDerivative(const double point[3],
							     double output[3],
							     double derivative[3][3])
{
  vtkpxTensorBSplineTransformDerivative(this,point,output,derivative);
}

void vtkpxTensorBSplineTransform::ForwardTransformDerivative(const float point[3],
							     float output[3],
							     float derivative[3][3])
{
  vtkpxTensorBSplineTransformDerivative(this,point,output,derivative);
}


//----------------------------------------------------------------------------
double vtkpxTensorBSplineTransform::B(int i, double t)
{
  switch (i) 
    {
    case 0:
      return (1-t)*(1-t)*(1-t)/6.0;
      break;
      
    case 1:
      return (3*t*t*t - 6*t*t + 4)/6.0;
    case 2:
      return (-3*t*t*t + 3*t*t + 3*t + 1)/6.0;
      
    case 3:
      return (t*t*t)/6.0;
    }
  return 0;
}
//----------------------------------------------------------------------------
double vtkpxTensorBSplineTransform::dB(int i, double t)
{
  switch (i) 
    {
    case 0:
      return -(1-t)*(1-t)/2.0;
    case 1:
      return (9*t*t - 12*t)/6.0;
    case 2:
      return (-9*t*t + 6*t + 3)/6.0;
    case 3:
      return (t*t)/2.0;
    }
  return 0;
}
//----------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::Load(const char* fname)
{  
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  int ok=Load(fin);
  gzclose(fin);

  return ok;
}

int vtkpxTensorBSplineTransform::Load(gzFile fin,int skipheader,const char* header)
{
  char pxtk_buffer[255];
  
  if (skipheader==0)
    gzgets(fin,pxtk_buffer,100);
  else
    strncpy(pxtk_buffer,header,255);
  
  int interpmode=0;
  if (gzfindstringinstring(pxtk_buffer,"#vtkpxBaseGridTransform File")==0)
    {
      if (gzfindstringinstring(pxtk_buffer,"#vtkpxBaseGridTransform2 File")==0)
	return 0;
      else
	interpmode=1;
    }
  
  gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
  sscanf(pxtk_buffer,"%lf %lf %lf",&this->Origin[0],&this->Origin[1],&this->Origin[2]);
  gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
  sscanf(pxtk_buffer,"%lf %lf %lf",&this->Spacing[0],&this->Spacing[1],&this->Spacing[2]);
  gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
  sscanf(pxtk_buffer,"%d %d %d",&this->Dimensions[0],&this->Dimensions[1],&this->Dimensions[2]);
  
  if (interpmode)
    {
      // Skip over interpolation stuff
      gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
    }

  if (this->Displacements==NULL)
    this->Displacements=vtkDoubleArray::New();
  this->Displacements->SetNumberOfComponents(3);
  this->Displacements->SetNumberOfTuples(this->Dimensions[0]*this->Dimensions[1]*this->Dimensions[2]);

  this->OffsetDimensions[0]=1;
  this->OffsetDimensions[1]=this->Dimensions[0];
  this->OffsetDimensions[2]=this->Dimensions[0]*this->Dimensions[1];

  this->NumberOfControlPoints=this->OffsetDimensions[2]*this->Dimensions[2];
  gzgets(fin,pxtk_buffer,100);  

  for (int ia=0;ia<=2;ia++)
    this->Displacements->FillComponent(ia,0.0);

  int np=this->Displacements->GetNumberOfTuples();
  int tmp=0;
  double dx[3];
  for (int i=0;i<np;i++)
    {
      gzgets(fin,pxtk_buffer,100);
      sscanf(pxtk_buffer,"%d %lf %lf %lf",&tmp,&dx[0],&dx[1],&dx[2]);
      this->Displacements->SetTuple(i,dx);
    }

  this->InitializeStatusAndCaches();

  return 1;
}
//----------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::Save(const char* fname)
{
  return this->Save(fname,0);
}

int vtkpxTensorBSplineTransform::Save(const char* fname,int compression)
{
  if (compression==0)
    {
      FILE* fout=fopen(fname,"w");
      if (!fout)
	return 0;
      
      int ok=Save(fout);
      fclose(fout);
      return ok;
    } 

  gzFile fout=gzsuffixopen(fname,"wb",compression);
  if (!fout)
    return 0;
  int ok=Save(fout);
  gzclose(fout);
  return ok;
  
}

int vtkpxTensorBSplineTransform::Save(FILE* fout)
{
  if (this->Displacements==NULL)
    return 0;
  int np=this->Displacements->GetNumberOfTuples();


  fprintf(fout,"#vtkpxBaseGridTransform2 File\n");
  fprintf(fout,"#Origin\n %.4f %.4f %.4f\n",this->Origin[0],this->Origin[1],this->Origin[2]);
  fprintf(fout,"#Spacing\n %.4f %.4f %.4f\n",this->Spacing[0],this->Spacing[1],this->Spacing[2]);
  fprintf(fout,"#Dimensions\n %d %d %d\n",this->Dimensions[0],this->Dimensions[1],this->Dimensions[2]);
  fprintf(fout,"#Interpolation Mode\n%d\n",4);
  fprintf(fout,"#Displacements\n");

  for (int i = 0; i < np;i++)
    fprintf(fout,"%d %.4f %.4f %.4f\n",i,
	     this->Displacements->GetComponent(i,0),
	     this->Displacements->GetComponent(i,1),
	     this->Displacements->GetComponent(i,2));

  return 1;
}
//----------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::Save(gzFile fout)
{
  if (this->Displacements==NULL)
    return 0;

  int np=this->Displacements->GetNumberOfTuples();
  gzprintf(fout,"#vtkpxBaseGridTransform2 File\n");
  gzprintf(fout,"#Origin\n %.4f %.4f %.4f\n",this->Origin[0],this->Origin[1],this->Origin[2]);
  gzprintf(fout,"#Spacing\n %.4f %.4f %.4f\n",this->Spacing[0],this->Spacing[1],this->Spacing[2]);
  gzprintf(fout,"#Dimensions\n %d %d %d\n",this->Dimensions[0],this->Dimensions[1],this->Dimensions[2]);
  gzprintf(fout,"#Interpolation Mode\n%d\n",3);
  gzprintf(fout,"#Displacements\n");
  
  for (int i = 0; i < np;i++)
    gzprintf(fout,"%d %.4f %.4f %.4f\n",i,
	     this->Displacements->GetComponent(i,0),
	     this->Displacements->GetComponent(i,1),
	     this->Displacements->GetComponent(i,2));
  
  return 1;
}
//----------------------------------------------------------------------------
// Index Manipulation
//----------------------------------------------------------------------------
int  vtkpxTensorBSplineTransform::GetControlPointIndex(int i,int j,int k)
{
  i=Irange(i,0,this->Dimensions[0]-1);
  j=Irange(j,0,this->Dimensions[1]-1);
  k=Irange(k,0,this->Dimensions[2]-1);

  return i+j*this->Dimensions[0]+k*this->OffsetDimensions[2];
}

void vtkpxTensorBSplineTransform::GetControlPointIndices(int index,int ind[3])
{
  index=Irange(index,0,this->NumberOfControlPoints-1);
  ind[2]=index/this->OffsetDimensions[2];
  int t=index%this->OffsetDimensions[2];
  ind[1]=t/this->Dimensions[0];
  ind[0]=t%this->Dimensions[0];
}
//----------------------------------------------------------------------------
// Get/Set Displacement of control point index 
//----------------------------------------------------------------------------
double vtkpxTensorBSplineTransform::GetControlPointDisplacement(int index,int comp)
{
  comp=Irange(comp,0,2);
  return this->Displacements->GetComponent(index,comp);
}
//-------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::GetControlPointDisplacement(int index,double val[3])
{
  for (int i=0;i<=2;i++)
    val[i]=this->Displacements->GetComponent(index,i);
}

void vtkpxTensorBSplineTransform::SetControlPointDisplacement(int index,int comp,double val)
{
  comp=Irange(comp,0,2);
  this->Displacements->SetComponent(index,comp,val);
}

void vtkpxTensorBSplineTransform::SetControlPointDisplacement(int index,double val[3])
{
  for (int i=0;i<=2;i++)
    this->Displacements->SetComponent(index,i,val[i]);
}

//----------------------------------------------------------------------------
// Move Control Point index
//----------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::ShiftControlPoint(int index,int comp,double val)
{
  comp=Irange(comp,0,2);
  double p=this->Displacements->GetComponent(index,comp);
  this->Displacements->SetComponent(index,comp,p+val);
}
//-------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::ShiftControlPoint(int index,double val[3])
{
  double* ptr=this->Displacements->GetTuple(index);
  for (int i=0;i<=2;i++)
    this->Displacements->SetComponent(index,i,ptr[i]+val[i]);
}
//-------------------------------------------------------------------------
//  Active Control Point Indexing
//-------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::IsControlPointActive(int cp)
{
  return (this->ControlPointStatus->GetComponent(cp,0)>0);
}
//-------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::GetNumberOfActiveControlPoints()
{
  return this->ActiveControlPointList->GetNumberOfTuples();
}

int vtkpxTensorBSplineTransform::GetActualControlPointIndexFromActiveIndex(int i)
{
  return (int)this->ActiveControlPointList->GetComponent(i,0);
}

//-------------------------------------------------------------------------
// Shift Active Control Point
//-------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::ShiftActiveControlPoint(int index,int comp,double val)
{
  int true_index=(int)this->ActiveControlPointList->GetComponent(index,0);
  this->ShiftControlPoint(true_index,comp,val);
}

void vtkpxTensorBSplineTransform::ShiftActiveControlPoint(int index,double val[3])
{
  int true_index=(int)this->ActiveControlPointList->GetComponent(index,0);
  this->ShiftControlPoint(true_index,val);
}

//-------------------------------------------------------------------------
// Get the Location of control point index
//-------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::GetControlPointLocation(int index,double p[3])
{
  int ind[3];  this->GetControlPointIndices(index,ind);
  for (int ia=0;ia<=2;ia++)
    p[ia]=this->Origin[ia]+this->Spacing[ia]*double(ind[ia]);
}
//----------------------------------------------------------------------------
double vtkpxTensorBSplineTransform::GetControlPointLocation(int index,int comp)
{
  double p[3];
  this->GetControlPointLocation(index,p);
  comp=Irange(comp,0,2);
  return p[comp];
}

//--------------------------------------------------------------------------------------------
// Get the Location of Active control point index by mapping through active control point list
//-------------------------------------------------------------------------------------------
void   vtkpxTensorBSplineTransform::GetActiveControlPointLocation(int index, double p[3])
{
  int true_index=(int)this->ActiveControlPointList->GetComponent(index,0);
  this->GetControlPointLocation(true_index,p);
}

double vtkpxTensorBSplineTransform::GetActiveControlPointLocation(int index, int comp)
{
  int true_index=(int)this->ActiveControlPointList->GetComponent(index,0);
  return this->GetControlPointLocation(true_index,comp);
}
//----------------------------------------------------------------------------
// Get Internal Coordinates both lattice and arclengths
//----------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::GetCoordinatesInGrid(double p[3],int lat[3],double s[3])
{
  for (int ia=0;ia<=2;ia++)
    {
      double x=(p[ia]-this->Origin[ia])/this->Spacing[ia]; 
      lat[ia]=int(floor(x+0.0001));
      s[ia]=x-lat[ia];
    }
}
//----------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::InitializeStatusAndCaches()
{
  if (this->Displacements==NULL)
    {
      vtkErrorMacro(<<"Bad Displacements Cannot Initialize Status and Cache Arrays");
      return 0;
    }

  if (this->WholeImageCache!=NULL)
    this->WholeImageCache->Delete();
  this->WholeImageCache=NULL;

  if (this->DetailImageCache!=NULL)
    this->DetailImageCache->Delete();
  this->DetailImageCache=NULL;

  if (this->CachedPointsInfo!=NULL)
    this->CachedPointsInfo->Delete();
  this->CachedPointsInfo=NULL;

  if (this->CachedPoints!=NULL)
    this->CachedPoints->Delete();
  this->CachedPoints=NULL;

  if (this->ActiveControlPointList==NULL)
    this->ActiveControlPointList=vtkIntArray::New();

  if (this->ControlPointStatus==NULL)
    this->ControlPointStatus=vtkCharArray::New();

  int nt=this->Displacements->GetNumberOfTuples();

  this->ActiveControlPointList->SetNumberOfComponents(1);
  this->ActiveControlPointList->SetNumberOfTuples(nt);

  this->ControlPointStatus->SetNumberOfComponents(4);
  this->ControlPointStatus->SetNumberOfTuples(nt);

  this->ControlPointStatus->FillComponent(0,1);
  for (int i=0;i<nt;i++)
    this->ActiveControlPointList->SetComponent(i,0,i);

  int index=0;
  for (int k=0;k<Dimensions[2];k++)
    {
      int zstatus=0;
      if (k==0)
	zstatus=-2;
      else if (k==1)
	zstatus=-1;
      else if (k==Dimensions[2]-2)
	zstatus=1;
      else if (k==Dimensions[2]-1)
	zstatus=2;
      
      for (int j=0;j<Dimensions[1];j++)
	{
	  int ystatus=0;
	  if (j==0)
	    ystatus=-2;
	  else if (j==1)
	    ystatus=-1;
	  else if (j==Dimensions[1]-2)
	    ystatus=1;
	  else if (j==Dimensions[1]-1)
	    ystatus=2;
	  
	  for (int i=0;i<Dimensions[2];i++)
	    {
	      
	      int xstatus=0;
	      if (i==0)
		xstatus=-2;
	      else if (i==1)
		xstatus=-1;
	      else if (i==Dimensions[0]-2)
		xstatus=1;
	      else if (i==Dimensions[0]-1)
		xstatus=2;
	      
	      this->ControlPointStatus->SetComponent(index,1,xstatus);
	      this->ControlPointStatus->SetComponent(index,2,ystatus);
	      this->ControlPointStatus->SetComponent(index,3,zstatus);
	      ++index;
	    }
	}
    }
  return 1;
}
//----------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::Initialize(vtkImageData* img,double spacing,double offset)
{
  return vtkpxTensorBSplineTransform::Initialize(img,spacing,spacing,spacing,offset);
}
//----------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::Initialize(vtkImageData* img,double spacingx,double spacingy,double spacingz,double offset,
					    int adjustspacing)
{
  if (img==NULL)
    return 0;
 

  double bounds[6];   img->GetBounds(bounds); 
  double sp[3];

  sp[0]=spacingx;
  sp[1]=spacingy;
  sp[2]=spacingz;
  double imgsp[3]; img->GetSpacing(imgsp);
  for (int ia=0;ia<=2;ia++)
    {
      if (sp[ia]<imgsp[ia])
	sp[ia]=imgsp[ia];
    }

  if (offset<0.0)
    offset=0.0;
  else if (offset>0.5)
    offset=0.5;



  for (int ia=0;ia<=2;ia++)
    {
      int up=ia*2+1;
      int lp=ia*2;
      double len=bounds[up]-bounds[lp];
      bounds[lp]-=offset*len;
      bounds[up]+=offset*len;
      double newlen=bounds[up]-bounds[lp];
      
      this->Dimensions[ia] = int(newlen/sp[ia]+1.5);
      if (this->Dimensions[ia]<4) this->Dimensions[ia]=4;
      this->Spacing[ia] =  double(newlen) / double(this->Dimensions[ia]-1);
      if (adjustspacing==1)
	{
	  if (this->Spacing[ia]<imgsp[ia])
	    {
	      this->Spacing[ia]=imgsp[ia];
	    }
	  else
	    {
	      int n=int(imgsp[ia]/this->Spacing[ia]+0.5);
	      this->Spacing[ia]=double(n)*imgsp[ia];
	    }
	}

      this->Origin[ia]=bounds[lp];
    }

  if (this->Displacements==NULL)
    this->Displacements=vtkDoubleArray::New();

  this->Displacements->SetNumberOfComponents(3);
  this->Displacements->SetNumberOfTuples(this->Dimensions[0]*this->Dimensions[1]*this->Dimensions[2]);

  this->OffsetDimensions[0]=1;
  this->OffsetDimensions[1]=this->Dimensions[0];
  this->OffsetDimensions[2]=this->Dimensions[0]*this->Dimensions[1];
  this->NumberOfControlPoints=this->OffsetDimensions[2]*this->Dimensions[2];
  
  for (int i=0;i<=2;i++)
    this->Displacements->FillComponent(i,0.0);

  this->InitializeStatusAndCaches();

  return this->NumberOfControlPoints;
}
//-------------------------------------------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::CachedInitialize(vtkImageData* img,double spacingx,double spacingy,double spacingz,double offset)
{
  int ok=this->Initialize(img,spacingx,spacingy,spacingz,offset,1);

  if (ok==0)
    return 0;
  else {
  // Build Image Cache ....
    return 0;
  }

  


}
//-------------------------------------------------------------------------------------------------------------
//
//   Optimizer Related Stuff, get current parameters and set current parameters using active control point list
//
//-------------------------------------------------------------------------------------------------------------
void  vtkpxTensorBSplineTransform::SetCurrentParameters(vtkDoubleArray* param)
{
  int numt=this->ActiveControlPointList->GetNumberOfTuples();
  int numit=this->NumberOfActiveComponents*numt;
  int num=param->GetNumberOfTuples();

  if (num!=numit)
    {
      vtkErrorMacro(<<"Bad Data for Optimization");
      return;
    }

    int index=0;
    for (int i=0;i<numt;i++)
      {
	int cp=(int)this->ActiveControlPointList->GetComponent(i,0);
	double p[3] = { 0.0,0.0,0.0};
	for (int j=0;j<this->NumberOfActiveComponents;j++)
	  {
	    p[j]=param->GetComponent(index,0);
	    ++index;
	  }
	this->Displacements->SetTuple(cp,p);
      }
}

// -----------------------------------------------------------------------------------------
void  vtkpxTensorBSplineTransform::GetCurrentParameters(vtkDoubleArray* param)
{
  int numt=this->ActiveControlPointList->GetNumberOfTuples();
  int numit=this->NumberOfActiveComponents*numt;

  if (param->GetNumberOfComponents()!=1 || param->GetNumberOfTuples()!=numit)
    {
      param->SetNumberOfComponents(1);
      param->SetNumberOfTuples(numit);
    }

  int index=0;
  for (int i=0;i<numt;i++)
    {
      int cp=(int)this->ActiveControlPointList->GetComponent(i,0);
      double* p=this->Displacements->GetTuple(cp);
      this->GetControlPointDisplacement(i,p);
      for (int j=0;j<=2;j++)
	{
	  param->SetComponent(index,0,p[j]);
	  ++index;
	}
    }
}
// -----------------------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::CreateActiveControlPointListFromControlPointStatusArray()
{
  if (this->ControlPointStatus==NULL || this->ActiveControlPointList==NULL)
    {
      vtkErrorMacro(<<"Copy Create Active Control Point List  NULL ControlPointStatus");
      return 0;
    }

  
  int nt=0;
  int numcp=this->ControlPointStatus->GetNumberOfTuples();
  for (int i=0;i<numcp;i++)
    {
      if (ControlPointStatus->GetComponent(i,0)>0)
	++nt;
    }

  this->ActiveControlPointList->SetNumberOfComponents(1);
  this->ActiveControlPointList->SetNumberOfTuples(nt);

  int index=0;
  for (int i=0;i<numcp;i++)
    {
      if (ControlPointStatus->GetComponent(i,0)>0)
	{
	  this->ActiveControlPointList->SetComponent(index,0,i);
	  ++index;
	}
    }

  return nt;
}
// -----------------------------------------------------------------------------------------
//
//        Points Cache Stuff
//
// -----------------------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::CreatePointCache(vtkPolyData* pts)
{
  if (pts==NULL)
    {
      vtkErrorMacro(<<"Copy Create Active Control Point List  NULL ControlPointStatus");
      return 0;
    }

  if (this->CachedPointsInfo!=NULL)
    this->CachedPointsInfo->Delete();
  if (this->CachedPoints!=NULL)
    this->CachedPoints->Delete();

  this->CachedPoints=vtkPolyData::New();
  this->CachedPoints->DeepCopy(pts);

  int nump=this->CachedPoints->GetNumberOfPoints();

  this->CachedPointsInfo=vtkFloatArray::New();

  // numcomp = 64 controlpoints + 64 weights =128
  this->CachedPointsInfo->SetNumberOfComponents(128);
  this->CachedPointsInfo->SetNumberOfTuples(nump);


  double s[3];
  int  lat[3];


  for (int point=0;point<nump;point++)
    {
      double pp[3]; this->CachedPoints->GetPoint(point,pp);
      this->GetCoordinatesInGrid(pp,lat,s);
      int index=0;
      for (int k = 0; k < 4; k++)
	{
	  int K = Irange(k + lat[2] - 1,0,Dimensions[2]-1);
	  for (int j = 0; j < 4; j++)
	    {
	      int J = Irange(j + lat[1] - 1,0,Dimensions[1]-1);
	      for (int i = 0; i < 4; i++)
		{
		  int I = Irange(i + lat[0] - 1,0,Dimensions[0]-1);
		  int   cpoint=I+J*Dimensions[0]+K*OffsetDimensions[2];
		  double wgt= vtkpxTensorBSplineTransform::B(i, s[0]) * vtkpxTensorBSplineTransform::B(j, s[1]) * vtkpxTensorBSplineTransform::B(k, s[2]);
		  this->CachedPointsInfo->SetComponent(point,index*2,cpoint);
		  this->CachedPointsInfo->SetComponent(point,index*2+1,wgt);
		  ++index;
		}
	    }
	}
    }
	return 1;
}

int vtkpxTensorBSplineTransform::CachedPointTransform(int index,double output[3])
{
  if (this->CachedPoints==NULL)
    {
      vtkErrorMacro(<<"Cannot Transform Cached Point no Points Cache");
      return 0;
    }

  //  int ind=Irange(index,0,this->CachedPointsInfo->GetNumberOfTuples()-1);

  this->CachedPoints->GetPoint(index,output);

  for (int ia=0;ia<64;ia++)
    {
      int offset=ia*2;
      int cpoint=(int)this->CachedPointsInfo->GetComponent(index,offset);
      double wgt=this->CachedPointsInfo->GetComponent(index,offset+1);
      double d[3]; this->GetControlPointDisplacement(cpoint,d);
      
      for (int ia=0;ia<=2;ia++)
	output[ia]+=wgt*d[ia];
    }
  return 1;
}
// -----------------------------------------------------------------------------------------
//
//        Bending Energy Stuff
//
// -----------------------------------------------------------------------------------------
double vtkpxTensorBSplineTransform::ComputeTotalBendingEnergy()
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
double vtkpxTensorBSplineTransform::ComputeBendingEnergyAroundControlPoint(int cpoint)
{
  double sum=this->ComputeBendingEnergyAtControlPoint(cpoint);

  for (int direction=0;direction<=2;direction++)
    {
      int status=(int)this->ControlPointStatus->GetComponent(cpoint,direction+1);
      int cmin=-1,cmax=1;
      if (status<0)
	cmin=0;
      if (status>0)
	cmax=0;

      for (int c=cmin;c<=cmax;c++)
	if (c!=0)
	  sum+=this->ComputeBendingEnergyAtControlPoint(cpoint+c*this->OffsetDimensions[direction]);
    }
  return sum;
}
// ---------------------------------------------------------------------------------------
double vtkpxTensorBSplineTransform::ComputeBendingEnergyAtControlPoint(int cpoint)
{
  double scale=0.01*(1.0/(double(this->NumberOfControlPoints)));
  

  int status[3];
  for (int ia=0;ia<=2;ia++)
    status[ia]=(int)this->ControlPointStatus->GetComponent(cpoint,ia+1);

  double* thisdisp=this->Displacements->GetTuple(cpoint);
  double sum=0.0;

  for (int row=0;row<=2;row++)
    {
      // First Ordinary Second derivatives i.e. d2u/dx2 ...  x=row
      int neigh[2]= { cpoint,cpoint };
      if (status[row]!=-2)
	neigh[0]-=this->OffsetDimensions[row];
      if (status[row]!=2)
	neigh[1]+=this->OffsetDimensions[row];

      double* pdisp=this->Displacements->GetTuple(neigh[0]);
      double* mdisp=this->Displacements->GetTuple(neigh[1]);
      
      for (int comp=0;comp<=2;comp++)
	sum+=pow(pdisp[comp]-2.0*thisdisp[comp]+mdisp[comp],2.0);

      // Now Mixed Derivatives i.e. d2u/dxdy x=row,y=col
      
      for (int col=row+1;col<=2;col++)
	{
	  int corner[2][2];
	  corner[0][0]=neigh[0];
	  corner[1][0]=neigh[0];
	  corner[0][1]=neigh[1];
	  corner[1][1]=neigh[1];
	  
	  if (status[col]!=-2)
	    {
	      for (int ib=0;ib<=1;ib++)
		corner[ib][0]-=this->OffsetDimensions[col];
	    }

	  if (status[col]!=2)
	    {
	      for (int ib=0;ib<=1;ib++)
		corner[ib][1]+=this->OffsetDimensions[col];
	    }
	    
	  double* pdisp1=this->Displacements->GetTuple(corner[0][0]);
	  double* pdisp2=this->Displacements->GetTuple(corner[1][1]);
	  double* mdisp1=this->Displacements->GetTuple(corner[0][1]);
	  double* mdisp2=this->Displacements->GetTuple(corner[1][0]);
	  
	  for (int comp=0;comp<=2;comp++)
	    sum+=2.0*pow(0.25*(pdisp1[comp]+pdisp2[comp]-mdisp1[comp]-mdisp2[comp]),2.0);
	}
    }
  return (sum)*scale;
}
// ---------------------------------------------------------------------------------------
//
//        Point Approximation Utilities
//
// ---------------------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::GenerateTestPoints(vtkPoints* points,double incr)
{
  if (points==NULL)
    return 0;

  int nc=this->GetNumberOfControlPoints();
  incr=Frange(incr,0.2,3.0);

  double max=nc/(incr*incr*incr)+10;
  points->Allocate((int)max,20);
  double p[3];


  for (double k=0;k<this->Dimensions[2];k+=incr)
    {
      p[2]=this->Origin[2]+this->Spacing[2]*(k);
      for (double j=0;j<this->Dimensions[1];j+=incr)
	{
	  p[1]=this->Origin[1]+this->Spacing[1]*(j);
	  for (double i=0;i<this->Dimensions[0];i+=incr)
	    {
	      p[0]=this->Origin[0]+this->Spacing[0]*(i);
	      points->InsertNextPoint(p);
	    }
	}
    }

  return (points->GetNumberOfPoints());
}
//-------------------------------------------------------------------------
double vtkpxTensorBSplineTransform::TestApproximation(vtkAbstractTransform* tr,vtkPoints* points)
{
  double error=0.0;
  int caching=0;

  if (points==NULL && this->CachedPoints!=NULL)
    {
      points=this->CachedPoints->GetPoints();
      caching=1;
    }

  if (points==NULL)
    return -1.0;

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
      if (!caching)
	this->InternalTransformPoint(x,t2);
      else
	this->CachedPointTransform(i,t2);
      error+=sqrt(vtkMath::Distance2BetweenPoints(t1,t2));
    }

  return error/double(nc);
}
//-------------------------------------------------------------------------
double vtkpxTensorBSplineTransform::TestApproximation(vtkPoints* points,vtkPoints* points2)
{
  double error=0.0;
  int caching=0;

  if (points==NULL && this->CachedPoints!=NULL)
    {
      points=this->CachedPoints->GetPoints();
      caching=1;
    }

  if (points==NULL)
    return -1.0;

  int nc=points->GetNumberOfPoints();
  if (nc<1)
    return -1.0;

  this->Update();

  double x[3],tx[3],x2[3];
  for (int i=0;i<nc;i++)
    {
      points->GetPoint(i,x);
      points2->GetPoint(i,tx);
      if (!caching)
	this->InternalTransformPoint(x,x2);
      else
	this->CachedPointTransform(i,x2);
      error+=sqrt(vtkMath::Distance2BetweenPoints(x2,tx));
    }
  return error/double(nc);
}
//-------------------------------------------------------------------------
double vtkpxTensorBSplineTransform::TestWeightedApproximation(vtkPoints* points,vtkPoints* points2,vtkpxMatrix* lsqweights,
							      double& sum2error)
{
  double error=0.0;
  sum2error=0.0;
  int caching=0;

  if (points==NULL && this->CachedPoints!=NULL)
    {
      points=this->CachedPoints->GetPoints();
      caching=1;
    }

  if (points==NULL)
    return -1.0;

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
      return -1.0;
    }

  this->Update();
  double x[3],x2[3],tx[3];
  for (int i=0;i<nc;i++)
    {
      points2->GetPoint(i,tx);
      if (!caching)
	{
	  points->GetPoint(i,x);
	  this->InternalTransformPoint(x,x2);
	}
      else
	{
	  this->CachedPointTransform(i,x2);
	}

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
double vtkpxTensorBSplineTransform::ComputeWeightedDistance(vtkpxTensorBSplineTransform* other,
							    int cpindex,vtkDoubleArray* weights0)
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
// Key Procedures for approximation of points via optimization
// ---------------------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::ShiftControlPoints(vtkDoubleArray* grad,double step)
{
  int nc=this->GetNumberOfControlPoints();
  for (int j=0;j<=2;j++)
    for (int i = 0; i < nc; i++)
      this->ShiftControlPoint(i,j,step*grad->GetComponent(i,j));
}
// ---------------------------------------------------------------------------------------
void vtkpxTensorBSplineTransform::ScaleGradients(vtkDoubleArray* grad,double scale)
{
  int nt=grad->GetNumberOfTuples();
  int nc=grad->GetNumberOfComponents();
  for (int t=0;t<nt;t++)
    for (int c=0;c<nc;c++)
      grad->SetComponent(t,c,grad->GetComponent(t,c) * scale);

}
// ---------------------------------------------------------------------------------------
int vtkpxTensorBSplineTransform::FormCollectionStructure(vtkPoints* SourcePoints,vtkCollection* ControlsCollection,double PointWeightThreshold)
{
  if (ControlsCollection==NULL)
    return 0;
  ControlsCollection->RemoveAllItems();

  double thr=Frange(PointWeightThreshold,0.0,0.2);

  int np=SourcePoints->GetNumberOfPoints();
  int nc=this->GetNumberOfControlPoints();

  vtkDoubleArray** arrlist=new vtkDoubleArray*[nc];

  for (int cp=0;cp<nc;cp++)
    {
      vtkDoubleArray* fl=vtkDoubleArray::New();
      fl->SetNumberOfComponents(2);
      fl->Allocate(10,10);
      ControlsCollection->AddItem(fl);
      arrlist[cp]=fl;
    }

  int GridSliceDimensions=this->Dimensions[0]*this->Dimensions[1];

  for (int node=0;node<np;node++)
    {
      double p1[3],s[3];
      int  lat[3];
      SourcePoints->GetPoint(node,p1);
      this->GetCoordinatesInGrid(p1,lat,s);

      for (int k = 0; k < 4; k++)
	{
	  int K = Irange(k + lat[2] - 1,0,this->Dimensions[2]-1);
	  for (int j = 0; j < 4; j++)
	    {
	      int J = Irange(j + lat[1] - 1,0,this->Dimensions[1]-1);
	      for (int i = 0; i < 4; i++)
		{
		  int I = Irange(i + lat[0] - 1,0,this->Dimensions[0]-1);
		  int   cpoint=I+J*this->Dimensions[0]+K*GridSliceDimensions;
		  double wgt= vtkpxTensorBSplineTransform::B(i, s[0]) * 
		    vtkpxTensorBSplineTransform::B(j, s[1]) * 
		    vtkpxTensorBSplineTransform::B(k, s[2]);
		  if (wgt>=thr)
		    {
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
  return m;

}



