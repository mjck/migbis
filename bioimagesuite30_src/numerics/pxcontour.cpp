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
// Calls ludcmp to compute matrrix linear system solution
//
//---------------------------------------------------------------------------------------------------------

#include "pxcontour.h"
#include "pxutil.h"
#include "nr.h"
#include "nrutil.h"
// Defining namespace for old code


//-----------------------------------------------------------------------
float PXContour::Break_const = 11.111;
//-----------------------------------------------------------------------

PXContour::PXContour()
{
  status=1;
  numpoints=0;
  closed=0;
  isolated=0;
  lastpoint=new PXContourPoint;
  lastpoint->next=NULL;
  lastpoint->previous=NULL;
  realnextpoint=NULL;
  toppoint=lastpoint;
  gradient=0;
  contourhascache=0;
  px=NULL;py=NULL;
}

// --------------------------------------------------------------------------
PXContour::PXContour(PXContour *oldcontour)
{
  status=1;
  numpoints=0;
  closed=0;
  isolated=0;
  lastpoint=new PXContourPoint;
  lastpoint->next=NULL;
  lastpoint->previous=NULL;
  realnextpoint=NULL;
  toppoint=lastpoint;
  gradient=0;
  contourhascache=0;
  px=NULL;py=NULL;
  PXContourPoint* temp=oldcontour->gettoppoint();
  for (int i=0;i<oldcontour->getnumpoints();i++)
    {
      Add(temp->x,temp->y);
      temp=temp->next;
    }
}
// --------------------------------------------------------------------------
PXContour::~PXContour()
{
  PXContourPoint* pts=gettoppoint();
  /*fprintf(stderr,"\n pts\t pts->p\t pts->n\t in\t np\t x\t y\n"); 
    int index=0;*/
  while (pts!=NULL)
    {
      /*fprintf(stderr," %9d \t %9d \t %9d \t %5d \t %5d \t %6.2f \t %6.2f \n",
	      pts,pts->previous,pts->next,index,(index<np),pts->x,pts->y);
	      ++index;*/
      PXContourPoint* old=pts;
      if (pts==lastpoint)
	pts=realnextpoint;
      else
	pts=pts->next;
      delete old;
    }
  return;
}
// --------------------------------------------------------------------------
void PXContour::RemovePoint()
{
  if (numpoints==0)
    return;

  lastpoint->next=realnextpoint;
  realnextpoint=lastpoint;
  
  lastpoint=lastpoint->previous;
  lastpoint->next=NULL;
  numpoints=numpoints-1;
  RemoveGrad();

  /*int rn=-1;
  if (realnextpoint!=NULL)
    rn=(int)realnextpoint->next;
  int pt=-1;
  if (lastpoint->previous!=NULL)
    pt=(int)lastpoint->previous->next;

  fprintf(stderr,"Removed (lastpoint=%d, lastpoint->next=%d l->p->n=%d, realnextpoint=%d (r->n=%d) toppoint=%d)\n",
  lastpoint,lastpoint->next,pt,realnextpoint,rn,toppoint);*/

  
  closed=0;
}
// --------------------------------------------------------------------------
void PXContour::DeleteAll()
{
  if (numpoints<1)
    return;

  /*  fprintf(stderr,"DeleteA  (lastpoint=%d, lastpoint->next=%d , realnextpoint=%d)\n",
      lastpoint,lastpoint->next,realnextpoint);*/
  
  lastpoint->next=realnextpoint;
  realnextpoint=toppoint->next;
  if (realnextpoint!=NULL)
    realnextpoint->previous=toppoint;
  toppoint->next=NULL;
  lastpoint=toppoint;
  
  DeleteGrad();
  closed=0;
  numpoints=0;
  /*  fprintf(stderr,"\t Done  (lastpoint=%d, lastpoint->next=%d , realnextpoint=%d toppoint=%d)\n",
      lastpoint,lastpoint->next,realnextpoint,toppoint);*/

}

// --------------------------------------------------------------------------
void PXContour::Add(float xx,float yy)
{
  lastpoint->x=xx;
  lastpoint->y=yy;
  if (realnextpoint==NULL)
    {
      lastpoint->next=new PXContourPoint;
      (lastpoint->next)->previous=lastpoint;
      (lastpoint->next->next)=NULL;
      lastpoint=lastpoint->next;
      lastpoint->x=-1.0;
      lastpoint->y=-1.0;
    }
  else
    {
      lastpoint->next=realnextpoint;
      lastpoint=realnextpoint;
      realnextpoint=realnextpoint->next;
      if (realnextpoint!=NULL)
	realnextpoint->previous=lastpoint;
      lastpoint->next=NULL;
    }
  numpoints++;
  DeleteGrad();
  closed=0;

  /*int pt=-1;
  if (lastpoint->previous!=NULL)
    pt=(int)lastpoint->previous->next;

  fprintf(stderr,"Added  (lastpoint=%d, lastpoint->next=%d l->p->n=%d, realnextpoint=%d toppoint=%d) %.1f %.1f\n",
    lastpoint,lastpoint->next,pt,realnextpoint,toppoint,xx,yy);*/

}

// --------------------------------------------------------------------------

void PXContour::AddP(unsigned long xx,unsigned long yy)
{
  Add(float(xx),float(yy));
}

// --------------------------------------------------------------------------
void PXContour::copypoints(PXContour *oldcontour)
{
  DeleteAll();
  PXContourPoint* temp=oldcontour->gettoppoint(); 
  for (int i=0;i<oldcontour->getnumpoints();i++)
      {
	Add(temp->x,temp->y);
	temp=temp->next;
      }

}
// --------------------------------------------------------------------------
void PXContour::setcircle(int np,float radius,float x,float y)
{
  DeleteAll();
  for (int j=1;j<=np;j++)
      {
	float t=3.141592*2.0*(float(j-1)/float(np));
	float xx=radius*cos(t)+x;
	float yy=radius*sin(t)+y;
	Add(xx,yy);
      }
  closed=1;
}
// --------------------------------------------------------------------------
void PXContour::AddGrad(float xx,float yy,float kk)
{
  lastgradient->x=xx;
  lastgradient->y=yy;
  lastgradient->k=kk;
  lastgradient->next=new PXContourGrad;
  (lastgradient->next)->previous=lastgradient;
  lastgradient=lastgradient->next;
  lastgradient->next=NULL;
}

// --------------------------------------------------------------------------

void PXContour::DeleteGrad()
{
  cleanupCache();
  if (gradient>0)
    {
      while (lastgradient->previous !=NULL)
	{
	  lastgradient=lastgradient->previous;
	  if (lastgradient->next!=NULL)
	    {
	      delete lastgradient->next;
	      lastgradient->next=NULL;
	    }
	}
      delete topgradient;
      gradient=0;
    }

}
// --------------------------------------------------------------------------


