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

#include "pximage.h"
#include <fcntl.h>
#include <stdlib.h>
#include "pxfloatmatrix.h"

#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#endif

#ifdef __CYGWIN__
#include <unistd.h>
#endif

#ifdef __APPLE__
 #include "/usr/include/unistd.h"
#endif



/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July 1995   papad@noodle.med.yale.edu

   See pximage.h for details

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
//-----------------------------------------------------------------------
float PXImage::gradient_mask[5]={-1.0/12.0 , 8.0/12.0 ,0.0 ,-8.0/12.0 ,1.0/12.0};
//-----------------------------------------------------------------------
PXImage::PXImage(int x ,int y)                
{
  xsize=x;   ysize=y;  status=0;
  filename=new char[1];
  setfilename("default.grl16"); 
  image=new unsigned short[xsize*ysize]; 
  status=1;
}

//-----------------------------------------------------------------------

PXImage::PXImage(const char* f,int x ,int y)  // Normal Constructor II;
{
  xsize=x;  ysize=y;  status=0;
  image=new unsigned short[xsize*ysize]; 
  filename=new char[1];
  setfilename(f);
  CheckFilename(filename,2);
}

//-----------------------------------------------------------------------

PXImage::PXImage(const char* f,int pxconst)           // Load Constrctor;
	
{
  xsize=0;   ysize=0;  maxgrayscale=0;    status=0;		
  filename=new char[1];

  if (pxconst==PXIm_Scil)
      {
	char *test=new char[256];
	setfilename(f,1);
	sprintf(test,"%s.ics",filename);
	CheckFilename(test,1);                // Check Header File First;
	if (status==1)
	    {
	      sprintf(test,"%s.ids",filename); // Check Data File;
	      CheckFilename(test,1);
	      if (status==1) LoadScil();
	    }
	delete [] test;
      }
  else 
      {
	setfilename(f);
	CheckFilename(filename,1);
	if (status==1)
	    {
	      switch (pxconst)
		  {
		  case PXIm_Pgm:
		    LoadPgm();   break;
		  case PXIm_grl:
		    LoadGrl(16); break;
		  case PXIm_grl8:
		    LoadGrl(8);  break;
		  }
	    }
      }
}

//-----------------------------------------------------------------------

PXImage::PXImage(PXImage* oldimage)
{
  filename=new char[1];
  status=0;
  xsize=oldimage->getsizex();    
  ysize=oldimage->getsizey();
  setfilename(oldimage->getfilename());
  image=new unsigned short[xsize*ysize]; 
  
  for(int i=0;i<oldimage->getsizex();i++)
      for(int j=0;j<oldimage->getsizey();j++)
	  setpixel(i,j,oldimage->getpixel(i,j));
  
  SetMaxgrayscale();   
  status=oldimage->isvalid();
}
 
//-----------------------------------------------------------------------

PXImage::PXImage(PXImage* oldimage,PXFloatMatrix* kernel)
     // Convolution Constructor

{
  filename=new char[1];
  xsize=oldimage->getsizex();            
  ysize=oldimage->getsizey();
  PXFloatMatrix* ResultsImage= new PXFloatMatrix(oldimage->getsizex(),
						 oldimage->getsizey());
  for(int ia=0;ia<xsize;ia++)
      for(int ja=0;ja<ysize;ja++)
	  ResultsImage->setpixel(ia,ja,0.0);
  
  float sum=0.0;
  
  fprintf(stderr,"Doing convolution ...");
  for(int i=kernel->getradiusx();
      i<(ResultsImage->getsizex()-kernel->getradiusx());i++)
      for(int j=kernel->getradiusy()+1;
	  j<(ResultsImage->getsizey()-kernel->getradiusy()-1);j++)
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
	    ResultsImage->setpixel(i,j,sum);
	  }
  fprintf(stderr,"\n");
  ConvFloat(ResultsImage);              delete ResultsImage;
  setfilename(oldimage->getfilename());
 }

//-----------------------------------------------------------------------
PXImage::PXImage(unsigned const char* img,int x ,int y)   
{
  filename=new char[1];
  xsize=x;   ysize=y;  status=0;
  setfilename("default.grl16"); 
  image=new unsigned short[xsize*ysize]; 
  for (int i=0;i<xsize;i++)
      for (int j=0;j<ysize;j++)
	  setpixel(i,j,img[i+j*xsize]);
  status=1;
}

//-----------------------------------------------------------------------

PXImage::~PXImage()

{
  delete [] image;
  delete [] filename;
}

//-----------------------------------------------------------------------
void PXImage::setfilename(const char * fname,int removesuffix)
{
  
  if (filename)
      delete [] filename;
  
  if (removesuffix)
      {
	filename=eatsuffix(fname);
      }
  else
      {
	filename=new char[strlen(fname)];
	strcpy(filename,fname);
      }
}



//-----------------------------------------------------------------------

void PXImage::ConvFloat(PXFloatMatrix* fim)
{
  xsize=fim->getsizex();                 ysize=fim->getsizey();
  float fmax=-1e+10,fmin=1e+10;
  
  for(int ii=0;ii<xsize;ii++)
    for(int jj=0;jj<ysize;jj++)
      {
	if (fim->getpixel(ii,jj)>fmax) fmax=fim->getpixel(ii,jj);
	if (fim->getpixel(ii,jj)<fmin) fmin=fim->getpixel(ii,jj);
      }
  
  image=new unsigned short[xsize*ysize]; 
  unsigned short temp;
  for(int i=0;i<xsize;i++)
    for(int j=0;j<ysize;j++)
      {
	temp=(unsigned short)((fim->getpixel(i,j)-fmin)*
			      (65535.0/(fmax-fmin)));
	setpixel(i,j,temp);
      }
  status=1;
  SetMaxgrayscale();  
}
//-----------------------------------------------------------------------
// Return texture mapping code
//-----------------------------------------------------------------------

