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
  Module:    $RCSfile: vtkpxSplineStackSource.cpp,v $
  Language:  C++
  Date:      $Date: 2004/02/03 15:58:09 $
  Version:   $Revision: 1.3 $


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
#include "vtkCellArray.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkpxSplineStackSource.h"
#include "vtkPolyDataCollection.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkAppendPolyData.h"
#include "vtkPolyDataNormals.h"
#include "vtkCollection.h"
#include "vtkImageData.h"
#include "pxcontour.h"
#include "pxchamferdistancemap.h"
#include "vtkImageThreshold.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkpxBaseCurve.h"
#include "vtkImageSeedConnectivity.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkpxImageExtract.h"
#include "vtkImageExtractComponents.h"
#include "vtkContourFilter.h"
#include "vtkPoints.h"
#include "vtkpxUtil.h"
#include "pxfpbsplinestack.h"
//------------------------------------------------------------------------------
vtkpxSplineStackSource* vtkpxSplineStackSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxSplineStackSource");
  if(ret)
    {
    return (vtkpxSplineStackSource*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxSplineStackSource;
}
//------------------------------------------------------------------------------
vtkpxSplineStackSource::vtkpxSplineStackSource()
{
  this->SplineStack=new PXFPBSplineStack();
  this->SplineStack->setellipticalcylinder(6,2,10.0,10.0,0.0,0.0,1.0,0.0);
  this->sliceSurfaceData=NULL;
  this->wholeSurfaceData=NULL;

  this->ResolutionU=2.0;
  this->ResolutionV=2.0;
  this->WholeSurface=0;
  this->PolarMode=0;

  this->DrawMode=0;
  this->PolarMiddle=0.0;
  this->RedrawSurface=1;
  this->ZDisplayOffset=0.001;
}
//------------------------------------------------------------------------------
vtkpxSplineStackSource::~vtkpxSplineStackSource()
{
  if (this->SplineStack)
      delete this->SplineStack;
  if (this->sliceSurfaceData)
    this->sliceSurfaceData->Delete();
  if (this->wholeSurfaceData)
    this->wholeSurfaceData->Delete();

}
//------------------------------------------------------------------------------
int vtkpxSplineStackSource::Save(const char* fname)
{
  if (this->SplineStack==NULL)
      return 0;
  return this->SplineStack->Save(fname);
}

int vtkpxSplineStackSource::Save(FILE* fout)
{
  if (this->SplineStack==NULL)
      return 0;
  return this->SplineStack->Save(fout);
}
//------------------------------------------------------------------------------
int vtkpxSplineStackSource::Load(const char* fname)
{
  if (this->SplineStack==NULL)
      this->SplineStack=new PXFPBSplineStack();
  int ok=this->SplineStack->Load(fname);
  if (ok)
    {
      this->SplineStack->enableapex(0);
      this->Modified();
      this->RedrawSurface=1;
      this->ResolutionV=this->GetZSpacing();
    }
  return ok;
}

int vtkpxSplineStackSource::Load(gzFile fin)
{
  if (this->SplineStack==NULL)
      this->SplineStack=new PXFPBSplineStack();
  int ok=this->SplineStack->Load(fin);
  if (ok)
    {
      this->SplineStack->enableapex(0);
      this->Modified();
      this->RedrawSurface=1;
      this->ResolutionV=this->GetZSpacing();
    }
  return ok;
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::SetEllipticalCylinder(int np,int numslices,float radiusx,float radiusy,
						   float x,float y,float zspacing,float bottomz)
{
  if (this->SplineStack==NULL)
      this->SplineStack=new PXFPBSplineStack();


  this->SplineStack->setellipticalcylinder(np,numslices,radiusx,radiusy,x,y,zspacing,bottomz);
  this->Modified();
  this->RedrawSurface=1;
}
//------------------------------------------------------------------------------
int vtkpxSplineStackSource::SetLizzardCylinder(int np,int numslices,int bottomtrim,
					       int lizzardslices,int blendslices,int thinslices,
					       float minradius,float maxradius,float yscale,
						float x,float y,float 
					       zspacing,float bottomz)
{
  if (numslices< (lizzardslices+bottomtrim))
    {
      vtkErrorMacro(<<"Bad Lizzard Cylinder slices spec total= " << numslices << " lizzard " <<lizzardslices << "trim= " << bottomtrim << "\n");
      return 0;
    }

  if (lizzardslices< 2*(blendslices+thinslices))
    {
      vtkErrorMacro(<<"Bad Lizzard Cylinder slices spec total= " << lizzardslices << " blend=2x " << blendslices << " min=2x " << thinslices << "\n");
      return 0;
    }
  

  if (this->SplineStack==NULL)
    this->SplineStack=new PXFPBSplineStack();

 this->SplineStack->setellipticalcylinder(np,numslices,minradius,minradius*yscale,x,y,zspacing,bottomz);
 this->SetTrimBottom(bottomtrim);
 this->SetTrimTop(bottomtrim+lizzardslices-1);



 int lowslice=bottomtrim+thinslices;
 int highslice=bottomtrim+lizzardslices-thinslices;

 int lowblend=lowslice+blendslices;
 int highblend=highslice-blendslices;

 fprintf(stderr,"Modifying %d:%d  no-blend %d:%d \n",lowslice,highslice,lowblend,highblend);

 for (int slice=lowslice;slice< highslice;slice++)
   {
     float ratio=1.0;
     if (slice< lowblend)
       {
	 ratio=float(slice-lowslice)/float(blendslices);
       }
     else if (slice>=highblend)
       {
	 ratio=float(highslice-slice)/float(blendslices);
       }

     float r=(-2.0*pow(ratio,float(3.0))+3.0*pow(ratio,float(2.0)))*(maxradius-minradius)+minradius;


     fprintf(stderr,"slice=%d r=%.2f (ratio=%.2f lowblend=%d, highblend=%d minr=%.2f,maxr=%.2f)\n",slice,r,ratio,lowblend,highblend,minradius,maxradius);
     float z=this->GetSpline(slice)->getzlevel();
     this->GetSpline(slice)->setellipse(np,r,r*yscale,x,y,2);
     this->GetSpline(slice)->setzlevel(z);
   }
 return 1;
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::Copy(vtkpxSplineStackSource* other,int ignore)
{
  this->SplineStack->copypoints(other->GetSplineStack(),ignore);
  this->SplineStack->enableapex(0);
  this->Modified();
  this->RedrawSurface=1;
}
//------------------------------------------------------------------------------
int  vtkpxSplineStackSource::GetTrimBottom()
{
  return this->SplineStack->gettrimbottom();
}
//------------------------------------------------------------------------------
int  vtkpxSplineStackSource::GetTrimTop()
{
  return this->SplineStack->gettrimtop();
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::SetTrimBottom(int tb)
{
  tb=Irange(tb,0,this->SplineStack->gettrimtop());
  if (tb==this->SplineStack->gettrimbottom())
    return;

  if (this->WholeSurface==1 || this->sliceSurfaceData==NULL)
    {
      this->SplineStack->settrimbottom(tb);
      this->Modified();
      this->RedrawSurface=1;
    }
  else
    {
      if (tb>this->SplineStack->gettrimbottom())
	{
	  int tb2=this->SplineStack->gettrimbottom();
	  this->SplineStack->settrimbottom(tb);
	  this->Modified();
	  this->RemoveSections(tb2,tb+1);
	  this->AddSections(tb,tb+1);
	}
      else
	{
	  int tb2=this->SplineStack->gettrimbottom();
	  this->SplineStack->settrimbottom(tb);
	  this->Modified();

	  this->RemoveSections(tb2,tb2+1);
	  this->AddSections(tb,tb2+1);
	}
      if (this->PolarMode)
	this->AddSections(this->SplineStack->gettrimtop(),this->SplineStack->gettrimtop()+1);
      this->Modified();
    }
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::SetTrimTop(int tt)
{
  tt=Irange(tt,this->SplineStack->gettrimbottom(),this->SplineStack->getnumsplines()-1);
  if (tt==this->SplineStack->gettrimtop())
    return;

  if (this->WholeSurface==1 || this->sliceSurfaceData==NULL)
    {
      this->SplineStack->settrimtop(tt);
      this->Modified();
      this->RedrawSurface=1;
    }
  else
    {
      if (tt<this->SplineStack->gettrimtop())
	{
	  int tt2=this->SplineStack->gettrimtop();
	  this->SplineStack->settrimtop(tt);
	  this->Modified();
	  this->RemoveSections(tt-1,tt2);
	  this->AddSections(tt-1,tt);
	}
      else
	{
	  int tt2=this->SplineStack->gettrimtop();
	  this->SplineStack->settrimtop(tt);
	  this->Modified();
	  this->RemoveSections(tt2-1,tt2);
	  this->AddSections(tt2-1,tt);
	}
      
      if (this->PolarMode)
	this->AddSections(this->SplineStack->gettrimtop(),this->SplineStack->gettrimtop()+1);
      this->Modified();
    }
}
//------------------------------------------------------------------------------
int vtkpxSplineStackSource::GetNumSplines()
{
  return this->SplineStack->getnumsplines();
}
//------------------------------------------------------------------------------
PXFPBSpline* vtkpxSplineStackSource::GetSpline(int i)
{
  return this->SplineStack->getspline(i);
}
//------------------------------------------------------------------------------
PXFPBSplineStack* vtkpxSplineStackSource::GetSplineStack()
{
  return this->SplineStack;
}
//------------------------------------------------------------------------------
float vtkpxSplineStackSource::GetZSpacing()
{
  float a=fabs(this->SplineStack->getspline(this->SplineStack->gettrimbottom()+1)->getzlevel()-
	       this->SplineStack->getspline(this->SplineStack->gettrimbottom())->getzlevel());
  if (a<0.01)
    a=1.0;
  return a;
}

//------------------------------------------------------------------------------
int vtkpxSplineStackSource::FitToContourStack(PXContourStack* stack,float smooth,float csmooth,
					      int npoints,int closed)
{
  fprintf(stderr,"Fitting to Contour Stack\n\n");
  int ok=this->SplineStack->fittocontourstack(stack,smooth,csmooth,npoints,closed);
  fprintf(stderr,"Done Fitting to Contour Stack\n\n");
  this->Modified();
  this->RedrawSurface=1;
  return ok;
}
//------------------------------------------------------------------------------
PXContourStack* vtkpxSplineStackSource::ExportToContourStack(real dist)
{
  return this->SplineStack->exporttocontourstack(dist);
}
//------------------------------------------------------------------------------
PXContourStack* vtkpxSplineStackSource::ExportToContourStack(real dist,real zdist)
{
  return this->SplineStack->exporttocontourstack(dist,zdist);
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::SetApex(float x,float y,float z)
{
  this->SplineStack->setapex(x,y,z);
  if (this->PolarMode)
    return;

  if ( this->WholeSurface==1)
    {
      this->RedrawSurface=1;
      this->Modified();
    }
  else
    {
      AddApex();
    }
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::GetApex(float &x,float &y,float &z)
{
  PX3Vector* vect=this->SplineStack->getapex();
  x=vect->x[0];
  y=vect->x[1];
  z=vect->x[2];
}
//------------------------------------------------------------------------------   
void vtkpxSplineStackSource::EnableApex(int enable)
{
  if (this->SplineStack->apexenabled()!=enable)
    {
      this->SplineStack->enableapex(enable);
      if (this->PolarMode)
	return;

      if ( this->WholeSurface==1)
	{
	  this->RedrawSurface=1;
	  this->Modified();
	}
      else
	{
	  AddApex();
	}
    }
}
//------------------------------------------------------------------------------
int  vtkpxSplineStackSource::ApexEnabled()
{
  return this->SplineStack->apexenabled();
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::AutoSetApex(int active)
{
  this->SplineStack->autosetapex(active);
  
  if (this->PolarMode)
    return;
  
  if ( this->WholeSurface==1)
    {
      this->RedrawSurface=1;
      this->Modified();
    }
  else
    {
      AddApex();
    }
}
//------------------------------------------------------------------------------
int vtkpxSplineStackSource::Resample(float ds_step,int npoints)
{
  for (int i=this->GetTrimBottom(); i<=this->GetTrimTop();i++)
      this->SplineStack->getspline(i)->resample(ds_step,npoints);
  this->Modified();
  this->RedrawSurface=1;
  return 1;
}

//------------------------------------------------------------------------------
int vtkpxSplineStackSource::ChangeSpacing(float smooth,float npoints,
					  float step,float csmooth,int iterations)
{
  for (int i=this->GetTrimBottom(); i<=this->GetTrimTop();i++)
    {
      int np=-1;
      if (npoints!=-1.0)
	np=int(0.5+float(npoints*this->SplineStack->getspline(i)->getnumpoints())*npoints);
      this->SplineStack->getspline(i)->changespacing(smooth,np,step,csmooth,iterations);
    }
  this->Modified();
  this->RedrawSurface=1;
  return 1;


}

//------------------------------------------------------------------------------
int vtkpxSplineStackSource::SetSpline(vtkpxSplineSource* source,int ind)
{
  return this->SetSpline(source->GetSpline(),ind);
}

int vtkpxSplineStackSource::SetSplineFromSplineStack(vtkpxSplineSource* source,int ind)
{
 if (ind>=GetTrimBottom() && ind<=GetTrimTop())
   {
     float z=source->GetSpline()->getzlevel();
     source->GetSpline()->copypoints(this->SplineStack->getspline(ind));
     source->GetSpline()->setzlevel(z);
     source->Modified();
     return ind;
   }
 else
   return -1;

}

//------------------------------------------------------------------------------
int vtkpxSplineStackSource::SetSpline(PXFPBSpline* spl,int ind)
{
  if (ind>=GetTrimBottom() && ind<=GetTrimTop())
      {
	if (spl!=NULL)
	  {
	    float z=this->SplineStack->getspline(ind)->getzlevel();
	    this->SplineStack->getspline(ind)->copypoints(spl);
	    this->SplineStack->getspline(ind)->setzlevel(z);
	  }

	if ( this->WholeSurface==1 || this->sliceSurfaceData==NULL)
	  {
	    this->RedrawSurface=1;
	    this->Modified();
	  }
	else
	  {
	    int bottom=ind-1;
	    if (bottom<this->SplineStack->gettrimbottom())
	      bottom=this->SplineStack->gettrimbottom();
	    int top=ind+1;
	    if (top>this->SplineStack->gettrimtop())
	      top=this->SplineStack->gettrimtop();

	    this->AddSections(bottom,top);
	    if (this->PolarMode && (ind==this->SplineStack->gettrimbottom() || ind==this->SplineStack->gettrimtop()))
	      this->AddSections(this->SplineStack->gettrimtop(),this->SplineStack->gettrimtop()+1);
	    this->Modified();
	  }
	return ind;
      }
  else
    {
      fprintf(stderr,"Out out Bounds\n");
    }
  return -1;
}

// -------------------------------------- Display Garbage --------------------------------
void vtkpxSplineStackSource::CreateSurface()
{
  if (this->SplineStack->gettrimbottom() == this->SplineStack->gettrimtop())
    {
      this->WholeSurface=1;
      if (this->RedrawSurface==1 || this->wholeSurfaceData==NULL)
	{
	  if (this->wholeSurfaceData==NULL)
	    this->wholeSurfaceData=vtkPolyData::New();

	  PXContourStack* stack=this->SplineStack->exporttocontourstack(this->ResolutionU);
	  stack->getcontour(1)->copypoints(stack->getcontour(0));
	  stack->setzlevel(1,stack->getzlevel(0)+0.1);
	  stack->setzlevel(0,stack->getzlevel(0)-0.1);
	  PXTriangulatedStack* ts=new PXTriangulatedStack(stack,0);
	  vtkpxSurfaceUtil::drawtstack(this->wholeSurfaceData,ts);
	  delete ts;
	  delete stack;
	  this->RedrawSurface=0;
	}
      return;
    }


  if (this->WholeSurface)
      {
	if (this->RedrawSurface==1 || this->wholeSurfaceData==NULL)
	  {
	    vtkDebugMacro(<<"Creating Whole Surface DrawMode =" << this->DrawMode << " Pmode = " << this->PolarMode);
	    if (this->wholeSurfaceData==NULL)
	      this->wholeSurfaceData=vtkPolyData::New();

	    PXContourStack* stack;
	    PXTriangulatedStack* ts;
	    if (this->PolarMode)
	      {
		stack=this->SplineStack->polarexporttocontourstack(this->PolarMiddle,this->ResolutionU);
		stack->nonShrinkSmooth2D(this->ResolutionU,1.0,50);
		ts=new PXTriangulatedStack(stack,0);
	      }
	    else
	      {
		if (this->DrawMode==0 || this->SplineStack->apexenabled() )
		  {
		    vtkDebugMacro(<<"In DrawMode == 0 ");
		    stack=this->SplineStack->exporttocontourstack(this->ResolutionU,this->ResolutionV,1);
		  }
		else
		  {
		    vtkDebugMacro(<<"In Comples DrawMode ");
		    PXContourStack* stack2=this->SplineStack->exporttocontourstack(this->ResolutionU,this->ResolutionV,0);
		    switch (this->DrawMode)
		      {
		      case 3:
			stack=new PXContourStack(stack2,0,0,1,1);
			break;
		      case 2:
			stack=new PXContourStack(stack2,0,0,0,1);
			break;
		      default:
			stack=new PXContourStack(stack2,0,0,1,0);
			break;
		      }
		    delete stack2;
		  }
		ts=new PXTriangulatedStack(stack,0);
	      }
	    //int num=this->SplineStack->getnumsplines();
	    vtkpxSurfaceUtil::drawtstack(this->wholeSurfaceData,ts);
	    delete ts;
	    delete stack;
	    this->RedrawSurface=0;
	  }
      }
  else if (this->RedrawSurface==1 || this->sliceSurfaceData==NULL)
    {
      vtkDebugMacro(<<"Creating Slice Surface");

      // First Make Sure that wholeSurfaceData has the right dimensions etc
      int num=this->SplineStack->getnumsplines()-1;
      if (this->PolarMode)
	++num;
      
      int doadd=0;
      if (this->sliceSurfaceData==NULL)
	{
	  this->sliceSurfaceData=vtkPolyDataCollection::New();
	  doadd=1;
	}
      else if (this->sliceSurfaceData->GetNumberOfItems()!=num)
	doadd=1;
      
      if (doadd==1)
	{
	  this->sliceSurfaceData->RemoveAllItems();
	  for (int i=0;i<num;i++)
	    {
	      vtkPolyData* poly=vtkPolyData::New();
	      this->sliceSurfaceData->AddItem(poly);
	      poly->Delete();
	    }
	  //vtkDebugMacro(<<"Created "<< this->sliceSurfaceData->GetNumberOfItems() << "Sub slices");
	}

      if (this->PolarMode)
	this->AddSections(this->SplineStack->gettrimbottom(),this->SplineStack->gettrimtop()+1);
      else
	this->AddSections(this->SplineStack->gettrimbottom(),this->SplineStack->gettrimtop());
      this->RedrawSurface=0;
    }
  else
    vtkDebugMacro(<<"Surface is up to date");
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::AddApex()
{
  if (this->PolarMode || !ApexEnabled())
      return;

  // Do Something Here 
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::AddSections(int lower,int upper)
{
  if (lower<0)
    lower=0;
  if (upper>=this->SplineStack->getnumsplines())
    upper=this->SplineStack->getnumsplines();

  for (int splineno=lower;splineno<upper;splineno++)
    CreateSingleSection(splineno);
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::RemoveSections(int lower,int upper)
{
  if (this->sliceSurfaceData==NULL)
    return;

  if (lower<0)
    lower=0;
  if (upper>=this->SplineStack->getnumsplines())
    upper=this->SplineStack->getnumsplines();


  for (int splineno=lower;splineno<upper;splineno++)
      {
	vtkPolyData* poly=(vtkPolyData*)this->sliceSurfaceData->GetItemAsObject(splineno);
	if (poly!=NULL)
	  poly->Initialize();
      }
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::CreateSingleSection(int splineno)
{
  vtkPolyData* sep=(vtkPolyData*)this->sliceSurfaceData->GetItemAsObject(splineno);
  if (sep==NULL)
      return;

  //  vtkDebugMacro(<<"Creating Section " << splineno);

  sep->Initialize();

  int oldap=this->SplineStack->apexenabled();
  int tb=this->SplineStack->gettrimbottom();
  int tt=this->SplineStack->gettrimtop();
  this->SplineStack->enableapex(0);

  PXContourStack* cstack;

  if (!this->PolarMode)
      {
	this->SplineStack->settrimbottom(splineno);
	this->SplineStack->settrimtop(splineno+1);
	if (splineno==tt-1 && splineno==tb  && this->DrawMode==3)
	  {
	    PXContourStack* cstack2=this->SplineStack->exporttocontourstack(this->ResolutionU,this->ResolutionV,1);
	    cstack=new PXContourStack(cstack2,0,0,1,1);
	    delete cstack2;
	  }
	else if (splineno==tt-1 && this->DrawMode>=2)
	  {
	    PXContourStack* cstack2=this->SplineStack->exporttocontourstack(this->ResolutionU,this->ResolutionV,1);
	    cstack=new PXContourStack(cstack2,0,0,0,1);
	    delete cstack2;
	  }
	else  if (splineno==tb && ( this->DrawMode==1 || this->DrawMode==3))
	  {
	    PXContourStack* cstack2=this->SplineStack->exporttocontourstack(this->ResolutionU,this->ResolutionV,1);
	    cstack=new PXContourStack(cstack2,0,0,1,0);
	    delete cstack2;
	  }
	else
	  cstack=this->SplineStack->exporttocontourstack(this->ResolutionU,this->ResolutionV,1);
      }
  else
      {
	if (splineno!=this->SplineStack->gettrimtop())
	    {
	      this->SplineStack->settrimbottom(splineno);
	      this->SplineStack->settrimtop(splineno+1);
	      cstack=this->SplineStack->exporttocontourstack(this->ResolutionU);
	    }
	else
	    {
	      cstack=new PXContourStack(2);
	      (cstack->getcontour(0))->copypoints(this->SplineStack->getspline(this->SplineStack->gettrimtop())->exporttocontour(this->ResolutionU));
	      cstack->setzlevel(0,this->SplineStack->getspline(this->SplineStack->gettrimtop())->getzlevel());
	      (cstack->getcontour(0))->ensureAnticlockwise();
	      (cstack->getcontour(1))->copypoints(this->SplineStack->getspline(this->SplineStack->gettrimbottom())->exporttocontour(this->ResolutionU));
	      cstack->setzlevel(1,this->SplineStack->getspline(this->SplineStack->gettrimbottom())->getzlevel());      
	      (cstack->getcontour(1))->ensureClockwise();
	    }
      }
  
  this->SplineStack->settrimbottom(tb);
  this->SplineStack->settrimtop(tt);
  this->SplineStack->enableapex(oldap);
  
  if (!this->PolarMode)
      {
	PXTriangulatedStack* ts=new PXTriangulatedStack(cstack,0);
	vtkDebugMacro(<<"Drawing Section " << splineno << "Np="<< ts->getnumpoints()<<" Nt="<<ts->getnumtriangles());
	vtkpxSurfaceUtil::drawtstack(sep,ts);
	delete ts;
      }
  else
      {	
	//vtkDebugMacro(<<"Drawing Section " << splineno);
	vtkpxSurfaceUtil::createsection(cstack,sep,1,this->PolarMiddle);
      }
  delete cstack;
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::Execute()
{
  if (this->SplineStack==NULL)
      {
	vtkErrorMacro(<<"No spline available");
	return;
      }
  vtkPolyData* output = this->GetOutput();

  CreateSurface();

  if (this->WholeSurface)
      {
	if (this->SplineStack->gettrimbottom() < this->SplineStack->gettrimtop())
	  {
	    vtkDebugMacro(<<"Using Whole Surface");
	    vtkPolyDataNormals* normalsFilter=vtkPolyDataNormals::New();
	    normalsFilter->SetInput(wholeSurfaceData);
	    normalsFilter->SetFeatureAngle(0.0);
	    normalsFilter->SetSplitting(0);
	    normalsFilter->Update();
	    output->ShallowCopy(normalsFilter->GetOutput());
	    output->GetPointData()->PassData(normalsFilter->GetOutput()->GetPointData());
	    normalsFilter->Delete();
	  }
	else
	  {
	    output->ShallowCopy(wholeSurfaceData);
	  }
      }
  else
      {
	vtkDebugMacro(<<"Using Spline Surface " << sliceSurfaceData->GetNumberOfItems()<< ","<<this->SplineStack->getnumsplines());
	this->sliceSurfaceData->InitTraversal();
	vtkAppendPolyData* append=vtkAppendPolyData::New();
	for (int j=0;j<sliceSurfaceData->GetNumberOfItems();j++)
	  {
	    vtkPolyData* poly=this->sliceSurfaceData->GetNextItem();
	    if (j>=SplineStack->gettrimbottom() && j<=SplineStack->gettrimtop() && poly!=NULL)
		append->AddInput(poly);
	  }

	append->Update();
	output->CopyStructure(append->GetOutput());
	append->Delete();
      }


  int tb=this->SplineStack->gettrimbottom();
  int tt=this->SplineStack->gettrimtop();

  if (tb==tt)
    return;


  // From here this is a tiny hack to make sure things display OK
  // when clipped close to bottom or top XY image slice
  // Xenios Jan 31st 2010
  double numgap=double(tt-tb);


  double bounds[6]; output->GetBounds(bounds);
  vtkPoints* outp=output->GetPoints();
  int np=outp->GetNumberOfPoints();
  //  int nshift=0;

  double offset=(bounds[5]-bounds[4])/numgap*this->ZDisplayOffset;

  for (int i=0;i<np;i++)
    {
      double p[3];
      outp->GetPoint(i,p);
      if ( fabs(p[2]-bounds[4])<offset)
	{
	  p[2]=p[2]-offset;
	  outp->SetPoint(i,p);
	  //  ++nshift;
	}
      else if (fabs(p[2]-bounds[5])<offset)
	{
	  p[2]=p[2]+offset;
	  outp->SetPoint(i,p);
	  //	  ++nshift;
	}
    }
  //  fprintf(stdout,"nshift=%d point shifted by %f\n",nshift,offset);
}
//------------------------------------------------------------------------------
float vtkpxSplineStackSource::GetVolume()
{
  float totalarea=0.0;
  for (int i=this->GetTrimBottom(); i<=this->GetTrimTop();i++)
    {
      PXContour* cntr=this->SplineStack->getspline(i)->exporttocontour(0.05);
      totalarea+=cntr->getarea();
      delete cntr;
    }

  int tb=this->GetTrimBottom();
  int tt=this->GetTrimTop();

  float z2=this->GetSpline(tt)->getzlevel();
  float z1=this->GetSpline(tb)->getzlevel();

  float dz=(z2-z1)/float(tt-tb);

  return totalarea*dz;
}

//------------------------------------------------------------------------------
float vtkpxSplineStackSource::GetVOIVolume(vtkImageData* image,int frame,float threshold)
{
  float volume=1.0,mean=0.0,std=0.0;
  this->GetVOIProperties(image,frame,volume,mean,std,threshold);
  return volume;
}
//------------------------------------------------------------------------------
int  vtkpxSplineStackSource::GetVOIProperties(vtkImageData* image,int frame,
					      float& volume,float& mean,float& std,float threshold)
{
  if (image==NULL)
    return -1;

  double sp[3],ori[3];
  int dim[3];
  image->GetDimensions(dim);
  image->GetSpacing(sp);
  image->GetOrigin(ori);

  int numvoxels=0;
  float sum=0.0,sum2=0.0;


  for (int k=this->GetTrimBottom(); k<=this->GetTrimTop();k++)
    {
      int level=Irange(k,0,dim[2]-1);
      PXContour* cntr=this->SplineStack->getspline(level)->exporttocontour(0.05);
      
      float x1,x2,y1,y2;
      cntr->getextent(x1,x2,y1,y2);
      int imin=Irange( int((x1/sp[0])-ori[0]-1.0),0,dim[0]-2);
      int imax=Irange( int((x2/sp[0])-ori[0]-1.0),imin+1,dim[0]-1);
      
      int jmin=Irange( int((y1/sp[1])-ori[1]-1.0),0,dim[1]-2);
      int jmax=Irange( int((y2/sp[1])-ori[1]-1.0),jmin+1,dim[1]-1);
      
      
      for (int i=imin;i<=imax;i++)
	for (int j=jmin;j<=jmax;j++)
	  {
	    float x[2];
	    x[0]=float(i)*sp[0]+ori[0];
	    x[1]=float(j)*sp[1]+ori[1];

	    float v=image->GetScalarComponentAsDouble(i,j,level,frame);
	    if ( (v>=threshold) && ( cntr->IsInside(x[0],x[1]) > 0 ))
	      {
		++numvoxels;
		sum+=v;
		sum2+=v*v;
	      }
	  }
      delete cntr;
    }

  mean=0.0;std=0.0;volume=0.0;

  if (numvoxels>0)
    {
      mean=sum/float(numvoxels);
      std= sqrt( sum2/float(numvoxels)-mean*mean);
      volume=float(numvoxels)*sp[0]*sp[1]*sp[2];
    }

  return numvoxels;

}
//------------------------------------------------------------------------------
int vtkpxSplineStackSource::AddToObjectMap(vtkImageData* objectmap,float value,int add,int background)
{
  if (objectmap==NULL)
    return -1;
  
  double sp[3],ori[3];
  int dim[3];
  objectmap->GetDimensions(dim);
  objectmap->GetSpacing(sp);
  objectmap->GetOrigin(ori);
  
  int numvoxels=0;
  int slicesize=dim[1]*dim[0];
  vtkDataArray* objectmapScalars=objectmap->GetPointData()->GetScalars();
  int numscalars=objectmapScalars->GetNumberOfTuples();

  fprintf(stderr,"Slices %d:%d ",this->GetTrimBottom(),this->GetTrimTop());
  for (int k=this->GetTrimBottom(); k<=this->GetTrimTop();k++)
    {
      fprintf(stderr,"%d ",k);
      int level=Irange(k,0,dim[2]-1);
      PXContour* cntr=this->SplineStack->getspline(level)->exporttocontour(0.05);
      float x1,x2,y1,y2;
      cntr->getextent(x1,x2,y1,y2);
      int imin=Irange( int((x1/sp[0])-ori[0]-2.0),0,dim[0]-2);
      int imax=Irange( int((x2/sp[0])-ori[0]+2.0),imin+1,dim[0]-1);
      
      int jmin=Irange( int((y1/sp[1])-ori[1]-2.0),0,dim[1]-2);
      int jmax=Irange( int((y2/sp[1])-ori[1]+2.0),jmin+1,dim[1]-1);
      
      /*fprintf(stderr,"Level = %d Ranges = %.1f:%.1f %.1f:%.1f --> vox (%d:d %d:%d)\n",
	k,x1,y1,x2,y2,imin,jmin,imax,jmax);*/
      
      for (int i=imin;i<=imax;i++)
	for (int j=jmin;j<=jmax;j++)
	  {
	    float x[2];
	    x[0]=float(i)*sp[0]+ori[0];
	    x[1]=float(j)*sp[1]+ori[1];

	    if ( cntr->IsInside(x[0],x[1]) > 0 )
	      {
		++numvoxels;
		int index=Irange(k*slicesize+j*dim[0]+i,0,numscalars-1);
		int current=(int)objectmapScalars->GetComponent(index,0);
		
		if (current==background || add==0)
		  {
		    objectmapScalars->SetComponent(index,0,value);
		  }
		else
		  {
		    objectmapScalars->SetComponent(index,0,value+current);
		  }
	      }
	  }
      delete cntr;
    }
  fprintf(stderr,"\n");
  return numvoxels;
}
//------------------------------------------------------------------------------
int vtkpxSplineStackSource::CreateBoundedObjectMap(vtkpxSplineStackSource* bounding,vtkImageData* objectmap,
						   int inside_below,int inside_above,
						   int outside,int offslice)
{
  if (objectmap==NULL || bounding==NULL)
    return -1;
  
  double sp[3],ori[3];
  int dim[3];
  objectmap->GetDimensions(dim);
  objectmap->GetSpacing(sp);
  objectmap->GetOrigin(ori);
  
  int numvoxels=0;
  int slicesize=dim[1]*dim[0];
  vtkDataArray* objectmapScalars=objectmap->GetPointData()->GetScalars();
  objectmapScalars->FillComponent(0,double(offslice));
  int numscalars=objectmapScalars->GetNumberOfTuples();

  for (int k=this->GetTrimBottom(); k<=this->GetTrimTop();k++)
    {
      int level=Irange(k,0,dim[2]-1);
      PXContour* cntr=this->SplineStack->getspline(level)->exporttocontour(0.05);
      float x1,x2,y1,y2;
      cntr->getextent(x1,x2,y1,y2);
      int imin=Irange( int((x1/sp[0])-ori[0]-2.0),0,dim[0]-2);
      int imax=Irange( int((x2/sp[0])-ori[0]+2.0),imin+1,dim[0]-1);
      
      int jmin=Irange( int((y1/sp[1])-ori[1]-2.0),0,dim[1]-2);
      int jmax=Irange( int((y2/sp[1])-ori[1]+2.0),jmin+1,dim[1]-1);
      

      PXContour* cntr2=bounding->GetSplineStack()->getspline(level)->exporttocontour(0.05);
      cntr2->getextent(x1,x2,y1,y2);
      double  boundy=y1;

      int baseindex=k*slicesize;
      for (int ia=0; ia <slicesize ; ia++)
	objectmapScalars->SetComponent(baseindex+ia,0,(double)outside);
      
      for (int i=imin;i<=imax;i++)
	for (int j=jmin;j<=jmax;j++)
	  {
	    float x[2];
	    x[0]=float(i)*sp[0]+ori[0];
	    x[1]=float(j)*sp[1]+ori[1];
	    int index=Irange(k*slicesize+j*dim[0]+i,0,numscalars-1);
	    if ( cntr->IsInside(x[0],x[1]) > 0 )
	      {
		++numvoxels;
		if (x[1]>boundy)
		  objectmapScalars->SetComponent(index,0,(double)inside_below);
		else
		  objectmapScalars->SetComponent(index,0,(double)inside_above);
	      }
	    else
	      {
		objectmapScalars->SetComponent(index,0,(double)outside);
	      }
	  }
      delete cntr;
      delete cntr2;
    }
  return numvoxels;
}

int vtkpxSplineStackSource::CreateObjectMap(vtkImageData* objectmap,int inside,int outside,int offslice)
{
  if (objectmap==NULL)
    return -1;
  
  double sp[3],ori[3];
  int dim[3];
  objectmap->GetDimensions(dim);
  objectmap->GetSpacing(sp);
  objectmap->GetOrigin(ori);
  
  int numvoxels=0;
  int slicesize=dim[1]*dim[0];
  vtkDataArray* objectmapScalars=objectmap->GetPointData()->GetScalars();
  objectmapScalars->FillComponent(0,double(offslice));
  int numscalars=objectmapScalars->GetNumberOfTuples();

  for (int k=this->GetTrimBottom(); k<=this->GetTrimTop();k++)
    {
      int level=Irange(k,0,dim[2]-1);
      PXContour* cntr=this->SplineStack->getspline(level)->exporttocontour(0.05);

      float x1,x2,y1,y2;
      cntr->getextent(x1,x2,y1,y2);
      int imin=Irange( int((x1/sp[0])-ori[0]-2.0),0,dim[0]-2);
      int imax=Irange( int((x2/sp[0])-ori[0]+2.0),imin+1,dim[0]-1);
      
      int jmin=Irange( int((y1/sp[1])-ori[1]-2.0),0,dim[1]-2);
      int jmax=Irange( int((y2/sp[1])-ori[1]+2.0),jmin+1,dim[1]-1);
      
      int baseindex=k*slicesize;
      for (int ia=0; ia <slicesize ; ia++)
	objectmapScalars->SetComponent(baseindex+ia,0,(double)outside);
      
      for (int i=imin;i<=imax;i++)
	for (int j=jmin;j<=jmax;j++)
	  {
	    float x[2];
	    x[0]=float(i)*sp[0]+ori[0];
	    x[1]=float(j)*sp[1]+ori[1];
	    int index=Irange(k*slicesize+j*dim[0]+i,0,numscalars-1);
	    if ( cntr->IsInside(x[0],x[1]) > 0 )
	      {
		++numvoxels;
		objectmapScalars->SetComponent(index,0,(double)inside);
	      }
	    else
	      {
		objectmapScalars->SetComponent(index,0,(double)outside);
	      }
	  }
      delete cntr;
    }
  return numvoxels;
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::ScaleShiftZ(float shift,float scale)
{
  if (scale<=0.01)
    scale=1.0;

  int ns=this->SplineStack->getnumsplines();
  for (int i=0;i<ns;i++)
    {
      PXFPBSpline* spl=this->SplineStack->getspline(i);
      if (spl!=NULL)
	{
	  float z=spl->getzlevel();
	  spl->setzlevel(shift+scale*z);
	}
    }
  this->Modified();
  this->RedrawSurface=1;
}

void vtkpxSplineStackSource::ScaleShiftXY(float shiftx,float shifty,float scalex,float scaley)
{
   int ns=this->SplineStack->getnumsplines();
  for (int i=0;i<ns;i++)
    {
      PXFPBSpline* spl=this->SplineStack->getspline(i);
      if (spl!=NULL)
	spl->scaleshift2D(shiftx,shifty,scalex,scaley);
    }
  this->Modified();
  this->RedrawSurface=1;
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::AdjustNumberOfSplines(int newnumber)
{
  this->SplineStack->adjustnumberofsplines(newnumber);
}
//------------------------------------------------------------------------------
int  vtkpxSplineStackSource::GetDualVOIProperties(vtkImageData* image,int frame,
						  float volume[2],float mean[2],float std[2],int numvoxels[2],
						  float threshold[2])
{
  if (image==NULL)
    return -1;

  double sp[3],ori[3];
  int dim[3];
  image->GetDimensions(dim);
  image->GetSpacing(sp);
  image->GetOrigin(ori);

  numvoxels[0]=0; numvoxels[1]=0;
  int total=0;
  float sum[2],sum2[2];
  
  sum[0]=0.0; sum[1]=0.0; sum2[0]=0.0; sum2[1]=0.0;


  for (int k=this->GetTrimBottom(); k<=this->GetTrimTop();k++)
    {
      int level=Irange(k,0,dim[2]-1);
      PXContour* cntr=this->SplineStack->getspline(level)->exporttocontour(0.05);
      float x1,x2,y1,y2;
      cntr->getextent(x1,x2,y1,y2);
      int imin=Irange( int((x1/sp[0])-ori[0]-1.0),0,dim[0]-2);
      int imax=Irange( int((x2/sp[0])-ori[0]-1.0),imin+1,dim[0]-1);
      
      int jmin=Irange( int((y1/sp[1])-ori[1]-1.0),0,dim[1]-2);
      int jmax=Irange( int((y2/sp[1])-ori[1]-1.0),jmin+1,dim[1]-1);

      fprintf(stderr,"k=%d i=%d:%d j=%d:%d\n",k,imin,imax,jmin,jmax);
      
      for (int i=imin;i<=imax;i++)
	for (int j=jmin;j<=jmax;j++)
	  {
	    float v=image->GetScalarComponentAsDouble(i,j,level,frame);
	    float x[2];
	    x[0]=float(i)*sp[0]+ori[0];
	    x[1]=float(j)*sp[1]+ori[1];
	    if (cntr->IsInside(x[0],x[1]) > 0)
	      {
		++total;
		for (int r=0;r<=1;r++)
		  {
		    if (v>threshold[r])
		      {
			++numvoxels[r];
			sum[r]+=v;
			sum2[r]+=v*v;
		      }
		  }
	      }
	  }
      delete cntr;
    }
	
  for (int r=0;r<=1;r++)
    {
      mean[r]=sum[r]/float(numvoxels[r]+0.00001);
      std[r]= sqrt( sum2[r]/float(numvoxels[r]+0.0001)-mean[r]*mean[r]);
      volume[r]=float(numvoxels[r])*sp[0]*sp[1]*sp[2];
    }

  return total;

}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::LoadAndFitContourStack(const char* fname)
{
  PXContourStack* cnt=new PXContourStack();
  int ok=cnt->Load(fname);
  if (ok)
    this->FitToContourStack(cnt,2.0,0.5,25,1);
  delete cnt;
}
//------------------------------------------------------------------------------  
int vtkpxSplineStackSource::ComputeZRange(vtkCollection* collection,int numframes,int numsurfaces,
					  int beginframe,int endframe,
					  int index,float z[2])
{
  float minz=0,maxz=0;
  index=Irange(index,0,numsurfaces-1);

  beginframe=Irange(beginframe,0,numframes-1);
  endframe=Irange(endframe,beginframe,numframes-1);

  for (int frame=beginframe;frame<=endframe;frame++)
    {
      vtkpxSplineStackSource* stack=(vtkpxSplineStackSource*)collection->GetItemAsObject(frame*numsurfaces+index);
      
      if (stack->GetPolarMode()==0)
	{
	  if (frame==beginframe)
	    {
	      minz=(float)stack->GetTrimBottom();
	      maxz=(float)stack->GetTrimTop();
	    }
	  else
	    {
	      minz=Fmax(minz,(float)stack->GetTrimBottom());
	      maxz=Fmin(maxz,(float)stack->GetTrimTop());
	    }
	  //fprintf(stderr,"Frame=%d minz=%.1f maxz=%.1f\n",frame,minz,maxz);
	}
      else
	{
	  if (frame==beginframe)
	    {
	      minz=stack->GetSplineStack()->getpolarbottomz()+3.0;
	      maxz=stack->GetSplineStack()->getpolartopz()-3.0;
	    }
	  else
	    {
	      minz=Fmax(minz,stack->GetSplineStack()->getpolarbottomz()+3.0);
	      maxz=Fmin(maxz,stack->GetSplineStack()->getpolartopz()-3.0);
	    }
	}
    }

  z[0]=minz;
  z[1]=maxz;

  return 1;
}

void vtkpxSplineStackSource::SetDrawMode(int dm)
{
  dm=Irange(dm,0,3);
  if (this->DrawMode!=dm)
    {
      this->DrawMode=dm;
      this->RedrawSurface=1;
      this->Modified();
    }
}

void vtkpxSplineStackSource::SetPolarMode(int pm)
{
  pm=pm>0;
  if (this->PolarMode!=pm)
    {
      this->PolarMode=pm;
      this->RedrawSurface=1;
      this->Modified();
    }
}


void vtkpxSplineStackSource::SetWholeSurface(int pm)
{
  pm=pm>0;
  if (this->WholeSurface!=pm)
    {
      this->WholeSurface=pm;
      this->RedrawSurface=1;
      this->Modified();
    }
}
// ------------------------------------------------------------------------------------------------------------------
int vtkpxSplineStackSource::FillDistanceMap(vtkImageData* distancemap)
{
  if (distancemap==NULL)
    return -1;
  
  double sp[3],ori[3];
  int dim[3];
  distancemap->GetDimensions(dim);
  distancemap->GetSpacing(sp);
  distancemap->GetOrigin(ori);
  
  float pxl=(sp[0]*100.0);
  PXChamferDistanceMap::pixel_size=short(pxl);
  PXChamferDistanceMap::obj_gray=10000;
  PXChamferDistanceMap::out_gray=-10000;
  
  int slicesize=dim[1]*dim[0];
  vtkDataArray* distances=distancemap->GetPointData()->GetScalars();
  distances->FillComponent(0,-10000);


  for (int k=this->GetTrimBottom(); k<=this->GetTrimTop();k++)
    {
      int level=k;
      int offset=slicesize*level;

      // Export To Contour and Scale in Voxel Units
      PXContour* cntr=this->SplineStack->getspline(level)->exporttocontour(0.05);

      PXContourPoint* pts=cntr->gettoppoint();
      int np=cntr->getnumpoints();
      for (int ib=1;ib<=np;ib++)
	{
	  pts->x=(pts->x-ori[0])/sp[0];
	  pts->y=(pts->y-ori[1])/sp[1];
	  pts=pts->next;
	}
      
      PXChamferDistanceMap* map=new PXChamferDistanceMap(cntr,1);

      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {
	    float v=(float(map->getvalue(i,j)));
	    int index=offset+j*dim[0]+i;
	    distances->SetComponent(index,0,v);
	  }
      delete map;
      delete cntr;
    }

  
  if (this->GetTrimBottom()>0)
    {
      int trim=this->GetTrimBottom();

      for (int k=trim-1;k>=0;k=k-1)
	{
	  int offset1=slicesize*k;
	  int offset2=slicesize+offset1;
	  
	  float maxv=1.0;
	  if (k==trim-1)
	    {
	      for (int index=0;index<slicesize;index++)
		maxv=Fmax(maxv,distances->GetComponent(index+offset2,0));
	    }
	  
	  for (int index=0;index<slicesize;index++)
	    {
	      float v=distances->GetComponent(index+offset2,0);
	      if (v<0.0)
		{
		  distances->SetComponent(index+offset1,0,v-sp[2]*100.0);
		}
	      else
		{
		  // True Boundary is tloc at away from bottom slice
		  // Hence distancemap at tloc=0
		  // d=  (1-tloc)*v + (tloc)*newv = 0
		  // --> newv=-(1-tloc)*v/tloc

		  // tloc=0.5 at v=maxv  and 0 and v=0
		  float tloc=0.5*pow(v/maxv,float(2.0));
		  float newv=-(1.0-tloc)*v/tloc;
		  distances->SetComponent(index+offset1,0,newv);
		}
	    }
	}
    }


  if (this->GetTrimTop()<dim[2]-1)
    {
      int trim=this->GetTrimTop();

      for (int k=trim+1;k<dim[2];k++)
	{
	  int offset1=slicesize*k;
	  int offset2=offset1-slicesize;
	  
	  float maxv=1.0;
	  if (k==trim+1)
	    {
	      for (int index=0;index<slicesize;index++)
		maxv=Fmax(maxv,distances->GetComponent(index+offset2,0));
	    }
	  
	  for (int index=0;index<slicesize;index++)
	    {
	      float v=distances->GetComponent(index+offset2,0);
	      if (v<0.0)
		{
		  distances->SetComponent(index+offset1,0,v-sp[2]*100.0);
		}
	      else
		{
		  // True Boundary is tloc at away from bottom slice
		  // Hence distancemap at tloc=0
		  // d=  (1-tloc)*v + (tloc)*newv = 0
		  // --> newv=-(1-tloc)*v/tloc

		  // tloc=0.5 at v=maxv  and 0 and v=0
		  float tloc=0.5*pow(v/maxv,float(2.0));
		  float newv=-(1.0-tloc)*v/tloc;
		  distances->SetComponent(index+offset1,0,newv);
		}
	    }
	}
    }


  return 1;
}
//------------------------------------------------------------------------------
void vtkpxSplineStackSource::UpSample()
{
  PXFPBSplineStack* newstack=new PXFPBSplineStack(this->SplineStack,1,1,0);
  delete this->SplineStack;
  this->SplineStack=newstack;
}

int vtkpxSplineStackSource::FlipX(vtkImageData* image)
{
  if (image==NULL)
    return 0;

  double spacing[3]; image->GetSpacing(spacing);
  double origin[3];  image->GetOrigin(origin);
  int dimensions[3]; image->GetDimensions(dimensions);
  float cx=spacing[0]*dimensions[0]/2+origin[0];
  
  for (int k=this->GetTrimBottom(); k<=this->GetTrimTop();k++)
    {
      PXFPBSpline* spl=this->SplineStack->getspline(k);

      for (int i=0;i<spl->getnumcontrolpoints();i++)
	{
	  float x1=spl->getcontrolx(i);
	  spl->shiftcontrol(i,-2.0*(x1-cx),0,0);
	  float x2=spl->getcontrolx(i);
	  //	  fprintf(stderr,"x1=%.2f --> x2=%.2f\n",x1,x2);
	}
      
    }
  this->RedrawSurface=1;
  this->Modified();
  
  return 1;
}

//------------------------------------------------------------------------------
int vtkpxSplineStackSource::CreateSurfaceFromImage(vtkImageData* img,int minslice0,int maxslice0,int frame,float curvesmooth,float smoothness)
{
  if (img==NULL)
    {
      return 0;
    }

  int nc=img->GetNumberOfScalarComponents();
  if (frame<1)
    frame=1;
  if (frame>nc)
    frame=nc;

  int minslice=minslice0;
  int maxslice=maxslice0;

  vtkImageData* thr=vtkpxSplineStackSource::PreprocessBinaryImage(img,frame-1);
  PXContourStack* stack=ExtractContourStackFromImage(thr,minslice,maxslice,smoothness);
  
  int ok=this->SplineStack->fittocontourstack(stack,curvesmooth,0.05,-1,1);
  fprintf(stderr,"Final Trims = %d:%d\n",minslice,maxslice);
  this->SetTrimBottom(minslice);
  this->SetTrimTop(maxslice);
  
  delete stack;
  thr->Delete();

  this->Modified();
  this->RedrawSurface=1;

  return ok;
  
}
//------------------------------------------------------------------------------
vtkImageData* vtkpxSplineStackSource::PreprocessBinaryImage(vtkImageData* img,int frame)
{
  if (img==NULL)
    {
      // fprintf();
      return NULL;
    }
  
  double range[2]; img->GetPointData()->GetScalars()->GetRange(range);
  double threshold=0.5*(range[1]+range[0]);
  if (fabs(range[0]-0)<0.01 && fabs(range[1]-1.0)<0.01)
    threshold=1.0;

  fprintf(stderr,"Threshold=%.2f\n",threshold);

  
  vtkImageExtractComponents* comp=  vtkImageExtractComponents::New();
  comp->SetInput(img);
  comp->SetComponents(frame);
  
  vtkImageThreshold* thr=vtkImageThreshold::New();
  thr->SetInput(comp->GetOutput());
  thr->ThresholdBetween(threshold,range[1]+1.0);
  thr->ReplaceInOn();
  thr->ReplaceOutOn();
  thr->SetInValue(100);
  thr->SetOutValue(0);
  thr->SetOutputScalarTypeToUnsignedChar();
  thr->Update();
  
  vtkImageData* tmp=vtkImageData::New();
  tmp->ShallowCopy(thr->GetOutput());
  //  smooth->Delete();
  thr->Delete();
  comp->Delete();
  
  return tmp;
}
//------------------------------------------------------------------------------
PXContourStack* vtkpxSplineStackSource::ExtractContourStackFromImage(vtkImageData* img,int& minslice,int& maxslice,float sigma)
{
  int dim[3]; img->GetDimensions(dim);
  double ori[3]; img->GetOrigin(ori);
  double spa[3]; img->GetSpacing(spa);
  
   
  PXContourStack* stack=new PXContourStack(dim[2]);

  double c[2],r[2];
  for (int ia=0;ia<=1;ia++)
    {
      c[ia]=ori[ia]+0.5*spa[ia]*double(dim[ia]-1);
      r[ia]=0.2*spa[ia]*double(dim[ia]-1);
    }
  stack->setellipticalcylinder(4,r[0],r[1],c[0],c[1]);
  
  for (int i=0;i<dim[2];i++)
    {
      double z=double(i)*spa[2]+ori[2];
      stack->setzlevel(i,z);
    }

  int found=0,newmin=minslice,newmax=maxslice;

  for (int i=minslice;i<=maxslice;i++)
    {
      PXContour* cntr=ExtractContourFromImage(img,i,sigma);
      if (cntr!=NULL)
	{
	  stack->getcontour(i)->copypoints(cntr);
	  delete cntr;
	  if (found==0)
	    {
	      newmin=i;
	      newmax=i;
	      found=1;
	    }
	  else if (found==1)
	    {
	      ++newmax;
	    }
	}
      else
	{
	  if (found==1)
	    i=maxslice+1;
	}
    }

   
  minslice=newmin;
  maxslice=newmax;

  return stack;
  

}
//------------------------------------------------------------------------------
int vtkpxSplineStackSource::SetFromPolyData(PXContour* cntr,vtkPolyData* polydata)
{
  if (polydata==NULL || cntr==NULL)
      return 0;

  cntr->DeleteAll();


  vtkPoints* newPoints=polydata->GetPoints();
  int np=newPoints->GetNumberOfPoints();

	
  int* neigh=new int[np*2];
  int* used=new int[np];

  for (int ib=0;ib<np*2;ib++)
      neigh[ib]=-1;
  for (int ic=0;ic<np;ic++)
      used[ic]=0;
 
  vtkCellArray* lines=polydata->GetLines();
  int numlines=lines->GetNumberOfCells();
  lines->InitTraversal();
  for (int ia=0;ia<numlines;ia++)
    {
      vtkIdType nump;
      vtkIdType *p;
      lines->GetNextCell(nump,p);
      
      for (int k=0;k<=1;k++)
	{
	  int index=p[k]*2;
	  if (neigh[index]==-1)
	    neigh[index]=p[1-k];
	  else
	    neigh[index+1]=p[1-k];
	}
    }

  int nearest=0;
  double x[3];
  newPoints->GetPoint(nearest,x);

  int done=0,numlinks=0,firstpoint=nearest;

  while (done==0 && numlinks<np-1)
      {
	int index=firstpoint*2;
	used[firstpoint]=1;

	//fprintf(stderr,"Parsing Point=%3d Neighbours %3d,%3d\n",firstpoint,neigh[index],neigh[index+1]);
	if (neigh[index]==-1 || neigh[index+1]==-1)
	    {
	      done=1;

	      // Unless we can merge 
	      double x1[3],x2[3];
	      newPoints->GetPoint(firstpoint,x1);
	      for (int kk=0;kk<np;kk++)
		  {
		    if (kk!=firstpoint)
			{
			  int index2=kk*2;
			  if (neigh[index2]==-1 || neigh[index2+1]==-1)
			      {
				newPoints->GetPoint(kk,x2);
				if (Fsqdist(x1[0],x1[1],x1[2],x2[0],x2[1],x2[2])<1.0)
				    {
				      /*fprintf(stderr,"\tAdding Link from %5.1f,%5.1f,%5.1f to %5.1f,%5.1f,%5.1f (index=%d,%d)\n",
					      x1[0],x1[1],x1[2],x2[0],x2[1],x2[2],kk,firstpoint);*/
				      done=0;

				      if (neigh[index]==-1)
					  neigh[index]=kk;
				      else
					  neigh[index+1]=kk;

				      // Chain Back

				      if (neigh[index2]==-1)
					  neigh[index2]=firstpoint;
				      else
					  neigh[index2+1]=firstpoint;
				      kk=np;
				    }
			      }
			}
		  }

	    }

	if (done==0)
	    {
	      int keeplooking=1;

	      if (neigh[index]!=-1)
		  {
		    if (used[neigh[index]]==0)
			{
			  firstpoint=neigh[index];
			  keeplooking=0;
			}
		  }

	      if (keeplooking==1 && neigh[index+1]!=-1)
		  {
		    if (used[neigh[index+1]]==0)
			{
			  firstpoint=neigh[index+1];
			  keeplooking=0;
			}
		  }

	      if (keeplooking==1)
		  done=1;
	      else
		  numlinks++;
	    }
      }

  /*fprintf(stderr,"\tFirstPoint =%d (Neighbours=%d,%d) numlinks=%d (np=%d)\n",firstpoint,neigh[firstpoint*2],
	  neigh[firstpoint*2+1],numlinks,np);*/

  for (int ie=0;ie<np;ie++)
      used[ie]=0;

  int numgoodpoints=0,found=0;
  int nextpoint=firstpoint;
  while (found==0 && numgoodpoints<np)
      {
	int currentpoint=nextpoint;
	used[currentpoint]=1;
	int index=currentpoint*2;

	/*fprintf(stderr,"Parsing Point=%3d Neighbours %3d,%3d Used=%d,%d\n",
		currentpoint,neigh[index],neigh[index+1],used[neigh[index]],
		used[neigh[index+1]]);*/
	numgoodpoints++;

	int keeplooking=1;
	if (neigh[index]!=-1)
	    {
	      if (used[neigh[index]]==0)
		  {
		    nextpoint=neigh[index];
		    keeplooking=0;
		  }
	    }
	
	if (keeplooking==1 && neigh[index+1]!=-1)
	    {
	      if (used[neigh[index+1]]==0)
		  {
		    nextpoint=neigh[index+1];
		    keeplooking=0;
		  }
	    }

	if (keeplooking==1)
	    found=1;
      }

  //fprintf(stderr,"\tNumgoodpoints=%d\n",numgoodpoints);

  // Restablish all links
  for (int iaa=0;iaa<np;iaa++)
      used[iaa]=0;

  for (int k=0;k<numgoodpoints;k++)
      {
	double x[3],y[3];
	newPoints->GetPoint(firstpoint,x);
	used[firstpoint]=1;

	cntr->Add(x[0],x[1]);
	int index=firstpoint*2;
	int keeplooking=1;

	if (neigh[index]!=-1)
	    {
	      if (used[neigh[index]]==0)
		  {
		    firstpoint=neigh[index];
		    keeplooking=0;
		  }
	    }

	if (keeplooking==1 && neigh[index+1]!=1)
	  {
	      if (used[neigh[index+1]]==0)
		  {
		    firstpoint=neigh[index+1];
		    keeplooking=0;
		  }
	    }

	if (keeplooking==1)
	    {
	      k=np;
	    }
      }
  
  delete [] used;
  delete [] neigh;
  return 1;
}

PXContour* vtkpxSplineStackSource::ExtractContourFromImage(vtkImageData* img,int slice,float sigma)
{
  vtkpxImageExtract* extr=vtkpxImageExtract::New();
  extr->SetInput(img);
  extr->SetCurrentPlane(2);
  extr->SetSliceNo(slice);
  extr->SetFrame(1);
  extr->Update();

  vtkImageSeedConnectivity* seed=  vtkImageSeedConnectivity::New();
  seed->SetInput(extr->GetOutput());
  seed->SetInputConnectValue(0);
  seed->SetOutputConnectedValue(0);
  seed->SetOutputUnconnectedValue(100);
  seed->AddSeed(0,0,0);
  seed->Update();

  vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
  smooth->SetInput(seed->GetOutput());
  smooth->SetStandardDeviations(sigma,sigma,0.0);
  smooth->SetDimensionality(2);
  smooth->Update();
  
  vtkContourFilter* contourExtractor = vtkContourFilter::New();
  contourExtractor->SetInput(smooth->GetOutput());
  contourExtractor->SetValue(0,50.0);
  contourExtractor->ComputeScalarsOff();
  contourExtractor->ComputeGradientsOff();
  contourExtractor->ComputeNormalsOff() ;
  contourExtractor->Update();

  vtkPolyDataConnectivityFilter* polycon=vtkPolyDataConnectivityFilter::New();
  polycon->SetInput(contourExtractor->GetOutput());
  polycon->SetExtractionModeToLargestRegion();
  polycon->Update();

  PXContour* cntr=NULL;
  vtkPoints* pts=polycon->GetOutput()->GetPoints();
  int np=0;

  if (pts!=NULL)
    np=pts->GetNumberOfPoints();

  if (np>0)
    {
      cntr=new PXContour();
      cntr->closedcontour(1);
      SetFromPolyData(cntr,polycon->GetOutput());
      fprintf(stderr,"Extracted %d points\n",np);
    }
  else
    {
      fprintf(stderr,"Failed to Extract Curve! slice=%d\n",slice);
    }
  polycon->Delete();
  contourExtractor->Delete();
  seed->Delete();
  smooth->Delete();
  extr->Delete();

  return cntr;
}

      
      
      
  

