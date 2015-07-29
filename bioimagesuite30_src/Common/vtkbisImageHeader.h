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





// .NAME vtkbisImageHeader - a class for storing medical image headers (Analyze/NIFTI) 
// .SECTION Description
// This class has functionality for reading/writing and storing a medical image header 
// It it essentially a wrapper around the NIFT1 format with Analyze 7.5 backwards compatibility.
// It also includes code for reformatting images from coronal and sagittal back to axial
//

#ifndef __vtkbisImageHeader_h
#define __vtkbisImageHeader_h

#include "vtkObject.h"
#include "bis_znzlib.h"
#include <string>

class vtkImageData;
class vtkbisNIFTIExtensionsList;
class vtkMatrix4x4;
class vtkXMLDataElement;

// These are NIFTY Things
#include <nifti1.h>
#include <nifti1_io.h>


class vtkbisImageHeader : public vtkObject
{
public:
  static vtkbisImageHeader *New();
  vtkTypeMacro(vtkbisImageHeader,vtkObject);

  // Description:
  // Print Self Diagnostic
  virtual void PrintAll(int show_extensions=1);
  virtual void PrintHeader(int long_mode=1);
  virtual  char* GetHeaderDescription(int long_mode=1);

  // Description:
  // Read Header -- this is generic and will test first NIFTI1 and then Analyze
  virtual int ReadHeader(char* fname);
  virtual int WriteHeader(char* fname,int compression=0);

  // Description:
  // Stuff for Execute Information
  // Get VTK Data Type from Header
  virtual int GetVTKDataType();
  // Simply Ordered from 0 on (from edit menu purposes)
  virtual int GetIndexedDataType();
  virtual void SetIndexedDataType(int ind);
  virtual void GetDimensions(int dim[3]);
  virtual void GetSpacing(double spa[3]);
  virtual int  GetNumberOfComponents();
  
  
  // Description:
  // Save NII Header in already open stream (used for .nii, .nii.gz files)
  virtual int WriteNIFTIHeader(znzFile fout);

  // Description:
  // Read/Write Headers
  int WriteNIFTIHeader(char* filename,int compression);
  int WriteAnalyzeHeader(char* filename);

  // Description:
  // Read Headers
  virtual int ReadNIFTIHeader(char* filename);
  virtual int ReadAnalyzeHeader(char* filename);
  virtual int CreateVarianHeader(vtkXMLDataElement* element);

  // Description:
  // Set All Fields to Default Values
  virtual int ResetFields();

  // Description:
  // Initialize (i.e. Reset Fields)
  virtual void Initialize() { this->ResetFields(); }

  // Description:
  // GetIsHeaderValid() -- diagnostic to test whether a valid header has been read
  // Force this to one for editor
  vtkGetMacro(IsHeaderValid,int);
  vtkSetMacro(IsHeaderValid,int);

  // Description:
  // Get Header Offset -- returns the offset from the start of the image (in case of .nii files)
  vtkGetMacro(HeaderOffset,int);
  vtkGetMacro(SwapBytes,int);
  

  // Description:
  // Static Conversion Functions
  static int  GetNIFTIDataType(int vtkdatatype);
  static int  GetBitPix(int vtkdatatype);
  static int  GetVTKDataType(int niftidatatype);
  
  // Description:
  // Create Header from vtkImageData
  virtual int CreateHeader(vtkImageData* img,int useorigin=0);

  // Description:
  // Often the header is copied from another image that 
  // is in the same space but reampled or truncated in time
  // ReconcileHeader will fix the header for this purpose
  // The assumption is that this has the same orientation!!!
  // Use this with extreme care!
  virtual int ReconcileHeader(vtkImageData* img,int useorigin=0);

  // Description:
  // This is the opposite of the above, the image will get reconciled
  // This will change the spacing and optionally the origin of the image
  virtual int ReconcileImage(vtkImageData* img,int useorigin=0);

  // Description:
  // This will scale the image to be in the new range (multiply by scale and add offset)
  // and change the intensity scale and offset in the header to 1.0 and 0.0 and
  // add a comment in the header to this effect
  // Often will be called on load as part of image reconciliation
  // If needed image will be made float!
  virtual int ReconcileImageIntensityRange(vtkImageData* img);

  // Description:
  // Given a header (raw) created from external software
  // accept as valid and create cached fields
  virtual int MakeStoredHeaderValid();


  // Description:
  // Newer code for copying parts of header
  // these are useful when reslicing images and need to preserve part of the 
  // header from the resliced image and part from the reference
  virtual int CopyExtensionsOnly(vtkbisImageHeader* other);
  virtual int CopyLocationInformationOnly(vtkbisImageHeader* other);


  // Description:
  // Copy Other Header
  virtual int CopyHeader(vtkbisImageHeader* other);
  virtual int CopyNIFTIHeader(const nifti_1_header* header);
  virtual int StoreInNIFTIHeader(nifti_1_header* header);

  // Description:
  // Add Comment -- with and without date stamp
  virtual int AddComment(const char* comment);
  virtual int AddComment(const char* comment,int code);
  
  // Description:
  // Mode: 0=Analyze, 1=NIFTI, 2= NIFTI .nii
  vtkGetMacro(Mode,int);
  virtual int HasFullNIFTIHeader() { return (this->Mode>0);}
  virtual int SetModeToAnalyze() { this->Mode=0; return 1;}
  virtual int SetModeToNIFTI_HDRIMG();
  virtual int SetModeToNIFTI_NII();


