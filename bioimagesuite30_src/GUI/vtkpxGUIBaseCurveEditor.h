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
  Module:    $RCSfile: vtkpxGUIBaseCurveEditor.h,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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
// .NAME vtkpxGUIBaseCurveEditor - Base class for curve/point editors
// .SECTION Description
// This is an abstract base class for curve/point editors
// Facilities for handling clicked points, maintaining a list of objects, setting
// Image,LookupTable and CurrentTransform
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION See Also
//  vtkpxGUIBaseCurveControl


#ifndef __vtkpxGUIBaseCurveEditor_h
#define __vtkpxGUIBaseCurveEditor_h

#include "vtkpxGUIComponent.h"

class vtkRenderer;
class vtkLookupTable;
class vtkImageData;
class vtkAbstractTransform;

class vtkpxGUIBaseCurveEditor : public vtkpxGUIComponent 
{
public:

  vtkTypeMacro(vtkpxGUIBaseCurveEditor,vtkpxGUIComponent);

  // Description:
  // Set Renderer for Display purposes 
  virtual void SetRenderer(vtkRenderer *ren);
  virtual void SetRenderer(vtkRenderer *rendererer,int num)=0;

  // Description:
  // Generic Event Handlers
  virtual void Update()=0;
  virtual void UpdateDisplay()=0;
  virtual int  HandleEvent(int)=0;
  virtual int  HandleClickedPoint(float x,float y,float z,int scaled=1,int state=0)=0;

  // Description:
  // Access Internal Storage i-vtkObjets
  virtual int         GetCurrentIndex()=0;
  virtual vtkObject*  GetItem(int i)=0;
  virtual void SetItemColor(int index,double color[3]) {};
  virtual void SetItemFilename(int index,const char* s) {};

  // Description:
  // Image Related Stuff 
  virtual void SetSpacingOrigin(double sp[3],double ori[3])=0;
  virtual void SetPlaneLevel(int plane,int level)=0;
  virtual void SetFrame(int frame)=0;
  virtual void SetImage(vtkImageData* image,int plane=0,int level=-1,int frame=0)=0;

  // Description:
  // Lookup Table
  virtual void SetLookupTable(vtkLookupTable* lookuptable)=0;

  // Description:
  // Current Transform 
  virtual void SetTransform(vtkAbstractTransform* transform)=0;
  virtual vtkAbstractTransform* GetTransform()=0;


};

#endif   