void PXContour::RemoveGrad()
{
  cleanupCache();
  if (gradient>0)
    {
      if (lastgradient->previous !=NULL)
	{
	  lastgradient=lastgradient->previous;
	  delete lastgradient->next;
	  lastgradient->next=NULL;
	}
    }
  gradient=0;
}

// --------------------------------------------------------------------------

int PXContour::Load(const char* fname)
{
  FILE* fin;
  char line[200];
  status=0;
  fin=fopen(fname,"r");
  if (fin!=NULL)
    {
      numpoints=0;
      while ( fgets(line,100,fin)!=NULL ) 
	  numpoints++;
      fclose(fin);

      if (numpoints<1)
	  {
	    status=0;
	    return(status);
	  }

      fin=fopen(fname,"r");
      float x,y;
      fscanf(fin,"%f %f", &x,&y);

      if (fabs(x)>10000.0 || fabs(y) > 10000)
	  {
	    status=0;
	    return(0);
	  }
      else
	{
	  DeleteAll();
	  Add(x,y);
	}

      
      for(int i=1;i<numpoints;i++)
	{
	  fscanf(fin,"%f %f",&x,&y);
	  if (fabs(x)>10000.0 || fabs(y) > 10000)
	      {
		status=0;
		DeleteAll();
		return(0);
	      }
	  Add(x,y);
	}
      fclose(fin);
      status=1;
      return(1);
    }
  status=0;
  return(0);
}

// -----------------------------------------------------------------
int PXContour::Save(const char* fname)
{
  FILE* fout;
  fout=fopen(fname,"w");
  if (fout!=NULL)
    {
      PXContourPoint* pts=gettoppoint();
      int np=getnumpoints();
      for (int ib=1;ib<=np;ib++)
	{
	  fprintf(fout,"%6.2f  %6.2f \n",pts->x,pts->y);
	  pts=pts->next;
	}
      fclose(fout);
      return(1);
    }
  return(0);
}
// -----------------------------------------------------------------
void PXContour::SaveStream(FILE* fout,int in2d,int opencurve,float z)
{
  PXContourPoint* temp=gettoppoint();
  int np=getnumpoints();
  for (int ib=1;ib<=np;ib++)
      {
	if (in2d)
	    fprintf(fout,"%6.2f %6.2f \n",temp->x,temp->y);
	else
	    fprintf(fout,"%12.8f %12.8f %12.8f\n",temp->x,temp->y,z);
	temp=temp->next;
      }
  if (!opencurve && isclosed())
      {
	temp=gettoppoint();
	/*if (in2d)
	  fprintf(fout,"%6.2f %6.2f \n",temp->x,temp->y);
	  else
	  fprintf(fout,"%12.8f %12.8f %12.8f\n",temp->x,temp->y,z);*/
      }	
}
// -----------------------------------------------------------------
int PXContour::SaveIGES(const char* filename,float z)
{
  int flag=0;
  FILE* fout=fopen(filename,"w");
  if (fout!=NULL)
      {
	// IGES Start Section
	// ------------------
	fprintf(fout,"%72sS      1\n",filename);
	fprintf(fout,",,,,,,32,38,6,308,15,,1.0D0,1,2HIN,1,0.0D0,13H960202.170136,            G      1\n"
	        "2.22044604925031D-16,0.0D0,,,9,0,13H960202.170136;                      G      2\n");
	for (int pass=0;pass<2;pass++)
	    {
	      int index=0;
	      PXContourPoint* temp=gettoppoint();
	      for (int ib=0;ib<getnumpoints();ib++)
		  {
		    index++;
		    switch(pass)
			{
			case 0:
			  fprintf(fout,"%8d%8d%8d%8d",116,index,2,2);
			  fprintf(fout,"%8d%8d%8d%8d%08dD%7d\n",0,0,
				  0,0,0,1);
			  fprintf(fout,"%8d%8d%8d%8d",116,0,0,1);
			  fprintf(fout,"%8d%8d%8d%8s%8dD%7d\n",0,0,0,
				  "POINT",index,2);
			  break;
			case 1:
			  fprintf(fout,"116, %15.4e, %15.4e, %15.4f;          %7dP%7d\n",
				  temp->x,temp->y,z,(index*2)-1,index);
			  temp=temp->next;
			  break;
			}
		  }
	      fprintf(fout,"S%7dG%7dD%7dP%7d                                        T%7d\n",
		      1,2,index*2,index,1);
	    }
	flag=1;
      }
  fclose(fout);
  return flag;
}
// -----------------------------------------------------------------
int PXContour::SaveInteger(const char* fname,int sc)
{
  FILE* fout=fopen(fname,"w");
  int xx,yy;
  if (fout!=NULL)
    {
       // fout << numpoints << "\n";
      PXContourPoint* pts=toppoint;
      while (pts->next !=NULL)
	{
	    xx=int(pts->x*sc+0.5);
	    yy=int(pts->y*sc+0.5);
	    fprintf(fout,"%5d  %5d \n",xx,yy);
	    pts=pts->next;
	}
      fclose(fout);
      return(1);
    }
  return(0);
}

// -----------------------------------------------------------------

int PXContour::Save4Mesh(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (fout!=NULL)
    {
      PXContourPoint* pts=toppoint;
      while ((pts->next)->next !=NULL)
	{
	  fprintf(fout,"%d %d %d %d \n", 
		  (int)pts->x,(int)pts->y,
		  (int)pts->next->x,(int)pts->next->y);
	  pts=pts->next;
	}

      // Ensure Closed Curve !! ;
      
      fprintf(fout,"%d %d %d %d \n",
	      (int)pts->x,(int)pts->y,
	      (int)toppoint->x,(int)toppoint->y);
      fclose(fout);
      return(1);
    }
  return(0);
}

// --------------------------------------------------------------------------
int PXContour::getcardinal(int ii,int np,unsigned char cl)
{
  if (cl>0)
    return (ii % np)*(ii>=0)+ (ii+np)*(ii<0);
  else
    return ii*(ii>=0)*(ii<np)+(np-1)*(ii>=np);
}

// --------------------------------------------------------------------------

void  PXContour::copy2vector(double* x,double *y,int& np)
{
  PXContourPoint *pts=toppoint;
  np=0;
  while(pts->next !=NULL)
    { 
      x[np]=pts->x;
      y[np]=pts->y;
      np++;	  
      pts=pts->next; 
    }
  if (closed>0)
    {  
      x[np]=x[0];
      y[np]=y[0];  
      np++;	
    }
}

// -----------------------------------------------------------------

