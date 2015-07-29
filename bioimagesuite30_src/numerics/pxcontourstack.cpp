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

#include "pxcontourstack.h"
#include "pxchamferdistancemap.h"
#include "pxtriangulatedstack.h"


// --------------------------------------------------------------------------
PXContourStack::PXContourStack(PXContourStack *old,int zbetween,int mode)
{
  initfromold(old,zbetween,mode);
  //  fprintf(stderr,"Done\n");
}
// --------------------------------------------------------------------------
PXContourStack::PXContourStack(PXContourStack* old,int zbetween,int mode,
			       int closedbottom,int closedtop,
			       float apexdz,float basedz)
{
  closedbottom=(closedbottom>0);
  closedtop=(closedtop>0);

  if ( closedbottom>0 || closedtop >0 )
      {
	int newnumcontours=old->getnumcontours()+closedbottom+closedtop;
	fprintf(stderr,"Closed Bottom =%d, CLosed top=%d\n",closedbottom,closedtop);
	fprintf(stderr,"Old Contours =%d, new=%d\n",old->getnumcontours(),newnumcontours);

	PXContourStack* temp=new PXContourStack(newnumcontours);

	int index=0;
	for (int i=closedbottom;i<=old->getnumcontours()+closedbottom;i++)
	    {
	      (temp->getcontour(i))->copypoints(old->getcontour(index));
	      temp->setzlevel(i,old->getzlevel(index));
	      index++;
	    }

	float x,y;
	if (closedbottom==1)
	    {
	      float zsize=fabs(old->getzlevel(1)-old->getzlevel(0));
	      if (apexdz>0.0)
		  temp->setzlevel(0,old->getzlevel(0)-apexdz);
	      else
		  temp->setzlevel(0,old->getzlevel(0)-0.3*zsize);
	      (old->getcontour(0))->getcentroid(x,y);
	      (temp->getcontour(0))->setcircle(4,0.02,x,y);
	    }
	
	if (closedtop==1)
	    {
	      float zsize=fabs(old->getzlevel(old->getnumcontours()-1)-
			       old->getzlevel(old->getnumcontours()-2));
	      if (basedz>0.0)
		  temp->setzlevel(newnumcontours-1,old->getzlevel(old->getnumcontours()-1)+apexdz);
	      else
		  temp->setzlevel(newnumcontours-1,old->getzlevel(old->getnumcontours()-1)+0.3*zsize);

	      (old->getcontour(old->getnumcontours()-1))->getcentroid(x,y);
	      (temp->getcontour(newnumcontours-1))->setcircle(4,0.02,x,y);
	    } 	     


	initfromold(temp,zbetween,mode);
	delete temp;
      }
  else
      initfromold(old,zbetween,mode);
}
// --------------------------------------------------------------------------
PXContourStack::PXContourStack(int numberofcontours)
{
  numcontours=numberofcontours;
  if (numcontours<2)
      numcontours=2;
  if (numcontours>=PXCONTOURSTACK_MAXLEVELS)
      numcontours=PXCONTOURSTACK_MAXLEVELS-1;
  for (int i=0;i<numcontours;i++)
      {
	contour[i]=new PXContour();
	zlevel[i]=float(i);
      }
}

