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
  Module:    $RCSfile: vtkpxUtil.h,v $
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
// .NAME vtkpxTriangulatedStackSource 


#ifndef __vtkpxTriangulatedStackSource_h
#define __vtkpxTriangulatedStackSource_h

#include "vtkPolyDataSource.h"
#include "vtkpxSplineStackSource.h"
#include "vtkUnsignedCharArray.h"

class vtkPolyDataCollection;
//BTX
class PXTriangulatedStack;
//ETX
class vtkpxTriangulatedStackSource : public vtkPolyDataSource
{
public:
  static vtkpxTriangulatedStackSource *New();
  vtkTypeMacro(vtkpxTriangulatedStackSource,vtkPolyDataSource);

  // Description:
  // Initialization
  virtual void Copy(vtkpxTriangulatedStackSource* other);
  virtual void SetFromSplineStackSource(vtkpxSplineStackSource* sur,
					float dist,float zmin=-1.0,float zmax=1000.0);
  virtual void SetFromSplineStackSource2(vtkpxSplineStackSource* sur,
					float dist,float zdist,float zmin=-1.0,float zmax=1000.0);

  //BTX
  // Description:
  // Set From Contour Stack (For Internal use only)
  virtual void SetFromContourStack(PXContourStack* cstack);
  //ETX
  

  // Description:
  // Set From Ordinary Poly Data ( Useful for computing curvatures)
  virtual void SetFromPolyData(vtkPolyData* polydata);

  // Description:
  // Compute Curvatures
  virtual void ComputeCurvatures(int scale);
  virtual void NonShrinkSmooth(float alpha,float beta,int iterations);

  // Description:
  // I/O Code
  virtual int Load(const char* fname);
  virtual int Save(const char* fname);


  // Description:
  // Raw Data Structure Output
  virtual PXTriangulatedStack* GetTStack();

  // Description:
  // Scale all in-plane points
  virtual void Scale(float scale);

  // Description:
  virtual void LoadAndFitContourStack(const char* fname);

  // Description:
  // Modes for Strain Value / Color Mode 
  vtkSetClampMacro(ColorMode,int,-1,5);
  vtkGetMacro(ColorMode,int);

  // Description:
  // Modes for Strain Value / Color Gamma 
  vtkSetClampMacro(ColorGamma,float,0.01,5);
  vtkGetMacro(ColorGamma,float);




protected:

  vtkpxTriangulatedStackSource();
  virtual ~vtkpxTriangulatedStackSource();
  vtkpxTriangulatedStackSource(const vtkpxTriangulatedStackSource&) {};
  void operator=(const vtkpxTriangulatedStackSource&) {};
  
  void Execute();
  PXTriangulatedStack* TriangulatedStack;

  void ComputeColors(vtkUnsignedCharArray* colors);

  int ColorMode;
  float ColorGamma;
};

#endif



