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
  Module:    $RCSfile: vtkpxImageAlgebra.h,v $
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

#include "vtkpxImageAlgebra.h"
#include "vtkObjectFactory.h"
#include "vtkpxUtil.h"
#include "vtkImageToImageFilter.h"
#include "vtkDoubleArray.h"
#include "vtkImageProgressIterator.h"

//------------------------------------------------------------------------------
vtkpxImageAlgebra* vtkpxImageAlgebra::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageAlgebra");
  if(ret)
    {
    return (vtkpxImageAlgebra*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageAlgebra;
}

//----------------------------------------------------------------------------
// Constructor sets default values
vtkpxImageAlgebra::vtkpxImageAlgebra()
{
  this->OutputScalarType = -1;
  this->Map=NULL;
}

vtkpxImageAlgebra::~vtkpxImageAlgebra()
{
    this->SetMap(NULL);
}

//----------------------------------------------------------------------------
void vtkpxImageAlgebra::ExecuteInformation(vtkImageData *inData, 
					    vtkImageData *outData)
{
  this->vtkImageToImageFilter::ExecuteInformation( inData, outData );

  if (this->OutputScalarType != -1)
    outData->SetScalarType(this->OutputScalarType);

}

//----------------------------------------------------------------------------
// This templated function executes the filter for any type of data.
template <class IT, class OT>
void vtkpxImageAlgebraExecute(vtkpxImageAlgebra *self,
				     vtkImageData *inData, 
				     vtkImageData *outData, 
				     int outExt[6], int id,  IT *, OT *)
{

  vtkDoubleArray* Map=self->GetMap();
  if (Map==NULL)
    return;

  int numt=Map->GetNumberOfTuples();
  //  fprintf(stderr,"Numt A=%d %d:%d %d:%d %d:%d\n\n",numt,outExt[0],outExt[1],outExt[2],outExt[3],outExt[4],outExt[5]);

  vtkImageIterator<IT> inIt(inData, outExt);
  vtkImageProgressIterator<OT> outIt(outData, outExt, self, id);

  // for preventing overflow
  double outTypeMin = outData->GetScalarTypeMin();
  double outTypeMax = outData->GetScalarTypeMax();
  

  int dt=outData->GetScalarType();
  int doround=1;
  if (dt==VTK_FLOAT || dt==VTK_DOUBLE)
    doround=0;

  /*  fprintf(stderr,"Data type=%d (fl=%d db=%d) round=%d\n",
      dt,VTK_FLOAT,VTK_DOUBLE,doround);*/

  while (!outIt.IsAtEnd())
    {
      IT* inSI = inIt.BeginSpan();
      OT* outSI = outIt.BeginSpan();
      OT* outSIEnd = outIt.EndSpan();

      while (outSI != outSIEnd)
        {
	  double val = ((float)(*inSI)); 
	  
	  int flag=0,test=0;
	  while (test<numt-1 && flag==0)
	    {
	      if (val< Map->GetComponent(test,0))
		{
		  val= Map->GetComponent(test,1)+val*Map->GetComponent(test,2);
		  flag=1;
		}
	      ++test;
	    }
	  
	  if (flag==0)
	    val= Map->GetComponent(numt-1,1)+val*Map->GetComponent(numt-1,2);
	  
	  if (val>outTypeMax)
	    val=outTypeMax;
	  else if (val<outTypeMin)
	    val=outTypeMin;
	  
	  if (doround)
	    *outSI = (OT)(val+0.5);
	  else
	    *outSI = (OT)(val);
	  ++outSI;
	  ++inSI;
	}
      inIt.NextSpan();
      outIt.NextSpan();
    }
}

template <class T>
void vtkpxImageAlgebraExecute1(vtkpxImageAlgebra *self,
			       vtkImageData *inData,
			       vtkImageData *outData,
			       int outExt[6], int id, T *)
{
  switch (outData->GetScalarType())
    {
      vtkTemplateMacro7(vtkpxImageAlgebraExecute, self, inData,
			outData,outExt, id,
			static_cast<T *>(0), static_cast<VTK_TT *>(0));
    default:
      vtkGenericWarningMacro("Execute: Unknown input ScalarType");
      return;
    }
}

//----------------------------------------------------------------------------
// This method is passed a input and output data, and executes the filter
// algorithm to fill the output from the input.
// It just executes a switch statement to call the correct function for
// the datas data types.
void vtkpxImageAlgebra::ThreadedExecute(vtkImageData *inData, 
					vtkImageData *outData,
					int outExt[6], int id)
{
  if (this->Map==NULL)
    {
      vtkErrorMacro(<<"No Mapping Array supplied");
      return;
    }

  int numc=this->Map->GetNumberOfComponents();
  int numt=this->Map->GetNumberOfTuples();

  /*  fprintf(stderr,"Numt=%d numc=%d\n",numt,numc);

  for (int i=0;i<numt;i++)
    fprintf(stderr,"Map i=%d  x=%f \t a1=%f \t a2=%f\n",i,
	    this->Map->GetComponent(i,0),
	    this->Map->GetComponent(i,1),
	    this->Map->GetComponent(i,2));
	    fprintf(stderr,"\n");*/

  /*  int nt1=inData->GetPointData()->GetScalars()->GetNumberOfTuples();
      int nt2=outData->GetPointData()->GetScalars()->GetNumberOfTuples();*/
  
  if (numc!=3)
    {
      vtkErrorMacro(<<"Bad Mapping Array supplied components="<<numc<<"!=3");
      return;
    }
  if (numt<1)
    {
      vtkErrorMacro(<<"Bad Mapping Array supplied tuples="<<numt<<"<1");
      return;
    }

  switch (inData->GetScalarType())
    {
      vtkTemplateMacro6(vtkpxImageAlgebraExecute1,this, 
			inData, outData, outExt, id,static_cast<VTK_TT *>(0));
    default:
      vtkErrorMacro(<< "Execute: Unknown ScalarType");
      return;
    }
}



void vtkpxImageAlgebra::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkImageToImageFilter::PrintSelf(os,indent);
  os << indent << "OutputMap: "<<this->Map;
  os << indent << "Output Scalar Type: " << this->OutputScalarType << "\n";
}


