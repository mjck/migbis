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
  Module:    $RCSfile: vtkpxGUIMultipleMosaicViewer.cpp,v $
  Language:  C++
  Date:      $Date: 2004/02/03 15:28:38 $
  Version:   $Revision: 1.3 $


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

#include "pxutil.h"
#include "vtkActor.h"
#include "vtkImageQuantizeRGBToIndex.h"
#include "vtkObjectFactory.h"
#include "vtkpxGUIMultipleMosaicViewer.h"
#include "vtkpxTalairachTransform.h"
// -------------------------------------------------------------------------
vtkpxGUIMultipleMosaicViewer* vtkpxGUIMultipleMosaicViewer::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIMultipleMosaicViewer");
  if(ret)
      {
	return (vtkpxGUIMultipleMosaicViewer*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIMultipleMosaicViewer;
}
// -------------------------------------------------------------------------
// Construct object with no children.
vtkpxGUIMultipleMosaicViewer::vtkpxGUIMultipleMosaicViewer()
{
  for (int i=0;i<=3;i++)
    AuxImages[i]=NULL;
}
// -------------------------------------------------------------------------
vtkpxGUIMultipleMosaicViewer::~vtkpxGUIMultipleMosaicViewer()
{
  for (int i=0;i<=3;i++)
    {
      if (AuxImages[i]!=NULL)
	AuxImages[i]->Delete();
    }

}
/* -------------------------------------------------------------------------*/
int vtkpxGUIMultipleMosaicViewer::ResetViewer()
{
  //  fprintf(stderr,"Reseting Viewer check for changes to auxiliary images\n");
  if (!(hasImage && this->Initialized))
      return 0;

  int dim[3],auxdim[3];

  this->currentImage->GetDimensions(dim);

  int flag=0;
  for (int i=0;i<=3;i++)
    {
      if (this->AuxImages[i]!=NULL)
	{
	  this->AuxImages[i]->GetDimensions(auxdim);
	  for (int i=0;i<=2;i++)
	    if (dim[i]!=auxdim[i])
	      flag++;
	}
    }

  if (flag>0)
    {
      for (int i=0;i<=3;i++)
	{
	  if (this->AuxImages[i]!=NULL)
	    this->AuxImages[i]->Delete();
	  this->AuxImages[i]=NULL;
	}
    }
  
  if (this->GetNumberOfImages()<=1)
    {
      viewerPresetsRow->enable(PXTrue);
      vtkpxGUIMosaicViewer::ResetViewer();
    }
  else
    {
      viewerPresetsRow->enable(PXTrue);
      viewerPresetsRow->setValue((float)this->GetNumberOfImages(),PXFalse);
      viewerPresetsRow->enable(PXFalse);
      this->SetConfiguration(this->GetNumberOfImages(),this->NumberOfColumns);
    }
  return TCL_OK;
}

// -------------------------------------------------------------------------
int vtkpxGUIMultipleMosaicViewer::UpdateViewers(int firstv,int lastv)
{
  if (!hasImage)
      return TCL_ERROR;

  firstv=Irange(firstv,0,this->NumberOfViewers-1);
  lastv=Irange(lastv,0,this->NumberOfViewers-1);

  // Read UI variables 
  int plane=2;
  if (!single_slice_mode && !xy_slice_only )
      plane=currentPlane->getIndex();
  int dim[3];     currentImage->GetDimensions(dim);
  int frame=this->CurrentFrame;

  double sp[3];  currentImage->GetSpacing(sp);
  double ori[3]; currentImage->GetOrigin(ori);

  this->EventManager->setWaitCursor(PXTrue);

  if (this->Orientation==3)
    {
      dim[0]=dim[1];
      dim[1]=dim[2];
    }

  //  fprintf(stderr,"\n************************************************\n Updating Viewers %d:%d\n",firstv,lastv);

  for (int k=firstv;k<=lastv;k++)
    {
      //      fprintf(stderr,"********* Updating Viewer %d\n",k);
      int doreset=0;
      int doclip=1;
      if (this->Orientation!=3)
	{
	  renderer[k]->SetClipMode(PXTrue);
	  doclip=1;
	}
      else
	renderer[k]->SetClipMode(PXFalse);

      renderer[k]->SetCameraScale(renderer[0]->GetCameraScale());

      if (renderer[k]->GetPlaneMode()!=plane)
	{
	  doreset=1;
	  renderer[k]->SetPlaneMode(plane);
	  imageSlice[k]->SetCurrentPlane(plane);
	  renderer[k]->SetFlipMode(0);
	}
      
      int slice=this->GetViewerSlice(k);
      int row=this->GetRow(k);
      vtkImageData* img=this->GetIndexedImage(row);

      //      fprintf(stderr,"\t\t\t Slice=%d row=%d \n",slice,row);

      renderer[k]->SetClipBounds(slice,slice,slice,sp,ori);
      renderer[k]->SetViewerCoordinates(slice,slice,slice);
      
      if (slice<dim[plane] && slice>=0)
	{
	  imageSlice[k]->SetInput(img);
	  imageSlice[k]->SetFrame(frame);
	  imageSlice[k]->SetLevel(slice);
	  imageSlice[k]->SetCurrentPlane(plane);
	  imageSlice[k]->SetDisplayMode(2);
	  imageSlice[k]->Update();
	  renderer[k]->SetViewerCoordinates(slice,slice,slice);
	  char line[250];

  
	  if (!single_slice_mode && !xy_slice_only ) 
	    {
	      int axis;
	      double mni=this->GetMNISliceNo(slice,plane,axis);
	      if (axis==-1)
		{
		  sprintf(line,"Slice=%d/%d (%s)",
			  slice,dim[plane]-1,
			  currentPlane->getLabel(imageSlice[k]->GetCurrentPlane()));
		}
	      else
		{
		  switch (axis) {
		  case 0: sprintf(line,"MNI X=%.0f Slice=%d/%d",mni,slice,dim[plane]-1); break;
		  case 1: sprintf(line,"MNI Y=%.0f Slice=%d/%d",mni,slice,dim[plane]-1); break;
		  case 2: sprintf(line,"MNI Z=%.0f Slice=%d/%d",mni,slice,dim[plane]-1); break;
		  }
		}
	      renderer[k]->SetLabel(line,0.05,0.05,0.73,0.73,0.0);
	    }
	}
      else
	{
	  renderer[k]->SetLabel("None",0.05,0.05,
				0.8,0.8,0.0);
	  imageSlice[k]->SetDisplayMode(1);
	  imageSlice[k]->Update();
	}
      
      if (this->showLabels!=NULL)
	{
	  if (this->showLabels->getState()==PXTrue)
	    renderer[k]->ShowLabel();
	  else
	    renderer[k]->HideLabel();
	}

      if (this->Orientation==3)
	renderer[k]->MoveAbove();

      if (doreset)
	{
	  renderer[k]->SetClipThickness(currentImage->GetSpacing()[plane]);
	  renderer[k]->Reset();
      	}
      
      if (doclip)
	renderer[k]->Clip();

    }  
  this->EventManager->setWaitCursor(PXFalse);
  return TCL_OK;
}

int vtkpxGUIMultipleMosaicViewer::GetNumberOfImages()
{
  if (!hasImage)
    return 0;

  int numimg=1;
  for (int i=0;i<=3;i++)
    {
      if (this->AuxImages[i]!=NULL)
	numimg++;
    }

  //  fprintf(stderr,"\t\t Number of Images=%d\n",numimg);

  return numimg;

}

vtkImageData* vtkpxGUIMultipleMosaicViewer::GetIndexedImage(int index)
{
  if (!hasImage)
      return NULL;

  if (this->GetNumberOfImages()==1)
    {
      //      fprintf(stderr,"Only One image in memory returning current\n");
      return this->currentImage;
    }

  if (index>4)
    {
      //      fprintf(stderr,"Bad Indexed Image=%d\n",index);
      return NULL;
    }

  if (index<=0)
    {
      //      fprintf(stderr,"Indexed image %d = current\n",index);
      return this->currentImage;
    }


  int count=0;
  int target=index;
  int newcount=0;

  //  fprintf(stderr,"Looking for indexed=%d\n",target);

  while (count<=3)
    {
      if (this->AuxImages[count]!=NULL)
	{
	  ++newcount;
	  //	  fprintf(stderr,"\t\t AuxImage[%d] not NULL newcound=%d (target=%d)\n",
	  //  count,newcount,target);
	  if (newcount==target)
	    {
	      //	      fprintf(stderr,"Indexed image %d = aux[%d]\n",index,count);
	      return this->AuxImages[count];
	    }
	} 

      ++count;
    }

  //  fprintf(stderr,"Bad Indexed Image=%d\n",index);
  return NULL;
}
// -------------------------------------------------------------------------
void vtkpxGUIMultipleMosaicViewer::ClearAuxImages()
{
  for (int i=0;i<=3;i++)
    {
      if (this->AuxImages[i]!=NULL)
	this->AuxImages[i]->Delete();
      this->AuxImages[i]=NULL;
    }
  viewerPresetsRow->enable(PXTrue);
  vtkpxGUIMosaicViewer::ResetViewer();
}
// -------------------------------------------------------------------------
void vtkpxGUIMultipleMosaicViewer::SetConfiguration(int numr,int numc)
{
  if (!hasImage)
      return;
  int numimg=this->GetNumberOfImages();

  if (numimg==1)
    vtkpxGUIMosaicViewer::SetConfiguration(numr,numc);
  else
    vtkpxGUIMosaicViewer::SetConfiguration(numimg,numc);

  ChangeSliceIncrement(-1,-1,-1);
  this->UpdateStatusLabels();

}
// -------------------------------------------------------------------------
int  vtkpxGUIMultipleMosaicViewer::GetViewerSlice(int v)
{
  if (!this->Initialized)
    return 0;


  if (this->GetNumberOfImages()<=1)
    return vtkpxGUIMosaicViewer::GetViewerSlice(v);

  v=Irange(v,0,this->NumberOfViewers-1);
  int row=this->GetRow(v);
  int col=v-row*this->NumberOfColumns;
    
  int firstslice=(int)beginSlice->getValue()-offset;
  if (firstslice<0)
      firstslice=0;
  
  int increment=this->GetIncrement();
  int final=firstslice+col*increment;

  //  fprintf(stderr,"Viewer =%d slice=%d (row=%d,col+%d)\n",v,final,row,col);

  return final;
	  
}
// -------------------------------------------------------------------------	    
int vtkpxGUIMultipleMosaicViewer::GetRow(int v)
{
  v=Irange(v,0,this->NumberOfViewers-1);
  if (!this->Initialized)
    return 0;
  
  int row=v/this->NumberOfColumns;
  //  fprintf(stderr,"Viewer =%d row=%d (num_cols=%d)\n",v,row,this->NumberOfColumns);
  return row;
}
// -------------------------------------------------------------------------	    

void vtkpxGUIMultipleMosaicViewer::SetAuxImage(vtkImageData* img,int index)
{
  if (this->currentImage==NULL)
    return;

  if (index<0)
    index=0;
  else if (index>3)
    index=3;

  if (img==NULL)
    {
      if (this->AuxImages[index]!=NULL)
	this->AuxImages[index]->Delete();
      this->AuxImages[index]=NULL;
      return;
    }

  int dim[3],auxdim[3],flag=0;
  this->currentImage->GetDimensions(dim);
  img->GetDimensions(auxdim);
  for (int i=0;i<=2;i++)
    if (dim[i]!=auxdim[i])
      flag++;

  if (flag>0)
    return;

  
  if (this->AuxImages[index]==NULL)
    this->AuxImages[index]=vtkImageData::New();

  this->AuxImages[index]->ShallowCopy(img);
  //  fprintf(stderr,"Setting Aux Image=%d\n",index);

  if (this->GetNumberOfImages()<=1)
    {
      viewerPresetsRow->enable(PXTrue);
      this->SetConfiguration(this->NumberOfRows,this->NumberOfColumns);
    }
  else
    {
      viewerPresetsRow->enable(PXTrue);
      viewerPresetsRow->setValue((float)this->GetNumberOfImages(),PXFalse);
      viewerPresetsRow->enable(PXFalse);
      this->SetConfiguration(this->GetNumberOfImages(),this->NumberOfColumns);
    }

}



