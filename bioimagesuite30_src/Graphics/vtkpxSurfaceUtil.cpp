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
  Module:    $RCSfile: vtkpxSurfaceUtil.cpp,v $
  Language:  C++
  Date:      $Date: 2002/07/15 12:58:44 $
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

#include "pxcontour.h"
#include "pxtriangulatedstack.h"
#include "vtkAppendPolyData.h"
#include "vtkCellArray.h"
#include "vtkContourFilter.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkImageCast.h"
#include "vtkImageData.h"
#include "vtkImageData.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageResample.h"
#include "vtkImageThreshold.h"
#include "vtkIntArray.h"
#include "vtkLookupTable.h"
#include "vtkMath.h"
#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"
#include "vtkPointLocator.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataCollection.h"
#include "vtkPolyDataNormals.h"
#include "vtkSurfaceReconstructionFilter.h"
#include "vtkpxAppendAndLabelPolyData.h"
#include "vtkpxColorMapUtil.h"
#include "vtkpxMath.h"
#include "vtkpxSurfaceUtil.h"
// ---------------------------

std::string vtkpxSurfaceUtil::buffer="";

vtkpxSurfaceUtil* vtkpxSurfaceUtil::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxSurfaceUtil");
  if(ret)
      {
	return (vtkpxSurfaceUtil*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxSurfaceUtil;
}


// --------------------------------------------------------------------------------------
//                      Surface Stuff
// --------------------------------------------------------------------------------------

void vtkpxSurfaceUtil::createsinglesection(vtkPolyData* sep,
				    PXContour* c1,PXContour* c2,
				    float z1,float z2,
				    float du,int debug)
{
  sep->Initialize();
  float z[2],*x[2],*y[2];
  int np[2];
  z[0]=z1; z[1]=z2;
  np[0]=c1->getnumpoints();
  np[1]=c2->getnumpoints();
  PXContour* cn[2];
  cn[0]=c1;
  cn[1]=c2;

  int c;
  for (c=0;c<=1;c++)
      {
	PXContourPoint* pts=NULL;
	PXContour* cntr=NULL;
	if (du>0.01)
	    {
	      cntr=new PXContour(cn[c]); 
	      cntr->Equispace(du);    
	      pts=cntr->gettoppoint();
	      np[c]=cntr->getnumpoints();
	    }
	else
	    {
	      pts=cn[c]->gettoppoint();
	      np[c]=cn[c]->getnumpoints();
	    }

	x[c]=new float[np[c]]; 
	y[c]=new float[np[c]];
	
	for(int i=0;i<np[c];i++)
	    {
	      x[c][i]=pts->x;  y[c][i]=pts->y;
	      pts=pts->next;
	    }
	if (du>0.01)
	    delete cntr;
      }

  // Add the Points 
  vtkPoints    *c_points    = vtkPoints::New();
  c_points->SetNumberOfPoints(np[0]+np[1]);


  int index=0; 
  for (c=0;c<=1;c++)
      {
	for(int i=0;i<np[c];i++)
	    {
	      c_points->SetPoint(index,x[c][i],y[c][i],z[c]);
	      index++;
	    }
      }

  int origin[2];
  PXTriangulatedStack::findorigin2D(np,x,y,origin);
  PXNeighboursList* facelist=new PXNeighboursList;
  int numfaces=PXTriangulatedStack::triangulate2D(np,x,y,origin,facelist,9000);


  // Add the Triangles 
  vtkCellArray *c_lines     = vtkCellArray::New();
  c_lines->Allocate(numfaces,5);

  vtkIdType indices[3];
  facelist->Rewind();
  for (int fc=0;fc<numfaces;fc++)
      {
	PXNeighboursNode* current=facelist->getcurrentnode();
	PXIntegerList* list=(current->points);

	list->Rewind();
	indices[0]  =list->getcurrent();
	list->Next();
	indices[1]=list->getcurrent();
	list->Next();
	indices[2]=list->getcurrent();
	c_lines->InsertNextCell(3,indices);
	facelist->Next();
      }

  sep->SetPoints(c_points);
  c_points->Delete();
  sep->SetPolys(c_lines);
  c_lines->Delete();

  // Clean-up
  for(c=0;c<=1;c++)
      {
	delete [] x[c];
	delete [] y[c];
      }
  delete facelist;
}
// --------------------------------------------------------------
void vtkpxSurfaceUtil::createsinglepolarsection(vtkPolyData* sep,PXContour* c1,PXContour* c2,
					 float theta1,float theta2,float middlex,
					 float du,int debug)
{
  sep->Initialize();
  float *x[2],*y[2],*z[2],theta[2];
  int np[2];
  theta[0]=theta1*M_PI/180.0; 
  theta[1]=theta2*M_PI/180.0;
  np[0]=c1->getnumpoints();
  np[1]=c2->getnumpoints();
  PXContour* cn[2];
  cn[0]=c1;
  cn[1]=c2;
  // fprintf(stderr,"[Ar: %d,%d" ,np[0],np[1]);
  // Create Point Arrays 
  // -------------------
  int c;
  for (c=0;c<=1;c++)
      {
	PXContourPoint* pts=NULL;
	PXContour* cntr=NULL;
	if (du>0.01)
	    {
	      cntr=new PXContour(cn[c]); 
	      cntr->Equispace(du);    
	      pts=cntr->gettoppoint();
	      np[c]=cntr->getnumpoints();
	    }
	else
	    {
	      pts=cn[c]->gettoppoint();
	      np[c]=cn[c]->getnumpoints();
	    }
	// fprintf(stderr,"Numpoints(%d) %d\n",c,np[c]);
	x[c]=new float[np[c]]; 	y[c]=new float[np[c]];	z[c]=new float[np[c]];
	
	for(int i=0;i<np[c];i++)
	    {
	      float r=(pts->x-middlex);
	      x[c][i]=(middlex+r*cos(theta[c]));
	      y[c][i]=(middlex+r*sin(theta[c]));
	      z[c][i]=pts->y;
	      pts=pts->next;
	    }
	if (du>0.01)
	    delete cntr;
      }
  

  // Add Points To Scene 
  // -------------------
  int index=0; 
  vtkPoints    *c_points    = vtkPoints::New();
  c_points->SetNumberOfPoints(np[0]+np[1]);

  for (c=0;c<=1;c++)
      {
	for(int i=0;i<np[c];i++)
	    {
	      c_points->SetPoint(index,x[c][i],y[c][i],z[c][i]);
	      index++;
	    }
      }

  // Clean-up
  // Find Triangulation
  // ------------------
  int origin[2];
  PXTriangulatedStack::findorigin(np,x,y,z,origin);
  PXNeighboursList* facelist=new PXNeighboursList;
  int numfaces=PXTriangulatedStack::triangulate(np,x,y,z,origin,facelist,9000);

  vtkCellArray *c_lines     = vtkCellArray::New();
  c_lines->Allocate(numfaces,5);

  vtkIdType indices[3];
  facelist->Rewind();

  for (int fc=0;fc<numfaces;fc++)
      {
	PXNeighboursNode* current=facelist->getcurrentnode();
	PXIntegerList* list=(current->points);
	list->Rewind();

	indices[0]  =list->getcurrent();
	list->Next();
	indices[1]=list->getcurrent();
	list->Next();
	indices[2]=list->getcurrent();
	c_lines->InsertNextCell(3,indices);
	facelist->Next();
      }

  sep->SetPoints(c_points);
  c_points->Delete();
  sep->SetPolys(c_lines);
  c_lines->Delete();

  for(c=0;c<=1;c++)
      {
	delete [] x[c];
	delete [] y[c];
	delete [] z[c];
      }

  delete facelist;
}

void vtkpxSurfaceUtil::createsection(PXContourStack* stack,vtkPolyData* grp,
			      int polar,float middlex,
			      int begin,int end)
{
  if (begin<0) begin=0;
  if (end>stack->getnumcontours()-1)
      end=stack->getnumcontours()-1;

  if (end<begin)
      return;

  vtkPolyDataCollection* collection=vtkPolyDataCollection::New();
  
  for (int i=begin;i<end;i++)
      {
	vtkPolyData* polydata=vtkPolyData::New();
	if (!polar)
	    vtkpxSurfaceUtil::createsinglesection(polydata,
					   stack->getcontour(i),stack->getcontour(i+1),
					   stack->getzlevel(i),stack->getzlevel(i+1),0.0,1);
	else
	    vtkpxSurfaceUtil::createsinglepolarsection(polydata,
						stack->getcontour(i),stack->getcontour(i+1),
						stack->getzlevel(i),stack->getzlevel(i+1),
						middlex,0.0,1);
	collection->AddItem(polydata);
	polydata->Delete();
      }

  vtkAppendPolyData* append=vtkAppendPolyData::New();
  collection->InitTraversal();
  for (int j=0;j<collection->GetNumberOfItems();j++)
      append->AddInput(collection->GetNextItem());

  append->Update();
  grp->CopyStructure(append->GetOutput());
  append->Delete();
  collection->Delete();
}


void vtkpxSurfaceUtil::drawtstack(vtkPolyData* surface,PXTriangulatedStack* tstack)
{
  if (tstack==NULL || surface==NULL)
      return;

  surface->Initialize();

  vtkPoints *points = vtkPoints::New();
  points->SetNumberOfPoints(tstack->getnumpoints());
  
  vtkCellArray *polys = vtkCellArray::New();

  int i;
  for (i=0; i<tstack->getnumpoints(); i++) 
      {
	PXTriangulatedStackPoint* pt=tstack->getpoint(i);
	points->SetPoint(i,pt->x);
      }
  for (i=0; i<tstack->getnumtriangles(); i++) 
      {
	PXTriangle* tri=tstack->gettriangle(i);
	vtkIdType pp[3];
	for (int ia=0;ia<=2;ia++)
	  pp[ia]=tri->p[ia];

	polys->InsertNextCell(3,pp);
      }
  
  surface->SetPoints(points);
  points->Delete();
  surface->SetPolys(polys);
  polys->Delete();
  

}

//------------------------------------------------------------------------
// Parse Object Maps
//------------------------------------------------------------------------
int vtkpxSurfaceUtil::ObjectMapToPolyData(vtkImageData* image,vtkPolyData* poly)
{
  return vtkpxSurfaceUtil::ObjectMapToPolyData(image,poly,-1,-1);
}
//------------------------------------------------------------------------
int vtkpxSurfaceUtil::ObjectMapToPolyData(vtkImageData* image,vtkPolyData* poly,
					  int minobj,int maxobj,double gaussianxy,double gaussianz, double resample)
{

  //  fprintf(stderr,"Gaussian=%f resample=%f\n",gaussian,resample);

  if (image==NULL || poly==NULL)
    return 0;
 
  double range[2];
  image->GetPointData()->GetScalars()->GetRange(range);
  double sp[3];image->GetSpacing(sp);
  
  if (minobj==-1)
    minobj=(int)range[0];
  if (maxobj==-1)
    maxobj=(int)range[1];

  if (maxobj<minobj)
    {
      int a=minobj;
      minobj=maxobj;
      maxobj=a;
    }
  minobj=Irange(minobj,1,(int)range[1]);
  maxobj=Irange(maxobj,1,(int)range[1]);

  vtkpxAppendAndLabelPolyData* append=vtkpxAppendAndLabelPolyData::New();
  append->UserManagedInputsOn();
  append->SetNumberOfInputs(maxobj-minobj+1);

  resample=Frange(resample,1.0,5.0);
  gaussianxy=Frange(gaussianxy,0.0,10.0);
  gaussianz=Frange(gaussianz,0.0,10.0);

  vtkImageCast* cast=vtkImageCast::New();
  cast->SetInput(image);
  cast->SetOutputScalarTypeToFloat();
  cast->Update();


  for (int level=minobj;level<=maxobj;level++)
    {
      double lowt=double(level)-0.5;
      double hight=double(level)+0.5;
      
      //      fprintf(stderr,"level = %d lowt=%.2f hight=%.2f\n",level,lowt,hight);

      vtkImageThreshold* thr=vtkImageThreshold::New();
      thr->SetInput(cast->GetOutput());
      thr->ThresholdBetween(lowt,hight);
      thr->ReplaceInOn();
      thr->ReplaceOutOn();
      thr->SetInValue(100.0);
      thr->SetOutValue(0.0);
      
      vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
      smooth->SetInput(thr->GetOutput());
      smooth->SetStandardDeviations(gaussianxy,gaussianxy,gaussianz);
      
      vtkImageResample* res=vtkImageResample::New();
      res->InterpolateOn();
      res->SetDimensionality(3);
      for (int ib=0;ib<=2;ib++)
	res->SetAxisOutputSpacing(ib,sp[ib]*resample);
      res->SetInput(smooth->GetOutput());
      res->Update();

      vtkContourFilter* contFilt=vtkContourFilter::New();
      contFilt->SetInput(res->GetOutput());
      contFilt->SetNumberOfContours(1);
      contFilt->SetValue(0,50.0);
      contFilt->ComputeScalarsOff();
      contFilt->ComputeGradientsOff();
      contFilt->ComputeNormalsOff();
      contFilt->Update();

      append->SetInputByNumber(level-minobj,contFilt->GetOutput());
      contFilt->Delete();
      res->Delete();
      smooth->Delete();
      thr->Delete();
    }

  append->Update();
  cast->Delete();
  poly->CopyStructure(append->GetOutput());
  poly->GetPointData()->PassData(append->GetOutput()->GetPointData());
  append->Delete();
  
  return 1;
}
//------------------------------------------------------------------------
int vtkpxSurfaceUtil::MapScalarsToSurface(vtkPolyData* source,vtkPolyData* destination)
{
  if (source==NULL || destination==NULL)
    return 0;

  if (destination->GetPoints()==NULL)
    return 0;

  if (source->GetPointData()==NULL)
    return 0;

  if (source->GetPointData()->GetScalars()==NULL)
    return 0;
	
  vtkPointLocator* locator=vtkPointLocator::New();
  locator->SetDataSet(source);
  locator->BuildLocator();
  
  vtkDataArray* newPtScalars = vtkDataArray::CreateDataArray(source->GetPointData()->GetScalars()->GetDataType());
  newPtScalars->SetNumberOfTuples(destination->GetPoints()->GetNumberOfPoints());
  vtkIdList* idlist=vtkIdList::New();
  idlist->SetNumberOfIds(destination->GetPoints()->GetNumberOfPoints());
  
  for (int i=0;i<destination->GetPoints()->GetNumberOfPoints();i++)
    {
      double x[3];
      destination->GetPoints()->GetPoint(i,x);
      int id=locator->FindClosestPoint(x);
      if (id>=0)
	idlist->SetId(i,id);
      else
	idlist->SetId(i,0);
    }
  
  source->GetPointData()->GetScalars()->GetTuples(idlist,newPtScalars);

  if (source->GetPointData()->GetScalars()->GetLookupTable()!=NULL)
    newPtScalars->SetLookupTable(source->GetPointData()->GetScalars()->GetLookupTable());

  destination->GetPointData()->SetScalars(newPtScalars);
  newPtScalars->Delete();
  idlist->Delete();
  
  return 1;

}
//------------------------------------------------------------------------

int vtkpxSurfaceUtil::AddShapeIndexLookupTableToSurface(vtkPolyData* source)
{
  vtkLookupTable*cmap =vtkLookupTable::New();
  cmap->SetNumberOfTableValues(256);
  float numc=0.5*float(255);
	  
  for (int i=0;i<=255;i++)
    {
      float v=(float(i)-127.5)/numc;

      if (v<-0.875)      { cmap->SetTableValue(i,0.0, 1.0 , 0.3,1.0); }
      else if(v<-0.625) { cmap->SetTableValue(i,0.0, 1.0 , 0.6,1.0); }
      else if(v<-0.375) { cmap->SetTableValue(i,0.0, 1.0 , 1.0,1.0); }
      else if(v<-0.125) { cmap->SetTableValue(i,0.5, 1.0 , 1.0,1.0); }
      else if(v<0.125)  { cmap->SetTableValue(i,1.0, 1.0 , 1.0,1.0); }
      else if(v<0.375)  { cmap->SetTableValue(i,1.0, 1.0 , 0.5,1.0); }
      else if(v<0.625)  { cmap->SetTableValue(i,1.0, 1.0 , 0.0,1.0); }
      else if(v<0.875)  { cmap->SetTableValue(i,1.0, 0.65, 0.0,1.0); }
      else              { cmap->SetTableValue(i,1.0, 0.41, 0.0,1.0); }
    }

  cmap->SetTableRange(-1.0,1.0);
  source->GetPointData()->GetScalars()->SetLookupTable(cmap);
  cmap->Delete();
  return 1;
}


int vtkpxSurfaceUtil::AddCurvatureLookupTableToSurface(vtkPolyData* source)
{
  vtkLookupTable*cmap =vtkLookupTable::New();
  vtkpxColorMapUtil::SetConstantHueColorMap(cmap,0.5,0.02,0.58,0.4,-0.4,0.4,256);
  cmap->SetTableRange(-1.0,1.0);
  source->GetPointData()->GetScalars()->SetLookupTable(cmap);
  cmap->Delete();
  return 1;
}
//------------------------------------------------------------------------
int vtkpxSurfaceUtil::DefaultObjectMapLookupTableNumberOfColors() 
{ 
  return 120;
}
//------------------------------------------------------------------------
int vtkpxSurfaceUtil::DefaultObjectMapLookupTable(vtkLookupTable* cmap,int num,int blackzero)
{
  static float stcolors [480] = {
    0.000,0.000,0.000,0.200,
    1.000,0.000,0.000,1.000,
    1.000,0.620,0.000,1.000,
    1.000,1.000,0.000,1.000,
    0.000,1.000,0.000,1.000,
    0.000,0.420,1.000,1.000,
    0.823,0.623,0.792,1.000,
    0.173,0.965,1.000,1.000,
    0.525,0.431,0.000,1.000,
    0.992,0.565,0.000,1.000,
    0.400,0.004,0.400,1.000,
    1.000,0.000,1.000,1.000,
    1.000,0.643,1.000,1.000,
    1.000,0.529,0.518,1.000,
    0.514,0.075,0.192,1.000,
    0.129,0.094,0.776,1.000,
    0.639,0.510,1.000,1.000,
    0.518,0.000,1.000,1.000,
    0.016,0.251,0.910,1.000,
    0.451,0.580,0.000,1.000,
    0.514,0.012,0.000,1.000,
    0.000,0.380,0.012,1.000,
    0.000,0.000,0.463,1.000,
    0.922,0.839,0.663,1.000,
    1.000,0.275,0.000,1.000,
    0.510,0.616,0.996,1.000,
    0.643,0.086,0.431,1.000,
    0.149,0.643,0.475,1.000,
    0.639,0.455,0.016,1.000,
    0.600,0.200,0.200,1.000,
    0.922,0.106,0.808,1.000,
    0.600,1.000,0.200,1.000,
    0.200,0.600,0.200,1.000,
    0.329,0.702,0.380,1.000,
    0.996,0.753,0.996,1.000,
    0.373,1.000,0.600,1.000,
    0.247,0.757,0.200,1.000,
    0.600,1.000,1.000,1.000,
    0.000,0.600,0.600,1.000,
    0.600,0.200,1.000,1.000,
    0.753,0.576,0.267,1.000,
    0.996,0.549,0.984,1.000,
    0.600,0.867,0.749,1.000,
    0.329,0.294,0.376,1.000,
    0.569,0.706,0.847,1.000,
    0.718,0.200,0.600,1.000,
    0.965,0.592,0.949,1.000,
    0.580,0.784,0.439,1.000,
    0.733,0.094,0.361,1.000,
    0.200,0.000,0.212,1.000,
    0.612,0.392,0.392,1.000,
    0.522,1.000,0.863,1.000,
    0.835,0.851,0.482,1.000,
    0.451,0.310,0.086,1.000,
    0.243,0.286,0.631,1.000,
    0.984,0.839,0.784,1.000,
    0.239,0.655,0.216,1.000,
    0.200,0.400,0.400,1.000,
    0.800,0.420,0.400,1.000,
    0.800,0.600,0.400,1.000,
    0.400,0.200,0.400,1.000,
    0.529,0.302,0.580,1.000,
    0.988,0.612,0.000,0.988,
    0.988,0.988,0.000,0.988,
    0.000,0.988,0.000,0.988,
    0.129,0.498,0.773,0.988,
    0.604,0.596,0.604,0.988,
    0.173,0.957,0.988,0.988,
    0.639,0.553,0.000,0.988,
    0.988,0.988,0.988,0.988,
    0.396,0.396,0.396,0.988,
    0.988,0.000,0.988,0.988,
    0.988,0.635,0.988,0.988,
    0.000,0.529,0.545,0.988,
    0.988,0.659,0.545,0.988,
    0.129,0.094,0.769,0.988,
    0.631,0.506,0.988,0.988,
    0.514,0.000,0.988,0.988,
    0.576,0.988,0.510,0.988,
    0.447,0.576,0.000,0.988,
    0.510,0.012,0.000,0.988,
    0.000,0.376,0.012,0.988,
    0.000,0.000,0.459,0.988,
    0.914,0.831,0.655,0.988,
    0.988,0.271,0.000,0.988,
    0.247,0.110,0.482,0.988,
    0.635,0.086,0.427,0.988,
    0.149,0.635,0.471,0.988,
    0.631,0.451,0.016,0.988,
    0.592,0.196,0.196,0.988,
    0.592,0.812,0.196,0.988,
    0.592,0.988,0.196,0.988,
    0.196,0.592,0.196,0.988,
    0.325,0.694,0.376,0.988,
    0.208,0.796,0.804,0.988,
    0.369,0.988,0.592,0.988,
    0.243,0.749,0.196,0.988,
    0.592,0.988,0.988,0.988,
    0.000,0.592,0.592,0.988,
    0.592,0.196,0.988,0.988,
    0.592,0.835,0.988,0.988,
    0.196,0.725,0.149,0.988,
    0.592,0.859,0.741,0.988,
    0.325,0.290,0.373,0.988,
    0.831,0.702,0.592,0.988,
    0.710,0.196,0.592,0.988,
    0.773,0.592,0.706,0.988,
    0.643,0.180,0.196,0.988,
    0.114,0.208,0.196,0.988,
    0.196,0.000,0.208,0.988,
    0.196,0.196,0.063,0.988,
    0.518,0.988,0.855,0.988,
    0.592,0.471,0.196,0.988,
    0.447,0.306,0.086,0.988,
    0.239,0.282,0.624,0.988,
    0.345,0.239,0.667,0.988,
    0.235,0.647,0.212,0.988,
    0.196,0.396,0.396,0.988,
    0.792,0.416,0.396,0.988,
    0.792,0.592,0.396,0.988};


  if (num<0)
    num=vtkpxSurfaceUtil::DefaultObjectMapLookupTableNumberOfColors();

  int maxcolor=120;
  blackzero=(blackzero>0);


  cmap->SetNumberOfTableValues(num);
  cmap->SetTableRange(0,num-1);
  float numc=float(cmap->GetNumberOfColors());
  //  fprintf(stderr,"NUMC = %f blackzero=%d\n",numc,blackzero);

  for (int i=blackzero;i<cmap->GetNumberOfColors();i++)
    {
      if (i<maxcolor)
	{
	  cmap->SetTableValue(i,stcolors[i*4],stcolors[i*4+1],stcolors[i*4+2],stcolors[i*4+3]);
	}
      else
	{
	  int sc=int(i/60);
	  int sh=i-sc*60;
	  double vv[4];
	  cmap->GetTableValue(sh,vv);
	  for (int ia=0;ia<sc;ia++)
	    for (int ib=0;ib<=3;ib++)
	      vv[ib]=0.99*vv[ib];
	  cmap->SetTableValue(i,vv);
	}
    }
  if (blackzero)
    cmap->SetTableValue(0,0.0,0.0,0.0,0.0);
  return 1;
}
//------------------------------------------------------------------------
int vtkpxSurfaceUtil::VOIObjectMapLookupTable(vtkLookupTable* cmap,int num)
{
  static float stcolors [60] = {
    0.000 ,0.000 ,0.000,
    1.000 ,0.412 ,1.000,
    0.000 ,0.620 ,0.035,
    0.173 ,0.965 ,1.000,
    0.839 ,0.996 ,0.741,
    0.000 ,1.000 ,0.000,
    0.678 ,0.827 ,0.906,
    1.000 ,0.486 ,0.451,
    0.737 ,0.490 ,0.776,
    0.973 ,1.000 ,0.776,
    0.400 ,0.004 ,0.400,
    0.518 ,0.451 ,0.318,
    0.816 ,0.733 ,1.000,
    0.878 ,0.733 ,0.518,
    0.996 ,0.839 ,0.965,
    0.765 ,0.765 ,0.000,
    0.647 ,0.000 ,1.000,
    0.518 ,0.000 ,0.000,
    0.984 ,0.788 ,0.263,
    0.000 ,0.580 ,0.612};

  if (num<0)
    num=vtkpxSurfaceUtil::DefaultObjectMapLookupTableNumberOfColors();

  int maxcolor=20;
  int blackzero=1;


  cmap->SetNumberOfTableValues(num);
  cmap->SetTableRange(0,num-1);

  //  fprintf(stderr,"Num=%d\n",num);

  float numc=float(cmap->GetNumberOfColors());


  for (int i=blackzero;i<cmap->GetNumberOfColors();i++)
    {
      if (i<maxcolor)
	{
	  cmap->SetTableValue(i,stcolors[i*3],stcolors[i*3+1],stcolors[i*3+2],1.0);
	}
      else
	{
	  int sc=int(i/60);
	  int sh=i-sc*60;
	  double vv[4];
	  cmap->GetTableValue(sh,vv);
	  for (int ia=0;ia<sc;ia++)
	    for (int ib=0;ib<=3;ib++)
	      vv[ib]=0.99*vv[ib];
	  cmap->SetTableValue(i,vv);
	}
    }
  if (blackzero)
    cmap->SetTableValue(0,0.0,0.0,0.0,0.0);
  return 1;
}
//------------------------------------------------------------------------

int vtkpxSurfaceUtil::AddObjectMapLookupTableToSurface(vtkPolyData* source)
{
  if (source==NULL)
    return 0;

  if (source->GetPointData()==NULL)
    return 0;

  if (source->GetPointData()->GetScalars()==NULL)
    return 0;


  double range[2];
  source->GetPointData()->GetScalars()->GetRange(range);
  if (range[1]==range[0])
    range[1]+=1.0;

  int num=int(range[1]-range[0])+1;
  vtkLookupTable*cmap =vtkLookupTable::New();
  vtkpxSurfaceUtil::DefaultObjectMapLookupTable(cmap,num);
  cmap->SetTableRange(range[0],range[1]);

  source->GetPointData()->GetScalars()->SetLookupTable(cmap);
  cmap->Delete();
  return 1;
}
//--------------------------------------------------------------------------------------
int vtkpxSurfaceUtil::AddObjectLabelsToSurface(vtkPolyData* source,int offset)
{
  if (source==NULL)
    return 0;

  if (source->GetPointData()==NULL)
    return 0;

  int useold=0;

  int out_offset=1;

  vtkDataArray* oldScalars=source->GetPointData()->GetScalars();

  if (offset >=0 ) 
    {
      if (oldScalars!=NULL)
	{
	  double range[2];
	  oldScalars->GetRange(range);
	  out_offset+=(int)range[1];
	  useold=1;
	}
    }
  else 
    {
      offset=-offset;
      useold=0;
    }

  vtkDataArray* ptScalars=vtkDataArray::CreateDataArray(VTK_SHORT);
  ptScalars->SetNumberOfComponents(1);
  ptScalars->SetNumberOfTuples(source->GetPoints()->GetNumberOfPoints());
  for (int i=0;i<ptScalars->GetNumberOfTuples();i++)
    {
      int old=0;
      if (useold==1)
	old=(int)oldScalars->GetComponent(i,0);
      ptScalars->SetComponent(i,0,offset+old);
    }
  source->GetPointData()->SetScalars(ptScalars);
  ptScalars->Delete();
  return out_offset;

}

//--------------------------------------------------------------------------------------
// Neighbours Stuff -- needed for curvature tracking etc 
//--------------------------------------------------------------------------------------
PXIntegerList* vtkpxSurfaceUtil::GenerateNeighbors(vtkPolyData* poly,int opensurface)
{
  static int debugmode=0;
  vtkPoints* points=poly->GetPoints();
  vtkCellArray* cells=poly->GetPolys();

  int numpoints=points->GetNumberOfPoints();
  int numtriangles=cells->GetNumberOfCells();

  PXIntegerList* neighbours=new PXIntegerList[numpoints];

  if (debugmode)
    fprintf(stderr,"Generating Neighbours (pt=%d,tr=%d):: ",numpoints,numtriangles);
  
  opensurface=Irange(opensurface,0,1);

  double bounds[6];
  poly->GetBounds(bounds);

  float bz=bounds[4]+0.002*(bounds[5]-bounds[4]);
  float tz=bounds[5]-0.002*(bounds[5]-bounds[4]);
  
  cells->InitTraversal();
  for (int tr=0;tr<numtriangles;tr++)
    {
      vtkIdType *cell,cellp;
      cells->GetNextCell(cellp,cell);
      for (int k=0;k<cellp;k++)
	{
	  int pt=cell[k];
	  double x[3];
	  points->GetPoint(pt,x);
	  for (int j=0;j<cellp;j++)
	    {
	      int pt2=cell[j];
	      if (pt2!=pt)
		{
		  double x2[3];
		  points->GetPoint(pt2,x2);
		  if (!opensurface)
		    {
		      neighbours[pt].AddNonDuplicate(pt2);
		    }
		  else
		    {
		      if (x[2]>=bz && x[2]<tz)
			{
			  neighbours[pt].AddNonDuplicate(pt2);
			}
		      else if (x[2]<bz && x2[2]<bz)
			{
			  neighbours[pt].AddNonDuplicate(pt2);
			}
		      else if (x[2]>tz && x2[2]>tz)
			{
			  neighbours[pt].AddNonDuplicate(pt2);
			}
		    }
		}
	    }
	}
    }

  return neighbours;
}

//--------------------------------------------------------------------------------------
int vtkpxSurfaceUtil::CollectNeighbours(vtkPolyData* polydata,int point,int distance,PXIntegerList* list,
					PXIntegerList* neighbours)
{

  list->DeleteAll();
  distance=Irange(distance,0,10);

  int numpoints=polydata->GetNumberOfPoints();

  point=Irange(point,0,numpoints-1);
  
  neighbours[point].Rewind();
  list->Add(point);
  
  if (distance==0)
    return list->getnumnodes();

  for (int i=0;i<neighbours[point].getnumnodes();i++)
      {
	list->Add(neighbours[point].getcurrent());
	neighbours[point].Next();
      }
  
  
  if (distance==1)
      return list->getnumnodes();
      
  int begin=1;
  for (int sc=2;sc<=distance;sc++)
      {
	list->Rewind();
	int end  =list->getnumnodes();
	// Skip over previously examined points 
	int k;
	for (k=0;k<begin;k++)
	  list->Next();
	for (k=begin;k<end;k++)
	  {
	    int pt=list->getcurrent();
	    neighbours[pt].Rewind();
	    for (int i=0;i<neighbours[pt].getnumnodes();i++)
	      {
		list->AddNonDuplicate(neighbours[pt].getcurrent(),-1);
		neighbours[pt].Next();
	      }
	  }
	begin=end;
      }
  return list->getnumnodes();
}
//--------------------------------------------------------------------------------------

int vtkpxSurfaceUtil::NonShrinkSmooth(vtkPolyData* poly,int opensurface,
				      float alpha,float beta,int iterations,int movez)
{
  PXIntegerList* listn=vtkpxSurfaceUtil::GenerateNeighbors(poly,opensurface);
  int ok=vtkpxSurfaceUtil::NonShrinkSmooth(poly,listn,alpha,beta,iterations,movez);
  delete [] listn;
  return ok;

}
int vtkpxSurfaceUtil::NonShrinkSmooth(vtkPolyData* poly,PXIntegerList* neighbours,
				      float alpha,float beta,int iterations,int movez)
{
  int debugmode=0;
  movez=Irange(movez,0,2);
  
  vtkPoints* points=poly->GetPoints();
  vtkPoints* temp=vtkPoints::New();

  
  double bounds[6]; poly->GetBounds(bounds);
  float minz=bounds[4];
  float maxz=bounds[5];
	      
  if (debugmode)
    fprintf(stderr,"Smoothing , iterations (movez=%d,alpha=%5.3f,beta=%5.3f,iter=%d): ",
	    movez,alpha,beta,iterations);

  for (int iter=1;iter<=iterations;iter++)
      {
	if (debugmode)
	  {
	    if (20*int(iter/20)==iter)
	      fprintf(stderr,"%d ",iter);
	  }
	for(int pass=0;pass<=1;pass++)
	  {
	    temp->DeepCopy(points);
	    for(int i=0;i<points->GetNumberOfPoints();i++)
	      {
		double oldx[3],newx[3];
		temp->GetPoint(i,oldx);
		
		for (int ia=0;ia<=2;ia++)
		  newx[ia]=0.0;
		float total=0.0;
		    
		for(int j=0;j<(neighbours[i].getnumnodes());j++)
		  {
		    int p=neighbours[i].getcurrent();
		    for (int ia=0;ia<=2;ia++)
		      newx[ia]+=temp->GetPoint(p)[ia];
		    total+=1.0;
		    neighbours[i].Next();
		  }
		if (total>0.0)
		  {
		    for (int ia=0;ia<=2;ia++)
		      newx[ia]=newx[ia]/total;
		    float factor;
		    if(pass==0)
		      factor=alpha;
		    else
		      factor=beta;
		    
		    double newp[3];
		    points->GetPoint(i,newp);
		    for (int ib=0;ib<=1;ib++)
		      newp[ib]=(1.0-factor)*oldx[ib]+factor*newx[ib];


		    switch(movez)
		      {
		      case 1:
			if (newp[2]>minz+0.01 && newp[2]<maxz-0.01)
			  newp[2]=(1.0-factor)*oldx[2]+factor*newx[2];
			break;
		      case 2:
			newp[2]=(1.0-factor)*oldx[2]+factor*newx[2];
			break;
		      }
		    points->SetPoint(i,newp);
		  }
	      }
	  }
      }
  temp->Delete();

  if (debugmode)
    fprintf(stderr,"\n");
  return 1;
}

//------------------------------------------------------------------------------
int vtkpxSurfaceUtil::CalculateCurvatures(vtkPolyData* polydata,int scale,PXIntegerList* neighbours)
{
  return vtkpxMath::CalculateCurvatures(polydata,scale,neighbours);
}

int vtkpxSurfaceUtil::CalculateCurvatures(vtkPolyData* polydata,int scale,int opensurface,int c_mode,float upper)
{
  PXIntegerList* listn=vtkpxSurfaceUtil::GenerateNeighbors(polydata,opensurface);
  int ok=vtkpxSurfaceUtil::CalculateCurvatures(polydata,scale,listn);
  delete [] listn;

  vtkDataArray* dat=polydata->GetPointData()->GetScalars();
  int nt=dat->GetNumberOfTuples();
  float k[2];

  vtkFloatArray* out=vtkFloatArray::New();
  out->SetNumberOfTuples(nt);

  if (c_mode==2)
    upper*=upper;

  float max=Frange(upper,0.01,10.0);
  float min=-max;

  float k0=2.0/M_PI;

  for (int i=0;i<nt;i++)
    {

      for (int ia=0;ia<=1;ia++)
	k[ia]=dat->GetComponent(i,ia);

      float meancurv =0.5*(k[0]+k[1]);
      float gausscurv=k[0]*k[1];

      float shapeindex=0.0;
      if(k[0]==k[1])
	shapeindex=1;
      else 
	shapeindex=k0*atan2( k[0]+k[1] , k[0]-k[1] );
		  
      switch (c_mode)
	{
	case 0:
	  out->SetComponent(i,0,shapeindex);
	  break;
	case 1:
	  out->SetComponent(i,0,Frange(meancurv,min,max));
	  break;
	case 2:
	  out->SetComponent(i,0,Frange(gausscurv,min,max));
	  break;
	case 4:
	  out->SetComponent(i,0,Frange(k[1],min,max));
	  break;
	default:
	  out->SetComponent(i,0,Frange(k[0],min,max));
	  break;
	}
    }
  polydata->GetPointData()->SetScalars(out);

  if (c_mode==0)
    vtkpxSurfaceUtil::AddShapeIndexLookupTableToSurface(polydata);
  else
    vtkpxSurfaceUtil::AddCurvatureLookupTableToSurface(polydata);
  
      
  out->Delete();

  return ok;
}


//------------------------------------------------------------------------------
void vtkpxSurfaceUtil::ZeroAllDisplacements(vtkPolyData* poly,vtkFloatArray* displ)
{
  int num=poly->GetNumberOfPoints();
  displ->SetNumberOfComponents(3);
  displ->SetNumberOfTuples(num);
  displ->FillComponent(0,0.0);
  displ->FillComponent(1,0.0);
  displ->FillComponent(2,0.0);

}
//------------------------------------------------------------------------------
int vtkpxSurfaceUtil::AddToDisplacements(vtkPolyData* orig,vtkPolyData* targ,vtkFloatArray* displ)
{
  int np=orig->GetNumberOfPoints();
  if (np!=targ->GetNumberOfPoints() || np!=displ->GetNumberOfTuples())
    return 0;


  vtkPoints* pt1=orig->GetPoints();
  vtkPoints* pt2=targ->GetPoints();

  double x1[3],x2[3];
  for (int ia=0;ia<np;ia++)
    {
      pt1->GetPoint(ia,x1);
      pt2->GetPoint(ia,x2);
      for (int ib=0;ib<=2;ib++)
	displ->SetComponent(ia,ib,displ->GetComponent(ia,ib)+x2[ib]-x1[ib]);
    }

  return 1;
}
//------------------------------------------------------------------------------
int vtkpxSurfaceUtil::ShiftPointsByDisplacements(vtkPolyData* poly,vtkFloatArray* displ,float factor)
{
  vtkPoints* points=poly->GetPoints();
  int np=displ->GetNumberOfTuples();
  if (points->GetNumberOfPoints()!=np)
    {
      fprintf(stderr,"Bad Number of Displacements, cannot shift\n");
      return 0;
    }


  for (int i=0;i<np;i++)
    {
      double x[3],tx[3];
      points->GetPoint(i,x);
      for (int ia=0;ia<3;ia++)
	tx[ia]=x[ia]+displ->GetComponent(i,ia)*factor;
      points->SetPoint(i,tx);
    }
  return 1;
}
//------------------------------------------------------------------------------
void vtkpxSurfaceUtil::GetColor(int i,double c[3])
{
  i=Irange(i,0,12);
  switch(i)
    {
    case 0:
      c[0]=0.89;c[1]=0.09;c[2]=0.04;
      break;
    case 1:
      c[0]=0.925;c[1]=0.957;c[2]=0.329;
      break;
    case 2:
      c[0]=0.188;c[1]=0.886;c[2]=0.259;
      break;
    case 3:
      c[0]=0.118;c[1]=0.714;c[2]=0.949;
      break;
    case 4:
      c[0]=0.949;c[1]=0.275;c[2]=0.871;
      break;
    case 5:
      c[0]=0.949;c[1]=0.275;c[2]=0.110;
      break;
    case 6:
      c[0]=0.5;c[1]=0.9;c[2]=0.75;
      break;
    case 7:
      c[0]=1.0; c[1]=1.0; c[2]=1.0;
    default:
      c[0]=0.1*i;c[1]=0.05*i;c[2]=0.1*i;
      break;
    }
}


const char* vtkpxSurfaceUtil::GetColor(int i) {
  double c[3];
  vtkpxSurfaceUtil::GetColor(i,c);
  char line[100];
  sprintf(line,"%.3f %.3f %.3f",c[0],c[1],c[2]);
  buffer=line;
  return buffer.c_str();
}

//------------------------------------------------------------------------------
int vtkpxSurfaceUtil::CleanPoints(vtkPolyData* input,vtkPolyData* other,vtkImageData* mask,int usemask)
{
  
  if (input==NULL || other==NULL)
    {
      fprintf(stderr,"Bad Inputs to CleanPoints\n");
      return 0;
    }

  if (mask==NULL)
    usemask=0;

  double sp[3],ori[3];
  if (usemask)
    {
      mask->GetSpacing(sp);
      mask->GetOrigin(ori);
    }


  int npoints=input->GetNumberOfPoints();
  vtkDataArray* inp=input->GetPointData()->GetScalars();

  int allocate=0;
  if (inp==NULL)
    allocate=1;
  else if (inp->GetNumberOfTuples()!=npoints)
    allocate=1;

  if (allocate || usemask)
    {
      vtkFloatArray* flarr=vtkFloatArray::New();
      flarr->SetNumberOfTuples(npoints);
      flarr->FillComponent(0,1000.0);
      input->GetPointData()->SetScalars(flarr);
      fprintf(stderr,"Allocating\n");
      flarr->Delete();
    }

  inp=input->GetPointData()->GetScalars();


  double x[3],tx[3];
  vtkPoints* p_inp=input->GetPoints();
  vtkPoints* p_ref=other->GetPoints();

  int smaller=0;

  for (int i=0;i<npoints;i++)
    {
      p_inp->GetPoint(i,x);
      int id=other->FindPoint(x);
      p_ref->GetPoint(id,tx);
      float d=sqrt(vtkMath::Distance2BetweenPoints(x,tx));
      float scale=1.0;

      if (usemask)
	{
	  int x1=int(x[0]/sp[0]-ori[0]+0.5);
	  int y1=int(x[1]/sp[1]-ori[1]+0.5);
	  int z1=int(x[2]/sp[2]-ori[2]+0.5);
	  double m=mask->GetScalarComponentAsDouble(x1,y1,z1,0);
	  if (m<1.0)
	    scale=0.0;
	}
      else
	{
	  float f=inp->GetComponent(i,0);
	  if (f<d)
	    {
	      ++smaller;
	      d=f;
	    }
	}
      inp->SetComponent(i,0,d*scale);
    }

  fprintf(stderr,"Done smaller=%d  \n",smaller);
	  

  return 1;
}

int vtkpxSurfaceUtil::ExportQHull(vtkPolyData* input,const char* fname)
{
  vtkPoints* pts=input->GetPoints();
  int numpoints=pts->GetNumberOfPoints();

  FILE* fout=fopen(fname,"w");
  if (fout)
      {
	fprintf(fout,"3\n");
	fprintf(fout,"%d\n",numpoints);
	for (int j=0;j<numpoints;j++)
	  {
	    double x[3]; pts->GetPoint(j,x);
	    fprintf(fout,"%6.3f %6.3f %6.3f\n",x[0],x[1],x[2]);
	  }
	fclose(fout);
      }
  fprintf(stderr,"Done save qhull in %s\n",fname);
  return 1;
}

vtkPolyData* vtkpxSurfaceUtil::ReadQHull(const char* fname)
{
  FILE* fin=fopen(fname,"r");
  if (fin)
    {
      int good=0,bad=0;
      char command[200];
      vtkPoints*     pts  = vtkPoints::New();
      vtkCellArray* cells = vtkCellArray::New();
      fgets(command,200,fin);
      int np,temp,nt;
      fscanf(fin,"%d %d %d\n",&np,&nt,&temp);
      fprintf(stderr,"Reading %s points=%d, tetra=%d\n",fname,np,nt);
      pts->SetNumberOfPoints(np);
      cells->Allocate(nt*4,10);
      
      float x1,y1,z1,t1;
      for (int j=0;j<np;j++)
	{
	  fscanf(fin,"%f %f %f %f\n",&x1,&y1,&z1,&t1);
	  pts->SetPoint(j,x1,y1,z1);
	}
      int points[4];
      for (int i=0;i<nt;i++)
	{
	  int npt;
	  fgets(command,200,fin);
	  sscanf(command,"%d %d %d %d %d",&npt,&points[0],&points[1],&points[2],&points[3]);
	  //	    fprintf(stderr,"%d : (%s)\n",npt,command);
	  if (npt==4)
	    {
	      vtkIdType p1[3],p2[3],p3[3],p4[3];
	      p1[0]=points[0]; 		p1[1]=points[1]; 		p1[2]=points[2];
	      p2[0]=points[3]; 		p2[1]=points[1]; 		p2[2]=points[0];
	      p3[0]=points[1];		p3[1]=points[2];		p3[2]=points[3];
	      p4[0]=points[0]; 		p4[1]=points[2]; 		p4[2]=points[3];
	      
	      cells->InsertNextCell(3,p1);
	      cells->InsertNextCell(3,p2);
	      cells->InsertNextCell(3,p3);
	      cells->InsertNextCell(3,p4);
	      ++good;
	    }
	  else
	    ++bad;
	}
      
      fclose(fin);
      fprintf(stderr,"Good=%d bad=%d\n",good,bad);
      vtkPolyData* out=vtkPolyData::New();
      out->SetPoints(pts);
      out->SetPolys(cells);
      pts->Delete();
      cells->Delete();
      return out;
    }
  return NULL;
}

// --------------------------------------------------------------------------------------
int vtkpxSurfaceUtil::AddLabels(vtkPolyData* inout,vtkPolyData* other)
{
  if (inout==NULL || other==NULL)
    {
      fprintf(stderr,"Bad Inputs to Add Labels\n");
      return 0;
    }
  
  int npoints=inout->GetNumberOfPoints();
  vtkFloatArray* flarr=vtkFloatArray::New();
  flarr->SetNumberOfTuples(npoints);
  flarr->FillComponent(0,1000.0);
  inout->GetPointData()->SetScalars(flarr);
  flarr->Delete();
  
  vtkDataArray* inp=inout->GetPointData()->GetScalars();
  vtkDataArray* ref=other->GetPointData()->GetScalars();
  
  double x[3];
  vtkPoints* p_inp=inout->GetPoints();
  
  for (int i=0;i<npoints;i++)
    {
      p_inp->GetPoint(i,x);
      int id=other->FindPoint(x);
      float c=ref->GetComponent(id,0);
      inp->SetComponent(i,0,c);
    }
  
  return 1;
}
// --------------------------------------------------------------------------------------
int vtkpxSurfaceUtil::SaveAbaqus(vtkPolyData* qhullsurface,const char* abname)
{
  vtkPoints* pts=qhullsurface->GetPoints();
  vtkCellArray* cells=qhullsurface->GetPolys();


  FILE* fout=fopen(abname,"w");

  int numnodes=pts->GetNumberOfPoints();
  int numelements=cells->GetNumberOfCells()/4;

  fprintf(fout,"*HEADING\nPelvic Model\n");
  fprintf(fout,"** numnodes=%d, numelements=%d\n",numnodes,numelements);
  fprintf(fout,"** ----------------------------------------------------\n");
  fprintf(fout,"** Shrink .dat file\n*PREPRINT,ECHO=NO,MODEL=NO,HISTORY=NO\n**\n");


  fprintf(fout,"*MATERIAL,NAME=SOFT\n*ELASTIC\n");
  fprintf(fout,"%8.2f,%8.2f\n",1.0,0.4);
  fprintf(fout,"*MATERIAL,NAME=BONE\n*ELASTIC\n");
  fprintf(fout,"%8.2f,%8.2f\n",100000.0,0.3);
  fprintf(fout,"*MATERIAL,NAME=BLADDER\n*ELASTIC\n");
  fprintf(fout,"%8.2f,%8.2f\n",3.0,0.4);
  fprintf(fout,"*MATERIAL,NAME=PROSTATE\n*ELASTIC\n");
  fprintf(fout,"%8.2f,%8.2f\n",3.0,0.4);
  fprintf(fout,"*MATERIAL,NAME=RECTUM\n*ELASTIC\n");
  fprintf(fout,"%8.2f,%8.2f\n",3.0,0.4);

  vtkDataArray* lab=qhullsurface->GetPointData()->GetScalars();

  double r2[2]; lab->GetRange(r2);
  int r[2]; r[0]=int(r2[0]); r[1]=(int)r2[1];
  double x[3];
  
  fprintf(fout,"** ---------- Nodes  --------------------------------------\n");

  for (int pass=r[0];pass<=r[1];pass++)
      {
	fprintf(fout,"*NODE,NSET=N_%d\n",pass);
	for (int index=0;index<numnodes;index++)
	  {
	    int p=(int)lab->GetComponent(index,0);
	    if (p==pass)
	      {
		pts->GetPoint(index,x);
		fprintf(fout," %5d , %7.3f , %7.3f , %7.3f \n",
			index+1000,
			x[0],x[1],x[2]);
	      }
	  }
      }
  
  fprintf(fout,"**\n");
  fprintf(fout,"*NSET, NSET=ALLNODES\n");
  for (int pass=r[0];pass<=r[1];pass++)
      {
	fprintf(fout,"N_%d",pass);
	if (pass!=r[1])
	  fprintf(fout,",");
      }
  fprintf(fout,"\n*NSET, NSET=N_DUMMY\n1000\n");
  
  
  fprintf(fout,"** ---------- Elements --------------------------------------\n");  
  char elementname[20];
  sprintf(elementname,"C3D4");

  fprintf(stderr,"Using element type=%s\n",elementname);



  int elindex=1000;

  for (int pass=r[0];pass<=r[1];pass++)
    {
      cells->InitTraversal();
      fprintf(fout,"*ELEMENT, TYPE=%s, ELSET=E_%d\n",elementname,pass);
      for (int element=0;element<numelements;element++)
	{
	  vtkIdType p[4],q[4],npts;
	  vtkIdType *p1,*p2,*p3,*p4;
	  cells->GetNextCell(npts,p1);
	  cells->GetNextCell(npts,p2);
	  cells->GetNextCell(npts,p3);
	  cells->GetNextCell(npts,p4);
	  
	  for (int ia=0;ia<=2;ia++)
	    p[ia]=p1[ia];
	  p[3]=p2[0];
	  
	  int mode=1;
	  for (int ia=0;ia<=3;ia++)
	    q[ia]=(int)lab->GetComponent(p[ia],0);
	  if (q[0]==q[1] && q[0]==q[2] && q[0]==q[3])
	    mode=q[0];
	  else
	    mode=1;
	  
	  fprintf(fout,"%d, %d,%d,%d,%d\n",elindex,p[0]+1000,p[1]+1000,p[2]+1000,p[3]+1000);
	  ++elindex;
	}
    }
  fprintf(fout,"** \n");
  fprintf(fout,"*ELSET, ELSET=E_DUMMY\n1000\n**\n");
  fprintf(fout,"*ELSET, ELSET=PELVIS\n");  
  fprintf(fout,"E_1,E_2,E_3,E_4,E_5\n");

  fprintf(fout,"** ---------- Solid  --------------------------------------\n");
  fprintf(fout,"*SOLID SECTION, ELSET=E_1,MATERIAL=SOFT\n**\n");
  fprintf(fout,"*SOLID SECTION, ELSET=E_2,MATERIAL=BONE\n**\n");
  fprintf(fout,"*SOLID SECTION, ELSET=E_3,MATERIAL=BLADDER\n**\n");
  fprintf(fout,"*SOLID SECTION, ELSET=E_4,MATERIAL=PROSTATE\n**\n");
  fprintf(fout,"*SOLID SECTION, ELSET=E_5,MATERIAL=RECTUM\n**\n");

  fprintf(fout,"** Dummy Step\n");
  fprintf(fout,"*STEP");
  //fprintf(fout,",NLGEOM");
  fprintf(fout,",INC=50\n*STATIC\n*BOUNDARY\nALLNODES,1,3,0.0\n");
  fprintf(fout,"*EL PRINT,POSITION=CENTROIDAL,ELSET=E_DUMMY\nEP1\n");
  fprintf(fout,"*NODE PRINT,NSET=N_DUMMY\nU1\n");
  fprintf(fout,"*EL FILE,POSITION=CENTROIDAL,ELSET=PELVIS\nE\n");
  fprintf(fout,"*EL FILE,ELSET=PELVIS\nEVOL\n*NODE FILE,NSET=ALLNODES\nU\n");
  //  if (resout>0)
  fprintf(fout,"*RESTART,WRITE\n");
  fprintf(fout,"*END STEP\n");

  fclose(fout);
  return 1;
}

