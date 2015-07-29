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
  Module:    $RCSfile: vtkpxSurfaceUtil.h,v $
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
// .NAME vtkpxGUIPolyDataControl - Base Image Viewer
// .SECTION Description

#ifndef __vtkpxGUIPolyDataControl_h
#define __vtkpxGUIPolyDataControl_h


#include "pxtkapp.h"
#include "pxtkcomplexwidgets.h"
#include "pxtkdialog.h"
#include "pxtkgadgets.h"
#include "pxvtkgadgets.h"
#include "vtkPolyData.h"
#include "vtkTexture.h"
#include "vtkpxColorMapUtil.h"
#include "vtkpxGUIComponent.h"

class  vtkImageData;
//class  vtkpxGUITransformControl;
class vtkActor;
class vtkPolyDataMapper;
class vtkProperty;
class vtkRenderer;
class vtkAbstractTransform;
class vtkIdentityTransform;
class vtkTransformPolyDataFilter;

class vtkpxGUIPolyDataControl : public vtkpxGUIComponent 
{
public:

  static vtkpxGUIPolyDataControl *New();
  vtkTypeMacro(vtkpxGUIPolyDataControl,vtkpxGUIComponent);
  
  // Initialize Display  
  virtual char*  Initialize(const char* name,int inside);

  virtual void SetRenderer(vtkRenderer *v) { SetRenderer(v,0,0);}
  virtual void SetRenderer(vtkRenderer *v,int num,int clip=0);

  virtual void UpdateDisplay();
  virtual int  HandleEvent(int);

  virtual int  SetCurrentSurface(int s);
  virtual int  UpdateStatus();

  virtual int  GetCurrentSurface();
  virtual vtkPolyData* GetSurface(int index=-1);

  virtual void SetSurface(int index,vtkPolyData* data);
  virtual void SetSurface(int index,vtkPolyData* data,const char* name);


  // Clipboard Surface Stuff -- all static
  // -------------------------------------
  static void SetClipboardSurface(vtkPolyData* data,const char* name=NULL);
  static vtkPolyData* GetClipboardSurface() { return clipboardPolyData;}
  static char* GetClipboardSurfaceName() { return clipboardName;}

  virtual void SetImage(vtkImageData* new_image);
  virtual void SetLastPoint(double pt[3]);
  virtual void SetLastPoint(double x,double y,double z) 
  { double p[3]; p[0]=x; p[1]=y; p[2]=z; SetLastPoint(p);}

  virtual void SetTransform(vtkAbstractTransform* trans);
  virtual vtkAbstractTransform* GetTransform() { return this->internalTransform;}

  virtual void SetSimpleMode();

  virtual int GetNumberOfSurfaces() { return num_surfaces;}
  
  // GetImage Macro
  vtkGetObjectMacro(CurrentImage,vtkImageData);

  virtual int LoadCurrentSurface(const char* fname);
  virtual int LoadCurrentTexture(const char* fname);
  virtual int LoadCustomLookupTable(const char* fname);

protected:

  vtkpxGUIPolyDataControl();
  virtual ~vtkpxGUIPolyDataControl();

  vtkImageData                 *CurrentImage;
  
  PXBool                        simple_mode;
  vtkActor                      *surfaceActor[4][10];
  vtkPolyData                   *polyData[10];
  int                           displayPolyData[10];
  vtkTransformPolyDataFilter    *polyDataFilter[10];
  vtkPolyData                   *previousPolyData;
  vtkPolyData                   *tempPolyData;
  static vtkPolyData            *clipboardPolyData;
  static char                    clipboardName[200];
  double                         lastPoint[3];
  
  vtkAbstractTransform*         internalTransform;
  vtkRenderer                   *renderer[4];
  PXBool                        has_surface;
  PXBool                        wireFrame;
  PXBool                        owns_transform;
  PXBool                        save_binary;

  int                           num_surfaces;
  int                           current_surface;
  int                           num_renderers;

  PXTkOptionMenu* currentSurface;
  PXTkLabel*      message;
  PXTkOptionMenu* displayMode;

