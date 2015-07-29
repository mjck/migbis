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
  Module:    $RCSfile: vtkpxTalairachTransform.h,v $
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

 ========================================================================*/

#ifndef __vtkpxTalairachTransform_h
#define __vtkpxTalairachTransform_h

#include "vtkWarpTransform.h"

class vtkTransform;
class vtkPolyData;
class vtkImageData;
class vtkbisImageReslice;



class vtkpxTalairachTransform : public vtkWarpTransform
{
public:
  vtkTypeMacro(vtkpxTalairachTransform,vtkWarpTransform);
  void PrintSelf(ostream& os, vtkIndent indent);
  static vtkpxTalairachTransform *New();

  // Description:
  // Set the source landmarks for the warp.  If you add or change the
  // vtkPoints object, you must call Modified() on it or the transformation
  // might not update.
  // Point 0 = AC , Point 1 = PC , Points 2-7 Define Bounds 
  virtual void SetLandmarks(vtkPoints *source);
  vtkGetObjectMacro(Landmarks,vtkPoints);


  // Description:
  // Get the MTime.
  unsigned long GetMTime();

  // Description:
  // Make another transform of the same type.
  vtkAbstractTransform *MakeTransform();

  // Description:
  // Generate a talarach cube 
  virtual vtkPolyData* GetTalairachCube(int inverse=1);
  
  // Description:
  // Reslice image to standard image (MNI?) given this talairach transform 
  vtkImageData* ResliceImage(vtkImageData* image,int linearOnly=0);

  // Description:
  // Reslice image as above but with given vtkbisImageReslice (allows for ProgressMethod stuff)
  vtkImageData* ResliceImage(vtkImageData* image,int linearOnly,vtkbisImageReslice* resl);

  // Description:
  // Define Talairach Atlas in mm
  vtkSetMacro(AC_FRONT,double);
  vtkGetMacro(AC_FRONT,double);
  vtkSetMacro(PC_AC,double);
  vtkGetMacro(PC_AC,double);
  vtkSetMacro(BACK_PC,double);
  vtkGetMacro(BACK_PC,double);
  vtkSetMacro(RIGHT_AC,double);
  vtkGetMacro(RIGHT_AC,double);
  vtkSetMacro(AC_LEFT,double);
  vtkGetMacro(AC_LEFT,double);
  vtkSetMacro(BOTTOM_AC,double);
  vtkGetMacro(BOTTOM_AC,double);
  vtkSetMacro(AC_TOP,double);
  vtkGetMacro(AC_TOP,double);

  // Description:
  // Define offset of image boundaries to talaraich cube
  vtkSetMacro(Offset,int);
  vtkGetMacro(Offset,int);

  // Description:
  // Define offset of image boundaries to talaraich cube
  vtkSetMacro(AdditionalZOffset,int);
  vtkGetMacro(AdditionalZOffset,int);

  // Description:
  // If StandardMode = 1 then image has been resliced to Talairach Axis etc.
  vtkGetMacro(StandardMode,int);
  virtual void SetStandardModeOn();
    
  // Description:
  // Get Linear Transform
  vtkTransform* GetLinearTransform() 
  { return this->LinearTransform;}

  // Description:
  // Save & Load Transform into .tal file
  int Load(const char* fname);
  int Save(const char* fname);


  // Description:
  // Set To MNI Brain Coordinates
  void SetMontrealMode(int yaxisfronttoback=1);

  // Description:
  // Colin Mode = 0  : not using the colin brain
  // Colin Mode = 1  : Colin Brain yaxis front to back.
  // Colin Mode = -1 : Colin Brain yaxis back to front.
  vtkGetMacro(ColinMode,int);
  vtkGetMacro(ColinYSize,int);
  void SetColinMode(int mode,vtkImageData* img);

  // Description:
  // Get MNI Coordinates
  virtual int GetMNICoordinates(double x[3],double mni[3]);

  // Description:
  // Map Coordinates from different orientations
  virtual int ReorientCoordinates(vtkImageData* img, int inputorientation,double x[3],double y[3]);

  // Description:
  // Generates a Lookup Table from the Current Transformation
  virtual vtkImageData* GenerateLookupTable(vtkImageData* img,vtkAbstractTransform* tr=NULL);

  // Description:
  // Set Colin Lookup Table Name
  virtual int SetColinLookupFileName(char* fname,int mode=0);

protected:
  vtkpxTalairachTransform();
  ~vtkpxTalairachTransform();
  vtkpxTalairachTransform(const vtkpxTalairachTransform&) {};
  void operator=(const vtkpxTalairachTransform&) {};

  // Description:
  // Prepare the transformation for application.
  void InternalUpdate();

  // Description:
  // This method does no type checking, use DeepCopy instead.
  void InternalDeepCopy(vtkAbstractTransform *transform);

  // Description:
  // Forward Transform Point -- OK
  void ForwardTransformPoint(const float in[3], float out[3]);
  void ForwardTransformPoint(const double in[3], double out[3]);

  // Description:
  // Forward Transform Derivative (Not done yet) -- OK
  void ForwardTransformDerivative(const float in[3], float out[3],
				  float derivative[3][3]);
  void ForwardTransformDerivative(const double in[3], double out[3],
				  double derivative[3][3]);


  // Description:
  // If the InverseFlag is set to 1, then a call to InternalTransformPoint
  // results in a call to InverseTransformPoint.  The inverse transformation
  // is calculated from using Newton's method.
  virtual void InverseTransformPoint(const float in[3], float out[3]);
  virtual void InverseTransformPoint(const double in[3], double out[3]);

  vtkPoints *Landmarks;
  vtkTransform* LinearTransform;

  //BTX
  double minc[3],maxc[3],ac[3],pc[3];
  //ETX

  // Description:
  // TalairachParameters --- Anterior to Posterior 
  double AC_FRONT;
  double PC_AC;
  double BACK_PC;
  
  // Description:
  // Talairach Parameters --- Right to Left
  double RIGHT_AC;
  double AC_LEFT;

  // Description:
  // Talairach Parameters -- Bottom to Bot
  double BOTTOM_AC;
  double AC_TOP;

  // Description:
  // Reslice Offset -- add extra boundary around
  int Offset;
  int AdditionalZOffset;
  int StandardMode;

  // Colin Mode, 0=No,1=Yes Y=Front-to-Back,-1=Yes Y=Back to Front
  int ColinMode;
  int ColinYSize;

  
  //BTX
  double scalex0,scalex1,scaley0,scaley1,scaley2,scalez0,scalez1;
  void SetScalingConstants();
  //ETX
};

//----------------------------------------------------------------------------

#endif