  // Description:
  // Analyze Stuff
  // 0 = Axial 1= Coronal 2=Sagittal
  vtkGetMacro(Orientation,int);
  virtual void SetOrientation(int orient);
  virtual void SetOrientationComplex(int orient,int nomatrixops);
  virtual void SetOrientationAsMatrix(mat44 newmat); 

  // Description:
  // More Complex Stuff 
  virtual int ApplyShift(double x,double y,double z);
  virtual int ModifyOrientation(mat44 mapping,int DataFlip[3],int do_transpose);
  virtual int ForceHeaderToBioImageSuiteCoordinatesAndReturnFlips(int dataflip[3],int& transpose);
  virtual int ForceHeaderToBioImageSuiteCoordinatesAndFlipImage(vtkImageData* inout);


  // Description:
  // Even more complex Stuff
  // Given a plane (i.e. 0=XY,1=XZ,2=YZ) and current orientation ,figure out flips for JPEG display 
  // Output is 0 no flips, 1=flipx,2=flipy,3=flipboth,4=transpose,5=transpose+flipx, ....
  virtual int GetJPEGFlip(int Plane);

  // Description:
  // Legacy Analyze Extensions 
  virtual void    GetOrigin(double origin[3]);
  virtual double  GetOriginComponent(int a);
  virtual void    SetOrigin(double origin[3]);
  virtual void    SetOrigin(double x,double y,double z);
  virtual short   GetVOIValue();
  virtual void    SetVOIValue(short a);
  virtual char*   GetDBName();
  virtual void    SetDBName(const char* name);
  virtual const char* GetOrientationCode();

  // Description:
  // Last Filename
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);


  // Description:
  // Usefull Stuff
  virtual void SetImageStructureFromHeader(vtkImageData* img);
  virtual vtkImageData* AllocateImageFromHeaderInfo();

  // BTX
  // Description:
  // Raw NIFTI1 Utilities
  static int CopyHeader(const nifti_1_header* input,nifti_1_header* output);
  static int InitializeHeader(nifti_1_header* header);

  // Description:
  // File Name Utilities
  static znzFile SuffixOpen (const char* path, const char* mode,int complevel=0);
  static int    FindExtension(const char* orig,const char* lookfor,const char* lookfor2=NULL);
  static char*  EatSuffix(const char *orig_filename);
  static char*  CreateFilename(const char* orig_filename,const char* wanted,const char *unwanted);
  static char*  CreateAnalyzeHDRFilename(const char* fname);
  static char*  CreateAnalyzeIMGFilename(const char* fname);

  // Description:
  // Access Raw Geader
  nifti_1_header* GetRawHeader() { return this->Header;}
  vtkbisNIFTIExtensionsList* GetExtensions() { return this->Extensions;}


  // Description:
  // Matrix Utilities in Mat44 for NIFTI Header Stuff argh
  //BTX
  static mat44 NIFTI_CreateOrientationMatrix( nifti_1_header *hdr);
  static const char* NIFTI_GetOrientationString( int ii);
  static const char* NIFTI_GetOrientationStringShort( int ii);


  static mat44 NIFTI_mat44_mmul( mat44 A , mat44 B );  /* multiply 2 4x4 matrices */
  static void NIFTI_print_mat44(char* s, mat44 A);
  static void NIFTI_identity_mat44(mat44* A);
  static mat44 NIFTI_Copy_Matrix4x4_mat44(vtkMatrix4x4* input);
  static vtkMatrix4x4* NIFTI_CopyMat44(mat44 input);

  
  // Description:
  // Apply Mapping
  virtual int ApplyMapping(mat44 mapping,int premultiply=0);
  //ETX


  // Description:
  // Map Nw Style Orientations to Old Style and back
  // New Style Axial=10:17, Coronal: 20-27, Sagital 30:37
  //           Transposed Axial 40:47
  //           Transposed Coronal 50:57
  //           Transposed Sagittal 60:67
  // Old Style Axial =0, coronal =1 , sagittal = 2
  static int MapOrientationToNewStyle(int oldorient);
  static int MapOrientationToOldStyle(int neworient);
  static char OrientationToAxisName(int neworient,int axis);

  // Description:
  // Intensity Scale and Offset
  virtual double GetIntensityScale();
  virtual double GetIntensityOffset();

protected:

  vtkbisImageHeader();
  virtual ~vtkbisImageHeader();
  vtkbisImageHeader(const vtkbisImageHeader&) {};
  void operator=(const vtkbisImageHeader&) {};

  // Description:
  // Mode (0=Analyze,1 =NIFT1)
  int Mode;

  // Description:
  // Filename last used
  char* FileName;

  // Description:
  // Header Stuf
  nifti_1_header* Header;
  vtkbisNIFTIExtensionsList* Extensions;

  // Description:
  // Other Flags
  int IsHeaderValid;
  int HeaderOffset;
  int SwapBytes;


  // Description:
  // Orientation:
  int Orientation;

  // Buffer to Keep the Origin in
  double OriginBuffer[3];


  
  // Description:
  // Get Orientation
  virtual int GetOrientationFromNIFTIHeader();

  // Description:
  // Buffer String
  //BTX
  std::string Buffer;
  //ETX
};
#endif

