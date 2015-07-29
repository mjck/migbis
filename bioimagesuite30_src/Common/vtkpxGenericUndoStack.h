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





#ifndef __vtkpxGenericStack_h
#define __vtkpxGenericStack_h

#include "vtkObject.h"

const int pxGenericArrayMAXSIZE = 100;

class vtkpxGenericUndoStack : public vtkObject {
  
public:
  vtkTypeMacro(vtkpxGenericUndoStack,vtkObject);

  // Description:
  // Initialize
  virtual void Initialize(int numelements);
  virtual void Reset();

  // Description:
  // Insert Stuff
  virtual vtkObject* GetNextObject();
  virtual vtkObject* GetCurrentObject();

  virtual vtkObject* GetNextUndoObject();
  virtual vtkObject* GetNextRedoObject();

  // Description:
  // Information
  vtkGetMacro(NumberOfElements,int);

  
  // Description:
  // Diagnostic Stuff
  vtkGetMacro(CurrentIndex,int);
  vtkGetMacro(CurrentUndoIndex,int);
  vtkGetMacro(CurrentRedoIndex,int);
  vtkGetMacro(MaxIndex,int);
  vtkGetMacro(CurrentlyUndoing,int);
  vtkGetMacro(CurrentlyRedoing,int);

protected:
  vtkpxGenericUndoStack();
  virtual ~vtkpxGenericUndoStack();



  // Description:
  // Data Members
  int               CurrentIndex;
  int               CurrentUndoIndex;
  int               CurrentRedoIndex;
  int               CurrentlyUndoing;
  int               CurrentlyRedoing;
  int               NumberOfElements;
  int               MaxIndex;
  vtkObject*        Elements[pxGenericArrayMAXSIZE];

  // Description:
  // Do Shuffle
  virtual void ShuffleElements();

  // Description:
  // Protected Stuff
  virtual vtkObject* CreateNewElement() = 0;
  virtual void InitializeElement(int index) = 0;
  
};
#endif

