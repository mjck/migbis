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

#include "pxgeometrystruct.h"
#include "pxutil.h"


// ---------------------------------------------------
// Access Functions
// ---------------------------------------------------
PXIntegerNode*  PXIntegerList::getcurrentnode()  { return iterationnode;}
// ---------------------------------------------------
int PXIntegerList::getcurrent()  
{
  return iterationnode->value;
}
// ---------------------------------------------------
int PXIntegerList::getnumnodes() 
{ 
  return numnodes;
} 

// ---------------------------------------------------
PXIntegerList::PXIntegerList()
{
  init();
}
// ---------------------------------------------------
PXIntegerList::~PXIntegerList()
{
  DeleteAll();
  delete topnode;
}
// ---------------------------------------------------
void PXIntegerList::init()
{
  numnodes=0;
  lastnode=new PXIntegerNode;;
  lastnode->next=NULL;
  lastnode->previous=NULL;
  lastnode->value=0;
  topnode=lastnode;
  iterationnode=lastnode;
}
// ---------------------------------------------------  
void  PXIntegerList::Rewind()
{
  iterationnode=topnode;
}
// ---------------------------------------------------
void  PXIntegerList::Next()
{
  if (iterationnode->next!=lastnode)
      iterationnode=iterationnode->next;
  else
      iterationnode=topnode;
}
// ---------------------------------------------------
void  PXIntegerList::Add(int pt)
{
  lastnode->value=pt;
  lastnode->next=new PXIntegerNode;
  (lastnode->next->previous)=lastnode;
  lastnode=lastnode->next;
  lastnode->next=NULL;
  lastnode->value=0;
  numnodes+=1;
}
// ---------------------------------------------------
void  PXIntegerList::AddNonDuplicate(int pt,int exclude)
{
  if (pt==exclude)
    return;
  
  int found=0,i=0;
  Rewind();
  while(i<numnodes && found==0)
    {
      if (getcurrent()==pt)
	found=1;
      ++i;
      Next();
    }
  
  if (found==0)
    {
      lastnode->value=pt;
      lastnode->next=new PXIntegerNode;
      (lastnode->next->previous)=lastnode;
      lastnode=lastnode->next;
      lastnode->next=NULL;
      lastnode->value=0;
      ++numnodes;
    }
}
// ---------------------------------------------------
void PXIntegerList::Sort(PXIntegerList* linked)
{
  if (numnodes<2)
      return;
  
  int hasother=0;

  if (linked!=NULL)
      {
	if (linked->getnumnodes()==numnodes)
	    hasother=1;
      }
  
  int* temp=new int[numnodes];
  Rewind();

  int* temp2=NULL;
  if (hasother)
      {
	temp2=new int[numnodes];
	linked->Rewind();
      }
  int i;
  for (i=0;i<numnodes;i++)
      {
	temp[i]=getcurrent();
	Next();
	if (hasother)
	    {
	      temp2[i]=linked->getcurrent();
	      linked->Next();
	    }
      }

  int oldnumnodes=numnodes;
  DeleteAll();
  if (hasother)
      linked->DeleteAll();

  for (i=0;i<oldnumnodes;i++)
      {
	int minval=temp[i];
	int minval2=0;
	if (hasother)
	    minval2=temp2[i];
	int best=i;
	for (int j=i+1;j<oldnumnodes;j++)
	    {
	      if (temp[j]<minval)
		  {
		    minval=temp[j];
		    best=j;
		    if (hasother)
			minval2=temp2[j];
		  }
	    }
	if (best!=i)
	    {
	      temp[best]=temp[i];
	      if (hasother)
		  temp2[best]=temp2[i];
	    }
	Add(minval);
	if (hasother)
	    linked->Add(minval2);
      }
  delete [] temp;

  if (hasother)
      delete [] temp2;
}
// ---------------------------------------------------
int  PXIntegerList::isSame(PXIntegerList *other)
{
  if (numnodes==other->getnumnodes())
      return (numberinCommon(other)==numnodes);
  return 0;
}
// ---------------------------------------------------
int  PXIntegerList::isSamePresorted(PXIntegerList* other)
{
  int same=1;
  if (numnodes==other->getnumnodes())
      {
	Rewind();
	other->Rewind();
	int i=0;
	while(i<numnodes && same==1)
	    {
	      if (getcurrent()!=other->getcurrent())
		  same=0;
	      other->Next();
	      Next();
	      i++;
	    }
      }
  else
      same=0;
  return same;
}
// ---------------------------------------------------
int  PXIntegerList::numberinCommon(int* points,int numpoints)
{
  Rewind();
  int same=0;
  for (int i=0;i<numnodes;i++)
      {
	int found=0,j=0;
	while(j<numpoints && found==0)
	    {
	      if(getcurrent()==points[j])
		  {
		    same++;
		    found=1;
		  }
	      j++;
	    }
	Next();
      }
  return same;
}
// ---------------------------------------------------
int  PXIntegerList::numberinCommon(PXIntegerList* other)
{
  Rewind();
  int same=0;
  for (int i=0;i<numnodes;i++)
      {
	other->Rewind();
	int found=0,j=0;
	while(j<other->getnumnodes() && found==0)
	    {
	      if(getcurrent()==other->getcurrent())
		  {
		    same++;
		    found=1;
		  }
	      other->Next();
	      j++;
	    }
	Next();
      }
  return same;
}
// ---------------------------------------------------
void  PXIntegerList::Copy(PXTriangle* tri)
{
  DeleteAll();
  for (int j=0;j<=2;j++)
      Add(tri->p[j]);
}
// ---------------------------------------------------
int   PXIntegerList::CopyTo(PXTriangle* tri)
{
  if (numnodes!=3)
      return 0;
  Rewind();
  for(int j=0;j<=2;j++)
      {
	tri->p[j]=getcurrent();
	Next();
      }
  return 1;
}
// ---------------------------------------------------
void  PXIntegerList::DeleteAll()
{
  if (numnodes==0)
    return;
  
  while (lastnode->previous != 0)
    {
      lastnode=lastnode->previous;
      delete lastnode->next;
      lastnode->next=0;
    }
  lastnode=topnode;
  iterationnode=lastnode;
  numnodes=0;  
}
// ---------------------------------------------------  
void  PXIntegerList::Copy(PXIntegerList* other)
{
  DeleteAll();
  other->Rewind();
  //  fprintf(stderr,"Numnodes = %d ",other->getnumnodes());

  for (int i=0;i<other->getnumnodes();i++)
      {
	//fprintf(stderr,"[%d]",i);
	Add(other->getcurrent());
	other->Next();
      }
  //  fprintf(stderr,"\n");
}
// ---------------------------------------------------  
void  PXIntegerList::eliminateDuplicates(int exclude)
{
  int  np=getnumnodes(),i;
  int* buffer=new int[np];
  Rewind();
  for (i=0;i<np;i++)
      {
	buffer[i]=getcurrent();
	Next();
      }
  DeleteAll();
  
  for (i=0;i<np-1;i++)
      for (int j=i+1;j<np;j++)
	  {
	    if (buffer[j]==buffer[i] || buffer[j]==exclude)
		buffer[j]=exclude;
	  }
  for (i=0;i<np;i++)
      {
	if (buffer[i]!=exclude)
	    Add(buffer[i]);
      }
  delete [] buffer;
}
// ---------------------------------------------------  
// ************************************************---
//  Integer List Collection 
// ************************************************---
PXNeighboursList::PXNeighboursList(PXIntegerList* lists,int numlists)
{
  init();
  for (int i=0;i<numlists;i++)
    Add(&lists[i],i);
}
// ************************************************---
PXNeighboursList::PXNeighboursList(PXNeighbours* neighbours,int numneighbours)
{
  init();
  for (int i=0;i<numneighbours;i++)
    Add(&neighbours[i],i);
}
// ************************************************---
PXNeighboursList::PXNeighboursList()
{
  init();
}
// ************************************************---
PXNeighboursList::~PXNeighboursList()
{
  DeleteAll();
  delete topnode;
}
// ************************************************---
void PXNeighboursList::init()
{
  numnodes=0;
  lastnode=new PXNeighboursNode;
  lastnode->next=NULL;
  lastnode->previous=NULL;
  lastnode->points=NULL;
  lastnode->index=-1;
  topnode=lastnode;
  iterationnode=lastnode;
  olditerationnode=lastnode;
  hascache=0;
}
// ************************************************---
void PXNeighboursList::DeleteAll()
{
  if (hascache)
    cleanupCache();
  if (numnodes==0)
    return;
  
  numnodes=0;
  while (lastnode->previous !=NULL)
    {
      if (lastnode->points!=NULL)
	delete lastnode->points;
      lastnode->index=-1;
      lastnode->points=NULL;
      lastnode=lastnode->previous;
      if (lastnode->next!=NULL)
	delete lastnode->next;
      lastnode->next=NULL;
    }
  lastnode=topnode;
  iterationnode=lastnode;
  olditerationnode=lastnode;
}
// ************************************************---
void PXNeighboursList::GotoEnd()
{
  iterationnode=lastnode->previous;
}
// ************************************************---
void PXNeighboursList::Rewind()
{
  iterationnode=topnode;
}
// ************************************************---
void PXNeighboursList::storeIterationPointer()
{
  olditerationnode=iterationnode;
}
// ************************************************---
void PXNeighboursList::popIterationPointer()
{
  iterationnode=olditerationnode;
}
// ************************************************---
void PXNeighboursList::Next()
{
  if (iterationnode->next!=lastnode)
      iterationnode=iterationnode->next;
  else
      iterationnode=topnode;
}
// ************************************************---
void PXNeighboursList::Add(PXNeighboursNode* node)
{
  cleanupCache();
  lastnode->index=node->index;
  lastnode->points=new PXIntegerList();
  (lastnode->points)->Copy(node->points);

  lastnode->next=new PXNeighboursNode;
  (lastnode->next->previous)=lastnode;
  lastnode=lastnode->next;
  lastnode->next=NULL;
  lastnode->points=NULL;
  lastnode->index=-1;
  numnodes++;
}
// ************************************************---
void PXNeighboursList::Add2(int p1,int p2,int externalindex)
{
  cleanupCache();
  if (externalindex>=0)
      lastnode->index=externalindex;
  else
      lastnode->index=numnodes;
  lastnode->points=new PXIntegerList();
  (lastnode->points)->Add(p1);
  (lastnode->points)->Add(p2);
  lastnode->next=new PXNeighboursNode;
  (lastnode->next->previous)=lastnode;
  lastnode=lastnode->next;
  lastnode->next=NULL;
  lastnode->points=NULL;
  lastnode->index=-1;
  numnodes++;


}
// ************************************************---
void PXNeighboursList::AddN(int *p,int np,int externalindex)
{
  cleanupCache();
  if (externalindex>=0)
      lastnode->index=externalindex;
  else
      lastnode->index=numnodes;
  lastnode->points=new PXIntegerList();
  for (int i=0;i<np;i++)
      (lastnode->points)->Add(p[i]);
  lastnode->next=new PXNeighboursNode;
  (lastnode->next->previous)=lastnode;
  lastnode=lastnode->next;
  lastnode->next=NULL;
  lastnode->points=NULL;
  lastnode->index=-1;
  numnodes++;
}
// ************************************************---
void PXNeighboursList::Add3(int p1,int p2,int p3,int externalindex)
{
  cleanupCache();
  if (externalindex>=0)
      lastnode->index=externalindex;
  else
      lastnode->index=numnodes;
  lastnode->points=new PXIntegerList();
  (lastnode->points)->Add(p1);
  (lastnode->points)->Add(p2);
  (lastnode->points)->Add(p3);
  lastnode->next=new PXNeighboursNode;
  (lastnode->next->previous)=lastnode;
  lastnode=lastnode->next;
  lastnode->next=NULL;
  lastnode->points=NULL;
  lastnode->index=-1;
  numnodes++;

}
// ************************************************---
void  PXNeighboursList::Copy(PXNeighboursList* other)
{
  DeleteAll();
  other->Rewind();
  for (int i=0;i<other->getnumnodes();i++)
      {
	Add(other->getcurrentnode());
	other->Next();
      }
}
// ************************************************---
void PXNeighboursList::Add(PXIntegerList* list,int externalindex)
{
  cleanupCache();
  if (externalindex>=0)
      lastnode->index=externalindex;
  else
      lastnode->index=numnodes;
  lastnode->points=new PXIntegerList();
  (lastnode->points)->Copy(list);
  lastnode->next=new PXNeighboursNode;
  (lastnode->next->previous)=lastnode;
  lastnode=lastnode->next;
  lastnode->next=NULL;
  lastnode->points=NULL;
  lastnode->index=-1;
  numnodes++;
}
// ************************************************---
void PXNeighboursList::Add(PXNeighbours* element,int externalindex)
{
  cleanupCache();
  if (externalindex>=0)
      lastnode->index=externalindex;
  else
      lastnode->index=numnodes;
  lastnode->points=new PXIntegerList();
  for(int j=0;j<element->num;j++)
      (lastnode->points)->Add(element->pt[j]);

  lastnode->next=new PXNeighboursNode;
  (lastnode->next->previous)=lastnode;
  lastnode=lastnode->next;
  lastnode->next=NULL;
  lastnode->points=NULL;
  lastnode->index=-1;
  numnodes++;
}
// ************************************************---
int PXNeighboursList::Save(const char* fname)
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
// ************************************************---
int PXNeighboursList::Load(const char* fname)
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
int PXNeighboursList::Save(FILE* fout)
{
  fprintf(fout,"#NeighbourList****************************************---\n");
  Rewind();
  fprintf(fout,"%d\n",numnodes);

  for (int i=0;i<numnodes;i++)
      {
	fprintf(fout,"%d\t%d",iterationnode->index,(iterationnode->points)->getnumnodes());
	(iterationnode->points)->Rewind();
	for(int j=0;j<(iterationnode->points)->getnumnodes();j++)
	    {
	      fprintf(fout,"\t%d",(iterationnode->points)->getcurrent());
	      (iterationnode->points)->Next();
	    }
	fprintf(fout,"\n");
	Next();
      }
  Rewind();  
  return 1;
}

