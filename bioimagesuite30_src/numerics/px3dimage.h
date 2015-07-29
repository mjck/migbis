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

// px3dimage.h
// Definition of Class PX3DImage

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris September 1995 papad@noodle.med.yale.edu


_Module_Name : PX3DImage 

_Description : This class can be used to store objects which are 3D images.
               Derived class from PXImage --> see pximage.h
	       
	       
_Call :   
  (1) PX3DImage(int x,int y,int z,int wd=1);        // Specify Column Width   
  (2) PX3DImage(const char*,int,int,int,int wd=1);  // Checks for filename too;
  (3) PX3DImage(const char*,int wd=1);                    // Load Constructors int=cwidth
  (4) PX3DImage(const char*,int,int,int in=1,int wd=1);   // Load Co  int=f:l:i:cwd
  (5) PX3DImage(PXImage *Sequence[],int ,int wd=1); // Stack Constructor;
  (6) PX3DImage(PX3DImage *)                        // Copy Constructor

_References : None
              
	      
_I/O : Analyse Format (16-bit)

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 22nd September 1995 - tested OK
                    2. Added Copy Constructor 7th October 1995
		    3. Reduced number of constructors by adding default arguments
		       and added getslice? functions 2nd Sep 1996
		    4. Cleaned up 3d interpolation  25th Aug 1997
		    5. Cleaned Save Bug to account for unequal height/width also automatic 8-bit save 
		       if (max<256)
		    6. Set Code for using signed short read/write 7th Jan 1997
		       getsvoxel(x,y,z) and setsvoxel(x,y,z,p) and gets3dinterp(x,y,z)
		    7. Added code to generate slices from echo data (with transparency enabled)
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#--#-#-#-#-#-#-#-#  */

#ifndef _PX3DImage
#define  _PX3DImage

#include "pximage.h"

class PX3DImage : public PXImage {
  
  // -----------------------------------------------------------------------;
  // Class Fields;
  // -----------------------------------------------------------------------;

protected:	

  int   zsize,norows,nocolumns;         // Size in z and arrangement in matrix;
  int   partwidth,partheight;           // Component Images Size;
  int   numbytes;
  int   polarmode;
  float outplanescale;

  // ******************************************************************************
  //          PUBLIC INTERFACE
  // ******************************************************************************
  
public:

  PX3DImage(int x,int y,int z,int wd=1);     
  PX3DImage(const char* fname,int wd=1);            // Load Constructors int=cwidth
  PX3DImage(const char* fname,int first,int last ,int increment =1,int wd=1);  
                                                  // Load Constructors int=PXIm_?;
  PX3DImage(PXImage *Sequence[],int n,int wd=1,int x0=0,int y0=0,
	                                       int x1=-1,int y1=-1);  // Stack Constructor;
  PX3DImage(PX3DImage *Sequence[],int n);  // 3D Stack Constructor;

  PX3DImage(PX3DImage *OldImage);                 // Copy Constructor;
  virtual ~PX3DImage();				
  virtual void init();

  // [A] Voxel Manipulation
  // ----------------------
  virtual void setvoxel(int x,int y,int z,unsigned short g)
    { int row=z/nocolumns;      int column=z-row*nocolumns;
      image[x+column*partwidth+xsize*(y+row*partheight)]=g;  }
  virtual unsigned short getvoxel(int x,int y,int z)
    { int row=z/nocolumns;     int column=z-row*nocolumns;
      return image[x+column*partwidth+xsize*(y+row*partheight)]; }
  unsigned short get3dechovoxel(float x,float y,float z,float middlex);
  
  virtual short  getsvoxel(int x,int y,int z)
      {	unsigned short a=getvoxel(x,y,z);
	return (short)(a-(65536*(a>32767))); }
  virtual void setsvoxel(int x,int y,int z,short p)
      {	unsigned short px=p+65536*(p>0);
	setvoxel(x,y,z,px); }
  virtual float gets3dinterp(float x,float y,float z)
      { float a=get3dinterp(x,y,z);
	return a-(65536.0*(a>32767.5)); }
  
