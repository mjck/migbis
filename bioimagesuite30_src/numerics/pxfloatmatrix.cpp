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

#include "pxfloatmatrix.h"


/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July 1995   papad@noodle.med.yale.edu

   see pxfloatmatrix.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
//-----------------------------------------------------------------------
float PXFloatMatrix::gradient_mask[5]={-1.0/12.0 , 8.0/12.0 ,0.0 ,-8.0/12.0 ,1.0/12.0};
//-----------------------------------------------------------------------
PXFloatMatrix::PXFloatMatrix(int x ,int y)
     //  Constructor for new matrixs
{
  if ((x/2)*2==x) x++;
  if ((y/2)*2==y) y++;
  xsize=x;
  ysize=y;
  centerx=x/2;
  centery=y/2;
  matrix=new float[xsize*ysize]; 
}
//-----------------------------------------------------------------------
PXFloatMatrix::~PXFloatMatrix()
{
  delete [] matrix;
}
//-----------------------------------------------------------------------
// Gradient functions 
//-----------------------------------------------------------------------
float PXFloatMatrix::GradientX(int x,int y)
{ 
  return getpixel(x+1,y)-getpixel(x,y); 
}

float PXFloatMatrix::GradientY(int x,int y)
{ 
  return getpixel(x,y+1)-getpixel(x,y);  
}
  
float PXFloatMatrix::GradientXMask(int x,int y)
{
  float sum=0;
  for(int i=-2;i<=2;i++)
      sum=sum+(gradient_mask[i+2]*getpixel(x+i,y));
  return sum;
}

float PXFloatMatrix::GradientYMask(int x,int y)
{
  float sum=0;
  for (int i=-2;i<=2;i++)
      sum=sum+(gradient_mask[i+2]*getpixel(x,y+i));
  return sum;
}
//-----------------------------------------------------------------------
//  Gaussian Generate Functions
//-----------------------------------------------------------------------
void PXFloatMatrix::GenGaussian(float sigma)
{
  if (sigma<0.1) sigma=0.11;
  for (int i=0;i<xsize;i++)
      for (int j=0;j<ysize;j++)
	  {
	    setpixel(i,j,(1/(sigma*2*3.1415926854)*
			  exp(-0.5*((0.0+(i-centerx)*(i-centerx)+(j-centery)*(j-centery))
				    /pow(sigma,float(2.0))))));
	  }
  Normalize(0);
}
//-----------------------------------------------------------------------
void PXFloatMatrix::GenGaussian1Dx(float sigma) // all in X-Direction !;
{
  if (sigma<0.1) sigma=0.1;
  for (int i=0;i<xsize;i++)
      {
	float temp=(1/(sqrt(sigma*2*3.1415926854))*
		    exp(-0.5*(0.0+(i-centerx)*(i-centerx)/pow(sigma,float(2.0)))));
	for (int j=0;j<ysize;j++)
	    setpixel(i,j,temp);
      }
  Normalize(1);
}
//-----------------------------------------------------------------------
void PXFloatMatrix::GenGaussian1Dy(float sigma) // all in Y-Direction !;
{
  if (sigma<0.1) sigma=0.1;
  for (int i=0;i<ysize;i++)
      {
	float temp=(1/(sqrt(sigma*2*3.1415926854))*
		    exp(-0.5*(0.0+(i-centery)*(i-centery)/pow(sigma,float(2.0)))));
	for (int j=0;j<xsize;j++)
	    setpixel(j,i,temp);
      }
  Normalize(1);
}
//-----------------------------------------------------------------------
void PXFloatMatrix::GenGradGaussian1Dx(float sigma) // all in X-Direction !;
{
  if (sigma<0.01) sigma=0.01;
  for (int i=0;i<xsize;i++)
    {
      float temp=(1/(sqrt(sigma*2*3.1415926854))*(float(centerx-i)/pow(sigma,float(2.0)))*
		  exp(-0.5*(0.0+(i-centerx)*(i-centerx)/pow(sigma,float(2.0)))));
      for (int j=0;j<ysize;j++)
	  setpixel(i,j,temp);
    }
  Normalize(1);
}
//-----------------------------------------------------------------------
void PXFloatMatrix::GenGradGaussian1Dy(float sigma) // all in Y-Direction !
{
  if (sigma<0.01) sigma=0.01;
  for (int i=0;i<ysize;i++)
      {
	float temp=(1/(sqrt(sigma*2*3.1415926854))*(float(centery-i)/pow(sigma,float(2.0)))*
	      exp(-0.5*(0.0+(i-centery)*(i-centery)/pow(sigma,float(2.0)))));
	for (int j=0;j<xsize;j++)
	    setpixel(j,i,temp);
      }
  Normalize(1);
}
//-----------------------------------------------------------------------
void PXFloatMatrix::GenGrad2Gaussian1Dx(float sigma) // all in X-Direction !;
{
  if (sigma<0.1) sigma=0.1;
  float A=1/( sqrt(2.0*3.1415926854) * pow(sigma,float(3.0)) );
  float S2=pow( sigma, float(2.0) );
  for (int i=0;i<xsize;i++)
       {
	float X2=pow( float(centerx-i), float(2.0) );
	float temp= A*(X2/S2 - 1.0)*exp(-0.5*(X2/S2));
	for (int j=0;j<ysize;j++)
	    setpixel(i,j,temp);
      }
  Normalize(1);
}

//-----------------------------------------------------------------------
void PXFloatMatrix::GenGrad2Gaussian1Dy(float sigma) // all in Y-Direction !
{
  if (sigma<0.1) sigma=0.1;
  float A=1/( sqrt(2.0*3.1415926854) * pow(sigma,float(3.0)) );
  float S2=pow( sigma, float(2.0) );
  for (int i=0;i<ysize;i++)
    {
      float Y2=pow( float(centery-i), float(2.0) );
      float temp= A*(Y2/S2 - 1.0)*exp(-0.5*(Y2/S2));
      for (int j=0;j<xsize;j++)
	  setpixel(j,i,temp);
    }
  Normalize(1);
}
//-----------------------------------------------------------------------
void PXFloatMatrix::LapGaussian(float sigma)
{
  if (sigma<0.1) sigma=0.1;
  for (int i=0;i<xsize;i++)
      for (int j=0;j<ysize;j++)
	  {
	    float r=(float)((i-centerx)*(i-centerx)+(j-centery)*(j-centery));
	    setpixel(i,j,(-1.0/(3.1415926854*pow(sigma,4))*(1-0.5*pow(r,float(2.0))/pow(sigma,float(2.0)))
			  *exp(-0.5*pow(r,float(2.0))/pow(sigma,float(2.0)))));
	  }
  Normalize(1);
}
//-----------------------------------------------------------------------
void PXFloatMatrix::Normalize(int absolutevalue)
{
  float sum=0.0;
  int ia,ib;
  for(ia=0;ia<xsize;ia++)
      for(ib=0;ib<ysize;ib++)
	  {
	    if (absolutevalue)
		sum=sum+fabs(getpixel(ia,ib));
	    else
		sum=sum+getpixel(ia,ib);
	  }
  for(ia=0;ia<xsize;ia++)
      for(ib=0;ib<ysize;ib++)
	  setpixel(ia,ib,getpixel(ia,ib)/sum);
}

//-----------------------------------------------------------------------