// ************************************************---
int PXNeighboursList::Load(gzFile fin)
{
  DeleteAll();
  char* line=new char[1000];
  gzgets(fin,line,200);
  if (gzfindstringinstring(line,"#NeighbourList")==0)
      {
	delete [] line;
	return 0;
      }
  
  PXNeighbours* a=new PXNeighbours();
  a->pt=new int[1000]; // Can't have more than 1000 neighbours here 
  int num2;
  gzgets(fin,line,200);
  sscanf(line,"%d",&num2);
  //  fprintf(stderr,"NeighboursList::Numel=%d\n",num2);
  for (int i=0;i<num2;i++)
      {
	int temp,num,ind;
	gzgets(fin,line,1000);
	sscanf(line,"%d %d",&ind,&num);
	//	fprintf(stderr,"%d %d : ",ind ,num);
	a->num=num;

	// Skip the first two numbers 
	int po=0;
	for (int lp=0;lp<2;lp++)
	    {
	      po=stringskipspace(line,po);
	      po=stringskipnumber(line,po);
	    }
	
	for(int j=0;j<num;j++)
	  {	
	      // Skip spaces before number 
	      po=stringskipspace(line,po);
	      sscanf(&line[po],"%d ",&temp);
	      po=stringskipnumber(line,po);
	      a->pt[j]=temp;
	    }
	//fscanf(fin,"\n");
	//fprintf(stderr,"\n");
	Add(a,ind);
      }
  delete [] a->pt;
  delete [] line;
  delete a;
  //  fprintf(stderr,"List::Numnodes=%d,%d\n",numnodes,getnumnodes());
  return 1;
}
// ************************************************---
void  PXNeighboursList::Sort()
{
  Rewind();
  for (int i=0;i<getnumnodes();i++)
      {
	(getcurrentnode()->points)->Sort();
	Next();
      }
}
// ************************************************---
void PXNeighboursList::RemoveInvalid()
{
  cleanupCache();
  //  fprintf(stderr,"\n Removing Invalid\n");
  Rewind();
  if (numnodes<1)
      return;
  while (iterationnode!=lastnode)
      {
	//fprintf(stderr,"Scanning to delete index =%d (%d)\n",iterationnode->index,numnodes);
	if (iterationnode->index<0)
	    {
	      PXNeighboursNode* temp=iterationnode;
	      if (iterationnode==topnode)
		  {
		    if (numnodes>1)
			{
			  topnode=iterationnode->next;
			  delete iterationnode->points;
			  delete iterationnode;
			  iterationnode=topnode;
			}
		  }
	      else
		  {
		    (iterationnode->previous)->next=(iterationnode->next);
		    (iterationnode->next)->previous=(iterationnode->previous);
		    iterationnode=iterationnode->next;
		    delete temp->points;
		    delete temp;
		  }
	      numnodes=numnodes-1;
	    }
	else
	    iterationnode=iterationnode->next;
      }
  Rewind();
  storeIterationPointer();
  //  fprintf(stderr,"\n Removing Invalid done=%d\n",numnodes);
}
// ************************************************---
void PXNeighboursList::generateCache()
{
  if (hascache)
      cleanupCache();

  //  fprintf(stderr,"Generating Cache\n");
  c_numnodes=numnodes;
  c_index=new int[c_numnodes];
  c_flag=new int[c_numnodes];
  c_buffer=new int[c_numnodes];
  c_neighbours=new PXNeighbours[c_numnodes];
  
  Rewind();
  
  for (int i=0;i<c_numnodes;i++)
      {
	c_index[i]=iterationnode->index;
	c_neighbours[i].num=(iterationnode->points)->getnumnodes();
	c_neighbours[i].pt=new int[c_neighbours[i].num];
	(iterationnode->points)->Rewind();
	for (int k=0;k<c_neighbours[i].num;k++)
	    {
	      c_neighbours[i].pt[k]=(iterationnode->points)->getcurrent();
	      (iterationnode->points)->Next();
	    }
	c_flag[i]=0;
	Next();
      }
  hascache=1;
}
// ************************************************---
void PXNeighboursList::cleanupCache()
{
  if (!hascache)
      return;

  for (int i=0;i<c_numnodes;i++)
      delete [] c_neighbours[i].pt;

  delete [] c_neighbours;
  delete [] c_index;
  delete [] c_flag;
  delete [] c_buffer;
  c_numnodes=0;
  hascache=0;
}
// ************************************************---
void PXNeighboursList::updateCacheIndices()
{
  if (hascache && c_numnodes==numnodes)
      {
	Rewind();
	for (int i=0;i<c_numnodes;i++)
	    {
	      c_index[i]=iterationnode->index;
	      Next();
	    }
      }
}
// ************************************************---
void PXNeighboursList::updateIndicesFromCache()
{
  if (hascache && c_numnodes==numnodes)
      {
	Rewind();
	for (int i=0;i<c_numnodes;i++)
	    {
	      iterationnode->index=c_index[i];
	      Next();
	    }
      }
}
// ************************************************---
int  PXNeighboursList::cacheIndexConversion(int index)
{
  if (!hascache)
      generateCache();

  // In cases where all nodes is present this will give a hit first time
  // else search 

  if (index>=0 && index<c_numnodes)
      if (c_index[index]==index)
	  return index;

  int i=0,found=0,ind=-1;
  while(i<c_numnodes && found==0)
      {
	if (c_index[i]==index && c_index[i]!=-1)
	    {
	      found=1;
	      ind=i;
	    }
	i++;
      }
  return ind;
}
// ************************************************---
int PXNeighboursList::getrelationship(int pt1,int pt2,int max)
{
  // pt1, pt2 are internal indices i.e. 0..c_numnodes-1

  if (!hascache)
      generateCache();

  // Trivial Case 
  if (pt1==pt2)
      return 0;

  if (pt1<0 || pt1>=c_numnodes || pt2<0 || pt2>=c_numnodes)
      return -1;

  for (int i=0;i<c_numnodes;i++)
      c_flag[i]=0;

  int num_flagged=0,found=0;

  // Starting Iteration 
  int minbuffer=0,maxbuffer=1;
  c_buffer[0]=pt1; // Start Looking from pt1;
  c_flag[pt1]=1;
  int distance=1;
  num_flagged=1;
  int debug=0;
  /*if (pt1==0 && pt2==14)
      debug=1;*/
  int num_flaggedold=0;
  
  while(num_flagged<c_numnodes && found==0 && distance<max)
      {
	// Step 1 -- Look for pt2 in the neighbours of the points in numbuffer
	int nextmaxbuffer=maxbuffer;
	if (debug)
	    fprintf(stderr,"\nBuffer %d:%d , num_flaggged=%d, distance=%d(%d)\n",
		    minbuffer,maxbuffer,num_flagged,distance,max);
	for(int j=minbuffer; j<maxbuffer; j++)
	    {
	      int current=c_buffer[j];
	      if (debug)
		  fprintf(stderr,"\t j=%d current=%d neighbours=%d\n",j,current,c_neighbours[current].num);

	      for (int k=0;k<c_neighbours[current].num;k++)
		  {
		    int pt3=cacheIndexConversion(c_neighbours[current].pt[k]);
		    if (debug)
			fprintf(stderr,"\t\t found %d **** > %d \n",c_neighbours[current].pt[k],pt3);
		    if (pt3!=-1)
			{
			  if (pt2==pt3)
			      {
				found=1;
				// Break out of loops here 
				k=c_neighbours[current].num;
				j=maxbuffer;
			      }
			  else
			      { // Add it to search array for next time 
				if (c_flag[pt3]==0)
				    {
				      c_buffer[nextmaxbuffer]=pt3;
				      c_flag[pt3]=-1;
				      nextmaxbuffer++;
				      num_flagged++;
				    }
			      }
			}
		  }
	    }

	if (!found)
	    {
	      distance++;            // Now looking one more scale 
	      minbuffer=maxbuffer;
	      maxbuffer=nextmaxbuffer;
	      if (num_flagged==num_flaggedold)
		  num_flagged=c_numnodes;
	      else
		  num_flaggedold=num_flagged;
	      
	    }
      }
  /*  fprintf(stderr,"pt1=%d, pt2=%d, distance=%d ,max=%d, num_flagged=%d\n",
      pt1,pt2,distance,max,num_flagged);*/
  
  if (found==0 && distance!=max)
      distance=-1;
  
  return distance;
  
}
// ************************************************---
int PXNeighboursList::tagconnectedto(int p,int maxrelationship)
{
  if (!hascache)
      generateCache();
  
  p=cacheIndexConversion(p);
  
  if (p<0)
      return -1;

  int numconnected=0;
  for (int i=0;i<c_numnodes;i++)
      {
	int dist=getrelationship(p,i,maxrelationship+1);
	if (dist==-1 || dist>maxrelationship)
	    c_index[i]=-1;
	else
	    numconnected++;
	/*fprintf(stderr,"Distance from %-2d to %-2d (max=%2d) =%d index=%d\n",
		p,i,maxrelationship+1,dist,c_index[i]);*/

      }
  //  updateIndicesFromCache();
  return numconnected;
}
// ************************************************---
int PXNeighboursList::getcentroid()
{
  if (!hascache)
      generateCache();

  int mindist=1000000000;
  int bestnode=-1;

  for (int i=0;i<c_numnodes;i++)
      {
	if (c_index[i]>=0)
	    {
	      int dist=0;
	      for (int j=0;j<c_numnodes;j++)
		  {
		    if (c_index[j]>=0)
			{
			  int a=getrelationship(i,j);
			  if (a<0)
			      return -1;
			  dist+=a;
			}
		  }
	      if (dist<mindist)
		  {
		    mindist=dist;
		    bestnode=i;
		  }
	    }
      }
  return c_index[bestnode];
}
// ************************************************---
int PXNeighboursList::getcacheindex(int i)
{
  if (!hascache)
      return -1;
  
  if (i<0 || i>=c_numnodes)
      return -1;

  return c_index[i];
}

