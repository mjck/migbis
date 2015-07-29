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
  Module:    $RCSfile: vtkpxGUIOrthogonal4DViewer.h,v $
  Language:  C++
  Date:      $Date: 2004/03/18 18:31:23 $
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

// .NAME vtkpxGUIOrthogonal4DViewer 
// .SECTION Description

#ifndef __vtkpxGUIOrthogonal4DViewer_h
#define __vtkpxGUIOrthogonal4DViewer_h

#include "vtkpxGUIOrthogonalViewer.h"
#include "pxvtkgadgets.h"
#include "pxtkscale.h"
#include "pxtkframe.h"
#include "pxtkoptionmenu.h"
#include "pxtklabel.h"
#include "pxutil.h"

class vtkActor;
class vtkPropCollection;
class vtkVolumeCollection;
class vtkActorCollection;
class vtkpxGUI4DRenderer;
class vtkpxImageSlice;
class vtkpxOrthoImageSlice;
class vtkOutlineFilter;
class vtkStructuredGrid;
class vtkpxVolume;

class  vtkpxGUIOrthogonal4DViewer : public vtkpxGUIOrthogonalViewer
{
public:
  
  static vtkpxGUIOrthogonal4DViewer *New();
  vtkTypeMacro(vtkpxGUIOrthogonal4DViewer,vtkpxGUIOrthogonalViewer);

  // Description:
  // Functions Overriden from vtkpxGUIOrthogonalViewer
  virtual int   InitializeViewer();
  virtual int   ResetViewer();
  virtual int   HandleEvent(int event);

  // Description:
  // Movie Stuff 
  virtual char* InitializeMovieControl(const char* name,int inside);
  virtual char* InitializeMovieControl(const char* name) { return InitializeMovieControl(name,1); }

  virtual int   ShowMovieControl(int show);
  virtual int   MoviePrepare();
  virtual int   MoviePlay();
  virtual int   MovieStop();
  
  virtual void  ShowFrameControls(int show=1);

  virtual vtkpxGUI4DRenderer*  GetMultiRenderer(int r);
protected:

  vtkpxGUIOrthogonal4DViewer();
  virtual ~vtkpxGUIOrthogonal4DViewer();

  vtkpxGUI4DRenderer*       MultiRenderer[4];
  vtkPropCollection*        ImageSliceCollection; // 5 Actors per frame 0-2 image slice[0-2] 3=orthoslice 4=obliqueslice
  vtkActorCollection*       AxisActorCollection;  // 10 Actors per frame 
  vtkActorCollection*       OutlineActorCollection;  // 4 Actors per frame 
  vtkVolumeCollection*      ImageVolumeCollection;
  int                       ActualNumberOfFrames;

  // Movie Related Stuff
  PXTkFrame*                MovieControl;
  PXTkArrowScale            *movieFrameScale;
  PXTkArrowScale            *movieRateScale;
  PXTkOptionMenu*           loopMenu;
  PXTkOptionMenu*           playMenu;
  //BTX
  PXTkButton                *playB,*prepareB,*pauseB,*rewindB,*closeB,*showMB;
  //ETX
  float                     MovieFrameDelay;
  int                       MovieLoopMode;
  int                       MovieFrameStep;
  int                       MoviePlayMode;
  int                       CurrentlyPlayingMovie;
  int                       MovieControlInside;


  virtual void InitViewer();

  // Description:
  // Change Frame -- copy outlines, actors axis/actors etc etc etc 
  virtual void ChangeFrame(int newframe);

  // Description:
  // Update Frame for new Input Image
  virtual void UpdateFrame(int frame,int full);
  virtual void UpdateFrameCartesian(int frame,int full);
  virtual void UpdateFramePolar(int frame,int full);

  // Description:
  // Reset Viewer Stuff
  virtual int ResetViewerUpdatePolar();
  virtual int ResetViewerUpdateCartesian();



  // Description:
  // Add Frame
  virtual void CreateFrames(int numframes);

  // Description:
  // Fast Change Frame 
  virtual void FastChangeFrame(int newframe=-1);

  // Description:
  // Handle Next Frame Event (timer)
  static void HandleNextFrameEvent(void *arg1);

};


#endif   


