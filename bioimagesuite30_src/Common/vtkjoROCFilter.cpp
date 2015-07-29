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
 * vtkjoROCFilter.cpp
 *
 *  Created on: Dec 1, 2008
 *      Author: jonofrey
 */

#include "vtkjoROCFilter.h"

#include "vtkDataObject.h"
#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkShortArray.h"

vtkjoROCFilter* vtkjoROCFilter::New()
{
    // First try to create the object from the vtkObjectFactory
    vtkObject* ret = vtkObjectFactory::CreateInstance("vtkjoROCFilter");
    if (ret)
    {
        return (vtkjoROCFilter*) ret;
    }
    // If the factory was unable to create the object, then create it here.
    return new vtkjoROCFilter;
}

//---------------------------------------------------------------------

vtkjoROCFilter::vtkjoROCFilter()
{
    this->TP = 0;
    this->FP = 0;
    this->TN = 0;
    this->FN = 0;

    this->TrueResult = NULL;
}

//---------------------------------------------------------------------

void vtkjoROCFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "TP: " << this->TP << "\n";
    os << indent << "TN: " << this->TN << "\n";
    os << indent << "FP: " << this->FP << "\n";
    os << indent << "FN: " << this->FN << "\n";
}

//---------------------------------------------------------------------

double vtkjoROCFilter::GetTruePositiveRate()
{
    return double(this->TP)/double(this->TP + this->FN);
}

//---------------------------------------------------------------------

double vtkjoROCFilter::GetFalsePositiveRate()
{
    return double(this->FP)/double(this->FP + this->TN);
}

//---------------------------------------------------------------------

double vtkjoROCFilter::GetAccuracy()
{
    return double(this->TP + this->TN)/
        double(this->TP + this->FP + this->TN + this->FN);
}

//---------------------------------------------------------------------

double vtkjoROCFilter::GetSpecificity()
{
    return double(this->TN)/double(this->FP + this->TN);
}

//---------------------------------------------------------------------

double vtkjoROCFilter::GetPositivePredictiveValue()
{
    return double(this->TP)/double(this->TP + this->FP);
}

//---------------------------------------------------------------------

double vtkjoROCFilter::GetNegativePredictiveValue()
{
    return double(this->TN)/double(this->TN + this->FN);
}

//---------------------------------------------------------------------

double vtkjoROCFilter::GetFalseDiscoveryRate()
{
    return double(this->FP)/double(this->FP + this->TP);
}

//---------------------------------------------------------------------

int vtkjoROCFilter::RequestInformation(vtkInformation* vtkNotUsed(request),
        vtkInformationVector** vtkNotUsed(inputVector),
        vtkInformationVector* outputVector)
{
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_SHORT, 4);
    return 1;
}

//---------------------------------------------------------------------

void vtkjoROCFilter::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
    if (this->TrueResult == NULL)
    {
        vtkErrorMacro("TrueResult is not set. Filter cannot run.");
        return;
    }

    if (output->GetScalarType() != VTK_SHORT)
    {
        vtkErrorMacro("Execute: This filter only outputs shorts");
        return;
    }

    // Get the predicted data
    vtkDataArray* inData = input->GetPointData()->GetScalars();
    // Get the actual, truth data
    vtkDataArray* trueData = this->TrueResult->GetPointData()->GetScalars();

    vtkShortArray* outData = static_cast<vtkShortArray*>(output->GetPointData()->GetScalars());
    int numComps = outData->GetNumberOfComponents();
    for (int i=0; i<numComps; i++)
        outData->FillComponent(i, 0.0);

    short* outData_ptr = outData->GetPointer(0);

    vtkIdType numPts = input->GetNumberOfPoints();
    for (int i=0; i<numPts; i++)
    {
        double inValue = inData->GetComponent(i, 0);
        double trueValue = trueData->GetComponent(i, 0);

        if (inValue > 0.0)
        {
            // Find the True Positives
            if (trueValue > 0.0)
            {
                this->TP++;
                outData->SetComponent(i, 0, 1.0);
            }
            // Find the False Positives
            else
            {
                this->FP++;
                outData->SetComponent(i, 1, 1.0);
            }
        }
        else
        {
            // Find the False Negatives
            if (trueValue > 0.0)
            {
                this->FN++;
                outData->SetComponent(i, 2, 1.0);
            }
            // Find the True Negative
            else
            {
                this->TN++;
                outData->SetComponent(i, 3, 1.0);
            }
        }

    }

//    PrintSelf(cout, vtkIndent());
}


