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
  Module:    $RCSfile: vtkpxShapeTracking.cpp,v $
  Language:  C++
  Date:      $Date: 2002/04/01 14:18:13 $
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
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkpxShapeTracking.h"
#include "vtkFloatArray.h"
#include "vtkpxSurfaceUtil.h"
#include "pxshapetrackingclass.h"
#include "vtkpxTriangulatedStackSource.h"
#include "vtkIterativeClosestPointTransform.h"
#include "vtkLandmarkTransform.h"
#include "vtkMath.h"
#include "vtkpxUtil.h"
#include "vtkPointLocator.h"
//------------------------------------------------------------------------------
vtkpxShapeTracking* vtkpxShapeTracking::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxShapeTracking");
  if(ret)
    {
    return (vtkpxShapeTracking*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxShapeTracking;
}
//------------------------------------------------------------------------------
vtkpxShapeTracking::vtkpxShapeTracking()
{
  this->Displacements=vtkFloatArray::New();
  this->Displacements->SetNumberOfComponents(3);
  this->Confidences=vtkFloatArray::New();
  this->Confidences->SetNumberOfComponents(1);
  this->Flags=vtkIntArray::New();
  this->Flags->SetNumberOfComponents(3);

  this->Mode=4;
  this->SearchRadius=0;
  this->ReferenceSurface=NULL;
  this->TargetSurface=NULL;
  this->InterpolateIterations=10;
  this->InterpolateThreshold=0.1;
  this->InitialMode=VTK_LANDMARK_AFFINE;
}
//------------------------------------------------------------------------------
vtkpxShapeTracking::~vtkpxShapeTracking()
{
  this->Displacements->Delete();
  this->Confidences->Delete();
  this->Flags->Delete();
}
//------------------------------------------------------------------------------
float vtkpxShapeTracking::GetDisplacement(int node,int comp)
{
  if (this->Displacements==NULL)
      return 0;

  comp=Irange(comp,0,this->Displacements->GetNumberOfComponents()-1);
  node=Irange(node,0,this->Displacements->GetNumberOfTuples()-1);
  return this->Displacements->GetComponent(node,comp);
}
//------------------------------------------------------------------------------
float vtkpxShapeTracking::GetConfidence(int node)
{
  if (this->Confidences==NULL)
      return 0;

  node=Irange(node,0,this->Confidences->GetNumberOfTuples()-1);
  return this->Confidences->GetComponent(node,0);
}
//------------------------------------------------------------------------------
void vtkpxShapeTracking::Execute()
{
  if (this->ReferenceSurface==NULL || this->TargetSurface==NULL)
    {
      vtkErrorMacro(<< "Bad Inputs\n");
      return;
    }

  vtkPolyData* output=this->GetOutput();
  output->DeepCopy(this->ReferenceSurface);

  vtkPolyData* tmp=vtkPolyData::New();


  switch (this->Mode)
    {
    case 1:
      this->ComputeSymmetricNearestNeighbor(this->ReferenceSurface,this->TargetSurface,
					    this->Displacements,this->Flags);
      break;
    case 2:
      this->ComputeAffineMatch(this->ReferenceSurface,this->TargetSurface,this->Displacements);
      tmp->DeepCopy(this->ReferenceSurface);
      vtkpxSurfaceUtil::ShiftPointsByDisplacements(tmp,this->Displacements,1.0);
      this->ComputeNearestNeighbor(tmp,this->TargetSurface,this->Displacements,this->Flags);
      vtkpxSurfaceUtil::AddToDisplacements(this->ReferenceSurface,tmp,this->Displacements);
      break;
    case 3:
      this->ComputeAffineMatch(this->ReferenceSurface,this->TargetSurface,this->Displacements);
      tmp->DeepCopy(this->ReferenceSurface);
      vtkpxSurfaceUtil::ShiftPointsByDisplacements(tmp,this->Displacements,1.0);
      this->ComputeSymmetricNearestNeighbor(tmp,this->TargetSurface,this->Displacements,this->Flags);
      vtkpxSurfaceUtil::AddToDisplacements(this->ReferenceSurface,tmp,this->Displacements);
      break;

    default:
      this->ComputeNearestNeighbor(this->ReferenceSurface,this->TargetSurface,
				   this->Displacements,this->Flags);
      break;

    }

  this->Confidences->SetNumberOfTuples(this->Displacements->GetNumberOfTuples());
  this->Confidences->FillComponent(0,1.0);

  if (this->SearchRadius>1)
    this->DoCurvatureTracking(this->ReferenceSurface,this->TargetSurface,
			      this->Displacements,this->Flags,this->Confidences);

  vtkpxSurfaceUtil::ShiftPointsByDisplacements(output,this->Displacements,1.0);
  tmp->Delete();

}
//------------------------------------------------------------------------------
int vtkpxShapeTracking::Save(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (fout==NULL)
    return 0;

  fprintf(fout,"#PXShapeTrackingClass numstacks,hasdisplacements,numpoints\n");
  fprintf(fout,"0\n1\n%d\n",this->Displacements->GetNumberOfTuples());
  fprintf(fout,"#-----------------Displacements-------------------------\n");
  for (int i=0;i<this->Displacements->GetNumberOfTuples();i++)
    fprintf(fout,"%6.3f %6.3f %6.3f %5.3f %.0f %.0f %.0f\n",
	     this->Displacements->GetComponent(i,0),
	     this->Displacements->GetComponent(i,1),
	     this->Displacements->GetComponent(i,2),
	     this->Confidences->GetComponent(i,0),
	     this->Flags->GetComponent(i,0),
	     this->Flags->GetComponent(i,1),
	     this->Flags->GetComponent(i,2));
      
  fclose(fout);
  return 1;
}
// --------------------------------------------------------------------------
int vtkpxShapeTracking::Load(const char* fname)
{
  gzFile fin=gzsuffixopen(fname,"r");
  if (!fin)
    return 0;

  char line[200];
  gzgets(fin,line,200);
  if (gzfindstringinstring(line,"#PXShapeTrackingClass")==0)
    return 0;
  
  gzgets(fin,line,200);
  gzgets(fin,line,200);
  int num=0;
  gzgets(fin,line,200);  sscanf(line,"%d",&num);

  this->Displacements->SetNumberOfComponents(3);
  this->Displacements->SetNumberOfTuples(num);
  this->Confidences->SetNumberOfComponents(1);
  this->Confidences->SetNumberOfTuples(num);
  this->Flags->SetNumberOfComponents(3);
  this->Flags->SetNumberOfTuples(num);
  gzgets(fin,line,200);
  for (int ia=0;ia<num;ia++)
    {
      gzgets(fin,line,200);
      float x[3],c,f[3];
      sscanf(line,"%f %f %f %f %f %f %f",&x[0],&x[1],&x[2],&c,&f[0],&f[1],&f[2]);
      this->Displacements->SetTuple(ia,&x[0]);
      this->Confidences->SetComponent(ia,0,c);
      this->Flags->SetTuple(ia,&f[0]);
    }
  gzclose(fin);
  return 1;
}
//------------------------------------------------------------------------------
// Using Old Code
//------------------------------------------------------------------------------

void vtkpxShapeTracking::OldStyleShapeTracking(vtkpxTriangulatedStackSource* st1,
					       vtkpxTriangulatedStackSource* st2,
					       int searchwindow,
					       vtkFloatArray *displacements,
					       const char* fname)
{
  if (displacements!=NULL)
    vtkpxShapeTracking::OldStyleShapeTracking(st1,st2,displacements,searchwindow);
  else if (fname!=NULL)
    vtkpxShapeTracking::OldStyleShapeTracking(st1,st2,fname,searchwindow);
}
//------------------------------------------------------------------------------
void vtkpxShapeTracking::OldStyleShapeTracking(vtkpxTriangulatedStackSource* st1,
					       vtkpxTriangulatedStackSource* st2,
					       vtkFloatArray *displ,
					       int searchwindow)
{
  PXShapeTrackingClass* shapeTr=new PXShapeTrackingClass(st1->GetTStack(),
							 st2->GetTStack());
  shapeTr->findSmoothMap(1,5,5,1);
  if (searchwindow>0)
    shapeTr->initialCurvatureTracking(searchwindow);
  
  int n=st1->GetTStack()->getnumpoints();
  
  displ->SetNumberOfComponents(3);
  displ->SetNumberOfTuples(n);
  
  for (int i=0;i<n;i++)
    {
      PX3Vector* vec=shapeTr->getdisplacement(i);
      displ->SetTuple(i,vec->x);
    }
  
  delete shapeTr;
}
//------------------------------------------------------------------------------
int vtkpxShapeTracking::OldStyleShapeTracking(vtkpxTriangulatedStackSource* st1,
					      vtkpxTriangulatedStackSource* st2,
					      const char *filename,
					      int searchwindow,int subdivide)
{



  PXShapeTrackingClass* shapeTr=new PXShapeTrackingClass(st1->GetTStack(),
							 st2->GetTStack());
  shapeTr->findSmoothMap(1,5,5,subdivide);
  if (searchwindow>0)
    shapeTr->initialCurvatureTracking(searchwindow);
  int ok=shapeTr->Save(filename,1);
  delete shapeTr;
  return ok;
}

int vtkpxShapeTracking::OldStyleShapeTracking(const char* name1,const char* name2,
					       char *outfilename,
					       int searchwindow,
					       int subdivide)
{

  PXShapeTrackingClass* shapeTr=new PXShapeTrackingClass(name1,name2);
  shapeTr->findSmoothMap(1,5,5,subdivide);
  if (searchwindow>0)
    shapeTr->initialCurvatureTracking(searchwindow);
  int ok=shapeTr->Save(outfilename,1);
  delete shapeTr;
  return ok;
}
//------------------------------------------------------------------------------
//
//
//                 New Implementations of Old Code
//
//
//------------------------------------------------------------------------------
void vtkpxShapeTracking::InitializeFlags(vtkPolyData* poly,vtkIntArray* flags)
{
  int num=poly->GetNumberOfPoints();
  flags->SetNumberOfComponents(3);
  flags->SetNumberOfTuples(num);
  flags->FillComponent(0,0);
  flags->FillComponent(1,0);
  flags->FillComponent(2,-1);
}
//------------------------------------------------------------------------------
void vtkpxShapeTracking::ComputeNearestNeighbor(vtkPolyData* ref,vtkPolyData* targ,
						vtkFloatArray* displ,vtkIntArray* flags)
{
  vtkpxSurfaceUtil::ZeroAllDisplacements(ref,displ);
  this->InitializeFlags(ref,flags);

  vtkPointLocator* locator=vtkPointLocator::New();
  locator->SetDataSet(targ);
  locator->BuildLocator();

  vtkPoints* points=ref->GetPoints();
  vtkPoints* points2=targ->GetPoints();
  int nump=points->GetNumberOfPoints();
  fprintf(stderr,"Tuples=%d (%d) Comp=%d\n",displ->GetNumberOfTuples(),nump,displ->GetNumberOfComponents());

  for (int ia=0;ia<nump;ia++)
    {
      double x[3],tx[3];
      points->GetPoint(ia,x);
      int id=locator->FindClosestPoint(x);
      points2->GetPoint(id,tx);
      for (int ib=0;ib<=2;ib++)
	displ->SetComponent(ia,ib,tx[ib]-x[ib]);
      flags->SetComponent(ia,0,0);
      flags->SetComponent(ia,1,1);
      flags->SetComponent(ia,2,id);
      
      if (ia<5)
	fprintf(stderr,"x=%.1f,%.1f,%.1f tx=%.1f,%.1f,%.1f  d=%.3f %.3f %.3f\n",
		x[0],x[1],x[2],tx[0],tx[1],tx[2],
		displ->GetComponent(ia,0),
		displ->GetComponent(ia,1),
		displ->GetComponent(ia,2));
    }
  
  locator->Delete();
}
//------------------------------------------------------------------------------
void vtkpxShapeTracking::ComputeSymmetricNearestNeighbor(vtkPolyData* ref,vtkPolyData* targ,
							 vtkFloatArray* displ,
							 vtkIntArray* flags)


{
  vtkpxSurfaceUtil::ZeroAllDisplacements(ref,displ);
  this->InitializeFlags(ref,flags);

  vtkPoints* points=ref->GetPoints();
  vtkPoints* points2=targ->GetPoints();

  vtkPointLocator* locator=vtkPointLocator::New();
  locator->SetDataSet(targ);
  locator->BuildLocator();

  vtkPointLocator* locator2=vtkPointLocator::New();
  locator2->SetDataSet(ref);
  locator2->BuildLocator();
  
  int nump=points->GetNumberOfPoints();

  int numgood=0;

  for (int ia=0;ia<nump;ia++)
    {
      double x[3],tx[3],x2[3];
      points->GetPoint(ia,x);

      int id1=locator->FindClosestPoint(x);
      points2->GetPoint(id1,tx);
      int id2=locator2->FindClosestPoint(tx);
      points->GetPoint(id2,x2);
      
      if (id2==ia)
	{
	  flags->SetComponent(ia,0,1);
	  flags->SetComponent(ia,1,1);
	  flags->SetComponent(ia,2,id1);
	  for (int ib=0;ib<=2;ib++)
	    displ->SetComponent(ia,ib,tx[ib]-x[ib]);
	  ++numgood;
	}
    }

  
  fprintf(stderr,"Done with initial symmetric neighbor numpoints=%d\n",numgood);

  vtkpxShapeTracking::InterpolateSymmetricNearestNeighbor(ref,targ,displ,
							  flags,locator,
							  this->InterpolateIterations,
							  this->InterpolateThreshold);

  locator->Delete();
  locator2->Delete();
}

//------------------------------------------------------------------------------
void vtkpxShapeTracking::ComputeAffineMatch(vtkPolyData* ref,vtkPolyData* targ,vtkFloatArray* displ)
{
  vtkpxSurfaceUtil::ZeroAllDisplacements(ref,displ);
  
  vtkIterativeClosestPointTransform* icp=vtkIterativeClosestPointTransform::New();
  icp->SetSource(ref);
  icp->SetTarget(targ);
  icp->GetLandmarkTransform()->SetMode(this->InitialMode);
  icp->SetMaximumNumberOfLandmarks(200);
  if (this->Debug)
    icp->DebugOn();
  icp->Update();

  vtkPoints* points=ref->GetPoints();
  int nump=points->GetNumberOfPoints();

  for (int ia=0;ia<nump;ia++)
    {
      double x[3],tx[3];
      points->GetPoint(ia,x);
      icp->TransformPoint(x,tx);
      for (int ib=0;ib<=2;ib++)
	displ->SetComponent(ia,ib,tx[ib]-x[ib]);
    }

  icp->Delete();
}
//------------------------------------------------------------------------------
int   vtkpxShapeTracking::SmoothSpreadDisplacements(PXIntegerList* neighbors,
						    vtkFloatArray* displ,vtkFloatArray* olddisp,
						    vtkIntArray* flags,int movefixed)
{
  int good=0;
  for (int pt=0;pt<displ->GetNumberOfTuples();pt++)
    {
      // If Point is not fixed or movefixed==1
      int fixed=(int)flags->GetComponent(pt,0);
      int hasdispl=(int)flags->GetComponent(pt,1);

      if ( fixed==0 || movefixed>0)
	{
	  neighbors[pt].Rewind();
	  int numgood=0;
	  double newx[3];
	  for (int ia=0;ia<=2;ia++)
	    newx[ia]=0.0;
	  for (int n=0;n<neighbors[pt].getnumnodes();n++)
	    {
	      int ind=neighbors[pt].getcurrent();
	      
	      if (flags->GetComponent(ind,1)==1)
		{
		  ++numgood;
		  for (int ia=0;ia<=2;ia++)
		    newx[ia]+=olddisp->GetComponent(ind,ia);
		}
	      neighbors[pt].Next();
	    }
	  
	  if (numgood>0)
	    {
	      int ia;
	      for (ia=0;ia<=2;ia++)
		newx[ia]=newx[ia]/float(numgood);
	      float factor=0.75;
	      if (flags->GetComponent(pt,1)<0.01)
		factor=1.0;
	      for (ia=0;ia<=2;ia++)
		displ->SetComponent(pt,ia,(1.0-factor)*olddisp->GetComponent(pt,ia)+factor*newx[ia]);

	      if (hasdispl==0)
		flags->SetComponent(pt,1,1.0);
	      ++good;
	      //fprintf(stderr,"pt = %d ... hasdispl=%d numneighbors=%d\n",pt,hasdispl,numgood);
	    }
	  else if ( hasdispl==1)
	    {
	      //fprintf(stderr,"pt = %d ... hasdispl=%d numneighbors=%d\n",pt,hasdispl,numgood);
	      ++good;
	    }
	  else
	    {
	      fprintf(stderr,"pt= %d hasdispl=%d numneighbors=%d B A D \n\t",pt,hasdispl,
		      neighbors[pt].getnumnodes());
	      neighbors[pt].Rewind();
	      for (int n=0;n<neighbors[pt].getnumnodes();n++)
		{
		  int ind=neighbors[pt].getcurrent();
		  int gd=(int)flags->GetComponent(ind,1);
		  fprintf(stderr,"pt=%d(%d) ",ind,gd);
		  neighbors[pt].Next();
		}
	      fprintf(stderr,"\n");
	    }
	}
      else if (movefixed==0 && fixed==1)
	{
	  //fprintf(stderr,"pt = %d ... fixed\n",pt);
	  ++good;
	}
    }

  return good;
}
//------------------------------------------------------------------------------
float vtkpxShapeTracking::MapDisplacements(vtkPoints* refpoints,vtkPoints* targpoints,
					   vtkFloatArray* displ,vtkFloatArray* olddisp,
					   vtkIntArray* flags,
					   vtkPointLocator* locator)
{
  float dist=0.0,maxshift=0.0,maxdist=0.0;
  int good=0;
  for (int pt=0;pt<refpoints->GetNumberOfPoints();pt++)
    {
      if (this->Debug)
	{
	  if (100*int(pt/100)==pt && pt>0)
	    fprintf(stderr,"%d/%d ",pt,refpoints->GetNumberOfPoints());
	  if (600*int(pt/600)==pt && pt>0)
	    fprintf(stderr,"\n");
	}

      if (flags->GetComponent(pt,1)>0)
	{
	  ++good;
	  double x[3];  refpoints->GetPoint(pt,x);
	  double x2[3];
	  for (int ib=0;ib<=2;ib++)
	    x2[ib]=x[ib]+displ->GetComponent(pt,ib);
	  int id=locator->FindClosestPoint(x2);
	  double y[3]; targpoints->GetPoint(id,y);
	  float d=sqrt(vtkMath::Distance2BetweenPoints(x,y));
	  dist+=d;
	  maxdist=Fmax(maxdist,d);
	  flags->SetComponent(pt,2,id);
	  for (int ia=0;ia<=2;ia++)
	    displ->SetComponent(pt,ia,y[ia]-x[ia]);
	  
	  maxshift=Fmax(maxshift,vtkMath::Distance2BetweenPoints(olddisp->GetTuple(pt),displ->GetTuple(pt)));
	}
    }
     
  if (this->Debug)
    fprintf(stderr,"\t\taverage mapping distance %6.3f (max=%6.3f) maxshift=%6.3f good=%d\n",
	    dist/float(good),maxdist,sqrt(maxshift),good);
  
  return sqrt(maxshift);
}

//------------------------------------------------------------------------------
void vtkpxShapeTracking::InterpolateSymmetricNearestNeighbor(vtkPolyData* ref,
							     vtkPolyData* targ,
							     vtkFloatArray* displ,
							     vtkIntArray* flags,
							     vtkPointLocator* loc,
							     int iterations,
							     float threshold)
{
  // old fixed[pt] == flags component 0
  // old used[pt]  == flags component 1
  // old correspondence[pt] = flags component 2

  PXIntegerList* neighbors=vtkpxSurfaceUtil::GenerateNeighbors(ref,0);

  /*  for (int pt=0;pt<ref->GetNumberOfPoints();pt++)
    {
      int num=neighbors[pt].getnumnodes();
      fprintf(stderr,"pt=%d num=%d\t",pt,num);
      neighbors[pt].Rewind();
      for (int n=0;n<num;n++)
	{
	  int ind=neighbors[pt].getcurrent();
	  int gd=(int)flags->GetComponent(ind,1);
	  fprintf(stderr,"%d (%d) ",ind,gd);
	  neighbors[pt].Next();
	}
      fprintf(stderr,"\n");
    }
    exit(1);*/

  iterations=Irange(iterations,1,20)+50;

  int movefixed=0;

  vtkFloatArray* olddisp=vtkFloatArray::New();
  olddisp->SetNumberOfComponents(3);
  olddisp->SetNumberOfTuples(displ->GetNumberOfTuples());
  vtkPoints* refpoints=ref->GetPoints();
  vtkPoints* targpoints=targ->GetPoints();

  vtkPolyData* tmpSurface=vtkPolyData::New();


  int good=0;
  for(int pt=0;pt<refpoints->GetNumberOfPoints();pt++)
    good+=(int)flags->GetComponent(pt,1);
  if (good==refpoints->GetNumberOfPoints())
    movefixed=1;
        
  for (int iter=1;iter<=iterations;iter++)
    {

      // Smooth Step
      // -----------
      if (this->Debug)
	{
	  fprintf(stderr,"----------------------\n Smoothing iteration %d/%d in_good=%d\n",iter,iterations,good);
	  fprintf(stderr,"\t1. Averaging step (moving fixed=%d)\n",movefixed);
	}
      olddisp->DeepCopy(displ);
      int good=vtkpxShapeTracking::SmoothSpreadDisplacements(neighbors,displ,olddisp,flags,movefixed);
      if ( good==refpoints->GetNumberOfPoints())
	movefixed=1;
      
      // Map Step
      // --------
      if (this->Debug)
	{
	  fprintf(stderr,"\t\t (%d/%d) points have displacements\n",good,refpoints->GetNumberOfPoints());
	  fprintf(stderr,"\t2. Mapping step\n");
	}
  
      float maxshift=vtkpxShapeTracking::MapDisplacements(refpoints,targpoints,displ,olddisp,flags,loc);
    
      if (movefixed>0)
	{
	  if (this->Debug)
	    fprintf(stderr,"\n Over Smoothing:\n");
	  tmpSurface->DeepCopy(ref);
	  vtkpxSurfaceUtil::ShiftPointsByDisplacements(tmpSurface,displ,1.0);
	  vtkpxSurfaceUtil::NonShrinkSmooth(tmpSurface,neighbors,0.16,-0.165,10,1);
	  vtkpxSurfaceUtil::ZeroAllDisplacements(ref,displ);
	  vtkpxSurfaceUtil::AddToDisplacements(ref,tmpSurface,displ);
	  
	  if (maxshift<threshold)
	    iter=iterations+1;
	}
    }

  olddisp->Delete();
  tmpSurface->Delete();
  delete [] neighbors;
}
//------------------------------------------------------------------------------
int vtkpxShapeTracking::DoCurvatureTracking(vtkPolyData* ref,vtkPolyData* targ,
					    vtkFloatArray* Displacements,
					    vtkIntArray* Correspondences,
					    vtkFloatArray* Confidences)
{
  int searchwindow=Irange(this->SearchRadius,0,20);
  if (this->Debug)
      fprintf(stderr,"\n Initial Curvature Matching Step (window=%d)\n",searchwindow);

  int numpoints=Displacements->GetNumberOfTuples();

  PXIntegerList* neighbors=vtkpxSurfaceUtil::GenerateNeighbors(targ,1);
  PXIntegerList* searchPoints=new PXIntegerList();
  double min1=100000.0,min2=100000.0;
  int nptot=0;

  // conf=new PX3Vector[numpoints];
  double* minMatchDist=new double[numpoints];
  double* aveMatchDist=new double[numpoints];
  double* sigMatchDist=new double[numpoints];
  double* extraFactor =new double[numpoints];

  vtkPoints* refpoints=ref->GetPoints();
  vtkPoints* targpoints=targ->GetPoints();
  
  vtkDataArray* refcurvatures=ref->GetPointData()->GetScalars();
  vtkDataArray* targcurvatures=targ->GetPointData()->GetScalars();

  // Find Initial Match 
  int pt1=0,pt2=0;

  double point2_k[3],point1_k[2],point1_x[3],point2_x[3];


  for (pt1=0;pt1<numpoints;pt1++)
    {
      refpoints->GetPoint(pt1,point1_x);
      refcurvatures->GetTuple(pt1,point1_k);
      pt2=(int)Correspondences->GetComponent(pt1,2);
      
      int np2=vtkpxSurfaceUtil::CollectNeighbours(targ,pt2,searchwindow,searchPoints,&neighbors[0]);
      nptot+=np2;
      int bestpt2=pt2;
      searchPoints->Rewind();
      
      double minebe=100000.0,sumebe=0.0,sum2ebe=0.0;
      for (pt2=0;pt2<np2;pt2++)
	{
	  int cur=searchPoints->getcurrent();
	  targcurvatures->GetTuple(cur,point1_k);
	  double ebe=0.0;
	  
	  for (int ia=0;ia<=1;ia++)
	    ebe+=pow(point2_k[ia]-point1_k[ia],2.0);
	  
	  ebe*=0.5;

	  if (ebe<minebe)
	    {
	      minebe=ebe;
	      bestpt2=searchPoints->getcurrent();
	    }
	    			
	    sumebe+=ebe;
	    sum2ebe+=pow(ebe,2.0);
	    searchPoints->Next();
	  }
	
      targpoints->GetPoint(bestpt2,point2_x);
      
      // This is the actual displacement estimate
      for (int ia=0;ia<=2;ia++)
	Displacements->SetComponent(pt1,ia,point2_x[ia]-point1_x[ia]);
      
      // Store min difference, mean difference and sigma for confidence estimation 
      if (np2>1)
	{
	  sumebe=(sumebe-minebe)/double(np2-1);
	  sum2ebe=(sum2ebe-minebe*minebe)/double(np2-1);
	  minMatchDist[pt1]=minebe;
	  aveMatchDist[pt1]=sumebe;
	  sigMatchDist[pt1]=(sum2ebe-pow(aveMatchDist[pt1],2.0));
	  double bend=pow(point1_k[0],2.0)+pow(point1_k[1],2.0);
	  if (bend<0.001)
	    bend=0.0001;
	  extraFactor[pt1]=sqrt(bend);// *double(np2);
	}
      else
	{
	  // In this unlikely case let it blow up to large values low confidence 
	  fprintf(stderr,"*np2=0*pt=%d\n",pt1);
	  minMatchDist[pt1]=10000.1;
	  aveMatchDist[pt1]=10000.0;
	  sigMatchDist[pt1]=10000.0;
	  extraFactor[pt1]=0.01;
	}
      
      
      // This is needed for scaling
      min1=Fmin(min1,minMatchDist[pt1]);
      min2=Fmin(min2,minMatchDist[pt1]/(Fmax(aveMatchDist[pt1]-sigMatchDist[pt1],0.00001)));
      
      if (this->Debug)
	{
	  if (100*int(pt1/100)==pt1)
	    {
	      fprintf(stderr,"%d/%d (np=%d,min=%10.8f ave=%10.8f sig=%10.8f)\n",
		      pt1,ref->GetNumberOfPoints(),np2,
		      minMatchDist[pt1],aveMatchDist[pt1],sigMatchDist[pt1]);
	      fprintf(stderr,"\t\t minebe=%f, sumebe=%f sum2ebe=%f\n",
		      minebe,sumebe,sum2ebe);
	      fprintf(stderr," pt1=%d k1=%7.5f k2=%7.5f \t pt2=%d k1=%7.5f k2=%7.5f\n",
		      pt1,point1_k[0],point1_k[1],bestpt2,point2_k[0],point2_k[1]);
	    }
	}
    }
  
  double K1 = 0.01 / Fmax(min1,0.0001);
  double K2 = 0.01 / Fmax(min2,0.0001);
  
  float maxconf=0.0;
  for(pt1=0; pt1<ref->GetNumberOfPoints(); pt1++)
    {
      double mg=minMatchDist[pt1];
      double mu=minMatchDist[pt1]/Fmax(aveMatchDist[pt1]-sigMatchDist[pt1],0.00001);
      Confidences->SetComponent(pt1,0,/*extraFactor[pt1]*/(1.0 / (  ( 1.0+ K1*mg)*(1.0+K2*mu))));
      maxconf=Fmax(maxconf,Confidences->GetComponent(pt1,0));
    }
  
  float sumconf=0.0;
  for (pt1=0;pt1<ref->GetNumberOfPoints();pt1++)
    {
      Confidences->SetComponent(pt1,0,Confidences->GetComponent(pt1,0)/maxconf);
      sumconf+=Confidences->GetComponent(pt1,0);;
    }
  sumconf/=(float(ref->GetNumberOfPoints()));
  // Do Gamma-like correction so that mean becomes 0.5
  float gamma=log(0.5)/log(sumconf);
  
  if (this->Debug)
    fprintf(stderr,"Done with confidence values\n\t mean=%6.4f gamma=%6.4f \n",sumconf,gamma);
  
  sumconf=0.0; 
  float sum2conf=0.0;
  
  for (pt1=0;pt1<ref->GetNumberOfPoints();pt1++)
    {
      float c=pow(Confidences->GetComponent(pt1,0),double(gamma));
      Confidences->SetComponent(pt1,0,c);
      sumconf+=c;
      sum2conf+=pow(c,float(2.0));
    }
  sumconf/=(float(ref->GetNumberOfPoints()));
  sum2conf/=(float(ref->GetNumberOfPoints())); 
  if (this->Debug)
    {
      fprintf(stderr,"Done with adjusting confidence values\n\t mean=%6.4f std=%6.4f\n",
	      sumconf,sqrt(sum2conf-sumconf*sumconf));
      fprintf(stderr,"Total points compared =%d ave=%6.2f\n",nptot,
	      float(nptot)/float(ref->GetNumberOfPoints()));
    }
  
  delete searchPoints;
  delete [] neighbors;
  delete [] minMatchDist;
  delete [] aveMatchDist;
  delete [] sigMatchDist;
  delete [] extraFactor;
  return 1;
}
//------------------------------------------------------------------------------

