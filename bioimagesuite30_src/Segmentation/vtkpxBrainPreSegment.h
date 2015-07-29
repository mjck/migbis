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
  Module:    $RCSfile: vtkpxBrainPreSegment.h,v $
  Language:  C++
  Date:      $Date: 2002/09/26 18:53:08 $
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
// .NAME vtkpxBrainPreSegment - simple brain segmentation algorithm
// .SECTION Description
// This is an implementation of a simple brain segmentation algorithm
// (originally from the work of Rik Stokking and Oskar Skrinjar)
// which consists of four steps <OL>
// <LI> Threshold Image  between <TT>LowThreshold</TT> and <TT>HighThreshold</TT>
// <LI> Erode  (if <TT>Erode</TT> >0) (<TT>NumErosions</TT> iterations)
// <LI> Apply Connectivity (if <TT>Regrow</TT>>0) based on (<TT>Seed</TT>[] coordinates)
// <LI> Apply a modified Dilation (if <TT>Geodilate</TT>>0) (<TT>NumErosions</TT>+1 iterations)
// </UL>
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>

#ifndef __vtkpxBrainPreSegment_h
#define __vtkpxBrainPreSegment_h

#include "vtkpxSimpleImageToImageFilter.h"

class vtkpxBrainPreSegment : public vtkpxSimpleImageToImageFilter
{
public:
  vtkTypeMacro(vtkpxBrainPreSegment,vtkpxSimpleImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct object to extract all of the input data.

  static vtkpxBrainPreSegment *New();

  // Description:
  // Description: Get/Set Thresholds for step 1
  vtkSetMacro(LowThreshold,float);
  vtkGetMacro(LowThreshold,float);
  vtkSetMacro(HighThreshold,float);
  vtkGetMacro(HighThreshold,float);

  // Description:
  // Description: Get/Set Seed position (in voxels) for connectivity
  vtkSetVectorMacro(Seed,int,3);
  vtkGetVectorMacro(Seed,int,3);

  // Description:
  // Flag to determine whether to erode or not
  vtkGetMacro(Erode,int);
  vtkSetMacro(Erode,int);

  // Description:
  // Flag to determine whether to run connectivity or not
  vtkGetMacro(Regrow,int);
  vtkSetMacro(Regrow,int);

  // Description:
  // Flag to determine whether to dilate or not
  vtkGetMacro(Geodilate,int);
  vtkSetMacro(Geodilate,int);

  // Description:
  // Number of Erosions/Dilations if enabled
  vtkGetMacro(NumErosions,int);
  vtkSetMacro(NumErosions,int);

protected:
  //BTX
  vtkpxBrainPreSegment();
  ~vtkpxBrainPreSegment() {};
  vtkpxBrainPreSegment(const vtkpxBrainPreSegment&) {};
  void operator=(const vtkpxBrainPreSegment&) {};

  virtual void SimpleExecute(vtkImageData *in,vtkImageData *op);

  int   Erode;
  int   Regrow;
  int   Geodilate;
  int   NumErosions;
  float LowThreshold;
  float HighThreshold;
  int Seed[3];
  //ETX
};

#endif



