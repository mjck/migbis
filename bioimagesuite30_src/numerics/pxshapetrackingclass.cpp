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
// ---------------------------------------------------------------------------------------------------------
#include "pxshapetrackingclass.h"
#include "pxutil.h"

//#include "nrutil.h"
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July 1995   papad@noodle.med.yale.edu

   see pxshapetrackingclass.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
// --------------------------------------------------------------------------
int  PXShapeTrackingClass::debugmode=1;
// --------------------------------------------------------------------------
PXTriangulatedStack*  PXShapeTrackingClass::getstack(int i)
{
  return tstack[Irange(i,0,1)];
}

PX3Vector* PXShapeTrackingClass::getdisplacement(int i)
{
  if (!hasdisplacements())
      return NULL;
  return &displacements[Irange(i,0,numpoints-1)];
}

float PXShapeTrackingClass::getconfidence(int i)
{
  if (!hasdisplacements())
      return 0.0;
  return confidences[Irange(i,0,numpoints-1)];
}

void PXShapeTrackingClass::setconfidence(int i,float s)
{
  if (hasdisplacements())
      {
	i=Irange(i,0,numpoints-1);
	confidences[i]=s;
      }
}

void PXShapeTrackingClass::setdisplacement(int i,int comp,float s)
{
  if (hasdisplacements())
      {
	i=Irange(i,0,numpoints-1);
	comp=Irange(comp,0,2);
	displacements[i].x[comp]=s;
      }
}

int PXShapeTrackingClass::getused(int i)
{
  if (!hasdisplacements())
      return 0;
  return used[Irange(i,0,numpoints-1)];
}

int PXShapeTrackingClass::getcorrespondence(int i)
{
   if (!hasdisplacements())
      return 0;
   return correspondence[Irange(i,0,numpoints-1)];
}

int PXShapeTrackingClass::getfixed(int i)
{
  if (!hasdisplacements())
      return 0;
  return fixed[Irange(i,0,numpoints-1)];
}

int PXShapeTrackingClass::hasdisplacements()
{
  return hasDisplacements;
}

int PXShapeTrackingClass::hasstack(int i)
{
  return hasStack[Irange(i,0,1)];
}

int PXShapeTrackingClass::hasstacks()
{
  return hasStack[0]*hasStack[1];
}

int PXShapeTrackingClass::getnumpoints()
{
  return numpoints;
}

// --------------------------------------------------------------------------
PXShapeTrackingClass::PXShapeTrackingClass()
{
  init(0);
  hasStack[0]=0; hasStack[1]=0; 
}

