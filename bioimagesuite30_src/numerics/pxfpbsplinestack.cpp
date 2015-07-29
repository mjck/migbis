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

#include "pxfpbsplinestack.h"
#include "pxcontourstack.h"
#include "pxchamferdistancemap.h"

// --------------------------------------------------------------------------
int  PXFPBSplineStack::debugmode=0;

// --------------------------------------------------------------------------
PXFPBSplineStack::PXFPBSplineStack(PXFPBSplineStack *old)
{
  numsplines =old->getnumsplines();
  trim_bottom=old->gettrimbottom();
  trim_top   =old->gettrimtop();
  hasapex    =old->apexenabled();
	
  for (int i=0;i<old->getnumsplines();i++)
      spline[i]=new PXFPBSpline(old->getspline(i));

  PX3Vector* v=old->getapex();
  for (int ia=0;ia<=2;ia++)
      apex.x[ia]=v->x[ia];



}
// --------------------------------------------------------------------------
PXFPBSplineStack::PXFPBSplineStack(PXFPBSplineStack *old,int zbetween,
				   int mode,int resample_original)
{
  PX3Vector* v=old->getapex();
  for (int ia=0;ia<=2;ia++)
      apex.x[ia]=v->x[ia];

  hasapex=old->apexenabled();

  zbetween=Irange(zbetween,0,10); 
  PXContourStack* cstack=old->exporttocontourstack(0.5);
  if (debugmode)
      fprintf(stderr,"Created: Normal stack:: First export step ok mode=%d\n",mode);
  
  PXContourStack* cstack2=new PXContourStack(cstack,zbetween,mode);
  if (debugmode)
      {
	fprintf(stderr,"Created interpolated cstack (slices %d-->%d) (between=%d,zmode=%d)\n",
		cstack->getnumcontours(),cstack2->getnumcontours(),zbetween,mode);
      }
  numsplines=0;
  trim_bottom=0;
  trim_top=0;

  if (debugmode)
    fprintf(stderr,"On to Fitting to ContourStack\n");
  if (fittocontourstack(cstack2,0.05,0.5,-1,1))
      {
	if (debugmode)
	    fprintf(stderr,"Fitting OK ...\n");
	// Copy original splines over interpolated ones 
	for (int i=trim_bottom;i<=trim_top;i++)
	    {
	      float z1=spline[i]->getzlevel();
	      int found=0,j=old->gettrimbottom()-1;
	      while(j<=old->gettrimtop() && found==0)
		  {
		    j++;
		    PXFPBSpline* oldspline=old->getspline(j);
		    float z2=oldspline->getzlevel();
		    if (z1==z2)
			{
			  spline[i]->copypoints(oldspline);
			  if (resample_original)
			      spline[i]->changespacing(0.5,-1,0.01);
			  found=1;
			}
		  }
	    }
      }
  else
      {
	if (debugmode)
	    fprintf(stderr,"Fitting Failed, just copying old BSPlineStack\n");

	numsplines=old->getnumsplines();
	settrimbottom(old->gettrimbottom());
	settrimtop(old->gettrimtop());
	
	for (int i=0;i<old->getnumsplines();i++)
	    spline[i]=new PXFPBSpline(old->getspline(i));
      }

  delete cstack2;
  delete cstack;

  if (debugmode)
      fprintf(stderr,"\t done\n");

}
// --------------------------------------------------------------------------

PXFPBSplineStack::PXFPBSplineStack(int numberofsplines)
{
  hasapex=0;
  for (int ia=0;ia<=2;ia++)
      apex.x[ia]=0.0;
  numsplines=numberofsplines;
  numsplines=Irange(numsplines,2,PXFPBSPLINESTACK_MAXLEVELS-1);
  trim_bottom=0;
  trim_top=numsplines-1;

  for (int i=0;i<numsplines;i++)
      spline[i]=new PXFPBSpline();

}
// --------------------------------------------------------------------------
PXFPBSplineStack::PXFPBSplineStack(PXFPBSplineStack *old1,PXFPBSplineStack* old2,float ratio,
				   float smooth,int doapex)
{
  ratio=Frange(ratio,0.001,0.999);
  numsplines=Imax(old1->getnumsplines(),old2->getnumsplines());
  trim_bottom=Imax(old1->gettrimbottom(),old2->gettrimbottom());
  trim_top=Imin(old1->gettrimtop(),old2->gettrimtop());

  if (trim_bottom>trim_top)
      {
	numsplines=0;
	trim_bottom=0;
	trim_top=0;
      }
  else
      {
	numsplines=Irange(numsplines,2,PXFPBSPLINESTACK_MAXLEVELS-1);
	int i;
	for (i=0;i<numsplines;i++)
	    spline[i]=new PXFPBSpline();
	
	if (doapex==0 || trim_bottom!=old1->gettrimbottom() || trim_bottom!=old2->gettrimbottom() ||
	    !old1->apexenabled() || !old2->apexenabled())
	    {
	      hasapex=0;
	      for (int ia=0;ia<=2;ia++)
		  apex.x[ia]=0.0;
	    }
	else
	    {
	      hasapex=1;
	      PX3Vector* v1=old1->getapex();
	      PX3Vector* v2=old2->getapex();
	      for (int ia=0;ia<=2;ia++)
		  apex.x[ia]=(1.0-ratio)*v1->x[ia]+ratio*v2->x[ia];
	    }
	
	
	int wgt1=int(0.5+1000.0*(1-ratio));
	int wgt2=int(0.5+1000.0*(ratio));

	fprintf(stderr,"Numsplines=%d trim_bottom=%d trim_top=%d wgt1=%d wgt2=%d\n",
		numsplines,trim_bottom,trim_top,wgt1,wgt2);

	fprintf(stderr,"Generating Spline :");
	for (i=trim_bottom;i<=trim_top;i++)
	    {
	      fprintf(stderr," %d",i);
	      PXContour* c1=old1->getspline(i)->exporttocontour(0.5);
	      PXContour* c2=old2->getspline(i)->exporttocontour(0.5);
	      
	      PXChamferDistanceMap* map=new PXChamferDistanceMap(c1,c2,wgt1,wgt2);
	      PXContour* cntr=map->extractContour();
	      spline[i]->fittocontour(cntr,smooth,0.0);
	      spline[i]->setzlevel((1.0-ratio)*old1->getspline(i)->getzlevel()+
				    ratio*old2->getspline(i)->getzlevel());
	      spline[i]->ensureAnticlockwise();
	      delete map;      
	      delete cntr;	      
	      delete c1;
	      delete c2;
	    }
	fprintf(stderr,"\n");
      }
}
// --------------------------------------------------------------------------
PXFPBSplineStack::~PXFPBSplineStack()
{
  for (int i=0;i<numsplines;i++)
      delete spline[i];
}
// --------------------------------------------------------------
void PXFPBSplineStack::settrimbottom(int tb)
{
  if (tb>=0 && tb<=trim_top)
      trim_bottom=tb;
}

