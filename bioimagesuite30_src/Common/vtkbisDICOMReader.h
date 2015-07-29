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

// .NAME vtkDICOMImageReader - Reads DICOM images
// .SECTION Description
// .SECTION See Also
// vtkBMPReader vtkPNMReader vtkTIFFReader

#ifndef __vtkbisDICOMReader_h
#define __vtkbisDICOMReader_h
#include "vtkbisImageHeader.h"


#include "vtkbisGDCMImageReader.h"
#include "vtkStdString.h"

class vtkbisDICOMReader : public vtkbisGDCMImageReader
{
 public:
  // Description:
  // Static method for construction.
  static vtkbisDICOMReader *New();
  vtkTypeRevisionMacro(vtkbisDICOMReader,vtkbisGDCMImageReader);

  // Reference Time
  // Used in PET Studies
  vtkGetStringMacro(ReferenceTime);
  vtkSetStringMacro(ReferenceTime);

  // UniqueIDs
  vtkSetStringMacro(StudyUID);
  vtkGetStringMacro(StudyUID);
  vtkSetStringMacro(SeriesUID);
  vtkGetStringMacro(SeriesUID);
  vtkSetStringMacro(PName);
  vtkGetStringMacro(PName);


  // ZPosition -- distance along z-axis of position
  // Used to sort slices to form right-handed 3D Image
  vtkGetMacro(ZPosition,double);
  vtkGetMacro(X0,double);
  vtkGetMacro(Y0,double);
  vtkGetMacro(Z0,double);

  // This is for fMRI
  vtkGetMacro(TimeFrame,double);
  vtkSetMacro(TimeFrame,double);

  // Description:
  // If On a vtkbisHeader is Created
  vtkSetClampMacro(CreateNIFTIHeader,int,0,1);
  vtkGetMacro(CreateNIFTIHeader,int);
  vtkBooleanMacro(CreateNIFTIHeader,int);

  // Description:
  // If On Extensions will be added to NIFTI Header to store some DICOM Tags
  vtkSetClampMacro(AddExtensions,int,0,1);
  vtkGetMacro(AddExtensions,int);
  vtkBooleanMacro(AddExtensions,int);

  vtkSetClampMacro(FlipZ,int,0,1);
  vtkGetMacro(FlipZ,int);
  vtkBooleanMacro(FlipZ,int);

  // fMRI Stuff
  vtkGetMacro(IsMosaic,int);
  vtkGetMacro(MosaicWidth,int);
  vtkGetMacro(MosaicHeight,int);
  vtkGetMacro(MosaicDepth,int);


  // fMRI Stuff
  // DWIMode = 0 no, 1 = baseline DTI, 2= weighted DTI
  vtkGetMacro(DWIMode,int);
  vtkGetMacro(DWIBValue,double);
  vtkGetVectorMacro(DWIDirections,double,3);


  // Description:
  // Image Header
  vtkGetObjectMacro(ImageHeader,vtkbisImageHeader);

protected:
  
  vtkbisDICOMReader();
  virtual ~vtkbisDICOMReader();

  char* ReferenceTime;
  char* StudyUID;
  char* SeriesUID;
  char* PName;
  int   CreateNIFTIHeader;
  int   AddExtensions;
  double ZPosition;
  double X0;
  double Y0;
  double Z0;

  double TimeFrame;
  int FlipZ;
  int IsMosaic;
  int MosaicWidth;
  int MosaicHeight;
  int MosaicDepth;
  int DWIMode;
  double DWIDirections[3];
  double DWIBValue;
  vtkbisImageHeader* ImageHeader;

  // Use this as a place holder to get more data
  //BTX
  virtual void FillMedicalImageInformation(const gdcm::ImageReader &reader);
  //ETX
};

#endif

