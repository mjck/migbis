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
//---------------------------------------------------------------------------------------------------------

#include "pxabaqussolid.h"
#include "pxmarkerclass.h"
#include "pxutil.h"
#include "nrutil.h"
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July 1995   papad@noodle.med.yale.edu

   see PXBSplineStackSolid.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
// --------------------------------------------------------------------------
int  PXAbaqusSolid::debugmode=0;
// --------------------------------------------------------------------------
//    Access Functions 
// ------------------------------------------------------------------------- 
void     PXAbaqusSolid::setOrientationMode(int ormode) { orientationMode=Irange(ormode,-1,3);}
int      PXAbaqusSolid::getelementtype() { return elementtype;}
int      PXAbaqusSolid::getelementmode() { return elementmode;}

int      PXAbaqusSolid::getenforcementmode()  { return enforcementMode;}
int      PXAbaqusSolid::getorientationmode()  { return orientationMode;}
float    PXAbaqusSolid::getspringmulitplier() { return springMultiplier;}

int      PXAbaqusSolid::hasstacks()           { return hasStacks;}
int      PXAbaqusSolid::hasfemdata()          { return hasFemData;}
int      PXAbaqusSolid::hasnodes()            { return hasNodes;}
int      PXAbaqusSolid::haselements()         { return hasElements;}
int      PXAbaqusSolid::hasspringelements()   { return hasSpringElements;}
int      PXAbaqusSolid::hasspringnodes()      { return hasSpringNodes;}
int      PXAbaqusSolid::hasmaterial()         { return hasMaterial;}
int      PXAbaqusSolid::hasvolumes()          { return hasVolumes;}
float*   PXAbaqusSolid::getvolumes()          { return volume;}
int      PXAbaqusSolid::getnumvolumes()       { return numvolumes;}
float    PXAbaqusSolid::getoutputstep()       { return outputStep;}
int      PXAbaqusSolid::getoutputinc()        { return outputInc;}
int      PXAbaqusSolid::getnumlayers()        { return numlayers;}
int      PXAbaqusSolid::getnumpointscontour() { return numpointscontour;}



PXNeighboursList*    PXAbaqusSolid::getelements()       { return elements;}
PXNeighboursList*    PXAbaqusSolid::getspringelements() { return springElements;}
PXSolidNodes*        PXAbaqusSolid::getnodes()          { return nodes;}
PXSolidNodes*        PXAbaqusSolid::getspringnodes()    { return springNodes;}
PXAbaqusMaterialModel*     PXAbaqusSolid::getmaterial()       { return material;}
PXAbaqusOutputData*           PXAbaqusSolid::getfemdata()        { return femData;}

PXTriangulatedStack* PXAbaqusSolid::getstack(int i)    { return tstack[Irange(i,0,1)];}

void PXAbaqusSolid::setorientationmode(int m)
{
  orientationMode=Irange(m,0,6);
}

void PXAbaqusSolid::sethybridmode(int h)
{
  if (h>0 && elementmode<100)
      elementmode=elementmode+100;
  if (h<1 && elementmode>100)
      elementmode=elementmode-100;
}
// ------------------------------------------------------------------------- 

PXAbaqusSolid::PXAbaqusSolid()
{
  init();
}

PXAbaqusSolid::PXAbaqusSolid(PXAbaqusSolid* old)
{
  init();
  Copy(old);
}
// --------------------------------------------------------------------------
void PXAbaqusSolid::Copy(PXAbaqusSolid* other)
{
  cleanup();
  init();

  if (other->hasstacks())
      {
	for (int i=0;i<=1;i++)
	    tstack[i]=new PXTriangulatedStack(other->getstack(i));
	hasStacks=1;
      }

  elementtype=other->getelementtype();
  elementmode=other->getelementmode();

  if (other->haselements())
      {
	elements=new PXNeighboursList();
	elements->Copy(other->getelements());
	hasElements=1;
      }
  
  if (other->hasspringelements())
      {
	springElements=new PXNeighboursList();
	springElements->Copy(other->getspringelements());
	hasSpringElements=1;
      }

  if (other->hasnodes())
      {
	nodes=new PXSolidNodes(other->getnodes());
	hasNodes=1;
      }

  if (other->hasspringnodes())
      {
	springNodes=new PXSolidNodes(other->getspringnodes());
	hasSpringNodes=1;
      }
  
  if (other->hasmaterial())
      {
	material=new PXAbaqusMaterialModel(other->getmaterial());
	hasMaterial=1;
      }

  springMultiplier=other->getspringmulitplier();
  numlayers=other->getnumlayers();

  if (other->hasvolumes())
      {
	numvolumes=other->getnumvolumes();
	volume=new float[numvolumes];
	float* ovol=other->getvolumes();
	for (int ia=0;ia<numvolumes;ia++)
	    volume[ia]=ovol[ia];
	hasVolumes=1;
      }

  outputStep=other->getoutputstep();
  outputInc=other->getoutputinc();
  enforcementMode=other->getenforcementmode();
  orientationMode=other->getorientationmode();

  if (other->hasfemdata())
      {
	femData=new PXAbaqusOutputData(other->getfemdata());
	hasFemData=1;
      }
}
// --------------------------------------------------------------------------
PXAbaqusSolid::PXAbaqusSolid(const char* fname1,const char* fname2)
{
  init();
  tstack[0]=new PXTriangulatedStack();
  tstack[1]=new PXTriangulatedStack();
  int ok=tstack[0]->Load(fname1);
  ok+=2*tstack[1]->Load(fname2);
  if (ok<3)
      {
	fprintf(stderr,"I/O Error reading from %s and %s (%d)\n",fname1,fname2,ok);
	delete tstack[0];
	delete tstack[1];
      }
  else
      {
	if (debugmode)
	    fprintf(stderr,"Read from %s and %s (%d)\n",fname1,fname2,ok);
	hasStacks=1;
      }
}
// --------------------------------------------------------------------------
PXAbaqusSolid::PXAbaqusSolid(PXTriangulatedStack* stack1,
			     PXTriangulatedStack* stack2)
{
  init();
  tstack[0]=new PXTriangulatedStack(stack1);
  tstack[1]=new PXTriangulatedStack(stack2);
  hasStacks=1;

/*  if (totalnumstacks>2)
      {
	PXContourStack* cst[ABAQUSSOLIDMAXLEVELS];
	PXContourStack* cst1=tstack[0]->exporttocontourstack();
	cst1->Equispace(0.5);
	PXContourStack* cst2=tstack[numstacks-1]->exporttocontourstack();
	cst2->Equispace(0.5);

	if (debugmode)
	    fprintf(stderr,"Slowly ...");
	
	for (int i=1;i<numstacks-1;i++)
	    cst[i-1]=new PXContourStack(cst1->getnumcontours());
	
	// Create Inbetween Stacks 
	if (debugmode)
	    fprintf(stderr,"Going to stackset\n");
	int flag=PXContourStack::createstackset(cst1,cst2,cst,tetra,totalnumstacks-2,bias);
	if (flag==0)
	    {
	      fprintf(stderr,"Unequal number of slices, check surfaces, exiting\n");
	      for (int i=1;i<numstacks-1;i++)
		  delete cst[i-1];
	      delete tstack[0];
	      delete tstack[numstacks-1];
	      numstacks=0;
	      delete cst1;
	      delete cst2;
	      return;
	    }
	for (i=1;i<numstacks-1;i++)
	    {
	      cst[i-1]->Equispace(sample/5.0);
	      cst[i-1]->Smooth();
	      if (tetra)
		  cst[i-1]->Equispace(sample);
	      tstack[i]=new PXTriangulatedStack(cst[i-1],0);
	      if (tetra)
		  tstack[i]->nonShrinkSmooth(0.33,-0.34,20);
	    }

	if (tetra)
	    {
	      cst1->Equispace(sample);
	      cst2->Equispace(sample);
	      tstack[0]->setfromcontourstack(cst1,0);
	      tstack[numstacks-1]->setfromcontourstack(cst2,0);
	    }

	// Cleanup at end 
	// --------------
	for (i=0;i<numstacks-2;i++)
	    delete cst[i];
	delete cst1;
	delete cst2;
      }*/
}
// --------------------------------------------------------------------------

void PXAbaqusSolid::init()
{
  elementtype=8; hasmidwallnodes=0; hasmidwallelements=0;
  springMultiplier=1.0; numlayers=1; numpointscontour=-1;
  outputStep=1.0; outputInc=40; enforcementMode=0;  orientationMode=-1;
  hasElements=0;  hasNodes=0;   hasMaterial=0; hasFemData=0;
  hasVolumes=0;   hasSpringNodes=0; hasSpringElements=0; hasStacks=0;

}
// --------------------------------------------------------------------------
PXAbaqusSolid::~PXAbaqusSolid()
{
  cleanup();
}
// --------------------------------------------------------------------------
void PXAbaqusSolid::cleanup()
{
  cleanupElementSpace();
  cleanupNodeSpace();
  cleanupMaterial();

  if (hasstacks())
      {
	for (int i=0;i<=1;i++)
	    delete tstack[i];
      }

  if (hasvolumes())
      delete [] volume;


  if (hasspringelements() || hasspringnodes())
      cleanupSpringSpace();

  if (hasfemdata())
      cleanupFemData();

}
// --------------------------------------------------------------------------
void PXAbaqusSolid::cleanupMaterial()
{
  if (hasmaterial())
      {
	delete material;
	hasMaterial=0;
      }
}
// --------------------------------------------------------------------------
void PXAbaqusSolid::allocateMaterial()
{
  if (hasmaterial())
      cleanupMaterial();
  material=new PXAbaqusMaterialModel(0);
  hasMaterial=1;
}
// --------------------------------------------------------------------------
void PXAbaqusSolid::cleanupSpringSpace()
{
  /*  fprintf(stderr,"Cleaning up hasel=%d hasnd=%d\n",
      hasspringelements(),hasspringnodes());*/
  if (hasspringelements())
      {
	delete springElements;
	hasSpringElements=0;
      }
  if (hasspringnodes())
      {
	delete springNodes;
	hasSpringNodes=0;
      }
}
// --------------------------------------------------------------------------
void PXAbaqusSolid::cleanupFemData()
{
  if (hasfemdata())
      delete femData;
  hasFemData=0;
}
// --------------------------------------------------------------------------
void PXAbaqusSolid::cleanupElementSpace()
{
  if (haselements())
      delete elements;
  hasElements=0;
}
// --------------------------------------------------------------------------
void PXAbaqusSolid::cleanupNodeSpace()
{
  if (hasnodes())
      delete nodes;
  hasNodes=0;
}
// --------------------------------------------------------------------------
void PXAbaqusSolid::allocateFemData()
{
  if (hasfemdata())
      cleanupFemData();
  
  if (!haselements() || !hasnodes())
      return;
  femData=new PXAbaqusOutputData(nodes->getnumnodes(),elements->getnumnodes());
  hasFemData=1;
}
// --------------------------------------------------------------------------
void PXAbaqusSolid::allocateSpringSpace(int order)
{
  if (!hasnodes() || !haselements())
      return;


  order=Irange(order,0,1);
  int np=0;

  
  /* Need to generate an
    PXIntegerList* array to store connectivity on walls for tracked points to
	apply data to. Then things become very easy.
	
	Also need a temperature field for the nodes and the spring nodes otherwise
	things will get confused if temperature is used to model both
	1. active contraction and
	2. varying spring stiffness
	
	*/	
  
  // Generating Neighbours 
  elements->Rewind();
  for (int el=0;el<elements->getnumnodes();el++)
      {
	if (debugmode)
	    {
	      if (100*int(el/100)==el)
		  fprintf(stderr,"%d/%d ",el,elements->getnumnodes());
	    }
	
	PXIntegerList* intList=((elements->getcurrentnode())->points);
	intList->Rewind();
	if (intList->getnumnodes()<12)
	    {
	      int elpt[12];
	      for (int k=0;k<intList->getnumnodes();k++)
		  {
		    elpt[k]=intList->getcurrent();
		    intList->Next();
		  }
	      
	      {
		for(int k=0;k<intList->getnumnodes();k++)
		    {
		      int st=nodes->getindex(elpt[k],0);
		      for (int j=0;j<intList->getnumnodes();j++)
			  {
			    int st2=nodes->getindex(elpt[j],0);
			    if (j!=k && (st==st2) && (st==0 || st==100))
				nodes->getneighbours(elpt[k])->AddNonDuplicate(elpt[j],-1);
			  }
		    }
	      }
	    }
	elements->Next();
      }

  //nodes->Save("nodes.list");
  
  for (int pt=0;pt<nodes->getnumnodes();pt++)
      {
	int st=nodes->getindex(pt,0);
	if (st==0 || st==100)
	    {
	      np++;
	      if (order==1)
		  {
		    PXIntegerList* nei=nodes->getneighbours(pt);
		    np+=nei->getnumnodes();
		  }
	    }
      }

  //fprintf(stderr,"np=%d\n",np);

  int baseel=elements->getnumnodes();
  int baseno=nodes->getnumnodes();

  int p[2];

  const float Lengthfactor=0.01;
  cleanupSpringSpace();
  if (np>0)
      {
	springNodes=new PXSolidNodes(np);
	springElements=new PXNeighboursList();
	hasSpringNodes=1;
	hasSpringElements=1;

	int springnodeno=0;
	
	for (int pt=0;pt<nodes->getnumnodes();pt++)
	    {
	      int st=nodes->getindex(pt,0);
	      if (st==0 || st==100)
		  {
		    for (int ia=0;ia<=2;ia++)
			{
			  float nx=(nodes->getpoint(pt))->nx[ia];
			  springNodes->getpoint(springnodeno)->x[ia]=nodes->getpoint(pt)->x[ia]+Lengthfactor*nx;
			}
		    springNodes->setindex(springnodeno,0,pt);
		    springNodes->setindex(springnodeno,1,0);

		    p[0]=pt;  
		    p[1]=springnodeno+baseno;
		    springElements->AddN(p,2,baseel);
		    ++baseel;
		    ++springnodeno;

		    if (order==1)
			{
			  PXIntegerList* nei=nodes->getneighbours(pt);
			  nei->Rewind();
			  for (int n=0;n<nei->getnumnodes();n++)
			      {
				int op=nei->getcurrent();
				if (op==pt)
				    {
				      fprintf(stderr,"Error\n Zero Length Spring %d %d %d %d\n",
					      pt,nodes->getnumnodes(),n,nei->getnumnodes());
				    }
				else
				    {
				      PX3Vector vp;
				      vp.setvector(nodes->getpoint(op)->x[0]-nodes->getpoint(pt)->x[0],
						   nodes->getpoint(op)->x[1]-nodes->getpoint(pt)->x[1],
						   nodes->getpoint(op)->x[2]-nodes->getpoint(pt)->x[2]);
				      
				      vp.normalize();
				      /*for (int ib=0;ib<=2;ib++)
					  vp.x[ib]+=nodes->getpoint(pt)->nx[ib];
				      vp.normalize();*/
				      
				      for (int ia=0;ia<=2;ia++)
					  springNodes->getpoint(springnodeno)->x[ia]=nodes->getpoint(pt)->x[ia]+
					  Lengthfactor*vp.x[ia];
				      
				      /*for (int ia=0;ia<=2;ia++)
					  {
					    float nx=(nodes->getpoint(pt))->nx[ia];
					    springNodes->getpoint(springnodeno)->x[ia]=
						nodes->getpoint(pt)->x[ia]+Lengthfactor*nx;
						}*/

				      springNodes->setindex(springnodeno,0,op);
				      springNodes->setindex(springnodeno,1,1);
				      
				      p[0]=pt;
				      p[1]=springnodeno+baseno;
				      springElements->AddN(p,2,baseel);
				      ++baseel;
				      ++springnodeno;
				    }
				nei->Next();
			      }
			}
		  }
	    }
      }
}

