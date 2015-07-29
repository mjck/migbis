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





#ifndef __vtkafBVImageWriter_h
#define __vtkafBVImageWriter_h

#include "vtkStructuredPointsWriter.h"

class vtkafBVImageWriter : public vtkStructuredPointsWriter
{
public:
  static vtkafBVImageWriter *New();
  vtkTypeMacro(vtkafBVImageWriter,vtkStructuredPointsWriter);


  // Convenience Routine: simply use Save("fname.hdr")
  void Save(const char* fname);


  // Set/Get Number of Time frames in case of 4D Images
  vtkGetMacro(NumFrames,int);
  vtkSetMacro(NumFrames,int);


  // Save byte-swapped if needed
  vtkGetMacro(ByteSwap,int);
  vtkSetClampMacro(ByteSwap,int,0,1);
  vtkBooleanMacro(ByteSwap, int);


  // Save orientation 0=axial 1=coronal 2=saggital (default=2)
  vtkGetMacro(Orientation,int);
  vtkSetClampMacro(Orientation,int,0,2);


  // SaveMultipleComponents As Frames 
  vtkGetMacro(MultipleComponentsAsFrames,int);
  vtkSetClampMacro(MultipleComponentsAsFrames,int,0,1);
  vtkBooleanMacro(MultipleComponentsAsFrames, int);


  //the following six functions are parameters for the relative positions with a VMR volume (must be 256^3 data set
  //for Talaraich VTC data

  // Set/Get relative positions
  vtkGetMacro(XStart,short int);
  vtkSetMacro(XStart,short int);

 // 
  vtkGetMacro(slices,int);
  vtkSetMacro(slices,int);
  //
  vtkGetMacro(NrOfVolumes,short int);
  vtkSetMacro(NrOfVolumes,short int);

  // Get/Set relative positions
  vtkGetMacro(XEnd,short int);
  vtkSetMacro(XEnd,short int);


  // Get/Set relative positions
  vtkGetMacro(YEnd,short int);
  vtkSetMacro(YEnd,short int);

  // Get/Set relative positions
  vtkGetMacro(ZEnd,short int);
  vtkSetMacro(ZEnd,short int);


  // Get/Set relative positions
  vtkGetMacro(YStart,short int);
  vtkSetMacro(YStart,short int);

 // Description:
  // Get/Set relative positions
  vtkGetMacro(ZStart,short int);
  vtkSetMacro(ZStart,short int);


  // Set/Get VTC-resolution 
  vtkGetMacro(resolution,short int);
  vtkSetMacro(resolution,short int);


  // Set/Get  FMR and PRT filenames
  void SetFilenames (const char *, const char *);
  char * GetFilename (char *);

  
 // Set/Get Hemodynamic Delay, simple shift value
  vtkGetMacro(hemodynamicDelay,short int);
  vtkSetMacro(hemodynamicDelay,short int);

 // Set/Get TR (ms)
  vtkGetMacro(TR,float);
  vtkSetMacro(TR,float);

 // Set/Get delta Parameter in the Hemodynamic function
  vtkGetMacro(deltaParam,float);
  vtkSetMacro(deltaParam,float);

 // Set/Get tau Parameter in the Hemodynamic function
  vtkGetMacro(tauParam,float);
  vtkSetMacro(tauParam,float);

 // Set/Get Segment size ,used for time course separation
  vtkGetMacro(segment_sz,short int);
  vtkSetMacro(segment_sz,short int);

// Set/Get Segment offset,used for time course separation
  vtkGetMacro(segment_offset,short int);
  vtkSetMacro(segment_offset,short int);
protected:
  //BTX
  vtkafBVImageWriter();
  ~vtkafBVImageWriter() {};
  
  
  void WriteData();
 
  char *FMRfilename; 
  char *PRTfilename; 
  int slices;
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

  int NumFrames;
  int ByteSwap;
  int Orientation;
  int MultipleComponentsAsFrames;
  //ETX
};

#endif


