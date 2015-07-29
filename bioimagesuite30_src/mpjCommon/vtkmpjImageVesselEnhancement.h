/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkmpjImageVesselEnhancement.h,v $
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
// .NAME vtkmpjImageVesselEnhancement -- combines images to form either mean or median
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// Implements Vessel Enhancement Filter from Frangi Niessen Hoogeveen and Viergever TMI October 1999

#ifndef __vtkmpjImageVesselEnhancement_h
#define __vtkmpjImageVesselEnhancement_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkFloatArray.h"

class vtkmpjImageVesselEnhancement : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkmpjImageVesselEnhancement *New();
  vtkTypeMacro(vtkmpjImageVesselEnhancement,vtkpxSimpleImageToImageFilter);

  // Description:
  // Set Parameters Alpha, Beta and Gamma
  vtkGetMacro(Alpha,double);
  vtkSetClampMacro(Alpha,double,0.05,10.0);

  vtkGetMacro(Beta,double);
  vtkSetClampMacro(Beta,double,0.05,10.0);

  vtkGetMacro(Gamma,double);
  vtkSetMacro(Gamma,double);

  // Description:
  // Set Vessneless Scale Factor
  vtkGetMacro(VesselnessScaleFactor,double);
  vtkSetClampMacro(VesselnessScaleFactor,double,1.0,1000.0);
  

  // Description:
  // Set Smoothing Sigma for Hessian Computation
  vtkGetMacro(Sigma,double);
  vtkSetMacro(Sigma,double);

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
  vtkmpjImageVesselEnhancement();
  virtual ~vtkmpjImageVesselEnhancement();
  vtkmpjImageVesselEnhancement(const vtkmpjImageVesselEnhancement&) {};
  void operator=(const vtkmpjImageVesselEnhancement&) {};
  //ETX
  virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);
  virtual void ExecuteInformation();

  // Description:
  // Workhorse Functions
  virtual double ComputeVesselLikelihood(float lambda[3],int bright,double a2,double b2,double c2);
  virtual double ComputePlateLikelihood(float lambda[3],int bright,double a2,double b2,double c2);
  virtual double ComputeBlobLikelihood(float lambda[3],int bright,double a2,double b2,double c2);

  vtkImageData *Mask;

  double Alpha;
  double Beta;
  double Gamma;
  double Sigma;
  double VesselnessScaleFactor;
  int    BrightStructure;
  int    StructureType;
};

#endif


