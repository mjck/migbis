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

// pxfloatimage.h
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : PXFloatImage

_Description : 
  This class can be used to store objects which are images (float)
               
	       
	       

_Call : 
  (1) PXFloatImage(xsize,ysize)      - used to generate temp image
  (2) PXFloatImage(PXImage* oldimage,PXFloatMatrix* kernel)  
                                  - convolution constructor

_References : see pximage.h
              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in July 1995
                    2. Revision to include binary read 29th August 1995
		    3. Added Magic number 31st August 1995
		    4. Added Convolution Constructor PXImage**PXFloatMatrix
		    5. Added Interpolation (Bilinear) 20th Nov 1995
		    6. Added Save Grl    20th Nov 1995
		    7. Added AddContour  30th Jan 1995

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
#ifndef _PXFloatImage
#define _PXFloatImage

#include <math.h>
#include "pximage.h"
const float PXFLIM = 2905.71;   // Magic Number for PXFloatImage Format

class PXFloatImage {

protected:	
  
  // Class fields self-explanatory;

  static float    gradient_mask[5];             // store 5x1 mask for derivatives 
  float  *image; 		  // image storage array;
  int	 xsize,ysize;	          // size and properties;
  float  maxgrayscale,mingrayscale;
  int	 status;		  // 1=OK 0=problem;
 
public:
  
  // These are the constructors and the destructor respectively
  
  PXFloatImage(int h,int w);
  PXFloatImage(const char*);
  PXFloatImage(PXImage* oldimage,PXFloatMatrix* kernel,
	       int x1=0,int x2=-1,int y1=0,int y2=-1);
  PXFloatImage(PXFloatImage* oldimage);
  PXFloatImage(PXFloatImage* oldimage,PXFloatMatrix* kernel,
	       int x1=0,int x2=-1,int y1=0,int y2=-1);
  virtual ~PXFloatImage();				

  // [A] Get/Set Pixel Values
  // ------------------------
  virtual void	setpixel(int x, int y, float g)	   
    {image[y*xsize+x]=g;}
  virtual float	getpixel(int x, int y)  
    {return image[y*xsize+x]; }
  float getinterp(float x,float y);

  // [B] Get/Set Other Parameters
  // ----------------------------
  virtual int	getsizex()	
    {return xsize;}		
  virtual int	getsizey()	
    {return ysize;}		
  virtual float	getmaxgrayscale() 
    {return maxgrayscale;}
  virtual float	getmingrayscale() 
    {return mingrayscale;}
  virtual float getrange()
    { return (maxgrayscale-mingrayscale)+1.0*( fabs(maxgrayscale-mingrayscale)<0.1); }
  virtual float* getimage() 
    {return image;}   
  virtual int  isvalid()	
    {return status;}	

  unsigned char* getslice(float gamma=1.5,int numbytes=1);
  unsigned char  adjust(float px,float gamma=1.5);
  
  // [C] I/O Related
  //----------------
  /*
     Load and Save   read/write image to the .xbin format
        4-byte float -- 
        first magic no then width then height then data
     
     SaveGrl -- save in 16-bit grl format -- useful for display

     LoadBin and SaveBin - read/write image to binary file
       No dimension header again 4-byte float
     
     Invert          is a diagnostic test - simply inerts image
     */

  virtual void	Load(const char* f);
  virtual void	Save(const char* f);		       
  virtual void  LoadBin(const char* f);
  virtual void  SaveBin(const char* f);
  virtual void  SaveGrl(const char* f);
  virtual void  SavePgm(const char* f);
  virtual void  SetMaxMingrayscale();
  virtual void	Invert();			
  virtual void  AddContour(PXContour* contour,float grl,
			   unsigned char touching,
			   int offx,int offy);
    
  virtual PXImage* getPXImage(int bytes=2);
  // [D] Gradient functions 
  // ----------------------
  float GradientX(int x,int y);
  float GradientY(int x,int y);
  float GradientXMask(int x,int y);
  float GradientYMask(int x,int y);
    
};

#endif /*  _PXFloatImage */















