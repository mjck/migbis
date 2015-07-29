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
  Module:    $RCSfile: vtkpxGUIFemPostControl.h,v $
  Language:  C++
  Date:      $Date: 2003/05/28 16:43:01 $
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
// .NAME vtkpxGUIFemPostControl - Base Image Viewer
// .SECTION Description

#ifndef __vtkpxGUIFemPostControl_h
#define __vtkpxGUIFemPostControl_h

#include "vtkpxGUITriangulatedStackControl.h"
#include "vtkpxFemSolid.h"

class vtkImageData;
class vtkpxGUI4DRenderer;
class vtkCollection;
class vtkActorCollection;
class vtkTransform;

class vtkpxGUIFemPostControl : public vtkpxGUIComponent 
{
public:

  static vtkpxGUIFemPostControl *New();
  vtkTypeMacro(vtkpxGUIFemPostControl,vtkpxGUITriangulatedStackControl);
  
  // Description:
  // Initialize Display and add Renderer 
  virtual char*  Initialize(const char* name,int inside);

  // Description:
  // Set Renderer
  virtual void   SetRenderer(vtkpxGUI4DRenderer *rendererer,int num);
  virtual void   SetRenderer(vtkpxGUI4DRenderer *rendererer,int num,int wireframe);

  // Description:
  // Update Display
  virtual void   Update();
  virtual void   UpdateDisplay();
  virtual void   UpdateScaling(float x,float y,float z);

  // Description:
  // Event Manager 
  virtual int    HandleEvent(int);

  // Description:
  // Enable Change Of Frame from this control -- bad idea 
  //virtual void EnableFrameScale(int enable);

  // Description:
  // Frame Stuff
  virtual void SetCurrentFrame(int fr);
  virtual int  GetCurrentFrame();
  virtual int  GetNumberOfFrames();

  // Description:
  // Load / Save
  virtual int  LoadStudy(const char* fname);
  virtual int  LoadReferencePoint(const char* fname);
  virtual int  LoadIndices(const char* fname);
  virtual int  SaveIndices(const char* fname);
  virtual int  SavePies(const char* fname);
  virtual int  ReslicePies();


  // Description:
  // Location and Normal
  vtkGetVectorMacro(ReferencePoint,float,3);
  vtkSetVectorMacro(ReferencePoint,float,3);

  // Description:
  // SetImage
  virtual void SetImage(vtkImageData* image,int orientation);

protected:

  vtkpxGUIFemPostControl();
  virtual ~vtkpxGUIFemPostControl();

  // Description:
  // Pointers to Current Frame Stuff
  vtkpxFemSolid     *currentSolid;
  vtkActor*         solidActor[TSP_MAX_RENDERERS];
  vtkRenderer*      Renderer[TSP_MAX_RENDERERS];

  // Description:
  // Collection Stuff
  vtkCollection*       solidCollection;
  vtkCollection*       transformFilterCollection;
  vtkActorCollection*  solidActorCollection;
  vtkTransform*        scalingTransformation;

  vtkpxGUI4DRenderer*  MultiRenderer[TSP_MAX_RENDERERS];
  int                  wireframemode[TSP_MAX_RENDERERS];
  char*                study_filename;
  float                ReferencePoint[3];
  float                ScaleFactor[3];

  // Description:
  // Frame Stuff
  int   CurrentFrame;
  int   NumberOfFrames;
  int   ActualNumberOfFrames;

  // Description:
  // Initialize SolidStacks
  virtual void InitializeSolids();
  virtual void ResetSolids();

  // Description:
  // Frame Update Stuff 
  virtual void CreateFrames(int numframes);
  virtual void UpdateFrame(int frame);
  virtual void ChangeFrame(int newframe);

  // Description:
  // Widgets
  //BTX
  PXTkOptionMenu  *modeMenu,*plotMenu,*resolutionMenu;
  PXTkLabel       *statusLine1,*statusLine2;
  PXTkOptionMenu  *numslicesMenu,*numsectorsMenu,*numtransmuralMenu;
  PXTkCheckButton *showHide,*anticlockwise,*autoscale;
  PXTkEntry*      scaleFactorEntry[3];
  int             displayMode;
  int             currentImageNumberOfFrames;
  int             currentOrientation;
  //ETX

  

};



#endif   


