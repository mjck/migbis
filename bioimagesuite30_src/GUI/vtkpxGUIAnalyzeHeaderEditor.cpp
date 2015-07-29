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
  Module:    $RCSfile: vtkpxGUIAnalyzeHeaderEditor.cpp,v $
  Language:  C++
  Date:      $Date: 2005/07/31 20:39:21 $
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
#include "vtkpxGUIAnalyzeHeaderEditor.h"
#include "vtkObjectFactory.h"
#include "pxtkapp.h"
#include "pxutil.h"
#include "vtkImageData.h"

vtkpxGUIAnalyzeHeaderEditor* vtkpxGUIAnalyzeHeaderEditor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIAnalyzeHeaderEditor");
  if(ret)
      {
	return (vtkpxGUIAnalyzeHeaderEditor*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIAnalyzeHeaderEditor;
}

// Construct object with no children.
vtkpxGUIAnalyzeHeaderEditor::vtkpxGUIAnalyzeHeaderEditor()
{
  header=new anaimagedata;
  header_filename=new char[40];
  strcpy(header_filename,"new_header.hdr");
  this->Mode=0;
}

vtkpxGUIAnalyzeHeaderEditor::~vtkpxGUIAnalyzeHeaderEditor()
{
  delete header;
  delete [] header_filename;
}

char*  vtkpxGUIAnalyzeHeaderEditor::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
    return GetWidgetName();
  
  int i;

  vtkpxGUIComponent::Initialize(name,inside);
  PXTkFrame* mainFrame=(PXTkFrame*)(this->EventManager->getMainWindow());
  mainFrame->configure("-bg","black");


  PXTkFrame* otherFrame=new PXTkFrame(this->EventManager,mainFrame);
  mainFrame->addChildren("-side top -expand true -fill both -pady 5",otherFrame);

  /*  if (this->Mode==0)
    {
      PXTkFrame* buttonFrame=new PXTkFrame(this->EventManager,mainFrame);
      PXTkButton* b0=new PXTkButton(this->EventManager,buttonFrame,"Update",104); 
      buttonFrame->addChildren("-side left -expand f -padx 10",b0);
      mainFrame->addChildren("-side top -expand true -fill both -pady 5",buttonFrame);
      }*/


  // Dimensions xsize,ysize,zsize,tsize
  // Pixel Dims xpix,ypix,zpix,orient,MinValue,MaxValue,bits

  PXTkLabelFrame* dimensionframe=new PXTkLabelFrame(this->EventManager,otherFrame,"Image Dimensions (voxels)");
  PXTkLabelFrame* voxelframe=new PXTkLabelFrame(this->EventManager,otherFrame,"Voxel Dimensions (mm)");
  PXTkLabelFrame* intensityframe=new PXTkLabelFrame(this->EventManager,otherFrame,"Intensity Range");
  PXTkLabelFrame* miscframe=new PXTkLabelFrame(this->EventManager,otherFrame,"Image Type");

  otherFrame->addChildren("-side top -fill both -expand t",dimensionframe,voxelframe,intensityframe,miscframe);

  PXTkFrame* barFrame=new PXTkFrame(this->EventManager,otherFrame);
  otherFrame->addChildren("-side bottom -fill x -expand t -pady 2",barFrame);
      
  PXTkButton* m_1=new PXTkButton(this->EventManager,barFrame,"New",100); 
  PXTkButton* m_2=new PXTkButton(this->EventManager,barFrame,"Load",101); 
  PXTkButton* m_3=new PXTkButton(this->EventManager,barFrame,"Save",102); 
  PXTkButton* m_4=new PXTkButton(this->EventManager,barFrame,"Save As",103); 
  barFrame->addChildren("-side left -expand t -fill x -padx 2",m_1,m_2,m_3,m_4);
  if (!this->EventManager->isInside())
    {
      PXTkButton* b0a=new PXTkButton(this->EventManager,barFrame,"Close",3); 
      barFrame->addChildren("-side right -expand t -fill x -padx 10",b0a);
    }

    

  // First Dimension Frame 
  for (i=0;i<=3;i++)
    {
      label[i]=new PXTkLabel(this->EventManager,dimensionframe,"X-Size");
      label[i]->configure("-width",5);
      entry[i]=new PXTkEntry(this->EventManager,dimensionframe);
      entry[i]->configure("-width",8);
      entry[i]->configure("-relief","sunken");
      switch(i) 
	{
	case 0:
	  label[i]->setText("xsize");
	  break;
	case 1:
	  label[i]->setText("ysize");
	  break;
	case 2:
	  label[i]->setText("zsize");
	  break;
	case 3:
	  label[i]->setText("tsize");
	  break;
	}
      dimensionframe->addChildren("-side left -padx 2 -fill x -expand t",label[i],entry[i]);
    }

  // Then Voxel Frame 
  for (i=4;i<=6;i++)
    {
      label[i]=new PXTkLabel(this->EventManager,voxelframe,"X-Size");
      label[i]->configure("-width",5);
      entry[i]=new PXTkEntry(this->EventManager,voxelframe);
      entry[i]->configure("-width",5);
      entry[i]->configure("-relief","sunken");
      switch(i) 
	{
	case 4:
	  label[i]->setText("x-size");
	  break;
	case 5:
	  label[i]->setText("y-size");
	  break;
	case 6:
	  label[i]->setText("z-size");
	  break;
	}
      voxelframe->addChildren("-side left -padx 2 -fill x -expand t",label[i],entry[i]);
    }


  // Then Intensity Frame 
  for (i=7;i<=8;i++)
    {
      label[i]=new PXTkLabel(this->EventManager,intensityframe,"X-Size");
      label[i]->configure("-width",5);
      entry[i]=new PXTkEntry(this->EventManager,intensityframe);
      entry[i]->configure("-width",5);
      entry[i]->configure("-relief","sunken");
      switch(i) 
	{
	case 7:
	  label[i]->setText("Min:");
	  break;
	case 8:
	  label[i]->setText("Max:");
	  break;
	}
      intensityframe->addChildren("-side left -padx 2 -fill x -expand t",label[i],entry[i]);
    }

  PXTkLabel* lab0=new PXTkLabel(this->EventManager,miscframe,"Orientation:");
  orientation=new PXTkOptionMenu(this->EventManager,miscframe,"Axial (0)",-1);
  orientation->addOption("Coronal (1)",-1);
  orientation->addOption("Sagittal (2)",-1);
  orientation->addOption("Other (3)",-1);
  
  PXTkLabel* lab1=new PXTkLabel(this->EventManager,miscframe,"Data Type:");
  data_type=new PXTkOptionMenu(this->EventManager,miscframe,"1-Byte (8)",-1);
  data_type->addOption("Short (2)",-1);
  data_type->addOption("Int   (3)",-1);
  data_type->addOption("Float (4)",-1);
  data_type->setIndex(1);

  byte_swapped=new PXTkCheckButton(this->EventManager,miscframe,"Byte Swapped",-1);
  miscframe->addChildren("-side left -fill x -expand t",lab0,orientation,lab1,data_type);
  miscframe->addChildren("-side left -fill x -expand t",byte_swapped);

 
  this->Initialized=1;
  this->ResetHeader();
  this->DisplayHeader();
  return GetWidgetName();
}
 