void PXFPBSplineStack::settrimtop(int tt)
{
  if (tt>=trim_bottom && tt<numsplines)
      trim_top=tt;
}
// --------------------------------------------------------------
void PXFPBSplineStack::autosettrims(float zmin,float zmax)
{
  int oldsl=this->gettrimbottom();
  int sl=oldsl;
  while(sl<=this->gettrimtop()-1)
    {
      if (this->getspline(sl)->getzlevel()<zmin)
	this->settrimbottom(sl+1);
      sl++;
    }
  
  if (zmin>=0.0)
    this->enableapex(0);
		    
  sl=this->gettrimtop();
  while(sl>=this->gettrimbottom()+1)
    {
      if ((this->getspline(sl))->getzlevel()>zmax)
	this->settrimtop(sl-1);
      sl=sl-1;
    }
}
// --------------------------------------------------------------
void PXFPBSplineStack::adjustnumberofsplines(int newnumber)
{
  if (debugmode)
    fprintf(stderr,"Adjusting %d (%d:%d) ---> %d \n",numsplines,trim_bottom,trim_top,newnumber);

  int oldnumsplines=numsplines;
  numsplines=newnumber;
  if (oldnumsplines!=numsplines)
    {
      if (oldnumsplines>numsplines)
	{
	  for (int i=numsplines;i<oldnumsplines;i++)
	    delete spline[i];
	}
      else
	{
	  float z=0.0;
	  if (oldnumsplines>0)
	    z=spline[oldnumsplines-1]->getzlevel()-spline[oldnumsplines-2]->getzlevel();
	  else
	    z=1.0;

	  for (int i=oldnumsplines;i<numsplines;i++)
	    {
	      spline[i]=new PXFPBSpline();
	      if (debugmode)
		fprintf(stderr,"Coping %d ---> %d \n",trim_top,i);
	      spline[i]->copypoints(spline[trim_top]);
	      if (i>0)
		spline[i]->setzlevel(spline[i-1]->getzlevel()+z);
	      else
		spline[i]->setzlevel(0.0);
	    }
	}
    }

  if (trim_top>numsplines)
    trim_top=numsplines-1;

  if (debugmode)
    fprintf(stderr,"Done\n");
}
// --------------------------------------------------------------
// Copy control points from another surface
// --------------------------------------------------------------
void PXFPBSplineStack::copypoints(PXFPBSplineStack* other,int ignoretrim)
{
  PX3Vector* v=other->getapex();
  for (int ia=0;ia<=2;ia++)
      apex.x[ia]=v->x[ia];
  hasapex=other->apexenabled();

  if (ignoretrim==0)
      {
	adjustnumberofsplines(other->getnumsplines()); 
	settrimbottom(other->gettrimbottom());
	settrimtop(other->gettrimtop());
	
	for (int i=0;i<numsplines;i++)
	    {
	      PXFPBSpline *spl= other->getspline(i);
	      spline[i]->copypoints(spl);
	    }
      }
  else
      {
	adjustnumberofsplines(other->gettrimtop()-other->gettrimbottom()+1);
	settrimbottom(0);
	settrimtop(numsplines-1);
	for (int i=0;i<numsplines;i++)
	    {
	      PXFPBSpline *spl= other->getspline(i+other->gettrimbottom());
	      spline[i]->copypoints(spl);
	    }
      }
}
// ------------------------------------------------------------------
//           Index Conversion
// --------------------------------------------------------------------------
int PXFPBSplineStack::indexbounded(int v,int np)
{
  if (np<0) 
      np=numsplines;

  if(v<0)       v=0;
  if(v>=np)     v=np-1;

  return v;
}

int PXFPBSplineStack::indexcyclic(int v,int np)
{
  while (v<0) v+=np;
  while (v>=np) v-=np;
  return v;
}
// --------------------------------------------------------------
void PXFPBSplineStack::setspline(PXFPBSpline* spl,int index)
{
  index=indexbounded(index);  
  if (index>=gettrimbottom() && index<=gettrimtop())
      spline[index]->copypoints(spl);

}
// --------------------------------------------------------------------------
int PXFPBSplineStack::sample(int sm,float step)
{
  int k=0;
  for (int i=trim_bottom;i<=trim_top;i++)
      k+=spline[i]->sample(sm,step);
  
  return k;
}
// --------------------------------------------------------------------------
int PXFPBSplineStack::subsample(int samples,float step)
{
  int k=0;
  for (int i=trim_bottom;i<=trim_top;i++)
      k+=spline[i]->subsample(samples,step);
  return k;
}
// --------------------------------------------------------------------------
int PXFPBSplineStack::resample(float ds_step,int npoints)
{
  int k=0;
  for (int i=trim_bottom;i<=trim_top;i++)
      k+=spline[i]->resample(ds_step,npoints);
  return k;
}
// --------------------------------------------------------------------------
int PXFPBSplineStack::changespacing(real smth,int npoints,float step)
{
  int k=0;
  for (int i=trim_bottom;i<=trim_top;i++)
      k+=spline[i]->changespacing(smth,npoints,step);
  return k;
}
// ------------------------------------------------------------------
//           I/O Functions
//
//        Save & Load -->              Surface Format (Control Points)
// ------------------------------------------------------------------
int  PXFPBSplineStack::Save(const char* filename)
{
  int ret=0;
  FILE* fout=fopen(filename,"w");
  if (fout!=NULL)
    {
      ret=Save(fout);
      fclose(fout);
    }
  if (debugmode)
      fprintf(stderr,"Saved in %s \n",filename);
  return ret;
}
// --------------------------------------------------------------
int  PXFPBSplineStack::Save(FILE* fout)
{
  fprintf(fout,  "#BFPSplineStackApex File , numsplines, has apex,trim_bottom,trim_top\n");
  fprintf(fout,  "#%d\n", numsplines);
  fprintf(fout,  "#%d\n", hasapex);
  fprintf(fout,  "#%d\n", trim_bottom);
  fprintf(fout,  "#%d\n", trim_top);
  fprintf(fout,  "%6.2f %6.2f %6.2f\n",apex.x[0],apex.x[1],apex.x[2]);
  fprintf(fout,  "#----------------------\n");
  for (int i=trim_bottom;i<=trim_top;i++)
      spline[i]->Save(fout);
  return(1);
}
// --------------------------------------------------------------
int PXFPBSplineStack::Load(const char* filename)
{
  int status=0;
  gzFile fin;

  fin=gzsuffixopen(filename,"r");
  if (fin!=NULL)
    {
      status=Load(fin);
      gzclose(fin);
    }
  
  if (status>0)
      {
	if (debugmode)
	  fprintf(stderr,"Loading from %s (numsplines=%2d) Trims=(%d:%d)\n",
		  filename,numsplines,trim_bottom,trim_top);
      }
  
  if (status)
      adjusttrimmed();
  
  if (debugmode && status==0)
      fprintf(stderr,"Failed to Load from %s\n",filename);

  return (status);
}

