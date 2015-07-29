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



/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

 see vtkpxBaseCurve.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "pxcontour.h"
#include "pxutil.h"
#include "vtkAppendPolyData.h"
#include "vtkCellArray.h"
#include "vtkCleanPolyData.h"
#include "vtkContourFilter.h"
#include "vtkDiskSource.h"
#include "vtkGlyph3D.h"
#include "vtkImageData.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageGradient.h"
#include "vtkImageGradientMagnitude.h"
#include "vtkImageMedian3D.h"
#include "vtkImageToStructuredPoints.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkOutlineFilter.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataReader.h"
#include "vtkSphereSource.h"
#include "vtkTransform.h"
#include "vtkTubeFilter.h"
#include "vtkpxBaseCurve.h"
#include "vtkpxCrossHairSource.h"
#include "vtkpxImageExtract.h"
//------------------------------------------------------------------------------
vtkpxBaseCurve* vtkpxBaseCurve::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxBaseCurve");
  if(ret)
    {
    return (vtkpxBaseCurve*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxBaseCurve;
}

// Construct object with no children.
vtkpxBaseCurve::vtkpxBaseCurve()
{
  this->PointScale=1.0;
  this->ShowControls=0;
  this->ControlsDisplayMode=0;
  this->ShowBoundingBox=0;
  this->ShowCurve=1;
  this->NumPoints=0;
  this->ClosedCurve=0;

  for (int i=0;i<=2;i++)
    {
      this->Spacing[i]=1.0;
      this->Origin[i]=0.0;
    }

  this->Points=vtkPoints::New();
  this->Points->SetNumberOfPoints(20);
  this->Points->SetPoint(0,0.0,0.0,0.0);
}

/* -------------------------------------------------------------------------*/
 
vtkpxBaseCurve::~vtkpxBaseCurve()
{
  this->Points->Delete();
}

/* -------------------------------------------------------------------------*/
void vtkpxBaseCurve::Copy(vtkpxBaseCurve* other)
{
  this->SetSpacing(other->GetSpacing());
  this->SetOrigin(other->GetOrigin());
  
  int np=other->GetNumPoints();
  if (np>this->NumPoints)
    this->Points->SetNumberOfPoints(np+10);
  
  this->NumPoints=np;
  double x[3];
  for (int i=0;i<np;i++)
    {
      other->GetPoints()->GetPoint(i,x);
      this->Points->SetPoint(i,x);
    }

  this->Modified();
  this->Update();
}
/* -------------------------------------------------------------------------*/
void vtkpxBaseCurve::GetPoint(int index,double& x,double& y,double &z)
{
  index=Irange(index,0,this->NumPoints-1);
  double xp[3];
  this->Points->GetPoint(index,xp);
  x=xp[0];  y=xp[1];  z=xp[2];
}
/* -------------------------------------------------------------------------*/
void vtkpxBaseCurve::SetPoint(int index,double x,double y,double z,
			      int scaled)
{
  index=Irange(index,0,this->NumPoints-1);
  if (scaled)
      {
	x=(x-Origin[0])/Spacing[0];
	y=(y-Origin[1])/Spacing[1];
	z=(z-Origin[2])/Spacing[2];
      }
  
  this->Points->SetPoint(index,x,y,z);
  this->Modified();
}
/* -------------------------------------------------------------------------*/
void vtkpxBaseCurve::AddPoint(double x,double y,double z,int scaled)
{
  if (scaled)
    {
      x=(x-Origin[0])/Spacing[0];
      y=(y-Origin[1])/Spacing[1];
      z=(z-Origin[2])/Spacing[2];
    }
  
  int index=this->NumPoints;

  double x1,y1,z1;
  this->GetPoint(index-1,x1,y1,z1);
  double d=fabs(x-x1)+fabs(y-y1)+fabs(z-z1);
  if (d<0.01)
    return;

  this->NumPoints++;
  if (this->NumPoints>=this->Points->GetNumberOfPoints()-1)
    {
      vtkPoints* tmpPoints=vtkPoints::New();
      tmpPoints->SetNumberOfPoints(this->NumPoints);
      for (int i=0;i<this->NumPoints;i++)
	tmpPoints->SetPoint(i,this->Points->GetPoint(i));
      
      this->Points->SetNumberOfPoints(this->NumPoints+20);
      for (int j=0;j<this->NumPoints;j++)
	this->Points->SetPoint(j,tmpPoints->GetPoint(j));
      tmpPoints->Delete();
    }
  
  this->Points->SetPoint(index,x,y,z);
  this->Modified();
}
/* -------------------------------------------------------------------------*/
void vtkpxBaseCurve::DeletePoint(int index)
{
  if (index==-1)
      index=this->NumPoints-1;
  else
      index=Irange(index,0,this->NumPoints-1);

  if (index!=this->NumPoints-1)
      {
	for (int k=index+1;k<this->NumPoints;k++)
	    this->Points->SetPoint(k-1,this->Points->GetPoint(k));
      }

  this->NumPoints-=1;
  this->Points->SetPoint(this->NumPoints,0.0,0.0,0.0);

  this->Modified();
}
/* -------------------------------------------------------------------------*/
void vtkpxBaseCurve::DeleteAllPoints()
{
  for (int i=0;i<this->NumPoints;i++)
      this->Points->SetPoint(this->NumPoints,0.0,0.0,0.0);

  this->NumPoints=0;
  this->Modified();
}
/* -------------------------------------------------------------------------*/
vtkPoints* vtkpxBaseCurve::GetPoints()
{
  return this->Points;
}
/* -------------------------------------------------------------------------*/
int vtkpxBaseCurve::Save(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (fout)
      {
	fprintf(fout,"#Landmark Data\n");
	fprintf(fout,"#Number of Landmarks Mode\n%d %d\n",this->NumPoints,(this->ClosedCurve>0)+1);
	fprintf(fout,"#Origin\n %6.3f %6.3f %6.3f\n",Origin[0],Origin[1],Origin[2]);
	fprintf(fout,"#Spacing\n %6.3f %6.3f %6.3f\n",Spacing[0],Spacing[1],Spacing[2]);
	fprintf(fout,"#Landmarks\n");
	for (int i=0;i<this->NumPoints;i++)
	    {
	      double x[3];
	      this->Points->GetPoint(i,x);
	      fprintf(fout,"%6.2f %6.2f %6.2f\n",x[0],x[1],x[2]);
	    }
	fclose(fout);
	vtkDebugMacro(<<"Saved landmarks ("<<this->NumPoints<<")  in "<<fname);
	return 1;
      }
  return 0;
}
/* -------------------------------------------------------------------------*/
int vtkpxBaseCurve::Load(const char* fname)
{
  char line[400];
  strncpy(line,fname,400);

  //fprintf(stderr,"Loading %s\n",line);
  if (gzfindstringinstring(line,".vtk")==1)
    {
      //  this->DebugOn();
      //      fprintf(stderr,"Found .vtk\n");
      // Read as surface file and adjust points later
      vtkPolyDataReader* source=vtkPolyDataReader::New();
      source->SetFileName(fname);
      source->Update();

      vtkPoints* newPoints=source->GetOutput()->GetPoints();

      /*fprintf(stderr,"Name=%s Points=%d Polys=%d\n",
	      fname,
	      source->GetOutput()->GetNumberOfPoints(),
	      source->GetOutput()->GetNumberOfCells());*/

      if (newPoints!=NULL)
	{
	  this->SetFromPoints(newPoints);
	  vtkDebugMacro(<<"Loaded  landmarks ("<<this->NumPoints<<")  from "<<fname << "via vtkPolyDataReader");
	  return 1;
	}
      else
	{
	  return 0;
	}
    }
  
  gzFile fin=gzsuffixopen(fname,"rb");
  if (fin)
      {

	gzgets(fin,line,100);
	if (gzfindstringinstring(line,"#Landmark Data")==0)
	  {
	    gzclose(fin);
	    return 0;
	  }
	gzgets(fin,line,100); 	
	gzgets(fin,line,100);
	int oldnum=this->NumPoints,new_num,tmp;
	
	sscanf(line,"%d %d",&new_num,&tmp);
	this->DeleteAllPoints();
	this->NumPoints=new_num;
	if (this->NumPoints>oldnum)
	  this->Points->SetNumberOfPoints(this->NumPoints+10);
	
	this->SetClosedCurve(tmp>1);

	gzgets(fin,line,100); 	gzgets(fin,line,100);
	sscanf(line,"%lf %lf %lf",&Origin[0],&Origin[1],&Origin[2]);

	gzgets(fin,line,100); 	gzgets(fin,line,100);
	sscanf(line,"%lf %lf %lf",&Spacing[0],&Spacing[1],&Spacing[2]);



	gzgets(fin,line,100);
	for (int i=0;i<this->NumPoints;i++)
	    {
	      gzgets(fin,line,100);
	      //	      fprintf(stderr,"line=%s\n",line);
	      double x[3];
	      sscanf(line,"%lf %lf %lf",&x[0],&x[1],&x[2]);
	      this->Points->SetPoint(i,x[0],x[1],x[2]);
	    }
	gzclose(fin);
	vtkDebugMacro(<<"Loaded  landmarks ("<<this->NumPoints<<")  from "<<fname);
	return 1;
      }
  return 0;
}
/* -------------------------------------------------------------------------*/
int vtkpxBaseCurve::Compact()
{
  vtkPoints* tmpPoints=vtkPoints::New();
  tmpPoints->DeepCopy(this->Points);

  this->Points->SetNumberOfPoints(this->NumPoints);
  for (int i=0;i<this->NumPoints;i++)
      {
	double x[3];
	tmpPoints->GetPoint(i,x);

	for (int j=0;j<=2;j++)
	    x[j]=(x[j])*Spacing[j]+Origin[j];
	this->Points->SetPoint(i,x);
      }
  
  for (int ia=0;ia<=2;ia++)
    {
      this->Spacing[ia]=1.0;
      this->Origin[ia]=0.0;
    }

  tmpPoints->Delete();
  return this->Points->GetNumberOfPoints();
}
/* -------------------------------------------------------------------------*/
int vtkpxBaseCurve::StoreInContour(PXContour* cntr,int plane)
{
  if (cntr==NULL)
      return -1;

  cntr->DeleteAll();

  for (int i=0;i<this->NumPoints;i++)
      {
	double x[3],y[2];
	this->Points->GetPoint(i,x);
	
	switch(plane)
	    {
	    case 0: // YZ 
	      y[0]=x[1];
	      y[1]=x[2];
	      break;
	      
	    case 1: // XZ
	      y[0]=x[0];
	      y[1]=x[2];
	      break;
	      
	    case 2: // XY
	      y[0]=x[0];
	      y[1]=x[1];
	      break;
	    }
	cntr->Add(y[0],y[1]);
      }

  cntr->setclosed(this->ClosedCurve);
  return cntr->getnumpoints();
}
/* -------------------------------------------------------------------------*/
int vtkpxBaseCurve::SetFromContour(PXContour* cntr,int plane,int level)
{
  if (cntr==NULL)
      return 0;

  DeleteAllPoints();
  
  this->Points->SetNumberOfPoints(cntr->getnumpoints()+10);
  this->NumPoints=cntr->getnumpoints();

  this->SetClosedCurve(cntr->isclosed());

  plane=Irange(plane,0,2);
  //PointScale=0.25;

  if (cntr->getnumpoints()==0)
      return 0;

  PXContourPoint* temp=cntr->gettoppoint(); 

  for (int i=0; i<cntr->getnumpoints(); i++) 
      {
	double y[3];
	switch(plane)
	    {
	    case 0: // YZ 
	      y[0]=double(level);
	      y[1]=temp->x;
	      y[2]=temp->y;
	      break;
	      
	    case 1: // XZ
	      y[0]=temp->x;
	      y[1]=double(level);
	      y[2]=temp->y;
	      break;
	      
	    case 2: // XY
	      y[0]=temp->x;
	      y[1]=temp->y;
	      y[2]=double(level);
	      break;
	    }
	this->Points->SetPoint(i,y);
	temp=temp->next;
      }
  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxBaseCurve::GetCurrentInternalPlane()
{
  if (this->NumPoints<2)
    return 2;

  double sum[3],sumsq[3],x[3];
  int ia;
  for (ia=0;ia<=2;ia++)
    {
      sum[ia]=0.0;
      sumsq[ia]=0.0;
    }

  for (int i=0;i<this->NumPoints;i++)
    {
      this->Points->GetPoint(i,x);
      for (int ia=0;ia<=2;ia++)
	{
	  sum[ia]+=x[ia];
	  sumsq[ia]+=x[ia]*x[ia];
	}
    }

  double np=double(this->NumPoints);

  for (ia=0;ia<=2;ia++)
    {
      sum[ia]/=np;
      sumsq[ia]=sumsq[ia]/np-sum[ia]*sum[ia];
    }

  if (sumsq[0]<sumsq[1] && sumsq[0]<sumsq[2])
    return 0;

  if (sumsq[1]<sumsq[2])
    return 1;
  
  return 2;

}

void vtkpxBaseCurve::GetCurrentInternalPlaneAndLevel(int &plane,int &level)
{
  if (this->NumPoints<1)
    {
      plane =2;
      level =0;
    }

  double sum[3],sumsq[3],x[3];
  int ia;
  for (ia=0;ia<=2;ia++)
    {
      sum[ia]=0.0;
      sumsq[ia]=0.0;
    }

  for (int i=0;i<this->NumPoints;i++)
    {
      this->Points->GetPoint(i,x);
      for (int ia=0;ia<=2;ia++)
	{
	  sum[ia]+=x[ia];
	  sumsq[ia]+=x[ia]*x[ia];
	}
    }

  if (this->NumPoints>1)
    {
      double np=double(this->NumPoints);
      for (ia=0;ia<=2;ia++)
	{
	  sum[ia]/=np;
	  sumsq[ia]=sumsq[ia]/np-sum[ia]*sum[ia];
	}
      
      if (sumsq[0]<sumsq[1] && sumsq[0]<sumsq[2])
	{
	  plane=0;
	}
      else if (sumsq[1]<sumsq[2])
	{
	  plane=1;
	} 
      else 
	{
	  plane=2;
	}
    } 
  else
    {
      plane=2;
    }

  level=(int)sum[plane];
}
/* -------------------------------------------------------------------------*/

int vtkpxBaseCurve::SetFromPolyData(vtkPolyData* polydata,double startx[3],int plane,int level)
{
  if (polydata==NULL)
      return 0;

  plane=Irange(plane,-1,2);
  DeleteAllPoints();

  vtkPoints* newPoints=polydata->GetPoints();
  int np=newPoints->GetNumberOfPoints();
	
  int* neigh=new int[np*2];
  int* used=new int[np];

  for (int ib=0;ib<np*2;ib++)
      neigh[ib]=-1;
  for (int ic=0;ic<np;ic++)
      used[ic]=0;
 
  vtkCellArray* lines=polydata->GetLines();
  int numlines=lines->GetNumberOfCells();
  lines->InitTraversal();
  for (int ia=0;ia<numlines;ia++)
      {
	vtkIdType nump;
	vtkIdType *p;
	lines->GetNextCell(nump,p);
	
	for (int k=0;k<=1;k++)
	    {
	      int index=p[k]*2;
	      if (neigh[index]==-1)
		  neigh[index]=p[1-k];
	      else
		  neigh[index+1]=p[1-k];
	    }
      }

  double x[3];
  int nearest=0;
  newPoints->GetPoint(0,x);
  double mindist=Fsqdist(x[0],x[1],x[2],startx[0],startx[1],startx[2]);
  for (int i=1;i<np;i++)
      {
	newPoints->GetPoint(i,x);
	double dist=Fsqdist(x[0],x[1],x[2],startx[0],startx[1],startx[2]);
	if (dist<mindist)
	    {
	      mindist=dist;
	      nearest=i;
	    }
      }
  newPoints->GetPoint(nearest,x);

  int done=0,numlinks=0,firstpoint=nearest;

  while (done==0 && numlinks<np-1)
      {
	int index=firstpoint*2;
	used[firstpoint]=1;

	//fprintf(stderr,"Parsing Point=%3d Neighbours %3d,%3d\n",firstpoint,neigh[index],neigh[index+1]);
	if (neigh[index]==-1 || neigh[index+1]==-1)
	    {
	      done=1;

	      // Unless we can merge 
	      double x1[3],x2[3];
	      newPoints->GetPoint(firstpoint,x1);
	      for (int kk=0;kk<np;kk++)
		  {
		    if (kk!=firstpoint)
			{
			  int index2=kk*2;
			  if (neigh[index2]==-1 || neigh[index2+1]==-1)
			      {
				newPoints->GetPoint(kk,x2);
				if (Fsqdist(x1[0],x1[1],x1[2],x2[0],x2[1],x2[2])<1.0)
				    {
				      /*fprintf(stderr,"\tAdding Link from %5.1f,%5.1f,%5.1f to %5.1f,%5.1f,%5.1f (index=%d,%d)\n",
					      x1[0],x1[1],x1[2],x2[0],x2[1],x2[2],kk,firstpoint);*/
				      done=0;

				      if (neigh[index]==-1)
					  neigh[index]=kk;
				      else
					  neigh[index+1]=kk;

				      // Chain Back

				      if (neigh[index2]==-1)
					  neigh[index2]=firstpoint;
				      else
					  neigh[index2+1]=firstpoint;
				      kk=np;
				    }
			      }
			}
		  }

	    }

	if (done==0)
	    {
	      int keeplooking=1;

	      if (neigh[index]!=-1)
		  {
		    if (used[neigh[index]]==0)
			{
			  firstpoint=neigh[index];
			  keeplooking=0;
			}
		  }

	      if (keeplooking==1 && neigh[index+1]!=-1)
		  {
		    if (used[neigh[index+1]]==0)
			{
			  firstpoint=neigh[index+1];
			  keeplooking=0;
			}
		  }

	      if (keeplooking==1)
		  done=1;
	      else
		  numlinks++;
	    }
      }

  /*fprintf(stderr,"\tFirstPoint =%d (Neighbours=%d,%d) numlinks=%d (np=%d)\n",firstpoint,neigh[firstpoint*2],
	  neigh[firstpoint*2+1],numlinks,np);*/

  for (int ie=0;ie<np;ie++)
      used[ie]=0;

  int numgoodpoints=0,found=0;
  int nextpoint=firstpoint;
  while (found==0 && numgoodpoints<np)
      {
	int currentpoint=nextpoint;
	used[currentpoint]=1;
	int index=currentpoint*2;

	/*fprintf(stderr,"Parsing Point=%3d Neighbours %3d,%3d Used=%d,%d\n",
		currentpoint,neigh[index],neigh[index+1],used[neigh[index]],
		used[neigh[index+1]]);*/
	numgoodpoints++;

	int keeplooking=1;
	if (neigh[index]!=-1)
	    {
	      if (used[neigh[index]]==0)
		  {
		    nextpoint=neigh[index];
		    keeplooking=0;
		  }
	    }
	
	if (keeplooking==1 && neigh[index+1]!=-1)
	    {
	      if (used[neigh[index+1]]==0)
		  {
		    nextpoint=neigh[index+1];
		    keeplooking=0;
		  }
	    }

	if (keeplooking==1)
	    found=1;
      }

  //fprintf(stderr,"\tNumgoodpoints=%d\n",numgoodpoints);

  // Restablish all links
  for (int iaa=0;iaa<np;iaa++)
      used[iaa]=0;

  this->Points->SetNumberOfPoints(numgoodpoints+10);
  this->NumPoints=numgoodpoints;
	
  for (int k=0;k<numgoodpoints;k++)
      {
	double x[3],y[3];
	newPoints->GetPoint(firstpoint,x);
	used[firstpoint]=1;

	//fprintf(stderr,"Adding %d Point=%3d Neighbours %5d,%5d\n",k,firstpoint,neigh[firstpoint*2],neigh[firstpoint*2+1]);
	
	switch(plane)
	    {
	    case -1:
	      {
		for (int ia=0;ia<=2;ia++)
		    y[ia]=(x[ia]-Origin[ia])/Spacing[ia];
	      }
	      break;
	      
	    case 0: // YZ 
	      y[0]=double(level);
	      y[1]=(x[0]-Origin[0])/Spacing[0];
	      y[2]=(x[1]-Origin[1])/Spacing[1];
	      break;
	      
	    case 1: // XZ
	      y[0]=(x[0]-Origin[0])/Spacing[0];
	      y[1]=double(level);
	      y[2]=(x[1]-Origin[1])/Spacing[1];
	      break;
	      
	    case 2: // XY
	      y[0]=(x[0]-Origin[0])/Spacing[0];
	      y[1]=(x[1]-Origin[1])/Spacing[1];
	      y[2]=double(level);
	      break;
	    }
	
	this->Points->SetPoint(k,y);
	
	int index=firstpoint*2;
	int keeplooking=1;

	if (neigh[index]!=-1)
	    {
	      if (used[neigh[index]]==0)
		  {
		    firstpoint=neigh[index];
		    keeplooking=0;
		  }
	    }

	if (keeplooking==1 && neigh[index+1]!=1)
	    {
	      if (used[neigh[index+1]]==0)
		  {
		    firstpoint=neigh[index+1];
		    keeplooking=0;
		  }
	    }

	if (keeplooking==1)
	    {
	      this->NumPoints=k+1;
	      k=np;
	    }
      }
  
  delete [] used;
  delete [] neigh;
  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxBaseCurve::SetFromPoints(vtkPoints* pts)
{
  if (pts==NULL)
    return -1;

  DeleteAllPoints();
  this->NumPoints=pts->GetNumberOfPoints();
  this->Points->SetNumberOfPoints(this->NumPoints+10);
  for (int i=0;i<pts->GetNumberOfPoints();i++)
    {
      double x[3];
      pts->GetPoint(i,x);
      this->SetPoint(i,x[0],x[1],x[2],1);
    }
  this->Modified();
  //fprintf(stderr,"Number of Points = %d (in=%d,outb=%d)\n",this->NumPoints,pts->GetNumberOfPoints(),points->GetNumberOfPoints());
  return this->NumPoints;
}

int vtkpxBaseCurve::CreateCircle(int plane,int level)
{
  if (this->NumPoints<2)
      return 0;

  double x[3],y[3];
  this->Points->GetPoint(0,x);
  this->Points->GetPoint(1,y);
  double radius=Fdist(x[0],x[1],x[2],y[0],y[1],y[2]);

  int np=int(radius*2.0);
  
  double xp[2];
  switch(plane)
      {
      case 0: // YZ 
	xp[0]=x[1];
	xp[1]=x[2];
	break;
	
      case 1: // XZ
	xp[0]=x[0];
	xp[1]=x[2];
	break;
	
      case 2: // XY
	xp[0]=x[0];
	xp[1]=x[1];
	break;
      }
  
  PXContour* cntr=new PXContour();
  cntr->setcircle(np,radius,xp[0],xp[1]);
  cntr->setclosed(1);
  SetFromContour(cntr,plane,level);
  delete cntr;

  this->Modified();
  return this->NumPoints;
}
/* -------------------------------------------------------------------------*/
int vtkpxBaseCurve::FindClosestPoint(double x,double y, double z,double threshold,int scaled) 
{
  double px[3];
  px[0]=x;px[1]=y;px[2]=z;
  return this->FindClosestPoint(px,threshold,scaled);
}
int vtkpxBaseCurve::FindClosestPoint(double px[3],double threshold,int scaled)
{
  int found=-1;
  if (this->NumPoints<1)
      return found;

  if (scaled)
    {
      for (int ia=0;ia<=2;ia++)
	px[ia]=(px[ia]-Origin[ia])/Spacing[ia];
    }
 
  for (int i=0;i<GetNumPoints();i++)
      {
	double lx[3];
	this->Points->GetPoint(i,lx);
	/*	fprintf(stderr,"Comparing %d (%.2f,%.2f,%.2f) with (%.2f,%.2f,%.2f) at threshold=%.2f\n",
	  i,lx[0],lx[1],lx[2],px[0],px[1],px[2],threshold);*/
	if (Fsamepoint(px[0],px[1],px[2],lx[0],lx[1],lx[2],threshold))
	  {
	    found=i;
	    i=GetNumPoints();
	  }
      }
  return found;
}
/* -------------------------------------------------------------------------*/
float vtkpxBaseCurve::GetLength()
{
  int np=this->NumPoints;
  float dist=0;
  for (int i=0;i<np;i++)
    {
      double x[3],tx[3];
      this->Points->GetPoint(i,x);
      int ip=i+1;
      if (ip<np || this->ClosedCurve==1)
	{
	  if (ip==np)
	    ip=0;
	  
	  this->Points->GetPoint(ip,tx);
	  for (int j=0;j<=2;j++)
	    {
	      x[j]=x[j]*Spacing[j]+Origin[j];
	      tx[j]=tx[j]*Spacing[j]+Origin[j];
	    }
	  dist+=sqrt(vtkMath::Distance2BetweenPoints(x,tx));
	}
    }
  return dist;
}
/* -------------------------------------------------------------------------*/
float vtkpxBaseCurve::GetPlaneArea(int plane)
{
  /*if (!this->ClosedCurve)
    return -1.0;*/



  if (this->NumPoints<3)
    {
      //      fprintf(stderr,"Number of Points < 3 \n");
      return 0.0;
    }

  float area=0.0;
  

  //	2 A( P ) = sum_{i=0}^{n-1} (x_i y_{i+1} - y_i x_{i+1}).
  for (int i=0;i<this->NumPoints;i++)
      {
      double xi[3],xip[3];

      this->Points->GetPoint(i,xi);
      if (i!=this->NumPoints-1)
	this->Points->GetPoint(i+1,xip);
      else
	this->Points->GetPoint(0,xip);

      switch(plane)
	{
	case 0:
	  area+= xi[1]*xip[2]-xi[2]*xip[1];
	  break;
	case 1:
	  area+= xi[0]*xip[2]-xi[2]*xip[0];
	  break;
	case 2:
	  area+= xi[0]*xip[1]-xi[1]*xip[0];
	  break;
	}
    }

  switch(plane)
    {
    case 0:
      return fabs(area*0.5)*Spacing[1]*Spacing[2];
      break;
    case 1:
      return fabs(area*0.5)*Spacing[0]*Spacing[2];
      break;
    default:
      return fabs(area*0.5)*Spacing[1]*Spacing[0];
      break;
    }
}
/* -------------------------------------------------------------------------*/	
float vtkpxBaseCurve::GetAreaFromPixels(int num,int image_plane)
{
  image_plane=Irange(image_plane,0,2);
  switch(image_plane)
    {
    case 0:
      return float(num)*Spacing[1]*Spacing[2];
      break;
    case 1:
      return float(num)*Spacing[0]*Spacing[2];
      break;
    default:
      return float(num)*Spacing[1]*Spacing[0];
      break;
    }
}

int vtkpxBaseCurve::ExtractVOIProperties(vtkImageData* image,int plane,
					 int level,int fr,float& mean,float& sigma)
{
  mean=-1.0;
  sigma=-1.0;

  if (image==NULL || plane==-1)
      return 0;

  if (this->NumPoints<1)
      return 0;

  int dim[3];
  image->GetDimensions(dim);
  plane=Irange(plane,0,2);
  
  if (dim[plane]<0)
      return 0;
  level=Irange(level,0,dim[plane]);

  vtkpxImageExtract* extr=vtkpxImageExtract::New();
  extr->SetInput(image);
  extr->SetCurrentPlane(plane);
  extr->SetSliceNo(level);
  extr->SetFrame(fr);
  extr->Update();

  vtkImageData* sourceImage=vtkImageData::New();
  sourceImage->ShallowCopy(extr->GetOutput());
  extr->Delete();  

  PXContour* cntr=new PXContour();
  if (this->NumPoints==2)
      CreateCircle(plane,level);
	
  StoreInContour(cntr,plane);
  cntr->setclosed(1);
  int numinside=0;
  int numbiggerthanzero=0;
  float sum=0,sumsq=0;

  if (cntr->getnumpoints()>1)
      {
	PXContourPoint* temp=cntr->gettoppoint(); 

	float minx=temp->x,maxx=temp->x,miny=temp->y,maxy=temp->y;
	temp=temp->next;
	for (int i=1; i<cntr->getnumpoints(); i++) 
	    {
	      minx=Fmin(minx,temp->x);
	      maxx=Fmax(maxx,temp->x);
	      miny=Fmin(miny,temp->y);
	      maxy=Fmax(maxy,temp->y);
	      temp=temp->next;
	    }
	
	for (int ia=(int)minx;ia<maxx;ia++)
	    for (int ib=(int)miny;ib<maxy;ib++)
		{
		  if (cntr->IsInside(float(ia),float(ib)))
		      {
			numinside++;
			double pxl=sourceImage->GetScalarComponentAsDouble(ia,ib,0,0);
			if (pxl>0.0)
			  ++numbiggerthanzero;
			sum+=pxl;
			sumsq+=(pxl*pxl);
		      }
		}
      }
  else
      {
	PXContourPoint* temp=cntr->gettoppoint(); 
	sum=sourceImage->GetScalarComponentAsDouble((int)temp->x,(int)temp->y,0,0);
	sumsq=sum*sum;
	numinside=1;
      }
	
  mean=0.0;
  sigma=0.0;

  if (numinside==0)
      return 0;

  mean=sum/float(numinside);
  sigma=sqrt(sumsq/float(numinside)-mean*mean);
  
  sourceImage->Delete();
  delete cntr;
  return numbiggerthanzero;
}

/* -------------------------------------------------------------------------*/

void vtkpxBaseCurve::Sample(int rate)
{
  if (this->NumPoints<4)
      return;

  vtkPoints* tmpPoints=vtkPoints::New();
  tmpPoints->SetNumberOfPoints(this->NumPoints);
  for (int i=0;i<this->NumPoints;i++)
      tmpPoints->SetPoint(i,this->Points->GetPoint(i));
  
  int np=this->NumPoints;
  this->NumPoints=0;
  for (int j=0;j<np;j+=rate)
      {
	this->Points->SetPoint(this->NumPoints,tmpPoints->GetPoint(j));
	this->NumPoints++;
      }
  tmpPoints->Delete();
  
  this->Modified();
}
/* -------------------------------------------------------------------------*/
void vtkpxBaseCurve::SubSample(int rate)
{
  if (this->NumPoints<2)
      return;

  vtkPoints* tmpPoints=vtkPoints::New();
  tmpPoints->SetNumberOfPoints(this->NumPoints+1);
  for (int i=0;i<this->NumPoints;i++)
      tmpPoints->SetPoint(i,this->Points->GetPoint(i));

  int np=this->NumPoints;

  if (this->ClosedCurve)
    {
      tmpPoints->SetPoint(this->NumPoints,this->Points->GetPoint(0));
      np+=1;
    }
  
  this->Points->SetNumberOfPoints(rate*this->NumPoints+20);

  this->NumPoints=0;
  for (int j=0;j<np-1;j++)
      {
	double x1[3],x2[3];
	tmpPoints->GetPoint(j,x1);
	tmpPoints->GetPoint(j+1,x2);

	for (int k=0;k<rate;k++)
	    {
	      float ratio2=((float)k)/((float)rate);
	      float ratio1=1.0-ratio2;
	      for (int l=0;l<=2;l++)
		  x1[l]=ratio1*x1[l]+ratio2*x2[l];
	      this->Points->SetPoint(this->NumPoints,x1);
	      this->NumPoints++;
	    }
      }
  
  // Add Last Point if open if closed this is the starting point hence ignore
  if (!ClosedCurve)
    {
      this->Points->SetPoint(this->NumPoints,tmpPoints->GetPoint(np-1));
      this->NumPoints++;
    }
  
  tmpPoints->Delete();
  
  this->Modified();

}
/* -------------------------------------------------------------------------*/ 
void vtkpxBaseCurve::Smooth(float relaxation)
{
  if (this->NumPoints<5)
      return;

  relaxation=Frange(relaxation,0.0,0.99);
  vtkPoints* tmpPoints=vtkPoints::New();
  tmpPoints->SetNumberOfPoints(this->NumPoints);
  int np=this->NumPoints;

  for (int pass=0;pass<=1;pass++)
      {
	for (int j=0;j<np;j++)
	    tmpPoints->SetPoint(j,this->Points->GetPoint(j));
	
	for (int i=0;i<np;i++)
	    {
	      int p0,p1;
	      if (this->ClosedCurve)
		  {
		    p0=Icyclic(i-1,np);
		    p1=Icyclic(i+1,np);
		  }
	      else
		  {
		    p0=Irange(i-1,0,np-1);
		    p1=Irange(i+1,0,np-1);
		  }
	      
	      double x0[3],x1[3],x[3];
	      tmpPoints->GetPoint(p0,x0);
	      tmpPoints->GetPoint(i,x);
	      tmpPoints->GetPoint(p1,x1);
	      
	      for (int ia=0;ia<=2;ia++)
		  {
		    if (pass==0)
			x[ia]=(1.0-relaxation)*x[ia]+0.5*relaxation*(x0[ia]+x1[ia]);
		    else
			x[ia]=(1.0-relaxation)*x[ia]+0.5*relaxation*(x0[ia]+x1[ia]);
		  }
	      this->Points->SetPoint(i,x);
	    }
	relaxation=-1.0*(relaxation+0.01);
      }
  tmpPoints->Delete();
  
  this->Modified();
}
/* -------------------------------------------------------------------------*/
float basecurve_getperimeter(vtkPoints* points,int closed,int numpoints)
{
  float perimeter=0.0;

  // Determine upper bound for loops
  int maxp=numpoints-1;
  if (closed)
      maxp++;

  // Find Perimeter Length
  for (int i=0;i<maxp;i++)
      {
	double x1[3],x2[3];
	points->GetPoint(i,x1);
	points->GetPoint(Icyclic(i+1,numpoints),x2);
	perimeter+=Fdist(x1[0],x1[1],x1[2],x2[0],x2[1],x2[2]);
      }
  return perimeter;
}
/* -------------------------------------------------------------------------*/
int basecurve_intersect(double P1[3],double P2[3],double P3[3],double P4[3],double P[3],int plane,int level)
{
  double x1=0,y1=0,x2=0,y2=0,x3=0,y3=0,x4=0,y4=0;
  switch (plane)
      {
      case 0:
	x1=P1[1]; 	y1=P1[2]; 
	x2=P2[1]; 	y2=P2[2]; 
	x3=P3[1]; 	y3=P3[2]; 
	x4=P4[1]; 	y4=P4[2]; 
	break;	  
      case 1:	  
	x1=P1[0]; 	y1=P1[2]; 
	x2=P2[0]; 	y2=P2[2]; 
	x3=P3[0]; 	y3=P3[2]; 
	x4=P4[0]; 	y4=P4[2]; 
	break;	  
      case 2:	  
	x1=P1[0]; 	y1=P1[1]; 
	x2=P2[0]; 	y2=P2[1]; 
	x3=P3[0]; 	y3=P3[1]; 
	x4=P4[0]; 	y4=P4[1]; 
	break;
      }
  
  double numA  =(x4-x3)*(y1-y3) - (y4-y3)*(x1-x3);
  double numB  =(x2-x1)*(y1-y3) - (y2-y1)*(x1-x3);
  double den   =(y4-y3)*(x2-x1) - (x4-x3)*(y2-y1);

  if (den==0.0)
      return 0;

  double ua=numA/den;
  double ub=numB/den;

  if (ua==0.0 && ub==0.0)
      return 0;

  if (ua>0.0 && ua<1.0 && ub>0.0 && ub<1.0)
      {
	double x=x1+ua*(x2-x1);
	double y=y1+ua*(y2-y1);
	switch (plane)
	    {
	    case 0:
	      P[0]=level;
	      P[1]=x;
	      P[2]=y;
	      break;
	    case 1:
	      P[0]=x;
	      P[1]=level;
	      P[2]=y;
	      break;
	    case 2:
	      P[0]=x;
	      P[1]=y;
	      P[2]=level;
	      break;
	    }
	return 1;
      }
  return 0;
}
/* -------------------------------------------------------------------------*/
int eliminate(vtkPoints* points,int numpoints,int begin,int end,int insert,double P[3])
{
  /*  vtkPoints* tmpPoints=vtkPoints::New();
  tmpPoints->SetNumberOfPoints(numpoints);
  for (int i=0;i<numpoints;i++)
      tmpPoints->SetPoint(i,points->GetPoint(i));*/

  begin=Irange(begin,0,numpoints-1);
  end=Irange(end,0,numpoints-1);

  //  insert=PXFalse;

  if (begin<=end)
      {
	int offset=0;
	if (insert)
	    {
	      points->SetPoint(begin,P);
	      offset=1;
	    }
	for (int i=end+1;i<numpoints;i++)
	    {
	      int index=i-(end+1)+begin+offset;
	      points->SetPoint(index,points->GetPoint(i));
	      //fprintf(stderr,"Setting point %d <--- %d\n",index,i);
	    }
	numpoints=numpoints-(end-begin+1)+offset;
	return numpoints;
      }
  else
      {
	for (int i=end+1;i<begin;i++)
	    {
	      int index=i-(end+1);
	      points->SetPoint(index,points->GetPoint(i));
	      //fprintf(stderr,"Setting point %d <--- %d\n",index,i);

	    }
	numpoints=(begin-(end+1));

	if (insert)
	    {
	      int index=begin-(end+1);
	      points->SetPoint(index,P);
	      numpoints++;
	    }
	return numpoints;
      }
}
/* -------------------------------------------------------------------------*/
int checkIntersections(vtkPoints* points,int numpoints,int closed,
		       int seed,double P[3],int longest,
		       int& begin,int& end,
		       int plane,int level)
{
  int found=-1;
  if (!closed && seed==numpoints-1)
      return 0;

  int maxp=numpoints-2;
  if (closed)
    maxp++;

  double P1[3],P2[3],P3[3],P4[3];
  
  points->GetPoint(seed,P1);
  points->GetPoint(Icyclic(seed+1,numpoints),P2);
  int test=seed+1;
  while (found==-1 && test<=maxp)
      {
	/*fprintf(stderr,"\t ---> Testing %d,%d to %d,%d",seed,Icyclic(seed+1,numpoints),test,
		Icyclic(test+1,numpoints));*/
	points->GetPoint(test,P3);
	points->GetPoint(Icyclic(test+1,numpoints),P4);
	if (basecurve_intersect(P1,P2,P3,P4,P,plane,level))
	    found=test;
	else
	    test++;
	//	fprintf(stderr,"Found =%d\n",found);
      }
  
  if (found==-1)
      return 0;

  if (!closed)
      {
	begin=seed+1;
	end=found;
	return 1;
      }

  int start[2],finish[2],dist[2];

  start[0] =found+1;
  finish[0]=  seed;
  start[1]=seed+1;
  finish[1]=found;

  for (int kk=0;kk<=1;kk++)
      dist[kk]=Icyclic(finish[kk]-start[kk],numpoints);


  int keep=0;
  if (dist[0]>dist[1])
      keep=0;
  else
      keep=1;

  if (longest==PXFalse)
      keep=1-keep;
  
  begin=start[1-keep];
  end=finish[1-keep];

  /*fprintf(stderr,"Found Possible points Set 1 (%d,%d) dist=%d Set 2 (%d,%d) dist=%d selected=%d (%d to %d)\n\n",
	  start[0],finish[0],dist[0],
	  start[1],finish[1],dist[1],keep,begin,end);*/


  return 1;		      
}
/* -------------------------------------------------------------------------*/	
void vtkpxBaseCurve::RemoveIntersections(int keeplongest,int plane,int level)
{
  if (plane==-1)
    {
      int pl,lv;
      this->GetCurrentInternalPlaneAndLevel(pl,lv);
      plane=pl;
      level=lv;
    }

  plane=Irange(plane,0,2);

  int found=1;
  if (this->NumPoints<3)
      return;

  int np=this->NumPoints-1;
  if (this->ClosedCurve)
      np+=1;

  while (found==1 )
      {
	found=0;
	int seed=0;
	while (seed<np && found==0)
	    {
	      double P[3];
	      int begin=-1,end=-1;
	      int ok=checkIntersections(this->Points,this->NumPoints,this->ClosedCurve,seed,P,keeplongest,
					begin,end,plane,level);

	      /*	      fprintf(stderr,"Checking intersections seed=%d (num_land=%d,max=%d) begin=%d end=%d (found=%d) point=(%5.2f,%5.2f,%5.2f)\n",
		seed,
		this->NumPoints,np,begin,end,ok,P[0],P[1],P[2]);*/

	      if (ok==0)
		  {
		    seed++;
		    found=0;
		  }
	      else
		  {
		    found=1;
		    //		    fprintf(stderr,"---> Going to eliminate\n");
		    this->NumPoints=eliminate(this->Points,this->NumPoints,begin,end,PXTrue,P);
		    np=this->NumPoints-1;
		    if (this->ClosedCurve)
			np+=1;    
		    //		    fprintf(stderr,"---> Eliminated this->NumPoints=%d\n\n",this->NumPoints);
		  }
	    }
      }
  
  this->Modified();

}
/* -------------------------------------------------------------------------*/
void vtkpxBaseCurve::Equispace(float sp)
{
  if (this->NumPoints<2)
      return;

  float perimeter=basecurve_getperimeter(this->Points,this->ClosedCurve,this->NumPoints);

// Determine upper bound for loops
  /* 
  int maxp=this->NumPoints-1;
  if (this->ClosedCurve)
      maxp++;

  // Find Perimeter Length
  for (int i=0;i<maxp;i++)
      {
	float x1[3],x2[3];
	this->Points->GetPoint(i,x1);
	this->Points->GetPoint(Icyclic(i+1,this->NumPoints),x2);
	perimeter+=Fdist(x1[0],x1[1],x1[2],x2[0],x2[1],x2[2]);
      }*/

  // Determine Appropriate Spacing 
  float Spacing=sp;
  if (Spacing<0.0)
      Spacing=perimeter/float(this->NumPoints);

  // Copy Point to temp array
  vtkPoints* tmpPoints=vtkPoints::New();
  tmpPoints->SetNumberOfPoints(this->NumPoints+1);
  int np=this->NumPoints;
  for (int j=0;j<this->NumPoints;j++)
      tmpPoints->SetPoint(j,this->Points->GetPoint(j));
  if (this->ClosedCurve)
      {
	tmpPoints->SetPoint(this->NumPoints,this->Points->GetPoint(0));
	np++;
      }

  // Allocate New set of points 
  int np2=int(perimeter/Spacing);
  this->Points->SetNumberOfPoints(np2+10);

  // Start New Set of Points at point 0
  this->Points->SetPoint(0,tmpPoints->GetPoint(0));
  this->NumPoints=1;

  // Initialize Control Points x0 and x1 
  int index=0;
  double x0[3],x1[3];
  tmpPoints->GetPoint(index,x0);
  tmpPoints->GetPoint(index+1,x1);  
  float segment=0.0;

  while(index<np-1)
      {
	float part=Fdist(x0[0],x0[1],x0[2],x1[0],x1[1],x1[2]);
	segment+=part;
	while (segment>=Spacing)
	    {
	      float ratio=1.0-(segment-Spacing)/part;
	      if (ratio<0.0)
		ratio=0.0;
	      if (ratio>1.0)
		ratio=1.0;
	      segment=segment-Spacing;
	      double tx[3];
	      for (int ia=0;ia<=2;ia++)
		  {
		    tx[ia]=(1.0-ratio)*x0[ia]+ratio*x1[ia];
		  }
	      this->Points->SetPoint(this->NumPoints,tx);
	      this->NumPoints++;
	    }
	index++;
	tmpPoints->GetPoint(index,x0);
	tmpPoints->GetPoint(index+1,x1);
      }

  if (!this->ClosedCurve)
      {
	this->Points->SetPoint(this->NumPoints,tmpPoints->GetPoint(np-1));
	this->NumPoints++;
      }
  tmpPoints->Delete();
  this->Modified();
}

/* -------------------------------------------------------------------------*/
int vtkpxBaseCurve::ExtractImageContour(vtkImageData* image,int plane,int level,int frame,
					float& threshold,int median,
					float sigma)
{
  int outnp;

  if (image==NULL)
    return 0;

  if (this->NumPoints==0)
    return 0;

  int dim[3];
  image->GetDimensions(dim);

  int pl,lv;

  if (plane == -1)
    {
      this->GetCurrentInternalPlaneAndLevel(pl,lv);
      plane=pl;
      level=lv;
    }

  //  fprintf(stderr,"Selected Plane =%d Level=%d\n",plane,level);
  
  frame=Irange(frame,0,image->GetNumberOfScalarComponents()-1);

  if (dim[plane]<0)
      return 0;

  if (this->NumPoints==0)
      return 0;

  int imagelevel=level;
  if (imagelevel>=10000 || imagelevel<0)
      {
	this->GetCurrentInternalPlaneAndLevel(pl,lv);
	level=lv;
      }
  imagelevel=Irange(imagelevel,0,dim[plane]-1);

  // Pick out image slice 
  // --------------------

  vtkpxImageExtract* extr=vtkpxImageExtract::New();
  extr->DebugOn();
  extr->SetInput(image);
  extr->SetCurrentPlane(plane);
  extr->SetSliceNo(imagelevel);
  extr->SetFrame(frame);
  extr->DebugOff();
  extr->Update();


  vtkImageData* inp=vtkImageData::New();
  inp->ShallowCopy(extr->GetOutput());
  extr->Delete();

  if (median>1)
    {
      vtkImageMedian3D* medianFilter=vtkImageMedian3D::New();
      medianFilter->SetInput(inp);
      median=Irange(median,0,9)*2+1;
      medianFilter->SetKernelSize(median,median,1);
      medianFilter->SetKernelSize(1,1,1);
      medianFilter->Update();
      inp->ShallowCopy(medianFilter->GetOutput());
      medianFilter->Delete();
    }

  if (sigma>=0.5)
    {
      vtkImageGaussianSmooth* smoothFilter=vtkImageGaussianSmooth::New();
      sigma=Frange(sigma,0.5,5.0);
      smoothFilter->SetStandardDeviations(1.0,1.0,0.0);
      smoothFilter->SetRadiusFactors(3.0*sigma,3.0*sigma,1);
      smoothFilter->SetInput(inp);
      smoothFilter->Update();
      inp->ShallowCopy(smoothFilter->GetOutput());
      smoothFilter->Delete();
    }

  
  //  fprintf(stderr,"Plane=%d\n",plane);

  if (threshold<=-100000.0)
      {
	threshold=0.0;
	double x[3];
	this->Points->GetPoint(0,x);
	for (int i=-1;i<=1;i++)
	    for (int j=-1;j<=1;j++)
		{
		  int y[3];
		  switch (plane)
		      {
		      case 0:
			y[0]=int(x[0]);
			y[1]=int(x[1]+float(i));
			y[2]=int(x[2]+float(j));
			break;
		      case 1:
			y[1]=int(x[1]);
			y[0]=int(x[0]+float(i));
			y[2]=int(x[2]+float(j));
			break;
		      case 2:
			y[2]=int(x[2]);
			y[1]=int(x[1]+float(i));
			y[0]=int(x[0]+float(j));
			break;
		      }
		  threshold+=float(image->GetScalarComponentAsDouble(y[0],y[1],y[2],0))/9.0;
		}
      }


  // Find Start Point 
  double startx[3];
  int stindex=0;
  
  this->Points->GetPoint(stindex,startx);
  for (int j=0;j<=2;j++)
    startx[j]=startx[j]*Spacing[j]+Origin[j];
  
  // Extract Contour
  vtkContourFilter* contourExtractor = vtkContourFilter::New();
  contourExtractor->SetInput(inp);
  contourExtractor->SetValue(0,threshold);
  contourExtractor->ComputeScalarsOff();
  contourExtractor->ComputeGradientsOff();
  contourExtractor->ComputeNormalsOff() ;
  contourExtractor->Update();
  outnp=contourExtractor->GetOutput()->GetPoints()->GetNumberOfPoints();
  if (outnp<3)
    {
      fprintf(stderr,"Less than 3 points\n");
      return 0;
    }

  SetFromPolyData(contourExtractor->GetOutput(),startx,plane,imagelevel);
  contourExtractor->Delete();
  
  // Check whether curve is closed or not
  // ------------------------------------
  double x1[3],x2[3];
  this->Points->GetPoint(0,x1);
  this->Points->GetPoint(this->NumPoints-1,x2);
  float dista=Fdist(x1[0],x1[1],x1[2],x2[0],x2[1],x2[2]);  
  if (dista<3.0)
    this->ClosedCurve=1;
  else
    this->ClosedCurve=0;

  // Update Display
  // --------------
  //PointScale=0.5;
  Equispace(2.5);
    
  inp->Delete();
  return 1;
}

/* -------------------------------------------------------------------------*/
float basecurve_getdirection(vtkPoints* points,int numpoints)
{
  double x0[3],x1[3],x2[3];

  int np1=Irange(numpoints/3,1,numpoints-1);
  int np2=Irange(2*np1,np1+1,numpoints-1);

  points->GetPoint(  0,x0);
  points->GetPoint(np1,x1);
  points->GetPoint(np2,x2);
  
  float v1x=x1[0]-x0[0];
  float v1y=x1[1]-x0[1];

  float v2x=x2[0]-x0[0];
  float v2y=x2[1]-x0[1];
  float zz=(v1x*v2y-v2x*v1y);
  
  if (zz<0.0)
      return 1.0;
  else
      return -1.0;
}

/* -------------------------------------------------------------------------*/
vtkImageData* vtkpxBaseCurve::UpdateSnake(vtkImageData* image,vtkImageData* energyMap,
						 int plane,int level,int frame,
						 float& residual,float edge,float baloon,
						 float smooth,
						 float sigma,float step,int maxiter)
{
  const int debugmode=0;

  if (image==NULL)
      return NULL;
  
  if (this->NumPoints<1)
      return 0;
  
  int dim[3];
  image->GetDimensions(dim);
  plane=Irange(plane,0,2);
  
  frame=Irange(frame,0,image->GetNumberOfScalarComponents()-1);

  if (dim[plane]<0)
      return 0;
  level=Irange(level,0,dim[plane]);

  sigma=Frange(sigma,0.1,5.0);
  
  if (energyMap==NULL)
    energyMap=vtkpxBaseCurve::CreateEnergyMap(image,plane,level,frame,sigma);
  
  float max_move=0.0;
  float dr=Frange(step,0.1,3.0);
	  
  double range[2],scale_int;
  energyMap->GetPointData()->GetScalars()->GetRange(range);
  if (range[1]==range[0])
      range[1]=range[0]+1.0;

  scale_int=fabs(range[1]-range[0])/10.0;
  /*  if (debugmode)
      fprintf(stderr,"Intensity Range=%f,%f sc=%f\n",range[0],range[1],scale_int);*/
  

  float direction=basecurve_getdirection(this->Points,this->NumPoints);

  double sp[3],ori[3];
  energyMap->GetSpacing(sp);
  energyMap->GetOrigin(ori);
  int dim2[3];
  energyMap->GetDimensions(dim2);
  
  float tolerance=residual;
  maxiter=Irange(maxiter,1,500);

  for(int n=0;n<maxiter;n++)
      {
	if (debugmode)
	    fprintf(stderr,"Starting Iteration %d/%d ",n,maxiter);
	
	for(int current=0;current<this->NumPoints;current++)
	    {
	      // Get Point Coordinates in pixels
	      double x[3];
	      this->Points->GetPoint(current,x);
	      
	      // Point coordinates in mm
	      double xc[3];
	      for (int ia=0;ia<=2;ia++)
		  xc[ia]=x[ia]*sp[ia]+ori[ia];

	      // Find neighbouring point coordinates in pixels
	      int prevpoint1,nextpoint1;//prevpoint2,nextpoint2;
	      if (this->ClosedCurve)
		  {
		    // prevpoint2=Icyclic(current-2,this->NumPoints);
		    prevpoint1=Icyclic(current-1,this->NumPoints);
		    nextpoint1=Icyclic(current+1,this->NumPoints);
		    //nextpoint2=Icyclic(current+2,this->NumPoints);
		  }
	      else
		  {
		    //prevpoint2=Irange(current-2,0,this->NumPoints-1);
		    prevpoint1=Irange(current-1,0,this->NumPoints-1);
		    nextpoint1=Irange(current+1,0,this->NumPoints-1);
		    //nextpoint2=Irange(current+2,0,this->NumPoints-1);
		  }
	      
	      double xp1[3],xn1[3];//,xn1[3],xn2[3];
	      //this->Points->GetPoint(prevpoint2,xp2);
	      this->Points->GetPoint(prevpoint1,xp1);
	      this->Points->GetPoint(nextpoint1,xn1);
	      //this->Points->GetPoint(nextpoint2,xn2);

	      // Estimate Normal
	      float tx=sp[0]*(xn1[0]-xp1[0]);
	      float ty=sp[1]*(xn1[1]-xp1[1]);
	      float mag=Frange(sqrt(tx*tx+ty*ty),0.01,100.0);
	      float nx=-direction*ty/mag;
	      float ny=direction*tx/mag;
	      
	      float inside=1.0;

	      /*float dot=
		  ((x[0]-0.5*(xp1[0]+xn1[0]))*nx)+
		      ((x[1]-0.5*(xp1[1]+xn1[1]))*ny);
	      if (dot<0.0)
		  inside=-1.0;*/
	      
	      // Estimate External Energy Function Change Along Normal
	      float ext_f=(vtkpxBaseCurve::GetImageValue(energyMap,xc[0]+dr*nx,xc[1]+dr*ny,0,sp,ori,dim2)-
			   vtkpxBaseCurve::GetImageValue(energyMap,xc[0]-dr*nx,xc[1]-dr*ny,0,sp,ori,dim2))
		  /(2.0*dr*scale_int);
	      float ext_2f=(vtkpxBaseCurve::GetImageValue(energyMap,xc[0]+dr*nx,xc[1]+dr*ny,0,sp,ori,dim2)+
			    vtkpxBaseCurve::GetImageValue(energyMap,xc[0]-dr*nx,xc[1]-dr*ny,0,sp,ori,dim2))/(range[1]-range[0]);

	      // If there is no gradient around just expand
	      if (ext_2f<0.1)
		  ext_f=0.05;
	      
	      // Estimate Internal Energy Function Along Normal
	      float ex11 = (2.0*(x[0]+dr*nx)-(xp1[0]+xn1[0]));
	      float ey11 = (2.0*(x[1]+dr*ny)-(xp1[1]+xn1[1]));
	      float ex12 = (2.0*(x[0]-dr*nx)-(xp1[0]+xn1[0]));   
	      float ey12 = (2.0*(x[1]-dr*ny)-(xp1[1]+xn1[1]));
	      
	      float int_f = ( sqrt(ex11*ex11+ey11*ey11) -  sqrt( ex12*ex12+ey12*ey12) )/(2.0*dr);
	      
	      // Find Total Shift
	      float shift=(-smooth*int_f + edge* ext_f + baloon*inside);
	      float dx= 0.2*shift*nx;
	      float dy= 0.2*shift*ny;
	      this->Points->SetPoint(current,x[0]+dx,x[1]+dy,x[2]);

	      // Check for convergence
	      float ds=sqrt(dx*dx+dy*dy);
	      max_move=Fmax(ds,max_move);
	    }
	if (debugmode)
	    fprintf(stderr,"Max_move =%f tolerance=%f\n",max_move,tolerance);
	
	if (max_move<tolerance)
	    {
	      n=maxiter+1;
	    }
	else if (int(n/20)*20==n)
	    {
	      RemoveIntersections(PXTrue,plane,level);
	    }
      }
  
  residual=max_move;
  float perimeter=basecurve_getperimeter(this->Points,this->ClosedCurve,this->NumPoints);
  float Spacing=perimeter/float(this->NumPoints);
  float newSpacing=Frange(Spacing*0.95,1.5,20.0);
  /*if (newSpacing<2.0)
    PointScale=0.5;*/
  RemoveIntersections(PXTrue,plane,level);
  Equispace(newSpacing);
  
  // Update Display
  // --------------
  /*fprintf(stderr,"Perimeter=%f this->NumPoints=%d -> %d Spacing=%f -> %f\n",
	  perimeter,np,this->NumPoints,Spacing,newSpacing);*/

  return energyMap;
}

/* -------------------------------------------------------------------------*/
void vtkpxBaseCurve::Execute()
{
  vtkPolyData* output = this->GetOutput();
  
  if ( (this->ShowControls==0 && this->ShowCurve==0 && this->ShowBoundingBox==0) || this->NumPoints<1)
    {
      output->Initialize();
      // Just Create a dummy sphere in this case 
      vtkSphereSource* sphere=vtkSphereSource::New();
      sphere->SetRadius(0.01);
      sphere->SetThetaResolution(3);
      sphere->SetPhiResolution(3);
      sphere->Update();
      output->CopyStructure(sphere->GetOutput());
      sphere->Delete();
      return;
    }

  /*fprintf(stderr,"Drawing Curve  %d (showc=%d showcont=%d)\n",
    this->NumPoints,this->ShowCurve,this->ShowControls);*/

  float pscalefactor=this->PointScale;
  pscalefactor*=this->Spacing[2];
  //  fprintf(stderr,"Point Scale %f --> %f\n",this->PointScale,pscalefactor);

  vtkPolyData* curve=vtkPolyData::New();
  
  vtkPoints    *c_points    = vtkPoints::New();
  vtkCellArray *c_lines     = vtkCellArray::New();
  
  c_points->Allocate(this->NumPoints,10);
  c_lines->Allocate(this->NumPoints-1+this->ClosedCurve,5);
  
  int np=this->NumPoints;
  int index=-1;
  vtkIdType pt[2];
  
  for (int i=0;i<np;i++)
    {
      double x[3],tx[3];
      this->Points->GetPoint(i,x);
      int add=1;

      if (i>0)
	{
	  this->Points->GetPoint(i-1,tx);
	  if (Fsamepoint(x[0],x[1],x[2],tx[0],tx[1],tx[2],0.05))
	    add=0;
	}

      if (add==1)
	{
	  ++index;
	  for (int j=0;j<=2;j++)
	    x[j]=x[j]*Spacing[j]+Origin[j];
	  c_points->InsertNextPoint(x);
      
	  if (i<np-1 && np>1)
	    {
	      pt[0]=index;
	      pt[1]=index+1;
	      c_lines->InsertNextCell(2,pt);
	    }
	}
    }
  
  if (this->ClosedCurve==1)
    {
      pt[0]=index;
      pt[1]=0;
      c_lines->InsertNextCell(2,pt);
    }
    
  
  curve=vtkPolyData::New();
  curve->SetPoints(c_points);
  curve->SetLines(c_lines);
  c_points->Delete();
  c_lines->Delete();


  vtkAppendPolyData* append=vtkAppendPolyData::New();

  if (this->ShowBoundingBox && np>2)
    {
      vtkOutlineFilter* outline=vtkOutlineFilter::New();
      outline->SetInput(curve);
      outline->Update();
      append->AddInput(outline->GetOutput());
      outline->Delete();
    }

  if (this->ShowCurve && np>1)
    {
      vtkTubeFilter* tube=vtkTubeFilter::New();
      //fprintf(stderr,"Doing Tube\n");
      //tube->DebugOn();
      tube->SetInput(curve);
      tube->SetRadius(0.375*pscalefactor);
      tube->SetNumberOfSides(4);
      tube->SetVaryRadiusToVaryRadiusOff();
      tube->Update();
      append->AddInput(tube->GetOutput());
      tube->Delete();

    }

  if (this->ShowControls)
    {
      vtkGlyph3D* glyph=vtkGlyph3D::New();
      if (this->ControlsDisplayMode==0)
	{
	  vtkpxCrossHairSource* sphere=vtkpxCrossHairSource::New();
	  sphere->SetLength(pscalefactor);
	  sphere->Update();
	  glyph->SetSource(sphere->GetOutput());
	  sphere->Delete();
	}
      else
	{
	  vtkDiskSource* disk=vtkDiskSource::New();
	  disk->SetInnerRadius(pscalefactor*0.5);
	  disk->SetOuterRadius(pscalefactor);
	  disk->SetRadialResolution(2);
	  disk->SetCircumferentialResolution(8);
	  disk->Update();
	  glyph->SetSource(disk->GetOutput());
	  disk->Delete();
	}
      glyph->SetInput(curve);
      glyph->Update();
      append->AddInput(glyph->GetOutput());
      glyph->Delete();
    }
  append->Update();


  output->CopyStructure(append->GetOutput());
  append->Delete();
  curve->Delete();

}
/* -------------------------------------------------------------------------*/
void vtkpxBaseCurve::RescaleSpacing(double sx,double sy,double sz)
{
  double sp[3],ori[3];
  for (int ia=0;ia<=2;ia++)
    ori[ia]=this->Origin[ia];

  sp[0]=sx;
  sp[1]=sy;
  sp[2]=sz;
  RescaleOriginSpacing(ori,sp);
}
/* -------------------------------------------------------------------------*/
void vtkpxBaseCurve::RescaleOriginSpacing(double ori[3],double sp[3])
{
  int donothing=1;
  int i;
  for (i=0;i<=2;i++)
    if (this->Spacing[i]!=sp[i] || this->Origin[i]!=ori[i])
      donothing=0;

  if (donothing)
    return;

  double oldsp[3],oldori[3];
  for (i=0;i<=2;i++)
    {
      oldsp[i]=this->Spacing[i];
      oldori[i]=this->Origin[i];
    }

  this->SetSpacing(sp);
  this->SetOrigin(ori);

  int np=this->NumPoints;
  for (i=0;i<np;i++)
    {
      double x[3];
      this->Points->GetPoint(i,x);
      for (int ia=0;ia<=2;ia++)
	{
	  x[ia]=x[ia]*oldsp[ia]+oldori[ia];
	  x[ia]=(x[ia]-this->Origin[ia])/this->Spacing[ia];
	}
      this->Points->SetPoint(i,x);
    }
  this->Modified();
}
/* -------------------------------------------------------------------------*/
int vtkpxBaseCurve::AddToImage(vtkImageData* image,float value)
{
  if (this->NumPoints<2)
    return 0;
  
  if (image==NULL)
    return 0;

  vtkDataArray* scal=image->GetPointData()->GetScalars();
  if (scal==NULL)
    return 0;

  vtkTransform* trans=vtkTransform::New();
  trans->Identity();
  trans->PostMultiply();
  trans->Scale(this->Spacing);
  trans->Translate(this->Origin);

  for(int i=1;i<=this->NumPoints;i++)
    {
      double x1[3],x2[3],p[3],d[3],tx[3],grad[3];
      int k;

      this->Points->GetPoint(i-1,x1);
      if (i!=this->NumPoints)
	this->Points->GetPoint(i,x2);
      else
	this->Points->GetPoint(0,x2);

      float maxdist=0.0;
      int   maxindex=0;
      
      for (k=0;k<=2;k++)
	{
	  d[k]=x2[k]-x1[k];
	  if (fabs(d[k])>fabs(maxdist))
	    {
	      maxdist=d[k];
	      maxindex=k;
	    }
	}

      // fprintf(stderr,"\nPoint 1 = [%.1f %.1f %.1f ]\t",x1[0],x1[1],x1[2]);
      // fprintf(stderr,"Point 2 = [%.1f %.1f %.1f ]\t",x2[0],x2[1],x2[2]);
      // fprintf(stderr,"Dist    = [%.1f %.1f %.1f ]\t%f,ind=%d\t",d[0],d[1],d[2],maxdist,maxindex);
      
      for (k=0;k<=2;k++)
	{
	  if (d[k]!=0.0)
	    grad[k]=d[k]/maxdist;
	  else
	    grad[k]=0.0;
	}
      
      // fprintf(stderr,"Gradients = [ %.2f %.2f %.2f ] %.2f \n",grad[0],grad[1],grad[2],maxdist);
      
      
      for (float dt=0.0;dt<fabs(maxdist);dt+=0.25)
	{
	  if (maxdist==0)
	    {
	      for (k=0;k<=2;k++)
		p[k]=x1[k];
	    }
	  else
	    {
	      for (k=0;k<=2;k++)
		p[k]=x1[k]+(dt/fabs(maxdist))*d[k];
	    }
	  
	  // fprintf(stderr,"Adding point %.2f (%.1f %.1f %.1f)\n",dt,p[0],p[1],p[2]);
	  
	  trans->TransformPoint(p,tx);
	  int index=image->FindPoint(tx);
	  
	  if (index>0)
	    scal->SetComponent(index,0,value);
	}
    
      if (this->ClosedCurve==0 && i==this->NumPoints-1)
	++i;
    }
  return 1;
}

//------------------------------------------------------------------------------
// Help with Snake Implementation
//------------------------------------------------------------------------------

double vtkpxBaseCurve::GetDoubleVoxel(vtkImageData* img,int i,int j,int k,int dimensions[3])
{
  if (img==NULL)
      return 0.0;

  i=Irange(i,0,dimensions[0]-1);
  j=Irange(j,0,dimensions[1]-1);
  k=Irange(k,0,dimensions[2]-1);
  int act=0;//img->GetPointData()->GetScalars()->GetActiveComponent();
  
  return img->GetScalarComponentAsDouble(i,j,k,act);
}


double vtkpxBaseCurve::GetImageValue(vtkImageData* img,double x,double y,double z,double spacing[3],double origin[3],int dim[3])
{
  //  vtkScalars* imageScalars=img->GetPointData()->GetScalars();
  
  double p[3];
  p[0]=x;p[1]=y;p[2]=z;
  
  // Find Point ``to the left''
  int pt[3];
  for (int id=0;id<=2;id++)
      pt[id]=int((p[id]-origin[id])/spacing[id]);

  // Calculate Weights
  double fpt[3][2];
  for (int i=0;i<=2;i++)
      {
	fpt[i][0]=double((pt[i]+1)*spacing[i])+origin[i]-p[i];
	fpt[i][1]=p[i]-(double(pt[i]*spacing[i])+origin[i]);
	double s=fpt[i][0]+fpt[i][1];
	if (s==0.0)
	    {
	      for (int k=0;k<=1;k++)
		  fpt[i][k]=0.5;
	    }
	else
	    {
	      for (int k=0;k<=1;k++)
		  fpt[i][k]/=s;
	    }
      }    
  
  // Find Value
  double sum=0.0;
  for (int ia=0;ia<=1;ia++)
      for (int ib=0;ib<=1;ib++)
	  for (int ic=0;ic<=1;ic++)
	      {
		double v=GetDoubleVoxel(img,pt[0]+ia,pt[1]+ib,pt[2]+ic,dim);
		double w=fpt[0][ia]*fpt[1][ib]*fpt[2][ic];
		sum+=v*w;
	      }
  return sum;
 
}

double vtkpxBaseCurve::GetImageVoxel(vtkImageData* img,double x,double y,double z,int dim[3])
{
  if (img==NULL)
      return 0.0;

  x=Frange(x,0.0,dim[0]-2);
  y=Frange(y,0.0,dim[1]-2);
  z=Frange(z,0.0,dim[2]-2);

  int x1=(int)x,x2=x1+1;
  int y1=(int)y,y2=y1+1;
  int z1=(int)z,z2=z1+1;

  int act=0;//img->GetPointData()->GetScalars()->GetActiveComponent();

  return (img->GetScalarComponentAsDouble(x1,y1,z1,act)*(x2-x)*(y2-y)*(z2-z)+
	  img->GetScalarComponentAsDouble(x1,y2,z1,act)*(x2-x)*(y-y1)*(z2-z)+
	  img->GetScalarComponentAsDouble(x2,y1,z1,act)*(x-x1)*(y2-y)*(z2-z)+
	  img->GetScalarComponentAsDouble(x2,y2,z1,act)*(x-x1)*(y-y1)*(z2-z)+
	  img->GetScalarComponentAsDouble(x1,y1,z2,act)*(x2-x)*(y2-y)*(z-z1)+
	  img->GetScalarComponentAsDouble(x1,y2,z2,act)*(x2-x)*(y-y1)*(z-z1)+
	  img->GetScalarComponentAsDouble(x2,y1,z2,act)*(x-x1)*(y2-y)*(z-z1)+
	  img->GetScalarComponentAsDouble(x2,y2,z2,act)*(x-x1)*(y-y1)*(z-z1));
  
}


vtkImageData*  vtkpxBaseCurve::CreateEnergyMap(vtkImageData* image,int plane,int level,int fr,float sigma)
{
  if (image==NULL)
    return NULL;
  vtkpxImageExtract* extr=vtkpxImageExtract::New();
  extr->SetInput(image);
  extr->SetCurrentPlane(plane);
  extr->SetSliceNo(level);
  extr->SetFrame(fr);
  extr->Update();
	
  vtkImageGaussianSmooth* smoothFilter=vtkImageGaussianSmooth::New();
  smoothFilter->SetStandardDeviations(sigma,sigma,0.0);
  smoothFilter->SetRadiusFactors(3.0*sigma,3.0*sigma,1);
  smoothFilter->SetInput(extr->GetOutput());
  smoothFilter->Update();
  
  vtkImageGradientMagnitude* magn=vtkImageGradientMagnitude::New();
  magn->SetInput(smoothFilter->GetOutput());
  magn->SetHandleBoundaries(1);
  magn->SetDimensionality(2);
  magn->Update();
  
  extr->Delete();
  smoothFilter->Delete();
  
  vtkImageData* energyMap=vtkImageData::New();
  energyMap->ShallowCopy(magn->GetOutput());
  magn->Delete();
  return energyMap;
}

/* -------------------------------------------------------------------------*/
int vtkpxBaseCurve::ExportToPolyData(vtkPolyData* output,float d)
{
  if (d>0.1)
    this->Equispace(d);

  output->Initialize();

  vtkPoints    *c_points    = vtkPoints::New();
  vtkCellArray *c_lines     = vtkCellArray::New();
  
  c_points->Allocate(this->NumPoints,10);
  c_lines->Allocate(this->NumPoints-1+this->ClosedCurve,5);
  
  int np=this->NumPoints;
  int index=-1;
  vtkIdType pt[2];
  
  for (int i=0;i<np;i++)
    {
      double x[3],tx[3];
      this->Points->GetPoint(i,x);
      int add=1;

      if (i>0)
	{
	  this->Points->GetPoint(i-1,tx);
	  if (Fsamepoint(x[0],x[1],x[2],tx[0],tx[1],tx[2],0.05))
	    add=0;
	}

      if (add==1)
	{
	  ++index;
	  for (int j=0;j<=2;j++)
	    x[j]=x[j]*Spacing[j]+Origin[j];
	  c_points->InsertNextPoint(x);
      
	  if (i<np-1 && np>1)
	    {
	      pt[0]=index;
	      pt[1]=index+1;
	      c_lines->InsertNextCell(2,pt);
	    }
	}
    }
  
  output->SetPoints(c_points);
  output->SetLines(c_lines);
  c_points->Delete();
  c_lines->Delete();
  return 1;
}

