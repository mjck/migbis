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
  Module:    $RCSfile: vtkpxPredictElectrodeStrip.h,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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
// .NAME vtkpxPredictElectrodeStrip - stores and manipulates a set of electrodes arranged as a grid
// .SECTION Description
// This class stores all the necesary information for an implanted Electrode Grid such as
// width x height (in electrodes e.g. 8x8)
// electrode spacing x,y
// electrode type and dimensions
// grid name
// and finally an array of the actual electrodes
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION See Also
//  vtkPoints vtkPolyData vtkPolyDataSource vtkpxElectrodeSource



#ifndef __vtkpxPredictElectrodeStrip_h
#define __vtkpxPredictElectrodeStrip_h


#include "vtkPolyData.h"
#include "vtkPolyDataSource.h"



class vtkpxPredictElectrodeStrip : public vtkPolyDataSource 
{
public:
  static vtkpxPredictElectrodeStrip *New();
  vtkTypeMacro(vtkpxPredictElectrodeStrip,vtkPolyDataSource);

public:

  // Description:
  // Grid Dimensions
  vtkSetClampMacro(Length,float,2.0,200.0);
  vtkGetMacro(Length,float);

  // Description:
  // Electrode Spacing
  vtkGetMacro(Step,float);
  vtkSetClampMacro(Step,float,1.0,50.0);

  // Description:
  // Electrode Spacing
  vtkGetMacro(Smoothness,double);
  vtkSetClampMacro(Smoothness,double,0.01,0.99);

  // Description:
  // Initial Points 1 & 2
  vtkSetVectorMacro(Point1,double,3);
  vtkGetVectorMacro(Point1,double,3);

  vtkSetVectorMacro(Point2,double,3);
  vtkGetVectorMacro(Point2,double,3);

  // Description:
  // Initial Points 1 & 2
  vtkGetObjectMacro(Input,vtkPolyData);

  // Description:
  // Set Input Surface
  virtual void SetInputSurface(vtkPolyData* poly);
    
protected:
  vtkpxPredictElectrodeStrip();
  virtual ~vtkpxPredictElectrodeStrip();

  vtkpxPredictElectrodeStrip(const vtkpxPredictElectrodeStrip&) {};
  void operator=(const vtkpxPredictElectrodeStrip&) {};
  virtual void Execute();  

  vtkSetObjectMacro(Input,vtkPolyData);


  //BTX
  float Length;
  float Step;
  double Smoothness;
  double Point1[3];
  double Point2[3];
  vtkPolyData* Input;
  //ETX

};

#endif   