unsigned char PXImage::adjust(float px,float gamma)
{ 
  float max=(float)maxgrayscale;
  float xf=px/max;
  xf=pow((double)xf,1.0/(double)gamma);
  xf=xf*255.0;
  return ((unsigned char)xf);
}

// ----------------------------------------------------------------------
unsigned char* PXImage::getslice(float gamma,int numbytes)
{
  SetMaxgrayscale();
  numbytes=Irange(numbytes,1,2);
  unsigned char *imagexy=new unsigned char[getsizex()*getsizey()*numbytes];
  for (int iy=0;iy<getsizey();iy++)
      for (int ix=0;ix<getsizex();ix++)
	  {
	    imagexy[ix*numbytes+iy*getsizex()*numbytes]=adjust(float(getpixel(ix,iy)),gamma);
	    if (numbytes==2)
		imagexy[ix*numbytes+iy*getsizex()*numbytes+1]=255;
	  }
  return imagexy;
}
// ----------------------------------------------------------------------
unsigned char* PXImage::getpaddedslice(int &width,int &height, float gamma)
{
  int xs=2;
  while (xs<getsizex()) 
      xs=2*xs;
  
  int ys=2;
  while (ys<getsizey())
      ys=2*ys;

  unsigned char *oldslice=getslice(gamma);
  unsigned char *texture=new unsigned char[xs*ys];
  for (int j=0;j<ys;j++)
      for (int i=0;i<xs;i++)
	  {
	    texture[i+j*xs]=0;
	    if (j<getsizey() && i<getsizex())
		texture[i+j*xs]=oldslice[i+j*getsizex()];
	  }
  delete [] oldslice;
  width=xs;
  height=ys;
  return texture;
}
// ----------------------------------------------------------------------
unsigned char* PXImage::scaleslice(int &width,int &height,unsigned char* oldslice,int numbytes,
				   int scalex,int scaley)
{
  numbytes=Irange(numbytes,1,2);
  scalex=Irange(scalex,1,3);
  scaley=Irange(scaley,1,3);

  if (scalex==3) scalex=4;
  if (scaley==3) scaley=4;

  int xs=2;
  while (xs<width) 
      xs=2*xs;
  
  int ys=2;
  while (ys<height)
      ys=2*ys;
  
  if (scalex==1 && scaley==1)
      {
	int shiftx=(xs-width)/2;
	int shifty=(ys-height)/2;
	unsigned char *texture=new unsigned char[xs*ys*numbytes];
	for (int j=0;j<ys;j++)
	    for (int i=0;i<xs;i++)
		{
		  if (numbytes==1)
		      {
			texture[i+j*xs]=0;
			if (j>=shifty && j<(height+shifty) && 
			    i>=shiftx && i<(width+shiftx))
			    texture[i+j*xs]=oldslice[(i-shiftx)+(j-shifty)*width];
		      }
		  else
		      {
			texture[i*numbytes+j*xs*numbytes]=255;
			if (j>=shifty && j<(height+shifty) && 
			    i>=shiftx && i<(width+shiftx))
			    {
			      texture[i*numbytes+j*xs*numbytes]=
				  oldslice[(i-shiftx)*numbytes+(j-shifty)*width*numbytes];
			      texture[i*numbytes+j*xs*numbytes+1]=
				  oldslice[(i-shiftx)*numbytes+(j-shifty)*width*numbytes+1];
			    }
			else
			    texture[i*numbytes+j*xs*numbytes+1]=0;
		      }
		}
	width=xs;
	height=ys;
	return texture;
      }
  else
      {
	int shiftx=(xs-width)/2;
	int shifty=(ys-height)/2;

	/*fprintf(stderr,"In xs=%d ys=%d scalex=%d scaley=%d shiftx=%d shifty=%d\n",
		xs,ys,scalex,scaley,shiftx,shifty);*/

	xs=xs/scalex; 
	ys=ys/scaley;

	unsigned char *texture=new unsigned char[xs*ys*numbytes];

	/*fprintf(stderr,"Out xs=%d ys=%d scalex=%d scaley=%d shiftx=%d shifty=%d\n",
		xs,ys,scalex,scaley,shiftx,shifty);*/

	for (int j=0;j<ys;j++)
	    {
	      int tj=j*scaley;
	      for (int i=0;i<xs;i++)
		  {
		    int ti=i*scalex;
		    if (numbytes==1)
			{
			  texture[i+j*xs]=0;
			  /*if (i==xs/2 && j==ys/2)
			    fprintf(stderr,"Point: i,j=(%d,%d) ti,tj=(%d,%d)(xs=%d,ys=%d)\n",i,j,ti,tj,xs,ys);
			    */
			  if (tj>=shifty && tj<(height+shifty) && 
			      ti>=shiftx && ti<(width+shiftx))
			      {
				int sum=0;
				for (int ia=0;ia<scalex;ia++)
				    for (int ib=0;ib<scaley;ib++)
					{
					  int ix=Irange(ti+ia-shiftx,0,width-1);
					  int iy=Irange(tj+ib-shifty,0,height-1);
					  /*if (i==xs/2 && j==ys/2)
					    fprintf(stderr,"Adding (%d,%d)\n",ix,iy);*/
					  sum+=oldslice[ix+iy*width];
					}
				texture[i+j*xs]=int(float(sum)/float(scalex*scaley));
			      }
			}
		    else
			{
			  texture[i*numbytes+j*xs*numbytes]=255;
			  if (tj>=shifty && tj<(height+shifty) && 
			      ti>=shiftx && ti<(width+shiftx))
			      {
				int sum=0;
				for (int ia=0;ia<scalex;ia++)
				    for (int ib=0;ib<scaley;ib++)
					{
					  int ix=Irange(ti+ia-shiftx,0,width-1);
					  int iy=Irange(tj+ib-shifty,0,height-1);
					  sum+=oldslice[(ix+iy*width)*numbytes];
					  if (ia==0 && ib==0)
					      texture[i*numbytes+j*xs*numbytes+1]=
						  oldslice[(ix+iy*width)*numbytes+1];
					}
				texture[i*numbytes+j*xs*numbytes]=int(float(sum)/float(scalex*scaley));
			      }
			  else
			      texture[i*numbytes+j*xs*numbytes+1]=0;
			}
		  }
	    }
	width=xs;
	height=ys;
	return texture;
      }
}
//-----------------------------------------------------------------------
// Interpolation Code
//-----------------------------------------------------------------------
float PXImage::getinterp(float x,float y)
{
  int ok=1;

  if (x<0.0) 
      { x=0.0; ok=0; }
  if (y<0.0) 
      { y=0.0; ok=0; }
  if (x>=xsize-1) 
      {	x=float(xsize-1); ok=0; }
  if (y>=ysize-1) 
      { y=float(ysize-1); ok=0; }


  int x1=(int)x;
  int y1=(int)y;

  if (ok==0)
      return (float)getpixel(x1,y1);

  int x2=x1+1;
  int y2=y1+1;

  return ((float)getpixel(x1,y1)*(x2-x)*(y2-y)+
	  (float)getpixel(x1,y2)*(x2-x)*(y-y1)+
	  (float)getpixel(x2,y1)*(x-x1)*(y2-y)+
	  (float)getpixel(x2,y2)*(x-x1)*(y-y1));

  
}
int PXImage::getbilinear(unsigned short* px,float x,float y)
{ 
  int x1=(int)x,y1=(int)y;
  int x2=x1+1,y2=y1+1;
  return (int)((float)getpixel(px,x1,y1)*(x2-x)*(y2-y)+
	       (float)getpixel(px,x1,y2)*(x2-x)*(y-y1)+
	       (float)getpixel(px,x2,y1)*(x-x1)*(y2-y)+
	       (float)getpixel(px,x2,y2)*(x-x1)*(y-y1));
}

