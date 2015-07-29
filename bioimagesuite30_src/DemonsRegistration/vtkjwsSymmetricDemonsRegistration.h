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
  Module:    $RCSfile: vtkjwsDemonsRegistration.h,v $
  Language:  C++
  Date:      $Date: 2002/09/18 20:22:27 $
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
// .NAME vtkjwsDemonsRegistration -- corrects for B0 inhomogeneity in 1-H CSI Data for fat quantification

#ifndef __vtkjwsSymmetricDemonsRegistration_h
#define __vtkjwsSymmetricDemonsRegistration_h


#include "vtkProcessObject.h"
#include "vtkImageData.h"
#include "vtkpxGridTransform.h"
#include "vtkIntArray.h"

class vtkjwsSymmetricDemonsRegistration : public vtkProcessObject
{
public:
  static vtkjwsSymmetricDemonsRegistration *New();
  vtkTypeMacro(vtkjwsSymmetricDemonsRegistration,vtkProcessObject);

  // Description:
  // Reference Image
  vtkSetObjectMacro(ReferenceDistanceMap,vtkImageData);
  vtkGetObjectMacro(ReferenceDistanceMap,vtkImageData);

  // Description:
  // Target Image
  vtkSetObjectMacro(TargetDistanceMap,vtkImageData);
  vtkGetObjectMacro(TargetDistanceMap,vtkImageData);


  // Description:
  // Transformation
  vtkGetObjectMacro(GridTransform,vtkpxGridTransform);


  // Description:
  // Run Filter
  // Mode = 1 => First  Demons
  // Mode = 2 => Second Demons
  virtual int RunExtendedDemons(int Mode);

  // Mode =3 (Implied) --> GrayLevel Images
  virtual int RunNormalDemons();


  // Description:
  // Processing Steps
  static int            DecideImageSize(vtkImageData* ref,vtkImageData* targ,int outdim[3]);
  static vtkIntArray* DecideImageSize(vtkImageData* ref,vtkImageData* targ);
  static vtkImageData*  PadImage(vtkImageData* inp,int outdim[3]);
  static vtkImageData*  PadImage(vtkImageData* inp,int x,int y,int z);

  static vtkImageData*  ComputeDistanceMap(vtkImageData* input,double lower_threshold,double upper_threshold);
  static vtkImageData*  MergeDistanceMaps(vtkImageData* dmap1,vtkImageData* dmap2);


protected:

  vtkjwsSymmetricDemonsRegistration();
  virtual ~vtkjwsSymmetricDemonsRegistration();
  vtkjwsSymmetricDemonsRegistration(const vtkjwsSymmetricDemonsRegistration&) {};
  void operator=(const vtkjwsSymmetricDemonsRegistration&) {};

  // Class Members
  // -------------
  vtkImageData*       ReferenceDistanceMap;
  vtkImageData*       TargetDistanceMap;
  vtkpxGridTransform* GridTransform;



  // Description:
  // Make Images Same size and multiples of 4




  
};
#endif

