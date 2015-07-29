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

#include "pxcrystalabaqus.h"
#include "pxabaqussolid.h"
#include "pxmarkerclass.h"
#include "pxutil.h"


// --------------------------------------------------------------------------
int  PXCrystalAbaqus::debugmode=0;
// --------------------------------------------------------------------------
//    Access Functions 
// ------------------------------------------------------------------------- 

int      PXCrystalAbaqus::getnumframes()        { return numframes;}
int      PXCrystalAbaqus::getnumgroups()        { return numgroups;}

int      PXCrystalAbaqus::hasfemdata()          { return hasFemData;}
int      PXCrystalAbaqus::hasdisplacements()    { return hasDisplacements;}
int      PXCrystalAbaqus::hasnodes()            { return hasNodes;}
int      PXCrystalAbaqus::hasextranodes()            { return hasExtraNodes;}
int      PXCrystalAbaqus::haselements()         { return hasElements;}

PXNeighboursList*     PXCrystalAbaqus::getelements()       { return elements;}
PXSolidNodes*         PXCrystalAbaqus::getnodes()          { return nodes;}
PXSolidNodes*         PXCrystalAbaqus::getextranodes()          { return extraNodes;}

PXNodalDisplacements* PXCrystalAbaqus::getdisplacements(int i)  
{ return displacements[Irange(i,0,numframes-1)];}
PXAbaqusOutputData*   PXCrystalAbaqus::getfemdata()        { return femData;}

// ------------------------------------------------------------------------- 

PXCrystalAbaqus::PXCrystalAbaqus()
{
  init();
}

