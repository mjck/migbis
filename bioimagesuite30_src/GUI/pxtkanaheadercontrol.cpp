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

 see pxtkanaheader.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */
#include "pxtkanaheadercontrol.h"
#include "pxtkcomplexwidgets.h"

PXTkAnaHeaderControl::PXTkAnaHeaderControl(PXTkEventObj* evpar,PXTkGadget* par):PXTkDialog(evpar,par,
											     "Ana-Header Control",PXFalse)
{
  header=new anaimagedata;
  header_filename=new char[40];
  strcpy(header_filename,"new_header.hdr");
}

PXTkAnaHeaderControl::~PXTkAnaHeaderControl()
{
  delete header;
  delete [] header_filename;
}
 
int  PXTkAnaHeaderControl::initDisplay()
{
  int i=0;

  PXTkDialog::initDisplay();
  PXTkFrame* mainFrame=(PXTkFrame*)(main_window);

  PXTkFrame* menubar=new PXTkFrame(this,mainFrame);
  mainFrame->addChildren("-side top -expand false -fill x -pady 5",menubar);

  PXTkFrame* otherFrame=new PXTkFrame(this,mainFrame);
  mainFrame->addChildren("-side top -expand true -fill both -pady 5",otherFrame);
  mainFrame->setPackMode(PXFalse);

  PXTkMenuButton* but1=new PXTkMenuButton(this,menubar,"Header",-1,PXTrue);
  menubar->configure("-relief","ridge");
  menubar->addChildren("-side left -expand false ",but1);

  PXTkMenu *menu1=new PXTkMenu(this,but1,PXFalse);
  menu1->addButton("New",100);
  menu1->addSeparator();
  menu1->addButton("Load",101);
  menu1->addButton("Save",102);
  menu1->addButton("Save As",103);
  menu1->addSeparator();
  menu1->addButton("Close",3);

  // Dimensions xsize,ysize,zsize,tsize
  // Pixel Dims xpix,ypix,zpix,orient,MinValue,MaxValue,bits

  PXTkLabelFrame* dimensionframe=new PXTkLabelFrame(this,otherFrame,"Image Dimensions (voxels)");
  PXTkLabelFrame* voxelframe=new PXTkLabelFrame(this,otherFrame,"Voxel Dimensions (mm)");
  PXTkLabelFrame* intensityframe=new PXTkLabelFrame(this,otherFrame,"Intensity Range");
  PXTkLabelFrame* miscframe=new PXTkLabelFrame(this,otherFrame,"Image Type");

  otherFrame->addChildren("-side top -fill both -expand t",dimensionframe,voxelframe,intensityframe,miscframe);


  // First Dimension Frame 
  for (i=0;i<=3;i++)
    {
      label[i]=new PXTkLabel(this,dimensionframe,"X-Size");
      label[i]->configure("-width",5);
      entry[i]=new PXTkEntry(this,dimensionframe);
      entry[i]->configure("-width",5);
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
      label[i]=new PXTkLabel(this,voxelframe,"X-Size");
      label[i]->configure("-width",5);
      entry[i]=new PXTkEntry(this,voxelframe);
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
      label[i]=new PXTkLabel(this,intensityframe,"X-Size");
      label[i]->configure("-width",5);
      entry[i]=new PXTkEntry(this,intensityframe);
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

  PXTkLabel* lab0=new PXTkLabel(this,miscframe,"Orientation:");
  orientation=new PXTkOptionMenu(this,miscframe,"Axial (0)",-1);
  orientation->addOption("Coronal (1)",-1);
  orientation->addOption("Sagittal (2)",-1);
  orientation->addOption("Polar (3)",-1);
  
  PXTkLabel* lab1=new PXTkLabel(this,miscframe,"Data Type:");
  data_type=new PXTkOptionMenu(this,miscframe,"1-Byte (8)",-1);
  data_type->addOption("Short (2)",-1);
  data_type->addOption("Float (4)",-1);
  data_type->setIndex(1);

  byte_swapped=new PXTkCheckButton(this,miscframe,"Byte Swapped",-1);
  miscframe->addChildren("-side left -fill x -expand t",lab0,orientation,lab1,data_type);
  miscframe->addChildren("-side left -fill x -expand t",byte_swapped);

 
  is_display_ok=PXTrue;
  resetHeader();
  displayHeader();
  return TCL_OK;

}

int  PXTkAnaHeaderControl::handleEvent(int event)
{
  if (event<10)
      return PXTkDialog::handleEvent(event);

  switch(event)
      {
      case 100:
	resetHeader();
	displayHeader();
	break;

      case 101:
	{
	  char* line=getOpenFilename("Header File name","Header Files","*.hdr*");
	  
	  if (line!=NULL)
	      {
		if (strlen(line)>0)
		    readHeader(line);
	      }
	}
	break;
	
      case 102:
	saveHeader(header_filename);
	break;
	
      case 103:
	{
	  char* line=getSaveFilename("Header File Name",header_filename,"Header Files","*.hdr");
	  if (line!=NULL)
	      {
		if (strlen(line)>0)
		    saveHeader(line);
	      }
	}
	break;
      }

  return TCL_OK;
}

int PXTkAnaHeaderControl::readHeader(const char* name)
{
  header_bits=readanahdr(name,header);
  if (header_bits==0)
      {
	setHeaderFromDialog();
      }
  else
      {
	delete [] header_filename;
	header_filename=new char[strlen(name)+1];
	strcpy(header_filename,name);
	displayHeader();
      }
  return TCL_OK;
}

int PXTkAnaHeaderControl::saveHeader(const char* name)
{
  strcpy(pxtk_buffer,name);
  setHeaderFromDialog();
  
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
  return displayHeader();

}

int PXTkAnaHeaderControl::setHeaderFromDialog()
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
      header_bits=32;
      break;
    }
  
  if (byte_swapped->getState())
    header_bits=-header_bits;

  return TCL_OK;
}
int PXTkAnaHeaderControl::resetHeader()
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


int PXTkAnaHeaderControl::displayHeader()
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
	      sprintf(pxtk_buffer,"%5.2f",header->xpix);
	      break;
	    case 5:
	      sprintf(pxtk_buffer,"%5.2f",header->ypix);
	      break;
	    case 6:
	      sprintf(pxtk_buffer,"%5.2f",header->zpix);
	      break;
	    case 7:
	      sprintf(pxtk_buffer,"%d",header->minpix);
	      break;
	    case 8:
	      sprintf(pxtk_buffer,"%d",header->maxpix);
	      break;
	    case 9:
	      {
		int or_nt=header->orient;
		if (or_nt<0 || or_nt>2)
		  or_nt=3;
		orientation->setIndex(or_nt);
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
		  case 32:
		    data_type->setIndex(2);
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
  sprintf(pxtk_buffer2,"%s",PXTkApp::getFilenameTail(pxtk_buffer));
  setTitle(pxtk_buffer2);
  return TCL_OK;
}

