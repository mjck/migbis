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
  Module:    $RCSfile: vtkpxGUIVolumeControl.h,v $
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
// .NAME vtkpxGUIVolumeControl - create hierarchies of actors
// .SECTION Description
#ifndef __vtkpxGUIVolumeControl_h
#define __vtkpxGUIVolumeControl_h

#include "vtkpxGUIComponent.h"
#include "vtkpxVolume.h"
#include "vtkpxGUIBaseRenderer.h"

#include "pxtkframe.h"
#include "pxtkscale.h"
#include "pxitkcombobox.h"
#include "vtkpxGUIOrthogonalViewer.h"
#include "vtkpxGUIBaseCurveControl.h"

class vtkpxGUIColormapEditor;

class vtkpxGUIVolumeControl : public vtkpxGUIComponent
{
public:
  static vtkpxGUIVolumeControl *New();
  vtkTypeMacro(vtkpxGUIVolumeControl,vtkpxGUIComponent);

  virtual char* Initialize(const char* name,int inside);
  virtual int   HandleEvent(int );

  virtual void         SetVolumeRenderer(vtkpxVolume* vol,vtkpxGUIBaseRenderer* viewer1);
  virtual void         SetRenderer(vtkpxGUIBaseRenderer* viewer1);

  virtual void         UpdateVolumeRenderer(vtkpxVolume* vol,vtkpxGUIBaseRenderer* ren);

  virtual void Update(int full=1);
  virtual void UpdateDisplay();
  virtual int  UpdateCrop();
  virtual vtkpxGUIBaseRenderer* GetViewer();
  virtual void SetVisibility(int vis);
  virtual void SetPresetLookupTable(int m);

  virtual vtkpxGUIColormapEditor* GetColorMapEditor() { return ColormapEditor;}

  virtual void SetLookupTable(vtkLookupTable* lkup,int auto_convert=1);

  virtual int  GetClipScaleValue(int i);
  virtual void SetClipScaleValue(int i,int v);


 // Description:
 // Pointer for Ortho Viewer
   vtkGetObjectMacro(OrthoViewer,vtkpxGUIOrthogonalViewer);
   vtkSetObjectMacro(OrthoViewer,vtkpxGUIOrthogonalViewer);

 // Description:
 // Pointer for Landmark Control
   vtkGetObjectMacro(LandmarkControl,vtkpxGUIBaseCurveControl);
   vtkSetObjectMacro(LandmarkControl,vtkpxGUIBaseCurveControl);


protected:

  vtkpxGUIVolumeControl();

  vtkpxVolume            *Volume;
  vtkpxGUIBaseRenderer      *Viewer;
  vtkpxGUIBaseRenderer      *internal_viewer;
  vtkpxGUIOrthogonalViewer  *OrthoViewer;
  vtkpxGUIBaseCurveControl* LandmarkControl;
  PXBool                 has_volume;
  PXBool                 Visibility;
  PXBool                 DualInput;

  /*PXTkScale              *minIntensityScale;
    PXTkScale              *maxIntensityScale;
    PXTkScale              *minTransparencyScale;
    PXTkScale              *maxTransparencyScale;*/

  
  // irregular cropping
  PXTkScale              *cropSphereRadius;
  PXTkScale              *cropCubeLength;
  PXTkScale              *cropEllipseRadiusX;
  PXTkScale              *cropEllipseRadiusY;
  PXTkScale              *cropEllipseRadiusZ;   
  PXTkScale              *cropCylinderRadius;


  PXTkScale              *clipScale[6];
  PXTkScale              *rateScale;
  PXTkScale              *rateScale2;  
  PXTkOptionMenu         *resliceMode;
  PXTkOptionMenu         *interpolationMode;
  PXTkOptionMenu         *croppingShape;
  //  PXITkComboBox          *interpolationMode;

  PXTkCheckButton        *show_volume;
  PXTkCheckButton        *dual_input;
  PXTkCheckButton        *shadeOn;
  PXTkCheckButton        *fourColorMode;

  PXTkOptionMenu         *volumeRenderingMode;
  PXTkCheckButton        *irregularClipping;
  PXTkCheckButton        *gradientEnhancement;
  PXTkCheckButton        *show_outline;
  vtkpxGUIColormapEditor *ColormapEditor;
  PXTkFrame              *viewerFrame;
  PXTkFrame              *volFrame;

  // Description:
  // Show/Hide Rate Controls
  virtual void ShowRateControls(int show);
  virtual void ShowCropControls(int show);

  // Description:
  // Update Orthogonal Viewer
  virtual int UpdateOrthogonalViewerNavigationPointer();

};

#endif

