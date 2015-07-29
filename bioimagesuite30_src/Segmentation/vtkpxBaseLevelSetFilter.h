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
  Module:    $RCSfile: vtkpxBaseLevelSetFilter.h,v $
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
// .NAME vtkpxBaseLevelSetFilter -- corrects for B0 inhomogeneity in 1-H CSI Data for fat quantification

#ifndef __vtkpxBaseLevelSetFilter_h
#define __vtkpxBaseLevelSetFilter_h

#include "vtkSimpleImageToImageFilter.h"
#include "vtkpxLevelSetUtil.h"

class vtkpxBaseLevelSetFilter : public vtkSimpleImageToImageFilter
{
public:
  vtkTypeMacro(vtkpxBaseLevelSetFilter,vtkSimpleImageToImageFilter);

  // Part 1 -- Input Seeds or Levelset (if the later is not NULL use it first)
  vtkSetObjectMacro(InitialLevelSet,vtkImageData);
  vtkGetObjectMacro(InitialLevelSet,vtkImageData);

  vtkSetObjectMacro(InitialPoints,vtkPoints);
  vtkGetObjectMacro(InitialPoints,vtkPoints);

  // Gradient Scale
  vtkSetClampMacro(InitialDistance,float,1.0,10.0);
  vtkGetMacro(InitialDistance,float);
  

  // Part 2 -- Actual Levelset Stuff

  // RMS Errors
  vtkSetClampMacro(RMSError,float,0.0,1.0);
  vtkGetMacro(RMSError,float);

  // LevelsetIterations
  vtkSetClampMacro(LevelsetIterations,int,1,1000);
  vtkGetMacro(LevelsetIterations,int);

  // Description::
  // Internal Stuff
  vtkGetObjectMacro(InputLevelSet,vtkImageData);


  // Description:
  // Debug Output 
  vtkSetStringMacro(IterationName);
  vtkGetStringMacro(IterationName);

  // Description:
  // Last Output
  vtkSetStringMacro(OutputStatus);
  vtkGetStringMacro(OutputStatus);


  // Description:
  // Debug Save On
  vtkSetClampMacro(IterationSave,int,0,1);
  vtkGetMacro(IterationSave,int);
  vtkBooleanMacro(IterationSave,int);

  vtkSetClampMacro(IterationSaveFrequency,int,1,1000000);
  vtkGetMacro(IterationSaveFrequency,int);
  vtkBooleanMacro(IterationSaveFrequency,int);

  vtkSetClampMacro(IterationSaveOrientation,int,0,2);
  vtkGetMacro(IterationSaveOrientation,int);
  vtkBooleanMacro(IterationSaveOrientation,int);
 
  // Part 3 -- Actual Levelset Stuff
  // Inflation Strength
  vtkSetClampMacro(PropagationWeight,float,0.0,1.0);
  vtkGetMacro(PropagationWeight,float);

  // CurvatureWeight
  vtkSetClampMacro(CurvatureWeight,float,0.0,1.0);
  vtkGetMacro(CurvatureWeight,float);

  // AdvectionWeight
  vtkSetClampMacro(AdvectionWeight,float,0.0,1.0);
  vtkGetMacro(AdvectionWeight,float);


  // AdvectionWeight
  vtkSetClampMacro(UpdatePercentage,float,0.05,1.01);
  vtkGetMacro(UpdatePercentage,float);


  // RealIteration
  vtkSetMacro(RealIteration,int);
  vtkGetMacro(RealIteration,int);
  
  
protected:

  vtkpxBaseLevelSetFilter();
  virtual ~vtkpxBaseLevelSetFilter();
  vtkpxBaseLevelSetFilter(const vtkpxBaseLevelSetFilter&) {};
  void operator=(const vtkpxBaseLevelSetFilter&) {};


  // Description
  // Execute Information Replacements
  virtual int RequestInformation(vtkInformation *vtkNotUsed(request),  vtkInformationVector **inputVector, 
				 vtkInformationVector *outputVector);

  // Description:
  // Basic Methods
  virtual int  GenerateInputLevelSet(vtkImageData* input,float factor=1.0);

  vtkImageData* InitialLevelSet;
  vtkPoints*    InitialPoints;
  float         InitialDistance;
  float         RMSError;
  int           LevelsetIterations;
  int           OffsetIteration;
  vtkImageData* InputLevelSet;



  //BTX
  // Some ITK Definitions
  char* IterationName,*OutputStatus;
  int IterationSave,IterationSaveFrequency,IterationSaveOrientation;
  float         PropagationWeight;
  float         CurvatureWeight;
  float         AdvectionWeight;
  float         UpdatePercentage;
  int           RealIteration;
   //ETX

};



#endif

