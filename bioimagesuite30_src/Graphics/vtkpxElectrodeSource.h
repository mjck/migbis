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
  Module:    $RCSfile: vtkpxElectrodeSource.h,v $
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
// .NAME vtkpxElectrodeSource - stores the information for a single electrode
// .SECTION Description
// This class stores all the necessary information for a single electrode
// including position and normal (for a disc)
// type (sphere/disc) dimensions r,t
// functional attributes motor,sensory,visual,auditory,language,user1,user2
// electrical attributes seizure onset, spikes
// and finally whether it actually was implanted or not 
// <br> <br> <EM>Copyright (c) 1995-2003 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION See Also
//  vtkPoints vtkPolyData vtkPolyDataSource 


#ifndef __vtkpxElectrodeSource_h
#define __vtkpxElectrodeSource_h

class vtkPoints;
class vtkPolyData;
#include "vtkAbstractTransform.h"
#include "vtkPolyDataSource.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "pxutil.h"

class vtkpxElectrodeSource : public vtkPolyDataSource 
{
public:
  static vtkpxElectrodeSource *New();
  vtkTypeMacro(vtkpxElectrodeSource,vtkPolyDataSource);

public:

  // Description:
  // Get Functional Info
  // Mode Settings
  /*
    Location Alone = 0;
    Epilepsy Map   = 1  (1=spikes present 2=seizure onset)
    Any Functional = 2;
    Motor Function = 3
    Sensory Function = 4
    Visual Function = 5
    Language Function = 6;
    Auditory Function = 7;
    User 1 Function = 8;
    User 2 Function = 9;
  */
  virtual int GetFunctionalInfo(int Mode);

  // Description:
  // Functional Attributes 
  vtkSetClampMacro(ColorMode,int,0,1);
  vtkGetMacro(ColorMode,int);
  virtual void SetColorModeToNormal() { this->SetColorMode(1); }
  virtual void SetColorModeToManual() { this->SetColorMode(0); }

  // Description:
  // Functional Attributes 
  vtkSetClampMacro(MotorFunction,int,0,1);
  vtkGetMacro(MotorFunction,int);
  vtkBooleanMacro(MotorFunction,int);

  vtkSetClampMacro(SensoryFunction,int,0,1);
  vtkGetMacro(SensoryFunction,int);
  vtkBooleanMacro(SensoryFunction,int);

  vtkSetClampMacro(VisualFunction,int,0,1);
  vtkGetMacro(VisualFunction,int);
  vtkBooleanMacro(VisualFunction,int);

  vtkSetClampMacro(AuditoryFunction,int,0,1);
  vtkGetMacro(AuditoryFunction,int);
  vtkBooleanMacro(AuditoryFunction,int);

  vtkSetClampMacro(LanguageFunction,int,0,1);
  vtkGetMacro(LanguageFunction,int);
  vtkBooleanMacro(LanguageFunction,int);

  vtkSetClampMacro(User1Function,int,0,1);
  vtkGetMacro(User1Function,int);
  vtkBooleanMacro(User1Function,int);

  vtkSetClampMacro(User2Function,int,0,1);
  vtkGetMacro(User2Function,int);
  vtkBooleanMacro(User2Function,int);

  // Description:
  // Epilepsy Related Flags
  vtkSetClampMacro(SeizureOnset,int,0,1);
  vtkGetMacro(SeizureOnset,int);
  vtkBooleanMacro(SeizureOnset,int);

  vtkSetClampMacro(SpikesPresent,int,0,1);
  vtkGetMacro(SpikesPresent,int);
  vtkBooleanMacro(SpikesPresent,int);

  // Description:
  // If Current Value !=-1 draw box if current value > 0
  vtkSetClampMacro(CurrentValueIndex,int,-1,20);
  vtkGetMacro(CurrentValueIndex,int);

  // Description:
  // Value -- Continuos Sampling
  vtkGetObjectMacro(Values,vtkFloatArray);
  
  // Description:
  // Number of Tuples in Values (vtkFloatArray Components=1)
  virtual void SetNumberOfValues(int vl);
  virtual int  GetNumberOfValues();


  // Description:
  // Location and Normal
  vtkGetVectorMacro(Position,double,3);
  vtkSetVectorMacro(Position,double,3);

  // Description:
  // Previous Position
  vtkGetVectorMacro(PreviousPosition,double,3);
  vtkSetVectorMacro(PreviousPosition,double,3);
  vtkGetVectorMacro(Normal,double,3);
  vtkSetVectorMacro(Normal,double,3);

  // Description:
  // Marker Type 0=Sphere, 1= Disc
  vtkGetMacro(ElectrodeType,int);
  vtkSetClampMacro(ElectrodeType,int,0,1);
  void SetElectrodeTypeToSphere() { this->SetElectrodeType(0);}
  void SetElectrodeTypeToDisc()   { this->SetElectrodeType(1);}
  
  // Description:
  // Set/Get Radius and Thickness (for disc) between 0.5 and 20mm
  vtkGetMacro(Radius,float);
  vtkSetClampMacro(Radius,float,0.5,20.0);

  vtkGetMacro(Thickness,float);
  vtkSetClampMacro(Thickness,float,0.01,2.0);

  // Description:
  // Is the electrode Present or has it been cut off
  vtkSetClampMacro(ElectrodePresent,int,0,1);
  vtkGetMacro(ElectrodePresent,int);
  vtkBooleanMacro(ElectrodePresent,int);

  // Description:
  // Get Color Name and Other Color Info
  vtkGetStringMacro(ColorName);
  virtual void StoreColorInColorName();
  virtual int ComputeColor();
  vtkGetVectorMacro(Color,float,3);
  vtkSetVectorMacro(Color,float,3);

  // Description:
  // Load/Save Electrode in file
  virtual int Load(const char* fname);
  virtual int Save(const char* fname);

  //BTX
  // Description:
  // Load/Save Electrode from already open file
  virtual int Load(gzFile fin);
  virtual int Save(FILE* fout);
  //ETX

  // Description:
  // Copy Electrode Attributs from another Electrode
  virtual void Copy(vtkpxElectrodeSource* other);
  virtual void CopyAttributes(vtkpxElectrodeSource* other);

  // Description:
  // Reset Default Attributes
  virtual void Reset();

  // Description:
  // Apply Transformation
  virtual int ApplyTransformation(vtkAbstractTransform* tr,int forward=1);

protected:
  vtkpxElectrodeSource();
  virtual ~vtkpxElectrodeSource();

  vtkpxElectrodeSource(const vtkpxElectrodeSource&) {};
  void operator=(const vtkpxElectrodeSource&) {};

  // Description:
  // Enclose attributs in BTX..ETX block to hide them from the TCL parser
  //BTX
  int   MotorFunction,SensoryFunction,VisualFunction,LanguageFunction;
  int   User1Function,User2Function,AuditoryFunction;
  int   SeizureOnset,SpikesPresent;
  double Position[3],Normal[3],PreviousPosition[3];
  int   ElectrodeType,ElectrodePresent;
  float Radius,Thickness;
  char* ColorName;
  float Color[3];
  float InternalColor[3];
  vtkFloatArray* Values;
  int   ColorMode;
  int   CurrentValueIndex;
  //ETX

  // Description:
  // Determine Color for Surface
  //BTX

  //ETX

  // Description:
  // Generate Output Surface
  virtual void Execute();  
};

#endif   


