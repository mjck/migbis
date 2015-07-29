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
  Module:    $RCSfile: vtkpxObliqueImageSlice.h,v $
  Language:  C++
  Date:      $Date: 2004/03/19 14:14:45 $
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

// .NAME vtkpxObliqueImageSlice - create hierarchies of actors
// .SECTION Description
#ifndef __vtkpxObliqueImageSlice_h
#define __vtkpxObliqueImageSlice_h

#include "vtkpxEnclosedPipeline.h"
#include "vtkImageData.h"


#include "vtkbisImageReslice.h"
#include "vtkTransform.h"
#include "vtkTexture.h"
#include "vtkLookupTable.h"
#include "vtkPlaneSource.h"
#include "vtkCubeSource.h"
#include "vtkProperty.h"
#include "vtkPolyDataMapper.h"
#include "vtkCamera.h"
#include "vtkImageExtractComponents.h"
#include "vtkConeSource.h"

class vtkpxObliqueImageSlice : public vtkpxEnclosedPipeline
{
public:
  static vtkpxObliqueImageSlice *New();
  vtkTypeMacro(vtkpxObliqueImageSlice,vtkpxEnclosedPipeline);

  // Description:
  // Add a part to the list of parts.

  virtual void SetInput(vtkImageData* image);
  virtual void SetLookupTable(vtkLookupTable* table);

  virtual void SetInterpolation(int on);
  virtual void SetOpacity(double opacity);
  virtual void SetFrame(int frame);
  virtual void SetResolution(int frame);

  // If 1 -> Ignore lookup table and render directly
  virtual void SetColorMode(int mode);

  virtual void UpdateImagePlane(double dist,double n1,double n2,double n3);
  virtual void UpdateImagePlane(double x1,double y1,double z1,
				double n1,double n2,double n3);
  virtual void UpdateImagePlane(vtkCamera* camera,int mode=0);
  
  virtual void SetDisplayMode(int md);

  virtual vtkImageData* GetMaskImage(vtkImageData* input,int above,int binary);

  // Description:
  // Internal Stuff
  virtual vtkImageData* GetObliqueImage();
  virtual vtkTransform* GetObliqueTransform();

  vtkGetMacro(Frame,int);
  vtkGetMacro(NumberOfFrames,int);

  vtkGetMacro(DisplayMode,int);
  vtkGetMacro(Opacity,double);
  vtkGetMacro(Interpolation,int);

  vtkGetObjectMacro(ImagePlane,vtkPlaneSource);
  vtkGetObjectMacro(InputImage,vtkImageData);

  
  vtkGetMacro(OwnsColorMap,int);

  vtkSetClampMacro(AutoUpdate,int,0,1);
  vtkGetMacro(AutoUpdate,int);
  vtkBooleanMacro(AutoUpdate,int);

  virtual void SetShowNormalArrow(int a);
  vtkGetMacro(ShowNormalArrow,int);

  vtkSetClampMacro(NormalArrowColor,int,0,1);
  vtkGetMacro(NormalArrowColor,int);


  virtual void Update() { this->UpdateSlice();}
  virtual void UpdateTexture();
  virtual void Reset();

  virtual void GetImageRange(float bounds[6]);

  vtkGetMacro(Distance,double);
  vtkGetVectorMacro(Center,double,3);
  vtkGetVectorMacro(Normal,double,3);

  

protected:

  vtkActor*                    ImageSliceOutline;
  vtkActor*                    ImageSlice;
  vtkActor*                    NormalArrow;
  int                          NormalArrowColor;
  vtkPlaneSource*              ImagePlane;


  vtkImageExtractComponents*   ImageComponentExtractor;
  vtkPolyDataMapper*           ImageMapper;
  vtkConeSource*               ArrowSource;

  vtkTexture*                  ImageTexture;
  double                       BaseImageOrigin[3];
  double                       BaseImageSpacing[3];
  int                          BaseImageExtent[6];


  vtkbisImageReslice*             BaseImageReslicer;
  vtkTransform*                BaseImageTransform;
  vtkImageData*                InputImage;

  int                          Frame;
  int                          Resolution;
  int                          NumberOfFrames;

  float                        OutlineBounds[6];
  vtkPoints*                   InitialPoints;

  double                       Opacity;
  int                          Interpolation;
  int                          DisplayMode;
  int                          AutoUpdate;
  int                          ShowNormalArrow;

  vtkLookupTable*              ColorMap;
  int                          OwnsColorMap;   
  int                          ColorMode;

  double                        Center[3];
  double                        Normal[3];
  double                        Distance;

  vtkpxObliqueImageSlice();
  virtual ~vtkpxObliqueImageSlice();
  vtkpxObliqueImageSlice(const vtkpxObliqueImageSlice&) {};
  void operator=(const vtkpxObliqueImageSlice&) {};

  // Slice Stuff
  virtual void BuildImageSlice();
  virtual void UpdateSlice();

  virtual void InternalUpdatePlane();

};


#endif





