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
  Module:    $RCSfile: vtkpxComputeStrains.h,v $
  Language:  C++
  Date:      $Date: 2003/02/17 16:20:30 $
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
#ifndef __vtkpxComputeStrains_h
#define __vtkpxComputeStrains_h

#include "vtkpxFemSolid.h"
#include "vtkAbstractTransform.h"
#include "vtkProcessObject.h"
#include "vtkImageData.h"

class vtkpxComputeStrains : public vtkProcessObject
{
public:

  static vtkpxComputeStrains *New();
  vtkTypeMacro(vtkpxComputeStrains,vtkProcessObject);

  // Description:
  // Set Solid
  vtkSetObjectMacro(BaseSolid,vtkpxFemSolid);
  vtkGetObjectMacro(BaseSolid,vtkpxFemSolid);

  // Description:
  // Set Transformation
  vtkSetObjectMacro(Transformation,vtkAbstractTransform);
  vtkGetObjectMacro(Transformation,vtkAbstractTransform);

  // Description:
  // Set/Get Coordinate Frame
  vtkSetClampMacro(CoordinateFrame,int,0,2);
  vtkGetMacro(CoordinateFrame,int);
  void SetCoordinateFrameToXYZ() { this->SetCoordinateFrame(0);}
  void SetCoordinateFrameToRadial() { this->SetCoordinateFrame(1);}
  void SetCoordinateFrameToFibres() { this->SetCoordinateFrame(2);}
  
  // Description:
  // I/O Code -- save as .sld file 
  virtual int SaveOutput(const char* fname);

  // Description:
  // Generate Output
  virtual int Run();

  // Description:
  // Generate Output Image Instead
  virtual int Run(vtkImageData* in,vtkImageData* out);


  // Description:
  // Generate .stat file
  static int GenerateStudyFile(const char* namestem,const char* sldname,int nframes);

protected:

  vtkpxComputeStrains();
  virtual ~vtkpxComputeStrains();
  vtkpxComputeStrains(const vtkpxComputeStrains&) {};
  void operator=(const vtkpxComputeStrains&) {};
  
  vtkpxFemSolid* BaseSolid;
  vtkAbstractTransform* Transformation;
  int            CoordinateFrame;

};

#endif