// ************************************************---
int PXNeighboursList::getcurrentindex()
{ 
  return iterationnode->index;
}
// ************************************************---
int PXNeighboursList::getnumnodes()
{ 
  return numnodes;
}
// ************************************************---
/*
    Vector Classes


*/
// ************************************************---
PX3Vector::PX3Vector()
{
  for (int ia=0;ia<=2;ia++)
      x[ia]=0.0;
}

PX3Vector::PX3Vector(float a1,float a2,float a3)
{
  setvector(a1,a2,a3);
}

void PX3Vector::setvector(float a1,float a2,float a3)
{
  x[0]=a1; x[1]=a2; x[2]=a3;
}

float PX3Vector::getlength()
{
  float sum=0.0;
  for (int ia=0;ia<=2;ia++)
      sum+=pow(x[ia],float(2.0));  
  return sqrt(sum);
}

float PX3Vector::getdistance(PX3Vector* other)
{
  float sum=0.0;
  for (int ia=0;ia<=2;ia++)
      sum+=pow(x[ia]-other->x[ia],float(2.0));  
  return sqrt(sum);
}

void  PX3Vector::copy(PX3Vector* other)
{
  for (int ia=0;ia<=2;ia++)
      x[ia]=other->x[ia];
}
 
float PX3Vector::normalize()
{
  float d=getlength();
  if (d!=0.0)
      {
	for (int ia=0;ia<=2;ia++)
	    x[ia]/=d;
      }
  return d;
}
  
