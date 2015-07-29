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
  Module:    $RCSfile: vtkpxSurfaceUtil.h,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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
#include "pxutil.h"
#include "pxfpbspline.h"
#include "pxcontour.h"
#include "pxchamferdistancemap.h"
#include "vtkCellArray.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkpxBaseCurve.h"
#include "vtkpxSplineSource.h"
#include "vtkTransform.h"
#include "vtkImageData.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageGradientMagnitude.h"
#include "vtkpxImageExtract.h"
#include "vtkProbeFilter.h"
#include "vtkDoubleArray.h"
//------------------------------------------------------------------------------
vtkpxSplineSource* vtkpxSplineSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxSplineSource");
  if(ret)
    {
    return (vtkpxSplineSource*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxSplineSource;
}

//------------------------------------------------------------------------------
vtkpxSplineSource::vtkpxSplineSource()
{
  this->Spline=new PXFPBSpline();
  this->Spline->setellipse(6,1,1,0,0,3);
  this->Resolution=1.0;

  for (int ia=0;ia<=2;ia++)
    {
      this->ImageOrigin[ia]=0.0;
      this->ImageSpacing[ia]=0.0;
    }

  // SNAKE Stuff
  this->SNAKE_img=3.0;
  this->SNAKE_smooth=0.5;
  this->SNAKE_step=0.5;
  this->SNAKE_baloon=0.0; 
  this->SNAKE_niterations=20;
  this->SNAKE_imagemax=1.0;

  this->EdgeImage=NULL;
  this->Optimizer=NULL;
  this->Position=NULL;
}
//------------------------------------------------------------------------------
vtkpxSplineSource::~vtkpxSplineSource()
{
  if (this->Spline)
      delete this->Spline;

  if (this->EdgeImage!=NULL)
    this->EdgeImage->Delete();

  if (this->Optimizer!=NULL)
    this->Optimizer->Delete();

  if (this->Position!=NULL)
    this->Position->Delete();

}
//------------------------------------------------------------------------------
int vtkpxSplineSource::Save(const char* fname)
{
  if (this->Spline==NULL)
      return 0;
  return this->Spline->Save(fname);
}
//------------------------------------------------------------------------------
int vtkpxSplineSource::Load(const char* fname)
{
  if (this->Spline==NULL)
      this->Spline=new PXFPBSpline();
  this->Modified();
  return this->Spline->Load(fname);

}
//------------------------------------------------------------------------------
void vtkpxSplineSource::SetEllipse(int np,
				   float radiusx,float radiusy,
				   float x,float y,float z,int dim)
{
  if (this->Spline==NULL)
      this->Spline=new PXFPBSpline(3,2,z,1);

  this->Spline->setellipse(np,radiusx,radiusy,x,y,dim);
  this->Spline->setzlevel(z);
  this->Modified();
}
//------------------------------------------------------------------------------
void vtkpxSplineSource::Copy(vtkpxSplineSource* other)
{
  this->Spline->copypoints(other->GetSpline());
  this->SetResolution(other->GetResolution());
  this->SetImageSpacing(other->GetImageSpacing());
  this->SetImageOrigin(other->GetImageOrigin());
  this->Modified();
}
//------------------------------------------------------------------------------
int   vtkpxSplineSource::GetNumPoints()
{
  return this->Spline->getnumpoints();
}
//------------------------------------------------------------------------------
float vtkpxSplineSource::GetZlevel()
{
  return this->Spline->getzlevel();
}
//------------------------------------------------------------------------------
void vtkpxSplineSource::SetZlevel(float lev)
{
  this->Spline->setzlevel(lev);
  this->Modified();
}
//------------------------------------------------------------------------------
PXFPBSpline* vtkpxSplineSource::GetSpline()
{
  return this->Spline;
}
//------------------------------------------------------------------------------
// Contour, vtkPoints Interfaces
PXContour* vtkpxSplineSource::ExportToContour()
{
  return this->Spline->exporttocontour(this->Resolution);
}
//------------------------------------------------------------------------------
int vtkpxSplineSource::FitToContour(PXContour* cntr,float smooth,float csmooth,
			   int numpoints,int closed,int iterations)
{
  int ok=this->Spline->fittocontour(cntr,smooth,csmooth,numpoints,
				    closed,iterations);
  this->Modified();
  return ok;
}
//------------------------------------------------------------------------------
int vtkpxSplineSource::ExportToPoints(vtkPoints* points)
{
  if (points==NULL)
      return -1;

  float length=this->Spline->getlength(0.0,1.0,0.05);
  int np=int(length/(0.5*this->Resolution)+0.5);
  float res=1.0/float(np);
  points->SetNumberOfPoints(np);

  for (int i=0;i<np;i++)
    {
      real  x,y,z;
      this->Spline->getposition(float(i*res),x,y,z);
      points->SetPoint(i,float(x),float(y),float(z));
    }
  return np;
}
//------------------------------------------------------------------------------
int vtkpxSplineSource::ExportKnotPointsToPoints(vtkPoints* points)
{
  if (points==NULL)
      return -1;

  int np=this->Spline->getnumpoints();
  points->SetNumberOfPoints(np);

  real xp,yp,zp;
  for (int ib=0;ib<this->Spline->getnumpoints();ib++)
    {
      this->Spline->getposition(this->Spline->getcknot(ib),xp,yp,zp);
      points->SetPoint(ib,xp,yp,zp);
    }
  return np;
}
//------------------------------------------------------------------------------
int vtkpxSplineSource::ExportToBaseCurve(vtkpxBaseCurve* baseCurve)
{
  if (baseCurve==NULL)
    return 0;
  baseCurve->DeleteAllPoints();

  int np=this->Spline->getnumpoints();
  (baseCurve->GetPoints())->SetNumberOfPoints(np+10);
  real xp,yp,zp;
  for (int ib=0;ib<this->Spline->getnumpoints();ib++)
    {
      this->Spline->getposition(this->Spline->getcknot(ib),xp,yp,zp);
      baseCurve->AddPoint(xp,yp,zp,0);
    }
  return np;
  
}
//------------------------------------------------------------------------------
int vtkpxSplineSource::SetFromBaseCurve(vtkpxBaseCurve* baseCurve)
{
  if (baseCurve==NULL)
    return 0;
  if (baseCurve->GetNumPoints()<4)
    return 0;
  return this->SetControlPoints(baseCurve->GetPoints(),baseCurve->GetNumPoints());
}
//------------------------------------------------------------------------------
int vtkpxSplineSource::InsertKnot(float x,float y)
{
  float s=this->Spline->getlength(0.0,1.0,0.05);
  float n=float(this->GetNumPoints());
  float d=s/(3.0*n);

  return this->InsertKnot(x,y,d);
}

int vtkpxSplineSource::InsertKnot(float x,float y,float mindist)
{
  real xn=x,yn=y;
  //  real md=mindist;


  this->Spline->insertknot(xn,yn,mindist);
  return 1;
}


int vtkpxSplineSource::SetControlPoints(vtkPoints* points,int np)
{
  if (points==NULL)
      return 0;

  if (np==-1)
      np=points->GetNumberOfPoints();
  
  vtkDebugMacro(<<"Setting Control Points np="<<np<<", dimensions="<<this->Spline->getdimension());

  real* xp=new real[np];
  real* yp=new real[np];
  real* zp=NULL;

  if (this->Spline->getdimension()==3)
      zp=new real[np];

  for (int i=0;i<np;i++)
      {
	double x[3];
	points->GetPoint(i,x);
	xp[i]=x[0];
	yp[i]=x[1];
	if (this->Spline->getdimension()==3)
	    zp[i]=x[2];
      }

  if (this->Spline->getdimension()==3)
      {
	this->Spline->ResetPoints(np,xp,yp,zp);
      }
  else
      {
	this->Spline->ResetPoints(np,xp,yp);
      }

  if (zp!=NULL)
    delete [] zp;

  delete [] xp;
  delete [] yp;

  this->Modified();
  return 1;
}
//------------------------------------------------------------------------------
  // Modify 
int vtkpxSplineSource::Resample(float ds_step,int npoints)
{
  int ok=this->Spline->resample(ds_step,npoints);
  this->Modified();
  return ok;
}
//------------------------------------------------------------------------------
int vtkpxSplineSource::ChangeSpacing(float smooth,int npoints,
				     float step,float csmooth,int iterations)
{
  int ok=this->Spline->changespacing(smooth,npoints,step,csmooth,iterations);
  this->Modified();
  return ok;

}
//------------------------------------------------------------------------------
int vtkpxSplineSource::FitPoints(vtkPoints* points,float smooth,int numpoints,
				 int closed,int iterations)
{
  if (points==NULL)
      return 0;

  PXContour* cntr=new PXContour();
  for (int i=0;i<points->GetNumberOfPoints();i++)
      {
	double x[3];
	points->GetPoint(i,x);
	cntr->Add(x[0],x[1]);
      }
  int ok=this->FitToContour(cntr,smooth,0.5,numpoints,closed,iterations);

  delete cntr;
  return ok;
}
//------------------------------------------------------------------------------

int vtkpxSplineSource::Transform(vtkTransform* transform)
{
  if (transform==NULL)
      return 0;

  float p[3],q[3];
  for(int i=0;i<this->Spline->getnumpoints();i++)
    {
      p[0]=this->Spline->getcontrolx(i);
      p[1]=this->Spline->getcontroly(i);
      p[2]=this->Spline->getcontrolz(i);
      
      transform->TransformPoint(p,q);
      if (this->Spline->getdimension()==2)
	q[2]=p[2];
      /*fprintf(stderr,"Point %d (%.1f,%.1f,%.1f) --> (%.1f,%.1f,%.1f)\n",
	i,p[0],p[1],p[2],q[0],q[1],q[2]);*/
      this->Spline->setcontrol(i,q[0],q[1],q[2]);
    }
  this->Modified();
  //fprintf(stderr,"\n");
  return 1;
}
//------------------------------------------------------------------------------

void vtkpxSplineSource::Execute()
{

  if (this->Spline==NULL)
      {
	vtkErrorMacro(<<"No spline available");
	return;
      }

  float length=this->Spline->getlength(0.0,1.0,0.05);
  int np=int(length/(0.5*this->Resolution)+0.5);
  float res=1.0/float(np);

  int   closed=0;

  if (this->Spline->isclosed()>0)
      closed=1;

  vtkPoints    *c_points    = vtkPoints::New();
  vtkCellArray *c_lines     = vtkCellArray::New();
  
  c_points->SetNumberOfPoints(np);
  c_lines->Allocate(np-1+closed,5);


  for (int i=0;i<np;i++)
      {
	real  x,y,z;
	this->Spline->getposition(float(i*res),x,y,z);
	c_points->SetPoint(i,float(x),float(y),float(z));
	vtkIdType pt[2];

	if (i<np-1 && np>1)
	    {
	      pt[0]=i;
	      pt[1]=i+1;
	      c_lines->InsertNextCell(2,pt);
	    }
	if (i==np-1 && closed==1)
	    {
	      pt[0]=i;
	      pt[1]=0;
	      c_lines->InsertNextCell(2,pt);
	    }
      }

  vtkPolyData* curve = this->GetOutput();
  curve->SetPoints(c_points);
  c_points->Delete();
  curve->SetLines(c_lines);
  c_lines->Delete();
}

//------------------------------------------------------------------------------
float vtkpxSplineSource::GetArea()
{
  PXContour* cntr=this->ExportToContour();
  float area=cntr->getarea();
  delete cntr;
  return area;
}

//------------------------------------------------------------------------------
float vtkpxSplineSource::GetVOIArea(vtkImageData* image,int level,int frame,float threshold)
{
  float area=1.0,mean=0.0,std=0.0;
  this->GetVOIProperties(image,level,frame,area,mean,std,threshold);
  return area;
}
//------------------------------------------------------------------------------
int  vtkpxSplineSource::GetVOIProperties(vtkImageData* image,int level,int frame,
					  float& area,float& mean,float& std,float threshold)
{
  
  PXContour* cntr=this->ExportToContour();
  
  int dim[3];
  image->GetDimensions(dim);
  level=Irange(level,0,dim[2]-1);

  double sp[3],ori[3];
  image->GetSpacing(sp);
  image->GetOrigin(ori);

  int numpixels=0;
  float sum=0.0,sum2=0.0;

  for (int i=0;i<dim[0];i++)
    for (int j=0;j<dim[1];j++)
      {
	float v=image->GetScalarComponentAsDouble(i,j,level,frame);
	float x[2];
	x[0]=float(i)*sp[0]+ori[0];
	x[1]=float(j)*sp[1]+ori[1];
	if ( (v>=threshold) && ( cntr->IsInside(x[0],x[1]) > 0 ))
	  {
	    ++numpixels;
	    sum+=v;
	    sum2+=v*v;
	  }
      }
	
  
  mean=sum/float(numpixels);
  std= sqrt( sum2/float(numpixels)-mean*mean);
  area=float(numpixels)*sp[0]*sp[1];

  /*fprintf(stderr,"Sum=%.1f sum2=%.1f numpixels=%d np2=%d (sp=%.1f,%.1f,%.1f) mean=%.1f, std=%.1f, area=%.1f\n",
    sum,sum2,numpixels,np2,sp[0],sp[1],sp[2],mean,std,area);*/

  delete cntr;
  return numpixels;

}
//------------------------------------------------------------------------------
int vtkpxSplineSource::GetDualVOIProperties(vtkImageData* image,int level,int frame,
					    float area[2],float mean[2],float std[2],int numpixels[2],
					    float threshold[2])
{
  PXContour* cntr=this->ExportToContour();
  
  int dim[3];
  image->GetDimensions(dim);
  level=Irange(level,0,dim[2]-1);

  double sp[3],ori[3];
  image->GetSpacing(sp);
  image->GetOrigin(ori);

  numpixels[0]=0; numpixels[1]=0;
  int total=0;
  float sum[2],sum2[2];
  
  sum[0]=0.0; sum[1]=0.0; sum2[0]=0.0; sum2[1]=0.0;

  for (int i=0;i<dim[0];i++)
    for (int j=0;j<dim[1];j++)
      {
	float v=image->GetScalarComponentAsDouble(i,j,level,frame);
	float x[2];
	x[0]=float(i)*sp[0]+ori[0];
	x[1]=float(j)*sp[1]+ori[1];
	if (cntr->IsInside(x[0],x[1]) > 0)
	  {
	    ++total;
	    for (int r=0;r<=1;r++)
	      {
		if (v>threshold[r])
		  {
		    ++numpixels[r];
		    sum[r]+=v;
		    sum2[r]+=v*v;
		  }
	      }
	  }
      }
	
  for (int r=0;r<=1;r++)
    {
      mean[r]=sum[r]/float(numpixels[r]+0.00001);
      std[r]= sqrt( sum2[r]/float(numpixels[r]+0.00001)-mean[r]*mean[r]);
      area[r]=float(numpixels[r])*sp[0]*sp[1];
    }

  delete cntr;
  return total;

}
//------------------------------------------------------------------------------
int vtkpxSplineSource::ThresholdAddToObjectMap(vtkImageData* objectmap,vtkImageData* img,
					       int level,int imagelevel,
					       float value,int thresholdmode,float min,float max)
{
  if (objectmap==NULL)
    return -1;
  
  double sp[3],ori[3];
  int dim[3];
  objectmap->GetDimensions(dim);
  objectmap->GetSpacing(sp);
  objectmap->GetOrigin(ori);
  
  int numvoxels=0;
  level=Irange(level,0,dim[2]-1);
  int offset=dim[0]*dim[1]*level;
  
  vtkDataArray* objectmapScalars=objectmap->GetPointData()->GetScalars();
  int numscalars=objectmapScalars->GetNumberOfTuples();

  PXContour* cntr=this->Spline->exporttocontour(0.05);
  
  float x1,x2,y1,y2;
  cntr->getextent(x1,x2,y1,y2);
  int imin=Irange( int((x1/sp[0])-ori[0]-2.0),0,dim[0]-2);
  int imax=Irange( int((x2/sp[0])-ori[0]+2.0),imin+1,dim[0]-1);

  int jmin=Irange( int((y1/sp[1])-ori[1]-2.0),0,dim[1]-2);
  int jmax=Irange( int((y2/sp[1])-ori[1]+2.0),jmin+1,dim[1]-1);

  for (int i=imin;i<=imax;i++)
    for (int j=jmin;j<=jmax;j++)
      {
	float x[2];
	x[0]=float(i)*sp[0]+ori[0];
	x[1]=float(j)*sp[1]+ori[1];
	
	if ( cntr->IsInside(x[0],x[1]) > 0 )
	  {
	    ++numvoxels;
	    int index=Irange(offset+j*dim[0]+i,0,numscalars-1);

	    int doadd=0;
	    if (thresholdmode==1)
	      {
		float v=img->GetScalarComponentAsDouble(i,j,imagelevel,0);
		if (v>=min && v<=max)
		  doadd=1;
	      }

	    if (thresholdmode==0 || doadd==1)
	      objectmapScalars->SetComponent(index,0,value);

	  }
      }
  delete cntr;
  return numvoxels;
}

//------------------------------------------------------------------------------
int vtkpxSplineSource::AddToObjectMap(vtkImageData* objectmap,int level,float value,int add)
{
  if (objectmap==NULL)
    return -1;
  
  double sp[3],ori[3];
  int dim[3];
  objectmap->GetDimensions(dim);
  objectmap->GetSpacing(sp);
  objectmap->GetOrigin(ori);
  
  int numvoxels=0;
  level=Irange(level,0,dim[2]-1);
  int offset=dim[0]*dim[1]*level;
  
  vtkDataArray* objectmapScalars=objectmap->GetPointData()->GetScalars();
  int numscalars=objectmapScalars->GetNumberOfTuples();

  PXContour* cntr=this->Spline->exporttocontour(0.05);
  
  float x1,x2,y1,y2;
  cntr->getextent(x1,x2,y1,y2);
  int imin=Irange( int((x1/sp[0])-ori[0]-2.0),0,dim[0]-2);
  int imax=Irange( int((x2/sp[0])-ori[0]+2.0),imin+1,dim[0]-1);

  int jmin=Irange( int((y1/sp[1])-ori[1]-2.0),0,dim[1]-2);
  int jmax=Irange( int((y2/sp[1])-ori[1]+2.0),jmin+1,dim[1]-1);

  /*  fprintf(stderr,"Ranges = %.1f:%.1f %.1f:%.1f --> vox (%d:%d %d:%d)\n",
      x1,y1,x2,y2,imin,jmin,imax,jmax);*/

  

  for (int i=imin;i<=imax;i++)
    for (int j=jmin;j<=jmax;j++)
      {
	float x[2];
	x[0]=float(i)*sp[0]+ori[0];
	x[1]=float(j)*sp[1]+ori[1];

	if ( cntr->IsInside(x[0],x[1]) > 0 )
	  {
	    ++numvoxels;
	    int index=Irange(offset+j*dim[0]+i,0,numscalars-1);
	    if (add)
	      objectmapScalars->SetComponent(index,0,value+objectmapScalars->GetComponent(index,0));
	    else
	      objectmapScalars->SetComponent(index,0,value);
	  }
      }
  delete cntr;
  return numvoxels;
}
//------------------------------------------------------------------------------
int vtkpxSplineSource::AddToDistanceMap(vtkImageData* distancemap,int level)
{
  if (distancemap==NULL)
    return -1;
  
  double sp[3],ori[3];
  int dim[3];
  distancemap->GetDimensions(dim);
  distancemap->GetSpacing(sp);
  distancemap->GetOrigin(ori);
  
  //  int numvoxels=0;
  level=Irange(level,0,dim[2]-1);
  int offset=dim[0]*dim[1]*level;

  PXChamferDistanceMap::pixel_size=short(sp[0]*100.0);
  PXChamferDistanceMap::obj_gray=10000;
  PXChamferDistanceMap::out_gray=-10000;
  
  // Export To Contour and Scale in Voxel Units
  PXContour* cntr=this->Spline->exporttocontour(0.05);
  PXContourPoint* pts=cntr->gettoppoint();
  int np=cntr->getnumpoints();
  for (int ib=1;ib<=np;ib++)
    {
      pts->x=(pts->x-ori[0])/sp[0];
      pts->y=(pts->y-ori[1])/sp[1];
      pts=pts->next;
    }
  
  PXChamferDistanceMap* map=new PXChamferDistanceMap(cntr,1);
  vtkDataArray* distances=distancemap->GetPointData()->GetScalars();

  for (int j=0;j<dim[1];j++)
    for (int i=0;i<dim[0];i++)
      {
	float v=(float(map->getvalue(i,j)));
	int index=offset+j*dim[0]+i;
	distances->SetComponent(index,0,v);
      }
  delete map;
  delete cntr;
  return 1;
}
//------------------------------------------------------------------------------
int   vtkpxSplineSource::HasSnake()
{
  if (this->EdgeImage==NULL)
    return 0;
  return 1;
}
//-------------------------------------------------------------------------
double vtkpxSplineSource::CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output)
{
  return ((vtkpxSplineSource*)self)->OptimizerGradient(position,output);
}

