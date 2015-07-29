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
 * vtkjoScalePolyData.cpp
 *
 *  Created on: Mar 13, 2009
 *      Author: jonofrey
 */

#include "vtkjoScalePolyData.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyDataNormals.h"

vtkjoScalePolyData* vtkjoScalePolyData::New()
{
    // First try to create the object from the vtkObjectFactory
    vtkObject* ret = vtkObjectFactory::CreateInstance("vtkjoScalePolyData");
    if (ret)
    {
        return (vtkjoScalePolyData*) ret;
    }
    // If the factory was unable to create the object, then create it here.
    return new vtkjoScalePolyData;
}


vtkjoScalePolyData::vtkjoScalePolyData()
{
    this->ScaleFactor = 0.0;
    this->SetScalingModeToUseCOG();
}


vtkjoScalePolyData::~vtkjoScalePolyData()
{
}


void vtkjoScalePolyData::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "ScaleFactor: " << this->ScaleFactor << "\n";
}


void vtkjoScalePolyData::ScaleUsingNormals(vtkPolyData* input,
        vtkPolyData* output, double scalar)
{
    vtkDebugMacro("Scaling the geometry along the surface normals...")

    // Get the surface normals normal filter
    vtkPolyDataNormals* normalFilter = vtkPolyDataNormals::New();
    normalFilter->SplittingOff();
    normalFilter->ComputePointNormalsOn();
    normalFilter->SetInput(input);
    normalFilter->Update();

    vtkDataArray* inPoints = input->GetPoints()->GetData();
    vtkDataArray* outPoints = output->GetPoints()->GetData();
    vtkDataArray* normals = normalFilter->GetOutput()->GetPointData()->GetNormals();

    vtkIdType nPts = input->GetNumberOfPoints();
    double point[3], normal[3];
    for (int i=0; i<nPts; i++)
    {
        inPoints->GetTuple(i, point);
        normals->GetTuple(i, normal);

        // Update the point along its normal
        for (int j=0; j<3; j++)
            point[j] = point[j] + normal[j]*scalar;

        outPoints->SetTuple(i, point);
    }

    // Cleanup
    normalFilter->Delete();
}


void vtkjoScalePolyData::ScaleUsingCOG(vtkPolyData* output,
        double scalar)
{
    vtkDebugMacro("Scaling the geometry about the COG...");

    vtkDataArray* points = output->GetPoints()->GetData();
    vtkIdType nPts = output->GetNumberOfPoints();

    // First, find the center of gravity (COG)
    double cog[3], point[3];
    for (int i=0; i<3; i++)
        cog[i] = 0.0;

    for (int i=0; i<nPts; i++)
    {
        points->GetTuple(i, point);
        for (int j=0; j<3; j++)
            cog[j] += point[j];
    }

    for (int i=0; i<3; i++)
        cog[i] /= double(nPts);

    vtkDebugMacro("COG = ("<<cog[0]<<","<<cog[1]<<","<<cog[2]<<")");

    // Scale along the normalized vector direction from each point
    // to the COG
    double direction[3];
    for (int i=0; i<nPts; i++)
    {
        points->GetTuple(i, point);

        for (int j=0; j<3; j++)
            direction[j] = cog[j] - point[j];
        // Normalize the direction vector
        vtkMath::Normalize(direction);

        // Update the point along its normal
        for (int j=0; j<3; j++)
            point[j] = point[j] + direction[j]*scalar;

        points->SetTuple(i, point);
    }
}


int vtkjoScalePolyData::RequestData(vtkInformation* vtkNotUsed(request),
        vtkInformationVector** inputVector,
        vtkInformationVector* outputVector)
{
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    vtkPolyData *input = vtkPolyData::SafeDownCast(
            inInfo->Get(vtkDataObject::DATA_OBJECT()));

    vtkDebugMacro(<<"Scaling poly data");

    if (input == NULL || input->GetPoints() == NULL)
    {
        return 1;
    }

    vtkPolyData *output = vtkPolyData::SafeDownCast(
            outInfo->Get(vtkDataObject::DATA_OBJECT()));

    // Copy the poly data to the output
    output->DeepCopy(input);

    switch (this->ScalingMode)
    {
    case VTK_SCALE_POLY_COG:
        this->ScaleUsingCOG(output, this->ScaleFactor);
        break;
    case VTK_SCALE_POLY_NORMALS:
        this->ScaleUsingNormals(input, output, this->ScaleFactor);
        break;
    default:
        break;
    }

    return 1;
}


