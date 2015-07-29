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

// Defining namespace for old code

//
//
//
//
// ---------------------------------------------------------------------------------------------------------
#include "pxutil.h"
#include "pxcontourstack.h"
#include "pxtriangulatedstack.h"
#include "nrutil.h"



/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July 1995   papad@noodle.med.yale.edu

   see pxtriangulatedstack.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
// ------------------------------------------------------------------------
float PXTriangulatedStack::max_curvature=-1000.0;
float PXTriangulatedStack::min_curvature= 1000.0;
int   PXTriangulatedStack::debugmode=0;
// ------------------------------------------------------------------------
// [A] Access Contours
// ------------------------------------------------------------------------
int   PXTriangulatedStack::haspoints()       { return haspointstructures; }
int   PXTriangulatedStack::hastriangles()    { return hastrianglestructures;}
int   PXTriangulatedStack::hasneighbours()   { return hasneighbourstructures;}
int   PXTriangulatedStack::hasnormals()      { return normalsok;}
int   PXTriangulatedStack::hascurvatures()   { return curvaturesok;}
int   PXTriangulatedStack::hascentroids()    { return trianglecentroidsok;}
int   PXTriangulatedStack::hasstackindex()   { return stackindexok; }

int   PXTriangulatedStack::getmaxconnection() { return maxconnection;}
int   PXTriangulatedStack::getnumpoints()     { return numpoints; }
int   PXTriangulatedStack::getnumtriangles()  { return numtriangles;}

PXStackIndex PXTriangulatedStack::getstackindex() { return stackIndex;}

// ------------------------------------------------------------------------

PXTriangulatedStack::PXTriangulatedStack(const char* stackfname,int mode)
{
  init();
  if (mode==1)
      {
	char line2[40],line[150];
	sprintf(line2,"/tmp/splstack.off");
	
	sprintf(line,"nuages %s -tri %s -off -prepros > /dev/null",stackfname,line2);
	/*if (mode)
	    fprintf(stderr,"Executing: %s\n",line);*/
	
	system(line);
	int a=LoadOffset(line2);
	if (debugmode)
	    fprintf(stderr,"Read from %s (%d)\n",line2,a);
	sprintf(line,"rm %s  ",line2);
	system(line);
      }
  PXContourStack* cstack=new PXContourStack();
  int ok=cstack->Load(stackfname);
  if (ok)
      {
	if (mode==0)
	    {
	      setfromcontourstack(cstack,mode);
	    }
	else
	    {
	      // fprintf(stderr,"Reading Stack slices=%d\n",cstack->getnumcontours());
	      initialiseStackIndex(cstack);
	    }
      }
  delete cstack;
}  
// ------------------------------------------------------------------------
PXTriangulatedStack::PXTriangulatedStack()
{
  init();
}
// ------------------------------------------------------------------------
PXTriangulatedStack::PXTriangulatedStack(PXContourStack* old,int mode)
{
  init();
  setfromcontourstack(old,mode);
}
// ------------------------------------------------------------------------
PXTriangulatedStack::~PXTriangulatedStack()
{
  cleanup();
}
// ------------------------------------------------------------------------
PXTriangulatedStack::PXTriangulatedStack(PXTriangulatedStack* old)
{
  init(0,0);
  Copy(old);
}