int PXImage::getbilinear(float x,float y)
{ 
  int x1=(int)x,y1=(int)y;
  int x2=x1+1,y2=y1+1;
  
  if (x1>=0 && y1>=0 && x2<xsize && y2<ysize)
      return (int)((float)getpixel(x1,y1)*(x2-x)*(y2-y)+
		   (float)getpixel(x1,y2)*(x2-x)*(y-y1)+
		   (float)getpixel(x2,y1)*(x-x1)*(y2-y)+
		   (float)getpixel(x2,y2)*(x-x1)*(y-y1));
  else
      return 0;
}

//-----------------------------------------------------------------------
//           Return image in unsigned char array -- this has many uses
//-----------------------------------------------------------------------

unsigned char*  PXImage::getimage()
{
  SetMaxgrayscale();
  float conv8bit= (255.0)/(float)maxgrayscale;
  if (maxgrayscale<256) conv8bit=1.0;
  unsigned char* eightbit=new unsigned char[xsize*ysize];
  for (int j=0;j<ysize;j++)
      for (int i=0;i<xsize;i++)
	  eightbit[j*xsize+i]=(unsigned char)(conv8bit*(float)getpixel(i,j));
  return eightbit;
}


//-----------------------------------------------------------------------
//  I/O Related Code
//-----------------------------------------------------------------------
void PXImage::CheckFilename(const char* f,int mode)

{
  status=1;
  switch (mode) 
    {
    case 1 :
      {
	FILE* fin=fopen(f,"r");
	if (!fin) status=0;
	fclose(fin);
	break;
      }
    case 2 :
      {
	FILE* fout=fopen(f,"w");
	if (!fout) status=0;
	fclose(fout);
	break;
      }
    }
}

//-----------------------------------------------------------------------
/*
		The  Format used for Float Images .grl16

Magic     Number  (2-byte unsigned short)
Number of Columns (2-byte unsigned short)
Number of Rows    (2-byte unsigned short)
Then data in 2-byte unsigned short

For 8-bit data in 1-byte unsigned short .grl
*/

//-----------------------------------------------------------------------
int PXImage::Load()
{ 
  return LoadGrl(16); 
}

int PXImage::Save(const char* fname)
{
  setfilename(fname);
  return Save();
}

int PXImage::Save()
{
  return SaveGrl(16); 
}