void  PXContour::copy2vector(float* x,float *y,int& np)
{
  PXContourPoint *pts=toppoint;
  np=0;
  while(pts->next !=NULL)
    { 
      x[np]=pts->x;
      y[np]=pts->y;
      np++;	  
      pts=pts->next; 
    }
  if (closed>0)
    {  
      x[np]=x[0];
      y[np]=y[0];  
      np++;	
    }
}

// -----------------------------------------------------------------

void PXContour::Smooth(float sigma,int width)
{
  cleanupCache();
  float factor,scale,a,b;
  unsigned char wasclosed=closed;
  if (numpoints>2*width)
    {
      float* temppointx=new float[numpoints+2];
      float* temppointy=new float[numpoints+2];
      int np=0;
      copy2vector(temppointx,temppointy,np);

      int p=0;
      DeleteAll();
      for(int i=0;i<np;i++)
	{
	  scale=0.0; a=0.0; b=0.0;
	  for(int j=-width;j<=width;j++)
	    {
	      factor=(1/(sqrt(sigma*2*M_PI))*
		      exp(-0.5*(pow(float(j),float(2.0)))/pow(sigma,float(2.0))));
	      scale=scale+factor;
	      p=getcardinal(i+j,np,wasclosed);
	      a=a+factor*temppointx[p];
	      b=b+factor*temppointy[p];
	    }
	  Add(a/scale,b/scale);
	}
      delete [] temppointx;
      delete [] temppointy;
      if (wasclosed>0) 
	RemovePoint();
      closedcontour(wasclosed);
    }
}
// --------------------------------------------------------------------------
void PXContour::Touching()
{
  cleanupCache();
  if (numpoints>1)
      {
	unsigned char wasclosed=closed;
	float *temppointx=new float[numpoints+2];
	float *temppointy=new float[numpoints+2];
	
	int np=0;
	copy2vector(temppointx,temppointy,np);
	DeleteAll();
	
	float dist1,dist2;
	for(int i=0;i<(np-1);i++)
	  {
	    Add(temppointx[i],temppointy[i]);
	    dist1=(temppointx[i+1]-temppointx[i]);
	    dist2=(temppointy[i+1]-temppointy[i]);
	    
	    // Check for non-Touching Points i.e |x|>1 and |y|>1
	    
	    if ( fabs(dist2)>1.0 || fabs(dist1)>1.0)
	      {
		if (fabs(dist2)>fabs(dist1))
		  {
		    int dt=int(fabs(dist2)+0.9999);
		    for(int xx=1;xx<dt;xx++)
		      {
			float fr=(float(xx)/float(dt));
			float spx=fr*dist1;
			float spy=fr*dist2;
			Add(temppointx[i]+spx,temppointy[i]+spy);
		      }
		  }
		else
		  {
		    int dt=int(fabs(dist1)+0.9999);
		    for(int yy=1;yy<dt;yy++)
		      {
			float fr=(float(yy)/float(dt));
			float spy=fr*dist2;
			float spx=fr*dist1;
			Add(temppointx[i]+spx,temppointy[i]+spy);
		      }
		  }
	      }
	    else
	      if (dist1<1.0 && dist2<1.0)
		i=i+1;  // Skip next Point -- Same as this one;
	  }
	
	// Get Last-Point to be added
	float x0=lastpoint->previous->x;
	float y0=lastpoint->previous->y;
	
	dist1=fabs(temppointx[np-1]-x0);
	dist2=fabs(temppointy[np-1]-y0);
	
	if (dist1>=1.0 || dist2>=1.0)
	    Add(temppointx[np-1],temppointy[np-1]);
	
	if (wasclosed>0)
	    RemovePoint();
	closedcontour(wasclosed);
	
	delete [] temppointx;
	delete [] temppointy;
      }
}


// --------------------------------------------------------------------------
void PXContour::Sample(int maxratio)
{
  Equisample((int)(numpoints/maxratio));
}

// --------------------------------------------------------------------------
void PXContour::Subsample(int maxratio)
{

  //  Alternatively use equisample aka
  //  Equisample((int)(numpoints*maxratio));
  //  return;
  cleanupCache();
  float ratio1,ratio2,a,b;
  int np=numpoints;
  unsigned char wasclosed;
  
  wasclosed=closed;
  
  if (numpoints>0)
      {
	if (maxratio>1)
	    {
	      float *temppointx=new float[maxratio*numpoints+1];
	      float *temppointy=new float[maxratio*numpoints+1];
	      PXContourPoint *pts=toppoint;
	      int i=0;
	      while(pts->next !=NULL)
		  {
		    temppointx[i]=pts->x;
		    temppointy[i]=pts->y;
		    i++;
		    pts=pts->next;
		  }
	      if (wasclosed>0)
		  {
		    numpoints++;
		    temppointx[numpoints-1]=temppointx[0];
		    temppointy[numpoints-1]=temppointy[0];
		  }
	      DeleteAll();
	      
	      for(i=0;i<(np-1);i++)
		  for(int j=0;j<maxratio;j++)
		      {
			ratio2=((float)j)/((float)maxratio);
			ratio1=1-ratio2;
			a=(((float)temppointx[i])  *ratio1+
			   ((float)temppointx[i+1])*ratio2);
			b=(((float)temppointy[i])  *ratio1+
			   ((float)temppointy[i+1])*ratio2);
			//newindex=i*maxratio+j;
			Add(a,b);
		      }
	      Add(temppointx[np-1],temppointy[np-1]);
	      np=(np-1)*maxratio+1;
	      if (wasclosed>0) 
		  {
		    RemovePoint();
		    closedcontour(1);
		  }
	      
	      delete [] temppointx;
	      delete [] temppointy;
	    }
      }
  
}

// --------------------------------------------------------------------------

void PXContour::Equisample(int npoints)
{
  if (npoints>1)
    {
      float np=(float)npoints;
      float l=getcontourlength();
      float sp=l/np;
      Equispace(sp);
    }
}

// --------------------------------------------------------------------------

