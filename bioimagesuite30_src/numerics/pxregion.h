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

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 


_Module_Name : pxsegment

_Description : PX2DRegion Class 

_Call : 1. PX2DRegion(PXImage* InputImage,int numiter,
	              float mean1,float mean2,float sigma1,float sigma2);


_References : Chakraborty et al (ICCV) 1995
              
	      
_I/O : 

_System : Unix
_Remarks : This is `translated' from Amit's original code. 
           mean1,mean2,sigma1,sigma2 in range 0..1 -- This are scaled by the max
	   image brightness.
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 22nd Feb 1996
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PX_REGION
#define _PX_REGION

#include <math.h>
#include "pxfloatimage.h"
#include "pximage.h"
// Defining namespace for old code

// -------------------------------------------------------------------------
//                      PX2DRegion Class
// -------------------------------------------------------------------------


struct FP_texture
{
  int   numzones;
  float mean[10],a1[10],a2[10],a3[10],a4[10],sigma[10];
};

class PX2DRegion {

  protected :  
  FP_texture    textureparams;
  PXImage       *outputimage;
  PXFloatImage  *gradx,*grady;
  unsigned char hasderivatives,hasoutput;

public:
  PX2DRegion();
  virtual ~PX2DRegion();

  // Access Functions
  // ----------------
  PXImage* getoutputimage()    { return outputimage; }
  float    getgradx(float x,float y);
  float    getgrady(float x,float y);

  unsigned char derivativesOK() { return hasderivatives;}
  unsigned char outputOK() { return hasoutput;}

  void     GenerateDerivatives(float sigma);
  void     CreateOutputStructures(PXImage* pximage);

  // ------------------------
  // Amit Chakraborty's Code 
  // ------------------------
  void segment_two_regions(PXImage* in,int mi,
			   float mean1,float mean2,
			   float sig, float sm);
  void segment_three_regions(PXImage* image,
			     int mi,float mean1,float mean2,float mean3,float sig);
  
  void texture_segment(PXImage* image,
		       float beta,int iter,int window,
		       int numtext,int* xi ,int* yi);
  
  // ----------------------------------------------------------------------
  // Auxiliary Code
  // ----------------------------------------------------------------------
  static void integrate(PXImage *im1,PXFloatImage* imx,PXFloatImage* imy);  
  
  static void sort_least(float* a,int n,int* k);
  static void  l_Coeff(float* in,unsigned short* in1,
		       int width,int height,
		       float* meanf,float* a1f,float* a2f,float* a3f,float* a4f,float *sigmaf);
  
};

#endif

