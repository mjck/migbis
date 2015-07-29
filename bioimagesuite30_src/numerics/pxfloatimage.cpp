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

#include "pxfloatimage.h"
#include "pximage.h"
#include "pxfloatmatrix.h"

//-----------------------------------------------------------------------
float PXFloatImage::gradient_mask[5]={-1.0/12.0 , 8.0/12.0 ,0.0 ,-8.0/12.0 ,1.0/12.0};
//-----------------------------------------------------------------------
PXFloatImage::PXFloatImage(int x ,int y)
{
  xsize=x;
  ysize=y;
  image=new float[xsize*ysize]; 
  if (image!=NULL) status=1;
  else
      status=0;
}
//-----------------------------------------------------------------------
PXFloatImage::PXFloatImage(const char* f)
     /* 
	Normal constructor.If file f exists it is read into
	the array.The fields are initialised to their default
	values
	*/
{
  xsize=0;
  ysize=0;
  maxgrayscale=0;
  mingrayscale=0;
  status=0;	      
  if (status==1) 
      Load(f);
}

//-----------------------------------------------------------------------
PXFloatImage::PXFloatImage(PXImage* oldimage,PXFloatMatrix* kernel,
			   int x1,int x2,int y1,int y2)
     // Convolution Constructor
{
  int i,j;
  if (x2==-1) x2=oldimage->getsizex();
  if (y2==-1) y2=oldimage->getsizey();
  
  xsize=oldimage->getsizex();            ysize=oldimage->getsizey();
  status=0;
  image=new float[xsize*ysize]; 
  if (image!=NULL) status=1;
  else
      status=0;
  
  if (status==1)
      {
	for(i=0;i<xsize;i++)
	  for(j=0;j<ysize;j++)
	      setpixel(i,j,0.0);
	
	float sum=0.0;
	
	for(i=kernel->getradiusx()+x1+1;
	    i<(x2-kernel->getradiusx());i++)
	    for(j=kernel->getradiusy()+1+y1;
		j<(y2-kernel->getradiusy()-1);j++)
		{
		  sum=0.0;
		  for(int column=-kernel->getradiusx();
		      column<=kernel->getradiusx();column++)
		      for(int row=-kernel->getradiusy();
			  row<=kernel->getradiusy();row++)
			  {
			    sum=sum+((float)(oldimage->getpixel(i+column,j+row))*
				     kernel->getpixel(kernel->getradiusx()-column,
						      kernel->getradiusy()-row));
			  }
		  setpixel(i,j,sum);
	      }
	SetMaxMingrayscale();
      }
}

//-----------------------------------------------------------------------
PXFloatImage::PXFloatImage(PXFloatImage *oldimage)
{
  xsize=oldimage->getsizex();
  ysize=oldimage->getsizey();
  image=new float[xsize*ysize]; 
  for (int i=0;i<xsize;i++)
      for (int j=0;j<ysize;j++)
	  setpixel(i,j,oldimage->getpixel(i,j));
  status=oldimage->isvalid();
}
//-----------------------------------------------------------------------

PXFloatImage::PXFloatImage(PXFloatImage* oldimage,PXFloatMatrix* kernel,
			   int x1,int x2,int y1,int y2)
     // Convolution Constructor
{
  int i,j;
  if (x2==-1) x2=oldimage->getsizex();
  if (y2==-1) y2=oldimage->getsizey();
  
  xsize=oldimage->getsizex();            ysize=oldimage->getsizey();
  status=0;
  image=new float[xsize*ysize]; 
  if (image!=NULL) status=1;
  else
      status=0;
  
  if (status==1)
      {
	for(i=0;i<xsize;i++)
	    for(j=0;j<ysize;j++)
		setpixel(i,j,0.0);
	
	float sum=0.0;
	
	for(i=kernel->getradiusx()+x1+1;
	    i<(x2-kernel->getradiusx());i++)
	    for(j=kernel->getradiusy()+y1+1;
		j<(y2-kernel->getradiusy()-1);j++)
		{
		  sum=0.0;
		  for(int column=-kernel->getradiusx();
		      column<=kernel->getradiusx();column++)
		      for(int row=-kernel->getradiusy();
			  row<=kernel->getradiusy();row++)
			  {
			    sum=sum+(oldimage->getpixel(i+column,j+row)*
				     kernel->getpixel(kernel->getradiusx()-column,
						      kernel->getradiusy()-row));
			  }
		  setpixel(i,j,sum);
		}
	SetMaxMingrayscale();
      }
}

