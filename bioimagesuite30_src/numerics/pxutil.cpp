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
//  This is a catch all utility file that is mostly written by Xenios Papademetris with code
//     from Larry Staib
//
// -------------------------------------------------------------------------------------------------------
// pxutil.cc
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July 1995   papad@noodle.med.yale.edu

   See pxutil.h for details

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#include "pxutil.h"

/* Byte Swapping *************************************************************
Byte Swap Stuff
******************************************************************************/

int pxanaswapint(int input)
{
  int a=input;
  unsigned char* bytes=(unsigned char*)&a;
  unsigned char tmp;

  tmp=*bytes;
  *bytes=*(bytes+3);
  *(bytes+3)=tmp;

  tmp=*(bytes+1);
  *(bytes+1)=*(bytes+2);
  *(bytes+2)=tmp;

  //fprintf(stderr,"Swapping Int %d to %d\n",input,a);
  return a;
}

float pxanaswapfloat(float input)
{
  float a=input;
  unsigned char* bytes=(unsigned char*)&a;
  unsigned char tmp;

  tmp=*bytes;
  *bytes=*(bytes+3);
  *(bytes+3)=tmp;

  tmp=*(bytes+1);
  *(bytes+1)=*(bytes+2);
  *(bytes+2)=tmp;

  //fprintf(stderr,"Swapping Float %f to %f\n",input,a);
  return a;
}

short pxanaswapshort(short input)
{
  short a=input;
  unsigned char *bytes=(unsigned char*)&a;
  unsigned char tmp;
  tmp=*bytes;
  *bytes=*(bytes+1);
  *(bytes+1)=tmp;
  //fprintf(stderr,"Swap %d to %d\n",input,a);
  return a;

}

double pxanaswapdouble(double input)
{
  double a=input;
  double b=input;
  unsigned char* ap=(unsigned char*)&a;
  unsigned char* bp=(unsigned char*)&b;

  ap[0]=bp[7];
  ap[1]=bp[6];
  ap[2]=bp[5];
  ap[3]=bp[4];
  ap[4]=bp[3];
  ap[5]=bp[2];
  ap[6]=bp[1];
  ap[7]=bp[0];

  return a;
}

/* Writeanahdr ****************************************************************
write ANALYZE .hdr file
******************************************************************************/

