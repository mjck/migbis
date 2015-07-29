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

// pxfloatmatrix.h

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : PXFlotMatrix

_Description : Class PXFloatMatrix - used to store matrixes of type float
               
_Call :   PXFloatMatrix(int,int);
        
_References : See pximage.h
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in July 1995
                    2. Added Derivatives of Gaussian 20 Nov 95
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
#ifndef _PXFloatMatrix
#define  _PXFloatMatrix

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>


class PXFloatMatrix {

 protected:	
  
  // Class fields self-explanatory;

  static float gradient_mask[5]; // store 5x1 mask for derivatives 
  float  *matrix; 		   // matrix storage array;
  int	 xsize,ysize;	           // size and properties;
  int    centerx,centery;          // radii
  
public:
  
  PXFloatMatrix(int,int);
  virtual ~PXFloatMatrix();				
  
  // [A] Member Access
  // -----------------
  virtual void	setpixel(int x, int y, float g)	   
      {matrix[y*xsize+x]=g;}
  virtual float	getpixel(int x, int y)  
      {return matrix[y*xsize+x]; }
  virtual int	getsizex()	
      {return xsize;}		
  virtual int	getsizey()	
      {return ysize;}		
  virtual int	getradiusx()	
      {return centerx;}		
  virtual int	getradiusy()	
      {return centery;}		
  
  // [B] Gradient functions 
  // ----------------------
  float GradientX(int x,int y);
  float GradientY(int x,int y);
  float GradientXMask(int x,int y);
  float GradientYMask(int x,int y);
  
  // [C] Form Gaussian's for Scale-Space
  // -----------------------------------
  virtual void GenGaussian1Dx(float sigma);
  virtual void GenGaussian1Dy(float sigma);
  virtual void GenGaussian(float sigma);
  virtual void GenGradGaussian1Dx(float sigma);
  virtual void GenGradGaussian1Dy(float sigma);
  virtual void GenGrad2Gaussian1Dx(float sigma);
  virtual void GenGrad2Gaussian1Dy(float sigma);
  virtual void LapGaussian(float sigma);

protected:
  void Normalize(int absolutevalue=0);

};

#endif /*  _PXFloatMatrix */














