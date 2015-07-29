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





// .NAME vtkbisNIFTIExtensionsList - a class for storing NIFTI Extensions -- replaces/converts the c code in nifty_io.c 

#ifndef __vtkbisNIFTIExtensionsList_h
#define __vtkbisNIFTIExtensionsList_h

#include "vtkObject.h"
#include "vtkCollection.h"
#include "bis_znzlib.h"
#include <stdio.h>


class  vtkbisNIFTIExtension;

class vtkbisNIFTIExtensionsList : public vtkObject
{
public:
  static vtkbisNIFTIExtensionsList *New();
  vtkTypeMacro(vtkbisNIFTIExtensionsList,vtkObject);


  // Description:
  // Print Self Diagnostic
  virtual void PrintAll();

  // Description:
  // Read Header -- this is generic and will test first NIFTI1 and then Analyze
  virtual int ReadExtensions(znzFile fin,int remain);

  // Description:
  // Save NII Header in already open stream (used for .nii, .nii.gz files)
  virtual int WriteExtensions(znzFile fdf);

  // Description:
  // Add Comment -- 
  virtual int AddExtension(const char* comment,int ecode);

  // Description:
  // Get Extension
  virtual const char* GetExtension(int index);

  // Description:
  // Get Number of Extensions
  virtual int GetNumberOfExtensions();
  
  // Description:
  virtual int Copy(vtkbisNIFTIExtensionsList* other);

  // Description:
  // Reset
  virtual int Cleanup();

  // Description:
  // GetRawExtension
  vtkbisNIFTIExtension* GetRawExtension(int i);

  // Description:
  // Get Total Byte Size
  int GetTotalByteSize();

protected:

  vtkbisNIFTIExtensionsList();
  virtual ~vtkbisNIFTIExtensionsList();
  vtkbisNIFTIExtensionsList(const vtkbisNIFTIExtensionsList&) {};
  void operator=(const vtkbisNIFTIExtensionsList&) {};
  
  // This stores all extensions;
  vtkCollection* ExtensionsCollection;


  
};
#endif


