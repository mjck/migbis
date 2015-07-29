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
  Module:    $RCSfile: vtkpxCrossHairSource.h,v $
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
// .NAME vtkpxCrossHairSource - creates a cross-hair source for clipping 
// .SECTION Description



#ifndef __vtkpxCrossHairSource_h
#define __vtkpxCrossHairSource_h

class vtkPoints;
class vtkPolyData;
#include "vtkPolyDataSource.h"
#include "pxutil.h"

class vtkpxCrossHairSource : public vtkPolyDataSource 
{
public:
  static vtkpxCrossHairSource *New();
  vtkTypeMacro(vtkpxCrossHairSource,vtkPolyDataSource);

public:

  // Description:
  // Location and Normal
  vtkGetVectorMacro(Position,double,3);
  vtkSetVectorMacro(Position,double,3);

  // Description:
  // Thickness i.e. distance between lines
  vtkGetMacro(Thickness,double);
  vtkSetMacro(Thickness,double);

  // Description:
  // Length i.e. length of lines 
  vtkGetMacro(Length,double);
  vtkSetMacro(Length,double);

  // Description:
  // Show Sphere
  vtkSetClampMacro(ShowSphere,int,0,1);
  vtkGetMacro(ShowSphere,int);
  vtkBooleanMacro(ShowSphere,int);

 

protected:
  vtkpxCrossHairSource();
  virtual ~vtkpxCrossHairSource();

  vtkpxCrossHairSource(const vtkpxCrossHairSource&) {};
  void operator=(const vtkpxCrossHairSource&) {};

  // Description:
  // Enclose attributs in BTX..ETX block to hide them from the TCL parser
  //BTX
  double Position[3];
  double Thickness;
  double Length;
  int   ShowSphere;
  //ETX

  // Description:
  // Generate Output Surface
  virtual void Execute();  
};

#endif   