int writeanahdr(const char* filename,anaimagedata *imdata,int bits)
{
  char *hdrfile= anacreateheaderfilename(filename);

  struct dsr hdr;
  FILE *fp;
  int dsize;

  int realbits=abs(bits);
  
  if (realbits != 8 && realbits != 16 && realbits!=32 && realbits!=31 && realbits<128)  
      {
	printf("Writeanahdr: invalid bits per pixel\n");
	return(0);
      }


  /*  fprintf(stderr,"Size of Header = %d\n" , sizeof(struct dsr));
      fprintf(stderr,"Short Int = %d\n" , sizeof(short int));
      fprintf(stderr,"Size of Header Key = %d\n" , sizeof(struct header_key));
      fprintf(stderr,"Size of Image Dimension = %d\n" , sizeof(struct image_dimension));
      fprintf(stderr,"Size of Data History = %d\n" , sizeof(struct data_history));*/

  int swap=0;
  if (bits<0)
      swap=1;
  
  if((fp=fopen(hdrfile,"w"))==0) {
    printf("Writeanahdr: unable to create: %s\n",hdrfile);
    dsize = 0;
  }
    else {

      memset(&hdr,0,sizeof(struct dsr)); 
      hdr.hk.sizeof_hdr = sizeof(struct dsr);
      hdr.hk.extents = 16384;
      hdr.hk.regular = 'r';

      if (imdata->tsize>1)
	hdr.dime.dim[0] = 4;
      else
	hdr.dime.dim[0] = 3;
      hdr.dime.dim[1] = imdata->xsize;
      hdr.dime.dim[2] = imdata->ysize;
      hdr.dime.dim[3] = imdata->zsize;
      hdr.dime.dim[4] = imdata->tsize;
      strcpy(hdr.dime.vox_units,"mm");
      strcpy(hdr.dime.cal_units," ");



      if (realbits>128)
	{
	  // Hack if > 128 subtract 128 and use as actual number
	  hdr.dime.datatype=realbits-128;
	}
      else
	{
	  switch(realbits) 
	    { 
	    case 1:  hdr.dime.datatype = 1;   break;
	    case 8:  hdr.dime.datatype = 2;   break; /* unsigned char */
	    case 16: hdr.dime.datatype = 4;   break; /*    short      */
	    case 31: hdr.dime.datatype = 8;   break; /*   signed int    */
	    case 32: hdr.dime.datatype = 16;  break; /*    float *//* 8  for int */
	    case 24: hdr.dime.datatype = 128; break; /*     rgb       */
	    case 64: hdr.dime.datatype = 32;  break; /*   complex*//* 64 for dbl */
	    }
	}
      
      switch (hdr.dime.datatype)
	{
	case 1:
	  hdr.dime.bitpix=1;
	  break;
	case 2:
	  hdr.dime.bitpix=8;
	  break;
	case 4:
	  hdr.dime.bitpix=16;
	  break;
	case 8:
	  hdr.dime.bitpix=32;
	  break;
	case 16:
	  hdr.dime.bitpix=32;
	  break;
	case 32:
	  hdr.dime.bitpix=64;
	  break;
	}
	  

      fprintf(stderr,"Saving swap=%d, datatype=%d bitpix=%d\n",swap,hdr.dime.datatype,hdr.dime.bitpix);

      hdr.dime.cal_max = 0.0;
      hdr.dime.cal_min = 0.0;
      hdr.dime.glmax  = imdata->maxpix;
      hdr.dime.glmin  = imdata->minpix;
      hdr.dime.pixdim[1] = imdata->xpix;
      hdr.dime.pixdim[2] = imdata->ypix;
      hdr.dime.pixdim[3] = imdata->zpix;
      hdr.dime.pixdim[4] = 1.0;
      hdr.dime.vox_offset = 0.0;
      hdr.hist.orient = imdata->orient;

      /*for (int ia=0;ia<=4;ia++)
	hdr.hist.origin[ia]=0;*/

      fprintf(stderr,"Saving swap=%d, datatype=%d %d,%d,%d\n",swap,hdr.dime.datatype,
	      hdr.dime.dim[1],
	      hdr.dime.dim[2],
	      hdr.dime.dim[3],
	      hdr.dime.dim[4]);

      hdr.dime.funused1=1.0;
      
      if (swap==1)
	  {
	    for (int k=0;k<=4;k++)
		{
		  hdr.dime.dim[k]=pxanaswapshort(hdr.dime.dim[k]);
		  //hdr.hist.origin[k]=pxanaswapshort(hdr.hist.origin[k]);
		  if (k>0)
		      hdr.dime.pixdim[k]=pxanaswapfloat(hdr.dime.pixdim[k]);
		}

	    hdr.dime.bitpix=pxanaswapshort(hdr.dime.bitpix);
	    hdr.dime.datatype=pxanaswapshort(hdr.dime.datatype);
	    hdr.dime.glmax=pxanaswapint(hdr.dime.glmax);
	    hdr.dime.glmin=pxanaswapint(hdr.dime.glmin);
	    hdr.dime.funused1=pxanaswapfloat(hdr.dime.funused1);
	  }
      
      strcpy(hdr.hist.exp_date,"");
      strcpy(hdr.hist.exp_time,"");
      strcpy(hdr.hist.patient_id,"");
      if (swap==0)
	  strcpy(hdr.hist.descrip,"created by writeanahdr2");
      else
	  strcpy(hdr.hist.descrip,"created by writeanahdr2 swapped");
      //fprintf(stderr,"Header=%s",hdr.hist.descrip);
      fwrite(&hdr,sizeof(struct dsr),1,fp);
      fclose(fp);
      dsize = 1;
    }
  delete [] hdrfile;
  return(dsize);
}

