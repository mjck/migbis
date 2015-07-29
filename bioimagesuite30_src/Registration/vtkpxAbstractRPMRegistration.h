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
  Module:    $RCSfile: vtkpxAbstractRPMRegistration.h,v $
  Language:  C++
  Date:      $Date: 2003/03/06 16:34:27 $
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

#ifndef __vtkpxAbstractRPMRegistration_h
#define __vtkpxAbstractRPMRegistration_h

class vtkpxGridTransform;
class vtkpxMatrix;
class vtkPointLocator;
class vtkPoints;
class vtkPolyData;
class vtkTransform;
class vtkMatrix4x4;
class vtkCollection;

#include "vtkAbstractTransform.h"
#include "vtkpxAbstractMatrix.h"
#include "vtkpxMatrix.h"
#include "vtkpxRPMCorrespondenceFinder.h"

class vtkpxAbstractRPMRegistration : public vtkpxRPMCorrespondenceFinder
{
public:
  static vtkpxAbstractRPMRegistration *New();
  vtkTypeMacro(vtkpxAbstractRPMRegistration,vtkpxRPMCorrespondenceFinder);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Get Current Transformation -- needed for displaying updates 
  virtual vtkAbstractTransform* GetCurrentTransformation();

  // Description:
  // Initial Temperature for deterministic annealing
  vtkSetClampMacro(InitialTemperature,float,0.0,1000.0);
  vtkGetMacro(InitialTemperature,float);

  // Description:
  // Final Temperature for deterministic annealing
  vtkSetClampMacro(FinalTemperature,float,0.0,1000.0);
  vtkGetMacro(FinalTemperature,float);

  // Description:
  // AnnealRate for annealing 
  vtkSetClampMacro(AnnealRate,float,0.5,0.999);
  vtkGetMacro(AnnealRate,float);

  // Description:
  // Smoothness Factors
  vtkSetClampMacro(InitialSmoothnessFactor,float,0.02,200.0);
  vtkGetMacro(InitialSmoothnessFactor,float);
  vtkSetClampMacro(FinalSmoothnessFactor,float,0.02,200.0);
  vtkGetMacro(FinalSmoothnessFactor,float);

  // Description: 
  // Symmetry Weight for exchanging parameters
  vtkSetClampMacro(SymmetryWeight, float,0,0.5);
  vtkGetMacro(SymmetryWeight, float);

  // Description: 
  // UseInitialTransform
  vtkSetClampMacro(UseSymmetricCorrespondence, int,0,1);
  vtkGetMacro(UseSymmetricCorrespondence, int);
  vtkBooleanMacro(UseSymmetricCorrespondence, int);
  
  // Description:
  // Execute The Transformation
  virtual void Run();


protected:

  vtkpxAbstractRPMRegistration();
  vtkpxAbstractRPMRegistration(const vtkpxAbstractRPMRegistration&) {};
  void operator=(const vtkpxAbstractRPMRegistration&) {};


  float AnnealRate;
  float InitialSmoothnessFactor;
  float FinalSmoothnessFactor;
  float InitialTemperature;
  float FinalTemperature;
  int   UseSymmetricCorrespondence;
  float SymmetryWeight;
  int CurrentlyUpdating;

  // -----------------------------------------------------------
  // Description:
  // Annealing Procedures
  // -----------------------------------------------------------
  virtual void Anneal();
  virtual void SymmetricAnneal();

  


};

#endif

