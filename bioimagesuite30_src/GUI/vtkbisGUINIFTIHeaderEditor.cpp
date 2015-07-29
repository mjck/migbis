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
  Module:    $RCSfile: vtkbisNIFTIHeaderEditor.cpp,v $
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
#include "vtkbisGUINIFTIHeaderEditor.h"
#include "vtkbisImageHeader.h"
#include "vtkObjectFactory.h"
#include "pxtkapp.h"
#include "vtkImageData.h"
#include "nrtk_iwidget_tabnotebook.h"
#include "vtkbisNIFTIExtensionsList.h"
#include "vtkbisNIFTIExtension.h"

vtkbisGUINIFTIHeaderEditor* vtkbisGUINIFTIHeaderEditor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisGUINIFTIHeaderEditor");
  if(ret)
    {
      return (vtkbisGUINIFTIHeaderEditor*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisGUINIFTIHeaderEditor;
}

// Construct object with no children.
vtkbisGUINIFTIHeaderEditor::vtkbisGUINIFTIHeaderEditor()
{
  this->Header=vtkbisImageHeader::New();
  this->OriginalHeader=vtkbisImageHeader::New();
  this->ChangingHeaderMode=0;
  this->ChangingDetailMode=0;

}

vtkbisGUINIFTIHeaderEditor::~vtkbisGUINIFTIHeaderEditor()
{
  this->Header->Delete();
  this->OriginalHeader->Delete();
}

// -----------------------------------------------------------------------------
char*  vtkbisGUINIFTIHeaderEditor::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
    return this->GetWidgetName();
  
  vtkpxGUIComponent::Initialize(name,inside);
  this->CreateGUI();
  this->ResetHeader();
  this->DisplayHeader();
  return GetWidgetName();
}

// -----------------------------------------------------------------------------
int  vtkbisGUINIFTIHeaderEditor::HandleEvent(int event)
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
	  char* line=this->EventManager->getOpenFilename("Header File name","Header Files","{*.hdr*,*.nii*}");
	  
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		this->ReadHeader(line);
	    }
	}
	break;
	
      case 102:
	if (this->Header->GetMode()!=2 )
	  {
	    char* line=this->EventManager->getSaveFilename("Header File Name",this->Header->GetFileName(),"Header Files","*.hdr");
	    if (line!=NULL)
	      {
		if (strlen(line)>0)
		  {
		    this->SaveHeader(line);
		  }
	      }
	  }
	else
	  {
	    this->EventManager->messageBox("Cannot save NIFTI .nii header independent of the image!","For your information ...");
	  }
	break;


      case 103:
	this->Header->CopyHeader(this->OriginalHeader);
	this->DisplayHeader();
	break;

      case 104:
	this->SetHeaderFromDialog();
	if (this->EventParent!=NULL && this->CallbackNo>0)
	  this->EventParent->HandleEvent(CallbackNo);
	
	if (this->CallbackName!=NULL)
	  PXTkApp::executeTclCommand(this->CallbackName);
	this->DisplayHeader();
	break;

	
      case 110:
      case 111:
      case 112:
	this->SetHeaderMode(event-110);
	break;

      case 120:
	this->SetDetailMode(-1,1);
	break;
      }
	  
  
  return TCL_OK;
}
// -----------------------------------------------------------------------------
int vtkbisGUINIFTIHeaderEditor::ReadHeader(char* name)
{
  vtkbisImageHeader* header=vtkbisImageHeader::New();
  int ok=header->ReadHeader(name);

  if (ok)
    {
      this->SetHeader(header);
    }
  else
    {
      this->EventManager->messageBox("Failed to Load Header!","Error Message ...");
    }

  header->Delete();
  return TCL_OK;
}

int vtkbisGUINIFTIHeaderEditor::SaveHeader(char* name)
{
  strcpy(pxtk_buffer,name);
  this->SetHeaderFromDialog();
  
  //  fprintf(stderr,"Mode = %d , Filename = %s\n",this->Header->GetMode(),pxtk_buffer);

  int ok=this->Header->WriteHeader(pxtk_buffer);
  if (!ok)
    {
      this->EventManager->messageBox("Unfortunately, failed to save header!","Error Message ...");
    }
  else
    {
      this->DisplayHeader();
    }


  return TCL_OK;
}


int vtkbisGUINIFTIHeaderEditor::ResetHeader()
{
  this->Header->ResetFields();
  this->Header->SetModeToNIFTI_HDRIMG();
  this->Header->SetIsHeaderValid(1);
  this->DisplayHeader();
  return TCL_OK;
}