// --------------------------------------------------------------------------
void PXAbaqusSolid::allocateNodeSpace(int numnode)
{
  if (hasnodes())
      cleanupNodeSpace();
  
  nodes=new PXSolidNodes(numnode);
  hasNodes=1;
  
}
// --------------------------------------------------------------------------
void PXAbaqusSolid::allocateElementSpace()
{
  if (haselements())
      cleanupElementSpace();

  elements=new PXNeighboursList();
  hasElements=1;
}
// --------------------------------------------------------------------------
// Geometry Related
// ----------------
// --------------------------------------------------------------------------
int PXAbaqusSolid::generateTetrahedralSolid(float spacing,int bias)
{
  spacing++; bias++;
  /*
     elementtype=4;
  char line[200];
  PXNeighboursList* tetra=new PXNeighboursList();
  PXTriangulatedStack::save2qhull("test.qhull",tstack,2);
  sprintf(line,"qhull < test.qhull d QJ o Qs > test.geom");
  if (debugmode)
      fprintf(stderr,"%s \n",line);
  system(line);
  int nt;
  PXTriangulatedStack::loadqhull("test.geom",tstack,2,tetra,nt);

  // Point Based Cleaning up
  // -----------------------
  PXTriangulatedStack::cleanup(tstack,2,tetra,nt);
  
  int total=0;
  for (int i=0;i<2;i++)
      total+=tstack[i]->getnumpoints();

  allocateElementSpace();
  allocateNodeSpace(total);
  
  // 2. Add Original Nodes
  // ---------------------
  int* totals=PXTriangulatedStack::gettotal(tstack,2);
  
  for ( i=0;i<total;i++)
      {
	PXTriangulatedStackPoint* point=getpoint(i,totals);
	for (int ia=0;ia<=2;ia++)
	    nodes->getpoint(i)->x[ia]=point->x[ia];
	for (int ib=0;ib<=2;ib++)
	    nodes->getpoint(i)->x[ib]=point->k[ib];
	nodes->setindex(i,0,PXTriangulatedStack::whichstack(i,totals,2));
	nodes->setindex(i,1,i-totals[nodes->getindex(i,0)]);
      }

  //3. Add Elements
  // ---------------

  tetra->Rewind();
  int p[4],elno=0;
  for (i=0;i<tetra->getnumnodes();i++)
      {
	if (debugmode)
	    {
	      if (int(i/50)*50==i && debugmode)
		  fprintf(stderr,"%d ",i);
	    }
	if (tetra->getcurrentindex()>=0)
	    {
	      if (debugmode)
		  {
		    if (int(i/50)*50==i && debugmode)
			fprintf(stderr,"-- %d ",i);
		  }

	      PXIntegerList* intList=((tetra->getcurrentnode())->points);
	      intList->Rewind();
	      for (int ia=0;ia<=3;ia++)
		  {
		    p[ia]=intList->getcurrent();
		    intList->Next();
		  }
	      elements->AddN(p,4,elno);
	      elno++;
	    }
	tetra->Next();
      }

  if (debugmode)
      {
	fprintf(stderr,"Done\n");
	elements->Save("elements.list");
      }
  delete tetra;
  return elements->getnumnodes();*/
  return 0;
}
// --------------------------------------------------------------------------
int PXAbaqusSolid::generateHexahedralSolid(int nodespercontour,int totalnumstacks,int bias,
					   float sample,int skipslices)
{
  elementtype=8;
  elementmode=8;
  nodespercontour=Irange(nodespercontour,10,200);
  if (debugmode)
    fprintf(stderr,"Nodes per contour = %d sample=%.1f numstacks=%d\n",nodespercontour,sample,totalnumstacks);
  char line[100];
  // Part 1, export to contourstacks and align contours properly
  // -----------------------------------------------------------
  PXContourStack* cstack[ABAQUSSOLIDMAXLEVELS-1];

  int numstacks=Irange(totalnumstacks,2,ABAQUSSOLIDMAXLEVELS-1);

  sample=Frange(sample,1.0,20.0);
  float isample=1.0/sample;
  tstack[0]->scalepoints(isample,isample);
  tstack[1]->scalepoints(isample,isample);

  cstack[0]=tstack[0]->exporttocontourstack(skipslices);
  cstack[numstacks-1]=tstack[1]->exporttocontourstack(skipslices);
  
  if (numstacks>2)
    {
      cstack[0]->Equispace(0.5);
      cstack[numstacks-1]->Equispace(0.5);
      
      if (debugmode)
	fprintf(stderr,"Slowly ...");
      
      int i;
      for (i=1;i<numstacks-1;i++)
	cstack[i]=new PXContourStack(cstack[0]->getnumcontours());
      
      // Create Inbetween Stacks 
      if (debugmode)
	fprintf(stderr,"Going to stackset\n");
      
      
      int flag=PXContourStack::createstackset(cstack,numstacks,bias);
      
      
      if (flag==0)
	{
	  fprintf(stderr,"Unequal number of slices, check surfaces, exiting %d %d\n",cstack[0]->getnumcontours(),cstack[numstacks-1]->getnumcontours());
	  for (int i=0;i<numstacks;i++)
	    delete cstack[i];
	  return 0;
	}
      
      for (i=1;i<numstacks;i++)
	{
	  cstack[i]->Equispace(0.5);
	  cstack[i]->Smooth();
	}
    }
  
  numlayers=numstacks;

  FILE* fout=NULL;

  if (debugmode)
      {
	fout=fopen("script.gpl","w");
	//fprintf(fout,"set xrange[30:75]\nset yrange[30:75]\n");
	fprintf(fout,"set grid\n");
      }
  int numslices=cstack[0]->getnumcontours();
  int mid=numslices/2;
  int direction=1;
  int sl=mid;



  for (int j=0;j<numstacks;j++)
    for (int i=0;i<numslices;i++)
      (cstack[j]->getcontour(i))->ensureAnticlockwise();

  if (debugmode)
    {
      fprintf(stderr,"Done with creating stacks\n");
      for (int i=0;i<numstacks;i++)
	{
	  fprintf(stderr,"Stack %d :",i);
	  for (int j=0;j<numslices;j++)
	    fprintf(stderr," %d(%d,%.1f) ",j,cstack[i]->getcontour(j)->getnumpoints(),
		    cstack[i]->getcontour(j)->getcontourlength());
	  fprintf(stderr,"\n");
	}
    }
		    
  
  while(sl>=0)
    {
      if (sl==mid)
	{
	  (cstack[0]->getcontour(mid))->setclosed(1);
	  (cstack[0]->getcontour(mid))->Equisample(nodespercontour+1);
	  (cstack[0]->getcontour(mid))->RemovePoint();
	  if (debugmode)
	    fprintf(stderr,"Number of points = %d\n",(cstack[0]->getcontour(mid))->getnumpoints());
	}
      else
	{
	  PXContour* cntr2=PXContour::mapContours(cstack[0]->getcontour(sl-direction),
						  cstack[0]->getcontour(sl),1);
	  cntr2->ensureAnticlockwise();
	  cstack[0]->getcontour(sl)->copypoints(cntr2);
	  delete cntr2;
	  if (debugmode)
	    fprintf(stderr,"**** Generated stack 0 slice=%d points=%d length=%.1f\n",sl,cstack[0]->getcontour(sl)->getnumpoints(),cstack[0]->getcontour(sl)->getcontourlength());
	}
      
      if (debugmode)
	{
	  fprintf(fout,"plot ");
	  /*sprintf(line,"level%02d.%02di",sl,0);
	    (cstack[0]->getcontour(sl))->Save(line);
	    fprintf(stderr,"%s\n",line);
	    fprintf(fout," '%s' w p ,",line);*/
	}
      
      for (int cn=0;cn<numstacks-1;cn++)
	{
	  /*if (debugmode)
	    {
	    sprintf(line,"level%02d.%02di",sl,cn+1);
	    (cstack[cn+1]->getcontour(sl))->Save(line);
	    fprintf(stderr,"%s\n",line);
	    fprintf(fout,"'%s' w p, ",line);
	    }*/
	  
	  PXContour* cntr=PXContour::mapContours(cstack[cn]->getcontour(sl),
						 cstack[cn+1]->getcontour(sl));
	  cntr->ensureAnticlockwise();

	  if (debugmode)
	    {
	      sprintf(line,"level%02d.%02d",sl,cn);
	      PXContour::saveContourPair(line,cstack[cn]->getcontour(sl),cntr);
	      fprintf(stderr,"\n------------\n%s",line);
	      fprintf(fout,"'%s' w l ",line);
	      
	      if (cn<numstacks-2)
		fprintf(fout," ,");
	      else
		fprintf(fout,"\n");
	      
	    }
	  cstack[cn+1]->getcontour(sl)->copypoints(cntr);
 	  if (debugmode)
	    fprintf(stderr,"**** Generated stack %d slice=%d points=%d length=%.1f\n",cn+1,sl,cstack[cn+1]->getcontour(sl)->getnumpoints(),cstack[cn+1]->getcontour(sl)->getcontourlength());

	  delete cntr;
	  
	}
      if (debugmode)
	fprintf(fout,"\npause -1\n");
      
      sl+=direction;
      if (sl>=numslices)
	{
	  sl=mid-1;
	  direction=-1;
	}
    }
  if (debugmode && fout!=NULL)
    fclose(fout);


  tstack[0]->scalepoints(sample,sample);
  tstack[1]->scalepoints(sample,sample);
  
  // Part 2, parse stacks to create elements
  // ---------------------------------------
  allocateElementSpace();
  allocateNodeSpace(nodespercontour*numstacks*numslices);
  
  numpointscontour=nodespercontour;



  // Do Nodes first 
  // --------------
  for (int layer=0;layer<numstacks;layer++)
    for (int slice=0;slice<numslices;slice++)
      {
	PXContourPoint *pts2=(cstack[layer]->getcontour(slice))->gettoppoint();
	for (int pt=0;pt<nodespercontour;pt++)
	  {
	    int i=layer*numslices*nodespercontour+slice*nodespercontour+pt;
	    nodes->getpoint(i)->x[0]=pts2->x*sample; 
	    nodes->getpoint(i)->x[1]=pts2->y*sample;
	    nodes->getpoint(i)->x[2]=cstack[layer]->getzlevel(slice);
	    
	    // Add Curvatures
	    if (layer==0 || layer==numstacks-1)
	      {
		int index=0;
		float x=nodes->getpoint(i)->x[0];
		float y=nodes->getpoint(i)->x[1];
		float z=nodes->getpoint(i)->x[2];
		
		tstack[layer>0]->findnearestpoint(x,y,z,index,0,0.2);
		nodes->getpoint(i)->k[0]=(tstack[layer>0]->getpoint(index))->k[0];
		nodes->getpoint(i)->k[1]=(tstack[layer>0]->getpoint(index))->k[1];
		for (int ia=0;ia<=2;ia++)
		  nodes->getpoint(i)->nx[ia]=tstack[layer>0]->getpoint(index)->nx[ia];
		
		if (layer==0)
		  nodes->setindex(i,0,0);
		else if (layer==numstacks-1)
		  nodes->setindex(i,0,100);
	      }
	    else
	      {
		if (bias==1 && layer==1)
		  {
		    nodes->setindex(i,0,int(100*(layer)/(numstacks)));
		  }
		else
		  nodes->setindex(i,0,int((100*layer)/(numstacks-1)));
	      }
	    pts2=pts2->next;
	  }
      }
  
  tstack[0]->setfromcontourstack(cstack[0],0);
  tstack[1]->setfromcontourstack(cstack[numstacks-1],0);
  int i;
  for (i=0;i<numstacks;i++)
      delete cstack[i];
   
  for (i=0;i<nodes->getnumnodes();i++)
    {
      int st=nodes->getindex(i,0);
      if (st==0 || st==100)
	{
	  float x=nodes->getpoint(i)->x[0];
	  float y=nodes->getpoint(i)->x[1];
	  float z=nodes->getpoint(i)->x[2];
	  int index=0;
	  tstack[st>0]->findnearestpoint(x,y,z,index,0,0.02);
	  nodes->setindex(i,1,index);
	}
    }
  
  // Elements Next
  // -------------
  int p[8];
  int numlevels=numslices-1;
  int sliceoffset=(numslices*nodespercontour);
  for (int st=0;st<numstacks-1;st++)
    for (int sl=0;sl<numlevels;sl++)
      for(int pt=0;pt<nodespercontour;pt++)
	{
	  int pp=pt+1;
	  if (pp>=nodespercontour)
	    pp-=(nodespercontour);
	  int elno=st*numlevels*nodespercontour+sl*nodespercontour+pt;
	  p[0]=sliceoffset*st    +nodespercontour*sl    +pt;
	  p[1]=sliceoffset*(st+1)+nodespercontour*sl    +pt;
	  p[2]=sliceoffset*(st+1)+nodespercontour*sl    +pp;
	  p[3]=sliceoffset*st    +nodespercontour*sl    +pp;
	  p[4]=sliceoffset*st    +nodespercontour*(sl+1)+pt;
	  p[5]=sliceoffset*(st+1)+nodespercontour*(sl+1)+pt;
	  p[6]=sliceoffset*(st+1)+nodespercontour*(sl+1)+pp;
	  p[7]=sliceoffset*st    +nodespercontour*(sl+1)+pp;
	  elements->AddN(p,8,elno);
	}
  
  hasmidwallnodes=numstacks>2;
  hasmidwallelements=numstacks>3;
  return elements->getnumnodes();
}
// --------------------------------------------------------------------------
int PXAbaqusSolid::\
setCurvaturesFromDenserStacks(PXTriangulatedStack* denseInner,PXTriangulatedStack*  denseOuter)
{
  if (!hasnodes())
      return 0;
  
  tstack[0]->updateCurvaturesFromStack(denseInner);
  tstack[1]->updateCurvaturesFromStack(denseOuter);
  
  for (int i=0;i<nodes->getnumnodes();i++)
      {
	int st=nodes->getindex(i,0);
	if (st>=0)
	    {
	      st=st>0;
	      int pt=nodes->getindex(i,1);
	      PXTriangulatedStackPoint* point=tstack[st]->getpoint(pt);
	      for (int ia=0;ia<=1;ia++)
		  nodes->getpoint(i)->k[ia]=point->k[ia];
	    }
      }
  
  return 1;
}
// --------------------------------------------------------------------------
// AbaqusRelated
// -------------
// --------------------------------------------------------------------------
void PXAbaqusSolid::setSpringParameters(int enfMode,float springk)
{
  while(enfMode>=10)
      enfMode-=10;

  enforcementMode=Irange(enfMode,-1,5);
  //             =-1 straight enforcement of surface displacements
  // Where enfMode=0 straight enforcement of ALL Displacements
  //              =1 enforcement of spring displacments
  //              =2 2-step dance with passive
  //              =3 echo 2d
  //              =4 2-step active
  //              =5 3-step actve with LL correction


  springMultiplier=springk;
}
// --------------------------------------------------------------------------

void PXAbaqusSolid::setNumericalParameters(float stepSize,int numInc)
{
  outputStep=stepSize;
  outputInc=numInc;
}

// -------------------------------------------------------------------

void px_createstring(FILE* fout,const char* newname,const char* input)
{
  fprintf(fout,"       CHARACTER*%d %s\n",strlen(input)+5,newname);         
}

void px_setstring(FILE* fout,const char* name,const char* input)
{
  fprintf(fout,"       %s='%s '\n",name,input);         
}