void PXContour::Equispace(float spacing)
{
  cleanupCache();
  if (numpoints<2)
    return;
  
  float sz=getcontourlength()/spacing;
  int ss=int(sz)+1;

  if (ss<4)
      {
	ss=4;
	spacing=getcontourlength()/float(ss-1);
      }

  if (spacing<0.01)
      return;

  ss+=5;

  float *temppointx=new float[ss];
  float *temppointy=new float[ss];
  
  unsigned char wasclosed=0;
  int i=0,np=0;
  float x1=0.0,x2=0.0,y1=0.0,y2=0.0;
  float x0=0.0,y0=0.0,segment=0.0,part=0.0,ratio=0.0;
      
  PXContourPoint *pts=gettoppoint();
  
  x0=pts->x; y0=pts->y;
  x1=x0;     y1=y0;
  pts=pts->next;
  temppointx[i]=x0;
  temppointy[i]=y0;
  
  while(pts->next !=NULL)
    {
      x2=pts->x;
      y2=pts->y;
      part=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
      segment=segment+part;
      while (segment>=spacing)
	{
	  i++;
	  ratio=1.0-(segment-spacing)/part;
	  segment=segment-spacing;
	  temppointx[i]=(1.0-ratio)*x1+ratio*x2;
	  temppointy[i]=(1.0-ratio)*y1+ratio*y2;
	}
      x1=x2;
      y1=y2;
      pts=pts->next;
    }
  
  if (closed>0)
    {
      wasclosed=1;
      part=sqrt((x0-x2)*(x0-x2)+(y0-y2)*(y0-y2));
      segment=segment+part;
      while (segment>=spacing)
	{
	  i++;
	  ratio=1.0-(segment-spacing)/part;
	  segment=segment-spacing;
	  temppointx[i]=(1.0-ratio)*x2+ratio*x0;
	  temppointy[i]=(1.0-ratio)*y2+ratio*y0;
	}
    }

  if (i>=ss)
    fprintf(stderr,"*****\n Error %d %d\n",i,ss);

  if (temppointx[i]==x0 && temppointy[i]==y0)
    i=i-1;
  
  np=i;
  DeleteAll();
  for(i=0;i<=np;i++)
    Add(temppointx[i],temppointy[i]);
  
  delete [] temppointx;
  delete [] temppointy;
  closedcontour(wasclosed);
}
// --------------------------------------------------------------------------
void PXContour::CalculateGradients(int scale)
{
  if (numpoints>2*scale+1)
      {
	//    cerr << "calculating gradient .. ";
	// Copy points out
	//-----------------
	unsigned char wasclosed=closed;
	double *pointx=new double[numpoints+2];
	double *pointy=new double[numpoints+2];
	
	int np=0;
	copy2vector(pointx,pointy,np);
	if (closed>0) np=np-1;  
	
	// cerr << "Finding orientation\n";
	
	// find Orientation by cross product
	//----------------------------------
	
	double v1x=pointx[np/3]-pointx[0];
	double v1y=pointy[np/3]-pointy[0];
	double v2x=pointx[2*np/3]-pointx[np/3];
	double v2y=pointy[2*np/3]-pointy[np/3];
	double zz=(v1x*v2y-v2x*v1y);
	double anticlockwise=-1.0;
	if (zz>0.0) anticlockwise=1.0;
	//    cerr << "anticlockwise " << anticlockwise << "\n";
	
	// Estimate number of neighbours scale =1,2,3
	// -----------------------------
	if (scale<1) scale=1;
	if (scale>5) scale=5;
	int neighbours = scale;
	int vectsize  = 2*scale+1;
	
	// cerr << "Preparing to add Gradients\n";
	
	// Prepare to Add Gradients
	//--------------------------
	DeleteGrad();
	gradient=1;
	lastgradient=new PXContourGrad;
	lastgradient->next=NULL;
	lastgradient->previous=NULL;
	topgradient=lastgradient;
	
	// cerr << "Preparing Least Square Matrices \n";
	
	// Prepare Least Square Matrices
	// ------------------------------
	
	double **A     = dmatrix(1, vectsize, 1, 3);
	double **AtA   = dmatrix(1, 3, 1, 3);
	
	double *tempx  =  dvector(1,vectsize);
	double *tempy  =  dvector(1,vectsize);
	double *aty    =  dvector(1,3);
	int   *indx   =   ivector(1,3);
	
	// First Calculate Gradients for internal
	// Find  First Estimate of Normal -- fit quadratic
	// -----------------------------------------------
	
	double tx,ty,nx,ny,mag;
	
	int k1,k_1;
	for(int i=0;i<np;i++)
	    {
	      k_1=getcardinal(i-1,np,wasclosed);
	      k1=getcardinal(i+1,np,wasclosed);
	      tx=pointx[k1]-pointx[k_1];
	      ty=pointy[k1]-pointy[k_1];
	      mag=sqrt(tx*tx+ty*ty);
	      if (mag>0.0)
		  {
		    tx =   tx/mag;
		    ty =   ty/mag;
		  }
	      nx=-ty;
	      ny= tx;
	      
	      
	      /* transform the neighbor points, such the     */
	      /* the k point becomes the origin,             */
	      /* the normal direction N becomes the y-axis,  */
	      /* the tangent directions T becomes the x-axis */
	      
	      double xx,yy;
	      int k,count=1;
	      for(int j=-neighbours ; j<=neighbours; j++)
		  {
		    k=getcardinal(i+j,np,wasclosed);
		    xx = pointx[k] - pointx[i];
		    yy = pointy[k] - pointy[i];
		    tempx[count]= xx * tx + yy * ty;
		    tempy[count]= xx * nx + yy * ny;
		    count++;
		    
		  }
	      
	      // Effectively Solve A'Ax=A'y ;
	      // Fit the local quadric curve using least square method 
	      // Step 1 == Form A
	      
	      for(int l=1; l<=vectsize; l++)
		  {
		    A[l][1] = 1.0;
		    A[l][2] = tempx[l];
		    A[l][3] = tempx[l]*tempx[l];
		  }
	      
	      // Step 2 Form A'A and form LU decomposition
	      
	      for (int mrow=1;mrow<=3;mrow++)
		  for (int mcol=1;mcol<=3;mcol++)
		      {
			AtA[mrow][mcol]=0.0;
			for (int mdummy=1;mdummy<=vectsize;mdummy++)
			    {
			      AtA[mrow][mcol]=AtA[mrow][mcol]+
				  A[mdummy][mrow]*A[mdummy][mcol];
			    }
		      }
	      
	      double det,curv;
	      int singular;
	      ludcmp(AtA,3,indx,&det,singular);
	      
	      if(singular!=1)
		  {
		    // Step 3 -- Form RHS A'y
		    
		    for (int vrow=1;vrow<=3;vrow++)
			{
			  aty[vrow]=0.0;
			  for (int vdummy=1;vdummy<=vectsize;vdummy++)
			      aty[vrow]=aty[vrow]+A[vdummy][vrow]*tempy[vdummy];
			}
		    
		    lubksb(AtA,3,indx,aty);
		    
		    // compute the curvatures -- at point (0,0) 
		    double xt  = 1.0;
		    double yt  = aty[2];
		    double ytt = aty[3] * 2.0; 
		    
		    curv= sqrt( (xt*xt+yt*yt)*(xt*xt+yt*yt)*(xt*xt+yt*yt) );
		    if (fabs(curv)>0.01)
			curv= xt*ytt / curv;
		    else
			{
			  curv=1e+5;
			  // cerr << "big \n";
			}
		    
		  }
	      else curv=0.0;
	      
	      AddGrad(nx*anticlockwise,ny*anticlockwise,curv);
	    }
	//    cerr << "...........done \n";
	
	free_dmatrix(A,1, vectsize, 1, 3);
	free_dmatrix(AtA,1, 3, 1, 3);
	free_dvector(tempx,1,vectsize);
	free_dvector(tempy,1,vectsize);
	free_dvector(aty,1,3);
	free_ivector(indx,1,3);
	delete [] pointx;
	delete [] pointy;
	
	gradient=1;
      }
}

