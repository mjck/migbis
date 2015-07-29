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

// pximage.h
// Definition of Class PXimage

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July 1995 papad@noodle.med.yale.edu

_Module_Name : PXImage 

_Description : This class can be used to store objects which are images.
               Also a routine to read Analyse File Slices in pre-allocated
	        PXImages.
               
_Call :   
  (1) PXImage(filename,numbits) - used to load image from disk
  (2) PXImage(filename)         - used to load ScilImage
  (3) PXImage(xsize,ysize)      - used to generate temp image
  (4) PXImage(filename,comment,xsize,ysize) - used to generate saveable image
  (5) PXImage(PXImage*,PXFloatMatrix*) - convolution
  (6) PXImage(PXImage*)          - Copy Constructor
  (7) PXImage(unsigned char*, int x ,int y) -- construct from array
     
_References : None
              
_I/O : PGM, 8-bit and 16-bit binary (and binary+ grl grl16) 
            for saving and loading

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in July 1995
                    2. Added Gradient code & Convolution Code July 31st 1995
		    3. Added ScilImage support August 22nd 1995 8/16 bit
		    4. Added Median Filtering and Thresholding 30/8/95
		    5. Added Binary I/O  and scaling 31/8/95 & Key Codes
		    6. Eliminated PXFloatImage -> Constructor ...
		    7. Shifted Median and added Convolution as functions 14/10/95
		    8. Added getbilinear() - Bilinear Interpolation 14/10/95
		    9. Added GrlScale() to scale greylevel below 65535*grscale
		   10. Added Contour Addition 20th October 1995
		   11. Auto-detected between pgm grl16 and grl8 3/11/95 
		                      (specify .pgm)
		   12. Split LoadGrl and LoadBin and added ability to skip 
		           LoadBin 23 Feb 96
		   13. getbilinear made private 
		   14. Tidied-up .h file added clear protected/public divide
		       added getimage() to  return unsigned char* array of image
		       also added constructor for (unsigned char*) array
		       2nd Sep 1996
		   15. Tidied up filename mess add proper setfilename and added
		       Save(filename) routines, eliminated comment field
		   16. Added ability to trace contour from image
		       (12th November 1997 -- from Francois code)
		   17. Added facility for (signed storage and retrieval)
                       13th Nov 1997 getspixel(i,j) setspixel(i,j,int)
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#--#-#-#-#-#-#-#-#  */

#ifndef _PXImage
#define  _PXImage

class PXFloatMatrix;

#include "pxcontour.h"
#include "pxutil.h"
#include <stdlib.h>
// ********** Constructor Codes *******************************
const int PXIm_Scil       =  0;        // Load Codes;
const int PXIm_Pgm        =  8;
const int PXIm_grl8      =  -8;
const int PXIm_Bin        = 15;
const int PXIm_grl       =  16;
const unsigned short PXIM = 29571;    // Magic Number for PXImage Format;

class PXImage {
  
  // ************** Class Fields *************************************

protected:	

  static float    gradient_mask[5];             // store 5x1 mask for derivatives 
  unsigned short  *image; 			// image storage array;
  int		  xsize,ysize,maxgrayscale;	// size and properties;
  int		  status;			// 1=OK 0=problem;
  char		  *filename;		  	// filename;

  // ******************************************************************************
  //          PUBLIC INTERFACE
  // ******************************************************************************
 
public:
  
  PXImage() {}                               // Dummy Constructor;
  PXImage(int,int);                          // Plain Constructor x*y;
  PXImage(const char*,int , int);                  // Checks for filename too;
  PXImage(const char* fname,int c=PXIm_Pgm);       // Load Constructors int=PXIm_?;
  PXImage(PXImage*);                         // Copy Constructor;
  PXImage(PXImage*,PXFloatMatrix*);          // Convolution I**M;
  PXImage(unsigned const char* img, int x ,int y); // construct from array
  virtual ~PXImage();				



  // [A] setpixel, getpixel getbilinear, getinterp  
  // ---------------------------------------------

