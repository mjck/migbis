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
  Module:    $RCSfile: vtkpxBSplineRPMRegistration.h,v $
  Language:  C++
  Date:      $Date: 2004/11/19 01:06:39 $
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

#ifndef __vtkpxBSplineRPMRegistration_h
#define __vtkpxBSplineRPMRegistration_h

#include "vtkpxComboTransform.h"
#include "vtkpxAbstractRPMRegistration.h"

class vtkpxBSplineRPMRegistration : public vtkpxAbstractRPMRegistration
{
public:
  static vtkpxBSplineRPMRegistration *New();
  vtkTypeMacro(vtkpxBSplineRPMRegistration,vtkpxAbstractRPMRegistration);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Get Current Transformation -- needed for displaying updates 
  virtual vtkAbstractTransform* GetCurrentTransformation();

  // Description:
  // Initial ControlPointSpacing for deterministic annealing and bspline grid
  vtkSetClampMacro(InitialControlPointSpacing,float,0.01,1000);
  vtkGetMacro(InitialControlPointSpacing,float);

  // Description:
  // Final ControlPointSpacing for deterministic annealing and bspline grid
  vtkSetClampMacro(FinalControlPointSpacing,float,0.01,900);
  vtkGetMacro(FinalControlPointSpacing,float);

  // Description:
  // Current Control Point Spacing
  vtkGetMacro(CurrentControlPointSpacing,float);

  // Description:
  // Get/Set Prior Weights
  vtkGetObjectMacro(FinalWeights,vtkDoubleArray);

protected:
  // Description:
  
  vtkpxBSplineRPMRegistration();
  ~vtkpxBSplineRPMRegistration();
  vtkpxBSplineRPMRegistration(const vtkpxBSplineRPMRegistration&) {};
  void operator=(const vtkpxBSplineRPMRegistration&) {};

  vtkpxGridTransform  *GridTransform;
  vtkpxComboTransform *ComboTransform;
  float InitialControlPointSpacing;
  float FinalControlPointSpacing;
  float CurrentControlPointSpacing;
  vtkDoubleArray       *FinalWeights;

  // Description
  // Master Optimization Routine
  virtual void Anneal();




  // Description:
  // Generate Transformation Details 
  virtual void GenerateTransformation(vtkpxGridTransform* gridxform,
				      vtkpxMatrix* x,vtkpxMatrix* vy, vtkpxMatrix* lx,
				      float cps,double bounds[6],float sm,
				      vtkCollection* W,vtkpxMatrix* OutlierColumn=NULL,
				      int finalmode=0);


  virtual void InitializeUsingInitialTransform(vtkpxMatrix* X,vtkpxMatrix* Y,
					       vtkpxMatrix* VX,
					       vtkAbstractTransform* usethistransformation);

  virtual void InitializeUsingInitialComboTransform(vtkpxMatrix* X,vtkpxMatrix* Y,
						    vtkpxMatrix* VX,
						    vtkpxComboTransform* usethiscombotransformation);



};

#endif