// --------------------------------------------------------------------------
int PXAbaqusSolid::saveAbaqusFortran(const char* fortranfile,const char* jobname,
				     const char* solidname,const char* endostem,const char* epistem,const char* priorstem,
				     int curvmode,float lamda,int frameoffset)
{

  if (enforcementMode==-1)
      {
	if (curvmode>=0)
	    curvmode=0;
	else
	    curvmode=-1;
      }

  if (jobname[0]=='/')
      {
	fprintf(stderr,"Absolute Path name error in jobname=%s\n",jobname);
	return(0);
      }
  if (solidname[0]=='/')
      {
	fprintf(stderr,"Absolute Path name error in solidname=%s\n",solidname);
	return(0);
      }
  if (endostem[0]=='/')
      {
	fprintf(stderr,"Absolute Path name error in endostem=%s\n",endostem);
	return(0);
      }
  if (epistem[0]=='/')
      {
	fprintf(stderr,"Absolute Path name error in epistem=%s\n",epistem);
	return(0);
      }

  if (priorstem[0]=='/')
      {
	fprintf(stderr,"Absolute Path name error in priorstem=%s\n",priorstem);
	return(0);
      }


  char* tmpname=new char[200],*command=new char[200],*path=new char[200];
  sprintf(tmpname,"/tmp/tempdir");


  sprintf(command,"pwd > %s",tmpname);
  //  fprintf(stderr,"executing: %s\n",command);
  system(command);

  fprintf(stderr,"Reading: %s\n",command);
  FILE *fin=fopen(tmpname,"r");
  if (fin)
      {
	fscanf(fin,"%s\n",path);
	sprintf(path,"%s/",path);
	fclose(fin);
      }
  else
      sprintf(path,"./");

  sprintf(command,"rm %s",tmpname);
  //  fprintf(stderr,"executing: %s\n",command);
  system(command);

  fprintf(stderr,"Step 1: Path=%s\n",path);

  FILE* fout=fopen(fortranfile,"w");
  if (!fout)
      return 0;

  char orient[10];
  if (orientationMode>=0)
      {
	orientationMode=Irange(orientationMode,0,6);
	switch(orientationMode)
	    {
	    case 6:
	      strcpy(orient,"FIBRESPL");
	      break;
	    case 5:
	      strcpy(orient,"XYZ123");
	      break;
	    case 4:
	      strcpy(orient,"FIBRESEP");
	      break;
	    case 3:
	      strcpy(orient,"CARDIACEP");
	      break;
	    case 2:
	      strcpy(orient,"FIBRES");
	      break;
	    case 1:
	      strcpy(orient,"CARDIAC");
	      break;
	    default:
	      strcpy(orient,"XYZ");
	      break;
	    }
      }

  float bias=0.0;
  // Fortran indentation 
  char space[10];
  strcpy(space,"     ");


  //  fprintf(fout,"*USER SUBROUTINES\n");
  fprintf(fout,"C%sAutomatically Generated Code by pxabaqusSolid.cc\nC\n",space);
  fprintf(fout,"C%s this  file   =%s\nC\n",space,fortranfile);
  fprintf(fout,"C%s path         =%s\n",space,path);
  fprintf(fout,"C%s solid file   =%s\n",space,solidname);
  fprintf(fout,"C%s endo stem    =%s\n",space,endostem);
  fprintf(fout,"C%s epi stem     =%s\nC\n",space,epistem);
  fprintf(fout,"C%s prior stem     =%s\nC\n",space,priorstem);
  fprintf(fout,"C%s curv mode    =%d\nC\n",space,curvmode);
  fprintf(fout,"C%s smooth lamda =%5.3f\nC\n",space,lamda);
  fprintf(fout,"C%s frame offset =%d\nC\n",space,frameoffset);

  // ----------------------------------------------------------------------------------

  fprintf(fout,"%s SUBROUTINE DISP(U,KSTEP,KINC,TIME,NODE,NOEL,JDOF,COORDS)\n\n",space); 
  fprintf(fout,"%s INCLUDE 'aba_param.inc'\n\n",space);
  fprintf(fout,"%s external kabsodisp\n\n",space);
  fprintf(fout,"%s DIMENSION U(3),TIME(2),COORDS(3)\n\n",space);

  fprintf(fout,"C%s Define strings new style\n",space);
  px_createstring(fout,"SS_PATHNAME",path);
  px_createstring(fout,"SS_SOLIDNAME",solidname);
  px_createstring(fout,"SS_JOBNAME",jobname);
  px_createstring(fout,"SS_ENDOSTEM",endostem);
  px_createstring(fout,"SS_EPISTEM",epistem);
  px_createstring(fout,"SS_PRIORSTEM",priorstem);

  fprintf(fout,"\n%s DOUBLE PRECISION F_BIAS,F_LAMBDA\n",space);
  fprintf(fout,"%s INTEGER I_FOFFSET,I_ENFMODE,I_CURVMODE\n\n",space);

  //  fprintf(fout,"\n%s CHARACTER*256 OUTDIR\n",space);
  //  fprintf(fout,"%s CALL GETOUTDIR(OUTDIR,LENOUTDIR)\n",space);
  //  fprintf(fout,"%s PRINT*,'OUTDIR=',OUTDIR,LENOUTDIR\n\n",space);

  px_setstring(fout,"SS_PATHNAME",path);
  px_setstring(fout,"SS_SOLIDNAME",solidname);
  px_setstring(fout,"SS_JOBNAME",jobname);
  px_setstring(fout,"SS_ENDOSTEM",endostem);
  px_setstring(fout,"SS_EPISTEM",epistem);
  px_setstring(fout,"SS_PRIORSTEM",priorstem);
  fprintf(fout,"\n\n");


  fprintf(fout,"%s F_BIAS=%f\n",space,bias);
  fprintf(fout,"%s I_FOFFSET=%d\n",space,frameoffset);
  fprintf(fout,"%s I_ENFMODE=%d\n",space,abs(enforcementMode));
  fprintf(fout,"%s I_CURVMODE=%d\n",space,curvmode);
  fprintf(fout,"%s F_LAMBDA=%f\n",space,lamda);
  fprintf(fout,"\n\n");
  
  /*  fprintf(fout,"%s PRINT*,'#  DISP---------------------------------------------'\n",space);
  fprintf(fout,"%s PRINT*,'ENFMODE=',I_ENFMODE\n",space);
  fprintf(fout,"%s PRINT*,'FOFFSET=',I_OFFSET\n",space);
  fprintf(fout,"%s PRINT*,'BIAS=',F_BIAS\n",space);
  fprintf(fout,"%s PRINT*,'CURVMODE=',I_CURVMODE\n",space);
  fprintf(fout,"%s PRINT*,'LAMBDA=',F_LAMBDA\n",space);
  fprintf(fout,"%s PRINT*,'NODE=',NODE\n",space);
  fprintf(fout,"%s PRINT*,'SS_PATHNAME=',SS_PATHNAME\n",space);
  fprintf(fout,"%s U(1)=2.0\n",space);*/
  


  fprintf(fout,"%s CALL kabsodisp(U(1),TIME(1),TIME(2),KSTEP,KINC,NODE,JDOF,\n",space);
  fprintf(fout,"%s1I_FOFFSET,I_ENFMODE,I_CURVMODE,F_BIAS,F_LAMBDA,\n",space);
  fprintf(fout,"%s1SS_PATHNAME,SS_SOLIDNAME,\n",space);
  fprintf(fout,"%s1SS_JOBNAME,SS_ENDOSTEM,\n",space);
  fprintf(fout,"%s1SS_EPISTEM,SS_PRIORSTEM)\n",space,path,solidname);
    //fprintf(fout,"%s U(1)=0.0\n",space);
  //  fprintf(fout,"%s PRINT*,'#  DISP DONE=',U(1)\n",space);
  fprintf(fout,"%s RETURN\n\n%s END\n\n\n",space,space);

  // ----------------------------------------------------------------------------------
  if (orientationMode>=0)
      {
	fprintf(fout,"%s SUBROUTINE ORIENT(T,NOEL,NPT,LAYER,KSPT,COORDS,BASIS,\n"
		"%s1ORNAME,NNODES,CNODES,JNNUM)\n\n",space,space);
	fprintf(fout,"%s INCLUDE 'aba_param.inc'\n\n",space);
	fprintf(fout,"%s external kabsoorient\n\n",space);
	fprintf(fout,"%s CHARACTER*80 ORNAME\n\n",space);         
	fprintf(fout,"%s DIMENSION T(3,3),COORDS(3),BASIS(3,3),CNODES(3,NNODES)\n\n",space);

	fprintf(fout,"C%s Define strings new style\n",space);
	px_createstring(fout,"SS_ORIENTNAME",orient);
	px_createstring(fout,"SS_PATHNAME",path);
	px_createstring(fout,"SS_SOLIDNAME",solidname);

	px_setstring(fout,"SS_ORIENTNAME",orient);
	px_setstring(fout,"SS_PATHNAME",path);
	px_setstring(fout,"SS_SOLIDNAME",solidname);
	fprintf(fout,"\n\n");

	fprintf(fout,"%s CALL kabsoorient(T(1,1),T(2,1),T(3,1),T(1,2),T(2,2),T(3,2),\n",space);
	fprintf(fout,"%s1T(1,3),T(2,3),T(3,3),NOEL,SS_ORIENTNAME,\n",space);
	fprintf(fout,"%s1SS_PATHNAME,SS_SOLIDNAME)\n\n",space);

	//	fprintf(fout,"%s PRINT*,'Orientation=', T(1,1),T(2,2),T(3,3)\n",space);

	fprintf(fout,"%s RETURN\n\n%s END\n\n\n",space,space);
      }
  
  if (enforcementMode>0 || material->getactive())
      {
	fprintf(stderr,"EnforcementMode=%d\n",enforcementMode);

	fprintf(fout,"%s SUBROUTINE UTEMP(TEMP,NSECPT,KSTEP,KINC,TIME,NODE,COORDS)\n\n",space);
	fprintf(fout,"%s INCLUDE 'aba_param.inc'\n\n",space);
	fprintf(fout,"%s external kabsotemp\n\n",space);
	fprintf(fout,"%s DIMENSION TEMP(NSECPT),TIME(2),COORDS(3)\n\n",space);

	fprintf(fout,"C%s Define strings new style\n",space);
	px_createstring(fout,"SS_PATHNAME",path);
	px_createstring(fout,"SS_SOLIDNAME",solidname);
	px_createstring(fout,"SS_JOBNAME",jobname);
	px_createstring(fout,"SS_ENDOSTEM",endostem);
	px_createstring(fout,"SS_EPISTEM",epistem);
	px_createstring(fout,"SS_PRIORSTEM",priorstem);
	
	fprintf(fout,"%s DOUBLE PRECISION F_LAMBDA\n",space);
	fprintf(fout,"%s INTEGER I_FOFFSET,I_ENFMODE,I_CURVMODE\n",space);


	px_setstring(fout,"SS_PATHNAME",path);
	px_setstring(fout,"SS_SOLIDNAME",solidname);
	px_setstring(fout,"SS_JOBNAME",jobname);
	px_setstring(fout,"SS_ENDOSTEM",endostem);
	px_setstring(fout,"SS_EPISTEM",epistem);
	px_setstring(fout,"SS_PRIORSTEM",priorstem);
	fprintf(fout,"\n\n");

	fprintf(fout,"%s I_FOFFSET=%d\n",space,frameoffset);
	fprintf(fout,"%s I_ENFMODE=%d\n",space,abs(enforcementMode));
	fprintf(fout,"%s I_CURVMODE=%d\n",space,curvmode);
	fprintf(fout,"%s F_LAMBDA=%f\n",space,lamda);
	fprintf(fout,"\n\n");

	fprintf(fout,"%s CALL kabsotemp(TEMP(1),KSTEP,KINC,TIME(1),TIME(2),NODE,\n",space);
	fprintf(fout,"%s1 I_ENF_MODE,I_FOFFSET,I_CURVMODE,F_LAMBDA,\n",space);
	fprintf(fout,"%s1SS_PATHNAME,SS_SOLIDNAME,\n",space);
	fprintf(fout,"%s1SS_JOBNAME,SS_ENDOSTEM,\n",space);
	fprintf(fout,"%s1SS_EPISTEM,SS_PRIORSTEM)\n",space,path,solidname);
	fprintf(fout,"%s RETURN\n\n%s END\n\n\n",space,space);

	if (material->getactive())
	    {
	      fprintf(stderr,"ACTIVE MATERIAL\n\n\n");
	      fprintf(fout,"%s SUBROUTINE UEXPAN(EXPAN,DEPXANDT,TEMP,TIME,DTIME,PREDEF,\n",space);
	      fprintf(fout,"%s1 DPRED,STATEV,CMNAME,NSTATV,NOEL)\n",space);
	      fprintf(fout,"%s INCLUDE 'aba_param.inc'\n\n",space);
	      fprintf(fout,"%s external kabsoexpand\n\n",space);
	      fprintf(fout,"%s CHARACTER*80 CMNAME\n\n",space);
	      fprintf(fout,"%s DIMENSION EXPAN(*),DEPXANDT(*),TEMP(2),TIME(2),PREDEF(*),\n",space);
	      fprintf(fout,"%s1 DPRED(*),STATEV(NSTATV)\n\n",space);

	      fprintf(fout,"%s I_ENFMODE=%d\n",space,abs(enforcementMode));

	      //	      fprintf(fout,"%s PRINT*,'NOEL=',NOEL\n",space);
	      //	      fprintf(fout,"%s PRINT*,'TIME=',TIME(1),TIME(2)\n",space);

	      
	      fprintf(fout,"%s CALL kabsoexpand(TEMP(1),TEMP(2),TIME(1),TIME(2),NOEL,I_ENFMODE,\n",space);
	      fprintf(fout,"%s1 EXPAN(1),EXPAN(2),EXPAN(3),EXPAN(4),EXPAN(5),EXPAN(6))\n",space);
	      //	      fprintf(fout,"%s1 DEPXANDT(1),DEPXANDT(2),DEPXANDT(3),\n",space);
	      //	      fprintf(fout,"%s1 DEPXANDT(4),DEPXANDT(5),DEPXANDT(6))\n\n",space);

	      //fprintf(fout,"%s PRINT*,'NOEL=',NOEL\n",space);
	      //	      fprintf(fout,"%s PRINT*,'EXPAN(2)=',EXPAN(2)\n",space);
	      //	      fprintf(fout,"%s PRINT*,'------------------------------------ DONE'\n",space);
	      fprintf(fout,"%s RETURN\n\n%s END\n\n\n",space,space);

	      // --------------------------------------------------------------------------------------
	      
	      /*fprintf(fout,"%s SUBROUTINE UFIELD(FIELD,KFIELD,NSECPT,KSTEP,KINC,TIME,NODE,\n",space);
	      fprintf(fout,"%s1 COORDS,TEMP,DTEMP,NFIELD)\n\n",space);
	      fprintf(fout,"%s INCLUDE 'ABA_PARAM.INC'\n\n",space);
	      fprintf(fout,"%s external kabsofield\n\n",space);
	      fprintf(fout,"%s DIMENSION  FIELD(NSECPT,NFIELD), TIME(2), COORDS(3),\n",space);
	      fprintf(fout,"%s1 TEMP(NSECPT), DTEMP(NSECPT)\n\n",space);

	      fprintf(fout,"%s DOUBLE PRECISION F_LAMBDA\n",space);
	      fprintf(fout,"%s INTEGER I_FOFFSET,I_ENFMODE,I_CURVMODE\n",space);


	      fprintf(fout,"C%s Define strings new style\n",space);
	      px_createstring(fout,"SS_PATHNAME",path);
	      px_createstring(fout,"SS_SOLIDNAME",solidname);
	      px_createstring(fout,"SS_JOBNAME",jobname);
	      px_createstring(fout,"SS_ENDOSTEM",endostem);
	      px_createstring(fout,"SS_EPISTEM",epistem);
	      px_createstring(fout,"SS_PRIORSTEM",priorstem);
	      
	      px_setstring(fout,"SS_PATHNAME",path);
	      px_setstring(fout,"SS_SOLIDNAME",solidname);
	      px_setstring(fout,"SS_JOBNAME",jobname);
	      px_setstring(fout,"SS_ENDOSTEM",endostem);
	      px_setstring(fout,"SS_EPISTEM",epistem);
	      px_setstring(fout,"SS_PRIORSTEM",priorstem);
	      fprintf(fout,"\n\n");
	      
	      fprintf(fout,"%s I_FOFFSET=%d\n",space,frameoffset);
	      fprintf(fout,"%s I_ENFMODE=%d\n",space,abs(enforcementMode));
	      fprintf(fout,"%s I_CURVMODE=%d\n",space,curvmode);
	      fprintf(fout,"%s F_LAMBDA=%f\n",space,lamda);
	      fprintf(fout,"\n\n");

	      fprintf(fout,"%s CALL kabsofield(TEMP(1),TIME(1),TIME(2),NODE,NFIELD,\n",space);
	      fprintf(fout,"%s1 KFIELD,FIELD(1,1),\n",space);
	      fprintf(fout,"%s1 KSTEP,KINC,\n",space);
	      fprintf(fout,"%s1 I_ENFMODE,I_FOFFSET,I_CURVMODE,F_LAMBDA,\n",space);
	      fprintf(fout,"%s1 SS_PATHNAME,SS_SOLIDNAME,SS_JOBNAME,\n",space);
	      fprintf(fout,"%s1 SS_ENDONAME,SS_EPINAME,SS_PRIORSTEM)\n",space);
	      fprintf(fout,"%s RETURN\n\n%s END\n\n\n",space,space);      */
	    }
      }


  // ----------------------------------------------------------------------------------
  fclose(fout);

  delete [] tmpname;
  delete [] path;
  delete [] command;

  return 1;
}
// --------------------------------------------------------------------------
int PXAbaqusSolid::insertKinematicConstraints(FILE* fout,int mode)
{
  if (!haselements() || !hasnodes() )
      return 0;

  if (mode<1 || hasmidwallnodes==0)
      return 0;

  if (numpointscontour==-1)
      {
	float botz=1000.0;//,topz=-1000.0;
	for (int i=0;i<nodes->getnumnodes();i++)
	    botz=Fmin(botz,nodes->getpoint(i)->x[2]);
	
	numpointscontour=0;
	{
	  for (int i=0;i<nodes->getnumnodes();i++)
	      if (nodes->getindex(i,0)==0 && nodes->getpoint(i)->x[2]<botz+0.1)
		  ++numpointscontour;
	}
	fprintf(stderr,"Botz=%7.4f numpoints=%d\n",botz,numpointscontour);
      }

  int numslices=nodes->getnumnodes()/(numpointscontour*numlayers);
  if (debugmode)
      fprintf(stderr,"Adding Kinematic Constraints mode=%d\n",mode);
  int added=0;

  fprintf(fout,"** ---------- Kinematic Constraints------------------------------------\n");

  for (int pt=0;pt<nodes->getnumnodes();pt++)
      {
	int la=pt/(numslices*numpointscontour);
	int sl=(pt-la*(numslices*numpointscontour))/numpointscontour;
	int pi=pt-la*(numslices*numpointscontour)-sl*numpointscontour;

	if (la>0 && la<numlayers-1)
	    {
	      if ( (sl==0 && (mode==1 || mode==3)) || (sl==numslices-1 && (mode==2 || mode==3)) )
		  {
		    ++added;
		    float w2=float(la)/float(numlayers-1);
		    float w1=1.0-w2;
		    
		    int in0=Icyclic(pi-1,numpointscontour)+sl*numpointscontour;
		    int in1=Icyclic(pi-0,numpointscontour)+sl*numpointscontour;
		    int in2=Icyclic(pi+1,numpointscontour)+sl*numpointscontour;
		    int ou0=in0+(numlayers-1)*numpointscontour*numslices;
		    int ou1=in1+(numlayers-1)*numpointscontour*numslices;
		    int ou2=in2+(numlayers-1)*numpointscontour*numslices;
		    fprintf(fout,"*EQUATION\n%d\n",7);
		    fprintf(fout,"%d,%d,%5.3f,\n" ,PXABAQUSOFFSET+pt ,3,-1.0);
		    fprintf(fout,"%d,%d,%5.3f, "  ,PXABAQUSOFFSET+in0,3,w1*0.25);
		    fprintf(fout,"%d,%d,%5.3f, "  ,PXABAQUSOFFSET+in1,3,w1*0.50);
		    fprintf(fout,"%d,%d,%5.3f,\n", PXABAQUSOFFSET+in2,3,w1*0.25);
		    fprintf(fout,"%d,%d,%5.3f, "  ,PXABAQUSOFFSET+ou0,3,w2*0.25);
		    fprintf(fout,"%d,%d,%5.3f, "  ,PXABAQUSOFFSET+ou1,3,w2*0.50);
		    fprintf(fout,"%d,%d,%5.3f\n"  ,PXABAQUSOFFSET+ou2,3,w2*0.25);
		  }
	    }
      }
  fprintf(stderr,"Added %d constraints\n",added);
  return (added);
}
// --------------------------------------------------------------------------
int PXAbaqusSolid::fixApexorBase(FILE* fout,int md)
{

  int mode;
  int user=0;
  fprintf(stderr,"FixApexorBase mode=%d user=%d\n",md,user);
  if (md<0)
      {
	md=-md;
	user=1;
      }

  //if (debugmode)
  fprintf(stderr,"FixApexorBase mode=%d user=%d\n",md,user);



  if (md>=12)
      mode=3;
  else if (md>=8)
      mode=2;
  else if (md>=4)
      mode=1;
  else
      mode=0;
    
  if (mode==1 || mode==3)
      fprintf(stderr,"Fixing Apex ..... ");
  if (mode==2 || mode==3)
      fprintf(stderr,"fixing base ..... ");
  if (user==1)
      fprintf(stderr,"User");
  fprintf(stderr,"\n");

  if (!haselements() || !hasnodes() )
      return 0;

  if (mode<1 || hasmidwallnodes==0)
      return 0;

  if (numpointscontour==-1)
      {
	float botz=1000.0;//,topz=-1000.0;
	for (int i=0;i<nodes->getnumnodes();i++)
	    botz=Fmin(botz,nodes->getpoint(i)->x[2]);
	
	numpointscontour=0;
	{
	  for (int i=0;i<nodes->getnumnodes();i++)
	      if (nodes->getindex(i,0)==0 && nodes->getpoint(i)->x[2]<botz+0.1)
		  ++numpointscontour;
	}
	fprintf(stderr,"Botz=%7.4f numpoints=%d\n",botz,numpointscontour);
      }

  int numslices=nodes->getnumnodes()/(numpointscontour*numlayers);
  if (debugmode)
      fprintf(stderr,"Fixing Apex and/or Base mode=%d\n",mode);
  int added=0;

  
  fprintf(fout,"**\n** Fixing Apex or Base\n**\n");
  if (user)
      fprintf(fout,"*BOUNDARY,USER,OP=NEW\n");
  else
      fprintf(fout,"*BOUNDARY,OP=NEW\n");
  for (int pt=0;pt<nodes->getnumnodes();pt++)
      {
	int la=pt/(numslices*numpointscontour);
	int sl=(pt-la*(numslices*numpointscontour))/numpointscontour;

	if (la==0 || la==numlayers-1)
	    {
	      if ( (sl==0 && (mode==1 || mode==3)) || (sl==numslices-1 && (mode==2 || mode==3)) )
		  {
		    if (user)
			{
			fprintf(fout," %d , 3 , 3 \n",pt+PXABAQUSOFFSET);
			}
		    else
			fprintf(fout," %d , 3 , 3 , 0.0\n",pt+PXABAQUSOFFSET);
		    ++added;
		  }
	    }
      }

  fprintf(stderr,"Fixed z  %d nodes\n",added);
  return (added);
}
// --------------------------------------------------------------------------
int PXAbaqusSolid::CreateJobBase(FILE* fout,const char* includefile,int springmode)
{
  fprintf(fout,"*HEADING\nMyocardium Model\n");
  fprintf(fout,"** numnodes=%d, numelements=%d\n",nodes->getnumnodes(),elements->getnumnodes());
  fprintf(fout,"** ----------------------------------------------------\n");
  //  fprintf(fout,"** Shrink .dat file\n*PREPRINT,ECHO=NO,MODEL=NO,HISTORY=NO\n**\n");

  if (includefile!=NULL)
      {
	fprintf(fout,"** ------------ Fortran Code ---------------------------\n");
	fprintf(fout,"**INCLUDE,INPUT=%s\n",includefile);
      }

  fprintf(fout,"** ----------Material Model --------------------------------------\n");
  
  material->SaveAbaqus(fout,"TISSUE");
  
  fprintf(fout,"** ---------- Nodes  --------------------------------------\n");
  for (int pass=0;pass<=2;pass++)
      {
	switch (pass)
	    {
	    case 0:
	      fprintf(fout,"*NODE,NSET=N_ENDO\n");
	      break;
	    case 1:
	      if (hasmidwallnodes)
		  fprintf(fout,"*NODE,NSET=N_MID\n");
	      break;
	    case 2:
	      fprintf(fout,"*NODE,NSET=N_EPI\n");
	      break;
	    }
	for (int i=0;i<nodes->getnumnodes();i++)
	    {
	      int pa=nodes->getindex(i,0);
	      
	      if (
		  (pa==0   && pass==0) || 
		  (pa==100 && pass==2) ||
		  (pass==1 && pa!=0 && pa!=100 && hasmidwallnodes) 
		  )
	    fprintf(fout," %5d , %7.3f , %7.3f , %7.3f \n",
		    i+PXABAQUSOFFSET,
		    nodes->getpoint(i)->x[0],
		    nodes->getpoint(i)->x[1],
		    nodes->getpoint(i)->x[2]);
	    }
      }
  
  if (springmode>0)
      {
	// Add Nodes to attach springs to
	fprintf(fout,"*NODE,NSET=EXTRA\n");
	for (int i=0;i<springNodes->getnumnodes();i++)
	    {
	      fprintf(fout," %6d , %7.4f , %7.4f, %7.4f\n",
		      i+nodes->getnumnodes()+PXABAQUSOFFSET,
		      springNodes->getpoint(i)->x[0],
		      springNodes->getpoint(i)->x[1],
		      springNodes->getpoint(i)->x[2]);
	    }
      }
  else
      fprintf(stderr,"Ignoring springNodes\n");
  
  fprintf(fout,"**\n");
  fprintf(fout,"*NSET, NSET=ALLNODES\n");
  fprintf(fout,"N_ENDO,N_EPI");
  if (hasmidwallnodes)
      fprintf(fout,",N_MID");
  fprintf(fout,"\n*NSET, NSET=N_DUMMY\n1000\n");
  
  
  fprintf(fout,"** ---------- Elements --------------------------------------\n");  
  char elementname[20];
  int inco=(elementmode>100);
  int numnodes=elementmode-inco*100;
  sprintf(elementname,"C3D%d",numnodes);
  if (inco)
      sprintf(elementname,"%sH",elementname);

  fprintf(stderr,"Using element type=%s\n",elementname);

  
  
  for (int layer=0;layer<numlayers-1;layer++)
      {
	fprintf(fout,"** layer=%d/%d\n",layer,numlayers);
	if (layer==0)
	  fprintf(fout,"*ELEMENT, TYPE=%s, ELSET=E_ENDO\n",elementname);
	else if (layer==numlayers-2)
	  fprintf(fout,"*ELEMENT, TYPE=%s, ELSET=E_EPI\n",elementname);
	else 
	  fprintf(fout,"*ELEMENT, TYPE=%s, ELSET=E_MID_%d\n",elementname,layer);
	
	elements->Rewind();
	int elno=PXABAQUSOFFSET;
	for (int el=0;el<elements->getnumnodes();el++)
	  {
	    PXIntegerList* intList=((elements->getcurrentnode())->points);
	    intList->Rewind();
	    int first=intList->getcurrent();
	    intList->Next();
	    int second=intList->getcurrent();
	    intList->Rewind();

	    int printelement=0;
	    int node1l=nodes->getindex(first,0);
	    int node2l=nodes->getindex(second,0);

	    if (node1l==0 && layer==0)
	      printelement=1;
	    else if (node2l==100 && layer==(numlayers-2))
	      printelement=1;
	    else if (node1l!=0 && node2l !=100 && layer!=0 && layer!=(numlayers-2))
	      printelement=1;

	    if (printelement)
	      {
		fprintf(fout,"%d,",elno);
		for (int i=0;i<intList->getnumnodes();i++)
		  {
		    fprintf(fout,"%d",intList->getcurrent()+PXABAQUSOFFSET);
		    if (i!=intList->getnumnodes()-1)
		      fprintf(fout,",");
		    else
		      fprintf(fout,"\n");
		    intList->Next();
		  }
	      }
	    elements->Next();
	    elno++;
	  }
      }
  fprintf(fout,"** \n");

  if (springmode>0)
      {
	fprintf(fout,"*ELEMENT,TYPE=SPRINGA,ELSET=ENFSET\n");
	springElements->Rewind();
	for (int i=0;i<springElements->getnumnodes();i++)
	    {
	      PXIntegerList* intList=((springElements->getcurrentnode())->points);
	      intList->Rewind();
	      int ep1=intList->getcurrent();
	      intList->Next();
	      int ep2=intList->getcurrent();
	      fprintf(fout,"%d , %d, %d\n",
		      elements->getnumnodes()+i+PXABAQUSOFFSET,
		      ep1+PXABAQUSOFFSET,ep2+PXABAQUSOFFSET);
	      springElements->Next();
	    }
	fprintf(fout,"** \n");
	fprintf(fout,"*SPRING,ELSET=ENFSET,DEPENDENCIES=2\n\n");
	fprintf(fout," %5.2f , 0.0 ,  0.0\n",0.01);
	fprintf(fout," %5.2f , 1.0 ,  0.0\n",0.01);
	fprintf(fout," %5.2f , 0.0 ,  1.0\n",springMultiplier);
	fprintf(fout," %5.2f , 1.0 ,  1.0\n",springMultiplier);
	fprintf(fout,"** \n");
      }

  
  fprintf(fout,"*ELSET, ELSET=HWALL\n");

  fprintf(fout,"E_ENDO");
  
  if (numlayers>2)
    {
      for (int ia=1;ia<numlayers-2;ia++)
	fprintf(fout,",E_MID_%d",ia);
    }
  if (numlayers>1)
    fprintf(fout,",E_EPI");



  fprintf(fout,"\n**\n");
  fprintf(fout,"*ELSET, ELSET=E_DUMMY\n1000\n**\n");
  
  fprintf(fout,"** ---------- Solid  --------------------------------------\n");
  char orient[10];
  orientationMode=Irange(orientationMode,0,4);
  if (orientationMode>=0)
      {
	switch(orientationMode)
	    {
	    case 6:
	      strcpy(orient,"FIBRESPL");
	      break;
	    case 4:
	      strcpy(orient,"FIBRESEP");
	      break;
	    case 3:
	      strcpy(orient,"CARDIACEP");
	      break;
	    case 2:
	      strcpy(orient,"FIBRES");
	      break;
	    case 1:
	      strcpy(orient,"CARDIAC");
	      break;
	    default:
	      strcpy(orient,"XYZ");
	      break;
	    }
	fprintf(fout,"*ORIENTATION,SYSTEM=USER,NAME=%s\n",orient);
	fprintf(fout,"*SOLID SECTION, ELSET=HWALL,MATERIAL=TISSUE,ORIENTATION=%s\n",orient);
      }
  else
      fprintf(fout,"*SOLID SECTION, ELSET=HWALL,MATERIAL=TISSUE\n");

  return 1;

}
// --------------------------------------------------------------------------
int  PXAbaqusSolid::CreateStepHeader(FILE* fout)
{
  fprintf(fout,"*STEP");
  if (material->getgeometry()==1)
      fprintf(fout,",NLGEOM");
  fprintf(fout,",AMPLITUDE=RAMP,INC=%d\n",outputInc);
  fprintf(fout,"*STATIC\n %f,1.0,0.00005,1.0\n",Frange(outputStep,0.05,1.0));
  fprintf(fout,"**\n");
  fprintf(fout,"** Accuracy Controls\n*CONTROLS,PARAMETERS=FIELD,FIELD=DISPLACEMENT\n");
  fprintf(fout,"0.01,1.0,10.0,  ,  , 1.E-3\n");
  return 1;
}
// --------------------------------------------------------------------------
int  PXAbaqusSolid::CreateStepFooter(FILE* fout,int resout,int fieldout)
{

  if (fieldout)
      {
	fprintf(fout,"**\n*EL PRINT,POSITION=CENTROIDAL,ELSET=HWALL\nFV\n");
	fprintf(fout,"*EL PRINT,POSITION=CENTROIDAL,ELSET=HWALL\nTHE\n");
	fprintf(fout,"*EL PRINT,POSITION=CENTROIDAL,ELSET=HWALL\nEE\n");
	fprintf(fout,"*EL PRINT,POSITION=CENTROIDAL,ELSET=HWALL\nE\n");
	fprintf(fout,"*NODE PRINT,NSET=ALLNODES\nU\n");
	fprintf(fout,"*NODE PRINT,NSET=EXTRA\nU\n");
	fprintf(fout,"*NODE PRINT,NSET=ALLNODES\nNT\n");
	fprintf(fout,"*NODE PRINT,NSET=EXTRA\nNT\n");
      }
  else
      {
	fprintf(fout,"**\n*EL PRINT,POSITION=CENTROIDAL,ELSET=E_DUMMY\nEP1\n");
	fprintf(fout,"*NODE PRINT,NSET=N_DUMMY\nU1\n");
	fprintf(fout,"*NODE PRINT,NSET=N_DUMMY\nNT\n");
      }
  fprintf(fout,"*EL FILE,POSITION=CENTROIDAL,ELSET=HWALL\nE\n");
  fprintf(fout,"*EL FILE,ELSET=HWALL\nEVOL\n*NODE FILE,NSET=ALLNODES\nU\n");
  //  fprintf(fout,"*EL FILE,ELSET=HWALL\nCOORD\n");
  fprintf(fout,"*NODE FILE,NSET=ALLNODES\nCOORD\n");


  if (resout>0)
      fprintf(fout,"*RESTART,WRITE\n");
  fprintf(fout,"*END STEP\n");
  return 1;
}