// --------------------------------------------------------------------------
void PXContourStack::initfromold(PXContourStack* old,int zbetween,int mode)
{
  if (zbetween==0)
      {
	numcontours=old->getnumcontours();
	for (int i=0;i<old->getnumcontours();i++)
	    {
	      // fprintf(stderr,"%d ",i);
	      contour[i]=new PXContour(old->getcontour(i));
	      zlevel[i]=old->getzlevel(i);
	    }
	return;
      }

  mode=Irange(mode,0,2);
  int index=0;
  int base=0;
  
  float zsize=fabs(old->getzlevel(old->getnumcontours()-1)-old->getzlevel(old->getnumcontours()-2));
  if (zsize<0.1)
      zsize=1.0;

  // Apex First 
  if ((old->getcontour(0))->getnumpoints()==1)
      {
	contour[0]=new PXContour(old->getcontour(0));
	contour[0]->setclosed(1);
	zlevel[0]=old->getzlevel(0);

	index++;
	base=1;
	float meandist=0.0;
	PXContourPoint* pts0=old->getcontour(0)->gettoppoint();
	PXContourPoint* pts1=old->getcontour(1)->gettoppoint();
	float z0=old->getzlevel(0);
	float z1=old->getzlevel(1);
	int np1=old->getcontour(1)->getnumpoints();
	int pt;
	for (pt=0;pt<np1;pt++)
	    {
	      meandist+=Fdist(pts0->x,pts0->y,z0,pts1->x,pts1->y,z1)/float(np1);
	      pts1=pts1->next;
	    }
	float xh=sqrt((-1.0+sqrt(1.0+4.0*meandist*meandist))/2.0);
	float uh=1.0+xh; // asinh not defined !!! asinh(2.0*xh);
	float dtrue=0.125*(2.0*uh+sinh(2.0*uh));
	
	int upper=int(0.5+float(zbetween)*(dtrue/zsize));
	upper=Irange(upper,1,20);

	for (int m=1;m<=upper;m++)
	    {
	      
	      float dx=float(m)/float(upper+1);
	      float n0=-(dx-1.0)*(dx+1.0);
	      float n1=0.5*(dx+1.0)*dx;
	      float n2=0.5*(dx-1.0)*dx;
	      zlevel[index]=n0*z0+(n1+n2)*z1;
	      
	      contour[index]=new PXContour();
	      pts1=old->getcontour(1)->gettoppoint();
	      PXContourPoint* pts2=pts1;
	      int pthalf=np1/2;
	      for (pt=0;pt<pthalf;pt++)
		  pts2=pts2->next;
	      
	      for (pt=0;pt<np1;pt++)
		  {
		    contour[index]->Add(n0*pts0->x+n1*pts1->x+n2*pts2->x,
					n0*pts0->y+n1*pts1->y+n2*pts2->y);
		    pts1=pts1->next;
		    if (pts2->next->next==NULL)
			pts2=old->getcontour(1)->gettoppoint();
		    else
			pts2=pts2->next;
		  }
	      contour[index]->setclosed(1);
	      contour[index]->Equispace(0.8);
	      contour[index]->Smooth(1);
	      index++;
	    }
      }

  if (mode>0)
      {
	for (int i=base;i<old->getnumcontours();i++)
	    {
	      contour[index]=new PXContour(old->getcontour(i));
	      contour[index]->setclosed(1);
	      zlevel[index]=old->getzlevel(i);
	      index++;
	      if (i!=old->getnumcontours()-1)
		  {
		    float znewsize=fabs(old->getzlevel(i+1)-old->getzlevel(i));
		    int upper=int(0.5+float(zbetween)*(znewsize/zsize));
		    for(int m=1;m<=upper;m++)
			{
			  PXChamferDistanceMap* map;
			  int f1=(upper+1-m);
			  int f2=m;
			  float z=float(f1)*old->getzlevel(i)+float(f2)*old->getzlevel(i+1);
			  z=z/float(f1+f2);
			  if (i==0 || i==(old->getnumcontours()-1) || mode==1 || numcontours<4)
			      map=new PXChamferDistanceMap(old->getcontour(i),
							   old->getcontour(i+1)
							   ,f1,f2);
			  else
			      map=new PXChamferDistanceMap(old->getcontour(i-1),old->getcontour(i),
							   old->getcontour(i+1),old->getcontour(i+2),
							   float(m)/float(zbetween+1));
			  contour[index]=map->extractContour();
			  zlevel[index]=z;
			  contour[index]->setclosed(1);
			  contour[index]->Smooth(1);
			  contour[index]->Equispace(0.8);
			  index++;
			}
		  }
	    }
	numcontours=index;
      }
  else
      {
	int npmax=0;
	int i;
	for (i=base;i<old->getnumcontours();i++)
	    npmax=Imax(npmax,old->getcontour(i)->getnumpoints());
	float *x[2],*y[2];
	x[0]=new float[npmax];     y[0]=new float[npmax];
	x[1]=new float[npmax];     y[1]=new float[npmax];
	
	for (i=base;i<old->getnumcontours();i++)
	    {
	      contour[index]=new PXContour(old->getcontour(i));
	      contour[index]->setclosed(1);
	      zlevel[index]=old->getzlevel(i);
	      index++;
	      int np[2];
	      float znewsize=fabs(old->getzlevel(i+1)-old->getzlevel(i));
	      int upper=int(0.5+float(zbetween)*(znewsize/zsize));
	      np[0]=old->getcontour(i)->getnumpoints();
	      np[1]=old->getcontour(i+1)->getnumpoints();
	      for (int c=0;c<=1;c++)
		  {
		    PXContourPoint* pts=old->getcontour(i+c)->gettoppoint();
		    np[c]=old->getcontour(i+c)->getnumpoints();
		    for(int ia=0;ia<np[c];ia++)
			{
			  x[c][ia]=pts->x;  y[c][ia]=pts->y;
			  pts=pts->next;
			}
		  }
	      
	      // Find Origin
	      // -----------
	      int origin[2];
	      PXTriangulatedStack::findorigin2D(np,x,y,origin);
	      
	      // Triangulation Algorithm
	      // -----------------------
	      int m;
	      for(m=1;m<=upper;m++)
		  {
		    float f1=float(upper+1-m),f2=float(m);
		    float sum=f1+f2;
		    f1=f1/(sum);f2=f2/(sum);
		    float z=f1*old->getzlevel(i)+f2*old->getzlevel(i+1);
		    contour[index+m-1]=new PXContour();
		    zlevel[index+m-1]=z;
		    contour[index+m-1]->Add(f1*x[0][origin[0]]+f2*x[1][origin[1]],
					    f1*y[0][origin[0]]+f2*y[1][origin[1]]);
		  }
	      
	      int done=0,cend[2],current[2],next[2];
	      float dist[2];
	      cend[0]=0; cend[1]=0;
	      current[0]=origin[0];  current[1]=origin[1];
	      
	      while(done==0)
		  {
		    for (int c=0;c<=1;c++)
			{
			  if (cend[c]==0)
			      {
				next[c]=Icyclic(current[c]+1,np[c]);
				int other=current[1-c];
				dist[c]=Fsqdist(x[c][next[c]],y[c][next[c]],0.0,
						x[1-c][other],y[1-c][other],0.0);
			      }
			  else
			      dist[c]=100000000.0;
			}
		    
		    if (dist[0]<dist[1])
			{
			  current[0]=next[0];
			  for (int m=1;m<=upper;m++)
			      {
				float f1=float(upper+1-m),f2=float(m);
				float sum=f1+f2;
				f1=f1/(sum);f2=f2/(sum);
				contour[index+m-1]->Add(f1*x[0][current[0]]+f2*x[1][current[1]],
							f1*y[0][current[0]]+f2*y[1][current[1]]);
			      }
			  if (current[0]==origin[0]) 
			      cend[0]=1;
			}
		    else
			{
			  current[1]=next[1];
			  for (int m=1;m<=upper;m++)
			      {
				float f1=float(upper+1-m),f2=float(m);
				float sum=f1+f2;
				f1=f1/(sum);f2=f2/(sum);
				contour[index+m-1]->Add(f1*x[0][current[0]]+f2*x[1][current[1]],
							f1*y[0][current[0]]+f2*y[1][current[1]]);
			      }
			  if (current[1]==origin[1]) 
			      cend[1]=1;
			}
		    
		    if (cend[0]==1 && cend[1]==1)
			done=1;
		    
			}
	      for (m=1;m<=upper;m++)
		  {
		    contour[index]->setclosed(1);
		    contour[index]->Equispace(0.8);
		    contour[index]->Smooth(1);
		    index++;
		  }
	    }
	delete [] x[0]; delete [] x[1];
	delete [] y[0]; delete [] y[1];
	numcontours=index;
      }
}
// --------------------------------------------------------------------------
PXContourStack::~PXContourStack()
{
  for (int i=0;i<numcontours;i++)
      delete contour[i];
}

