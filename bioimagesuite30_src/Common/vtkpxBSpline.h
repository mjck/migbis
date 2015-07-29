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
  Module:    $RCSfile: vtkpxMath.h,v $
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
// .NAME vtkpxBSpline - a routine to implement Bsplines of arbitrary dimensions
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION Caveats
// This requires fitpack

#ifndef __vtkpxBSpline_h
#define __vtkpxBSpline_h

#include "vtkObject.h"
typedef float real;
class vtkDoubleArray;

class vtkpxBSpline : public vtkObject
{
public:
  static vtkpxBSpline *New();
  vtkTypeMacro(vtkpxBSpline,vtkObject);
  

  // Description: Get Position,Derivative
  // ------------------------------------
  virtual float GetPosition(float s);

  // Description: Modify Control Points 
  // -----------------------------------
  virtual float GetControl(int i);
  virtual void  SetControl(int i,float v);
  virtual void  ShiftControl(int i,float v);
  virtual float GetKnot(int i);

  
  // Description: Fit Points
  // ------------------------
  virtual int FitArray(vtkDoubleArray* inputdata,int npoints,float smooth,int component);
  virtual int FitArray(vtkDoubleArray* inputdata,vtkDoubleArray* ordinate,int npoints,float smooth,int component);

  // Description: Access Methods
  // Degree of Polynomial
  vtkSetClampMacro(Degree, int,1,3);
  vtkGetMacro(Degree, int);

  // Description: Access Methods
  // Degree of Polynomial
  vtkSetClampMacro(Offset, float,0.0,2.0);
  vtkGetMacro(Offset, float);

  vtkGetMacro(NumberOfPoints, int);
  vtkGetMacro(NumberOfControlPoints, int);
  vtkGetMacro(NumberOfKnots, int);

  int   GetControlPointKnotPosition(int index);
  float GetControlPointLowerBound(int index);
  float GetControlPointUpperBound(int index);
  void  GetControlPointBounds(int index,float bounds[2]);

protected:

  // Description: Constructor/Desctructor etc.
  vtkpxBSpline();
  virtual ~vtkpxBSpline();

  vtkpxBSpline(const vtkpxBSpline&) {};
  void operator=(const vtkpxBSpline&) {};

  // Member variables
  
  int Degree;
  int NumberOfPoints;
  int NumberOfKnots;
  int NumberOfControlPoints;
  real* controlpoints;
  real* knots;
  float Offset;


  virtual int   GetControlPointIndex(int i);
  
  //BTX
  void Cleanup();
  //ETX
  
  
};
#endif

