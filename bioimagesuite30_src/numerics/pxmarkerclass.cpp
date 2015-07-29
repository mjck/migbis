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


#include "pxmarkerclass.h"
#include "pxutil.h"

// ------------------------------------------------------------------------------------------------
PXClickedPoints::PXClickedPoints()
{
  cntrxy=new PXContour();
  cntrzt=new PXContour();
}

PXClickedPoints::~PXClickedPoints()
{
  delete cntrxy;
  delete cntrzt;
}

void PXClickedPoints::removelast()
{
  cntrxy->RemovePoint();
  cntrzt->RemovePoint();
}

void PXClickedPoints::addpoint(float x,float y,float z,float i,int remove)
{
  PXContourPoint* ptsxy=cntrxy->gettoppoint();
  PXContourPoint* ptszt=cntrzt->gettoppoint();
  int found=0,k=0;
  while (k<cntrxy->getnumpoints() && found==0)
      {
	if (fabs(ptsxy->x-x)<0.1 && fabs(ptsxy->y-y)<0.1 && fabs(ptszt->x-z)<0.1)
	    {
	      //fprintf(stderr,"Found\n");
	      found=1;
	    }
	else
	    {
	      ptsxy=ptsxy->next;
	      ptszt=ptszt->next;
	      k++;
	    }
      }

  if (found==0)
      {
	cntrxy->Add(x,y);
	cntrzt->Add(z,i);
	//fprintf(stderr,"Adding point %d/%d \n",k,cntrxy->getnumpoints());
      }
  else if (remove==1)
      {
	//	fprintf(stderr,"Deleting point %d/%d \n",k,cntrxy->getnumpoints());
	if (cntrxy->getnumpoints()>1)
	    {
	      PXContourPoint* lastxy=cntrxy->getlastpoint();
	      PXContourPoint* lastzt=cntrzt->getlastpoint();
	      ptsxy->x=lastxy->previous->x;     ptsxy->y=lastxy->previous->y;
	      ptszt->x=lastzt->previous->x;	  ptszt->y=lastzt->previous->y;
	    }
	removelast();
      }
  /*else
      {
	fprintf(stderr,"Found point %d/%d Not adding\n",k,cntrxy->getnumpoints());
      }*/
}



void PXClickedPoints::clearpoints()
{
  cntrxy->DeleteAll();
  cntrzt->DeleteAll();
}

void PXClickedPoints::getcentroid(float& x,float& y,float &z,float max)
{
  x=0.0,y=0.0,z=0.0;
  float total=0.0;

  if (cntrxy->getnumpoints()==0)
      return;

  PXContourPoint* ptsxy=cntrxy->gettoppoint();
  PXContourPoint* ptszt=cntrzt->gettoppoint();

  for (int i=0;i<cntrxy->getnumpoints();i++)
      {
	float intensity=ptszt->y;
	
	if (max>0.0)
	    {
	      intensity=max-intensity;
	    }

	x+=intensity*ptsxy->x;
	y+=intensity*ptsxy->y;
	z+=intensity*ptszt->x;
	total+=intensity;
	ptsxy=ptsxy->next;
	ptszt=ptszt->next;
      }
  x/=total;
  y/=total;
  z/=total;

}
PXContour* PXClickedPoints::getpositionxy()
{
  return cntrxy;
}

PXContour* PXClickedPoints::getpositionzintensity()
{
  return cntrzt;
}
// ------------------------------------------------------------------------------------------------
PXMarker::PXMarker(int maxnum)
{
  numframes=Irange(maxnum,1,1000);
  position=new PX3Vector[numframes];
  points=new PXClickedPoints[numframes];
  markerclass=0;
  strcpy(name,"Mark");
}

PXMarker::~PXMarker()
{
  delete [] position;
  delete [] points;
}


void PXMarker::setname(const char* nam)
{
  strncpy(name,nam,12);
}

char* PXMarker::getname()
{
  return name;
}

PXClickedPoints* PXMarker::getclickedpoints(int fr)
{
  fr=Irange(fr,0,numframes-1);
  return &points[fr];
}