// --------------------------------------------------------------
int PXContourStack::getnumpoints()
{
  int np=0;
  for (int i=0;i<numcontours;i++)
      np+=contour[i]->getnumpoints();
  return np;
}

// --------------------------------------------------------------
// Copy control points from another surface
// --------------------------------------------------------------
void PXContourStack::copypoints(PXContourStack* other)
{
  int oldnumcontours=numcontours;
  numcontours=other->getnumcontours();

  if (oldnumcontours!=numcontours)
      {
	if (oldnumcontours>numcontours)
	    {
	      for (int i=numcontours;i<oldnumcontours;i++)
		  delete contour[i];
	    }
	else
	    {
	      for (int i=oldnumcontours;i<numcontours;i++)
		  contour[i]=new PXContour();
	    }
	
      }
  
  for (int i=0;i<numcontours;i++)
      {
	contour[i]->copypoints(other->getcontour(i));
	zlevel[i]=other->getzlevel(i);
      }
  
}
// --------------------------------------------------------------
void PXContourStack::setcontour(int index,PXContour* spl)
{
  index=indexbounded(index);  
  if (index>=0 && index<numcontours)
      {
	contour[index]->copypoints(spl);
      }
}
// ------------------------------------------------------------------
void PXContourStack::setzlevel(int index,float zl)
{
  index=indexbounded(index);  
  if (index>=0 && index<numcontours)
      {
	zlevel[index]=zl;
      }
}
// --------------------------------------------------------------
int  PXContourStack::getzbetween(float zdist)
{
  if (zdist<0.0)
      return 0;
  
  int middle=numcontours/2;
  float zsize=fabs(zlevel[middle]-zlevel[middle-1]);
  zdist=zdist/zsize; 
  int zbetween;
  if (zdist<0.1)  zdist=0.1;
  if (zdist>=1.0) 
      zbetween=0;
  else
      zbetween=int(0.5+1.0/zdist)-1;

  return zbetween;
}
// --------------------------------------------------------------
void PXContourStack::setellipticalcylinder(int np,float rx,float ry,float cx,float cy)
{
  int i;
  for (i=0;i<numcontours;i++)
      contour[i]->DeleteAll();
  
  for (int j=0;j<np;j++)
      {
	float t=M_PI*2.0*(float(j-0)/float(np));
	float xx=rx*cos(t)+cx;
	float yy=ry*sin(t)+cy;
	for (int i=0;i<numcontours;i++)
	    contour[i]->Add(xx,yy);
      }
  for (i=0;i<numcontours;i++)
      contour[i]->setclosed(1);
  
}
// --------------------------------------------------------------
void  PXContourStack::Smooth(float sigma,int width)
{
  for (int i=0;i<numcontours;i++)
      {
	contour[i]->setclosed(1);
	contour[i]->Smooth(sigma,width);
      }
}
// --------------------------------------------------------------
void  PXContourStack::Equispace(float dist)
{
  if (dist>0.0)
      {
	for (int i=0;i<numcontours;i++)
	    {
	      contour[i]->setclosed(1);
	      contour[i]->Equispace(dist);
	    }
      }
}
void  PXContourStack::Equisample(int npoints)
{
  if (npoints>0)
      {
	for (int i=0;i<numcontours;i++)
	    {
	      contour[i]->setclosed(1);
	      contour[i]->Equisample(npoints);
	    }
      }
}
// ------------------------------------------------------------------
//           I/O Functions
//
//        Load & Save -->              Surface Format (Control Points)
// ------------------------------------------------------------------
int PXContourStack::Load(const char* filename)
{
  char line[200];
  float x,y,z;
  int npoints;


  gzFile fin=gzsuffixopen(filename,"r");
  if (fin!=Z_NULL)
      {
	npoints=0; 
	gzgets(fin,line,100);
	
	if (line[0]=='s')
	    {
	      // Test Nuages Format Found 
	      gzclose(fin);
	      return LoadNuages(filename);
	    }
	
	int i;
	for(i=0;i<numcontours;i++)
	    delete contour[i];

	while ( gzgets(fin,line,100)!=NULL )    
	    npoints++;
	gzclose(fin);
	// Read File
	fin=gzsuffixopen(filename,"r");
	
	// Eat First line x-y-z
	gzgets(fin,line,100);
	
	numcontours=0;
	contour[numcontours]=new PXContour();
	
	for(i=0;i<npoints;i++)
	    {
	      gzgets(fin,line,100);
	      sscanf(line,"%f %f %f\n",&x,&y,&z);
	      if (i==0)
		  zlevel[numcontours]=z;
	      
	      if (zlevel[numcontours]==z)
		  {
		    contour[numcontours]->Add(x,y);
		  }
	      else
		  {
		    numcontours++;
		    zlevel[numcontours]=z;
		    contour[numcontours]=new PXContour();
		    contour[numcontours]->Add(x,y);
		  }
	    }
	gzclose(fin);
	numcontours++;
	//fprintf(stderr,"Loading from %s as contourstack (numcontours=%2d,npoints=%d)\n",filename,numcontours,npoints);
	return 1;
      }
  //fprintf(stderr,"Failed to open %s\n",filename);
  return 0;
}
// --------------------------------------------------------------
int PXContourStack::Save(const char* filename,float dist,float zdist,int mode,int clb,int clt)
{
  int zbet=0;
  if (zdist>0.0)
      zbet=getzbetween(zdist);
  PXContourStack* stack=new PXContourStack(this,zbet,mode,clb,clt);
  if (dist>0.0)  stack->Equispace(dist);

  int flag=stack->Save(filename);
  delete stack;
  return flag;
}
// --------------------------------------------------------------
int  PXContourStack::Save(const char* filename,int complevel)
{
  FILE* fout=fopen(filename,"w");
  if (fout!=NULL)
      {
	fprintf(fout,"# x  y  z\n");
	for (int i=0;i<numcontours;i++)
	  contour[i]->SaveStream(fout,0,1,zlevel[i]);
	fclose(fout);
	return(1);
      }
  return(0);
}
// --------------------------------------------------------------
int PXContourStack::SaveIGES(const char* filename,float dist,float zdist,int mode,int clb,int clt)
{
  PXContourStack* stack=new PXContourStack(this,getzbetween(zdist),mode,clb,clt);
  if (dist>0.0)
      stack->Equispace(dist);
  int flag=stack->SaveIGES(filename);
  delete stack;
  return flag;
}
// --------------------------------------------------------------
int PXContourStack::SaveIGES(const char* filename)
{
  int flag=0;
  FILE* fout=fopen(filename,"w");
  if (fout!=NULL)
      {
	// IGES Start Section
	// ------------------
	fprintf(fout,"%72sS      1\n",filename);
	fprintf(fout,",,,,,,32,38,6,308,15,,1.0D0,1,2HIN,1,0.0D0,13H960202.170136,            G      1\n"
	        "2.22044604925031D-16,0.0D0,,,9,0,13H960202.170136;                      G      2\n");
	for (int pass=0;pass<2;pass++)
	    {
	      int index=0;
	      for(int c=0;c<numcontours;c++)
		  {
		    PXContourPoint* temp=contour[c]->gettoppoint();
		    for (int ib=0;ib<contour[c]->getnumpoints();ib++)
			{
			  index++;
			  switch(pass)
			      {
			      case 0:
				fprintf(fout,"%8d%8d%8d%8d",116,index,2,2);
				fprintf(fout,"%8d%8d%8d%8d%08dD%7d\n",0,0,
					0,0,0,1);
				fprintf(fout,"%8d%8d%8d%8d",116,0,0,1);
				fprintf(fout,"%8d%8d%8d%8s%8dD%7d\n",0,0,0,
					"POINT",index,2);
				break;
			      case 1:
				fprintf(fout,"116, %15.4e, %15.4e, %15.4f;          %7dP%7d\n",
					temp->x,temp->y,zlevel[c],(index*2)-1,index);
				temp=temp->next;
				break;
			      }
			}
		  }
	      fprintf(fout,"S%7dG%7dD%7dP%7d                                        T%7d\n",
		      1,2,index*2,index,1);
	    }
	flag=1;
      }
  fclose(fout);
  return flag;
}
// --------------------------------------------------------------
int PXContourStack::SaveNuages(const char* filename,float dist,float zdist,int mode,
			       int clbottom,int cltop)
{
  int zbet=0;
  if (zdist>0.0) zbet=getzbetween(zdist);

  PXContourStack* stack=new PXContourStack(this,zbet,mode,clbottom,cltop);
  if (dist>0.0) stack->Equispace(dist);

  int flag=stack->SaveNuages(filename);
  delete stack;
  return flag;
}
// --------------------------------------------------------------
int  PXContourStack::SaveNuages(FILE* fout)
{
  fprintf(fout,"s %d\n", numcontours);

  float totaldist=0.0,maxdist=-10000.0,mindist=100000.0,mean,max,min;
  int   totalsegm=0, nsegm;


  for(int j=0;j<numcontours;j++)
    {
      if (contour[j]->getnumpoints()>1)
	{
	  fprintf(fout,"v %d z %5.2f \n{\n",contour[j]->getnumpoints(),zlevel[j]);
	  contour[j]->calcStats(mean,max,min,nsegm,0);
	  totaldist+=float(nsegm)*mean;
	  totalsegm+=nsegm;
	  mindist=Fmin(mindist,min);
	  maxdist=Fmax(maxdist,max);
	  contour[j]->ensureClockwise();
	  contour[j]->SaveStream(fout,1,1);
	  fprintf(fout,"}\n");
	}
      else if (contour[j]->getnumpoints()==1)
	{
	  fprintf(fout,"v %d z %5.2f \n{\n",4,zlevel[j]);
	  PXContourPoint* pts=contour[j]->gettoppoint();
	  fprintf(fout,"%6.2f %6.2f \n",pts->x-0.01,pts->y-0.01);
	  fprintf(fout,"%6.2f %6.2f \n",pts->x-0.01,pts->y+0.01);
	  fprintf(fout,"%6.2f %6.2f \n",pts->x+0.01,pts->y+0.01);
	  fprintf(fout,"%6.2f %6.2f \n",pts->x+0.01,pts->y-0.01);
	  fprintf(fout,"}\n");
	}
      else
	{
	  fprintf(fout,"v %d z %5.2f \n{\n",0,zlevel[j]);
	  fprintf(fout,"}\n");
	}
      
    }
  fprintf(fout,"# Stats : nsegm=%d Mean=%5.3f Range=%5.3f:%5.3f\n",
	  totalsegm,totaldist/float(totalsegm),mindist,maxdist);
  return(1);
  
}
// --------------------------------------------------------------
int PXContourStack::SaveNuages(const char* filename,int complevel)
{
  int flag=0;
  FILE* fout=fopen(filename,"w");
  if (fout!=NULL)
      {
	flag=SaveNuages(fout);
	fclose(fout);
      }
  return flag;
}
// --------------------------------------------------------------------------
int  PXContourStack::LoadNuages(gzFile fin)
{
  for(int i=0;i<numcontours;i++)
      delete contour[i];

  char* line=new char[100];
  gzgets(fin,line,100);
  sscanf(line,"s %d", &numcontours);
  if (numcontours<0 || numcontours>1000)
      return 0;
  //  fprintf(stderr,"Numcontours=%d\n",numcontours);

  for(int j=0;j<numcontours;j++)
      {
	contour[j]=new PXContour();
	int np;
	float zl,x,y;
	//fscanf(fin,"v %d z %f \n{\n",&np,&zl);
	gzgets(fin,line,100);
	sscanf(line,"v %d z %f",&np,&zl);
	gzgets(fin,line,100);
	zlevel[j]=zl;
	for (int pt=0;pt<np;pt++)
	    {
	      //fscanf(fin,"%f %f\n",&x,&y);
	      gzgets(fin,line,100);
	      sscanf(line,"%f %f",&x,&y);
	      contour[j]->Add(x,y);
	    }

	if (np==4)
	    {
	      // Detect 1 point section artificially inflated for visualization
	      PXContourPoint* pts1=contour[j]->gettoppoint();
	      PXContourPoint* pts2=pts1->next;
	      PXContourPoint* pts3=pts2->next;
	      PXContourPoint* pts4=pts3->next;
	      /*fprintf(stderr,"Checking for single point mess (%5.2f,%5.2f) (%5.2f,%5.2f) (%5.2f,%5.2f) (%5.2f,%5.2f)\n",
		      pts1->x,pts1->y,
		      pts2->x,pts2->y,
		      pts3->x,pts3->y,
		      pts4->x,pts4->y);*/
	      /*v 4 z  0.00 
		{
		44.31  23.21 
		44.31  23.23 
		44.33  23.23 
		44.33  23.21 
		}*/

	      if  ((pts1->x==pts2->x && pts3->x==pts4->x && fabs(pts3->x - pts1->x - 0.02)<0.001) &&
		   (pts1->y==pts4->y && pts3->y==pts2->y && fabs(pts3->y - pts1->y - 0.02)<0.001))
		      {
			//			fprintf(stderr,"Single Point");
			float x=pts1->x+0.001;
			float y=pts1->y+0.001;
			contour[j]->DeleteAll();
			contour[j]->Add(x,y);
		      }
	    }
	contour[j]->setclosed(1);
	gzgets(fin,line,100);
	//fscanf(fin,"}\n");
      }
  delete [] line;
  return(1);
}
// --------------------------------------------------------------
int PXContourStack::LoadNuages(const char* filename)
{
  int flag=0;
  gzFile fin=gzsuffixopen(filename,"rb");
  if (fin!=NULL)
      {
	flag=LoadNuages(fin);
	gzclose(fin);
      }
  return flag;
}
// --------------------------------------------------------------------------
//           Index Conversion
// --------------------------------------------------------------------------
int PXContourStack::indexbounded(int v,int np)
{
  if (np<0) 
      np=numcontours;

  if(v<0)       v=0;
  if(v>=np)     v=np-1;

  return v;
}

