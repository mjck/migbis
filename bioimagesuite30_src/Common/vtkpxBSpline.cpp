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





/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxMath.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/


#include "vtkMath.h"
#include "pxutil.h"
#include "vtkObjectFactory.h"
#include "vtkDoubleArray.h"
#include "vtkpxBSpline.h"
#include "fitpack.h"

// ------------------------------------------------------------------------

vtkpxBSpline* vtkpxBSpline::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxBSpline");
  if(ret)
    {
      return (vtkpxBSpline*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxBSpline;
}
// ------------------------------------------------------------------
vtkpxBSpline::vtkpxBSpline() 
{
  Degree=3;
  NumberOfPoints=0;
  NumberOfKnots=0;
  NumberOfControlPoints=0;
  controlpoints=NULL;
  knots=NULL;
  this->Offset=0.0;
}
// ------------------------------------------------------------------
vtkpxBSpline::~vtkpxBSpline() 
{
  Cleanup();
}

void vtkpxBSpline::Cleanup()
{
  if (controlpoints!=NULL)
    delete [] controlpoints;
  if (knots!=NULL)
    delete [] knots;
}

// ------------------------------------------------------------------
int  vtkpxBSpline::GetControlPointIndex(int i)
{
  return Irange(i,0,this->NumberOfControlPoints-1);
}
// --------------------------------------------------------------------------
float vtkpxBSpline::GetControl(int i)
{
  if (controlpoints==NULL)
    return 0.0;
  return controlpoints[GetControlPointIndex(i)];
}

void  vtkpxBSpline::SetControl(int i,float v)
{
  if (controlpoints==NULL)
    return;
  controlpoints[GetControlPointIndex(i)]=v;
}

void  vtkpxBSpline::ShiftControl(int i,float v)
{
  if (controlpoints==NULL)
    return;
  controlpoints[GetControlPointIndex(i)]+=v;
}

float vtkpxBSpline::GetKnot(int i)
{
  if (knots==NULL)
    return 0.0;
  return knots[Irange(i,0,NumberOfKnots-1)];

}

// ----------------------------------------------------------------------------------
int   vtkpxBSpline::GetControlPointKnotPosition(int index)
{
  int i=this->GetControlPointIndex(index);
  return i+this->Degree-1;
}

float vtkpxBSpline::GetControlPointLowerBound(int index)
{
  int lb=this->GetControlPointKnotPosition(index);
  return GetKnot(lb-this->Degree+1);
}

float vtkpxBSpline::GetControlPointUpperBound(int index)
{
  int lb=this->GetControlPointKnotPosition(index);
  return GetKnot(lb+this->Degree-1);
}

void  vtkpxBSpline::GetControlPointBounds(int index,float bounds[2])
{
  bounds[0]=this->GetControlPointLowerBound(index);
  bounds[1]=this->GetControlPointUpperBound(index);
}
// ----------------------------------------------------------------------------------
float vtkpxBSpline::GetPosition(float s)
{
  if (knots==NULL)
    return 0.0;
  
  integer ier=0;
  integer M=1;
  integer K=3;
  integer N=this->NumberOfKnots;
  integer NC=this->NumberOfKnots;
  integer mx=3;
  real    X[3];
  integer dimension=1;

  // Calling FITPACK 
  curev_(&dimension,
	 knots,&N,
	 controlpoints,&NC,
	 &K,&s,&M,X,&mx,&ier);

  return X[0];
}

// ----------------------------------------------------------------------------------
// B-Spline Fitting
// ----------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------

int vtkpxBSpline::FitArray(vtkDoubleArray* input,int npoints,float smooth,int component)
{
  return this->FitArray(input,NULL,npoints,smooth,component);
}


int vtkpxBSpline::FitArray(vtkDoubleArray* input,vtkDoubleArray* ordinate,int npoints,float smooth,int component)
  /*PXContour* cnt_in,real smth,real cntrsmth,
    int deg,int cl,int uniform,int npoints,int iterations)*/
{
  static int printed=0;
  integer  M       =input->GetNumberOfTuples();
  int useordinate=0;
  if (ordinate!=NULL)
    {
      if (ordinate->GetNumberOfTuples()!=M)
	{
	  fprintf(stderr,"Unequal Arrays specified for fitarray\n");
	  return 0;
	}
    }

  //  fprintf(stderr,"M=%d\n",M);
  component=Irange(component,0,input->GetNumberOfComponents()-1);

  real*    X   =new real[M];
  real*    Y   =new real[M];
  real*    W   =new real[M];
  integer  IOPT=0;

  for (int k=0;k<M;k++)
    {
      Y[k]=input->GetComponent(k,component);
      X[k]=float(k);
      W[k]=1.0/float(M);
    }

  integer N;
  real XB=-this->Offset;
  real XE=real(M-1)+this->Offset;

  integer NEST=M;
  real* T=new real[NEST];

  if (npoints>0)
    {
      int minv=this->Degree+1;
      N=npoints+2*this->Degree;
      int between=N-2*minv+1;
      IOPT=-1;

      int i;
      for(i=0;i<minv;i++)
	T[i]=XB;
      for(i=N-minv;i<N;i++)
	T[i]=XE;
      
      for (i=minv;i<(N-minv);i++)
	T[i]= float(i-minv+1)/float(between)*(XE-XB)+XB;
    }
    
  integer K=this->Degree;
  real S=smooth;
  real* C=new real[NEST];
  real FP;
  integer LWRK=2*(M*(K+1)+NEST*(7+3*K));
  real* WRK=new real[LWRK];
  integer* IWRK=new integer[NEST];
  integer IER=0;

      
  //  fprintf(stderr,"On to curfit\n");
  int ret=curfit_(&IOPT,&M,X,Y,W,&XB,&XE,&K,&S,&NEST,&N,T,C,&FP,WRK,&LWRK,IWRK,&IER);


  if (IER<=0)
    {
      //      fprintf(stderr,"Curfit output OK IER=%d N=%d FP=%.3f\n",(int)IER,(int)N,FP);
      this->Cleanup();
      this->NumberOfPoints=N-2*K+2;
      this->NumberOfKnots=(this->NumberOfPoints-2)+2*this->Degree;   
      this->NumberOfControlPoints=this->NumberOfPoints;
      
      this->knots=new real[this->NumberOfKnots];
      this->controlpoints=new real[this->NumberOfControlPoints];

      float bounds[2];
      for (int j=0;j<this->NumberOfKnots;j++)
	this->knots[j]=T[j];

      for (int j=0;j<this->NumberOfControlPoints;j++)
	this->controlpoints[j]=C[j];

      for (int ia=0;ia<this->NumberOfControlPoints;ia++)
	{
	  int center=this->GetControlPointKnotPosition(ia);
	  float p=this->GetKnot(center);
	  this->GetControlPointBounds(ia,bounds);
	  //	  fprintf(stderr,"controlpoint(%d) = %.2f p=%.2f (%.2f : %.2f)\n",ia,C[ia],p,bounds[0],bounds[1]);
	}

      ret=1;
    }
  else
    {
      fprintf(stderr,"Curfit error IER=%d\n",(int)IER);
      ret=0;
    }
      
  printed=1;

  delete [] T;
  delete [] C;
  delete [] WRK;
  delete [] IWRK;
  delete [] W;
  delete [] X;
  delete [] Y;

  return ret;
}