/* Readanahdr *****************************************************************
read ANALYZE .hdr file
******************************************************************************/

int readanahdr(const char *filename, anaimagedata *imdata)

{
  //fprintf(stderr,"In read anahdr\n");
  char *hdrfile=anacreateheaderfilename(filename);
  //  fprintf(stderr,"Trying %s\n",hdrfile);
  
  gzFile fd=gzsuffixopen(hdrfile,"rb");
  if ((long)fd==0)
    {
      char* hdrfile2=anacreatefilename(hdrfile,".HDR",".hdr");
      //      fprintf(stderr,"Trying %s instead of %s\n",hdrfile2,hdrfile);
      fd=gzsuffixopen(hdrfile2,"rb");
      delete [] hdrfile2;
    }

  struct dsr hdr;
  short psize=0;
  if ((long)fd==0)
    {
      delete [] hdrfile;
      return 0;
    }
  else
    {
      gzread(fd,&hdr,sizeof(struct dsr));
      imdata->xsize  = hdr.dime.dim[1];
      imdata->ysize  = hdr.dime.dim[2];
      imdata->zsize  = hdr.dime.dim[3];
      imdata->tsize  = hdr.dime.dim[4];
      imdata->xpix   = hdr.dime.pixdim[1];
      imdata->ypix   = hdr.dime.pixdim[2];
      imdata->zpix   = hdr.dime.pixdim[3];
      imdata->minpix = hdr.dime.glmin;
      imdata->maxpix = hdr.dime.glmax;
      imdata->orient = hdr.hist.orient;
      psize          = hdr.dime.bitpix;
      imdata->datatype=hdr.dime.datatype;
      // Use 31 as flag for integer 
      
      gzclose(fd);
      
      //fprintf(stderr,"Sizes = %d x %d x %d \n",imdata->xsize,imdata->ysize,imdata->zsize);
      // Check for swapped bytes
      if (psize<0 || psize>64)
	{
	  imdata->xsize=pxanaswapshort(imdata->xsize);
	  imdata->ysize=pxanaswapshort(imdata->ysize);
	  imdata->zsize=pxanaswapshort(imdata->zsize);
	  imdata->tsize=pxanaswapshort(imdata->tsize);
	  
	  //fprintf(stderr,"Swapped Sizes = %d x %d x %d \n",imdata->xsize,imdata->ysize,imdata->zsize);
	  imdata->minpix=pxanaswapint(imdata->minpix);
	  imdata->maxpix=pxanaswapint(imdata->maxpix);
	  
	  imdata->xpix=pxanaswapfloat(imdata->xpix);
	  imdata->ypix=pxanaswapfloat(imdata->ypix);
	  imdata->zpix=pxanaswapfloat(imdata->zpix);
	  
	  imdata->trueminpix=pxanaswapint(imdata->trueminpix);
	  imdata->truemaxpix=pxanaswapint(imdata->truemaxpix);
	  
	  psize=-pxanaswapshort(psize);
	  imdata->datatype=pxanaswapshort(hdr.dime.datatype);
	}
    }




  delete [] hdrfile;


  if ( psize == 32 )
    {
      if (hdr.dime.datatype==8)
	psize=31;
    }

  return psize;
}

// ---------------------------------------------------------------------------

int Anagetparams(const char* filename,int& x,int& y,int &z)
{
  anaimagedata *hdr=new anaimagedata;
  int i=abs(readanahdr(filename,hdr));
  z=hdr->zsize*hdr->tsize;
  x=hdr->xsize;
  y=hdr->ysize;
  delete hdr;
  return i;
}

int  Anagetparams(const char* filename,int& x,int& y,int &z,int &bytes)
{
  anaimagedata *hdr=new anaimagedata;
  int i=abs(readanahdr(filename,hdr));
  z=hdr->zsize*hdr->tsize;
  x=hdr->xsize;
  y=hdr->ysize;
  bytes=int(i/8);
  if (bytes<1)
      bytes=1;

  delete hdr;
  return i;
}