void PXMarker::setposition(int fr,float x,float y,float z)
{
  fr=Irange(fr,0,numframes-1);
  position[fr].setvector(x,y,z);
}

void PXMarker::setposition(int fr,PX3Vector*  v)
{
  fr=Irange(fr,0,numframes-1);
  position[fr].copy(v);
}

void PXMarker::setclass(int markclass)
{
  markerclass=Irange(markclass,0,2);
}

int PXMarker::getnumframes()
{
  return numframes;
}

int PXMarker::getclass()
{
  return markerclass;
}

PX3Vector* PXMarker::getposition(int fr)
{
  fr=Irange(fr,0,numframes-1);
  return &position[fr];
}

float PXMarker::getcomponent(int fr,int comp)
{
  fr=Irange(fr,0,numframes-1);
  comp=Irange(comp,0,2);
  return position[fr].x[comp];
}
// ------------------------------------------------------------------------------------------------

PXMarkerCollection::PXMarkerCollection(int num,int numframes)
{
  nummarkers=Irange(num,1,400);
  fprintf(stderr,"In marker collection nummarkers=%d,numframes=%d\n",nummarkers,numframes);
  for (int i=0;i<nummarkers;i++)
      {
	char line[13];
	marker[i]=new PXMarker(numframes);
	sprintf(line,"Marker %3d",i);
	marker[i]->setname(line);
      }
}
  
PXMarkerCollection::~PXMarkerCollection()
{
  for (int i=0;i<nummarkers;i++)
      delete marker[i];
}

PXMarker* PXMarkerCollection::getmarker(int markerno)
{
  markerno=Irange(markerno,0,nummarkers-1);
  return marker[markerno];
}

int PXMarkerCollection::getnummarkers()
{
  return nummarkers;
}

int PXMarkerCollection::loadmarkercollection(const char* fname)
{
  FILE* fin=fopen(fname,"r");
  if (!fin)
      return 0;

  char line[200];

  fgets(line,200,fin);
  int st=strncmp(line, "#PXMarkerCollection",19);
  if (st!=0)
      {
	fclose(fin);
	return 0;
      }
  
  fprintf(stderr,"Old num markers=%d\n",nummarkers);

  //fgets(line,200,fin);
  int newnummark;
  fscanf(fin,"%d\n",&newnummark);
    // Clean up old markers
  int i;
  for (i=0;i<nummarkers;i++)
      delete marker[i];

  nummarkers=Irange(newnummark,1,399);
  fgets(line,200,fin);

  fprintf(stderr,"New num markers=%d\n",nummarkers);

  for (i=0;i<nummarkers;i++)
      {
	// This could become a label
	fgets(line,200,fin);
	if (strlen(line)>0)
	    line[strlen(line)-1]=(char)0;
	int mc,mnum;
	fscanf(fin,"%d\n%d\n",&mc,&mnum);
	//	fprintf(stderr,"Creating marker %d class=%d frames=%d\n",i,mc,mnum);
	marker[i]=new PXMarker(mnum);
	marker[i]->setclass(mc);
	marker[i]->setname(line);
	for (int f=0;f<mnum;f++)
	    {
	      float x,y,z;
	      fscanf(fin,"%f %f %f\n",&x,&y,&z);
	      marker[i]->setposition(f,x,y,z);
	      //fprintf(stderr,"Position %6.2f %6.2f %6.2f\n",x,y,z);
	      int nump;
	      fscanf(fin,"%d\n",&nump);
	      PXClickedPoints* points=marker[i]->getclickedpoints(f);
	      for (int p=0;p<nump;p++)
		  {
		    float t;
		    fscanf(fin,"%f %f %f %f\n",&x,&y,&z,&t);
		    points->addpoint(x,y,z,t);
		  }
	    }
	fgets(line,200,fin);
      }
  fclose(fin);
  return 1;
}

