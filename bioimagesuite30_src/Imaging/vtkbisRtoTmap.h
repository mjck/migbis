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

/*
 * vtkbisRtoTmap.h
 *
 *  Created on: May 6, 2009
 *      Author: isabella
 */

#ifndef VTKBISRTOTMAP_H_
#define VTKBISRTOTMAP_H_

#include "vtkSimpleImageToImageFilter.h"

class vtkbisRtoTmap : public vtkSimpleImageToImageFilter{
public:
	static vtkbisRtoTmap *New();
	vtkTypeMacro(vtkbisRtoTmap,vtkSimpleImageToImageFilter);

	//number of subjects
	vtkSetMacro(NumSubject,int);
	vtkGetMacro(NumSubject,int);

	virtual float RtoTmap(float r);
protected:
	vtkbisRtoTmap();
	virtual ~vtkbisRtoTmap(){};
	virtual int RequestInformation(vtkInformation*,vtkInformationVector**,vtkInformationVector* outputVector);
	// Execute Function -- main function
	virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);
private:
	vtkbisRtoTmap(const vtkbisRtoTmap& src) {};
	vtkbisRtoTmap& operator=(const vtkbisRtoTmap& rhs) {};
	int NumSubject;

};

#endif /* VTKBISRTOTMAP_H_ */