int  Anagetparams(const char* filename,int& x,int& y,int &z,int &t,int &bytes)
{
  anaimagedata *hdr=new anaimagedata;
  int i=abs(readanahdr(filename,hdr));
  z=hdr->zsize;
  t=hdr->tsize;
  x=hdr->xsize;
  y=hdr->ysize;
  bytes=int(i/8);
  if (bytes<1)
      bytes=1;
  delete hdr;
  return i;
}

// ---------------------------------------------------------------------------
// FILENAME UTILITIES Some by Larry Staib  staib@ipag.med.yale.edu
// ---------------------------------------------------------------------------

char *eatsuffix(const char *orig_filename)
{
  int   len = (int)strlen(orig_filename);
  char* newname = new char[len+10];
  char* filename= new char[len +10];
  strcpy(newname,orig_filename);
  strcpy(filename,orig_filename);

  char* ext = strrchr(filename,'.');
  char* sla = strrchr(filename,'/');

  if (ext != NULL) 
    {  
      if ((sla == NULL) || (strlen(sla) >= strlen(ext))) 
	{ /* file.ext or ../file.ext */
	  int lenext = (int)strlen(ext);
	  strncpy(newname,filename,len-lenext);
	  newname[len-lenext] = '\0';
	}
    }
  
  delete [] filename;
  return(newname);
}

/* nothisextension ************************************************************
 
  Removes specific extension from filename if there is one.

  Handles: file ../file.ext file.ext  ../file

******************************************************************************/
char *nothisextension(const char *orig_filename,const char *extension)

{ 
  char* filename=new char[strlen(orig_filename)+1 ];
  strcpy(filename,orig_filename);

  char *ext = strrchr(filename,'.');
 
  if (ext == NULL) 
    {
      char *help2=new char[strlen(filename+1)];
      strcpy(help2,filename);
      delete [] filename;
      return(help2);
    }

  if (strcmp(ext,extension) == 0) 
    {
      char* f=(eatsuffix(filename));
      delete [] filename;
      return f;
    }

  char *help2=new char[strlen(filename+1)];
  strcpy(help2,filename);
  delete [] filename;
  return(help2);
  
}

char* anaeatsuffix(const char* fname)
{ 
  char* newfname=new char[strlen(fname)+1];
  strcpy(newfname,fname);
  char* a=strtok(newfname,".");
  char* outname=new char[strlen(fname)+1];
  strcpy(outname,a);
  delete [] newfname;
  return outname;
}
     
char* anaeatpath(const char* fname)
{
#ifdef _WIN32
//  char* ffname=strrchr(fname,(int)'\\');          //This is ANSI-C
    const char* ffname0=strrchr(fname,(int)'\\');	//This is ANSI-C++
    char* ffname = (char*)ffname0;					//This is ANSI-C++//hirohito
#else
    char* ffname = strrchr((char*)fname,(int)'/');
#endif

  if (ffname==NULL) 
    {
      ffname=new char[strlen(fname)+1];
      strcpy(ffname,fname);
      return ffname;
    }
  
  ffname++;
  char* newname=new char[strlen(fname)+1];
  strcpy(newname,ffname);
  return newname;
}    

/******************************************************************************/
char* anacreateheaderfilename(const char* fname)
{

  char* temp=anacreatefilename(fname,"",".gz");
  char* temp1=anacreatefilename(temp,"",".HDR");
  char* temp2=anacreatefilename(temp1,".hdr",".img");
  delete [] temp;
  delete [] temp1;
  return temp2;
}

char* anacreateimagefilename(const char* fname)
{
  char* temp=anacreatefilename(fname,"",".gz");
  char* temp2=anacreatefilename(temp,"",".HDR");
  char* temp3=anacreatefilename(temp2,".img",".hdr");

  delete [] temp;
  delete [] temp2;
  return temp3;

}

