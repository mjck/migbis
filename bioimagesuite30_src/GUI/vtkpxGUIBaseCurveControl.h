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





// .NAME vtkpxGUIBaseCurveControl
// .SECTION Description

#ifndef __vtkpxGUIBaseCurveControl_h
#define __vtkpxGUIBaseCurveControl_h

#include "vtkpxBaseCurve.h"
#include "vtkpxGUIBaseCurveEditor.h"
#include "pxtkapp.h"
#include "pxtkeventobj.h"
#include "pxtkgadgets.h"
#include "pxtkcomplexwidgets.h"
#include "pxtkdialog.h"
#include "pxtkmenu.h"
#include "pxcontour.h"


const int BCC_MAX_COLLECTIONS = 40; 
const int BCC_MAX_RENDERERS   = 16; 
class vtkActor;
class vtkPolyData;
class vtkSphereSource;
class vtkTransformPolyDataFilter;

class vtkpxGUIBaseCurveControl : public vtkpxGUIBaseCurveEditor 
{
public:

  static vtkpxGUIBaseCurveControl *New();
  vtkTypeMacro(vtkpxGUIBaseCurveControl,vtkpxGUIBaseCurveEditor);
  
  // Initialize Display  
  virtual char*  Initialize(const char* name,int inside);

  virtual void SetRenderer(vtkRenderer *rendererer,int num);

  virtual void Update();
  virtual void UpdateDisplay();
  virtual int  HandleEvent(int);
  
  virtual void UpdateStatus();
  virtual int  HandleClickedPoint(float x,float y,float z,int scaled=1,int state=0);
  virtual void SetSpacingOrigin(double sp[3],double ori[3]);


  virtual int  GetNumberOfCollections(); 
  virtual vtkpxBaseCurve* GetCollection(int index=-1);
  virtual vtkObject* GetItem(int index) { return GetCollection(index);}
  virtual int  GetCurrentIndex();

  virtual void SetCollection(vtkpxBaseCurve* land,int index=-1);
  virtual void SetCollection(PXContour* cntr,int index=-1,int plane=2,int level=0);
  virtual void SetClipboardCollection(vtkpxBaseCurve* data,int scaled=1);
  virtual void SetClipboardCollection(vtkPoints* data);

  virtual void DisableLabels();
  virtual void DisableList();
  virtual void DisableCurve();
  virtual void DisableComplex();

  virtual void SetImage(vtkImageData* image,int plane=0,int level=-1,int frame=0);
  virtual void SetPlaneLevel(int plane,int level);
  virtual void SetFrame(int frame);
  virtual void SetLookupTable(vtkLookupTable* lookuptable);
  virtual void SetTransform(vtkAbstractTransform* transform);
  virtual vtkAbstractTransform* GetTransform() { return this->transform;}

  virtual int  DoExtract(int apply);
  virtual int  DoSnake(int apply);
  virtual int  DoThreshold(int apply);

  virtual void SetHighlight(int num=-1);

  virtual int  SetTalarachSetup();
  virtual int  SetTransformSetup();
  virtual int  SetArticulatedMouseSetup(int hasfeet=1,int hashindlimbs=1,int hasforelimbs=0,int hashead=0,
					int taildivisions=0,int splinedivisions=0);
  virtual void SaveTransform(const char* fname);
  virtual int  SetBrainstripSetup();

  // Description:
  // Outside Interface
  virtual void SetCurrentCollection(int num);
  virtual void LoadLandmarks(const char* fname);

protected:

  vtkpxGUIBaseCurveControl();
  virtual ~vtkpxGUIBaseCurveControl();

  vtkpxBaseCurve *landmarkCollection;
  vtkpxBaseCurve *allCollections[BCC_MAX_COLLECTIONS];
  PXBool         showCollection[BCC_MAX_COLLECTIONS];
  static vtkpxBaseCurve *clipboardCollection;

  vtkRenderer*       renderer[BCC_MAX_RENDERERS];
  vtkActor*          landmarkActor[BCC_MAX_RENDERERS][BCC_MAX_COLLECTIONS];
  vtkActor*          highlightActor[BCC_MAX_RENDERERS];
  vtkSphereSource*   highlightData;
  vtkTransformPolyDataFilter* highlightFilter;
  vtkTransformPolyDataFilter* landmarkFilter[BCC_MAX_COLLECTIONS];
  int                highlightIndex;

  PXTkDialog*     edit_window;
  PXTkDialog*     label_window;
  PXTkDialog*     extractGUI;
  PXTkDialog*     snakeGUI;
  PXTkDialog*     thresholdGUI;
  PXTkOptionMenu* extractType;

  PXTkArrowScale* thresholdScale;
  PXTkArrowScale* medianScale;
  PXTkArrowScale* sigmaScale;
  PXTkArrowScale* snakeEdgeWeight;
  PXTkArrowScale* snakeSmoothWeight;
  PXTkArrowScale* snakeTolerance;
  PXTkArrowScale* snakeStep;
  PXTkArrowScale* snakeSigma;
  PXTkArrowScale* snakeIterations;
  PXTkArrowScale* snakeBaloon;
  PXTkEntry*      entryThreshold;

  PXBool          has_landmark_collection;
  PXBool          enable_labels;
  PXBool          enable_list;
  PXBool          enable_curve;
  PXBool          enable_complex;
  int             current_collection;
  int             num_collections;
  PXBool          click_mode;
  PXBool          owns_transform;

  PXTkListBox*     listBox;
  PXTkListBox*     labelListBox;
  PXTkLabel*       message;
  PXTkLabel*       currentLabel;
  PXTkEntry        *coord[3];
  PXTkEntry        *label_entry;
  PXTkOptionMenu*  curveMode;
  PXTkOptionMenu*  mouseMode;
  PXTkOptionMenu   *currentCollection;
  //PXTkOptionMenu   *displayMode;
  PXTkCheckButton    *displayCollection;
  PXTkOptionMenu   *scaleMode;
  //*clickMode,*addMode,*autoMode
  int             callback_no;
  int             edit_item;

  char            clipboard[100];
  static char     lamstring[400];

  vtkLookupTable*      backupColormap;
  vtkLookupTable       *currentColormap;
  vtkImageData*         image;
  vtkAbstractTransform* transform;

  int             image_plane;
  int             image_level;
  int             image_frame;
  double          image_range[2];

  virtual void ClipboardCommand(int mode);
  virtual void AddListLandmark(float x,float y,float z,int index=-1,int scaled=0,int listonly=0);
  virtual void ExtractCoordinates(const char* line,float& x,float& y,float& z);

  virtual void SaveSeedFile(const char* fname);
  virtual void LoadSeedFile(const char* fname);

  virtual void UpdateListFromCurrentCollection();

  virtual void CreateEditWindow(int callbackno);
  virtual int  UpdateEditWindow();
  virtual int  HandleEditWindowCallback();

  virtual int  LoadSetupFile(const char* fname);
  virtual int  SaveSetupFile(const char* fname);
  virtual void CreateLabelWindow();
  virtual int  HandleLabelWindowCallback(int option);
  
  virtual void SetHighlightDisplay(int disp=1);
};



#endif   