// ---------------------------------------------------------------------
int PXImage::LoadGrl(int bits)
{
  long n=0;
  unsigned short test;
  bits=8+8*(bits>8);

  gzFile fp=gzsuffixopen(filename,"rb");
  if (fp)
    {
      n += gzread(fp,&test,2);
      if (test==PXIM)
	{
	    n += gzread(fp,&ysize,4);
	    n += gzread(fp,&xsize,4);
	    image=new unsigned short[ysize*xsize];      
	    if (bits==16)
		{
		    n +=gzread(fp,&image[0],xsize*ysize*2);
		    if (n==(ysize*xsize*2+10))
			{
			  fprintf(stderr,"File %s read (%d,grl16)\n",filename ,(int)n);
			  SetMaxgrayscale();
			  status=1;
			}
		    else
			{
			    if (n==(ysize*xsize+10))
				{
				    // Rewind File
				  n=(long)gzseek(fp,10,0);
				  bits=8; // It was really an 8-bit file

				}
			    else
				{
				  status=0;
				  fprintf(stderr,"Failed Reading 16-bit grl file\n");
				}
			}
		}
	    
	    if (bits==8)
		{
		  unsigned char* temp=new unsigned char[xsize*ysize];
		  n +=gzread(fp,&temp[0],xsize*ysize);
		  if (n==xsize*ysize+10)
		      {
			for (int j=0;j<ysize;j++)
			    for (int i=0;i<xsize;i++)
				setpixel(i,j,temp[j*xsize+i]);
			fprintf(stderr,"File %s read (%d,grl)\n",filename ,(int)n);
			SetMaxgrayscale();
			status=1;
		      }
		  else
		      {
			fprintf(stderr,"Failed Reading 8-bit grl file\n");
			status=0;
		      }
		  delete [] temp;
	      }
	}
      else
	  fprintf(stderr, "Not a grl file \n");
      
      gzclose(fp);
    }
  else 
      {
	fprintf(stderr, "File %s does not exist \n",filename);
	status=0;
      }
  return status;
}

//-----------------------------------------------------------------------
int PXImage::SaveGrl(const char* fname,int bits)
{
  setfilename(fname);
  return SaveGrl(bits);
}

int PXImage::SaveGrl(int bits)
{
  long bytes=0;
  gzFile fdf=gzsuffixopen(filename,"wb");
  if (fdf)
      {
	unsigned short a=PXIM;
	bytes +=gzwrite(fdf,&a,2);
	bytes +=gzwrite(fdf,&ysize,4);
	bytes +=gzwrite(fdf,&xsize,4);
	
	if (bits==16)
	    bytes += gzwrite(fdf,&image[0],2*xsize*ysize);
	if (bits==8)
	    {
	      SetMaxgrayscale();
	      float conv8bit= (255.0)/(float)maxgrayscale;
	      if (maxgrayscale<256) conv8bit=1.0;
	      unsigned char* temp=new unsigned char[xsize*ysize];
	      for (int j=0;j<ysize;j++)
		  for (int i=0;i<xsize;i++)
		      temp[j*xsize+i]=(unsigned char)(conv8bit*(float)getpixel(i,j));
	      bytes += gzwrite(fdf,&temp[0],xsize*ysize);
	      delete [] temp;
	    }
	fprintf(stderr,"Image saved in : %s (%d)\n", filename,(int)bytes);
      }
  gzclose(fdf);
  return (bytes>0);
}

// -----------------------------------------------------------------------------
//
//       Binary File Read and Write         -- can avoid skip values
//     
//       bits=8 or 16     skip = ignore first skip skip*bits/2 bytes
// -----------------------------------------------------------------------------

int PXImage::LoadBin(int bits,int skip)
{
  long n=0;
  bits=8+8*(bits>8);

  gzFile fp=gzsuffixopen(filename,"rb");
  if (fp)
    {
      if (skip>0)
	{
	  n=(long)gzseek (fp,skip,0);
	  n=0;
	}

      if (bits==16)
	  {
	    n +=gzread(fp,&image[0],xsize*ysize*2);
	    if (n==(ysize*xsize*2))
		{
		  SetMaxgrayscale();
		  status=1;
		}
	    else
		status=0;
	  }
      
      if (bits==8)
	  {
	    unsigned char* temp=new unsigned char[xsize*ysize];
	    n +=gzread(fp,&temp[0],xsize*ysize);
	    if (n==xsize*ysize)
		{
		  for (int j=0;j<ysize;j++)
		      for (int i=0;i<xsize;i++)
			  setpixel(i,j,temp[j*xsize+i]);
		  SetMaxgrayscale();
		  status=1;
		}
	    else 
		status=0;
	    
	    delete [] temp;
	}
      gzclose(fp);
    }
  else 
      {
	fprintf(stderr, "File %s does not exist \n",filename);
	status=0;
      }
  return status;
}


//-----------------------------------------------------------------------
int PXImage::SaveBin(const char* fname,int bits)
{
  setfilename(fname);
  return SaveBin(bits);
}

int PXImage::SaveBin(int bits)
{
  
  long bytes=0;
  gzFile fdf=gzsuffixopen(filename,"rb");
  if (fdf)
    {
      switch (bits)
	  {
	  case 32:
	    {
	      float* temp32=new float(xsize*ysize);
	      for (int j=0;j<ysize;j++)
		  for (int i=0;i<xsize;i++)
		      temp32[j*xsize+i]=float(getpixel(i,j));
	      bytes += gzwrite(fdf,temp32,4*xsize*ysize);
	      delete [] temp32;
	    }
	    break;
	  case 16:
	    bytes += gzwrite(fdf,&image[0],2*xsize*ysize);
	    break;
	  case 8:
	    unsigned char* temp=getimage();
	    bytes += gzwrite(fdf,temp,xsize*ysize);
	    delete [] temp;
	    break;
	  }
      fprintf(stderr,"Image saved in : %s(%d)", filename, (int)bytes);
    }
  gzclose(fdf);
  return (bytes>0);
}

