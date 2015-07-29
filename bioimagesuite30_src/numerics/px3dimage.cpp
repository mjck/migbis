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

#include "px3dimage.h"
#include "bis_znzlib.h"

//-----------------------------------------------------------------------
PX3DImage::PX3DImage(int x ,int y,int z,
		     int wd):PXImage(x*wd,y*((z+wd-1)/wd)) 
{
  init();
  zsize=z;
  norows=(z+wd-1)/wd;
  nocolumns=wd;
  partwidth=x;
  partheight=y;
}

//-----------------------------------------------------------------------
PX3DImage::PX3DImage(const char* f,int cwidth):PXImage()
{
  init();
  xsize=0;   ysize=0;  maxgrayscale=0;    status=0;		
  zsize=0;   partheight=0; partwidth=0; nocolumns=cwidth;

  filename=new char[1];
  setfilename(f);
  CheckAnaFilename(1);                // Check Files Present;
  if (status==1)
    {
      Anagetparams(f,partwidth,partheight,zsize,numbytes);
      if (nocolumns==-1)
	{
	  nocolumns=(int)sqrt(float(zsize));
	  if (zsize>nocolumns*nocolumns) nocolumns++;
	}
      if (nocolumns==0)
	nocolumns=zsize;
      
      norows=(zsize+nocolumns-1)/nocolumns;
      xsize=partwidth*nocolumns;
      ysize=partheight*norows;
      Load();
      fprintf(stderr,"Size = %d*%d*%d numbytes=%d\n",partwidth,partheight,zsize,numbytes);
    }

}

//-----------------------------------------------------------------------
PX3DImage::PX3DImage(const char* f,int first,int last,int increment,
		     int cwidth):PXImage()
	