// ------------------------------------------------------------------------
void PXTriangulatedStack::Copy(PXTriangulatedStack* old)
{
  cleanup();
  init(old->getnumpoints(),old->getnumtriangles());
  if (haspoints())
      {
	for(int i=0;i<numpoints;i++)
	    {
	      PXTriangulatedStackPoint* other=old->getpoint(i);
	      point[i].index=other->index;
	      int ia;
	      for (ia=0;ia<=2;ia++)
		  {
		    point[i].x[ia]=other->x[ia];
		    point[i].nx[ia]=other->nx[ia];
		  }
	      for (ia=0;ia<=1;ia++)
		  point[i].k[ia]=other->k[ia];
	    }
	normalsok=old->hasnormals();
	curvaturesok=old->hascurvatures();
      }
      
  if (hastriangles())
      {
	for (int i=0;i<numtriangles;i++)
	    {
	      PXTriangle* other=old->gettriangle(i);
	      triangle[i].index=other->index;
	      for (int j=0;j<=2;j++)
		  {
		    triangle[i].p[j]=other->p[j];
		    triangle[i].c[j]=other->c[j];
		  }
	    }
	trianglecentroidsok=old->hascentroids();
      }

  if (old->hasneighbours())
      {
	hasneighbourstructures=1;
	neighboursList=new PXNeighboursList();
	neighbours=new PXIntegerList[numpoints];
	for (int i=0;i<numpoints;i++)
	    {
	      //	      fprintf(stderr,"Node =%d(%d)\n",i,numpoints);
	      neighbours[i].Copy(old->getneighbours(i));
	      neighboursList->Add(&neighbours[i],i);
	    }
      }
  

  if (old->hasstackindex())
      {
	stackindexok=1;
	PXStackIndex st=old->getstackindex();
	stackIndex.numcontours=st.numcontours;
	stackIndex.index=new int[stackIndex.numcontours+1];
	stackIndex.zlevel=new float[stackIndex.numcontours];
	for (int i=0;i<=stackIndex.numcontours;i++)
	    stackIndex.index[i]=st.index[i];
	for (int j=0;j<stackIndex.numcontours;j++)
	    stackIndex.zlevel[j]=st.zlevel[j];
      } 
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::setfrompolarcontourstack(PXContourStack* cstack,float middlex)
{
  int npt=0,nt=0,i;
  for (i=0;i<cstack->getnumcontours();i++)
      {
	npt+=cstack->getcontour(i)->getnumpoints();
	if (i<cstack->getnumcontours()-1)
	    {
	      nt+=cstack->getcontour(i)->getnumpoints();
	      if (cstack->getcontour(i)->getnumpoints()==1)
		  nt=nt-1;
	      nt+=cstack->getcontour(i+1)->getnumpoints();
	    }
      }

  i=cstack->getnumcontours();
  nt+=cstack->getcontour(i)->getnumpoints();
  nt+=cstack->getcontour(0)->getnumpoints();
  
  init(npt,nt);

  // Copy Points once and for all
  int pt=0,maxp=0;
  for (i=0;i<cstack->getnumcontours();i++)
      {
	maxp=Imax(maxp,cstack->getcontour(i)->getnumpoints());
	float theta=cstack->getzlevel(i)*M_PI/180.0;
	PXContourPoint* pts=cstack->getcontour(i)->gettoppoint();
	for (int j=0;j<cstack->getcontour(i)->getnumpoints();j++)
	    {
	      float r=(pts->x-middlex);
	      point[pt].index=pt;
	      point[pt].x[0]=middlex+r*cos(theta);
	      point[pt].x[1]=middlex+r*sin(theta);
	      point[pt].x[2]=pts->y;
	      pts=pts->next;
	      pt++;
	    }
      }

  int basetr=0,basept=0;
  PXNeighboursList* facelist=new PXNeighboursList();
  float *x[2],*y[2],*z[2];
  int origin[2],np[2];
  x[0]=new float[maxp];  y[0]=new float[maxp]; z[0]=new float[maxp];
  x[1]=new float[maxp];  y[1]=new float[maxp]; z[1]=new float[maxp];
  for(i=0;i<cstack->getnumcontours();i++)
      {
	for (int c=0;c<=1;c++)
	    {
	      np[c]=cstack->getcontour(Icyclic(i+c,cstack->getnumcontours()))->getnumpoints();
	      PXContourPoint* pts=(cstack->getcontour(Icyclic(i+c,cstack->getnumcontours())))->gettoppoint();
	      float theta=cstack->getzlevel(Icyclic(i+c,cstack->getnumcontours()))*M_PI/180.0;
	      for(int ia=0;ia<np[c];ia++)
		  {
		    int ib=ia;
		    // Flip over for last contour 
		    if (c==1 && i==cstack->getnumcontours()-1)
			ib=np[c]-1-ia;
		    float r=(pts->x-middlex);
		    x[c][ib]=(middlex+r*cos(theta));
		    y[c][ib]=(middlex+r*sin(theta));
		    z[c][ib]=pts->y;
		    pts=pts->next;
		  }    
	    }
	
	PXTriangulatedStack::findorigin(np,x,y,z,origin);
	int numfaces=PXTriangulatedStack::triangulate(np,x,y,z,origin,
						      facelist,np[0]+np[1]+1);
	      
	for (int k=0;k<numfaces;k++)
	    {
	      PXNeighboursNode* current=facelist->getcurrentnode();
	      PXIntegerList* list=(current->points);
	      list->Rewind();
	      for (int ia=0;ia<=2;ia++)
		  {
		    if (list->getcurrent()+basept < npt)
			{
			  triangle[k+basetr].p[ia]=list->getcurrent()+basept;
			}
		    else
			{
			  int p=list->getcurrent()-np[0];
			  p=np[1]-1-p;
			  triangle[k+basetr].p[ia]=p;
			}
		    list->Next();
		  }
	      facelist->Next();
	    }
	facelist->DeleteAll();
	
	basept+=np[0];
	basetr+=numfaces;
      }
  delete [] x[0]; delete [] y[0]; delete [] z[0];
  delete [] x[1]; delete [] y[1]; delete [] z[1];

  delete facelist;
  
  generateNormals();
  
  return 1;
}
// --------------------------------------------------------------
int PXTriangulatedStack::trimtstack(float zbot,float ztop)
{
  // First the points bussiness 
  float zmin=+100000.0,zmax=-1000000.0;

  for (int i=0;i<numpoints;i++)
      {
	zmin=Fmin(zmin,point[i].x[2]);
	zmax=Fmax(zmax,point[i].x[2]);
      }
  
  zbot=Frange(zbot,0.0,0.99);
  ztop=Frange(ztop,zbot+0.005,1.0);

  float bottomz=zmin+zbot*(zmax-zmin);
  float topz   =zmin+ztop*(zmax-zmin);
  return trimtstackz(bottomz,topz);
}
// --------------------------------------------------------------
int PXTriangulatedStack::trimtstackz(float bottomz,float topz)
{
  int np=0,nt=0;
  int *ok=new int[numpoints];
  int *okt=new int[numtriangles];

  if (topz==-1000.0)
      topz=10000.0;
  if (bottomz==-1000.0)
      bottomz=-10000.0;
  int i;
  for (i=0;i<numpoints;i++)
      {
	if (point[i].x[2]>=bottomz && point[i].x[2]<=topz)
	    ok[i]=1;
	else
	    ok[i]=0;
	np+=ok[i];
      }

  for (i=0;i<numtriangles;i++)
      {
	int sum=0;
	for (int ia=0;ia<=2;ia++)
	    sum+=ok[triangle[i].p[ia]];
	if (sum==3)
	    okt[i]=1;
	else
	    okt[i]=0;
	nt+=okt[i];
      }

  // fprintf(stderr,"Num good points=%d, good triangles=%d\n",np,nt);

  PXTriangulatedStack* old=new PXTriangulatedStack(this); //OK 
  init(np,nt);
  
  int *index=new int[old->getnumpoints()];
  int base=0;
  for (i=0;i<old->getnumpoints();i++)
      {
	//if (50*int(base/50)==base)
	//	    fprintf(stderr,"Base pt=%d\n",base);
	if (ok[i]==1)
	    {
	      PXTriangulatedStackPoint* other=old->getpoint(i);
	      point[base].index=other->index;
	      int ia;
	      for (ia=0;ia<=2;ia++)
		  {
		    point[base].x[ia]=other->x[ia];
		    point[base].nx[ia]=other->nx[ia];
		  }
	      for (ia=0;ia<=1;ia++)
		  point[base].k[ia]=other->k[ia];
	      index[i]=base;
	      ++base;
	    }
      }

  base=0;
  for (i=0;i<old->getnumtriangles();i++)
      {
	//if (50*int(base/50)==base)
	//  fprintf(stderr,"Base tr=%d\n",base);

	if (okt[i]==1)
	    {
	      PXTriangle* other=old->gettriangle(i);
	      triangle[base].index=other->index;
	      for (int j=0;j<=2;j++)
		  {
		    triangle[base].p[j]=index[other->p[j]];
		    triangle[base].c[j]=other->c[j];
		  }
	      ++base;
	    }
      }
  
  normalsok=old->hasnormals();
  curvaturesok=old->hascurvatures();
  trianglecentroidsok=old->hascentroids();
  
  delete [] ok;
  delete [] okt;
  delete [] index;

  if (old->hasstackindex())
      {
	stackindexok=1;
	PXStackIndex st=old->getstackindex();
	int numc=0;
	int minc=-1;
	// fprintf(stderr,"Bottomz=%6.2f, topz=%7.2f\n",bottomz,topz);
	int j;
	for (j=0;j<st.numcontours;j++)
	    {
	      if (st.zlevel[j]>=bottomz && st.zlevel[j]<=topz)
		  {
		    if (minc==-1)
			minc=j;
		    numc++;
		  }
	    }
	// fprintf(stderr,"  minc=%d good=%d \n",minc,numc);
	
	stackIndex.numcontours=numc;
	stackIndex.index=new int[numc+1];
	stackIndex.zlevel=new float[stackIndex.numcontours];
	for (int i=0;i<=stackIndex.numcontours;i++)
	    stackIndex.index[i]=st.index[i+minc]-st.index[minc];
	for (j=0;j<stackIndex.numcontours;j++)
	    stackIndex.zlevel[j]=st.zlevel[j+minc];
	// fprintf(stderr,"Num good slices=%d\n",numc);
      } 
  else
      // fprintf(stderr,"Old has no stackindex\n");

  delete old;
  return 1;
}
// --------------------------------------------------------------
void PXTriangulatedStack::setfromcontourstack(PXContourStack* old,int mode)
{
  /*if (mode==1)
      {
	char line1[40],line2[40],line[150];
	sprintf(line1,"/tmp/splstack.cnt");
	sprintf(line2,"/tmp/splstack.off");
	if (mode)
	    // fprintf(stderr,"Saving in Nuages Format %s\n",line1);
	old->SaveNuages(line1);
	sprintf(line,"nuages %s -tri %s -off -prepros > /dev/null",line1,line2);
	if (mode)
	    // fprintf(stderr,"Executing: %s\n",line);
	
	system(line);
	int a=LoadOffset(line2);
	if (debugmode)
	    fprintf(stderr,"Read from %s (%d)\n",line2,a);
	sprintf(line,"rm %s %s ",line1,line2);
	system(line);
      }
  else
      {*/

  // Forget about Nuages 
  if (mode==1)
      mode=0;

  int npt=0,nt=0,i;
  for (i=0;i<old->getnumcontours();i++)
      {
	npt+=old->getcontour(i)->getnumpoints();
	if (i<old->getnumcontours()-1)
	    {
	      nt+=old->getcontour(i)->getnumpoints();
	      if (old->getcontour(i)->getnumpoints()==1)
		  nt=nt-1;
	      nt+=old->getcontour(i+1)->getnumpoints();
	    }
      }
  cleanup();
  init(npt,nt);
  
  // Copy Points once and for all
  int pt=0,maxp=0;
  for (i=0;i<old->getnumcontours();i++)
      {
	maxp=Imax(maxp,old->getcontour(i)->getnumpoints());
	      PXContourPoint* pts=old->getcontour(i)->gettoppoint();
	for (int j=0;j<old->getcontour(i)->getnumpoints();j++)
	    {
	      point[pt].index=pt;
	      point[pt].x[0]=pts->x; point[pt].x[1]=pts->y;
	      point[pt].x[2]=old->getzlevel(i);
	      pts=pts->next;
	      pt++;
	    }
      }
  
  int basetr=0,basept=0;
  PXNeighboursList* facelist=new PXNeighboursList();
  
  float *x[2],*y[2];
  int origin[2],np[2];
  x[0]=new float[maxp];  y[0]=new float[maxp];
  x[1]=new float[maxp];  y[1]=new float[maxp];
  for(i=0;i<old->getnumcontours()-1;i++)
      {
	int goahead=1;
	int nextc=1;
	
	if (old->getcontour(i)->getnumpoints()==0)
	    {
	      goahead=0;
	    }
	else 
	    {
	      int j=i+1;
	      goahead=0;
	      while (j<old->getnumcontours() && goahead==0)
		  {
		    if (old->getcontour(j)->getnumpoints()==0)
			j++;
		    else
			goahead=1;
		  }
	      nextc=j;
	    }
	
	if (goahead==1)
	    {
	      for (int c=0;c<=1;c+=1)
		  {
		    int cnt=i;
		    if (c==1)
			cnt=nextc;
		    np[c]=old->getcontour(cnt)->getnumpoints();
		    PXContourPoint* pts=(old->getcontour(cnt))->gettoppoint();
		    for(int ia=0;ia<np[c];ia++)
			{
			  x[c][ia]=pts->x;   y[c][ia]=pts->y; 
				pts=pts->next;
			}
		  }
	      
	      PXTriangulatedStack::findorigin2D(np,x,y,origin);
	      int numfaces=PXTriangulatedStack::triangulate2D(np,x,y,origin,
							      facelist,np[0]+np[1]+1);
		  for (int k=0;k<numfaces;k++)
		      {
			PXNeighboursNode* current=facelist->getcurrentnode();
			PXIntegerList* list=(current->points);
			list->Rewind();
			for (int ia=0;ia<=2;ia++)
			    {
			      triangle[k+basetr].p[ia]=list->getcurrent()+basept;
			      list->Next();
			    }
			facelist->Next();
		      }
	      facelist->DeleteAll();
	      
	      basept+=np[0];
	      basetr+=numfaces;
	    }
      }
  delete [] x[0]; delete [] y[0];
  delete [] x[1]; delete [] y[1];
  delete facelist;
  generateNormals();
  initialiseStackIndex(old);
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::generateNormals()
{
  if (!haspoints() || !hastriangles())
      {
	/*fprintf(stderr,"Cannot generate normals as points=%d , triangles=%d\n",
		haspoints(),hastriangles());*/
	return 0;
      }
  
  PXIntegerList *belong=new PXIntegerList[numpoints];
  for (int tr=0;tr<numtriangles;tr++)
      for (int ia=0;ia<=2;ia++)
	  belong[triangle[tr].p[ia]].Add(tr);
  
  for (int i=0;i<numpoints;i++)
      {
	float nx=0.0,ny=0.0,nz=0.0;
	belong[i].Rewind();
	for (int tr=0;tr<belong[i].getnumnodes();tr++)
	    {
	      int tri=belong[i].getcurrent();
	      belong[i].Next();
	      PXTriangulatedStackPoint* p0=getpoint(triangle[tri].p[0]);
	      PXTriangulatedStackPoint* p1=getpoint(triangle[tri].p[1]);
	      PXTriangulatedStackPoint* p2=getpoint(triangle[tri].p[2]);
	      
	      // Offset Coordinates
	      for (int ia=0;ia<=2;ia++)
		  {
		    p2->x[ia]=p2->x[ia]-p1->x[ia];  p1->x[ia]=p1->x[ia]-p0->x[ia];
		  }
	      
	      float lx=p2->x[1]*p1->x[2]-p1->x[1]*p2->x[2];
	      float ly=p2->x[2]*p1->x[0]-p1->x[2]*p2->x[0];
	      float lz=p2->x[0]*p1->x[1]-p1->x[0]*p2->x[1];
	      float mag=sqrt(lx*lx+ly*ly+lz*lz)*float(belong[i].getnumnodes());
	      
	      nx-=lx/mag; ny-=ly/mag; nz-=lz/mag;
	      
	      for (int ib=0;ib<=2;ib++)
		  {
		    p1->x[ib]=p1->x[ib]+p0->x[ib];  p2->x[ib]=p2->x[ib]+p1->x[ib];
		  }
	    }
	point[i].nx[0]=nx;  point[i].nx[1]=ny;     point[i].nx[2]=nz;
      }
  normalsok=1;
  delete [] belong;
  return 1;
}
// ------------------------------------------------------------------------
PXContourStack* PXTriangulatedStack::exporttocontourstack(int skiplevels,int skipapex)
{
  if (!hasstackindex())
    {
      fprintf(stderr,"Failed to export \n");
      return NULL;
    }

  if (debugmode)
      fprintf(stderr,"skiplevels=%d\n",skiplevels);
  skiplevels=Irange(skiplevels,0,stackIndex.numcontours);

  int level=0;

  if (skipapex)
      {
	// Find Apical bottom and cut out
	// This is going to be a bug !!
	if (debugmode)
	    fprintf(stderr,"Cannot skip apex\n");
      }
  else
      level=0;
	
  if (debugmode)
      fprintf(stderr,"skiplevels=%d\n",skiplevels);
  int numl=0,i;
  for (i=level;i<stackIndex.numcontours;i+=skiplevels+1)
      numl++;
  
  if(debugmode)
      fprintf(stderr,"Tstack->Cstack Numlevels=%d\n",numl);
  PXContourStack* stack=new PXContourStack(numl);

  for (i=0;i<stack->getnumcontours();i++)
      {
	stack->setzlevel(i,stackIndex.zlevel[level]);
	if (debugmode)
	    fprintf(stderr,"Zlevels %d <--%d = %6.2f <-- %6.2f \n",i,level,stackIndex.zlevel[level],stack->getzlevel(i));
	PXContour* cntr=stack->getcontour(i);
	for (int j=stackIndex.index[level];j<stackIndex.index[level+1];j++)
	    cntr->Add(getpoint(j)->x[0],getpoint(j)->x[1]);
	cntr->setclosed(1);
	level+=skiplevels+1;
      }
  return stack;
}
// ------------------------------------------------------------------------
void PXTriangulatedStack::cleanup()
{
  //fprintf(stderr,"In Cleanup\n");

  if (haspointstructures)
      {
	delete [] point;
	haspointstructures=0;
      }

  //fprintf(stderr,"Done with Points\n");

  if (hastrianglestructures)
      {
	delete [] triangle;
	hastrianglestructures=0;
      }
  
  //fprintf(stderr,"Done with Triangles\n");

  if (hasneighbourstructures)
      {
	cleanupneighbours();
	hasneighbourstructures=0;
      }

  //fprintf(stderr,"Done with Neighbours\n");

  if (stackindexok)
      {
	if (stackIndex.index!=NULL)
	    delete [] stackIndex.index;
	if (stackIndex.zlevel!=NULL)
	    delete [] stackIndex.zlevel;
	stackindexok=0;
      }
  //fprintf(stderr,"Done with Stack\n");

  init(0,0);

  //fprintf(stderr,"Done with cleanup\n");


}
// ------------------------------------------------------------------------
void PXTriangulatedStack::cleanupneighbours()
{
  if (hasneighbourstructures)
      {
	if (neighboursList!=NULL)
	  {
	    delete neighboursList;
	  }
	if (neighbours!=NULL)
	  delete []  neighbours;
      }
  maxconnection=0;
}
// ------------------------------------------------------------------------
void PXTriangulatedStack::init(int np,int nt)
{
  trianglecentroidsok=0; stackindexok=0; stackIndex.numcontours=0;
  haspointstructures=0; hastrianglestructures=0; hasneighbourstructures=0; 
  normalsok=0; curvaturesok=0; maxconnection=0;
  numpoints=0; numtriangles=0; useindex=0; 
  point=NULL;
  triangle=NULL;
  neighboursList=NULL;
  stackIndex.index=NULL;
  stackIndex.zlevel=NULL;
  neighbours=NULL;

  if (np>0)
      {
	numpoints=np;
	point=new PXTriangulatedStackPoint[np];
	for (int i=0;i<np;i++)
	    {
	      point[i].index=i;
	      for (int ia=0;ia<=2;ia++)
		  {
		    point[i].x[ia]=0.0; 
		    point[i].nx[ia]=0.0;
		  }
	      for (int ib=0;ib<=1;ib++)
		  point[i].k[ib]=0.0;
	    }
	haspointstructures=1;
      }
  if (nt>0)
      {
	numtriangles=nt;
	triangle=new PXTriangle[nt];
	for (int i=0;i<nt;i++)
	    {
	      triangle[i].index=i;
	      for (int j=0;j<=2;j++)
		  {
		    triangle[i].p[j]=-1;
		    triangle[i].c[j]=0.0;
		  }
	    }
	hastrianglestructures=1;
      }
}

// ------------------------------------------------------------------------
void PXTriangulatedStack::initialiseStackIndex(PXContourStack* old)
{
  if (hasstackindex())
      {
	//fprintf(stderr,"Cleaning ip stackIndex\n");
	delete [] stackIndex.index;
	delete [] stackIndex.zlevel;
      }
  else
      stackindexok=1;
  
  stackIndex.numcontours=old->getnumcontours();
  stackIndex.index=new int[stackIndex.numcontours+1];
  stackIndex.zlevel=new float[stackIndex.numcontours];
  stackIndex.index[0]=0;
  for (int i=1;i<=stackIndex.numcontours;i++)
      stackIndex.index[i]=stackIndex.index[i-1]+(old->getcontour(i-1))->getnumpoints();
  for (int j=0;j<stackIndex.numcontours;j++)
      stackIndex.zlevel[j]=old->getzlevel(j);
}
// ------------------------------------------------------------------------
PXTriangulatedStackPoint* PXTriangulatedStack::getpoint(int pointidx)
{
  if (haspoints())
      {
	pointidx=Irange(pointidx,0,numpoints-1);
	return &point[pointidx];
      }
  
  return NULL;
}
// ------------------------------------------------------------------------
PXIntegerList* PXTriangulatedStack::getneighbours(int idx)
{
  if (hasneighbours())
      {
	idx=Irange(idx,0,numpoints-1);
	return &neighbours[idx];
      }
  
  return NULL;
}
// ------------------------------------------------------------------------
PXTriangle* PXTriangulatedStack::gettriangle(int triangleidx)
{
  if (hastriangles())
      {
	triangleidx=Irange(triangleidx,0,numtriangles-1);
	return &triangle[triangleidx];
      }

  return NULL;

}
// ------------------------------------------------------------------------
int PXTriangulatedStack::SaveAbaqusRigid(const char* filename,int offset,int index)
{
  int ok=0;
  FILE* fout=fopen(filename,"w");
  if (fout)
      {
	ok=SaveAbaqusRigid(fout,offset,index);
	fclose(fout);
      }
  return ok;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::SaveAbaqusRigid(FILE* fout,int offset,int index)
{
  if (!fout)
      return 0;
  fprintf(fout,"*NODE,NSET=NSURF_%02d\n",index);
  int i;
  for (i=0;i<numpoints;i++)
      {
	fprintf(fout,"%6d, %6.3f, %6.3f, %6.3f\n",i+offset,
		point[i].x[0],point[i].x[1],point[i].x[2]);
      }
  fprintf(fout,"*ELEMENT, TYPE=R3D3,ELSET=ESURF_%02d\n",index);
  for (i=0;i<numtriangles;i++)
      {
	fprintf(fout,"%6d, %6d, %6d, %6d\n",
		i+offset,
		triangle[i].p[0]+offset,triangle[i].p[1]+offset,
		triangle[i].p[2]+offset);
      }
  fprintf(fout,"*RIGID BODY,ELSET=ESURF_%02d,REF NODE=%d\n",index,offset);
  fprintf(fout,"*SURFACE DEFINITION,NAME=RGDS_%02d\n",index);
  fprintf(fout,"ESURF_%02d,SPOS\n",index);
  return (1);
}
// ------------------------------------------------------------------------
PX3Vector* PXTriangulatedStack::nonShrinkSmoothD(float alpha,float beta,int iterations,
						 int movez,int )
{
  PX3Vector* disp=new PX3Vector[numpoints];
  int i;
  for (i=0;i<numpoints;i++)
      for (int ia=0;ia<=2;ia++)
	  disp[i].x[ia]=point[i].x[ia];  
  
  nonShrinkSmooth(alpha,beta,iterations,movez);
  
  for (i=0;i<numpoints;i++)
      for (int ia=0;ia<=2;ia++)
	  disp[i].x[ia]=point[i].x[ia]-disp[i].x[ia];  

  return disp;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::nonShrinkSmooth(float alpha,float beta,int iterations,int movez,
					 int uniform)
{
  int st=1;
  if (!hasneighbourstructures)
      st=generateNeighbours();

  if (st==0)
      return 0;

  movez=Irange(movez,0,2);
  uniform=Irange(uniform,0,1);

  PX3Vector* temp=new PX3Vector[numpoints];
  
  float minz=point[0].x[2];
  float maxz=point[0].x[2];

  if (movez==1)
      {
	for (int i=0;i<numpoints;i++)
	    {
	      minz=Fmin(minz,point[i].x[2]);
	      maxz=Fmax(maxz,point[i].x[2]);
	    }
      }

  if (hasstackindex())
      {
	// If we have an apex smooth its z coordinate as well 
	if (stackIndex.index[1]==1)
	    minz-=1.0;
      }

	      
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
	      int i;
	      for (i=0;i<numpoints;i++)
		  {
		    for (int ia=0;ia<=2;ia++)
			temp[i].x[ia]=point[i].x[ia];  
		  }
	      
	      for(i=0;i<numpoints;i++)
		  {
		    float oldx[3],newx[3];
		    for (int ia=0;ia<=2;ia++)
			{
			  oldx[ia]=temp[i].x[ia];
			  newx[ia]=0.0;
			}
		    float total=0.0;
		    
		    for(int j=0;j<(neighbours[i].getnumnodes());j++)
			{
			  int p=neighbours[i].getcurrent();
			  for (int ia=0;ia<=2;ia++)
			      newx[ia]+=temp[p].x[ia];
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

			  for (int ib=0;ib<=1;ib++)
			      point[i].x[ib]=(1.0-factor)*oldx[ib]+factor*newx[ib];

			  switch(movez)
			      {
			      case 1:
				if (point[i].x[2]>minz+0.01 && point[i].x[2]<maxz-0.01)
				    point[i].x[2]=(1.0-factor)*oldx[2]+factor*newx[2];
				break;
			      case 2:
				point[i].x[2]=(1.0-factor)*oldx[2]+factor*newx[2];
				break;
			      default:
				point[i].x[2]=oldx[2];
				break;
			      }
			}
		  }
	    }
      }
  delete [] temp;  

  if (debugmode)
      fprintf(stderr,"\n");
  return 1;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::calculateCentroids()
{
  if (hastrianglestructures)
      {
	if (debugmode)
	    fprintf(stderr,"Calculating Centroids\n");
	for (int tr=0;tr<numtriangles;tr++)
	    {
	      float x=0.0,y=0.0,z=0.0;
	      for (int pt=0;pt<=2;pt++)
		  {
		    int index=triangle[tr].p[pt];
		    x+=point[index].x[0];
		    y+=point[index].x[1];
		    z+=point[index].x[2];
		  }
	      triangle[tr].c[0]=x/3.0;
	      triangle[tr].c[1]=y/3.0;
	      triangle[tr].c[2]=z/3.0;
	    }
	trianglecentroidsok=1;
      }
  return (trianglecentroidsok);
}
// ------------------------------------------------------------------------
float PXTriangulatedStack::findnearestpoint(float&x,float& y,float &z,int &rpt,
					    int interpolate,float maxdist)
{

  if (numpoints==0)
      {
	if (debugmode)
	    fprintf(stderr,"No points in tstack\n");
	exit(1);
      }

  rpt=0;
  //  static int count=0;
  float mindist2=200000.0;
  float maxdist2=maxdist*maxdist;

  //  count++;
  if (interpolate<1)
      {
	int bestpt=0,pt=0,maxp=numpoints,minp=0;	
	int besti=0;
	int imin=0;
	int imax=stackIndex.numcontours;
	int above=0,below=0;

	if (hasstackindex())
	    {
	      // Chances are good that we are looking within a slice or two 
	      // Quickscan
	      int found=0;
	      if (z<point[0].x[2])
		  {
		    pt=0;
		    maxp=stackIndex.index[1];
		    found=1;
		    below=1;
		  }
	      
	      int i=0;
	      while(i<stackIndex.numcontours-1 && found==0)
		  {
		    if (z>=stackIndex.zlevel[i] && z<stackIndex.zlevel[i+1])
			found=1;
		    else 
			i++;
		  }	      
	      
	      if (found==0)
		  {
		    above=1;
		    i=stackIndex.numcontours-1;
		    imin=i;
		    imax=i+1;
		  }
	      else
		  {
		    besti=i;
		    imin=Irange(besti,0,stackIndex.numcontours);
		    imax=Irange(besti+1,imin+1,stackIndex.numcontours);
		  }
	    }
	
	pt=stackIndex.index[imin];
	maxp=stackIndex.index[imax];
	minp=pt;

	
	int done=0;
	int oldpt=maxp+1;
	int oldmaxp=maxp+1;
	int iminold=imin;
	int imaxold=imax;

	

	float zmin=point[Irange(minp-1,0,numpoints-1)].x[2];
	float zmax=point[Irange(maxp,0,numpoints-1)].x[2];

	//int first=0;
	     
	while(done==0)
	    {
	      /*first++;
	      if (first>=5)
		  {
		    if (first==5)
			fprintf(stderr,"----------above=%d,below=%d %6.3f %6.3f %6.3f\n",above,below,x,y,z);
		    fprintf(stderr,"Expanding range from %d:%d to %d %d or %d:%d to %d:%d\n",
			    iminold,imaxold,imin,imax,
			    oldpt,oldmaxp,pt,maxp-1);
		    fprintf(stderr,"\n\t Running in %d,%d (imin=%d,imax=%d) or z=%6.3f:%6.3f\n",
			    pt,maxp-1,imin,imax,zmin,zmax);
		  }*/
	
	      while (pt<maxp  && mindist2>maxdist2)
		  {
		    if (pt==oldpt)
			{
			  /*  if (first>=5)
			      fprintf(stderr,"Skipping from %d to %d\n",oldpt,oldmaxp-1);*/
			  pt=oldmaxp;
			}
		    float dist2=Fsqdist(x,y,z,
					point[pt].x[0],point[pt].x[1],point[pt].x[2]);
		    if (dist2<mindist2)
			{
			  bestpt=pt;
			  mindist2=dist2;
			}
		    pt++;
		  }
	      
	      float zminrange=Fmin(fabs(z-zmin),fabs(zmax-z));
	      if (above==1)
		  zminrange=z-zmin;
	      else if (below==1)
		  zminrange=zmax-z;

	      /*if (first>=5)
		  fprintf(stderr,"zrange=%6.3f, (zmin=%6.3f zmax=%6.3f z=%6.3f) mindist=%6.3f\n",zminrange,
			  zmin,zmax,z,sqrt(mindist2));*/
	      if (zminrange>sqrt(mindist2))
		  {
		    done=1;
		  }
	      else
		  {
		    iminold=imin;
		    imaxold=imax;
		    if ((fabs(z-zmin)<fabs(zmax-z)))
			{
			  imin=Irange(imin-1,0,stackIndex.numcontours);
			  if (imin==iminold)
			      {
				imax=Irange(imax+1,imin+1,stackIndex.numcontours);
				below=1;
			      }
			}
		    else
			{
			  imax=Irange(imax+1,imin+1,stackIndex.numcontours);
			  if (imax==imaxold)
			      {
				imin=Irange(imin-1,0,stackIndex.numcontours);
				above=1;
			      }
			}
		    
		    if (imin==iminold && imax==imaxold)
			{
			  done=1;
			}
		    else
			{
			  oldpt=minp;
			  oldmaxp=maxp;
			  pt=stackIndex.index[imin];
			  maxp=stackIndex.index[imax];
			  zmin=point[Irange(pt-1,0,numpoints-1)].x[2];
			  zmax=point[Irange(maxp,0,numpoints-1)].x[2];			  
			}
		  }
	    }
	//for (int ia=0;ia<=2;ia++)
	//xp[ia]=point[bestpt].x[ia];
	// rpt=bestpt;
	//x=xp[0]; y=xp[1]; z=xp[2];
	rpt=bestpt;
	x=point[bestpt].x[0];	y=point[bestpt].x[1];	z=point[bestpt].x[2];
      }
  else
      {
	if (!hascentroids())
	    calculateCentroids();

	int tr=0,besttr=0;
	while (tr<numtriangles && mindist2>maxdist2)
	    {
	      float dist2=Fsqdist(x,y,z,//p[0],xp[1],xp[2],
				  triangle[tr].c[0],triangle[tr].c[1],triangle[tr].c[2]);
	      if (dist2<mindist2)
		  {
		    besttr=tr;
		    mindist2=dist2;
		  }
	      tr++;
	    }
	float dist[3],totaldist=0.0;
	
	int org=triangle[besttr].p[0],pt;
	PX3Vector nod[3];
	for (int ia=0;ia<=2;ia++)
	    nod[ia].setvector(point[org].x[ia],point[org].x[ia],point[org].x[ia]);
	PX3Vector vertex(x,y,z);//xp[0],xp[1],xp[2]);
	for (pt=0;pt<=2;pt++)
	    {
	      int pt1=Icyclic(pt+1,3);
	      int pt2=Icyclic(pt+2,3);

	      PX3Vector leg1,leg2;
	      leg1.copy(&nod[pt1]);       leg2.copy(&nod[pt2]);
	      leg1.subtract(&vertex);     leg2.subtract(&vertex);
	      leg1.crosswithoverwrite(&leg2);
	      dist[pt]=leg1.getlength();
	      totaldist+=dist[pt];
	    }
	float px[3];
	for (int ib=0;ib<=2;ib++)
	    px[ib]=0.0;
	
	for (pt=0;pt<=2;pt++)
	    {
	      int index=triangle[besttr].p[pt];
	      for (int ia=0;ia<=2;ia++)
		  px[ia]+=dist[pt]/totaldist*point[index].x[ia];
	    }

	// Find Closest Point as well 
	float mindist=dist[0];
	rpt=triangle[besttr].p[0];

	for (int ic=1;ic<=2;ic++)
	    {
	      if(dist[ic]<mindist)
		  {
		    mindist=dist[ic];
		    rpt=triangle[besttr].p[ic];
		  }
	    }
	mindist2=Fdist(px[0],px[1],px[2],x,y,z);//xp[0],xp[1],xp[2]);
	x=px[0];y=px[1];z=px[2];
      }

  //  if (count==50)      count=0;
  return sqrt(mindist2);
}
// ------------------------------------------------------------------------
void PXTriangulatedStack::deformUsingDisplacements(PX3Vector *dx,float factor,
						   int smoothiter)
{
  PX3Vector *temp=NULL;
  /*if (fabs(factor)<0.01)
      factor=0.01;*/

  if (smoothiter>0)
      {
	temp=new PX3Vector[numpoints];
	for (int i=0;i<numpoints;i++)
	    for (int ia=0;ia<=2;ia++)
		temp[i].x[ia]=point[i].x[ia];  
      }

  for (int i=0;i<numpoints;i++)
      for (int ia=0;ia<=2;ia++)
	  point[i].x[ia]=point[i].x[ia]+factor*dx[i].x[ia];
  
  if (smoothiter>0)
      {
	if (debugmode)
	    fprintf(stderr,"Smoothing Deformed Stack %d\n",smoothiter);
	nonShrinkSmooth(0.33,-0.34,smoothiter,2);
	for (int i=0;i<numpoints;i++)
	    for (int ia=0;ia<=2;ia++)
		dx[i].x[ia]=(point[i].x[ia]-temp[i].x[ia])/factor;

	delete [] temp;
      }

  if (hascentroids())
      calculateCentroids();

}
// ------------------------------------------------------------------------
void PXTriangulatedStack::findbestpair(PXDispPair* pair,int npoints,
				       float& x,float& y,float& z,float threshold)
{
  int i=0;
  int found=0;
  int ip=0;
  float bestdist=10000000.0;

  while(found==0 && i<npoints)
      {
	float d=Fsqdist(x,y,z,pair[i].x1,pair[i].y1,pair[i].z1);
	if (d<bestdist)
	    {
	      ip=i;
	      bestdist=d;
	      if (Fsamepoint(x,y,z,pair[i].x1,pair[i].y1,pair[i].z1,threshold))
		  found=1;
	    }
	i++;
      }

  x=pair[ip].x2;
  y=pair[ip].y2;
  z=pair[ip].z2;

  /*  pair[ip].x1=-10000.0;
      pair[ip].y1=-10000.0;
      pair[ip].z1=-10000.0;*/
}
// ------------------------------------------------------------------------
void PXTriangulatedStack::deformUsingShapeDisplacements(const char* dispname,float threshold)
{
  //  int debugmode=1;
  PXDispPair* pairs;
  int  npoints;
  char line[100];

  // Read in Displacements
  // ---------------------
  FILE* fin=fopen(dispname,"r");
  if (fin!=NULL)
      {
	npoints=0; 
	while ( fgets(line,100,fin)!=NULL )    
	    npoints++;
	fclose(fin);
	/*	if (debugmode)
		fprintf(stderr,"\t%s (np=%d) ",anaeatpath(dispname),npoints);*/
	pairs=new PXDispPair[npoints];
	
	fin=fopen(dispname,"r");
	for (int i=0;i<npoints;i++)
	    fscanf(fin,"%f %f %f %f %f %f\n",
		   &pairs[i].x1,&pairs[i].y1,&pairs[i].z1,
		   &pairs[i].x2,&pairs[i].y2,&pairs[i].z2);
	fclose(fin);
      }
  else
      {
	if (debugmode)
	    fprintf(stderr,"Cannot Open %s\n",dispname);
	return;
      }
  
  if (debugmode)
      fprintf(stderr,"\n");
  
  // Find Matching Pairs of Neighbours, 
  // ---------------------------------------------------
  int numn=0;
  float dist=0.0;
  
  if (debugmode)
      fprintf(stderr,"Finding Best Pair :: (%d)",numpoints);

  for (int pt=0;pt<numpoints;pt++)
      {
	if (100*int(pt/100)==pt && debugmode)
	    fprintf(stderr,"%2d ",pt);
	float px=point[pt].x[0];
	float py=point[pt].x[1];
	float pz=point[pt].x[2];
	float px0=px,py0=py,pz0=pz;
	findbestpair(pairs,npoints,px,py,pz,threshold);
	point[pt].x[0]=px;
	point[pt].x[1]=py;
	point[pt].x[2]=pz;
	dist+=sqrt(pow(px-px0,float(2.0))+pow(py-py0,float(2.0))+pow(pz-pz0,float(2.0)));
	numn++;
      }
  
  if (numn==0)  numn++;

  if (debugmode)
      fprintf(stderr,"\n**Average Displacements=%6.4f(%d)\n",
	      dist/float(numn),numn);
	      
  // Cleanup at end 
  delete [] pairs;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::LoadOffset(const char* fname)
{
  int status=0;
  char line[200];
  FILE* fin=fopen(fname,"r");
  if (fin)
      {
	do 
	    {
	      fgets(line,100,fin);
	    }
	while (line[0]=='#');
	int st=strncmp(line, "NOFF",4);
	if (st==0)
	    {
	      int npoints,ntriangles,temp;
	      fscanf(fin,"%d %d %d\n",&npoints,&ntriangles,&temp);
	      cleanup();
	      init(npoints,ntriangles);
	      int i;
	      for (i=0;i<npoints;i++)
		  {
		    float x,y,z,nx,ny,nz;
		    fscanf(fin,"%f %f %f %f %f %f\n",&x,&y,&z,&nx,&ny,&nz);
		    point[i].index=i;
		    point[i].x[0]=x;   point[i].x[1]=y;   point[i].x[2]=z; 
		    point[i].nx[0]=nx; point[i].nx[1]=ny; point[i].nx[2]=nz; 
		  }
	      for (i=0;i<ntriangles;i++)
		  {
		    int dummy,n1,n2,n3;
		    fscanf(fin,"%d %d %d %d\n",&dummy,&n1,&n2,&n3);
		    triangle[i].index=i;
		    triangle[i].p[0]=n1; triangle[i].p[1]=n2; triangle[i].p[2]=n3; 
		  }
	      trianglecentroidsok=0;
	      normalsok=1;
	      status=1;
	    }
	fclose(fin);
      }
  return status;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::LoadOffset2(const char* fname)
{
  int status=0;
  FILE* fin=fopen(fname,"r");
  if (fin)
      {
	int temp;
	fscanf(fin,"%d\n",&temp);
	if (temp==3)
	    {
	      int npoints,ntriangles,temp;
	      fscanf(fin,"%d %d %d\n",&npoints,&ntriangles,&temp);
	      cleanup();
	      init(npoints,ntriangles);
	      int i;
	      for (i=0;i<npoints;i++)
		  {
		    float x,y,z;
		    fscanf(fin,"%f %f %f\n",&x,&y,&z);
		    point[i].index=i;
		    point[i].x[0]=x;   point[i].x[1]=y;   point[i].x[2]=z; 
		  }
	      for (i=0;i<ntriangles;i++)
		  {
		    int dummy,n1,n2,n3;
		    fscanf(fin,"%d %d %d %d\n",&dummy,&n1,&n2,&n3);
		    triangle[i].index=i;
		    triangle[i].p[0]=n1; triangle[i].p[1]=n2; triangle[i].p[2]=n3; 
		  }
	      trianglecentroidsok=0;
	      normalsok=1;
	      status=1;
	    }
	fclose(fin);
      }
  return status;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::Load(const char* filename)
{
  gzFile fin=gzsuffixopen(filename,"r");
  if (!fin)
    {
      fprintf(stderr,"Failed to open %s\n",filename);
      return 0;
    }
  int st=Load(fin);
  gzclose(fin);
  return st;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::Load(gzFile fin)
{
  char line[400];
  gzgets(fin,line,200);
  if (gzfindstringinstring(line, "#TriangulatedStack")==0)
    return 0;

  cleanup();

  int npoints,ntriangles,connect,norm,curv,ncntr;
  gzgets(fin,line,200); sscanf(line,"#%d\n",&ncntr);
  gzgets(fin,line,200); sscanf(line,"#%d\n",&npoints);
  gzgets(fin,line,200); sscanf(line,"#%d\n",&ntriangles);
  gzgets(fin,line,200); sscanf(line,"#%d\n",&connect);
  gzgets(fin,line,200); sscanf(line,"#%d\n",&norm);
  gzgets(fin,line,200); sscanf(line,"#%d\n",&curv);

  init(npoints,ntriangles);

  stackindexok=ncntr>0;
  stackIndex.numcontours=ncntr;

  hasneighbourstructures=(connect>0);
  maxconnection=connect;
  curvaturesok=(curv>0);
  normalsok=(norm>0);

  if (hasstackindex())
      {
	//  stackindexok=ncntr>0;
	stackIndex.index=new int[stackIndex.numcontours+1];
	stackIndex.zlevel=new float[stackIndex.numcontours];
	gzgets(fin,line,200); 
	for (int i=0;i<stackIndex.numcontours;i++)
	    {
	      gzgets(fin,line,200);
	      sscanf(line,"%d %f\n",&stackIndex.index[i],&stackIndex.zlevel[i]);
	    }
	stackIndex.index[stackIndex.numcontours]=numpoints;
      }

  

  if (haspoints())
      {
	int i;
	gzgets(fin,line,200); 
	for (i=0;i<numpoints;i++)
	    {
	      gzgets(fin,line,200);
	      sscanf(line,"%d %f %f %f %f %f %f %f %f\n",
		     &point[i].index,&point[i].x[0],&point[i].x[1],&point[i].x[2],
		     &point[i].nx[0],&point[i].nx[1],&point[i].nx[2],&point[i].k[0],&point[i].k[1]);
	      if (curvaturesok==0)
		  {
		    for (int ia=0;ia<=1;ia++)
			point[i].k[ia]=0.0;
		  }
	      if (normalsok==0)
		  {
		    for (int ia=0;ia<=2;ia++)
			point[i].nx[ia]=1.0*(ia==0);
		  }
	    }
	      
	if (hastriangles())
	    {
	      gzgets(fin,line,200);
	      for (i=0;i<numtriangles;i++)
		  {
		    gzgets(fin,line,200);
		    sscanf(line,"%d %d %d %d\n",&triangle[i].index,
			   &triangle[i].p[0],&triangle[i].p[1],&triangle[i].p[2]);
		  }
	      //fprintf(stderr,"Last Triangle = %s\n",line);
	      if (hasneighbours())
		  {
		    neighbours=new PXIntegerList[numpoints];
		    neighboursList=new PXNeighboursList();
		    gzgets(fin,line,200);
		    for (i=0;i<numpoints;i++)
			{
			  int temp,num;
			  gzgets(fin,line,400);
			  sscanf(line,"%d %d ",&temp,&num);
			  int po=0;
			  po=stringskipspace(line,po);
			  po=stringskipnumber(line,po);
			  po=stringskipspace(line,po);
			  po=stringskipnumber(line,po);

			  for(int j=0;j<num;j++)
			    {
				po=stringskipspace(line,po);
				sscanf(&line[po],"%d ",&temp);
				po=stringskipnumber(line,po);
				neighbours[i].Add(temp);
			    }

			  /*if (i<30)
			    {
			      int num=neighbours[i].getnumnodes();
			      neighbours[i].Rewind();
			      fprintf(stderr,"pt=%d \n",i);
			      for (int ii=0;ii<num;ii++)
				{
				  fprintf(stderr,"%d ",neighbours[i].getcurrent());
				  neighbours[i].Next();
				}
			      fprintf(stderr,"\n");
			      }*/

			  neighboursList->Add(&neighbours[i],i);
			}
		  }
	    }
	else if (debugmode)
	    fprintf(stderr,"No Triangles\n");
      }
  else if (debugmode)
      fprintf(stderr,"Does not have any points\n");
  return 1;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::Save(const char* filename)
{
  int st=0;
  FILE* fout=fopen(filename,"w");
  if (fout==NULL)
    return 0;
  st=Save(fout);
  fclose(fout);
  return st;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::Save(FILE* fout)
{
  fprintf(fout,"#TriangulatedStack File,numcontours,numpoints,numtriangles,maxconnection,hasnormals,hascurvatures\n");
  fprintf(fout,"#%d\n",stackIndex.numcontours);
  fprintf(fout,"#%d\n",numpoints);
  fprintf(fout,"#%d\n",numtriangles);
  fprintf(fout,"#%d\n",maxconnection);
  fprintf(fout,"#%d\n",normalsok);
  fprintf(fout,"#%d\n",curvaturesok);
  
  if (hasstackindex())
      {
	fprintf(fout,"#--------------Contour Indices-------------------------------------\n");
	for (int i=0;i<stackIndex.numcontours;i++)
	    fprintf(fout,"%-6d %-8.3f\n",stackIndex.index[i],stackIndex.zlevel[i]);
      }

  if (haspoints())
      {
	fprintf(fout,"#--------------Points-----------------------------------------------\n");
	for (int i=0;i<numpoints;i++)
	    {
	      fprintf(fout,"%6d %7.3f %7.3f %7.3f  %5.3f %5.3f %5.3f %10.6f %10.6f\n",
		      point[i].index,point[i].x[0],point[i].x[1],point[i].x[2],
		      point[i].nx[0],point[i].nx[1],point[i].nx[2],
		      point[i].k[0],point[i].k[1]);
	    }

	if (hastriangles())
	    {
	      fprintf(fout,"#--------------Triangles--------------------------------------------\n");
	      for (int i=0;i<numtriangles;i++)
		  {
		    fprintf(fout,"%6d %6d %6d %6d\n",triangle[i].index,
			    triangle[i].p[0],triangle[i].p[1],triangle[i].p[2]);
		  }
	      if (hasneighbours())
		{
		  fprintf(fout,"#--------------Neighbours-------------------------------------------\n");
		  for (int i=0;i<numpoints;i++)
		    {
		      neighbours[i].Rewind();
		      int numn=neighbours[i].getnumnodes();
		      fprintf(fout,"%-6d %2d ",i,numn);
		      //fprintf(stderr,"%-6d %2d ",i,numn);
		      for(int j=0;j<numn;j++)
			{
			  int a=neighbours[i].getcurrent();
			  //  fprintf(stderr,"%-6d ",a);
			  fprintf(fout,"%-6d ",a);
			  neighbours[i].Next();
			}
		      fprintf(fout,"\n");
		      //fprintf(stderr,"\n");
		    }
		}
	    }
      }
  return(1);
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::generateNeighbours(int openSurfaceMode)
{

  if (!haspoints() || !hastriangles())
    {
      if (debugmode)
	fprintf(stderr,"Cannot generate neighbours as points=%d , triangles=%d\n",
		haspoints(),hastriangles());
      return 0;
    }
  //  int* buffer=new int[200];
  maxconnection=0;

  cleanupneighbours();
  neighboursList=new PXNeighboursList();
  neighbours=new PXIntegerList[numpoints];
  
  if (debugmode)
    fprintf(stderr,"Generating Neighbours (pt=%d,tr=%d):: ",numpoints,numtriangles);
  
  openSurfaceMode=Irange(openSurfaceMode,0,1);
  int bt=0,tp=0;
  if (!hasstackindex())
    {
      openSurfaceMode=0;
    }
  else
    {
      bt=stackIndex.index[1];
      // Accept apex, i.e. normal surface on bottom 
      if (bt==1)
	bt=0;
      tp=stackIndex.index[stackIndex.numcontours-1];
    }
  
  for (int tr=0;tr<numtriangles;tr++)
    {
      if (debugmode>0)
	{
	  if (100*int(tr/100)==tr)
	    fprintf(stderr,"%d/%d ",tr,numtriangles);
	}
      
      for (int k=0;k<=2;k++)
	{
	  int pt=triangle[tr].p[k];
	  for (int j=0;j<=2;j++)
	    {
	      if (!openSurfaceMode)
		{
		  neighbours[pt].AddNonDuplicate(triangle[tr].p[j],pt);
		}
	      else
		{
		  if (pt>=bt && pt<tp)
		    {
		      neighbours[pt].AddNonDuplicate(triangle[tr].p[j],pt);
		    }
		  else if (pt<bt && triangle[tr].p[j]<bt)
		    {
		      neighbours[pt].AddNonDuplicate(triangle[tr].p[j],pt);
		    }
		  else if (pt>=tp && triangle[tr].p[j]>=tp)
		    {
		      neighbours[pt].AddNonDuplicate(triangle[tr].p[j],pt);
		    }
		}
	    }
	}
    }
  
  for (int pt=0;pt<numpoints;pt++)
    {
      neighboursList->Add(&neighbours[pt],pt);
      maxconnection=Imax(maxconnection,neighbours[pt].getnumnodes());
    }
  if (debugmode)
    fprintf(stderr,"\n");
  hasneighbourstructures=1;
  return 1;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::calculateCurvatures(int scale)
{
  int mode=0;

  if (scale==-1000)
      {
	// Debug mode
	if (debugmode)
	    fprintf(stderr,"Curvature external force mode\n");
	curvaturesok=1;
	normalsok=1;
	return 0;
      }

  if (scale<0)
      {
	mode=1;
	scale=-scale;
      }

  scale=Irange(scale,1,10);
  if (mode==1 && debugmode)
      fprintf(stderr,"Curvature mode=%d, scale=%d\n",mode,scale);
  int st=1;
  if (!hasneighbours())
      st=generateNeighbours();

  if (st==0)
      {
	curvaturesok=0;
	return 0;
      }

  Vertex_3d neighbour_point[500],xformed_neighbour_point[500];
  int neighbour_index[500];

  Node_3d N, T1, T2, T;
  double c[4], length;
  double x, y, z;

  double fx, fy, fxx, fxy, fyy;
  //  Node_3d k1, k2;
  double K1, K2;
  int nrot;
  
  double** A     = dmatrix(0, 500-1, 0, 5);
  double** At    = dmatrix(0, 5, 0, 500-1);
  double** B     = dmatrix(0, 5, 0, 5);
  double** B_1   = dmatrix(0, 5, 0, 5);
  int*     indx  = ivector(0, 5);
  double** C     = dmatrix(0, 5, 0, 0);
  double** C2    = dmatrix(0, 5, 0, 0);
  double*  coeff = dvector(0, 5);
  double** data  = dmatrix(0, 500-1, 0, 0);
  
  double** g     = dmatrix(0, 1, 0, 1);
  double** g_1   = dmatrix(0, 1, 0, 1);
  double** b     = dmatrix(0, 1, 0, 1);
  double** beta  = dmatrix(0, 1, 0, 1);
  double ** a1_repl = new double*[2];
  a1_repl[0] = new double[2];
  a1_repl[1] = new double[2];

  double ** v1_repl = new double*[2];
  v1_repl[0] = new double[2];
  v1_repl[1] = new double[2];

  double* d1_repl=new double[2];


  
  
  for(int i=0; i<numpoints; i++)
    {
      if (debugmode)
	{
	  if (100*int(i/100)==i)
	    fprintf(stderr,"%d/%d ",i,numpoints);
	}
      int j;
      for(j=0; j<500; j++)
	{
	  neighbour_point[j].x   = 0.0;  neighbour_point[j].y   = 0.0;
	  neighbour_point[j].z   = 0.0;
	  neighbour_point[j].K   = 0.0;   neighbour_point[j].H   = 0.0;
	  neighbour_point[j].k1  = 0.0;  neighbour_point[j].k2  = 0.0;
	  neighbour_point[j].k1x = 0.0;  neighbour_point[j].k1y = 0.0;
	  neighbour_point[j].k1z = 0.0;
	  neighbour_point[j].k2x = 0.0;  neighbour_point[j].k2y = 0.0;
	  neighbour_point[j].k2z = 0.0;  neighbour_point[j].nx  = 0.0;
	  neighbour_point[j].ny  = 0.0;  neighbour_point[j].nz  = 0.0;
	}
      
      /* find the neighbouring points within the defined radius */
      neighbour_index[0] = i;
      int k = 1;
      
      /* run through the layers of neighbours */
      int num1 = 0, num2 = k;
      for(int s=1; s<=scale; s++)
	{
	  for(int j=num1; j<num2; j++)
	    {
	      neighbours[neighbour_index[j]].Rewind();
	      for(int m=0; m<neighbours[neighbour_index[j]].getnumnodes(); m++)
		{
		  int index=neighbours[neighbour_index[j]].getcurrent();
		  int flag = 0;
		  for (int n=0;n<k;n++)
		    {
		      if (index==neighbour_index[n])
			flag=1;
		    }
		  if (flag==0)
		    {
		      neighbour_index[k]=index;
		      k++;
		    }
		  neighbours[neighbour_index[j]].Next();
		}
	    }
	  num1 = num2;
	  num2 = k;
	}
      int num_neighbours = k;
      if (debugmode)
	{
	  if (100*int(i/100)==i)
	    fprintf(stderr,"num_neighbours[%d] = %d\t", i, num_neighbours);
	}
      
      /*for(j=0; j<num_neighbours; j++)
	neighbour_point[j] = point[neighbour_index[j]];*/
      for (j=0;j<num_neighbours;j++)
	{
	  neighbour_point[j].x =point[neighbour_index[j]].x[0];
	  neighbour_point[j].y =point[neighbour_index[j]].x[1];
	  neighbour_point[j].z =point[neighbour_index[j]].x[2];
	  neighbour_point[j].nx=point[neighbour_index[j]].nx[0];
	  neighbour_point[j].ny=point[neighbour_index[j]].nx[1];
	  neighbour_point[j].nz=point[neighbour_index[j]].nx[2];
	}
      
      if(num_neighbours<5)
	{
	  if (debugmode)
	    {
	      fprintf(stderr,"point no.%d: very few neighbours: %d\n", i, num_neighbours);
	      fprintf(stderr,"add three interpolated points\n");
	    }
	  /* there are at least four neighbours (including myself) */
	  /* add three neighbours from the interpolation between   */
	  /* me and the three existing neighbours. 		*/
	  /* (need at least five neighbours 			*/ 
	  for(j=0; j<3; j++){
	    neighbour_point[j+num_neighbours].x 
	      = (neighbour_point[0].x + neighbour_point[j].x) / 2.0;
	    neighbour_point[j+num_neighbours].y 
	      = (neighbour_point[0].y + neighbour_point[j].y) / 2.0;
	    neighbour_point[j+num_neighbours].z 
	      = (neighbour_point[0].z + neighbour_point[j].z) / 2.0;
	  }
	}
      if(num_neighbours>499 && debugmode)
	{
	  fprintf(stderr,"point no.%d: too many neighbours: %d\n", i, num_neighbours);
	  fprintf(stderr,"do you really need that many neighbours??\n");
	}
      
      
      /* the normal vector at point[i] */
      N.x = point[i].nx[0];
      N.y = point[i].nx[1];
      N.z = point[i].nx[2];
      
      /* the coeffs of the tangent plane at point[i] */
      c[0] = N.x;
      c[1] = N.y;
      c[2] = N.z;
      c[3] = c[0] * point[i].x[0] + c[1] * point[i].x[1] + c[2] * point[i].x[2];
      
      /* pick a random point T on the tangent plane */
      if(c[0]==1.0 || c[0]==-1.0)
	{
	  T.x = 0.0; T.y = 1.0;  T.z = 0.0;
	}
      else if(c[1]==1.0 || c[1]==-1.0)
	{
	  T.x = 0.0;  T.y = 0.0;  T.z = 1.0;
	}
      else if(c[2]==1.0 || c[2]==-1.0)
	{
	  T.x = 1.0;  T.y = 0.0;  T.z = 0.0;
	}
      else if(c[0]!=0.0)
	{
	  T.x = c[3] / c[0];  T.y = 0.0;  T.z = 0.0;
	}
      else if(c[1]!=0.0)
	{
	  T.x = 0.0;  T.y = c[3] / c[2];T.z = 0.0;
	}
      else if(c[2]!=0.0)
	{
	  T.x = 0.0; T.y = 0.0; T.z = c[3] / c[2];
	}
      
      /* the vector between point[i] and T is the first(x) tangent vector */ 
      T1.x = point[i].x[0] - T.x;
      T1.y = point[i].x[1] - T.y;
      T1.z = point[i].x[2] - T.z;
      length = T1.x * T1.x + T1.y * T1.y + T1.z * T1.z; 
      length = sqrt(length);
      T1.x /= length;
      T1.y /= length;	    
      T1.z /= length;
      
      /* the cross product N x T1 is the second(y) tangent vector */
      T2.x = N.y * T1.z - T1.y * N.z;
      T2.y = N.z * T1.x - T1.z * N.x;
      T2.z = N.x * T1.y - T1.x * N.y;
      /*
	fprintf(stderr,"N:  (%4.2f, %4.2f, %4.2f)\n", N.x, N.y, N.z);	    
	fprintf(stderr,"Ti: (%4.2f, %4.2f, %4.2f)\n", T1.x, T1.y, T1.z);	    
	fprintf(stderr,"T2: (%4.2f, %4.2f, %4.2f)\n", T2.x, T2.y, T2.z);	    
      */	    
      /* transform the neighbour points, such that   */
      /* the current point (I) becomes the origin,  */
      /* the normal direction N becomes the z-axis, */
      /* and the tangent directions T1/T2 becomes   */
      /* the x/y-axes.	 			  */
      int m;
      for(m=0; m<num_neighbours; m++)
	{
	  x = neighbour_point[m].x - point[i].x[0];
	  y = neighbour_point[m].y - point[i].x[1];
	  z = neighbour_point[m].z - point[i].x[2];
	  xformed_neighbour_point[m].x = x * T1.x + y * T1.y + z * T1.z;
	  xformed_neighbour_point[m].y = x * T2.x + y * T2.y + z * T2.z;
	  xformed_neighbour_point[m].z = x * N.x  + y * N.y  + z * N.z;
	}
      
      
      /* fit the local quadric surface */
      /* using least square method     */
      for(m=0; m<num_neighbours; m++)
	data[m][0] = xformed_neighbour_point[m].z;
      
      for(m=0; m<num_neighbours; m++){
	A[m][0] = xformed_neighbour_point[m].x
	  * xformed_neighbour_point[m].x;
	A[m][1] = xformed_neighbour_point[m].y
	  * xformed_neighbour_point[m].y;
	A[m][2] = xformed_neighbour_point[m].x
	  * xformed_neighbour_point[m].y;
	A[m][3] = xformed_neighbour_point[m].x;
	A[m][4] = xformed_neighbour_point[m].y;
	A[m][5] = 1.0;
      }
      
      double det=0;
      int sing=0;
      
      matrix_transpose(A, At, num_neighbours, 6);
      matrix_product(B, At, 6, num_neighbours, A, num_neighbours, 6);
      if (mode==0)
	matrix_inverse(B, B_1, 6);
      else
	{
	  ludcmp0(B,6,indx,&det,sing);
	  //fprintf(stderr,"Det =%f, sing=%d ",det,sing);
	}
      
      matrix_product(C, At, 6, num_neighbours, data, num_neighbours, 1);
      
      if (mode==0)
	{
	  matrix_product(C2, B_1, 6, 6, C, 6, 1);
	  for (int ii=0;ii<6;ii++)
	    coeff[ii]=C2[ii][0];
	}
      else
	{
	  for (int ii=0;ii<6;ii++)
	    coeff[ii]=C[ii][0];
	  lubksb0(B,6,indx,coeff);
	}
      /*
	// compute the residual of the fit 
	double residual = 0.0;
	for(m=0; m<num_neighbours; m++)
	residual += fabs(data[m]
	- coeff[0] * A[m][0]
	- coeff[1] * A[m][1]
	- coeff[2] * A[m][2]
	- coeff[3] * A[m][3]
	- coeff[4] * A[m][4]
	- coeff[5] * A[m][5]); 
      */
      /* compute the curvatures */
      fx  = coeff[3];
      fy  = coeff[4];
      fxx = coeff[0] * 2.0;
      fxy = coeff[2];
      fyy = coeff[1] * 2.0;
      
      g[0][0] = 1.0 + fx*fx;
      g[0][1] = fx * fy;
      g[1][0] = fx * fy;
      g[1][1] = 1.0 + fy*fy;
      
      matrix_inverse(g, g_1, 2);
      
      b[0][0] = fxx / ((1.0+fx*fx+fy*fy) * (1.0+fx*fx+fy*fy));
      b[0][1] = fxy / ((1.0+fx*fx+fy*fy) * (1.0+fx*fx+fy*fy));
      b[1][0] = fxy / ((1.0+fx*fx+fy*fy) * (1.0+fx*fx+fy*fy));
      b[1][1] = fyy / ((1.0+fx*fx+fy*fy) * (1.0+fx*fx+fy*fy));
      
      matrix_product(beta, g_1, 2, 2, b, 2, 2);
      
      //point[i].K = beta[0][0] * beta[1][1] - beta[0][1] * beta[1][0];
      //point[i].H = (beta[0][0] + beta[1][1]) / 2.0;
      
      
      for(m=1; m<3; m++)
	for(int n=1; n<3; n++)
	  a1_repl[m-1][n-1] = beta[m-1][n-1];

      vtkMath_JacobiN(a1_repl,2,d1_repl,v1_repl);

      /*
      get_eigenvalues_vectors(a1, 2, d1, v1, &nrot);
      sort_eigenvalues_vectors(d1, v1, 2);*/
      
      K1 = d1_repl[0];
      K2 = d1_repl[1];
      
      if (debugmode)
	{
	  if (100*int(i/100)==i)
	    fprintf(stderr," k1=%6.3f, k2=%6.3f\n",K1,K2);
	}
      
      /* the prinipal curvatures and directions in local space */
      point[i].k[0] = K1;
      point[i].k[1] = K2;
      
      //k1.x = v1[1][1]; k1.y = v1[2][1]; k1.z = 0.0;
      //k2.x = v1[1][2]; k2.y = v1[2][2]; k2.z = 0.0;
      
      /* the two principal directions in global space */
      /*point[i].k1x = T1.x*k1.x + T2.x*k1.y + N.x*k1.z;
	point[i].k1y = T1.y*k1.x + T2.y*k1.y + N.y*k1.z;
	point[i].k1z = T1.z*k1.x + T2.z*k1.y + N.z*k1.z;
	point[i].k2x = T1.x*k2.x + T2.x*k2.y + N.x*k2.z;
	point[i].k2y = T1.y*k2.x + T2.y*k2.y + N.y*k2.z;
	point[i].k2z = T1.z*k2.x + T2.z*k2.y + N.z*k2.z;*/
      
      /* the normal vector in global space */
      point[i].nx[0] = N.x;
      point[i].nx[1] = N.y;
      point[i].nx[2] = N.z;	
    }
  
  free_dmatrix(A,     0,500-1,0,5);
  free_dmatrix(At,    0,5,0,500-1);
  free_dmatrix(B,     0,5,0,5);
  free_dmatrix(B_1,   0,5,0,5);
  free_ivector(indx  ,0,5);
  free_dmatrix(C,     0,5,0,5);
  free_dmatrix(C2,    0,5,0,5);
  free_dvector(coeff, 0,5);
  free_dmatrix(data,  0,500-1,0,0);
  free_dmatrix(g,     0, 1, 0, 1);
  free_dmatrix(g_1,   0, 1, 0, 1);
  free_dmatrix(b,     0, 1, 0, 1);
  free_dmatrix(beta,  0, 1, 0, 1);

  delete [] a1_repl[1]; 
  delete [] a1_repl[0]; 
  delete [] a1_repl;


  delete [] v1_repl[1]; 
  delete [] v1_repl[0]; 
  delete [] v1_repl;

  delete [] d1_repl;

  curvaturesok=1;
  //  debugmode=0;
  return 1;
   
}
// ------------------------------------------------------------------------
// Color Lookup Table Functions, from Peng-Cheng's shape_index
// ------------------------------------------------------------------------
float PXTriangulatedStack::curvature_lookup_red(float val)
{

  if(val < 0.0 || val > 1.0)
      {
	if (debugmode)
	  fprintf(stderr,"error in red curvature value(%10.5f)\n",val);
	val=Frange(val,0.0,1.0);
      }
  
  if(val < 0.5)
    return(1.0);
  else if(val == 0.5)
    return(1.0);

  return(2.0 - 2.0*val);
}

float PXTriangulatedStack::curvature_lookup_green(float val)
{
  if(val < 0.0 || val > 1.0)
      {
	  if (debugmode)
		fprintf(stderr,"error in green curvature value(%10.5f)\n",val);
	val=Frange(val,0.0,1.0);
      }
  
  if(val < 0.5)
      return(2.0 * val);
  else if(val == 0.5)
      return(1.0);
  
  return(1.0);
}

float PXTriangulatedStack::curvature_lookup_blue(float val)
     
{
  if(val < 0.0 || val > 1.0)
      {
		if (debugmode)
		fprintf(stderr,"error in blue curvature value(%10.5f)\n",val);
	val=Frange(val,0.0,1.0);
      }

  if(val < 0.5)
      return(2.0 * val);
  else if(val == 0.5)
      return(1.0);
  
  return(2.0 - 2.0*val);
}


/* the follow three functions are for the bending energy rgb lookup table */ 
float PXTriangulatedStack::bending_energy_lookup_red(float val)
{
  if(val < 0.0 || val > 1.0)
      {
		if (debugmode)
		fprintf(stderr,"error in red bending energy value(%10.5f)\n",val);
	val=Frange(val,0.0,1.0);
      }

  return(2.0 - 2.0*val);
}

float PXTriangulatedStack::bending_energy_lookup_green(float val)
{
  if(val < 0.0 || val > 1.0)
      {
		if (debugmode)
		fprintf(stderr,"error in green bending energy value(%10.5f)\n",val);
	val=Frange(val,0.0,1.0);
      }
 
  return(1.0);
}

float PXTriangulatedStack::bending_energy_lookup_blue(float val)
{
  if(val < 0.0 || val > 1.0)
      {
		if (debugmode)
		fprintf(stderr,"error in blue bending energy value(%10.5f)\n",val);
	val=Frange(val,0.0,1.0);
      }

  return(2.0 - 2.0*val);
}


/* the follow three functions are for the shape index rgb lookup table */
float PXTriangulatedStack::shape_index_lookup_red(float val)
{
  if(val < -1.0 || val > 1.0)
      {
	if (debugmode)
	  fprintf(stderr,"error in red shape index value(%10.5f)\n",val);
	val=Frange(val,-1.0,1.0);
      }
  if(val < -7.0/8.0) 
      return(0.0);
  else if(val < -5.0/8.0) 
      return(0.0);
  else if(val < -3.0/8.0) 
      return(0.0);
  else if(val < -1.0/8.0) 
      return(0.5);
  else if(val < 1.0/8.0) 
      return(1.0);
  else if(val < 3.0/8.0) 
      return(1.0);
  else if(val < 5.0/8.0) 
      return(1.0);
  else if (val < 7.0/8.0) 
      return(1.0);

  return(1.0);
}

float PXTriangulatedStack::shape_index_lookup_green(float val)
{
  if(val < -1.0 || val > 1.0)
      {
	if (debugmode)
	  fprintf(stderr,"error in green shape index value(%10.5f)\n",val);
	val=Frange(val,-1.0,1.0);
      }


  if(val < -7.0/8.0) 
      return(1.0);
  else if(val < -5.0/8.0) 
      return(1.0);
  else if(val < -3.0/8.0) 
      return(1.0);
  else if(val < -1.0/8.0) 
      return(1.0);
  else if(val < 1.0/8.0) 
      return(1.0);
  else if(val < 3.0/8.0) 
      return(1.0);
  else if(val < 5.0/8.0) 
      return(1.0);
  else if (val < 7.0/8.0) 
      return(0.5);

  return(0.0);
}

float PXTriangulatedStack::shape_index_lookup_blue(float val)
{
  if(val < -1.0 || val > 1.0)
    {
      if (debugmode)
	fprintf(stderr,"error in blue shape index value(%10.5f)\n",val);
      val=Frange(val,-1.0,1.0);
    }
  
  if(val < -7.0/8.0) 
    return(0.0);
  else if(val < -5.0/8.0) 
    return(0.5);
  else if(val < -3.0/8.0) 
    return(1.0);
  else if(val < -1.0/8.0) 
    return(1.0);
  else if(val < 1.0/8.0) 
    return(1.0);
  else if(val < 3.0/8.0) 
    return(0.5);
  else if(val < 5.0/8.0) 
    return(0.0);
  else if (val < 7.0/8.0) 
    return(0.0);
  
  return(0.0);
}
// ------------------------------------------------------------------------
//      Generating Lookup Table Colors
// ------------------------------------------------------------------------
PXColorPoint* PXTriangulatedStack::generateColors(int mode,float gamma)
{
  if (!curvaturesok)
      return NULL;
  
  gamma=Frange(gamma,0.05,10.0);
  
  if (debugmode)
      fprintf(stderr,"Allocating colors %d\n",numpoints);
  PXColorPoint* colors=new PXColorPoint[numpoints];
  float* value=new float[numpoints]; float max=0.00001;
  mode=Irange(mode,0,5);
  float amin=1000.0,amax=-1000.0;
  int i;
  for (i=0;i<numpoints;i++)
      {
	switch (mode)
	    {  //(K=0, H=1, k1=2, k2=3, B=4, Index=5)
	    case 0: // Gaussian
	      value[i]=point[i].k[0]*point[i].k[1];
	      break;
	    case 1: // Mean Curvature
	      value[i]=0.5*(point[i].k[0]+point[i].k[1]);
	      break;
	    case 2: // Principal Curvature 1
	      value[i]=point[i].k[0];
	      break;
	    case 3: // Principal Curvature 2
	      value[i]=point[i].k[1];
	      break;
	    case 4: // Bending Energy
	      value[i]=sqrt(0.5*(pow(point[i].k[0],float(2.0))+pow(point[i].k[1],float(2.0))));
	      break;
	    case 5: // Shape Index 
	      if(point[i].k[0]==point[i].k[1])
		  {
		    if(point[i].k[0]>0.0)  value[i] =  1.0;
		    if(point[i].k[0]<0.0)  value[i] = -1.0;
		    if(point[i].k[0]==0.0) value[i] =  0.0;
		  }
	      else
		  value[i] = 2.0 / M_PI * atan((point[i].k[1]+point[i].k[0])/(point[i].k[1]-point[i].k[0]));
	      break;
	    }
	if (mode<5)
	    {
	      max=Fmax(fabs(value[i]),max);
	      amax=Fmax(amax,value[i]);
	      amin=Fmin(amin,value[i]);
	      if (mode<4)
		  {
		    max_curvature=Fmax((value[i]),max_curvature);
		    min_curvature=Fmin((value[i]),min_curvature);
		  }
	    }
      }
  
  for (i=0;i<numpoints;i++)
      {

	if (mode==4)
	    {
	      value[i]=value[i]/(max);
	      value[i]=pow(value[i],float(1.0/gamma));
	      colors[i].red=bending_energy_lookup_red(value[i]);
	      colors[i].green=bending_energy_lookup_green(value[i]);
	      colors[i].blue=bending_energy_lookup_blue(value[i]);
	    }
	else if (mode==5)
	    {
	      //value[i]=value[i]/(max);
	      colors[i].red=shape_index_lookup_red(value[i]);
	      colors[i].green=shape_index_lookup_green(value[i]);
	      colors[i].blue=shape_index_lookup_blue(value[i]);
	    }
	else
	    {
	      max=Fmax(max_curvature,fabs(min_curvature));
	      value[i]=value[i]/(2.0*max);
	      float v=pow(fabs(value[i]),float(1.0/gamma));
	      if (value[i]<0.0)
		  v=0.5-v;
	      else
		  v=0.5+v;
	      v=Frange(v,0.0,1.0);
	      colors[i].red=curvature_lookup_red(v);
	      colors[i].green=curvature_lookup_green(v);
	      colors[i].blue=curvature_lookup_blue(v);
	    }
      }
  delete [] value;
  return colors;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::SaveInventor(const char* fname,PXColorPoint* colors)
{
  if (!haspoints() || !hastriangles())
      {
	  if (debugmode)
		fprintf(stderr,"Cannot save inventor as points=%d , triangles=%d\n",
			haspoints(),hastriangles());
	return 0;
      }
  
  int status=0;
  FILE* fout=fopen(fname,"w");
  if (fout)
      {
	fprintf(fout,"#Inventor V2.0 ascii\n");
	status=SaveInventor(fout,colors);
	if (debugmode && status)
	    fprintf(stderr,"Saved inventor in %s\n",fname);

	fclose(fout);
      }
  return(status);
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::SaveInventor(FILE* fout,PXColorPoint* colors)
{
  int status=0;
  if (!haspoints() || !hastriangles())
      {
	  if (debugmode)
		fprintf(stderr,"Cannot save inventor as points=%d , triangles=%d\n",
			haspoints(),hastriangles());
	return 0;
      }
  
  if (fout)
      {
	fprintf(fout,"\tSeparator {\n");
	fprintf(fout,"\t\tShapeHints {\n vertexOrdering  COUNTERCLOCKWISE\n  faceType CONVEX\n }\n\n");
	fprintf(fout,"# %d Points,%d Triangles\n\n",numpoints,numtriangles);
	fprintf(fout,"Coordinate3{ point[\n");
	int i;
	for (i=0;i<numpoints;i++)
	    fprintf(fout,"%6.3f %6.3f %6.3f,\n",point[i].x[0],point[i].x[1],point[i].x[2]);
	fprintf(fout,"]}\n");
	if (normalsok)
	    {
	      fprintf(fout,"Normal { vector [\n");
	      for (i=0;i<numpoints;i++)
		  fprintf(fout,"%6.3f %6.3f %6.3f,\n",point[i].nx[0],point[i].nx[1],point[i].nx[2]);
	      fprintf(fout,"]}\n");
	    }
	if (colors!=NULL)
	    {
	      fprintf(fout,"Material { diffuseColor [ \n");
	      for (i=0;i<numpoints;i++)
		  fprintf(fout,"%5.3f %5.3f %5.3f,\n",colors[i].red,colors[i].green,colors[i].blue);
	      fprintf(fout,"]\n}\nMaterialBinding {\nvalue PER_VERTEX_INDEXED\n}\n");
	    }

	fprintf(fout,"IndexedFaceSet{ coordIndex[\n");
	for (i=0;i<numtriangles;i++)
	    fprintf(fout,"%d,%d,%d,-1,\n",triangle[i].p[0],triangle[i].p[1],triangle[i].p[2]);
	fprintf(fout,"]\n");
	if (normalsok)
	    {
	      fprintf(fout,"normalIndex  [\n");
	      for (i=0;i<numtriangles;i++)
		  fprintf(fout,"%d, %d, %d,-1,\n",triangle[i].p[0],triangle[i].p[1],triangle[i].p[2]);
	      fprintf(fout,"]");
	    }
	fprintf(fout,"}\n}\n");
	status=1;
      }
  return status;
}
// ------------------------------------------------------------------------
void PXTriangulatedStack::findorigin(int np[2],float* x[2],float* y[2],float* z[2],
				     int origin[2],int dimension)
{

  int found=0;
  origin[0]=0;	  origin[1]=0;
  int i=0;
  while(!found && i<np[0])
      {
	origin[0]=i;
	int bestj=0;
	float mindist=10000000.0;
	for(int j=0;j<np[1];j++)
	    {
	      float dist;
	      if (dimension==3)
		  dist=Fsqdist(x[0][i],y[0][i],z[0][i],x[1][j],y[1][j],z[1][j]);
	      else
		  dist=Fsqdist(x[0][i],y[0][i],0.0,x[1][j],y[1][j],0.0);
	      if (dist<mindist)
		  {
		    mindist=dist;
		    bestj=j;
		  }
	    }
	mindist=10000000.0;
	int besti=i;
	for (int k=0;k<np[0];k++)
	    {
	      float dist;
	      if (dimension==3)
		  dist=Fsqdist(x[0][k],y[0][k],z[0][k],x[1][bestj],y[1][bestj],z[1][bestj]);
	      else
		  dist=Fsqdist(x[0][k],y[0][k],0.0,x[1][bestj],y[1][bestj],0.0);
	      if (dist<mindist)
		  {
		     mindist=dist;
		     besti=k;
		   }
	    }
	if (besti==i)
	    {
	      found=1;
	      origin[0]=besti;
	      origin[1]=bestj;
	    }
	i++;
      }
  if (!found)
      {
	  if (debugmode)
		fprintf(stderr,"Error in origin\n");
	origin[0]=0;
	origin[1]=0;
      }
}
// --------------------------------------------------------------
int PXTriangulatedStack::triangulate(int np[2],float* x[2],float* y[2],float* z[2],int origin[2],
				     PXNeighboursList* faces,int maxfaces,int dimension)
{
  int faceindex=0,done=0,numfaces=0,cend[2],current[2];
  cend[0]=0; cend[1]=0;
  current[0]=origin[0];  current[1]=origin[1];

  // Initialize Triangle Structure 
  PXNeighbours* triangle=new PXNeighbours();
  triangle->num=3;
  triangle->pt=new int[3];

  while(!done)
      {
	float dist[2];
	int next[2];
	
	for (int c1=0;c1<=1;c1++)
	    if (np[c1]==1)
		cend[c1]=1;

	for (int c=0;c<=1;c++)
	    {
	      if (cend[c]==0)
		  {
		    next[c]=Icyclic(current[c]+1,np[c]);
		    int other=current[1-c];
		    if (dimension==3)
			dist[c]=Fsqdist(x[c][next[c]],y[c][next[c]],z[c][next[c]],
					x[1-c][other],y[1-c][other],z[1-c][other]);
		    else
			dist[c]=Fsqdist(x[c][next[c]],y[c][next[c]],0.0,
					x[1-c][other],y[1-c][other],0.0);
		  }
	      else
		  dist[c]=100000000.0;
	    }
	
	if (dist[0]<dist[1])
	    {
	      triangle->pt[0]=current[0];
	      triangle->pt[1]=next[0];
	      triangle->pt[2]=current[1]+np[0];
	      faces->Add(triangle,faceindex);
	      faceindex++;
	      current[0]=next[0];
	      if (current[0]==origin[0]) 
		  cend[0]=1;
	      
	    }
	else
	    {
	      triangle->pt[0]=current[0];
	      triangle->pt[1]=next[1]+np[0];
	      triangle->pt[2]=current[1]+np[0];
	      faces->Add(triangle,faceindex);
	      faceindex++;
	      current[1]=next[1];
	      if (current[1]==origin[1]) 
		  cend[1]=1;
	    }
	numfaces++;
	
	if (cend[0]==1 && cend[1]==1)
	    done=1;
	
	if (faceindex>=maxfaces)
	    {
		if (debugmode)
	      fprintf(stderr,"\n\t\tError not closing in %d\n",maxfaces);
	      done=1;
	    }
      }
  delete [] triangle->pt;
  delete triangle;
  return faceindex;
}
// ------------------------------------------------------------------------
void PXTriangulatedStack::findorigin2D(int np[2],float* x[2],float* y[2],
				       int origin[2])
{
  findorigin(np,x,y,NULL,origin,2);
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::triangulate2D(int np[2],float* x[2],float* y[2],int origin[2],
				       PXNeighboursList* faces,int maxfaces)
{
  return triangulate(np,x,y,NULL,origin,faces,maxfaces,2);
}
// ------------------------------------------------------------------------
void PXTriangulatedStack::getstatistics(float mean[3],float var[6],PXTriangulatedStack* other)
{
  PXTriangulatedStack *st[2];
  st[0]=this;
  st[1]=other;

  int numst=2,np=0;
  if (other==NULL)
      {
	//fprintf(stderr,"Single Mode\n");
	numst=1;
      }
  /*se 
      fprintf(stderr,"Dual Mode\n");*/
  int i;
  for (i=0;i<3;i++)
      mean[i]=0.0;
  for (i=0;i<6;i++)
      var[i]=0.0;

  int k;
  for (k=0;k<numst;k++)
      {
	for (int i=0;i<numpoints;i++)
	    {
	      PXTriangulatedStackPoint* pointtemp=st[k]->getpoint(i);
	      for (int ia=0;ia<=2;ia++)
		  mean[ia]+=pointtemp->x[ia];
	    }
	np+=st[k]->getnumpoints();
      }
  float scale=1.0/float(np);
  if (np<1)
      return;

  for (i=0;i<=2;i++)
      mean[i]=mean[i]*scale;
  
  for (k=0;k<numst;k++)
      {
	for (int i=0;i<numpoints;i++)
	    {
	      PXTriangulatedStackPoint* pointtemp=st[k]->getpoint(i);
	      float x[3];
	      for (int ia=0;ia<=2;ia++)
		  x[ia]=pointtemp->x[ia]-mean[ia];
	      
	      var[0]+=x[0]*x[0];
	      var[1]+=x[1]*x[1];
	      var[2]+=x[2]*x[2];
	      
	      var[3]+=x[0]*x[1];
	      var[4]+=x[0]*x[2];
	      var[5]+=x[1]*x[2];
	    }
      }

  for (i=0;i<=2;i++)
      var[i]=scale*(var[i]);

  var[3]=scale*(var[3]);
  var[4]=scale*(var[4]);
  var[5]=scale*(var[5]);
  
  if (debugmode)
      fprintf(stderr,"\nPoints =%d (%8.5f)\nMean Position = (%6.2f,%6.2f,%6.2f)\n,\tVar= (%6.3f,%6.3f,%6.3f)\n"
	      "\tCovariance = (%6.3f,%6.3f,%6.3f)\n",np,scale,
	      mean[0],mean[1],mean[2],
	      var[0],var[1],var[2],
	      var[3],var[4],var[5]);
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::getrelationship(int p1,int p2,int max)
{
  if (!hasneighbours())
      generateNeighbours();
  
  return (neighboursList->getrelationship(p1,p2,max));
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::findCentroid(int* p,int np,int iterations,int maxd)
{
  if (np<2)
      return -1;
  
  PXIntegerList* list=new PXIntegerList();
  for (int i=0;i<np;i++)
      list->Add(p[i]);
  int r=findCentroid(list,iterations,maxd);
  delete list;
  return r;

}
// ------------------------------------------------------------------------
int PXTriangulatedStack::findCentroid(PXIntegerList* origList,int iterations,int maxd)
{
  if (origList->getnumnodes()<2)
      return -1;
  
  // int a=getrelationship(0,1);
  //  fprintf(stderr,"Relationship between 0 and 1 = %d\n",a);
  //  neighboursList->Save("n.list");
  PXIntegerList* intList=new PXIntegerList();
  origList->Rewind();
  int i;
  for (i=0;i<origList->getnumnodes();i++)
      {
	intList->Add(origList->getcurrent());
	origList->Next();
      }

  for (int iter=0;iter<=iterations;iter++)
      {
	origList->Rewind();
	for (int pt=0;pt<origList->getnumnodes();pt++)
	    {
	      int ptr=origList->getcurrent();
	      // fprintf(stderr,"Looking for point %d\n",ptr);
	      neighbours[ptr].Rewind();

	      for (int k=0;k<neighbours[ptr].getnumnodes();k++)
		  {
		    //fprintf(stderr," test against %d \n",neighbours[ptr].getcurrent());
		    intList->AddNonDuplicate(neighbours[ptr].getcurrent(),-1);
		    neighbours[ptr].Next();
		  }
	      origList->Next();
	    }
      }

  // Compile Distance Table 
  int mindist=1000000000;
  int bestnode=-1;

  origList->Rewind();
  for( i=0;i<origList->getnumnodes();i++)
      {
	int dist=0;
	intList->Rewind();
	for (int j=0;j<intList->getnumnodes();j++)
	    {
	      int a=getrelationship(origList->getcurrent(),intList->getcurrent(),maxd);
	      dist=Imax(dist,a);
	      intList->Next();
	    }
	
	if (dist<mindist)
	    {
	      mindist=dist;
	      bestnode=origList->getcurrent();
	    }
	origList->Next();
      }
  delete intList;
  return bestnode;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::saveTstackPair(const char *fname,
					PXTriangulatedStack* stack1,PXTriangulatedStack* stack2,
					PX3Vector* dx,int* used,int* fixed)
{
  int status=0;
  FILE *fout=fopen(fname,"w");
  if (fout)
      {
	status=1;
	fprintf(fout,"#Inventor V2.0 ascii\nSeparator {\n");
	fprintf(fout,"Coordinate3{ point[\n");
	int i;
	for (i=0;i<stack1->getnumpoints();i++)
	    fprintf(fout,"%6.3f %6.3f %6.3f,\n",
		    (stack1->getpoint(i))->x[0],
		    (stack1->getpoint(i))->x[1],
		    (stack1->getpoint(i))->x[2]);
	float mult=1.0;
	for (i=0;i<stack1->getnumpoints();i++)
	    fprintf(fout,"%6.3f %6.3f %6.3f,\n",
		    (stack1->getpoint(i))->x[0]+mult*dx[i].x[0],
		    (stack1->getpoint(i))->x[1]+mult*dx[i].x[1],
		    (stack1->getpoint(i))->x[2]+mult*dx[i].x[2]);
	
	fprintf(fout,"]}\n");
	
	int maxpas=0;
	if (fixed!=NULL && used!=NULL)
	    maxpas=2;
	
	for (int pass=0;pass<=maxpas;pass++)
	    {
	      switch(pass)
		  {
		  case 0:
		    fprintf(fout,"Material {  diffuseColor 1.0 1.0 0.0 \n transparency 0.0 }\n");
		    break;
		  case 1:
		    fprintf(fout,"Material {  diffuseColor 1 0.0 0.0 \n transparency 0.0}\n");
		    break;
		  case 2:
		    fprintf(fout,"Material {  diffuseColor 0.0 1.0 0.0 \n transparency 0.0}\n" );
		    break;
		  }
	      
	      fprintf(fout,"IndexedLineSet{ coordIndex[\n");
	      
	      for (i=0;i<stack1->getnumpoints();i++)
		  {
		    int num1=0;
		    if (maxpas>0)
			num1=fixed[i]+used[i];
		    
		    if (pass==num1)
			fprintf(fout,"%d,%d,-1,\n",i,i+stack1->getnumpoints());
		  }
	      fprintf(fout,"]}\n");
	    }
	fprintf(fout,"}\n");
	//fprintf(fout,"DrawStyle {	style LINES	}\n");
	fprintf(fout,"Material {  diffuseColor 0.2 0.2 0.2 \n transparency 0.0}\n" );
	stack1->SaveInventor(fout);
	fprintf(fout,"Material {  diffuseColor 0.0 0.0 1.0 \n transparency 0.2}\n" );
	stack2->SaveInventor(fout);
	fclose(fout);
	if (debugmode)
	    fprintf(stderr,"Saving in %s \n",fname);
      }
  return status;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::findSymmetricPair(PXTriangulatedStack* tstack1,PXTriangulatedStack* tstack2,
					   PXNeighboursList* pairs,int seedt)
{
  static int count=0;

  if (debugmode && count==0)
      {
	fprintf(stderr,"Using Symmetric Nearest Neighbour\n");
	count=1;
      }
  int done=0,steps=0;
  int seedpoint=Irange(seedt,0,tstack1->getnumpoints()-1);
  int bestpoint=0;
  int bestpoint2=0;
  
  while(!done && steps<10)
      {
	float x[3],x0[3];
	for (int ia=0;ia<=2;ia++)
	    x[ia]=(tstack1->getpoint(seedpoint))->x[ia];
	tstack2->findnearestpoint(x[0],x[1],x[2],bestpoint,0,0.01);
	
	for (int ib=0;ib<=2;ib++)
	     x0[ib]=(tstack2->getpoint(bestpoint))->x[ib];

	tstack1->findnearestpoint(x0[0],x0[1],x0[2],bestpoint2,0,0.01);
	
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
	firstpairs->Add(bestpoint+tstack1->getnumpoints());
	
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
// ------------------------------------------------------------------------
int PXTriangulatedStack::smoothEuclidean(PXTriangulatedStack* tstack1,PXTriangulatedStack* tstack2,
					 PX3Vector* newdisp,PX3Vector* olddisp,
					 int* used,int* fixed,int* corr,
					 int mode,int saveno,int smoothOutput)
{
  if (debugmode)
      fprintf(stderr,"\t1. Averaging step (moving fixed=%d,subpixel=%d)\n",
	      mode>=tstack1->getnumpoints(),mode>tstack1->getnumpoints());
  int good=0;
  //int mid=tstack1->getnumpoints()/2;
  int pt;
  for (pt=0;pt<tstack1->getnumpoints();pt++)
      {
	
	if (fixed[pt]==0 || mode>=tstack1->getnumpoints())
	    {
	      good+=used[pt];
	      PXIntegerList* neigh=tstack1->getneighbours(pt);
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
		    for (int ia=0;ia<=2;ia++)
			newx[ia]=newx[ia]/float(numgood);
		    float factor=0.5;
		    if (used[pt]==0)
			factor=1.0;
		    
		    for (int ib=0;ib<=2;ib++)
			newdisp[pt].x[ib]=(1.0-factor)*olddisp[pt].x[ib]+factor*newx[ib];
		    used[pt]=1;
		  }
	    }
      }
  
  good=0;
  for( pt=0;pt<tstack1->getnumpoints();pt++)
      {
	used[pt]=abs(used[pt]);
	good+=used[pt];
      }

  if (debugmode)
      {
	fprintf(stderr,"\t\t (%d/%d) points have displacements\n",good,tstack1->getnumpoints());
	fprintf(stderr,"\t2. Mapping step\n");
      }


  float dist=0.0,maxshift=0.0,maxdist=0.0;
  int reverse=0;
  for (pt=0;pt<tstack1->getnumpoints();pt++)
      {
	if (100*int(pt/100)==pt && pt>0 && debugmode)
	    fprintf(stderr,"%d/%d ",pt,tstack1->getnumpoints());
	if (600*int(pt/600)==pt && pt>0 && debugmode)
	    fprintf(stderr,"\n");

	PXTriangulatedStackPoint* point1=tstack1->getpoint(pt);
	if ((used[pt]>0) && (fixed[pt]!=1 || good==tstack1->getnumpoints()))
	    {
	      float x[3];
	      for (int ia=0;ia<=2;ia++)
		  x[ia]=point1->x[ia]+newdisp[pt].x[ia]; 
	      int mp;
	      float d=tstack2->findnearestpoint(x[0],x[1],x[2],mp,(mode>tstack1->getnumpoints()),0.0);
	      dist+=d;
	      for (int ib=0;ib<=2;ib++)
		  {
		    x[ib]=x[ib]-point1->x[ib]; 
		    newdisp[pt].x[ib]=x[ib];
		  }

	      maxdist=Fmax(maxdist,d);
	      maxshift=Fmax(maxshift,Fsqdist( x[0],x[1],x[2],0.0,0.0,0.0));
	      corr[pt]=mp;
	    }
      }
  if (debugmode)
      fprintf(stderr,"\t\taverage mapping distance %6.3f (max=%6.3f) maxshift=%6.3f rev=%d\n",
	      dist/float(good),maxdist,sqrt(maxshift),reverse);


  for (pt=0;pt<tstack1->getnumpoints();pt++)
      {
	for (int ia=0;ia<=2;ia++)
	    olddisp[pt].x[ia]=newdisp[pt].x[ia];
      }
  
  char line[100];
  /*  if (saveno>0)
      {
	sprintf(line,"/tmp/iter.%02d.iv",saveno);
	saveTstackPair(line,tstack1,tstack2,newdisp,used,fixed);
      }*/
  if (smoothOutput>0 || saveno>0)
      tstack1->deformUsingDisplacements(newdisp,1.0,0); //smoothOutput);
  if (saveno>0)
      {
	  if (debugmode)
		fprintf(stderr,"Saving smoothed output\n");
	sprintf(line,"/tmp/deformed.%02d.iv",saveno);
	tstack1->SaveInventor(line);
      }
  if (smoothOutput>0 || saveno>0)
      tstack1->deformUsingDisplacements(newdisp,-1.0);
  return good;
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::smoothConnection(PXTriangulatedStack* tstack1,PXTriangulatedStack* tstack2,
					  PX3Vector* disp,
					  int* /*used*/,int* fixed,int* corr,
					  int mode,int saveno,int smoothOutput)
{
  if (debugmode)
      fprintf(stderr,"\t1. Maxdistance minimizing step (moving fixed=%d)\n",mode==1);
  int shift=0;
  //int mid=tstack1->getnumpoints()/2;
  for (int pt=0;pt<tstack1->getnumpoints();pt++)
      {
	if (fixed[pt]==0 || mode==1)
	    {
	      PXIntegerList* neigh=tstack1->getneighbours(pt);
	      neigh->Rewind();
	      int bestcenter=corr[pt];
	      int bestdist=0,bestmindist=100000000;
	      for (int n=0;n<neigh->getnumnodes();n++)
		  {
		    int ind=neigh->getcurrent();
		    int dist=tstack2->getrelationship(corr[pt],corr[ind],100);
		    if (dist<0) 
			dist=100;
		    bestdist=Imax(dist,bestdist);
		    bestmindist=Imin(dist,bestmindist);
		    neigh->Next();
		  }
	      PXIntegerList* candidates=tstack2->getneighbours(corr[pt]);
	      candidates->Rewind();
	      for (int i=0;i<candidates->getnumnodes();i++)
		  {
		    int cand=candidates->getcurrent();
		    neigh->Rewind();
		    int maxd=0,mind=100000000;
		    for (int j=0;j<neigh->getnumnodes();j++)
			{
			  int ind=neigh->getcurrent();
			  int dist=tstack2->getrelationship(cand,corr[ind],100);
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
	      PXTriangulatedStackPoint* point1=tstack1->getpoint(pt);
	      PXTriangulatedStackPoint* point2=tstack2->getpoint(bestcenter);
	      if (bestcenter!=corr[pt])
		  {
		    corr[pt]=bestcenter;
		    shift++;
		  }
	      for (int ia=0;ia<=2;ia++)
		  disp[pt].x[ia]=point2->x[ia]-point1->x[ia];

	    }

	if (100*int(pt/100)==pt && pt>0 && debugmode)
	    fprintf(stderr,"%d/%d (%d), ",pt,tstack1->getnumpoints(),shift);
	if (600*int(pt/600)==pt && pt>0 && debugmode)
	    fprintf(stderr,"\n");
      }
  if (debugmode)
      fprintf(stderr,"   [ %d points shifted]\n",shift);

  char line[100];
  
  if (smoothOutput>0 || saveno>0)
      tstack1->deformUsingDisplacements(disp,1.0,smoothOutput);
  if (saveno>0)
      {
	sprintf(line,"/tmp/deformed.%02d.iv",saveno);
	tstack1->SaveInventor(line);
      }
  if (smoothOutput>0 || saveno>0)
      tstack1->deformUsingDisplacements(disp,-1.0,0);
  return shift;
}
// -----------------------------------------------------------------------
// Find Smooth Map
// -----------------------------------------------------------------------
int PXTriangulatedStack::findSmoothMap(PXTriangulatedStack* tstack1,PXTriangulatedStack* tstack2,
				       int* corr,PX3Vector* dx,int mode,int iterations,
				       int curvmode,float lamda)

{
  //  debugmode=1;
  int numiter=0;
  int postmode=0;

  if (mode==3)
      {
	mode=1;
	postmode=1;
      }
  if (mode>3)
      {
	mode=2;
	postmode=1;
      }

  if (debugmode)
      {
	fprintf(stderr," Find Smooth Map ::: \n");
	switch(mode)
	    {
	    case 2:
	      fprintf(stderr," Using global `connection-based' nearest neighbour initialization (post=%d)\n",postmode);
	      
	      break;
	    case 1:
	      fprintf(stderr," Using global `euclidean-based' nearest neighbour initialization (post=%d)\n",postmode);
	      break;
	    default:
	      fprintf(stderr," Using nearest neighbour initialization \n");
	      break;
	}
	if (curvmode==0)
	    fprintf(stderr," Using Shape Tracking no smoothing\n");
	if (curvmode>0)
	    fprintf(stderr," Using Shape Tracking and smoothing\n");
	if (curvmode<0)
	    fprintf(stderr," No shape tracking\n");
	
      }

  if (!tstack2->hascurvatures())
      {
	tstack2->generateNeighbours(1);
	tstack2->nonShrinkSmooth(0.33,-0.34,50,2);
	tstack2->generateNeighbours();
      }
  
  // Part 1 Initialization
  // ---------------------

  if (mode==1 || mode ==2 )
      {
	if (debugmode)
	    fprintf(stderr,"Generating Neighbours\n");
	if (!tstack1->hasneighbours())
	    tstack1->generateNeighbours();
	if (!tstack2->hasneighbours())
	    tstack2->generateNeighbours();

	int inc=1;//tstack1->getnumpoints()/400+1;
	PXNeighboursList* tetra=new PXNeighboursList();

	if (debugmode)
	    fprintf(stderr,"\nStarting Symmetric Neighbour Search inc=%d points=%d\n",
		    inc,tstack1->getnumpoints());

	for (int pt=0;pt<tstack1->getnumpoints();pt+=inc)
	    {
	      if (debugmode)
		  {
		    if (100*int(pt/100)==pt && pt>0)
			fprintf(stderr,"%d/%d (%d)  ",pt,
				tstack1->getnumpoints(),
				tetra->getnumnodes());
		    if (700*int(pt/700)==pt && pt>0)
			fprintf(stderr,"\n");
		  }
	      findSymmetricPair(tstack1,tstack2,tetra,pt);
	    }
	if (debugmode)
	    fprintf(stderr," done\n");
      
  
	if (debugmode)
	    {
	      fprintf(stderr,"\n Stack1 (numpoints) = %d , Stack 2 (numpoints) =%d \n",
		      tstack1->getnumpoints(),tstack2->getnumpoints());
	      fprintf(stderr,"Number of matches = %d \n", tetra->getnumnodes());
	      fprintf(stderr,"----------------------------------\n");
	    }

	tetra->Rewind();
	int* used=new int[tstack1->getnumpoints()];
	int* fixed=new int[tstack1->getnumpoints()];
	
	PX3Vector *dx0=new PX3Vector[tstack1->getnumpoints()];
	
	for (int j=0;j<tstack1->getnumpoints();j++)
	    {
	      corr[j]=-1; used[j]=0;	fixed[j]=0;
	      for (int ia=0;ia<=2;ia++)
		  {
		    dx0[j].x[ia]=0.0;
		    dx[j].x[ia]=0.0;  
		  }
	    }
	
	// Find displacement vectors
	for (int i=0;i<tetra->getnumnodes();i++)
	    {
	      PXIntegerList* pair=(tetra->getcurrentnode())->points;
	      pair->Rewind();	      
	      int index1=pair->getcurrent();
	      pair->Next();
	      int index2=pair->getcurrent();
	      //fprintf(stderr," Point 1 = %d , Point 2 =%d \n",index1,index2);
	      PXTriangulatedStackPoint* point1=tstack1->getpoint(index1);
	      PXTriangulatedStackPoint* point2=tstack2->getpoint(index2-tstack1->getnumpoints());

	      for (int ia=0;ia<=2;ia++)
		  dx0[index1].x[ia]=point2->x[ia]-point1->x[ia];

	      used[index1]=1;	fixed[index1]=1;
	      corr[index1]=index2-tstack1->getnumpoints();
	      tetra->Next();
	    }
	
	int numgood=tetra->getnumnodes();
	delete tetra;
	if (debugmode>1)
	    {
	      saveTstackPair("/tmp/iter.01.iv",tstack1,tstack2,dx,used,fixed);
	      tstack1->SaveInventor("/tmp/deformed.01.iv");
	    }

	int id=-1;
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
		    numgood=smoothEuclidean(tstack1,tstack2,dx,dx0,used,fixed,corr,
					    (numgood+1),id);
		    
		  }
	      else
		  {
		    int mved=1;
		    if (numgood<tstack1->getnumpoints())
			{
			  numgood=smoothEuclidean(tstack1,tstack2,dx,dx0,used,fixed,corr,
						  numgood,id);
			}
		    else
			{
			  if (nmode==0)
			      {
				tstack1->generateNeighbours(1);
				nmode=1;
			      }
			  if (debugmode)
			      fprintf(stderr,"Smoothing Connection\n");
			  mved=smoothConnection(tstack1,tstack2,dx,used,fixed,corr,
						1,id);
			  if (mved==0)
			      iter=iterations+1;
			}
		  }
	    }
	
	if (postmode)
	    {
	      tstack1->generateNeighbours(1);
	      tstack1->deformUsingDisplacements(dx,1.0,0);
	      PX3Vector* dp=tstack1->nonShrinkSmoothD(0.16,-0.165,35,2);
	      if (debugmode>1)
		  {
		    char line[100];
		    sprintf(line,"/tmp/deformed.%02d.iv",id+1);
		    tstack1->SaveInventor(line);
		  }
	      tstack1->deformUsingDisplacements(dx,-1.0,0);
	      float meandp[3],maxdp[3];
	      for (int p=0;p<=2;p++)
		  {
		    meandp[p]=0.0;  maxdp[p]=0.0;
		  }
	      for (int i=0;i<tstack1->getnumpoints();i++)
		  {
		    for (int ia=0;ia<=2;ia++)
			{
			  dx[i].x[ia]+=dp[i].x[ia];
			  meandp[ia]+=dp[i].x[ia]*float(1.0/tstack1->getnumpoints());
			  maxdp[ia]=Fmax(maxdp[ia],dp[i].x[ia]);
			}
		  }
	      
	      if (debugmode)
		  {
		    fprintf(stderr,"Mean dp (%6.3f %6.3f %6.3f)\n",
			    meandp[0],meandp[1],meandp[2]);
		    fprintf(stderr,"Max dp (%6.3f %6.3f %6.3f)\n",
			    maxdp[0],maxdp[1],maxdp[2]);
		  }
	      delete [] dp;
	    }
	delete [] used; delete [] fixed; delete [] dx0;
      }
  else
      {
	numiter=2;
	for (int pt=0;pt<tstack1->getnumpoints();pt++)
	    {
	      PXTriangulatedStackPoint* point1=tstack1->getpoint(pt);
	      float x[3];
	      for (int ia=0;ia<=2;ia++)
		  x[ia]=point1->x[ia];
	      int mp;
	      tstack2->findnearestpoint(x[0],x[1],x[2],mp,1,0.0);
	      corr[pt]=mp;
	      for (int ib=0;ib<=2;ib++)
		  dx[pt].x[ib]=x[ib]-point1->x[ib];
	    }
	if (debugmode>1)
	    {
	      saveTstackPair("/tmp/iter.02.iv",tstack1,tstack2,dx);
	      tstack1->deformUsingDisplacements(dx,1.0,0);
	      tstack1->SaveInventor("/tmp/deformed.02.iv");
	      tstack1->deformUsingDisplacements(dx,-1.0);
	    }
      }

  // Part 2 Shape-Tracking if needed
  // -------------------------------

  if (debugmode)
      fprintf(stderr,"Curvmode =%d\n",curvmode);

  if (curvmode>=0)
      {
	int smoothiter=curvmode;
	PXTriangulatedStack::curvatureTracking(tstack1,tstack2,dx,corr,lamda,smoothiter);
	for (int i=0;i<tstack1->getnumpoints();i++)
	    for (int ib=0;ib<=1;ib++)
		(tstack1->getpoint(i))->k[ib]=(tstack2->getpoint(corr[i]))->k[ib];
      }

  return (numiter);
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::collectNeighbours(int point,int distance,PXIntegerList* list)
{
  int st=1;
  if (!hasneighbourstructures)
    st=generateNeighbours();
  
  if (st==0)
    return 0;
  
  list->DeleteAll();
  distance=Irange(distance,0,10);
  point=Irange(point,0,numpoints-1);
  

  list->Add(point);
  
  if (distance==0)
    return list->getnumnodes();

  neighbours[point].Rewind();  
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
// ------------------------------------------------------------------------
int PXTriangulatedStack::\
curvatureTracking(PXTriangulatedStack* tstack1,PXTriangulatedStack* tstack2,
		  PX3Vector* disp,int* corr,float lamda,int smoothIterations)
{
  /*float radius=3.0;

  if (debugmode)
      fprintf(stderr,"Estimating scale for radius=%6.3f\n",radius);

  // 1. Estimate Spacing and scale for radius
  // ----------------------------------------
  float x1[3],x2[3];
  int p1=tstack1->getnumpoints()/2;
  PXIntegerList* nlist=tstack1->getneighbours(p1);
  nlist->Rewind();
  int p2=nlist->getcurrent();

  for (int ia=0;ia<=2;ia++)
      {
	x1[ia]=(tstack1->getpoint(p1))->x[ia];
	x2[ia]=(tstack1->getpoint(p2))->x[ia];
      }
	
  float dist=Fdist(x1[0],x1[1],x1[2],x2[0],x2[1],x2[2])+0.01;
  int scale=Irange(int(radius/dist+0.5),1,10);*/
  int scale=3;

  if (debugmode)
      fprintf(stderr,"\t Scale = %d\n",scale);

  // 2. Estimate Initial Matching
  // ----------------------------
  if (debugmode)
      fprintf(stderr,"\n Initial matching step \n");

  PX3Vector* conf=new PX3Vector[tstack1->getnumpoints()];
  PXIntegerList* searchPoints=new PXIntegerList();
  float globalmindist=100000.0;
  // Find Initial Match 
  for (int pt1=0;pt1<tstack1->getnumpoints();pt1++)
      {
	PXTriangulatedStackPoint* point1=tstack1->getpoint(pt1);
	float bend1=sqrt(point1->k[0]*point1->k[0]+point1->k[1]*point1->k[1]);
	float minbend=100000.0,sumbend=0.0,sum2bend=0.0;
	int pt2=corr[pt1];
	int np2=tstack2->collectNeighbours(pt2,scale,searchPoints);
	if (debugmode)
	    {
	      if (100*int(pt1/100)==pt1 && pt1>0)
		  fprintf(stderr,"%d/%d (np=%d)",pt1,tstack1->getnumpoints(),np2);
	      if (700*int(pt1/700)==pt1 && pt1>0)
		  fprintf(stderr,"\n");
	    }

	int bestpt2=pt2;
	for (int pass=0;pass<=1;pass++)
	    {
	      searchPoints->Rewind();
	      for (pt2=0;pt2<np2;pt2++)
		  {
		    PXTriangulatedStackPoint* point2=tstack2->getpoint(searchPoints->getcurrent());
		    float bend2=sqrt(point2->k[0]*point2->k[0]+point2->k[1]*point2->k[1]);
		    float bendist=sqrt(pow(bend2-bend1,float(2.0)));
		    if (pass==0)
			{
			  if (bendist<minbend)
			      {
				minbend=bendist;
				bestpt2=searchPoints->getcurrent();
			      }
			  sumbend+=bendist;
			}
		    else
			sum2bend+=pow(bendist-conf[pt1].x[0],float(2.0));
		    searchPoints->Next();
		  }
	      if (pass==0)
		  {
		    conf[pt1].x[0]=minbend;
		    globalmindist=Fmin(globalmindist,minbend);
		    conf[pt1].x[1]=sumbend/(float(np2)+0.0001);
		    for (int ia=0;ia<=2;ia++)
			disp[pt1].x[ia]=tstack2->getpoint(bestpt2)->x[ia]-point1->x[ia];
		  }
	      else
		  conf[pt1].x[2]=sum2bend/(float(np2)+0.0001);
	    }
      }

  char line[50];
  if (debugmode>1)
      {
	fprintf(stderr,"\n Min match dist = %6.3f",globalmindist);
	tstack1->deformUsingDisplacements(disp,1.0,0); 
	sprintf(line,"/tmp/defcurv.01.iv");
	tstack1->SaveInventor(line);
	tstack1->deformUsingDisplacements(disp,-1.0,0); 
      }


  delete searchPoints;

  // 3. Calculate confidence measures
  // --------------------------------

  if (smoothIterations>0)
      {
	tstack1->generateNeighbours(); 

	if (debugmode)
	    fprintf(stderr,"\n Calculating Confidence Values\n");
	
	float K1 = 0.01 / (globalmindist+0.000001);
	float K2 = 0.01 / (globalmindist+0.000001);	
	float mind = 100.0;
	int i;
	for(i=0; i<tstack1->getnumpoints(); i++)
	    mind=Fmin(mind,conf[i].x[2]/(conf[i].x[1]-conf[i].x[0]));
	

	for(i=0; i<tstack1->getnumpoints(); i++)
	    {
	      float denom=(conf[i].x[1]-conf[i].x[0]);
	      float denom2=(1.0+K1*conf[i].x[0]);
	      
	      if (denom <0.0001)	    denom =0.0001;
	      if (denom2<0.0001)	    denom2=0.0001;
	      
	      conf[i].x[0] =  (1.0/denom2)/(1.0 + (K2*conf[i].x[2])/denom);
	    }
	if (debugmode)
	    fprintf(stderr,"Done with confidence values\n");
      }
  
  // 4. Smooth Output
  // ----------------
  if (smoothIterations>0)
      {
	PX3Vector* temp=new PX3Vector[tstack1->getnumpoints()];
	for (int iter=0;iter<smoothIterations;iter++)
	    {
	      int i;
	      for (i=0;i<tstack1->getnumpoints();i++)
		  for (int ia=0;ia<=2;ia++)
		      temp[i].x[ia]=disp[i].x[ia];  
	      
	      if (debugmode)
		  fprintf(stderr,"\n\n Smoothing Iteration %d \n",iter+1);

	      for(i=0;i<tstack1->getnumpoints();i++)
		  {
		    if (debugmode)
			{
			  if (100*int(i/100)==i && i>0)
			      fprintf(stderr,"%d/%d ",i,tstack1->getnumpoints());
			  if (700*int(i/700)==i && i>0)
			      fprintf(stderr,"\n");
			}

		    float oldx[3],newx[3];
		    for (int ia=0;ia<=2;ia++)
			{
			  oldx[ia]=temp[i].x[ia];
			  newx[ia]=temp[i].x[ia];
			}
		    float total=conf[i].x[0];
		    
		    PXIntegerList* nlist=tstack1->getneighbours(i);
		    
		    for(int j=0;j<nlist->getnumnodes();j++)
			{
			  int p=nlist->getcurrent();
			  for (int ia=0;ia<=2;ia++)
			      newx[ia]+=temp[p].x[ia]*conf[p].x[0];
			  total+=conf[p].x[0];
			  nlist->Next();
			}
		    
		    for (int ib=0;ib<=2;ib++)
			{
			  newx[ib]=newx[ib]/total;
			  disp[i].x[ib]=(1.0-lamda)*oldx[ib]+lamda*newx[ib];
			}
		  }
		    
	      // Mapping to Boundary Step
	      // ------------------------
	      float dist=0.0,maxshift=0.0,maxdist=0.0;
	      if (debugmode)
		  fprintf(stderr,"\n\n Mapping Iteration %d \n",iter+1);
	      
	      for (int pt=0;pt<tstack1->getnumpoints();pt++)
		  {
		    if (debugmode)
			{
			  if (100*int(pt/100)==pt && pt>0)
			      fprintf(stderr,"%d/%d ",pt,tstack1->getnumpoints());
			  if (700*int(pt/700)==pt && pt>0)
			      fprintf(stderr,"\n");
			}
		    
		    
		    //PXTriangulatedStackPoint* point1=tstack1->getpoint(pt);
		    float x[3];
		    for (int ia=0;ia<=2;ia++)
			x[ia]=tstack1->getpoint(pt)->x[ia]+disp[pt].x[ia]; 
		    int mp;
		    float d=tstack2->findnearestpoint(x[0],x[1],x[2],mp,1,0.001);
		    dist+=d;
		    for (int ib=0;ib<=2;ib++)
			{
			  x[ib]=x[ib]-tstack1->getpoint(pt)->x[ib]; 
			  disp[pt].x[ib]=x[ib];
			}
		    
		    maxdist=Fmax(maxdist,d);
		    maxshift=Fmax(maxshift,Fsqdist( x[0],x[1],x[2],0.0,0.0,0.0));
		    corr[pt]=mp;
		  }

	      if (debugmode>1)
		  {
		    tstack1->deformUsingDisplacements(disp,1.0,0); 
		    sprintf(line,"/tmp/defcurv.%02d.iv",iter+2);
		    tstack1->SaveInventor(line);
		    tstack1->deformUsingDisplacements(disp,-1.0,0); 
		  }
	    }
	delete [] temp;
      }

  if (debugmode)
      fprintf(stderr,"\nDone\n");
	
  delete [] conf;
  return 1;
	
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::loadPengChengDisplacementsSingle(const char* filename,
							  PX3Vector* dx,
							  float tolerance)
{
  if (debugmode)
	fprintf(stderr,"Reading Peng-Cheng Displacements from %s\n",filename);

  FILE* dispfile=fopen(filename,"r");
  
  if (dispfile==NULL)
      {
	fclose(dispfile);
	return 0;
      }
  
  char line[200];
  float *pointsx,*pointsy,*pointsz;
  // Count Lines in file
  // -------------------
  int numdispframes=2;
  int numdisppoints=0;
  
  while ( fgets(line,100,dispfile)!=NULL )    
      numdisppoints++;
  fclose(dispfile);
  dispfile=fopen(filename,"r");
  if (debugmode)
	fprintf(stderr,"Number of points in %s = %d\n",filename,numdisppoints);

  
  // Allocate Data and Read Rest of Files 
  // ------------------------------------
  pointsx=new float[numdisppoints*numdispframes];
  pointsy=new float[numdisppoints*numdispframes];
  pointsz=new float[numdisppoints*numdispframes];
  int i;
  for ( i=0;i<numdisppoints;i++)
      {
	for (int j=0;j<numdispframes;j++)
	    {
	      int index=i*numdispframes+j;
	      fscanf(dispfile,"%f %f %f ",
		     &pointsx[index],
		     &pointsy[index],
		     &pointsz[index]);
	    }
      }
  
  if (debugmode)
      fprintf(stderr,"\tFrames=%d, Points=%d\n",numdispframes,numdisppoints);
  fclose(dispfile);
  
  // Create Structures for Solid to find and store displacements
  // -----------------------------------------------------------
  
  int numd=0,numtest=0;
  tolerance*=tolerance;
  
  for (i=0;i<getnumpoints();i++)
      {
	numtest++;
	int pt=0,found=0;
	int bestpt=0;
	float mindist=10000.0;
	while(pt<numdisppoints && found==0)
	    {
	      int po_index=pt*numdispframes;
	      float x=pointsx[po_index];
	      float y=pointsy[po_index];
	      float z=pointsz[po_index];
	      float dist=Fsqdist(x,y,z,point[i].x[0],point[i].x[1],point[i].x[2]);
	      if (dist<mindist)
		  {
		    mindist=dist;
		    bestpt=pt;
		  }
	      
	      if (mindist<tolerance)
		  found=1;
	      else
		  pt++;
	      
	    }
	int po_index=bestpt*numdispframes;			
	numd++;
	dx[i].x[0]=pointsx[po_index+1]-pointsx[po_index];
	dx[i].x[1]=pointsy[po_index+1]-pointsy[po_index];
	dx[i].x[2]=pointsz[po_index+1]-pointsz[po_index];

	if (i==10 || i==200 && debugmode)
	    {
	      fprintf(stderr,"Original Node %d \n (%6.3f %6.3f %6.3f ) \n",i,
		      point[i].x[0],point[i].x[1],point[i].x[2]);
	      fprintf(stderr,"Closest Point %d \n (%6.3f %6.3f %6.3f ) \n",bestpt,
		      pointsx[po_index],
		      pointsy[po_index],
		      pointsz[po_index]);
	      fprintf(stderr,"Goes to %d \n (%6.3f %6.3f %6.3f ) \n",bestpt,
		      pointsx[po_index+1],
		      pointsy[po_index+1],
		      pointsz[po_index+1]);
	      fprintf(stderr,"Displacement = (%5.3f %5.3f %5.3f)\n",
		      dx[i].x[0],dx[i].x[1],dx[i].x[2]);
	    }
      }

  if (debugmode)
	fprintf(stderr,"\tDone with surface, found %d displacements (%d)\n",numd,numtest);
 
  delete [] pointsx;
  delete [] pointsy;
  delete [] pointsz;
  return numd;
}

// ------------------------------------------------------------------------
void setnode12(int* node1,int* node2,int index,int& current,int max)
{
  if (index<0 || index>=max || current<0 || current>=max)
      {
	fprintf(stderr,"************************** Error in node12");
	exit(0);
	// Error Message
      }
  else
      {	
	node2[current]=index;
	node1[index]=current;
	++current;
      }
}
// ------------------------------------------------------------------------
int PXTriangulatedStack::subdivide() // Stupid Algorithm -- but it works!!
{
  //  debugmode=1;

  if (debugmode)
    fprintf(stderr,"Subviding numpoints=%d\n",numpoints);
  
  if (!haspoints() || !hastriangles())
    {
      if (debugmode)
	fprintf(stderr,"Cannot subdivide neighbours as points=%d , triangles=%d\n",
		haspoints(),hastriangles());
      return 0;
    }
  
  generateNeighbours(0);
  
  // ---------------------------------------------------------------------
  //  Preliminary Stuff --- add neighbours
  // ---------------------------------------------------------------------
  PXIntegerList* midpoint=new PXIntegerList[numpoints];     // OK
  PXIntegerList* oldpoint=new PXIntegerList[numpoints];     // OK
  PXIntegerList* newpoint1=new PXIntegerList();             // OK
  PXIntegerList* newpoint2=new PXIntegerList();             // OK
  
  int tr;
  int newpoint=numpoints;
  
  if (debugmode)
    fprintf(stderr,"Step 1: find mid points \n");
  
  for (int pt=0;pt<numpoints;pt++)
    {
      if (debugmode)
	if (100*int(pt/100)==pt)
	  fprintf(stderr,"%d/%d ",pt,numpoints);
      
      neighbours[pt].Rewind();
      for (int i=0;i<neighbours[pt].getnumnodes();i++)
	{
	  int pt2=neighbours[pt].getcurrent();
	  neighbours[pt].Next();
	  
	  if (pt<pt2)
	    {
	      oldpoint[pt].Add(pt2);
	      midpoint[pt].Add(newpoint);
	      newpoint1->Add(pt);
	      newpoint2->Add(pt2);
	      newpoint++;
	    }
	}
    }

  if (debugmode)
    fprintf(stderr,"\n Done --> New Points = %d \n",newpoint);

  if (debugmode)
    fprintf(stderr,"Step 2: Generate New Stack (Points)\n");

  PXTriangulatedStack* old=new PXTriangulatedStack(this); //OK 
  
  // Save Stack Index 
  PXStackIndex oldstackindex;
  int hadstackindex=0;
  if (hasstackindex())
      {
	hadstackindex=1;
	oldstackindex.numcontours=stackIndex.numcontours;
	oldstackindex.index=new int[stackIndex.numcontours+1]; // OK
	oldstackindex.zlevel=new float[stackIndex.numcontours]; // OK
	for (int i=0;i<=stackIndex.numcontours;i++)
	  oldstackindex.index[i]=stackIndex.index[i];
	for (int j=0;j<stackIndex.numcontours;j++)
	  oldstackindex.zlevel[j]=stackIndex.zlevel[j];
      }
  
  cleanup();

  init(newpoint,old->getnumtriangles()*4);
  
  // First generate the points
  newpoint1->Rewind();
  newpoint2->Rewind();

  for (int newpt=0;newpt<numpoints;newpt++)
    {
      if (newpt<old->getnumpoints())
	{
	  PXTriangulatedStackPoint* other=old->getpoint(newpt);
	  point[newpt].index=other->index;
	  for (int ia=0;ia<=2;ia++)
	    {
	      point[newpt].x[ia]=other->x[ia];
	      point[newpt].nx[ia]=other->nx[ia];
	    }
	  for (int ib=0;ib<=1;ib++)
	    point[newpt].k[ib]=other->k[ib];
	}
      else
	{
	  point[newpt].index=newpt;
	  int base1=newpoint1->getcurrent();
	  int base2=newpoint2->getcurrent();
	  
	  newpoint1->Next();
	  newpoint2->Next();
	  for (int ia=0;ia<=2;ia++)
	    {
	      point[newpt].x[ia]=0.5*(point[base1].x[ia]+point[base2].x[ia]);
	      point[newpt].nx[ia]=0.5*(point[base1].nx[ia]+point[base2].nx[ia]);
	    }
	  
	  point[newpt].k[0]=0.5*(point[base1].k[0]+point[base2].k[0]);
	  point[newpt].k[1]=0.5*(point[base1].k[1]+point[base2].k[1]);


	}
      normalsok=old->hasnormals();
      curvaturesok=old->hascurvatures();
    }
  delete newpoint1;
  delete newpoint2;
  
  if (debugmode)
    fprintf(stderr,"Step 3: Generate New Stack (Triangles)\n");
  
  for (tr=0;tr<old->getnumtriangles();tr++)
    {
      PXTriangle* other=old->gettriangle(tr);
      int midpt[3];
      for (int ia=0;ia<=2;ia++)
	{
	  int ib=Icyclic(ia+1,3);
	  int p1=other->p[ia];
	  int p2=other->p[ib];
	  int test=Imin(p1,p2);
	  int target=Imax(p1,p2);
	  for (int k=0;k<oldpoint[test].getnumnodes();k++)
	    {
	      int t1=oldpoint[test].getcurrent();
	      int t2=midpoint[test].getcurrent();
	      
	      if (target==t1)
		{
		  k=oldpoint[test].getnumnodes()+1;
		  midpt[ia]=t2;
		}
	      midpoint[test].Next();
	      oldpoint[test].Next();
	    }
	}
      
      for (int tr1=0;tr1<4;tr1++)
	{
	  int trno=tr*4+tr1;
	  switch(tr1)
	    {
	    case 0:
	      triangle[trno].p[0]=other->p[0];
	      triangle[trno].p[1]=midpt[0];
	      triangle[trno].p[2]=midpt[2];
	      break;
	    case 1:
	      triangle[trno].p[0]=other->p[1];
	      triangle[trno].p[1]=midpt[1];
	      triangle[trno].p[2]=midpt[0];
	      break;
	    case 2:
	      triangle[trno].p[0]=other->p[2];
	      triangle[trno].p[1]=midpt[2];
	      triangle[trno].p[2]=midpt[1];
	      break;
	    case 3:
	      triangle[trno].p[0]=midpt[0];
	      triangle[trno].p[1]=midpt[1];
	      triangle[trno].p[2]=midpt[2];
	      break;
	    }
	}
    }
  
  delete old;

  //this->Save("unsorted.tstack");

  if (hadstackindex==1)
      {
	if (debugmode)
	    fprintf(stderr,"Step 4: Sort to generate new Stackindex\n");

	int* node1=new int[numpoints]; // OK
	int* node2=new int[numpoints]; // OK 

	for (int i=0;i<numpoints;i++)
	    {
	      node1[i]=-1; node2[i]=-2;
	    }
	stackindexok=1;
	stackIndex.numcontours=1+2*(oldstackindex.numcontours-1);
	stackIndex.index=new int[stackIndex.numcontours+1];
	stackIndex.zlevel=new float[stackIndex.numcontours];
	PXIntegerList* addList=new PXIntegerList(); // OK
	stackIndex.index[0]=0;
	int ind=0,current=0;

	for (int base=0;base<oldstackindex.numcontours;base++)
	    { 
	      if (debugmode)
		fprintf(stderr,"Base =%d\n",base);
	      for (int pt=oldstackindex.index[base];pt<oldstackindex.index[base+1];pt++)
		{
		  //fprintf(stderr, "%d ",pt);
		  setnode12(node1,node2,pt,current,numpoints);
		  oldpoint[pt].Rewind();
		  midpoint[pt].Rewind();
		  for (int k=0;k<oldpoint[pt].getnumnodes();k++)
		    {
		      int pt2=oldpoint[pt].getcurrent();
		      if (pt2<oldstackindex.index[base+1])
			setnode12(node1,node2,midpoint[pt].getcurrent(),current,numpoints);
		      oldpoint[pt].Next();
		      midpoint[pt].Next();
		    }
		}
	      //	      fprintf(stderr,"\n");
	      stackIndex.index[ind+1]=current;
	      stackIndex.zlevel[ind]=oldstackindex.zlevel[base];
	      ind++;

	      if (base!=oldstackindex.numcontours-1)
		  {
		    addList->DeleteAll();

		    for (int pt=oldstackindex.index[base];pt<oldstackindex.index[base+1];pt++)
			{
			  //fprintf(stderr, "%d ",pt);
			  oldpoint[pt].Rewind();
			  midpoint[pt].Rewind();
			  for (int k=0;k<oldpoint[pt].getnumnodes();k++)
			    {
			      int pt2=oldpoint[pt].getcurrent();
			      if (pt2>=oldstackindex.index[base+1])
				addList->AddNonDuplicate(midpoint[pt].getcurrent(),-1);
			      oldpoint[pt].Next();
			      midpoint[pt].Next();
			    }
			}
		    //		    fprintf(stderr,"\n");
		    addList->Rewind();
		    for (int k=0;k<addList->getnumnodes();k++)
		      {
			setnode12(node1,node2,addList->getcurrent(),current,numpoints);
			addList->Next();
		      }
		    stackIndex.index[ind+1]=current;
		    stackIndex.zlevel[ind]=0.5*(oldstackindex.zlevel[base]+oldstackindex.zlevel[base+1]);
		    ind++;
		  }
	    }

	//stackIndex.index[stackIndex.numcontours]=numpoints;
	// So all noded reindexed in node2 
	// Re-index triangles first 
	for (int tr=0;tr<numtriangles;tr++)
	    for (int tp=0;tp<=2;tp++)
		triangle[tr].p[tp]=node1[triangle[tr].p[tp]];
	// Move points around also 
	PXTriangulatedStackPoint* oldpoint=new PXTriangulatedStackPoint[numpoints]; // OK
	int pt;
	for (pt=0;pt<numpoints;pt++)
	    {
	      for (int ia=0;ia<=2;ia++)
		{
		  oldpoint[pt].x[ia]=point[pt].x[ia];
		  oldpoint[pt].nx[ia]=point[pt].nx[ia];
		}
	      for (int ib=0;ib<=1;ib++)
		oldpoint[pt].k[ib]=point[pt].k[ib];
	    }
	
	for (pt=0;pt<numpoints;pt++)
	    {
	      point[pt].index=pt;
	      for (int ia=0;ia<=2;ia++)
		{
		  point[pt].x[ia]=oldpoint[node2[pt]].x[ia];
		  point[pt].nx[ia]=oldpoint[node2[pt]].nx[ia];
		}
	      for (int ib=0;ib<=1;ib++)
		point[pt].k[ib]=oldpoint[node2[pt]].k[ib];
	    }
	
	/*if (debugmode)
	  {
	    FILE* fout=fopen("indices.txt","w");
	    for (int i=0;i<numpoints;i++)
	      fprintf(fout,"%d \t %d \t %d\n",i,node1[i],node2[i]);
	    fclose(fout);
	    }*/


	delete [] node1; delete [] node2;  
	delete [] oldpoint; 
	delete addList;     
	delete [] oldstackindex.index; 
	delete [] oldstackindex.zlevel;
      }
  else
      {
	stackindexok=1;
	stackIndex.numcontours=2;
	stackIndex.index=new int[stackIndex.numcontours+1];
	stackIndex.zlevel=new float[stackIndex.numcontours];
	stackIndex.index[0]=0;
	stackIndex.index[1]=0;
	stackIndex.index[2]=numpoints;
	stackIndex.zlevel[0]=0.0;
	stackIndex.zlevel[0]=1.0;
      }
  delete [] midpoint;
  delete [] oldpoint;

  //  Save("subdivide.tstak");
  //  delete [] belongtoTri;
  //  SaveInventor("sub.iv");
  return 1;
}
// ---------------------------------------------------------------------
// Generate Solid -- these are static
// ----------------------------------
// --------------------------------------------------------------------------
int PXTriangulatedStack::save2qhull(const char* fname,PXTriangulatedStack** stack,int numstacks)
{
  FILE* fout=fopen(fname,"w");
  if (fout)
      {
	fprintf(fout,"3\n");
	int total=0;
	for (int i=0;i<numstacks;i++)
	    total+=stack[i]->getnumpoints();
	fprintf(fout,"%d\n",total);
	for (int j=0;j<numstacks;j++)
	    for (int i=0;i<stack[j]->getnumpoints();i++)
		fprintf(fout,"%6.3f %6.3f %6.3f\n",(stack[j]->getpoint(i))->x[0],
			(stack[j]->getpoint(i))->x[1],(stack[j]->getpoint(i))->x[2]);
	fclose(fout);
	return 1;
      }
  return 0;
}
// --------------------------------------------------------------------------
int PXTriangulatedStack::loadqhull(const char* fname, PXTriangulatedStack** stack,int numstacks,
					PXNeighboursList* tetra,int& nt)
{
  char line[200];
  int i;
  for (i=0;i<numstacks;i++)
      if (!stack[i]->hasneighbours())
	  stack[i]->generateNeighbours();
  FILE* fin=fopen(fname,"r");
  if (!fin)
      return 0;

  int total=0;
  for (i=0;i<numstacks;i++)
      total+=stack[i]->getnumpoints();
  
  fgets(line,200,fin);
  int np,temp;
  fscanf(fin,"%d %d %d\n",&np,&nt,&temp);
  if (debugmode)
	fprintf(stderr,"Reading %s points=%d, tetra=%d\n",fname,np,nt);
  if (np!=(total))
      {
	  if (debugmode)
		fprintf(stderr,"Error, some points missing\n");
	fclose(fin);
	return (0);
      }
  
  float x1,y1,z1,t1;
  // Check Stack1 First, Then Stack2
  for (int j=0;j<numstacks;j++)
      for (int i=0;i<stack[j]->getnumpoints();i++)
	  {
	    fscanf(fin,"%f %f %f %f\n",&x1,&y1,&z1,&t1);
	    if (!Fsamepoint((stack[j]->getpoint(i))->x[0],
			    (stack[j]->getpoint(i))->x[1],
			    (stack[j]->getpoint(i))->x[2],
			    x1,y1,z1,0.05))
		{
		 if (debugmode)
		  fprintf(stderr,"Points not matching\n");
		  exit(1);
		}
	  }
  
  PXIntegerList* intList=new PXIntegerList();
  for (i=0;i<nt;i++)
      {
	int points;
	fscanf(fin,"%d",&points);
	if (points==4)
	    {
	      intList->DeleteAll();
	      int temp;
	      for (int j=0;j<points;j++)
		  {
		    fscanf(fin,"%d",&temp);
		    intList->Add(temp);
		  }
	      tetra->Add(intList,i);
	    }
	else
	    {
	      fgets(line,200,fin); // Bad triangle !!
	      if (debugmode)
		fprintf(stderr,"Error %d %s\n",points,line);
	    }
      }
  nt=tetra->getnumnodes();
  delete intList;
  fclose(fin);
  if (debugmode)
	fprintf(stderr,"Done Reading\n");
  return (1);

}
// --------------------------------------------------------------------------

int PXTriangulatedStack::whichstack(int index,int* total,int numstacks)
{
  int done=0,i=0;
  while (done==0 && i<numstacks)
      {
	if (index>=total[i] && index<total[i+1])
	    done=1;
	else
	    i++;
      }
  if (done==0)
      return numstacks-1;
  else
      return i;
}
// --------------------------------------------------------------------------

int PXTriangulatedStack::cleanup(PXTriangulatedStack* tstack[],int numstacks,
				      PXNeighboursList* tetra,int nt)
{
  int count=0,count1=0,count2=0,count3=0,tr=0,count4=0;
  if (debugmode)
      fprintf(stderr,"Checking Tetrahedra 1 : Planar\n\t");

  int* total=new int[numstacks+1];
  total[0]=0;
  for (int i=0;i<numstacks;i++)
      total[i+1]=total[i]+tstack[i]->getnumpoints();

  /*for (i=0;i<total[numstacks];i+=25)
      fprintf(stderr," i=%d, stack=%d \n",i,whichstack(i,total,numstacks));*/

  if (debugmode)
      fprintf(stderr,"Phase 1: Volume Check\n");
  tetra->Rewind();
  for ( tr=0;tr<nt;tr++)
      {
	float x[4],y[4],z[4];
	if (100*int(tr/100)==tr && debugmode)
	    fprintf(stderr," %d/%d",tr,nt);
	
	if (tetra->getcurrentindex()>0)
	    {
	      PXIntegerList* intList=((tetra->getcurrentnode())->points);
	      intList->Rewind();
	      int i;
	      for (i=0;i<=3;i++)
		  {
		    int index=intList->getcurrent();
		    intList->Next();
		    int st=whichstack(index,total,numstacks);
		    index=index-total[st];
		    x[i]=(tstack[st]->getpoint(index))->x[0];
		    y[i]=(tstack[st]->getpoint(index))->x[1];
		    z[i]=(tstack[st]->getpoint(index))->x[2];
		  }
	      // Make point 0 the origin 
	      for (i=1;i<=3;i++)
		  {
		    x[i]=x[i]-x[0]; y[i]=y[i]-y[0]; z[i]=z[i]-z[0];
		  }
	      
	      float nx=(y[2]*z[1]-z[2]*y[1]);
	      float ny=(z[2]*x[1]-x[2]*z[1]);
	      float nz=(x[2]*y[1]-y[2]*x[1]);
	      float vol=fabs(x[3]*nx+y[3]*ny+z[3]*nz);
	      
	      if (vol<=0.5)
		  {
		    (tetra->getcurrentnode())->index=-1;
		    count1++;
		    count++;
		  }
	    }
	tetra->Next();
      }
  
  if (debugmode)
      fprintf(stderr,"\n\tDone Planar Rejected=%d\nChecking Tetrahedra 2: Surface Connectivity \n\t",count);
  tetra->Rewind();

  int sum[5];
  sum[0]=0; sum[1]=0; sum[2]=0; sum[3]=0;
  for ( tr=0;tr<nt;tr++)
      {
	if (100*int(tr/100)==tr && debugmode)
	    fprintf(stderr," %d/%d",tr,nt);
	if(tetra->getcurrentindex()>=0) // Otherwise already not valid 
	    {
	      PXIntegerList* intList=(tetra->getcurrentnode())->points;
	      intList->Rewind();
	      int ind[4],whst=0;
	      for (int pt=0;pt<4;pt++)
		  {
		    ind[pt]=intList->getcurrent();
		    whst+=whichstack(ind[pt],total,numstacks);
		    intList->Next();
		  }
	      if (whst==0 || whst==(numstacks-1)*4)
		  {
		    int st=0;
		    if (whst==0)
			{
			  st=0;
			}
		    else 
			{
			  st=numstacks-1;
			  for (int j=0;j<4;j++)
			      ind[j]-=total[numstacks-1];
			}
		    
		    // Step One determine relationships <2
		    int conn1=0,conn2=0;
		    for (int k1=0;k1<=2;k1++)
			for (int k2=k1+1;k2<=3;k2++)
			    {
			      int dist=tstack[st]->getrelationship(ind[k1],ind[k2],3);
			      if (dist<=2)
				  conn2++;
			      if (dist<=1)
				  conn1++;
			    }

		    if ((conn2+conn1)<3 || (conn1<2))
			{
			  (tetra->getcurrentnode())->index=-1;
			  count++;
			  count2++;
			}
		    else
			{
			  int suminside=0,sumoutside=0;
			  float xm=0.0,ym=0.0,zm=0.0;
			  int i;
			  for (i=0;i<=3;i++)
			      {
				xm+=(tstack[st]->getpoint(ind[i]))->x[0];
				ym+=(tstack[st]->getpoint(ind[i]))->x[1];
				zm+=(tstack[st]->getpoint(ind[i]))->x[2];
			      }
			  xm/=4.0; ym/=4.0; zm/=4.0;
			  
			  for ( i=0;i<4;i++)
			      {
				float xp=(tstack[st]->getpoint(ind[i]))->x[0];
				float yp=(tstack[st]->getpoint(ind[i]))->x[1];
				float zp=(tstack[st]->getpoint(ind[i]))->x[2];
				
				float nx=(tstack[st]->getpoint(ind[i]))->nx[0];
				float ny=(tstack[st]->getpoint(ind[i]))->nx[1];
				float nz=(tstack[st]->getpoint(ind[i]))->nx[2];
				float dt=(nx*(xp-xm)+ny*(yp-ym)+nz*(zp-zm));
				
				
				if (st==0)
				    dt=-dt;
				//fprintf(stderr,"dt = %7.4f ",dt);
				if (dt<0.01)
				    suminside++;
				else
				    sumoutside++;
			      }
			  if (suminside<4)
			      sum[suminside]++;
			  if (suminside>0)
			      {
				(tetra->getcurrentnode())->index=-1;
				count++;
				count3++;
			      }
			  else
			      {
				count4++;
			      }
			}
		  }
	    }
	tetra->Next();
      }
  if (debugmode)
      {
	fprintf(stderr,"\n\tDone Surface Connectivity Rejected=%d (connec=%d) (normal=%d(%d)) (vol=%d)\n",count,count2,count3,count4,count1);
	
	PXTriangulatedStack::save2inventor("part.01.iv",tstack,numstacks,tetra,nt);
      }
  /*
  tetra->Rewind();
  fprintf(stderr,"Peng-Cheng Ultimate Test\n");
  int count5=0;
  PXContourStack* cst1=tstack[0]->exporttocontourstack();
  cst1->Equispace(1.0);
  PXContourStack* cst2=tstack[numstacks-1]->exporttocontourstack();
  cst2->Equispace(1.0);

  for ( tr=0;tr<nt;tr++)
      {
	if (100*int(tr/100)==tr)
	    fprintf(stderr," %d/%d",tr,nt);
	if(tetra->getcurrentindex()>=0) // Otherwise already not valid 
	    {
	      float x=0.0,y=0.0,z=0.0;
	      PXIntegerList* intList=(tetra->getcurrentnode())->points;
	      intList->Rewind();
	      for (int i=0;i<=3;i++)
		  {
		    int index=intList->getcurrent();
		    intList->Next();
		    int st=whichstack(index,total,numstacks);
		    index=index-total[st];
		    x+=0.25*(tstack[st]->getpoint(index))->x[0];
		    y+=0.25*(tstack[st]->getpoint(index))->x[1];
		    z+=0.25*(tstack[st]->getpoint(index))->x[2];
		  }
	      
	      int le=0,found=0;
	      while (found==0 && le<cst2->getnumcontours()-1)
		  {
		    if (z<cst2->getzlevel(le))
			found=1;
		    else
			le++;
		  }
	      le=Irange(le,0,cst2->getnumcontours()-2);
	      
	      if (fabs(z-cst2->getzlevel(le+1))<fabs(z-cst2->getzlevel(le)))
		  le++;
	      
	      PXContour* cntr1=cst1->getcontour(le);
	      PXContour* cntr2=cst2->getcontour(le);
	      
	      if (cntr2->IsInside(x,y) && !cntr1->IsInside(x,y))
		  {
		    //
		  }
	      else
		  {
		    (tetra->getcurrentnode())->index=-1;
		    count++;
		    count5++;
		  }
	      tetra->Next();
	    }
      }
  delete cst1;
  delete cst2;
  PXTriangulatedStack::save2inventor("part.02.iv",tstack,numstacks,tetra,nt);
  fprintf(stderr,"\n\tDone Ult test Rejected=%d (connec=%d) (normal=%d(%d)) (vol=%d) ultra=%d\n",count,
	  count2,count3,count4,count1,count5);
	  */
  return tetra->getnumnodes()-count;
}
// --------------------------------------------------------------------------
int PXTriangulatedStack::save2inventor(const char *fname,PXTriangulatedStack** tstack,int numstacks,
				       PXNeighboursList* tetra,int nt)
{
  FILE *fout=fopen(fname,"w");
  if (!fout)
      return 0;

  int st=PXTriangulatedStack::save2inventor(fout,tstack,numstacks,tetra,nt);
  if (st && debugmode)
      fprintf(stderr,"\n\nSaving in %s \n",fname);
  fclose(fout);
  return st;
}
// --------------------------------------------------------------------------
int PXTriangulatedStack::save2inventor(FILE* fout,PXTriangulatedStack** tstack,int numstacks,
					    PXNeighboursList* tetra,int nt)
{
  int* total=new int[numstacks];
  total[0]=0;
  int i;
  for (i=0;i<numstacks;i++)
      total[i+1]=total[i]+tstack[i]->getnumpoints();
  
  fprintf(fout,"#Inventor V2.0 ascii\nSeparator {\n");
  fprintf(fout,"ShapeHints {\n vertexOrdering  COUNTERCLOCKWISE\n  faceType CONVEX\n }\n\n");
  fprintf(fout,"# %d Points,%d Triangles\n\n",total[numstacks],nt);
  fprintf(fout,"# Hello\n");
  fprintf(fout,"Coordinate3{ point[\n");
  for (int j=0;j<numstacks;j++)
      for (int i=0;i<tstack[j]->getnumpoints();i++)
	  fprintf(fout,"%6.3f %6.3f %6.3f,\n",
		  (tstack[j]->getpoint(i))->x[0]*1.0,
		  (tstack[j]->getpoint(i))->x[1]*1.0,
		  (tstack[j]->getpoint(i))->x[2]);
  
  fprintf(fout,"]}\n");
  int valid=0;
  
  for (int pass=0;pass<numstacks-1;pass++)
      {
	
	fprintf(fout,"Material {  diffuseColor    %4.2f %4.2f 0.0 \n}\n",
		1.0-float(pass)/float(numstacks-1),
		float(pass)/float(numstacks));

	fprintf(fout,"IndexedFaceSet{ coordIndex[\n");
	tetra->Rewind();
	for (i=0;i<nt;i++)
	    {
	      if (tetra->getcurrentindex()>=0)
		  {
		    int num=0;
		    PXIntegerList* intList=(tetra->getcurrentnode())->points;
		    intList->Rewind();
		    int index[4];
		    for (int pt=0;pt<4;pt++)
			{
			  index[pt]=intList->getcurrent();
			  num+=whichstack(index[pt],total,numstacks);
			  intList->Next();
			}
		    if (pass==num/4)
			{
			  valid++;
			  fprintf(fout,"%d,%d,%d,-1,\n",index[0],index[1],index[2]);
			  fprintf(fout,"%d,%d,%d,-1,\n",index[1],index[2],index[3]);
			  fprintf(fout,"%d,%d,%d,-1,\n",index[0],index[1],index[3]);
			  fprintf(fout,"%d,%d,%d,-1,\n",index[0],index[2],index[3]);
			}
		  }
	      tetra->Next();
	    }
	fprintf(fout,"]}\n");
      }
  fprintf(fout,"}\n");
  fprintf(fout,"# Valid tetrahedra %d\n",valid);
  fprintf(fout,"DrawStyle {	style LINES	}\n");
  fprintf(fout,"Material {  diffuseColor 0.2 0.2 0.2 \n transparency 0.0}\n" );
  tstack[0]->SaveInventor(fout);
  fprintf(fout,"Material {  diffuseColor 0.0 0.0 1.0 \n transparency 0.0}\n" );
  tstack[numstacks-1]->SaveInventor(fout);
  //fclose(fout);
  return 1;

}
// --------------------------------------------------------------------------
int* PXTriangulatedStack::gettotal(PXTriangulatedStack** stack,int numstacks)
{
  int* total=new int[numstacks+1];
  total[0]=0;
  for (int i=0;i<numstacks;i++)
      total[i+1]=total[i]+stack[i]->getnumpoints();
  
  return total;
}
// --------------------------------------------------------------------------
float*  PXTriangulatedStack::generateVolumes(const char* fnameendo,const char* fnameepi,
					     int end,int begin)
{

  PXTriangulatedStack* stack1=new PXTriangulatedStack();
  PXTriangulatedStack* stack2=new PXTriangulatedStack();
  if (end<begin)
      return 0;
  
  float* volumes=new float[end-begin+1];
  int ok=1;

  for (int i=begin;i<=end;i++)
      {
	char line1[100],line2[100];
	sprintf(line1,"%s.%02d.tstack",fnameendo,i);
	int a1=stack1->Load(line1);
	sprintf(line2,"%s.%02d.tstack",fnameepi,i);
	int a2=stack2->Load(line2);
	
	if (a1+a2==2)
	    {
	      PXContourStack* cnt1=stack1->exporttocontourstack();
	      PXContourStack* cnt2=stack2->exporttocontourstack();
	      float cx,cy,cz,sx2,sy2,sxy,vol1,vol2;
	      cnt1->getPositionAndOrientation(cx,cy,cz,sx2,sy2,sxy,vol1);
	      cnt2->getPositionAndOrientation(cx,cy,cz,sx2,sy2,sxy,vol2);
	      volumes[i-begin+1]=fabs(vol2-vol1);
	      delete cnt1;
	      delete cnt2;
	    }
	else
	    {
	      ok=0;
	      i=end+1;
	    }
      }
  
  if (ok==0)
      {
	for (int i=0;i<end-begin+1;i++)
	    volumes[i]=1.0;
      }

  delete stack1;  delete stack2;
  return volumes;
}
// ------------------------------------------------------------------------------
void PXTriangulatedStack::updateCurvaturesFromStack(PXTriangulatedStack* denser)
{
  if (!denser->hascurvatures() || !denser->hasnormals())
      return;

  for (int i=0;i<numpoints;i++)
      {
	float x[3];
	int ia;
	for (ia=0;ia<=2;ia++)
	    x[ia]=point[i].x[ia];

	int best;
	denser->findnearestpoint(x[0],x[1],x[2],best,0,5.0);
	PXTriangulatedStackPoint* other=denser->getpoint(best);

	for (ia=0;ia<=2;ia++)
	    point[i].nx[ia]=other->nx[ia];

	for (ia=0;ia<=1;ia++)
	    point[i].k[ia]=other->k[ia];
      }
  normalsok=denser->hasnormals();
  curvaturesok=denser->hascurvatures();
}
// ------------------------------------------------------------------------------
int PXTriangulatedStack::updateStackIndexZlevels()
{
  if (hasstackindex())
      {
	for (int i=0;i<stackIndex.numcontours;i++)
	    {
	      float z=0.0;
	      int np=0;
	      for (int j=stackIndex.index[i];j<stackIndex.index[i+1];j++)
		  {
		    z+=getpoint(j)->x[2];
		    ++np;
		  }
	      if (np>0)
		  {
		    z=z/float(np);
		    stackIndex.zlevel[i]=z;
		  }
	    }
	return 1;
      }
  else
      return 0;
}
// ------------------------------------------------------------------------------
int PXTriangulatedStack::scalepoints(float sx,float sy,float sz)
{
  for (int i=0;i<numpoints;i++)
    {
      point[i].x[0]*=sx;
      point[i].x[1]*=sy;
      point[i].x[2]*=sz;
    }

  normalsok=0;
  curvaturesok=0;
  
  if (hasstackindex())
    {
      for (int i=0;i<stackIndex.numcontours;i++)
	stackIndex.zlevel[i]*=sz;
    }

  return 1;
}

// End of namespace

