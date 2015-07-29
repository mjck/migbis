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
  Module:    $RCSfile: vtkpxAnalyzeImageWriter.h,v $
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
// .NAME vtkpxAnalyzeImageWriter - writes vtkStructuredPoints to file
// .SECTION Description
// Can save compressed (gzipped) , byte swapped and multi-frame analyze images
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION Caveats
// This uses the pxutil library
// .SECTION See Also


#ifndef __vtkpxAnalyzeImageWriter_h
#define __vtkpxAnalyzeImageWriter_h

#include "vtkStructuredPointsWriter.h"
#include "vtkbisImageHeader.h"

class vtkpxAnalyzeImageWriter : public vtkStructuredPointsWriter
{
public:
  static vtkpxAnalyzeImageWriter *New();
  vtkTypeMacro(vtkpxAnalyzeImageWriter,vtkStructuredPointsWriter);

  // Description:
  // Convenience Routine: simply use Save("fname.hdr")
  virtual void Save(const char* fname);

  // Description:
  // Set Compression Level 0=Uncompressed, 9=Maximum (adds .gz suffix if compressed)
  vtkGetMacro(Compression,int);
  vtkSetClampMacro(Compression,int,0,9);

  // Description:
  // Save byte-swapped if needed
  vtkGetMacro(ByteSwap,int);
  vtkSetClampMacro(ByteSwap,int,0,1);
  vtkBooleanMacro(ByteSwap, int);

  // Description:
  // Save orientation 0=axial 1=coronal 2=saggital (default=0), 3=polar (ultrasound)
  vtkGetMacro(Orientation,int);
  vtkSetClampMacro(Orientation,int,0,3);

  // Description:
  // SaveStatus = 1 Success, 0 = failure
  vtkGetMacro(SaveStatus,int);

  // Description:
  // Set Proper Header
  vtkGetObjectMacro(ImageHeader,vtkbisImageHeader);
  virtual void SetImageHeader(vtkbisImageHeader* header);


  // Description:
  // Set Force Output File Format
  static int SetForceOutputFileFormat(int mode);
  static int GetForceOutputFileFormat();
  
protected:
  vtkpxAnalyzeImageWriter();
  virtual ~vtkpxAnalyzeImageWriter();

  // Description : do the work
  void WriteData();
  void WriteDataAsAnalyze();
  void WriteDataAsNifty();


  // Description:
  // Use this to create a default header if one was not specified
  void CreateImageHeader(int print);

  //BTX
  int Compression;
  int ByteSwap;
  int Orientation;
  int SaveStatus;
  vtkbisImageHeader* ImageHeader;
  static int ForceOutputFileFormat;
  //ETX
};

#endif