{
  init();
  filename=new char[1];
  status=0;		
  nocolumns=cwidth;
  setfilename(f);
  CheckAnaFilename(1);                // Check  File Names First;

  if (status==1)
    {
      Anagetparams(filename,partwidth,partheight,zsize,numbytes);
      if (last>zsize) last=zsize;
      if (first<1) first=1;
      if (last<first) last=first;
      if (increment>(last-first)) increment=last-first;
      if (increment<1) increment=1;      

      zsize=(last-first+increment)/increment;

      if (nocolumns==-1)
	{
	  nocolumns=(int)sqrt(float(zsize));
	  if (zsize>nocolumns*nocolumns) nocolumns++;
	}
      if (nocolumns==0)
	nocolumns=zsize;
      
      norows=(zsize+nocolumns-1)/nocolumns;
      xsize=partwidth*nocolumns;
      ysize=partheight*((zsize+nocolumns-1)/nocolumns);
      
      if (status==1) LoadPart(first,last,increment);
    }
}
//-----------------------------------------------------------------------
PX3DImage::PX3DImage(PX3DImage *Sequence[],int n)  // 3D Stack Constructor
{
  init();
  filename=new char[1];
  partwidth= Sequence[0]->get3dsizex();
  partheight=Sequence[0]->get3dsizey();
  zsize=n*Sequence[0]->get3dsizez();

  if (partwidth>0 && partheight>0 && zsize>0)
      {
	xsize=partwidth;
	ysize=partheight;
	
	image=new unsigned short[get3dsizex()*get3dsizey()*get3dsizez()];
	
	for (int fr=0;fr<n;fr++)
	    {
	      for(int k=0;k<Sequence[0]->get3dsizez();k++)
		  for(int i=0;i<get3dsizex();i++)
		      for(int j=0;j<get3dsizey();j++)
			  setvoxel(i,j,k+fr*Sequence[0]->get3dsizez(),
				   Sequence[k]->getvoxel(i,j,k)
				   );
	    }
	status=1;
      }
  else
      {
	partwidth=0;
	partheight=0;
	status=0;
      }
}
//-----------------------------------------------------------------------
PX3DImage::PX3DImage(PXImage *Sequence[],int length,int cwidth,
		     int x0,int y0,int x1,int y1):PXImage()
{
  init();
  filename=new char[1];
  
  if (x0==0 && y0==0 && x1==-1 && y1==-1)
      {
	for (int i=0;i<length;i++)
	    {
	      x1=Imax(x1,Sequence[i]->getsizex());
	      y1=Imax(y1,Sequence[i]->getsizey());
	    }
	x1-=1;
	y1-=1;
      }
  else
      {
	if (x1==-1 || x1>=Sequence[0]->getsizex()) x1=Sequence[0]->getsizex()-1;
	if (y1==-1 || y1>=Sequence[0]->getsizey()) y1=Sequence[0]->getsizey()-1;
      }
  partheight=y1-y0+1;
  partwidth =x1-x0+1;
  nocolumns=cwidth;
  zsize=length;   
  if (nocolumns==-1)
    {
      nocolumns=(int)sqrt(float(zsize));
      if (zsize>nocolumns*nocolumns) nocolumns++;
    }
  if (nocolumns==0)
    nocolumns=zsize;

  norows=(zsize+nocolumns-1)/nocolumns;
  xsize=partwidth*nocolumns;   
  ysize=partheight*norows;

  maxgrayscale=0;    
  status=0;		
  char* ff=new char[500];
  sprintf(ff,"%s_st",eatsuffix(Sequence[0]->getfilename()));
  setfilename(ff);
  delete [] ff;

  if (zsize>0 && xsize>0 && ysize>0)
    {
      image=new unsigned short[get3dsizex()*get3dsizey()*get3dsizez()];

      for(int k=0;k<get3dsizez();k++)
	  {
	    if (get3dsizex()==Sequence[k]->getsizex() && get3dsizey()==Sequence[k]->getsizey())
		{
		  for(int i=0;i<get3dsizex();i++)
		      for(int j=0;j<get3dsizey();j++)
			setvoxel(i,j,k,Sequence[k]->getpixel(x0+i,y0+j));
		}
	    else
		{
		  int dx=(get3dsizex()-Sequence[k]->getsizex())/2;
		  int dy=(get3dsizey()-Sequence[k]->getsizey())/2;
		  fprintf(stderr,"Frame = %d , sizes = %d*%d , %d*%d offsets=%d,%d\n",
			  k,get3dsizex(),get3dsizey(),Sequence[k]->getsizex(),Sequence[k]->getsizey(),
			  dx,dy);
		  for (int i=0;i<get3dsizex();i++)
		      for (int j=0;j<get3dsizey();j++)
			  {
			    if (i>=dx && i<Sequence[k]->getsizex()+dx && 
				j>=dy && j<Sequence[k]->getsizey()+dy)
				setvoxel(i,j,k,Sequence[k]->getpixel(x0+i-dx,y0+j-dy));
			    else
				setvoxel(i,j,k,Sequence[k]->getmaxgrayscale());
			  }
		}
	    status=1;
	    SetMaxgrayscale();
	    fprintf(stderr,"size = %d * %d , max=%d \n",partwidth,partheight,getmaxgrayscale());
	  }
    }
}
//-----------------------------------------------------------------------

PX3DImage::PX3DImage(PX3DImage* OldImage)

{
  init();
  // ****  Copy 2d - Sizes ****;
  filename=new char[1];
  xsize=OldImage->getsizex();
  ysize=OldImage->getsizey();


  // **** Copy 3d - Sizes ****;

  zsize=OldImage->get3dsizez();
  partwidth=OldImage->get3dsizex();
  partheight=OldImage->get3dsizey();

  // **** Copy 2.5 D - Sizes ****;

  norows=OldImage->getnorows();
  nocolumns=OldImage->getnocolumns();

  // **** Copy Graylevel maximum ****;

  maxgrayscale=OldImage->getmaxgrayscale();

  // **** Copy Filename related ****;

  setfilename(OldImage->getfilename());
  
  // **** Copy Actual Image ****;

  image=new unsigned short[xsize*ysize];
  for(int i=0;i<partwidth;i++)
    for(int j=0;j<partheight;j++) 
      for(int k=0;k<zsize;k++)
	setvoxel(i,j,k,OldImage->getvoxel(i,j,k));

  // **** Copy Status ****;

  status=OldImage->isvalid();
  
  polarmode=OldImage->getpolarmode();
  outplanescale=OldImage->getoutplanescale();
}