// --------------------------------------------------------------
int PXFPBSplineStack::Load(gzFile fin)
{
  char line[200];
  int status=0;
  int  filenumsplines;
  int headerok=0;
  int trimt,trimb;

  gzgets(fin,line,100);
  
  if (gzfindstringinstring(line, "#BFPSplineStack File"))
      {
	gzgets(fin,line,100);	sscanf(line,"#%d",&filenumsplines);
	gzgets(fin,line,100);	sscanf(line,"#%d",&trimb);
	gzgets(fin,line,100);	sscanf(line,"#%d",&trimt);
	gzgets(fin,line,100);
	hasapex=0;
	headerok=1;
      }
  else if (gzfindstringinstring(line,"#BFPSplineStackApex File"))
      {
	gzgets(fin,line,100); sscanf(line,"#%d",&filenumsplines);
	gzgets(fin,line,100); sscanf(line,"#%d",&hasapex);
	hasapex=hasapex>0;
	gzgets(fin,line,100); sscanf(line,"#%d\n",&trimb);
	gzgets(fin,line,100); sscanf(line,"#%d\n",&trimt);
	gzgets(fin,line,100); sscanf(line,"%f %f %f\n",&apex.x[0],&apex.x[1],&apex.x[2]);
	gzgets(fin,line,100);
	headerok=1;
      }

  if (debugmode)
    {
      fprintf(stderr,"Headerok=%d, filenumsplines=%d \n",headerok,filenumsplines);
      fprintf(stderr,"Trimbottom=%d, trimtop=%d\n",trimb,trimt);
    }
  
  if (headerok==1)
      {
	adjustnumberofsplines(filenumsplines); 
	settrimbottom(0);
	settrimtop(trimt);
	settrimbottom(trimb);
	status=1;
	
	for (int i=trimb;i<=trimt;i++)
	  {
	    if (debugmode)
	      fprintf(stderr,"Reading spline %d\n",i);
	    spline[i]->Load(fin);
	  }
	
	if (hasapex==0)
	  autosetapex(0);
      }
  return(status);
}
// --------------------------------------------------------------
int PXFPBSplineStack::fittocontourstack(PXContourStack* stack,float smooth,float csmooth,
					int npoints,int closed)
{  
  float dz=stack->getzlevel(stack->getnumcontours()-1)-stack->getzlevel(stack->getnumcontours()-2);
  
  int offset=int(stack->getzlevel(0)/dz+0.5);

  adjustnumberofsplines(stack->getnumcontours()+offset);
  trim_bottom=offset;
  trim_top=trim_bottom+stack->getnumcontours()-1;
  int success=0;

  /*  offset-=1;

  if (offset<0)
      offset=0;*/

  fprintf(stderr,"offset=%d (dz=%6.2f) trim_bottom=%d trim_top=%d numsplines=%d\n",
	  offset,dz,trim_bottom,trim_top,numsplines);

  for (int kk=0;kk<stack->getnumcontours();kk++)
      fprintf(stderr,"%5.2f ",stack->getzlevel(kk));
  fprintf(stderr,"\n");


  for(int spl=0;spl<stack->getnumcontours();spl++)
      {
	//	if (debugmode)
	fprintf(stderr,"fitting Spl %d from Cntr %d (z=%5.2f)\t",spl+offset,spl,stack->getzlevel(spl));
	success+=spline[spl+offset]->fittocontour(stack->getcontour(spl),
						  smooth,csmooth,npoints,closed);
	spline[spl+offset]->setzlevel(stack->getzlevel(spl));
	spline[spl+offset]->ensureAnticlockwise();
	fprintf(stderr," (nc=%d)\n", (int)spline[spl+offset]->getnumcontrolpoints());
      }
	
  success=(success==stack->getnumcontours());
  autosetapex(0);
  hasapex=0;
  return success;
}
// --------------------------------------------------------------
void PXFPBSplineStack::adjusttrimmed()
{
  PXFPBSpline* tempspl=new PXFPBSpline();
  tempspl->setcircle(4,5.0,50.0,50.0);
  
  if (gettrimbottom()>0)
    {
      float z1=spline[gettrimbottom()+1]->getzlevel()-spline[gettrimbottom()]->getzlevel();
      for(int ia=gettrimbottom()-1;ia>=0;ia-=1)
	{
	  spline[ia]->copypoints(tempspl);
	  spline[ia]->setzlevel(spline[ia+1]->getzlevel()-z1);
	}
    }
  if (gettrimtop()<numsplines-1)
    {
      float z2=spline[gettrimtop()]->getzlevel()-spline[gettrimtop()-1]->getzlevel();
      for (int ib=gettrimtop()+1;ib<numsplines-1;ib++)
	{
	  spline[ib]->copypoints(tempspl);
	  spline[ib]->setzlevel(spline[ib-1]->getzlevel()+z2);
	}
    }
  delete tempspl;
}