//-----------------------------------------------------------------------
/*
		The PGM Raw Format

	The FIrst Line has the form :
P5
	This indicates the format code.
	Then a number of comment lines follows.They all begin with a # i.e.
#Creator XV.....
	The next line contains the size in columns,rows for example
640,480
	The next line contains the maximum Graylevel value for example
255
	From there on the pixel Graylevel valus are stored in one
	byte/pixel format, as they are scanned row by row

*/
//-----------------------------------------------------------------------
int PXImage::LoadPgm()
{
  unsigned char c=0,d=0;
  FILE* fin;
  char line[200];
  

  // Get Header (Size)

  fin=fopen(filename,"r");
  fgets(line,100,fin);			// read in firstline
  
  int numpix=1;
  int mode=-1;
  status=0;

  int st=strncmp(line, "P5",2);
  if (st==0)
      mode=1; // Normal PGM
  else
      {
	st=strncmp(line, "P6",2);
	if (st==0)
	    {
	      mode=3;
	      numpix=3;
	    }
	else
	    {
	      st=strncmp(line,"P2",2);
	      if (st==0)
		  {
		    mode=1;
		    numpix=1;
		  }
	      else
		  {
		    st=strncmp(line,"P3",2);
		    if (st==0)
			{
			  mode=2;
			  numpix=3;
			}
		  }

	    }
      }
  
  if (mode>0)
      {
	do fgets(line,100,fin);		// read in comment lines
	while ( line[0]=='#');
	
	while (c<255)
	    {  c=line[d];
	       ++d;
	       if (c!=32) xsize=10*xsize+(c-48);
	       else c=255; }	 
	c=0;
	while (c<255)
	    { c=line[d];
	      ++d;
	      if ((c!='\n') && (c!=0) ) ysize=10*ysize+(c-48);
	      else c=255;}
	
	fscanf(fin,"%d\n",&maxgrayscale);
	
	if (mode==1 || mode==3)
	    {
	      fclose(fin);

	      if (ysize>0 && xsize>0 && maxgrayscale>0 && 
		  xsize<2048 && ysize<2048 && maxgrayscale<256)
		  {
		    int n=0;
		    gzFile fp=gzsuffixopen(filename,"rb");
		    n=(int)gzseek (fp,0,2);
		    n=n-xsize*ysize*numpix;
		    unsigned char* pp=new unsigned char[xsize*ysize*numpix];
		    
		    // Rewind back to top of header
		    n=(int)gzseek(fp,n,0);
		    n=(int)gzread(fp,&pp[0],xsize*ysize*numpix);
		    int ng=0;
		    if (n==xsize*ysize*numpix)
			{
			  image=new unsigned short[xsize*ysize]; 
			  for (int j=0;j<ysize;j++)
			      for (int i=0;i<xsize;i++)
				  {
				    int offset=j*xsize*numpix+i*numpix;
				    if (numpix==1)
					{
					  setpixel(i,j,pp[offset]);
					}
				    else
					{ // Get intensity from color image 
					  if (pp[offset]==pp[offset+1] && pp[offset]==pp[offset+2])
					      setpixel(i,j,Irange(pp[offset],0,255));
					  else
					      setpixel(i,j,0);
					}
				  }
			  if (numpix==1)
			      fprintf(stderr,"Image read from %s (%d,.pgm)\n",filename,n);
			  else
			      fprintf(stderr,"Image read from %s (%d(%d,max=%d),.ppm)\n",filename,n,ng,
				      maxgrayscale);
			  SetMaxgrayscale();
			  status=1;
			}
		    else 
			status=0;
		    delete [] pp;
		  }
	    }
	else
	    {
	      if (ysize>0 && xsize>0 && maxgrayscale>0 && 
		  xsize<2048 && ysize<2048 && maxgrayscale<65536)
		  {
		    int d[3],ng=0;
		    image=new unsigned short[xsize*ysize]; 
		    for (int j=0;j<ysize;j++)
			for (int i=0;i<xsize;i++)
			    {
			      if (numpix==1)
				  {
				    fscanf(fin,"%d",&d[0]);
				    setpixel(i,j,d[0]);
				  }
			      else
				  {
				    for (int k=0;k<3;k++)
					fscanf(fin,"%d",&d[k]);
				    if (d[0]==d[1] && d[0]==d[2])
					{
					  setpixel(i,j,d[0]);
					  ng++;
					}
				    else
					setpixel(i,j,0);
				  }
			    }
		    if (maxgrayscale>255)
			{
			  for (int j=0;j<ysize;j++)
			      for (int i=0;i<xsize;i++)
				  setpixel(i,j,getpixel(i,j)/256);
			  maxgrayscale/=256;
			}

		    if (numpix==1)
			fprintf(stderr,"Image read from %s (%d,.pgm-ascii)\n",filename,xsize*ysize);
		    else
			fprintf(stderr,"Image read from %s (%d(%d),.ppm-ascii)\n",filename,xsize*ysize,ng);
		    status=1;
		  }
	      fclose(fin);
	    }
      }
  // If not try grl format first 16-bit then 8-bit
  // ---------------------------------------------
  if (status==0)
      {
	fprintf(stderr,"Not a pgm file, trying grl\n");
	return LoadGrl(16); 
      }

  return status;
}


//-----------------------------------------------------------------------
int PXImage::SavePgm(const char* fname)
{
  setfilename(fname);
  return SavePgm();
}