  virtual void setpixel(int x, int y, unsigned short g)	   
      {image[y*xsize+x]=g;}
  virtual unsigned short getpixel(int x, int y)  
      {return image[y*xsize+x]; }
  virtual short  getspixel(int x,int y)
      {	unsigned short a=getpixel(x,y);
	return (short)a; }
  virtual void setspixel(int x,int y,short p)
      {	unsigned short px=(unsigned short)p;
	setpixel(x,y,px); }
  float getinterp(float x,float y);
  
  // [B] Access other fields
  // -----------------------

  virtual int getsize()    {return xsize*ysize;}
  virtual int getsizex()   {return xsize;}		
  virtual int getsizey()   {return ysize;}		
  virtual int getwidth()   {return xsize;}		
  virtual int getheight()  {return ysize;}		

  virtual int	getmaxgrayscale()     {return maxgrayscale;}
  virtual void  SetMaxgrayscale();   

  virtual char*	getfilename()      {return filename;}	
  virtual void setfilename(const char * fname,int removesuffix=0);
  
  virtual int	isvalid()          {return status;}	
  virtual void  nowisvalid()       {status=1;}

  virtual unsigned char* getimage();
  virtual unsigned short* getimagepointer() { return image; }
  unsigned char* getslice(float gamma=1.0,int numbytes=1); 
  unsigned char* getpaddedslice(int &w,int &h,float gamma=1.0); 
  unsigned char* scaleslice(int &w,int &h,unsigned char* slice,int numbytes=1,int sclaex=1,int scaley=1); 

  // [C]  I/O Related Code;
  //-----------------------
  virtual int	Save(const char* fname);
  virtual int   SavePgm(const char* fname);
  virtual int	SaveGrl(const char* fname,int bits);
  virtual int	SaveBin(const char* fname,int );

  virtual int	Save();
  virtual int   SavePgm();
  virtual int	SaveGrl(int bits);
  int    	LoadBin(int bits,int skip=0);
  virtual int	SaveBin(int );

  // [D] Image Processing Routines (Median,Convolutions) 
  // ---------------------------------------------------
  virtual void Zero(unsigned short px=0);
  virtual void Threshold(float t);
  virtual void Invert();			
  virtual void Convolve(PXFloatMatrix* kernel);
  virtual void Median(int radius);
  virtual void Scale(float scale);
  virtual void Grlscale(float grscale);

  // [D] Insert and Extract Contours 
  // ---------------------------------------------------
  virtual void AddContour(PXContour* contour,unsigned short grl,
			  unsigned char touching,
			  int offx,int offy);
  virtual PXContour* tracefromimage(int value);

  // [E] GetGradients
  // ----------------
  float GradientX(int x,int y);
  float GradientY(int x,int y);
  float GradientXMask(int x,int y);
  float GradientYMask(int x,int y);
  float Laplacian(int x,int y);
  
  // [F] Get Sub-Pixel Gradients
  // ---------------------------
  float IntGradX(float x,float y);
  float IntGradY(float x,float y);
  float Int5GradX(float x,float y);
  float Int5GradY(float x,float y);
  float IntLaplacian(float x,float y);


  // ******************************************************************************
  //          IMPLEMENTATION SPECIFIC
  // ******************************************************************************

  protected :
  
  unsigned char adjust(float px,float gamma);
  void ConvFloat(PXFloatMatrix*);
  
  virtual void setpixel(unsigned short* px,int x, int y, unsigned short g)	   
      {px[y*xsize+x]=g;}

  virtual int	getpixel(unsigned short* px,int x, int y)  
      {return px[y*xsize+x]; }

  virtual int getbilinear(unsigned short* px,float x,float y);
  virtual int getbilinear(float x,float y);

  // [C]  I/O Related Code;
  //------------------------------------------------------------------------
  virtual int  Load();
  virtual int  LoadPgm();
  virtual int  LoadGrl(int bits);
  virtual int  LoadScil();                
  virtual void	CheckFilename(const char*,int);		
  virtual void	CheckFilename(int mode)    {  CheckFilename(filename,mode); }

};

#endif /*  _PXImage */






