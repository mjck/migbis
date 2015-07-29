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
  Module:    $RCSfile: vtkpxFemSolid.h,v $
  Language:  C++
  Date:      $Date: 2003/05/28 16:42:57 $
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
// .NAME vtkpxFemSolid 


#ifndef __vtkpxFemSolid_h
#define __vtkpxFemSolid_h


#include "vtkPolyDataSource.h"
#include "vtkpxTriangulatedStackSource.h"


class vtkPolyDataCollection;
class vtkCollection;
//BTX
class PXAbaqusSolid;
class PXAbaqusOutputData;
//ETX

class vtkpxFemSolid : public vtkPolyDataSource
{
public:
  static vtkpxFemSolid *New();
  vtkTypeMacro(vtkpxFemSolid,vtkPolyDataSource);

  // Description:
  // Initialization
  virtual void Copy(vtkpxFemSolid* other);

  // Description:
  // Create Mesh 
  virtual int GenerateHexahedralSolid(vtkpxTriangulatedStackSource* endo,
				       vtkpxTriangulatedStackSource* epi,
				       int nodes,int totalnumstacks,int bias);

  virtual int GenerateHexahedralSolid(vtkpxTriangulatedStackSource* endo,
				       vtkpxTriangulatedStackSource* epi,
				       int nodes,int totalnumstacks,int bias,float sample,
				       int skipslices);
  
  // Description:
  // I/O Code -- save as .sld file 
  virtual int Load(const char* fname);
  virtual int Save(const char* fname);


  // Description:
  // Modes for Strain Value / Color Mode 
  vtkSetClampMacro(ColorMode,int,0,6);
  vtkGetMacro(ColorMode,int);
  vtkSetClampMacro(StrainMode,int,0,11);
  vtkGetMacro(StrainMode,int);

  // Description:
  // If Warp=on transform mesh using internal displacements 
  vtkSetClampMacro(WarpMode,int,0,1);
  vtkGetMacro(WarpMode,int);

  // Description:
  // Access Fem Data
  virtual int CreateFemData();
  virtual int LoadFemData(const char* line);
  virtual int SaveFemData(const char* line);

  // Description:
  // Access Fem Data
  virtual float GetDisplacement(int node,int comp);
  virtual float GetStrain(int node,int comp);

  // Description:
  // Get Internal Solid and Fem Data
  //BTX
  virtual PXAbaqusSolid*      GetInternalSolid();
  virtual PXAbaqusOutputData* GetInternalFemData();
  //ETX

  // Description:
  // Scale Points
  virtual void Scale(float s);


  virtual int GetNumberOfNodes();
  virtual int GetNumberOfElements();


  // Description:
  // Some Static Stuff
  //BTX
  static int ReslicePies(vtkCollection* solidCollection,int numframes,
			 int numslices,int numsectors,int numtransmural,int anti,
			 float ref[3]);
  static int SavePies(const char* fname,vtkCollection* solidCollection,int numframes,
		      int numslices,int numsectors,int numtransmural,int anti,
		      float ref[3]);
  //ETX


  // Description:
  // Final Issues
  virtual int ReorientStrainsToFiberOrientation(vtkImageData* img);
  virtual vtkPolyData* GenerateFibreMaps(vtkImageData* img,double scale=2.0);
  virtual vtkPolyData* GenerateInternalFibreMaps(double scale=2.0);


protected:

  vtkpxFemSolid();
  virtual ~vtkpxFemSolid();
  vtkpxFemSolid(const vtkpxFemSolid&) {};
  void operator=(const vtkpxFemSolid&) {};
  
  PXAbaqusSolid* InternalSolid;
  int            ColorMode;
  int            StrainMode;
  int            WarpMode;

  // Description:
  // Generate Output
  void Execute();
  
  // Description:
  // Get Color Value for Scalars
  void LookupColorMode(float t[3],float val,int mode);
};

#endif