// --------------------------------------------------------------
real PXFPBSplineStack::getsqdist(float x1,float x2,float y1,float y2)
{
  return pow(x1-x2,float(2.0))+pow(y1-y2,float(2.0));
}
// --------------------------------------------------------------
PXContour* PXFPBSplineStack::compareSplines(PXFPBSpline* spl1,PXFPBSpline* spl2,int np)
{
  PXContour* cntr=new PXContour();
  real  ds=1.0/float(np);
  //  spl1->resample(np,ds/10.0);
  //  spl2->resample(np,ds/10.0);
  real* x1=new real[np]; real* y1=new real[np];
  real* x2=new real[np]; real* y2=new real[np];
  int* corresp1=new int[np];
  int* corresp2=new int[np];

  float s=0.0;
  int i=0,j=0;
  
  // Copy Points in Arrays for speed 
  for(i=0;i<np;i++)
      {
	spl1->getposition(s,x1[i],y1[i]);
	spl2->getposition(s,x2[i],y2[i]);
	s=s+ds;
      }

  real dist=0.0,mindist=10000000.0;
  int shift=0;
  
  // Preliminary Test for range to look for rough shift
  // --------------------------------------------
  for(i=0;i<np;i+=2)
      {
	dist=0;
	for (j=0;j<np;j+=2)
	    {
	      int sj=indexcyclic(j+i,np);
	      dist+=getsqdist(x1[j],x2[sj],y1[j],y2[sj]);
	    }
	if (dist<mindist)
	    {
	      mindist=dist; 
	      shift=i;
	    }
      }
  
  //  cerr << "Shift : " << shift << "(" << mindist << ")\n";

  // Loop 1 from spline to spline2
  // -----------------------------
  for (i=0;i<np;i++)
      {
	real mindist1=100000000.0,mindist2=100000000.0;
	int  i1=i+shift,i2=i-shift;
	int  index1=i1,index2=i2;
	for(j=-5;j<=5;j++)
	    {
	      int jp=indexcyclic(j+i1,np);
	      int jm=indexcyclic(j+i2,np);
	      
	      real dist1=getsqdist(x1[i],x2[jp],y1[i],y2[jp]);
	      if (dist1<mindist1)
		  { index1=jp;  mindist1=dist1; }

	      real dist2=getsqdist(x1[jm],x2[i],y1[jm],y2[i]);
	      if (dist2<mindist2)
		  { index2=jm;  mindist2=dist2; }
	    }
	corresp1[i]=index1; corresp2[i]=index2;
	//	fprintf(stderr,"%2d: ( %3d %3d) (%5.2f %5.2f) \n ",i,corresp1[i],corresp2[i],mindist1,mindist2);
      }

  //  fprintf(stderr,"-------------------\n\n");
  //  cerr << "Failed :" ;
  int count=0;
  for (i=0;i<np;i++)
      {
	int ind1=corresp1[i];
	int ind2=corresp2[ind1];
	if (ind2==i)
	    count++;
	else
	    {
	      //      cerr << i << " ";
	      corresp1[i]=-1;
	    }
      }
  //  cerr << "\n";
  if (count<2)
      {
	fprintf(stderr,"Exiting \n");
	exit(1);
      }
  else
      {
	int j1=0,j2=0;
	for (i=0;i<count;i++)
	    {
	      while(corresp1[indexcyclic(j1,np)]<0)  j1++;
	      j2=j1+1;
       	      while(corresp1[indexcyclic(j2,np)]<0)  j2++;


	      if (j2-j1>1)
		  {
		    // fprintf(stderr,"\t%2d, ***** , 1->2  *** %2d \t\t2->1  *** %2d\n",
		    //indexcyclic(j1,np),corresp1[indexcyclic(j1,np)],corresp2[indexcyclic(j1,np)]);

		    //  fprintf(stderr,"Interpolating in %d:%d\n",j1,j2);
		    // DO for table 1 first
		    for (j=j1+1;j<j2;j++)
			{
			  float fraction=float(j-j1)/float(j2-j1);
			  float gap1=float(corresp1[indexcyclic(j2,np)]-corresp1[indexcyclic(j1,np)]);
			  if (gap1<0) gap1+=float(np);
			  float gap2=float(corresp2[indexcyclic(j2,np)]-corresp2[indexcyclic(j1,np)]);
			  if (gap2<0) gap2+=float(np);


			  int   c1=int(0.5+fraction*gap1)+corresp1[indexcyclic(j1,np)];
			  corresp1[indexcyclic(j,np)]=indexcyclic(c1,np);
			  // fprintf(stderr,"\t%2d, %5.2f , 1->2  %3.0f %2d\t",
			  //indexcyclic(j,np),fraction,gap1,corresp1[indexcyclic(j,np)]);

			  int   c2=int(0.5+fraction*gap2)+corresp2[indexcyclic(j1,np)];
			  corresp2[indexcyclic(j,np)]=indexcyclic(c2,np);
			  // fprintf(stderr,"\t2->1  %3.0f %2d\n",gap2,corresp2[indexcyclic(j,np)]);
			}
		    // fprintf(stderr,"\t%2d, ***** , 1->2  *** %2d \t\t2->1  *** %2d\n",
		    //indexcyclic(j2,np),corresp1[indexcyclic(j2,np)],corresp2[indexcyclic(j2,np)]);
		  }
	      j1=j2;
	    }
	//	cerr << "+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_++_+_+_+_+_+_\n";
	int index=1,old,current;
	for (i=0;i<np;i++)
	    {
	      //fprintf(stderr,"Flag %2d: ( %3d %3d ) \n",i,corresp1[i],corresp2[i]);
	      float s1=float(i*ds);
	      current=corresp1[i];
	      float s2=float(current*ds);
	      if (i>0 && i<(np-1) && old!=(current-1))
		  {
		    for (j=old+1;j<current;j++)
			{
			  float s2a=float(j*ds);
			  int cur2=corresp2[j];
			  float s1a=float(cur2*ds);
			  cntr->Add(s1a,s2a);
			  // fprintf(stderr,"--> %2d %2d %2d ( %4.2f %4.2f )\n",index,cur2
			  //,indexcyclic(j,np),s1a,s2a);
			  index++;
			}
		  }
	      cntr->Add(s1,s2);
	      // fprintf(stderr,"    %2d %2d %2d ( %4.2f %4.2f )\n",index,i,current,s1,s2);
	      old=current;
	      index++;
	      if (i==(np-1))
		  {
		    int add=1;
		    
		    if (current==corresp1[0]-1 || current==corresp1[0]-np)
			add=0;
		    else
			{
			  current=current=np;
			  if (current==corresp1[0]-1 || current==corresp1[0]-np)
			      add=0;
			}
		    if (add)
			{
			  for(j=current+1;j<corresp1[0];j++)
			      {
				int ja=indexcyclic(j,np);
				float s2a=float(ja*ds);
				int cur2=corresp2[ja];
				float s1a=float(cur2*ds);
				      cntr->Add(s1a,s2a);
				// fprintf(stderr,"--> %2d %2d %2d ( %4.2f %4.2f )\n",index,cur2,ja,s1a,s2a);
				index++;
			      }
			  
			}
		  }
	    }
	// fprintf(stderr,"Numpoints = %d \n",cntr->getnumpoints());
      }
  
  // Deleting 
  delete [] x1;delete [] x2;delete [] y1;delete [] y2;
  delete [] corresp1; delete [] corresp2;
  return cntr;
}
// --------------------------------------------------------------
void PXFPBSplineStack::setellipticalcylinder(int np,int numslices,float radiusx,float radiusy,
					     float x,float y,float zscale,float bottomz)
{
  PXFPBSpline* tmp=new PXFPBSpline();
  tmp->setellipse(np,radiusx,radiusy,x,y);
  numslices=Irange(numslices,2,PXFPBSPLINESTACK_MAXLEVELS);
  adjustnumberofsplines(numslices);
  
  numsplines=numslices;
  trim_bottom=0;
  trim_top=numslices-1;
  
  for(int i=0;i<numsplines;i++)
      {
	if (debugmode)
	    fprintf(stderr,"Spline %d\n",i);
	//spline[i]=new PXFPBSpline();
	spline[i]->copypoints(tmp);
	spline[i]->setzlevel(bottomz+float(i)*zscale);
      }
  delete tmp;
  autosetapex(0);
}
// --------------------------------------------------------------
void PXFPBSplineStack::setfromboundingbox(int np,float zfactor,
					float bx,float by,float bz,float bw,float bh,
					float tx,float ty,float tz,float tw,float th)
{
  for (int i=0;i<numsplines;i++)
      {
	float zl=zfactor*float(i);
	spline[i]->setzlevel(zl);

	float ratio=((float(i)-bz)/(tz-bz));
	float xl=bx+ratio*(tx-bx);
	float yl=by+ratio*(ty-by);
	float wl=bw+ratio*(tw-bw);
	float hl=bh+ratio*(th-bh);
	spline[i]->setellipse(np,wl,hl,xl,yl);
      }
  settrimbottom(int(bz));
  settrimtop(int(tz));
  autosetapex(0);
}
// --------------------------------------------------------------
PXContourStack* PXFPBSplineStack::exporttocontourstack(real dist,real zdist,int mode)
{
  int zbet=0;
  //  fprintf(stderr,"Exprting to Contour Stack (2) %.2f %.2f\n",dist,zdist);

  if (gettrimbottom()<gettrimtop())
    {
      float zsize=((getspline(gettrimbottom()+1)->getzlevel())-
		   (getspline(gettrimbottom()  )->getzlevel())
		   );

      if (fabs(zsize)<0.0001)
	{
	  zbet=1;
	}
      else
	{
	  zdist=zdist/zsize; 
	  zbet=0;
	  if (zdist<0.05)  zdist=0.05;

	  if (zdist<1.0) 
	    zbet=int(0.5+1.0/zdist)-1;
	  //fprintf(stderr,"zdist=%.2f zbet=%d\n",zdist,zbet);
	}

      //fprintf(stderr,"dist=%.2f zbet=%d (zdist=%.3f zsize=%.3f) mode=%d\n",dist,zbet,zdist,zsize,mode);
    }

  return exporttocontourstack(dist,zbet,mode);
}
// --------------------------------------------------------------
PXContourStack* PXFPBSplineStack::exporttocontourstack(real dist,int zbetween,int mode)
{
  
  /*PXFPBSplineStack* temp=new PXFPBSplineStack(this,zbetween,mode,0);
  PXContourStack* stack=temp->exporttocontourstack(dist);
  delete temp; */
  
  PXContourStack* temp=exporttocontourstack(dist*0.2);
  PXContourStack* stack=new PXContourStack(temp,zbetween,mode);
  stack->Equispace(dist);
  delete temp;   
  return stack;
}
// --------------------------------------------------------------
PXContourStack* PXFPBSplineStack::exporttocontourstack(real dist)
{
  PXContourStack* stack=new PXContourStack(gettrimtop()-gettrimbottom()+1+(hasapex>0));

  if (hasapex>0)
      {
	PXContour* apexc=stack->getcontour(0);
	apexc->Add(apex.x[0],apex.x[1]);
	stack->setzlevel(0,apex.x[2]);
      }

  for(int j=gettrimbottom();j<=gettrimtop();j++)
      {
	if (debugmode)
	    fprintf(stderr,"Exporting %d ",j);
	PXContour* cntr=stack->getcontour(j-gettrimbottom()+(hasapex>0));
	PXContour* tempcntr=spline[j]->exporttocontour(dist);
	cntr->copypoints(tempcntr);
	cntr->ensureAnticlockwise();
	cntr->setclosed(1);
	delete tempcntr;
	stack->setzlevel(j-gettrimbottom()+(hasapex>0),spline[j]->getzlevel());
	if (debugmode)
	    fprintf(stderr,"(%5.2f) ",spline[j]->getzlevel());
      }
  if (debugmode)
      fprintf(stderr,"Done\n");
  return stack;
}
// --------------------------------------------------------------
int PXFPBSplineStack::polarpoint(float zl,int slice,real middlex,int pass,
				 real& x,real& y)
{
  int status=0;
  PXFPBSpline* spl=getspline(slice);
  float theta,radius,minx,maxx;
  
  switch(pass)
      {
      case 0:
	theta=(getspline(slice))->getzlevel()-180.0;
	minx=-1000000000.0;maxx=middlex-0.01;
	break;
      case 1:
	theta=(getspline(slice))->getzlevel();
	minx=middlex+0.01;
	maxx=100000000000.0;
	break;
      }

  radius=spl->findIntersectY(zl,minx,maxx);
  float ds=0.05;  int att=0; float dy=0.05;
  while(radius==-1111.0 && att<5)
      {
	ds=ds*0.5;
	att++;
	radius=spl->findIntersectY(zl,minx,maxx,ds,dy);
	dy=dy*0.5;
      }
  float r=fabs(radius-middlex);
  float t=theta*M_PI/180.0;
  
  if (radius!=-1111.0)
      {
	x=(middlex+r*cos(t));
	y=(middlex+r*sin(t));
	status=1;
      }
  return status;
}