double vtkpxSplineSource::CalculateFunction(vtkObject* self,vtkDoubleArray* position)
{
  return ((vtkpxSplineSource*)self)->OptimizerValue(position);
}

void vtkpxSplineSource::OptimizerFeedback(vtkObject* self,const char* line,double val)
{
  fprintf(stderr,"Current Snake = %s \t %f\n",line,val);
}
//-------------------------------------------------------------------------
void vtkpxSplineSource::SetCurrentParameters(vtkDoubleArray* par)
{
  int np=Spline->getnumpoints();
  int nt=np*2;
  if (par->GetNumberOfTuples()!=nt)
    {
      vtkErrorMacro(<<"Bad Parameter Array\n");
      return;
    }

  for(int i=0;i<np;i++)
    this->Spline->setClosedControl(i,par->GetComponent(i*2,0),par->GetComponent(i*2+1,0));
  this->Modified();
}

void vtkpxSplineSource::GetCurrentParameters(vtkDoubleArray* par)
{
  int np=Spline->getnumpoints();
  int nt=np*2;
  if (par->GetNumberOfTuples()!=nt)
    par->SetNumberOfTuples(nt);

  for(int i=0;i<np;i++)
    {
      par->SetComponent(i*2,0,this->Spline->getcontrolx(i));
      par->SetComponent(i*2+1,0,this->Spline->getcontroly(i));
    }
}

