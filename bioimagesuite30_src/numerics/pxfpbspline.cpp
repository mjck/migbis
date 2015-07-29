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
//
// This calls cyclictriadiag --  the only NR function called here. Snake functionality is obsolete and
// can be eliminated
//
//---------------------------------------------------------------------------------------------------------

#include "pxfpbspline.h"
#include "pxutil.h"
#include "nrutil.h"
#include "nr.h"
#include "pxregion.h"
#include "pxfloatimage.h"
#include "fitpack.h"
#include "pxfloatmatrix.h"
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris Decemeber 1997   papad@noodle.med.yale.edu

   see PXFPBSpline.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
// -------------------------------------------------------------------------
int  PXFPBSpline::debugmode=0;
real PXFPBSpline::lastfiterror=-1.0;
// -------------------------------------------------------------------------
PXFPBSpline::PXFPBSpline(PXFPBSpline *old)
{
  hasvectors=0;
  hassnake=0;  ownsgradients=0;  ownsenergymap=0;
  hasmatrices=0;
  numknots=0;
  numpoints=old->getnumpoints();
  setclosed(old->isclosed());
  setdegree(old->getdegree());
  dimension=old->getdimension();
  oldnumpoints=-1;
  generatecontrolvectors();

  int i;

  for (i=0;i<old->getnumknots();i++)
      setknot(i,old->getknot(i));

  for (i=0;i<numknots*dimension;i++)
      controlpoints[i]=old->getrawcontrol(i);
  
  zlevel=old->getzlevel();
}
// --------------------------------------------------------------------------
PXFPBSpline::PXFPBSpline(int deg,int dim,real z,int closed)
{
  oldnumpoints=-1;
  dimension=2+(dim>2);
  hasvectors=0;
  hassnake=0;  ownsgradients=0;  ownsenergymap=0;
  hasmatrices=0;
  numpoints=0;
  numknots=0;
  closedcurve=(closed>0);
  setdegree(deg);
  zlevel=z;
}
// --------------------------------------------------------------------------
PXFPBSpline::~PXFPBSpline()
{
  if (hasvectors==1)
      {
	delete [] controlpoints;
        delete [] knots;
      }

  if (hasmatrices)
      free_dmatrix(invMatrix,1,matsize,1,matsize);

  if (hassnake)
      removesnake();

}
// --------------------------------------------------------------
//  Vector Manipulation
// --------------------------------------------------------------

void PXFPBSpline::generatecontrolvectors()
{
  if (hasvectors==1)
      {
        delete [] knots;
        delete [] controlpoints;
      }

  if (hasmatrices)
      {
	free_dmatrix(invMatrix,1,matsize,1,matsize);
	matsize=0;
	hasmatrices=0;
      }

  if (isclosed())
      {
	numknots=(numpoints+2*degree+1);
	numcontrolpoints=(numpoints+degree);
      }
  else
      {
	numknots=(numpoints-2)+2*degree;   //	numknots=numpoints+2*degree;   
	numcontrolpoints=numpoints;
      }
  
  controlpoints=new real[numknots*dimension];
  controlx=&controlpoints[0];
  controly=&controlpoints[numknots];
  if (dimension==3)
      controlz=&controlpoints[numknots*2];
  else
      controlz=NULL;

  knots        =new real[numknots];  
  hasvectors   =1;
}

// --------------------------------------------------------------
//   Set Spline degree 1=linear ,  3=cubic (FPBSpline=3)
// --------------------------------------------------------------
void PXFPBSpline::setdegree(integer deg)
{
  if (deg<1)
      deg=1;
  if (deg>1)
      deg=3;

  degree=deg;
}