// ***********************************************************************
void PXContour::AddSpline(double** x,int n,int np)
{
  cleanupCache();
  int i,j,k;
  double** z=dmatrix(1,n,1,2);
  double** R=dmatrix(1,2,1,2);
  
  int flag=-1;
  double theta=0.0;
  // Find theta which puts these in increasing order
  // First guess is tangent of line joining first and last

  while (flag== -1 )
    {
      R[1][1]=cos(theta); R[1][2]=-sin(theta);
      R[2][1]=sin(theta); R[2][2]= cos(theta);

      for(int pnt=1;pnt<=n;pnt++)
	{
	  z[pnt][1]=R[1][1]*x[pnt][1]+R[1][2]*x[pnt][2];
	  z[pnt][2]=R[2][1]*x[pnt][1]+R[2][2]*x[pnt][2];
	}

      flag=1;
      double r1=z[2][1]-z[1][1];
      for(i=2;i<n;i++)
	if ((z[i+1][1]-z[i][1])*r1 <= 0) flag=-1;
      
      if (theta==0.0) 
	theta=1.51;
      else 
	theta=theta+0.5;
    }

  double** A=dmatrix(1,n,1,n);
  double*  b=dvector(1,3);

  for(i=1;i<=n;i++)
    {
      for(j=1;j<=n;j++)
	A[i][j]=pow(z[i][1],j-1.0);
      b[i]=z[i][2];
    }
  
  double det;
  int singular,*indx=ivector(1,n);
  ludcmp(A,n,indx,&det,singular);
  lubksb(A,n,indx,b);

  double xn[3],z1,z2,gap;  
  for(i=1;i<n;i++)
    {
      gap=z[i+1][1]-z[i][1];
      for (k=0;k<=np;k++)
	{
	  z1=z[i][1]+(float(k)/float(np+1.0))*gap;
	  z2=0.0;
	  for (j=1;j<=n;j++)
	    z2=z2+pow(z1,j-1.0)*b[j];

	  xn[1]=R[1][1]*z1+R[2][1]*z2;
	  xn[2]=R[1][2]*z1+R[2][2]*z2;
	  Add((float)xn[1],(float)xn[2]);      
	}
      Add((float)x[i+1][1],(float)x[i+1][2]);      
    }

  
  free_ivector(indx,1,n);      free_dmatrix(A,1,n,1,n);
  free_dvector(b,1,n);         free_dmatrix(z,1,n,1,2);    
  free_dmatrix(R,1,2,1,2);     
}


// ***********************************************************************
//  Routines adapted from Francois' code
// ***********************************************************************

