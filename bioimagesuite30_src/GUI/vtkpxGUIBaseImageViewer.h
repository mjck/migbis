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
// .NAME vtkpxGUIBaseImageViewer - Base Image Viewer
// .SECTION Description

#ifndef __vtkpxGUIBaseImageViewer_h
#define __vtkpxGUIBaseImageViewer_h

class vtkImageData;
class vtkPoints;
class vtkImageData;

#include "vtkpxGUIComponent.h"
#include "vtkRenderWindow.h"
#include "vtkWindow.h"
#include "vtkPoints.h"
#include "vtkTransform.h"
#include "vtkpxGUIColormapEditor.h"
#include "vtkRenderer.h"
#include <vtkstd/string>

class vtkpxTalairachTransform;
class vtkLookupTable;
class vtkRenderer;
class vtkScalarBarActor;

class vtkpxGUIBaseImageViewer : public vtkpxGUIComponent
{
public:
  
  vtkTypeMacro(vtkpxGUIBaseImageViewer,vtkpxGUIComponent);
  
  // Initialize Display  
  virtual char*  Initialize(const char* name,int inside);
  
  // Display Stuff 
  virtual int  InitializeViewer() { return 1;}
  virtual int  ResetViewer()      { return 1;}
  virtual int  UpdateDisplay()    { return 0;}
  virtual int  CustomPick(float selectionX, float selectionY, float selectionZ,
			  float &x1,float& x2,float &x3) { return 0; }

  // Description:
  // Set Input Image
  virtual int  SetImage(vtkImageData* im) {   return this->SetImage(im,0,NULL); }
  virtual int  SetImage(vtkImageData* im,int orientation) { return this->SetImage(im,orientation,NULL,0.0,1.0); }
  virtual int  SetImage(vtkImageData* im,int orientation,vtkLookupTable* lkp) { return this->SetImage(im,orientation,lkp,0.0,1.0); }
  virtual int  SetImage(vtkImageData* im,int orientation,double offset,double scale) { return this->SetImage(im,orientation,NULL,offset,scale); }
  virtual int  SetImage(vtkImageData* im,int orientation,vtkLookupTable* lkp,double offset,double scale);
  virtual vtkImageData* GetImage();

  // Description:
  // Set Value Image (for overlays)
  virtual int SetValueImage(vtkImageData* val);
  virtual vtkImageData* GetValueImage();

  // Description:
  // CLicked Points
  vtkGetVectorMacro(LastClickedPoint,double,3);
  vtkSetVectorMacro(LastClickedPoint,double,3);

  vtkGetVectorMacro(LastClickedPointScaled,double,3);
  vtkSetVectorMacro(LastClickedPointScaled,double,3);

  // Description:
  // Information for Images
  vtkGetMacro(Orientation,int);
  vtkGetMacro(NumberOfFrames,int);

  vtkSetClampMacro(PresetLookupTableMode,int,-1,7);
  vtkGetMacro(PresetLookupTableMode,int);

  // Export Facilities
  virtual void SaveAsTiff(vtkRenderWindow* renwin);
  virtual void SaveAsTiff(vtkRenderWindow* renwin,const char* fname);

  virtual void SaveAllAsTiff(vtkRenderWindow* renwin);
  virtual void SaveAllAsTiff(vtkRenderWindow* renwin,const char* fname);


  // Scale factor -- render higher resolution images
  vtkSetClampMacro(MagnificationFactor,int,1,8);
  vtkGetMacro(MagnificationFactor,int);

  virtual void ExportAsVRML(vtkRenderWindow* renwin);
  virtual void ExportAsX3D(vtkRenderWindow* renwin, vtkRenderer *renderer);
  virtual void SetBackgroundColor(vtkRenderWindow* renwin);



  virtual void LoadTalairachLandmarks(const char* fname=NULL);
  virtual vtkpxTalairachTransform* GetTalairachTransform()
  { return talairachTransform; }
  virtual void SetTalairachPoints(vtkPoints* pt);


  virtual int LoadTalairachTransform(const char* fname=NULL);
  virtual int SaveTalairachTransform(const char* fname=NULL);

  virtual vtkLookupTable* GetLookupTable() { return this->mainLookupTable;}
  virtual void SetLookupTable(vtkLookupTable* lkp);
  virtual void LinkLookupTable(vtkLookupTable* lkp);
  virtual void SetFmriMergeLookupTable();
  virtual void SetFmriMergeLookupTable(int mode);
  virtual void SetPresetLookupTable(int mode=0);

  // Description:
  // If this is 0 then off, 1=auto
  // if auto then 3-frame images unsigned char are treated as color
  // everything else as multiframe
  virtual void SetColorMode(int md=1);

  // Description:
  // If 1 viewer is treating current image as color
  vtkGetMacro(CurrentImageIsColor,int);