//-----------------------------------------------------------------------
PXFloatImage::~PXFloatImage()
{
  delete [] image;
}
//-----------------------------------------------------------------------
/*
		The  Format used for Float Images .xbin

Magic     Number  (4-byte float)    
Number of Columns (4-byte float)
Number of Rows    (4-byte float)
Then data in 4-byte float
*/
//-----------------------------------------------------------------------
void PXFloatImage::Load(const char* filename)
{
  int n=0;
  float test;
  
  gzFile fp=gzsuffixopen(filename,"rb");
  if (fp)
      {
	n += gzread(fp,&test,4);
	if (test==PXFLIM)
	    {
	      n += gzread(fp,&ysize,4);
	      n += gzread(fp,&xsize,4);
	      image=new float[xsize*ysize]; // initialise image array 
	      n +=gzread(fp,&image[0],4*xsize*ysize);
	      gzclose(fp);
	      SetMaxMingrayscale();
	      status=1;
	    }
	else
	    {
	      //cerr << "Wrong File format : " << filename << "\n";
	      status=0;
	    }
      }
  else 
      {
	//cerr << "File " << filename <<" does not exist \n";
	status=0;
      }
}
//-----------------------------------------------------------------------
void PXFloatImage::Save(const char* filename)
{
  int bytes=0;
  gzFile fdf=gzsuffixopen(filename,"rb");
  if (fdf)
      {
	float a=PXFLIM;
	bytes +=gzwrite(fdf,&a,4);
	bytes +=gzwrite(fdf,&ysize,4);
	bytes +=gzwrite(fdf,&xsize,4);
	bytes +=gzwrite(fdf,&image[0],4*xsize*ysize);
      }
  gzclose(fdf);
}
//-----------------------------------------------------------------------
void PXFloatImage::LoadBin(const char* filename)
{
  int n=0;
  

  gzFile fp=gzsuffixopen(filename,"rb");
  if (fp)
      {
	n +=gzread(fp,&image[0],xsize*ysize*4);
	SetMaxMingrayscale();
	status=1;
	gzclose(fp);
      }
  else 
      {
	status=0;
      }
}
//-----------------------------------------------------------------------

void PXFloatImage::SaveBin(const char* filename)
{
  int bytes=0;
  gzFile fdf=gzsuffixopen(filename,"wb");
  if (fdf)
      {
	bytes += gzwrite(fdf,&image[0],4*xsize*ysize);
      }
  gzclose(fdf);
}
//-----------------------------------------------------------------------
PXImage* PXFloatImage::getPXImage(int bytes)
{
  int maxpix=65535;
  if (bytes==1)
      maxpix=255;
  PXImage* temp=new PXImage(xsize,ysize);
  temp->nowisvalid();
  SetMaxMingrayscale();
  for (int i=0;i<xsize;i++)
      for (int j=0;j<ysize;j++)
	  temp->setpixel(i,j,int(maxpix*(getpixel(i,j)-mingrayscale)/getrange()));
  return temp;
}
//-----------------------------------------------------------------------
void PXFloatImage::SaveGrl(const char* filename)
{
  PXImage* temp=getPXImage();
  temp=getPXImage(2);
  temp->SaveGrl(filename,2);
  delete temp;
}
//-----------------------------------------------------------------------
void PXFloatImage::SavePgm(const char* filename)
{
  PXImage* temp=getPXImage(1);
  temp->SavePgm(filename);
  delete temp;
}

//-----------------------------------------------------------------------
// Return texture mapping code
//-----------------------------------------------------------------------
unsigned char PXFloatImage::adjust(float px,float gamma)
{ 
  float max=maxgrayscale;
  float xf=px/max;
  xf=pow((double)xf,1.0/(double)gamma);
  xf=xf*255.0;
  return ((unsigned char)xf);
}

