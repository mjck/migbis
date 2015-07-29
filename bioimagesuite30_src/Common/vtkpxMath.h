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
//  This file depends heavily on NR code in vtkpxNonLinearFit.cpp
//
//---------------------------------------------------------------------------------------------------------

#ifndef __vtkpxMath_h
#define __vtkpxMath_h

#include "vtkObject.h"



//BTX
class vtkFloatArray;
class vtkPolyData;
#include "pxgeometrystruct.h"
//ETX

class vtkpxMath : public vtkObject
{
public:
  static vtkpxMath *New();
  vtkTypeMacro(vtkpxMath,vtkObject);
  
  // ---------------------------------------------------------------------
  // This Code is Primarily Derived From Numerical Recipes
  // ---------------------------------------------------------------------

  // Description:
  // Conversion Routines 
  static void EuclideanToPolar(float X,float Y,float& R,float& Theta);
  static void PolarToEuclidean(float R,float Theta,float& X,float& Y);

  // Description:
  // Evaluate Gaussian Sigma
  static float Gaussian(float x,float m,float sigma2);
  static float Rayleigh(float x,float beta2);
  
  // Description:
  // Normal Random Generator
  static float GaussianRandom();


  // Description:
  // Ttest to Pvalue
  static float TtestToPvalue(float t,int df);
  static float TtestToPvalue_itk(float t,int df);
  static float PvalueToTvalue(float p,int df);
  static float ZscoreToPvalue(float z);
  static float PvalueToZscore(float z);
  
  // Description:
  // Some Static Utility Functions to compute distances around a circle (radians)
  static float FindClosestAngleP1(float p1,float p2);
  static float FindMinAngleDistance(float p1,float p2);

  // Description:
  // Compute Differences between ph and distance step and store in temp 
  // (input np points output np-step differences)
  static int   ComputeAngleDistancePairs(float* ph,float* temp,int step,int np);



  // Description:
  // Lower Level Numerical Recipes Helper Routines -- do not call directly 
  static float NRSelect(unsigned long k, unsigned long n, float* arr);
  static float NRrofunc(float b,float* xt,float* yt,float& aa,float& abdevt,int ndatat,float* arr);
  
  // Description:
  // Functions To Compute Order Statistics 
  static float SelectKthLargest(unsigned long k0offset, unsigned long n, float* arr0offset);
  static float GetMedian(unsigned long n, float* arr);


  // Description:
  // Fit Line y=ax+b to x,y pairs with temp storage space (output a,b and fit error abdev)
  // Medfit uses absolute deviation, meanfit uses least squares fitting 


  static void  Medfit(float* x, float* y, int ndata, float& a, float& b, float& abdev);
  static void  Meanfit(float* x, float* y, int ndata, float& a, float& b, float& abdev);
  static void WeightedMeanfit(float *x, float *y, int ndata, float *sig, int mwt, float &a,
			      float &b, float &siga, float &sigb, float &chi2, float &q);


  // Description:
  // Use this version of medfit to pre-allocate temporary storage (for many succesive invocations)
  static void  Medfit(float* x, float* y, int ndata, float& a, float& b, float& abdev,float* temp);


  // Description:
  // Sorting Code to either sort an array (Heapsort) or to index an array (IndexArray)
  static void Heapsort(unsigned long n, float* ra0offset);


  // Description:
  // Short Code
  static void Meanfit_Short(short* x, short* y, int ndata, short& a, short& b, short& abdev);
  static void Heapsort_Short(unsigned long n, short* ra0offset);



  // Description:
  //BTX
  static int CalculateCurvatures(vtkPolyData* polydata,int scale,PXIntegerList* neighbours);

  //ETX

  



protected:

};
#endif