int  vtkpxGUIAnalyzeHeaderEditor::HandleEvent(int event)
{
  if (event<=10)
      return this->EventManager->handleDirectEvent(event);

  switch(event)
      {
      case 100:
	ResetHeader();
	DisplayHeader();
	break;

      case 101:
	{
	  char* line=this->EventManager->getOpenFilename("Header File name","Header Files","*.hdr*");
	  
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		ReadHeader(line);
	    }
	}
	break;
	
      case 102:
	SaveHeader(header_filename);
	break;
	
      case 103:
	{
	  char* line=this->EventManager->getSaveFilename("Header File Name",header_filename,"Header Files","*.hdr");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		this->SaveHeader(line);
	    }
	}
	break;

      case 104:
	if (this->EventParent!=NULL && this->CallbackNo>0)
	  this->EventParent->HandleEvent(CallbackNo);
	
	if (this->CallbackName!=NULL)
	  PXTkApp::executeTclCommand(this->CallbackName);
	break;
      }
  
  return TCL_OK;
}

int vtkpxGUIAnalyzeHeaderEditor::ReadHeader(const char* name)
{
  header_bits=readanahdr(name,header);
  if (header_bits==0)
    {
      SetHeaderFromDialog();
    }
  else
    {
      delete [] header_filename;
      header_filename=new char[strlen(name)+1];
      strcpy(header_filename,name);
      DisplayHeader();
    }
  return TCL_OK;
}