// ----------------------------------------------------------------------
unsigned char* PXFloatImage::getslice(float gamma,int numbytes)
{
  PXImage* temp=getPXImage(1);
  unsigned char* sl=temp->getslice(gamma,numbytes);
  delete temp;
  return sl;
}
//-----------------------------------------------------------------------
void PXFloatImage::SetMaxMingrayscale()
{
  float max=0,min=1e+20;
  
  for (int i=0;i<(xsize*ysize);i++)
      {
	if (image[i]>max) max=image[i];
	if (image[i]<min) min=image[i];
      }
  mingrayscale=min;
  maxgrayscale=max;
}

//-----------------------------------------------------------------------
void PXFloatImage::Invert()
{
  SetMaxMingrayscale();
  float maxmin=maxgrayscale-mingrayscale;

  for (int i=0;i<(xsize*ysize);i++)
      image[i]=(maxmin-(image[i]-mingrayscale))+mingrayscale;
}
//-----------------------------------------------------------------------
void PXFloatImage::AddContour(PXContour* contour,float grl,
			      unsigned char touching,
			      int offx,int offy)
{
  int x1,x2,y1,y2;
  float w11,w12,w21,w22;

  PXContour* modcontour=new PXContour(contour);
  modcontour->closedcontour(contour->isclosed());
  if (touching>0) modcontour->Touching();
  PXContourPoint *pts=modcontour->gettoppoint();
  while (pts->next !=NULL)
    {
      if ( (pts->x>0.0 && pts->x<xsize-1) &&
	  (pts->y>0.0 && pts->y<ysize-1))
	  {
	    x1=int(pts->x); x2=x1+1;
	    y1=int(pts->y); y2=x2+1;
	    
	    w11=(x2-pts->x)*(y2-pts->y)*grl;
	    w12=(x2-pts->x)*(pts->y-y1)*grl;
	    w21=(pts->x-x1)*(y2-pts->y)*grl;
	    w22=(pts->x-x1)*(pts->y-y2)*grl;
	    
	    setpixel(offx+x1,offy+y1,w11);
	    setpixel(offx+x2,offy+y1,w21);
	    setpixel(offx+x1,offy+y2,w12);
	    setpixel(offx+x2,offy+y2,w22);
	  }
      pts=pts->next;
    }
  delete modcontour;
}

//-----------------------------------------------------------------------
float PXFloatImage::getinterp(float x,float y)
{ 
  int x1=(int)x,y1=(int)y;
  int x2=x1+1,y2=y1+1;
  if (x1>=0 && y1>=0 && x2<xsize && y2<ysize)
      {
	float a= (getpixel(x1,y1)*(x2-x)*(y2-y)+
		  getpixel(x1,y2)*(x2-x)*(y-y1)+
		  getpixel(x2,y1)*(x-x1)*(y2-y)+
		  getpixel(x2,y2)*(x-x1)*(y-y1));
	return a;
      }
  else
      {
	if (x2>=xsize) { x2=x1;}
	if (y2>=ysize) { y2=y1;}
	
	if (x1==x2 && y1==y2)
	    return getpixel(x1,y1);
	if (x1==x2)
	    return getpixel(x1,y1)*(y2-y)+getpixel(x1,y2)*(y-y1);
	if (y1==y2)
	    return getpixel(x1,y1)*(x2-x)+getpixel(x2,y1)*(x-x1);
      }
  return 0.0;
}
//-----------------------------------------------------------------------
//  Gradient Related Code
//-----------------------------------------------------------------------  
float PXFloatImage::GradientX(int x,int y)
{ 
  return getpixel(x+1,y)-getpixel(x,y); 
}

float PXFloatImage::GradientY(int x,int y)
{ 
  return getpixel(x,y+1)-getpixel(x,y);  
}
  
float PXFloatImage::GradientXMask(int x,int y)
{
  float sum=0;
  for(int i=-2;i<=2;i++)
      sum=sum+(gradient_mask[i+2]*getpixel(x+i,y));
  return sum;
}

float PXFloatImage::GradientYMask(int x,int y)
{
  float sum=0;
  for (int i=-2;i<=2;i++)
      sum=sum+(gradient_mask[i+2]*getpixel(x,y+i));
  return sum;
}
//-----------------------------------------------------------------------

