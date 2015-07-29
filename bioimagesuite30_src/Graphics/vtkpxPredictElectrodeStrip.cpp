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

 see vtkpxPredictElectrodeStrip.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkAppendPolyData.h"
#include "vtkCellArray.h"
#include "vtkUnsignedShortArray.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkpxPredictElectrodeStrip.h"
#include "vtkpxElectrodeSource.h"
#include "vtkCollection.h"
#include "vtkUnsignedCharArray.h"
#include "vtkShortArray.h"
#include "vtkMath.h"
#include "vtkFloatArray.h"
#include "vtkThinPlateSplineTransform.h"
//------------------------------------------------------------------------------
vtkpxPredictElectrodeStrip* vtkpxPredictElectrodeStrip::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxPredictElectrodeStrip");
  if(ret)
    {
      return (vtkpxPredictElectrodeStrip*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxPredictElectrodeStrip;
}

// Construct object with no children.
vtkpxPredictElectrodeStrip::vtkpxPredictElectrodeStrip()
{
  this->Length=50.0;
  this->Step=5.0;  
  this->Point1[0]=1.0;  this->Point1[1]=1.0;  this->Point1[2]=1.0;
  this->Point2[0]=0.8;  this->Point2[1]=0.8;  this->Point2[2]=0.8;
  this->Input=NULL;
  this->Smoothness=0.8;
}

/* -------------------------------------------------------------------------*/
 
vtkpxPredictElectrodeStrip::~vtkpxPredictElectrodeStrip()
{
  this->SetInput(NULL);
}
/* -------------------------------------------------------------------------*/
void vtkpxPredictElectrodeStrip::SetInputSurface(vtkPolyData* poly)
{
  this->SetInput(poly);
  if (this->Input==NULL)
    return;
    
  //  this->Input->BuildLinks();
  //  this->Input->BuildCells();
}

/* -------------------------------------------------------------------------*/
void vtkpxPredictElectrodeStrip::Execute()
{
  if (this->Input==NULL)
    {
      vtkErrorMacro(<<"No Input Surface, not executing\n");
      return;
    }


  vtkPolyData* output = this->GetOutput();
  vtkPoints*   points    = vtkPoints::New();
  vtkPoints*   points2   = vtkPoints::New();

  int maxpoints=int(this->Length/this->Step+1.5);
  if (maxpoints<2)
    maxpoints=2;
  float spacing=this->Length/double(maxpoints-1);

  //  fprintf(stderr,"Spacing=%.2f\n",spacing);

  points->Allocate(maxpoints+1,10);

  double Origin[3],Second[3],Direction[3];
  for (int i=0;i<=2;i++)
    Direction[i]=this->Point2[i]-this->Point1[i];
  vtkMath::Normalize(Direction);
  

  int id=this->Input->FindPoint(this->Point1);
  this->Input->GetPoint(id,Origin);

  for (int ia=0;ia<=2;ia++)
    Second[ia]=Origin[ia]+spacing*Direction[ia];

  points->InsertNextPoint(Origin);
  points->InsertNextPoint(Second);
  
  int numpoints=2,flag=1;

  while (numpoints<maxpoints && flag==1)
    {
      // Insert Dummy Point
      points->InsertNextPoint(0.0,0.0,0.0);
      ++numpoints;
      
      // Reset First Point

      for (int i=2;i<numpoints;i++)
	{
	  double p0[3]; points->GetPoint(i-2,p0);
	  double p1[3]; points->GetPoint(i-1,p1);
	  float d[3];
	  for (int ia=0;ia<=2;ia++)
	    d[ia]=p1[ia]-p0[ia];
	  vtkMath::Normalize(d);
	  for (int ia=0;ia<=2;ia++)
	    p1[ia]+=d[ia]*spacing;

	  int id=this->Input->FindPoint(p1);
	  this->Input->GetPoint(id,p1);
	  points->SetPoint(i,p1);
	}

      points2->DeepCopy(points);
  
      double off_w=(1.0-this->Smoothness);
      
      for (int i=2;i<numpoints;i++)
	{
	  double p[3],p1[3];
	  points2->GetPoint(i, p);

	  int im=i-1;
	  points2->GetPoint(im,p1);
	  
	  for (int ia=0;ia<=2;ia++)
	    p[ia]=off_w*p[ia]+this->Smoothness*(p1[ia]);
	  
	  int id=this->Input->FindPoint(p);
	  this->Input->GetPoint(id,p);
	  points->SetPoint(i,p);
	}
    }
      
  vtkCellArray *c_lines     = vtkCellArray::New();
  c_lines->Allocate(points->GetNumberOfPoints(),10);

  for (int i=0;i<points->GetNumberOfPoints()-1;i++)
    {
      vtkIdType pt[2];
      pt[0]=i; pt[1]=pt[0]+1;
      c_lines->InsertNextCell(2,pt);
    }

  output->SetPoints(points); points->Delete();
  output->SetLines(c_lines); c_lines->Delete();
  points2->Delete();
}