//-----------------------------------------------------------------------
PX3DImage::~PX3DImage()
{

}
//-----------------------------------------------------------------------
void PX3DImage::init()
{
  polarmode=0;
  outplanescale=1.0;
}
//-----------------------------------------------------------------------
float PX3DImage::get3dinterp(float x,float y,float z)
{	 
  z=z/outplanescale;

  x=Frange(x,0.0,float(get3dsizex()-1));
  y=Frange(y,0.0,float(get3dsizey()-1));
  z=Frange(z,0.0,float(get3dsizez()-1));

  int x1=(int)x;
  int y1=(int)y;
  int z1=(int)z;

  int x2=x1+1;
  int y2=y1+1;
  int z2=z1+1;

  if (x2>=get3dsizex())
      x2=get3dsizex()-2;
  if (y2>=get3dsizey())
      y2=get3dsizey()-2;
  if (z2>=get3dsizez())
      z2=get3dsizez()-2;

  return ((float)getvoxel(x1,y1,z1)*(x2-x)*(y2-y)*(z2-z)+
	  (float)getvoxel(x1,y2,z1)*(x2-x)*(y-y1)*(z2-z)+
	  (float)getvoxel(x2,y1,z1)*(x-x1)*(y2-y)*(z2-z)+
	  (float)getvoxel(x2,y2,z1)*(x-x1)*(y-y1)*(z2-z)+
	  (float)getvoxel(x1,y1,z2)*(x2-x)*(y2-y)*(z-z1)+
	  (float)getvoxel(x1,y2,z2)*(x2-x)*(y-y1)*(z-z1)+
	  (float)getvoxel(x2,y1,z2)*(x-x1)*(y2-y)*(z-z1)+
	  (float)getvoxel(x2,y2,z2)*(x-x1)*(y-y1)*(z-z1));
}
//-----------------------------------------------------------------------
//  I/O Related Code
//-----------------------------------------------------------------------
/*
   Analyse Format .hdr in `filename' and .img in `comment'
*/
//-----------------------------------------------------------------------

int PX3DImage::LoadBin(const char* fname,int numbytes,int offset)
{
  int n=0,nu=0,i=0,j=0,k=0;
  gzFile fp;
  unsigned short *pp=NULL;
  unsigned char  *ppchar=NULL;
  fprintf(stderr,"LoadBin %s\n",fname);

  fp=gzsuffixopen(fname,"rb");
  if (fp)
      {
	if (offset>0)
	    n +=gzseek (fp,offset,0);
	
	int sz=partheight*partwidth;
	if (numbytes==2)
	    pp=new unsigned short[sz];
	else
	    ppchar=new unsigned char[sz];
	
	image=new unsigned short[xsize*ysize];
	for(int imageno=0;imageno<get3dsizez();imageno++)
	    {
	      if (numbytes==2)
		  {
		    nu +=gzread(fp,&pp[0],sz*2);
		    for (j=0;j<partheight;j++)
			for (i=0;i<partwidth;i++)
			    setvoxel(i,j,k,pp[j*partwidth+i]);
		    
		  }
	      else
		  {
		    nu +=gzread(fp,&ppchar[0],sz);
		    for (j=0;j<partheight;j++)
			for (i=0;i<partwidth;i++)
			    setvoxel(i,j,k,ppchar[j*partwidth+i]);
		    
		  }
	      k++;
	    }
	SetMaxgrayscale();
	gzclose(fp);
	if (numbytes==2)
	    delete [] pp;
	else
	    delete [] ppchar;
	status=1;
      }
  else
      {
	fprintf(stderr," LoadBin:: Could not open file %s\n", fname);
	status=(0);
      }
  return status;
}
//-----------------------------------------------------------------------
int PX3DImage::Load()
{
  return LoadPart(1,zsize,1);
}