// --------------------------------------------------------------
float PXFPBSplineStack::getpolarbottomz()
{
  float bottomz=0.0;
  
  for (int i=gettrimbottom();i<=gettrimtop();i++)
    {
      float z=getspline(i)->getcontroly(0);
      for (int j=1;j<spline[i]->getnumpoints();j++)
	z=Fmin((spline[i])->getcontroly(j),z);
      if (i==gettrimbottom())
	bottomz=z;
      else
	bottomz=Fmax(z,bottomz);
    }
  return bottomz;

}

float PXFPBSplineStack::getpolartopz()
{
  float topz=0.0;
  for (int i=gettrimbottom();i<=gettrimtop();i++)
    {
      float z=getspline(i)->getcontroly(0);
      for (int j=1;j<spline[i]->getnumpoints();j++)
	z=Fmax(z,(spline[i])->getcontroly(j));
      if (i==gettrimbottom())
	topz=z;
      else
	topz=Fmin(z,topz);
    }
  return topz;
}
// --------------------------------------------------------------
PXContourStack* PXFPBSplineStack::polarexporttocontourstack(real middlex,float zsp,
							    float bottomz,float topz)
{
  if (bottomz==-1000.0)
    bottomz=getpolarbottomz()+3.0;
  
  if (topz==-1000.0)
    topz=getpolartopz()-3.0;
  
  if (topz<bottomz)
      {
	real t=topz;
	topz=bottomz;
	bottomz=t;
      }

  bottomz=float(int(bottomz+0.5));
  topz=float(int(topz+0.5));

  int  numslices=int((topz-bottomz)/zsp);

  PXContourStack* cstack=new PXContourStack(numslices);
  numslices=cstack->getnumcontours();

  fprintf(stderr,"\n\nBottomz=%6.2f, Topz=%6.2f, numslices=%d, zsp=%5.3f\n",
	  bottomz,topz,numslices,zsp);

  int i;
  for (i=0;i<numslices;i++)
      {
	float zl=bottomz+float(i)*zsp;//(real(i)/real(numslices-1))*(topz-bottomz);
	cstack->setzlevel(i,zl);
	PXContour* cntr=cstack->getcontour(i);

	fprintf(stderr,"i=%d zl=%6.3f , ",i,zl);


	for (int pass=0;pass<=1;pass++)
	    {
	      for (int sl=gettrimbottom();sl<=gettrimtop();sl++)
		  {
		    real x,y;
		    if (polarpoint(zl,sl,middlex,pass,x,y))
			cntr->Add(x,y);
		    else
			{
			  sl=gettrimtop()+1;
			  pass=2;
			}
		  }
	    }
      }

  int numgood=0;
  int maxp=0;
  for (i=0;i<cstack->getnumcontours();i++)
      maxp=Imax((cstack->getcontour(i))->getnumpoints(),maxp);

  for (i=0;i<cstack->getnumcontours();i++)
      {
	if ((cstack->getcontour(i))->getnumpoints()==maxp)
	    numgood++;
      }

  if (numgood==cstack->getnumcontours())
      {
	fprintf(stderr,"Polar Contour OK\n");
	return cstack;
      }

  PXContourStack* cstack2=new PXContourStack(numgood);

  int index1=0,index2=0;
  while (index1<numgood)
      {
	while((cstack->getcontour(index2))->getnumpoints()!=maxp)
	    index2++;
	
	(cstack2->getcontour(index1))->copypoints(cstack->getcontour(index2));
	cstack2->setzlevel(index1,cstack->getzlevel(index2));
	index1++;
	index2++;
      }

  fprintf(stderr," middlex=%5.2f,maxp=%d, old num contours %d returning %d\n",
	  middlex,maxp,cstack->getnumcontours(),numgood);
  delete cstack;
  return cstack2;

}
// --------------------------------------------------------------
float PXFPBSplineStack::findnearestneighbour(float& xn,float& yn,float &zn,float dt,int dbg)
{
  float dist=0.0;
  if (zn<=spline[gettrimbottom()]->getzlevel())
      {
	dist=spline[gettrimbottom()]->findnearestneighbour(xn,yn,dt);
      }
  else if (zn>=spline[gettrimtop()]->getzlevel())
      {
	dist=spline[gettrimtop()]->findnearestneighbour(xn,yn,dt);
      }
  else
      {
	int i=gettrimbottom();
	int found=0;
	while (found==0 && i<gettrimtop())
	    {
	      if (zn==spline[i]->getzlevel())
		  {
		    dist=spline[i]->findnearestneighbour(xn,yn,dt);
		    found=1;
		  }
	      if (zn>spline[i]->getzlevel() && zn<spline[i+1]->getzlevel())
		  {
		    real x0=xn,y0=yn,x1=xn,y1=yn,d0=1.0,d1=1.0;
		    float dz=(spline[i+1]->getzlevel()-spline[i]->getzlevel());
		    float w0=(spline[i+1]->getzlevel()-zn)/dz;
		    float w1=(zn-spline[i]->getzlevel())/dz;

		    d0=spline[i]->findnearestneighbour(x0,y0,dt,dbg);
		    d1=spline[i+1]->findnearestneighbour(x1,y1,dt,dbg);

		    float xf=(w1*x1+w0*x0);
		    float yf=(w1*y1+w0*y0);
		    float zf=(w1*spline[i+1]->getzlevel()+w0*spline[i]->getzlevel());
		    if (dbg==1)
			{
			  fprintf(stderr," Point (%5.2f %5.2f %5.2f)\n",xn,yn,zn);
			  fprintf(stderr," Neighbours (%5.2f,%5.2f,%5.2f) and (%5.2f,%5.2f,%5.2f)\n",
				  x0,y0,spline[i]->getzlevel(),x1,y1,spline[i+1]->getzlevel());
			  fprintf(stderr," Distances %5.2f %5.2f , Weight %5.2f %5.2f \n",
				  d0,d1,w0,w1);
			  fprintf(stderr," Result (%5.2f,%5.2f,%5.2f)\n",xf,yf,zf);
			  spline[i]->SaveContour("/intdisk2b/experiments/papad/simulation/cylinder/splinei.cntr",0.1);
			  spline[i+1]->SaveContour("/intdisk2b/experiments/papad/simulation/cylinder/splineip.cntr",0.1);
			}

		    dist=sqrt(pow(xn-xf,float(2.0))+pow(yn-yf,float(2.0))+pow(zn-zf,float(2.0)));
		    xn=xf; yn=yf; zn=zf;
		  }
	      i++;
	    }
      }
  return dist;
}
// --------------------------------------------------------------
int PXFPBSplineStack::equitrimstacks(PXFPBSplineStack* stack1,PXFPBSplineStack* stack2,int trim[4])
{
  // Store Old Trims
  trim[0]=stack1->gettrimbottom();
  trim[1]=stack1->gettrimtop();
  trim[2]=stack2->gettrimbottom();
  trim[3]=stack2->gettrimtop();

  int found=0,i=trim[0]-1;

  while(i<=trim[1] && found==0)
      {
	i++;
	float z1=(stack1->getspline(i))->getzlevel();
	int j=trim[2]-1;
	while (j<=trim[3] && found==0)
	    {
	      j++;
	      float z2=(stack2->getspline(j))->getzlevel();
	      if (z2==z1)
		  {
		    found=1;
		    stack1->settrimbottom(i);
		    stack2->settrimbottom(j);
		    /*if (debugmode)
			{
			  fprintf(stderr,"Common Bot Z at Slice1=%2d , Slice2=%2d (z1=%5.2f z2=%5.2f )\n",
				  i,j,z1,z2);
			}*/
		  }
	    }
      }

  if (found==0)
      return 0;

  found=0;
  i=trim[1]+1;
 
  while(i>=stack1->gettrimbottom() && found==0)
      {
	i=i-1;
	float z1=(stack1->getspline(i))->getzlevel();
	int j=trim[3]+1;
	while (j>=stack2->gettrimbottom() && found==0)
	    {
	      j=j-1;
	      float z2=(stack2->getspline(j))->getzlevel();
	      if (z2==z1)
		  {
		    found=1;
		    stack1->settrimtop(i);
		    stack2->settrimtop(j);
		    /*if (debugmode)
		      {
		      fprintf(stderr,"Common Top Z at Slice1=%2d , Slice2=%2d (z1=%5.2f z2=%5.2f )\n",
		      i,j,z1,z2);
		      }*/
		  }
	    }
      }
  
  return found;  	
}
// --------------------------------------------------------------
void PXFPBSplineStack::untrimstacks(PXFPBSplineStack* stack1,PXFPBSplineStack* stack2,int trim[4])
{
  stack1->settrimbottom(trim[0]);
  stack1->settrimtop(trim[1]);
  stack2->settrimbottom(trim[2]);
  stack2->settrimtop(trim[3]);

}
// --------------------------------------------------------------
int PXFPBSplineStack::createstackset(PXFPBSplineStack* newstack[],
				     PXFPBSplineStack *stack1,PXFPBSplineStack *stack2,
				     int numstacks,int NP)
{
  int trims[4];
  equitrimstacks(stack1,stack2,trims);
  
  int nsp=stack1->gettrimtop()-stack1->gettrimbottom()+1;

  for (int ia=0;ia<numstacks;ia++)
      newstack[ia]=new PXFPBSplineStack(nsp);
  

  char line[100];

  for (int i=0;i<nsp;i++)
      {
	PXContour* cntr1=(stack1->getspline(i+stack1->gettrimbottom()))->exporttocontour(0.8);
	PXContour* cntr2=(stack2->getspline(i+stack2->gettrimbottom()))->exporttocontour(0.8);
	
	PXChamferDistanceMap* map1=new PXChamferDistanceMap(cntr1,0);
	map1->generateMap(1,0,0.0);
	PXChamferDistanceMap* map2=new PXChamferDistanceMap(cntr2,0);
	map2->generateMap(1,0,0.0);

	/*	sprintf(line,"inner%02d.cntr",i+1);
		stack1->getspline(i+stack1->gettrimbottom())->SaveContour(line,float(2.0));
		fprintf(stderr,"%2d : %s ,",i+1,line);
		sprintf(line,"outer%02d.cntr",i+1);
		fprintf(stderr,"%s : ",line);
		stack2->getspline(i+stack2->gettrimbottom())->SaveContour(line,float(2.0));*/
	
	for (int j=0;j<numstacks;j++)
	    {
	      int fraction1=numstacks-j;
	      int fraction2=j+1;

	      if (j==0)
		  fraction1++;

	      PXChamferDistanceMap* map=new PXChamferDistanceMap(map1,map2,fraction1,fraction2);
	      PXContour* cntr=map->extractContour();
	      
	      (newstack[j]->getspline(i))->fittocontour(cntr,1.0,0.0);
	      (newstack[j]->getspline(i))->setzlevel(stack1->getspline(i+stack1->gettrimbottom())->getzlevel());
	      (newstack[j]->getspline(i))->ensureAnticlockwise();
	      delete map;

	      /*sprintf(line,"middle%02d%02d.cntr",i+1,j+1);
		fprintf(stderr,"%s(%3d-->%3d), ",line,cntr->getnumpoints(),
		(newstack[j]->getspline(i))->getnumpoints());
		(newstack[j]->getspline(i))->SaveContour(line,float(2.0));
		*/
	      
	      delete cntr;
	    }
	//	fprintf(stderr,"\n");

	if (i>-1)
	    {
	      float* px1=new float[NP+1];   float* py1=new float[NP+1];
	      float* px2=new float[NP+1];   float* py2=new float[NP+1];
	      PXContour *mesh[10];
	      
	      mesh[0]=(stack1->getspline(i+stack1->gettrimbottom()))->exporttoNcontour(NP);
	      for (int mm=1;mm<=numstacks;mm++)
		  mesh[mm]=(newstack[mm-1]->getspline(i))->exporttoNcontour(NP);
	      mesh[numstacks+1]=(stack2->getspline(i+stack2->gettrimbottom()))->exporttoNcontour(NP);
	      
	      
	      for (int k=1;k<=numstacks+1;k++)
		  PXContourStack::aligncontours(mesh[k-1],mesh[k]);
	      
	      if (i==8)
		  {
		    mesh[0]->Save4Mesh("in_curve");
		    mesh[(numstacks+2)/2]->Save4Mesh("curve");
		    mesh[numstacks+1]->Save4Mesh("out_curve");
		  }

	      FILE* fout;
	      sprintf(line,"mesh.%02d",i);
	      fout=fopen(line,"w");
	      if (fout!=NULL)
		  {
		    fprintf(stderr,"Opened : %s\n",line);
		    for (int ia=0;ia<=numstacks;ia++)
			{
			  fprintf(stderr,"Numpoints = %d,%d\n",
				  mesh[ia]->getnumpoints(),mesh[ia+1]->getnumpoints());
			  int npi;
			  mesh[ia]->copy2vector(px1,py1,npi);
			  mesh[ia+1]->copy2vector(px2,py2,npi);
			  float x0,x1,x2,x3,y0,y1,y2,y3;			  
			  for(int j=0;j<NP;j++)
			      {
				int jp=j+1;
				if (jp==NP)
				    jp=1;

				
				x0=px1[j];  y0=py1[j];
				x1=px2[j];  y1=py2[j];
				x2=px1[jp]; y2=py1[jp];
				x3=px2[jp]; y3=py2[jp];
				
				fprintf(fout," %5.2f %5.2f \n",x0,y0);
				fprintf(fout," %5.2f %5.2f \n",x1,y1);
				fprintf(fout," %5.2f %5.2f \n",x3,y3);
				fprintf(fout," %5.2f %5.2f \n",x2,y2);
				fprintf(fout," %5.2f %5.2f \n",x0,y0);
			      }
			  fprintf(fout," %5.2f %5.2f \n",x1,y1);
			  fprintf(fout," %5.2f %5.2f \n",x3,y3);
			}
		    fclose(fout);
		    fprintf(stderr,"Saved in %s \n",line);
		  }
	      for (int l=0;l<=numstacks+1;l++)
		  delete mesh[l];
	      delete [] px1;   delete [] py1;
	      delete [] px2;   delete [] py2;      
	    }
	delete map1;
	delete map2;
	delete cntr1;
	delete cntr2;

      }

  untrimstacks(stack1,stack2,trims);
  return nsp;
}
// --------------------------------------------------------------
int PXFPBSplineStack::temporallySmoothApex(PXFPBSplineStack** stacks,int numstacks,
					   int areacheck,float sigma,int closed)
{
  int doareas=(areacheck>0);
  float* area,meanarea=0.0,maxarea=0.0001;

  if (doareas)
      {
	area=new float[numstacks];
	for (int ia=0;ia<numstacks;ia++)
	    {
	      int bo=stacks[ia]->gettrimbottom();
	      PXContour* temp=stacks[ia]->getspline(bo)->exporttoNcontour(10,1);
	      temp->setclosed(1);
	      area[ia]=sqrt(temp->getarea());
	      meanarea+=area[ia]/float(numstacks);
	      maxarea=Fmax(maxarea,area[ia]);
	      delete temp;
	    }
	maxarea=Fmax(maxarea,1.5*meanarea);
      }

  PXContour* cntr1=new PXContour();
  PXContour* cntr2=new PXContour();
  //fprintf(stderr,"Smoothing %d stack apexes\n",numstacks);

  int i;
  for (i=0;i<numstacks;i++)
      {
	if (!stacks[i]->apexenabled())
	    stacks[i]->autosetapex();

	PX3Vector* vec0=stacks[i]->getapex();
	PX3Vector vec;
	vec.copy(vec0);
	

	if (doareas)
	    {
	      float zp=stacks[i]->getspline(stacks[i]->gettrimbottom())->getzlevel();
	      float zp2=stacks[i]->getspline(stacks[i]->gettrimbottom()+1)->getzlevel();
	      fprintf(stderr,"Original z=%6.3f area=%6.3f maxarea=%5.3f newz=",
		      vec.x[2],area[i],maxarea);
	      vec.x[2]=zp-0.75*(zp2-zp)*(area[i]/maxarea);
	      fprintf(stderr,"%6.3f\n",vec.x[2]);
	    }

	real x=vec.x[0];
	real y=vec.x[1];
	real z=vec.x[2];
	cntr1->Add(z,z);
	cntr2->Add(x,y);
      }

  cntr1->setclosed(closed);
  cntr2->setclosed(closed);

  cntr1->Smooth(sigma,3);
  cntr2->Smooth(sigma,3);

  PXContourPoint *pts1=cntr1->gettoppoint();
  PXContourPoint *pts2=cntr2->gettoppoint();

  for (i=0;i<numstacks;i++)
      {
	stacks[i]->enableapex(1);
	stacks[i]->setapex(pts2->x,pts2->y,pts1->x);
	pts2=pts2->next;
	pts1=pts1->next;
      }
  
  fprintf(stderr,"Smoothing %d stack apexes (area=%d) done\n",numstacks,doareas);
  delete cntr1;
  delete cntr2;

  if (doareas)
      delete [] area;
  return 1;
}

// --------------------------------------------------------------
void PXFPBSplineStack::autosetapex(int active)
{
  real cx,cy,cz;
  PXFPBSpline* spl=getspline(gettrimbottom());
  PXFPBSpline* spl2=getspline(gettrimbottom()+1);
  spl->getcentroid(cx,cy,cz,0.1);
  
  cx=quantize(cx,1);
  cy=quantize(cy,1);
  cz=quantize(cz-0.5*(spl2->getzlevel()-cz),1);
  setapex(cx,cy,cz);

  if (active)
    enableapex(1);
}
// --------------------------------------------------------------

