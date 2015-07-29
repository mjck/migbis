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
 * vtkjoScalePolyData.h
 *
 *  Created on: Mar 13, 2009
 *      Author: jonofrey
 */
// .NAME vtkjoScalePolyData - Scales a vtkPolyData using the data set's
// points.
// .SECTION Description
// vtkjoScalePolyData scales a given vtkPolyData input using the data set's
// points.  This filter can scale the geometry in two ways:
// <p>1) Scale the points in the direction of the geometry's center of
// gravity (COG).  Positive scaling factor values shrink the geometry in
// towards the COG.  This is the default scaling mode.
// <p>2) Scale the points in the direction of the geometry's inward facing
// surface normals.
// Using negative ScalingFactor values causes the geometry to expand
// outwards.
// .SECTION Caveats
// This filter can result in inverting surface meshes if the scaling factor
// is too large.  Also, when using the surface normals scaling mode, concave
// parts of the surface can expand outwards causing the geometry to
// self-intersect.

#ifndef VTKJOSCALEPOLYDATA_H_
#define VTKJOSCALEPOLYDATA_H_

#include "vtkPolyDataAlgorithm.h"

#define VTK_SCALE_POLY_COG 0
#define VTK_SCALE_POLY_NORMALS 1

class vtkjoScalePolyData : public vtkPolyDataAlgorithm
{
public:
    static vtkjoScalePolyData *New();
    vtkTypeMacro(vtkjoScalePolyData,vtkPolyDataAlgorithm);

    void PrintSelf(ostream& os, vtkIndent indent);

    // Description:
    // ScaleFactor - the scalar value to shrink the surface by.
    vtkGetMacro(ScaleFactor, double);
    vtkSetMacro(ScaleFactor, double);

    // Description:
    // Set the scaling mode (default: COG)
    vtkSetClampMacro(ScalingMode, int,
            VTK_SCALE_POLY_COG, VTK_SCALE_POLY_NORMALS)
    vtkGetMacro(ScalingMode, int);
    void SetScalingModeToUseCOG() {
        this->SetScalingMode(VTK_SCALE_POLY_COG); };
    void SetScalingModeToUseNormals() {
        this->SetScalingMode(VTK_SCALE_POLY_NORMALS); };

protected:

    double ScaleFactor;
    int ScalingMode;

    void ScaleUsingCOG(vtkPolyData* output, double scalar);
    void ScaleUsingNormals(vtkPolyData* input, vtkPolyData* output, double scalar);

    int RequestData(vtkInformation* request,
            vtkInformationVector** inputVector,
            vtkInformationVector* outputVector);

    vtkjoScalePolyData();
    virtual ~vtkjoScalePolyData();
};

#endif /* VTKJOSCALEPOLYDATA_H_ */

