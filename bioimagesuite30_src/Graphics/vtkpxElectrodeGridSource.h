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
  Module:    $RCSfile: vtkpxElectrodeGridSource.h,v $
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
// .NAME vtkpxElectrodeGridSource - stores and manipulates a set of electrodes arranged as a grid
// .SECTION Description
// This class stores all the necesary information for an implanted Electrode Grid such as
// width x height (in electrodes e.g. 8x8)
// electrode spacing x,y
// electrode type and dimensions
// grid name
// and finally an array of the actual electrodes
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION See Also
//  vtkPoints vtkPolyData vtkPolyDataSource vtkpxElectrodeSource



#ifndef __vtkpxElectrodeGridSource_h
#define __vtkpxElectrodeGridSource_h

class vtkPoints;
class vtkPolyData;

class vtkPoints;
class vtkPolyData;
class vtkCollection;
class vtkFloatArray;
class vtkAbstractTransform;
#include "vtkpxElectrodeSource.h"
#include "vtkPolyDataSource.h"


class vtkpxElectrodeGridSource : public vtkPolyDataSource 
{
public:
  static vtkpxElectrodeGridSource *New();
  vtkTypeMacro(vtkpxElectrodeGridSource,vtkPolyDataSource);

public:

  // Description:
  // Grid Dimensions
  vtkGetVectorMacro(Dimensions,int,2);

  // Description:
  // Electrode Spacing
  vtkGetVectorMacro(ElectrodeSpacing,float,2);
  vtkSetVectorMacro(ElectrodeSpacing,float,2);


  // Description:
  // Electrode Spacing
  vtkGetVectorMacro(Color,float,3);
  vtkSetVectorMacro(Color,float,3);

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
  // Specify name of the Grid 
  vtkSetStringMacro(GridName);
  vtkGetStringMacro(GridName);

  // Description:
  // Set/Get Current Value Index
  virtual void SetCurrentValueIndex(int a);
  virtual int  GetCurrentValueIndex();


  // Description:
  // Visible -- to enable/disable drawing for ElectrodeGridCollection
  vtkSetClampMacro(Visible,int,0,1);
  vtkGetMacro(Visible,int);
  vtkBooleanMacro(Visible,int);

  // Description:
  // ColorMode = 0 : ForceUniform,  1=Electrode Normal, 2=Electrode Manual
  vtkSetClampMacro(ColorMode,int,0,2);
  vtkGetMacro(ColorMode,int);

  // Description:
  // Visible -- to enable/disable drawing lines for ElectrodeGridCollection
  vtkSetClampMacro(JustShowElectrodes,int,0,1);
  vtkGetMacro(JustShowElectrodes,int);
  vtkBooleanMacro(JustShowElectrodes,int);

  // Description:
  // Visible -- to enable Synchronization of radius/thickness/type during drawing
  vtkSetClampMacro(EnableSynchronization,int,0,1);
  vtkGetMacro(EnableSynchronization,int);
  vtkBooleanMacro(EnableSynchronization,int);

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
  virtual void Copy(vtkpxElectrodeGridSource* other);
  virtual void CopyAttributes(vtkpxElectrodeGridSource* other);

  // Description:
  // Get Individual Electrode
  virtual vtkpxElectrodeSource* GetElectrode(int i,int j);
  virtual vtkpxElectrodeSource* GetElectrode(int index);
  virtual int GetElectrodeRealIndex(int i,int j);
  virtual int GetElectrodeRealIndex(int index);

  // Description:
  // Label Stuff
  virtual int CheckWhetherToShowLabelName(int i,int j);

  vtkSetClampMacro(ShowLabelsGap,int,0,3);
  vtkGetMacro(ShowLabelsGap,int);
  vtkBooleanMacro(ShowLabelsGap,int);



  // Description:
  // Get Distances
  virtual void GetNeighborDistances(int index,vtkFloatArray* array);
  virtual void GetNeighborDistances(int index,vtkFloatArray* array,float x,float y,float z);

  // Description:
  // Initialze Grid
  virtual void InitializeGrid(int dimx,int dimy);
  virtual void TransposeGrid();
  virtual void GridFlipX();
  virtual void GridFlipY();

  // Description:
  // Ensure that all electrodes have the same type and dimensions
  virtual void SynchronizeElectrodeTypeAndDimensions();
  
  // Description:
  // Handle Mouse Event from Viewer (for Editing Purposes)
  virtual int HandleClickedPoint(float x,float y,float z,int state);
  virtual int FindClosestElectrodeIndex(double x[3]);
  virtual int FindClosestElectrodeIndex(float x,float y,float z);

  // Description: 
  // Get Object Stuff
  vtkGetObjectMacro(LabelSurface, vtkPolyData);
  vtkGetObjectMacro(ElectrodeCollection,vtkCollection);

  // Description:
  // Apply Transformation
  virtual int ApplyTransformation(vtkAbstractTransform* tr,int forward=1);

  // Description:
  // Is Electrode Modified
  virtual int IsElectrodeModified(int index);
  virtual int GetNumberOfModifiedElectrodes();
  virtual int AutoWarp();

  // Description:
  // Set Number Of Values for all electrodes
  virtual void SetNumberOfValues(int vl);
  virtual int  GetNumberOfValues();

    
protected:
  vtkpxElectrodeGridSource();
  virtual ~vtkpxElectrodeGridSource();

  vtkpxElectrodeGridSource(const vtkpxElectrodeGridSource&) {};
  void operator=(const vtkpxElectrodeGridSource&) {};

  virtual void Execute();  


  //BTX
  int   Dimensions[2];
  float ElectrodeSpacing[2];
  float Color[3];
  int   ElectrodeType;
  float Radius,Thickness;
  char *GridName;
  vtkCollection*  ElectrodeCollection;
  vtkPolyData*    LabelSurface;

  // Display Only Stuff
  int   Visible;
  int   JustShowElectrodes;
  int   EnableSynchronization;
  int   ShowLabelsGap;
  int   ColorMode;
  //ETX


};

#endif   


