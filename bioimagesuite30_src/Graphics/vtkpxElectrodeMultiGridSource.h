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
  Module:    $RCSfile: vtkpxElectrodeMultiGridSource.h,v $
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
// .NAME vtkpxElectrodeMultiGridSource - stores and manipulates a set of electrodes arranged as a grid
// .SECTION Description
// This class stores all the necesary information for a collection
// of electrode grids as implanted in a patient e.g.
// number of grids
// patient name
// patient id
// and finally an array of the actual electrodegrods
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION See Also
//  vtkPoints vtkPolyData vtkPolyDataSource vtkpxElectrodeGridSource vtkpxElectrodeSource



#ifndef __vtkpxElectrodeMultiGridSource_h
#define __vtkpxElectrodeMultiGridSource_h

class vtkPoints;
class vtkPolyData;
class vtkImageData;
class vtkPoints;
class vtkPolyData;
class vtkCollection;
class vtkAbstractTransform;
class vtkLookupTable;
#include "vtkpxElectrodeGridSource.h"
#include "vtkPolyDataSource.h"



class vtkpxElectrodeMultiGridSource : public vtkPolyDataSource 
{
public:
  static vtkpxElectrodeMultiGridSource *New();
  vtkTypeMacro(vtkpxElectrodeMultiGridSource,vtkPolyDataSource);

public:
  
  // Description:
  virtual int GetNumberOfGrids();

  // Description:
  // Specify Description
  vtkSetStringMacro(Description);
  vtkGetStringMacro(Description);

  // Description:
  // Specify additional comment
  vtkSetStringMacro(Comment);
  vtkGetStringMacro(Comment);

    // Description:
  // Load/Save Electrode in file
  virtual int Load(const char* fname);
  virtual int Save(const char* fname);
  virtual int ExportValues(const char* fname,const char* description,const char* column,const char* columnnames);

  //BTX
  // Description:
  // Load/Save Electrode from already open file
  virtual int Load(gzFile fin);
  virtual int Save(FILE* fout);
  //ETX


  // Description:
  // Copy From Old Grid 
  virtual void Copy(vtkpxElectrodeMultiGridSource* other);
  virtual void CopyAttributes(vtkpxElectrodeMultiGridSource* other);

  // Description:
  // Get Individual Electrode
  virtual vtkpxElectrodeGridSource* GetElectrodeGrid(int i);

  // Description:
  // Initialze Grid
  virtual void InitializeMultiGrid(int numgrids);

  // Description:
  // Insert New Grid
  virtual void InsertNewGrid();

  // Description:
  // Delete Grid
  virtual void DeleteGrid(int index);

  // Description:
  // Export to Single Points Surface for Reconstruction Purposes
  // Mode = Functional Mode (see vtkpxElectrode::GetFunctionalInfo(Mode))
  virtual vtkPolyData* ExportToPolyDataAsPoints(int Mode);
  virtual vtkPolyData* ExportToPolyDataAsPoints(int Mode,float radius);

  // Description:
  // Create Image where number is 1 for electrode present 0 absent
  virtual vtkImageData* CreateSinglePixelImage(vtkDataArray* ModeArray,vtkImageData* baseimage);

  // Description:
  // Get Labels Surface
  vtkGetObjectMacro(LabelSurface, vtkPolyData);

  // Description:
  // Show All Labels
  virtual void ShowLabelsGap(int all=0);


  // Description:
  // Apply Transformation
  virtual int ApplyTransformation(vtkAbstractTransform* tr,int forward=1);

  // Description:
  // Apply Transformation
  virtual int PickFunction(vtkImageData* img);
  virtual int SetElectrodeColorsFromFunction(vtkLookupTable* cmap);

  // Description:
  // Set Number Of Values for all electrodes
  virtual void SetNumberOfValues(int vl);
  virtual int  GetNumberOfValues();

  // Description:
  // Set/Get Current Value Index
  virtual void SetCurrentValueIndex(int a);
  virtual int  GetCurrentValueIndex();

  
protected:
  vtkpxElectrodeMultiGridSource();
  virtual ~vtkpxElectrodeMultiGridSource();

  vtkpxElectrodeMultiGridSource(const vtkpxElectrodeMultiGridSource&) {};
  void operator=(const vtkpxElectrodeMultiGridSource&) {};

  virtual void Execute();  


  //BTX
  char *Description;
  char *Comment;
  vtkCollection*  ElectrodeGridCollection;
  vtkPolyData* LabelSurface;
  //ETX


};

#endif   