int vtkpxGUIAnalyzeHeaderEditor::SaveHeader(const char* name)
{
  strcpy(pxtk_buffer,name);
  SetHeaderFromDialog();
  
  int ok=writeanahdr(pxtk_buffer,header,header_bits);
  
  if (ok)
    {
      if (strcmp(header_filename,pxtk_buffer)!=0)
	{
	  delete [] header_filename;
	  header_filename=new char[strlen(pxtk_buffer)+1];
	  strcpy(header_filename,pxtk_buffer);
	}
    }
  return DisplayHeader();

}

int vtkpxGUIAnalyzeHeaderEditor::SetHeaderFromDialog()
{
  header->xsize=Irange(atoi(entry[0]->getText()),1,1024);
  header->ysize=Irange(atoi(entry[1]->getText()),1,1024);
  header->zsize=Irange(atoi(entry[2]->getText()),1,1024);
  header->tsize=Irange(atoi(entry[3]->getText()),1,1024);

  header->xpix=Frange(atof(entry[4]->getText()),(float)0.01,(float)100.0);
  header->ypix=Frange(atof(entry[5]->getText()),(float)0.01,(float)100.0);
  header->zpix=Frange(atof(entry[6]->getText()),(float)0.01,(float)100.0);

  header->minpix=Irange(atoi(entry[7]->getText()),-65535,65535);
  header->maxpix=Irange(atoi(entry[8]->getText()),-65535,65535);
  
  header->orient=Irange(orientation->getIndex(),0,5);

  switch (data_type->getIndex())
    {
    case 0:
      header_bits=8;
      break;
    case 1:
      header_bits=16;
      break;
    case 2:
      header_bits=31;
      break;
    case 3:
      header_bits=32;
      break;
    }
  
  if (byte_swapped->getState())
    header_bits=-header_bits;
  
  return TCL_OK;
}

int vtkpxGUIAnalyzeHeaderEditor::ResetHeader()
{
  header->zsize=128;
  header->xsize=128;
  header->ysize=128;
  header->tsize=1;
  header->zasp=1.0; header->yasp=1.0;header->xasp=1.0;
  header->zpix=1.0; header->ypix=1.0;header->xpix=1.0;
  header->minpix=0;header->maxpix=65536;
  header->trueminpix=0; header->truemaxpix=65536;
  header->orient=0;
  header_bits=16;
  return TCL_OK;
}


