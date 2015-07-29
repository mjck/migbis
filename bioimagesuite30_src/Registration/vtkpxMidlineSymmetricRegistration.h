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

// ------------------------------------------------------------------------
//
// This file derives from original work
// in the vtkCISG Toolkit
// that was released under the terms of 
// the GNU General Public License (either version 2,
// of the License, or any later version.)
// 
// The code has been modified for inclusion in BioImage Suite
//
// It may consist of work derived from more than one source
// file in the vtkCISG Toolkit. The original copyright notice
// was:
//
//  Authors   : Thomas Hartkens <thomas@hartkens.de>
//              Daniel Rueckert <d.rueckert@doc.ic.ac.uk>
//	        Julia Schnabel  <julia.schnabel@kcl.ac.uk>
//	        under the supervision of Dave Hawkes and Derek Hill
//  Web       : http://www.image-registration.com
//  Copyright : King's College London
//              Div. of Radiological Sciences, 
//              Computational Imaging Science Group, 1997 - 2000
//	        http://www-ipg.umds.ac.uk/cisg
//
// ------------------------------------------------------------------------


#ifndef _VTKPXMIDLINEREGISTRATION_H
#define _VTKPXMIDLINEREGISTRATION_H

#include "vtkpxLinearRegistration.h"

class vtkpxMidlineSymmetricRegistration : public vtkpxLinearRegistration {

public:
  static vtkpxMidlineSymmetricRegistration* New();
  vtkTypeMacro(vtkpxMidlineSymmetricRegistration,vtkpxLinearRegistration);

protected:
  // Make all these protected 
  // Constructor
  vtkpxMidlineSymmetricRegistration();
  vtkpxMidlineSymmetricRegistration(const vtkpxMidlineSymmetricRegistration&) {};
  void operator=(const vtkpxMidlineSymmetricRegistration&) {};

  // Description:
  // Evaluates the registration. This function evaluates the registration by
  // looping over the target image and interpolating the transformed source 
  // image while filling the joint histogram. This function returns the value
  // of the similarity measure using Similarity().
  virtual double Evaluate();

  virtual int InitializeAll();



};


#endif