int PXContour::Intersect (float x1,float y1, 
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
int PXContour::IsInside(float testpointx,float testpointy)
{
  int  count=0;

  PXContourPoint* top=toppoint;
  float x2 = top->x;
  float y2 = top->y;
  
  PXContourPoint* pts=top->next;

  while (pts->next !=NULL)
      {
	float x1 = x2;
	float y1 = y2;
	x2 = pts->x;
	y2 = pts->y;
	if (Intersect (x1, y1, x2, y2, testpointx, testpointy))
	    count ++;
	pts=pts->next;
      }

  if (Intersect (x2, y2, top->x,top->y,testpointx, testpointy))
      count ++;

  int isodd=count%2;
  return isodd;
}

int PXContour::IsInside(float testpointx,float testpointy,
			float *pointx,float* pointy,
			int numberofpoints)
{
  
  int  index=0,count=0;
  float x1, y1,x2, y2;

  x2 = pointx[index];
  y2 = pointy[index];

  while (index<numberofpoints) 
      {
	index++;
	x1 = x2;
	y1 = y2;
	x2 = pointx [index];
	y2 = pointy [index];

	if (Intersect (x1, y1, x2, y2, testpointx, testpointy))
	    count ++;
      }

  int isodd=count%2;
  return isodd;
}

// ***********************************************************************
int PXContour::ensureAnticlockwise()
{
  return setDirection(1);
}

// ***********************************************************************
int PXContour::ensureClockwise()
{
  return setDirection(-1);
}
// ***********************************************************************
int PXContour::setDirection(int dir)
{
  int  flag=0;
  if (numpoints>2)
      {
	int np=numpoints;
	unsigned char wasclosed=closed;
	float px0,px1,px2,py0,py1,py2;

	PXContourPoint* pts=gettoppoint();
	px0=pts->x; py0=pts->y;
	
	int i;
	for(i=0;i<np/3;i++)
	    pts=pts->next;
	px1=pts->x; py1=pts->y;

	for(i=0;i<np/3;i++)
	    pts=pts->next;
	px2=pts->x; py2=pts->y;

	// Find Orientation by cross product
	//----------------------------------
	
	float v1x=px1-px0;
	float v1y=py1-py0;
	float v2x=px2-px1;
	float v2y=py2-py1;
	float zz=(v1x*v2y-v2x*v1y);

	int flag=0;
	if (dir>0 && zz<0.0)
	    flag=1;
	if (dir<=0 && zz>0.0) 
	    flag=1;

	if (flag)
	    {
	      int np;
	      double *pointx=new double[numpoints+2];
	      double *pointy=new double[numpoints+2];
	      copy2vector(pointx,pointy,np);
	      if (closed>0) 
		np=np-1;  
	      DeleteAll();
	      for (int i=np-1;i>=0;i=i-1)
		Add(pointx[i],pointy[i]);
	      delete [] pointx;
	      delete [] pointy;
	      closed=wasclosed;
	      cleanupCache();
	    }
      }
  return flag;
}
// ------------------------------------------------------------------------------------
void PXContour::cleanupCache()
{
  if (hascache())
    {
      delete [] px;
      delete [] py;
      contourhascache=0;
    }
  
}
// ------------------------------------------------------------------------------------
int PXContour::getcyclicindex(int pt,int np)
{
  while (pt<0)
      pt=pt+np;
  while (pt>=np)
      pt=pt-np;
  return pt;
}
// ------------------------------------------------------------------------------------
void PXContour::generateCache(int forceClosed)
{
  if (forceClosed)
      setclosed(1);

  cleanupCache();

  px=new float[numpoints+1];
  py=new float[numpoints+1];
  int np;
  copy2vector(px,py,np);
  contourhascache=1;
}
// ------------------------------------------------------------------------------------
float PXContour::findnearestneighbour(float&x,float& y,float dt,int )
{
  if (dt<1.0 && !hascache())
      generateCache();
      
  int np=getnumpoints(),index=0;
  float x0=0,y0=0,mindist=10000000.0;

  int step=int(np/20)+1;

  // One pass at 20 points /contour
  for (int ib=0;ib<np;ib+=step)
      {
	float dist=pow(px[ib]-x,float(2.0))+pow(py[ib]-y,float(2.0));
	if (dist<mindist)
	    {
	      index=ib;
	      x0=px[ib];
	      y0=py[ib];
	      mindist=dist;
	    }

      }

  // Second Pass at pixel-resolution around last solution
  if (step>1)
      {
	for (int ib=index-step*2;ib<index+step*2;ib++)
	    {
	      int ia=getcyclicindex(ib,np);
	      float dist=pow(px[ia]-x,float(2.0))+pow(py[ia]-y,float(2.0));
	      if (dist<mindist)
		  {
		    index=ia;
		    x0=px[ia];
		    y0=py[ia];
		    mindist=dist;
		  }
	    }
      }

  // If needed do subpixel also
  if (dt>0.0 && dt<1.0)
      {
	float ds=0.5;
	while (ds>dt)
	    ds=0.5*ds;

	for (int st=0;st<=1;st++)
	    {
	      float tx0=px[getcyclicindex(index-1+st,np)];
	      float ty0=py[getcyclicindex(index-1+st,np)];
	      float tx1=px[getcyclicindex(index+st  ,np)];
	      float ty1=py[getcyclicindex(index+st  ,np)];

	      for (float s=ds;s<1.0;s+=ds)
		  {
		    float tx=s*tx1+(1-s)*tx0;
		    float ty=s*ty1+(1-s)*ty0;
		    float dist=pow(tx-x,float(2.0))+pow(ty-y,float(2.0));

		    if (dist<mindist)
			{
			  x0=tx;
			  y0=ty;
			  mindist=dist;
			}
		  }
	    }
	
      }
  x=x0;
  y=y0;
  return sqrt(mindist);
}
// ------------------------------------------------------------------------------------
void  PXContour::nonShrinkSmooth(float sigma,int iterations,
				 float alpha,float beta,int width)

{
  iterations=Irange(iterations,1,1000);
  sigma=Frange(sigma,0.0,10.0);
  int wd=int(sigma*2.0+1);
  if (wd<3) wd=3;
  int wd2=19;
  if (wd>wd2) wd2=wd+2;
  width=Irange(width,wd,wd2);
  cleanupCache();
  float factor,scale,a,b;
  unsigned char wasclosed=closed;
  if (numpoints>2*width)
    {
      float* temppointx=new float[numpoints+2];
      float* temppointy=new float[numpoints+2];
      for (int iter=0;iter<iterations;iter++)
	  {
	    for(int pass=0;pass<=1;pass++)
		{
		  int np=0;
		  copy2vector(temppointx,temppointy,np);
		  int p=0;
		  DeleteAll();
		  for(int i=0;i<np;i++)
		      {
			scale=0.0; a=0.0; b=0.0;
			for(int j=-width;j<=width;j++)
			    {
			      factor=(1/(sqrt(sigma*2*M_PI))*
				      exp(-0.5*(pow(float(j),float(2.0)))/pow(sigma,float(2.0))));
			      scale=scale+factor;
			      p=getcardinal(i+j,np,wasclosed);
			      a=a+factor*temppointx[p];
			      b=b+factor*temppointy[p];
			    }
			float newx=a/scale; 	  float newy=b/scale;
			float oldx=temppointx[i]; float oldy=temppointy[i];
			if (pass==0)
			    Add((1-alpha)*oldx+alpha*newx,(1-alpha)*oldy+alpha*newy);
			else
			    Add((1-beta)*oldx+beta*newx,(1-beta)*oldy+beta*newy);
		      }
		}
	  }
      delete [] temppointx;
      delete [] temppointy;
      if (wasclosed>0) 
	  RemovePoint();
      closedcontour(wasclosed);
    }
}
// --------------------------------------------------------------------------
int  PXContour::saveContourPair(const char* fname,PXContour* cntr1,PXContour* cntr2)
{
  if (cntr1->getnumpoints() !=cntr2->getnumpoints())
      return 0;
  FILE* fout=fopen(fname,"w");
  if (fout)
      {
	PXContourPoint *pts2=cntr2->gettoppoint();
	while(pts2->next !=NULL)
	    { 
	      fprintf(fout,"%6.3f %6.3f\n",pts2->x,pts2->y);
	      pts2=pts2->next; 
	    }
	pts2=cntr2->gettoppoint();
	fprintf(fout,"%6.3f %6.3f\n",pts2->x,pts2->y);
	PXContourPoint *pts1=cntr1->gettoppoint();
	while(pts2->next !=NULL)
	    {
	      fprintf(fout,"%6.3f %6.3f\n",pts1->x,pts1->y);
	      if (pts1!=cntr1->gettoppoint())
		  {
		    fprintf(fout,"%6.3f %6.3f\n",pts2->x,pts2->y);
		    fprintf(fout,"%6.3f %6.3f\n",pts1->x,pts1->y);
		  }
	      pts2=pts2->next; 
	      pts1=pts1->next;
	    }
	pts1=cntr1->gettoppoint();
	fprintf(fout,"%6.3f %6.3f\n",pts1->x,pts1->y);
	fclose(fout);
	return 1;
      }
  return 0;
}
// --------------------------------------------------------------------------
int  PXContour::saveContourPair(const char* fname,
				float* px1,float* py1,
				float* px2,float* py2,
				int np1,int np2,
				int* used,int* corr)
{
  FILE* fout=fopen(fname,"w");
  if (fout)
      {
	int done=0,deb=0,link=0;
	while(deb<np1 && done==0)
	    {
	      if (used[deb]==1)
		  done=1;
	      else
		  deb++;
	    }
	for (int j=0;j<=np2;j++)
	    {
	      int ind=Icyclic(corr[deb]+j,np2);
	      fprintf(fout,"%6.3f %6.3f %d\n",px2[ind],py2[ind],ind);
	    }
	for (int i=0;i<=np1;i++)
	    {
	      int ind=Icyclic(deb+i,np1);
	      fprintf(fout,"%6.3f %6.3f %d\n",px1[ind],py1[ind],ind);
	      if (used[ind]==1)
		  {
		    link++;
		    fprintf(fout,"%6.3f %6.3f %d\n",px2[corr[ind]],py2[corr[ind]],corr[ind]);
		    fprintf(fout,"%6.3f %6.3f %d\n",px1[ind],py1[ind],ind);
		  }
	    }
//	fprintf(stderr,"Saved in %s (links=%d)\n",fname,link);
	fclose(fout);
	return 1;
      }
  return 0;
}
// --------------------------------------------------------------------------
int PXContour::cyclicDistance(int p1,int p2,int np)
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
// --------------------------------------------------------------------------
PXContour*  PXContour::mapContours(PXContour* cntr1,PXContour* cntr2,int resample)
{
  /*static int count_map=0;
  ++count_map;
  fprintf(stderr,"\n Beginning Points =%d (%.1f),%d(%.1f) count=%d\n",
	  cntr1->getnumpoints(),
	  cntr1->getcontourlength(),
	  cntr2->getnumpoints(),
	  cntr2->getcontourlength(),
	  count_map);*/
  int debug=0;
  /*  PXContour* cntr2=new PXContour(cnt2);*/

  if (resample)
    {
      cntr2->setclosed(1);
      int np2=cntr1->getnumpoints()*8;
      if (np2>3998)
	np2=3998;
	
      cntr2->Equisample(np2);
      cntr2->Smooth();
    }


  static float px1[4000], py1[4000], px2[4000], py2[4000];
  int np1=cntr1->getnumpoints();
  int np2=cntr2->getnumpoints();

  if (np1 >3999 || np2>3999)
    {
      fprintf(stderr,"Can only handle contours with 4000 points or less\n");
      exit(1);
    }

  static int corr[4000], used[4000], fixed[4000];
  int np0;
  cntr2->setclosed(1);
  cntr1->setclosed(1);
  cntr1->copy2vector(px1,py1,np0);
  cntr2->copy2vector(px2,py2,np0);
    
  // Pass 1 --- Find all points that have symmetric neighbours
  // ---------------------------------------------------------
  int numgood=0;
  for (int p1=0;p1<np1;p1++)
      {
	used[p1]=0;
	fixed[p1]=0;
	corr[p1]=-1;
	float mindist=10000000.0;
	int   besti=0;
	for (int i=0;i<np2;i++)
	    {
	      float dist=Fsqdist(px1[p1],py1[p1],0.0,px2[i],py2[i],0.0);
	      if (dist<mindist)
		  {
		    mindist=dist;
		    besti=i;
		  }
	    }

	//mindist=10000000.0;
	int bestj=p1;
	for (int j=0;j<np1;j++)
	    {
	      float dist=Fsqdist(px1[j],py1[j],0.0,px2[besti],py2[besti],0.0);
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
	      ++numgood;
	    }
      }

  if (debug==1)
      PXContour::saveContourPair("part0.cnt",px1,py1,px2,py2,np1,np2,used,corr);


  if (numgood<2)
    {
      for (int i=0;i<np1;i++)
	fprintf(stderr,"i=%d/%d px1[%d]=%.2f py1[%d]=%.2f\n",i,np1,i,px1[i],i,py1[i]);
      for (int j=0;j<np2;j++)
	fprintf(stderr,"j=%d/%d px1[%d]=%.2f py1[%d]=%.2f\n",j,np2,j,px2[j],j,py2[j]);
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

		    int dist0=PXContour::cyclicDistance(p0,p1,np1);
		    int dist2=PXContour::cyclicDistance(p1,p2,np1);
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

  if (debug==1)
      PXContour::saveContourPair("part1.cnt",px1,py1,px2,py2,np1,np2,used,corr);

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
				int dist=PXContour::cyclicDistance(corr[p2],ptest,np2);
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
	
	//for (int p1=0;p1<np1;p1++)
	  //  corr[p1]=Icyclic(corr[p1]+shifts[p1],np2);

	if (debug==2)
	    {
	      fprintf(stderr,"Smoothing Iteration %d moved=%d\n",iter,nummoved);
	      char line[100];
	      sprintf(line,"iter.%02d",iter);
	      PXContour::saveContourPair(line,px1,py1,px2,py2,np1,np2,used,corr);
	    }
	iter++;
      }
  if (debug)
    fprintf(stderr,"Iterations %d\n",iter);


  // Cleanup
  // -------
  PXContour* out=new PXContour();
  for (int i=0;i<np1;i++)
      out->Add(px2[corr[i]],py2[corr[i]]);
      
  if (debug==1)
      out->Save("final.cnt");


  /*  delete [] px1;  delete [] px2;  delete [] py1;  delete [] py1; 
      delete [] corr; delete [] used; delete [] fixed;*/

  //cnt2->DeleteAll();
  //cnt2->copypoints(cntr2);
  //delete cntr2;
  return out;

}
// ***********************************************************************
//
//      Contour Statistics 
//
// ***********************************************************************
float PXContour::getarea()
{
  if (numpoints<3)
      return 0.0;

  if (isclosed()>0)
      {
	float area=0.0;
	PXContourPoint* temp=toppoint;
	
	//	2 A( P ) = sum_{i=0}^{n-1} (x_i y_{i+1} - y_i x_{i+1}).

	for (int i=0;i<numpoints;i++)
	    {
	      float xi=temp->x;
	      float yi=temp->y;
	      float xip,yip;
	      if (i!=numpoints-1)
		  {
		    xip=temp->next->x;
		    yip=temp->next->y;
		  }
	      else
		  {
		    xip=toppoint->x;
		    yip=toppoint->y;
		  }
	      area+= xi*yip-yi*xip;
	      temp=temp->next;
	    }
	return fabs(area*0.5);
      }
  else
      return -1.0;
  
}
// ***********************************************************************
void PXContour::getcentroid(float& x,float& y)
{
  int np=0;
  float sumx=0.0,sumy=0.0;
  PXContourPoint* pts=toppoint;
  while (pts->next !=NULL)
      {
	sumx+=pts->x;
	sumy+=pts->y;
	np++;
	pts=pts->next;
      }
  x=sumx/float(np);
  y=sumy/float(np);
}
// ***********************************************************************
void PXContour::getextent(float &xmin,float& xmax,float& ymin,float& ymax)
{
  PXContourPoint* pts=toppoint;
  xmin=pts->x;  xmax=pts->x;
  ymin=pts->y;  ymax=pts->y;
  //  fprintf(stderr,"pt (%.2f \t %>2f)\n",pts->x,pts->y);
  pts=pts->next;
  while (pts->next !=NULL)
      {
	xmin=Fmin(pts->x,xmin); 	ymin=Fmin(pts->y,ymin);
	xmax=Fmax(pts->x,xmax); 	ymax=Fmax(pts->y,ymax);
	//	fprintf(stderr,"pt (%.2f \t %.2f)\n",pts->x,pts->y);
	pts=pts->next;
      }
  //  fprintf(stderr,"Bounds x=%.2f:%.2f y=%.2f:%.2f\n",xmin,xmax,ymin,ymax);
}
// ***********************************************************************
void PXContour::getextrema(int direction,int min,float& x,float& y)
{
  min=min>0;
  direction=Irange(direction,1,2);
  float xmin,xmax,ymin,ymax;
  getextent(xmin,xmax,ymin,ymax);
  
  PXContourPoint* pts=toppoint;
  int found=0;
  while (pts->next !=NULL && found==0)
      {
	if (min==1 && direction==1)
	    {
	      if (fabs(xmin-pts->x)<0.01)
		  found=1;
	    }

	if (min==1 && direction==2)
	    {
	      if (fabs(ymin-pts->y)<0.01)
		  found=1;
	    }
	if (min==0 && direction==1)
	    {
	      if (fabs(xmin-pts->x)<0.01)
		  found=1;
	    }
	if (min==0 && direction==2)
	    {
	      if (fabs(ymax-pts->y)<0.01)
		  found=1;
	    }
	if (found!=1)
	    pts=pts->next;
      }
  x=pts->x;
  y=pts->y;
}
// ***********************************************************************
void PXContour::calcStats(float& mean,float& max,float& min,int& nsegm,int opencurve)
{
  PXContourPoint* temp=gettoppoint();
  int np=getnumpoints();
  float x0=0,y0=0,dist=0.0;
  
  min=1000.0,max=-10000.0;
  int numdist=0;
  for (int ib=1;ib<=np;ib++)
      {
	if (ib>1)
	    {
	      float d=sqrt( pow(temp->x-x0,float(2.0)) + pow(temp->y-y0,float(2.0)) );
	      dist+=d;
	      min=Fmin(min,d);
	      max=Fmax(max,d);
	      numdist++;
	    }
	x0=temp->x;
	y0=temp->y;
	temp=temp->next;
      }
  if (!opencurve)
      {
	temp=gettoppoint();
	float d=sqrt(pow(temp->x-x0,float(2.0))+pow(temp->y-y0,float(2.0)));
	dist+=d;
	min=Fmin(min,d);
	max=Fmax(max,d);
	numdist++;
	x0=temp->x;
	y0=temp->y;
	numdist++;
      } 
  mean=dist/float(numdist);
  nsegm=numdist;
}
// ***********************************************************************
float PXContour::getcontourlength()
{
  float length=0.0;
  float x1=0.0,x2=0.0,y1=0.0,y2=0.0;
  float x0=0.0,y0=0.0;

  if (numpoints>1)
    {
      PXContourPoint *pts=toppoint;
      x0=pts->x; y0=pts->y;
      x1=x0;     y1=y0;
      pts=pts->next;

      while(pts->next !=NULL)
	{
	  x2=pts->x;
	  y2=pts->y;
	  length+=sqrt(pow(x1-x2,float(2.0))+pow(y1-y2,float(2.0)));
	  x1=x2;
	  y1=y2;
	  pts=pts->next;
	}
      
      if (closed>0)
	length=length+sqrt((x0-x2)*(x0-x2)+(y0-y2)*(y0-y2));
    }
  return length;
}
// ***********************************************************************
void  PXContour::getmajoraxis(float& nx,float& ny)
{
  float *px=new float[getnumpoints()+1];
  float *py=new float[getnumpoints()+1];
  int wasclosed=isclosed();
  setclosed(0);
  int np=0;
  copy2vector(px,py,np);
  setclosed(wasclosed);
  getmajoraxis(px,py,np,nx,ny);
  delete [] px;
  delete [] py;

}
// ***********************************************************************  
void  PXContour::getdualcentroid(PXContour* other,float& x,float& y)
{
  float x1=0.0,y1=0.0,l1=0.0;
  float x2=0.0,y2=0.0,l2=0.0;
  getcentroid(x1,y1); other->getcentroid(x2,y2);
  l1=getcontourlength(); l2=other->getcontourlength();

  x= (x1*l1+x2*l2)/(l1+l2);
  y= (y1*l1+y2*l2)/(l1+l2);
}
// ***********************************************************************  
void PXContour::getdualmajoraxis(PXContour* other,float& nx,float& ny)
{
  int nptot=getnumpoints()+other->getnumpoints();
  float *px=new float[nptot];
  float *py=new float[nptot];
  int wasclosed=isclosed();
  setclosed(0);
  int np=0;
  copy2vector(px,py,np);
  setclosed(wasclosed);
  int np2=0;
  wasclosed=other->isclosed();
  other->setclosed(0);
  copy2vector(&px[np],&py[np],np2);
  other->setclosed(wasclosed);
  np+=np2;
  getmajoraxis(px,py,np,nx,ny);
  delete [] px;
  delete [] py;

}
// ***********************************************************************  
void PXContour::getmajoraxis(float* x,float* y,int np,float& nx,float& ny)
{
  
  float cx=0.0,cy=0.0,scale=1.0/float(np);
  float sxx=0.0,syy=0.0,sxy=0.0;
  int i;
  for (i=0;i<np;i++)
    {
      cx+=x[i]; cy+=y[i];
    }
  cx*=scale; cy*=scale;
  
  for (i=0;i<np;i++)
    {
      x[i]-=cx;
      y[i]-=cy;
    }
  
  for (i=0;i<np;i++)
    {
      sxx+=x[i]*x[i];
      syy+=y[i]*y[i];
      sxy+=x[i]*y[i];
    }
  
  sxx*=scale; syy*=scale; sxy*=scale;

  double** a1    = dmatrix(0,1,0,1);
  double*  d1    = dvector(0,1);
  double** v1    = dmatrix(0,1,0,1);

  a1[0][0]=sxx;   a1[0][1]=sxy;
  a1[1][0]=sxy;   a1[1][1]=syy;
  
  vtkMath_JacobiN(a1,2,d1,v1);

  if (d1[0]>d1[1])
    {
      nx=v1[0][0]; ny=v1[1][0];
    }
  else
    {
      nx=v1[0][1]; ny=v1[1][1];
    }
  
  float mag=sqrt(nx*nx+ny*ny);
  nx/=mag;
  ny/=mag;
  
  free_dmatrix(v1,0,1,0,1);  
  free_dmatrix(a1,0,1,0,1);  
  free_dvector(d1,0,1);      
}
// ***********************************************************************  



