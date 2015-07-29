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





#ifndef __vtkafBVImageReader
#define __vtkafBVImageReader

#include <stdio.h>
#include "vtkStructuredPointsReader.h"
#include "pxutil.h"

//BTX
class vtkDataArray;
//ETX

class vtkafBVImageReader : public vtkStructuredPointsReader
{

public:
  vtkTypeMacro(vtkafBVImageReader,vtkStructuredPointsReader);
  static vtkafBVImageReader* New();
  
  // Description:
  // Convenience Routine for analyze images -- simply use Load("fname.hdr")
  virtual int  Load(const char* fname);

  // Description:
  // Reads Header first set using SetFilePrefix
  virtual int  ReadHeaderInfo();

  // Description:
  // Get the orientation from analyze header 0=axial 1=coronal 2=saggital (default=2)
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

  //the following six functions are parameters for the relative positions with a VMR volume (must be 256^3 data set
  //for Talaraich VTC data

  // Set/Get relative positions
  vtkGetMacro(XStart,short int);

  // Get/Set relative positions
  vtkGetMacro(XEnd,short int);

  // Get/Set relative positions
  vtkGetMacro(YEnd,short int);

  // Get/Set relative positions
  vtkGetMacro(ZEnd,short int);

  // Get/Set relative positions
  vtkGetMacro(YStart,short int);

 // Description:
  // Get/Set relative positions
  vtkGetMacro(ZStart,short int);

  // Set/Get VTC-resolution 
  vtkGetMacro(resolution,short int);

  // Set/Get Hemodynamic Delay, simple shift value
  vtkGetMacro(hemodynamicDelay,short int);

  // Set/Get TR (ms)
  vtkGetMacro(TR,float);

  // Set/Get delta Parameter in the Hemodynamic function
  vtkGetMacro(deltaParam,float);

  // Set/Get tau Parameter in the Hemodynamic function
  vtkGetMacro(tauParam,float);

  // Set/Get Segment size ,used for time course separation
  vtkGetMacro(segment_sz,short int);

  // Set/Get Segment offset,used for time course separation
  vtkGetMacro(segment_offset,short int);

  virtual int  GetNumTimeFrames();

protected:
  //BTX
  int read_volume_ok;
  int FileType;
 

  int Orientation;
  int ForceOutputToShort;

  int DataDimensions[3];
  int NumTimeFrames;
 
  int HeaderSize;

  float DataSpacing[3];
  float DataOrigin[3];

  short int version;
  char *Filename;
  char *FMRfilename;
  char *PRTfilename;
  short int NrOfVolumes;
  short int resolution;
   short int XStart;
  short int XEnd;
  short int YStart;
  short int YEnd;
  short int ZStart; 
  short int ZEnd;

  short int hemodynamicDelay;
  float TR;
  float deltaParam;
  float tauParam;
  short int segment_sz;
  short int segment_offset;

  vtkafBVImageReader();
  virtual void Execute();
  virtual void ExecuteInformation();

  virtual vtkDataArray* ReadVolume();
  //ETX
};

#endif