int vtkpxGUIAnalyzeHeaderEditor::DisplayHeader()
{
  for (int i=0;i<=11;i++)
    {
      switch(i)
	{
	case 0:
	  sprintf(pxtk_buffer,"%d",header->xsize);
	  break;
	case 1:
	  sprintf(pxtk_buffer,"%d",header->ysize);
	  break;
	case 2:
	  sprintf(pxtk_buffer,"%d",header->zsize);
	  break;
	case 3:
	  sprintf(pxtk_buffer,"%d",header->tsize);
	  break;
	case 4:
	  sprintf(pxtk_buffer,"%7.5f",header->xpix);
	  break;
	case 5:
	  sprintf(pxtk_buffer,"%7.5f",header->ypix);
	  break;
	case 6:
	  sprintf(pxtk_buffer,"%7.5f",header->zpix);
	  break;
	case 7:
	  sprintf(pxtk_buffer,"%d",header->minpix);
	  break;
	case 8:
	  sprintf(pxtk_buffer,"%d",header->maxpix);
	  break;
	case 9:
	  {
	    int orn=header->orient;
	    if (orn<0 || orn>2)
	      orn=3;
	    orientation->setIndex(orn);
	  }
	  break;
	case 10:
	  {
	    int h=abs(header_bits);
	    switch (h)
	      {
	      case 8:
		data_type->setIndex(0);
		break;
	      case 16:
		data_type->setIndex(1);
		break;
	      case 31:
		data_type->setIndex(2);
		break;
	      case 32:
		data_type->setIndex(3);
		break;
	      }
	  }
	  break;
	case 11:
	  if (header_bits>0)
	    byte_swapped->setState(PXFalse);
	  else
	    byte_swapped->setState(PXTrue);
	  break;
	}
      if (i<9)
	entry[i]->setText(pxtk_buffer);
    }
  
  strcpy(pxtk_buffer,header_filename);
  sprintf(pxtk_buffer2,"Analyze Header Editor: %s",PXTkApp::getFilenameTail(pxtk_buffer));
  this->EventManager->setTitle(pxtk_buffer2);
  return TCL_OK;
}

int vtkpxGUIAnalyzeHeaderEditor::SetHeader(vtkImageData* image,int orient)
{
  return SetHeader(image,orient,-1);
}


int vtkpxGUIAnalyzeHeaderEditor::SetHeader(vtkImageData* image,int orient,int frames)
{
  if (image==NULL)
    return TCL_ERROR;

  int Dim[3];
  image->GetDimensions(Dim);

  header->xsize=Dim[0];
  header->ysize=Dim[1];

  if (frames != -1 ) {
    header->tsize=frames;
    header->zsize=Dim[2]/header->tsize;
  } else {
    header->zsize=Dim[2];
    header->tsize=image->GetNumberOfScalarComponents();
  }

  double Sp[3];
  image->GetSpacing(Sp);

  header->xpix=Sp[0];
  header->ypix=Sp[1];
  header->zpix=Sp[2];

  double range[2];
  header->orient=Irange(orient,0,5);

  if (image->GetPointData()->GetScalars()!=NULL)
    {
      image->GetPointData()->GetScalars()->GetRange(range);
      header->minpix=(int)range[0];
      header->maxpix=(int)range[1];

      double r[2];
      image->GetPointData()->GetScalars()->GetDataTypeRange(r);
      float diff=r[1]-r[0];
      if (diff<256)
	header_bits=8;
      else if (diff<65536)
	header_bits=16;
      else if (image->GetPointData()->GetScalars()->GetDataType()==VTK_FLOAT)
	header_bits=32;
      else
	header_bits=31;
    }
  DisplayHeader();
  return TCL_OK;
}

int vtkpxGUIAnalyzeHeaderEditor::SetImageName(const char* headername)
{
  if (headername==NULL)
    return TCL_ERROR;

  delete [] header_filename;
  header_filename=new char[strlen(headername)+1];
  strcpy(header_filename,headername);
  strcpy(pxtk_buffer,header_filename);
  sprintf(pxtk_buffer2,"Analyze Header Editor: %s",PXTkApp::getFilenameTail(pxtk_buffer));
  this->EventManager->setTitle(pxtk_buffer2);
  return TCL_OK;
}

int vtkpxGUIAnalyzeHeaderEditor::GetOrientation()
{
  header->orient=Irange(orientation->getIndex(),0,5);
  return header->orient;
}

int vtkpxGUIAnalyzeHeaderEditor::SetImageSpacingToImage(vtkImageData* img)
{
  img->SetSpacing(header->xpix,header->ypix,header->zpix);
  return 1;
}