  // Procedures for handling multiple frames
  virtual int  GetCurrentFrame();
  virtual void SetCurrentFrame(int t);
  virtual void ShowFrameControls(int show=1);

  // Description:
  // Callback On Change Of Frame 
  virtual void   SetFrameChangeCallback(const char* CallbackName);
  vtkGetMacro(EnableFrameChangeCallback,int);
  vtkSetClampMacro(EnableFrameChangeCallback,int,0,1);

  // Description:
  // Callback On Change Of Coordinates 
  virtual void   SetCoordinatesChangeCallback(const char* CallbackName);
  vtkGetMacro(EnableCoordinatesChangeCallback,int);
  vtkSetClampMacro(EnableCoordinatesChangeCallback,int,0,1);
  virtual const char*   GetCoordinateChangeCallback() { return this->coordinatesChangeCallbackName; }
  virtual const char*   GetFrameChangeCallback() { return this->frameChangeCallbackName; }


  vtkGetMacro(EnableRendering,int);
  vtkSetMacro(EnableRendering,int);

  // Description:
  // Get RenderWidget Name
  virtual char* GetRenderWidgetName() =0 ;
  

  // Description:
  // GetKeyRenderer
  vtkGetObjectMacro(KeyRenderer,vtkRenderer);
  vtkGetMacro(ShowKeyRenderer,int);
  vtkSetMacro(ShowKeyRenderer,int);
  virtual void ToggleKeyRenderer();
  virtual void UpdateKeyRenderer() { };
  vtkGetObjectMacro(ScalarBar,vtkScalarBarActor);


  // Description:
  // Enable or Disable Talairach Buttons
  virtual void DisableTalairachButtons();
  virtual void EnableTalairachButtons();

  // Description:
  // Has Objectmap
  virtual int HasObjectmap() { return 0;}


  // Description:
  // Painting Enabled
  virtual int PaintingEnabled() { return 0;}


  // Description:
  // Map Nw Style Orientations to Old Style and back
  // New Style Axial=10:17, Coronal: 20-27, Sagital 30:37
  // Old Style Axial =0, coronal =1 , sagittal = 2
  static int MapOrientationToNewStyle(int oldorient);
  static int MapOrientationToOldStyle(int neworient);

  // This generates the axis-names
  const char* OrientationToAxisName(int neworient,int axis);

protected:

  vtkImageData*           currentImage;
  vtkImageData*           valueImage;
  vtkLookupTable*         mainLookupTable;
  vtkRenderer*            KeyRenderer;
  vtkScalarBarActor*      ScalarBar;
  vtkpxGUIColormapEditor* ColormapEditor;
  int                     ownsLookupTable;
  int                     ShowKeyRenderer;
  char*                   frameChangeCallbackName; 
  int                     EnableFrameChangeCallback;

  char*                   coordinatesChangeCallbackName; 
  int                     EnableCoordinatesChangeCallback;


  PXBool                  hasImage;
  int                     Orientation;
  double                  IntensityScale;
  double                  IntensityOffset;
  int                     PresetLookupTableMode;
  int                     CurrentFrame;
  int                     ColorMode;
  int                     CurrentImageIsColor;
  int                     NumberOfFrames;
  int                     EnableRendering;
  int                     MagnificationFactor;

  PXBool                    createTalairachButtons;

  vtkpxGUIBaseImageViewer();
  virtual ~vtkpxGUIBaseImageViewer();

  double                   LastClickedPoint[3];
  double                   LastClickedPointScaled[3];
  vtkpxTalairachTransform* talairachTransform;


  virtual void     ExecuteFrameChangeCallback();
  virtual void     ExecuteCoordinatesChangeCallback();



  // Description:
  // Talairach Grid
  virtual void UpdateTalairachGrid(int force);
  virtual void  RenderDelay(int t=3);

  // Description:
  // Coordinate Updates
  virtual int  UpdateIntensityLabelInfo() =0;
  virtual int  DisplayNeuroCoordinates(double pos[3],int coord[3],PXTkLabel* talairachlabel,PXTkLabel* mnilabel,vtkImageData* img=NULL);
  const  char* GetFormatedNumber(double vl,int dp);
  virtual double GetMNISliceNo(int index,int plane,int& axis);

  // Description:
  // Utility Functions
  virtual int DoImagesHaveSameDimensions(vtkImageData* img,vtkImageData* img2,int checknumframes=0);

  // Description:
  // Colormap Stuff
  virtual int ResetColormap();
  virtual int ResetEditor() { return 0;}

  // Objectmap Value
  //BTX
  virtual std::string GetObjectmapValueAsText(int a=0,int b=0,int c=0);
  //ETX

private:
  static void handleRenderEvent(void *arg1);

};

#endif   