float PX3Vector::dotwith(PX3Vector* other)
{
  float sum=0.0;
  for (int ia=0;ia<=2;ia++)
      sum+=x[ia]*other->x[ia];
  return sum;
}

PX3Vector* PX3Vector::crosswith(PX3Vector* other)
{
  PX3Vector* cross=new PX3Vector(0.0,0.0,0.0);

  for (int ia=0;ia<=2;ia++)
      {
	int n1=Icyclic(ia+1,3);
	int n2=Icyclic(ia+2,3);
	cross->x[ia]=this->x[n1]*other->x[n2] - this->x[n2]*other->x[n1];
      }
  return cross;
}

void PX3Vector::crosswithoverwrite(PX3Vector* other)
{
  float cr[3];

  for (int ia=0;ia<=2;ia++)
      {
	int n1=Icyclic(ia+1,3);
	int n2=Icyclic(ia+2,3);
	cr[ia]=this->x[n1]*other->x[n2] - this->x[n2]*other->x[n1];
      }

  setvector(cr[0],cr[1],cr[2]);
}

void PX3Vector::subtract(PX3Vector* other,float factor)
{
  for (int ia=0;ia<=2;ia++)
      x[ia]=x[ia]-factor*other->x[ia];
}

void PX3Vector::add(PX3Vector* other,float factor,float thisfactor)
{
  for (int ia=0;ia<=2;ia++)
      x[ia]=thisfactor*x[ia]+factor*other->x[ia];
}

