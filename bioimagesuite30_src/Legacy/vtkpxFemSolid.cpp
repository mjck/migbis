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
  Module:    $RCSfile: vtkpxFemSolid.cpp,v $
  Language:  C++
  Date:      $Date: 2003/06/05 21:07:43 $
  Version:   $Revision: 1.2 $


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
#include "pxabaqussolid.h"
#include "vtkCellArray.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkpxFemSolid.h"
#include "vtkFloatArray.h"
#include "pxabaqussolid.h"
#include "vtkUnsignedCharArray.h"
#include "vtkSphereSource.h"
#include "vtkCollection.h"
#include "vtkCellData.h"
#include "vtkMath.h"
#include "nrutil.h"
#include "pxgeometrystruct.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
//------------------------------------------------------------------------------
vtkpxFemSolid* vtkpxFemSolid::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxFemSolid");
  if(ret)
    {
    return (vtkpxFemSolid*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxFemSolid;
}
//------------------------------------------------------------------------------
vtkpxFemSolid::vtkpxFemSolid()
{
  this->InternalSolid=new PXAbaqusSolid();
  this->ColorMode=0;
  this->StrainMode=0;
  this->WarpMode=0;
}
//------------------------------------------------------------------------------
vtkpxFemSolid::~vtkpxFemSolid()
{
  if (this->InternalSolid)
      delete this->InternalSolid;

}
//------------------------------------------------------------------------------
int vtkpxFemSolid::Save(const char* fname)
{
  if (this->InternalSolid==NULL)
      return 0;
  return this->InternalSolid->Save(fname);
}
//------------------------------------------------------------------------------
int vtkpxFemSolid::Load(const char* fname)
{
  int ok=this->InternalSolid->Load(fname);
  if (ok)
    this->Modified();

  return ok;
}
//------------------------------------------------------------------------------
PXAbaqusSolid* vtkpxFemSolid::GetInternalSolid()
{
  return this->InternalSolid;

}

PXAbaqusOutputData* vtkpxFemSolid::GetInternalFemData()
{
  return this->InternalSolid->getfemdata();
}
//------------------------------------------------------------------------------
void vtkpxFemSolid::Copy(vtkpxFemSolid* other)
{
  this->InternalSolid->Copy(other->GetInternalSolid());
  this->Modified();
}
//------------------------------------------------------------------------------
int vtkpxFemSolid::CreateFemData()
{
  this->InternalSolid->allocateFemData();
  return 1;
}

int vtkpxFemSolid::LoadFemData(const char* line)
{
  return this->InternalSolid->getfemdata()->Load(line);
}

int vtkpxFemSolid::SaveFemData(const char* line)
{ 
  return this->InternalSolid->getfemdata()->Save(line);
}

float vtkpxFemSolid::GetDisplacement(int node,int comp)
{
  if (this->InternalSolid->getfemdata()!=NULL)
    return this->InternalSolid->getfemdata()->getdispcomponent(node,comp);
  
  return -1.0;
}

float vtkpxFemSolid::GetStrain(int node,int comp)
{
  if (this->InternalSolid->getfemdata()!=NULL)
    return this->InternalSolid->getfemdata()->getstraincomponent(node,comp);
 
  return 0.0;
}

//------------------------------------------------------------------------------
int vtkpxFemSolid::GenerateHexahedralSolid(vtkpxTriangulatedStackSource* endo,
					    vtkpxTriangulatedStackSource* epi,
					    int nodes,int totalnumstacks,int bias)
{
  return this->GenerateHexahedralSolid(endo,epi,nodes,totalnumstacks,bias,1.0,0);
}
//------------------------------------------------------------------------------
int vtkpxFemSolid::GenerateHexahedralSolid(vtkpxTriangulatedStackSource* endo,
					   vtkpxTriangulatedStackSource* epi,
					   int nodes,int totalnumstacks,int bias,float sample,
					   int skipslices)
{
  if (endo==NULL || epi == NULL )
    return 0;

  PXAbaqusSolid::debugmode=0;

  PXAbaqusSolid* temp=new PXAbaqusSolid(endo->GetTStack(),epi->GetTStack());
  int ok=temp->generateHexahedralSolid(nodes,totalnumstacks,bias,sample,skipslices);
  temp->allocateMaterial();
  temp->allocateSpringSpace(1);
  PXAbaqusMaterialModel* mat=temp->getmaterial();
  mat->setElastic();
  temp->setSpringParameters(1,1.0);
  
  if (ok==0)
    {
      delete temp;
      return 0;
    }
  
  delete this->InternalSolid;
  this->InternalSolid=temp;
  this->Modified();
  return 1;
}
//------------------------------------------------------------------------------
void vtkpxFemSolid::Execute()
{
  if (!this->InternalSolid->haselements() || !this->InternalSolid->hasnodes())
    {
      vtkSphereSource* sph=vtkSphereSource::New();
      sph->SetRadius(0.1);
      sph->Update();
      this->GetOutput()->ShallowCopy(sph->GetOutput());
      sph->Delete();
      return;
    }

  PXNeighboursList* elements=this->InternalSolid->getelements();
  PXSolidNodes* nodes=this->InternalSolid->getnodes();

  int nel=elements->getnumnodes();
  int nump=nodes->getnumnodes();
  
  vtkPoints *points = vtkPoints::New();
  points->SetNumberOfPoints(nump);
  
  int doscalars=0,dowarp=0;
  PXAbaqusOutputData* femdata=this->InternalSolid->getfemdata();

  if ( this->InternalSolid->hasfemdata()==1 && this->ColorMode>0)
    doscalars=1;
 
  if ( this->InternalSolid->hasfemdata()==1 && this->WarpMode>0)
    dowarp=1;


  vtkCellArray *polys = vtkCellArray::New();
  vtkUnsignedCharArray* col=vtkUnsignedCharArray::New();

  if (doscalars)
    {
      col->SetNumberOfComponents(3);
      col->Allocate(nel*6,20);
    }
  

  if (dowarp==0)
    {
      for(int i=0; i<nump; i++)
	points->SetPoint(i,nodes->getpoint(i)->x[0], nodes->getpoint(i)->x[1], nodes->getpoint(i)->x[2]);	       
    }
  else
    {
      for(int i=0; i<nump; i++)
	points->SetPoint(i,
			 nodes->getpoint(i)->x[0]+femdata->getdispcomponent(i,0), 
			 nodes->getpoint(i)->x[1]+femdata->getdispcomponent(i,1), 
			 nodes->getpoint(i)->x[2]+femdata->getdispcomponent(i,1));	       
    }

  
  elements->Rewind();
  int cindex=0;
  for(int e=0; e<nel; e++)
    {
      PXIntegerList* intList=((elements->getcurrentnode())->points);
      float c[3];
      int numcolors=0;
      
      if (doscalars)
	{
	  float val=femdata->getstraincomponent(e,this->StrainMode);
	  if (this->ColorMode ==1 || this->ColorMode == 2)
	    val=float(e);
	  this->LookupColorMode(c,val,this->ColorMode);
	}
	 
      intList->Rewind();
      if (intList->getnumnodes()==8)
	{
	  vtkIdType pt[4],index[8];
	  for (int ia=0;ia<8;ia++)
	    {
	      index[ia]=intList->getcurrent();  
	      intList->Next(); 
	    }
	  pt[0]=index[0]; pt[1]=index[3]; pt[2]=index[7]; pt[3]=index[4]; polys->InsertNextCell(4,pt);
	  pt[0]=index[1]; pt[1]=index[2]; pt[2]=index[6]; pt[3]=index[5]; polys->InsertNextCell(4,pt);
	  pt[0]=index[0]; pt[1]=index[1]; pt[2]=index[2]; pt[3]=index[3]; polys->InsertNextCell(4,pt);
	  pt[0]=index[4]; pt[1]=index[5]; pt[2]=index[6]; pt[3]=index[7]; polys->InsertNextCell(4,pt);
	  pt[0]=index[0]; pt[1]=index[1]; pt[2]=index[5]; pt[3]=index[4]; polys->InsertNextCell(4,pt);
	  pt[0]=index[2]; pt[1]=index[3]; pt[2]=index[7]; pt[3]=index[6]; polys->InsertNextCell(4,pt);
	  numcolors=6;
	}
      else
	{
	  vtkIdType pt[3],index[4];
	  for (int ia=0;ia<4;ia++)
	    {
	      index[ia]=intList->getcurrent();  
	      intList->Next(); 
	    }
	  
	  /* the twelve triangles */
	  pt[0]  = index[0]; pt[1]  = index[1]; pt[2]  = index[2];  polys->InsertNextCell(3,pt);
	  pt[0]  = index[0]; pt[1]  = index[1]; pt[2]  = index[3];  polys->InsertNextCell(3,pt);
	  pt[0]  = index[0]; pt[1]  = index[2]; pt[2]  = index[3];  polys->InsertNextCell(3,pt);
	  pt[0]  = index[1]; pt[1]  = index[2]; pt[2]  = index[3];  polys->InsertNextCell(3,pt);
	  numcolors=4;
	}
      

      if (doscalars)
	{
	  for (int ia=0;ia<numcolors;ia++)
	    col->InsertNextTuple3(c[0],c[1],c[2]);
	  ++cindex;
	}
      elements->Next();
    }
  
  vtkPolyData *output = this->GetOutput();
  output->SetPoints(points);
  points->Delete();
  output->SetPolys(polys);
  polys->Delete();

  if (doscalars)
    output->GetCellData()->SetScalars(col);
  col->Delete();

}
//------------------------------------------------------------------------------
void vtkpxFemSolid::LookupColorMode(float t[3],float val,int mode)
{
  float x[3];
  /*

  Modes

  1 = Pies BW
  2 = Pies
  3 = Strain Peng-Cheng
  4 = Strain Threshold
  5,6 = Strain HSV

  */


  if (mode==1 || mode ==2)
    {
      int element=int(val);
      int se=this->GetInternalFemData()->getnumsectors(); 
      int nt=this->GetInternalFemData()->getnumtransmural();
      int pie=this->GetInternalFemData()->getpieindex(element);

      int slno=pie/(se*nt);
      int odd=1;
      if (2*int(slno/2)==slno)
	odd=0;
      
      int pi=pie-slno*se;
      int pi2=Icyclic(pi,3);

      if (odd==0)
	{
	  if (mode==2)
	    {
	      if (pi==0)
		{
		  x[0]=0.7; x[1]=0.7; x[2]=0.7;
		}
	      else if (pi==se-1)
		{
		  x[0]=0.2; x[1]=0.2; x[2]=0.2; 
		}
	      else
		{
		  switch(pi2)
		    {
		    case 0:
		      x[0]=0.2; x[1]=0.9; x[2]=0.9; break;
		    case 1:
		      x[0]=0.95; x[1]=1.0; x[2]=0.5; break;
		    case 2:
		      x[0]=1.0; x[1]=0.5; x[2]=0.2; break;
		    }
		}
	    }
	  else
	    {
	      switch (pi2)
		{
		case 0:
		  x[0]=0.35; x[1]=0.35; x[2]=0.35; break;
		case 1:
		  x[0]=0.85; x[1]=0.85; x[2]=0.85; break;
		case 2:
		  x[0]=0.65; x[1]=0.65; x[2]=0.65; break;
		  
		}
	    }
	}
      else
	{
	  if (mode==2)
	    {
	      if (pi==0)
		{
		  x[0]=0.9; x[1]=0.9; x[2]=0.9;
		}
	      else if (pi==se-1)
		{
		  x[0]=0.2; x[1]=0.1; x[2]=0.1;
		}
	      else
		{
		  switch(pi2)
		    {
		    case 0:
		      x[0]=1.0; x[1]=0.5; x[2]=0.75; break;
		    case 1:
		      x[0]=0.5; x[1]=1.0; x[2]=0.78; break;
		    case 2:
		      x[0]=1.0; x[1]=0.7; x[2]=0.4; break;
		    }
		}
	    }
	  else
	    {
	      switch(pi2)
		{
		case 1:
		  x[0]=0.3; x[1]=0.3; x[2]=0.3; break;
		case 2:
		  x[0]=0.8; x[1]=0.8; x[2]=0.8; break;
		case 0:
		  x[0]=0.5; x[1]=0.5; x[2]=0.5; break;
		}
	    }
	}
    }
  else if (mode==3)
    {
      // Standard Peng-Cheng Color Scheme
      if(val > 0.325)       x[0]=(0.75);
      else if(val > 0.275)  x[0]=(1.00);	
      else if(val > 0.225)  x[0]=(1.00);
      else if(val > 0.175)  x[0]=(1.00);	
      else if(val > 0.125)  x[0]=(1.00);	
      else if(val > 0.075)  x[0]=(1.00);	
      else if(val > 0.025)  x[0]=(1.00);	
      else if(val > -0.025) x[0]=(1.00);	
      else if(val > -0.075) x[0]=(0.80);	
      else if(val > -0.125) x[0]=(0.40);	
      else if(val > -0.175) x[0]=(0.0);	
      else if(val > -0.225) x[0]=(0.60);	
      else if(val > -0.275) x[0]=(0.20);	
      else if(val > -0.325) x[0]=(0.0);	
      else                  x[0]=(0.0);	
      
      if(val > 0.325)       x[1]=(0.0);
      else if(val > 0.275)  x[1]=(0.2);	
      else if(val > 0.225)  x[1]=(0.5);
      else if(val > 0.175)  x[1]=(0.8);	
      else if(val > 0.125)  x[1]=(0.8);	
      else if(val > 0.075)  x[1]=(1.00);	
      else if(val > 0.025)  x[1]=(1.00);	
      else if(val > -0.025) x[1]=(1.00);	
      else if(val > -0.075) x[1]=(1.00);	
      else if(val > -0.125) x[1]=(1.00);	
      else if(val > -0.175) x[1]=(0.5);	
      else if(val > -0.225) x[1]=(1.00);	
      else if(val > -0.275) x[1]=(0.50);	
      else if(val > -0.325) x[1]=(0.0);	
      else                  x[1]=(0.0);	
      
      if(val > 0.325)       x[2]=(0.15);
      else if(val > 0.275)  x[2]=(0.3);	
      else if(val > 0.225)  x[2]=(0.55);
      else if(val > 0.175)  x[2]=(0.8);	
      else if(val > 0.125)  x[2]=(0.0);	
      else if(val > 0.075)  x[2]=(0.00);	
      else if(val > 0.025)  x[2]=(0.75);	
      else if(val > -0.025) x[2]=(1.00);	
      else if(val > -0.075) x[2]=(0.6);	
      else if(val > -0.125) x[2]=(0.2);	
      else if(val > -0.175) x[2]=(0.0);	
      else if(val > -0.225) x[2]=(1.00);	
      else if(val > -0.275) x[2]=(1.0);	
      else if(val > -0.325) x[2]=(1.0);	
      else                  x[2]=(0.5);	
      
    }
  else if (mode==4)
    {
      float v=0.0;
      if (val>0.3)
	v=1.0;
      else if (val<=0.3 && val>=-0.3)
	v=(val+0.3)*(1.666667);
      else
	v=-1.0;
      
      for (int ia=0;ia<=2;ia++)
	x[ia]=v;
    }
  else if (mode==5 || mode==6)
    {
      
      float h=0.60,s=0.0;
      
      if (mode==4)
	{
	  // Spect Data
	  h=0.58;
	  s=Frange(val/100.0,0.0,1.0);
	}
      else
	{
	  h=0.60;
	  if (val>0.0)
	    h=0.00;
	  s=3.0*Frange(fabs(val),0.0,0.33);
	}
      
      float v=0.8;
      
      h*=6.0;
      int i=int(h);
      float aa,bb,cc,f,r=0,g=0,b=0;
      
      f=h-float(i);
      
      aa= v*(1.0-s);
      bb= v*(1.0- (s*f));
      cc= v*(1.0- (s* (1.0-f)));
      
      switch (i)
	{
	case 0: 
	  r=v; g=cc; b=aa; 
	  break;
	case 1: 
	  r=bb; g=v; b=aa;
	  break;
	case 2:
	  r=aa; g=v; b=cc;
	  break;
	case 3:
	  r=aa; g=bb; b=v;
	  break;
	case 4:
	  r=cc; g=aa; b=v;
	  break;
	case 5:
	  r=v;  g=aa; b=bb;
	  break;
	}
      x[0]=r;
      x[1]=g;
      x[2]=b;
    }
  else
    {
      x[0]=0.5;
      x[1]=0.5;
      x[2]=0.5;
    }
  
  for (int ia=0;ia<=2;ia++)
    t[ia]=(unsigned char)(254.0 * x[ia] +0.5);
}


// --------------------------------------------------------------


void vtkpxFemSolid::Scale(float s)
{
  PXSolidNodes* nodes=this->InternalSolid->getnodes();
  int nump=nodes->getnumnodes();

  for(int i=0; i<nump; i++)
    for (int ia=0;ia<=2;ia++)
      nodes->getpoint(i)->x[ia]=s*nodes->getpoint(i)->x[ia];
    
  if (this->InternalSolid->hasstacks())
    {
      this->InternalSolid->getstack(0)->scalepoints(s,s,s);
      this->InternalSolid->getstack(1)->scalepoints(s,s,s);
    }
}

int vtkpxFemSolid::GetNumberOfNodes()
{
 PXSolidNodes* nodes=this->InternalSolid->getnodes();
 return nodes->getnumnodes();

}

int vtkpxFemSolid::GetNumberOfElements()
{
  PXNeighboursList* elements=this->InternalSolid->getelements();
  return elements->getnumnodes();
}

// --------------------------------------------------------------
//  Reslice into pies
// --------------------------------------------------------------

int vtkpxFemSolid::ReslicePies(vtkCollection* solidCollection,int numframes,
			       int numslices,int numsectors,int numtransmural,int anti,
			       float ref[3])
{
  PXAbaqusSolid* basesolid=((vtkpxFemSolid*)solidCollection->GetItemAsObject(0))->GetInternalSolid();
  if (basesolid->hasfemdata()==PXFalse)
    {
      fprintf(stderr,"No Data!\n");
      return 0;
    }

  numslices=Irange(numslices,1,100);
  numsectors=Irange(numsectors,1,100);
  numtransmural=Irange(numtransmural,1,3);

  fprintf(stderr,"++++++++++++++ Subdivide Pies (anti=%d) numslices=%d numsectors=%d numt=%d\n",anti,
	  numslices,numsectors,numtransmural);
  fprintf(stderr,"++++++++++++++ Ref = (%f,%f,%f) \n",ref[0],ref[1],ref[2]);


  int numpies=numslices*numsectors*numtransmural;
  fprintf(stderr,"Numpies=%d nodes=%d elements=%d frames=%d\n",numpies,
	  basesolid->getnodes()->getnumnodes(),
	  basesolid->getelements()->getnumnodes(),
	  numframes);

  // Precompute some stuff 
  // Global Solid Stuff
  float globalminz=100000.0,globalmaxz=-1000000.0,cx=0.0,cy=0.0,tvol=0.0;
  int i=0;
  for (i=0;i<basesolid->getnodes()->getnumnodes();i++)
    {
      float z=basesolid->getnodes()->getpoint(i)->x[2];
      globalminz=Fmin(globalminz,z);
      globalmaxz=Fmax(globalmaxz,z);
    }

  for (i=0;i<basesolid->getelements()->getnumnodes();i++)
    {
      float v=basesolid->getfemdata()->getvolume(i);
      if (v<0.01)
	v=1.0;
      cx+=(basesolid->getfemdata()->getcentroid(i))->x[0]*v;
      cy+=(basesolid->getfemdata()->getcentroid(i))->x[1]*v;
      tvol+=v;
    }
  cx/=tvol; cy/=tvol;

  fprintf(stderr,"tvol=%.2f cx=%.2f cy=%.2f zrange=%.1f:%.1f\n",tvol,cx,cy,globalminz,globalmaxz);

  
  // For all elements stuff
  // ----------------------
  float* elementZmin=new float[basesolid->getelements()->getnumnodes()];
  float* elementZmax=new float[basesolid->getelements()->getnumnodes()];
  float* elementPmin=new float[basesolid->getelements()->getnumnodes()];
  float* elementPmax=new float[basesolid->getelements()->getnumnodes()];
  float* elementTmin=new float[basesolid->getelements()->getnumnodes()];
  float* elementTmax=new float[basesolid->getelements()->getnumnodes()];
  float *ave=new float[numpies];
  float *vol0=new float[numpies];

  float phase_ref=atan2(ref[1]-cy,ref[0]-cx)*180/M_PI;

  basesolid->getelements()->Rewind();
  for (int el=0;el<basesolid->getelements()->getnumnodes();el++)
    {
      PXAbaqusSolid::getElementRange(basesolid->getelements()->getcurrentnode()->points,
				     basesolid->getnodes(),
				     cx,cy,phase_ref,
				     elementZmin[el],elementZmax[el],elementPmin[el],elementPmax[el],
				     elementTmin[el],elementTmax[el]);
      basesolid->getelements()->Next();
    }
  
  
  int maxstrain=5;
  int orient=basesolid->getorientationmode();
  if (orient==5)
    maxstrain=2;
  
  for (int f=0;f<numframes;f++)
    {
      PXAbaqusOutputData* currentdata=((vtkpxFemSolid*)solidCollection->GetItemAsObject(f))->GetInternalFemData();
      currentdata->setPieStats(numpies,
			       numsectors,numslices,numtransmural);
      
      for (int strain=0;strain<=maxstrain;strain++)
	{
	  for (int j=0;j<numpies;j++)
	    {
	      ave[j]=0.0;
	      if (f==0)
		vol0[j]=0.0;
	    }

	  for (int el=0;el<basesolid->getelements()->getnumnodes();el++)
	    {
	      float elvol0=(basesolid->getfemdata())->getvolume(el);
	      int   bestpie=0;
	      float maxfr=0.0;

	      for (int slice=0;slice<numslices;slice++)
		{
		  float zfr=1.0;
		  if (numslices>1)
		    zfr=PXAbaqusSolid::returnElementFraction(elementZmin[el],elementZmax[el],
							     globalminz,globalmaxz,slice,numslices);
		  
		  for (int sector=0;sector<numsectors;sector++)
		    {
		      int realsector=(numsectors)-sector;
		      if (anti==1)
			realsector=sector+1;
		      
		      if (realsector==numsectors)
			realsector=0;
		      
		      float secfr=1.0;
		      if (numsectors>1)
			{
			  float elminangle= elementPmin[el];
			  float elmaxangle= elementPmax[el];
			  
			  if (sector<numsectors-1)
			    {
			      if (elminangle>elmaxangle)
				elminangle-=360.0;
			    }
			  else
			    {
			      if (elminangle>elmaxangle)
				elmaxangle+=360.0;
			    }
			  
			  secfr=PXAbaqusSolid::returnElementFraction(elminangle,elmaxangle,
								     0.0,360.0,
								     sector,numsectors);
			}
		      
		      for (int subsector=0;subsector<numtransmural;subsector++)
			{
			  float wedfr=1.0;
			  int   pie=(subsector+(realsector*numtransmural)+(slice*numsectors*numtransmural));
		      
			  if (numtransmural>1)
			    {
			      wedfr=PXAbaqusSolid::returnElementFraction(elementTmin[el],
									 elementTmax[el],
									 0,100,subsector,numtransmural);
			    }

			  float frac=secfr*zfr*wedfr;
			  
			  if (f==0)
			    vol0[pie]+=frac*elvol0;
			  
			  float stcomp=
			    currentdata->getstraincomponent(el,strain);
			  ave[pie]+=stcomp*frac*elvol0;

			  if (strain==0)
			    {
			      if (frac>maxfr)
				{
				  maxfr=frac;
				  bestpie=pie;

				}
			    }
			}
		    }
		}
	      if (strain==0)
		currentdata->setElementPieIndex(el,bestpie);
	    }
	  
	  for (int i=0;i<numpies;i++)
	    {
	      if (vol0[i]<0.001)
		ave[i]=0.0;
	      else
		ave[i]/=vol0[i];
	    }
	      
	  for (int el2=0;el2<basesolid->getelements()->getnumnodes();el2++)
	    {
	      int pindex=currentdata->getpieindex(el2);
	      currentdata->setstraincomponent(el2,strain+6,ave[pindex]);
	    }
	}
    }

  for (int f2=0;f2<numframes;f2++)
    ((vtkpxFemSolid*)solidCollection->GetItemAsObject(0))->Modified();
    
  delete [] ave;
  delete [] vol0;
  delete [] elementZmin;
  delete [] elementZmax;
  delete [] elementPmin;
  delete [] elementPmax;
  delete [] elementTmin;
  delete [] elementTmax;

  return 1;

}

// --------------------------------------------------------------

int vtkpxFemSolid::SavePies(const char* fname,vtkCollection* solidCollection,int numframes,
			    int numslices,int numsectors,int numtransmural,int anticlockwise,
			    float ref[3])
{
  PXAbaqusSolid* basesolid=((vtkpxFemSolid*)solidCollection->GetItemAsObject(0))->GetInternalSolid();
  if (basesolid->hasfemdata()==PXFalse)
    {
      fprintf(stderr,"No Data!\n");
      return 0;
    }


  numslices=Irange(numslices,1,100);
  numsectors=Irange(numsectors,1,100);
  numtransmural=Irange(numtransmural,1,3);

  int numpies=numslices*numsectors*numtransmural;
  fprintf(stderr,"Numpies=%d\n",numpies);
  fprintf(stderr,"++++++++++++++ Subdivide and Save in `%s' (anti=%d) \n",fname,anticlockwise);

  if (!basesolid->hasfemdata())
      return 0;

  FILE* fout=fopen(fname,"w");
  if (fout==NULL)
      return 0;


  fprintf(fout,"# Pie data \n");
  int orient=basesolid->getorientationmode();
  {
    if (orient==1 || orient==3)
      fprintf(fout,"# Strains defined in Cardiac Coordinate Space\n");
    else if (orient==5)
      fprintf(fout,"# Strains defined in Principal Directions\n");
    else
      fprintf(fout,"# Strains defined in Fibre-Specific Coordinate Space\n");
  }
  
  // Precompute some stuff 
  // Global Solid Stuff
  float globalminz=100000.0,globalmaxz=-1000000.0,cx=0.0,cy=0.0,tvol=0.0;
  {
    for (int i=0;i<basesolid->getnodes()->getnumnodes();i++)
      {
	float z=basesolid->getnodes()->getpoint(i)->x[2];
	globalminz=Fmin(globalminz,z);
	globalmaxz=Fmax(globalmaxz,z);
      }
    
    
    {
      for (int i=0;i<basesolid->getelements()->getnumnodes();i++)
	{
	  float v=basesolid->getfemdata()->getvolume(i);
	  if (v<0.01)
	    v=1.0;
	  cx+=(basesolid->getfemdata()->getcentroid(i))->x[0]*v;
	  cy+=(basesolid->getfemdata()->getcentroid(i))->x[1]*v;
	  tvol+=v;
	}
    }
    cx/=tvol; cy/=tvol;
    fprintf(fout,"# Total volume= %9.3f\n",tvol);
  }
  
  // For all elements stuff
  // ----------------------
  float* elementZmin=new float[basesolid->getelements()->getnumnodes()];
  float* elementZmax=new float[basesolid->getelements()->getnumnodes()];
  float* elementPmin=new float[basesolid->getelements()->getnumnodes()];
  float* elementPmax=new float[basesolid->getelements()->getnumnodes()];
  float* elementTmin=new float[basesolid->getelements()->getnumnodes()];
  float* elementTmax=new float[basesolid->getelements()->getnumnodes()];

  float phase_ref=atan2(ref[1]-cy,ref[0]-cx)*180/M_PI;

  fprintf(stderr,"Phase_ref=%f\n",phase_ref);
  

  fprintf(fout,"# LV-RV Junction Defined as (%6.2f,%6.2f,%6.2f)\n",ref[0],ref[1],ref[2]);
  fprintf(fout,"# Phase_ref =%6.3f degrees (cx=%6.2f,cy=%6.2f)\n",phase_ref,cx,cy);
  fprintf(fout,"# Number of slices=%d, sectors=%d\n",numslices,numsectors);

  basesolid->getelements()->Rewind();
  for (int el=0;el<basesolid->getelements()->getnumnodes();el++)
      {
	PXAbaqusSolid::getElementRange(basesolid->getelements()->getcurrentnode()->points,
				       basesolid->getnodes(),
				       cx,cy,phase_ref,
				       elementZmin[el],elementZmax[el],elementPmin[el],elementPmax[el],
				       elementTmin[el],elementTmax[el]);
	/*	if (10*int(el/10)==el)
	    fprintf(stderr,"Element el=%d %f:%f\n",el,elementTmin[el],elementTmax[el]);*/
	basesolid->getelements()->Next();
      }
  
  float *piesmat=new float [numpies*numframes];
  float *ave=new float[numpies];
  float *vol=new float[numpies];
  float *vol0=new float[numpies];
  int numel=basesolid->getelements()->getnumnodes();

  int start=-1;
  
  int maxstrain=5;
  if (orient==5)
      maxstrain=2;
  
  for (int strain=start;strain<=maxstrain;strain++)
      {
	for (int f=0;f<numframes;f++)
	    {
	      PXAbaqusOutputData* currentdata=((vtkpxFemSolid*)solidCollection->GetItemAsObject(f))->GetInternalFemData();
	      for (int j=0;j<numpies;j++)
		  {
		    ave[j]=0.0;
		    vol[j]=0.0;
		    if (f==0)
			vol0[j]=0.0;
		  }
	      
	      if (strain==start)
		  currentdata->setPieStats(numslices*numsectors*numtransmural,
							 numsectors*numtransmural,numslices);

	      for (int el=0;el<basesolid->getelements()->getnumnodes();el++)
		  {
		    float elvol0=(basesolid->getfemdata())->getvolume(el);
		    float elvol =currentdata->getvolume(el);
		    
		    int bestpie=0;
		    float maxfr=0.0;
		    for (int slice=0;slice<numslices;slice++)
			{
			  float zfr=1.0;
			  if (numslices>1)
			    zfr=PXAbaqusSolid::returnElementFraction(elementZmin[el],elementZmax[el],
								     globalminz,globalmaxz,slice,numslices);
			  
			  for (int sector=0;sector<numsectors;sector++)
			      {
				int realsector=(numsectors)-sector;
				if (anticlockwise==1)
				    realsector=sector+1;

				if (realsector==numsectors)
				    realsector=0;

				float secfr=1.0;
				if (numsectors>1)
				    {
				      float elminangle= elementPmin[el];
				      float elmaxangle= elementPmax[el];
				      
				      if (sector<numsectors-1)
					  {
					    if (elminangle>elmaxangle)
						elminangle-=360.0;
					  }
				      else
					  {
					    if (elminangle>elmaxangle)
						elmaxangle+=360.0;
					  }
				      
				      secfr=PXAbaqusSolid::returnElementFraction(elminangle,elmaxangle,
										 0.0,360.0,
										 sector,numsectors);
				    }
				for (int subsector=0;subsector<numtransmural;subsector++)
				    {
				      int   pie=subsector+realsector*numtransmural+
					  slice*numsectors*numtransmural;
				      
				      float wedfr=1.0;
				      if (numtransmural>1)
					  {
					    wedfr=PXAbaqusSolid::returnElementFraction(elementTmin[el],
										       elementTmax[el],
										       0,100,subsector,numtransmural);
					  }
				      float frac=secfr*zfr*wedfr;
				      
				      if (frac>maxfr)
					  {
					    maxfr=frac;
					    bestpie=pie;
					  }
				      vol[pie]+=frac*elvol;
				      if (f==0)
					  vol0[pie]+=frac*elvol0;
				      if (strain>=0)
					  {
					    float stcomp=
						currentdata->getstraincomponent(el,strain);
					    ave[pie]+=stcomp*frac*elvol0;
					  }
				    }
			      }
			}
		    if (strain==start)
			currentdata->setElementPieIndex(el,bestpie);
		  }
	      if (strain>=0)
		  {
		    for (int i=0;i<numpies;i++)
			{
			  if (vol0[i]<0.001)
			      ave[i]=0.0;
			  else
			      ave[i]/=vol0[i];
			}
		    for (int j=0;j<numpies;j++)
			piesmat[j*numframes+f]=ave[j];
		  }
	      else
		  {
		    for (int j=0;j<numpies;j++)
			piesmat[j*numframes+f]=vol[j];
		  } 
	    }
	fprintf(fout,"\n");
	
	if (orient<2 || orient==3 || strain<0)
	    {
	      switch(strain)
		  {
		  case -1:
		    fprintf(fout,"#\n\t\t Volume \n"); break;
		  case 0:
		    fprintf(fout,"#\n\t\t Radial Strain RR\n"); break;
		  case 1:
		    fprintf(fout,"#\n\t\t Circumferential Strain CC\n"); break;
		  case 2:
		    fprintf(fout,"#\n\t\t Longitudinal Strain LL\n"); break;
		  case 3:
		    fprintf(fout,"#\n\t\t Radial-Circumferential Shear Strain RC\n"); break;
		  case 4:
		    fprintf(fout,"#\n\t\t Radial-Longitudinal Strain RL\n"); break;
		  case 5:
		    fprintf(fout,"#\n\t\t Circumferential-Longitudinal Strain CL\n"); break;
		  }
	    }
	else if (orient==5)
	    {
	      fprintf(fout,"#\n\t\t Principal Strain %d\n",strain+1);
	    }
	else
	    {
	      switch(strain)
		  {
		  case 0:
		    fprintf(fout,"#\n\t\t Circumferential Cross-Fibre Strain XX\n"); break;
		  case 1:
		    fprintf(fout,"#\n\t\t Radial Strain RR\n"); break;
		  case 2:
		    fprintf(fout,"#\n\t\t Fibre Strain FF\n"); break;
		  case 3:
		    fprintf(fout,"#\n\t\t Circumferential-Radial Shear Strain XR\n"); break;
		  case 4:
		    fprintf(fout,"#\n\t\t Circumferential-Fibre Strain XF\n"); break;
		  case 5:
		    fprintf(fout,"#\n\t\t Radial-Fibre Strain RF\n"); break;
		  }
	    }
	
	fprintf(fout,"Pie/Frame ");
	{
	  for (int f=0;f<numframes;f++)
	      fprintf(fout," %7d",f+1);
	}
	fprintf(fout,"\n");
	int minj=0;
	if (basesolid->getnodes()->getnumnodes()<100)
	    minj=2;
	for (int j=minj;j<numpies;j++)
	    {
	      fprintf(fout,"  %7d   ",j+1);
	      for (int f=0;f<numframes;f++)
		  if (strain==-1)
		      fprintf(fout," %6.1f ",piesmat[j*numframes+f]);
		  else
		      fprintf(fout," %6.3f ",piesmat[j*numframes+f]);
	      fprintf(fout,"\n");
	    }
      }
  
  if (basesolid->getnodes()->getnumnodes()<100)
      {
	fprintf(fout, "\n\nPie division for each elements\nElement\tPie\n");
	for (int i=0;i<numel;i++)
	    {
	      int p=basesolid->getfemdata()->getpieindex(i);
	      fprintf(fout,"%d \t%d\n!",i+1,p);
	    }
      }
  
  fclose(fout);
  char line[500];
  sprintf(line,"%s.pieindex",fname);
  basesolid->getfemdata()->savePieDivision(line);

  delete [] piesmat;
  delete [] ave;
  delete [] vol;
  delete [] vol0;
  delete [] elementZmin;
  delete [] elementZmax;
  delete [] elementPmin;
  delete [] elementPmax;
  delete [] elementTmin;
  delete [] elementTmax;
  return 1;
}


int vtkpxFemSolid::ReorientStrainsToFiberOrientation(vtkImageData* e1xyzimage)
{
  
  if (e1xyzimage==NULL)
    {
      fprintf(stderr,"Bad Image\n");
      return 0;
    }

  if (this->InternalSolid->getfemdata()==NULL)
    {
      fprintf(stderr,"No displacement/strain data\n");
      return 0;
    }


  PXAbaqusOutputData* femData=this->InternalSolid->getfemdata();
  
  float** R	 = matrix(0, 2, 0, 2);
  float** R1	 = matrix(0, 2, 0, 2);
  float** R2	 = matrix(0, 2, 0, 2);
  float** E	 = matrix(0, 2, 0, 2);
  float** E2	 = matrix(0, 2, 0, 2);

  // inverse = 1
  fprintf(stderr,"Rotating strains Cardiac --> Fibre \n");
  

  PXNeighboursList* elements=this->InternalSolid->getelements();
  PXSolidNodes*    nodes   =this->InternalSolid->getnodes();   

  elements->Rewind();
  for (int el=0;el<elements->getnumnodes();el++)
      {
	PXIntegerList* intList=((elements->getcurrentnode())->points);
	elements->Next();

	float cx,cy,cz;
	int layer=0;
	this->InternalSolid->getElementCentroid(intList,cx,cy,cz);
	for (int ka=0;ka<intList->getnumnodes();ka++)
	  {
	      layer+=nodes->getindex(intList->getcurrent(),0); 
	      intList->Next();
	    }
	layer=layer/intList->getnumnodes();

	float x1,x2,x3,y1,y2,y3,z1,z2,z3;
	
	intList->Rewind();
	this->InternalSolid->getdirections(intList,
					   x2,y2,z2,x3,y3,z3,x1,y1,z1,0);
	
	R1[0][0]=x2; R1[1][0]=y2; R1[2][0]=z2; // Radial
	R1[0][1]=x3; R1[1][1]=y3; R1[2][1]=z3; // Circ?
	R1[0][2]=x1; R1[1][2]=y1; R1[2][2]=z1; // Long?


	float angle=this->InternalSolid->getFibreAngle(cz,layer);
	this->InternalSolid->rotateFibreAngle(x3,y3,z3,  
					      x1,y1,z1,
					      x2,y2,z2, // Keep Radial Fixed
					      angle);
	
	
	double rad[3]; rad[0]=x2; rad[1]=y2; rad[2]=z2;
	vtkMath::Normalize(rad);

	double oldf[3];  oldf[0]=x3; oldf[1]=y3; oldf[2]=z3;
	vtkMath::Normalize(oldf);

	// here get fiber orientations
	int img_index=e1xyzimage->FindPoint(cx,cy,cz);

	double fib[3];
	for (int ia=0;ia<=2;ia++)
	  fib[ia]=e1xyzimage->GetPointData()->GetScalars()->GetComponent(img_index,ia);
	
	vtkMath::Normalize(fib);
	
	fprintf(stderr,"Element = %d --> centroid (%.2f,%.2f,%.2f) --> fibres (%.2f,%.2f,%.2f) , oldf (%.2f,%.2f,%.2f)\n",
		el,cx,cy,cz,fib[0],fib[1],fib[2],oldf[0],oldf[1],oldf[2]);

	double cfib[3];
	vtkMath::Cross(fib,rad,cfib);
	vtkMath::Cross(cfib,fib,rad);
	
	fprintf(stderr,"\t\t Final --> fibres (%.2f,%.2f,%.2f) , radial (%.2f,%.2f,%.2f) cf (%.2f,%.2f,%.2f)\n",
		fib[0],fib[1],fib[2],rad[0],rad[1],rad[2],cfib[0],cfib[1],cfib[2]);
	
	
	R2[0][0]=rad[0]; R2[1][0]=rad[1]; R2[2][0]=rad[2]; // Cross
	R2[0][1]=cfib[0]; R2[1][1]=cfib[1]; R2[2][1]=cfib[2]; // Radial
	R2[0][2]=fib[0]; R2[1][2]=fib[1]; R2[2][2]=fib[2]; // Fibre
	
	for(int i=0; i<=2; i++)
	  for(int j=0; j<=2; j++)
	    {
	      R[i][j]=0.0;
	      for(int m=0; m<=2; m++)
		R[i][j] += R2[m][i] * R1[m][j];
	    }
      	
	E[0][0] = femData->getstraincomponent(el,0); 
	E[0][1] = femData->getstraincomponent(el,3); 
	E[0][2] = femData->getstraincomponent(el,4); 

	E[1][0] = femData->getstraincomponent(el,3); 
	E[1][1] = femData->getstraincomponent(el,1); 
	E[1][2] = femData->getstraincomponent(el,5); 

	E[2][0] = femData->getstraincomponent(el,4); 
	E[2][1] = femData->getstraincomponent(el,5);
	E[2][2] = femData->getstraincomponent(el,2); 

	
	// E2=R*E*R' --> E2(i,j)=R(i,k)*E(k,m)*R'(m,j)
	for (int i=0;i<=2;i++)
	  for (int j=0;j<=2;j++)
	    {
	      E2[i][j]=0.0;
	      for (int k=0;k<=2;k++)
		for (int m=0;m<=2;m++)
		  E2[i][j] += R[i][k]*E[k][m]*R[j][m];
	    }
	
	femData->setElementInfo(el,1,-1.0,
				E2[0][0],E2[1][1],E2[2][2],E2[0][1],E2[0][2],E2[1][2]);
	
	
	/*if (el==0)
	  {
	      fprintf(stderr," E= %5.2f %5.2f %5.2f\n    %5.2f, %5.2f %5.2f\n    %5.2f %5.2f %5.2f\n\n",
		       E[0][0], E[0][1], E[0][2], E[1][0], E[1][1], E[1][2], E[2][0], E[2][1], E[2][2]);
	      fprintf(stderr,"E2= %5.2f %5.2f %5.2f\n    %5.2f, %5.2f %5.2f\n    %5.2f %5.2f %5.2f\n\n",
		      E2[0][0],E2[0][1],E2[0][2],E2[1][0],E2[1][1],E2[1][2],E2[2][0],E2[2][1],E2[2][2]);
	      E[0][0] = femData->getstraincomponent(el,0); 
	      E[0][1] = femData->getstraincomponent(el,3); 
	      E[0][2] = femData->getstraincomponent(el,4); 
	      
	      E[1][0] = femData->getstraincomponent(el,3); 
	      E[1][1] = femData->getstraincomponent(el,1); 
	      E[1][2] = femData->getstraincomponent(el,5); 
	      
	      E[2][0] = femData->getstraincomponent(el,4); 
	      E[2][1] = femData->getstraincomponent(el,5);
	      E[2][2] = femData->getstraincomponent(el,2); 
	      fprintf(stderr,"+E= %5.2f %5.2f %5.2f\n    %5.2f, %5.2f %5.2f\n    %5.2f %5.2f %5.2f\n\n",
		      E[0][0], E[0][1], E[0][2], E[1][0], E[1][1], E[1][2], E[2][0], E[2][1], E[2][2]);
	    }*/
	
	
      }

  free_matrix(R,0,2,0,2); free_matrix(R1,0,2,0,2); free_matrix(R2,0,2,0,2);
  free_matrix(E,0,2,0,2); free_matrix(E2,0,2,0,2);
  return 1;
}

vtkPolyData* vtkpxFemSolid::GenerateFibreMaps(vtkImageData* e1xyzimage,double scale)
{
  
  if (e1xyzimage==NULL)
    {
      fprintf(stderr,"Bad Image\n");
      return NULL;
    }

  PXNeighboursList* elements=this->InternalSolid->getelements();

  vtkPoints *c_points = vtkPoints::New();
  c_points->SetNumberOfPoints(elements->getnumnodes()*2);

  vtkCellArray *c_lines     = vtkCellArray::New();
  c_lines->Allocate(elements->getnumnodes(),10);

  elements->Rewind();
  for (int el=0;el<elements->getnumnodes();el++)
    {
      PXIntegerList* intList=((elements->getcurrentnode())->points);
      elements->Next();
      
      float cx,cy,cz;

      this->InternalSolid->getElementCentroid(intList,cx,cy,cz);
	
      // here get fiber orientations
      int img_index=e1xyzimage->FindPoint(cx,cy,cz);
      
      double fib[3];
      for (int ia=0;ia<=2;ia++)
	fib[ia]=e1xyzimage->GetPointData()->GetScalars()->GetComponent(img_index,ia);
      
      vtkMath::Normalize(fib);
	
	
      c_points->SetPoint(el*2,cx,cy,cz);
      c_points->SetPoint(el*2+1,cx+scale*fib[0],cy+scale*fib[1],cz+scale*fib[2]);

      vtkIdType pt[2];
      pt[0]=el*2;
      pt[1]=el*2+1;
      c_lines->InsertNextCell(2,pt);
    }

  vtkPolyData* sur=vtkPolyData::New();
  sur->SetPoints(c_points);
  sur->SetLines(c_lines);
  c_points->Delete();
  c_lines->Delete();

  return sur;
}

// -------------------------------------------------------------------------------------

vtkPolyData* vtkpxFemSolid::GenerateInternalFibreMaps(double scale)
{



  PXNeighboursList* elements=this->InternalSolid->getelements();
  PXSolidNodes* nodes=this->InternalSolid->getnodes();

  vtkPoints *c_points = vtkPoints::New();
  c_points->SetNumberOfPoints(elements->getnumnodes()*2);

  vtkCellArray *c_lines     = vtkCellArray::New();
  c_lines->Allocate(elements->getnumnodes(),10);

  elements->Rewind();
  for (int el=0;el<elements->getnumnodes();el++)
    {
      PXIntegerList* intList=((elements->getcurrentnode())->points);
      elements->Next();
      
      float cx,cy,cz;
      this->InternalSolid->getElementCentroid(intList,cx,cy,cz);
      int layer=0;
      for (int ka=0;ka<intList->getnumnodes();ka++)
	{
	  layer+=nodes->getindex(intList->getcurrent(),0); 
	  intList->Next();
	}
      layer=layer/intList->getnumnodes();
      
      float x1,x2,x3,y1,y2,y3,z1,z2,z3;
	
      intList->Rewind();
      this->InternalSolid->getdirections(intList,x2,y2,z2,x3,y3,z3,x1,y1,z1,0);
      	
      double fib[3]; fib[0]=x3; fib[1]=y3;fib[2]=z3;
      vtkMath::Normalize(fib);

      c_points->SetPoint(el*2,cx,cy,cz);
      c_points->SetPoint(el*2+1,cx+scale*fib[0],cy+scale*fib[1],cz+scale*fib[2]);

      vtkIdType pt[2];
      pt[0]=el*2;
      pt[1]=el*2+1;
      c_lines->InsertNextCell(2,pt);
    }

  vtkPolyData* sur=vtkPolyData::New();
  sur->SetPoints(c_points);
  sur->SetLines(c_lines);
  c_points->Delete();
  c_lines->Delete();

  return sur;
}


