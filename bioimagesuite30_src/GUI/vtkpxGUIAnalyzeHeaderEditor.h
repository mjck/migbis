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
// .NAME vtkpxGUIAnalyzeHeaderEditor - create hierarchies of actors
// .SECTION Description
#ifndef __vtkpxGUIAnalyzeHeaderEditor_h
#define __vtkpxGUIAnalyzeHeaderEditor_h

#include "vtkpxGUIComponent.h"

#include "pxutil.h"
#include "pxtkapp.h"
#include "pxtkdialog.h"
#include "pxtkgadgets.h"
#include "pxtkcomplexwidgets.h"
#include "pxtkmenu.h"

#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkpxGUIAnalyzeHeaderEditor : public vtkpxGUIComponent
{
public:
  static vtkpxGUIAnalyzeHeaderEditor *New();
  vtkTypeMacro(vtkpxGUIAnalyzeHeaderEditor,vtkpxGUIComponent);
  
  virtual char* Initialize(const char* name,int inside);
  virtual int   HandleEvent(int );

  virtual int   ReadHeader(const char* name);
  virtual int   SaveHeader(const char* name);
  virtual int   ResetHeader();
  virtual int   SetHeaderFromDialog();
  virtual int   DisplayHeader();

  // Description: 
  // UseLabels
  vtkSetClampMacro(Mode, int,0,2);
  vtkGetMacro(Mode, int);

  virtual int SetHeader(vtkImageData* image,int orient,int frames);
  virtual int SetHeader(vtkImageData* image,int orient);
  virtual int SetImageName(const char* headername);

  virtual int  GetOrientation();
  virtual int  SetImageSpacingToImage(vtkImageData* img);

protected:

  vtkpxGUIAnalyzeHeaderEditor();
  virtual  ~vtkpxGUIAnalyzeHeaderEditor();

  int Mode;

  anaimagedata *header;
  int           header_bits;
  char*         header_filename;
 
  PXTkLabel* label[9];
  PXTkEntry* entry[9];

  PXTkOptionMenu* orientation;
  PXTkOptionMenu* data_type;
  PXTkCheckButton* byte_swapped;

};

#endif

