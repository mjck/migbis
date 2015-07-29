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
  Module:    $RCSfile: vtkpxGUIAnalyzeHeaderEditor.h,v $
  Language:  C++
  Date:      $Date: 2002/06/07 20:34:50 $
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
// .NAME vtkbisGUINIFTIHeaderEditor - create hierarchies of actors
// .SECTION Description
#ifndef __vtkbisGUINIFTIHeaderEditor_h
#define __vtkbisGUINIFTIHeaderEditor_h

#include "vtkpxGUIComponent.h"

#include "pxtkapp.h"
#include "pxtkdialog.h"
#include "pxtkgadgets.h"
#include "pxtkcomplexwidgets.h"
#include "pxtkmenu.h"
#include "pxtkradiobutton.h"
#include "vtkbisImageHeader.h"

//BTX
class NRTkIWTabNotebook;
//ETX

class vtkbisGUINIFTIHeaderEditor : public vtkpxGUIComponent
{
public:
  static vtkbisGUINIFTIHeaderEditor *New();
  vtkTypeMacro(vtkbisGUINIFTIHeaderEditor,vtkpxGUIComponent);
  
  virtual char* Initialize(const char* name,int inside);
  virtual int   HandleEvent(int );
  
  virtual int   ReadHeader(char* name);
  virtual int   SaveHeader(char* name);
  virtual int   ResetHeader();


  virtual int SetHeader(vtkbisImageHeader *header);
  vtkGetObjectMacro(Header,vtkbisImageHeader);

protected:

  vtkbisGUINIFTIHeaderEditor();
  virtual  ~vtkbisGUINIFTIHeaderEditor();

  vtkbisImageHeader* Header;
  vtkbisImageHeader* OriginalHeader;

  // Parameter Stuff etc.
  PXTkLabel* dim_label[9];
  PXTkEntry* dim_entry[9];

  PXTkLabel* misc_sclabel[6];
  PXTkEntry* misc_scentry[6];

  PXTkLabel* misc_txlabel[6];
  PXTkEntry* misc_txentry[6];

  PXTkOptionMenu* orientationMenu;
  PXTkLabel*      orientationDescription;

  PXTkOptionMenu* qformMenu;
  PXTkLabel* qformDescription;

  PXTkOptionMenu* sformMenu;
  PXTkText*       sformMatrix;
  

  PXTkMenu*  editMenu;
  PXTkMenu*  detailMenu;
  PXTkOptionMenu* datatypeMenu;
  PXTkText*  textBox;
  //BTX
  NRTkIWTabNotebook* notebook;
  //ETX


  // Description:
  // 
  virtual int   SetHeaderFromDialog();
  virtual int   DisplayHeader();
  virtual int   CreateGUI();


  // Description:
  // Modify Header Mode
  int ChangingHeaderMode;
  virtual int   SetHeaderMode(int md);

  // Description:
  // Show Header Detail
  int ChangingDetailMode;


  virtual int SetDetailMode(int dt,int switchto=1);
};

#endif

