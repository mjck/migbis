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





#ifndef __vtkpxAnalyzeImageSourceOld
#define __vtkpxAnalyzeImageSourceOld

#include <stdio.h>
#include "vtkImageSource.h"
#include "pxutil.h"
#include "vtkConfigure.h"
#include "vtkImageData.h"

class vtkDataArray;

class vtkpxAnalyzeImageSourceOld : public vtkImageSource
{
public:
  vtkTypeMacro(vtkpxAnalyzeImageSourceOld,vtkImageSource);
  static vtkpxAnalyzeImageSourceOld* New();

  // Description:
  // Convenience Routine for analyze images -- simply use Load("fname.hdr")
  virtual int  Load(const char* fname);

  // Description:
  // Reads Header first set using SetFilePrefix
  virtual int  ReadHeaderFile();

  // Description:
  // Set HeaderInfo manually for binary files
  virtual void SetHeaderInfo(int width,int height,int depth,int swap,int bytes);
  virtual void SetHeaderInfo(int width,int height,int depth,int nframes,int swap,int bytes);
  virtual void SetVoxelDimensions(float vox1,float vox2,float vox3);

  // Description:
  // Set Number of TimeFrames for binary files (in case of 4D Images)
  virtual void SetNumTimeFrames(int timeframes);

  // Description:
  // In case of 4D Images returns number of timeframes
  virtual int  GetNumTimeFrames();

  // Description:
  // In case of 4D Images returns number of slices/frame
  virtual int  GetNumSlices();

  // Description:
  // Get the orientation from analyze header 0=axial 1=coronal 2=saggital (default=0) 3=polar (ultrasound)
  vtkGetMacro(Orientation,int);
  vtkSetMacro(Orientation,int);

  // Description:
  // Get the file type 1=bit 2=short 3=unsigned int 4=float
  vtkGetMacro(FileType,int);
  vtkSetClampMacro(FileType,int,1,4);


  // Description:
  // Force the output scalars to have type short
  vtkGetMacro(ForceOutputToShort,int);
  vtkSetMacro(ForceOutputToShort,int);
  vtkBooleanMacro(ForceOutputToShort,int);

  // Description:
  // SaveMultiple Frames As Components 
  vtkGetMacro(MultipleFramesAsComponents,int);
  vtkSetClampMacro(MultipleFramesAsComponents,int,0,1);
  vtkBooleanMacro(MultipleFramesAsComponents, int);

  // Description:
  // Specify the dimensions for the data.
  vtkSetVector2Macro(DataDimensions,int);
  vtkGetVectorMacro(DataDimensions,int,2);

  // Description:
  // Specify the number of bytes to seek over at start of image.
  vtkSetMacro(HeaderSize,int);
  vtkGetMacro(HeaderSize,int);

  // Description:
  // Specify file prefix for the image file(s).
  vtkSetStringMacro(FilePrefix);
  vtkGetStringMacro(FilePrefix);

  // Description:
  // The sprintf format used to build filename from FilePrefix and number.
  vtkSetStringMacro(FilePattern);
  vtkGetStringMacro(FilePattern);

  // Description:
  // Set the range of files to read.
  vtkSetVector2Macro(ImageRange,int);
  vtkGetVectorMacro(ImageRange,int,2);

  // Description:
  // Specify the spacing for the data.
  vtkSetVector3Macro(DataSpacing,double);
  vtkGetVectorMacro(DataSpacing,double,3);

  // Description:
  // Specify the origin for the data.
  vtkSetVector3Macro(DataOrigin,double);
  vtkGetVectorMacro(DataOrigin,double,3);

  // Description:
  // Turn on/off byte swapping.
  vtkSetMacro(SwapBytes,int);
  vtkGetMacro(SwapBytes,int);
  vtkBooleanMacro(SwapBytes,int);


  // Pipeline Stuff
  virtual int FillOutputPortInformation(int port, vtkInformation* info);
  

protected:
  //BTX
  int read_volume_ok;
  int FileType;
  int AnalyzeMode;
  int NumTimeFrames;
  int Orientation;
  int ForceOutputToShort;
  int MultipleFramesAsComponents;

  int  DataDimensions[2];
  int   SwapBytes,HeaderSize;


  char *FilePrefix;
  char *FilePattern;
  int ImageRange[2];
  double DataSpacing[3];
  double DataOrigin[3];


  vtkpxAnalyzeImageSourceOld();
  virtual ~vtkpxAnalyzeImageSourceOld();
  //ETX

  virtual void Execute();
  virtual void ExecuteInformation();

  //BTX
  virtual vtkDataArray* ReadVolume(int FirstImage, int LastImage);
  int ReadBinaryImage(gzFile fp,  short *pixels, int xsize, int ysize,  
		      int skip, int swapBytes);

  int ReadCharBinaryImage(gzFile fp,  char *pixels, int xsize, int ysize,  
		      int skip);

  int ReadIntBinaryImage(gzFile fp,  int *pixels, int xsize, int ysize,  
			 int skip, int swapBytes);
  
  int ReadFloatBinaryImage(gzFile fp,  float *pixels, int xsize, int ysize,  
			   int skip, int swapBytes);
  //ETX
};

#endif