void PXFPBSpline::setclosed(int c)
{
  closedcurve=(c>0);
}
// --------------------------------------------------------------
real PXFPBSpline::getrawcontrol(int i) 
{ 
  if (hasvectors)
      {
	i=Irange(i,0,int(numknots*dimension-1));
	return controlpoints[i];
      }
  else
      return 0.0;
}
// --------------------------------------------------------------
real PXFPBSpline::getcontrolx(int i) 
{ 
  if (hasvectors)
      return controlx[indexbound(i,int(numcontrolpoints))];
  else
      return 0.0;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
real PXFPBSpline::getcontroly(int i)       
{ 
  if (hasvectors) 
      return controly[indexbound(i,int(numcontrolpoints))];
  else
      return 0.0;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
real PXFPBSpline::getcontrolz(int i)       
{ 
  if (hasvectors && dimension==3) 
      return controlz[indexbound(i,int(numcontrolpoints))];      
  else
      return zlevel;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void PXFPBSpline::setcontrol(int i,real x,real y,real z)
{
  if (hasvectors)
      {
	i=indexbound(i,int(numcontrolpoints));
	controlx[i]=x;
	controly[i]=y;
	if (dimension==3)
	    controlz[i]=z;
      }
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void PXFPBSpline::shiftcontrol(int i,real dx,real dy,real dz)
{
  if (hasvectors)
    {
      //int old=i;
      i=indexbound(i,int(numcontrolpoints));
      controlx[i]=controlx[i]+dx;
      controly[i]=controly[i]+dy;
      if (dimension==3)
	controlz[i]=controlz[i]+dz;
    }
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  
void PXFPBSpline::setClosedControl(int i,real x,real y,real z,int off)
{
  if (!isclosed())
    {
      setcontrol(i,x,y,z);
	return;
      }
  
  if (hasvectors)
    {
      i=indexbound(i,int(numpoints))+off;
      controlx[i]=x;
      controly[i]=y;
      if (dimension==3)
	controlz[i]=z;
      if (i<degree)
	{
	  controlx[i+numpoints]=x;
	  controly[i+numpoints]=y;
	  if (dimension==3)
	    controlz[i+numpoints]=z;
	}
      if (i==numpoints)
	{
	  controlx[0]=x;
	  controly[0]=y;
	  if (dimension==3)
	    controlz[0]=z;
	}
    }
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  
void PXFPBSpline::shiftClosedControl(int i,real dx,real dy,real dz,int off)
{
  if (!isclosed())
      {
	shiftcontrol(i,dx,dy,dz);
	return;
      }

  if (hasvectors)
      {
	i=indexbound(i,int(numpoints))+off;
	controlx[i]+=dx;
	controly[i]+=dy;
	if (dimension==3)
	    controlz[i]+=dz;
	if (i<degree)
	    {
	      controlx[i+numpoints]+=dx;
	      controly[i+numpoints]+=dy;
	      if (dimension==3)
		  controlz[i+numpoints]+=dz;
	    }
	if (i==numpoints)
	    {
	      controlx[0]+=dx;
	      controly[0]+=dy;
	      if (dimension==3)
		  controlz[0]+=dz;
	    }
      }
}
     
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  
real PXFPBSpline::getzlevel()
{
  return zlevel;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -     
void  PXFPBSpline::setzlevel(real z)
{
  zlevel=z;
  if (hasvectors && dimension==3) 
      {
	for(int i=0;i<numknots;i++)
	    controlz[i]=z;
      }
}
// --------------------------------------------------------------
real PXFPBSpline::getknot(int i)
{
  if (hasvectors) 
      {
	int j=indexbound(i,int(numknots));
	return knots[j];
      }
  else
      return 0.0;
}
// --------------------------------------------------------------
real PXFPBSpline::getcknot(int i)
{
  if (hasvectors) 
      {
	int j=indexbound(i,int(numcontrolpoints));
	if (isclosed())
	    return knots[j+degree];
        
	// Open Curve
	if (j!=1 && j!=numpoints-2)
	    return knots[j+degree-1];

	if (j==1)
	    return (1.0*knots[degree+1]+2.0*knots[degree])/3.0;
	if (j==(numpoints-2))
	    return (2.0*knots[degree+numpoints-3]+1.0*knots[degree+numpoints-4])/3.0;

      }
  return 0.0;
}

// --------------------------------------------------------------
void PXFPBSpline::setknot(int i,real t)
{
  if (hasvectors)
    {
      i=indexbound(i,int(numknots));
      knots[i]=t;
    }
}

// --------------------------------------------------------------
//       Set spline to be a circle or  an ellipse
// --------------------------------------------------------------
void PXFPBSpline::setcircle(int np,real radius,real x,real y,int dim)
{
  setellipse(np,radius,radius,x,y,dim);
}
// --------------------------------------------------------------
void PXFPBSpline::setellipse(int npoints,real radiusx,real radiusy,
			     real x,real y,int dim)
{
  int np2=int(float(Irange(npoints,4,40))*1.20);

  dimension=Irange(dim,2,3);
  closedcurve=1;
  zlevel=0.0;  
  degree=3;

  integer  np       =np2+closedcurve; // Number of Knots 
  integer  npm      =np*dimension;
  real*    points   =new real[npm];
  int      ret=0;

  real zp=zlevel;
  for (int k=0;k<np-closedcurve;k++)
    {
      int index=k*dimension;
      real t=2.0*M_PI*(real(k)/real(np-closedcurve));
      real xp=radiusx*cos(t)+x;
      real yp=radiusy*sin(t)+y;
      

      points[index]  =xp;
      points[index+1]=yp;
      if (dimension==3)
	points[index+2]=zp;
    }
  
  if (isclosed())
    {
      points[(np-1)*dimension]=points[0];      
      points[(np-1)*dimension+1]=points[1];
      if (dimension==3)
	points[(np-1)*dimension+2]=points[2];
    }
  
  real*    W   =new real[np];
  real*    U   =new real[np];
  integer  IPAR=0;
  integer  IOPT=-1;
  
  for(int i=0;i<np;i++)
    W[i]=1.0;
	
  ret=fitpoints(np,W,U,npm,points,IOPT,IPAR,1.0,npoints);
  
  delete [] W;
  delete [] U;
  delete [] points;
  return;
}
// --------------------------------------------------------------
// Copy control points from another spline
// --------------------------------------------------------------
void PXFPBSpline::copypoints(PXFPBSpline* other)
{
  if (hasmatrices)
      free_dmatrix(invMatrix,1,matsize,1,matsize);
  hasmatrices=0;
  
  setclosed(other->isclosed());
  dimension=(other->getdimension());
  setdegree(other->getdegree());
  numpoints=other->getnumpoints();
  numknots=other->getnumknots();
  generatecontrolvectors();
  zlevel=other->getzlevel();

  int i;
  for (i=0;i<other->getnumknots();i++)
      setknot(i,other->getknot(i));
  for (i=0;i<numknots*dimension;i++)
      controlpoints[i]=other->getrawcontrol(i);
}
// --------------------------------------------------------------
//  Set control points to xcntrl[1:np] ycntr[1:np]  zcntrl[1:np] doz=1
// --------------------------------------------------------------
void PXFPBSpline::ResetUniformKnots()
{
  int i;
  int order=int(degree)+1;

  if (numpoints<1)
      return;

 if (isclosed())
      {
	for(i=int(degree)+1;i<=degree+numpoints;i++)
	    {
	      if (i<numknots)
		  knots[i]=real(i-degree)/real(numpoints);
	      else
		  fprintf(stderr,"Overflow %d\n", i);
	    }
	
	for (i=0;i<=degree;i++)
	    {
	      knots[i]=knots[i+numpoints]-1.0;
	      knots[numknots-1-i]=knots[numknots-1-numpoints-i]+1.0;
	    }
      }
  else
      {
		// First Exterior Knots 
	for (i=0;i<order;i++)
	    {
	      knots[i]=0.0;
	      knots[numknots-i-1]=1.0;
	    }
	// Then Interior Knots ---  uniform Parameterization
	for (i=int(degree)+1;i<numpoints+degree-2;i++)
	    {
	      if (i<numknots)
		  knots[i]=real(i-degree)/real(numpoints-3);
	      else
		  fprintf(stderr,"Overflow \n");
	    }

      }
  
}
// ------------------------------------------------------------------
int PXFPBSpline::indexbound(int t,int np,int off)
{
  if (np<0)   
      np=int(getnumknots());
  
  if (t<off)    
      t=off;
  if (t>=np+off)  
      t=np+off-1;
  return t;
}
// ------------------------------------------------------------------
int PXFPBSpline::indexcyclic(int t,int np,int off)
{
  if (np<0)   
      np=int(getnumpoints());
  
  while(t<off)    
      t=t+np;
  while(t>=np+off)  
      t=t-np;
  return t;
}
// --------------------------------------------------------------------------
real PXFPBSpline::arclengthcyclic(real t)
{
  if (isclosed())
      {
	while(t<0.0)  t=t+1.0;
	while(t>=1.0) t=t-1.0;
      }
  else
      {
	if (t<=0.0)   t=0.001;
	if (t>=1.0)   t=0.999;
      }
  return t;
}


// --------------------------------------------------------------------------
PXContour* PXFPBSpline::storeincontour(int nodepoints)
{
  PXContour* cntr=new PXContour();
  if (nodepoints==0)
      {
	for (int i=0;i<numpoints;i++)
	    cntr->Add(getcontrolx(i),getcontroly(i));
      }
  else
      {
	real xp,yp;
	for (int ib=0;ib<numpoints;ib++)
	    {
	      getposition(getcknot(ib),xp,yp);
	      cntr->Add(xp,yp);
	    }
      }
  return cntr;
}
// --------------------------------------------------------------------------
void PXFPBSpline::getknotpoints(real* xp,real* yp,int np)
{
  for (int ib=0;ib<numpoints;ib++)
      {
	if (ib<np)
	    getposition(getcknot(ib),xp[ib],yp[ib]);
      }
}
// --------------------------------------------------------------------------
void PXFPBSpline::getknotpoints(real* xp,real* yp,real* zp,int np)
{
  
  for (int ib=0;ib<numpoints;ib++)
      {
	if (ib<np)
	    getposition(getcknot(ib),xp[ib],yp[ib],zp[ib]);
      }
}
// --------------------------------------------------------------------------
PXContour* PXFPBSpline::exporttoNcontour(int np,int resample)
{
  PXContour* cntr=new PXContour();;
  np=Irange(np,4,1000);
  real x,y;
  float step=1.0/(float(np)*5.0);

  if (resample>0)
      {
	PXFPBSpline* temp=new PXFPBSpline(this);
	//	fprintf(stderr,"In change Spacing np=%d step=%f closed=%d\n",np,step,this->isclosed());
	temp->changespacing(0.5,-1,step);
	//	fprintf(stderr,"In change Spacing Done\n");
	for (int i=0;i<np;i++)
	    {
	      float s=float(i)/float(np);
	      temp->getposition(s,x,y);
	      cntr->Add(x,y);
	    }
	delete temp;
      }
  else
      {
	for (int i=0;i<np;i++)
	    {
	      float s=float(i)/float(np);
	      getposition(s,x,y);
	      cntr->Add(x,y);
	    }
      }
  cntr->setclosed(this->isclosed());
  return cntr;
}

// --------------------------------------------------------------------------
PXContour* PXFPBSpline::exporttocontour(real dist)
{
  dist=Frange(dist,0.01,20.0);
  float length=getlength(0.0,1.0,0.05);
  int np=int(length/dist+0.5);

  if (debugmode>1)
      fprintf(stderr," Input Distance=%f, Length=%f (0.05) np=%d\n",dist,length,np);

  float ds=1.0/float(2*np);
  length=getlength(0.0,1.0,ds);
  np=int(length/dist+0.5);
      
  if (debugmode>1)
      fprintf(stderr," Input Distance=%f, Length=%f (%7.4f) np=%d\n",dist,length,ds,np);

  if (np<3)
      np=3;

  // MAJOR CHANGE np,1 --> np,0   np,1 ---> major error!!!
  PXContour* cntr=exporttoNcontour(np,0);
  cntr->setclosed(this->isclosed());
  return cntr;
}
// --------------------------------------------------------------------------
int PXFPBSpline::sample(int sm,float step)
{
  return changespacing(0.5,int(numpoints)/sm,step);
}
// --------------------------------------------------------------------------
int PXFPBSpline::subsample(int samples,float step)
{
  return changespacing(0.5,int(numpoints)*samples,step);
}
// --------------------------------------------------------------------------
int PXFPBSpline::resample(float ds_step,int npoints)
{
  return changespacing(0.5,npoints,ds_step);
}
// --------------------------------------------------------------------------
int PXFPBSpline::changespacing(real smth,int npoints,float step,float ,int iterations)
{
  float zl=zlevel;
  if (dimension==3)
      {
	if (debugmode)
	  fprintf(stderr, "Cannot resample 3-D curve sorry\n");
	return 0;
      }

  int ret=0;

  if (step<0)
    {
      real minstep=1.0;
      for (int i=0;i<numknots-1;i++)
	{
	  real s=fabs(knots[i+1]-knots[i]);
	  if (s<minstep)
	    minstep=s;
	}
      step=minstep*0.05;
    }
  else
    {
      if (step>0.25)
	step=0.25;
      if (step<0.01)
	step=0.01;
    }

  PXContour* cntr=new PXContour();
  for (real s=0.0;s<1.0;s=s+step)
      {
	real x,y;
	getposition(s,x,y);
	cntr->Add(x,y);
      }




 
  if (npoints<degree)
    {
      //      fprintf(stderr,"+++ Points = %d,%d deg=%d\n",cntr->getnumpoints(),npoints,degree);
      ret=fitcontour(cntr,smth,0.0,int(degree),isclosed(),0,-1,iterations);

    }
  else
    {
      //      fprintf(stderr,"** Points = %d,%d\n",cntr->getnumpoints(),npoints);
      ret=fitcontour(cntr,smth,0.0,int(degree),isclosed(),1,npoints,iterations);
    }
  
  delete cntr;
  zlevel=zl;
  return ret;
}

// --------------------------------------------------------------------------
int PXFPBSpline::fittocontour(PXContour* cntr,real smooth,real csmooth,
			      int numpoints,int closed,int iterations)
{
  int uniform=(numpoints>0);

  if (numpoints==-2)
    uniform=1;

  return (fitcontour(cntr,smooth,csmooth,3,closed,uniform,numpoints,iterations)<=0);
}
// --------------------------------------------------------------------------

int PXFPBSpline::fitcontour(PXContour* cnt_in,real smth,real cntrsmth,
			    int deg,int cl,int uniform,int npoints,int iterations)
{
  PXContour* cntr=new PXContour(cnt_in);
  if (debugmode)
    fprintf(stderr,"Numberof Points %d uniform=%d npoints=%d\n",cnt_in->getnumpoints(),uniform,npoints);

  // Set bspline parameters 
  closedcurve=(cl>0);  dimension=2;  zlevel=0.0;  degree=deg;
  integer  np       =cntr->getnumpoints()+closedcurve; // Number of Knots 
  integer  npm      =np*dimension;
  real*    points   =new real[npm];
  int ret=0;

  if (cntrsmth>=0.5)
      {
	//cntr->Smooth(cntrsmth);
	cntr->nonShrinkSmooth(cntrsmth,iterations);
      }
  
  PXContourPoint* temp=cntr->gettoppoint();
  for (int k=0;k<np-closedcurve;k++)
      {
	int index=k*2;
	points[index]  =temp->x;   
	points[index+1]=temp->y;
	temp=temp->next;
      }
  
  if (isclosed())
      {
	points[(np-1)*2]=points[0];      
	points[(np-1)*2+1]=points[1];
      }
  
  if (cntrsmth<0.5 && uniform<1)
      {
	ret=fitpoints(np,npm,points,smth);
      }
  else
      {
	real*    W   =new real[np];
	real*    U   =new real[np];
	integer  IPAR=(cntrsmth>=0.5);
	integer  IOPT=-1;
	if (uniform==0)
	  IOPT=0;
	
	
	for(int i=0;i<np;i++)
	    W[i]=1.0;
	
	if (IPAR==1)
	    {
	      U[0]=0.0;
	      PXContourPoint* temp=cntr->gettoppoint();
	      real x0=temp->x,y0=temp->y;
	      
	      for (int j=1;j<np-closedcurve;j++)
		  {
		    temp=temp->next;
		    U[j]=U[j-1]+sqrt(pow(temp->x-x0,float(2.0))+
				     pow(temp->y-y0,float(2.0)));
		    x0=temp->x;
		    y0=temp->y;
		  }
	      
	      if (isclosed())
		  {
		    temp=cntr->gettoppoint();
		    U[np-1]=U[np-2]+sqrt(pow(x0-temp->x,float(2.0))+pow(temp->y-y0,float(2.0)));
		  }
	      for(int l=0;l<np;l++)
		  U[l]=U[l]/U[np-1];
	    }

	ret=fitpoints(np,W,U,npm,points,IOPT,IPAR,smth,npoints);

	delete [] W;
	delete [] U;
      }

  delete cntr;
  delete [] points;
  return ret;
}
// --------------------------------------------------------------------------
int PXFPBSpline::fitpoints(integer m,integer mx,real* X,float smth)
{  
  real*    W   =new real[m];
  real*    U   =new real[m];

  for(int i=0;i<m;i++)
      W[i]=1.0;

  int ret=fitpoints(m,W,U,
		    mx,X,
		    0,0,smth);

  delete [] W;
  delete [] U;
  return ret;
}
// ------------------------------------------------------------------
int PXFPBSpline::fitpoints(integer m,real* W,real* U,integer mx,real* X,
			   integer IOPT,integer IPAR,float smth,int npoints)
{  
  integer  M   =m;
  integer  MX  =mx;
  integer  K   =getdegree();
  real     S   =smth;                // Smoothing Factor
  integer  NEST=2*(M+K);             // Temporary Storage Space size
  integer  N   =0;                   // Number of Knots
  real*    T   =new real[NEST];      // Store Knots on Exit
  integer  NC = NEST*dimension;
  real*    C   =new real[NC];        // Store Weights on Exit

  real     FP  =0.0;                 // Residual Error
  integer  LWRK=M*(K+1)+NEST*(7+dimension+5*K);
  real*    WRK =new real[LWRK];
  integer* IWRK=new integer[NEST];
  integer  IER =0;


  if (IOPT==-1)
      {
	if (isclosed())
	    N=(npoints+2*degree+1);
	else
	    N=(npoints-2)+2*degree;
		
	for (int i=0;i<npoints;i++)
	    {
	      T[i+degree]=real(i)/real(npoints-3*(1-closedcurve));
	    }
      }

  if (isclosed())
      {
	// Calling FITPACK 
	clocur_(&IOPT, &IPAR,&dimension,
		&M, U,&MX,X,W,
		&K,&S,&NEST,&N,T,&NC,C,
		&FP,WRK,&LWRK,IWRK,&IER);
      }
  else
      {
	real UB,UM;
	if (IPAR==1)
	    {
	      UB=U[0];
	      UM=U[M-1];
	    }
		  
	// Calling FITPACK 
	parcur_(&IOPT, &IPAR,&dimension,
		&M, U,&MX,X,W,&UB,&UM,
		&K,&S,&NEST,&N,T,&NC,C,
		&FP,WRK,&LWRK,IWRK,&IER);
      }

  
  if (int(IER)<=0)
      {
	
	if (isclosed())
	    numpoints=N-2*K-1;
	else
	    numpoints=N-2*K+2;
	generatecontrolvectors();
	if (debugmode)
	    fprintf(stderr,"Output :  Clocur(%1d)/Parcur(%1d)  IER=%d FP=%7.4f N=%d ,IOPT=%d , IPAR=%d\n",
		    isclosed(),!isclosed(),
		    int(IER),FP,int(N),int(IOPT),int(IPAR));
	
	for (int i=0;i<numknots;i++)
	    knots[i]=T[i];
	for (int j=0;j<numknots*dimension;j++)
	    controlpoints[j]=C[j];
      }
  else
      {
	if (debugmode)
	    {
	      fprintf(stderr,"E R R O R   :  Clocur/Parcur output IER=%d FP=%7.4f N=%d ,IOPT=%d , IPAR=%d\n",int(IER),FP,int(N),int(IOPT),int(IPAR));
	    }
      }
  lastfiterror=FP;
  delete [] IWRK;
  delete [] WRK;
  delete [] C;
  delete [] T;
  return (int)IER;
}
// ------------------------------------------------------------------
//           I/O Functions
//
//        Save & Load -->              Spline Format (Control Points)
//        SaveContour & LoadContour    Contour Format
// ------------------------------------------------------------------
int  PXFPBSpline::Save(const char* filename)
{
  int ret=0;
  FILE* fout=fopen(filename,"w");
  if (fout!=NULL)
    {
      ret=Save(fout);
      fclose(fout);
    }
  if (debugmode)
      fprintf(stderr,"Saved in %s (Status=%1d)\n",filename,ret);
  return(ret);
}
// ------------------------------------------------------------------
int  PXFPBSpline::Save(FILE* fout )
{
  fprintf(fout, "#FPBSpline File header= dimension,zlevel,degree,closed,numpoints\n");
  fprintf(fout, "#%d\n" , (int)dimension );
  fprintf(fout, "#%f\n" , zlevel );
  fprintf(fout, "#%d\n" , (int)degree );
  fprintf(fout, "#%d\n" , closedcurve );
  fprintf(fout, "#%d\n" , (int)numpoints );
  fprintf(fout, "#----------------------\n");

  int i;
  fprintf(fout,"#");
  for (i=0;i<numknots;i++)
      fprintf(fout,"%8.5f ",real(knots[i]));

  fprintf(fout,"\n"); 
  for (i=0;i<numcontrolpoints;i++)
      {
	for (int j=0;j<dimension;j++)
	    {
	      fprintf(fout,"%6.2f ",controlpoints[i+j*numknots]);
	    }
	fprintf(fout,"\n");
      }
  return 1;
}
// --------------------------------------------------------------
int PXFPBSpline::Load(const char* filename)
{
  int status=0;
  gzFile fin=gzsuffixopen(filename,"r");
  if (fin!=Z_NULL)
      status=Load(fin);

  gzclose(fin);

  if (debugmode && status==1)
      fprintf(stderr,"Loading from %s (numpoints=%2d) Status=%d\n",filename,
	      int(numpoints),status);

  if (debugmode && status==0)
      fprintf(stderr,"Failed to Load from %s\n",filename);

  return(status);
}
// --------------------------------------------------------------
int PXFPBSpline::Load(gzFile fin)
{
  char line[200];
  int status=0;
  gzgets(fin,line,200);

  if (gzfindstringinstring(line, "#FPBSpline File"))
      {
	int deg=3,clo=0,dim,np;
	real kn;
	gzgets(fin,line,100); sscanf(line,"#%d",&dim);
	gzgets(fin,line,100); sscanf(line,"#%f",&zlevel);
	gzgets(fin,line,100); sscanf(line,"#%d",&deg);
	
	gzgets(fin,line,100); sscanf(line,"#%d",&clo);
	setclosed(clo);
	setdegree(deg);
	dimension=Irange(dim,2,3);
	gzgets(fin,line,100); sscanf(line,"#%d",&np);
	numpoints=np;
	gzgets(fin,line,100);
	if (numpoints>0 && numpoints<2048)
	    {
	      status=1;
	      generatecontrolvectors();
	      gzgetc(fin); // eat the leading #
	      for (int i=0;i<numknots;i++)
		{
		  kn=getgzstreamfloat(fin);
		  setknot(i,kn);
		}
	      if (isclosed())
		{
		  for (int i=0;i<degree;i++)
		    {
		      int ik=i+numpoints;

		      double v=knots[ik]-1.0;
		      if (fabs(knots[i]-v)>0.01)
			{
			  knots[i]=v;
			  //  fprintf(stderr,"Mapping %d to %d (%.3f to %.3f)\n",
			  //		  i,ik,knots[i],v);
			}
		      int ip=i+numpoints+int(degree)+1;
		      int il=i+degree+1;
		      double vv=knots[il]+1.0;
		      if (fabs(knots[ip]-vv)>0.01)
			{
			  //  fprintf(stderr,"Mapping %d to %d (%.3f to %.3f)\n",
			  //	  ip,il,knots[ip],vv);
			  knots[ip]=vv;
			}
		    }
		}

	      //gzgets(fin,line,100);
	      for(int i=0;i<numcontrolpoints;i++)
		{
		  for (int j=0;j<dimension;j++)
		    {
		      controlpoints[i+j*numknots]=getgzstreamfloat(fin);
		      //  fprintf(stderr,"Ctrl %d val %f\n",i+j*numknots,controlpoints[i+j*numknots]);
		    }
		}
	      gzgets(fin,line,100);
	    }

      }



  return(status);
}
// --------------------------------------------------------------
int PXFPBSpline::SaveContour(const char* filename,real dist)
{
  PXContour* tempcntr=exporttocontour(dist);
  int r=tempcntr->Save(filename);
  delete tempcntr;
  if (debugmode)
      fprintf(stderr,"Saved in %s as contour d=%5.2f (Status=%1d)\n",filename,dist,r);

  return r;
}
// --------------------------------------------------------------
//      Evaluate FPBSpline position and derivative info
//             xpoint[] , ypoint[] contain the conrol points
//                        use control2vector() to get these
//
//   getposition(real s,real& x,real &y)
//   getderivative(real s,real &x,real &y)
//   get2derivative(real s,real x,real &y)
//   real  getcurvature(real s)
//   real getlength(real s0,real s1,real ds_step)
//   getmeanposition(real& x,real& y)
//
// --------------------------------------------------------------
void  PXFPBSpline::getposition(real s,real& x,real& y)
{
  real z;
  getposition(s,x,y,z);
     
}
// --------------------------------------------------------------
void  PXFPBSpline::getposition(real s,real& x,real& y,real& z)
{
  integer ier=0;
  integer M=1;
  integer K=3;
  integer N=numknots;

  integer NC=numknots*dimension;
  integer mx=3;
  real    X[3];
  
  s=arclengthcyclic(s);

  // Calling FITPACK 
  curev_(&dimension,
	 knots,&N,
	 controlpoints,&NC,
	 &K,&s,&M,X,&mx,&ier);
	
  x=X[0]; y=X[1]; 
  if (dimension==3)
      z=X[2];
  else
      z=zlevel;

}
// --------------------------------------------------------------
void PXFPBSpline::getallderivatives(integer n,real  s,real* xd,real* yd,real* zd)
{
  integer ier=0;
  integer NC=numknots*dimension;
  integer k1=degree+1;
  integer nd=dimension*k1;
  real    D[20];

  // Calling FITPACK 
  s=arclengthcyclic(s);
  cualde_(&dimension,knots,&numknots,
          controlpoints,&NC,
          &k1,&s,
          D,&nd,&ier);

  for (int i=1;i<=n;i++)
      {
	xd[i-1]=D[dimension*i];
	yd[i-1]=D[dimension*i+1];
	if (dimension==3)
	    zd[i-1]=D[dimension*i+2];
      }
}
// --------------------------------------------------------------
void  PXFPBSpline::getderivative(real s,real& x,real& y)
{
  real xd[3],yd[3],zd[3];
  getallderivatives(3,s,xd,yd,zd);
  x=xd[0];
  y=yd[0];
  
}
// --------------------------------------------------------------
void  PXFPBSpline::getderivative(real s,real& x,real& y,real& z)
{
  real xd[3],yd[3],zd[3];
  getallderivatives(3,s,xd,yd,zd);
  x=xd[0];
  y=yd[0];
  z=zd[0];
}
// --------------------------------------------------------------
void  PXFPBSpline::get2derivative(real s,real& x,real& y)
{
  real xd[3],yd[3],zd[3];
  getallderivatives(3,s,xd,yd,zd);
  x=xd[1];
  y=yd[1];
}

// --------------------------------------------------------------
void  PXFPBSpline::get2derivative(real s,real& x,real& y,real& z)
{
  real xd[3],yd[3],zd[3];
  getallderivatives(3,s,xd,yd,zd);
  x=xd[1];
  y=yd[1];
  z=zd[1];
}

// --------------------------------------------------------------
real  PXFPBSpline::getcurvature(real s)
{
  real xd[3],yd[3],zd[3];
  getallderivatives(3,s,xd,yd,zd);
  real xder,yder,x2der,y2der;

  xder =xd[0];
  x2der=xd[1];
  yder =yd[0];
  y2der=yd[1];
  real nom=pow(xder*y2der-x2der*yder,float(2.0));
  real denom=pow(xder*xder+yder*yder,float(3.0));
  if ( denom <= 0.000001 ) 
      denom = 0.000001;
  real curv=nom/denom;
  // Round to 4 d.p. to reduce round-off errors
  // -------------------------------------------
  return quantize(curv,4);
}

// --------------------------------------------------------------
real PXFPBSpline::getlength(real s0,real s1,real ds_step)
{
  real x0=0.0,x,y0=0.0,y,z0=zlevel,z=zlevel;
  real length=0.0;

  for (real t=s0;t<=s1;t=t+ds_step)
      {
	if (dimension==2)
	      getposition(t,x,y);
	else
	    getposition(t,x,y,z);
	if (t!=s0)
	    length=length+sqrt(pow(x-x0,float(2.0))+pow(y-y0,float(2.0))+pow(z-z0,float(2.0)));
	
	x0=x;
	y0=y;
	z0=z;
      }
  return length;
}
// --------------------------------------------------------------
// S N A K E  F U N C T I O N A L I T Y
// --------------------------------------------------------------
void PXFPBSpline::createsnake(PXImage* pimage,float img_coeff,float smooth_coeff,
			      float timestep,float sigma,int niter,float baloon_coeff)
{
  if (hassnake)
      removesnake();

  if (getnumpoints()>0 && isclosed())
      {
	hassnake=1;
      }
  else
      {
	hassnake=0;
	return;
      }
  
  sn_offsetx=0;
  sn_offsety=0;
  sn_imweight=img_coeff;
  sn_smweight=smooth_coeff;
  sn_usrtstep=timestep/float(numpoints);
  sn_numiter=niter;
  sn_sigma=sigma;
  sn_baloon=baloon_coeff;

  
  ownsgradients=0;
  sn_maxgrad=calcpotential(pimage);
  ownsenergymap=1;
  ownsgradients=1;
  

  if (debugmode)
      fprintf(stderr,"Creating Snake %d (%d)\n",int(numpoints),isclosed());
}
// --------------------------------------------------------------
void PXFPBSpline::createsnake(PX2DRegion* region,
			      float region_weight,float smooth_coeff,
			      float timestep,float sigma,int niter,float baloon_coeff)
{
  createsnake(NULL,region,NULL,0.0,region_weight,0.0,smooth_coeff,timestep,
	      sigma,niter,baloon_coeff);
}
// --------------------------------------------------------------
void PXFPBSpline::createsnake(PXImage* pimage,PX2DRegion* region,
			      float img_coeff,float region_weight,
			      float smooth_coeff,
			      float timestep,float sigma,int niter,float baloon_coeff)
{
  createsnake(pimage,region,NULL,img_coeff,region_weight,0.0,smooth_coeff,timestep,
	      sigma,niter,baloon_coeff);
}
// --------------------------------------------------------------
void PXFPBSpline::createsnake(PXImage* pimage,PXFloatImage* external,
			      float img_coeff,float ext_weight,
			      float smooth_coeff,
			      float timestep,float sigma,int niter,float baloon_coeff)
{
  if (debugmode)
    fprintf(stderr,"ext_weight2=%5.2f\n",ext_weight);
  createsnake(pimage,NULL,external,img_coeff,0.0,ext_weight,smooth_coeff,timestep,
	      sigma,niter,baloon_coeff);

}
// --------------------------------------------------------------
void PXFPBSpline::createsnake(PX2DRegion* region,PXFloatImage* external,
			      float region_weight,float ext_weight,
			      float smooth_coeff,
			      float timestep,float sigma,int niter,float baloon_coeff)
{
  if (debugmode)
    fprintf(stderr,"ext_weight3=%5.2f\n",ext_weight);
  createsnake(NULL,region,external,0.0,region_weight,ext_weight,smooth_coeff,timestep,
	      sigma,niter,baloon_coeff);
}

// --------------------------------------------------------------

void PXFPBSpline::createsnake(PXImage* pimage,PX2DRegion* region,PXFloatImage* external,
			      float img_coeff,float region_weight,float ext_weight,
			      float smooth_coeff,
			      float timestep,float sigma,int niter,float baloon_coeff)
{
  if (debugmode)
    fprintf(stderr,"ext_weight4=%5.2f\n",ext_weight);

  if (hassnake)
      removesnake();
  
  if (getnumpoints()>0 && isclosed())
      {
	hassnake=1;
      }
  else
      {
	hassnake=0;
	return;
      }
  sn_offsetx=0;  sn_offsety=0;
  sn_imweight=img_coeff+region_weight;  
  sn_smweight=smooth_coeff;
  sn_usrtstep=timestep/float(numpoints);
  sn_numiter=niter;  sn_sigma=sigma;
  sn_maxgrad=calc3potential(pimage,region,external,img_coeff,region_weight,ext_weight);
  sn_baloon=baloon_coeff;
  ownsgradients=1;  ownsenergymap=1; 
}
// --------------------------------------------------------------
void PXFPBSpline::createsnake(PXFloatImage* pimage,
			      float img_coeff,float smooth_coeff,
			      float timestep,float sigma,int niter,float baloon_coeff)
{
  if (hassnake)
      removesnake();
  if (getnumpoints()>0 && isclosed())
      {
	hassnake=1;
      }
  else
      {
	hassnake=0;
	return;
      }
  
  sn_offsetx=0;  sn_offsety=0;
  sn_imweight=img_coeff;  sn_smweight=smooth_coeff;
  sn_usrtstep=timestep/float(numpoints);
  sn_numiter=niter;  sn_sigma=sigma;

  /*sn_energymap=pimage;
  ownsenergymap=0;*/

  sn_energymap=new PXFloatImage(pimage);
  ownsenergymap=1;
  sn_maxgrad=generategradients();

}
// --------------------------------------------------------------
real PXFPBSpline::generategradients()
{
  int sg=int(sn_sigma*2.0+0.5)*2+1;
  PXFloatMatrix* kernelx=new PXFloatMatrix(sg,1);
  PXFloatMatrix* kernely=new PXFloatMatrix(1,sg);
  kernelx->GenGradGaussian1Dx(fabs(sn_sigma)/2.0);
  kernely->GenGradGaussian1Dy(fabs(sn_sigma)/2.0);
  
  int x1=2000,x2=0,y1=2000,y2=0,x,y;
  
  for (int kk=0;kk<getnumpoints();kk++)
      {
	x=(int)getcontrolx(kk);
	y=(int)getcontroly(kk);
	if (x<x1) x1=x;
	if (x>x2) x2=x;
	if (y<y1) y1=y;
	if (y>y2) y2=y;
      }
  
  x1=x1-10; x2=x2+10; y1=y1-10; y2=y2+10;
  if (x1<1) x1=1; 	if (y1<1) y1=1;
  if (x2>=sn_energymap->getsizex()-1) x2=sn_energymap->getsizex()-1;
  if (y2>=sn_energymap->getsizey()-1) y2=sn_energymap->getsizey()-1;
  
  sn_energymap->SetMaxMingrayscale();
  sn_gradx = new PXFloatImage(sn_energymap,kernelx,x1,x2,y1,y2);
  sn_grady = new PXFloatImage(sn_energymap,kernely,x1,x2,y1,y2);
  float maxg=Fmax(sn_energymap->getmaxgrayscale(),
		  fabs(sn_energymap->getmingrayscale()));
  
  delete kernelx;
  delete kernely;
  ownsgradients=1;
  return maxg;
}
// --------------------------------------------------------------
real PXFPBSpline::updatesnake(real tolerance)
{
  if (!hassnake)
      return 0.0;


  real der_step=0.5*sn_sigma,max_move=0.0;
  real fx=0.0,fy=0.0;
  real sx=0.0,sy=0.0;
  real bx=0.0,by=0.0;
  
  for(int n=0;n<sn_numiter;n++)
      {
	if (debugmode)
	  fprintf(stderr,"\n Iteration:%d\n ",n);
	max_move=0.0;
	int shift=int(float(getnumpoints())*float(rand()/32768.0)+0.5);
	for(int aj=0;aj<getnumpoints();aj++)
	  {
	    int j=indexcyclic(aj+shift,int(numpoints),0);
	    if (debugmode)
	      fprintf(stderr,"%2d ",j);
	    controlexternalforce(fx,fy,j);
	    controlbaloonforce(bx,by,j);
	    splinechangeinternalenergy(j,der_step,sx,sy);
	    
    
	    if (debugmode)
	      fprintf(stderr,"Forces Ext(%f:%f) Baloon (%f,%f) Int (%f:%f)\n ",fx,fy,bx,by,sx,sy);
	      
	    real dx0=(-sn_smweight*sx*0.001 + sn_baloon*bx + sn_imweight*fx);
	    real dy0=(-sn_smweight*sy*0.001 + sn_baloon*by + sn_imweight*fy);
	      
	    // adaptively estimate the time step size 
	    real t_step = 2.0*sn_usrtstep,a_step=0.0;
	    do
	      {
		t_step = t_step/2.0;
		a_step = t_step*sqrt(dx0*dx0+dy0*dy0);
	      }
	    while( a_step > 3.0 );
	    
	    // Euler update 
	    real dx= t_step*dx0;
	    real dy= t_step*dy0;
	    
	    if (debugmode)
	      fprintf(stderr,"Shift %f:%f",dx,dy);
	    
	    // Check for convergence
	    // Make sure snake stays inside image
	    if(checkbounds(j,			     
			   sn_offsetx,sn_gradx->getsizex()+sn_offsetx,
			   sn_offsety,sn_grady->getsizey()+sn_offsety))
	      {
		shiftClosedControl(j,dx,dy);
		real ds=sqrt(dx*dx+dy*dy);
		max_move=Fmax(ds,max_move);
	      }
	  }
	if (debugmode)
	  fprintf(stderr,"\n");
	
	if (max_move<tolerance)
	  return max_move;
      }
  
  return max_move;
}
// --------------------------------------------------------------
PXFloatImage* PXFPBSpline::getenergymap()
{ 
  if (hassnake)
      return sn_energymap;
  else
      return NULL;
}
// --------------------------------------------------------------
unsigned char* PXFPBSpline::getenergytexturemap(float gamma,int numbytes) 
{ 
  if (hassnake)
      return sn_energymap->getslice(gamma,numbytes);
  else
      return NULL;

}
// --------------------------------------------------------------
void PXFPBSpline::removesnake()
{
  if (hassnake)
      {
	if (ownsgradients)
	    {
	      delete sn_gradx;
	      delete sn_grady;
	      ownsgradients=0;
	    }
	if (ownsenergymap)
	    {
	      delete sn_energymap;
	      ownsenergymap=0;
	    }
	hassnake=0;
      }
}
// --------------------------------------------------------------
real PXFPBSpline::calc3potential(PXImage* pimage,PX2DRegion *region,PXFloatImage* external,
				 float w_im,float w_r,float w_ext,
				 int fullimage)
{
  if (debugmode)
    fprintf(stderr,"W_im =%6.2f , w_r=%6.2f w_e=%6.3f\n",w_im,w_r,w_ext);

  real mag,max=0.0;
  sn_sigma=fabs(sn_sigma);
  int sg=int(sn_sigma*2.0+0.5)*2+1;
  
  PXFloatMatrix* kernelx=new PXFloatMatrix(sg,1);
  PXFloatMatrix* kernely=new PXFloatMatrix(1,sg);
  kernelx->GenGradGaussian1Dx(fabs(sn_sigma)/2.0);
  kernely->GenGradGaussian1Dy(fabs(sn_sigma)/2.0);

  PXFloatImage *tempx1=NULL,*tempy1=NULL,*tempx2=NULL,*tempy2=NULL;

  // Find derivatives of energy field by convolving with gaussian 
  // *************************************************************
  int x1=2000,x2=0,y1=2000,y2=0,x,y,xsize=0,ysize=0;

  if (fullimage)
      {
	if (w_im>0.0)
	    {
	      x1=1; y1=1; x2=pimage->getsizex()-1; y2=pimage->getsizey()-1;
	      xsize=pimage->getsizex(); ysize=pimage->getsizey();
	    }
	else if (w_ext>0.0)
	    {
	      x1=1; y1=1; x2=external->getsizex()-1; y2=external->getsizey()-1;
	      xsize=external->getsizex(); ysize=external->getsizey();
	    }
	else if (w_r>0.0)
	    {
	      x1=1; y1=1; x2=region->getoutputimage()->getsizex()-1; y2=region->getoutputimage()->getsizey()-1;
	      xsize=region->getoutputimage()->getsizex(); ysize=region->getoutputimage()->getsizey();
	    }

	if (w_im>0.0)
	    {
	      tempx1 = new PXFloatImage(pimage,kernelx);
	      tempy1 = new PXFloatImage(pimage,kernely);
	    }
	else
	    w_im=0.0;

	if (w_r>0.0)
	    {
	      tempx2 = new PXFloatImage(region->getoutputimage(),kernelx);
	      tempy2 = new PXFloatImage(region->getoutputimage(),kernely);
	    }
	else 
	    w_r=0.0;
	
	if (w_ext<=0.0)
	    w_ext=0.0;
      }
  else
      {
	for (int kk=0;kk<getnumpoints();kk++)
	    {
	      x=(int)getcontrolx(kk);
	      y=(int)getcontroly(kk);
	      if (x<x1) x1=x;
	      if (x>x2) x2=x;
	      if (y<y1) y1=y;
	      if (y>y2) y2=y;
	    }
	
	x1=x1-10; x2=x2+10; y1=y1-10; y2=y2+10;
	
	if (w_im>0.0)
	    {
	      xsize=pimage->getsizex(); ysize=pimage->getsizey();
	    }
	else if (w_ext>0.0)
	    {
	      xsize=external->getsizex(); ysize=external->getsizey();
	    }
	else if (w_r>0.0)
	    {
	      xsize=region->getoutputimage()->getsizex(); ysize=region->getoutputimage()->getsizey();
	    }

	x1=Irange(x1,1,xsize-2);	x2=Irange(x2,x1,xsize-1);
	y1=Irange(y1,1,ysize-2);	y2=Irange(y2,y1,ysize-1);
	if (w_im>0.0)
	    {
	      tempx1 = new PXFloatImage(pimage,kernelx,x1,x2,y1,y2);
	      tempy1 = new PXFloatImage(pimage,kernely,x1,x2,y1,y2);
	    }
	if (w_r>0.0)
	    {
	      tempx2 = new PXFloatImage(region->getoutputimage(),kernelx,x1,x2,y1,y2);
	      tempy2 = new PXFloatImage(region->getoutputimage(),kernely,x1,x2,y1,y2);
	    }
      }

  //  fprintf(stderr,"Energymap %d*%d (%d:%d,%d:%d)\n",xsize,ysize,x1,x2,y1,y2);
  sn_energymap = new PXFloatImage(xsize,ysize);
  float max1=0.1,max2=0.1,max3=0.1;
  for (int k=0;k<sn_energymap->getsizex();k++)
      for(int j=0;j<sn_energymap->getsizey();j++)
	  {
	    if (k==0 || k==sn_energymap->getsizex()-1 || j==0 || j==sn_energymap->getsizey()-1)
		sn_energymap->setpixel(k,j,255.0);
	    else
		sn_energymap->setpixel(k,j,0.0);
	  }


  if (w_im>0.0)
      {
	for (int k=0;k<sn_energymap->getsizex();k++)
	    for(int j=0;j<sn_energymap->getsizey();j++)
		max1=Fmax(max1,pow(tempx1->getpixel(k,j),float(2.0))+pow(tempy1->getpixel(k,j),float(2.0)));
      }
  
  if (w_r>0.0)
      {
	for (int k=0;k<sn_energymap->getsizex();k++)
	    for(int j=0;j<sn_energymap->getsizey();j++)
		max2=Fmax(max2,pow(tempx2->getpixel(k,j),float(2.0))+pow(tempy2->getpixel(k,j),float(2.0)));
      }
  if (w_ext>0.0)
      {
	for (int k=0;k<sn_energymap->getsizex();k++)
	    for(int j=0;j<sn_energymap->getsizey();j++)
		max3=Fmax(max3,external->getpixel(k,j));
      }

  if (w_im>0.0)
      {
	w_r=(w_r/w_im)*max1/max2;
	w_ext=(w_ext/w_im)*max1/max3;
      }
  else
      {
	if (w_r>0.0)
	    w_ext=(w_ext/w_r)*max2/max3;
      }
  

  if (debugmode)
    fprintf(stderr," Weight/Max  Img=%f/%f Reg=%f/%f Prior =%f/%f\n",
	    max1,w_im,max2,w_r,max3,w_ext);

  float sum=w_im+w_r;

  for (int i=x1;i<x2;i++)
      for (int j=y1;j<y2;j++)
	  {
	    float magx=0.0,magy=0.0;
	    if (w_im>0.0)
		{
		  magx+=w_im*tempx1->getpixel(i,j);
		  magy+=w_im*tempy1->getpixel(i,j);
		}
	    if (w_r>0.0)
		{
		  magx+=w_r*tempx2->getpixel(i,j);
		  magy+=w_r*tempy2->getpixel(i,j);
		} 
	    magx/=sum;
	    magy/=sum;
	    mag=sqrt(magx*magx+magy*magy);
	    if (w_ext>0.0)
		mag+=w_ext*external->getpixel(i,j);
	    
	    sn_energymap->setpixel(i,j,mag);
	    if (max<mag) max=mag;
	  }
  
  
  sn_gradx = new PXFloatImage(sn_energymap,kernelx,x1,x2,y1,y2);
  sn_grady = new PXFloatImage(sn_energymap,kernely,x1,x2,y1,y2);
  
  
  if (w_im>0.0)
      {
	delete tempx1;  delete tempy1;
      }
  if (w_r>0.0)
      {
	delete tempx2;  delete tempy2;
      }

  delete kernelx;
  delete kernely;
  ownsgradients=1;
  return max;
}
// --------------------------------------------------------------
real PXFPBSpline::calcpotential(PXImage* pimage,int fullimage)
{
  real mag,max=0.0;
  sn_sigma=fabs(sn_sigma);
  int sg=int(sn_sigma*2.0+0.5)*2+1;

  
  PXFloatMatrix* kernelx=new PXFloatMatrix(sg,1);
  PXFloatMatrix* kernely=new PXFloatMatrix(1,sg);
  kernelx->GenGradGaussian1Dx(fabs(sn_sigma)/2.0);
  kernely->GenGradGaussian1Dy(fabs(sn_sigma)/2.0);

  PXFloatImage *tempx,*tempy;

  // Find derivatives of energy field by convolving with gaussian st.dev 1.0
  // ***********************************************************************
  int x1=2000,x2=0,y1=2000,y2=0,x,y;

  if (fullimage)
      {
	x1=1; y1=1; x2=pimage->getsizex()-1; y2=pimage->getsizey()-1;
	tempx = new PXFloatImage(pimage,kernelx);
	tempy = new PXFloatImage(pimage,kernely);
      }
  else
      {
	for (int kk=0;kk<getnumpoints();kk++)
	    {
	      x=(int)getcontrolx(kk);
	      y=(int)getcontroly(kk);
	      if (x<x1) x1=x;
	      if (x>x2) x2=x;
	      if (y<y1) y1=y;
	      if (y>y2) y2=y;
	    }
	
	x1=x1-15; x2=x2+15; y1=y1-15; y2=y2+15;
	if (x1<1) x1=1; 
	if (y1<1) y1=1;
	if (x2>=pimage->getsizex()-1) x2=pimage->getsizex()-1;
	if (y2>=pimage->getsizey()-1) y2=pimage->getsizey()-1;
	tempx = new PXFloatImage(pimage,kernelx,x1,x2,y1,y2);
	tempy = new PXFloatImage(pimage,kernely,x1,x2,y1,y2);
      }
  sn_energymap = new PXFloatImage(pimage->getsizex(),pimage->getsizey());
  for (int k=0;k<sn_energymap->getsizex();k++)
      for(int j=0;j<sn_energymap->getsizey();j++)
	{
	  if (k==0 || k==sn_energymap->getsizex()-1 || j==0 || j==sn_energymap->getsizey()-1)
	    sn_energymap->setpixel(k,j,255.0);
	  else
	    sn_energymap->setpixel(k,j,0.0);
	}
  
  for (int i=x1;i<x2;i++)
    for (int j=y1;j<y2;j++)
      {
	float magx=tempx->getpixel(i,j);
	float magy=tempy->getpixel(i,j);
	mag=sqrt(magx*magx+magy*magy);
	sn_energymap->setpixel(i,j,mag);
	if (max<mag) max=mag;
      }
  delete tempx;
  delete tempy;
   
  sn_gradx = new PXFloatImage(sn_energymap,kernelx,x1,x2,y1,y2);
  sn_grady = new PXFloatImage(sn_energymap,kernely,x1,x2,y1,y2);

  delete kernelx;
  delete kernely;
  ownsgradients=1;
  return max;

}
// --------------------------------------------------------------

int  PXFPBSpline::checkbounds(int j,int x0,int x1,int y0,int y1,int tol)
{
  j=indexbound(j,int(numpoints));
  if (getcontrolx(j)<real(x0+tol)) 
      return 0;

  if (getcontrolx(j)>real(x1-tol)) 
      return 0;
			  
  if (getcontroly(j)<real(y0+tol))
      return 0;

  if (getcontroly(j)>real(y1-tol)) 
      return 0;

  return 1;
}
// --------------------------------------------------------------

real PXFPBSpline::splineinternalenergy(int k1,int k2)
{
  static int firsttime=0;


  if (firsttime==0)
      {
	//fprintf(stderr,"Using proper internal energy\n");
	firsttime=1;
      }
  real energy = 0.0;
    
  if (k1<0) k1=0;
  if (k2<0) k2=int(getnumpoints()-1);

  if (k1<k2)   {	int k3=k2; k2=k1; k1=k3;      }

  real t0=getknot(k1+int(degree)-1);
  real t1=getknot(k2+int(degree)+1);
  real dt=(t1-t0)/50.0;

  for (real s=t0+dt;s<t1-dt;s=s+2.0*dt)
      {
	real s1=s;
	if (s1<0)
	    s1=s1+1.0;
	else if(s1>=1.0)
	    s1=s1-1.0;
	real x2,y2;
	get2derivative(s1,x2,y2);
	//	real ds=getlength(s1-dt,s1+dt,0.25*dt);
	real kappa = sqrt(x2*x2 + y2*y2);
	/*	if (debugmode)
	    fprintf(stderr," At %f (%f,%f) (%f,%f) \n",s,x2,y2,ds,kappa);*/

	energy = energy+kappa;
      }
  /*if (debugmode)
      fprintf(stderr,"\n");*/

  return(energy);
}
// --------------------------------------------------------------
void PXFPBSpline::splinechangeinternalenergy(int j,float dr,real &sx,real& sy)
{
  // Control Distance 
  // ----------------
  /*  {
    int jm=indexcyclic(j-1,numpoints);
    int jp=indexcyclic(j-1,numpoints);
    
    real xm=getcontrolx(jm);
    real xp=getcontrolx(jp);
    real x =getcontroly(j);
    real ym=getcontroly(jm);
    real yp=getcontroly(jp);
    real y =getcontroly(j);
    
    real ex1 = fabs(2.0*(x+dr)-xm-xp);
    real ex2 = fabs(2.0*(x-dr)-xm-xp);
    sx = (ex1-ex2)/(2.0*dr);
    
    real ey1 = fabs(2.0*(y+dr)-ym-yp);
    real ey2 = fabs(2.0*(y-dr)-ym-yp);
    sy = (ey1-ey2)/(2.0*dr);
  } */
  //Curvature Regularization 
  shiftClosedControl(j,dr,0.0);
  real ex1 = splineinternalenergy(j,j);
  shiftClosedControl(j,-2.0*dr,0.0);
  real ex2 = splineinternalenergy(j,j);
  shiftClosedControl(j,dr,0.0);
  sx += (ex1-ex2)/(2.0*dr);
  
  // y-direction
  shiftClosedControl(j,0.0,dr);
  real ey1 = splineinternalenergy(j,j);
  shiftClosedControl(j,0.0,-2.0*dr);
  real ey2 = splineinternalenergy(j,j);
  shiftClosedControl(j,0.0,dr);
  sy += (ey1-ey2)/(2.0*dr);

}
// --------------------------------------------------------------
void PXFPBSpline::controlexternalforce(real& fx,real& fy,int k)
{
  fx = 0.0;
  fy = 0.0;
  
  real t0=getknot(k+int(degree)-1);
  real t1=getknot(k+int(degree)+2);
  real dt=(t1-t0)/10.0;
  for (real s=t0+dt;s<t1-dt;s=s+2.0*dt)
    {
      real x,y;
      real s1=s;
      if (s1<0)
	s1=s1+1.0;
      else if(s1>=1.0)
	s1=s1-1.0;
      
      getposition(s1,x,y);
      real ds=getlength(s1-dt,s1+dt,0.5*dt);
      real wgt=evalweight(s1,k);
      
      fx+=sn_gradx->getinterp(x-float(sn_offsetx),y-float(sn_offsety))*ds*wgt/sn_maxgrad;
      fy+=sn_grady->getinterp(x-float(sn_offsetx),y-float(sn_offsety))*ds*wgt/sn_maxgrad;
    }
}
// --------------------------------------------------------------------------
void PXFPBSpline::controlbaloonforce(real& fx,real& fy,int k)
{
  fx = 0.0;
  fy = 0.0;
  
  real t0=getknot(k+int(degree)-1);
  real t1=getknot(k+int(degree)+2);
  real dt=(t1-t0)/10.0;
  for (real s=t0+dt;s<t1-dt;s=s+2.0*dt)
    {
      real x,y;
      real s1=s;
      if (s1<0)
	s1=s1+1.0;
      else if(s1>=1.0)
	s1=s1-1.0;
      
      getderivative(s1,x,y);
      real ds=getlength(s1-dt,s1+dt,0.5*dt);
      real wgt=evalweight(s1,k);
      
      fx+=y*ds*wgt/sn_maxgrad;
      fy+=(-x)*ds*wgt/sn_maxgrad;
    }
}
// --------------------------------------------------------------------------
int PXFPBSpline::genmatrices(int msize)
{
  if (hasmatrices)
      free_dmatrix(invMatrix,1,matsize,1,matsize);

  if (msize<0)
      matsize=int(numpoints);
  else
      matsize=msize;

  invMatrix=dmatrix(1,matsize,1,matsize);
  hasmatrices=1;

  if (debugmode>1)
      fprintf(stderr, "Generating Matrix : %d*%d\n", matsize,matsize);

  for (int ia=1;ia<=matsize;ia++)
      for (int ib=1;ib<=matsize;ib++)
	{
	  invMatrix[ia][ib]=float(ia==ib);
	}
  
  real h[10];
  float* a=vector(1,matsize);
  float* b=vector(1,matsize);
  float* c=vector(1,matsize);
  
  if (isclosed())
      {
	for (int i=0;i<matsize;i++)
	  {
	    real arg=getcknot(i);
	    //int row=indexcyclic(i+1,matsize,1);
	    integer l=evalbspl(arg,h);
	    a[i+1]=evalweight(h,l,i-1);	
	    b[i+1]=evalweight(h,l,i);	
	    c[i+1]=evalweight(h,l,i+1);	
	  }
      }
  else
    {
      a[1]=0.0;       b[1]=1.0;        c[1]=0.0;
      a[matsize]=0.0; b[matsize]=1.0;  c[matsize]=0.0;
      for (int i=2;i<matsize;i++)
	{
	  real arg=getcknot(i);
	  integer l=evalbspl(arg,h);
	  a[i+1]=evalweight(h,l,i-1);	
	  b[i+1]=evalweight(h,l,i);	
	  c[i+1]=evalweight(h,l,i+1);	
	}
    }
  
  // Next shift a ....
  float aold=a[1];
  for (int i=1;i<matsize;i++)
    a[i]=a[i+1];
  a[matsize]=aold;

  // get the inverse column by column 
  float* r=vector(1,matsize);
  float* x=vector(1,matsize);
  
  for(int col=1;col<=matsize;col++)
      {
	for(int i=1;i<=matsize;i++) 
	    r[i]=0.0;
	r[col] = 1.0;
	
	// call the cylic tridiagonal routine 
	//	cyclictridiag(a,b,c,alpha,beta,r,x,matsize);
	solve_cyc_tridiag_nonsym_gsl(b,c,a,r,x,matsize);
	
	// store in back into the inverse matrix 
	for(int j=1;j<=matsize;j++)
	    invMatrix[j][col]=x[j];
      }

  if (debugmode>1)
    {
      int np=matsize;
      printdmatrix(invMatrix,np,np,"%6.3f ","Inv",12);
    }


  free_vector(x,1,matsize);
  free_vector(r,1,matsize);
  free_vector(c,1,matsize);
  free_vector(b,1,matsize);
  free_vector(a,1,matsize);
      
  return 1;
}
// --------------------------------------------------------------------------
integer PXFPBSpline::evalbspl(real x,real* h,integer l)
{
  real arg=x;
  if (l<0)
      {
	l=0;
	while (!(getknot(int(l))<=arg && getknot(int(l)+1)>arg))
	    l++;
	l=l+1;
      }
  fpbspl_(knots,&numknots,&degree,&x,&l,h);
  return l;
}
// --------------------------------------------------------------------------
real PXFPBSpline::evalweight(real* h,integer l, integer index)
{
  integer shift=5;
  if (degree==1)
      shift=2;

  int m=int(index-l+shift);
  if (m>=0 && m<=4)
      return h[m];
  else	
      if (isclosed())
	  {
	    if (m>0)
		index=index-getnumpoints();
	    else
		index=index+getnumpoints();
	    
	    m=int(index-l+shift);
	    if (m>=0 && m<=4)
		return h[m];
	  }
  return 0.0;
}
// --------------------------------------------------------------------------
real PXFPBSpline::evalweight(real s, integer index)
{
  real h[5];
  integer l=evalbspl(s,h);
  return evalweight(h,l,index);
}
// --------------------------------------------------------------------------
real PXFPBSpline::findnearestknot(int& index,real& mindist,
				  real xn,real yn,real zn)
{
  mindist=10000000.0;
  index=0;
  for (int i=0;i<getnumpoints();i++)
      {
	float s=getcknot(i);
	real x,y,z;
	getposition(s,x,y,z);
	real dist=(pow(x-xn,float(2.0))+pow(y-yn,float(2.0)));
	if (dimension==3)
	  {
	    dist+=pow(z-zn,float(2.0));
	  }
	if (dist<mindist)
	  {
	    mindist=dist;
	    index=i;
	  }
      }
  mindist=sqrt(mindist);
  return getcknot(index);
}
// --------------------------------------------------------------------------
int PXFPBSpline::insertknot(real xn,real yn,real& mindist)
{
  if (mindist<0.01)
    mindist=0.01;

  real testdist=0.0;
  if (debugmode)
    fprintf(stderr,"Input mindist=%f (%f,%f)\n",mindist,xn,yn);
  real s=findnearestneighbour(xn,yn,mindist,0);

  int proceed=1;
  if (mindist<testdist)
    proceed=0;

  if (debugmode)
    fprintf(stderr,"Nearest Neighbor to %f,%f=%f (testdist=%f,mindist=%f) Proceed=%d\n",xn,yn,s,testdist,mindist,proceed);

  if (proceed==0)
    return 0;

  int knot_index=0;
  int flag=0;

  while (knot_index<numknots && flag==0)
    {
      if ( (knots[knot_index]<= s) && ( knots[knot_index+1] >s ))
	{
	  s=0.5*(knots[knot_index]+knots[knot_index+1]);
	  flag=1;
	}
      else
	++knot_index;
    }


  this->setclosed(1);
  PXFPBSpline* new_spl=new PXFPBSpline();
  new_spl->setclosed(1);
  new_spl->setellipse(numpoints+1,10.0,10.0,0.0,0.0,2);

  real* xp=new real[numpoints+1];
  real* yp=new real[numpoints+1];
  float z=zlevel;

  int k=0;
  real x0,y0;
  for (int i=degree;i<numpoints+degree;i++)
    {
      getposition(knots[i],x0,y0);
      xp[k]=x0;
      yp[k]=y0;
      new_spl->setknot(k+degree,knots[i]);
      ++k;

      if (i==knot_index)
	{
	  getposition(s,x0,y0);
	  xp[k]=x0;
	  yp[k]=y0;
	  new_spl->setknot(k+degree,s);
	  ++k;
	}
    }

  for (int ia=0;ia<degree;ia++)
    {
      new_spl->setknot(ia,new_spl->getknot(ia+numpoints+1)-1.0);
      new_spl->setknot(ia+numpoints+degree+1,new_spl->getknot(ia+degree)+1.0);
    }
  new_spl->ResetPoints(this->numpoints+1,xp,yp);
  this->copypoints(new_spl);
  this->zlevel=z;
  delete [] xp;
  delete [] yp;
  delete new_spl;


  return 1;
}
// --------------------------------------------------------------------------

void PXFPBSpline::getcentroid(real& x,real&y, real& z,float ds)
{
  float count=0.0;
  if (numpoints<4)
    return;

  x=0.0;y=0.0;z=0.0;
  real xt,yt,zt;
  if (dimension==2)
      {
	for(float s=0.0;s<1.0;s+=ds)
	  {
	    count=count+1.0;
	    getposition(s,xt,yt);
	    x+=xt;
	    y+=yt;
	  }
	x=x/count;
	y=y/count;
	z=zlevel;
      }
  else
    {
      for (float s=0.0;s<1.0;s+=ds)
	{
	  count=count+1.0;
	  getposition(s,xt,yt,zt);
	  x+=xt;
	  y+=yt;
	  z+=zt;
	}
    }
}
// --------------------------------------------------------------------------
void PXFPBSpline::rigid2D(float tx,float ty,float xc,float yc,float angle,int reverse)
{
  if (dimension!=2)
    return;
  
  if (fabs(angle)>0.2)
    {
	angle*=M_PI/180.0;
	for(int i=0;i<getnumcontrolpoints();i++)
	    {
	      if (reverse==0)
		  {
		    // Eliminate center and translate 
		    real x=getcontrolx(i)-xc+tx;
		    real y=getcontroly(i)-yc+ty;
		    // Rotate about center 
		    real xn=x*cos(angle)-y*sin(angle);
		    real yn=x*sin(angle)+y*cos(angle);
		    // Add center 
		    setcontrol(i,xn+xc,yn+yc);
		  }
	      else
		  {
		    real x=getcontrolx(i)-xc;
		    real y=getcontroly(i)-yc;
		    // Rotate about center 
		    real xn=x*cos(angle)-y*sin(angle);
		    real yn=x*sin(angle)+y*cos(angle);
		    // Add center and then translation 
		    setcontrol(i,xn+xc+tx,yn+yc+ty);
		  }
	    }
      }
  else
      {
	for(int i=0;i<getnumcontrolpoints();i++)
	    shiftcontrol(i,tx,ty);
      }
  }
// --------------------------------------------------------------------------
void PXFPBSpline::rigid2D(float tx,float ty,float angle)
{
  if (dimension!=2)
      return;

  if (fabs(angle)>0.2)
      {
	angle=angle*M_PI/180.0;
	real xc,yc,zc;
	getcentroid(xc,yc,zc,0.02);
	tx=tx+xc;
	ty=ty+yc;
	for(int i=0;i<getnumcontrolpoints();i++)
	    {
	      real x=getcontrolx(i)-xc;
	      real y=getcontroly(i)-yc;
	      setcontrol(i,x*cos(angle)-y*sin(angle)+tx,
			 x*sin(angle)+y*cos(angle)+ty);
	    }
      }
  else
      for(int i=0;i<getnumcontrolpoints();i++)
	  shiftcontrol(i,tx,ty);
  
}

// --------------------------------------------------------------------------
void PXFPBSpline::scaleshift2D(float tx,float ty,float sx,float sy)
{
  if (fabs(sx)<0.01)
      sx=1.0;
  if (fabs(sy)<0.01)
      sy=1.0;
      
  if (sx==1.0 && sy==1.0 && tx==0.0 && ty==0.0)
    return;

  for(int i=0;i<getnumcontrolpoints();i++)
    {
      real x=getcontrolx(i)*sx+tx;
      real y=getcontroly(i)*sy+ty;
      setcontrol(i,x,y);
    }
}

// --------------------------------------------------------------------------
int PXFPBSpline::ensureAnticlockwise()
{
  int flag=0;
  if (dimension==2)
      {
	real x0,x1,x2,y0,y1,y2;
	getposition(0.0,x0,y0);
	getposition(0.33,x1,y1);
	getposition(0.66,x2,y2);
	real v1x=x1-x0;
	real v1y=y1-y0;
	real v2x=x2-x1;
	real v2y=y2-y1;
	real zz=(v1x*v2y-v2x*v1y);
	
	if (zz<0.0) 
	    {
	      real* oldknots=new real[numknots];
	      real* oldcontrol=new real[numknots*dimension];
	      
	      int i;
	      for (i=0;i<numknots;i++)
		  oldknots[i]=knots[i];
	      for (i=0;i<numknots*dimension;i++)
		  oldcontrol[i]=controlpoints[i];

	      int numk=int(getnumknots()-1);
	      int numc=int(getnumcontrolpoints()-1);
	      for (i=0;i<=numk;i++)
		  setknot(i,1.0-oldknots[numk-i]);
	      for (i=0;i<=numc;i++)
		  {
		    int k=numc-i;
		    for(int j=0;j<dimension;j++)
			{
			  controlpoints[i+j*numknots]=oldcontrol[k+j*numknots];
			}
		  }
	      delete [] oldknots;
	      delete [] oldcontrol;
	      flag=1;
	    }
      }
  return flag;
}
// --------------------------------------------------------------------------
real PXFPBSpline::findIntersectY(real y,real minx,real maxx,
				 real ds,real tol,int maxatt)
{
  int found=0;
  real xintersect=-1111.0;
  float s0=0.0;
  while(s0<=(1.0-ds) && found==0)
      {
	real x1,y1,x2,y2;
	getposition(s0,x1,y1);
	getposition(s0+ds,x2,y2);
	if ((y-y1)*(y-y2)<=0.0)
	    {     
	      real xp,yp;
	      int att=0;
	      float stest=0.0;
	      real smin=s0;
	      real smax=s0+ds;
	      do
		  {
		    att++;
		    stest=(smin+smax)*0.5;
		    getposition(stest,xp,yp);
		    if ((y-y1)*(y-yp)<=0.0)
			smax=stest;
		    else
			smin=stest;
		    getposition(stest,xp,yp);
		  }
	      while (att<maxatt && fabs(yp-y)>tol);
			  
	      getposition(stest,xp,yp);
	      if (xp>=minx && xp<=maxx)
		  {
		    found=1;
		    xintersect=xp;
		  }
	    }
	s0+=ds;
      }
  return xintersect;
}
// --------------------------------------------------------------------------
int PXFPBSpline::findIntersectX(real x,real& first,real& second,real ds,real tol,int maxatt)
{
  real slevels[20];
  for (int pass=0;pass<=20;pass++)
      slevels[pass]=-1111.0;

  int found=0;
  float s0=0.0;
  while(s0<=(1.0-ds))
      {
	real x1,y1,x2,y2;
	getposition(s0,x1,y1);
	getposition(s0+ds,x2,y2);
	if ((x-x1)*(x-x2)<=0.0)
	    {     
	      real xp,yp;
	      int att=0;
	      float stest=0.0;
	      real smin=s0;
	      real smax=s0+ds;
	      do
		  {
		    att++;
		    stest=(smin+smax)*0.5;
		    getposition(stest,xp,yp);
		    if ((x-x1)*(x-xp)<=0.0)
			smax=stest;
		    else
			smin=stest;
		    getposition(stest,xp,yp);
		  }
	      while (att<maxatt && fabs(xp-x)>tol);
	      
	      getposition(stest,xp,yp);
	      found++;
	      slevels[found]=stest;
	    }
	s0+=ds;
      }
  if (found==0)
      {
	first=0.0;
	second=0.5;
      }
  else if (found==1)
      {
	first=slevels[0];
	second=slevels[0]+0.5;
	if (second>1.0)
	    second-=1.0;
      }
  else if (found==2)
      {
	first=slevels[0];
	second=slevels[1];
      }
  else
      {
	first=slevels[0];
	second=slevels[found/2];
      }
  return found;
}
// --------------------------------------------------------------------------
real PXFPBSpline::findnearestneighbour(real& xn,real& yn,real& mindist,int dbg)
{
  float bests=0.0;
  real x,y;   getposition(0.0,x,y);
  real bestdist=(pow(x-xn,float(2.0))+pow(y-yn,float(2.0)));


  for (real s=0;s<1.0;s=s+0.02)
    {
      real x,y;    
      getposition(s,x,y);
      real dist=(pow(x-xn,float(2.0))+pow(y-yn,float(2.0)));
      if (dist<bestdist)
	{
	  bestdist=dist;
	  bests=s;
	}
    }
  
  mindist=sqrt(bestdist);
  getposition(bests,xn,yn);
  return bests;
}
// ------------------------------------------------------------------
void PXFPBSpline::ResetPoints(int np,real* xp,real* yp)
{
  if (dimension!=2)
      return;

  if (np!=numpoints || !isclosed())
    {
      setclosed(1);
      setellipse(np,10.0,10.0,0.0,0.0,2);
    }

  if (!hasmatrices)
    genmatrices(np);
  
  for (int ia=0;ia<np;ia++)
    {
      real xc=0.0,yc=0.0;
      for (int ib=0;ib<np;ib++)
	{
	  xc=xc+invMatrix[ia+1][ib+1]*xp[ib];
	  yc=yc+invMatrix[ia+1][ib+1]*yp[ib];
	}
      setClosedControl(ia,xc,yc);
    }
}
// ------------------------------------------------------------------
void PXFPBSpline::ResetPoints(int np,real* xp,real* yp,real* zp)
{
  if (dimension!=3)
      return;

  if (np!=numpoints || !isclosed())
      {
	setclosed(1);
	setellipse(np,10.0,10.0,0.0,0.0,3);
      }
  
  if (!hasmatrices)
    genmatrices(np);
  
  for (int ia=0;ia<np;ia++)
    {
      real xc=0.0,yc=0.0,zc=0.0;
      for (int ib=0;ib<np;ib++)
	{
	  xc=xc+invMatrix[ia+1][ib+1]*xp[ib];
	  yc=yc+invMatrix[ia+1][ib+1]*yp[ib];
	  zc=zc+invMatrix[ia+1][ib+1]*zp[ib];
	}
      setClosedControl(ia,xc,yc,zc);
    }
}
// ------------------------------------------------------------------