int PXImage::SavePgm()
{
  int st=0;
  SetMaxgrayscale();
  FILE* fout=fopen(filename,"w");
    
  if (fout)
      {
	fprintf(fout,"P5\n");				// Write Header
	fprintf(fout,"# Saved from PXImage::SavePgm\n");
	fprintf(fout,"%d %d\n",xsize,ysize);
	fprintf(fout,"255\n");
	fclose(fout);
	
	
	int fp=open(filename,O_RDWR,0);
	lseek(fp,0,2);
	unsigned char* temp=getimage();
	int n=(int)write(fp,temp,xsize*ysize);
	delete [] temp;
	fprintf(stderr, "Image saved in : %s (%d,pgm)\n" , filename,n);
	st=1;
      }

  return st;
}

//-----------------------------------------------------------------------
/*
		The ScilImage Format

Header File .ics

ics_version     1.0
filename        exn2
layout  parameters      3
layout  order   bits    x       y
layout  sizes   8       256     256
layout  coordinates     video
layout  significant_bits        8
representation  format  integer
representation  sign    unsigned
representation  SCIL_TYPE       g2d

Strategy for loading -- first pen header file name in comment field
Extract bits,x,y
Read in Data file

*/
//-----------------------------------------------------------------------
int PXImage::LoadScil()
{

  char line[256],l1[25],l2[25];
  int c=0,numbits=8,n=0;
 
  sprintf(line,"%s.ics",filename);
  FILE* fin=fopen(line,"r");
  fgets(line,200,fin);			// read in firstline - blank;
  fgets(line,200,fin);			// read in 2nd line ;
  fgets(line,200,fin);			// read in 3rd line ;
  fscanf(fin,"%20s %20s %d\n",l1,l2,&c);
  status=0;
  if (c==3)
    {
      fgets(line,200,fin);                 // layout order bits x y;
      fscanf(fin,"%s %s %d %d %d",l1,l2,&numbits,&xsize,&ysize);
      fclose(fin);
      sprintf(line,"%s.ids",filename);
      gzFile fp=gzsuffixopen(line,"rb");
      if (fp)
	{
	  image=new unsigned short[xsize*ysize]; 
	  
	  if (numbits==8) 
	    {
	      unsigned char *bytedata=new unsigned char[xsize*ysize];
	      n +=gzread(fp,&bytedata[0],xsize*ysize);
	      for(int i=0;i<xsize;i++)
		for(int j=0;j<ysize;j++)
		  setpixel(i,j,bytedata[i*xsize+j]);
	    delete [] bytedata;
	      
	    }
	  if (numbits==16) 
	    {
	      unsigned short* worddata;
	      worddata=new unsigned short[xsize*ysize];
	      n +=gzread(fp,&worddata[0],xsize*ysize*2);
	      for(int i=0;i<xsize;i++)
		for(int j=0;j<ysize;j++)
		  setpixel(i,j,worddata[i*xsize+j]);
	      delete [] worddata;
	    }
	  
	  gzclose(fp);
	  SetMaxgrayscale();
	  status=1;
	}
    }
  else
    {
      fprintf(stderr, "Error! 3D - Scilimage file don't know how to handle it \n");
    }
  return status;
}
//-----------------------------------------------------------------------

void PXImage::SetMaxgrayscale()
{
  unsigned short max=0;
  
  for (int i=0;i<(xsize*ysize);i++)
    if (image[i]>max) max=image[i];
  
  maxgrayscale=max;
}

//-----------------------------------------------------------------------
// Image Processing Related Code
//-----------------------------------------------------------------------
void PXImage::Invert()

{
  SetMaxgrayscale();  
  for(int i=0;i<xsize;i++)
      for(int j=0;j<ysize;j++)
	  setpixel(i,j,getmaxgrayscale()-getpixel(i,j));
}

//-----------------------------------------------------------------------
void PXImage::Grlscale(float grscale)

{
  long i;
  SetMaxgrayscale();
  float scaleby=(65535.0*grscale)/((float)maxgrayscale);

  if ( scaleby<1.0 && grscale<1.0)
    {
      for (i=0;i<(xsize*ysize);i++)
	image[i]=(int)((float)image[i]*scaleby);		
    }
  SetMaxgrayscale();

}

//-----------------------------------------------------------------------
void PXImage::Zero(unsigned short px)

{
  for (long i=0;i<(xsize*ysize);i++)
      image[i]=px;
  SetMaxgrayscale();  

}

//-----------------------------------------------------------------------
void PXImage::Threshold(float t)
{
  int threshold;
  SetMaxgrayscale();
  threshold=(int)(t*(float)maxgrayscale);

  for (long i=0;i<(xsize*ysize);i++)
    image[i]=maxgrayscale*(image[i]>threshold);
  SetMaxgrayscale();  
}

//-----------------------------------------------------------------------