double vtkpxSplineSource::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)
{
  if (grad->GetNumberOfTuples()!=params->GetNumberOfTuples())
    grad->SetNumberOfTuples(params->GetNumberOfTuples());

  int nt=params->GetNumberOfTuples();

  double GradientNorm=0.0;

  for (int dof=0;dof<nt;dof++)
    {
      double orig=params->GetComponent(dof,0);
      params->SetComponent(dof,0,orig+this->SNAKE_step);
      double a=this->OptimizerValue(params);
      params->SetComponent(dof,0,orig-this->SNAKE_step);
      double b=this->OptimizerValue(params);
      params->SetComponent(dof,0,orig);
      double g=-0.5*(b-a)/this->SNAKE_step;
      grad->SetComponent(dof,0,g);
      GradientNorm+=pow(g,2.0);
    }
  GradientNorm = (sqrt(GradientNorm));

  for (int j=0;j<nt;j++)
    grad->SetComponent(j,0,grad->GetComponent(j,0)/GradientNorm);

  return GradientNorm;
}
 
double vtkpxSplineSource::OptimizerValue(vtkDoubleArray* params)
{

  this->SetCurrentParameters(params);

  if (this->EdgeImage==NULL)
    return 0.0;

  float spa=this->EdgeImage->GetSpacing()[0];
  float length=this->Spline->getlength(0.0,1.0,0.05);
  int np=int(length/(0.5*spa+0.5));
  float res=1.0/float(np);
	     
  vtkPoints* points=vtkPoints::New();
  points->SetNumberOfPoints(np);
  
  for (int i=0;i<np;i++)
    {
      real  x,y,z;
      this->Spline->getposition(float(i*res),x,y,z);
      points->SetPoint(i,float(x),float(y),0.0);
    }

  double bend=0.0;

  for (int i=0;i<np;i++)
    {
      int ip=i-1; if (ip<0)    ip=np-1;
      int in=i+1; if (in>np-1) in=0;
      
      double x[3],x2[3],x0[3];
      points->GetPoint(i,x);
      points->GetPoint(ip,x0);
      points->GetPoint(in,x2);
      bend+=pow(x2[0]+x0[0]-2.0*x[0],2.0)+pow(x2[1]+x0[1]-2.0*x[1],2.0);
    }

  vtkPolyData* poly=vtkPolyData::New();
  poly->SetPoints(points);

  vtkProbeFilter* probe=vtkProbeFilter::New();
  probe->SetInput(poly);
  probe->SetSource(this->EdgeImage);
  probe->Update();
  
  vtkDataArray* data=probe->GetOutput()->GetPointData()->GetScalars();
      
  int   np2=data->GetNumberOfTuples();
  double sum=0.0;
  for (int i=0;i<np2;i++)
    {
      double v=data->GetComponent(i,0);
      sum+=v*v;
    }
  sum/=double(np2*this->SNAKE_imagemax*this->SNAKE_imagemax);
  bend/=double(np2);

  probe->Delete();
  poly->Delete();
  points->Delete();

  if (this->Debug)
    fprintf(stderr,"\n OptimizerValue:\t\t img=%.2f Smooth=%.2f\n",
	    -SNAKE_img*sum,SNAKE_smooth*bend);

  return -SNAKE_img*sum+SNAKE_smooth*bend;
}
//------------------------------------------------------------------------------