//-----------------------------------------------------------------------

int PX3DImage::LoadPart(int start,int end,int increment)
{
  int n=0,nu=0,i=0,j=0,k=0,sz=0;
  unsigned short *pp=NULL;
  unsigned char  *ppchar=NULL;

  char* fname=anacreateimagefilename(filename);
  gzFile fp=gzsuffixopen(fname,"r");
  
  if (fp!=Z_NULL)
      {
	if (start>1)
	    {
	      sz=(start-1)*partheight*partwidth;
	      n +=gzseek (fp,sz*numbytes,0);
	    }
	sz=partheight*partwidth;
	if (numbytes==2)
	    pp=new unsigned short[sz];
	else
	    ppchar=new unsigned char[sz];
	
	image=new unsigned short[xsize*ysize];
	for(int imageno=start;imageno<=end;imageno=imageno+increment)
	    {
	      if (numbytes==2)
		  {
		    nu +=gzread(fp,&pp[0],sz*2);
		    for (j=0;j<partheight;j++)
			for (i=0;i<partwidth;i++)
			    setvoxel(i,j,k,pp[j*partwidth+i]);
		    
		  }
	      else
		  {
		    nu +=gzread(fp,&ppchar[0],sz);
		    for (j=0;j<partheight;j++)
			for (i=0;i<partwidth;i++)
			    setvoxel(i,j,k,ppchar[j*partwidth+i]);
		    
		  }
	      k++;
	      if (increment>1)
		  n += gzseek(fp,sz*numbytes*(increment-1),1);
	    }
	SetMaxgrayscale();
	gzclose(fp);
	if (numbytes==2)
	    delete [] pp;
	else
	    delete [] ppchar;
	status=1;
      }
  else
      {
	fprintf(stderr," LoadPart:: Could not open file %s\n", fname);
	status=(0);
      }
  delete [] fname;
  return status;
}

//-----------------------------------------------------------------------
int PX3DImage::Save(const char* fname)
{
  setfilename(fname);
  return Save();
}

int PX3DImage::Save()
{
  SetMaxgrayscale();
  anaimagedata *hdr=new anaimagedata;
  hdr->zsize=zsize;
  hdr->xsize=partwidth;
  hdr->ysize=partheight;
  hdr->zasp=outplanescale; hdr->yasp=1.0;hdr->xasp=1.0;
  hdr->zpix=1; hdr->ypix=1;hdr->xpix=1;
  hdr->minpix=0;hdr->maxpix=getmaxgrayscale();
  hdr->trueminpix=0; hdr->truemaxpix=getmaxgrayscale();
  hdr->orient=polarmode*5;
  int bits=16;
  if (getmaxgrayscale()<256)
      bits=8;
  writeanahdr(filename,hdr,bits);
  delete hdr;
  return SaveImg();
}

//-----------------------------------------------------------------------

int PX3DImage::SaveImg(int bits)
{
  if (bits!=8 && bits!=16)
      {
	if (getmaxgrayscale()<256)
	    bits=8;
	else
	    bits=16;
      }
  char* fname=anacreateimagefilename(filename);
  int sz=get3dsizex()*get3dsizey();
  long bytes=0;
  gzFile fdf=gzsuffixopen(fname,"wb");
  if (fdf)
      {
	if (bits==8) 
	    {
	      unsigned char* temp=new unsigned char[sz];
	      for (int k=0;k<get3dsizez();k++)
		  {
		    for (int j=0;j<get3dsizey();j++)
			for (int i=0;i<get3dsizex();i++)
			    temp[j*get3dsizex()+i]=getvoxel(i,j,k);
		    bytes += gzwrite(fdf,&temp[0],sz);
		  }
	      delete [] temp;
	    }
	else
	    {
	      unsigned short* temp=new unsigned short[sz];
	      for (int k=0;k<get3dsizez();k++)
		  {
		    for (int j=0;j<get3dsizey();j++)
			for (int i=0;i<get3dsizex();i++)
			    temp[j*get3dsizex()+i]=getvoxel(i,j,k);
		    bytes += gzwrite(fdf,&temp[0],sz*2);
		  }
	      delete [] temp;
	    }
	fprintf(stderr,"Image saved in : %s (%d,%d)\n", fname , int(bytes) , bits);
      }
  else 
      {
	fprintf(stderr,"SaveImg :: Could not write file %s\n" , fname);
      }
  gzclose(fdf);
  delete [] fname;
  return (bytes>0);
}

