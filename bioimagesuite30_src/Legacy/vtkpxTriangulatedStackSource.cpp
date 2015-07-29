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
  Module:    $RCSfile: vtkpxTriangulatedStackSource.cpp,v $
  Language:  C++
  Date:      $Date: 2003/05/28 21:03:01 $
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
#include "pxutil.h"
#include "pxgeometrystruct.h"
#include "pxcontourstack.h"
#include "pxfpbsplinestack.h"
#include "vtkCellArray.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkpxTriangulatedStackSource.h"
#include "vtkFloatArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkTriangleFilter.h"
#include "vtkPointData.h"
//------------------------------------------------------------------------------
vtkpxTriangulatedStackSource* vtkpxTriangulatedStackSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxTriangulatedStackSource");
  if(ret)
    {
    return (vtkpxTriangulatedStackSource*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxTriangulatedStackSource;
}
//------------------------------------------------------------------------------
vtkpxTriangulatedStackSource::vtkpxTriangulatedStackSource()
{
  this->TriangulatedStack=new PXTriangulatedStack();
  this->ColorMode=5;
  this->ColorGamma=1.0;
}
//------------------------------------------------------------------------------
vtkpxTriangulatedStackSource::~vtkpxTriangulatedStackSource()
{
  if (this->TriangulatedStack)
      delete this->TriangulatedStack;

}
//------------------------------------------------------------------------------
int vtkpxTriangulatedStackSource::Save(const char* fname)
{
  if (this->TriangulatedStack==NULL)
      return 0;
  return this->TriangulatedStack->Save(fname);
}
//------------------------------------------------------------------------------
int vtkpxTriangulatedStackSource::Load(const char* fname)
{
  int ok=this->TriangulatedStack->Load(fname);
  if (ok)
    this->Modified();

  return ok;
}
//------------------------------------------------------------------------------
PXTriangulatedStack* vtkpxTriangulatedStackSource::GetTStack()
{
  return this->TriangulatedStack;
}
//------------------------------------------------------------------------------
void vtkpxTriangulatedStackSource::Copy(vtkpxTriangulatedStackSource* other)
{
  this->TriangulatedStack->Copy(other->GetTStack());
  this->Modified();
}
//------------------------------------------------------------------------------
void vtkpxTriangulatedStackSource::SetFromSplineStackSource(vtkpxSplineStackSource* vtksur,float dist,
							    float zmin,float zmax)
{
  PXFPBSplineStack* sur=vtksur->GetSplineStack();
  int tb=sur->gettrimbottom();
  int tt=sur->gettrimtop();
  sur->autosettrims(zmin,zmax);

  PXContourStack* stack=sur->exporttocontourstack(dist);
  this->TriangulatedStack->setfromcontourstack(stack,0);
  delete stack;
  
  sur->settrimbottom(tb);
  sur->settrimtop(tt);
  this->Modified();
}
//------------------------------------------------------------------------------
void vtkpxTriangulatedStackSource::SetFromSplineStackSource2(vtkpxSplineStackSource* vtksur,float dist,float zdist,
							    float zmin,float zmax)
{
  PXFPBSplineStack* sur=vtksur->GetSplineStack();
  int tb=sur->gettrimbottom();
  int tt=sur->gettrimtop();
  sur->autosettrims(zmin,zmax);

  fprintf(stderr,"Exporting to Contour Stack %.2f %.2f\n",dist,zdist);
  PXContourStack* stack=NULL;
  if (vtksur->GetPolarMode()==0)
    {
      stack=sur->exporttocontourstack(dist,zdist);
    }
  else
    {
      stack=sur->polarexporttocontourstack(vtksur->GetPolarMiddle(),dist);
      stack->nonShrinkSmooth2D(dist,1.0,50);
    }
  this->TriangulatedStack->setfromcontourstack(stack,0);
  delete stack;

  sur->settrimbottom(tb);
  sur->settrimtop(tt);
  this->Modified();
}
//------------------------------------------------------------------------------
void vtkpxTriangulatedStackSource::SetFromContourStack(PXContourStack* stack)
{
  this->TriangulatedStack->setfromcontourstack(stack,0);
  this->Modified();
}

//------------------------------------------------------------------------------
void vtkpxTriangulatedStackSource::ComputeCurvatures(int scale)
{
 if (this->TriangulatedStack->hasneighbours()==0)
    this->TriangulatedStack->generateNeighbours(0);
 this->TriangulatedStack->calculateCurvatures(scale);
}
//------------------------------------------------------------------------------
void vtkpxTriangulatedStackSource::Execute()
{
  int nump=this->TriangulatedStack->getnumpoints();
  int numt=this->TriangulatedStack->getnumtriangles();

  if (nump<3 || numt < 1 )
    return;

  vtkPoints *points = vtkPoints::New();
  points->SetNumberOfPoints(nump);

  vtkFloatArray* norm=vtkFloatArray::New();
  
  int hascurvatures=this->TriangulatedStack->hascurvatures();
  int hasnormals=this->TriangulatedStack->hasnormals();

  if (hasnormals)
    {
      norm->SetNumberOfComponents(3);
      norm->SetNumberOfTuples(nump);
    }

  for (int i=0;i<nump;i++)
    {
      PXTriangulatedStackPoint* point=this->TriangulatedStack->getpoint(i);
      points->SetPoint(i,point->x[0],point->x[1],point->x[2]);
      if (hasnormals)
	{
	  norm->SetComponent(i,0,point->nx[0]);
	  norm->SetComponent(i,1,point->nx[1]);
	  norm->SetComponent(i,2,point->nx[2]);
	}
    }

  vtkCellArray *polys = vtkCellArray::New();

  for (int j=0;j<numt;j++)
    {
      PXTriangle* tri=this->TriangulatedStack->gettriangle(j);
      vtkIdType pp[3];
      for (int ia=0;ia<=2;ia++)
	pp[ia]=tri->p[ia];
	
      polys->InsertNextCell(3,pp);
    }
  

  vtkPolyData *output = this->GetOutput();
  output->SetPoints(points);
  points->Delete();
  output->SetPolys(polys);
  polys->Delete();

  if (hasnormals)
    {
      output->GetPointData()->SetNormals(norm);
    }
  norm->Delete();

  if (hascurvatures>0 && this->ColorMode>=0)
    {
      vtkUnsignedCharArray* curvcolor=vtkUnsignedCharArray::New();
      this->ComputeColors(curvcolor);
      output->GetPointData()->SetScalars(curvcolor);
      curvcolor->Delete();
    }



}
//------------------------------------------------------------------------------
void vtkpxTriangulatedStackSource::SetFromPolyData(vtkPolyData* polydata)
{
  if (polydata==NULL)
    return;
  
  vtkTriangleFilter* filt=vtkTriangleFilter::New();
  filt->SetInput(polydata);
  filt->PassLinesOff();
  filt->PassVertsOff();
  filt->Update();

  vtkPoints* pts=filt->GetOutput()->GetPoints();
  vtkCellArray* cells=filt->GetOutput()->GetPolys();

  int nump=pts->GetNumberOfPoints();
  int numt=cells->GetNumberOfCells();
  this->TriangulatedStack->init(nump,numt);

  double p[3];
  for (int ia=0;ia<nump;ia++)
    {
      PXTriangulatedStackPoint* pt=this->TriangulatedStack->getpoint(ia);
      pts->GetPoint(ia,p);
      for (int ib=0;ib<=2;ib++)
	pt->x[ib]=p[ib];
    }


  cells->InitTraversal();
  vtkIdType *ln,num;
  for (int ic=0;ic<numt;ic++)
    {
      cells->GetNextCell(num,ln);
      PXTriangle* tri=this->TriangulatedStack->gettriangle(ic);
      for (int ib=0;ib<=2;ib++)
	tri->p[ib]=ln[ib];
    }

  filt->Delete();
}
//------------------------------------------------------------------------------
void vtkpxTriangulatedStackSource::NonShrinkSmooth(float alpha,float beta,int iterations)
{
  if (this->TriangulatedStack->hasneighbours()==0)
    this->TriangulatedStack->generateNeighbours(0);
  this->TriangulatedStack->nonShrinkSmooth(0.33,-0.34,iterations);
}
//------------------------------------------------------------------------------
void vtkpxTriangulatedStackSource::Scale(float scale)
{
  this->TriangulatedStack->scalepoints(scale,scale,scale);
}
//------------------------------------------------------------------------------
void vtkpxTriangulatedStackSource::LoadAndFitContourStack(const char* fname)
{
  PXContourStack* cnt=new PXContourStack();
  int ok=cnt->Load(fname);
  if (ok)
    this->SetFromContourStack(cnt);
  delete cnt;
}
  
//------------------------------------------------------------------------------
void vtkpxTriangulatedStackSource::ComputeColors(vtkUnsignedCharArray* colors)
{
  int numpoints=this->TriangulatedStack->getnumpoints();
  float value=1.0,max_value=0.0,red,green,blue;

  colors->SetNumberOfComponents(3);
  colors->SetNumberOfTuples(numpoints);



  int minpass=1;
  if (this->ColorMode==4)
    minpass=0;

  for (int pass=minpass;pass<=1;pass++)
    {
      for (int i=0;i<numpoints;i++)
	{
	  PXTriangulatedStackPoint* point=this->TriangulatedStack->getpoint(i);

	  switch (this->ColorMode)
	    {  
	      //(K=0, H=1, k1=2, k2=3, B=4, Index=5)
	    case 0: // Gaussian
	      value=point->k[0]*point->k[1];
	      break;
	    case 1: // Mean Curvature
	      value=0.5*(point->k[0]+point->k[1]);
	      break;
	    case 2: // Principal Curvature 1
	      value=point->k[0];
	      break;
	    case 3: // Principal Curvature 2
	      value=point->k[1];
	      break;
	    case 4: // Bending Energy
	      value=sqrt(0.5*(pow(point->k[0],float(2.0))+pow(point->k[1],float(2.0))));
	      break;
	    case 5: // Shape Index 
	      if(point->k[0]==point->k[1])
		  {
		    if(point->k[0]>0.0)  value =  1.0;
		    if(point->k[0]<0.0)  value = -1.0;
		    if(point->k[0]==0.0) value =  0.0;
		  }
	      else
		value = 2.0 / M_PI * atan((point->k[1]+point->k[0])/(point->k[1]-point->k[0]));
	      break;
	    }
	  
	  if (pass==0 && this->ColorMode==4)
	    {
	      if (i==0)
		max_value=fabs(value);
	      else
		max_value=Fmax(fabs(value),max_value);
	    }
	  
	  if (pass==1)
	    {
	      if (this->ColorMode==4)
		{
		  value=pow(value/max_value,float(1.0/this->ColorGamma));
		  green=PXTriangulatedStack::bending_energy_lookup_green(value);
		  red=PXTriangulatedStack::bending_energy_lookup_red(value);
		  blue=PXTriangulatedStack::bending_energy_lookup_blue(value);
		}
	      else if (this->ColorMode==5)
		{
		  red=PXTriangulatedStack::shape_index_lookup_red(value);
		  green=PXTriangulatedStack::shape_index_lookup_green(value);
		  blue=PXTriangulatedStack::shape_index_lookup_blue(value);
		}
	      else
		{
		  value=Frange(value,-0.3,0.3);
		  value=value/(0.6);
		  float v=pow(fabs(value),float(1.0/this->ColorGamma));
		  if (value<0.0)
		    v=0.5-v;
		  else
		    v=0.5+v;
		  v=Frange(v,0.0,1.0);
		  red=PXTriangulatedStack::curvature_lookup_red(v);
		  green=PXTriangulatedStack::curvature_lookup_green(v);
		  blue=PXTriangulatedStack::curvature_lookup_blue(v);
		}

	      colors->SetTuple3(i,red*255.0,green*255.0,blue*255.0);
	    }
	}
    }
}

