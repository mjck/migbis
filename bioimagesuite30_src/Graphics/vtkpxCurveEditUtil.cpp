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





/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxCurveEditUtil.cpp,v $
  Language:  C++
  Date:      $Date: 2003/06/05 21:01:46 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#include "vtkpxCurveEditUtil.h"
#include "vtkpxUtil.h"
#include "vtkpxBaseCurve.h"
#include "vtkActor.h"
#include "vtkSphereSource.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"

// ---------------------------

vtkpxCurveEditUtil* vtkpxCurveEditUtil::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxCurveEditUtil");
  if(ret)
      {
	return (vtkpxCurveEditUtil*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxCurveEditUtil;
}

// ------------------------------------------------------------------------------------------------------------------

void vtkpxCurveEditUtil::AddListLandmark(float x,float y,float z,
					 int index,int scaled,
					 vtkpxBaseCurve* landmarkCollection)
{
  if (index==-1)
    landmarkCollection->AddPoint(x,y,z,scaled);
  else
    landmarkCollection->SetPoint(index,x,y,z,scaled);    

  landmarkCollection->Modified();
  landmarkCollection->Update();
}

// ------------------------------------------------------------------------------------------------------------------

int vtkpxCurveEditUtil::HandleClickedPoint(float x,float y,float z,int scaled,
					   int state,int mode,int index,
					   vtkpxBaseCurve* landmarkCollection,
					   vtkSphereSource* highlightData,
					   vtkActor* highlightActor)
{
  if (mode==0)
      return -1;
  
  //int index=this->highlightIndex;//listBox->getCurrentItem();
  int numitems=landmarkCollection->GetNumPoints();//listBox->getNumItems();
  
  switch (mode)
    {
    case 4:  // Continuous Add Mode 
      switch(state)
	{
	case 0:
	  break;
	case 1:
	  vtkpxCurveEditUtil::AddListLandmark(x,y,z,-1,scaled,landmarkCollection);
	  break;
	case 2:
	  vtkpxCurveEditUtil::SetHighlight(landmarkCollection,highlightData,highlightActor,index);
	  break;
	}
      return index;
      break;
      

    case 3:  // Add Mode 
      switch(state)
	{
	case 0:
	  vtkpxCurveEditUtil::AddListLandmark(x,y,z,-1,scaled,landmarkCollection);
	  index=landmarkCollection->GetNumPoints()-1;
	  vtkpxCurveEditUtil::SetHighlight(landmarkCollection,highlightData,highlightActor,index);
	  break;
	case 1:
	  vtkpxCurveEditUtil::AddListLandmark(x,y,z,index,scaled,landmarkCollection);
	  vtkpxCurveEditUtil::SetHighlight(landmarkCollection,highlightData,highlightActor,index);
	  break;
	case 2:
	  vtkpxCurveEditUtil::AddListLandmark(x,y,z,index,scaled,landmarkCollection);
	  vtkpxCurveEditUtil::SetHighlight(landmarkCollection,highlightData,highlightActor,index);
	  break;
	}
      return index;
      break;
      
    case 1: // Pick Mode 
      if (numitems<1)
	return -1;
      switch (state)
	{
	case 0:
	  {
	    double lx[3];
	    lx[0]=x;lx[1]=y;lx[2]=z;
	    index=landmarkCollection->FindClosestPoint(lx,landmarkCollection->GetPointScale()*1.2,scaled);
	    vtkpxCurveEditUtil::SetHighlight(landmarkCollection,highlightData,highlightActor,index);
	    break;
	  }
	case 1:
	  if (index>=0)
	    {
	      vtkpxCurveEditUtil::AddListLandmark(x,y,z,index,scaled,landmarkCollection);
	      vtkpxCurveEditUtil::SetHighlight(landmarkCollection,highlightData,highlightActor,index);
	    }
	    break;
	case 2:
	  if (index>=0)
	    {
	      vtkpxCurveEditUtil::AddListLandmark(x,y,z,index,scaled,landmarkCollection);
	      vtkpxCurveEditUtil::SetHighlight(landmarkCollection,highlightData,highlightActor,index);
	    }
	  break;
	}
      return index;
      break;
      
    case 2: // Edit Mode 
      if (index<0)
	return 0;
      switch(state)
	{
	case 0:
	  vtkpxCurveEditUtil::AddListLandmark(x,y,z,index,scaled,landmarkCollection);
	  break;
	case 1:
	  vtkpxCurveEditUtil::AddListLandmark(x,y,z,index,scaled,landmarkCollection);
	  vtkpxCurveEditUtil::SetHighlight(landmarkCollection,highlightData,highlightActor,index);
	  break;
	case 2:
	  vtkpxCurveEditUtil::AddListLandmark(x,y,z,index,scaled,landmarkCollection);
	  if (index<numitems-1)
	    index++;
	  vtkpxCurveEditUtil::SetHighlight(landmarkCollection,highlightData,highlightActor,index);
	  break;
	}
      return index;
    }
  return -1;
}

/* -------------------------------------------------------------------------*/
void vtkpxCurveEditUtil::SetHighlightDisplay(int disp,int highlightIndex,
					     vtkSphereSource* highlightData,vtkActor* highlightActor)
{
  int flag=1;
  if (disp==0 || highlightIndex==-1)
    flag=0;

  if (highlightActor!=NULL)
    {
      if (highlightActor->GetVisibility() != flag )
	    highlightActor->SetVisibility(flag);
    }

}	  


void vtkpxCurveEditUtil::SetHighlight(vtkpxBaseCurve* landmarkCollection,vtkSphereSource* highlightData,
				      vtkActor* highlightActor,int numh)
{

  if (highlightData==NULL || landmarkCollection==NULL)
    {
      SetHighlightDisplay(0,-1,highlightData,highlightActor);
      return;
    }

  int np=landmarkCollection->GetNumPoints();
  
  if (numh<0 || np<1)
    {
      highlightData->SetRadius(0.01);
      highlightData->Update();
      SetHighlightDisplay(0,-1,highlightData,highlightActor);
      return;
    }

  numh=Irange(numh,0,np-1);

  double sp[3]; landmarkCollection->GetSpacing(sp);
  double ori[3]; landmarkCollection->GetOrigin(ori);

  numh=Irange(numh,0,np-1);

  double x[3];
  landmarkCollection->GetPoints()->GetPoint(numh,x);
  for (int ia=0;ia<=2;ia++)
    x[ia]=x[ia]*sp[ia]+ori[ia];

  highlightData->SetCenter(x);
  highlightData->SetRadius(1.5*landmarkCollection->GetPointScale());
  highlightData->Update();
  SetHighlightDisplay(1,numh,highlightData,highlightActor);
}



