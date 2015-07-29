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
  Module:    $RCSfile: vtkpxGUI4DTriangulatedStackControl.h,v $
  Language:  C++
  Date:      $Date: 2002/04/09 15:46:27 $
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
// .NAME vtkpxGUI4DTriangulatedStackControl - Base Image Viewer
// .SECTION Description

#ifndef __vtkpxGUI4DTriangulatedStackControl_h
#define __vtkpxGUI4DTriangulatedStackControl_h

#include "vtkpxGUITriangulatedStackControl.h"


class vtkImageData;
class vtkpxGUI4DRenderer;
class vtkCollection;
class vtkActorCollection;

class vtkpxGUI4DTriangulatedStackControl : public vtkpxGUITriangulatedStackControl 
{
public:

  static vtkpxGUI4DTriangulatedStackControl *New();
  vtkTypeMacro(vtkpxGUI4DTriangulatedStackControl,vtkpxGUITriangulatedStackControl);
  
  // Description:
  // Initialize Display and add Renderer 
  virtual char*  Initialize(const char* name,int inside);
  virtual char*  Initialize(const char* name,const char* name2,int inside);

  // Description:
  // Set Renderer
  virtual void   SetRenderer(vtkpxGUI4DRenderer *rendererer,int num);
  virtual void   SetRenderer(vtkpxGUI4DRenderer *rendererer,int num,int wireframe);

  // Description:
  // Update Display
  virtual void   Update();
  virtual void   UpdateDisplay();


  // Description:
  // Set Image Level,Plane And Frame 
  virtual void SetImage(vtkImageData* image,int orientation,int level=0,int frame=0);
  virtual void SetImage(vtkImageData* image);

  // Description:
  // Event Manager 
  virtual int    HandleEvent(int);

  // Description:
  // Get Triangulated Stack from Collection
  virtual vtkpxTriangulatedStackSource* GetTriangulatedStack(int frame,int index);

  // Description:
  // If outside process modified triangulateds (using GetTriangulatedStack) this functions needs to
  // be called to re-synchronize the GUI etc
  virtual void UpdateAfterExternalEditing();

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
  virtual int  MultiLoad(const char* fname);
  virtual int  MultiSave(const char* fname);

protected:

  vtkpxGUI4DTriangulatedStackControl();
  virtual ~vtkpxGUI4DTriangulatedStackControl();

  // Description:
  // Disable these two functions insist on multirenderer 
  virtual void   SetRenderer(vtkRenderer *rendererer,int num);
  virtual void   SetRenderer(vtkRenderer *rendererer,int num,int wireframe);


  // Description:
  // Collection Stuff
  vtkCollection*       triangulatedStackCollection;
  vtkActorCollection*  triangulatedStackActorCollection;
  vtkpxGUI4DRenderer*  MultiRenderer[TSP_MAX_RENDERERS];
  int                  wireframemode[TSP_MAX_RENDERERS];
  char*                surface_filename[TSP_MAX_COLLECTIONS];


  // Description:
  // Frame Stuff
  int   CurrentFrame;
  int   NumberOfFrames;
  int   ActualNumberOfFrames;


  // Description:
  // Initialize TriangulatedStacks
  virtual void InitializeTriangulatedStacks();
  virtual void ResetTriangulatedStacks();

  // Description:
  // Frame Update Stuff 
  virtual void CreateFrames(int numframes);
  virtual void UpdateFrame(int frame);
  virtual void ChangeFrame(int newframe);

  // Description:
  // Handle Callbacks for GUI Stuff
  virtual void DoCurvatures(int mode,int moption=-1);
  virtual void DoShapeTracking(int mode,int moption=-1);

};



#endif   