char* anacreatefilename(const char* orig_filename,const char* wanted,const char *unwanted)
{
  char* tmp,*hdrfile;
  char* filename=new char[strlen(orig_filename)+1 ];
  strcpy(filename,orig_filename);

  char* ext = strrchr(filename,'.');
 
  if (ext)
    {
      if ( (strcmp(ext,unwanted) == 0 ) ||  ( strcmp(ext,".") == 0 ) )
	{
	  tmp=eatsuffix(filename);
	  hdrfile=new char[strlen(tmp)+strlen(wanted)+5];
	  sprintf(hdrfile,"%s%s",tmp,wanted);
	  delete [] tmp;
	  delete [] filename;
	  return hdrfile;
	}
      
      if (strcmp(ext,wanted) == 0)
	{
	  hdrfile=new char[strlen(filename)+5];
	  strcpy(hdrfile,filename);
	  delete [] filename;
	  return hdrfile;
	}
      
      hdrfile=new char[strlen(filename)+strlen(wanted)+5];
      sprintf(hdrfile,"%s%s",filename,wanted);
      delete [] filename;
      return hdrfile;
    }
  hdrfile=new char[strlen(filename)+strlen(wanted)+5];
  sprintf(hdrfile,"%s%s",filename,wanted);
  delete [] filename;
  return hdrfile;
}

/******************************************************************************/
char* anaextractpath(const char* fname)
{
  char* path;
  int where=-1;
  for (int c=0;c<(int)strlen(fname);c++)
      {
	if (fname[c]=='/')
	  where=c;
      }
  if (where==-1)
    {
      path=new char[3];
      strcpy(path,"./");
    }
  else
    {
      path=new char[where+2];
      for (int c=0;c<=where;c++)
	path[c]=fname[c];
      path[where+1]=(char)0;
    }
  return path;
}
/******************************************************************************/
int anabackupfile(const char* fname)
{
  FILE* fin=fopen(fname,"r");
  if (fin)
      {
	fclose(fin);
	char* command=new char[strlen(fname)*2+50];
	char* temp=new char[strlen(fname)+2];
	strcpy(command,fname);
	strcpy(temp,fname);
	char* path=anaextractpath(command);
#ifndef _WIN32
	sprintf(command,"/usr/bin/cp %s %s/backup_of_%s",fname,path,anaeatpath(temp));
#else
	sprintf(command,"copy %s %s\backup_of_%s",fname,path,anaeatpath(temp));
#endif
	delete [] path;
	int ok=system(command);
	delete [] command;
	delete [] temp;
	return ok;
      }
  else
      return 0;
}
/******************************************************************************/
int stringskipspace(const char* line,int po)
{
  // Ascii code 0=48,..,9=57, + = 43 - = 45 . = 46
 
  int flag=0;
  do
      {
	int p=(int)line[po];
	if ((p<48 || p>57) && (p!=45 &&p!=46 && p!=43) && ( po<(int)strlen(line) ))
	    po++;
	else
	    flag=1;
      }
  while (flag==0);
  return po;
}
/******************************************************************************/
int stringskipnumber(const char* line,int po)
{
  // Ascii code 0=48,..,9=57, + = 43 - = 45 . = 46

  int flag=0;
  do
      {
	int p=(int)line[po];
	if ( ((p>=48 &&  p<=57) || (p==45 || p==46 || p==43)) && ( po<(int)strlen(line) ))
	    po++;
	else
	    flag=1;
      }
  while (flag==0);
  return po;
}
/******************************************************************************/
float   getgzstreamfloat(gzFile fin)
{
  char line[26];
  strcpy(line,"");
  int po=0,flag=0;
  
  line[po]=(char)0;

  int mode=0;
  do
      {
	int p=gzgetc(fin);
	if (p==-1)
	    flag=1;
	else
	    {
	      if( ((p>=48 &&  p<=57) || (p==45 || p==46 || p==43)))
		  {
		    line[po]=(char)p;
		    line[po+1]=(char)0;
		    po++;
		    if (po==25)
			flag=1;
		    mode=1;
		  }
	      else 
		  {
		    if (mode==1)
			flag=1;
		  }
	    }
      }
  while (flag==0);
  return atof(line);
  
}

int getgzstreaminteger(gzFile fin)
{
  return int(getgzstreamfloat(fin));

}

