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
//  Uses a little bit of trivial eigenvector/eigenvalue code ... should be easy to remove
//
//---------------------------------------------------------------------------------------------------------


#include "pxabaqusstructures.h"
#include "pxutil.h"
#include "nrutil.h"





// ************************************************
// Solid Nodes 
// ************************************************

PXSolidNodes::PXSolidNodes(int numn)
{
 init(numn);
}

PXSolidNodes::PXSolidNodes(PXSolidNodes* oldnodes)
{
 init(oldnodes->getnumnodes());
 Copy(oldnodes);
}

PXSolidNodes::PXSolidNodes(PXSolidNodes* oldnodes1,PXSolidNodes* oldnodes2)
{
  init(oldnodes1->getnumnodes()+oldnodes2->getnumnodes());
  Copy(oldnodes1,0,0);
  Copy(oldnodes2,0,oldnodes1->getnumnodes());
}

void PXSolidNodes::init(int numn)
{
 numnodes=Irange(numn,2,100000);
 points=new PXTriangulatedStackPoint[numnodes];
 pointDetails=new PXNodeAuxiliary[numnodes];
 for (int ii=0;ii<numnodes;ii++)
     pointDetails[ii].neighbours=new PXIntegerList();

 for (int i=0;i<numnodes;i++)
     {
       points[i].index=i;
       for (int ia=0;ia<=2;ia++)
	   {
	     points[i].x[ia]=0.0;
	     points[i].nx[ia]=0.0;
	     pointDetails[i].dx[ia]=0.0;
	   }
       
       for (int iia=0;iia<=1;iia++)
	   {
	     points[i].k[iia]=0.0;
	     pointDetails[i].index[iia]=-1;
	   }
       pointDetails[i].temperature=0.0;
     }
 
}
// ************************************************
PXSolidNodes::~PXSolidNodes()
{
  cleanup();
}
// ************************************************
void PXSolidNodes::cleanup()
{
  for (int i=0;i<numnodes;i++)
      delete pointDetails[i].neighbours;
  delete [] pointDetails;
  delete [] points;
}
// ************************************************
int PXSolidNodes::getnumnodes()
{
  return numnodes;
}
// ************************************************
PXTriangulatedStackPoint* PXSolidNodes::getpoint(int pointidx)
{
  //  fprintf(stderr,"getting %d < %d ",pointidx,numnodes);
  pointidx=Irange(pointidx,0,numnodes-1);
  return &points[pointidx];
}
// ************************************************
PXNodeAuxiliary* PXSolidNodes::getdetails(int pointidx)
{
  pointidx=Irange(pointidx,0,numnodes-1);
  return &pointDetails[pointidx];
}
// ************************************************
int PXSolidNodes::getindex(int pointidx,int index)
{
  pointidx=Irange(pointidx,0,numnodes-1);
  index=Irange(index,0,1);
  return pointDetails[pointidx].index[index];
}
// ************************************************
void  PXSolidNodes::settemperature(int pointidx,float val)
{
  pointidx=Irange(pointidx,0,numnodes-1);
  pointDetails[pointidx].temperature=val;
}
// ************************************************
float PXSolidNodes::gettemperature(int pointidx)
{
  pointidx=Irange(pointidx,0,numnodes-1);
  return pointDetails[pointidx].temperature;
}
// ************************************************
void PXSolidNodes::setindex(int pointidx,int index,int val)
{
  pointidx=Irange(pointidx,0,numnodes-1);
  index=Irange(index,0,1);
  pointDetails[pointidx].index[index]=val;
}
// ************************************************
PXIntegerList* PXSolidNodes::getneighbours(int pointidx)
{
  pointidx=Irange(pointidx,0,numnodes-1);
  return pointDetails[pointidx].neighbours;
}
// ************************************************---
int PXSolidNodes::Save(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (fout!=NULL)
      {
       int st=Save(fout);
       fclose(fout);
       return st;
     }
 return 0;
}
// ************************************************---
int PXSolidNodes::Load(const char* fname)
{
 gzFile fin=gzsuffixopen(fname,"r");
 if (fin)
     {
       int st=Load(fin);
       gzclose(fin);
       return st;
     }
 return 0;
}
// ************************************************---
int PXSolidNodes::Save(FILE* fout)
{
  fprintf(fout,"#SolidNodesList\n");
  fprintf(fout,"%d\n",numnodes);
  int ia=0;
  
 for (int i=0;i<numnodes;i++)
   {
       fprintf(fout,"%6d ",points[i].index+1000);
       for (ia=0;ia<=2;ia++)
	   fprintf(fout," %7.3f ",points[i].x[ia]);
       for (ia=0;ia<=2;ia++)
	   fprintf(fout," %7.3f ",points[i].nx[ia]);
       for (ia=0;ia<=1;ia++)
	   fprintf(fout," %7.3f ",points[i].k[ia]);
       fprintf(fout,"\n ");
       for (ia=0;ia<=2;ia++)
	   fprintf(fout," %7.3f ",pointDetails[i].dx[ia]);
       for (ia=0;ia<=1;ia++)
	   fprintf(fout," %5d ",pointDetails[i].index[ia]);
       fprintf(fout,"%7.3f\n",pointDetails[i].temperature);
       
       fprintf(fout," %3d ",pointDetails[i].neighbours->getnumnodes());
       for(int j=0;j<pointDetails[i].neighbours->getnumnodes();j++)
	   {
	     fprintf(fout,"\t%d",pointDetails[i].neighbours->getcurrent());
	     pointDetails[i].neighbours->Next();
	   }
       fprintf(fout,"\n");
     }
 return 1;
}

// ************************************************---
int PXSolidNodes::Load(gzFile fin)
{
 char* line=new char[400];
 gzgets(fin,line,200);
 if (gzfindstringinstring(line,"#SolidNodesList")==0)
     {
       //fprintf(stderr,"line=%s Failed\n",line);
       delete [] line;
       return 0;
     }
 //fprintf(stderr,"line=%s OK\n",line);
     
 int num2;

 gzgets(fin,line,200);
 sscanf(line,"%d\n",&num2);
 cleanup();
 init(num2);
 
 for (int i=0;i<numnodes;i++)
     {
       gzgets(fin,line,200);
       sscanf(line,"%d %f %f %f %f %f %f %f %f",
	      &points[i].index,
	      &points[i].x[0],
	      &points[i].x[1],
	      &points[i].x[2],
	      &points[i].nx[0],
	      &points[i].nx[1],
	      &points[i].nx[2],
	      &points[i].k[0],
	      &points[i].k[1]);
       points[i].index-=1000;

       //fscanf(fin,"%d",&points[i].index);
       //points[i].index-=1000;
       
       //for (int ia=0;ia<=2;ia++)
	 //  fscanf(fin,"%f",&points[i].x[ia]);
       //for (ia=0;ia<=2;ia++)
	 //  fscanf(fin,"%f",&points[i].nx[ia]);
       //for (ia=0;ia<=1;ia++)
	 //  fscanf(fin,"%f",&points[i].k[ia]);
       // fscanf(fin,"\n");
       gzgets(fin,line,200);
       sscanf(line,"%f %f %f %d %d %f",
	      &pointDetails[i].dx[0],
	      &pointDetails[i].dx[1],
	      &pointDetails[i].dx[2],
	      &pointDetails[i].index[0],
	      &pointDetails[i].index[1],
	      &pointDetails[i].temperature);
	      
       //for (ia=0;ia<=2;ia++)
       //	   fscanf(fin," %f",&pointDetails[i].dx[ia]);
       //     for (ia=0;ia<=1;ia++)
       //   fscanf(fin," %d",&pointDetails[i].index[ia]);
       //fscanf(fin,"%f\n ",&pointDetails[i].temperature);
       
       gzgets(fin,line,400);
       int num;
       sscanf(line,"%d ",&num);

       int po=0;
       po=stringskipspace(line,po);
       po=stringskipnumber(line,po);
       for(int j=0;j<num;j++)
	   {
	     int temp;
	     po=stringskipspace(line,po);
	     sscanf(&line[po],"%d",&temp);
	     po=stringskipnumber(line,po);
	     pointDetails[i].neighbours->Add(temp);
	   }
     }
 delete [] line;
 
 return 1;
}
// ************************************************
void PXSolidNodes::Copy(PXSolidNodes* other,int check,int offset)
{
  if (check)
      {
	if (other->getnumnodes()!=numnodes)
	    {
	      cleanup();
	      init(other->numnodes);
	    }
      }
  
  for (int i=offset;i<other->getnumnodes()+offset;i++)
      {
	int j=i-offset;
	//	fprintf(stderr,"Copying node %d --> %d offset=%d \n",j,i);
	points[i].index=other->getpoint(j)->index+offset;
	int ia=0;
	for (ia=0;ia<=2;ia++)
	    {
	      points[i].x[ia] =other->getpoint(j)->x[ia];
	      points[i].nx[ia]=other->getpoint(j)->nx[ia];
	      pointDetails[i].dx[ia]=other->getdetails(j)->dx[ia];
	    }
	
	for (ia=0;ia<=1;ia++)
	    points[i].k[ia]=other->getpoint(j)->k[ia];
	
	pointDetails[i].index[0]=other->getindex(j,0);
	pointDetails[i].index[1]=other->getindex(j,1);
	pointDetails[i].temperature=other->getdetails(j)->temperature;
      }
}
// ************************************************
void PXSolidNodes::setfromnodecollection(PXNodeCollection* oldnodes,int scale)
{
  if (scale==0)
      scale=-1;
  cleanup();
  init(oldnodes->numnodes);
  for (int i=0;i<numnodes;i++)
      {
	points[i].index=i;
	int ia=0;
	for (ia=0;ia<=2;ia++)
	    {
	      points[i].x[ia]=oldnodes->x[i].x[ia];
	      points[i].nx[ia]=sqrt(1.0/3.0);
	      pointDetails[i].dx[ia]=oldnodes->dx[i].x[ia];
	    }
	
	for (ia=0;ia<=1;ia++)
	    points[i].k[ia]=oldnodes->k[i].x[ia];

	pointDetails[i].index[1]=oldnodes->pointno[i];
	if (scale==-1)
	    {
	      pointDetails[i].index[0]=oldnodes->stackno[i];
	    }
	else
	    {
	      pointDetails[i].index[0]=(100*oldnodes->stackno[i]/scale);
	      if  (pointDetails[i].index[0]!=0 && pointDetails[i].index[0]!=100)
		  pointDetails[i].index[1]=-1;
	    }

	pointDetails[i].temperature=0.0;
      }
}
// --------------------------------------------------------------------------
// Displacement Related 
// --------------------
PXNodalDisplacements::PXNodalDisplacements(PXSolidNodes* nodes,int endo,int epi)
{
  init(nodes->getnumnodes());
  endoindex=endo;
  epiindex=epi;
  solidNodes=nodes;
}