int PXContourStack::indexcyclic(int v,int np)
{
  while (v<0) v+=np;
  while (v>=np) v-=np;
  return v;
}
// --------------------------------------------------------------
float PXContourStack::getsqdist(float x1,float x2,float y1,float y2)
{
  return pow(x1-x2,float(2.0))+pow(y1-y2,float(2.0));
}
// --------------------------------------------------------------
int PXContourStack::aligncontours(PXContour* cntr1,PXContour* cntr2)
{
  int np1=cntr1->getnumpoints(),np2=cntr2->getnumpoints();
  int i=0;

  float* x1=new float[np1+1];   float* y1=new float[np1+1];
  cntr1->copy2vector(x1,y1,i);

  float* x2=new float[np2+1];   float* y2=new float[np2+1];
  cntr2->copy2vector(x2,y2,i);

  float dist=0.0,mindist=10000000.0;
  int shift=0;
  
  int j;
  for(j=0;j<np2;j++)
      {
	dist=0;
	for (i=0;i<np1;i++)
	    {
	      int index=int(0.5+float(np2)/float(np1)*float(i));
	      int sj=indexcyclic(j+index,np2);
	      dist+=getsqdist(x1[i],x2[sj],y1[i],y2[sj]);
	    }
	if (dist<mindist)
	    {
	      mindist=dist; 
	      shift=j;
	    }
      }
  
  if (shift!=0)
      {
	cntr2->DeleteAll();
	for (j=0;j<np2;j++)
	    {
	      int sj=indexcyclic(j+shift,np2);
	      cntr2->Add(x2[sj],y2[sj]);
	    }
      }
  
  delete [] x1;  delete [] y1;  delete [] x2;  delete [] y2;
  return shift; 
}
// --------------------------------------------------------------
int PXContourStack::align2basecontours(PXContour* cntr0,PXContour* cntr1,PXContour* cntr2)
{
  int np0=cntr0->getnumpoints(),np1=cntr1->getnumpoints(),np2=cntr2->getnumpoints();
  int i=0;

  float* x0=new float[np0+1];   float* y0=new float[np0+1];
  cntr0->copy2vector(x0,y0,i);

  float* x1=new float[np1+1];   float* y1=new float[np1+1];
  cntr1->copy2vector(x1,y1,i);

  float* x2=new float[np2+1];   float* y2=new float[np2+1];
  cntr2->copy2vector(x2,y2,i);

  float dist=0.0,mindist=10000000.0;
  int shift=0,j;
  
  for(j=0;j<np2;j++)
      {
	dist=0.0;
	for (i=0;i<np0;i++)
	    {
	      int index=int(0.5+float(np2)/float(np0)*float(i));
	      int sj=indexcyclic(j+index,np2);
	      dist+=getsqdist(x0[i],x2[sj],y0[i],y2[sj]);
	    }
	for (i=0;i<np1;i++)
	    {
	      int index=int(0.5+float(np2)/float(np1)*float(i));
	      int sj=indexcyclic(j+index,np2);
	      dist+=getsqdist(x1[i],x2[sj],y1[i],y2[sj]);
	    }
	
	if (dist<mindist)
	    {
	      mindist=dist; 
	      shift=j;
	    }
      }
  
  if (shift!=0)
      {
	cntr2->DeleteAll();
	for (j=0;j<np2;j++)
	    {
	      int sj=indexcyclic(j+shift,np2);
	      cntr2->Add(x2[sj],y2[sj]);
	    }
      }
  
  delete [] x0; delete [] y0; delete [] x1;  delete [] y1;  delete [] x2;  delete [] y2;
  return shift; 
}
// --------------------------------------------------------------
void PXContourStack::selfalign()
{
  for (int i=0;i<numcontours-1;i++)
	aligncontours(contour[i],contour[i+1]);

}
// --------------------------------------------------------------
int PXContourStack::aligntocontourstack(PXContourStack* other)
{
  float z1=zlevel[0],z2=z1-1.0;
  int j=-1;
  while(j<other->getnumcontours() && z2!=z1)
      {
	j++;
	z2=other->getzlevel(j);
      }

  if (z2!=z1)
      return 0;

  aligncontours(other->getcontour(j),contour[0]);
  selfalign();
  return 1;
}
// --------------------------------------------------------------
void PXContourStack::shiftandscalez(float shift,float scale)
{
  for (int i=0;i<getnumcontours();i++)
      {
	float z=getzlevel(i);
	z=scale*(z+shift);
	setzlevel(i,z);
      }
}
// --------------------------------------------------------------
void PXContourStack::getPositionAndOrientation(float& cx,float& cy,float& cz,
					       float& sx2,float& sy2,float& sxy,
					       float& vol)
{
  cx=0.0;cy=0.0;cz=0.0;
  sx2=0.0;sy2=0.0;sxy=0.0;
  vol=0.0;

  int middle=numcontours/2;
  float zsize=fabs(zlevel[middle]-zlevel[middle-1]);
  int numpoints=0;

  for (int pass=0;pass<=1;pass++)
      {
	for (int i=0;i<getnumcontours();i++)
	    {
	      int np=(getcontour(i))->getnumpoints();
	      if (pass==0)
		  {
		    float z=getzlevel(i);
		    float c1,c2;
		    (getcontour(i))->getcentroid(c1,c2);
		    float area=fabs((getcontour(i))->getarea());
		    //fprintf(stderr,"cntr %d , area=%5.2f c1=%5.2f, c2=%5.2f\n",i+1,area,c1,c2);

		    vol+=area*zsize;
		    cx+=(c1*area*zsize);
		    cy+=(c2*area*zsize);
		    cz+=(z*area*zsize);
		  }
	      if (pass==1)
		  {
		    PXContourPoint* temp=(getcontour(i))->gettoppoint();
		    for (int ib=1;ib<=np;ib++)
			{
			  numpoints++;
			  float x=temp->x,y=temp->y;
			  sx2+=pow(x-cx,float(2.0)); sy2+=pow(y-cy,float(2.0)); sxy+=(x-cx)*(y-cy);
			  temp=temp->next;
			}
		  }
	    }
	if (pass==0)
	    {
	      cx=cx/vol;
	      cy=cy/vol;
	      cz=cz/vol;
	    }
	else
	    {
	      sx2=sx2/float(numpoints);
	      sy2=sy2/float(numpoints);
	      sxy=sxy/float(numpoints);
	    }
      }
}
// --------------------------------------------------------------
float PXContourStack::findnearestneighbour(float& xn,float& yn,float &zn,float dt,int dbg)
{
  float dist=0.0;
  if (zn<=getzlevel(0))
      {
	dist=contour[0]->findnearestneighbour(xn,yn,dt,0);
      }
  else if (zn>=getzlevel(getnumcontours()-1))
      {
	dist=contour[getnumcontours()-1]->findnearestneighbour(xn,yn,dt);
      }
  else
      {
	int i=0;
	int found=0;
	while (found==0 && i<getnumcontours()-1)
	    {
	      if (zn==zlevel[i])
		  {
		    dist=contour[i]->findnearestneighbour(xn,yn,dt);
		    found=1;
		  }
	      if (zn>getzlevel(i) && zn<=getzlevel(i+1))
		  {
		    float x0=xn,y0=yn,x1=xn,y1=yn,d0=1.0,d1=1.0;
		    float dz=(zlevel[i+1]-zlevel[i]);
		    float w0=(zlevel[i+1]-zn)/dz;
		    float w1=(zn-zlevel[i])/dz;
		    
		    d0=contour[i]->findnearestneighbour(x0,y0,dt,dbg);
		    d1=contour[i+1]->findnearestneighbour(x1,y1,dt,dbg);

		    float xf=(w1*x1+w0*x0);
		    float yf=(w1*y1+w0*y0);
		    float zf=(w1*zlevel[i+1]+w0*zlevel[i]);
		    dist=sqrt(pow(xn-xf,float(2.0))+pow(yn-yf,float(2.0))+pow(zn-zf,float(2.0)));
		    if (dbg==1)
			{
			  fprintf(stderr," Point (%5.2f %5.2f %5.2f)\n",xn,yn,zn);
			  fprintf(stderr," Neighbours (%5.2f,%5.2f,%5.2f) and (%5.2f,%5.2f,%5.2f)\n",
				  x0,y0,zlevel[i],x1,y1,zlevel[i+1]);
			  fprintf(stderr," Distances %5.2f %5.2f , Weight %5.2f %5.2f \n",
				  d0,d1,w0,w1);
			  fprintf(stderr," Result (%5.2f,%5.2f,%5.2f) dist=%5.2f\n",xf,yf,zf,dist);
			}

		    xn=xf; yn=yf; zn=zf;
		  }
	      i++;
	    }
      }
  return dist;
}
// --------------------------------------------------------------
int PXContourStack::SaveAbaqusRigid(const char* filename,int offset,int index)
{
  char line1[40],line2[40],line[150];
  sprintf(line1,"/tmp/splstack.cnt");
  sprintf(line2,"/tmp/splstack.off");
  SaveNuages(line1);
  sprintf(line,"nuages %s -tri %s -off -prepros > /dev/null",line1,line2);
  system(line);
  FILE* fin=fopen(line2,"r");
  if (fin)
      {
	FILE* fout=fopen(filename,"w");
	fprintf(fout,"*NODE,NSET=NSURF_%02d\n",index);
	int i;
	for (i=1;i<=5;i++)
	    fgets(line,100,fin);

	int npoints,ntriangles,temp;
	fscanf(fin,"%d %d %d\n",&npoints,&ntriangles,&temp);
	for (i=0;i<npoints;i++)
	    {
	      float x,y,z,nx,ny,nz;
	      fscanf(fin,"%f %f %f %f %f %f\n",&x,&y,&z,&nx,&ny,&nz);
	      fprintf(fout,"%6d, %6.3f, %6.3f, %6.3f\n",i+offset,x,y,z);
	    }
	fprintf(fout,"*ELEMENT, TYPE=R3D3,ELSET=ESURF_%02d\n",index);
	for (i=0;i<ntriangles;i++)
	    {
	      int dummy,n1,n2,n3;
	      fscanf(fin,"%d %d %d %d\n",&dummy,&n1,&n2,&n3);
	      fprintf(fout,"%6d, %6d, %6d, %6d\n",
		      i+offset,n1+offset,n2+offset,n3+offset);
	    }
	fprintf(fout,"*RIGID BODY,ELSET=ESURF_%02d,REF NODE=%d\n",index,offset);
	fprintf(fout,"*SURFACE DEFINITION,NAME=RGDS_%02d\n",index);
	fprintf(fout,"ESURF_%02d,SPOS\n",index);
	fclose(fout);
	fclose(fin);
	sprintf(line,"rm %s %s ",line1,line2);
	system(line);
	return 1;
      }
  
  fprintf(stderr,"Error in creating surfacefile\n");
  return 0;
  
}
// --------------------------------------------------------------
int PXContourStack::nonShrinkSmooth(float alpha,float beta,int iterations)
{
  char name[300],name2[300],command[400];
  sprintf(name,"/tmp/tmpsur05d");
  sprintf(name2,"%s.cnt",name);
  fprintf(stderr,"Saving in %s\n",name2);
  fprintf(stderr,"Status =%d\n",SaveNuages(name2));
  sprintf(command,"nuages %s.cnt -tri %s.idx -prepros",name,name);
  fprintf(stderr,"%s\n",command); 	system(command);
  sprintf(command,"patch2index %s.idx %s.myidx /tmp/extent 1 >/dev/null",name,name);
  fprintf(stderr,"%s\n",command); 	system(command);
  sprintf(command,"surface_smooth %s.myidx %s.stack %6.2f %6.2f %d >/dev/null",name,name,
	  alpha,beta,iterations);
  fprintf(stderr,"%s\n",command); 	system(command);
  sprintf(name2,"%s.stack",name);
  PXContourStack* stack2=new PXContourStack();
  int status=stack2->Load(name2);
  if (status)
      copypoints(stack2);
  delete stack2;
  //Load(name2);
  sprintf(command,"/usr/bin/rm %s*\n",name);
  system(command);
  return status;
}
// --------------------------------------------------------------
int PXContourStack::nonShrinkSmooth2D(float ds,float sigma,int iterations)
{
  fprintf(stderr,"Smoothing Contours (%d) ",getnumcontours());
  for (int k=0;k<getnumcontours();k++)
      {
        PXContour* cntr=getcontour(k);
	fprintf(stderr,"%d ",k);
        cntr->setclosed(1);
	//cntr->Subsample(3);
        cntr->nonShrinkSmooth(sigma,iterations);
        cntr->Subsample(5);
        cntr->Equispace(ds);
      }
  fprintf(stderr,"\n");
  return 1;
}
// ------------------------------------------------------------------------- 
int PXContourStack::createstackset(PXContourStack** stack,
				   int totalnumstacks,int bias)