// --------------------------------------------------------------------------
PXShapeTrackingClass::PXShapeTrackingClass(PXShapeTrackingClass* old)
{
  init(0);
  hasStack[0]=0; hasStack[1]=0; 
  Copy(old);
}
// --------------------------------------------------------------------------
PXShapeTrackingClass::PXShapeTrackingClass(const char* fname1,const char* fname2)
{
  tstack[0]=new PXTriangulatedStack();
  int ok=tstack[0]->Load(fname1);
  if (debugmode)
      fprintf(stderr,"Loaded tstack[0] from %s (%d)\n",fname1,ok);
  tstack[0]->generateNeighbours(1);
  tstack[1]=new PXTriangulatedStack();
  ok=tstack[1]->Load(fname2);
  if (debugmode)
      fprintf(stderr,"Loaded tstack[1] from %s (%d)\n",fname2,ok);
  tstack[1]->generateNeighbours(1);
  init(tstack[0]->getnumpoints());
  hasStack[0]=1; hasStack[1]=1; 
}
// --------------------------------------------------------------------------
PXShapeTrackingClass::PXShapeTrackingClass(PXTriangulatedStack* stack1,
					   PXTriangulatedStack* stack2)
{
  tstack[0]=new PXTriangulatedStack(stack1);
  tstack[0]->generateNeighbours(1);
  tstack[1]=new PXTriangulatedStack(stack2);
  tstack[1]->generateNeighbours(1);
  init(tstack[0]->getnumpoints());
  hasStack[0]=1; hasStack[1]=1; 
}
// --------------------------------------------------------------------------
PXShapeTrackingClass::~PXShapeTrackingClass()
{
  cleanup();
}
// --------------------------------------------------------------------------
void PXShapeTrackingClass::nextFrame(const char* newstack2fname,int deform)
{
  if (!hasstack(0))
      {
	fprintf(stderr,"Does not have stack 0\n");
	return;
      }

  if (!hasstack(1))
      tstack[1]=new PXTriangulatedStack();

  int ok=tstack[1]->Load(newstack2fname);
  if (ok)
      tstack[1]->generateNeighbours(1);
  if (debugmode)
      fprintf(stderr,"Loaded tstack[1] from %s (%d)\n",newstack2fname,ok);
  if (deform && hasdisplacements())
      tstack[0]->deformUsingDisplacements(displacements,1.0,0);
}
// --------------------------------------------------------------------------
void PXShapeTrackingClass::nextFrame(PXTriangulatedStack* newstack2,int deform)
{
  if (!hasstack(0))
      {
	fprintf(stderr,"Does not have stack 0\n");
	return;
      }

  if (!hasstack(1))
      tstack[1]=new PXTriangulatedStack();


  tstack[1]->Copy(newstack2);
  tstack[1]->generateNeighbours(1);
  if (deform && hasdisplacements())
      tstack[0]->deformUsingDisplacements(displacements,1.0,0);
}
// --------------------------------------------------------------------------
void PXShapeTrackingClass::init(int np)
{
  numpoints=Irange(np,0,10000000);
  if (numpoints>0)
      {
	displacements=new PX3Vector[numpoints];
	confidences=new float[numpoints];
	used=new int[numpoints];
	correspondence=new int[numpoints];
	fixed=new int[numpoints];
	for (int i=0;i<numpoints;i++)
	    {
	      displacements[i].setvector(0.0,0.0,0.0);
	      confidences[i]=1.0;
	      used[i]=0; fixed[i]=0; correspondence[i]=0;
	    }
	hasDisplacements=1;
      }
  else
      hasDisplacements=0;
}
// --------------------------------------------------------------------------
void PXShapeTrackingClass::cleanup() 
{
  if (hasDisplacements)
      {
	delete [] displacements;
	delete [] confidences;
	delete [] used;
	delete [] correspondence;
	delete [] fixed;
      }
  for (int i=0;i<=1;i++)
      {
	if (hasstack(i))
	    delete tstack[i];
	hasStack[i]=0;
      }
  hasDisplacements=0;
  numpoints=0;
}
// --------------------------------------------------------------------------
void PXShapeTrackingClass::Copy(PXShapeTrackingClass* other)
{
  cleanup();
  for (int ia=0;ia<=1;ia++)
      {
	if (other->hasstack(ia))
	    {
	      tstack[ia]=new PXTriangulatedStack(other->getstack(ia));
	      hasStack[ia]=1;
	    }
	  }

  int np=other->getnumpoints();
  if (np>0)
      {
	init(np);
	for (int i=0;i<np;i++)
	    {
	      displacements[i].copy(other->getdisplacement(i));
	      confidences[i]=other->getconfidence(i);
	      used[i]=other->getused(i);
	      correspondence[i]=other->getcorrespondence(i);
	      fixed[i]=other->getfixed(i);
	    }
      }
}
// --------------------------------------------------------------------------
int PXShapeTrackingClass::saveStackInventor(const char* fname,int colormode)
{
  if (!hasstack(0) || !hasdisplacements())
      {
	fprintf(stderr,"Does not have stack 0 or displacements\n");
	return 0;
      }

  colormode=Irange(colormode,0,2);
  
  if (debugmode)
      fprintf(stderr,"Saving stack %d in %s (colormode=%d)\n",0,fname,colormode);
  
  if (colormode==0 || !hasdisplacements())
      return tstack[0]->Save(fname);

  PXColorPoint* colors=new PXColorPoint[numpoints];
  float max=0.00001,v=0.0;
 
  int i;
  for (i=0;i<numpoints;i++)
      {
	if (colormode==1)
	    v=displacements[i].getlength();
	else
	    v=confidences[i];
	max=Fmax(v,max);
      }
  
  for (i=0;i<numpoints;i++)
      {
	if (colormode==1)   
	    v=displacements[i].getlength()/max;
	else 
	    v=confidences[i];

	if (v<0.3)
	    {
	      colors[i].red  =0.0;
	      colors[i].blue =0.0;
	      colors[i].green=1.0;
	    }
	else if (v<0.7)
	    {
	      colors[i].red =0.5;
	      colors[i].blue=1.0;
	      colors[i].green=1.0;
	    }
	else
	    {
	      colors[i].red=  1.0;
	      colors[i].blue=  0.0;
	      colors[i].green =0.5;
	    }
      }
  int ok=tstack[0]->SaveInventor(fname,colors);  
  delete [] colors;
  return ok;
}
// --------------------------------------------------------------------------
int PXShapeTrackingClass::saveInventor(const char* fname,int number)
{
  if (!hasstacks() || !hasdisplacements())
      {
	fprintf(stderr,"Does not have stacks or displacements\n");
	return 0;
      }

  char* line=new char[strlen(fname)+10];
  sprintf(line,"%s.%02d.iv",fname,number);
  int ok=saveInventor(line);
  delete [] line;
  return ok;
}
// --------------------------------------------------------------------------
int PXShapeTrackingClass::saveDeformed(const char* base,int number)
{
  if (!hasstack(0) || !hasdisplacements())
      {
	fprintf(stderr,"Does not have stacks or displacements\n");
	return 0;
      }

  char* line=new char[strlen(base)+10];
  sprintf(line,"%s.%02d.iv",base,number);
  tstack[0]->deformUsingDisplacements(displacements,1.0,0);
  int ok=tstack[0]->SaveInventor(line);
  tstack[0]->deformUsingDisplacements(displacements,-1.0,0);
  delete [] line;
  return ok;
}
// --------------------------------------------------------------------------
int PXShapeTrackingClass::saveInventor(const char* fname)
{
  if (!hasstack(0) || !hasdisplacements())
      {
	fprintf(stderr,"Does not have stacks or displacements\n");
	return 0;
      }

  int status=0;
  FILE *fout=fopen(fname,"w");
  if (fout)
      {
	status=1;
	fprintf(fout,"#Inventor V2.0 ascii\nSeparator {\n");
	fprintf(fout,"DrawStyle { lineWidth 0.25 }\n");
	fprintf(fout,"Material {  diffuseColor 0.2 0.2 0.0 \n transparency 0.2}\n" );
	tstack[0]->SaveInventor(fout);
	if (hasstack(1))
	    {
	      fprintf(fout,"Material {  diffuseColor 0.0 0.0 0.2 \n transparency 0.4}\n" );
	      tstack[1]->SaveInventor(fout);
	    }

	fprintf(fout,"DrawStyle { lineWidth 4.0 }\n");
	fprintf(fout,"Coordinate3{ point[\n");
	int i;
	for (i=0;i<tstack[0]->getnumpoints();i++)
	    fprintf(fout,"%6.3f %6.3f %6.3f,\n",
		    (tstack[0]->getpoint(i))->x[0],
		    (tstack[0]->getpoint(i))->x[1],
		    (tstack[0]->getpoint(i))->x[2]);
	float mult=1.0;
	for (i=0;i<tstack[0]->getnumpoints();i++)
	    fprintf(fout,"%6.3f %6.3f %6.3f,\n",
		    (tstack[0]->getpoint(i))->x[0]+mult*displacements[i].x[0],
		    (tstack[0]->getpoint(i))->x[1]+mult*displacements[i].x[1],
		    (tstack[0]->getpoint(i))->x[2]+mult*displacements[i].x[2]);
	
	fprintf(fout,"]}\n");
	
	for (int pass=0;pass<=2;pass++)
	    {
	      switch(pass)
		  {
		  case 0:
		    fprintf(fout,"Material {  emissiveColor 0.5 0.5 0.0 \n transparency 0.0 }\n");
		    break;
		  case 1:
		    fprintf(fout,"Material {  emissiveColor 0.5 0.0 0.0 \n transparency 0.0}\n");
		    break;
		  case 2:
		    fprintf(fout,"Material {  emissiveColor 0.0 0.5 0.0 \n transparency 0.0}\n" );
		    break;
		  }
	      
	      fprintf(fout,"IndexedLineSet{ coordIndex[\n");
	      
	      for (i=0;i<tstack[0]->getnumpoints();i++)
		  {
		    if (pass==fixed[i]+used[i])
			fprintf(fout,"%d,%d,-1,\n",i,i+tstack[0]->getnumpoints());
		  }
	      fprintf(fout,"]}\n");
	    }
	fprintf(fout,"}\n");
	fclose(fout);


	if (debugmode)
	    fprintf(stderr,"Saving in %s \n",fname);
      }
  return status;
}
// --------------------------------------------------------------------------
int PXShapeTrackingClass::findSymmetricPair(PXNeighboursList* pairs,int seedt)
{
  if (!hasstacks())
      return 0;

  static int count=0;

  if (debugmode && count==0)
      {
	fprintf(stderr,"Using Symmetric Nearest Neighbour (%d,%d)\n",
		tstack[0]->getnumpoints(),tstack[1]->getnumpoints());
	count=1;
      }

  int done=0,steps=0;
  int seedpoint=Irange(seedt,0,tstack[0]->getnumpoints()-1);
  int bestpoint=0,bestpoint2=0;
  
  while(!done && steps<10)
      {
	float x[3],x0[3];
	int ia;
	for (ia=0;ia<=2;ia++)
	    x[ia]=(tstack[0]->getpoint(seedpoint))->x[ia];
	tstack[1]->findnearestpoint(x[0],x[1],x[2],bestpoint,0,0.01);
	
	for (ia=0;ia<=2;ia++)
	     x0[ia]=(tstack[1]->getpoint(bestpoint))->x[ia];

	tstack[0]->findnearestpoint(x0[0],x0[1],x0[2],bestpoint2,0,0.01);
	if (seedpoint==bestpoint2)
	    done=1;
	else
	    seedpoint=bestpoint2;
	
	steps++;
      }
  
  if (steps<10)
      {
	PXIntegerList* firstpairs=new PXIntegerList();
	firstpairs->Add(seedpoint);
	firstpairs->Add(bestpoint+tstack[0]->getnumpoints());
	
	if (pairs->getnumnodes()==0)
	    {
	      pairs->Add(firstpairs,pairs->getnumnodes()+1);
	    }
	else
	    {
	      pairs->Rewind();
	      int found=0,i=0;
	      while (i<pairs->getnumnodes() && found==0)
		  {
		    PXIntegerList* element=(pairs->getcurrentnode())->points;
		    if (element->numberinCommon(firstpairs)==2)
			found=1;
		    i++;
		    pairs->Next();
		  }
	      if (found==0)
		  {
		    pairs->Add(firstpairs,pairs->getnumnodes()+1);
		  }
	    }
	delete firstpairs;
	return 1;
      }
  return 0;
}
// --------------------------------------------------------------------------
int PXShapeTrackingClass::smoothEuclidean(int movefixed,int subpixel)
{
  if (!hasdisplacements() || !hasstacks())
      return 0;

  if (debugmode)
      fprintf(stderr,"\t1. Averaging step (moving fixed=%d,subpixel=%d)\n",
	      movefixed,subpixel);
  
  int good=0,pt;
  
  PX3Vector* olddisp=new PX3Vector[numpoints];

  for (pt=0;pt<tstack[0]->getnumpoints();pt++)
      {
	if (fixed[pt]==0 || movefixed>0)
	    {
	      olddisp[pt].copy(&displacements[pt]);
	      good+=used[pt];
	      PXIntegerList* neigh=tstack[0]->getneighbours(pt);
	      neigh->Rewind();
	      int numgood=0;
	      float newx[3];
	      for (int ia=0;ia<=2;ia++)
		  newx[ia]=0.0;
	      for (int n=0;n<neigh->getnumnodes();n++)
		  {
		    int ind=neigh->getcurrent();
		    if (used[ind]==1)
			{
			  numgood++;
			  for (int ia=0;ia<=2;ia++)
			      newx[ia]+=olddisp[ind].x[ia];
			}
		    neigh->Next();
		  }
	      if (numgood>0)
		  {
		    int ia;
		    for (ia=0;ia<=2;ia++)
			newx[ia]=newx[ia]/float(numgood);
		    float factor=0.75;
		    if (used[pt]==0)
			factor=1.0;
		    for (ia=0;ia<=2;ia++)
			displacements[pt].x[ia]=(1.0-factor)*olddisp[pt].x[ia]+factor*newx[ia];
		    used[pt]=1;
		  }
	    }
      }
  delete [] olddisp;
  good=0;
  for( pt=0;pt<tstack[0]->getnumpoints();pt++)
      {
	used[pt]=abs(used[pt]);
	good+=used[pt];
      }
  
  if (debugmode)
      {
	fprintf(stderr,"\t\t (%d/%d) points have displacements\n",good,tstack[0]->getnumpoints());
	fprintf(stderr,"\t2. Mapping step\n");
      }


  float dist=0.0,maxshift=0.0,maxdist=0.0;
  int reverse=0;
  for (pt=0;pt<tstack[0]->getnumpoints();pt++)
      {
	if (debugmode)
	    {
	      if (100*int(pt/100)==pt && pt>0)
		  fprintf(stderr,"%d/%d ",pt,tstack[0]->getnumpoints());
	      if (600*int(pt/600)==pt && pt>0)
		  fprintf(stderr,"\n");
	    }
	PXTriangulatedStackPoint* point1=tstack[0]->getpoint(pt);
	if ((used[pt]>0) && (fixed[pt]!=1 || good==tstack[0]->getnumpoints()))
	    {
	      float x[3];
	      int ia;
	      for (ia=0;ia<=2;ia++)
		  x[ia]=point1->x[ia]+displacements[pt].x[ia]; 
	      int mp;
	      float d=tstack[1]->findnearestpoint(x[0],x[1],x[2],mp,subpixel,0.0);
	      dist+=d;
	      for (ia=0;ia<=2;ia++)
		  {
		    x[ia]=x[ia]-point1->x[ia]; 
		    displacements[pt].x[ia]=x[ia];
		  }
	      maxdist=Fmax(maxdist,d);
	      maxshift=Fmax(maxshift,Fsqdist( x[0],x[1],x[2],0.0,0.0,0.0));
	      correspondence[pt]=mp;
	    }
      }
  if (debugmode)
      fprintf(stderr,"\t\taverage mapping distance %6.3f (max=%6.3f) maxshift=%6.3f rev=%d\n",
	      dist/float(good),maxdist,sqrt(maxshift),reverse);

  return good;
}
// --------------------------------------------------------------------------
int PXShapeTrackingClass::smoothConnection(int movefixed)
{
  if (!hasdisplacements() || !hasstacks())
      return 0;

  if (debugmode)
      fprintf(stderr,"\t1. Maxdistance minimizing step (moving fixed=%d)\n",movefixed);
  int shift=0;

  for (int pt=0;pt<tstack[0]->getnumpoints();pt++)
      {
	if (fixed[pt]==0 || movefixed==1)
	    {
	      PXIntegerList* neigh=tstack[0]->getneighbours(pt);
	      neigh->Rewind();
	      int bestcenter=correspondence[pt];
	      int bestdist=0,bestmindist=100000000;
	      for (int n=0;n<neigh->getnumnodes();n++)
		  {
		    int ind=neigh->getcurrent();
		    int dist=tstack[1]->getrelationship(correspondence[pt],correspondence[ind],100);
		    if (dist<0) 
			dist=100;
		    bestdist=Imax(dist,bestdist);
		    bestmindist=Imin(dist,bestmindist);
		    neigh->Next();
		  }
	      PXIntegerList* candidates=tstack[1]->getneighbours(correspondence[pt]);
	      candidates->Rewind();
	      for (int i=0;i<candidates->getnumnodes();i++)
		  {
		    int cand=candidates->getcurrent();
		    neigh->Rewind();
		    int maxd=0,mind=100000000;
		    for (int j=0;j<neigh->getnumnodes();j++)
			{
			  int ind=neigh->getcurrent();
			  int dist=tstack[1]->getrelationship(cand,correspondence[ind],100);
			  if (dist<0)
			      dist=101;
			  maxd=Imax(maxd,dist);
			  mind=Imin(mind,dist);
			  neigh->Next();
			}
		    if (maxd<bestdist)
			{
			  bestdist=maxd;
			  bestmindist=mind;
			  bestcenter=cand;
			}
		    candidates->Next();
		  }
	      PXTriangulatedStackPoint* point1=tstack[0]->getpoint(pt);
	      PXTriangulatedStackPoint* point2=tstack[1]->getpoint(bestcenter);
	      if (bestcenter!=correspondence[pt])
		  {
		    correspondence[pt]=bestcenter;
		    shift++;
		  }
	      for (int ia=0;ia<=2;ia++)
		  displacements[pt].x[ia]=point2->x[ia]-point1->x[ia];
	    }
	
	if (debugmode)
	    {
	      if (100*int(pt/100)==pt && pt>0)
		  fprintf(stderr,"%d/%d (%d), ",pt,tstack[0]->getnumpoints(),shift);
	      if (600*int(pt/600)==pt && pt>0)
		  fprintf(stderr,"\n");
	    }
      }
  if (debugmode)
      fprintf(stderr,"   [ %d points shifted]\n",shift);

  return shift;
}
// --------------------------------------------------------------------------
int PXShapeTrackingClass::findSmoothMap(int mode,int iterations,int smoothiterations,int subdivide)
{
  if (!hasdisplacements() || !hasstacks())
      return 0;

  subdivide=Irange(subdivide,0,3);
  for(int ip=0;ip<subdivide;ip++)
      tstack[1]->subdivide();

  int numiter=0;
  int postmode=smoothiterations>0;
  mode=Irange(mode,0,2);
  int id=-1;

  if (debugmode)
      {
	fprintf(stderr," Find Smooth Map:\n");
	switch(mode)
	    {
	    case 2:
	      fprintf(stderr," Using global `connection-based' nearest neighbour initialization (post=%d)\n",
		      postmode);
	      break;
	    case 1:
	      fprintf(stderr," Using global `euclidean-based' nearest neighbour initialization (post=%d)\n",
		      postmode);
	      break;
	    default:
	      fprintf(stderr," Using nearest neighbour initialization (post=%d)\n",postmode);
	      break;
	    }
      }

  if (mode>0) // One of the two fancy initialization algorithms 
      {
	for (int ia=0;ia<=1;ia++)
	  {
	    if (debugmode)
	      fprintf(stderr,"Generating Neighbours for stack %d np=%d\n",ia,tstack[ia]->getnumpoints());
	    tstack[ia]->generateNeighbours();
	  }
	//if (!tstack[ia]->hasneighbours())

	int inc=1;//tstack[0]->getnumpoints()/400+1;
	PXNeighboursList* tetra=new PXNeighboursList();

	if (debugmode)
	    fprintf(stderr,"\nStarting Symmetric Neighbour Search inc=%d points=%d\n",
		    inc,tstack[0]->getnumpoints());

	for (int pt=0;pt<tstack[0]->getnumpoints();pt+=inc)
	    {
	      if (debugmode)
		  {
		    if (100*int(pt/100)==pt && pt>0)
		      fprintf(stderr,"%d/%d (%d)  ",pt,tstack[0]->getnumpoints(),tetra->getnumnodes());
		    if (400*int(pt/400)==pt && pt>0)
		      fprintf(stderr,"\n");
		  }
	      findSymmetricPair(tetra,pt);
	    }
	if (debugmode)
	    {
	      fprintf(stderr," done\n");
	      fprintf(stderr,"\n Stack1 (numpoints) = %d , Stack 2 (numpoints) =%d \n",
		      tstack[0]->getnumpoints(),tstack[1]->getnumpoints());
	      fprintf(stderr,"Number of matches = %d \n", tetra->getnumnodes());
	      fprintf(stderr,"----------------------------------\n");
	    }

	tetra->Rewind();
	
	// Find displacement vectors
	for (int i=0;i<tetra->getnumnodes();i++)
	    {
	      PXIntegerList* pair=(tetra->getcurrentnode())->points;
	      pair->Rewind();	      
	      int index1=pair->getcurrent();
	      pair->Next();
	      int index2=pair->getcurrent();
	      PXTriangulatedStackPoint* point1=tstack[0]->getpoint(index1);
	      PXTriangulatedStackPoint* point2=tstack[1]->getpoint(index2-tstack[0]->getnumpoints());
	      
	      for (int ia=0;ia<=2;ia++)
		  displacements[index1].x[ia]=point2->x[ia]-point1->x[ia];

	      used[index1]=1;	fixed[index1]=1;
	      correspondence[index1]=index2-tstack[0]->getnumpoints();
	      tetra->Next();
	    }
	
	int numgood=tetra->getnumnodes();
	delete tetra;
	if (debugmode>1)
	    {
	      saveInventor("/tmp/iter",1);
	      saveDeformed("/tmp/deformed",1);
	    }
	
	int nmode=0;

	for (int iter=1;iter<=iterations;iter++)
	    {
	      numiter=iter;
	      if (debugmode)
		  {
		    fprintf(stderr,"\n----------------------\n");
		    fprintf(stderr,"Smoothing iteration %d\n",iter);
		    if (debugmode>1)
			  id=iter+1;
		  }
	      
	      if (mode==1)
		  {
		    if (numgood==tstack[0]->getnumpoints())
			numgood=smoothEuclidean(1,0);
		    else
			numgood=smoothEuclidean(0,0);
		  }
	      else
		  {
		    int mved=1;
		    if (numgood<tstack[0]->getnumpoints())
			{
			  numgood=smoothEuclidean(0,0);
			}
		    else
			{
			  if (nmode==0)
			      {
				tstack[0]->generateNeighbours(1);
				nmode=1;
			      }
			  if (debugmode)
			      fprintf(stderr,"Smoothing Connection\n");
			  mved=smoothConnection(1);
			  if (mved==0)
			      iter=iterations+1;
			}
		  }
	      if (debugmode>1)
		  {
		    saveInventor("/tmp/iter",id);
		    saveDeformed("/tmp/deformed",id);
		  }
	    }
      }
  else // Simple Euclidean Nearest Neighbour 
      {
	numiter=2;
	for (int pt=0;pt<tstack[0]->getnumpoints();pt++)
	    {
	      if (debugmode)
		  {
		    if (100*int(pt/100)==pt && pt>0)
			fprintf(stderr,"%d/%d ",pt,tstack[0]->getnumpoints());
		    if (700*int(pt/700)==pt && pt>0)
			fprintf(stderr,"\n");
		  }
	      PXTriangulatedStackPoint* point1=tstack[0]->getpoint(pt);
	      float x[3];
	      int ia;
	      for (ia=0;ia<=2;ia++)
		  x[ia]=point1->x[ia];
	      int mp;
	      tstack[1]->findnearestpoint(x[0],x[1],x[2],mp,0,0.0);
	      correspondence[pt]=mp;
	      fixed[pt]=0; used[pt]=1;
	      for (ia=0;ia<=2;ia++)
		  displacements[pt].x[ia]=x[ia]-point1->x[ia];
	    }
	id=1;
	if (debugmode>1)
	    {
	      saveInventor("/tmp/iter",id);
	      saveDeformed("/tmp/deformed",id);
	    }
      }
  
  if (postmode>0)
      {
	tstack[0]->generateNeighbours(1);
	tstack[0]->deformUsingDisplacements(displacements,1.0,0);
	PX3Vector* dp=tstack[0]->nonShrinkSmoothD(0.16,-0.165,
						  Irange(smoothiterations,1,50),2);
	tstack[0]->deformUsingDisplacements(dp,-1.0,0);
	tstack[0]->deformUsingDisplacements(displacements,-1.0,0);
	for (int i=0;i<tstack[0]->getnumpoints();i++)
	    for (int ia=0;ia<=2;ia++)
		displacements[i].x[ia]+=dp[i].x[ia];


	delete [] dp;
      }
  
  if (debugmode>1)
      {
	saveInventor("/tmp/iter",id+1);
	saveDeformed("/tmp/deformed",id+1);
      }
  return (numiter);
}
// ------------------------------------------------------------------------
int PXShapeTrackingClass::initialCurvatureTracking(int searchwindow)
{
  if (!hasdisplacements() || !hasstacks())
      return 0;

  searchwindow=Irange(searchwindow,0,20);
  if (debugmode)
      fprintf(stderr,"\n Initial Curvature Matching Step (window=%d)\n",searchwindow);

  PXIntegerList* searchPoints=new PXIntegerList();
  double min1=100000.0,min2=100000.0;
  int nptot=0;
  // conf=new PX3Vector[numpoints];
  double* minMatchDist=new double[numpoints];
  double* aveMatchDist=new double[numpoints];
  double* sigMatchDist=new double[numpoints];
  double* extraFactor =new double[numpoints];

  // Find Initial Match 
  int pt1;
  for (pt1=0;pt1<tstack[0]->getnumpoints();pt1++)
      {
	PXTriangulatedStackPoint* point1=tstack[0]->getpoint(pt1);
	int pt2=correspondence[pt1];
	PXTriangulatedStackPoint* point2;//=tstack[1]->getpoint(pt2);
	int np2=tstack[1]->collectNeighbours(pt2,searchwindow,searchPoints);
	nptot+=np2;
	int bestpt2=pt2;
	searchPoints->Rewind();
	
	double minebe=100000.0,sumebe=0.0,sum2ebe=0.0;
	for (pt2=0;pt2<np2;pt2++)
	  {
	    point2=tstack[1]->getpoint(searchPoints->getcurrent());
	    double ebe=0.0;
	    
	    for (int ia=0;ia<=1;ia++)
	      {
		ebe+=pow(point2->k[ia]-point1->k[ia],float(2.0));
	      }
	    ebe*=0.5;

	    if (ebe<minebe)
	      {
		minebe=ebe;
		bestpt2=searchPoints->getcurrent();
	      }
	    

			
	    sumebe+=ebe;
	    sum2ebe+=pow(ebe,double(2.0));
	    searchPoints->Next();
	  }
	
	point2=tstack[1]->getpoint(bestpt2);
	
	// This is the actual displacement estimate
	for (int ia=0;ia<=2;ia++)
	  displacements[pt1].x[ia]=point2->x[ia]-point1->x[ia];
	

	// Store min difference, mean difference and sigma for confidence estimation 
	if (np2>1)
	  {
	    sumebe=(sumebe-minebe)/double(np2-1);
	    sum2ebe=(sum2ebe-minebe*minebe)/double(np2-1);
	    minMatchDist[pt1]=minebe;
	    aveMatchDist[pt1]=sumebe;
	    sigMatchDist[pt1]=(sum2ebe-pow(aveMatchDist[pt1],double(2.0)));
	    double bend=pow(point1->k[0],float(2.0))+pow(point1->k[1],float(2.0));
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
	
	if (debugmode>1)
	  {
	    if (100*int(pt1/100)==pt1)
	      {
		fprintf(stderr,"%d/%d (np=%d,min=%10.8f ave=%10.8f sig=%10.8f)\n",
			pt1,tstack[0]->getnumpoints(),np2,
			minMatchDist[pt1],aveMatchDist[pt1],sigMatchDist[pt1]);
		fprintf(stderr,"\t\t minebe=%f, sumebe=%f sum2ebe=%f\n",
			minebe,sumebe,sum2ebe);
		fprintf(stderr," pt1=%d k1=%7.5f k2=%7.5f \t pt2=%d k1=%7.5f k2=%7.5f\n",
			pt1,point1->k[0],point1->k[1],bestpt2,point2->k[0],point2->k[1]);
	      }
	  }
      }
  
  double K1 = 0.01 / Fmax(min1,0.0001);
  double K2 = 0.01 / Fmax(min2,0.0001);
  
  float maxconf=0.0;
  for(pt1=0; pt1<tstack[0]->getnumpoints(); pt1++)
    {
      double mg=minMatchDist[pt1];
      double mu=minMatchDist[pt1]/Fmax(aveMatchDist[pt1]-sigMatchDist[pt1],0.00001);
      confidences[pt1] = /*extraFactor[pt1]*/(1.0 / (  ( 1.0+ K1*mg)*(1.0+K2*mu)));
      maxconf=Fmax(maxconf,confidences[pt1]);
    }
  
  float sumconf=0.0;
  for (pt1=0;pt1<tstack[0]->getnumpoints();pt1++)
    {
      confidences[pt1]/=maxconf;
      sumconf+=confidences[pt1];
    }
  sumconf/=(float(tstack[0]->getnumpoints()));
  // Do Gamma-like correction so that mean becomes 0.5
  float gamma=log(0.5)/log(sumconf);
  
  if (debugmode)
    fprintf(stderr,"Done with confidence values\n\t mean=%6.4f gamma=%6.4f \n",sumconf,gamma);
  
  sumconf=0.0; 
  float sum2conf=0.0;
  
  for (pt1=0;pt1<tstack[0]->getnumpoints();pt1++)
    {
      confidences[pt1]=pow(confidences[pt1],gamma);
      sumconf+=confidences[pt1];
      sum2conf+=pow(confidences[pt1],float(2.0));
      
      if (debugmode>1)
	if (100*int(pt1/100)==pt1 && pt1>0)
	  fprintf(stderr,"%d/%d (min=%10.7f ave=%10.7f sig=%10.7f conf=%10.7f)\n",
		  pt1,tstack[0]->getnumpoints(),
		  minMatchDist[pt1],aveMatchDist[pt1],sigMatchDist[pt1],confidences[pt1]);
    }
  sumconf/=(float(tstack[0]->getnumpoints()));
  sum2conf/=(float(tstack[0]->getnumpoints())); 
  if (debugmode)
    {
      fprintf(stderr,"Done with adjusting confidence values\n\t mean=%6.4f std=%6.4f\n",
	      sumconf,sqrt(sum2conf-sumconf*sumconf));
      fprintf(stderr,"Total points compared =%d ave=%6.2f\n",nptot,
	      float(nptot)/float(tstack[0]->getnumpoints()));
    }
  
  if (debugmode>1)
    {
      tstack[0]->deformUsingDisplacements(displacements,1.0,0); 
      tstack[0]->SaveInventor("/tmp/defcurv.01.iv");
      tstack[0]->deformUsingDisplacements(displacements,-1.0,0); 
    }
  delete searchPoints;
  delete [] minMatchDist;
  delete [] aveMatchDist;
  delete [] sigMatchDist;
  delete [] extraFactor;
  return 1;
}
// ------------------------------------------------------------------------
int PXShapeTrackingClass::smoothCurvatureTracking(int iterations,float lambda)
  {
  if (!hasdisplacements() || !hasstacks())
      return 0;

  lambda=Frange(lambda,0.01,0.99);
  iterations=Irange(iterations,1,1000);
  const float EPS=1.e-4;

  if (debugmode>1)
      {
	saveInventor("/tmp/curviter",1);
	saveDeformed("/tmp/curvdeformed",1);
      }

  /* smoothing the initial matching vector */
  /** the smoothing part **/
  float* sm=new float[numpoints];
  int k;
  for(k=0; k<3; k++)
      {
	int i;
	for(i=0; i<numpoints; i++)
	    sm[i] = displacements[i].x[k];
       
       /*** set the converge rule */
       float anormf = 0.0;
       for(i=0; i<numpoints; i++)
	   if(displacements[i].x[k]!=-1000.0)
	       anormf += fabs(displacements[i].x[k]);
       
       if(k==0)
	   printf("dx:  anormf = %f, EPS*anormf = %f\n",anormf,EPS*anormf);
       if(k==1)
	   printf("dy:  anormf = %f, EPS*anormf = %f\n",anormf,EPS*anormf);
       if(k==2)
	   printf("dz:  anormf = %f, EPS*anormf = %f\n",anormf,EPS*anormf);
       
       /*** smoothing iteration ***/
       for(int m=0; m<iterations; m++)
	   {
	     if (debugmode)
		 fprintf(stderr,"iteration no. %d:\n", m);
	     
	     float resid = 0.0;
	     for(int i=0; i<numpoints; i++)
		 {
		   /**** compute the new value and residual ****/		
		   float oldp = sm[i];
		   float factor=confidences[i]*(1.0-lambda);
		   float sumconf = factor;
		   sm[i] = displacements[i].x[k] * factor;
		   
		   PXIntegerList* nlist=tstack[0]->getneighbours(i);
		   nlist->Rewind();
		   int n=nlist->getnumnodes();
		   for(int j=0;j<nlist->getnumnodes();j++)
		       {
			 int p=nlist->getcurrent();
			 nlist->Next();
			 factor=lambda * 1.0/float(n+0.0001) * confidences[p];
			 sumconf+=factor;
			 sm[i] += sm[p] * factor;
		       }
		   
		   sm[i] = sm[i] / sumconf;
		   float newp = sm[i];
		   resid += fabs(newp-oldp);
		   
		 }/* end of i loop */
	     if (debugmode)
		 fprintf(stderr,"resid = %f\n", resid);
	     if(m>0 && resid<=EPS*anormf) 
		 m = iterations;
	   }/* end of iteration */
       
       /** get the smoothed dx/dy/dz **/
       for(i=0; i<numpoints; i++)
	   displacements[i].x[k]=sm[i];
     }

 delete [] sm;

 float maxshift=0.0,dist=0.0,maxdist=0.0;
 
 for (int pt=0;pt<tstack[0]->getnumpoints();pt++)
     {
       if (debugmode)
	   {
	     if (100*int(pt/100)==pt && pt>0)
		 fprintf(stderr,"%d/%d ",pt,tstack[0]->getnumpoints());
	     if (600*int(pt/600)==pt && pt>0)
		 fprintf(stderr,"\n");
	   }
       PXTriangulatedStackPoint* point1=tstack[0]->getpoint(pt);
       float x[3];
       int ia;
       for (ia=0;ia<=2;ia++)
	   x[ia]=point1->x[ia]+displacements[pt].x[ia]; 
       int mp;
       float d=tstack[1]->findnearestpoint(x[0],x[1],x[2],mp,1,0.0);
       dist+=d;
       for (ia=0;ia<=2;ia++)
	   {
	     x[ia]=x[ia]-point1->x[ia]; 
	     displacements[pt].x[ia]=x[ia];
	   }
       
       maxdist=Fmax(maxdist,d);
       maxshift=Fmax(maxshift,Fsqdist( x[0],x[1],x[2],0.0,0.0,0.0));
       correspondence[pt]=mp;
     }
 if (debugmode)
     fprintf(stderr,"\t\taverage mapping distance %6.3f (max=%6.3f) maxshift=%6.3f ",
	     dist/float(numpoints),maxdist,sqrt(maxshift));

  if (debugmode>1)
      {
	saveInventor("/tmp/curviter",2);
	saveDeformed("/tmp/curvdeformed",2);
      }
 
 return 1;
}
// ------------------------------------------------------------------------
int PXShapeTrackingClass::Save(const char* fname,int donotsavebothstacks,int complevel)
{
  FILE* fout=fopen(fname,"w");
  if (fout!=NULL)
    {
      int st=Save(fout,donotsavebothstacks);
      fclose(fout);
      return st;
    }
 return 0;
}
// ---------------------------------------------------
int PXShapeTrackingClass::Load(const char* fname,int donotloadbothstacks)
{
 gzFile fin=gzsuffixopen(fname,"r");
 if (fin)
   {
     int st=Load(fin,donotloadbothstacks);
     gzclose(fin);
     return st;
   }
 return 0;
}
// ---------------------------------------------------
int PXShapeTrackingClass::Save(FILE* fout,int donotsavebothstacks)
{
  fprintf(fout,"#PXShapeTrackingClass numstacks,hasdisplacements,numpoints\n");
  if (donotsavebothstacks)
      fprintf(fout,"%d\n",hasStack[0]);
  else
      fprintf(fout,"%d\n",hasStack[0]+hasStack[1]);
  fprintf(fout,"%d\n",hasDisplacements);
  fprintf(fout,"%d\n",numpoints);
 
 if (hasDisplacements)
     {
       fprintf(fout,"#-----------------Displacements-------------------------\n");
       for (int i=0;i<numpoints;i++)
	   {
	     for (int ia=0;ia<=2;ia++)
		 fprintf(fout,"%6.3f ",displacements[i].x[ia]);
	     fprintf(fout,"%5.3f %d %d %6d\n",confidences[i],used[i],fixed[i],correspondence[i]);
	   }
     }

 if (hasstack(0))
     tstack[0]->Save(fout);
 
  if (hasstack(1) && !donotsavebothstacks)
      tstack[1]->Save(fout);
  
  return 1;
}

// ---------------------------------------------------
int PXShapeTrackingClass::Load(gzFile fin,int donotloadbothstacks)
{
  char* line=new char[200];
  gzgets(fin,line,200);
  if (gzfindstringinstring(line,"#PXShapeTrackingClass")==0)
    {
      delete [] line;
      return 0;
    }
  
  cleanup();
  
  int num2;
  gzgets(fin,line,200); sscanf(line,"%d",&num2);
  gzgets(fin,line,200); sscanf(line,"%d",&hasDisplacements);
  gzgets(fin,line,200); sscanf(line,"%d",&numpoints);
 
  fprintf(stderr,"num2=%d hasDisplacements=%d numpoints=%d\n",
	  num2,hasDisplacements,numpoints);
 init(numpoints);
 if (hasDisplacements)
     {
       gzgets(fin,line,200); 
       for (int i=0;i<numpoints;i++)
	   {
	     gzgets(fin,line,200); 
	     sscanf(line," %f %f %f %f %d %d %d",
		    &displacements[i].x[0],&displacements[i].x[1],&displacements[i].x[2],
		    &confidences[i],&used[i],&fixed[i],&correspondence[i]);
	   }
     }
  

 if (num2>0)
     {
       if (num2>1 && donotloadbothstacks)
	   num2=1;
       for (int ia=0;ia<num2;ia++)
	   {
	     tstack[ia]=new PXTriangulatedStack();
	     tstack[ia]->Load(fin);
	     hasStack[ia]=1;
	   }
     }


 delete [] line;
 if (debugmode)
   {
     fprintf(stderr,"Loaded shapetrclass hasStacks=%d,%d hasdispl=%d numpoints=%d (donotloadbothflag=%d)\n",
	     hasStack[0],hasStack[1],hasDisplacements,numpoints,donotloadbothstacks);
     if (hasStack[0])
       fprintf(stderr,"\t stack[0] numpoints=%d\n",tstack[0]->getnumpoints());
     if (hasStack[1])
       fprintf(stderr,"\t stack[1] numpoints=%d\n",tstack[1]->getnumpoints());
   }

 return 1;
}

// ---------------------------------------------------
void PXShapeTrackingClass::translate(float px,float py,float pz)
{
  if (!hasdisplacements())
      return;
  for (int i=0;i<getnumpoints();i++)
      {
	displacements[i].setvector(px,py,pz);
	confidences[i]=1.0;
      }

}
// ---------------------------------------------------
void PXShapeTrackingClass::expand(float factor1,float factor2,float factor3)
{
  if (!hasdisplacements() || !hasstack(0))
      return;
  for (int i=0;i<getnumpoints();i++)
      {
	displacements[i].setvector(tstack[0]->getpoint(i)->nx[0]*factor1,
				   tstack[0]->getpoint(i)->nx[1]*factor2,
				   tstack[0]->getpoint(i)->nx[2]*factor3);
	confidences[i]=1.0;
      }
}
// ---------------------------------------------------
int PXShapeTrackingClass::volumePreserveZshift(PXShapeTrackingClass* other)
{
  if (!hasstack(1) || !hasstack(0))
      return 0;
  
  if (!other->hasstack(1) || !other->hasstack(0))
      return 0;
  
  if (!getstack(0)->hasstackindex() || !getstack(1)->hasstackindex())
      return 0;

  if (!other->getstack(0)->hasstackindex() || !other->getstack(1)->hasstackindex())
      return 0;


  PXTriangulatedStack* endo[2];
  PXTriangulatedStack* epi[2];
  endo[0]=getstack(0);
  endo[1]=getstack(1);
  epi[0]=other->getstack(0);
  epi[1]=other->getstack(1);
  
  if (endo[0]->getstackindex().numcontours!=epi[0]->getstackindex().numcontours)
      {
	fprintf(stderr,"Unequal number of levels endo/epi 0 exiting\n");
	return 0;
      }

  if (endo[1]->getstackindex().numcontours!=epi[1]->getstackindex().numcontours)
      {
	fprintf(stderr,"Unequal number of levels endo/epi 1 exiting\n");
	return 0;
      }

  // This is a tough one 
  /*if (endo[0]->getstackindex().numcontours!=endo[1]->getstackindex().numcontours)
      {
	fprintf(stderr,"Endo[0]!=Endo[1] exiting\n");
      }*/


  int numlevels[2];
  float* area[2];
  float* z[2];

  for (int fr=0;fr<=1;fr++)
      {
	numlevels[fr]=endo[fr]->getstackindex().numcontours;
	area[fr]=new float[numlevels[fr]];
	z[fr]=new float[numlevels[fr]];
      }
  

  for (int frame=0;frame<=1;frame++)
      {
	for (int level=0;level<numlevels[frame];level++)
	    {
	      int be0=endo[frame]->getstackindex().index[level];
	      int ee0=endo[frame]->getstackindex().index[level+1];
	      int be1=epi[frame]->getstackindex().index[level];
	      int ee1=epi[frame]->getstackindex().index[level+1];
	      int j=0;
	      
	      PXContour* cntr=new PXContour();
	      for (j=be0;j<ee0;j++)
		  cntr->Add(endo[frame]->getpoint(j)->x[0],endo[frame]->getpoint(j)->x[1]);
	      cntr->setclosed(1);
	      
	      PXContour* cntr2=new PXContour();
	      for (j=be1;j<ee1;j++)
		  cntr2->Add(epi[frame]->getpoint(j)->x[0],epi[frame]->getpoint(j)->x[1]);
	      cntr2->setclosed(1);
	      
	      area[frame][level]=fabs(cntr2->getarea()-cntr->getarea());
	      z[frame][level]=endo[frame]->getstackindex().zlevel[level];
	      delete cntr2;
	      delete cntr;
	    }
      }
  
  fprintf(stderr,"Numlevels fr1=%d,fr2=%d\n",numlevels[0],numlevels[1]);
  
  for (int i=0;i<numlevels[0]-1;i++)
      {
	int corr[2];
	float ar[2];
	float zl[2];
	float dz=0.0;
	
	for (int k=0;k<=1;k++)
	    {
	      corr[k]=0;
	      while (z[0][i+k]<z[1][corr[k]] && corr[k]<numlevels[1])
		  corr[k]++;
	      
	      dz=z[1][corr[k]+1]-z[1][corr[k]];
	    
	      float m1=(z[1][corr[k]+1]-z[0][i+k])/dz;
	      float m2=1.0-m1;
	
	      ar[k]=m1*area[1][corr[k]]+m2*area[1][corr[k]+1];
	      zl[k]=m1*z[1][corr[k]]+m2*z[1][corr[k]+1];
	    }



	float vol2=0.5*(zl[1]-zl[0])*(ar[1]+ar[0]);
	float vol1=(z[0][i+1]-z[0][i])*0.5*(area[0][i]+area[0][i+1]);
	
	float dv=vol2-vol1;
	float zshift=dv/(0.5*(ar[1]+ar[0]));
	
	//	if (int(i/7)*7==i || i==numlevels[0]-1)
	fprintf(stderr,"level %02d , \tvol1=%4.0f(%5.2f:%5.2f)"
		" vol2=%4.0f(%5.2f:%5.2f) \t z[0][i+1]:%6.2f-->%6.2f\n",
		i,
		vol1,z[0][i],z[0][i+1],
		vol2,zl[0],zl[1],
		z[0][i+1],z[0][i+1]-zshift);
	
	z[0][i+1]=z[0][i+1]-zshift;
	
	int j=0;
	int be0=endo[0]->getstackindex().index[i+1];
	int ee0=endo[0]->getstackindex().index[i+2];
	for (j=be0;j<ee0;j++)
	    endo[0]->getpoint(j)->x[2]-=zshift;
	endo[0]->getstackindex().zlevel[i+1]-=zshift;
	
	int be1=epi[0]->getstackindex().index[i+1];
	int ee1=epi[0]->getstackindex().index[i+2];
	for (j=be1;j<ee1;j++)
	    epi[0]->getpoint(j)->x[2]-=zshift;
	epi[0]->getstackindex().zlevel[i+1]-=zshift;
	/*if (int(i/7)*7==i || i==numlevels-1)
	    fprintf(stderr,"\n(%d:%d, %d:%d)\n",be0,ee0,be1,ee1);*/
      }

  //endo[0]->Save("en.00.tstack");
  //epi[0]->Save("ep.00.tstack");
  return 1;

}

// ------------------------------------------------------------------------
int PXShapeTrackingClass::initialDualCurvatureTracking(int searchwindow,PXShapeTrackingClass* other)
{
  if (!hasdisplacements() || !hasstacks() || !other->hasdisplacements() || !other->hasstacks())
      return 0;

  searchwindow=Irange(searchwindow,0,20);
  if (debugmode)
      fprintf(stderr,"\n Initial Curvature Matching Step (window=%d)\n",searchwindow);

  PXIntegerList* searchPoints=new PXIntegerList();
  double min1=100000.0,min2=100000.0;
  int nptot=0;
  // conf=new PX3Vector[numpoints];
  int doublenumpoints=numpoints+other->getnumpoints();

  double* minMatchDist=new double[doublenumpoints];
  double* aveMatchDist=new double[doublenumpoints];
  double* sigMatchDist=new double[doublenumpoints];
  double* extraFactor =new double[doublenumpoints];

  int baseindex=numpoints;

  PXShapeTrackingClass* shapeTr[2];
  shapeTr[0]=this;
  shapeTr[1]=other;
  
  // Find Initial Match 
  for (int pt1=0;pt1<doublenumpoints;pt1++)
      {
	PXTriangulatedStackPoint *point1,*point2;
	int pt2,np2;
	int sha=0,offset=0;

	if (pt1>=baseindex)
	    {
	      sha=1;  offset=baseindex;
	    }
	
	point1=shapeTr[sha]->getstack(0)->getpoint(pt1-offset);
	pt2=shapeTr[sha]->getcorrespondence(pt1-offset);
	np2=shapeTr[sha]->getstack(1)->collectNeighbours(pt2,searchwindow,searchPoints);
	nptot+=np2;
	
	int bestpt2=pt2;
	searchPoints->Rewind();
	
	double minebe=100000.0,sumebe=0.0,sum2ebe=0.0;
	for (pt2=0;pt2<np2;pt2++)
	    {
	      point2=shapeTr[sha]->getstack(1)->getpoint(searchPoints->getcurrent());
	      double ebe=0.0;
	      for (int ia=0;ia<=1;ia++)
		  ebe+=pow(point2->k[ia]-point1->k[ia],float(2.0));
	      ebe*=0.5;
	      if (ebe<minebe)
		  {
		    minebe=ebe;
		    bestpt2=searchPoints->getcurrent();
		  }
	      sumebe+=ebe;
	      sum2ebe+=pow(ebe,double(2.0));
	      searchPoints->Next();
	    }
	
	point2=shapeTr[sha]->getstack(1)->getpoint(bestpt2);
	// This is the actual displacement estimate

	for (int ia=0;ia<=2;ia++)
	    shapeTr[sha]->setdisplacement(pt1-offset,ia,point2->x[ia]-point1->x[ia]);

	// Store min difference, mean difference and sigma for confidence estimation 
	if (np2>1)
	    {
	      sumebe=(sumebe-minebe)/double(np2-1);
	      sum2ebe=(sum2ebe-minebe*minebe)/double(np2-1);
	      minMatchDist[pt1]=minebe;
	      aveMatchDist[pt1]=sumebe;
	      sigMatchDist[pt1]=(sum2ebe-pow(aveMatchDist[pt1],double(2.0)));
	      double bend=pow(point1->k[0],float(2.0))+pow(point1->k[1],float(2.0));
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

	if (debugmode>1)
	    {
	      if (100*int(pt1/100)==pt1)
		  {
		    fprintf(stderr,"%d/%d (np=%d,min=%10.8f ave=%10.8f sig=%10.8f)\n",
			    pt1,doublenumpoints,np2,
			    minMatchDist[pt1],aveMatchDist[pt1],sigMatchDist[pt1]);
		    fprintf(stderr,"\t\t minebe=%f, sumebe=%f sum2ebe=%f\n",
			    minebe,sumebe,sum2ebe);
		    fprintf(stderr," pt1=%d k1=%7.5f k2=%7.5f \t pt2=%d k1=%7.5f k2=%7.5f\n",
			    pt1,point1->k[0],point1->k[1],bestpt2,point2->k[0],point2->k[1]);
		  }
	    }
      }
  double K1 = 0.01 / Fmax(min1,0.0001);
  double K2 = 0.01 / Fmax(min2,0.0001);
  
  float maxconf=0.0;
  int sha;
  for(sha=0;sha<=1;sha++)
      for(int pt1=0; pt1<shapeTr[sha]->getstack(0)->getnumpoints(); pt1++)
	  {
	    int index=pt1;
	    if (sha==1)
		index+=baseindex;
	    
	    double mg=minMatchDist[index];
	    double mu=minMatchDist[index]/Fmax(aveMatchDist[index]-sigMatchDist[index],0.00001);
	    float cnf=(1.0 / (  ( 1.0+ K1*mg)*(1.0+K2*mu)));
	    maxconf=Fmax(maxconf,cnf);
	    shapeTr[sha]->setconfidence(pt1,cnf);
	  }
  
  float sumconf=0.0,sum2conf=0.0;
  for (sha=0;sha<=1;sha++)
      for (int pt1=0;pt1<shapeTr[sha]->getstack(0)->getnumpoints();pt1++)
	  {
	    float cnf=shapeTr[sha]->getconfidence(pt1);
	    cnf/=maxconf;
	    sumconf+=cnf;
	    sum2conf+=cnf*cnf;
	    shapeTr[sha]->setconfidence(pt1,cnf);
	  }
  
  sumconf/=float(doublenumpoints);
  sum2conf/=float(doublenumpoints);
  // Do Gamma-like correction so that mean becomes 0.5
  //float gamma=log(0.5)/log(sumconf);
  
  /*if (debugmode)
      fprintf(stderr,"Done with confidence values\n\t mean=%6.4f gamma=%6.4f \n",sumconf,gamma);
  
  sumconf=0.0; 
  sum2conf=0.0;

  for (sha=0;sha<=1;sha++)
      for (int pt1=0;pt1<tstack[0]->getnumpoints();pt1++)
      {
	float cnf=shapeTr[sha]->getconfidence(pt1);
	cnf=pow(cnf,gamma);
	sumconf+=cnf;
	sum2conf+=pow(cnf,float(2.0));
	shapeTr[sha]->setconfidence(pt1,cnf);
      }
  
  sumconf/=(float(doublenumpoints));
  sum2conf/=(float(doublenumpoints));*/

  if (debugmode)
      {
	fprintf(stderr,"Done with adjusting confidence values\n\t mean=%6.4f std=%6.4f\n",
		sumconf,sqrt(sum2conf-sumconf*sumconf));
	fprintf(stderr,"Total points compared =%d ave=%6.2f\n",nptot,
		float(nptot)/float(doublenumpoints));
      }
  
  
  delete searchPoints;
  delete [] minMatchDist;
  delete [] aveMatchDist;
  delete [] sigMatchDist;
  delete [] extraFactor;
  return 1;
}
// ------------------------------------------------------------------------