//-----------------------------------------------------------------------
void PX3DImage::CheckAnaFilename(int mode)
{
  status=1;
  switch (mode) 
      {
      case 1 :
	{
	  char* fname=anacreateheaderfilename(filename);
	  gzFile fin=gzsuffixopen(fname,"r");
	  if (fin==Z_NULL) 
	      status=0;
	  else
	      gzclose(fin);
	  delete [] fname;
	  
	  if (status==1)
	      {
		char* fname2=anacreateimagefilename(filename);
		fin=gzsuffixopen(fname2,"r");
		if (!fin) 
		    status=0;
		else
		    gzclose(fin);
		delete [] fname2;
	      }
	  break;
	}
      case 2 :
	{
	  char* fname=anacreateheaderfilename(filename);
	  FILE* fout=fopen(fname,"w");
	  if (!fout) status=0;
	  fclose(fout);
	  delete [] fname;
	  break;
	}
      }
}

//-----------------------------------------------------------------------
void PX3DImage::Add3DContour(PXContour* contour,unsigned short grl,
			     unsigned char touching,
			     int offx,int offy,int offz)
{
  int px=getpixelx(offx,offy,offz);
  int py=getpixely(offx,offy,offz);
  AddContour(contour,grl,touching,px,py);
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void PX3DImage::Scale(float scale)
{
  PXImage::Scale(scale);
  partheight=int(partheight*scale);
  partwidth=int(partwidth*scale);
}
//-----------------------------------------------------------------------
// [D] Get Slices 
//
//      getslicex(int x0) -- returns plane x=x0
//     
// ----------------------------------------------------------------------

unsigned char* PX3DImage::getslicex(int x,float gamma,int numbytes)
{
  numbytes=Irange(numbytes,1,2);
  unsigned char *imageyz=new unsigned char[get3dsizey()*get3dsizez()*numbytes];
  for (int iz=0;iz<get3dsizez();iz++)
      for (int iy=0;iy<get3dsizey();iy++)
	  {
	    imageyz[iy*numbytes+iz*get3dsizey()*numbytes]=adjust(float(getvoxel(x,iy,iz)),gamma);
	    if (numbytes==2)
		imageyz[iy*numbytes+iz*get3dsizey()*numbytes+1]=255;
	  }

  return imageyz;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
unsigned char* PX3DImage::getslicey(int y,float gamma,int numbytes)
{
  numbytes=Irange(numbytes,1,2);
  unsigned char *imagexz=new unsigned char[get3dsizex()*get3dsizez()*numbytes];
  for (int iz=0;iz<get3dsizez();iz++)
      for (int ix=0;ix<get3dsizex();ix++)
	  {
	    imagexz[ix*numbytes+iz*get3dsizex()*numbytes]=adjust(float(getvoxel(ix,y,iz)),gamma);
	    if (numbytes==2)
		imagexz[ix*numbytes+iz*get3dsizex()*numbytes+1]=255;
	  }
  return imagexz;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
unsigned char* PX3DImage::getslicez(int z,float gamma,int numbytes) 
{
  numbytes=Irange(numbytes,1,2);
  unsigned char *imagexy=new unsigned char[get3dsizex()*get3dsizey()*numbytes];
  for (int iy=0;iy<get3dsizey();iy++)
      for (int ix=0;ix<get3dsizex();ix++)
	  {
	    imagexy[ix*numbytes+iy*get3dsizex()*numbytes]=adjust(float(getvoxel(ix,iy,z)),gamma);
	    if (numbytes==2)
		imagexy[ix*numbytes+iy*get3dsizex()*numbytes+1]=255;
	  }

  return imagexy;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
unsigned char* PX3DImage::getinterpslicex(float x,float gamma,int numbytes)
{
  numbytes=Irange(numbytes,1,2);
  unsigned char *imageyz=new unsigned char[get3dsizey()*get3dsizez()*numbytes];
  for (int iz=0;iz<get3dsizez();iz++)
      for (int iy=0;iy<get3dsizey();iy++)
	  {
	    imageyz[iy*numbytes+iz*get3dsizey()*numbytes]=
		adjust(get3dinterp(x,float(iy),float(iz)),gamma);
	    if (numbytes==2)
		imageyz[iy*numbytes+iz*get3dsizey()*numbytes+1]=255;
	  }
  return imageyz;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
unsigned char* PX3DImage::getinterpslicey(float y,float gamma,int numbytes)
{
  numbytes=Irange(numbytes,1,2);
  unsigned char *imagexz=new unsigned char[get3dsizex()*get3dsizez()*numbytes];
  for (int iz=0;iz<get3dsizez();iz++)
      for (int ix=0;ix<get3dsizex();ix++)
	  {
	    imagexz[ix*numbytes+iz*get3dsizex()*numbytes]=adjust(get3dinterp(float(ix),y,float(iz)),gamma);
	    if (numbytes==2)
		imagexz[ix*numbytes+iz*get3dsizex()*numbytes+1]=255;
	  }
  return imagexz;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
unsigned char* PX3DImage::getinterpslicez(float z,float gamma,int numbytes) 
{
  numbytes=Irange(numbytes,1,2);
  unsigned char *imagexy=new unsigned char[get3dsizex()*get3dsizey()*numbytes];
  for (int iy=0;iy<get3dsizey();iy++)
      for (int ix=0;ix<get3dsizex();ix++)
	  {
	    imagexy[ix*numbytes+iy*get3dsizex()*numbytes]=adjust(get3dinterp(float(ix),float(iy),z),gamma);
	    if (numbytes==2)
		{
		  imagexy[ix*numbytes+iy*get3dsizex()*numbytes+1]=255;
		}
	  }
  return imagexy;
}
//-----------------------------------------------------------------------
// Ultrasound images related code 
//-----------------------------------------------------------------------
unsigned short PX3DImage::get3dechovoxel(float x,float y,float z,float middlex)
{
  if (middlex<0.0) middlex=(get3dsizex())/2.0;
  float slice_y=Frange(z,0.0,(float)get3dsizey()-1);

  float px=float(x)-middlex;
  float py=float(y)-middlex;

  float radius=sqrt(px*px+py*py);
  float theta=atan2(py,px)*180.0/M_PI;
  if (theta< (000.0-0.5*outplanescale)) theta+=180.0;
  if (theta>=(180.0-0.5*outplanescale)) theta-=180.0;
  float slice_z=fabs(theta)/outplanescale;
  
  int slice_no=int(slice_z+0.5);
	    
  if (py<0.0 && slice_no!=0)  radius=-radius;
  if (slice_no==0 && px<0.0)  radius=-radius;
  slice_no=Irange(slice_no,0,get3dsizez()-1);
  float slice_x=radius+middlex;
  if (slice_x>=0 && slice_x<get3dsizex())
      return getvoxel(int(slice_x+0.5),int(slice_y+0.5),slice_no);
  else
      return 0;
}
//-----------------------------------------------------------------------
unsigned char* PX3DImage::getechoslicexy(float zlevel,float gamma,int numbytes,float middlex)
{
  if (middlex<0.0)   middlex=(get3dsizex())/2.0;
  numbytes=Irange(numbytes,1,2);

  unsigned char *imagexy=new unsigned char[get3dsizex()*get3dsizex()*numbytes];

  
  for (int iy=0;iy<get3dsizex();iy++)
      for (int ix=0;ix<get3dsizex();ix++)
	  {
	    imagexy[iy*get3dsizex()*numbytes+ix*numbytes]=
		adjust(get3dechovoxel(float(ix),float(iy),zlevel,middlex),gamma);
	    if (numbytes==2)
		{
		  float radius=sqrt(pow(float(ix)-middlex,float(2.0))+
				    pow(float(iy)-middlex,float(2.0)));
		  if (fabs(radius)<middlex-5)
		      imagexy[iy*get3dsizex()*numbytes+ix*numbytes+1]=255;
		  else
		      imagexy[iy*get3dsizex()*numbytes+ix*numbytes+1]=0;
		}
	  }
  return imagexy;
}
//-----------------------------------------------------------------------
unsigned char* PX3DImage::getechoslicetheta(int slice,float gamma,int numbytes)
{
  unsigned char* texture=getslicez(slice,gamma,numbytes);
  
  if (numbytes==2)
      {
	for (int iy=0;iy<get3dsizey();iy++)
	    for (int ix=0;ix<get3dsizex();ix++)
		{
		  unsigned char a=texture[iy*get3dsizex()*2+ix*2];
		  if (a>250)
		      texture[iy*get3dsizex()*2+ix*2+1]=0;
		  else
		      texture[iy*get3dsizex()*2+ix*2+1]=255;
		}
      }
  return texture;
}
//-----------------------------------------------------------------------
int PX3DImage::saveArray(const char* filename,PX3DImage** sequence,int numf)
{

  int maxg=0;
  for (int i=0;i<numf;i++)
      {
	sequence[i]->SetMaxgrayscale();
	maxg=Imax(maxg,sequence[i]->getmaxgrayscale());
      }

  anaimagedata *hdr=new anaimagedata;
  hdr->zsize=sequence[0]->get3dsizez()*numf;
  hdr->xsize=sequence[0]->get3dsizex();
  hdr->ysize=sequence[0]->get3dsizey();

  hdr->zasp=sequence[0]->getoutplanescale();
  hdr->yasp=1.0;hdr->xasp=1.0;
  hdr->zpix=1; hdr->ypix=1;hdr->xpix=1;
  hdr->minpix=0;hdr->maxpix=maxg;
  hdr->trueminpix=0; hdr->truemaxpix=maxg;
  hdr->orient=sequence[0]->getpolarmode()*5;
  int bits=16;
  if (maxg<256)
      bits=8;
  writeanahdr(filename,hdr,bits);
  
  char* fname=anacreateimagefilename(filename);
  int sz=sequence[0]->get3dsizex()*sequence[0]->get3dsizey();
  long bytes=0;

  gzFile fdf=gzsuffixopen(fname,"wb");
  if (fdf)
      {
	if (bits==8) 
	    {
	      unsigned char* temp=new unsigned char[sz];
	      for (int t=0;t<numf;t++)
		  for (int k=0;k<sequence[0]->get3dsizez();k++)
		      {
			for (int j=0;j<sequence[0]->get3dsizey();j++)
			    for (int i=0;i<sequence[0]->get3dsizex();i++)
				temp[j*sequence[0]->get3dsizex()+i]=sequence[t]->getvoxel(i,j,k);
			bytes += gzwrite(fdf,&temp[0],sz);
		      }
	      delete [] temp;
	    }
	else
	    {
	      unsigned short* temp=new unsigned short[sz];
	      for (int t=0;t<numf;t++)
		  for (int k=0;k<sequence[0]->get3dsizez();k++)
		      {
			for (int j=0;j<sequence[0]->get3dsizey();j++)
			    for (int i=0;i<sequence[0]->get3dsizex();i++)
				temp[j*sequence[0]->get3dsizex()+i]=sequence[t]->getvoxel(i,j,k);
			bytes += gzwrite(fdf,&temp[0],sz*2);
		      }
	      delete [] temp;
	    }
	fprintf(stderr,"Image saved in : %s (%d,%d) frames=%d\n",fname,int(bytes),bits , numf);
      }
  else 
      {
	fprintf(stderr,"SaveImg :: Could not write file %s\n" , fname);
      }
  gzclose(fdf);
  delete [] fname;
  delete hdr;
  return (bytes>0);
}

