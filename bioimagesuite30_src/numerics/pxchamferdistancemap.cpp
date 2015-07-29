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
//
//
//---------------------------------------------------------------------------------------------------------

#include "pxchamferdistancemap.h"

short PXChamferDistanceMap::pixel_size=100;
short PXChamferDistanceMap::obj_gray=30000;
short PXChamferDistanceMap::out_gray=-30000;
short PXChamferDistanceMap::margin=30;
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
PXChamferDistanceMap::PXChamferDistanceMap(PXContour* cnt,int filter_mode,int subpxl,
					   int imsizex,int imsizey)
{
  initializemembers();
  contour=new PXContour(cnt);
  contour->setclosed(1);
  init(filter_mode,subpxl,imsizex,imsizey);
}

PXChamferDistanceMap::PXChamferDistanceMap(PXContour* cntr,int filter_mode,
					   int subpxl,PXChamferDistanceMap* clone)
{
  initializemembers();
  contour=new PXContour(cntr);
  contour->setclosed(1);
  subpixel=(subpxl>0);

  int np=contour->getnumpoints();
  if (np==0)
    {
      fprintf(stderr,"pxchamferdistancemap::Bad Contour\n");
      return;
    }
  offsetx=clone->getoffsetx();
  offsety=clone->getoffsety();
  min_x=clone->getminx();
  min_y=clone->getminy();
  max_x=clone->getmaxx();
  max_y=clone->getmaxy();

  int imsizex=clone->getDistanceMap()->getsizex();
  int imsizey=clone->getDistanceMap()->getsizey();

    
  image=new PXImage(imsizex,imsizey);
  hasmap=0;
  
  if (filter_mode>0)
    generateMap(filter_mode);
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
PXChamferDistanceMap::PXChamferDistanceMap(PXContour* cntr1,PXContour* cntr2,
					   int fraction1,int fraction2)
{
  initializemembers();
  PXChamferDistanceMap* map1=new PXChamferDistanceMap(cntr1); 
  PXChamferDistanceMap* map2=new PXChamferDistanceMap(cntr2); 
  lininterpolate(map1,map2,fraction1,fraction2);
  delete map1;
  delete map2;
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
PXChamferDistanceMap::PXChamferDistanceMap(PXContour* cntr0,PXContour* cntr1,
					   PXContour* cntr2,PXContour* cntr3,
					   float spc)
{
  initializemembers();
  PXChamferDistanceMap* map0=new PXChamferDistanceMap(cntr0); 
  PXChamferDistanceMap* map1=new PXChamferDistanceMap(cntr1); 
  PXChamferDistanceMap* map2=new PXChamferDistanceMap(cntr2); 
  PXChamferDistanceMap* map3=new PXChamferDistanceMap(cntr3); 
  catsplineinterpolate(map0,map1,map2,map3,spc);
  delete map0;
  delete map1;
  delete map2;
  delete map3;
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

PXChamferDistanceMap::PXChamferDistanceMap(PXChamferDistanceMap* map1,
					   PXChamferDistanceMap* map2,
					   int fraction1,int fraction2,
					   int imsizex,int imsizey)
{
  initializemembers();
  lininterpolate(map1,map2,fraction1,fraction2,imsizex,imsizey);
}
PXChamferDistanceMap::PXChamferDistanceMap(PXChamferDistanceMap* map0,
					   PXChamferDistanceMap* map1,
					   PXChamferDistanceMap* map2,
					   PXChamferDistanceMap* map3,
					   float spc)
{
  initializemembers();
  catsplineinterpolate(map0,map1,map2,map3,spc);
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
void PXChamferDistanceMap::initializemembers()
{
  image=NULL;
  contour=NULL;
  offsetx=0; offsety=0; subpixel=0; hasmap=0; min_x=0; max_x=0; min_y=0;max_y=0;
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
void PXChamferDistanceMap::init(int filter_mode,int subpxl,int input_imagesizex,int input_imagesizey)
{
  subpixel=(subpxl>0);

  int np=contour->getnumpoints();
  if (np==0)
    {
      fprintf(stderr,"pxchamferdistancemap::Bad Contour\n");
      return;
    }

  PXContourPoint *pts=contour->gettoppoint();
  min_x=int(pts->x+0.5),max_x=int(pts->x+0.5);
  min_y=int(pts->y+0.5),max_y=int(pts->y+0.5);

  for (int pt=0;pt<np;pt++)
    { 
      float x=pts->x;	        float y=pts->y;	
      int i=int(x+0.5);	int j = int(y+0.5);
      pts=pts->next;      
      min_x = Imin( min_x, i );	 min_y = Imin( min_y, j );
      max_x = Imax( max_x, i );  max_y = Imax( max_y, j );
    }
  
  offsetx=0;   offsety=0;
  max_x+=margin;    min_x-=margin;
  max_y+=margin;    min_y-=margin;

  int imsizex=max_x-min_x+1;	offsetx=min_x; 

  if (input_imagesizex>imsizex)
    {
      offsetx=-(input_imagesizex-imsizex)/2;
      imsizex=input_imagesizex;
    }

  int imsizey=max_y-min_y+1;  offsety=min_y; 
  if (input_imagesizey>imsizey)
    {
      offsety=-(input_imagesizey-imsizey)/2;
      imsizey=input_imagesizey;
    }
  
  //contour->Save("imagecontour.cntr");
  
  image=new PXImage(imsizex,imsizey);
  hasmap=0;
  
  if (filter_mode>0)
    generateMap(filter_mode);
  
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
PXChamferDistanceMap::~PXChamferDistanceMap()
{
  if (image!=NULL)
    delete image;
  if (contour!=NULL)
    delete contour;
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
void PXChamferDistanceMap::lininterpolate(PXChamferDistanceMap* map1,
					  PXChamferDistanceMap* map2,
					  int fraction1,int fraction2,
					  int imsizex,int imsizey)
{
  if (imsizex<=0)
    {
      min_x=Imin(map1->getminx(),map2->getminx());
      max_x=Imax(map1->getmaxx(),map2->getmaxx());
      imsizex=max_x-min_x+1;
      offsetx=min_x;
    }
  else
    {
      offsetx=0;
      min_x=0;
      max_x=imsizex-1;
    }
  
  if (imsizey<=0)
    {
      min_y=Imin(map1->getminy(),map2->getminy());
      max_y=Imax(map1->getmaxy(),map2->getmaxy());
      imsizey=max_y-min_y+1;
      offsety=min_y;
    }
  else
    {
      offsety=0;
      min_y=0;
      max_y=imsizey-1;
    }

  image=new PXImage(imsizex,imsizey);
  
  for (int i=min_x;i<=max_x;i++)
    for (int j=min_y;j<=max_y;j++)
      {
	short p1=map1->getvalue(i,j);    
	short p2=map2->getvalue(i,j);
	short p=out_gray;
	
	if (p1*p2<=0)
	  p=(fraction1*p1+fraction2*p2)/(fraction1+fraction2);
	else if (p1>0)
	  p=obj_gray;
	
	if (p==0)
	  p++;
	image->setspixel(i-offsetx,j-offsety,p);
      }
  contour=new PXContour();
  hasmap=1;
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
void PXChamferDistanceMap::catsplineinterpolate(PXChamferDistanceMap* map0,
						PXChamferDistanceMap* map1,
						PXChamferDistanceMap* map2,
						PXChamferDistanceMap* map3,
						float fr)
{
  min_x=Imin(map0->getminx(),map1->getminx());
  min_x=Imin(min_x,map2->getminx());
  min_x=Imin(min_x,map3->getminx());

  min_y=Imin(map0->getminy(),map1->getminy());
  min_y=Imin(min_y,map2->getminy());
  min_y=Imin(min_y,map3->getminy());

  max_x=Imax(map0->getmaxx(),map1->getmaxx());
  max_x=Imax(max_x,map2->getmaxx());
  max_x=Imax(max_x,map3->getmaxx());

  max_y=Imax(map0->getmaxy(),map1->getmaxy());
  max_y=Imax(max_y,map2->getmaxy());
  max_y=Imax(max_y,map3->getmaxy());
  
  int imsizex=max_x-min_x+1;
  offsetx=min_x;
  int imsizey=max_y-min_y+1;
  offsety=min_y;
  image=new PXImage(imsizex,imsizey);
  
  for (int i=min_x;i<=max_x;i++)
    for (int j=min_y;j<=max_y;j++)
      {
	short p0=map1->getvalue(i,j);
	short p1=map1->getvalue(i,j);
	short p2=map2->getvalue(i,j);
	short p3=map2->getvalue(i,j);
	
	float d=float(p1);
	float c=float(p2-p0)/2.0;
	float e=float(p2);
	float f=float(p3-p1)/2.0;
	
	float a=(f-2*e)+c+2.0*d;
	float b=e-a-c-d;
	short p=short(a*pow(fr,float(3.0))+b*pow(fr,float(2.0))+c*fr+d+0.5);
	
	if (p==0)
	  p++;
	image->setspixel(i-offsetx,j-offsety,p);
      }
  contour=new PXContour();
  hasmap=1;
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
void PXChamferDistanceMap::setlevels(short obj,short out)
{
  if (obj<0)
    obj_gray=-obj;
  else
    obj_gray=obj;
  
  if (obj_gray>30000)
    obj_gray=30000;
  
  
  if (out>0)
    out_gray=-out;
  else
    out_gray=out;
  
  if (out_gray<-30000)
    out_gray=-30000;
  
  //  fprintf(stderr,"Levels : = obj_gray=%d, out_gray=%d\n",obj_gray,out_gray);
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
short PXChamferDistanceMap::getvalue(int i,int j)
{
  i=i-offsetx; j=j-offsety;
  
  if (i<0 || i>=image->getsizex() || j<0 || j>=image->getsizey())
    return out_gray;
  else
    return image->getspixel(i,j);
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
PXImage* PXChamferDistanceMap::getEnergyMap()
{
  short ps=1;
  PXImage* energy=new PXImage(image->getsizex(),image->getsizey());
  for (int i=0;i<image->getsizex();i++)
    for (int j=0;j<image->getsizey();j++)
      {
	short a=image->getspixel(i,j);
	if (a<-ps)
	  energy->setpixel(i,j,0);
	else if (a>ps)
	  energy->setpixel(i,j,2*ps);
	else 
	  energy->setpixel(i,j,a+ps);
      }
  return energy;
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
void PXChamferDistanceMap::initial_contour(int perturb,float pert_amount,int debug)
{
  if (debug)
    contour->Save("initial_contour.cntr");
  // 1. Make all points=obj_gray
  for(int ia=0;ia<image->getsizex();ia++)
    for(int ib=0;ib<image->getsizey();ib++)
      image->setspixel(ia,ib,obj_gray);
  
  // 2. Put Distance map for points on contour 
  if (!subpixel)
    {
      if (debug)
	fprintf(stderr,"Not sub-pixel\n");
      PXContourPoint *pts=contour->gettoppoint();
      while(pts->next !=NULL)
	{ 
	  float x=pts->x;	float y=pts->y;	
	  int i = int(x+0.5);	int j = int(y+0.5);
	  image->setspixel(i-offsetx,j-offsety,0);
	  pts=pts->next;      
	}
      return;
    }
  
  // SubPixel
  // Check whether contour is clockwise or anticlockwise
  // ---------------------------------------------------
  int np=contour->getnumpoints();
  int i=0;
  float xold,yold, v1x=0,v1y=0,v2x=0,v2y=0;
  PXContourPoint *pts=contour->gettoppoint();
  xold=pts->x;  yold=pts->y;

  if (debug)
    fprintf(stderr,"Doing sub-pixel\n");
  
  
  while(pts->next!=NULL)
    {
      i++;
      if (i==int(np/3))
	{
	  v1x=pts->x-xold;   v1y=pts->y-yold;
	  xold=pts->x; 	     yold=pts->y;
	}
      if (i==2*int(np/3))
	{
	  v2x=pts->x-xold;   v2y=pts->y-yold;
	}
      if (i==(np-1))
	{
	  xold=pts->x; yold=pts->y;
	}
      pts=pts->next;
    }
  
  float zz=(v1x*v2y-v2x*v1y);
  float anti=-1.0;
  if (zz>0.0) anti=1.0;
  float edge=float(pixel_size);

  pts=contour->gettoppoint();
  while(pts->next !=NULL)
    { 
      int lowx=0,highx=0,lowy=0,highy=0;
      float x=pts->x;	float y=pts->y;	
      if (x==float(int(x)) && y==float(int(y)))
	if(perturb==1)
	  {
	    double xnext,ynext;
	    if ((pts->next)->next == NULL)
	      { xnext=(contour->gettoppoint())->x;
	      ynext=(contour->gettoppoint())->y;
	      }
	    else
	      { xnext=(pts->next)->x;
	      ynext=(pts->next)->y;
	      }
	    double ny=xnext-xold;
	    double nx=yold-ynext;
	    double mag=sqrt(nx*nx+ny*ny);
	    if (mag>0.0)
	      { nx = nx/mag; ny=ny/mag; }    		    
	    if (debug)
	      fprintf(stderr,"(%5.2f,%5.2f)->",x,y);
	    x=x-pert_amount*anti*nx; y=y-pert_amount*anti*ny;
	    if (debug)
	      fprintf(stderr,"  (%5.2f,%5.2f)\n",x,y);
	    
	    pts->x=x; pts->y=y;
	  }
	else
	  {
	    if (x==float(int(x)))
	      lowx=1;
	    if (y==float(int(y)))
	      lowy=1;
	  }
      
      xold=x;yold=y;
      
      int x0=int(x);  int x1=int(x+1);
      int y0=int(y);  int y1=int(y+1);
      
      for(int i=x0-offsetx-lowx;i<=x1-offsetx+highx;i++)
	for (int j=y0-offsety-lowy;j<=y1-offsety+highy;j++)
	  {
	    if (debug)
	      {
		if (i<0 || j<0 || i>=image->getsizex() || j>=image->getsizey())
		  {
		    fprintf(stderr," Original (%d,%d) to (%d,%d) Current(%d,%d)\n",
			    x0,y0,x1,y1,i,j);
		    fprintf(stderr,"(Highs %d:%d) (Lows %d:%d) offsets(%d:%d)\n"
			    ,highx,highy,lowx,lowy,offsetx,offsety);
		  }
	      }
	    short val=image->getspixel(i,j);
	    short dist=short(0.5+edge*sqrt(pow(x-float(i+offsetx),float(2.0))+
					   pow(y-float(j+offsety),float(2.0))));
	    if (dist<abs(val))
	      {
		if (contour->IsInside(float(i+offsetx),float(j+offsety)))
		  {
		    image->setspixel(i,j,dist);
		    if (debug>1)
		      fprintf(stderr,"\t(%2d,%2d) %3d(%3d)",i+offsetx,j+offsety,
			      image->getspixel(i,j),dist);
		  }
		else
		  {
		    image->setspixel(i,j,-dist);
		    short a=image->getspixel(i,j);
		    if (debug>1)
		      fprintf(stderr,"\t(%2d,%2d) %3d(%3d)",i+offsetx,j+offsety,
			      a,-dist);
		    
		  }
	      }
	  }
      if (debug)
	fprintf(stderr,"\n");
      pts=pts->next;
    } 
  if (debug)
    contour->Save("shifted.cntr");
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
int PXChamferDistanceMap::initial_outside(int x,int y)
{ 
  int 	retval_x,retval_y;
  if ( x<0 || x>=image->getsizex() || y<0 || y>=image->getsizey())
    {
      return(0);
    }
  
  short val = image->getspixel(x,y);

  if ( abs(val)>(2*pixel_size)  && val!=out_gray)
    {
      image->setspixel(x,y,out_gray);
      for (int i=-1; i <= 1; i += 2) 
	{
	  x += i;
	  retval_x = initial_outside(x,y);
	  x -= i;
	  y += i;
	  retval_y = initial_outside(x,y);
	  y -= i;
	}
    }
  else 
    {
      return(1);
    }
  
  return(retval_x && retval_y);
  
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
void PXChamferDistanceMap::generateMap(int filtermode,int perturb,float per_amount)
{
  //fprintf(stderr,"Generating Map\n");
  initial_contour(perturb,per_amount,0);
  //  fprintf(stderr,"Initial Done\n");
  initial_outside();
  //  fprintf(stderr,"Outside Done\n");
  
  if (filtermode>=1)
    chamferfilter();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
// Original Code by Lei Zhang 
// Modified and corrected for sub-pixel
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
short makedistplus(short val,short add)
{
  if (val>0)
    return val+add;
  else
    return 32767;
}
  
short makedistminus(short val,short add)
{
  if (val<0)
    return val+add;
  else
    return -32767;
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - - 
void PXChamferDistanceMap::chamferfilter()
{
  //  fprintf(stderr,"Filter 3x3\n");
  short         temp[10];
  short         px1=pixel_size;
  short         px2=short(0.5+float(pixel_size)*sqrt(2.0)); 
  PXImage* tempimage=new PXImage(image);
  
  // Filter 1 Top-Right to Bottom Left 
  // saveImage("step0.pgm",1);
  int j;
  
  for (j=2; j<tempimage->getheight()-2; j++ ) 
    for (int i=2; i<tempimage->getwidth()-2 ; i++) 
      { 
	if ( tempimage->getspixel(i,j) > px2 )
	  {	
	    short immin=image->getspixel(i,j);
	    temp[0]=makedistplus(image->getspixel(i-1,j-1),px2);
	    temp[1]=makedistplus(image->getspixel(i  ,j-1),px1);
	    temp[2]=makedistplus(image->getspixel(i+1,j-1),px2);
	    temp[3]=makedistplus(image->getspixel(i-1,j  ),px1);
	    
	    for (int r=0;r<=3;r++)
	      {
		if (temp[r]<immin)
		  immin=temp[r];
	      }
	    image->setspixel(i,j,immin);
	  }
	else if ( tempimage->getspixel(i,j) < -px1 ) 
	  {
	    short immax=image->getspixel(i,j);
	    temp[0]=makedistminus(image->getspixel(i-1,j  ),-px1);
	    temp[1]=makedistminus(image->getspixel(i-1,j-1),-px2);
	    temp[2]=makedistminus(image->getspixel(i  ,j-1),-px1);
	    temp[3]=makedistminus(image->getspixel(i+1,j-1),-px2);
	    
	    for(int r=0;r<=3;r++)
	      {
		if (temp[r]>immax)
		  immax=temp[r];
	      }
	    image->setspixel(i,j,immax);
	  }
      }
  
  // Filter 2 from bottom right to top left
  // --------------------------------------
  //  fprintf(stderr,"Chamfer Step 2 %d %d\n",px1,px2);
  for (j=tempimage->getheight()-2; j>=2; j-- ) 
    for (int i=tempimage->getwidth()-2; i>=2; i-- ) 
      { 
	if ( tempimage->getspixel(i,j) > px2 )
	  {
	    short immin=image->getspixel(i,j);
	    temp[0]=makedistplus(image->getspixel(i-1,j+1),px2);
	    temp[1]=makedistplus(image->getspixel(i  ,j+1),px1);
	    temp[2]=makedistplus(image->getspixel(i+1,j+1),px2);
	    temp[3]=makedistplus(image->getspixel(i+1,j  ),px1);
	    
	    for (int r=0;r<=3;r++)
	      {
		if (temp[r]<immin)
		  immin=temp[r];
	      }
	    //immin=Imin(temp[r],immin);
	    image->setspixel(i,j,immin);
	  }
	else if ( tempimage->getspixel(i,j) < -px1 ) 
	  {
	    short immax=image->getspixel(i,j);
	    temp[0]=makedistminus(image->getspixel(i-1,j+1),-px2);
	    temp[1]=makedistminus(image->getspixel(i  ,j+1),-px1);
	    temp[2]=makedistminus(image->getspixel(i+1,j+1),-px2);
	    temp[3]=makedistminus(image->getspixel(i+1,j  ),-px1);
	    
	    for(int r=0;r<=3;r++)
	      {
		if (temp[r]>immax)
		  immax=temp[r];
	      }
	    //    immax=Imax(temp[r],immax);
	    image->setspixel(i,j,immax);
	  }
      }
  // saveImage("step2.pgm",1);
  delete tempimage;
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
int PXChamferDistanceMap::findexit(int m,int n,float xin,float yin,
				   float& xout,float& yout,
				   int& mout,int& nout,int )
{
  int edge=1,found=0;

  // Determine Starting Edge , Clockwise
  // -----------------------------------
  if (xin==float(m))
      edge=1;
  else if(yin==float(n))
      edge=2;
  else if(xin==float(m+1))
      edge=3;
  else 
      edge=4;


  while(edge <=4 && found==0)
    {
      
      int i=0,j=0,ip=0,jp=0,shifti=0,shiftj=0;
      switch(edge)
	{
	case 1:  
	  i=0;j=0;ip=1;jp=0; shifti=0;shiftj=-1;    
	  break;
	case 2:  
	  i=1;j=0;ip=1;jp=1; shifti=1;shiftj=0;
	  break;
	case 3:  
	  i=1;j=1;ip=0;jp=1; shifti=0;shiftj=1;
	  break;
	case 4:  
	  i=0;j=1;ip=0;jp=0; shifti=-1;shiftj=0;
	  break;
	}
      
      short v0=image->getspixel(m+i ,n+j);
      short v1=image->getspixel(m+ip,n+jp);
      
      if (v0*v1 < 0)
	{
	  // Change of Sign Indicates zero crossing 
	  
	  float xm,ym;
	  float x0=float(m+i), x1=float(m+ip);
	  float y0=float(n+j), y1=float(n+jp);	
	  
	  xm=( x1*v0-x0*v1)/(v0-v1);
	  ym=( y1*v0-y0*v1)/(v0-v1);
	  
	  if (xm!=xin || ym!=yin)
	    {
	      xout=xm;		    yout=ym;
	      mout=m+shifti;	    nout=n+shiftj;
	      found=1;
	    }
	}
      edge++;
      if (edge>4)
	edge=1;
    }
  if (found==0)
    fprintf(stderr, "Error\n");
  
  return found;
}

int  PXChamferDistanceMap::validcell(int x,int y,int )
{
  // Cell is Valid is some +ve and some -ve

  int positive=0,negative=0;
  for(int i=0;i<=1;i++)
    for(int j=0;j<=1;j++)
      {
	short px=image->getspixel(x+i,y+j);
	if (px<0)
	  negative++;
	if (px>0)
	  positive++;
      }

  if (positive>0 && negative>0)
    return(1);

  return(0);


}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
PXContour* PXChamferDistanceMap::extractContour()
{
  //static int no=1;

  int starti=0,startj=0,i=0,found=0;
  // Find First Point in cell to make sense 
  while(i<image->getsizex()-1 && found==0)
    {
      int j=0;
      while(j<image->getsizey()-1 && found==0)
	{
	  if (validcell(i,j)==1)
	    {
	      starti=i;startj=j;
	      found=1;
	    }
	  j++;
	}
      i++;
    }
  
  PXContour* cntr=new PXContour;
  if (found==0)
    {
      fprintf(stderr,"No Contour Found ... \n");
      return cntr;
    }
  float startx=0.0,starty=0.0,oldx=0.0,oldy=0.0,newx=0.0,newy=0.0;
  int   oldi=0,oldj=0,newi=0,newj=0;
  
  if (!findexit(starti,startj,0.0,0.0,startx,starty,newi,newj,0))
    {
      fprintf(stderr,"No Contour Found ... \n");
      return cntr;
    }
  cntr->Add(startx+float(offsetx),starty+float(offsety));
  int stop=0; 
  newx=startx;
  newy=starty;
  
  while(stop==0)
    {
      oldx=newx; oldy=newy;
      oldi=newi; oldj=newj;
      if(!findexit(oldi,oldj,oldx,oldy,newx,newy,newi,newj,0))
	{
	  fprintf(stderr,"No Contour Found ... \n");
	  return cntr;
	}
      if (newx==startx && newy==starty)
	stop=1;
      else
	cntr->Add(newx+float(offsetx),newy+float(offsety));
    }
  
  // cerr << "Closing : " << startx << "," << starty << ":" << newx << " " << newy << "\n";
  
  /*  char line[100];
      sprintf(line,"extract%02d.cntr",no);
      no++;
      cntr->Save(line);
      fprintf(stderr,"Saved in %s (%d)\n",line,cntr->getnumpoints());*/
  
  return cntr;
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
void PXChamferDistanceMap::saveImage(const char* fname,int binary)
{
  PXImage* im2=new PXImage(image);
  
  if (binary==1)
    {
      for(int i=0;i<im2->getsizex();i++)
	for(int j=0;j<im2->getsizey();j++)
	  {
	    short px=im2->getspixel(i,j);
	    if (px<-1280)
	      im2->setpixel(i,j,0);
	    else if (px>1280)
	      im2->setpixel(i,j,255);
	    else
	      {
		short a=128+px/10;
		im2->setpixel(i,j,a);
	      }
	  }
      im2->SavePgm(fname);
    }
  else
    {
      FILE* fout=fopen(fname,"w");
      if (fout!=NULL)
	{
	  for(int j=0;j<im2->getsizey();j++)
	    {
	      fprintf(fout,"%4d  ",j-offsety);
	      if (binary==-1)
		{
		  for(int i=0;i<im2->getsizex();i++)
		    fprintf(fout,"%5d  ",image->getspixel(i,j));
		  fprintf(fout, "\n");
		}
	      else
		{
		  
		  fprintf(fout,"%5d\n",image->getspixel(image->getsizex()/2,j));
		}
	    }
	}
      fclose(fout);
    }
  delete im2;
}