int PXMarkerCollection::savemarkercollection(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
      return 0;

  fprintf(fout,"#PXMarkerCollection\n");
  fprintf(fout,"%d\n",nummarkers);
  fprintf(fout,"#------------------------------------------\n");
  
  for (int m=0;m<nummarkers;m++)
      {
	fprintf(fout,"%s\n",marker[m]->getname());
	fprintf(fout,"%d\n",marker[m]->getclass());
	fprintf(fout,"%d\n",marker[m]->getnumframes());
	for (int fr=0;fr<marker[m]->getnumframes();fr++)
	    {
	      for (int k=0;k<=2;k++)
		  fprintf(fout,"%6.2f ",marker[m]->getcomponent(fr,k));
	      fprintf(fout,"\n");
	      PXClickedPoints* point=marker[m]->getclickedpoints(fr);
	      
	      PXContourPoint* cnt1=point->getpositionxy()->gettoppoint();
	      PXContourPoint* cnt2=point->getpositionzintensity()->gettoppoint();
	      int np=point->getpositionxy()->getnumpoints();
	      fprintf(fout,"%d\n",np);
	      for (int ib=1;ib<=np;ib++)
		  {
		    fprintf(fout,"%6.2f %6.2f %6.2f %6.2f \n",cnt1->x,cnt1->y,cnt2->x,cnt2->y);
		    cnt1=cnt1->next;
		    cnt2=cnt2->next;
		  }
	    }
	fprintf(fout,"# ---------------------- \n");
      }
  fclose(fout);
  return 1;
}

int PXMarkerCollection::scalemarkerz(float zx)
{
  fprintf(stderr,"Scaling Marker z-coordinate by %f\n",zx);
  for (int m=0;m<nummarkers;m++)
      {
	for (int fr=0;fr<marker[m]->getnumframes();fr++)
	    {
	      PXClickedPoints* point=marker[m]->getclickedpoints(fr);      
	      PXContourPoint* cnt2=point->getpositionzintensity()->gettoppoint();
	      int np=point->getpositionxy()->getnumpoints();
	      for (int ib=1;ib<=np;ib++)
		  {
		    cnt2->x=zx*cnt2->x;
		    cnt2=cnt2->next;
		  }
	      int c=marker[m]->getclass();
	      float x,y,z;
	      if (c==0)
		  point->getcentroid(x,y,z,4096.0);
	      else
		  point->getcentroid(x,y,z);
	      marker[m]->setposition(fr,x,y,z);
	    }
      }
  return 1;
}

int PXMarkerCollection::loadmarkercollectiondon(const char* fname,int numframes)
{
  FILE* fin=fopen(fname,"r");
  if (fin==NULL)
      {
	fprintf(stderr,"Failed to open %s\n",fname);
	return 0;
      }
  
  // Count total frames and points 
  int nptsmark[100];
  int i;
  for (i=0;i<100;i++)
      nptsmark[i]=0;


  int oldt=-1,current=0,nummark=1;
  int x,y,z,t,intensity;
  while (fscanf(fin,"%d %d %d %d %d\n",&x,&y,&z,&t,&intensity)>0) 
      {
	if (t<oldt)
	    {
	      ++current;
	      ++nummark;
	    }
	++nptsmark[current];
	oldt=t;
      }
  fclose(fin);


  fprintf(stderr,"Nummarkers=%d \n",nummarkers);
  
    for (i=0;i<nummarkers;i++)
      delete marker[i];

  nummarkers=Irange(nummark,1,399);
  fprintf(stderr,"New num markers=%d\n",nummarkers);

  fin=fopen(fname,"r");
  for (i=0;i<nummarkers;i++)
      {
	marker[i]=new PXMarker(numframes);
	marker[i]->setclass(0);
	char line[12];
	sprintf(line,"#Marker %d",i);
	marker[i]->setname(line);
	int j;
	for (j=0;j<nptsmark[i];j++)
	    {
	      int x,y,z,t,intensity;
	      fscanf(fin,"%d %d %d %d %d\n",&x,&y,&z,&t,&intensity);
	      marker[i]->getclickedpoints(t-1)->addpoint(float(x),float(y),
							 float(z)*1.5,
							 float(intensity));
	    }
	for (j=0;j<numframes;j++)
	    {
	      PXClickedPoints* points=marker[i]->getclickedpoints(j);
	      float x,y,z;
	      points->getcentroid(x,y,z,4096.0);
	      marker[i]->setposition(j,x,y,z);
	    }
      }
  fclose(fin);
  return nummarkers;
}