float vtkpxSplineSource::UpdateSnake(float img_weight,float smooth_weight)
{
  // If snake not created simply return 0.0
  // --------------------------------------
  if (this->Position==NULL)
    return 0.0;
  // --------------------------------------

  if (img_weight>=0.0)
    this->SNAKE_img=img_weight;
  
  if (smooth_weight>=0.0)
    this->SNAKE_smooth=smooth_weight;

  this->GetCurrentParameters(this->Position);

  this->DebugOn();
  this->OptimizerValue(this->Position);
  this->DebugOff();
  fprintf(stderr,"\n\n Updating Snake (%.2f)\n\n",this->SNAKE_imagemax);
  

  if (this->Optimizer!=NULL)
    return this->Optimizer->ComputeConjugateGradient(this->Position,SNAKE_niterations,SNAKE_step);

    
  this->DebugOn();
  this->OptimizerValue(this->Position);
  fprintf(stderr,"\n\n Updating Snake Done\n\n");
  this->DebugOff();

  return 0.0;
}
//------------------------------------------------------------------------------

void vtkpxSplineSource::CreateSnake(vtkImageData* image,int level,int frame,
				    float img_coeff,float smooth_coeff,
				    float timestep,float sigma,int niter)
{
  if (image==NULL)
      return;

  this->SNAKE_niterations=niter;
  this->SNAKE_step=timestep;
  this->SNAKE_smooth=smooth_coeff;
  this->SNAKE_img=img_coeff;

  
  vtkpxImageExtract* extr=vtkpxImageExtract::New();
  extr->SetInput(image);
  extr->SetSliceNo(level);
  extr->SetFrame(frame);
  extr->SetCurrentPlane(2);
  extr->Update();
  
  vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
  smooth->SetInput(extr->GetOutput());
  smooth->SetDimensionality(2);
  smooth->SetStandardDeviations(sigma,sigma,0.0);

  vtkImageGradientMagnitude* magn=vtkImageGradientMagnitude::New();
  magn->SetInput(smooth->GetOutput());
  magn->HandleBoundariesOn();
  magn->SetDimensionality(2);
  magn->Update();

  if (this->EdgeImage==NULL)
    this->EdgeImage=vtkImageData::New();

  this->EdgeImage->ShallowCopy(magn->GetOutput());
  this->SNAKE_imagemax=this->EdgeImage->GetPointData()->GetScalars()->GetRange()[1];

  magn->Delete();
  smooth->Delete();
  extr->Delete();

  this->EdgeImage->SetSpacing(1.0,1.0,1.0);
  this->EdgeImage->SetOrigin(0.0,0.0,0.0);
      
  int np=this->Spline->getnumpoints()*2;

  if (this->Optimizer!=NULL)
    {
      this->Optimizer->Delete();
      this->Position->Delete();
    }

  this->Optimizer=vtkpxOptimizer::New();
  this->Position=vtkDoubleArray::New();

  this->GetCurrentParameters(this->Position);
  this->Optimizer->SetMasterObject(this);
  this->Optimizer->SetEvaluateFunctionMethod(&vtkpxSplineSource::CalculateFunction);
  this->Optimizer->SetEvaluateGradientMethod(&vtkpxSplineSource::CalculateGradient);
  this->Optimizer->SetFeedbackMethod(&vtkpxSplineSource::OptimizerFeedback);
  this->Optimizer->ComputeConjugateGradient(this->Position,niter,timestep);

    
}
//------------------------------------------------------------------------------