// ------------------------------------------------------------------------------------------------
PX2Vector::PX2Vector()
{
  for (int ia=0;ia<=1;ia++)
      x[ia]=0.0;
}

PX2Vector::PX2Vector(float a1,float a2)
{
  setvector(a1,a2);
}

void PX2Vector::setvector(float a1,float a2)
{
  x[0]=a1; x[1]=a2; 
}
float PX2Vector::getlength()
{
  float sum=0.0;
  for (int ia=0;ia<=1;ia++)
      sum+=pow(x[ia],float(2.0));  
  return sqrt(sum);
}

float PX2Vector::getdistance(PX2Vector* other)
{
  float sum=0.0;
  for (int ia=0;ia<=1;ia++)
      sum+=pow(x[ia]-other->x[ia],float(2.0));  
  return sqrt(sum);
}

void  PX2Vector::copy(PX2Vector* other)
{
  for (int ia=0;ia<=1;ia++)
      x[ia]=other->x[ia];
}
 
float PX2Vector::normalize()
{
  float d=getlength();
  if (d!=0.0)
      {
	for (int ia=0;ia<=1;ia++)
	    x[ia]/=d;
      }
  return d;
}
  
float PX2Vector::dotwith(PX2Vector* other)
{
  float sum=0.0;
  for (int ia=0;ia<=1;ia++)
      sum+=x[ia]*other->x[ia];
  return sum;
}

PX2Vector* PX2Vector::crosswith(PX2Vector* other)
{
  PX2Vector* cross=new PX2Vector(0.0,0.0);

  for (int ia=0;ia<=1;ia++)
      {
	int n1=Icyclic(ia+1,3);
	int n2=Icyclic(ia+2,3);
	cross->x[ia]=this->x[n1]*other->x[n2] - this->x[n2]*other->x[n1];
      }
  return cross;
}



