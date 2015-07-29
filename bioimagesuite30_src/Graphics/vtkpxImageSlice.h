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
  Module:    $RCSfile: vtkpxImageSlice.h,v $
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

// .NAME vtkpxImageSlice - create hierarchies of actors
// .SECTION Description
#ifndef __vtkpxImageSlice_h
#define __vtkpxImageSlice_h

#include "vtkpxEnclosedPipeline.h"
#include "vtkImageData.h"
#include "vtkpxImageExtract.h"
#include "vtkLookupTable.h"
#include "vtkPlaneSource.h"
#include "vtkTexture.h"
#include "vtkCubeSource.h"
#include "vtkProperty.h"

class vtkpxImageSlice : public vtkpxEnclosedPipeline
{
public:
  static vtkpxImageSlice *New();
  vtkTypeMacro(vtkpxImageSlice,vtkpxEnclosedPipeline);

  // Description:
  // Add a part to the list of parts.

  virtual void SetInput(vtkImageData* image);


  virtual void SetLookupTable(vtkLookupTable* table);
  virtual void SetInterpolation(int on);
  virtual void SetOpacity(float opacity);
  virtual void SetLevel(int level);
  virtual void SetFrame(int frame);
  virtual void SetCurrentPlane(int CurrentPlane);

  // If 1 -> Ignore lookup table and render directly
  virtual void SetColorMode(int mode);

  virtual void SetPolarMode(int mode);
  virtual void SetPolarMiddle(float polarmiddle=-1);


  virtual void SetDisplayMode(int md);

  virtual void SetDisplayHighlight(int hg);
  virtual void SetHighlightBounds(int b1,int b2,int b3,int b4,int b5,int b6);

  vtkGetMacro(CurrentPlane,int);
  vtkGetMacro(Level,int);
  vtkGetMacro(Frame,int);
  vtkGetMacro(NumberOfFrames,int);

  vtkGetMacro(DisplayMode,int);
  vtkGetMacro(Opacity,float);
  vtkGetMacro(Interpolation,int);

  vtkGetMacro(OwnsColorMap,int);

  vtkGetMacro(PolarMode,int);
  vtkGetMacro(PolarMiddle,float);


  vtkSetClampMacro(AutoUpdate,int,0,1);
  vtkGetMacro(AutoUpdate,int);
  vtkBooleanMacro(AutoUpdate,int);


  virtual void Update() { this->UpdateSlice();}
  virtual void UpdateTexture();

protected:

  vtkActor*                    ImageSliceHighlight;
  vtkActor*                    ImageSliceOutline;
  vtkActor*                    ImageSlice;
  vtkPlaneSource*              ImagePlane;
  vtkPlaneSource*              ImageHighlightPlane;
  vtkCubeSource*               ImageHighlightCube;
  vtkpxImageExtract*           ImageVOI;
  vtkImageData*                CurrentImage;
  vtkTexture*                  ImageTexture;
  int                          Level;

  int                          Frame;
  int                          NumberOfFrames;

  float                        OutlineBounds[6];
  int                          CurrentPlane;

  float                        Opacity;
  int                          Interpolation;
  int                          DisplayMode;
  int                          ShowHighlight;
  int                          AutoUpdate;

  vtkLookupTable*              ColorMap;
  int                          OwnsColorMap;   
  int                          ColorMode;

  int                          PolarMode;
  float                        PolarMiddle;

  vtkpxImageSlice();
  virtual ~vtkpxImageSlice();
  vtkpxImageSlice(const vtkpxImageSlice&) {};
  void operator=(const vtkpxImageSlice&) {};

  // Slice Stuff
  virtual void BuildImageSlice();
  virtual void UpdateSlice();
  virtual void UpdateSlicePolar();


};


#endif





