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
  Module:    $RCSfile: vtkbisImageReOrient.h,v $
  Language:  C++
  Date:      $Date: 2003/05/13 15:37:02 $
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
// .NAME vtkbisImageReOrient - Reslices an image and creates a new header for the new orientation


#ifndef __vtkbisImageReOrient_h
#define __vtkbisImageReOrient_h

#include "vtkSimpleImageToImageFilter.h"
#include "vtkbisImageHeader.h"
#include "vtkMatrix4x4.h"

class vtkbisImageReOrient : public vtkSimpleImageToImageFilter
{
public:
  vtkTypeMacro(vtkbisImageReOrient,vtkSimpleImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  // Description:
  // Construct object to extract all of the input data.
  
  static vtkbisImageReOrient *New();
  
  vtkGetObjectMacro(InputImageHeader,vtkbisImageHeader);
  vtkSetObjectMacro(InputImageHeader,vtkbisImageHeader);
  vtkGetObjectMacro(OutputImageHeader,vtkbisImageHeader);


  // Description:
  // Polar Mode used for ultrasound images
  vtkSetClampMacro(SimpleMode,int,0,1);
  vtkGetMacro(SimpleMode,int);
  vtkBooleanMacro(SimpleMode,int);


  // Description:
  // Complex Mode -- if SimpleMode is zero use matrix instead
  vtkSetObjectMacro(FullMatrix,vtkMatrix4x4);
  vtkGetObjectMacro(FullMatrix,vtkMatrix4x4);

  // Description:
  // Standard OutputOrientations
  virtual void SetOutputOrientationToLPS();
  virtual void SetOutputOrientationToRAS();
  virtual void SetOutputOrientationToLIP();
  virtual void SetOutputOrientationToRSA();
  virtual void SetOutputOrientationToPSR();
  virtual void SetOutputOrientationToASR();


  
  
  // Description:
  // Get Numerical Code for Orientation
  vtkGetMacro(OutputOrientation,int);

protected:
  //BTX
  vtkbisImageReOrient();
  ~vtkbisImageReOrient() {};
  vtkbisImageReOrient(const vtkbisImageReOrient&) {};
  void operator=(const vtkbisImageReOrient&) {};

  virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);
  
  // Execute Information Replacements
  virtual int RequestInformation(vtkInformation *vtkNotUsed(request),  vtkInformationVector **inputVector, 
				 vtkInformationVector *outputVector);

  //BTX
  virtual int GenerateArraysAndOutputImageHeader();

  // Variables
  vtkbisImageHeader* InputImageHeader;
  vtkbisImageHeader* OutputImageHeader;
  vtkMatrix4x4*      FullMatrix;

  // Variables -- old style orientation
  int OutputOrientation;
  int SimpleMode;
  int Axis[3],Flip[3];

  // Description:
  // OutputOrientation Code
  vtkSetMacro(OutputOrientation,int);
  //ETX
};

#endif