int gzfindstringinstring(const char* orig,const char* lookfor)
{
  if (strlen(lookfor)>strlen(orig))
      return 0;

  int i=0,found=0;
  int maxi=int(strlen(orig)-strlen(lookfor))+1;

  while(found==0 && i<maxi)
      {
	if (strncmp(&orig[i],lookfor,strlen(lookfor))==0)
	    found=1;
	i++;
      }
  
  return found;
}

/******************************************************************************/
gzFile ZEXPORT gzsuffixopen (const char* path, const char* mode,int complevel)
{
  // Default 

  if (mode==NULL)
      {
	//fprintf(stderr,"Returning NULL\n");
	return Z_NULL;
      }

  if (mode[0]=='r')
      {
	//fprintf(stderr,"Going to gz_open %s %s\n",path,mode);
	gzFile s=gzopen (path, mode);
	if (s!=Z_NULL)
	    return s;
	
	// Take 2 check for .gz suffix if missing

	char* newpath=new char[strlen(path)+5];
	sprintf(newpath,"%s.gz",path);
	s=gzopen(newpath,mode);
	//fprintf(stderr,"Reading %s failed trying %s (status=%d)\n",path,newpath,int (s!=Z_NULL));
	delete [] newpath;
	return s;
      }
  
  // Write or append 
  char modestr[20];
  int foundgz=0;

  complevel=Irange(complevel,0,9);  
  if (complevel==0)
      {
	if (gzfindstringinstring((char* )path,".gz")==1)
	    {
	      complevel=6;
	      foundgz=1;
	    }
      }
  
  if (complevel>0)
      sprintf(modestr,"%s%1d",mode,complevel);
  else
      sprintf(modestr,"%s0",mode);
  
  if (complevel==0 || foundgz==1)
      return gzopen(path,modestr);
  
  char* newpath=anacreatefilename((char*)path,".gz",".gz");
  gzFile s=gzopen(newpath,modestr);
  //fprintf(stderr,"Since compressed not writing in %s but trying %s (status=%d)\n",path,newpath,int (s!=Z_NULL));
  delete [] newpath;
  return s;
}

/* Readanahdr *****************************************************************
read SIGNA lx header from file
******************************************************************************/
int readlxhdr(const char* fname,anaimagedata* hdr)
{
  struct SUITEDATATYPE shdr;
  struct EXAMDATATYPE ehdr;
  struct SERIESDATATYPE sehdr;
  struct MRIMAGEDATATYPE ihdr;
  struct PixHdr phdr;
  

  int orient=0;
  gzFile fd=gzsuffixopen(fname,"rb");
  if (fd==0)
    {
      fprintf(stderr,"Cannot open lx header %s\n", fname);
      return(0);
    }

  /* Read suite header */
  gzread(fd,&shdr,sizeof(struct SUITEDATATYPE));

  /* Read exam header */
  gzread(fd,&ehdr,sizeof(struct EXAMDATATYPE));

  /* swap condition: 1.5 Tesla */
  int swap = (ehdr.magstrength != 15000);

  /*if (swap) 
    printf("Swapping to read\n");*/

  gzread(fd,&sehdr,sizeof(struct SERIESDATATYPE));

  /* Read MR image header */
  gzread(fd,&ihdr,sizeof(struct MRIMAGEDATATYPE));
      
  /* Read pixel header */
  gzread(fd,&phdr,sizeof(struct PixHdr));

  /*printf("magic: %d hdr_length: %d \n",
    lhs_byteswapint(phdr.img_magic,swap), 
    lhs_byteswapint(phdr.img_hdr_length,swap));*/

  int comp=phdr.img_compress;
  int comp2=comp;
  if (swap)
    comp=pxanaswapint(comp);
  
  if (comp!=1)
    {
      fprintf(stderr,"mrstriplx: Error, Image compression not supported compression=%d,%d\n",comp,comp2);
      return(0);
    }
  else
    fprintf(stderr,"Image compression=%d,%d\n",comp,comp2);

  gzclose(fd);
  
  short orn=ihdr.plane;
  if (swap)
    orn=pxanaswapshort(orn);
  
  switch (orn)
    {
    case 1:  
    case 2:
    case 16:
    case 18:
      orient = 0; 
      break;
      
    case 4:  
    case 20:
      orient = 2; 
      break;
      
    case 8: 
    case 24:
      orient = 1; 
      break;
      
    default:
      orient=0;
      break;
    }
    

      hdr->zsize = 1 ;
      hdr->tsize = 1 ;    
      hdr->orient = orient;


  hdr->minpix=-32768;
  hdr->maxpix=32767;

  if (swap)
    {
      hdr->xsize = pxanaswapint(phdr.img_width);
      hdr->ysize = pxanaswapint(phdr.img_height);
      hdr->xpix  = pxanaswapfloat(ihdr.pixsize_X);
      hdr->ypix  = pxanaswapfloat(ihdr.pixsize_Y);
      hdr->zpix  = pxanaswapfloat(ihdr.slthick);
      return -16;
    }

  hdr->xsize = phdr.img_width;
  hdr->ysize = phdr.img_height;
  hdr->xpix  = ihdr.pixsize_X;
  hdr->ypix  = ihdr.pixsize_Y;
  hdr->zpix  = ihdr.slthick;
  return 16;

}