PXNodalDisplacements::PXNodalDisplacements(PXNodalDisplacements *other)
{
  init(other->getsolidnodes()->getnumnodes());
  solidNodes=other->getsolidnodes();
  Copy(other);
}
// --------------------------------------------------------------------------
void PXNodalDisplacements::init(int n)
{
  //fprintf(stderr,"Nodal Displacements :: Number of Nodes = %d\n",n);
  disp=new PX3Vector[n];
  conf=new float[n];
  valid=new int[n];
  
  for (int i=0;i<n;i++)
      {
	valid[i]=0;
	disp[i].setvector(0.0,0.0,0.0);
	conf[i]=1.0;
      }
}
// --------------------------------------------------------------------------
PXNodalDisplacements::~PXNodalDisplacements()
{
  cleanup();
}
// --------------------------------------------------------------------------
void PXNodalDisplacements::cleanup()
{
  delete [] disp;
  delete [] conf;
  delete [] valid;
}
// --------------------------------------------------------------------------
int PXNodalDisplacements::loadDisplacementsSingle(const char* filename,int all)
{
  fprintf(stderr,"Reading Displacements from %s \n",filename);
  fprintf(stderr,"--------------------------------------------\n");
  
  PXAbaqusOutputData* outputStructures=new PXAbaqusOutputData(solidNodes->getnumnodes(),1);
  int ok=outputStructures->loadNodeDetails(filename);
  if (ok==0)
      return 0;
  
  int count=0;
  for (int i=0;i<solidNodes->getnumnodes();i++)
      {
	if (all==1 || solidNodes->getindex(i,0)==endoindex || solidNodes->getindex(i,0)==epiindex)
	    {
	      for (int ia=0;ia<=2;ia++)
		  disp[i].x[ia]=outputStructures->getdispcomponent(i,ia);
	      conf[i]=1.0;
	      valid[i]=1;
	      count++;
	    }
      }
  fprintf(stderr,"Num Displacements= %d\n",count);
  delete outputStructures;
  return count;
}
// --------------------------------------------------------------------------
int PXNodalDisplacements::\
setfromshapetrackingclassnosearch(PXShapeTrackingClass* shapeTr,int index0)
{
  if (!shapeTr->hasdisplacements())
      return 0;

  int ng=0;
  for (int i=0;i<solidNodes->getnumnodes();i++)
      {
	if (solidNodes->getindex(i,0)==index0)
	    {
	      ++ng;
	      int index=solidNodes->getindex(i,1);
	      for (int ia=0;ia<=2;ia++)
		  disp[i].x[ia]=shapeTr->getdisplacement(index)->x[ia];
	      setconfidence(i,shapeTr->getconfidence(index));
	      if (i==0)
		  {
		    fprintf(stderr,"setfromshns:: disp=(%5.3f %5.3f %5.3f) conf=%5.3f --->",
			    disp[i].x[0],disp[i].x[1],disp[i].x[2],conf[i]);
		    PXTriangulatedStackPoint* pt2=shapeTr->getstack(1)->getpoint(shapeTr->getcorrespondence(i));
		    fprintf(stderr," (%6.2f %6.2f %6.2f)\n",pt2->x[0],pt2->x[1],pt2->x[2]);
		  }
	      valid[i]=1;
	    }
      }
  return ng;
}
// --------------------------------------------------------------------------
int PXNodalDisplacements::\
setfromshapetrackingclass(PXShapeTrackingClass* shapeTr,int index,float tolerance)
{
  if (!shapeTr->hasdisplacements() || !shapeTr->hasstack(0))
      return 0;
  
  int ng=0;
  PXTriangulatedStack* tstack=shapeTr->getstack(0);
  //  PXIntegerList* searchPoints=new PXIntegerList();
  for (int i=0;i<solidNodes->getnumnodes();i++)
      {
	if (solidNodes->getindex(i,0)==index)
	    {
	      ++ng;
	      float x[3],xp[3];
	      PXNodeAuxiliary* details=solidNodes->getdetails(i);
	      int ia=0;
	      for (ia=0;ia<=2;ia++)
		  {
		    x[ia]=solidNodes->getpoint(i)->x[ia]+details->dx[ia];
		    xp[ia]=x[ia];
		  }
	      
	      if (i==0 || i==200)
		  fprintf(stderr,"setfromsh : Point 0 (%6.3f %6.3f %6.3f) + (%5.3f %5.3f %5.3f) ="
			  "(%6.3f %6.3f %6.3f)\n\t\t",
			  solidNodes->getpoint(i)->x[0],
			  solidNodes->getpoint(i)->x[1],
			  solidNodes->getpoint(i)->x[2],
			  details->dx[0],details->dx[1],details->dx[2],
			  x[0],x[1],x[2]);
			  
			  
	      int corr=0;
	      tstack->findnearestpoint(x[0],x[1],x[2],corr,0,tolerance);
	      
	      float mindist[3];
	      int   pt[3];
	      for (ia=0;ia<=1;ia++)
		  {
		    mindist[ia]=10000000.0;
		    pt[ia]=-1;
		  }
	      
	      mindist[2]=Fdist(x[0],x[1],x[2],xp[0],xp[1],xp[2]);
	      pt[2]=corr;
	      
	      PXIntegerList* nei=tstack->getneighbours(corr);
	      nei->Rewind();
	      for (int ic=0;ic<nei->getnumnodes();ic++)
		  {
		    int p=nei->getcurrent();
		    float dist=Fdist(xp[0],xp[1],xp[2],
				     tstack->getpoint(p)->x[0],
				     tstack->getpoint(p)->x[1],
				     tstack->getpoint(p)->x[2]);

		    if (dist<mindist[0])
			{
			  mindist[1]=mindist[0];
			  pt[1]=pt[0];
			  mindist[0]=dist;
			  pt[0]=p;
			}
		    else
			{
			  if (dist<mindist[1])
			      {
				mindist[1]=dist;
				pt[1]=p;
			      }
			}
		    nei->Next();
		  }
	      
	      for (ia=0;ia<=1;ia++)
		  {
		    if (pt[ia]==-1)
			{
			  pt[ia]=corr;
			  mindist[ia]=mindist[2];
			}
		  }
	      
	      
	      disp[i].setvector(0.0,0.0,0.0);
	      conf[i]=0.0;
	      
	      float sum=0.0;
	      for (ia=0;ia<=2;ia++)
		  {
		    mindist[ia]=1.0/Fmax(mindist[ia],0.01);
		    sum+=mindist[ia];
		  }
	      
	      for (ia=0;ia<=2;ia++)
		  {
		    mindist[ia]/=sum;
		    conf[i]+=mindist[ia]*shapeTr->getconfidence(pt[ia]);
		    for (int ib=0;ib<=2;ib++)
			disp[i].x[ib]+=mindist[ia]*shapeTr->getdisplacement(pt[ia])->x[ib];
		  }
	      
	      if (i==0 || i==200)
		  {
		    fprintf(stderr," ---> (%6.3f %6.3f %6.3f) , disp=(%5.3f %5.3f %5.3f) conf=%5.3f \n",
			    x[0],x[1],x[2],
			    disp[i].x[0],disp[i].x[1],disp[i].x[2],conf[i]);
		    fprintf(stderr," pt[0]=%d(%5.2f), pt[1]=%d(%5.2f) pt[2]=%d(%5.2f) \n",
			    pt[0],mindist[0],pt[1],mindist[1],pt[2],mindist[2]);
		  }
	      
	      /*int np2=tstack->collectNeighbours(corr,0,searchPoints);
	      
	      disp[i].setvector(0.0,0.0,0.0);
	      conf[i]=0.0;
	      float wgt=1.0/float(np2+1);
	      searchPoints->Rewind();
	      for (int j=0;j<np2;j++)
		  {
		    int ind=searchPoints->getcurrent();
		    searchPoints->Next();
		    float wgt2=wgt;
		    if (j==0)
			wgt2*=2.0;
		    for (int ia=0;ia<=2;ia++)
			disp[i].x[ia]=wgt2*shapeTr->getdisplacement(ind)->x[ia];
		    conf[i]+=wgt2*shapeTr->getconfidence(ind);
		  }

	      if (i==0)
		  fprintf(stderr," ---> (%6.3f %6.3f %6.3f) , disp=(%5.3f %5.3f %5.3f) conf=%5.3f np2=%d\n",
			  x[0],x[1],x[2],
			  disp[i].x[0],disp[i].x[1],disp[i].x[2],conf[i],np2);
			  */
	      valid[i]=1;
	    }
      }
  return ng;
}
// --------------------------------------------------------------------------
int PXNodalDisplacements::Save(const char* fname)
{
FILE* fout=fopen(fname,"w");
 if (fout)
     {
       int st=Save(fout);
       fclose(fout);
       return st;
     }
 return 0;
}
// --------------------------------------------------------------------------
int PXNodalDisplacements::Load(const char* fname)
{
 gzFile fin=gzsuffixopen(fname,"r");
 if (fin)
     {
       int st=Load(fin);
       gzclose(fin);
       return st;
     }
 return 0;
}
// --------------------------------------------------------------------------
int PXNodalDisplacements::Save(FILE* fout)
{
  fprintf(fout,"#PXNodalDisplacements File\n");
  fprintf(fout,"#Numnodes\n%d\n",solidNodes->getnumnodes());
  fprintf(fout,"#Endoindex\n%d\n#Epiindex\n%d\n",endoindex,epiindex);
  int numvalid=0;
  for (int ia=0;ia<solidNodes->getnumnodes();ia++)
      if (valid[ia])
	  numvalid++;
  
  fprintf(fout,"#Numdisplacements\n%d\n",numvalid);
  for (int i=0;i<solidNodes->getnumnodes();i++)
      {
	if (valid[i])
	    fprintf(fout,"%6d %6.3f %6.3f %6.3f  %5.3f \n",
		    i,disp[i].x[0],disp[i].x[1],disp[i].x[2],conf[i]);
      }
  return 1;
}
// --------------------------------------------------------------------------
int PXNodalDisplacements::Load(gzFile fin)
{
  char* line=new char[200];
  

  gzgets(fin,line,200);
  if (gzfindstringinstring(line,"#PXNodalDisplacements File")==0)
      {
	delete [] line;
	return 0;
      }
  gzgets(fin,line,200);
  int num=0;
  gzgets(fin,line,200);
  sscanf(line,"%d",&num);
  if (num!=solidNodes->getnumnodes())
      {
	fprintf(stderr,"Wrong number of nodes %d vs %d line=%s\n\n\n\n",num,solidNodes->getnumnodes(),line);
	delete [] line;
	return 0;
      }
  gzgets(fin,line,200);  gzgets(fin,line,200);
  sscanf(line,"%d",&endoindex);
  gzgets(fin,line,200);  gzgets(fin,line,200);
  sscanf(line,"%d\n",&epiindex);
  gzgets(fin,line,200);  gzgets(fin,line,200);
  int numvalid=0;
  sscanf(line,"%d\n",&numvalid);
  int index;


  for (int ia=0;ia<num;ia++)
      {
	valid[ia]=0; disp[ia].setvector(0.0,0.0,0.0); conf[ia]=1.0;
      }

  for (int i=0;i<numvalid;i++)
      {
	int tmp=0;
	gzgets(fin,line,200);
	sscanf(line,"%d ",&index);
	valid[index]=1;
	sscanf(line,"%d %f %f %f  %f",&tmp,
	       &disp[index].x[0],&disp[index].x[1],&disp[index].x[2],&conf[index]);
      }
  delete [] line;
  return 1;
}
// --------------------------------------------------------------------------
int PXNodalDisplacements::\
loadPengChengDisplacementsSingle(const char* endofile,const char* epifile,float tolerance)
{
  FILE *dispfile[2];
  const char* filename[2];
  
  filename[0]=endofile;
  filename[1]=epifile;
  
  fprintf(stderr,"Reading Peng-Cheng Displacements from %s,%s\n",filename[0],filename[1]);
  
  for (int ii=0;ii<=1;ii++)
      dispfile[ii]=fopen(filename[ii],"r");
  
  if (dispfile[0]==NULL || dispfile[1]==NULL)
      {
	for (int i=0;i<=1;i++)
	    {
	      if (dispfile[i]!=NULL)
		  fclose(dispfile[i]);
	    }
	return 0;
      }
  
  char line[200];
  float *pointsx[2],*pointsy[2],*pointsz[2];
  int numdisppoints[2],sur;
  
  // Count Lines in file
  // -------------------
  for (sur=0;sur<=1;sur++)
      {
	numdisppoints[sur]=0;
	
	while ( fgets(line,100,dispfile[sur])!=NULL ) 
	    numdisppoints[sur]++;
	fclose(dispfile[sur]);
	dispfile[sur]=fopen(filename[sur],"r");
	fprintf(stderr,"Number of points in %s = %d\n",filename[sur],numdisppoints[sur]);
      }
  
  
  
  // Allocate Data and Read Rest of Files 
  // ------------------------------------
  for (sur=0;sur<=1;sur++)
      {
	pointsx[sur]=new float[numdisppoints[sur]*2];
	pointsy[sur]=new float[numdisppoints[sur]*2];
	pointsz[sur]=new float[numdisppoints[sur]*2];
	for (int i=0;i<numdisppoints[sur];i++)
	    {
	      for (int j=0;j<2;j++)
		  {
		    int index=i*2+j;
		    fscanf(dispfile[sur],"%f %f %f ",
			   &pointsx[sur][index],
			   &pointsy[sur][index],
			   &pointsz[sur][index]);
		  }
	    }
	
	fprintf(stderr,"\tStack %d, Points=%d\n",sur+1,numdisppoints[sur]);
	fclose(dispfile[sur]);
      } 
  
  // Create Structures for Solid to find and store displacements
  // -----------------------------------------------------------
  int numd[2],numtest=0;
  numd[0]=0,numd[1]=0;
  int numdisplacements=0;
  tolerance*=tolerance;
  
  for (int i=0;i<solidNodes->getnumnodes();i++)
      {
	int sur=0;
	if (solidNodes->getindex(i,0)==endoindex || solidNodes->getindex(i,0)==epiindex)
	    {
	      numtest++;
	      if (solidNodes->getindex(i,0)==epiindex)
		  sur=1;
	      int pt=0,found=0;
	      int bestpt=0;
	      float mindist=10000.0;
	      while(pt<numdisppoints[sur] && found==0)
		  {
		    int po_index=pt*2;
		    float x=pointsx[sur][po_index];
		    float y=pointsy[sur][po_index];
		    float z=pointsz[sur][po_index];
		    float dist=Fsqdist(x,y,z,
				       solidNodes->getpoint(i)->x[0]+solidNodes->getdetails(i)->dx[0],
				       solidNodes->getpoint(i)->x[1]+solidNodes->getdetails(i)->dx[1],
				       solidNodes->getpoint(i)->x[2]+solidNodes->getdetails(i)->dx[2]);
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
	      int po_index=bestpt*2;			
	      numdisplacements++;
	      numd[sur]++;
	      disp[i].setvector(pointsx[sur][po_index+1]-pointsx[sur][po_index],
				pointsy[sur][po_index+1]-pointsy[sur][po_index],
				pointsz[sur][po_index+1]-pointsz[sur][po_index]);
	      conf[i]=1.0;
	      valid[i]=1;
	      
	      
	      if (i==10 || i==200)
		  {
		    fprintf(stderr,"Original Node %d \n (%6.3f %6.3f %6.3f ) ",i,
			    solidNodes->getpoint(i)->x[0],
			    solidNodes->getpoint(i)->x[1],
			    solidNodes->getpoint(i)->x[2]);
		    fprintf(stderr," +dx (%6.3f %6.3f %6.3f ) =",
			    solidNodes->getdetails(i)->dx[0],
			    solidNodes->getdetails(i)->dx[1],
			    solidNodes->getdetails(i)->dx[2]);
		    fprintf(stderr," (%6.3f %6.3f %6.3f ) \n",
			    solidNodes->getdetails(i)->dx[0]+solidNodes->getpoint(i)->x[0],
			    solidNodes->getdetails(i)->dx[1]+solidNodes->getpoint(i)->x[1],
			    solidNodes->getdetails(i)->dx[2]+solidNodes->getpoint(i)->x[2]);
		    fprintf(stderr,"Closest Point %d \n (%6.3f %6.3f %6.3f ) -->",bestpt,
			    pointsx[sur][po_index],
			    pointsy[sur][po_index],
			    pointsz[sur][po_index]);
		    fprintf(stderr,"(%d) (%6.3f %6.3f %6.3f ) \n",bestpt,
			    pointsx[sur][po_index+1],
			    pointsy[sur][po_index+1],
			    pointsz[sur][po_index+1]);
		    fprintf(stderr,"Displacement = (%5.3f %5.3f %5.3f)\n",
			    disp[i].x[0], disp[i].x[1], disp[i].x[2]);
		  }
	    }
      }
  
  fprintf(stderr,"\tDone with surfaces, found %d,%d displacements (%d)\n",numd[0],numd[1],numtest);
  
  delete [] pointsx[0]; delete [] pointsx[1]; delete [] pointsy[0];
  delete [] pointsy[1]; delete [] pointsz[0]; delete [] pointsz[1];
  return numdisplacements;
}
// --------------------------------------------------------------------------
void PXNodalDisplacements::Copy(PXNodalDisplacements *other)
{
  if (other->getsolidnodes()->getnumnodes() !=solidNodes->getnumnodes())
      {
	cleanup();
	init(other->getsolidnodes()->getnumnodes());
      }
  solidNodes=other->getsolidnodes(); 
  endoindex=other->getendoindex(); epiindex=other->getepiindex();
  
  for (int i=0;i<solidNodes->getnumnodes();i++)
      {
	valid[i]=other->getvalid(i);
	disp[i].copy(other->getdisplacement(i));
	conf[i]=(other->getconfidence(i));
      }
}
// --------------------------------------------------------------------------
PX3Vector* PXNodalDisplacements::getdisplacement(int i)
{
  i=Irange(i,0,solidNodes->getnumnodes());
  return &disp[i];
}

float PXNodalDisplacements::getdispcomponent(int i,int j)
{
  i=Irange(i,0,solidNodes->getnumnodes());
  j=Irange(j,0,2);
  return disp[i].x[j];
}

float PXNodalDisplacements::getconfidence(int i)
{
  i=Irange(i,0,solidNodes->getnumnodes());
  return conf[i];
}

void PXNodalDisplacements::setdisplacement(int i,float x,float y,float z)
{
  i=Irange(i,0,solidNodes->getnumnodes());
  disp[i].setvector(x,y,z);
  valid[i]=1;
}

void PXNodalDisplacements::setdispcomponent(int i,int j,float x)
{
  i=Irange(i,0,solidNodes->getnumnodes());
  j=Irange(j,0,2);
  disp[i].x[j]=x;
}

void PXNodalDisplacements::setdisplacement(int i,PX3Vector* vec)
{
  i=Irange(i,0,solidNodes->getnumnodes());
  disp[i].copy(vec);
  valid[i]=1;
}
 
void PXNodalDisplacements::setconfidence(int i,float cnf)
{
  i=Irange(i,0,solidNodes->getnumnodes());
  conf[i]=cnf;
}

int PXNodalDisplacements::getvalid(int i)
{
  i=Irange(i,0,solidNodes->getnumnodes());
  return valid[i];
}

int PXNodalDisplacements::getendoindex()
{
  return endoindex;
}

int PXNodalDisplacements::getepiindex()
{
  return epiindex;
}

PXSolidNodes* PXNodalDisplacements::getsolidnodes()
{
  return solidNodes;
}
// --------------------------------------------------------------------------
/*
 Definition of PXAbaqusMaterialModel
 */
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
PXAbaqusMaterialModel::PXAbaqusMaterialModel(int mode)
{
  init(mode);
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
PXAbaqusMaterialModel::PXAbaqusMaterialModel(PXAbaqusMaterialModel* old)
{
  init(old->getmodeltype());
  Copy(old);
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
PXAbaqusMaterialModel::~PXAbaqusMaterialModel()
{
  cleanup();
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
void PXAbaqusMaterialModel::init(int mode)
{
 modeltype=Irange(mode,0,2);
 switch(modeltype)
     {
     case 0:
       numparameters=2; break;
     case 1:
       numparameters=9; break;
     case 2:
       numparameters=2; break;
     }
 active=0;
 parameter=new float[numparameters];
 setgeometry(1);
 setheatcoeff();
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
void PXAbaqusMaterialModel::cleanup()
{
  numparameters=0;
  delete [] parameter;
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
void PXAbaqusMaterialModel::setgeometry(int nl)
{
  nlgeom=nl>0;
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
int PXAbaqusMaterialModel::getgeometry()
{
  return nlgeom;
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
void PXAbaqusMaterialModel::setactive(int av)
{
  active=av>0;
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
int PXAbaqusMaterialModel::getactive()
{
  return active;
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_ 
void PXAbaqusMaterialModel::setHyperelastic(float incompressibility)
{
  if (numparameters!=2)
      {
	cleanup();
	init(2);
      }
  else
      modeltype=2;
  
  parameter[0]=1.0;
  parameter[1]=incompressibility;
  setgeometry(1);
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
void PXAbaqusMaterialModel::setElastic(float Modulus,float Poisson)
{
  if (numparameters!=2)
      {
	cleanup();
	init(0);
      }
  else 
      modeltype=0;
  parameter[0]=Modulus;
  parameter[1]=Poisson;
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
void PXAbaqusMaterialModel::setTransverseIsotropy(float Ep,float vp,float Et,float vtp)
{
  if (numparameters!=9)
      {
	cleanup();
	init(1);
      }
  else 
      modeltype=1;
  
  parameter[0]=Ep;
  parameter[1]=Ep;
  parameter[2]=Et;
  parameter[3]=vp;
  parameter[4]=vtp;
  parameter[5]=vtp*(Ep/Et);;
  parameter[6]=Ep/(2.0*(1+vp));
  parameter[7]=Et/(2.0*(1+vp));
  parameter[8]=Et/(2.0*(1+vp));
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
int PXAbaqusMaterialModel::getmodeltype()
{
  return modeltype;
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
float PXAbaqusMaterialModel::getparameter(int i)
{
  return parameter[Irange(i,0,numparameters-1)];
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
void PXAbaqusMaterialModel::setheatcoeff(float a,float b,float c)
{
  heatcoeff[0]=a;
  heatcoeff[1]=b;
  heatcoeff[2]=c;

}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
float PXAbaqusMaterialModel::getheatcoeff(int i)
{
  return heatcoeff[Irange(i,0,2)];
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
void PXAbaqusMaterialModel::Copy(PXAbaqusMaterialModel* other)
{
  if (other->getmodeltype() !=modeltype)
      {
	cleanup();
	init(other->getmodeltype());
      }
  
  setgeometry(other->getgeometry());
  for (int i=0;i<numparameters;i++)
      parameter[i]=other->getparameter(i);
  for (int j=0;j<=2;j++)
      heatcoeff[j]=other->getheatcoeff(j);
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
void PXAbaqusMaterialModel::setfrommaterialmodel(PXMaterialModel* old)
{
  cleanup();
  init(old->modeltype);
  setgeometry(old->nlgeom);
  for (int i=0;i<numparameters;i++)
      parameter[i]=old->parameter[i];
  setheatcoeff();
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
void PXAbaqusMaterialModel::SaveAbaqus(FILE* fout,const char* name)
{
  switch(modeltype)
      {
      case 0:
	fprintf(fout,"*MATERIAL,NAME=%s\n*ELASTIC\n",name);
	fprintf(fout,"%8.2f,%8.2f\n",parameter[0],parameter[1]);
	break;
      case 1:
	fprintf(fout,"*MATERIAL,NAME=TISSUE\n*ELASTIC,TYPE=ENGINEERING CONSTANTS\n");
	{
	  for (int i=0;i<=8;i++)
	      {
		fprintf(fout,"%8.2f",parameter[i]);
		if (i<7)
		    fprintf(fout,",");
		else
		    fprintf(fout,"\n");
	      }
	}
	break;
      case 2:
	fprintf(fout,"*MATERIAL,NAME=TISSUE\n*HYPERELASTIC,N=1,OGDEN\n");
	fprintf(fout,"%7.3f,2,%7.3f\n",parameter[0],parameter[1]);
	break;
      }
  if (active)
      {
	//fprintf(fout,"*EXPANSION,TYPE=ORTHO\n%7.3f,%7.3f,%7.3f\n",heatcoeff[0],heatcoeff[1],heatcoeff[2]);
	fprintf(fout,"*EXPANSION,TYPE=ANISO,USER,ZERO=0.0\n");
      }
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
int PXAbaqusMaterialModel::Save(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (fout)
      {
	int st=Save(fout);
	fclose(fout);
	return st;
      }
  return 0;
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
int PXAbaqusMaterialModel::Load(const char* fname)
{
 gzFile fin=gzsuffixopen(fname,"r");
 if (fin)
     {
       int st=Load(fin);
       gzclose(fin);
       return st;
     }
 return 0;
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
int PXAbaqusMaterialModel::Save(FILE* fout)
{
  fprintf(fout,"#AbaqusMaterialModel\n");
  fprintf(fout,"%d\n",modeltype);
  fprintf(fout,"%d\n",nlgeom);
  fprintf(fout,"%d\n",active);
  for (int i=0;i<numparameters;i++)
      fprintf(fout,"%7.3f ",parameter[i]);
  fprintf(fout,"\n");
  active=active>0;
  if (active==1)
      {
	for (int i=0;i<3;i++)
	    fprintf(fout,"%7.3f ",heatcoeff[i]);
	fprintf(fout,"\n");
      }
  return 1;
}
// _-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-__-_-_
int PXAbaqusMaterialModel::Load(gzFile fin)
{
  char* line=new char[400];
  gzgets(fin,line,200);
  if (gzfindstringinstring(line,"#AbaqusMaterialModel")==0)
      {
	delete [] line;
	return 0;
      }
  int temp;
  gzgets(fin,line,200);
  sscanf(line,"%d",&temp);
  cleanup();
  init(temp);
  gzgets(fin,line,200);
  sscanf(line,"%d",&nlgeom);
  gzgets(fin,line,200);
  sscanf(line,"%d",&active);
  gzgets(fin,line,400);
  int po=0;
  
  for (int i=0;i<numparameters;i++)
      {
	po=stringskipspace(line,po);
	sscanf(&line[po],"%f ",&parameter[i]);
	po=stringskipnumber(line,po);
      }
  if (active)
      {
	gzgets(fin,line,400);
	po=0;
	for (int i=0;i<3;i++)
	    {
	      po=stringskipspace(line,po);
	      sscanf(&line[po],"%f",&heatcoeff[i]);
	      po=stringskipnumber(line,po);
	    }
      }

  delete [] line;
  return 1;
}
// _+_-__+_-__+_-__+_-__+_-__+_-__+_-__+_-__+_-__+_-__+_-__+_-__+_-__+_-_
/*
 Definition of PXAbaqusOutputData

 */
// ----------------------------------------
PXAbaqusOutputData::PXAbaqusOutputData(int numn,int numel)
{
  init(numn,numel);
}
// ----------------------------------------
PXAbaqusOutputData::PXAbaqusOutputData(PXAbaqusOutputData* other)
{
  init(other->getnumnodes(),other->getnumelements());
  Copy(other);
}
// ----------------------------------------
PXAbaqusOutputData::~PXAbaqusOutputData()
{
  cleanup();
}
// ----------------------------------------
void PXAbaqusOutputData::init(int numno,int numel)
{
  numnodes=numno;
  numelements=numel;

  for (int ia=0;ia<=11;ia++)
      {
	strain[ia]=new float[numelements];
	for (int i=0;i<numelements;i++)
	    strain[ia][i]=0.0;
      }
  
  elementVolumes=new float[numelements];
  elementCentroids=new PX3Vector[numelements];
  nodalDisplacements=new PX3Vector[numnodes];
  radial=new PX3Vector[numnodes];
  circum=new PX3Vector[numnodes];
  for (int i=0;i<numnodes;i++)
      {
	radial[i].setvector(1.0,0.0,0.0);
	circum[i].setvector(0.0,1.0,0.0);
      }

  numpies=1; 
  numsectors=1; 
  numslices=1;
  numtransmural=1;
  junctionpoint.x[0]=0.0; 
  junctionpoint.x[1]=0.0; 
  junctionpoint.x[2]=0.0;
  pieindex=new int[numelements];
  {
    for (int i=0;i<numelements;i++)
	pieindex[i]=0;
  }
}
// ----------------------------------------
void PXAbaqusOutputData::cleanup()
{
 for (int ia=0;ia<=11;ia++)
     delete [] strain[ia];
 
 delete [] elementVolumes;
 delete [] elementCentroids;
 delete [] nodalDisplacements;
 delete [] pieindex;
 delete [] radial;
 delete [] circum;
}
// ----------------------------------------
float PXAbaqusOutputData::getvolume(int el)
{
  el=Irange(el,0,numelements-1);
  return elementVolumes[el];
}
// ----------------------------------------
float PXAbaqusOutputData::getstraincomponent(int el,int stra)
{
  el=Irange(el,0,numelements-1);
  stra=Irange(stra,0,11);
  return strain[stra][el];
}
// ----------------------------------------
void PXAbaqusOutputData::setstraincomponent(int el,int stra,float v)
{
  el=Irange(el,0,numelements-1);
  stra=Irange(stra,0,11);
  strain[stra][el]=v;
}
// ----------------------------------------
PX3Vector* PXAbaqusOutputData::getdisplacement(int nd)
{
  nd=Irange(nd,0,numnodes-1);
  return &nodalDisplacements[nd];
}
// ----------------------------------------
float PXAbaqusOutputData::getdispcomponent(int nd,int comp)
{
 nd=Irange(nd,0,numnodes-1);
 comp=Irange(comp,0,2);
 return nodalDisplacements[nd].x[comp];
}
// ----------------------------------------
PX3Vector* PXAbaqusOutputData::getcentroid(int el)
{
 el=Irange(el,0,numelements-1);
 return &elementCentroids[el];
}
// ----------------------------------------
PX3Vector* PXAbaqusOutputData::getradial(int el)
{
 el=Irange(el,0,numelements-1);
 return &radial[el];

}
// ----------------------------------------

PX3Vector* PXAbaqusOutputData::getcircumeferential(int el)
{
 el=Irange(el,0,numelements-1);
 return &circum[el];
}
// ----------------------------------------
int PXAbaqusOutputData::getpieindex(int el)
{
 el=Irange(el,0,numelements-1);
 return pieindex[el];
}
// ----------------------------------------
int PXAbaqusOutputData::getnumpies()
{
 return numpies;
}
// ----------------------------------------

int PXAbaqusOutputData::getnumsectors()
{
 return numsectors;
}
// ----------------------------------------

int PXAbaqusOutputData::getnumslices()
{
 return numslices;
}
// ----------------------------------------
int PXAbaqusOutputData::getnumtransmural()
{
 return numtransmural;
}
// ----------------------------------------
PX3Vector* PXAbaqusOutputData::getjunctionpoint()
{
 return &junctionpoint;
}
// ----------------------------------------
void PXAbaqusOutputData::converttoprincipal()
{

  double** a1    = dmatrix(0,2, 0,2);
  double*  d1    = dvector(0,2);
  double** v1    = dmatrix(0,2, 0,2);
  int nrot=0;

  

  
  for (int ia=0;ia<numelements;ia++)
      {
	a1[1][1] = strain[0][ia]; a1[1][2] = strain[3][ia]; a1[1][3] = strain[4][ia]; 
	a1[2][1] = strain[3][ia]; a1[2][2] = strain[1][ia]; a1[2][3] = strain[5][ia]; 
	a1[3][3] = strain[4][ia]; a1[3][2] = strain[5][ia]; a1[3][3] = strain[2][ia];
	/*get_eigenvalues_vectors(a1, 3, d1, v1, &nrot);
	  sort_eigenvalues_vectors(d1, v1, 3);*/
	vtkMath_Jacobi(a1,d1,v1);
	strain[0][ia]=d1[0];
	strain[1][ia]=d1[1];
	strain[2][ia]=d1[2];
	strain[3][ia]=0.0;
	strain[4][ia]=0.0;
	strain[5][ia]=0.0;
      }
  free_dmatrix(v1,0,2,0,2);  
  free_dmatrix(a1,0,2,0,2);
  free_dvector(d1,0,2);    

}
// ----------------------------------------
int PXAbaqusOutputData::subdivideinpies(float junctionx,float junctiony,float junctionz,
					int numsl,int numse,int anticlockwise,int nump)
{
  if (numslices==-1)
      numslices=1;
  
  anticlockwise=anticlockwise>0;
  junctionpoint.setvector(junctionx,junctiony,junctionz);
  
  numsectors=Irange(numse,1,32);
  numslices=Irange(numsl,1,32);

  if (nump<numsectors*numslices)
      numpies=numsectors*numslices;
  else
      numpies=nump;
  
  fprintf(stderr,"Subviding numslices=%d, numsectors=%d numpies=%d\n",numslices,numsectors,numpies);
  
  PX3Vector ce;
  float minz=10000.0,maxz=-10000.0,vol=0.0;
  for (int i=0;i<numelements;i++)
      {
	maxz=Fmax(maxz,elementCentroids[i].x[2]);
	minz=Fmin(minz,elementCentroids[i].x[2]);
	for (int ia=0;ia<=2;ia++)
	    ce.x[ia]+=elementCentroids[i].x[ia]*elementVolumes[i];
	vol+=elementVolumes[i];
      }
  for (int ia=0;ia<=2;ia++)
      ce.x[ia]/=vol;
 

  /*fprintf(stderr,"ElementCentroids = %5.2f %5.2f %5.2f Vol=%5.2f\n",
    ce.x[0],ce.x[1],ce.x[2],vol);*/
  
  float dv=0.5*pow(double(vol)/double(numelements),double(1.0/3.0));
  minz-=dv;
  maxz+=dv;
  
  junctionx-=ce.x[0]; junctiony-=ce.x[1]; junctionz-=ce.x[2];
  
  float phase_ref=atan2(junctiony,junctionx)*180/M_PI;
  fprintf(stderr,"phase_ref=%6.3f (cx=%6.2f,cy=%5.2f)\n",phase_ref,ce.x[0],ce.x[1]);

  //float angleint=360.0/float(numsectors);
  
  float dz=1.0/float(numslices);
  float zr=maxz-minz;
  
  for (int i=0;i<numelements;i++)
      {
	// Slice no
	float frac=Frange((elementCentroids[i].x[2]-minz)/(zr),0,0.99);
	
	int sliceno=int(frac/dz);
	
	// Sector no
	float cx=elementCentroids[i].x[0]-ce.x[0];
	float cy=elementCentroids[i].x[1]-ce.x[1];
	float phase=atan2(cy,cx)*180/M_PI;

	
	/*float dphase=Fcyclic(phase-phase_ref,360.0);
	
	int sectorno=int(dphase/angleint);
	if (anticlockwise==0)
	    sectorno=(numsectors-1)-sectorno; // i.e. if 8 sectors 0->7 7->0 etc */
	int sectorno=getsectorno(phase,phase_ref,numsectors,anticlockwise);
	pieindex[i]=sectorno+sliceno*numsectors;
	pieindex[i]=Irange(pieindex[i],0,numslices*numsectors-1);
	if (pieindex[i]>numsectors*numslices)
	    {
	      fprintf(stderr,"centroid = %5.1f %5.1f %5.1f\n",elementCentroids[i].x[0],
		      elementCentroids[i].x[1],elementCentroids[i].x[2]);
	      fprintf(stderr,"junction = %5.1f %5.1f %5.1f\n",junctionx,junctiony,0.0);
	      //fprintf(stderr,"phase=%5.1f phase_ref=%5.1f dphase=%5.1f \n",phase,phase_ref,dphase);
	      fprintf(stderr,"minz=%5.1f zr=%5.1f dz=%5.1f ",minz,zr,dz);		 
	      fprintf(stderr,"sector =%d sliceno=%d, coords[i]=%d\n",sectorno,sliceno,pieindex[i]);
	    }
      }
  return 1;
}
// ----------------------------------------
int PXAbaqusOutputData::averagestrains(PXAbaqusOutputData* ref)
{
  //fprintf(stderr,"In average strains numpies=%d\n",numpies);
  /*{
    for (int i=0;i<numelements;i++)
	fprintf(stderr,"%d ",pieindex[i]);
    fprintf(stderr,"\n");
  }*/

  float *ave=new float[numpies+1];
  float *vol0=new float[numpies+1];

  for (int index=0;index<=5;index++)
      {
	//fprintf(stderr,"Index =%d\n",index);
	for (int j=0;j<numpies;j++)
	  {
	    ave[j]=0.0;
	    vol0[j]=0.0;
	  }
	
	int i;
	for (i=0;i<numelements;i++)
	  {
	    int p=Irange(pieindex[i],0,numpies-1);
	    vol0[p]+=ref->getvolume(i);
	    //vol[p]+=elementVolumes[i];
	    ave[p]+=strain[index][i]*ref->getvolume(i);//elementVolumes[i];
	  }
	for (i=0;i<numpies;i++)
	  {
	    if (vol0[i]<0.001)
	      ave[i]=0.0;
	    else
	      ave[i]/=vol0[i];
	  }
	
	for (i=0;i<numelements;i++)
	  {
	    strain[index+6][i]=ave[Irange(pieindex[i],0,numpies-1)];
	  }
      }
  
  delete [] ave;
  delete [] vol0;
  return 1;
}

// ----------------------------------------
int PXAbaqusOutputData::LoadOld(const char* fnamestem,int rotate)
{
 char dispname[200],elname[200],strainname[200];
 int nd=numnodes;
 int numel=numelements;

 sprintf(dispname,"%s.disp",fnamestem);
 sprintf(elname,"%s.element",fnamestem);
 sprintf(strainname,"%s.strain",fnamestem);

 FILE* fin;
 
 //fprintf(stderr,"Filenames = `%s' , `%s' , `%s', nel=%d, nd=%d\n",dispname,elname,strainname,numel,nd);

 fin=fopen(dispname,"r");
 if(fin!=NULL)
     {
       int temp;
       for (int ia=0;ia<nd;ia++)
	   {
	     fscanf(fin,"%d %f %f %f\n",&temp,
		    &nodalDisplacements[ia].x[0],
		    &nodalDisplacements[ia].x[1],
		    &nodalDisplacements[ia].x[2]);
	   }
       fclose(fin);
     }
 else
     {
       for (int ia=0;ia<nd;ia++)
	   {
	     for (int ib=0;ib<=2;ib++)
		 nodalDisplacements[ia].x[ib]=0.0;
	   }
     }
 
 fin=fopen(elname,"r");
 if (fin!=NULL)
     {
       int temp;
       for (int ia=0;ia<numel;ia++)
	 {
	   fscanf(fin,"%d %f %f %f %f %f %f %f %f %f %f\n",&temp,
		  &elementCentroids[ia].x[0],
		  &elementCentroids[ia].x[1],
		  &elementCentroids[ia].x[2],
		  &elementVolumes[ia],
		  &circum[ia].x[0],&circum[ia].x[1],&circum[ia].x[2],
		  &radial[ia].x[0],&radial[ia].x[1],&radial[ia].x[2]);
	   if (elementVolumes[ia]<0.01)
	     elementVolumes[ia]=0.01;
	 }
       fclose(fin);
     }
 else
     {
       for (int ia=0;ia<numel;ia++)
	   {
	     for (int ib=0;ib<=2;ib++)
		 elementCentroids[ia].x[ib]=0.0;
	     elementVolumes[ia]=0.01;
	   }
     }
 
 fin=fopen(strainname,"r");
 if (fin!=NULL)
     {
       int temp;
       for (int ia=0;ia<numel;ia++)
	   {
	     fscanf(fin,"%d %f %f %f %f %f %f\n",&temp,
		    &strain[0][ia],
		    &strain[1][ia],
		    &strain[2][ia],
		    &strain[3][ia],
		    &strain[4][ia],
		    &strain[5][ia]);
	   }
       fclose(fin);
     }
 
 if (rotate)
     rotatestrains();
 else
     {
       for (int ia=0;ia<numel;ia++)
	   for (int ib=0;ib<=11;ib++)
	       strain[ib][ia]=0.0;
     }
 
 return 1;
}
// --------------------------------------------------------------------------
int PXAbaqusOutputData::rotatestrains()
{
  fprintf(stderr," (rot)");
  double** R	 = dmatrix(0, 2, 0, 2);
  double** Rt	 = dmatrix(0, 2, 0, 2);
  double** E	 = dmatrix(0, 2, 0, 2);
  double** E1	 = dmatrix(0, 2, 0, 2);
  double** E2	 = dmatrix(0, 2, 0, 2);
  
  PX3Vector longit;
  
  for (int ia=0;ia<numelements;ia++)
      {
	E[0][0] = strain[0][ia]; E[0][1] = strain[3][ia]; E[0][2] = strain[4][ia]; 
	E[1][0] = strain[3][ia]; E[1][1] = strain[1][ia]; E[1][2] = strain[5][ia]; 
	E[2][2] = strain[4][ia]; E[2][1] = strain[5][ia]; E[2][2] = strain[2][ia];
	
	R[0][0] = radial[ia].x[0]; R[0][1] = radial[ia].x[1]; R[0][2] = radial[ia].x[2]; 
	R[1][0] = circum[ia].x[0]; R[1][1] = circum[ia].x[1]; R[1][2] = circum[ia].x[2]; 	 	 

	longit.copy(&radial[ia]);
	longit.crosswith(&circum[ia]);

	R[2][0] = longit.x[0]; R[2][1] = longit.x[1]; R[2][2] = longit.x[2];
	
	matrix_transpose(R, Rt, 3, 3);
	matrix_product(E1, R, 3, 3, E, 3, 3);
	matrix_product(E2, E1, 3, 3, Rt, 3, 3);
	
	strain[0][ia] = E2[0][0]; strain[3][ia] = E2[0][1]; strain[4][ia] = E2[0][2];
	strain[1][ia] = E2[1][1]; strain[5][ia] = E2[1][2]; strain[2][ia] = E2[2][2];
	
	for (int ib=0;ib<=5;ib++)
	    strain[ib+6][ia]=strain[ib][ia];
	
	setRadial(ia,1.0,0.0,0.0);
	setCircum(ia,0.0,1.0,0.0);
      }
  free_dmatrix(R,0,2,0,2); free_dmatrix(Rt,0,2,0,2);
  free_dmatrix(E,0,2,0,2); free_dmatrix(E1,0,2,0,2); free_dmatrix(E2,0,2,0,2);
  
  return 1;
}
// --------------------------------------------------------------------------
int PXAbaqusOutputData::getnumelements()
{
  return numelements;
}
// --------------------------------------------------------------------------
int PXAbaqusOutputData::getnumnodes()
{
 return numnodes;
}
// --------------------------------------------------------------------------
int PXAbaqusOutputData::copypieindex(PXAbaqusOutputData* other)
{
 if (numelements!=other->getnumelements())
     return 0;

 numpies=other->getnumpies();
 numsectors=other->getnumsectors();
 numslices=other->getnumslices();
 numtransmural=other->getnumtransmural();
 junctionpoint.copy(other->getjunctionpoint());

 for (int i=0;i<numelements;i++)
     pieindex[i]=Irange(other->getpieindex(i),0,numpies-1);
 return 1;
}
// --------------------------------------------------------------------------
void PXAbaqusOutputData::setOutputStepInc(int os,int oi)
{
 outputStepNo=os; 
 outputIncNo=oi;
}
// ------------------------------------------------------------------------ 
int PXAbaqusOutputData::getoutputstep()
{
 return outputStepNo;
}

int PXAbaqusOutputData::getoutputinc()
{
 return outputIncNo;
}
// ------------------------------------------------------------------------ 
int PXAbaqusOutputData::Save(const char* fname,int nodirections)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
     return 0;
  
 if (nodirections)
     fprintf(fout,"#PXAbaqusOutputDataNoDirections\n");
 else
     fprintf(fout,"#PXAbaqusOutputData\n");
 fprintf(fout,"Numnodes\n%d\n",numnodes);
 fprintf(fout,"Numelements\n%d\n",numelements);
 fprintf(fout,"Pie stats\n%d %d %d\n",numpies,numslices,numsectors);
 fprintf(fout,"Junction Point\n%6.2f %6.2f %6.2f\n",
	 junctionpoint.x[0],junctionpoint.x[1],junctionpoint.x[2]);
 fprintf(fout,"Time Points\n%d %d\n",outputStepNo,outputIncNo);
 fprintf(fout,"Nodes\n");



 {
   for (int i=0;i<numnodes;i++)
       {
	 fprintf(fout,"%5d %6.3f %6.3f %6.3f\n",i+1000,
		  nodalDisplacements[i].x[0],nodalDisplacements[i].x[1],nodalDisplacements[i].x[2]);
       }
 }
 // fprintf(stderr,"Done with nodes\n");
 fprintf(fout,"Elements\n");
 for (int i=0;i<numelements;i++)
     {
       fprintf(fout,"%5d ",i+1000);
       fprintf(fout,"%10.6f %10.6f %10.6f"
	       "%10.6f %10.6f %10.6f\n",
	       strain[0][i],strain[1][i],strain[2][i],strain[3][i],strain[4][i],strain[5][i]);
       if (nodirections)
	   {
	     fprintf(fout," %10.6f %10.6f %10.6f %10.6f %3d\n",
		     elementVolumes[i],elementCentroids[i].x[0],elementCentroids[i].x[1],
		     elementCentroids[i].x[2],
		     Irange(pieindex[i],0,numpies-1));
	   }
       else
	   {
	     fprintf(fout," %10.6f %10.6f %10.6f %10.6f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %3d\n",
		     elementVolumes[i],
		     elementCentroids[i].x[0],elementCentroids[i].x[1],elementCentroids[i].x[2],
		     radial[i].x[0],radial[i].x[1],radial[i].x[2],
		     circum[i].x[0],circum[i].x[1],circum[i].x[2],pieindex[i]);
	   }
     }
 fclose(fout);
 return 1;
}
// ------------------------------------------------------------------------
int PXAbaqusOutputData::Load(const char* fname,int rotate,int check,int integrate)
{
 gzFile fin=gzsuffixopen(fname,"r");
 if (!fin)
     return 0;
 
 char line[200];
 gzgets(fin,line,200);
 
 int nodirections=1;

 if (gzfindstringinstring(line, "#PXAbaqusOutputDataNoDirections")==0)
     {
       if (gzfindstringinstring(line, "#PXAbaqusOutputData")==0)
	   {
	     gzclose(fin);
	     return LoadOld(fname,rotate);
	   }
       else
	   nodirections=0;
     }
 
 
 int np,ne;
 
 gzgets(fin,line,200);  gzgets(fin,line,200);
 sscanf(line,"%d",&np);
 gzgets(fin,line,200);  gzgets(fin,line,200);
 sscanf(line,"%d",&ne);

  if (np!=numnodes || ne!=numelements)
      {
	if (check)
	    {
	      fprintf(stderr,"Wrong number of nodes (%d vs %d) or elements (%d vs %d)\n",
		      np,numnodes, ne,numelements);
	      gzclose(fin);
	      return(0);
	    }
	else
	    {
	      cleanup();
	      init(np,ne);
	    }
      }
 /*else
   fprintf(stderr,"All OK no new allocations numelements=%d,numnodes=%d\n",
   numelements,numnodes);*/
 
 gzgets(fin,line,200);  gzgets(fin,line,200);
 sscanf(line,"%d %d %d",&numpies,&numslices,&numsectors);
 gzgets(fin,line,200);  gzgets(fin,line,200); 
 sscanf(line,"%f %f %f",&junctionpoint.x[0],&junctionpoint.x[1],&junctionpoint.x[2]);
 gzgets(fin,line,200);  gzgets(fin,line,200); 
 sscanf(line,"%d %d ",&outputStepNo,&outputIncNo);
 gzgets(fin,line,200);  

 int temp;
 if (integrate==0)
     {
       for (int i=0;i<numnodes;i++)
	   {
	     gzgets(fin,line,200);
	     sscanf(line,"%d %f %f %f",&temp,
		    &nodalDisplacements[i].x[0],
		    &nodalDisplacements[i].x[1],
		    &nodalDisplacements[i].x[2]);
	   }
     }
 else
     {
       fprintf(stderr,"Reading and integrating displacements\n");
       float x,y,z;
       for (int i=0;i<numnodes;i++)
	   {
	     gzgets(fin,line,200);
	     sscanf(line,"%d %f %f %f",&temp,&x,&y,&z);
	     nodalDisplacements[i].x[0]+=x;
	     nodalDisplacements[i].x[1]+=y;
	     nodalDisplacements[i].x[2]+=z;
	   }
       gzclose(fin);
       return 1;
     }
 
 gzgets(fin,line,200);
 // fprintf(stderr,"Separator : `%s'\n",line);

 for (int i=0;i<numelements;i++)
     {
       gzgets(fin,line,200);
       sscanf(line,"%d %f %f %f %f %f %f",&temp,
	      &strain[0][i],&strain[1][i],&strain[2][i],&strain[3][i],&strain[4][i],&strain[5][i]);
       if (nodirections)
	   {
	     gzgets(fin,line,200);
	     sscanf(line," %f %f %f %f  %d",
		    &elementVolumes[i],
		    &elementCentroids[i].x[0],&elementCentroids[i].x[1],&elementCentroids[i].x[2],
		    &pieindex[i]);
	     radial[i].setvector(1.0,0.0,0.0);
	     circum[i].setvector(0.0,1.0,0.0);
	   }
       else
	   {
	     gzgets(fin,line,200);
	     sscanf(line,"%f %f %f %f %f %f %f %f %f %f %d",
		    &elementVolumes[i],
		    &elementCentroids[i].x[0],&elementCentroids[i].x[1],&elementCentroids[i].x[2],
		    &radial[i].x[0],&radial[i].x[1],&radial[i].x[2],
		    &circum[i].x[0],&circum[i].x[1],&circum[i].x[2],&pieindex[i]);
	   }
       /*if (i<15)
	   fprintf(stderr,"\ni=%d, strains=(%6.2f %6.2f %6.2f %6.2f %6.2f %6.2f)\n"
		   "vol=%10.5f centroid=(%6.2f %6.2f %6.2f)\n"
		   "directions (%5.2f %5.2f %5.2f) (%5.2f %5.2f %5.2f) pie=%d)\n",i,
		   strain[0][i],strain[1][i],strain[2][i],strain[3][i],strain[4][i],strain[5][i],
		   elementVolumes[i],
		   elementCentroids[i].x[0],elementCentroids[i].x[1],elementCentroids[i].x[2],
		   radial[i].x[0],radial[i].x[1],radial[i].x[2],
		   circum[i].x[0],circum[i].x[1],circum[i].x[2],
		   pieindex[i]);*/
     }
 //fprintf(stderr,"pieindex[5]=%02d ",pieindex[5]);
 gzclose(fin); 
 
 /*if (rotate)
   {
       fprintf(stderr,"Going to rotate strains\n");
       rotatestrains();
       }*/
 
 /*if (numpies==1)
   fprintf(stderr,"Numpies=1\n");*/
 /* for (i=0;i<numelements;i++)
    fprintf(stderr,"%d ",pieindex[i]);
    fprintf(stderr,"Going to average strains\n");*/
 // averagestrains();
 
 return 1;
 
}
// ------------------------------------------------------------------------
void PXAbaqusOutputData::setRadial(int el,float x,float y,float z)
{
 el=Irange(el,0,numelements-1);
 radial[el].setvector(x,y,z);
}
void PXAbaqusOutputData::setCircum(int el,float x,float y,float z)
{
 el=Irange(el,0,numelements-1);
 circum[el].setvector(x,y,z);
}
void PXAbaqusOutputData::setCentroid(int el,float x,float y,float z)
{
 el=Irange(el,0,numelements-1);
 elementCentroids[el].setvector(x,y,z);
}
// ------------------------------------------------------------------------
void PXAbaqusOutputData::setPieStats(int nump,int numse,int numsl,int numt)
{
  numpies=nump;
  numsectors=numse;
  numslices=numsl;
  numtransmural=numt;
}
// ------------------------------------------------------------------------
int PXAbaqusOutputData::getElementPieIndex(int elno)
{
  elno=Irange(elno,0,numelements-1);
  return pieindex[elno];
}

void PXAbaqusOutputData::setElementPieIndex(int elno,int pieind)
{
  elno=Irange(elno,0,numelements-1);
  pieind=Irange(pieind,0,numpies-1);
  pieindex[elno]=pieind;
}
// ------------------------------------------------------------------------
void PXAbaqusOutputData::setElementInfo(int elno,int mode,float evol,
					float e11,float e22,float e33,
						 float e12,float e13,float e23)
{
 elno=Irange(elno,0,numelements-1);
 mode=Irange(mode,0,2);
 
 if (mode==0 || mode==2)
     elementVolumes[elno]=evol;
 
 if (mode==1 || mode==2)
     {
       strain[0][elno]=e11;	strain[1][elno]=e22;	strain[2][elno]=e33;
       strain[3][elno]=e12;	strain[4][elno]=e13;	strain[5][elno]=e23;
     }
}
// ------------------------------------------------------------------------- 
// Node Output Info referes to last calculated displacement
// ------------------------------------------------------------------------- 
void PXAbaqusOutputData::setNodeInfo(int nodeno,float du,float dv,float dw)
{
 nodeno=Irange(nodeno,0,numnodes-1);
 nodalDisplacements[nodeno].x[0]=du;
 nodalDisplacements[nodeno].x[1]=dv;
 nodalDisplacements[nodeno].x[2]=dw;
}
// ------------------------------------------------------------------------- 
int PXAbaqusOutputData::saveNodeDetails(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
      return 0;
  
  fprintf(fout,"Numnodes\n%d\n",numnodes);
  for (int i=0;i<numnodes;i++)
      fprintf(fout,"%5d %6.3f %6.3f %6.3f\n",i+1000,
	      nodalDisplacements[i].x[0],nodalDisplacements[i].x[1],
	      nodalDisplacements[i].x[2]);
 
  fclose(fout);
  return(1);
}
// ------------------------------------------------------------------------- 
int PXAbaqusOutputData::loadNodeDetails(const char* fname)
{
 gzFile fin=gzsuffixopen(fname,"r");
 if (!fin)
     return 0;
 
 char line[200];
 gzgets(fin,line,200);  gzgets(fin,line,200);
 int np;
 sscanf(line,"%d",&np);
 if (np!=numnodes)
     {
       fprintf(stderr,"Wrong number of nodes\n");
       gzclose(fin);
       return(0);
     }
 
 int temp;
 for (int i=0;i<np;i++)
     {
       gzgets(fin,line,200);
       sscanf(line,"%d %f %f %f",&temp,
	      &nodalDisplacements[i].x[0],
	      &nodalDisplacements[i].x[1],
	      &nodalDisplacements[i].x[2]);
     }
 gzclose(fin);
 return(1);
}
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- 
int PXAbaqusOutputData::modifyFromDisplacements(PXSolidNodes* nodes,int inner,int outer,
					 PX3Vector* dispinner,PX3Vector* dispouter)
{
 for (int i=0;i<nodes->getnumnodes();i++)
     {
       int st=nodes->getindex(i,0);
       int pt=nodes->getindex(i,1);
       if (st==inner)
	   setNodeInfo(i,
		       dispinner[pt].x[0]+nodes->getdetails(i)->dx[0],
		       dispinner[pt].x[1]+nodes->getdetails(i)->dx[1],
		       dispinner[pt].x[2]+nodes->getdetails(i)->dx[2]);
       else if (st==outer)
	   setNodeInfo(i,
		       dispouter[pt].x[0]+nodes->getdetails(i)->dx[0],
		       dispouter[pt].x[1]+nodes->getdetails(i)->dx[1],
		       dispouter[pt].x[2]+nodes->getdetails(i)->dx[2]);
     }
 return 1;
}
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- 
void PXAbaqusOutputData::Copy(PXAbaqusOutputData* other)
{
 if (other->getnumnodes()!=numnodes || other->getnumelements()!=numelements)
     {
       cleanup();
       init(other->getnumnodes(),other->getnumelements());
     }
 
 // Pie Stuff 
 junctionpoint.copy(other->getjunctionpoint());
 numpies=other->getnumpies();
 numsectors=other->getnumsectors();
 numslices=other->getnumslices();

 // Node Stuff
 {
   for (int i=0;i<numnodes;i++)
       nodalDisplacements[i].copy(other->getdisplacement(i));
 }


 // Element Stuff
 for (int i=0;i<numelements;i++)
     {
       for (int ia=0;ia<12;ia++)
	   strain[ia][i]=other->getstraincomponent(i,ia);
       elementVolumes[i]=other->getvolume(i);
       elementCentroids[i].copy(other->getcentroid(i));
       radial[i].copy(other->getradial(i));
       circum[i].copy(other->getcircumeferential(i));
       pieindex[i]=Irange(other->getpieindex(i),0,numpies);
     }
}
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- 
int PXAbaqusOutputData::savePieDivision(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
      return 0;
  
  fprintf(fout,"#PXAbaqusOutputDataPieIndex2\n");
  fprintf(fout,"Numelements\n%d\n",numelements);
  fprintf(fout,"Pie stats\n%d %d %d %d\n",numpies,numslices,numsectors,numtransmural);
  fprintf(fout,"# Element  Pie \n");
  for (int i=0;i<numelements;i++)
      fprintf(fout,"%5d %3d\n",i+1000,pieindex[i]);
  fclose(fout);
  return 1;
}
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- 
int PXAbaqusOutputData::loadPieDivision(const char* fname)
{
  gzFile fin=gzsuffixopen(fname,"r");
  if (!fin)
      return 0;
  char* line=new char[200];
  gzgets(fin,line,200);
  int mode=1;

  if (gzfindstringinstring(line,"#PXAbaqusOutputDataPieIndex2")==0)
    {
      if (gzfindstringinstring(line,"#PXAbaqusOutputDataPieIndex")==0)
	{
	  delete [] line;
	  return 0;
	} 
    } 
  else 
    {
      mode =2;
    }
  
  gzgets(fin,line,200);   gzgets(fin,line,200);
  int ne;
  sscanf(line,"%d",&ne);
  
  if (ne!=numelements)
      {
	fprintf(stderr,"Wrong number of elements in read pies (%d vs %d)\n",
		ne,numelements);
	gzclose(fin);
	delete [] line;
	return(0);
      }
  
  gzgets(fin,line,200);   gzgets(fin,line,200);
  if (mode == 1) 
    {
      sscanf(line,"%d %d %d",&numpies,&numslices,&numsectors);
      numtransmural=1;
    }
  else 
    {
      sscanf(line,"%d %d %d %d",&numpies,&numslices,&numsectors,&numtransmural);
    }
  gzgets(fin,line,200);
  
  int temp;
  for (int i=0;i<numelements;i++)
      {
	gzgets(fin,line,200);
	sscanf(line,"%d %d",&temp,&pieindex[i]);
	pieindex[i]=Irange(pieindex[i],0,numpies-1);
      }
  gzclose(fin);
  return 1;
}
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- 
int PXAbaqusOutputData::getsectorno(float phase,float phase_ref,int numsectors,int mode)
{
  float da=360.0/float(numsectors);
  float angle=Fcyclic(phase-phase_ref,360.0);
  int   sector=int(angle/da);

  if (mode==0)
      {
	sector=numsectors-sector;
	if (sector==numsectors)
	    sector=0;
      }
  else if (mode==1)
      {
	sector+=1;
	if (sector==numsectors)
	    sector=0;
      }
      

  if (sector<0 || sector>=numsectors)
      fprintf(stderr,"Error sector=%d, numsectors=%d mode=%d da=%f angle=%f\n",sector,numsectors,mode,da,angle);
  return sector;
}
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- 
int PXAbaqusOutputData::combineWithOtherScale(PXAbaqusOutputData* other,float scaleother,float scalethis)
{
   if (other->getnumnodes()!=numnodes || other->getnumelements()!=numelements)
     {
       fprintf(stderr,"Wrong Number of Nodes and or elements\n");
       return 0;
     }
 
 // Pie Stuff does not change 

 // Node Stuff
   {
     for (int i=0;i<numnodes;i++)
	 nodalDisplacements[i].add(other->getdisplacement(i),scaleother,scalethis);
   }
   
 // Element Stuff
 for (int i=0;i<numelements;i++)
     {
       for (int ia=0;ia<12;ia++)
	   strain[ia][i]=scaleother*other->getstraincomponent(i,ia)+scalethis*strain[ia][i];
       elementVolumes[i]=scaleother*other->getvolume(i)+scalethis*elementVolumes[i];
       elementCentroids[i].add(other->getcentroid(i),scaleother,scalethis);
       radial[i].add(other->getradial(i),scaleother,scalethis);
       circum[i].add(other->getcircumeferential(i),scaleother,scalethis);
       //  pieindex[i]=Irange(other->getpieindex(i),0,numpies);
     }
   return 1;
} 
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- 