// --------------------------------------------------------------------------
int  PXAbaqusSolid::CreateSteps2DEcho(FILE* fout,int numframes,int resout)
{
  int upper=numframes*2;
  
  for (int fr=0;fr<upper;fr++)
      {
	int odd=1;
	if (2*int(fr/2)==fr)
	    odd=0;
	
	fprintf(fout,"** -------------------- Frame %d Part %d---------------------\n**\n",
		fr/2+1,odd+1);
	CreateStepHeader(fout);
	
	if (odd==1)
	    {
	      fprintf(fout,"*BOUNDARY,USER,OP=NEW\n");
	      fprintf(fout,"EXTRA,1,2\n");
	      fprintf(fout,"N_ENDO,1,2\nN_EPI,1,2\n");
	      fprintf(fout,"*BOUNDARY,OP=NEW\n");
	      fprintf(fout,"EXTRA,3,3,0.0\n");
	      fprintf(fout,"N_ENDO,3,3,0.0\nN_EPI,3,3,0.0\n");
	    }
	else
	    {
	      fprintf(fout,"*BOUNDARY,USER,OP=NEW\n");
	      fprintf(fout,"EXTRA,1,2\n");
	      fprintf(fout,"*BOUNDARY,OP=NEW\n");
	      fprintf(fout,"EXTRA,3,3,0.0\n");

	      if (material->getactive()>0)
		  {
		    fprintf(fout,"*TEMPERATURE,USER,OP=NEW\n");
		    fprintf(fout,"EXTRA,\n");
		    fprintf(fout,"N_ENDO,\n");
		    if (hasmidwallnodes)
			fprintf(fout,"N_MID,\n");
		    fprintf(fout,"N_EPI\n");
		  }
	    }
	CreateStepFooter(fout,resout,0);
      }
  return 1;
}
// --------------------------------------------------------------------------
int  PXAbaqusSolid::CreateStepsPassive3D(FILE* fout,int numframes,int resout,int flattopbottom)
{
  int upper=numframes*2;
  
  for (int fr=0;fr<upper;fr++)
      {
	int odd=1;
	if (2*int(fr/2)==fr)
	    odd=0;
	
	fprintf(fout,"** -------------------- Frame %d Part %d---------------------\n**\n",
		fr/2+1,odd+1);

	CreateStepHeader(fout);

	if (odd==1)
	    {
	      fprintf(fout,"*BOUNDARY,USER,OP=NEW\n");
	      fprintf(fout,"EXTRA,1,3\n");
	      fprintf(fout,"N_ENDO,1,3\nN_EPI,1,3\n");
	    }
	else
	    {
	      fprintf(fout,"*BOUNDARY,USER,OP=NEW\n");
	      fprintf(fout,"EXTRA,1,3\n");
	      /*if (flattopbottom>3)
		  fixApexorBase(fout,flattopbottom);*/
	      
	      fprintf(fout,"*TEMPERATURE,USER,OP=NEW\n");
	      fprintf(fout,"EXTRA,\n");
	      fprintf(fout,"N_ENDO,\n");
	      if (hasmidwallnodes)
		  fprintf(fout,"N_MID,\n");
	      fprintf(fout,"N_EPI\n");
	    }

	CreateStepFooter(fout,resout,0);
      }
  return 1;
}
// --------------------------------------------------------------------------
int  PXAbaqusSolid::CreateStepsActive3D(FILE* fout,int numframes,int resout,int flattopbottom)
{
  int numsteps=2;
  if (enforcementMode==5)
      numsteps=3;

  for (int fr=0;fr<numframes;fr++)
      {
	for (int substep=0;substep<numsteps;substep++)
	    {
	      fprintf(fout,"** -------------------- Frame %d Part %d---------------------\n**\n",
		      fr+1,substep+1);
	      fprintf(stderr,"** -------------------- Frame %d Part %d---------------------\n**\n",
		      fr+1,substep+1);
	      
	      float old=outputStep;
	      if (substep<2)
		  outputStep=1.0;
	      CreateStepHeader(fout);
	      outputStep=old;

	      if (enforcementMode==5)
		  {
		    if (substep==0)
			fprintf(fout,"*MODEL CHANGE,REMOVE,TYPE=ELEMENT\nENFSET,\n");
		    if (substep==2)
			fprintf(fout,"*MODEL CHANGE,ADD,TYPE=ELEMENT\nENFSET,\n");
		  }
		    
	      if (substep==0)
		  {
		    fprintf(fout,"*BOUNDARY,USER,OP=NEW\n");
		    if (enforcementMode==4)
			fprintf(fout,"EXTRA,1,3\n");
		    fprintf(fout,"N_ENDO,1,3\nN_EPI,1,3\n");
		  }
	      
	      if (substep==1)
		  {
		    fprintf(fout,"*BOUNDARY,USER,OP=NEW\n");
		    if (enforcementMode==5)
			{
			  for (float s=0.0;s<=0.667;s+=0.333)
			      {
				int nd=getnodenofromfractionalcoordinates(0.0,0,s);
				fprintf(fout,"%d , 1  ,3 \n",nd+PXABAQUSOFFSET);
			      }
			}
		    else
			fprintf(fout,"EXTRA,1,3\n");
		    
		    if (flattopbottom>3)
			fixApexorBase(fout,-flattopbottom);
		  }
		   
	      if (substep==2)
		  {
		    fprintf(fout,"*BOUNDARY,USER,OP=NEW\n");
		    fprintf(fout,"EXTRA,1,3\n");
		    if (flattopbottom>3)
			fixApexorBase(fout,flattopbottom);
		  }

	      if (substep==1 || substep==2)
		  {
		    /*		    for (int kk=1;kk<=4;kk++)
			{
			  fprintf(fout,"*FIELD,VARIABLE=%d,USER,OP=NEW\n",kk);
			  fprintf(fout,"ALLNODES,0.0\n");
			  }*/
		    fprintf(fout,"*TEMPERATURE,USER,OP=NEW\n");
		    if ((substep==1 && enforcementMode==5) || enforcementMode==4)
			fprintf(fout,"EXTRA,\n");
		    fprintf(fout,"N_ENDO,\n");
		    if (hasmidwallnodes)
			fprintf(fout,"N_MID,\n");
		    fprintf(fout,"N_EPI\n");
		  }
	      CreateStepFooter(fout,resout,0);
	    }
      }
  return 1;
}
// --------------------------------------------------------------------------
int  PXAbaqusSolid::CreateStepsSinglePass(FILE* fout,int numframes,int springmode,int resout,
					  int flattopbottom)
{

  float old=outputStep;
  if (springmode<=0)
      outputStep=1.0;

  for (int fr=0;fr<numframes;fr++)
      {
	fprintf(fout,"** -------------------- Frame %d ---------------------------\n**\n",fr+1);
	CreateStepHeader(fout);
	
	switch(springmode)
	    {
	      case 0:
	      fprintf(fout,"*BOUNDARY,USER,OP=NEW\n");
	      fprintf(fout,"ALLNODES,1,3\n");
	      break;
	    case -1:
	      fprintf(fout,"*BOUNDARY,USER,OP=NEW\n");
	      fprintf(fout,"N_ENDO,1,3\n");
	      fprintf(fout,"N_EPI,1,3\n");
	      break;
	    default:
	      fprintf(fout,"*BOUNDARY,USER,OP=NEW\n");
	      fprintf(fout,"EXTRA,1,3\n");
	      if (flattopbottom>3)
		  fixApexorBase(fout,flattopbottom);
	      fprintf(fout,"*TEMPERATURE,USER,OP=NEW\n");
	      fprintf(fout,"EXTRA,\n");
	      fprintf(fout,"N_ENDO,\n");
	      if (hasmidwallnodes)
		  fprintf(fout,"N_MID,\n");
	      fprintf(fout,"N_EPI\n");
	    }
	CreateStepFooter(fout,resout,0);
      }
      outputStep=old;
  return 1;
}
// --------------------------------------------------------------------------