int vtkbisGUINIFTIHeaderEditor::SetHeader(vtkbisImageHeader* hdr)
{
  if (hdr==NULL)
    return 0;

  this->OriginalHeader->CopyHeader(hdr);
  this->Header->CopyHeader(hdr);
  this->DisplayHeader();
  return 1;
}
// -------------------------------------------------------------------------------------------
//
//  The heart of the functionality is below .....
//
// -------------------------------------------------------------------------------------------
int vtkbisGUINIFTIHeaderEditor::CreateGUI()
{
  PXTkFrame* mainFrame=(PXTkFrame*)(this->EventManager->getMainWindow());
  mainFrame->configure("-bg","black");

  PXTkFrame* menubar=new PXTkFrame(this->EventManager,mainFrame);
  mainFrame->addChildren("-side top -expand false -fill x",menubar);

  PXTkLabel* lab=new PXTkLabel(this->EventManager,menubar,"");
  lab->configure("-image","$::pxtclvtkpxcontrib::smalllogo ");
  menubar->addChildren("-side right -expand false  -padx 5 -pady 5",lab);

  PXTkMenuButton* but1=new PXTkMenuButton(this->EventManager,menubar,"File",-1,PXTrue);
  PXTkMenuButton* but2=new PXTkMenuButton(this->EventManager,menubar,"Edit",-1,PXTrue);
  PXTkMenuButton* but3=new PXTkMenuButton(this->EventManager,menubar,"View",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but1,but2,but3);
  
  PXTkMenu *menu1=new PXTkMenu(this->EventManager,but1,PXFalse);
  menu1->addButton("New Header",100);
  menu1->addSeparator();
  menu1->addButton("Load",101);
  menu1->addButton("Save As",102);
  menu1->addSeparator();
  menu1->addButton("Close",3);
  
  editMenu=new PXTkMenu(this->EventManager,but2,PXFalse);
  editMenu->addRadioButton("Analyze 7,5",0,0,PXTrue,110);
  editMenu->addRadioButton("NIFTI-1 (.hdr)",0,1,PXFalse,111);
  editMenu->addRadioButton("NIFTI-1 (.nii)",0,2,PXFalse,112);

  detailMenu=new PXTkMenu(this->EventManager,but3,PXFalse);
  detailMenu->addRadioButton("Short Header",0,0,PXFalse,120);
  detailMenu->addRadioButton("Long Header",0,1,PXTrue,120);
  detailMenu->addRadioButton("Complete Header",0,2,PXFalse,120);
  detailMenu->addRadioButton("Extensions Only",0,3,PXFalse,120);
  
  int width=600;
  int height=300;

  this->notebook = new NRTkIWTabNotebook(this->EventManager, mainFrame, "n",width,height);
  PXTkFrame* dim_frame=this->notebook->addPage("Dimensions");
  PXTkFrame* or_frame=this->notebook->addPage("Orientation");
  PXTkFrame* mis_frame=this->notebook->addPage("Misc");
  PXTkFrame* det_frame=this->notebook->addPage("Detail_View");

  PXTkFrame* bot_frame=new PXTkFrame(this->EventManager,mainFrame);
  mainFrame->addChildren("-side bottom -expand true -fill x",bot_frame);
  mainFrame->addChildren("-side top -expand true -fill both",this->notebook->getMainWindow());

  PXTkButton* b0=new PXTkButton(this->EventManager,bot_frame,"Apply Changes to Viewer",104); 
  PXTkButton* b1=new PXTkButton(this->EventManager,bot_frame,"Revert to Original",103); 
  bot_frame->addChildren("-side left -expand true -fill x -padx 5",b0,b1);
  


  // Step 1 Dimensions Frame
  // -----------------------
  PXTkLabelFrame* dimensionframe=new PXTkLabelFrame(this->EventManager,dim_frame,"Image Dimensions (voxels)");
  PXTkLabelFrame* voxelframe=new PXTkLabelFrame(this->EventManager,dim_frame,"Voxel Dimensions (mm or s)");
  PXTkLabelFrame* miscframe=new PXTkLabelFrame(this->EventManager,dim_frame,"Image Type");

  dim_frame->addChildren("-side top -fill both -expand t",dimensionframe,voxelframe,miscframe);
  
  for (int i=0;i<=3;i++)
    {
      dim_label[i]=new PXTkLabel(this->EventManager,dimensionframe,"X-Size");
      dim_label[i]->configure("-width",5);
      dim_entry[i]=new PXTkEntry(this->EventManager,dimensionframe);
      dim_entry[i]->configure("-width",8);
      dim_entry[i]->configure("-relief","sunken");
      switch(i) 
	{
	case 0:
	  dim_label[i]->setText("x:");
	  break;
	case 1:
	  dim_label[i]->setText("y:");
	  break;
	case 2:
	  dim_label[i]->setText("z:");
	  break;
	case 3:
	  dim_label[i]->setText("t:");
	  break;
	}
      dimensionframe->addChildren("-side left -padx 2 -fill x -expand t",dim_label[i],dim_entry[i]);
    }
  
  // Then Voxel Frame 
  for (int i=4;i<=7;i++)
    {
      dim_label[i]=new PXTkLabel(this->EventManager,voxelframe,"X-Size");
      dim_label[i]->configure("-width",5);
      dim_entry[i]=new PXTkEntry(this->EventManager,voxelframe);
      dim_entry[i]->configure("-width",5);
      dim_entry[i]->configure("-relief","sunken");
      switch(i) 
	{
	case 4:
	  dim_label[i]->setText("x:");
	  break;
	case 5:
	  dim_label[i]->setText("y:");
	  break;
	case 6:
	  dim_label[i]->setText("z:");
	  break;
	case 7:
	  dim_label[i]->setText("t:");
	  break;
	}
      voxelframe->addChildren("-side left -padx 2 -fill x -expand t",dim_label[i],dim_entry[i]);
    }


  PXTkLabel* lab1=new PXTkLabel(this->EventManager,miscframe,"Data Type:");
  datatypeMenu=new PXTkOptionMenu(this->EventManager,miscframe,"Unsigned Char",-1);
  datatypeMenu->addOption("Char",-1);
  datatypeMenu->addOption("Unsigned Short",-1);
  datatypeMenu->addOption("Short",-1);
  datatypeMenu->addOption("Usigned Int",-1);
  datatypeMenu->addOption("Int",-1);
  datatypeMenu->addOption("Float",-1);
  datatypeMenu->addOption("Double",-1);
  datatypeMenu->setIndex(3);

  dim_label[8]=new PXTkLabel(this->EventManager,miscframe,"Bits/pixel:");
  dim_label[8]->configure("-width",5);
  dim_entry[8]=new PXTkEntry(this->EventManager,miscframe);
  dim_entry[8]->configure("-width",5);
  dim_entry[8]->configure("-relief","sunken");
  dim_entry[8]->enable(PXFalse);
  miscframe->addChildren("-side left -fill x -expand t",lab1,datatypeMenu,dim_label[8],dim_entry[8]);

  // Step 2 Misc_frame
  //PXTkFrame* mis_frame=this->notebook->addPage("Misc");
  PXTkLabelFrame* mis_frame1=new PXTkLabelFrame(this->EventManager,mis_frame,"Scaling");
  //  PXTkLabelFrame* mis_frame2=new PXTkLabelFrame(this->EventManager,mis_frame,"Offsets/Units (mm or s)");
  PXTkLabelFrame* mis_frame3=new PXTkLabelFrame(this->EventManager,mis_frame,"Text Labels");
  //  PXTkLabelFrame* mis_frame4=new PXTkLabelFrame(this->EventManager,mis_frame,"Intents");
  mis_frame->addChildren("-side top -pady 2 -fill x -expand t",mis_frame1,mis_frame3);//,mis_frame2,mis_frame4);

  PXTkFrame* m_frame1=new PXTkFrame(this->EventManager,mis_frame1);
  PXTkFrame* m_frame2=new PXTkFrame(this->EventManager,mis_frame1);
  mis_frame1->addChildren("-side top -pady 2 -fill x -expand t",m_frame1,m_frame2);
  
  misc_sclabel[0]=new PXTkLabel(this->EventManager,m_frame1,"Int_scale (scl_slope)");
  misc_sclabel[1]=new PXTkLabel(this->EventManager,m_frame1,"Int_shift (scl_offset)");
  for (int i=0;i<=1;i++)
    {
      misc_scentry[i]=new PXTkEntry(this->EventManager,m_frame1);
      misc_scentry[i]->configure("-relief","sunken");
    }
  m_frame1->addChildren("-side left -padx 2 -fill x -expand true",
			misc_sclabel[0],misc_scentry[0],
			misc_sclabel[1],misc_scentry[1]);
  

  for (int i=2;i<=5;i++)
    {
      misc_sclabel[i]=new PXTkLabel(this->EventManager,m_frame2,"T");
      misc_scentry[i]=new PXTkEntry(this->EventManager,m_frame2);
      misc_scentry[i]->configure("-relief","sunken");
      misc_scentry[i]->configure("-width",4);
      
      switch(i) 
	{
	case 2:
	  misc_sclabel[i]->setText("cal_min:");
	  break;
	case 3:
	  misc_sclabel[i]->setText("cal_max");
	  break;
	case 4:
	  misc_sclabel[i]->setText("gl_min:");
	  break;
	case 5:
	  misc_sclabel[i]->setText("gl_max");
	  break;
	}
      m_frame2->addChildren("-side left -padx 0 -fill x -expand t",misc_sclabel[i],misc_scentry[i]);
    }
  

  for (int i=0;i<=2;i++)
    {
      misc_txlabel[i]=new PXTkLabel(this->EventManager,mis_frame3,"T");
      misc_txentry[i]=new PXTkEntry(this->EventManager,mis_frame3);
      misc_txentry[i]->configure("-relief","sunken");
      switch(i) 
	{
	case 0:
	  misc_txlabel[i]->setText("DB_Name:");
	  misc_txentry[i]->configure("-width",14);
	  break;
	case 1:
	  misc_txlabel[i]->setText("Extent (VOI Value):");
	  misc_txentry[i]->configure("-width",3);
	  break;
	case 2:
	  misc_txlabel[i]->setText("Intent_Name:");
	  misc_txentry[i]->configure("-width",10);
	  break;
	  
	}
      mis_frame3->addChildren("-side left -padx 0 -fill x -expand t",misc_txlabel[i],misc_txentry[i]);
    }
  
  // Step 2.5 -- Orientation Frame
  // -----------------------------
  //  PXTkFrame* or_frame=this->notebook->addPage("Orientation");
  PXTkFrame* or_frame1=new PXTkFrame(this->EventManager,or_frame);
  PXTkLabelFrame* or_frame2=new PXTkLabelFrame(this->EventManager,or_frame,"Quaternion (qform_code)");
  PXTkLabelFrame* or_frame3=new PXTkLabelFrame(this->EventManager,or_frame,"Affine Transform (sform_code)");
  or_frame->addChildren("-side top -pady 0 -fill x -expand t",or_frame1,or_frame2,or_frame3);  

  PXTkLabel* orlabel=new PXTkLabel(this->EventManager,or_frame1,"Orientation:");
  this->orientationMenu=new PXTkOptionMenu(this->EventManager,or_frame1,"Axial",-1);
  this->orientationMenu->addOption("Coronal",-1);
  this->orientationMenu->addOption("Sagittal",-1);
  this->orientationMenu->addOption("Polar/Other",-1);

  orientationDescription=new PXTkLabel(this->EventManager,or_frame1," ");
  orientationDescription->configure("-width",10);
  or_frame1->addChildren("-side left -padx 2 -fill x -expand t",orlabel,this->orientationMenu,
			 this->orientationDescription);

  PXTkFrame* q_frame1=new PXTkFrame(this->EventManager,or_frame2);   
  PXTkFrame* q_frame2=new PXTkFrame(this->EventManager,or_frame2);   
  or_frame2->addChildren("-side top -pady 2 -fill both -expand true",q_frame1,q_frame2);

  PXTkLabel* q_label=new PXTkLabel(this->EventManager,q_frame1,"Qform_code : ");
  this->qformMenu=new PXTkOptionMenu(this->EventManager,q_frame1,"Unknown/Not used",-1);
  this->qformMenu->addOption("Scanner Anatomical");
  this->qformMenu->addOption("Aligned Anatomical");
  this->qformMenu->addOption("Talairach");
  this->qformMenu->addOption("MNI 152");
  this->qformDescription=new PXTkLabel(this->EventManager,q_frame2," ");
  this->qformDescription->configure("-width",20);

  q_frame1->addChildren("-side left -padx 2 -fill x -expand t",q_label, this->qformMenu);
  q_frame2->addChildren("-side left -padx 2 -fill x -expand t",this->qformDescription);

  PXTkFrame* s_frame1=new PXTkFrame(this->EventManager,or_frame3);   
  PXTkFrame* s_frame2=new PXTkFrame(this->EventManager,or_frame3);   
  or_frame3->addChildren("-side top -pady 2 -fill x -expand f",s_frame1);
  or_frame3->addChildren("-side top -pady 2 -fill both -expand true",s_frame2);
  
  PXTkLabel* s_label=new PXTkLabel(this->EventManager,s_frame1,"Sform_code : ");
  this->sformMenu=new PXTkOptionMenu(this->EventManager,s_frame1,"Unknown/Not used",-1);
  this->sformMenu->addOption("Scanner Anatomical");
  this->sformMenu->addOption("Aligned Anatomical");
  this->sformMenu->addOption("Talairach");
  this->sformMenu->addOption("MNI 152");
  s_frame1->addChildren("-side left -padx 2 -fill x -expand t",s_label, this->sformMenu);
  
   
  PXTkLabel* s_label2=new PXTkLabel(this->EventManager,s_frame2,"Matrix : ");
  this->sformMatrix=new PXTkText(this->EventManager,s_frame2);
  this->sformMatrix->configure("-width",60);
  this->sformMatrix->configure("-height",3);
  this->sformMatrix->configure("-wrap","none");
  this->sformMatrix->configure("-relief","ridge");
  this->sformMatrix->setEditableState(PXFalse);
  s_frame2->addChildren("-side left -expand fals",s_label2);
  s_frame2->addChildren("-side left -expand true -fill both",this->sformMatrix);


  // Step 3 Extensions Frame
  // ------------------------
  //PXTkFrame* det_frame=this->notebook->addPage("Detail");
  det_frame->configure("-bg","red");
  PXTkScrollbar* xbar=new PXTkScrollbar(this->EventManager,det_frame,PXFalse);
  det_frame->addChildren("-side bottom -expand false -fill x",xbar);

  PXTkFrame* top_frame=new PXTkFrame(this->EventManager,det_frame);
  det_frame->addChildren("-side top -expand true -fill both",top_frame);

  PXTkScrollbar* ybar=new PXTkScrollbar(this->EventManager,top_frame,PXTrue);
  
  this->textBox=new PXTkText(this->EventManager,top_frame);
  ybar->attachGadget(this->textBox);
  xbar->attachGadget(this->textBox);

  this->textBox->configure("-width",80);
  this->textBox->configure("-height",15);
  this->textBox->configure("-wrap","none");
  this->textBox->configure("-relief","ridge");
  this->textBox->setEditableState(PXFalse);
  top_frame->addChildren("-side right -expand false -fill y",ybar);
  top_frame->addChildren("-side left -expand true -fill both",this->textBox);

  // Done
  this->notebook->viewPage("Dimensions");
  this->Initialized=1;
  return 1;
}

