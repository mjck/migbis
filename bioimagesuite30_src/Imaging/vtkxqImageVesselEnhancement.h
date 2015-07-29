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
  Module:    $RCSfile: vtkxqImageVesselEnhancement.h,v $
  Language:  C++
  Date:      $Date: 2002/09/26 18:08:12 $
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
// .NAME vtkxqImageVesselEnhancement -- combines images to form either mean or median
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// Implements Vessel Enhancement Filter from Chen and Amini TMI October 2004

#ifndef __vtkxqImageVesselEnhancement_h
#define __vtkxqImageVesselEnhancement_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkFloatArray.h"

class vtkxqImageVesselEnhancement : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkxqImageVesselEnhancement *New();
  vtkTypeMacro(vtkxqImageVesselEnhancement,vtkpxSimpleImageToImageFilter);

  // Description:
  // Set Parameters Alpha, Beta and Gamma
  vtkGetMacro(Alpha,float);
  vtkSetClampMacro(Alpha,float,0.05,10.0);

  vtkGetMacro(Beta,float);
  vtkSetClampMacro(Beta,float,0.05,10.0);

  vtkGetMacro(Gamma,float);
  vtkSetMacro(Gamma,float);

  vtkGetMacro(Aa,float);
  vtkSetMacro(Aa,float);

  // Description:
  // Set Smoothing Sigma for Hessian Computation
  vtkGetMacro(Sigma,float);
  vtkSetMacro(Sigma,float);

  // Description:
  // Bright Structure
  vtkSetClampMacro(BrightStructure,int,0,2);
  vtkGetMacro(BrightStructure,int);
  virtual void SetBrightStructureToDark()   { this->SetBrightStructure(0); }
  virtual void SetBrightStructureToBright() { this->SetBrightStructure(1); }
  virtual void SetBrightStructureToBoth()   { this->SetBrightStructure(2); }

  // Description: 
  // Sets/Gets mask for computing statistics.
  // If a mask is not provided, tensor is
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
  // Description:
  // Bright Structure
  vtkSetClampMacro(StructureType,int,0,4);
  vtkGetMacro(StructureType,int);
  virtual void SetStructureTypeToTubular() { this->SetStructureType(0); }
  virtual void SetStructureTypeToPlate()   { this->SetStructureType(1); }
  virtual void SetStructureTypeToBlob()    { this->SetStructureType(2); }
  virtual void SetStructureTypeToAll()     { this->SetStructureType(3); }
  virtual void SetStructureTypeToNone()    { this->SetStructureType(4); }

  // Description:
  // This a messy function that takes as inputs
  // Hessian1, Vessel1, Sigma1, Hessian2, Vessel2, Sigma2, and spits out a corrected Set 1 by using the maximum of the vessel number
  static int UpdateHessianResponse(vtkImageData* hessian1,vtkImageData* vessel1,vtkImageData* sigma,
				   vtkImageData* hessian2,vtkImageData* vessel2,double newsigma);
  
protected:

  //BTX
  vtkxqImageVesselEnhancement();
  virtual ~vtkxqImageVesselEnhancement();
  vtkxqImageVesselEnhancement(const vtkxqImageVesselEnhancement&) {};
  void operator=(const vtkxqImageVesselEnhancement&) {};
  //ETX
  virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);
  virtual void ExecuteInformation();

  // Description:
  // Workhorse Functions
  virtual double ComputeVesselLikelihood(float lambda[3],int bright,double a2,double b2,double c2);
  virtual double ComputexqVesselLikelihood(float lambda[3], int bright, double a);
  virtual double ComputePlateLikelihood(float lambda[3],int bright,double a2,double b2,double c2);
  virtual double ComputeBlobLikelihood(float lambda[3],int bright,double a2,double b2,double c2);

  vtkImageData *Mask;

  double Alpha;
  double Beta;
  double Gamma;
  double Sigma;
  double Aa;
  int    BrightStructure;
  int    StructureType;
};

#endif