int PXAbaqusSolid::saveAbaqus(const char* fname,int resout,int numframes,const char* includefile,
			      int flattopbottom)
{
  if (!hasnodes() || !hasmaterial() || !haselements())
      {
	fprintf(stderr,"No nodes,elements or material\n");
	return 0;
      }

  if (numframes==-1)
      numframes=0;

  // [1] Open File with proper filename
  // ----------------------------------
  char* abname=anacreatefilename(fname,".inp",".sld");
  fprintf(stderr,"%s --> %s (%d)\n",fname,abname,numframes);

  FILE* fout=fopen(abname,"w");
  if (!fout)
      {
	delete [] abname;
	return 0;
      }

  // [2] Add Nodes and Elements and Material Props etc
  // -------------------------------------------------
  int springmode=1; // 1--> creates springs 
  if (enforcementMode<=0)
      springmode=0;
  
  
  CreateJobBase(fout,includefile,springmode);

  // [3] Add Kinematic Constraints to keep bottom and top smooth
  // -----------------------------------------------------------
  int flat=flattopbottom;
  if (flat>=8) flat-=8;
  if (flat>=4) flat-=4;
  int kin=insertKinematicConstraints(fout,flat);
  fprintf(stderr,"Number of kinematic constraints=%d (mode=%d)\n",kin,flat);

  // [4] Add Initial Conditions for temperature and fields (if active)
  // -----------------------------------------------------------------
  fprintf(fout,"** -------------------- Initial Conditions --------------\n**\n");  
  fprintf(fout,"*INITIAL CONDITIONS,TYPE=TEMPERATURE\n");
  if (springmode>0)
      fprintf(fout,"EXTRA , 1.0\n");
  fprintf(fout,"ALLNODES , 1.0\n");
  
  /*  if (material->getactive())
         {
	for (int kk=1;kk<=4;kk++)
	    {
	      fprintf(fout,"*INITIAL CONDITIONS,TYPE=FIELD,VARIABLE=%d\n",kk);
	      fprintf(fout,"ALLNODES , 0.0\n");
	    }
	    }*/
  
  // [5] Create a dummy step just to make sure of everything
  // -------------------------------------------------------
  fprintf(fout,"** -------------------- Dummy Step ---------------------------\n**\n");
  fprintf(fout,"** Dummy Step\n");
  fprintf(fout,"*STEP");
  if (material->getgeometry()==1)
      fprintf(fout,",NLGEOM");
  fprintf(fout,",INC=50\n*STATIC\n*BOUNDARY\nN_ENDO,1,3,0.0\nN_EPI,1,3,0.0\n");
  if (springmode>0 && enforcementMode!=0)
      fprintf(fout,"EXTRA,1,3,0.0\n");
  fprintf(fout,"*EL PRINT,POSITION=CENTROIDAL,ELSET=E_DUMMY\nEP1\n");
   
  fprintf(fout,"*NODE PRINT,NSET=N_DUMMY\nU1\n");
  fprintf(fout,"*EL FILE,POSITION=CENTROIDAL,ELSET=HWALL\nE\n");
  fprintf(fout,"*EL FILE,ELSET=HWALL\nEVOL\n*NODE FILE,NSET=ALLNODES\nU\n");
  //  fprintf(fout,"*EL FILE,ELSET=HWALL\nCOORD\n");
  fprintf(fout,"*NODE FILE,NSET=ALLNODES\nCOORD\n");
  if (resout>0)
      fprintf(fout,"*RESTART,WRITE\n");
  fprintf(fout,"*END STEP\n");
  
  /* enforcement Modes
     All Displacements Specified via *USER option and all temperatures via *USER
     
     First two modes 
     Mode 0= Simply enforce displacements on all nodes
     Mode-1=Enforce displacements of boundary nodes;
     Mode 1= Attach Springs and enforce displacements on outer spring node via user
     Mode 2= Two Step Process first enforce outer then both (passive)
     Mode 3= 2D Echo
     Mode 4= 3-step active

     */

  // [6] Create real steps depending on enforcementMode
  // --------------------------------------------------

  fprintf(fout,"** -------------------- Real Steps ---------------------------\n**\n");
  fprintf(stderr,"*** ---- Creating Real Steps (enfMode=%d , resout=%d)\n",enforcementMode,resout);
  switch (enforcementMode)
      {
      case 2:
	CreateStepsPassive3D(fout,numframes,resout,flattopbottom);
	break;
      case 3:
	CreateSteps2DEcho(fout,numframes,resout);
	break;
      case 4:
	CreateStepsActive3D(fout,numframes,resout,flattopbottom);
	break;
      case 5:
	CreateStepsActive3D(fout,numframes,resout,flattopbottom);
	break;
      default:
	CreateStepsSinglePass(fout,numframes,enforcementMode,resout,flattopbottom);
	break;
      }
  
  fprintf(fout,"** End of File\n");
  fclose(fout);
  return 1;
}
// --------------------------------------------------------------------------
PXTriangulatedStackPoint* PXAbaqusSolid::getpoint(int index,int *total,int numstacks)
{
  int st=PXTriangulatedStack::whichstack(index,total,numstacks);
  int pt=index-total[st];
  return tstack[st]->getpoint(pt);
}
// --------------------------------------------------------------------------
int PXAbaqusSolid::Save(const char* fname)
{
  if (!haselements() || !hasnodes())
      return 0;
  
  FILE* fout=fopen(fname,"w");
  if (fout!=NULL)
      {
	int st=Save(fout);
	fclose(fout);
	return st;
      }
  return 0;
}
// --------------------------------------------------------------------------
int PXAbaqusSolid::Save(FILE* fout)
{
  if (!haselements() || !hasnodes())
      return 0;

  fprintf(fout,"#NewAbaqusSolid3\n");
  fprintf(fout,"#Number of layers,points per slice \n");
  fprintf(fout,"%d\n",numlayers);
  fprintf(fout,"%d\n",numpointscontour);
  for (int t=0;t<2;t++)
      tstack[t]->Save(fout);
  fprintf(fout,"# ---------------------------------------------------\n");
  fprintf(fout,"#Material Model\n");
  if (!hasmaterial())
      fprintf(fout,"# No Material\n");
  else
      material->Save(fout);
  fprintf(fout,"# ---------------------------------------------------\n");
  fprintf(fout,"# Solid Nodes\n");
  nodes->Save(fout);
  fprintf(fout,"# ---------------------------------------------------\n");
  fprintf(fout,"# Spring Nodes\n");
  if (hasspringnodes())
      springNodes->Save(fout);
  else
      fprintf(fout,"#No Spring Nodes\n");

  fprintf(fout,"# ---------------------------------------------------\n");
  fprintf(fout,"# Solid Elements\n");
  elements->Save(fout);

  fprintf(fout,"# ---------------------------------------------------\n");
  fprintf(fout,"# Spring Elements\n"); 
  if (hasspringelements())
      springElements->Save(fout);
  else
      fprintf(fout,"#No Spring Elements\n"); 
  fprintf(fout,"# ---------------------------------------------------\n");
  fprintf(fout,"#Numerical Constants\n");
  fprintf(fout,"#Element Mode\n%d\n",elementmode);
  fprintf(fout,"#Output Step\n%6.3f\n",outputStep);
  fprintf(fout,"#Output Inc\n%d\n",outputInc);
  fprintf(fout,"#Enforcement Strategy\n%d\n",enforcementMode);
  fprintf(fout,"#Spring Multiplier\n%5.3f\n",springMultiplier);
  fprintf(fout,"#Orientation\n%d\n",orientationMode);
  return 1;
  
}
// --------------------------------------------------------------------------
int PXAbaqusSolid::Load(const char* filename)
{

  //fprintf(stderr,"In solid loading filename %s\n",filename);

  gzFile fin=gzsuffixopen(filename,"r");
  //fprintf(stderr,"After gzFile\n");
  if (!fin)
      {
	if (debugmode)
	    fprintf(stderr,"File: %s not found\n",filename);
	return 0;
      }
  int st=Load(fin);
  gzclose(fin);

  return st;
}
// ------------------------------------------------------------------------
int PXAbaqusSolid::Load(gzFile fin)
{
  char line[200];
  gzgets(fin,line,200);
  //fprintf(stderr,"Line=%s\n",line);
  int mode=0;
  if (gzfindstringinstring(line, "#NewAbaqusSolid3"))
      mode=3;
  else
      {
	if (gzfindstringinstring(line, "#NewAbaqusSolid2"))
	    mode=2;
	else 
	    {
	      if (gzfindstringinstring(line, "#NewAbaqusSolid"))
		  mode=1;
	    }
      }

  if (debugmode)
      fprintf(stderr,"Line=%s\n",line);
  if (mode==0)
      return 0;
  
  // Step 1: Read in Stacks
  // ----------------------
  cleanup();
  init();
  
  int numstacks=2;
  gzgets(fin,line,200);   gzgets(fin,line,200);
  sscanf(line,"%d",&numlayers);
  if (debugmode)
      fprintf(stderr,"Numstacks=%d\n",numlayers);

  if (mode==3)
      {
	gzgets(fin,line,200);
	sscanf(line,"%d",&numpointscontour);
      }
  hasmidwallnodes=numlayers>1;
  hasmidwallelements=numlayers>2;

  tstack[0]=new PXTriangulatedStack();
  tstack[1]=new PXTriangulatedStack();
  
  if (mode==1)
      numstacks=numlayers;

  for (int i=0;i<numstacks;i++)
      {
	if (debugmode)
	    fprintf(stderr,"Reading stack %d ... ",i);

	int ok=tstack[i>0]->Load(fin);
	if (debugmode)
	    fprintf(stderr,"status %d\n",ok);
      }
  
  if (debugmode)
      fprintf(stderr,"Done Stacks\n");
  
  // Step 2: Read in Material Model
  // ------------------------------
  gzgets(fin,line,200);   gzgets(fin,line,200);
  if (debugmode)
    fprintf(stderr,"%s",line);
  allocateMaterial();
  if (material->Load(fin)==0)
      cleanupMaterial();
  
  // Step 3: Read in Nodes and Elements
  // ----------------------------------
  gzgets(fin,line,200);   gzgets(fin,line,200);
  if (debugmode) fprintf(stderr,"%s",line);
  allocateNodeSpace(1);

  springNodes=new PXSolidNodes(1);
  springElements=new PXNeighboursList();
  hasSpringNodes=1;
  hasSpringElements=1;

  if (nodes->Load(fin)==0)
      {
	cleanupNodeSpace();
	return 0;
      }

  allocateElementSpace();

  gzgets(fin,line,200);   gzgets(fin,line,200);
  if (debugmode) fprintf(stderr,"%s",line);
  int spN=springNodes->Load(fin);
  if (debugmode)
      if (spN>0)
	  fprintf(stderr,"Numspr nd =%d \n",springNodes->getnumnodes());
      else
	  fprintf(stderr,"No Spring Nodes\n");

  gzgets(fin,line,200);   gzgets(fin,line,200);
  if (debugmode) fprintf(stderr,"%s",line);
  elements->Load(fin);
  if (debugmode) fprintf(stderr,"Numelements=%d\n",elements->getnumnodes());

  gzgets(fin,line,200);   gzgets(fin,line,200);
  if (debugmode) 
      fprintf(stderr,"%s",line);
  int spS=springElements->Load(fin);
  if (debugmode)
      if (spS>0)
	  fprintf(stderr,"Numspr el =%d \n",springElements->getnumnodes());
      else
	  fprintf(stderr,"No Spring Elements \n");

  if (spS==0 || spN==0)
      cleanupSpringSpace();	


  if (debugmode)
    {
      fprintf(stderr,"Stats nodes=%d hasnodes=%d, elements=%d, haselements=%d\n",
	      nodes->getnumnodes(),
	      hasnodes(),
	      elements->getnumnodes(),
	      haselements());
      
      if (hasspringnodes() && hasspringelements())
	fprintf(stderr,"\tspringnodes=%d hasspringnodes=%d, springelements=%d, hasspringelements=%d\n",
		springNodes->getnumnodes(),
		hasspringnodes(),
		springElements->getnumnodes(),
		hasspringelements());
    }
  
  // Step 4: Read in Constants
  // -------------------------
  gzgets(fin,line,200);   gzgets(fin,line,200);
  if (debugmode) fprintf(stderr,"%s",line);
  gzgets(fin,line,200);   gzgets(fin,line,200); sscanf(line,"%d",&elementmode);
  gzgets(fin,line,200);   gzgets(fin,line,200); sscanf(line,"%f",&outputStep);
  gzgets(fin,line,200);   gzgets(fin,line,200); sscanf(line,"%d",&outputInc);
  gzgets(fin,line,200);   gzgets(fin,line,200); sscanf(line,"%d",&enforcementMode);
  gzgets(fin,line,200);   gzgets(fin,line,200); sscanf(line,"%f",&springMultiplier);

  if (gzgets(fin,line,200))
      {
	gzgets(fin,line,200);
	sscanf(line,"%d",&orientationMode);
      }
  else
      orientationMode=0;
  
  // Step 5: Cleanup up all the rest
  // -------------------------------
  cleanupFemData();
  if (hasvolumes())
      {
	delete [] volume;
	hasVolumes=0;
      }
  
  if (mode!=3)
      {
	float botz=1000.0;
	for (int i=0;i<nodes->getnumnodes();i++)
	    botz=Fmin(botz,nodes->getpoint(i)->x[2]);
	numpointscontour=0;
	{
	  for (int i=0;i<nodes->getnumnodes();i++)
	    if (nodes->getindex(i,0)==0 && nodes->getpoint(i)->x[2]<botz+0.1)
		++numpointscontour;
	}
	//fprintf(stderr,"Load:: Botz=%7.4f numpoints=%d\n",botz,numpointscontour);
      }
  return mode;
}
// ------------------------------------------------------------------------
void PXAbaqusSolid::getdirections(PXIntegerList* element,
				  float& nx1,float& ny1,float& nz1,
				  float& nx2,float& ny2,float& nz2,
				  float& nx3,float& ny3,float& nz3,
				  int movetoepi)
{
  if (!haselements() || !hasnodes() || element->getnumnodes()!=8)
      return;
  
  element->Rewind();
  
  PX3Vector c0(0.0 ,0.0,0.0);
  PX3Vector c1(0.0 ,0.0,0.0);
  PX3Vector r0(0.0 ,0.0,0.0);
  PX3Vector r1(0.0 ,0.0,0.0);

  int nodeoffset=0;
  if (movetoepi && numlayers>1)
      {
	int offsetmult=nodes->getnumnodes()/numlayers;
	int basest=int(0.5+float(numlayers)*
		       float(nodes->getindex(element->getcurrent(),0))/float(100.0));
	nodeoffset=((numlayers-2)-basest)*offsetmult;
      }
    

  for (int localnode=1;localnode<=8;localnode++)
      {
	int index=element->getcurrent()+nodeoffset;

	if (localnode==1 || localnode==2 || localnode==5 || localnode==6)
	    { // Anticlockwise-most face
	      for (int ia=0;ia<=2;ia++)
		  {
		    c0.x[ia]+=nodes->getpoint(index)->x[ia]; 
		    if (hasfemdata())
			c0.x[ia]+=femData->getdispcomponent(index,ia);
		  }
	    }
	else
	    { // Clockwisemost face  
	      for (int ia=0;ia<=2;ia++)
		  {
		    c1.x[ia]+=nodes->getpoint(index)->x[ia]; 
		    if (hasfemdata())
			c1.x[ia]+=femData->getdispcomponent(index,ia);
		  }
	    }

	if (localnode==1 || localnode==4 || localnode==5 || localnode==8)
	    { // Anticlockwise-most face
	      for (int ia=0;ia<=2;ia++)
		  {
		    r0.x[ia]+=nodes->getpoint(index)->x[ia]; 
		    if (hasfemdata())
			r0.x[ia]+=femData->getdispcomponent(index,ia);
		  }
	    }
	else
	    { // Clockwisemost face  
	      for (int ia=0;ia<=2;ia++)
		  {
		    r1.x[ia]+=nodes->getpoint(index)->x[ia]; 
		    if (hasfemdata())
			r1.x[ia]+=femData->getdispcomponent(index,ia);
		  }
	    }

	element->Next();
      }

  
  // Store circumferential direction in c1
  // -------------------------------------

  /*if (count==0)
      {
	fprintf(stderr,"\nc1= %6.3f %6.3f %6.3f  -",
		c1.x[0],c1.x[1],c1.x[2]);
	fprintf(stderr,"c0= %6.3f %6.3f %6.3f  =",
		c0.x[0],c0.x[1],c0.x[2]);
      }*/
  c1.subtract(&c0);  
  c1.normalize();   
  /*if (count==0)
      fprintf(stderr,"c1= %6.3f %6.3f %6.3f  \n",
	      c1.x[0],c1.x[1],c1.x[2]);*/
  
  // Store initial radial direction in r1
  // ------------------------------------
  /*if (count==0)
      {
	fprintf(stderr,"r1= %6.3f %6.3f %6.3f  -",
		r1.x[0],r1.x[1],r1.x[2]);
	fprintf(stderr,"r0= %6.3f %6.3f %6.3f  =",
		r0.x[0],r0.x[1],r0.x[2]);
      }*/

  r1.subtract(&r0);
  r1.normalize();

  /*if (count==0)
      fprintf(stderr,"r1= %6.3f %6.3f %6.3f  \n",
	      r1.x[0],r1.x[1],r1.x[2]);*/
  
  // Find component of r1 along c1 and subtract if from r1
  // -----------------------------------------------------
  float dotp=c1.dotwith(&r1);
  r1.subtract(&c1,dotp);
  r1.normalize();

  /*if (count==0)
      fprintf(stderr,"dot=%5.2f r1= %6.3f %6.3f %6.3f  \n",
	      dotp,r1.x[0],r1.x[1],r1.x[2]);*/

  // Find l1 as c1 * r1 
  // ------------------
  PX3Vector l1;
  l1.copy(&r1);
  l1.crosswithoverwrite(&c1);
  l1.normalize();
  

  /*if (count==0)
      {
	fprintf(stderr,"\n Vectors\n----------\n");
	fprintf(stderr,"r1= %6.3f %6.3f %6.3f  \n",
		r1.x[0],r1.x[1],r1.x[2]);
	fprintf(stderr,"c1= %6.3f %6.3f %6.3f  \n",
		c1.x[0],c1.x[1],c1.x[2]);
	fprintf(stderr,"l1= %6.3f %6.3f %6.3f  \n",
		l1.x[0],l1.x[1],l1.x[2]);
	count=1;
      }*/

  // Copy over the vectors
  // ---------------------
  nx1=r1.x[0]; ny1=r1.x[1]; nz1=r1.x[2];
  nx2=c1.x[0]; ny2=c1.x[1]; nz2=c1.x[2];
  nx3=l1.x[0]; ny3=l1.x[1]; nz3=l1.x[2];
  
}
// -------------------------------------------------------------------------
void PXAbaqusSolid::getElementCentroid(PXIntegerList* element,float&x,float&y,float& z)
{
  float cx[3];
  for (int ia=0;ia<=2;ia++)
      cx[ia]=0.0;

  if (!hasnodes() || !haselements())
      return;

  element->Rewind();
  for (int i=0;i<element->getnumnodes();i++)
      {
	int nd=element->getcurrent();
	for (int ia=0;ia<=2;ia++)
	    {
	      cx[ia]+=nodes->getpoint(nd)->x[ia];
	      if (hasfemdata())
		  cx[ia]+=femData->getdispcomponent(nd,ia);
	    }
	element->Next();
      }
  
  x=cx[0]/float(element->getnumnodes());
  y=cx[1]/float(element->getnumnodes());
  z=cx[2]/float(element->getnumnodes());

}
// ------------------------------------------------------------------------
int PXAbaqusSolid::modifyStacksFromNodes()
{
  if (!hasnodes() || !haselements())
      return 0;

  for (int i=0;i<nodes->getnumnodes();i++)
      {
	int st=nodes->getindex(i,0);
	int pt=nodes->getindex(i,1);
	PXTriangulatedStackPoint* point=tstack[st]->getpoint(pt);
	for (int ia=0;ia<=2;ia++)
	    point->x[ia]=nodes->getpoint(i)->x[ia]+nodes->getdetails(i)->dx[ia];
      }
  return 1;
}
// ------------------------------------------------------------------------
int PXAbaqusSolid::updateFromOutputStructures(int position)
{
  if (!hasfemdata() || !hasnodes())
      return 0;

  for (int i=0;i<nodes->getnumnodes();i++)
      {
	if (position==0)
	    {
	      for (int ia=0;ia<=2;ia++)
		  nodes->getdetails(i)->dx[ia]=femData->getdispcomponent(i,ia);
	    }
	else
	    {
	      for (int ia=0;ia<=2;ia++)
		  {
		    nodes->getpoint(i)->x[ia]+=femData->getdispcomponent(i,ia);
		    nodes->getdetails(i)->dx[ia]=0.0;
		  }
	    }

	int st=nodes->getindex(i,0);
	if (st==0 || st==100)
	    {
	      int pt=nodes->getindex(i,1);
	      PXTriangulatedStackPoint* point=tstack[st>0]->getpoint(pt);
	      if (i==0)
		  fprintf(stderr,"Point 0 = (%6.3f %6.3f %6.3f) + (%5.3f %5.3f %5.3f) ---> ",
			  point->x[0],
			  point->x[1],
			  point->x[2],
			  femData->getdispcomponent(i,0),
			  femData->getdispcomponent(i,1),
			  femData->getdispcomponent(i,2));
	      for (int ia=0;ia<=2;ia++)
		  point->x[ia]=nodes->getpoint(i)->x[ia]+nodes->getdetails(i)->dx[ia];
	      if (i==0)
		  fprintf(stderr,"%6.3f %6.3f %6.3f\n",point->x[0],
			  point->x[1],point->x[2]);

	    }
      }

  for (int sur=0;sur<=1;sur++)
      {
	//fprintf(stderr,"Updating tstack[%d] zlevels\n",sur+1);
	tstack[sur]->updateStackIndexZlevels();
      }

  return 1;
}
// ------------------------------------------------------------------------- 
int PXAbaqusSolid::correctForIncormpressibility(PXAbaqusOutputData* baseData)
{
  if (!hasnodes() || !haselements() || !hasfemdata())
      return 0;
  if (elements->getnumnodes()!=baseData->getnumelements())
      return 0;

  fprintf(stderr,"+.+.+.+. Correcting for Volume by shifting down z-coordinate\n");
  int numslices=nodes->getnumnodes()/(numpointscontour*numlayers);
  int elpointslayer=(numslices-1)*numpointscontour;

  // Store current positions in details 
  for (int nd=0;nd<nodes->getnumnodes();nd++)
      for(int ia=0;ia<=2;ia++)
	  nodes->getdetails(nd)->dx[ia]=nodes->getpoint(nd)->x[ia];
    
  float maxvolf=-1000.0,minvolf=1000.0;

  for (int slice=0;slice<numslices-1;slice++)
      {
	float oldvol=0.0,newvol=0.0;
	
	for (int lb=0;lb<numlayers-1;lb++)
	    for (int pt=0;pt<numpointscontour;pt++)
		{
		  int el=(lb-1)*elpointslayer+(slice-1)*numpointscontour+pt;
		  oldvol+=baseData->getvolume(el);
		  newvol+=femData->getvolume(el);
		}
	
	float volf=oldvol/newvol-1.0;
	maxvolf=Fmax(volf,maxvolf);
	minvolf=Fmin(volf,minvolf);
	fprintf(stderr,"Slice: %2d oldvol=%10.2f newvol=%10.2f volf=%6.4f\n",slice,oldvol,newvol,volf);
	
	// Adjust Nodes
	for (int la=0;la<numlayers;la++)
	    for (int pt=0;pt<numpointscontour;pt++)
		{
		  int nd=la*(numslices*numpointscontour)+(slice+1)*numpointscontour+pt;
		  int nd0=la*(numslices*numpointscontour)+(slice)*numpointscontour+pt;
		  
		  for (int ia=0;ia<=2;ia++)
		      {
			float x=(nodes->getpoint(nd)->x[ia]-nodes->getpoint(nd0)->x[ia]);
			nodes->getpoint(nd)->x[ia]+=volf*x;
			for (int slplus=slice+2;slplus<numslices;slplus++)
			    {
			      int nd1=la*(numslices*numpointscontour)+(slplus)*numpointscontour+pt;
			      nodes->getpoint(nd1)->x[ia]+=volf*x;
			    }
		      }

		  int st=nodes->getindex(nd,0);
		  if (st==0 || st==100)
		      {
			int pt=nodes->getindex(nd,1);
			PXTriangulatedStackPoint* point=tstack[st>0]->getpoint(pt);
			for (int ia=0;ia<=2;ia++)
			    point->x[ia]=nodes->getpoint(nd)->x[ia];
		      }
		}
      }
  
  {
    for (int nd=0;nd<nodes->getnumnodes();nd++)
	{
	  float x[3];
	  for(int ia=0;ia<=2;ia++)
	      {
		x[ia]=nodes->getpoint(nd)->x[ia]-nodes->getdetails(nd)->dx[ia];
		x[ia]+=femData->getdispcomponent(nd,ia);
		nodes->getdetails(nd)->dx[ia]=0.0;
	      }
	  femData->setNodeInfo(nd,x[0],x[1],x[2]);
	}
  }

  fprintf(stderr,"\n Maximum Ratios min=%f max=%f\n",minvolf,maxvolf);
  return 1;
}
// ------------------------------------------------------------------------- 

int PXAbaqusSolid::updateCurvaturesFromStacks()
{
  if (!hasnodes() || !haselements() || !hasfemdata())
      return 0;

  for (int i=0;i<nodes->getnumnodes();i++)
      {
	for (int ia=0;ia<=2;ia++)
	    nodes->getdetails(i)->dx[ia]=femData->getdispcomponent(i,ia);

	int st=nodes->getindex(i,0);
	if (st>=0)
	    {
	      int pt=nodes->getindex(i,1);
	      PXTriangulatedStackPoint* point=tstack[st]->getpoint(pt);
	      for (int ia=0;ia<=1;ia++)
		  nodes->getpoint(i)->k[ia]=point->k[ia];
	    }
      }
  return 1;
}
// ------------------------------------------------------------------------- 
void PXAbaqusSolid::rotateFibreAngle(float& nx1,float& ny1,float& nz1,
						float& nx2,float& ny2,float& nz2,
						float nx3,float ny3,float nz3,
						float angle)
{
  /* 
     Assume Circumferential nx1,ny1,nz1
            Longitudinal    nx2,ny2,nz2
            Radial          nx3,ny3,nz3
     
	    Rotate about radial axis using Formulation in Graig Introduction to Robotics p.52
	    */
  
  // convert to radians
  angle=M_PI*angle/180.0;
  float kx=nx3,ky=ny3,kz=nz3;
  float ct=cos(angle),st=sin(angle),vt=1-cos(angle);

  float R[3][3];
  R[0][0]=kx*kx*vt+ct;          R[0][1]=kx*ky*vt-kz*st;    R[0][2]=kx*kz*vt+ky*st;
  R[1][0]=kx*ky*vt+kz*st;       R[1][1]=ky*ky*vt+ct;       R[1][2]=ky*kz*vt-kx*st;
  R[2][0]=kx*kz*vt-ky*st;       R[2][1]=ky*kz*vt+kx*st;    R[2][2]=kz*kz*vt+ct;

  float tx1=nx1,tx2=nx2;
  float ty1=ny1,ty2=ny2;
  float tz1=nz1,tz2=nz2;

  nx1=R[0][0]*tx1+R[0][1]*ty1+R[0][2]*tz1;
  ny1=R[1][0]*tx1+R[1][1]*ty1+R[1][2]*tz1;
  nz1=R[2][0]*tx1+R[2][1]*ty1+R[2][2]*tz1;

  nx2=R[0][0]*tx2+R[0][1]*ty2+R[0][2]*tz2;
  ny2=R[1][0]*tx2+R[1][1]*ty2+R[1][2]*tz2;
  nz2=R[2][0]*tx2+R[2][1]*ty2+R[2][2]*tz2;

}
// -------------------------------------------------------------------------   
//  This is the key routine for specifying fibre orientation 
// -------------------------------------------------------------------------
float PXAbaqusSolid::getFibreAngle(float z,int layer,float bottomz,float topz)