// -------------------------------------------------------------------------------------------
int vtkbisGUINIFTIHeaderEditor::SetDetailMode(int dt,int switchto)
{
  if (this->Initialized==0)
    return 0;

  if (this->ChangingDetailMode==1)
    {
      //      fprintf(stderr,"Already changing detail_mode\n");
      return 0;
    }

  this->ChangingDetailMode=1;

  if (dt<0 || dt > 3 )
    dt=this->detailMenu->getMenuWidgetVariable(0);

  this->textBox->setEditableState(PXTrue);
  this->textBox->clearText();

  nifti_1_header *hdr=this->Header->GetRawHeader();
  char line[1000];
  int long_mode=dt;
  
  if (dt != 3 )
    {
      this->textBox->addText("File Name = ");
      this->textBox->addText(this->Header->GetFileName());


      this->textBox->addText("\n\nOrientation =");

      int oldstyle=vtkbisImageHeader::MapOrientationToOldStyle(this->Header->GetOrientation());


      switch (oldstyle)
	{
	case 0: this->textBox->addText(" Axial "); break;
	case 1: this->textBox->addText(" Coronal "); break;
	case 2: this->textBox->addText(" Sagittal "); break;
	case 3: this->textBox->addText(" Polar "); break;
	}
      
      mat44 original=vtkbisImageHeader::NIFTI_CreateOrientationMatrix(hdr);
      int icod,jcod,kcod;  nifti_mat44_to_orientation(original,&icod,&jcod,&kcod);
      if (long_mode)
	{
	  sprintf(line,"(%s,%s,%s)", nifti_orientation_string(icod),
		  nifti_orientation_string(jcod),
		  nifti_orientation_string(kcod));
	  this->textBox->addText(line);
	}
      else
	{
	  sprintf(line,"(%s,%s,%s)", vtkbisImageHeader::NIFTI_GetOrientationString(icod),
		  vtkbisImageHeader::NIFTI_GetOrientationString(jcod),
		  vtkbisImageHeader::NIFTI_GetOrientationString(kcod));
	  this->textBox->addText(line);
	  
	}
      
      
      if (long_mode)
	{
	  this->textBox->addText("\n\nMode = ");
	  switch (this->Header->GetMode())
	    {
	    case 0: this->textBox->addText(" Analyze 7.5"); break;
	    case 1: this->textBox->addText(" NIFTI (.hdr/.img)"); break;
	    case 2: this->textBox->addText(" NIFTI (.nii)"); break;
	    }
	  sprintf(line,"(magic=%s) (Valid = %d SwapBytes=%d)\n",hdr->magic,this->GetHeader()->GetIsHeaderValid(),this->Header->GetSwapBytes());
	  this->textBox->addText(line);
	  this->textBox->addText("\nDimensions =");
	}
      
      sprintf(line," %dx%dx%d, %d ",hdr->dim[1],hdr->dim[2],hdr->dim[3],hdr->dim[4]);
      this->textBox->addText(line);
      if (long_mode)
	{
	  sprintf(line,"\n\nVoxel Dimensions = %.3fx%.3fx%.3f, %.2f \n",hdr->pixdim[1],hdr->pixdim[2],hdr->pixdim[3],hdr->pixdim[4]);
	  this->textBox->addText(line);
	  sprintf(line,"\nData Type = %s(%d) (Bits per Pixel=%d) Vox_Offset=%d Num Extensions=%d (Size =%d)\n",nifti_datatype_string(hdr->datatype),hdr->datatype,hdr->bitpix,
		  (int)hdr->vox_offset,this->Header->GetExtensions()->GetNumberOfExtensions(),this->Header->GetExtensions()->GetTotalByteSize());
	  this->textBox->addText(line);
	  
	  sprintf(line,"\nScale= %.3f Shift=%.3f \n",hdr->scl_slope,hdr->scl_inter);
	  this->textBox->addText(line);
	  sprintf(line,"\nOrientation Matrix Stuff (qform_code=%d sform_code=%d)\n",hdr->qform_code,hdr->sform_code);
	  this->textBox->addText(line);
	  if (hdr->qform_code>0)
	    {
	      sprintf(line,"\nQuatenion (qfac=%f) quat=(%.3f,%.3f,%.3f) offset=(%.3f,%.3f,%.3f)\n",
		      hdr->pixdim[0],
		      hdr->quatern_b,hdr->quatern_c,hdr->quatern_d,
		      hdr->qoffset_x,hdr->qoffset_y,hdr->qoffset_z);
	      this->textBox->addText(line);
	    }
	  
	  if (hdr->sform_code>0)
	    {
	      sprintf(line,"\nMatrix_x = \t( %+8.3f %+8.3f %+8.3f %+8.3f )\n",
		      hdr->srow_x[0],hdr->srow_x[1],hdr->srow_x[2],hdr->srow_x[3]);
	      this->textBox->addText(line);
	      sprintf(line,"\t       y = \t( %+8.3f %+8.3f %+8.3f %+8.3f )\n",
		      hdr->srow_y[0],hdr->srow_y[1],hdr->srow_y[2],hdr->srow_y[3]);
	      this->textBox->addText(line);
	      sprintf(line,"\t       z = \t( %+8.3f %+8.3f %+8.3f %+8.3f)\n",
		      hdr->srow_z[0],hdr->srow_z[1],hdr->srow_z[2],hdr->srow_z[3]);
	      this->textBox->addText(line);
	    }
	  
	  if (this->Header->GetMode()==0)
	    {
	      double ori[3]; this->Header->GetOrigin(ori);
	      sprintf(line,"\n(Analyze Origin) = (%.1f,%1.f,%.1f)\n",ori[0],ori[1],ori[2]);
	      this->textBox->addText(line);
	    }
	}
      else
	{
	  sprintf(line," (%.3fx%.3fx%.3f, %.2f)\n",hdr->pixdim[1],hdr->pixdim[2],hdr->pixdim[3],hdr->pixdim[4]);
	  this->textBox->addText(line);
	  sprintf(line,"\nData Type = %s (Bits=%d) NumExt=%d (Sz =%d)",nifti_datatype_string(hdr->datatype),hdr->bitpix,
		  this->Header->GetExtensions()->GetNumberOfExtensions(),this->Header->GetExtensions()->GetTotalByteSize());
	  this->textBox->addText(line);
	}
      
      
      if (long_mode>0)
	{
	  sprintf(line,"\nDBName : %s , VOIValue= %d\n",this->Header->GetDBName(),this->Header->GetVOIValue());
	  this->textBox->addText(line);
	  
	}
    }

  if (long_mode>1)
    {
      int next=this->Header->GetExtensions()->GetNumberOfExtensions(); 
      sprintf(line,"\n\n\t Extensions = %d , size=%d\n",
	      next,this->Header->GetExtensions()->GetTotalByteSize());
      this->textBox->addText(line);
      for (int i=0;i<next;i++)
	{
	  vtkbisNIFTIExtension* extn=this->Header->GetExtensions()->GetRawExtension(i);
	  sprintf(line,"*********** Extensions %d (code=%d size=%d)\n",
		  i+1,extn->GetECode(),extn->GetESize());
	  this->textBox->addText(line);
	  this->textBox->addText(extn->GetText());
	  this->textBox->addText("\n");
	}
    }
  

  if (switchto)
    this->notebook->viewPage("Detail_View");
  PXTkApp::executeTclCommand("update idletasks");
  this->ChangingDetailMode=0;
  this->textBox->setEditableState(PXFalse);
  return 1;
  //  fprintf(stderr,"Done changing detail_mode\n");
}