// -------------------------------------------------------------
//  Quantization -- quantize numbers to specified decimal places 
// -------------------------------------------------------------
float  quantize(float a,int dp)
{
  float d=pow(float(10.0),float(dp));
  return float(float(long(a*d+0.5))/d);
}
double quantize(double a,int dp)
{
  double d=pow(double(10.0),double(dp));
  return double(double(long(a*d+0.5))/d);
}

// -------------------------------------------------------------
float Fmin(float a,float b)
{
  if (a<b)
      return a;
  else
      return b;
}

float Fmax(float a,float b)
{
  if (a>b)
      return a;
  else
      return b;
}

int Imin(int a,int b)
{
  if (a<b)
      return a;
  else
      return b;
}

int Imax(int a,int b)
{
  if (a>b)
      return a;
  else
      return b;
}
// -------------------------------------------------------------
float Frange(float a,float minv,float maxv)
{
  if (a<minv) a=minv;
  if (a>maxv) a=maxv;
  return a;
}
// -------------------------------------------------------------
float FcyclicDistance(float p1,float p2,float circum)
{
  float half=circum/2.0;
  float dist=0.0;
  if (fabs(p1-p2)<half)
      {
	dist=p1-p2;
      }
  else
      {
	if (p1>p2)
	    dist=p2+circum-p1;
	else
	    dist=p1+circum-p2;
      }

  return fabs(dist);  
}
// -------------------------------------------------------------
int   Irange(int   a,int   minv,int   maxv)
{
  if (a<minv) a=minv;
  if (a>maxv) a=maxv;
  return a;
}

int  Icyclic(int v,int np)
{
  while (v<0) v+=np;
  while (v>=np) v-=np;
  return v;
}

int  Ipowerof2(int v)
{
  int i=2;
  while(i<v)
      i=i*2;
  return i;
}

float   Fcyclic(float v, float maxv)
{
  while (v<0.0)
      v+=maxv;
  while (v>=maxv)
      v-=maxv;
  return v;
}

int Fsamepoint(float x1,float y1,float z1,float x2,float y2,float z2,float maxdist)
{
  float distx=fabs(x1-x2);
  float disty=fabs(y1-y2);
  float distz=fabs(z1-z2);
  float dist=Fmax(distx,disty);
  dist=Fmax(dist,distz);
  return (dist<maxdist);
}

float Fdist(float x1,float y1,float z1,float x2,float y2,float z2)
{
  return sqrt(pow(x1-x2,float(2.0))+pow(y1-y2,float(2.0))+pow(z1-z2,float(2.0)));
}

float Fsqdist(float x1,float y1,float z1,float x2,float y2,float z2)
{
  return (pow(x1-x2,float(2.0))+pow(y1-y2,float(2.0))+pow(z1-z2,float(2.0)));
}