{
  if (stack[0]->getnumcontours()!=stack[totalnumstacks-1]->getnumcontours())
    {
      fprintf(stderr,"Unequal number of contours");
      return 0;
    }
  
  int numslices=stack[0]->getnumcontours(),i;
  for (i=1;i<totalnumstacks-1;i++)
    stack[i]->copypoints(stack[0]);
	
  int numbetween=totalnumstacks-2;

  // Proceed with Chamfer Interpolation
  for ( i=0;i<numslices;i++)
    {
      PXChamferDistanceMap* map2=new PXChamferDistanceMap(stack[totalnumstacks-1]->getcontour(i),1);
      //int xs=map2->getmaxx()-map2->getminx()+1;
      //      int ys=map2->getmaxy()-map2->getminy()+1;
      


      PXChamferDistanceMap* map1=new PXChamferDistanceMap(stack[0]->getcontour(i),1,1,map2);//1,1,xs,ys);

      for (int j=0;j<numbetween;j++)
	{
	  int fraction1=numbetween-j;
	  int fraction2=j+1;
	  if (j==0 && bias>0)
	    fraction1++;
	  
	  PXChamferDistanceMap* map=new PXChamferDistanceMap(map1,map2,fraction1,fraction2);
	  PXContour* cntr=map->extractContour();
	  (stack[j+1]->getcontour(i))->copypoints(cntr);
	  stack[j+1]->setzlevel(i,stack[0]->getzlevel(i));
	  delete map;
	  delete cntr;
	}
      delete map1;
      delete map2;
    }

  /*  char line[200];
  for (i=0;i<totalnumstacks;i++)
    {
      sprintf(line,"stack%d.cnt",i);
      //stack[i]->SaveNuages(line);
      }    */

  return numbetween;
}
// --------------------------------------------------------------------------
int PXContourStack::ensureAnticlockwise()
{
  return setDirection(1);
}
// ***********************************************************************
int PXContourStack::ensureClockwise()
{
  return setDirection(-1);
}
// ***********************************************************************
int PXContourStack::setDirection(int dir)
{
  for (int i=0;i<numcontours;i++)
    contour[i]->setDirection(dir);
  return 1;
}
// ***********************************************************************
int PXContourStack::cleancopypoints(PXContourStack* other)
{
  int oldnumcontours=numcontours;
  numcontours=other->getnumcontours();

  int i;
  for (i=0;i<other->getnumcontours();i++)
    {
      int np=other->getcontour(i)->getnumpoints();
      if (np<3)
	numcontours-=1;
    }


  if (numcontours==0)
    return 0;
  
  if (oldnumcontours!=numcontours)
      {
	if (oldnumcontours>numcontours)
	    {
	      for (int i=numcontours;i<oldnumcontours;i++)
		  delete contour[i];
	    }
	else
	    {
	      for (int i=oldnumcontours;i<numcontours;i++)
		  contour[i]=new PXContour();
	    }
	
      }
  
  int index=-1;
  for (i=0;i<numcontours;i++)
      {
	int np=0;
	while (np<3)
	  {
	    ++index;
	    np=other->getcontour(index)->getnumpoints();
	  }

	contour[i]->copypoints(other->getcontour(index));
	zlevel[i]=other->getzlevel(index);
      }
  return numcontours;
}
// ***********************************************************************

