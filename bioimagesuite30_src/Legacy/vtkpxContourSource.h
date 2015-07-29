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
  Module:    $RCSfile: vtkpxContourSource.h,v $
  Language:  C++
  Date:      $Date: 2002/04/02 15:57:16 $
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
// .NAME vtkpxContourSource 


#ifndef __vtkpxContourSource_h
#define __vtkpxContourSource_h

#include "vtkPolyDataSource.h"
#include "vtkpxSplineStackSource.h"


class vtkPolyDataCollection;

class vtkpxContourSource : public vtkPolyDataSource
{
public:
  static vtkpxContourSource *New();
  vtkTypeMacro(vtkpxContourSource,vtkPolyDataSource);

  // Description:
  // Is Contour Closed?
  vtkSetClampMacro(Closed,int,0,1);
  vtkGetMacro(Closed,int);
  vtkBooleanMacro(Closed, int);

  // Description:
  // Get Points
  vtkGetObjectMacro(Points,vtkPoints)

  // Description:
  // I/O Code
  virtual int Load(const char* fname);
  virtual int Save(const char* fname);

  // Description:
  // Initialization
  virtual void Copy(vtkpxContourSource* other);
  virtual void SetFromSplineStackSource(vtkpxSplineStackSource* sur,int level,float dist);

  //BTX
  // Description:
  // Set From Contour Stack (For Internal use only)
  virtual void SetFromContourStack(PXContourStack* cstack,int level);
  virtual void SetFromContour(PXContour* cntr);
  //ETX

  // Description:
  // Orientation Stuff
  virtual int  EnsureAnticlockwise();
  virtual int  EnsureClockwise();
  virtual int  SetDirection(int dir=1); // + = Anti , -1 =Clock

  // Description:
  // Initialize 
  virtual void SetCircle(int np=10,float radius=5.0,float x=50.0,float y=50.0);


  // Description:
  // Check Inside/Outside
  virtual int   IsInside (float testpointx,float testpointy);
  static  int   Intersect (float x1,float y1, 
			   float x2,float y2, 
			   float x,float y); 
  // Description:
  // Contour Statistics
  // ----------------------
  virtual float GetArea();  
  virtual float GetContourLength();
  virtual void  GetCentroid(float& x,float& y);

 
  // Description:
  // Contour Manipulation
  // ------------------------
  virtual void  Smooth(float relaxation);
  virtual void  Sample(int width);
  virtual void  Subsample(int maxratio);

  virtual void  Equispace(float spacing);
  virtual void  Equisample(int npoints);


  // Description:
  // Symmetric Nearest Neighbor Map of Contours
  // ------------------------------------------
  static vtkpxContourSource*  MapContours(vtkpxContourSource* cnt1,vtkpxContourSource* cntr2,int resample=1);
  static int                  CyclicDistance(int p1,int p2,int np);

protected:

  vtkpxContourSource();
  virtual ~vtkpxContourSource();
  vtkpxContourSource(const vtkpxContourSource&) {};
  void operator=(const vtkpxContourSource&) {};
  
  void Execute();


  vtkPoints* Points;
  int        Closed;
};

#endif