int vtkbisGUINIFTIHeaderEditor::SetHeaderMode(int md)
{
  if (this->ChangingHeaderMode==1)
    return 0;

  this->ChangingHeaderMode=1;

  if (md<0) md=0;
  if (md>2) md=2;

  int mode=this->Header->GetMode();
  if (mode==md)
    return 1;

  int ok=0;

  if (md!=mode)
    {
      if (md==0 && mode==1)
	ok=this->EventManager->questionBox("Changing the header mode from NIFTI to Analyze may result in loss of information. Are you sure?");
      else if (md==1 && mode==0)
	{
	  this->EventManager->messageBox("Changing Header from .hdr (analyze) to .hdr (NIFTI)");
	  ok=1;
	}
      else if (md==2)
	this->EventManager->messageBox("Cannot change a header from .hdr to .nii");
      else if (mode==2)
	this->EventManager->messageBox("Cannot change a header from .nii to .hdr");
    }

  if (ok!=0)
    mode=md;

  if (this->Initialized)
    this->editMenu->setMenuWidgetVariable(0,mode);

  if (ok==0)
    return 0;

  //  fprintf(stderr,"Setting Mode to %d\n",mode);

  if (mode==0)
    this->Header->SetModeToAnalyze();
  else if (mode==1)
    this->Header->SetModeToNIFTI_HDRIMG();
  else if (mode==2)
    fprintf(stderr,"Error, we should never get here!\n");

  this->ChangingHeaderMode=0;  
  return 1;
}
// -------------------------------------------------------------------------------------------
int vtkbisGUINIFTIHeaderEditor::DisplayHeader()
{
  if (this->Initialized==0)
    return 0;

  int md=this->Header->GetMode();
  if (md<0) md=0;
  if (md>2) md=2;


  strcpy(pxtk_buffer,this->Header->GetFileName());
  switch (md)
    {
    case 0:
      sprintf(pxtk_buffer2,"Header Editor: %s (ANA) ",PXTkApp::getFilenameTail(pxtk_buffer));
      break;
    case 1:
      sprintf(pxtk_buffer2,"Header Editor: %s (NIFTI .hdr) ",PXTkApp::getFilenameTail(pxtk_buffer));
      break;
    case 2:
      sprintf(pxtk_buffer2,"Header Editor: %s (NIFTI .nii) ",PXTkApp::getFilenameTail(pxtk_buffer));
      break;
    }

  this->EventManager->setTitle(pxtk_buffer2);

  
  // ----------------------------------------------------------------------
  // First Dimensions Part
  // ----------------------------------------------------------------------

  for (int i=0;i<9;i++)
    {
      switch(i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	  sprintf(pxtk_buffer,"%d",this->Header->GetRawHeader()->dim[i+1]);
	  break;
	  break;
	case 4:
	case 5:
	case 6:
	case 7:
	  sprintf(pxtk_buffer,"%7.5f",this->Header->GetRawHeader()->pixdim[i-3]);
	  break;
	case 8:
	  sprintf(pxtk_buffer,"%d",this->Header->GetRawHeader()->bitpix);
	  break;
	}
      dim_entry[i]->setText(pxtk_buffer);
    }
  int ind=this->Header->GetIndexedDataType();
  //  fprintf(stderr,"Index=%d\n",ind);
  datatypeMenu->setIndex(ind,PXFalse);

  // ----------------------------------------------------------------------------
  // Misc
  // ----------------------------------------------------------------------------
  sprintf(pxtk_buffer,"%f",this->Header->GetRawHeader()->scl_slope);
  misc_scentry[0]->setText(pxtk_buffer);

  sprintf(pxtk_buffer,"%f",this->Header->GetRawHeader()->scl_inter);
  misc_scentry[1]->setText(pxtk_buffer);
  
  sprintf(pxtk_buffer,"%f",this->Header->GetRawHeader()->cal_min);
  misc_scentry[2]->setText(pxtk_buffer);
  
  sprintf(pxtk_buffer,"%f",this->Header->GetRawHeader()->cal_max);
  misc_scentry[3]->setText(pxtk_buffer);
  
  sprintf(pxtk_buffer,"%d",this->Header->GetRawHeader()->glmin);
  misc_scentry[4]->setText(pxtk_buffer);
  
  sprintf(pxtk_buffer,"%d",this->Header->GetRawHeader()->glmax);
  misc_scentry[5]->setText(pxtk_buffer);

  misc_txentry[0]->setText(this->Header->GetRawHeader()->db_name);
  sprintf(pxtk_buffer,"%d",this->Header->GetRawHeader()->extents);
  misc_txentry[1]->setText(pxtk_buffer);
  misc_txentry[2]->setText(this->Header->GetRawHeader()->intent_name);
  // ----------------------------------------------------------------------------
  // Orientation
  // ----------------------------------------------------------------------------
  int oldstyle=vtkbisImageHeader::MapOrientationToOldStyle(this->Header->GetOrientation());
  this->orientationMenu->setIndex(oldstyle,PXFalse);
  mat44 original=vtkbisImageHeader::NIFTI_CreateOrientationMatrix(this->Header->GetRawHeader());
  int icod,jcod,kcod;  nifti_mat44_to_orientation(original,&icod,&jcod,&kcod);
  sprintf(pxtk_buffer,"(%s,%s,%s)", vtkbisImageHeader::NIFTI_GetOrientationString(icod),
	  vtkbisImageHeader::NIFTI_GetOrientationString(jcod),
	  vtkbisImageHeader::NIFTI_GetOrientationString(kcod));
  this->orientationDescription->setText(pxtk_buffer);


  nifti_1_header *hdr=this->Header->GetRawHeader();
  int qf=hdr->qform_code; if (qf<0 || qf>4) qf=0;
  this->qformMenu->setIndex(qf,PXFalse);
  int sf=hdr->sform_code; if (sf<0 || sf>4) sf=0;
  this->sformMenu->setIndex(sf,PXFalse);

  
  sprintf(pxtk_buffer,"Quatenion (qfac=%f) quat=(%.3f,%.3f,%.3f) offset=(%.3f,%.3f,%.3f)\n",
		      hdr->pixdim[0],
		      hdr->quatern_b,hdr->quatern_c,hdr->quatern_d,
		      hdr->qoffset_x,hdr->qoffset_y,hdr->qoffset_z);
  this->qformDescription->setText(pxtk_buffer);

  this->sformMatrix->clearText();
  this->sformMatrix->setEditableState(PXTrue);
  sprintf(pxtk_buffer,"x= \t( %+8.3f %+8.3f %+8.3f %+8.3f )\n",
	  hdr->srow_x[0],hdr->srow_x[1],hdr->srow_x[2],hdr->srow_x[3]);
  this->sformMatrix->addText(pxtk_buffer);
  sprintf(pxtk_buffer,"y = \t( %+8.3f %+8.3f %+8.3f %+8.3f )\n",
	  hdr->srow_y[0],hdr->srow_y[1],hdr->srow_y[2],hdr->srow_y[3]);
  this->sformMatrix->addText(pxtk_buffer);
  sprintf(pxtk_buffer,"z = \t( %+8.3f %+8.3f %+8.3f %+8.3f)",
	  hdr->srow_z[0],hdr->srow_z[1],hdr->srow_z[2],hdr->srow_z[3]);
  this->sformMatrix->addText(pxtk_buffer);
  this->sformMatrix->setEditableState(PXFalse);

    
  // Detail View
  // ----------------------------------------------------------------------------
  this->SetDetailMode(-1,0);
  // Last thing, fix the menu
  this->editMenu->setMenuWidgetVariable(0,md);
  return TCL_OK;
}
// -----------------------------------------------------------------------------
int vtkbisGUINIFTIHeaderEditor::SetHeaderFromDialog()
{
  
  if (this->Initialized==0)
    return 0;

  //  fprintf(stderr,"Setting Header from Dialog \n");

  nifti_1_header *hdr=this->Header->GetRawHeader();
  for (int i=1;i<=4;i++)
    hdr->dim[i]=Irange(atoi(dim_entry[i-1]->getText()),1,2048);
  this->Header->SetIndexedDataType(datatypeMenu->getIndex());

  // ----------------------------------------------------------------------------
  // Misc
  // ----------------------------------------------------------------------------
  hdr->scl_slope=atof(misc_scentry[0]->getText());
  hdr->scl_inter=atof(misc_scentry[1]->getText());
  hdr->cal_min=  atof(misc_scentry[2]->getText());
  hdr->cal_max=  atof(misc_scentry[3]->getText());
  hdr->glmin=  atoi(misc_scentry[4]->getText());
  hdr->glmax=  atoi(misc_scentry[5]->getText());

  strncpy(hdr->db_name,misc_txentry[0]->getText(),17);
  hdr->extents=atoi(misc_txentry[1]->getText());
  strncpy(hdr->intent_name,misc_txentry[2]->getText(),15);

  // ----------------------------------------------------------------------------
  // Orientation
  // ----------------------------------------------------------------------------
  this->Header->SetOrientation(this->orientationMenu->getIndex());
  
  int qf=this->qformMenu->getIndex();
  if (hdr->qform_code>0 && qf>0)
    hdr->qform_code=qf;

  int sf=this->sformMenu->getIndex();
  if (hdr->sform_code>0 && sf>0)
    hdr->sform_code=sf;

  double oldpixdim[3],newpixdim[3];
  for (int i=1;i<=3;i++)
    {
      oldpixdim[i-1]=Frange(fabs(hdr->pixdim[i]),0.00001,1000.0);
      newpixdim[i-1]=Frange(fabs(atof(dim_entry[i+3]->getText())),0.00001,1000.0);
    }
  hdr->pixdim[4]=atof(dim_entry[7]->getText());
  mat44 mapping; vtkbisImageHeader::NIFTI_identity_mat44(&mapping);
  for (int ia=0;ia<=2;ia++)
    mapping.m[ia][ia]=Frange(newpixdim[ia]/oldpixdim[ia],0.001,1000.0);

  /*  if (hdr->qform_code==0)
    {
      for (int i=0;i<=2;i++)
	hdr->pixdim[i+1]=mapping.m[i][i]*oldpixdim[i];
	}*/
  
  //  vtkbisImageHeader::NIFTI_print_mat44("mapping",mapping);
  this->Header->ApplyMapping(mapping);

  //  fprintf(stderr,"Setting Header from Dialog Done\n");
  return TCL_OK;
}
// -----------------------------------------------------------------------------