int vtkpxImageAlgebra::CreateMapArrayFromKnots(vtkDoubleArray* knots)
{
  int numknots=knots->GetNumberOfTuples();
  int numcomp=knots->GetNumberOfComponents();

  if (numknots<1 || numcomp!=2)
    return 0;


  //  fprintf(stderr,"Creating map array from knots Numt=%d numc=%d\n",numknots,numcomp);

  vtkDoubleArray*  newmap= vtkDoubleArray::New();
  newmap->SetNumberOfComponents(3);

  int count=0;
  double oldx=0.0,oldy=0.0;

  
  for (int i=0;i<numknots;i++)
    {
      double x=knots->GetComponent(i,0);
      double y=knots->GetComponent(i,1);

      if (count==0)
	{
	  newmap->InsertNextTuple3(x,y,0.0);
	} 
      else 
	{
	  double d1=y-oldy;
	  double d2=x-oldx;
	  double a=0.0;
	  if (d2>0.0)
	    a=d1/d2;
	  double b=y-a*x;
	  newmap->InsertNextTuple3(x,b,a);
	}
      oldx=x;
      oldy=y;
      ++count;
    }
  this->SetMap(newmap);


  for (int i=0;i<numknots;i++)
    {
      double x=newmap->GetComponent(i,0);
      double b=newmap->GetComponent(i,1);
      double a=newmap->GetComponent(i,2);
      double y=a*x+b;

      double b1=newmap->GetComponent(i+1,1);
      double a1=newmap->GetComponent(i+1,2);
      double y1=a1*x+b1;
      //      fprintf(stdout,"Mapping %.2f  -> %f or %f\n",x,y,y1);
    }
	  

  newmap->Delete();
  return count;

}