void PXImage::Convolve(PXFloatMatrix* kernel)
{
  PXFloatMatrix* ResultsImage= new PXFloatMatrix(getsizex(),
						 getsizey());
  for(int ia=0;ia<xsize;ia++)
    for(int ja=0;ja<ysize;ja++)
      ResultsImage->setpixel(ia,ja,0.0);
  
  float sum=0.0;
  
  fprintf(stderr, "Doing convolution ...");
  for(int i=kernel->getradiusx();
      i<(ResultsImage->getsizex()-kernel->getradiusx());i++)
    for(int j=kernel->getradiusy()+1;
	j<(ResultsImage->getsizey()-kernel->getradiusy()-1);j++)
      {
	sum=0.0;
	for(int column=-kernel->getradiusx();
	    column<=kernel->getradiusx();column++)
	  for(int row=-kernel->getradiusy();
	      row<=kernel->getradiusy();row++)
	    {
	      sum=sum+((float)(getpixel(i+column,j+row))*
		       kernel->getpixel(kernel->getradiusx()-column,
					kernel->getradiusy()-row));
	    }
	ResultsImage->setpixel(i,j,sum);
      }
  fprintf(stderr, "done \n");
  ConvFloat(ResultsImage);              
  delete ResultsImage;
}

//-----------------------------------------------------------------------

void PXImage::Median(int radius)
{
  unsigned short *old=new unsigned short[xsize*ysize];

  for(long ii=0;ii<xsize*ysize;ii++)
    old[ii]=image[ii];

  int winsize=2*radius+1,max=0,position=0;
  unsigned short* sorted=new unsigned short[winsize*winsize];

  fprintf(stderr, "Median Filetring (%d)\n", winsize );

  for(int i=radius;i<(xsize-radius-1);i++)
    for(int j=radius;j<(ysize-radius-1);j++)
      {
	for(int countx=-radius;countx<=radius;countx++)
	      for(int county=-radius;county<=radius;county++)
		sorted[(radius+countx)+(radius+county)*winsize]=
		  getpixel(old,i+countx,j+county);
	    
	    for(int outer=0;outer<=(winsize*winsize)/2;outer++)
	      {
		max=sorted[0];
		position=0;
		for(int inner=1;inner<(winsize*winsize);inner++)
		  {
		    if (sorted[inner]>max)
		      {
			max=sorted[inner];
			position=inner;
		      }
		  }
		sorted[position]=0;
	      }
	    setpixel(i,j,max);
      }
  delete [] old;
  delete [] sorted;
  status=1;
}



//-----------------------------------------------------------------------

void PXImage::Scale(float scale)
{
  scale=fabs(scale);
  if (scale>0.1)
    {
      unsigned short *old=new unsigned short[xsize*ysize];
      for(long ii=0;ii<xsize*ysize;ii++)
	  old[ii]=image[ii];
      
      int xs=(int)((float)xsize*scale);
      int ys=(int)((float)ysize*scale);

      delete [] image;
      image=new unsigned short[xs*ys];
      float a,b;
      
      for(int i=0;i<xs;i++)
	  for(int j=0;j<ys;j++)
	      {
		a=((float)i/scale);
		b=((float)j/scale);
		image[j*xs+i]=getbilinear(old,a,b);
	      }
      xsize=xs;
      ysize=ys;
      delete [] old;
      status=1;
    }
}

//-----------------------------------------------------------------------

void PXImage::AddContour(PXContour* contour,unsigned short grl,
			 unsigned char touching,
			 int offx,int offy)
{
  unsigned short a,b;
  if (touching>0)
    {
      PXContour* modcontour=new PXContour(contour);
      modcontour->closedcontour(1);
      modcontour->Touching();
      PXContourPoint *pts=modcontour->gettoppoint();
      while (pts->next !=NULL)

	{
	  a=offx+(int)pts->x;
	  b=offy+(int)pts->y;
	  if (a>=xsize) a=xsize-1;
	  if (b>=ysize) b=ysize-1;
	  setpixel(a,b,grl);
	  pts=pts->next;
	}
      delete modcontour;
    }
  else
    {
      PXContourPoint *pts=contour->gettoppoint();
      while (pts->next !=NULL)
	{
	  a=offx+(int)pts->x;
	  b=offy+(int)pts->y;
	  if (a>=xsize) a=xsize-1;
	  if (b>=ysize) b=ysize-1;
	  setpixel(a,b,grl);
	  pts=pts->next;
	}
    }
}

// ---------------------------------------------------------------------------
//                  Gradient Related Code
// ---------------------------------------------------------------------------
float PXImage::GradientX(int x,int y)
{ 
  return getpixel(x+1,y)-getpixel(x,y); 
}

float PXImage::GradientY(int x,int y)
{ 
  return getpixel(x,y+1)-getpixel(x,y);  
}
  
float PXImage::GradientXMask(int x,int y)
{ 
  float sum=0;
  for(int i=-2;i<=2;i++)
      sum=sum+(gradient_mask[i+2]*getpixel(x+i,y));
  return sum;  
}
  
float PXImage::GradientYMask(int x,int y)
{ 
  float sum=0;
  for (int i=-2;i<=2;i++)
      sum=sum+(gradient_mask[i+2]*getpixel(x,y+i));
  return sum; 
}

float PXImage::Laplacian(int x,int y)
{  
  return 4*getpixel(x,y)-getpixel(x+1,y)-getpixel(x-1,y)-
      getpixel(x,y+1)-getpixel(x,y-1);
}

// Get Sub-Pixel Gradients
// -----------------------

float PXImage::IntGradX(float x,float y)
{ 
  return getinterp(x+1.0,y)-getinterp(x,y); 
}

float PXImage::IntGradY(float x,float y)
{ 
  return getinterp(x,y+1.0)-getinterp(x,y);  
}
  
float PXImage::Int5GradX(float x,float y)
{ 
  float sum=0;
  for(int i=-2;i<=2;i++)
      sum=sum+(gradient_mask[i+2]*getinterp(x+float(i),y));
  return sum;  
}
  
