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
  Module:    $RCSfile: vtkpxUtil.h,v $
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
#include "vtkCellArray.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkpxContourSource.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkpxContourSource.h"
#include "pxfpbsplinestack.h"
#include "pxutil.h"
//------------------------------------------------------------------------------
vtkpxContourSource* vtkpxContourSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxContourSource");
  if(ret)
    {
      return (vtkpxContourSource*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxContourSource;
}
//------------------------------------------------------------------------------
vtkpxContourSource::vtkpxContourSource()
{
  this->Points=vtkPoints::New();
  this->Points->SetNumberOfPoints(0);
  this->Closed=1;
}
//------------------------------------------------------------------------------
vtkpxContourSource::~vtkpxContourSource()
{
  if (this->Points)
    this->Points->Delete();
}
//------------------------------------------------------------------------------
int vtkpxContourSource::Save(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (fout!=NULL)
      {
	int np=this->Points->GetNumberOfPoints();
	for (int ib=0;ib<np;ib++)
	  {
	    double x[3];
	    this->Points->GetPoint(ib,x);
	    fprintf(fout,"%6.2f  %6.2f \n",x[0],x[1]);

	  }
	fclose(fout);
	return(1);
      }
  return(0);

}
//------------------------------------------------------------------------------
int vtkpxContourSource::Load(const char* fname)
{
  FILE* fin;
  char line[200];

  int numpoints=0;
  fin=fopen(fname,"r");
  if (fin==NULL)
    return 0;

  while ( fgets(line,100,fin)!=NULL ) 
    numpoints++;
  fclose(fin);

  if (numpoints<1)
    return 0;
    
  fin=fopen(fname,"r");
  this->Points->SetNumberOfPoints(numpoints);

  for (int i=0;i<numpoints;i++)
    {
      float x,y,z=0.0;
      fscanf(fin,"%f %f", &x,&y);
      this->Points->SetPoint(i,x,y,z);
    }
  fclose(fin);
  this->Modified();
  return numpoints;
}
//------------------------------------------------------------------------------
void vtkpxContourSource::Copy(vtkpxContourSource* other)
{
  this->Points->DeepCopy(other->GetPoints());
  this->Closed=other->GetClosed();
  this->Modified();
}
//------------------------------------------------------------------------------
void vtkpxContourSource::SetFromSplineStackSource(vtkpxSplineStackSource* vtksur,int level,float dist)
{
  PXFPBSplineStack* sur=vtksur->GetSplineStack();
  PXFPBSpline* spline=sur->getspline(level);
  
  this->SetClosed(spline->isclosed());
  float z=spline->getzlevel();
  dist=Frange(dist,0.01,20.0);
  float length=spline->getlength(0.0,1.0,0.05);
  int np=int(length/dist+0.5);
  float ds=1.0/float(2*np);
  length=spline->getlength(0.0,1.0,ds);
  np=int(length/dist+0.5);
  float step=1.0/(float(np)*5.0);

  PXFPBSpline* temp=new PXFPBSpline(spline);
  temp->changespacing(0.5,-1,step);
  this->Points->SetNumberOfPoints(np);
  float x,y;
  for (int i=0;i<np;i++)
    {
      float s=float(i)/float(np);
      temp->getposition(s,x,y);
      this->Points->SetPoint(i,x,y,z);
    }
  delete temp;
  this->Modified();
}
//------------------------------------------------------------------------------
void vtkpxContourSource::SetFromContourStack(PXContourStack* stack,int level)
{
  PXContour* cntr=stack->getcontour(level);
  this->SetFromContour(cntr);
}

void vtkpxContourSource::SetFromContour(PXContour* cntr)
{
  this->Points->SetNumberOfPoints(cntr->getnumpoints());
  this->SetClosed(cntr->isclosed());

  if (cntr->getnumpoints()==0)
    return;
  
  PXContourPoint* temp=cntr->gettoppoint(); 

  for (int i=0; i<cntr->getnumpoints(); i++) 
    {
      this->Points->SetPoint(i,temp->x,temp->y,0.0);
      temp=temp->next;
    }
  this->Modified();
}

//------------------------------------------------------------------------------
int vtkpxContourSource::EnsureAnticlockwise()
{
  return this->SetDirection(1);
}

// ***********************************************************************
int vtkpxContourSource::EnsureClockwise()
{
  return this->SetDirection(-1);
}
// ***********************************************************************
int vtkpxContourSource::SetDirection(int dir)
{
    int numpoints=this->Points->GetNumberOfPoints();
  if (numpoints<3)
    return 0;

  int np=numpoints;
  double px0[3],px1[3],px2[3];

  this->Points->GetPoint(0,px0);
  this->Points->GetPoint(np/3,px1);
  this->Points->GetPoint((2*np)/3,px2);
	
  float v1x=px1[0]-px0[0];
  float v1y=px1[1]-px0[1];
  float v2x=px2[0]-px1[0];
  float v2y=px2[1]-px1[1];
  float zz=(v1x*v2y-v2x*v1y);

  int flag=0;
  if (dir>0 && zz<0.0)
    flag=1;
  if (dir<=0 && zz>0.0) 
    flag=1;

  if (flag)
    {
      vtkPoints* temp=vtkPoints::New();
      temp->DeepCopy(this->Points);
      for (int i=0;i<np;i++)
	this->Points->SetPoint(i,temp->GetPoint(np-1-i));
      temp->Delete();
      this->Modified();
    }
  return flag;
}
// ------------------------------------------------------------------------------------
void vtkpxContourSource::SetCircle(int np,float radius,float x,float y)
{
  this->Points->SetNumberOfPoints(np);
  for (int j=0;j<np;j++)
    {
      float t=3.141592*2.0*(float(j)/float(np));
      float xx=radius*cos(t)+x;
      float yy=radius*sin(t)+y;
      this->Points->SetPoint(j,xx,yy,0.0);
    }
  this->Closed=1;
  this->Modified();
}
// ------------------------------------------------------------------------------------
int  vtkpxContourSource::Intersect (float x1,float y1, 
				    float x2,float y2, 
				    float x,float y)
{
  float xint;			// co-ordinate of the intersection 
  
  int intersect=0;	       	// 1 if line l intersects the edge      
  int ToTheLeft=0;	       	// 1 if the intersection is on the left 
				//      of z = (x, y)                           

  if (y1 < y2) 
      {
	if ((y1 <= y)  && (y  <  y2)) 
	    {	
	      // l intersects edge 
	      intersect = 1;
	      xint = x1 + (y - y1) * (x2 - x1) /(y2 - y1);
	      ToTheLeft = (xint <= x);
	    }
	else 
	    intersect = 0;
      }
  else 
      if (y2 < y1) 
	  {
	    if ((y2 <= y) && (y  <  y1)) 
		{
		  // l intersects edge 
		  intersect = 1;
		  xint = x1 + (y - y1) * (x2 - x1) /(y2 - y1);
		  ToTheLeft = (xint <= x);
		}
	    else 
		intersect = 0;
	  }
      else				// horizontal edge   
	  intersect = 0;
  
  return ((intersect) && (ToTheLeft));
}
    
/*
 *--------------------------------------------------------------------------
 *_Subject : Test wether a point z = (x,y) is internal to a simple N-gon.
 *           
 *_Author: Francois G. Meyer 
 *_History  :                 Initial Key-In
 *
 *_Reference : Computational Geometry, Preparata and Shamos
 *--------------------------------------------------------------------------
 */
int vtkpxContourSource::IsInside (float testpointx,float testpointy)
{
  int numberofpoints=this->Points->GetNumberOfPoints();
  int  index=0,count=0;
  float x1, y1,x2, y2;

  x2 = this->Points->GetPoint(index)[0];
  y2 = this->Points->GetPoint(index)[1];
  
  while (index<numberofpoints) 
      {
	index++;
	x1 = x2;
	y1 = y2;
	x2 = this->Points->GetPoint(index)[0];
	y2 = this->Points->GetPoint(index)[1];
	if (this->Intersect (x1, y1, x2, y2, testpointx, testpointy))
	  count ++;
      }
  
  int isodd=count%2;
  return isodd;

}
// ------------------------------------------------------------------------------------
float vtkpxContourSource::GetArea()
{
  int numpoints=this->Points->GetNumberOfPoints();
  if (numpoints<3)
      return 0.0;

  if (this->Closed==0)
    return -1.0;
  
  float area=0.0;

 
  //	2 A( P ) = sum_{i=0}^{n-1} (x_i y_{i+1} - y_i x_{i+1}).
  
  for (int i=0;i<numpoints;i++)
    {
      float xi = this->Points->GetPoint(i)[0];
      float yi = this->Points->GetPoint(i)[1];
      float xip,yip;
      if (i!=numpoints-1)
	{
	  xip=this->Points->GetPoint(i+1)[0];
	  yip=this->Points->GetPoint(i+1)[1];
	}
      else
	{
	  xip=this->Points->GetPoint(0)[0];
	  yip=this->Points->GetPoint(0)[1];
	}
      area+= xi*yip-yi*xip;
    }
  return fabs(area*0.5);
}
// ------------------------------------------------------------------------------------
float vtkpxContourSource::GetContourLength()
{
  int numpoints=this->Points->GetNumberOfPoints();
  if (numpoints<1)
    return 0.0;

  float length=0.0;
  double x[3],y[3];

  for (int i=0;i<numpoints-1;i++)
    {
      this->Points->GetPoint(i,x);
      this->Points->GetPoint(i+1,y);
      length+=sqrt(vtkMath::Distance2BetweenPoints(x,y));
    }

  if (this->Closed)
    {
      this->Points->GetPoint(numpoints-1,x);
      this->Points->GetPoint(0,y);
      length+=sqrt(vtkMath::Distance2BetweenPoints(x,y));
    }
  return length;
}
// ------------------------------------------------------------------------------------
void vtkpxContourSource::GetCentroid(float& xm,float& ym)
{
  int numpoints=this->Points->GetNumberOfPoints();
  if (numpoints<1)
    {
      xm=0.0;
      ym=0.0;
      return;
    }

  float sum[3];
  for (int i=0;i<numpoints;i++)
    {
      double x[3];
      this->Points->GetPoint(i,x);
      for (int ia=0;ia<=2;ia++)
	sum[ia]+=x[ia];
    }

  for (int ia=0;ia<=2;ia++)
    sum[ia]/=float(numpoints);
  
  xm=sum[0]; 
  ym=sum[1];
}
// ------------------------------------------------------------------------------------
void vtkpxContourSource::Smooth(float relaxation)
{
  
  if (this->Points->GetNumberOfPoints()<5)
      return;

  relaxation=Frange(relaxation,0.0,0.99);
  vtkPoints* tmpPoints=vtkPoints::New();
  int np=this->Points->GetNumberOfPoints();

  for (int pass=0;pass<=1;pass++)
      {
	tmpPoints->DeepCopy(this->Points);
	
	for (int i=0;i<np;i++)
	  {
	    int p0,p1;
	    if (this->Closed)
	      {
		p0=Icyclic(i-1,np);
		p1=Icyclic(i+1,np);
	      }
	    else
	      {
		p0=Irange(i-1,0,np-1);
		p1=Irange(i+1,0,np-1);
	      }
	    
	    double x0[3],x1[3],x[3];
	    tmpPoints->GetPoint(p0,x0);
	    tmpPoints->GetPoint(i,x);
	    tmpPoints->GetPoint(p1,x1);
	      
	    for (int ia=0;ia<=2;ia++)
	      {
		if (pass==0)
		  x[ia]=(1.0-relaxation)*x[ia]+0.5*relaxation*(x0[ia]+x1[ia]);
		else
		  x[ia]=(1.0-relaxation)*x[ia]+0.5*relaxation*(x0[ia]+x1[ia]);
	      }
	    this->Points->SetPoint(i,x);
	  }
	relaxation=-1.0*(relaxation+0.01);
      }
  tmpPoints->Delete();
  this->Modified();


}
// ------------------------------------------------------------------------------------
void vtkpxContourSource::Sample(int rate)
{
  if (this->Points->GetNumberOfPoints()<4)
      return;

  vtkPoints* tmpPoints=vtkPoints::New();
  tmpPoints->DeepCopy(this->Points);
  
  this->Points->SetNumberOfPoints(0);
  for (int j=0;j<tmpPoints->GetNumberOfPoints();j+=rate)
    this->Points->InsertNextPoint(tmpPoints->GetPoint(j));

  tmpPoints->Delete();
  this->Modified();


}
// ------------------------------------------------------------------------------------
void vtkpxContourSource::Subsample(int rate)
{
  if (this->Points->GetNumberOfPoints()<2)
    return;

  vtkPoints* tmpPoints=vtkPoints::New();
  tmpPoints->SetNumberOfPoints(this->Points->GetNumberOfPoints()+1);
  for (int i=0;i<this->Points->GetNumberOfPoints();i++)
    tmpPoints->SetPoint(i,this->Points->GetPoint(i));

  int np=this->Points->GetNumberOfPoints();

  if (this->Closed)
    {
      tmpPoints->SetPoint(this->Points->GetNumberOfPoints(),this->Points->GetPoint(0));
      np+=1;
    }
  
  this->Points->SetNumberOfPoints(0);

  for (int j=0;j<np-1;j++)
    {
      double x1[3],x2[3];
      tmpPoints->GetPoint(j,x1);
      tmpPoints->GetPoint(j+1,x2);

      for (int k=0;k<rate;k++)
	{
	  float ratio2=((float)k)/((float)rate);
	  float ratio1=1.0-ratio2;
	  for (int l=0;l<=2;l++)
	    x1[l]=ratio1*x1[l]+ratio2*x2[l];
	  this->Points->InsertNextPoint(x1);
	}
    }
  
  // Add Last Point if open if closed this is the starting point hence ignore
  if (!this->Closed)
    this->Points->InsertNextPoint(tmpPoints->GetPoint(np-1));
  tmpPoints->Delete();
  this->Modified();
}
// ------------------------------------------------------------------------------------
void vtkpxContourSource::Equispace(float sp)
{
  if (this->Points->GetNumberOfPoints()<2)
      return;

  int NumPoints=this->Points->GetNumberOfPoints();
  float perimeter=this->GetContourLength();
  float Spacing=sp;
  if (Spacing<0.0)
    Spacing=perimeter/float(NumPoints);
  
  // Copy Point to temp array
  vtkPoints* tmpPoints=vtkPoints::New();
  tmpPoints->SetNumberOfPoints(NumPoints+1);
  int np=NumPoints;
  for (int j=0;j<NumPoints;j++)
    tmpPoints->SetPoint(j,this->Points->GetPoint(j));
  if (this->Closed)
    {
      tmpPoints->SetPoint(NumPoints,this->Points->GetPoint(0));
      np++;
    }

  // Allocate New set of points 
  // Start New Set of Points at point 0
  this->Points->SetNumberOfPoints(0);
  this->Points->InsertNextPoint(tmpPoints->GetPoint(0));

  // Initialize Control Points x0 and x1 
  int index=0;
  double x0[3],x1[3];
  tmpPoints->GetPoint(index,x0);
  tmpPoints->GetPoint(index+1,x1);  
  float segment=0.0;
  while(index<np-1)
      {
	float part=Fdist(x0[0],x0[1],x0[2],x1[0],x1[1],x1[2]);
	segment+=part;
	while (segment>=Spacing)
	  {
	    float ratio=1.0-(segment-Spacing)/part;
	    segment=segment-Spacing;
	    float tx[3];
	    for (int ia=0;ia<=2;ia++)
	      {
		tx[ia]=(1.0-ratio)*x0[ia]+ratio*x1[ia];
	      }
	      this->Points->InsertNextPoint(tx);
	  }
	index++;
	tmpPoints->GetPoint(index,x0);
	tmpPoints->GetPoint(index+1,x1);
      }

  if (!this->Closed)
    this->Points->InsertNextPoint(tmpPoints->GetPoint(np-1));

  tmpPoints->Delete();
  this->Modified();
}

// ------------------------------------------------------------------------------------
void vtkpxContourSource::Equisample(int npoints)
{
  if (this->Points->GetNumberOfPoints()<2)
    return;
  
  float perimeter=this->GetContourLength();
  float sp=perimeter/float(npoints);
  this->Equispace(sp);
}

// ------------------------------------------------------------------------------------
int vtkpxContourSource::CyclicDistance(int p1,int p2,int np)
{
  int half=np/2;
  int dist=0;
  if (abs(p1-p2)<half)
      {
	dist=p1-p2;
      }
  else
    {
      if (p1>p2)
	dist=p2+np-p1;
      else
	dist=p1+np-p2;
    }
  
  return abs(dist);  
}
// ------------------------------------------------------------------------------------
vtkpxContourSource* vtkpxContourSource::MapContours(vtkpxContourSource* cntr1,
						    vtkpxContourSource* cntr2,
						    int resample)
{
  if (resample)
    {
      cntr2->SetClosed(1);
      cntr2->Equisample(cntr1->GetPoints()->GetNumberOfPoints()*8);
      cntr2->Smooth(0.5);
    }
  
  int np1=cntr1->GetPoints()->GetNumberOfPoints();
  int np2=cntr2->GetPoints()->GetNumberOfPoints();

  vtkPoints* points1=vtkPoints::New();
  points1->DeepCopy(cntr1->GetPoints());
  
  vtkPoints* points2=vtkPoints::New();
  points2->DeepCopy(cntr2->GetPoints());
  
  int* corr   =new int[np1];
  int* used   =new int[np1];
  int* fixed  =new int[np1];
    
  // Pass 1 --- Find all points that have symmetric neighbours
  // ---------------------------------------------------------
  for (int p1=0;p1<np1;p1++)
    {
      used[p1]=0;
      fixed[p1]=0;
      corr[p1]=-1;
      float mindist=10000000.0;
      int   besti=0;
      for (int i=0;i<np2;i++)
	{
	  float dist=vtkMath::Distance2BetweenPoints(points1->GetPoint(p1),points2->GetPoint(i));
	  if (dist<mindist)
	    {
	      mindist=dist;
	      besti=i;
	    }
	}
      
      mindist=10000000.0;
      int bestj=p1;
      for (int j=0;j<np1;j++)
	    {
	      float dist=vtkMath::Distance2BetweenPoints(points1->GetPoint(j),points2->GetPoint(besti));
	      if (dist<mindist)
		{
		  bestj=j;
		  mindist=dist;
		}
	    }
      
      if (bestj==p1)
	{
	  used[p1]=1;
	  fixed[p1]=1;
	  corr[p1]=besti;
	}
    }
  
  // Pass 2: Fill in by averaging 
  int notdone=1;
  while (notdone==1)
    {
      notdone=0;
      int p1;
      for (p1=0;p1<np1;p1++)
	{
	  if (used[p1]==0)
	    {
	      int p0=Icyclic(p1-1,np1);
	      while (used[p0]==0)
		p0=Icyclic(p0-1,np1);
	      int p2=Icyclic(p1+1,np1);
	      while (used[p2]==0)
		p2=Icyclic(p2+1,np1);
	      
	      int dist0=vtkpxContourSource::CyclicDistance(p0,p1,np1);
	      int dist2=vtkpxContourSource::CyclicDistance(p1,p2,np1);
	      float frac=(float(dist0)/float(dist0+dist2));
	      
	      int c1=0;
	      if (corr[p2]<corr[p0])
		c1=int(float(corr[p2]+np2-corr[p0])*frac+corr[p0]);
	      else
		c1=int(float(corr[p2]-corr[p0])*frac+corr[p0]);
	      corr[p1]=Icyclic(c1,np2);
	    }
	}
      for (p1=0;p1<np1;p1++)
	{
	  if (corr[p1]>=0)
	    used[p1]=1;
	}
    }
  
  // Pass 3: Smoothing Iterations
  // ----------------------------
  int nummoved=1,iter=1;
  while(nummoved>0 && iter<500)
    {
      nummoved=0;
      for (int p=0;p<np1;p++)
	{
	  int p1=p;
	  if (fixed[p1]==0 || iter>0)
	    {
	      int bestdist=1000000000;
	      int shift=0;
	      for (int s=0;s<=2;s++)
		{
		  int sh=s;
		  if (s==2)
		    sh=-1;
		  int maxd=0,mind=100000000;
		  int ptest=Icyclic(corr[p1]+sh,np2);
		  for (int k=-1;k<=1;k+=2)
		    {
		      int p2=Icyclic(p1+k,np1);
		      int dist=vtkpxContourSource::CyclicDistance(corr[p2],ptest,np2);
		      maxd=Imax(maxd,dist);
		      mind=Imin(mind,dist);
		    }
		  if (maxd<bestdist)
		    {
		      bestdist=maxd;
		      shift=sh;
		    }
		}
	      if (shift!=0)
		{
		  nummoved++;
		  corr[p1]=Icyclic(corr[p1]+shift,np2);
		}
	    }
	}
      iter++;
    }

  // Cleanup
  // -------
  vtkpxContourSource* out=vtkpxContourSource::New();
  out->SetClosed(1);
  vtkPoints* outp=out->GetPoints();
  for (int i=0;i<np1;i++)
  outp->InsertNextPoint(points2->GetPoint(corr[i]));
			
  delete [] corr; delete [] used; delete [] fixed;
  return out;
}

void vtkpxContourSource::Execute()
{
vtkPolyData* output = this->GetOutput();

if (this->Points->GetNumberOfPoints()==0)
    return;

  vtkPoints    *c_points    = vtkPoints::New();
  vtkCellArray *c_lines     = vtkCellArray::New();
  
  c_points->DeepCopy(this->Points);
  c_lines->Allocate(this->Points->GetNumberOfPoints()+2);
  
  int np=this->Points->GetNumberOfPoints();
  vtkIdType pt[2];
  for (int i=0;i<np;i++)
    {
      pt[0]=i;
      pt[1]=i+1;
      c_lines->InsertNextCell(2,pt);
    }
  
  if (this->Closed==1)
    {
      pt[0]=np-1;
      pt[1]=0;
      c_lines->InsertNextCell(2,pt);
    }
  
  output->SetPoints(c_points);
  output->SetLines(c_lines);
  c_points->Delete();
  c_lines->Delete();
}