  //  vtkpxGUITransformControl* transformControl;
  PXTkDialog      *decimateGUI;
  PXTkDialog      *smoothGUI;
  PXTkDialog      *normalsGUI;
  PXTkDialog      *cleanGUI;
  PXTkDialog      *curvatureGUI;
  PXTkDialog      *connectGUI;
  PXTkDialog      *extractGUI;
  PXTkDialog      *extractObjectMapGUI;
  PXTkDialog      *displaySizeGUI;
  PXTkDialog      *mapFunctionalGUI;
  PXTkDialog      *appendGUI;
  PXTkDialog      *thresholdPointsGUI;
  // mpj
  PXTkDialog      *textureGUI;
  PXTkDialog      *materialGUI;
  PXTkScale       *ambientFactor;
  PXTkScale       *diffuseFactor;
  PXTkScale       *specularFactor;
  PXTkScale       *specularPower;
  PXTkScale       *opacityFactor;
  PXTkScale       *textureOrigin[2];
  PXTkScale       *textureTranslate[2];
  PXTkScale       *textureScale[2];
  // mpj

  PXTkArrowScale  *thresholdScale[3];
  PXTkCheckButton *extractDoThreshold;
  PXTkArrowScale  *extractSmoothScale;
  PXTkOptionMenu  *extractDoSmooth;
  PXTkArrowScale  *extractResampleScale;
  PXTkCheckButton *extractDoResample;

  PXTkCheckButton *appendButton[10];
  PXTkCheckButton *appendObjectMap;
  PXTkOptionMenu  *appendResult;

  PXTkScale       *decimateFactor;
  PXTkCheckButton *preserveTopology;
  
  PXTkScale         *smoothIterations;
  PXTkScale         *smoothRelaxation;
  PXTkCheckButton*  smoothEdgeButton;
  PXTkOptionMenu*   smoothMethod;
  
  PXTkLabel*        extractObjectMapLabel;
  PXTkArrowScale   *objectMapRange[2];
  PXTkOptionMenu   *objectMapValues[3];

  PXTkScale*        featureAngle;
  PXTkCheckButton*  edgeSplitButton;
  PXTkCheckButton*  flipNormalsButton;

  PXTkOptionMenu*   cleanMethod;
  PXTkScale*        cleanTolerance;

  PXTkOptionMenu*   curvatureType;
  PXTkOptionMenu*   curvatureMode;
  PXTkScale*        curvatureScale;
  PXTkScale*        curvatureRange;

  PXTkOptionMenu*   textureMode;
  PXTkOptionMenu*   sphereMode;
  PXTkCheckButton*  textureSeam;
  PXTkCheckButton*  textureCyl;
  PXTkCheckButton*  textureFlipR;
  PXTkCheckButton*  textureFlipS;
  PXTkEntry*        P1x, *P1y, *P1z;
  PXTkEntry*        P2x, *P2y, *P2z;
 
  PXTkOptionMenu* connectType;
  PXTkLabel*      connectPoint;

  PXTkOptionMenu* mapFunctionalType;
  PXTkOptionMenu* mapFunctionalMode;
  PXTkArrowScale*      mapThreshold[2];
  PXTkArrowScale*      mapDistance[2];

  PXTkArrowScale*    displaySize[2];
  


  PXTkArrowScale*      pointsThresholdScale[2];
  PXTkOptionMenu*      pointsThresholdInput;

  
  virtual void InitGraphics();


  virtual int doDecimate(int apply=0);
  virtual int doSmooth(int apply=0);
  virtual int doNormals(int apply=0);

  virtual int doTriangulate(int apply=0);
  virtual int doDelaunay3D(int apply=0);
  virtual int doSubdivision(int apply=0);
  virtual int doClip(int apply=0);
  virtual int doConnect(int apply=0);
  virtual int doClean(int apply=0);
  virtual int doCurvature(int apply=0);
  virtual int doTexture(int apply=0);
  //  virtual int doTransform(int apply=0);
  virtual int doExtract(int apply=0);
  virtual int doMapFunctional(int apply=0);
  virtual int doExtractObjectMap(int apply=0);
  virtual int doChangeDisplaySize(int apply=0);
  virtual int doAppend(int apply=0);
  virtual int doThresholdPoints(int apply=0);

  virtual int doMaterial(int apply=0);
};



#endif   