float PXImage::Int5GradY(float x,float y)
{ 
  float sum=0;
  for (int i=-2;i<=2;i++)
      sum=sum+(gradient_mask[i+2]*getinterp(x,y+float(i)));
  return sum; 
}
  
float PXImage::IntLaplacian(float x,float y)
{
  return 4.0*getinterp(x,y)-getinterp(x+1.0,y)-getinterp(x-1.0,y)-
      getinterp(x,y+1.0)-getinterp(x,y-1.0);
}

/* ------------------------------------------------------------------------ 
_Description : Trace the contour of region. The region is defined by a region
               number. All the pixel of the region have the same region number.
	       The algorithm computes the border of the region. It tracks the
	       points of the border, one after the other. 

_References : Geometrie Discrete/ J.M. Chassery/ A. Montanvert
              Traite des Nouvelles Technologies
	      HERMES 1991
 
_Author : Francois G. Meyer
_Revisions History:
      7-AUG-1991                       Initial Key-in
     22-AUG-1991           Add the corners to the contour. If (a,b)
                   belongs to the contour we test for point derived 
		   from (i,j) in direction dir +1 to belong to the 
		   contour.

------------------------------------------------------------------------ */
PXContour* PXImage::tracefromimage(int value)
{
  PXContour* cntr=new PXContour;
    
  int keep_going;				/* loop controler */
  int npoint = 0,  end = 0;
  int i=0,j=0;           /* current point on the contour */
  int a=0,b=0;           /* next point on the contour */
  int abis=0, bbis=0;        /* point in direcion dir +1 form (i,j) */
  int dir=1;         /* Freeman code to get (a,b) from (i,j) */
  int dirbis=2;      /* dir +1 */
  int dirprec=1;     /* Freeman code [from (a,b)!] of the point of the back- */
  /* ground wich has been tested just before (a,b)        */
  /* from (x,y)                                           */    

  int P0x=0,P0y=0,P1x=0,P1y=0; /* two 1st point of the chain */
  
  /* scan the picture from top to down and left to right to get the 1st*/
  /* point of the object */
  
  i = 0;   j  = 0;

  while ( (i < getsizex()) && (getpixel( i, j) != value) )
      {
	j++;
	if (j == getsizey()-1) 
	    {
	      j = 0;
	      i ++;
	    }
      }

  if (getpixel(i,j)!=value) 
      {
	return cntr;
      }

  /* (i,j) is the 1st point of the value */
  /* in direction 4 you will find a point of the background */
  /* record the 1st point in the chain */
  
  cntr->AddP( i,j);
  P0x = i;
  P0y = j;
  
  npoint++;
  

  /* set current pointer to the 1st point */
  
  dir = 4;
  end = 0;
  
  /* get the other points */

  while (!end)
      {
	/* search the following point belonging to the region */
	do
	    {
	      dir = (dir+1) % 8;
	      switch (dir)
		  {
		  case 0: a = i; b = j + 1; 	break;
		  case 1: a = i - 1; b = j + 1; break;           
		  case 2: a = i - 1; b = j;     break;            
		  case 3: a = i - 1; b = j - 1; break;            
		  case 4: a = i;     b = j - 1; break;            
		  case 5: a = i + 1; b = j - 1; break;            
		  case 6: a = i + 1; b = j;     break;
		  case 7: a = i + 1; b = j + 1; break;
		  }
	      if ((i > 0) && (i < getsizex()-1) && 
		  (j < getsizey()-1) && (j > 0))
		  {
		    keep_going = (getpixel( a, b) != value);
		  }
	      else
		  {
		    keep_going = 0;
		  }
	    }
	while (keep_going);
	
	/* update the direction wich from (a,b) leads to */
	/* a point of the background */
	
	switch (dir)
	    {
	    case 0:
	    case 1: dirprec = 6;  break;
	    case 2:
	    case 3: dirprec = 0;  break;
	    case 4:
	    case 5: dirprec = 2;  break;
	    case 6:
	    case 7: dirprec = 4;  break;
	    }
	
	/* if its the 2nd point, then save it in P1*/
	if (npoint == 1)
	    {
	      P1x = a;
	      P1y = b;
	    }
	else
	    {
	      /* if the current point and the next one fit with the */
	      /* 1st and the 2nd points of the chain, then it' finished */
	      
	      if ((i == P0x) && (j == P0y) && (a == P1x) && (b == P1y)) 
		  end = 1;
	    }
	
	if (!end)
	    {
	      npoint++;
	      
	      /* if the contour is oblique */
	      if (dir % 2 == 1)
		  {
		    /* add corner if it belongs to the image */
		    
		    dirbis = (dir+1) % 8;
		    switch (dirbis)
			{
			case 0: abis = i;     bbis = j + 1; break;
			case 1: abis = i - 1; bbis = j + 1; break;           
			case 2: abis = i - 1; bbis = j;     break;            
			case 3: abis = i - 1; bbis = j - 1; break;            
			case 4: abis = i;     bbis = j - 1; break;            
			case 5: abis = i + 1; bbis = j - 1; break;            
			case 6: abis = i + 1; bbis = j;     break;
			case 7: abis = i + 1; bbis = j + 1; break;
			}
		    
		    if (getpixel( abis, bbis) == value)
			cntr->AddP(abis,bbis);
		  }
	      
	      /* process (a,b) */
	      
	      cntr->AddP(a,b);
	      i = a;
	      j = b;
	      dir = dirprec;
	    }
      }
  cntr->closedcontour(1);
  return cntr;
}