{
  // Define Base z=bottomz
  // Define Top  z=topz
  
  /*
     Fibre Angle Distribution from Guccione and McCulloch 
     in Finite Element Modelling of Ventricular Mechanics Chapter  of Theory of Heart
     page 136 Figure 6.7 */

  if (!haselements() || !hasnodes())
      return 0.0;
  
  float zperc=(z-bottomz)/(topz-bottomz);
  zperc=Frange(zperc,0.0,1.0);
  float lperc=float(layer)/float(100.0);

  float aendo=-zperc*15.0+97.0;
  float aepi = zperc*10.0-53.0;

  return lperc*(aepi-aendo)+aendo;

}
// ------------------------------------------------------------------------- 
float PXAbaqusSolid::temperatureForVolChange(float vol1,float vol2,
							int nlgeom)
{
  float strainratio=pow(vol2/vol1,float(1.0/3.0));
  if (nlgeom)
      return log(strainratio);
  else
      return strainratio;
}
// -------------------------------------------------------------------------   
int  PXAbaqusSolid::generateVolumes(const char* fnameendo,const char* fnameepi,
				    int end,int begin)
{
  if (hasvolumes())
      {
	delete [] volume;
      }
  hasVolumes=1;
  volume=PXTriangulatedStack::generateVolumes(fnameendo,fnameepi,end,begin);
  return 1;
}
// --------------------------------------------------------------------------
int PXAbaqusSolid::saveInventor(const char* fname,int mode,int cmode,float* params)
{
  if (!haselements() || !hasnodes())
      return 0;

  FILE* fout=fopen(fname,"w");
  if (!fout)
      return 0;
  int st=saveInventor(fout,mode,cmode,params);
  fclose(fout);
  return st;
}
// --------------------------------------------------------------------------
int PXAbaqusSolid::saveInventor(FILE* fout,int mode,int cmode,float* params)
{
  if (hasfemdata())
      cmode=Irange(cmode,0,11);
  else 
      cmode=-1;
  if (!haselements() || !hasnodes())
      return 0;

  fprintf(fout, "#Inventor V2.0 ascii\n\n");
  fprintf(fout, "Separator {\n");		    
  fprintf(fout, "ShapeHints { \n");	
  fprintf(fout, "  vertexOrdering  COUNTERCLOCKWISE \n");	
  fprintf(fout, "  faceType CONVEX \n");	
  fprintf(fout, " }\n");	

  int index[8],i=0,j=0;
  PXTriangle triangle[12];
  int numt=12;

  int nel=elements->getnumnodes();

  if (hasFemData)
      {
	for(i=0; i<nodes->getnumnodes(); i++)
	    {
	      nodes->getpoint(i)->x[0]+=(femData->getdisplacement(i))->x[0]; 
	      nodes->getpoint(i)->x[1]+=(femData->getdisplacement(i))->x[1];
	      nodes->getpoint(i)->x[2]+=(femData->getdisplacement(i))->x[2];
	    }
      }


  fprintf(fout, "\tCoordinate3 { point [ ");
  for(i=0; i<nodes->getnumnodes()-1; i++)
      fprintf(fout, "\n\t\t %4.2f %4.2f %4.2f,", 
	      nodes->getpoint(i)->x[0], nodes->getpoint(i)->x[1], nodes->getpoint(i)->x[2]);		
  i = nodes->getnumnodes()-1;
  fprintf(fout, "\n\t\t %4.2f %4.2f %4.2f]\n\t}\n", 
	  nodes->getpoint(i)->x[0], nodes->getpoint(i)->x[1], nodes->getpoint(i)->x[2]);		  
  if (mode>=0 && cmode>=0)
      {
	/* write out the material (color) of each triangular facet */
	
	fprintf(fout, "\tMaterial { diffuseColor [ ");		
	elements->Rewind();
	for(i=0; i<nel; i++)
	    {
	      PX3Vector color;
	      if (mode>0)
		  color_lookup(color,femData->getstraincomponent(i,cmode),params,mode);
	      else
		  {
		    int pi=Icyclic(femData->getpieindex(i),3);
		    switch(pi)
			{
			case 0:
			  color.x[0]=1.0; color.x[1]=0.0; color.x[2]=0.0; break;
			case 1:
			  color.x[0]=0.0; color.x[1]=1.0; color.x[2]=0.0; break;
			case 2:
			  color.x[0]=0.8; color.x[1]=0.8; color.x[2]=0.0; break;
			}
		  }
	      PXIntegerList* intList=((elements->getcurrentnode())->points);
	      intList->Rewind();
	      if (intList->getnumnodes()==8)
		  numt=12;
	      else
		  numt=4;
	      elements->Next();
	      
	      for(j=0; j<numt; j++)
		  {
		    if (j<(numt-1) || i<nel-1)
			fprintf(fout, "\n\t\t %4.2f %4.2f %4.2f,", 
				color.x[0], color.x[1], color.x[2]);
		    else
			fprintf(fout, "\n\t\t %4.2f %4.2f %4.2f]\n\t}\n",
				color.x[0], color.x[1], color.x[2]);
		  }
	    }
	fprintf(fout, "MaterialBinding {\n\tvalue PER_FACE\n}\n");
      }

  /* write out the facet index */ 
  fprintf(fout, "\tIndexedFaceSet {\n\t coordIndex  [ \n");
  elements->Rewind();
  for(i=0; i<nel; i++)
      {
	PXIntegerList* intList=((elements->getcurrentnode())->points);
	intList->Rewind();
	if (intList->getnumnodes()==8)
	    {
	      numt=12;
	      index[0] = intList->getcurrent();  intList->Next(); 
	      index[1] = intList->getcurrent();  intList->Next(); 
	      index[2] = intList->getcurrent();  intList->Next(); 
	      index[3] = intList->getcurrent();  intList->Next(); 
	      index[4] = intList->getcurrent();  intList->Next(); 
	      index[5] = intList->getcurrent();  intList->Next(); 
	      index[6] = intList->getcurrent();  intList->Next(); 
	      index[7] = intList->getcurrent(); 
	      
	      triangle[0].p[0]  = index[0]; triangle[0].p[1]  = index[1]; triangle[0].p[2]  = index[2];  
	      triangle[1].p[0]  = index[3]; triangle[1].p[1]  = index[3]; triangle[1].p[2]  = index[0];  
	      triangle[2].p[0]  = index[1]; triangle[2].p[1]  = index[5]; triangle[2].p[2]  = index[6];  
	      triangle[3].p[0]  = index[2]; triangle[3].p[1]  = index[6]; triangle[3].p[2]  = index[2];  
	      triangle[4].p[0]  = index[4]; triangle[4].p[1]  = index[7]; triangle[4].p[2]  = index[6];  
	      triangle[5].p[0]  = index[5]; triangle[5].p[1]  = index[4]; triangle[5].p[2]  = index[6];  
	      triangle[6].p[0]  = index[0]; triangle[6].p[1]  = index[4]; triangle[6].p[2]  = index[7];  
	      triangle[7].p[0]  = index[3]; triangle[7].p[1]  = index[3]; triangle[7].p[2]  = index[7];  
	      triangle[8].p[0]  = index[3]; triangle[8].p[1]  = index[2]; triangle[8].p[2]  = index[6];  
	      triangle[9].p[0]  = index[7]; triangle[9].p[1]  = index[7]; triangle[9].p[2]  = index[3];  
	      triangle[10].p[0] = index[4]; triangle[10].p[1] = index[5]; triangle[10].p[2] = index[1];  
	      triangle[11].p[0] = index[0]; triangle[11].p[1] = index[0]; triangle[11].p[2] = index[4];
	    }
	else
	    {
	      numt=4;
	      index[0] = intList->getcurrent();  intList->Next(); 
	      index[1] = intList->getcurrent();  intList->Next(); 
	      index[2] = intList->getcurrent();  intList->Next(); 
	      index[3] = intList->getcurrent(); 
	      
	      /* the twelve triangles */
	      triangle[0].p[0]  = index[0]; triangle[0].p[1]  = index[1]; triangle[0].p[2]  = index[2];  
	      triangle[1].p[0]  = index[0]; triangle[1].p[1]  = index[1]; triangle[1].p[2]  = index[3];  
	      triangle[2].p[0]  = index[0]; triangle[2].p[1]  = index[2]; triangle[2].p[2]  = index[3];  
	      triangle[3].p[0]  = index[1]; triangle[3].p[1]  = index[2]; triangle[3].p[2]  = index[3];  
	    }
	
	for(j=0; j<numt; j++)
	    {
	      if (numt==4)
		  {
		    if (j<(numt-1) || i<nel-1)
			fprintf(fout, "\t\t%d,%d,%d,%d, \n", 
				triangle[j].p[0], triangle[j].p[1], triangle[j].p[2],-1);
		    else
			fprintf(fout,"\t\t%d,%d,%d,%d] \n", 
				triangle[j].p[0], triangle[j].p[1], triangle[j].p[2],-1);
		  }
	      else
		  {
		    if (j<(numt-2) || i<nel-1)
			fprintf(fout, "\t\t%d,%d,%d,%d,%d, \n", 
				triangle[j].p[0], triangle[j].p[1], triangle[j].p[2],
				triangle[j+1].p[0],-1);
		    else
			fprintf(fout,"\t\t%d,%d,%d,%d,%d] \n", 
				triangle[j].p[0], triangle[j].p[1], triangle[j].p[2],
				triangle[j+1].p[0],-1);
		    j++;
		  } 
	    }
	elements->Next();
      }
  
  /* the last line */
  fprintf(fout, "}\n}\n# End of file.\n");
  if (hasFemData)
      {
	for(i=0; i<nodes->getnumnodes(); i++)
	    {
	      nodes->getpoint(i)->x[0]-=(femData->getdisplacement(i))->x[0]; 
	      nodes->getpoint(i)->x[1]-=(femData->getdisplacement(i))->x[1];
	      nodes->getpoint(i)->x[2]-=(femData->getdisplacement(i))->x[2];
	    }
      }
  return 1;
}
// -------------------------------------------------------------------------
void PXAbaqusSolid::color_lookup(PX3Vector& output,float val,float* params,int mode)
{
  if (mode==1)
      {
	// Standard Peng-Cheng Color Scheme
	if(val > 0.325)       output.x[0]=(0.75);
	else if(val > 0.275)  output.x[0]=(1.00);	
	else if(val > 0.225)  output.x[0]=(1.00);
	else if(val > 0.175)  output.x[0]=(1.00);	
	else if(val > 0.125)  output.x[0]=(1.00);	
	else if(val > 0.075)  output.x[0]=(1.00);	
	else if(val > 0.025)  output.x[0]=(1.00);	
	else if(val > -0.025) output.x[0]=(1.00);	
	else if(val > -0.075) output.x[0]=(0.80);	
	else if(val > -0.125) output.x[0]=(0.40);	
	else if(val > -0.175) output.x[0]=(0.0);	
	else if(val > -0.225) output.x[0]=(0.60);	
	else if(val > -0.275) output.x[0]=(0.20);	
	else if(val > -0.325) output.x[0]=(0.0);	
	else                  output.x[0]=(0.0);	

	if(val > 0.325)       output.x[1]=(0.0);
	else if(val > 0.275)  output.x[1]=(0.2);	
	else if(val > 0.225)  output.x[1]=(0.5);
	else if(val > 0.175)  output.x[1]=(0.8);	
	else if(val > 0.125)  output.x[1]=(0.8);	
	else if(val > 0.075)  output.x[1]=(1.00);	
	else if(val > 0.025)  output.x[1]=(1.00);	
	else if(val > -0.025) output.x[1]=(1.00);	
	else if(val > -0.075) output.x[1]=(1.00);	
	else if(val > -0.125) output.x[1]=(1.00);	
	else if(val > -0.175) output.x[1]=(0.5);	
	else if(val > -0.225) output.x[1]=(1.00);	
	else if(val > -0.275) output.x[1]=(0.50);	
	else if(val > -0.325) output.x[1]=(0.0);	
	else                  output.x[1]=(0.0);	

	if(val > 0.325)       output.x[2]=(0.15);
	else if(val > 0.275)  output.x[2]=(0.3);	
	else if(val > 0.225)  output.x[2]=(0.55);
	else if(val > 0.175)  output.x[2]=(0.8);	
	else if(val > 0.125)  output.x[2]=(0.0);	
	else if(val > 0.075)  output.x[2]=(0.00);	
	else if(val > 0.025)  output.x[2]=(0.75);	
	else if(val > -0.025) output.x[2]=(1.00);	
	else if(val > -0.075) output.x[2]=(0.6);	
	else if(val > -0.125) output.x[2]=(0.2);	
	else if(val > -0.175) output.x[2]=(0.0);	
	else if(val > -0.225) output.x[2]=(1.00);	
	else if(val > -0.275) output.x[2]=(1.0);	
	else if(val > -0.325) output.x[2]=(1.0);	
	else                  output.x[2]=(0.5);	

	return;
      }
  else if (mode==2)
      {
	if (params!=NULL)
	    {
	      if(val > params[1])      
		  {
		    output.x[0]=0.7;
		    output.x[1]=0.0;
		    output.x[2]=0.0;
		  }
	      else if (val>params[0])
		  {
		    output.x[0]=1.0;
		    output.x[1]=1.0;
		    output.x[2]=1.0;
		  }
	      else
		  {
		    output.x[0]=0.0;
		    output.x[1]=0.0;
		    output.x[2]=0.7;
		  }
	    }
	return;
      }
  else if (mode==3)
      {
	float v=0.0;
	if (val>0.3)
	    v=1.0;
	else if (val<=0.3 && val>=-0.3)
	    v=(val+0.3)*(1.666667);
	else
	    v=-1.0;

	for (int ia=0;ia<=2;ia++)
	    output.x[ia]=v;
      }
  else if (mode==4 || mode==5)
      {
	
	float h=0.60,s=0.0;
	
	if (mode==5)
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
	output.x[0]=r;
	output.x[1]=g;
	output.x[2]=b;
      }
  else
      {
	output.x[0]=0.1;
	output.x[1]=0.1;
	output.x[2]=0.1;
      }
}
// --------------------------------------------------------------------------
int PXAbaqusSolid::loadJobIntoSolidArray(const char* fname,PXAbaqusSolid** solids,
					 int maxsolids,int numslices,int numsectors,int rotate)
{
  gzFile fin=gzsuffixopen(fname,"r");
  if (fin==NULL)
      return 0;
  
  char line[400];
  gzgets(fin,line,200);
  int mode=0;
  
  if (gzfindstringinstring(line, "#XPResults File Set"))
      mode=1;
  else
      {
	if (gzfindstringinstring(line, "#AbaqusResults File Set"))
	    mode=2;
      }

  if (mode==0)
      {
	gzclose(fin);
	fprintf(stderr,"Not a .stat file\n");
	return 0;
      }
  
  int numframes,numel,numnodes;
  gzgets(fin,line,200); sscanf(line,"%d",&numframes);
  gzgets(fin,line,200); sscanf(line,"%d",&numnodes);
  gzgets(fin,line,200); sscanf(line,"%d",&numel);
  
  char solidname[100];  
  gzgets(fin,line,200);     gzgets(fin,line,200);  
  sscanf(line,"%s\n",&solidname[0]);
  char jobname[100]; 
  gzgets(fin,line,200);     gzgets(fin,line,200);  
  sscanf(line,"%s\n",&jobname[0]);

  fprintf(stderr,"Reading set file %s \n",fname);
  fprintf(stderr,"Job stem =`%s' solid name=`%s' frames=%d nodes=%d elements=%d\n",
	  jobname,solidname,numframes,numnodes,numel);

  gzclose(fin);

  char* path=anaextractpath(fname);


  sprintf(line,"%s%s",path,solidname);
  int ok=solids[0]->Load(line);

  if (ok==0)
      {
	fprintf(stderr,"Failed to Load solid from %s \n",line);
	return 0;
      }

  if (numel!=(solids[0]->getelements())->getnumnodes() || numnodes!=(solids[0]->getnodes()->getnumnodes()))
      {
	fprintf(stderr,"Not correct numnodes or numelements\n");
	return 0;
      }

  if (numframes>maxsolids)
      {
	fprintf(stderr,"Real num frames=%d, using only %d\n",numframes,maxsolids);
	numframes=maxsolids;
      }

  for (int ii=1;ii<numframes;ii++)
      solids[ii]->Copy(solids[0]);

  fprintf(stderr,".. Strain data from %s%s : (or=%d,rot=%d) mode=%d,",path,jobname,
	  solids[0]->getorientationmode(),rotate,mode);
  
  for (int i=0;i<numframes;i++)
      {
	solids[i]->allocateFemData();
	if (mode==1)
	    {
	      sprintf(line,"%s%s_%02d",path,jobname,i+1);
	      if (debugmode)
		  fprintf(stderr,"Reading old %s\n",line);
	      else
		  fprintf(stderr,"(%d) ",i+1);
	      (solids[i]->getfemdata())->LoadOld(line,1);
	    }
	else if (mode>1)
	    {
	      sprintf(line,"%s%s.%02d.dat",path,jobname,i+1);
	      if (debugmode)
		  fprintf(stderr,"Reading new data: %s\n",line);
	      else
		  fprintf(stderr,"%d ",i+1);
	      (solids[i]->getfemdata())->Load(line,0,1);
	    } 
      }
  
  if (debugmode)
      fprintf(stderr,"Done Reading\n");

  float rx=0.0,ry=0.0,rz=0.0;
  if (solids[0]->getnodes()->getnumnodes()>100)
      {
	sprintf(line,"%s%s.ref",path,jobname);
	if (debugmode)
	    fprintf(stderr,"Opening %s ... ",line);
	else
	    fprintf(stderr,"\n");
	
	fin=gzsuffixopen(line,"r");
	if (fin)
	    {
	      gzgets(fin,line,100);
	      sscanf(line,"%f %f %f",&rx,&ry,&rz);
	      gzclose(fin);
	    }
	else
	    fprintf(stderr,"Failed to load junction point from %s: using ",line);
	fprintf(stderr,"Junction Point (%5.2f %5.2f %5.2f)\n",rx,ry,rz);
	averageStrainsInSolidArray(solids,numframes,rx,ry,rz,numslices,numsectors);
      }
  else
      {
	sprintf(line,"%s%s.pieindex",path,jobname);
	if (debugmode)
	    fprintf(stderr,"Opening pineindex file %s ... ",line);
	else
	    fprintf(stderr,"\n");
	
	if (solids[0]->getfemdata()->loadPieDivision(line))
	    {
	      fprintf(stderr,"Loaded pies ok\n");
	      averageStrainsInSolidArray(solids,numframes);
	    }
	else
	    {
	      fprintf(stderr,"Failed...Using Junction Point (%5.2f %5.2f %5.2f)\n",rx,ry,rz);
	      averageStrainsInSolidArray(solids,numframes,rx,ry,rz,numslices,numsectors);
	    }
      }
  
  delete [] path;
  return numframes;
}
// --------------------------------------------------------------------------
int  PXAbaqusSolid::averageStrainsInSolidArray(PXAbaqusSolid** solids,int numframes)
{
  fprintf(stderr,"In the non-bothersome averaging\n");
  for (int i=0;i<numframes;i++)
    {
      if (i==0)
	{
	  (solids[0]->getfemdata())->averagestrains(solids[0]->getfemdata());
	}
      else
	{
	  (solids[i]->getfemdata())->copypieindex(solids[0]->getfemdata());
	  (solids[i]->getfemdata())->averagestrains(solids[0]->getfemdata());
	}
    }
  return 1;
}
// --------------------------------------------------------------------------
int  PXAbaqusSolid::averageStrainsInSolidArray(PXAbaqusSolid** solids,int numframes,
					       float rx,float ry,float rz,
					       int numslices,int numsectors,int anticlockwise)
{
  int oldnum=numslices;
  numslices=Irange(numslices,1,(solids[0]->getstack(0))->getstackindex().numcontours-1);
  anticlockwise=anticlockwise>0;
  if (numslices!=oldnum)
      fprintf(stderr,"Reduced numslices to %d (from %d) to avoid empty slices\n",numslices,oldnum);

  fprintf(stderr,"In the bothersome averaging\n");

  for (int i=0;i<numframes;i++)
      {
	if (i==0)
	    {
	      (solids[0]->getfemdata())->subdivideinpies(rx,ry,rz,numslices,numsectors,anticlockwise);
	      (solids[0]->getfemdata())->averagestrains(solids[0]->getfemdata());
	    }
	else
	    {
	      (solids[i]->getfemdata())->copypieindex(solids[0]->getfemdata());
	      (solids[i]->getfemdata())->averagestrains(solids[0]->getfemdata());
	    }
	//sprintf(line,"/tmp/ave.%02d.data",i+1);
	//solids[i]->getfemdata()->Save(line);
      }
  fprintf(stderr,"Done with averageStrainsIn\n");
  return 1;
}
// --------------------------------------------------------------------------
int  PXAbaqusSolid::savePies(const char* fname,PXAbaqusSolid** solids,int numframes,int mode,int skipframes)
{
  if (!solids[0]->hasfemdata())
      return 0;

  int numpies=solids[0]->getfemdata()->getnumpies();
  FILE* fout=fopen(fname,"w");
  if (fout==NULL)
    return 0;
  
  fprintf(fout,"# Pie data \n");
  int orient=solids[0]->getorientationmode();
  {
    if (orient==1 || orient==3 )
      fprintf(fout,"# Strains defined in Cardiac Coordinate Space\n");
    else if (orient==5)
      fprintf(fout,"# Principal Strains\n");
    else
      fprintf(fout,"# Strains defined in Fibre-Specific Coordinate Space\n");
  }
  
  fprintf(fout,"# LV-RV Junction Defined as (%6.2f,%6.2f,%6.2f)\n#\n",
	  solids[0]->getfemdata()->getjunctionpoint()->x[0],
	  solids[0]->getfemdata()->getjunctionpoint()->x[1],
	  solids[0]->getfemdata()->getjunctionpoint()->x[2]);
  
  float** piesmat=matrix(0,numpies-1,0,numframes-1);
  float *ave=new float[numpies];
  float *vol=new float[numpies];
  float *vol0=new float[numpies];
  int numel=solids[0]->getelements()->getnumnodes();
  
  int start=-1;
  if (mode>0)
      start=0;

  int maxstrain=5;
  if (orient==5)
    maxstrain=2;
  
  for (int strain=start;strain<=maxstrain;strain++)
    {
      for (int f=0;f<numframes;f++)
	{
	  for (int j=0;j<numpies;j++)
	    {
	      ave[j]=0.0;
	      vol[j]=0.0;
	      vol0[j]=0.0;
	    }
	  
	  for (int i=0;i<numel;i++)
	    {
	      int p=Irange((solids[f]->getfemdata())->getpieindex(i),0,numpies-1);
	      float v=(solids[f]->getfemdata())->getvolume(i);
	      vol[p]+=v;
	      vol0[p]+=(solids[0]->getfemdata())->getvolume(i);
	      if (strain>=0)
		{
		  v=(solids[0]->getfemdata())->getvolume(i);
		  ave[p]+=(solids[f]->getfemdata())->getstraincomponent(i,strain)*v;
		}
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
		piesmat[j][f]=ave[j];
	    }
	  else
	    {
	      for (int j=0;j<numpies;j++)
		piesmat[j][f]=vol[j];
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
	for (int f=0;f<numframes;f+=skipframes)
	  fprintf(fout," %7d",f+1);
      }
      fprintf(fout,"\n");
      int minj=0;
      if (solids[0]->getnodes()->getnumnodes()<100)
	minj=2;
      for (int j=minj;j<numpies;j++)
	{
	  fprintf(fout,"  %7d   ",j+1);
	  for (int f=0;f<numframes;f+=skipframes)
	    if (strain==-1)
	      fprintf(fout," %6.1f ",piesmat[j][f]);
	    else
	      fprintf(fout," %6.3f ",piesmat[j][f]);
	  
	  fprintf(fout,"\n");
	}
    }
  
  if (solids[0]->getnodes()->getnumnodes()<100)
    {
      fprintf(fout, "\n\nPie division for each elements\nElement\tPie\n");
      for (int i=0;i<numel;i++)
	{
	  int p=solids[0]->getfemdata()->getpieindex(i);
	  fprintf(fout,"%d \t%d\n!",i+1,p);
	}
    }
  
  fclose(fout);
  free_matrix(piesmat,0,numpies-1,0,numframes-1);
  delete [] ave;
  delete [] vol;
  delete [] vol0;
  return 1;
}
// --------------------------------------------------------------------------
void  PXAbaqusSolid::getElementRange(PXIntegerList* element,PXSolidNodes* nodes,
				     float cx,float cy,float phase_ref,
				     float& minz,float& maxz,
				     float& minangle,float& maxangle,
				     float& mintrans,float& maxtrans)
{
  static int test=0;
  test++;
  minangle=0.0,maxangle=0.0;
  minz=100000.0,maxz=-100000.0;
  mintrans=200.0; maxtrans=-1.0;

  float oldmin=0.0,oldmax=0.0;
  element->Rewind();
  for (int i=0;i<element->getnumnodes();i++)
    {
      int index=element->getcurrent();       element->Next();
      float x=nodes->getpoint(index)->x[0]-cx;
      float y=nodes->getpoint(index)->x[1]-cy;
      float z=nodes->getpoint(index)->x[2];
      float t=float(nodes->getindex(index,0));
      minz=Fmin(minz,z);
      maxz=Fmax(maxz,z);
      mintrans=Fmin(mintrans,t);
      maxtrans=Fmax(maxtrans,t);
      
      float phase=Fcyclic(atan2(y,x)*180/M_PI-phase_ref,360.0);
      
      if (i==0 || i==1 || i==4 || i==5)
	{
	  if (fabs(phase-oldmin)>180.0 && i!=0)
	    {
	      if (phase>oldmin)
		phase-=360.0;
	      else
		phase+=360.0;
	    }
	  minangle+=0.25*phase;
	  oldmin=phase;
	}
      else
	{
	  if (fabs(phase-oldmax)>180.0 && i!=2)
	    {
	      if (phase>oldmax)
		phase-=360.0;
	      else
		phase+=360.0;
	    }
	  maxangle+=0.25*phase;
	  oldmax=phase;
	}
    }
  
  minangle=Fcyclic(minangle,360.0);
  maxangle=Fcyclic(maxangle,360.0);

  if (maxangle-minangle>180.0)
    {
      float temp=maxangle;
      maxangle=minangle;
      minangle=temp;
    }
}
// --------------------------------------------------------------------------
float PXAbaqusSolid::returnElementFraction(float elementminz,float elementmaxz,float globalminz,
					   float globalmaxz,int slice,int numslices,int debug)
{
  float dz=(globalmaxz-globalminz)/float(numslices);

  // Pie has z_extent z0--z1
  float z1=dz*float(slice+1)+globalminz;
  float z0=dz*float(slice)+globalminz;


  float frac=0.0;

  if (elementmaxz<z0 || elementminz>z1)
      frac=0.0;
  else
      {
	// Option 1 -- Element Fully Enclosed in slice
	if (z1>=elementmaxz && z0<=elementminz)
	    frac=1.0;
      else
	  {
	    // Option 2 -- Slice Fully Enclosed in element
	    if (z1<=elementmaxz && z0>=elementminz)
		frac=(z1-z0)/(elementmaxz-elementminz);
	    else
		{
		  // Option 3 -- Element extends below slice
		  if (z0>=elementminz && z1>=elementmaxz)
		      frac=(elementmaxz-z0)/(elementmaxz-elementminz);
		  else
		      {
			// Option 4 -- Element extends above slice 
			if (z0<=elementminz && z1<=elementmaxz)
			    frac=(z1-elementminz)/(elementmaxz-elementminz);
			else
			    {
			      fprintf(stderr,"Error*\n");
			      frac=0.0;
			    }
		      }
		}
	  }
      }

  if (debug)
      {
	fprintf(stderr,"\nIn element fraction elmin=%6.2f elmax=%6.2f, globmin=%6.2f globmax=%6.2f\n",
		elementminz,elementmaxz,globalminz,globalmaxz);
	fprintf(stderr,"z1=%6.2f z2=%6.2f dz=%7.3f Fraction=%6.4f\n",z0,z1,dz,frac);
      }
  return frac;
}
			    
// --------------------------------------------------------------------------
int  PXAbaqusSolid::subdivideandSavePies(const char* fname,PXAbaqusSolid** solids,int numframes,int mode,
					 float rx,float ry,float rz,int numslices,int numsectors,
					 int numtransmural,
					 int anticlockwise,int skipframes)
{
  numslices=Irange(numslices,1,100);
  numsectors=Irange(numsectors,1,100);
  numtransmural=Irange(numtransmural,1,3);

  int numpies=numslices*numsectors*numtransmural;
  fprintf(stderr,"Numpies=%d\n",numpies);
  fprintf(stderr,"++++++++++++++ Subdivide and Save in `%s' (anti=%d) \n",fname,anticlockwise);

  if (!solids[0]->hasfemdata())
      return 0;

  FILE* fout=fopen(fname,"w");
  if (fout==NULL)
      return 0;


  fprintf(fout,"# Pie data \n");
  int orient=solids[0]->getorientationmode();
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
    for (int i=0;i<solids[0]->getnodes()->getnumnodes();i++)
      {
	float z=solids[0]->getnodes()->getpoint(i)->x[2];
	globalminz=Fmin(globalminz,z);
	globalmaxz=Fmax(globalmaxz,z);
      }
    
    
    {
      for (int i=0;i<solids[0]->getelements()->getnumnodes();i++)
	{
	  float v=solids[0]->getfemdata()->getvolume(i);
	  if (v<0.01)
	    v=1.0;
	  cx+=(solids[0]->getfemdata()->getcentroid(i))->x[0]*v;
	  cy+=(solids[0]->getfemdata()->getcentroid(i))->x[1]*v;
	  tvol+=v;
	}
    }
    cx/=tvol; cy/=tvol;
    fprintf(fout,"# Total volume= %9.3f\n",tvol);
  }
  
  // For all elements stuff
  // ----------------------
  float* elementZmin=new float[solids[0]->getelements()->getnumnodes()];
  float* elementZmax=new float[solids[0]->getelements()->getnumnodes()];
  float* elementPmin=new float[solids[0]->getelements()->getnumnodes()];
  float* elementPmax=new float[solids[0]->getelements()->getnumnodes()];
  float* elementTmin=new float[solids[0]->getelements()->getnumnodes()];
  float* elementTmax=new float[solids[0]->getelements()->getnumnodes()];

  float phase_ref=atan2(ry-cy,rx-cx)*180/M_PI;

  fprintf(stderr,"Phase_ref=%f\n",phase_ref);
  

  fprintf(fout,"# LV-RV Junction Defined as (%6.2f,%6.2f,%6.2f)\n",rx,ry,rz);
  fprintf(fout,"# Phase_ref =%6.3f degrees (cx=%6.2f,cy=%6.2f)\n",phase_ref,cx,cy);
  fprintf(fout,"# Number of slices=%d, sectors=%d\n",numslices,numsectors);

  solids[0]->getelements()->Rewind();
  for (int el=0;el<solids[0]->getelements()->getnumnodes();el++)
      {
	getElementRange(solids[0]->getelements()->getcurrentnode()->points,
			solids[0]->getnodes(),
			cx,cy,phase_ref,
			elementZmin[el],elementZmax[el],elementPmin[el],elementPmax[el],
			elementTmin[el],elementTmax[el]);
	/*	if (10*int(el/10)==el)
	    fprintf(stderr,"Element el=%d %f:%f\n",el,elementTmin[el],elementTmax[el]);*/
	solids[0]->getelements()->Next();
      }
  
  float** piesmat=matrix(0,numpies-1,0,numframes-1);
  float *ave=new float[numpies];
  float *vol=new float[numpies];
  float *vol0=new float[numpies];
  int numel=solids[0]->getelements()->getnumnodes();

  int start=-1;
  if (mode>0)
      start=0;
  
  int maxstrain=5;
  if (orient==5)
      maxstrain=2;
  
  for (int strain=start;strain<=maxstrain;strain++)
      {
	for (int f=0;f<numframes;f++)
	    {
	      for (int j=0;j<numpies;j++)
		  {
		    ave[j]=0.0;
		    vol[j]=0.0;
		    if (f==0)
			vol0[j]=0.0;
		  }
	      
	      if (strain==start)
		  (solids[f]->getfemdata())->setPieStats(numslices*numsectors*numtransmural,
							 numsectors*numtransmural,numslices);

	      for (int el=0;el<solids[0]->getelements()->getnumnodes();el++)
		  {
		    float elvol0=(solids[0]->getfemdata())->getvolume(el);
		    float elvol =(solids[f]->getfemdata())->getvolume(el);
		    
		    int bestpie=0;
		    float maxfr=0.0;
		    for (int slice=0;slice<numslices;slice++)
			{
			  float zfr=1.0;
			  if (numslices>1)
			      zfr=returnElementFraction(elementZmin[el],elementZmax[el],
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
				      
				      secfr=returnElementFraction(elminangle,elmaxangle,
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
					    wedfr=returnElementFraction(elementTmin[el],
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
						(solids[f]->getfemdata())->getstraincomponent(el,strain);
					    ave[pie]+=stcomp*frac*elvol0;
					  }
				    }
			      }
			}
		    if (strain==start)
			(solids[f]->getfemdata())->setElementPieIndex(el,bestpie);
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
			piesmat[j][f]=ave[j];
		  }
	      else
		  {
		    for (int j=0;j<numpies;j++)
			piesmat[j][f]=vol[j];
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
	  for (int f=0;f<numframes;f+=skipframes)
	      fprintf(fout," %7d",f+1);
	}
	fprintf(fout,"\n");
	int minj=0;
	if (solids[0]->getnodes()->getnumnodes()<100)
	    minj=2;
	for (int j=minj;j<numpies;j++)
	    {
	      fprintf(fout,"  %7d   ",j+1);
	      for (int f=0;f<numframes;f+=skipframes)
		  if (strain==-1)
		      fprintf(fout," %6.1f ",piesmat[j][f]);
		  else
		      fprintf(fout," %6.3f ",piesmat[j][f]);
	      fprintf(fout,"\n");
	    }
      }
  
  if (solids[0]->getnodes()->getnumnodes()<100)
      {
	fprintf(fout, "\n\nPie division for each elements\nElement\tPie\n");
	for (int i=0;i<numel;i++)
	    {
	      int p=solids[0]->getfemdata()->getpieindex(i);
	      fprintf(fout,"%d \t%d\n!",i+1,p);
	    }
      }
  
  fclose(fout);
  char line[200];
  sprintf(line,"%s.pieindex",fname);
  solids[0]->getfemdata()->savePieDivision(line);

  free_matrix(piesmat,0,numpies-1,0,numframes-1);
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
// --------------------------------------------------------------------------
int PXAbaqusSolid::updateFemDataDirections(int centroidsonly)
{
  if (!hasfemdata() || !hasnodes() || !haselements())
      return 0;
  
  femData->setPieStats(1,1,1);
  elements->Rewind();
  for (int i=0;i<elements->getnumnodes();i++)
      {
	float x,y,z;
	getElementCentroid((elements->getcurrentnode())->points,x,y,z);
	femData->setCentroid(i,x,y,z);
	if (centroidsonly==0)
	    {
	       float nx1,ny1,nz1,nx2,ny2,nz2,nx3,ny3,nz3;
	       getdirections((elements->getcurrentnode())->points,
			     nx1,ny1,nz1,nx2,ny2,nz2,nx3,ny3,nz3,0);

	       femData->setRadial(i,nx1,ny1,nz1);
	       femData->setCircum(i,nx2,ny2,nz2);
	     }
	 else
	     {
	       femData->setCircum(i,0,1,0);
	       femData->setRadial(i,1,0,0);
	     }
	femData->setElementPieIndex(i,0);
	elements->Next();
       }
  //fprintf(stderr,"Directions cent=%d, numnodes=%d\n",centroidsonly,nodes->getnumnodes());
  return 1;
 }
 // --------------------------------------------------------------------------
 int PXAbaqusSolid::getforcingnode(int nodeno,int offset)
 {
   nodeno-=offset;
   int forcingnode=0;
   if (nodeno<nodes->getnumnodes())
       forcingnode=nodeno;
   else if (nodeno>=nodes->getnumnodes() && hasspringnodes())
       {
	 nodeno-=nodes->getnumnodes();
	 forcingnode=springNodes->getindex(nodeno,0);
       }
   return forcingnode;
 }
 // --------------------------------------------------------------------------
 int PXAbaqusSolid::updateTemperaturesFromDisplacements(PXNodalDisplacements* disp,int mode)
 {
   int active=mode>0;
   if (mode<0)
       mode=-mode;
   double factor=double(mode)/(100.0);
   fprintf(stderr,"Weight Factor =%6.3f mode=%d\n",factor,mode);

   // Part 1 Interior 
   if (!hasnodes())
       return 0;

   if (disp->getsolidnodes()->getnumnodes() != nodes->getnumnodes())
       {
	 fprintf(stderr,"Incompatible Displacement Structure\n");
	 return 0;
       }

   if (active>0)
       {
	 fprintf(stderr,"Assuming all myocardium healthy T=1.0\n");
	 for (int i=0;i<nodes->getnumnodes();i++)
	     nodes->settemperature(i,1.0);
       }

   // Part 2 Spring Nodes
   if  (hasspringnodes())
       {

	 for (int i=0;i<springNodes->getnumnodes();i++)
	     {
	       int nd=springNodes->getindex(i,0);
	       float v0=factor*disp->getconfidence(nd);
	       float t1=nodes->gettemperature(nd);
	       float wgt=pow(float(0.5),float(springNodes->getindex(i,1)));

	       // spring : v0 = wgt*(t1+t2)/2.0
	       // t2= 2.0*(v0-t1)/(wgt);
	       float t2=(2.0*v0-t1)/(wgt);
	       
	       if (i==0)
		   fprintf(stderr,"Node i =%d(t=%5.2f), Real Node=%d(t=%5.2f) conf=%5.3f \n",i,t2,nd,t1,wgt);

	       springNodes->settemperature(i,t2);
	     }
       }
   return 1;
 }
// --------------------------------------------------------------------------
int PXAbaqusSolid::getnodecoordinates(int node,int& layer,int& slice,int& point)
{
  layer=0; slice=0; point=0;
  if (numpointscontour==-1 || !haselements() || !hasnodes())
      return -1;

  int numslices=nodes->getnumnodes()/(numpointscontour*numlayers);
  
  layer=node/(numslices*numpointscontour);
  slice=(node-layer*(numslices*numpointscontour))/numpointscontour;
  point=node-layer*(numslices*numpointscontour)-slice*numpointscontour;
  return 1;
}  
// --------------------------------------------------------------------------
int PXAbaqusSolid::getelementcoordinates(int elno,float& layer,float& slice,float& point)
{
  layer=0.0;  slice=0.0;  point=0.0;

  if (numpointscontour==-1 || !haselements() || !hasnodes())
      return -1;
  
  int ellayers=numlayers-1;
  int elpercontour=numpointscontour;
  int elslices=elements->getnumnodes()/(elpercontour*ellayers);

  //return la*(elslices*elpercontour)+sl*elpercontour+pt;

  int la=int(elno/(elslices*elpercontour));
  int sl=int((elno-la*elpercontour*elslices)/elpercontour);
  int pt=int(elno-la*elpercontour*elslices-sl*elpercontour);
  
  layer=float(la)/float(numlayers-1);
  slice=float(sl)/float(elslices-1);
  point=float(pt)/float(elpercontour-1);
  return 1;


}
// --------------------------------------------------------------------------
int PXAbaqusSolid::getnodenofromfractionalcoordinates(float layer,float slice,float point)
{
  int numslices=nodes->getnumnodes()/(numpointscontour*numlayers);
  
  int la=Irange(int(layer*float(numlayers)),0,numlayers-1);
  int sl=Irange(int(slice*float(numslices)),0,numslices-1);
  int pt=Irange(int(point*float(numpointscontour)),0,numpointscontour-1);

  return la*(numslices*numpointscontour)+sl*numpointscontour+pt;
} 
// --------------------------------------------------------------------------
int PXAbaqusSolid::getelementno(double layer,double slice,double point)
{
  if (numpointscontour==-1  || !haselements() || !hasnodes())
      return -1;
  
  int ellayers=numlayers-1;
  int elpercontour=numpointscontour;
  int elslices=elements->getnumnodes()/(elpercontour*ellayers);

  int la=int(layer);
  int sl=int(slice);
  int pt=int(point);

  return la*(elslices*elpercontour)+sl*elpercontour+pt;
}

// --------------------------------------------------------------------------
int PXAbaqusSolid::rotateStrainsFiberToCardiac(int inverse)
{
  if (!hasfemdata()  || !haselements() || !hasnodes())
      return -1;

  
  float** R	 = matrix(0, 2, 0, 2);
  float** R1	 = matrix(0, 2, 0, 2);
  float** R2	 = matrix(0, 2, 0, 2);
  float** E	 = matrix(0, 2, 0, 2);
  float** E2	 = matrix(0, 2, 0, 2);

  if (inverse==0)
      fprintf(stderr,"Rotating strains Fiber --> Cardiac (%d)\n",inverse);
  else
      fprintf(stderr,"Rotating strains Cardiac --> Fibre (%d)\n",inverse);
  
  elements->Rewind();
  for (int el=0;el<elements->getnumnodes();el++)
      {
	PXIntegerList* intList=((elements->getcurrentnode())->points);
	elements->Next();

	float cx,cy,cz;
	int layer=0;
	getElementCentroid(intList,cx,cy,cz);
	for (int ka=0;ka<intList->getnumnodes();ka++)
	    {
	      layer+=nodes->getindex(intList->getcurrent(),0); 
	      intList->Next();
	    }
	layer=layer/intList->getnumnodes();

	float x1,x2,x3,y1,y2,y3,z1,z2,z3;
	
	intList->Rewind();
	getdirections(intList,
		      x2,y2,z2,x3,y3,z3,x1,y1,z1,0);

	R1[0][0]=x2; R1[1][0]=y2; R1[2][0]=z2; // Radial
	R1[0][1]=x3; R1[1][1]=y3; R1[2][1]=z3; // Radial
	R1[0][2]=x1; R1[1][2]=y1; R1[2][2]=z1; // Radial
	
	float angle=getFibreAngle(cz,layer);
	
	rotateFibreAngle(x3,y3,z3,  
			 x1,y1,z1,
			 x2,y2,z2, // Keep Radial Fixed
			 angle);
	
	
	R2[0][0]=x1; R2[1][0]=y1; R2[2][0]=z1; // Cross
	R2[0][1]=x2; R2[1][1]=y2; R2[2][1]=z2; // Radial
	R2[0][2]=x3; R2[1][2]=y3; R2[2][2]=z3; // Fibre
	


	if (inverse==0)
	    {
	      // R=R1'*R2
	      for(int i=0; i<=2; i++)
		  for(int j=0; j<=2; j++)
		      {
			R[i][j]=0.0;
			for(int m=0; m<=2; m++)
			    R[i][j] += R1[m][i] * R2[m][j];
		      }
	    }
	else
	    {
	      // R=R2'*R1
	      for(int i=0; i<=2; i++)
		  for(int j=0; j<=2; j++)
		      {
			R[i][j]=0.0;
			for(int m=0; m<=2; m++)
			    R[i][j] += R2[m][i] * R1[m][j];
		      }
	    }


	/*if (el==0)
	    {
	      fprintf(stderr,"\n\n C=(%5.2f %5.2f %5.2f) an=%6.3f\n",cx,cy,cz,angle);
	      fprintf(stderr," R= %5.2f %5.2f %5.2f\n    %5.2f, %5.2f %5.2f\n    %5.2f %5.2f %5.2f\n\n",
		      R[0][0],R[0][1],R[0][2],R[1][0],R[1][1],R[1][2],R[2][0],R[2][1],R[2][2]);
	      fprintf(stderr,"R1= %5.2f %5.2f %5.2f\n    %5.2f, %5.2f %5.2f\n    %5.2f %5.2f %5.2f\n\n",
		      R1[0][0],R1[0][1],R1[0][2],R1[1][0],R1[1][1],R1[1][2],R1[2][0],R1[2][1],R1[2][2]);
	      fprintf(stderr,"R2= %5.2f %5.2f %5.2f\n    %5.2f, %5.2f %5.2f\n    %5.2f %5.2f %5.2f\n\n",
		      R2[0][0],R2[0][1],R2[0][2],R2[1][0],R2[1][1],R2[1][2],R2[2][0],R2[2][1],R2[2][2]);
	    }*/
	
	
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
// --------------------------------------------------------------------------
int PXAbaqusSolid::makeIncompressibleLL()
{
  if (!hasfemdata()  || !haselements() || !hasnodes())
      return -1;

  float E[3][3];

  fprintf(stderr,"Adjusting LL for incompressibility\n");

  rotateStrainsFiberToCardiac(0);
  for (int el=0;el<elements->getnumnodes();el++)
      {
	E[0][0] = femData->getstraincomponent(el,0); 
	E[0][1] = femData->getstraincomponent(el,3); 
	E[0][2] = femData->getstraincomponent(el,4); 
	E[1][1] = femData->getstraincomponent(el,1); 
	E[1][2] = femData->getstraincomponent(el,5); 
	E[2][2] = femData->getstraincomponent(el,2); 


	float e22= -(E[0][0]+E[1][1]);

	

	if (int(el/10)*10 == el)
	    fprintf(stderr,"EL=%d RR=%5.2f CC=%5.2f LL=%5.2f RC=%5.2f , RL=%5.2f CL=%5.2f\te22=%5.2f\n",
		    el,E[0][0],E[1][1],E[2][2],E[0][1],E[0][2],E[1][2],e22);
	

	E[2][2]=0.5*e22+0.5*E[2][2];
	
	femData->setElementInfo(el,1,-1.0,
				E[0][0],E[1][1],E[2][2],E[0][1],E[0][2],E[1][2]);
      }

  rotateStrainsFiberToCardiac(1);
  return 1;
}
// --------------------------------------------------------------------------
/*
   Strain Comparison Stuff
*/
// --------------------------------------------------------------------------
void PXAbaqusSolid::generatePoints(PXIntegerList* element,PX3Vector* vec,int& maxp)
{
  PX3Vector nd[8];
    for (int i=0;i<element->getnumnodes();i++)
      {
	int index=element->getcurrent();
	float x=getnodes()->getpoint(index)->x[0];
	float y=getnodes()->getpoint(index)->x[1];
	float z=getnodes()->getpoint(index)->x[2];
	nd[i].setvector(x,y,z);
	element->Next();
      }
  
  switch (element->getnumnodes())
      {
      case 4:
	if (maxp>=5)
	    {
	      vec[4].setvector(0.0,0.0,0.0);
	      for (int i=0;i<=3;i++)
		  {
		    vec[i].copy(&nd[i]);
		    vec[4].add(&nd[i],0.25);
		  }
	      maxp=5;
	    }
	else
	    maxp=0;
	break;
      case 6:
	if (maxp>=30)
	    {
	      for (int ii=0;ii<30;ii++)
		  vec[ii].setvector(0.0,0.0,0.0);
	      for (int i=0;i<=1;i++)
		  {
		    vec[i*6+0].copy(&nd[i*3+0]);
		    vec[i*6+1].add(&nd[i*3+0],0.5);  vec[i*6+1].add(&nd[i*3+1],0.5);
		    vec[i*6+2].copy(&nd[i*3+1]);
		    vec[i*6+3].add(&nd[i*3+0],0.5);  vec[i*6+3].add(&nd[i*3+2],0.5);
		    vec[i*6+4].add(&nd[i*3+2],0.5);  vec[i*6+1].add(&nd[i*3+1],0.5);
		    vec[i*6+5].copy(&nd[i*3+2]);
		  }
	      for (int j=1;j<=3;j++)
		  {
		    float f2=float(j)*0.25;
		    for (int i=0;i<=5;i++)
			{
			  int index=i+(j+1)*6;
			  vec[index].add(&vec[i],1.0-f2);
			  vec[index].add(&vec[i+6],f2);
			}
		  }
	      maxp=24;
	    }
	else
	    maxp=0;
	break;
      case 8:
	if (maxp>=65)
	    {
	      for (int ii=0;ii<65;ii++)
		  vec[ii].setvector(0.0,0.0,0.0);
	      
	      for (int i=0;i<=1;i++)
		  {
		    int offset=i*13;
		    vec[offset+0].copy(&nd[i*4+0]);
		    vec[offset+2].copy(&nd[i*4+1]);
		    vec[offset+8].copy(&nd[i*4+2]);
		    vec[offset+6].copy(&nd[i*4+3]);
		    
		    vec[offset+1].add(&vec[offset+0],0.5); vec[offset+1].add(&vec[offset+2],0.5);
		    
		    vec[offset+3].add(&vec[offset+0],0.5); vec[offset+3].add(&vec[offset+6],0.5);
		    vec[offset+5].add(&vec[offset+2],0.5); vec[offset+5].add(&vec[offset+8],0.5);
		    vec[offset+7].add(&vec[offset+6],0.5); vec[offset+7].add(&vec[offset+8],0.5);
		    
		    vec[offset+4].add(&vec[offset+1],0.25); vec[offset+4].add(&vec[offset+3],0.25);
		    vec[offset+4].add(&vec[offset+5],0.25); vec[offset+4].add(&vec[offset+7],0.25);
		    
		    vec[offset+9].add(&vec[offset+0],0.25); vec[offset+9].add(&vec[offset+1],0.25);
		    vec[offset+9].add(&vec[offset+3],0.25); vec[offset+9].add(&vec[offset+4],0.25);

		    vec[offset+10].add(&vec[offset+1],0.25); vec[offset+10].add(&vec[offset+2],0.25);
		    vec[offset+10].add(&vec[offset+4],0.25); vec[offset+10].add(&vec[offset+5],0.25);
		    
		    vec[offset+11].add(&vec[offset+6],0.25); vec[offset+11].add(&vec[offset+7],0.25);
		    vec[offset+11].add(&vec[offset+4],0.25); vec[offset+11].add(&vec[offset+3],0.25);

		    vec[offset+12].add(&vec[offset+7],0.25); vec[offset+12].add(&vec[offset+8],0.25);
		    vec[offset+12].add(&vec[offset+4],0.25); vec[offset+12].add(&vec[offset+5],0.25);


		  }
	      
	      for (int j=1;j<=3;j++)
		  {
		    float f2=0.25*float(j);
		    for (int i=0;i<=12;i++)
			{
			  int index=i+(j+1)*13;
			  vec[index].add(&vec[i],1.0-f2);
			  vec[index].add(&vec[i+13],f2);
			}
		  }
	      maxp=65;
	    }
	else
	    maxp=0;
	break;
      }
}
// --------------------------------------------------------------------------
void PXAbaqusSolid:: getElementZThetaRange(PXIntegerList* element,
					   float cx,float cy,
					   float& minz,float& maxz,
					   float& minangle,float& maxangle)
{
  minangle=10000.0,maxangle=-100000.0;
  minz=100000.0,maxz=-100000.0;

  element->Rewind();
  for (int i=0;i<element->getnumnodes();i++)
      {
	int index=element->getcurrent();
	element->Next();
	float x=getnodes()->getpoint(index)->x[0]-cx;
	float y=getnodes()->getpoint(index)->x[1]-cy;
	float z=getnodes()->getpoint(index)->x[2];
	minz=Fmin(minz,z);
	maxz=Fmax(maxz,z);
	float phase=Fcyclic(atan2(y,x)*180/M_PI,360.0);
	minangle=Fmin(phase,minangle);
	maxangle=Fmax(phase,maxangle);
	fprintf(stderr,"Node %d (%5.2f ,%5.2f) Phase=%6.3f\n",i,x,y,phase);
      }

  minangle=Fcyclic(minangle,360.0);
  maxangle=Fcyclic(maxangle,360.0);


  /*if (maxangle-minangle>180.0)
      {
	float temp=maxangle;
	maxangle=minangle;
	minangle=temp;
      } */
}
// --------------------------------------------------------------------------  
int PXAbaqusSolid::compareWithOtherSolid(PXAbaqusSolid* sld,const char* fname,float mindist)
{
  fprintf(stderr,"++++++++++++++ Compare Strains and Save in %s \n",fname);

  if (!sld->hasfemdata() || !hasfemdata())
      {
	fprintf(stderr,"No strains in one or other solid\n");
	return 0;
      }



  /*if (sld->getorientationmode() !=getorientationmode())
      {
	fprintf(stderr,"Orientations are different\n");
	//return 0;
      }*/

  sld->getfemdata()->converttoprincipal();
  getfemdata()->converttoprincipal();

  setorientationmode(5);
  sld->setorientationmode(5);

  FILE* fout=fopen(fname,"w");
  if (fout==NULL)
      return 0;

  fprintf(fout,"# Comparison data \n");
  int orient=sld->getorientationmode();
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
    {
      for (int i=0;i<getnodes()->getnumnodes();i++)
	  {
	    float z=getnodes()->getpoint(i)->x[2];
	    globalminz=Fmin(globalminz,z);
	    globalmaxz=Fmax(globalmaxz,z);
	  }
    }

    for (int i=0;i<getelements()->getnumnodes();i++)
	{
	  cx+=(getfemdata()->getcentroid(i))->x[0]*
	      getfemdata()->getvolume(i);
	  cy+=(getfemdata()->getcentroid(i))->x[1]*
	      getfemdata()->getvolume(i);
	  tvol+=(getfemdata())->getvolume(i);
	}
    cx/=tvol; cy/=tvol;
    fprintf(fout,"# Total volume= %9.3f\n",tvol);
  }

  fprintf(fout,"# Centroid (cx=%6.2f,cy=%6.2f)\n",cx,cy);
  
  // For all elements stuff
  // ----------------------
  /*float* elementZ=new float[getelements()->getnumnodes()];
  float* elementA=new float[getelements()->getnumnodes()];
  getelements()->Rewind();
  for (int el=0;el<getelements()->getnumnodes();el++)
      {
	float x,y,z;
	getElementCentroid(getelements()->getcurrentnode()->points,x,y,z);
	elementA[el]=Fcyclic(atan2(y-cy,x-cx)*180/M_PI,360.0);
	elementZ[el]=z;
	getelements()->Next();
      }*/
  
  int nummarkerpies=sld->getelements()->getnumnodes(); 
  float *ave=new float[nummarkerpies];
  float *vol=new float[nummarkerpies];

  //int    numel=getelements()->getnumnodes();

  int start=-1;
  int maxstrain=5;
  if (orient==5)
      maxstrain=2;
  getfemdata()->setPieStats(2,2,1);

  PX3Vector* vec=new PX3Vector[65];

  for (int strain=start;strain<=maxstrain;strain++)
      {
	sld->getelements()->Rewind();
	for (int pie=0;pie<sld->getelements()->getnumnodes();pie++)
	    {
	      // Get Range for Marker Element and Set Ave, vol=0 
	      PXIntegerList* markerElement=sld->getelements()->getcurrentnode()->points;
	      sld->getelements()->Next();
	      /*float zmin,zmax,pmin,pmax;
	      sld->getElementZThetaRange(markerElement,cx,cy,zmin,zmax,pmin,pmax);*/
	      
	      int maxp=65;
	      sld->generatePoints(markerElement,vec,maxp);

	      ave[pie]=0.0; 	
	      vol[pie]=0.0;
	      //fprintf(stderr,"Processing pie %d/%d\n",pie,sld->getelements()->getnumnodes());
	      /*fprintf(stderr,"\t Zrange %5.2f:%5.2f Arange %6.2f:%6.2f\n",
		      zmin,zmax,pmin,pmax);*/

	      elements->Rewind();
	      
	      for (int el=0;el<getelements()->getnumnodes();el++)
		  {
		    float x,y,z;
		    getElementCentroid(getelements()->getcurrentnode()->points,x,y,z);
		    float elvol=(getfemdata())->getvolume(el);
		    
		    PX3Vector cent;
		    cent.setvector(x,y,z);
		    int i=0;
		    int found=0;
		    while (i<maxp && found==0)
			{
			  float dist=cent.getdistance(&vec[i]);
			  if (dist<=mindist)
			      found=1;
			  i++;
			}
		    if (found==1)
			{
			  if (strain>=0)
			      ave[pie]+=getfemdata()->getstraincomponent(el,strain)*elvol;
			  vol[pie]+=elvol;
			  getfemdata()->setElementPieIndex(el,1);
			}
		    else
			getfemdata()->setElementPieIndex(el,0);
		    elements->Next();
	    
		  }
	      if (strain==0)
		  {
		    char line[400];
		    sprintf(line,"Pie%d.pieindex",pie+1);
		    getfemdata()->savePieDivision(line);
		    PXMarkerCollection* markers=new PXMarkerCollection(maxp,1);
		    sprintf(line,"Pie%d.mark",pie+1);
		    for (int m=0;m<maxp;m++)
			{
			  markers->getmarker(m)->getclickedpoints(0)->addpoint(vec[m].x[0],
									       vec[m].x[1],
									       vec[m].x[2],0.0);
			  markers->getmarker(m)->setposition(0,vec[m].x[0],vec[m].x[1],vec[m].x[2]);
			}
		    markers->savemarkercollection(line);
		    delete markers;
		  }
	    }
	if (strain>=0)
	    {
	      for (int i=0;i<nummarkerpies;i++)
		  {
		    if (vol[i]<0.001)
			ave[i]=0.0;
		    else
			ave[i]/=vol[i];
		  }
	    }
	if (orient<2 || orient==3 || strain<0)
	    {
	      switch(strain)
		  {
		  case -1:
		    fprintf(fout,"#\n\t Volume \n\n"); break;
		  case 0:
		    fprintf(fout,"#\n\t Radial Strain RR\n\n"); break;
		  case 1:
		    fprintf(fout,"#\n\t Circumferential Strain CC\n\n"); break;
		  case 2:
		    fprintf(fout,"#\n\t Longitudinal Strain LL\n\n"); break;
		  case 3:
		    fprintf(fout,"#\n\t Radial-Circumferential Shear Strain RC\n\n"); break;
		  case 4:
		    fprintf(fout,"#\n\t Radial-Longitudinal Strain RL\n\n"); break;
		  case 5:
		    fprintf(fout,"#\n\t Circumferential-Longitudinal Strain CL\n\n"); break;
		  }
	    }
	else if (orient==5)
	    {
	      fprintf(fout,"#\n\t Principal Strain %d\n\n",strain+1);
	    }
	else
	    {
	      switch(strain)
		  {
		  case 0:
		    fprintf(fout,"#\n\t Circumferential Cross-Fibre Strain XX\n\n"); break;
		  case 1:
		    fprintf(fout,"#\n\t Radial Strain RR\n\n"); break;
		  case 2:
		    fprintf(fout,"#\n\t Fibre Strain FF\n\n"); break;
		  case 3:
		    fprintf(fout,"#\n\t Circumferential-Radial Shear Strain XR\n\n"); break;
		  case 4:
		    fprintf(fout,"#\n\t Circumferential-Fibre Strain XF\n\n"); break;
		  case 5:
		    fprintf(fout,"#\n\t Radial-Fibre Strain RF\n\n"); break;
		  }
	    }
	
	fprintf(fout,"Pie\tSolid\tMarker \n");
	for (int j=0;j<nummarkerpies;j++)
	    {
	      fprintf(fout,"%2d\t",j+1);
	      if (strain==-1)
		  fprintf(fout,"%6.1f\t %6.1f ",vol[j],sld->getfemdata()->getvolume(j));
	      else
		  fprintf(fout,"%6.3f\t %6.3f ",ave[j],sld->getfemdata()->getstraincomponent(j,strain));
	      fprintf(fout,"\n");
	    }

      }
  fclose(fout);

  delete [] ave;
  delete [] vol;
  delete [] vec;
  /*delete [] elementZ;
  delete [] elementA;*/
  return 1;
}

