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
  Module:    $RCSfile: vtkpxLinearRPMRegistration.h,v $
  Language:  C++
  Date:      $Date: 2002/03/12 16:50:35 $
  Version:   $Revision: 1.2 $


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

#ifndef __vtkpxLinearRPMRegistration_h
#define __vtkpxLinearRPMRegistration_h

#include "vtkTransform.h"
#include "vtkpxAbstractRPMRegistration.h"

class vtkpxLinearRPMRegistration : public vtkpxAbstractRPMRegistration
{
public:
  static vtkpxLinearRPMRegistration *New();
  vtkTypeMacro(vtkpxLinearRPMRegistration,vtkpxAbstractRPMRegistration);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Get Current Transformation -- needed for displaying updates 
  virtual vtkAbstractTransform* GetCurrentTransformation();

  // Description:
  // Set the current mode 0=Rigid, 1=Similarity 2=Affine 
  vtkSetClampMacro(TransformMode,int,0,2);
  vtkGetMacro(TransformMode,int);

  // Description:
  // Dedicated Set/Get TransformMode stuff 
  virtual void SetTransformModeToRigid()      { this->SetTransformMode(0);}
  virtual void SetTransformModeToSimilarity() { this->SetTransformMode(1);}
  virtual void SetTransformModeToAffine()     { this->SetTransformMode(2);}

protected:
  // Description:
  
  vtkpxLinearRPMRegistration();
  ~vtkpxLinearRPMRegistration();
  vtkpxLinearRPMRegistration(const vtkpxLinearRPMRegistration&) {};
  void operator=(const vtkpxLinearRPMRegistration&) {};

  vtkTransform *ForwardTransform;
  int TransformMode;

  // Description:
  // Annealing Routines both one-way and symmetric
  virtual void Anneal();

  // Description:
  // Generate Transformation Details 
  virtual void GenerateTransformation(vtkTransform* transform,vtkpxMatrix* x,vtkpxMatrix* vy,int mode,vtkpxMatrix* weights=NULL,vtkpxMatrix* labels=NULL);
  
};

#endif

