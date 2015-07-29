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
  Module:    $RCSfile: vtkpxNormalShrinkPolyData.h,v $
  Language:  C++
  Date:      $Date: 2003/06/05 21:03:09 $
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

// .NAME vtkpxNormalShrinkPolyData - shrink cells composing PolyData
// .SECTION Description
// vtkpxNormalShrinkPolyData shrinks cells composing a polygonal dataset (e.g., 
// vertices, lines, polygons, and triangle strips) towards their centroid. 
// The centroid of a cell is computed as the average position of the
// cell points. Shrinking results in disconnecting the cells from
// one another. The output dataset type of this filter is polygonal data.
//
// During execution the filter passes its input cell data to its
// output. Point data attributes are copied to the points created during the
// shrinking process.

// .SECTION Caveats
// It is possible to turn cells inside out or cause self intersection
// in special cases.

// .SECTION See Also
// vtkShrinkFilter

#ifndef __vtkpxNormalShrinkPolyData_h
#define __vtkpxNormalShrinkPolyData_h

class vtkDataSet;

#include "vtkPolyDataToPolyDataFilter.h"
#include "vtkDataArray.h"
#include "vtkCellArray.h"
#include "vtkDataObject.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"

class vtkpxNormalShrinkPolyData : public vtkPolyDataToPolyDataFilter 
{
public:
  static vtkpxNormalShrinkPolyData *New();
  vtkTypeMacro(vtkpxNormalShrinkPolyData,vtkPolyDataToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the distance to move in/out from the normal
  vtkSetClampMacro(Distance,float,-20.0,20.0);

  // Description:
  // Get the distance to move in/out from the normal
  vtkGetMacro(Distance,float);


protected:
  vtkpxNormalShrinkPolyData();

  vtkpxNormalShrinkPolyData(const vtkpxNormalShrinkPolyData&) {};
  void operator=(const vtkpxNormalShrinkPolyData&) {};

  void Execute();
  float Distance;

};

#endif