  float get3dinterp(float x,float y,float z);



  // [B] Get 3D sizes
  // ----------------
  virtual int get3dsizez()    {return zsize;}
  virtual int get3dsizex()    {return partwidth;}
  virtual int get3dsizey()    {return partheight;}

  virtual int	getwidth()    {return get3dsizex();}		
  virtual int	getheight()   {return get3dsizey();}		
  virtual int	getdepth()    {return get3dsizez();}		

  virtual void Scale(float scale);

  virtual void  setpolarmode(int mode,int )        { polarmode=mode>0;}
  virtual int   getpolarmode()                { return polarmode;}
  virtual int   setoutplanescale(float scale,int fo=0) { outplanescale=fabs(scale); return 1; }
  virtual float getoutplanescale()            { return outplanescale;}

  // [C] I/O Related Code
  // --------------------
  virtual int	Save(const char* fname);
  virtual int	Save();
  virtual int	SaveImg(int bits=-1);

  // [D] Get Slices 
  // ---------------
  unsigned char* getslicex(int x,float gamma=1.0,int numbytes=1);    // No-interpolation
  unsigned char* getslicey(int y,float gamma=1.0,int numbytes=1);
  unsigned char* getslicez(int z,float gamma=1.0,int numbytes=1);

  unsigned char* getinterpslicex(float x,float gamma=1.0,int numbytes=1);  // Interpolate
  unsigned char* getinterpslicey(float y,float gamma=1.0,int numbytes=1);
  unsigned char* getinterpslicez(float z,float gamma=1.0,int numbytes=1);

  unsigned char* getechoslicetheta(int slice,float gamma=1.0,int numbytes=1);
  unsigned char* getechoslicexy(float zlevel,float gamma=1.0,int numbytes=1,float middlex=-1.0);

  // [E] Other Code
  // --------------
  virtual void Add3DContour(PXContour* contour,unsigned short grl,
			    unsigned char touching,
			    int offx,int offy,int offz);
  
  // **** 3D -> 2D  Conversion ****

  virtual int getpixelx(int x,int ,int z)
    { int row=z/nocolumns;  int column=z-row*nocolumns;
      return x+column*partwidth; }
  virtual int getpixely(int ,int y,int z)
    { int row=z/nocolumns;     return (y+row*partheight);  }

  // **** 2D -> 3D  Conversion ****
  virtual int getvoxelx(int x,int)
      { int column=(x/partwidth);  return x-column*partwidth; }
  
  virtual int getvoxely(int ,int y)
      { int row=(y/partheight);    return y-row*partheight;  }
  
  virtual int getvoxelz(int x,int y)
      { int column=(x/partwidth);  int row=(y/partheight);
	return column+row*nocolumns; }
  
  virtual int getopt(int z)
      {
	int b=(int)sqrt((float)(z));
	if (b==0) return 1;
	if (b*b+1>z) return b;
	else return b+1;
      }
   
  int  LoadBin(const char* fname,int numbytes=2,int offset=0);


  static int saveArray(const char* filename,PX3DImage** sequence,int n);


protected:

  // ******************************************************************************
  //          IMPLEMENTATION SPECIFIC
  // ******************************************************************************

  virtual int  Load();
  virtual int  LoadPart(int first,int last,int increment);
  virtual void CheckAnaFilename(int mode);
  

  // **** 2.5D Sizes ****

  virtual int getnorows()    { return norows;}
  virtual int getnocolumns() { return nocolumns;}

  virtual int get3dbilinear(float x,float y,int z)
    { int row=z/nocolumns;      int column=z-row*nocolumns;
      return getbilinear(x+(float)(column*partwidth),
			 y+(float)(row*partheight));  }
  

  //  virtual void getechovoxel(float x,float y,float z,float middlex);

};

#endif /*  _PX3DImage */
