PXCrystalAbaqus::PXCrystalAbaqus(PXCrystalAbaqus* old)
{
  init();
  Copy(old);
}
// --------------------------------------------------------------------------
PXCrystalAbaqus::PXCrystalAbaqus(PXMarkerCollection* markers)
{
  init();
  numframes=markers->getmarker(0)->getnumframes();
  nodes=new PXSolidNodes(markers->getnummarkers());
  int i;
  for (i=0;i<markers->getnummarkers();i++)
      {
	nodes->setindex(i,0,i);
	nodes->setindex(i,1,0);
	for (int ia=0;ia<=2;ia++)
	    nodes->getpoint(i)->x[ia]=markers->getmarker(i)->getcomponent(0,ia);
      }
  hasNodes=1;
  for (i=0;i<numframes;i++)
      displacements[i]=new PXNodalDisplacements(nodes,0,100);
  hasDisplacements=1;

  for (int j=0;j<nodes->getnumnodes();j++)
      for (int i=0;i<numframes;i++)
	  for (int ia=0;ia<=2;ia++)
	      displacements[i]->setdispcomponent(j,ia,
						 markers->getmarker(j)->getcomponent(i,ia)-
						 markers->getmarker(j)->getcomponent(0,ia));
}
// --------------------------------------------------------------------------
PXCrystalAbaqus::PXCrystalAbaqus(PXMarkerCollection* markers,int frameinc,int maxframes)
{
  init();
  int actualframes=markers->getmarker(0)->getnumframes();
  
  fprintf(stderr,"Numframes=%d actualframes=%d\n",numframes,actualframes);

  if (actualframes<2)
      return;

  if (maxframes==-1)
      maxframes=actualframes;
  else
      maxframes=Irange(maxframes,2,actualframes);
  
  fprintf(stderr,"Maxframes=%d actualframes=%d\n",maxframes,actualframes);
  frameinc=Irange(frameinc,1,actualframes);

  if (frameinc>1)
      {
	numframes=0;
	for (int j=0;j<maxframes;j+=frameinc)
	    ++numframes;
      }
  else
      numframes=actualframes;
  
  nodes=new PXSolidNodes(markers->getnummarkers());
  int i;
  for (i=0;i<markers->getnummarkers();i++)
      {
	nodes->setindex(i,0,i);
	nodes->setindex(i,1,0);
	for (int ia=0;ia<=2;ia++)
	    nodes->getpoint(i)->x[ia]=markers->getmarker(i)->getcomponent(0,ia);
      }
  hasNodes=1;
  for (i=0;i<numframes;i++)
      displacements[i]=new PXNodalDisplacements(nodes,0,100);
  hasDisplacements=1;

  int fr=0;
  fprintf(stderr,"Numframes=%d actualframes=%d\n",numframes,actualframes);
  for (i=0;i<maxframes;i+=frameinc)
      {
	fprintf(stderr,"Actual frame %d, Analysis frame %d\n",i,fr);
	for (int j=0;j<nodes->getnumnodes();j++)
	    {
	      for (int ia=0;ia<=2;ia++)
		  {
		    if (fabs(markers->getmarker(j)->getcomponent(i,ia))>0.01)
			displacements[fr]->setdispcomponent(j,ia,
							    markers->getmarker(j)->getcomponent(i,ia)-
							    markers->getmarker(j)->getcomponent(0,ia));
		    else
			displacements[fr]->setdispcomponent(j,ia,0.0);
		    if (j==0)
			fprintf(stderr,"Mark=%d Frame=%d,Comp=%d %f (%f-%f)\n",
				j,i,ia,displacements[fr]->getdispcomponent(j,ia),
				markers->getmarker(j)->getcomponent(i,ia),
				markers->getmarker(j)->getcomponent(0,ia));
		  }
	    }
	fr++;
      }
  numframes=fr;
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::Copy(PXCrystalAbaqus* other)
{
  cleanup();
  init();

  numframes=other->getnumframes();

  if (other->haselements())
      {
	elements=new PXNeighboursList();
	elements->Copy(other->getelements());
	hasElements=1;
      }
  
  if (other->hasnodes())
      {
	nodes=new PXSolidNodes(other->getnodes());
	hasNodes=1;
      }

  if (other->hasextranodes())
      {
	extraNodes=new PXSolidNodes(other->getextranodes());
	hasExtraNodes=1;
      }

  if (other->hasfemdata())
      {
	femData=new PXAbaqusOutputData(other->getfemdata());
	hasFemData=1;
      }

  if (other->hasdisplacements())
      {
	for (int i=0;i<numframes;i++)
	    displacements[i]=new PXNodalDisplacements(other->getdisplacements(i));
      }
}
// --------------------------------------------------------------------------
PXCrystalAbaqus::PXCrystalAbaqus(const char* fname1,const char* fname2)
{
  init();
  createFromCrystalFiles(fname1,fname2);
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::init()
{
  numframes=0;    numgroups=0;  
  hasElements=0;  hasNodes=0;   hasDisplacements=0; hasFemData=0; hasExtraNodes=0;
}
// --------------------------------------------------------------------------
PXCrystalAbaqus::~PXCrystalAbaqus()
{
  cleanup();
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::cleanup()
{
  cleanupElementSpace();
  cleanupNodeSpace();
  
  if (hasfemdata())
      cleanupFemData();

  if (hasdisplacements())
      cleanupDisplacements();

  if (hasextranodes())
      {
	delete extraNodes;
	hasExtraNodes=0;
      }
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::cleanupDisplacements()
{
  if (hasdisplacements())
      {
	for (int i=0;i<numframes;i++)
	    delete displacements[i];
	hasDisplacements=0;
      }
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::allocateDisplacements()
{
  if (hasdisplacements())
      cleanupDisplacements();
  if (numframes>0 && hasnodes())
      {
	for (int i=0;i<numframes;i++)
	    displacements[i]=new PXNodalDisplacements(nodes,0,100);
	hasDisplacements=1;
      }
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::cleanupFemData()
{
  if (hasfemdata())
      delete femData;
  hasFemData=0;
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::cleanupElementSpace()
{
  if (haselements())
      delete elements;
  hasElements=0;
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::cleanupNodeSpace()
{
  if (hasnodes())
      delete nodes;

  hasNodes=0;

}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::allocateFemData()
{
  if (hasfemdata())
      cleanupFemData();
  
  if (!haselements() || !hasnodes())
      return;
  femData=new PXAbaqusOutputData(nodes->getnumnodes(),elements->getnumnodes());
  hasFemData=1;
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::allocateNodeSpace(int numnode)
{
  if (hasnodes())
      cleanupNodeSpace();
  
  nodes=new PXSolidNodes(numnode);
  hasNodes=1;
  
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::allocateElementSpace()
{
  if (haselements())
      cleanupElementSpace();

  elements=new PXNeighboursList();
  hasElements=1;
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::setorientations(int group,PX3Vector& radial,PX3Vector& circum,PX3Vector& longit)
{
  // Only do real groups
  int debug=1;

  group=Irange(group,2,numgroups-1);
  if (debug) fprintf(stderr,"---------Orientation group %d----------------------------\n\n",group);  
  // First the radial direction
  
  elements->Rewind();

  // Max of 10 lines 0--9
  int* point1=new int[10];
  int* point2=new int[10];

  int i;
  for (i=0;i<10;i++)
      {
	point1[i]=-1;
	point2[i]=-1;
      }

  for (i=0;i<nodes->getnumnodes();i++)
      {
	if (nodes->getindex(i,0)==group)
	    {
	      int index2=nodes->getindex(i,1);
	      if (index2<10)
		    point1[index2]=i;
	      else if (index2>=100)
		  point2[index2-100]=i;
	    }
      }
  
  radial.setvector(0.0,0.0,0.0);
  int flag=1;
  if (debug) fprintf(stderr,"Cube = %d \n Radial Direction\n",group);
  for (i=0;i<10;i++)
      {
	if (point1[i]>=0 && point2[i]>=0)
	    {
	      if (debug) fprintf(stderr,"i=%d endo =%d epi=%d\n",i,point1[i]+1,point2[i]+1);
	      radial.setvector(radial.x[0]+nodes->getpoint(point1[i])->x[0]-nodes->getpoint(point2[i])->x[0],
			       radial.x[1]+nodes->getpoint(point1[i])->x[1]-nodes->getpoint(point2[i])->x[1],
			       radial.x[2]+nodes->getpoint(point1[i])->x[2]-nodes->getpoint(point2[i])->x[2]);
	      flag++;
	    }
      }
  if (flag>0)
      radial.normalize();
  else
      radial.setvector(1.0,0.0,0.0);

  delete [] point1;  delete [] point2;


  // Longitudinal Next 
  // -----------------
  longit.setvector(0.0,0.0,1.0);
  // Find second element internal reference
  if (debug) fprintf(stderr,"Cube = %d \n Long Direction\n",group);
  elements->Rewind(); 
  PXIntegerList* intList=new PXIntegerList;
  for (int e=0;e<elements->getnumnodes();e++)
      {
	if (elements->getcurrentnode()->index==1)
	    {
	      PXIntegerList* intList2=((elements->getcurrentnode())->points);
	      intList2->Rewind();
	      for (int ia=0;ia<intList2->getnumnodes();ia++)
		  {
		    intList->AddNonDuplicate(intList2->getcurrent(),-1);
		    intList2->Next();
		  }
	    }
	elements->Next();
      }
  int* pt=new int[intList->getnumnodes()];
  intList->Rewind();
  for (i=0;i<intList->getnumnodes();i++)
      {
	pt[i]=intList->getcurrent();
	intList->Next();
      }

  if (intList->getnumnodes()==2)
      {
	longit.setvector(nodes->getpoint(pt[1])->x[0]-nodes->getpoint(pt[0])->x[0],
			 nodes->getpoint(pt[1])->x[1]-nodes->getpoint(pt[0])->x[1],
			 nodes->getpoint(pt[1])->x[2]-nodes->getpoint(pt[0])->x[2]);
      }
  else if (intList->getnumnodes()==3)
      {
	if (debug) fprintf(stderr,"Using nodes pt[1]=%d, pt[2]=%d and apex(pt[0])=%d\n",pt[1]+1,pt[2]+1,pt[0]+1);
	longit.setvector(0.5*(
			      nodes->getpoint(pt[1])->x[0]+nodes->getpoint(pt[2])->x[0])
			 -nodes->getpoint(pt[0])->x[0],
			 0.5*(
			      nodes->getpoint(pt[1])->x[1]+nodes->getpoint(pt[2])->x[1])
			 -nodes->getpoint(pt[0])->x[1],
			 0.5*(nodes->getpoint(pt[1])->x[2]+nodes->getpoint(pt[2])->x[2])
			 -nodes->getpoint(pt[0])->x[2]);
      }

  delete [] pt;
  delete intList;

  // Orthogonalization Step
  longit.normalize();
  
  /* This assumes radial correct
  float dotp=longit.dotwith(&radial);
  longit.subtract(&radial,dotp);  
  longit.normalize();*/

  // This assumes long correct
  float dotp=longit.dotwith(&radial);
  radial.subtract(&longit,dotp);  
  radial.normalize();


  /* Iterate over both 
  for (int k=0;k<=9;k++)
      {
	float dotp=longit.dotwith(&radial);
	longit.subtract(&radial,dotp*0.2);  
	longit.normalize();
	float dotp2=longit.dotwith(&radial);
	radial.subtract(&longit,dotp*0.2);
	radial.normalize();
	fprintf(stderr,"dotp=%6.2f dotp2=%6.2f\n",dotp,dotp2);
      }
  float dotp=longit.dotwith(&radial);
  longit.subtract(&radial,dotp*0.5);  
  longit.normalize();*/
  float dotp2=longit.dotwith(&radial);
  fprintf(stderr,"dotp=%6.2f dotp2=%6.2f\n",dotp,dotp2);
	
  // Circumferential Vector
  circum.copy(&radial);  
  circum.crosswithoverwrite(&longit); 
  circum.normalize();

  if (debug) fprintf(stderr,"-----------------------%d----------------------------------\n\n",group);  
}
// --------------------------------------------------------------------------
int PXCrystalAbaqus::saveAbaqus(const char* fname,int resout,int usenumframes,int skipframes)
{
  if (!hasnodes() ||  !hasdisplacements() || !haselements())
      {
	fprintf(stderr,"No nodes,elements or displacements\n");
	return 0;
      }

  if (usenumframes==-1)
      usenumframes=numframes;
  skipframes=Irange(skipframes,1,usenumframes);

  char* abname=anacreatefilename(fname,".inp",".crt");
  fprintf(stderr,"%s --> %s (%d skip %d)\n",fname,abname,usenumframes,skipframes);

  FILE* fout=fopen(abname,"w");

  if (!fout)
      {
	delete [] abname;
	return 0;
      }

  fprintf(fout,"*HEADING\nMyocardium Model\n");
  fprintf(fout,"** Numsteps=%d numnodes=%d, numelements=%d\n",usenumframes,nodes->getnumnodes(),
	  elements->getnumnodes());
  fprintf(fout,"** ----------------------------------------------------\n");
  fprintf(fout,"** Shrink .dat file\n*PREPRINT,ECHO=NO,MODEL=NO,HISTORY=NO\n**\n");

  fprintf(fout,"** ----------Material Model --------------------------------------\n");
  fprintf(fout,"*MATERIAL,NAME=TISSUE\n*ELASTIC\n");
  fprintf(fout,"%7.3f,%7.3f\n",1.0,0.3);
  
  fprintf(fout,"** ---------- Nodes  --------------------------------------\n");
  
  fprintf(fout,"*NODE,NSET=ALLNODES\n");
  
  for (int i=0;i<nodes->getnumnodes();i++)
      {
	if (nodes->getindex(i,0)>=0)
	    {
	      fprintf(fout," %5d , %7.3f , %7.3f , %7.3f \n",
		      i+1,
		      nodes->getpoint(i)->x[0],
		      nodes->getpoint(i)->x[1],
		      nodes->getpoint(i)->x[2]);
	    }
      }

  if (hasextranodes())
      {
	for (int i=0;i<extraNodes->getnumnodes();i++)
	    {
	      if (extraNodes->getindex(i,0)>=0)
		  {
		    fprintf(fout," %5d , %7.3f , %7.3f , %7.3f \n",
			    i+1+nodes->getnumnodes(),
			    extraNodes->getpoint(i)->x[0],
			    extraNodes->getpoint(i)->x[1],
			    extraNodes->getpoint(i)->x[2]);
		  }
	      
	    }
      }
  fprintf(fout,"**\n*NSET, NSET=N_DUMMY\n1\n");
  
  fprintf(fout,"** ---------- Elements --------------------------------------\n");  
  // This is trickier but
  // 2 nodes --- Truss       T3D2
  // 3 nodes --  3xT3D2
  // 4 nodes --  Tetrahedron C3D4
  // 6 nodes --  Prism       C3D6
  // 8 nodes --  Hexahedron  C3D8

  char line[100],orient[20];
  int elno=1;

  int numgood[10];

  for (int gr=0;gr<numgroups;gr++)
      {
	if (gr>1)
	    {
	      PX3Vector rad,circ,longit;
	      setorientations(gr,rad,circ,longit);
	      sprintf(orient,"ORIENT%d",gr+1);
	      fprintf(fout,"**\n*ORIENTATION,NAME=%s,SYSTEM=RECTANGULAR\n",orient);
	      fprintf(fout,"%8.5f,%8.5f,%8.5f, %8.5f,%8.5f,%8.5f\n",
		      rad.x[0],rad.x[1],rad.x[2],
		      circ.x[0],circ.x[1],circ.x[2]);
	    }
	int numn;
	for (numn=2;numn<8;numn++)
	    numgood[numn]=0;

	for (numn=2;numn<8;numn++)
	    {
	      if (numn==5)
		  numn++;
	      
	      elements->Rewind();
	      for (int el=0;el<elements->getnumnodes();el++)
		  {
		    PXIntegerList* intList=((elements->getcurrentnode())->points);
		    if (intList->getnumnodes()==numn && elements->getcurrentnode()->index==gr)
			{
			  /*fprintf(stderr,"Element = %d ,  numnodes=%d , index=%d:group=%d\n",
				  el, intList->getnumnodes(), elements->getcurrentnode()->index,gr);*/
			  numgood[numn]++;
			}
		    elements->Next();
		  }
	      
	      if (numgood[numn]>0)
		  {
		    sprintf(line,"E_E%02d%02d",numn,gr);
		    switch (numn)
			{
			case 2:
			  fprintf(fout,"*ELEMENT, TYPE=T3D2, ELSET=%s\n",line);
			  break;
			case 3:
			  fprintf(fout,"*ELEMENT, TYPE=T3D2, ELSET=%s\n",line);
			  break;
			case 4:
			  fprintf(fout,"*ELEMENT, TYPE=C3D4, ELSET=%s\n",line);
			  break;
			case 6:
			  fprintf(fout,"*ELEMENT, TYPE=C3D6, ELSET=%s\n",line);
			  break;
			case 8:
			  fprintf(fout,"*ELEMENT, TYPE=C3D8, ELSET=%s\n",line);
			  break;
			}
		    
		    elements->Rewind();
		    for(int el=0;el<elements->getnumnodes();el++)
			{
			  PXIntegerList* intList=((elements->getcurrentnode())->points);
			  if (intList->getnumnodes()==numn && elements->getcurrentnode()->index==gr)
			      {
				if (numn!=3)
				    {
				      fprintf(fout,"%d,",elno);
				      PXIntegerList* intList=((elements->getcurrentnode())->points);
				      for (int i=0;i<intList->getnumnodes();i++)
					  {
					    fprintf(fout,"%d",intList->getcurrent()+1);
					    if (i!=intList->getnumnodes()-1)
						fprintf(fout,",");
					    else
						fprintf(fout,"\n");
					    intList->Next();
					  }
				      elno++;
				    }
				else
				    {
				      int pp[3];
				      intList->Rewind();
				      for (int ia=0;ia<=2;ia++)
					  {
					    pp[ia]=intList->getcurrent()+1;
					    intList->Next();
					  }
				      fprintf(fout,"%d,%d,%d\n",elno,pp[0],pp[1]);
				      elno++;
				      fprintf(fout,"%d,%d,%d\n",elno,pp[1],pp[2]);
				      elno++;
					    fprintf(fout,"%d,%d,%d\n",elno,pp[0],pp[2]);
				      elno++;
				    }
			      }
			  elements->Next();
			}
		    if (gr<2)
			{
			  fprintf(fout,"*SOLID SECTION, ELSET=%s,MATERIAL=TISSUE\n**\n** End of group %d \n**\n",
				  line,gr+1);
			}
		    else
			{
			  fprintf(fout,"*SOLID SECTION, ELSET=%s,MATERIAL=TISSUE,ORIENTATION=%s\n",
				  line,orient);
			  fprintf(fout,"**\n** End of group %d \n**\n",gr+1);
			}
		  }
	    }
      }
  fprintf(fout,"**\n");
  fprintf(fout,"*ELSET, ELSET=E_DUMMY\n1\n**\n");

  for (int fr=0;fr<usenumframes;fr+=skipframes)
      {
	fprintf(fout,"** -------------------- Frame %d ---------------------------\n**\n",fr+1);
	fprintf(fout,"*STEP,NLGEOM,AMPLITUDE=RAMP,INC=40\n");
	fprintf(fout,"*STATIC\n %f,1.0,0.00005,1.0\n",1.0);
	fprintf(fout,"**\n");
	fprintf(fout,"** Accuracy Controls\n*CONTROLS,PARAMETERS=FIELD,FIELD=DISPLACEMENT\n");
	fprintf(fout,"0.01,1.0,10.0,  ,  , 1.E-3\n");
	fprintf(fout,"*BOUNDARY,OP=NEW\n");
	for (int i=0;i<displacements[fr]->getsolidnodes()->getnumnodes();i++)
	    {
	      if (displacements[fr]->getsolidnodes()->getindex(i,0)>=0)
		  {
		    for (int j=0;j<3;j++)
			fprintf(fout,"%d , %d , %d, %8.4f\n",i+1,j+1,j+1,
				displacements[fr]->getdispcomponent(i,j));
		  }
	    }
	fprintf(fout,"**\n*EL PRINT,POSITION=CENTROIDAL,ELSET=E_DUMMY\nE11\n");
	fprintf(fout,"*NODE PRINT,NSET=N_DUMMY\nU1\n");
	fprintf(fout,"*EL FILE,POSITION=CENTROIDAL\nE\n");
	fprintf(fout,"*EL FILE\nEVOL\n*NODE FILE,NSET=ALLNODES\nU\n");
	if (resout>0)
	    fprintf(fout,"*RESTART,WRITE\n");
	fprintf(fout,"*END STEP\n");
      }
  fprintf(stderr,"** Resout=%d\n",resout);
  fclose(fout);
  return 1;
}
// -------------------------------------------------------------------------
void PXCrystalAbaqus::getElementCentroid(PXIntegerList* element,float&x,float&y,float& z)
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
// --------------------------------------------------------------------------
int PXCrystalAbaqus::saveInventor(const char* fname,int mode,int cmode,float* params)
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
int PXCrystalAbaqus::saveInventor(FILE* fout,int mode,int cmode,float* params)
{
  if (hasfemdata())
      cmode=Irange(cmode,0,11);
  else 
      cmode=-1;
  if (!haselements() || !hasnodes())
      return 0;

  fprintf(fout, "#Inventor V2.0 ascii\n\n");

  int index[8],i=0,j=0;
  PXTriangle triangle[12];
  int numt=12;

  int nel=elements->getnumnodes();
  int bel=0;

  if (hasfemdata())
      {
	for(i=0; i<nodes->getnumnodes(); i++)
	    for (j=0;j<=2;j++)
		nodes->getpoint(i)->x[j]+=(femData->getdisplacement(i))->x[j]; 
	
	if (hasextranodes())
	    {
	      for (i=0;i<extraNodes->getnumnodes();i++)
		  for (j=0;j<=2;j++)
		      extraNodes->getpoint(i)->x[j]+=(femData->getdisplacement(i+nodes->getnumnodes()))->x[j]; 
	    }
      }


  fprintf(fout, "Coordinate3 { point [ ");
  int a=1;
  if (hasextranodes())
      a=0;
  for(i=0; i<nodes->getnumnodes()-a; i++)
      fprintf(fout, "\n\t\t %4.2f %4.2f %4.2f,", 
	      nodes->getpoint(i)->x[0], nodes->getpoint(i)->x[1], nodes->getpoint(i)->x[2]);		
  if (!hasextranodes())
      {
	i = nodes->getnumnodes()-1;
	fprintf(fout, "\n\t\t %4.2f %4.2f %4.2f]\n\t}\n", 
		nodes->getpoint(i)->x[0], nodes->getpoint(i)->x[1], nodes->getpoint(i)->x[2]);		  
      }
  else
      {
	for(i=0; i<extraNodes->getnumnodes()-1; i++)
	    fprintf(fout, "\n\t\t %4.2f %4.2f %4.2f,", 
		    extraNodes->getpoint(i)->x[0], 
		    extraNodes->getpoint(i)->x[1], 
		    extraNodes->getpoint(i)->x[2]);		
	i = extraNodes->getnumnodes()-1;
	fprintf(fout, "\n\t\t %4.2f %4.2f %4.2f]\n\t}\n", 
		extraNodes->getpoint(i)->x[0], 
		extraNodes->getpoint(i)->x[1], 
		extraNodes->getpoint(i)->x[2]);		  
      }

  int hasface=0,hasline=0;
  elements->Rewind();
  for (i=0;i<bel;i++)
      elements->Next();
  for (i=bel;i<nel;i++)
      {
	PXIntegerList* intList=((elements->getcurrentnode())->points);
	if (intList->getnumnodes()>3)
	    hasface++;
	else
	    hasline++;
	elements->Next();
      }

  //  fprintf(stderr,"Hasface=%d hasline=%d\n",hasface,hasline);

  if (hasface>0)
      {
	
	fprintf(fout, "Separator {\n");		    
	fprintf(fout, "\tShapeHints { \n");	
	fprintf(fout, "\t  vertexOrdering  COUNTERCLOCKWISE \n");	
	fprintf(fout, "\t  faceType CONVEX \n");	
	fprintf(fout, "\t }\n");	
	if (mode>=0 && cmode>=0)
	    {
	      /* write out the material (color) of each triangular facet */

	      fprintf(fout, "\tMaterial { diffuseColor [ ");		
	      elements->Rewind();
	      for(i=0; i<bel; i++)
		  elements->Next();
	      for(i=bel; i<nel; i++)
		  {
		    PXIntegerList* intList=((elements->getcurrentnode())->points);
		    intList->Rewind();
		    int n=intList->getnumnodes();
		    if (n>3)
			{
			  PX3Vector color;
			  if (mode>0)
			      PXAbaqusSolid::color_lookup(color,femData->getstraincomponent(i,cmode),params,mode);
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
			  switch(n)
			      {
			      case 4:
				numt=4;
			      case 6:
				numt=8;
			      case 8:
				numt=4; /*12*/
				break;
			      }
			  
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
		    elements->Next();
		  }
	      fprintf(fout, "\tMaterialBinding {\n\tvalue PER_FACE\n}\n");
	    }


	/* write out the facet index */ 
	fprintf(fout, "\tIndexedFaceSet {\n\t coordIndex  [ \n");
	elements->Rewind();
	for(i=0; i<bel;i++)
	    elements->Next();
	for(i=bel; i<nel; i++)
	    {
	      PXIntegerList* intList=((elements->getcurrentnode())->points);
	      if (intList->getnumnodes()>3)
		  {
		    intList->Rewind();
		    for (int ia=0;ia<intList->getnumnodes();ia++)
			{
			  index[ia]=intList->getcurrent();
			  intList->Next();
			}
		    switch(intList->getnumnodes())
			{
			case 8:
			  numt=4;
			  triangle[0].p[0]=index[0]; triangle[0].p[1]=index[1]; triangle[0].p[2]=index[2];  
			  triangle[1].p[0]=index[0]; triangle[1].p[1]=index[2]; triangle[1].p[2]=index[3];  
			  {
			    for (int tr=2;tr<=3;tr++)
				for (int p=0;p<=3;p++)
				    triangle[tr].p[p]=triangle[tr-2].p[p]+4;
			  }
			  /*triangle[2].p[0]=index[1]; triangle[2].p[1]=index[5]; triangle[2].p[2]=index[6];  
			    triangle[3].p[0]=index[2]; triangle[3].p[1]=index[6]; triangle[3].p[2]=index[2];  */
			  
			  triangle[4].p[0]=index[4]; triangle[4].p[1]=index[7]; triangle[4].p[2]=index[6];  
			  triangle[5].p[0]=index[5]; triangle[5].p[1]=index[4]; triangle[5].p[2]=index[6];  
			  triangle[6].p[0]=index[0]; triangle[6].p[1]=index[4]; triangle[6].p[2]=index[7];  
			  triangle[7].p[0]=index[3]; triangle[7].p[1]=index[3]; triangle[7].p[2]=index[7];  
			  triangle[8].p[0]=index[3]; triangle[8].p[1]=index[2]; triangle[8].p[2]=index[6];  
			  triangle[9].p[0]=index[7]; triangle[9].p[1]=index[7]; triangle[9].p[2]=index[3];  
			  triangle[10].p[0] = index[4]; triangle[10].p[1] = index[5]; triangle[10].p[2] = index[1];  
			  triangle[11].p[0] = index[0]; triangle[11].p[1] = index[0]; triangle[11].p[2] = index[4];
			  break;
			  
			case 6:
			  numt=8;
			  triangle[0].p[0]=index[0]; triangle[0].p[1]=index[1]; triangle[0].p[2]=index[2];  
			  triangle[1].p[0]=index[0]; triangle[1].p[1]=index[1]; triangle[1].p[2]=index[3];  
			  triangle[2].p[0]=index[1]; triangle[2].p[1]=index[3]; triangle[2].p[2]=index[4];  
			  triangle[3].p[0]=index[1]; triangle[3].p[1]=index[4]; triangle[3].p[2]=index[5];  
			  triangle[4].p[0]=index[1]; triangle[4].p[1]=index[5]; triangle[4].p[2]=index[2];  
			  triangle[5].p[0]=index[0]; triangle[5].p[1]=index[3]; triangle[5].p[2]=index[2];  
			  triangle[6].p[0]=index[5]; triangle[6].p[1]=index[3]; triangle[6].p[2]=index[2];  
			  triangle[7].p[0]=index[3]; triangle[7].p[1]=index[4]; triangle[7].p[2]=index[5];  
			  break;
			  
			case 4:
			  numt=4;
			  triangle[0].p[0]=index[0]; triangle[0].p[1]=index[1]; triangle[0].p[2]=index[2];  
			  triangle[1].p[0]=index[0]; triangle[1].p[1]=index[1]; triangle[1].p[2]=index[3];  
			  triangle[2].p[0]=index[0]; triangle[2].p[1]=index[2]; triangle[2].p[2]=index[3];  
			  triangle[3].p[0]=index[1]; triangle[3].p[1]=index[2]; triangle[3].p[2]=index[3];  
			}
		    
		    for(j=0; j<numt; j++)
			{
			  if (j<(numt-1) || i<nel-1)
			      fprintf(fout, "\t\t%d,%d,%d,%d, \n", 
				      triangle[j].p[0], triangle[j].p[1], triangle[j].p[2],-1);
			  else
			      fprintf(fout,"\t\t%d,%d,%d,%d \n", 
				      triangle[j].p[0], triangle[j].p[1], triangle[j].p[2],-1);
			}
		  }
	      elements->Next();
	    }
	fprintf(fout, "]}\n}\n");
      }

  if (hasline>0)
      {
	fprintf(fout, "Separator {\n");		    
	fprintf(fout, "\tMaterial { diffuseColor [ 1.0 1.0 1.0 ]\n transparency [0.5 ] \n} ");		
	fprintf(fout, "\tIndexedLineSet {\n\t coordIndex  [ \n");
	elements->Rewind();
	for(i=0; i<bel;i++)
	    elements->Next();
	for(i=bel; i<nel; i++)
	    {
	      PXIntegerList* intList=((elements->getcurrentnode())->points);
	      if (intList->getnumnodes()<=3)
		  {
		    intList->Rewind();
		    for (int ia=0;ia<intList->getnumnodes();ia++)
			{
			  index[ia]=intList->getcurrent();
			  intList->Next();
			}
		    
		    switch(intList->getnumnodes())
			{
			case 3:
			  numt=3;
			  triangle[0].p[0]=index[0]; triangle[0].p[1]=index[1];
			  triangle[1].p[0]=index[0]; triangle[1].p[1]=index[2];
			  triangle[2].p[0]=index[2]; triangle[2].p[1]=index[1];
			  break;
			  
			case 2:
			  numt=1;
			  triangle[0].p[0]=index[0]; triangle[0].p[1]=index[1];
			  break;
			}
		    
		    for(j=0; j<numt; j++)
			{
			  if (j<(numt-1) || i<nel-1)
			      fprintf(fout, "\t\t%d,%d,%d, \n", 
				      triangle[j].p[0], triangle[j].p[1],-1);
			  else
			      fprintf(fout,"\t\t%d,%d,%d \n", 
				      triangle[j].p[0], triangle[j].p[1],-1);
			}
		  }
	      elements->Next();
	    }
	fprintf(fout, "]}\n}\n");
      }

  // Add Balls
  if (nodes->getnumnodes()<32)
      {
	fprintf(fout, "Separator {\n");		    
	
	fprintf(fout, "\tMaterial { diffuseColor [ 1.0 1.0 0.0 ]\n transparency [0.5 ] \n} ");		
	for (i=0;i<nodes->getnumnodes();i++)
	    {
	      fprintf(fout,"Separator {");
	      fprintf(fout,"Transform {\n\ttranslation %6.2f %6.2f %6.2f\n  scaleFactor     1 1 1}\n",
		      nodes->getpoint(i)->x[0],
		      nodes->getpoint(i)->x[1],
		      nodes->getpoint(i)->x[2]);
	      fprintf(fout,"Sphere {  radius 1.0   }\n");
	      fprintf(fout,"}");
	    }
	fprintf(fout,"}\n");
      }


  if (hasfemdata())
      {
	for(i=0; i<nodes->getnumnodes(); i++)
	    {
	      nodes->getpoint(i)->x[0]-=(femData->getdisplacement(i))->x[0]; 
	      nodes->getpoint(i)->x[1]-=(femData->getdisplacement(i))->x[1];
	      nodes->getpoint(i)->x[2]-=(femData->getdisplacement(i))->x[2];
	    }
	if (hasextranodes())
	    {
	      for (i=0;i<extraNodes->getnumnodes();i++)
		  for (j=0;j<=2;j++)
		      extraNodes->getpoint(i)->x[j]-=(femData->getdisplacement(i+nodes->getnumnodes()))->x[j]; 
	    }
	
      }
  return 1;
}
// --------------------------------------------------------------------------
int PXCrystalAbaqus::createFromCrystalFiles(const char* classfile,const char* positionfile,float minvol,
					    float minangle,int extra)
{
  int doextranodes=Irange(extra,0,2);
  

  FILE *fin;
  fin=fopen(positionfile,"r");
  if (!fin)
      {
	fprintf(stderr,"Cannot read %s\n",positionfile);
	return(0);
      }
  
  // Read the displacements file 
  int numcrystals;
  fscanf(fin,"%d\n%d\n",&numcrystals,&numframes);

  if (numframes>0 && numframes<200 && numcrystals>0 && numcrystals<33)
      {
	nodes=new PXSolidNodes(numcrystals);
	hasNodes=1;
        allocateDisplacements();
	fprintf(stderr,"Reading frames(%d) : crystals(%d)\n",numframes,numcrystals);
      }
  else
      {
	fprintf(stderr,"Invalid number of frames(%d) or crystals(%d)\n",numframes,numcrystals);
	return 0;
      }
  
  int fr;
  for (fr=0;fr<numframes;fr++)
      {
	for (int cr=0;cr<numcrystals;cr++)
	    {
	      float tempx=0.0,tempy=0.0,tempz=0.0;
	      fscanf(fin,"%f %f %f",&tempx,&tempy,&tempz);
	      displacements[fr]->setdisplacement(cr,tempx,tempy,fabs(tempz));
	    }
	fscanf(fin,"\n");
      }
  fclose(fin);
  
  // Read the Class file and generate elements -- read once to count, second time to generate
  
  fin=fopen(classfile,"r");
  if (!fin)
      {
	fprintf(stderr,"Cannot read %s\n",classfile);
	cleanup();
	return(0);
      }

  char line[200];
  fgets(line,200,fin);
  fprintf(stderr,"LIne=%s",line);
  int st=strncmp(line, "#CRYSTAL CONFIGURATION",22);
  if (st!=0)
      {
	fprintf(stderr,"Wrong header line %s in %s\n",line,classfile);
	cleanup();
	return(0);
      }

  fgets(line,200,fin);  fgets(line,200,fin); // Read 2 comment lines 

  numgroups=0;
  fscanf(fin,"%d\n",&numgroups);
  
  if (numgroups==0)
      {
	fprintf(stderr,"Bad number of groups %d in %s\n",numgroups,classfile);
	cleanup();
	return(0);
      }

  elements=new PXNeighboursList();
  hasElements=1;
  
  // Make positions into real displacements
  for (int c=0;c<numcrystals;c++)
      for (int j=0;j<=2;j++)
	  nodes->getpoint(c)->x[j]=displacements[0]->getdispcomponent(c,j);


  for (fr=numframes-1;fr>=0;fr-=1)
      for (int c=0;c<numcrystals;c++)
	  for (int j=0;j<=2;j++)
	      displacements[fr]->setdispcomponent(c,j,
						  displacements[fr]->getdispcomponent(c,j)-
						  displacements[0]->getdispcomponent(c,j));
						    
  int  points[100],pt2[10];
 
  for (int k=0;k<=1;k++)
      {
	int numdepth=0;
	fgets(line,200,fin);
	fscanf(fin,"%d\n",&numdepth);
	for (int j=0;j<numdepth;j++)
	    {
	      fscanf(fin,"%d",&points[j]);
	      points[j]-=PXABCR_PT_OFFSET;
	      nodes->setindex(points[j],0,k);
	      nodes->setindex(points[j],1,j);
	    }
	if (numdepth==2)
	    elements->AddN(points,numdepth,k);
	else
	    {
	      for (int ia=0;ia<=2;ia++)
		  {
		    pt2[0]=points[ia];
		    pt2[1]=points[Icyclic(ia+1,3)];
		    elements->AddN(pt2,2,k);
		  }
	    }
	fscanf(fin,"\n");
      }
  
  // Data Groups

  if (hasextranodes())
      {
	delete extraNodes;
	hasExtraNodes=0;
      }
  if (doextranodes>0)
      {
	if (doextranodes==1)
	    extraNodes=new PXSolidNodes( (numgroups-2));
	if (doextranodes==2)
	    extraNodes=new PXSolidNodes(5*4*(numgroups-2));
	hasExtraNodes=1;
      }

  int usedqhull=0;
  for (int group=2;group<numgroups;group++)
      {
	int nummid=0;
	fgets(line,200,fin);
	fprintf(stderr,"\nLine(%d)=%s",group,line);
	int numdepth,numrows;
	fscanf(fin,"%d %d\n",&numrows,&numdepth);
	fprintf(stderr,"numrows=%d, numdepth=%d\n",numrows,numdepth);
	int ind=0;
	for (int i=0;i<numrows;i++)
	    {
	      for (int j=0;j<numdepth;j++)
		  {
		    int temp;
		    fscanf(fin,"%d",&temp);
		    if (temp>0)
			{
			  points[ind]=temp-PXABCR_PT_OFFSET;
			  nodes->setindex(points[ind],0,group);
			  if (j==0)
			      nodes->setindex(points[ind],1,i);
			  else if (j==numdepth-1)
			      nodes->setindex(points[ind],1,100+i);
			  else
			      {
				nodes->setindex(points[ind],1,j*25);
				nummid++;
			      }
			  ind++;
			}
		  }
	      fscanf(fin,"\n");
	    }

	if (doextranodes==1)
	    {
	      int pindex=(group-2);
	      for (int j=0;j<=2;j++)
		  {
		    float temp=0.0;
		    for (int i=0;i<ind;i++)
			temp+=nodes->getpoint(points[i])->x[j];
		    temp/=float(ind);
		    extraNodes->getpoint(pindex)->x[j]=temp;
		  }
	      extraNodes->setindex(pindex,0,group);
	      extraNodes->setindex(pindex,1,75);
	      points[ind]=pindex+nodes->getnumnodes();
	      ind++;
	    }
	else if (doextranodes==2)
	    {
	      int pointindex2[4];
	      int pindex=5*4*(group-2);
	      int nel0=elements->getnumnodes();

	      doqhull(points,ind,group);
	      
	      int nel=elements->getnumnodes();
	      float* vol=new float[nel-nel0+1];
	      fprintf(stderr,"Nel=%d, nel0=%d\n",nel,nel0);

	      elements->Rewind();
	      int i;
	      for (i=0;i<nel0;i++)
		  elements->Next();      
	      for (i=nel0;i<nel;i++)
		  {
		    PXIntegerList* intList=((elements->getcurrentnode())->points);
		    intList->Rewind();
		    for (int j=0;j<=3;j++)
			{
			  pointindex2[j]=intList->getcurrent();
			  intList->Next();
			}
		    vol[i-nel0]=tetravolume(pointindex2);
		    elements->Next();
		  }
	      
	      for (int pass=0;pass<5;pass++)
		  {
		    float maxvol=0.0;
		    int volindex=0;
		    for (i=0;i<nel-nel0;i++)
			{
			  if (maxvol<vol[i])
			      {
				maxvol=vol[i];
				volindex=i;
			      }
			}
		    fprintf(stderr,"          -------------\n");
		    fprintf(stderr,"Inserting centroids of tetra %d (%7.3f)\n",
			    volindex+nel0+1,vol[volindex]);
		    vol[volindex]=-1.0;
		    elements->Rewind();
		    for (int i=0;i<nel0+volindex;i++)
			elements->Next();
		    PXIntegerList* intList=((elements->getcurrentnode())->points);
		    
		    for (int count=0;count<=3;count++)
			{
			  for (int dim=0;dim<=2;dim++)
			      {
				intList->Rewind();
				float tp=0.0;
				for (int j=0;j<=3;j++)
				    {
				      int current=intList->getcurrent();
				      intList->Next();
				      float tp2=nodes->getpoint(current)->x[dim];
				      fprintf(stderr,"(%d %6.2f) ",current,tp2);
				      if (count==j)
					  tp+=5.0*tp2;
				      else
					  tp+=tp2;
				    }
				tp*=0.125;
				extraNodes->getpoint(pindex)->x[dim]=tp;
				fprintf(stderr,"%6.2f \n",tp);
			      }
			  extraNodes->setindex(pindex,0,group);
			  extraNodes->setindex(pindex,1,75);
			  points[ind]=pindex+nodes->getnumnodes();
			  ind++;
			  pindex++;
			  fprintf(stderr,")\n");
			}
		  }
	      delete [] vol;
	      elements->Rewind();
	      for (i=0;i<elements->getnumnodes();i++)
		  {
		    if (elements->getcurrentnode()->index==group)
			elements->getcurrentnode()->index=-1;
		    elements->Next();
		  }
	      elements->RemoveInvalid();
	    }
	if (nummid==0 && doextranodes==0)
	    {
	      fprintf(stderr,"****** No midwall nodes\n");
	      // Generate Elements by hand
	      elements->AddN(points,ind,group);
	    }
	else
	    {
	      doqhull(points,ind,group);
	      usedqhull=1;
	    }
      }
  
  if (usedqhull==1)
      {
	checkelements(minvol,minangle);
	checkelements(minvol,minangle);
	
	int num=numcrystals;
	if (hasextranodes())
	    num+=extraNodes->getnumnodes();
	int i;
	for (i=0;i<num;i++)
	    points[i]=0;
	
	elements->Rewind();  
	for (int el=0;el<elements->getnumnodes();el++)
	    {
	      PXIntegerList* intList=((elements->getcurrentnode())->points);
	      intList->Rewind();
	      for (int pt=0;pt<intList->getnumnodes();pt++)
		  {
		    int nd=intList->getcurrent();
		    points[nd]++;
		    intList->Next();
		  }
	      elements->Next();
	    }
	
	int bad=0;
	
	fprintf(stderr,"Bad crystals: ");
	for (i=0;i<numcrystals;i++)
	    {
	      if (points[i]==0)
		  {
		    bad++;
		    fprintf(stderr,"%d(%d:%d) , ",i+1,nodes->getindex(i,0),nodes->getindex(i,1));
		    nodes->setindex(i,0,-1);
		  }
	    }
	fprintf(stderr," number =%d \n",bad);
      }
  //elements->Save("elements.out");

  return 1;
}
// --------------------------------------------------------------------------
int PXCrystalAbaqus::saveSolid(const char* fname)
{

  char* abname=anacreatefilename(fname,".sld",".inp");
  FILE* fout=fopen(abname,"w");
  if (!fout)
      return(0);

  PXContourStack *c1=new PXContourStack(2);
  c1->setellipticalcylinder(4,10.0,10.0,50.0,50.0);
  PXTriangulatedStack* tstack[2];

  tstack[0]=new PXTriangulatedStack(c1,0);
  c1->setellipticalcylinder(4,15.0,15.0,50.0,50.0);
  tstack[1]=new PXTriangulatedStack(c1,0);
  delete c1;

  if (!haselements() || !hasnodes())
      return 0;

  fprintf(fout,"#NewAbaqusSolid3\n");
  fprintf(fout,"#Number of layers,points per slice \n");
  fprintf(fout,"%d\n",2);
  fprintf(fout,"%d\n",2);
  for (int t=0;t<2;t++)
      {
	tstack[t]->Save(fout);
	delete tstack[t];
      }
  fprintf(fout,"# ---------------------------------------------------\n");
  fprintf(fout,"#Material Model\n");
  fprintf(fout,"# No Material\n");
  fprintf(fout,"# ---------------------------------------------------\n");
  fprintf(fout,"# Solid Nodes\n");
  if (hasextranodes())
      {
	PXSolidNodes* tempNodes=new PXSolidNodes(nodes,extraNodes);
	tempNodes->Save(fout);
	delete tempNodes;
      }
  else
      nodes->Save(fout);
  fprintf(fout,"# ---------------------------------------------------\n");
  fprintf(fout,"# Spring Nodes\n");
  fprintf(fout,"#No Spring Nodes\n");

  fprintf(fout,"# ---------------------------------------------------\n");
  fprintf(fout,"# Solid Elements\n");

  elements->Save(fout);

  fprintf(fout,"# ---------------------------------------------------\n");
  fprintf(fout,"# Spring Elements\n"); 
  fprintf(fout,"#No Spring Elements\n"); 
  fprintf(fout,"# ---------------------------------------------------\n");
  fprintf(fout,"#Numerical Constants\n");
  fprintf(fout,"#Element Mode\n%d\n",8);
  fprintf(fout,"#Output Step\n%6.3f\n",1.0);
  fprintf(fout,"#Output Inc\n%d\n",40);
  fprintf(fout,"#Enforcement Strategy\n%d\n",0);
  fprintf(fout,"#Spring Multiplier\n%5.3f\n",1.0);
  fprintf(fout,"#Orientation\n%d\n",1);
  fclose(fout);
  delete [] abname;
  return 1;
  
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::setPieIndices()
{
   if (!hasnodes() ||  !haselements())
      {
	fprintf(stderr,"No nodes orelements\n");
	return ;
      }
  allocateFemData();
  femData->setPieStats(numgroups,numgroups,1);
  elements->Rewind();

  for (int i=0;i<elements->getnumnodes();i++)
      {
	int pie=elements->getcurrentnode()->index;
	femData->setElementPieIndex(i,pie);
	elements->Next();
      }
}
// --------------------------------------------------------------------------
float PXCrystalAbaqus::trianglearea(int* points)
{
  PX3Vector v[3];
  for (int i=0;i<=2;i++)
      {
	for (int j=0;j<=2;j++)
	    v[i].x[j]=nodes->getpoint(points[i])->x[j];
	if (i>0)
	    v[i].subtract(&v[0]);
      }
  return 0.5*v[1].dotwith(&v[2]);
  
}
// --------------------------------------------------------------------------
float PXCrystalAbaqus::tetraminangle(int* points)
{
  PX3Vector v[4],normal[4];
  int i;
  for (i=0;i<=3;i++)
      {
	for (int j=0;j<=2;j++)
	    v[i].x[j]=nodes->getpoint(points[i])->x[j];
      }
  
  //  fprintf(stderr,"--------------------------------------------\n");
  
  for (i=0;i<=3;i++)
      {
	normal[3].setvector(0.0,0.0,0.0);
	for (int j=1;j<=2;j++)
	    {
	      PX3Vector temp;
	      int p0=i;
	      int p1=Icyclic(i+j,4);
	      int p2=Icyclic(i+j+1,4);

	      normal[i].copy(&v[p1]);
	      normal[i].subtract(&v[p0]);
	      temp.copy(&v[p0]);
	      temp.subtract(&v[p2]);

	      normal[i].crosswithoverwrite(&temp); 
	      normal[i].normalize();
	    }
      }

  float minangle=180.0;
  //fprintf(stderr,"Angles : ");
  for (i=0;i<=2;i++)
      for (int j=i+1;j<=3;j++)
	  {
	    float dotp=normal[i].dotwith(&normal[j]);
	    float a=acos(dotp);
	    a=a*180.0/M_PI;
	    if (a>90.0)
		a=180-a;
	    //fprintf(stderr," %d:%d =%6.3f,",i,j,a);
	    if (minangle>a)
		minangle=a;
	  }
  //  fprintf(stderr," min=%6.3f\n",minangle);

  return (minangle);
}
// --------------------------------------------------------------------------
float PXCrystalAbaqus::tetravolume(int* points)
{
  PX3Vector v[4];
  for (int i=0;i<=3;i++)
      {
	for (int j=0;j<=2;j++)
	    {
	      if (points[i]<nodes->getnumnodes())
		  v[i].x[j]=nodes->getpoint(points[i])->x[j];
	      else
		  v[i].x[j]=extraNodes->getpoint(points[i]-nodes->getnumnodes())->x[j];
	    }
	if (i>0)
	    v[i].subtract(&v[0]);
      }
  
  v[2].crosswithoverwrite(&v[3]);
  return v[2].dotwith(&v[1])/3.0;

}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::saveqhull(const char* fname,int* points,int numpoints)
{
  fprintf(stderr,"In save qhull %s \n",fname);
  FILE* fout=fopen(fname,"w");
  if (fout)
      {
	// Write File out to qhull format
	fprintf(fout,"3\n");
	fprintf(fout,"%d\n",numpoints);
	for (int j=0;j<numpoints;j++)
	    {
	      //	      fprintf(stderr,"%d ",points[j]);
	      if (points[j]<nodes->getnumnodes())
		  {
		    fprintf(fout,"%6.3f %6.3f %6.3f\n",
			    nodes->getpoint(points[j])->x[0],
			    nodes->getpoint(points[j])->x[1],
			    nodes->getpoint(points[j])->x[2]);
		  }
	      else
		  {
		    int nd=points[j]-nodes->getnumnodes();
		    fprintf(fout,"%6.3f %6.3f %6.3f\n",
			    extraNodes->getpoint(nd)->x[0],
			    extraNodes->getpoint(nd)->x[1],
			    extraNodes->getpoint(nd)->x[2]);
		    //		    fprintf(stderr,"Point %d -->%d\n",points[j],nd);
		  }
	    }
	fclose(fout);
      }
  fprintf(stderr,"Done save qhull\n");
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::checkelements(float minvol,float minangle,int start,int end)
{
  if (end==-1)
      end=elements->getnumnodes();
  
  start=Irange(start,0,elements->getnumnodes()-2);
  end=Irange(end,start+1,elements->getnumnodes());

  elements->Rewind();
  int i;
  for (i=0;i<start;i++)
      elements->Next();

  fprintf(stderr,"\n\n-------------------------------------\n");
  fprintf(stderr,"Checking Elements %d:%d (of total %d) minvol=%6.2f minangle=%5.3f\n",
	  start,end,elements->getnumnodes(),minvol,minangle);
  
  for (i=start;i<end;i++)
      {
	PXIntegerList* intList=elements->getcurrentnode()->points;
	if (intList->getnumnodes()==4)
	    {
	      int  points2[4];
	      intList->Rewind();
	      for (int j=0;j<=3;j++)
		  {
		    points2[j]=intList->getcurrent();
		    intList->Next();
		  }
	      float vol=tetravolume(points2);
	      float angle=tetraminangle(points2);
	      int ok=1;
	      
	      if (vol<-minvol)
		  {
		    int tmp=points2[2];
		    points2[2]=points2[1];
		    points2[1]=tmp;
		    vol=tetravolume(points2);
		  }
	      
	      if (vol<minvol || angle<minangle)
		  ok=0;
	      
	      if (ok>=1)
		  {
		    fprintf(stderr,"Accepting %d [%d,%d,%d,%d] vol=%5.3f ok=%d angle=%6.3f\n",
			    i+1,
			    points2[0]+1,points2[1]+1,
			    points2[2]+1,points2[3]+1,vol,ok,angle);
		  }
	      else
		  {
		    fprintf(stderr,"Rejecting %d [%d %d %d %d ] vol=%5.3f angle=%6.3f\n",i+1,
			    points2[0]+1,points2[1]+1,
			    points2[2]+1,points2[3]+1,
			    vol,angle);
		    elements->getcurrentnode()->index=-1;
		  }
	    }
	elements->Next();
      }
  elements->RemoveInvalid();
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::readqhull(const char* fname,int* points,int numpoints,int group)
{
  char command[200];
  FILE* fin=fopen(fname,"r");
  if (fin)
      {
	fgets(command,200,fin);
	int np,temp,nt;
	fscanf(fin,"%d %d %d\n",&np,&nt,&temp);
	fprintf(stderr,"Reading %s points=%d, tetra=%d\n",fname,np,nt);
	float x1,y1,z1,t1;
	for (int j=0;j<numpoints;j++)
	    fscanf(fin,"%f %f %f %f\n",&x1,&y1,&z1,&t1);
	int points2[4];
	for (int i=0;i<nt;i++)
	    {
	      int npt;
	      fscanf(fin,"%d",&npt);
	      for (int j=0;j<npt;j++)
		  {
		    fscanf(fin,"%d",&temp);
		    points2[j]=points[temp];
		  }
	      elements->AddN(points2,npt,group);
	    }
      }
}
  /* 
     Subject 5.21: How can the circumsphere of a tetrahedron be computed?

     Let a, b, c, and d be the corners of the tetrahedron, with ax, ay, and az the components of a, and likewise for b, c, and d.  Let |a| denote the Euclidean
     norm of a, and let a x b denote the cross product of a and b.  Then the center m and radius r of the circumsphere are given by
     

          |                                                                       |
          | |d-a|^2 [(b-a)x(c-a)] + |c-a|^2 [(d-a)x(b-a)] + |b-a|^2 [(c-a)x(d-a)] |
          |                                                                       |
       r= -------------------------------------------------------------------------
                                    | bx-ax  by-ay  bz-az |
                                  2 | cx-ax  cy-ay  cz-az |
                                    | dx-ax  dy-ay  dz-az |
      

    and

              |d-a|^2 [(b-a)x(c-a)] + |c-a|^2 [(d-a)x(b-a)] + |b-a|^2 [(c-a)x(d-a)]
       m= a + ---------------------------------------------------------------------
                                      | bx-ax  by-ay  bz-az |
                                    2 | cx-ax  cy-ay  cz-az |
                                      | dx-ax  dy-ay  dz-az |
				      

int PXCrystalAbaqus::isinsidetetra(PX3Vector* pt,int npt,PXIntegerList* tetra,int testpoint)
  
  int index[4];
  tetra->Rewind();
  for (int i=0;i<=3;i++)
      {
	index[i]=tetra->getcurrent();
	tetra->Next();
      }

  PX3Vector a,b,c,d;
  a.copy(&pt[index[0]]);  b.copy(&pt[index[0]]);  c.copy(&pt[index[0]]);  d.copy(&pt[index[0]]);
	 
  PX3Vector bma,cma,dma;
  bma.copy(&b); bma.subtract(&a);
  cma.copy(&c); cma.subtract(&a);
  dma.copy(&d); dma.subtract(&a);
 
  PX3Vector temp;
  temp.copy(&cma);
  // FInd volume i.e bottom determinant
  temp.crosswithoverwrite(&dma);
  float det2=fabs(2.0*temp.dotwith(&bma));
		  
  PX3Vector bxc;  bxc.copy(&bma); bxc.crosswithoverwrite(&cma);
  PX3Vector dxb;  dxb.copy(&dma);  dxb.crosswithoverwrite(&bma);
  PX3Vector cxd;  bxc.copy(&cma);  bxc.crosswithoverwrite(&dma);


  float magdma=pow(dma.getlength(),2.0);
  float magcma=pow(cma.getlength(),2.0);
  float magbma=pow(bma.getlength(),2.0);

  PX3Vector center;
  for (i=0;i<=2;i++)
      center.x[i]=a.x[i]+ ((magdma*bxc.x[i]+magcma*dxb.x[i]+magbma*cxd.x[i])/det2);

  temp.copy(&center);
  temp.subtract(&a);

  float radius=temp.getlength();

  temp.copy(&pt[testpoint]);
  temp.subtract(&center);
  
  float dist=temp.getlength();

  return dist<radius;

}*/

// --------------------------------------------------------------------------
void PXCrystalAbaqus::doqhull(int* points,int numpoints,int group)
{
  char line[50],command[150];
  sprintf(line,"tmp%02d.qhull",group);

  fprintf(stderr,"Group = %d Points = ",group);
  for (int i=0;i<numpoints;i++)
      fprintf(stderr,"%d ",points[i]);
  fprintf(stderr,"\n");


  saveqhull(line,points,numpoints);
  
  // Execute Qhull
  sprintf(command,"qhull2 < %s d -Qbb -o  > %s.geom",line,line);
  system(command);
  sprintf(command,"/sbin/rm %s",line);
  system(command);
  
  // Read Back output
  sprintf(line,"%s.geom",line);
  readqhull(line,points,numpoints,group);
  sprintf(command,"/sbin/rm %s",line);
  system(command);
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::savedistances(const char* fname1,const char* fname2,int crystal1,int crystal2)
{
  fprintf(stderr,"Comparing positions of %d and %d\n",crystal1,crystal2);
  crystal1-=1;
  crystal2-=1;
  int numcr,numfr;
  FILE* fin=fopen(fname1,"r");
  if (!fin)
      {
	fprintf(stderr,"Cannot open %s\n",fname1);
	return ;
      }
  
  fscanf(fin,"%d\n%d\n",&numcr,&numfr);

  if (numfr!=numframes)
      {
	fprintf(stderr,"Different number of frames!!!\n");
	return;
      }
  
  PXSolidNodes* tempnodes=new PXSolidNodes(numcr);
  PXNodalDisplacements* disp[100];
  int i;
  for (i=0;i<numfr;i++)
      disp[i]=new PXNodalDisplacements(tempnodes,1,100);

  fprintf(stderr,"Reading frames(%d) : crystals(%d)\n",numfr,numcr);
  
  for (int fr=0;fr<numfr;fr++)
      {
	for (int cr=0;cr<numcr;cr++)
	    {
	      float tempx=0.0,tempy=0.0,tempz=0.0;
	      fscanf(fin,"%f %f %f",&tempx,&tempy,&tempz);
	      disp[fr]->setdisplacement(cr,tempx,tempy,tempz);
	    }
	fscanf(fin,"\n");
      }
  fclose(fin);
  
  FILE* fout=fopen(fname2,"w");
  fprintf(fout,"#Distance between crystal pair %d-%d\n",crystal1+1,crystal2+1);


  PX3Vector* pos01=disp[0]->getdisplacement(crystal1);
  PX3Vector* pos02=disp[0]->getdisplacement(crystal2);

  float dist012=pos01->getdistance(pos02);
  
  PX3Vector pos11,pos12;
  for (int j=0;j<=2;j++)
      {
	pos11.x[j]=nodes->getpoint(crystal1)->x[j];
	pos12.x[j]=nodes->getpoint(crystal2)->x[j];
      }

  float dist112=pos11.getdistance(&pos12);
  
  fprintf(fout,"#Original Positions 0 (%6.2f %6.2f %6.2f) and (%6.2f %6.2f %6.2f)\n",
	  pos01->x[0],pos01->x[1],pos01->x[2],pos02->x[0],pos02->x[1],pos02->x[2]);
  fprintf(fout,"#Original Positions 1 (%6.2f %6.2f %6.2f) and (%6.2f %6.2f %6.2f)\n",
	  pos11.x[0],pos11.x[1],pos11.x[2],pos12.x[0],pos12.x[1],pos12.x[2]);
  fprintf(fout,"#Frame distance1 distance2 strain1 strain2\n");
  for (i=0;i<numframes;i++)
      {
	PX3Vector* pos01=disp[i]->getdisplacement(crystal1);
	PX3Vector* pos02=disp[i]->getdisplacement(crystal2);

	float dist0=pos01->getdistance(pos02);

	PX3Vector pos11,pos12;
	for (int j=0;j<=2;j++)
	    {
	      pos11.x[j]=nodes->getpoint(crystal1)->x[j]+displacements[i]->getdispcomponent(crystal1,j);
	      pos12.x[j]=nodes->getpoint(crystal2)->x[j]+displacements[i]->getdispcomponent(crystal2,j);;
	    }

	float dist1=pos11.getdistance(&pos12);

	fprintf(fout,"%5d %6.3f %6.3f %5.3f %5.3f \n",i+1,
		dist0,dist1,(dist0-dist012)/dist012,(dist1-dist112)/dist112);
      }
  fclose(fout);

  delete  tempnodes;
  for ( i=0;i<numfr;i++)
      delete disp[i];


}
// --------------------------------------------------------------------------
int PXCrystalAbaqus::createTagMarkerSolid(float avevol,float anrange)
{
  if (!hasnodes())
      return 0;

  int* points=new int[nodes->getnumnodes()];  
  int i;
  for (i=0;i<nodes->getnumnodes();i++)
      points[i]=i;
  
  if (hasElements==1)
      delete elements;
  
  elements=new PXNeighboursList();
  hasElements=1;
  
  doqhull(points,nodes->getnumnodes(),0);

  fprintf(stderr,"nodes=%d elements=%d\n",nodes->getnumnodes(),elements->getnumnodes());
  int bad=0;
  elements->Rewind();
  int el;
  for (el=0;el<elements->getnumnodes();el++)
      {
	if (15*int(el/15)==el)
	    fprintf(stderr,"El=%d/%d \t",el,elements->getnumnodes());
	PXIntegerList* intList=((elements->getcurrentnode())->points);
	int pt[4];
	for (int count=0;count<=3;count++)
	    {
	      pt[count]=intList->getcurrent();
	      intList->Next();
	    }

	float vol=fabs(tetravolume(pt));
	float an=tetraminangle(pt);
	//if (15*int(el/15)==el)
	fprintf(stderr,"Volume =%6.2f angle=%6.2f\n",vol,an);
	
	vol/=avevol;

	if (vol<0.2 || vol>2.4 || an<anrange || an>(180.0-anrange))
	    {
	      elements->getcurrentnode()->index=-1;
	      bad++;
	    }
      	elements->Next();
      }
  
  fprintf(stderr,"Bad=%d\n",bad);
  elements->RemoveInvalid();
  fprintf(stderr,"Good elements=%d\n",elements->getnumnodes());
  
  for (i=0;i<nodes->getnumnodes();i++)
      points[i]=0;
  
  
  elements->Rewind();  
  for (el=0;el<elements->getnumnodes();el++)
      {
	PXIntegerList* intList=((elements->getcurrentnode())->points);
	intList->Rewind();
	for (int pt=0;pt<intList->getnumnodes();pt++)
	    {
	      int nd=intList->getcurrent();
	      points[nd]++;
	      intList->Next();
	    }
	elements->Next();
      }
	
  bad=0;
	
 for (i=0;i<nodes->getnumnodes();i++)
      {
	if (points[i]==0)
	    {
	      bad++;
	      nodes->setindex(i,0,-1);
	    }
      }
  fprintf(stderr,"Number of bad markers=%d/%d \n",bad,nodes->getnumnodes());

  delete [] points;

  return elements->getnumnodes();
}
// --------------------------------------------------------------------------
int PXCrystalAbaqus::createMarkerSolid(const char* fname)
{
  if (hasElements==1)
      delete elements;
  
  elements=new PXNeighboursList();
  int ok=elements->Load(fname);
  if (ok==0)
      {
	fprintf(stderr,"Failed to read elements from %s\n",fname);
	delete elements;
	hasElements=0;
	return 0;
      }
  else
      fprintf(stderr,"Elements(%d) read  from %s\n",elements->getnumnodes(),fname);

  hasElements=1;
  int* points=new int[nodes->getnumnodes()];  
  int i;
  for (i=0;i<nodes->getnumnodes();i++)
      points[i]=0;
  
  elements->Rewind();  
  for (int el=0;el<elements->getnumnodes();el++)
      {
	PXIntegerList* intList=((elements->getcurrentnode())->points);
	intList->Rewind();
	for (int pt=0;pt<intList->getnumnodes();pt++)
	    {
	      int nd=intList->getcurrent();
	      points[nd]++;
	      intList->Next();
	    }
	elements->Next();
      }
  int bad=0;
  for (i=0;i<nodes->getnumnodes();i++)
      {
	if (points[i]==0)
	    {
	      bad++;
	      nodes->setindex(i,0,-1);
	    }
      }
  fprintf(stderr,"Number of bad markers=%d/%d \n",bad,nodes->getnumnodes());
  delete [] points;

  elements->Save("elements.list");

  return elements->getnumnodes();
  
}
// --------------------------------------------------------------------------
void PXCrystalAbaqus::getElementCentoid(PXIntegerList* element,float& x,float& y,float& z)
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
// --------------------------------------------------------------------------
int PXCrystalAbaqus::createOrientations(PX3Vector* orient1,PX3Vector* orient2,PX3Vector* orient3,
					PXAbaqusSolid* sld)
{
  if (!haselements() || !hasnodes() || !sld->haselements() || !sld->hasnodes())
      return 0;

  PXNeighboursList* sldElem=sld->getelements();
  PXIntegerList* sldElements[20000];

  sldElem->Rewind();
  int i;
  for (i=0;i<sldElem->getnumnodes();i++)
      {
	sldElements[i]=(sldElem->getcurrentnode())->points;
	sldElem->Next();
      }
  
  // Calculate Centroids 
  PX3Vector* centroid=new PX3Vector[sldElem->getnumnodes()];
  for (i=0;i<sldElem->getnumnodes();i++)
      sld->getElementCentroid(sldElements[i],centroid[i].x[0],centroid[i].x[1],centroid[i].x[2]);
	
  elements->Rewind();
  
  PX3Vector cent;

  for (i=0;i<elements->getnumnodes();i++)
      {
	getElementCentroid(elements->getcurrentnode()->points,cent.x[0],cent.x[1],cent.x[2]);
	float mindist=1000000.0;
	int best=0;
	for (int j=0;j<sldElem->getnumnodes();j++)
	    {
	      float d=cent.getdistance(&centroid[j]);
	      if (d<mindist)
		  {
		    mindist=d;
		    best=j;
		  }
	    }
	
	sld->getdirections(sldElements[best],
			   orient1[i].x[0],orient1[i].x[1],orient1[i].x[2],  // Radial
			   orient2[i].x[0],orient2[i].x[1],orient2[i].x[2],  // Circumferential
			   orient3[i].x[0],orient3[i].x[1],orient3[i].x[2],  // Longidutinal
			   0);
	if (25*int(i/25)==i)
	    {
	      fprintf(stderr,"Finding orientations for element %d/%d Best sld elem=%d dist=%6.2f\n",
		      i,elements->getnumnodes(),best,mindist);
	      fprintf(stderr,"\t Radial (%5.2f %5.2f %5.2f) Circ (%5.2f %5.2f %5.2f) Long (%5.2f %5.2f %5.2f)\n",
		      orient1[i].x[0],orient1[i].x[1],orient1[i].x[2], 
		      orient2[i].x[0],orient2[i].x[1],orient2[i].x[2], 
		      orient3[i].x[0],orient3[i].x[1],orient3[i].x[2]);
	    }
	elements->Next();
      }
  delete [] centroid;
  return 1;

}

// --------------------------------------------------------------------------
int PXCrystalAbaqus::saveMarkerAbaqus(const char* fname,int resout,PXAbaqusSolid* sld,int useorientations)
{


  if (!hasnodes() ||  !hasdisplacements() || !haselements())
      {
	fprintf(stderr,"No nodes,elements or displacements\n");
	return 0;
      }

  PX3Vector* orient1=new PX3Vector[getelements()->getnumnodes()];
  PX3Vector* orient2=new PX3Vector[getelements()->getnumnodes()];
  PX3Vector* orient3=new PX3Vector[getelements()->getnumnodes()];
  if (useorientations==1)
      createOrientations(orient1,orient2,orient3,sld);
  else
      {
	for (int i=0;i<getelements()->getnumnodes();i++)
	    {
	      orient1[i].setvector(1.0,0.0,0.0);
	      orient2[i].setvector(0.0,1.0,0.0);
	      orient3[i].setvector(0.0,0.0,1.0);
	    }
      }
	
  char* abname=anacreatefilename(fname,".inp",".mark");
  fprintf(stderr,"Creating Job %s --> %s \n",fname,abname);

  FILE* fout=fopen(abname,"w");

  if (!fout)
      {
	delete [] abname;
	return 0;
      }

  fprintf(fout,"*HEADING\nMyocardium Model\n");
  fprintf(fout,"** Numnodes=%d, numelements=%d\n",nodes->getnumnodes(),
	  elements->getnumnodes());
  fprintf(fout,"** ----------------------------------------------------\n");
  fprintf(fout,"** Shrink .dat file\n*PREPRINT,ECHO=NO,MODEL=NO,HISTORY=NO\n**\n");

  fprintf(fout,"** ----------Material Model --------------------------------------\n");
  fprintf(fout,"*MATERIAL,NAME=TISSUE\n*ELASTIC\n");
  fprintf(fout,"%7.3f,%7.3f\n",1.0,0.3);
  
  fprintf(fout,"** ---------- Nodes  --------------------------------------\n");
  
  fprintf(fout,"*NODE,NSET=ALLNODES\n");
  
  for (int i=0;i<nodes->getnumnodes();i++)
      {
	if (nodes->getindex(i,0)>=0)
	    {
	      fprintf(fout," %5d , %7.3f , %7.3f , %7.3f \n",
		      i+1000,
		      nodes->getpoint(i)->x[0],
		      nodes->getpoint(i)->x[1],
		      nodes->getpoint(i)->x[2]);
	    }
      }
  

  
  fprintf(fout,"** ---------- Elements --------------------------------------\n");  

  char line[100],orient[20];
  
  fprintf(stderr,"** \n** Orientations for each elements\n");
  elements->Rewind();
  for (int el=0;el<elements->getnumnodes();el++)
      {
	sprintf(orient,"ORT_%04d",el+1000);
	fprintf(fout,"**\n*ORIENTATION,NAME=%s,SYSTEM=RECTANGULAR\n",orient);
	fprintf(fout,"%8.5f,%8.5f,%8.5f, %8.5f,%8.5f,%8.5f\n",
		orient1[el].x[0],orient1[el].x[1],orient1[el].x[2],
		orient2[el].x[0],orient2[el].x[1],orient2[el].x[2]);

	PXIntegerList* intList=((elements->getcurrentnode())->points);
	int np=intList->getnumnodes();
	sprintf(line,"E_E%04d",el+1000);
	fprintf(fout,"*ELEMENT, TYPE=C3D%1d, ELSET=%s\n",np,line);
	intList->Rewind();
	fprintf(fout,"%d,",el+1000);
	for (int i=0;i<intList->getnumnodes();i++)
	    {
	      fprintf(fout,"%d",intList->getcurrent()+1000);
	      if (i!=intList->getnumnodes()-1)
		  fprintf(fout,",");
	      else
		  fprintf(fout,"\n");
	      intList->Next();
	    }
	fprintf(fout,"*SOLID SECTION, ELSET=%s,MATERIAL=TISSUE,ORIENTATION=%s\n",
		line,orient);
	elements->Next();
      }
  fprintf(fout,"**\n");
  fprintf(fout,"*ELSET, ELSET=E_DUMMY\n1000\n**\n");

  for (int fr=0;fr<numframes;fr+=1)
      {
	fprintf(fout,"** -------------------- Frame %d ---------------------------\n**\n",fr+1);
	fprintf(fout,"*STEP,NLGEOM,AMPLITUDE=RAMP,INC=40\n");
	fprintf(fout,"*STATIC\n %f,1.0,0.00005,1.0\n",1.0);
	fprintf(fout,"**\n");
	fprintf(fout,"** Accuracy Controls\n*CONTROLS,PARAMETERS=FIELD,FIELD=DISPLACEMENT\n");
	fprintf(fout,"0.01,1.0,10.0,  ,  , 1.E-3\n");
	fprintf(fout,"*BOUNDARY,OP=NEW\n");
	for (int i=0;i<displacements[fr]->getsolidnodes()->getnumnodes();i++)
	    {
	      if (displacements[fr]->getsolidnodes()->getindex(i,0)>=0)
		  {
		    for (int j=0;j<3;j++)
			fprintf(fout,"%d , %d , %d, %8.4f\n",i+1000,j+1,j+1,
				displacements[fr]->getdispcomponent(i,j));
		  }
	    }
	fprintf(fout,"**\n*EL PRINT,POSITION=CENTROIDAL,ELSET=E_DUMMY\nE11\n");
	fprintf(fout,"*NODE PRINT,NSET=ALLNODES\nU1\n");
	fprintf(fout,"*EL FILE,POSITION=CENTROIDAL\nE\n");
	fprintf(fout,"*EL FILE\nEVOL\n*NODE FILE,NSET=ALLNODES\nU\n");
	if (resout>0)
	    fprintf(fout,"*RESTART,WRITE\n");
	fprintf(fout,"*END STEP\n");
      }
  fprintf(stderr,"** Resout=%d\n",resout);
  fclose(fout);

  delete [] orient1;
  delete [] orient2;
  delete [] orient3;
  return 1;
}
// --------------------------------------------------------------------------

