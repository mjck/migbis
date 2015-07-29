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
 * vtkjoPolyDataRobustBETTraining.cpp
 *
 *  Created on: Feb 6, 2009
 *      Author: jonofrey
 */

#include <vtkjoPolyDataRobustBETTraining.h>

#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkDoubleArray.h"
#include "vtkImageAppendComponents.h"
#include "vtkImageContinuousDilate3D.h"
#include "vtkImageContinuousErode3D.h"
#include "vtkImageExtractComponents.h"
#include "vtkImageReslice.h"
#include "vtkImageThreshold.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkjoBETHistogram.h"
#include "vtkjoBETImageFilter.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyDataNormals.h"
#include "vtkPolyDataToImageStencil.h"
#include "vtkpxImageAlgebra.h"
#include "vtkpxUtil.h"
#include "vtkShortArray.h"
#include "vtkStreamingDemandDrivenPipeline.h"

vtkjoPolyDataRobustBETTraining* vtkjoPolyDataRobustBETTraining::New()
{
    // First try to create the object from the vtkObjectFactory
    vtkObject* ret = vtkObjectFactory::CreateInstance("vtkjoRobustBETTraining");
    if (ret)
    {
        return (vtkjoPolyDataRobustBETTraining*) ret;
    }
    // If the factory was unable to create the object, then create it here.
    return new vtkjoPolyDataRobustBETTraining;
}

void vtkjoPolyDataRobustBETTraining::PrintSelf(ostream &os, vtkIndent indent)
{
    vtkPolyDataAlgorithm::PrintSelf(os, indent);
}


vtkjoPolyDataRobustBETTraining::vtkjoPolyDataRobustBETTraining()
{
    this->SrcImage = NULL;
    this->Appender = vtkImageAppendComponents::New();

    this->MinIntensitySearchDist = 7.0;
    this->MaxIntensitySearchDist = 3.5;
    this->LocalThresholdConst = 0.5;

    this->NormalizeInputsOn();
    this->SearchOutwardsOff();
}

vtkjoPolyDataRobustBETTraining::~vtkjoPolyDataRobustBETTraining()
{
    if (this->Appender != NULL)
        this->Appender->Delete();
}

void vtkjoPolyDataRobustBETTraining::SetSrcImage(vtkImageData* image)
{
    this->SrcImage = image;
    this->Modified();
}

//=========================================================================

void vtkjoPolyDataRobustBETTraining::AddInput(vtkImageData* image)
{
    if (image == NULL) {
        vtkErrorMacro("Unable to add NULL image");
        return;
    }

    if (this->NormalizeInputs) {
        // First, normalize the input image
        double range[2];
        image->GetPointData()->GetScalars()->GetRange(range);

        float auto_min, auto_max;
        vtkpxUtil::ImageRobustRange(image, 0.01, 0.99, auto_min, auto_max);

        // Create a new map
        vtkDoubleArray* map = vtkDoubleArray::New();
        map->SetNumberOfComponents(3);

        // add to the map x,a,b triples
        map->InsertNextTuple3(auto_min, 0.0, 0.0);
        map->InsertNextTuple3(auto_max, 0.0, 100.0/(auto_max-auto_min));
        map->InsertNextTuple3(range[1], 100.0, 0.0);

        vtkpxImageAlgebra* filter = vtkpxImageAlgebra::New();
        filter->SetInput(image);
        filter->SetMap(map);
        filter->Update();

        // Now, append the normalized image
        this->Appender->AddInput(filter->GetOutput());
        this->Appender->Update();

        map->Delete();
        filter->Delete();
    } else {
        // Append the image
        this->Appender->AddInput(image);
        this->Appender->Update();
    }

    this->Modified();
}

//=========================================================================

vtkImageData* vtkjoPolyDataRobustBETTraining::GetInputImage()
{
    vtkImageData* output = vtkImageData::New();
    output->ShallowCopy(this->SrcImage);
    return output;
}

//=========================================================================

double vtkjoPolyDataRobustBETTraining::GetMin(const char *arrayName)
{
    if (!arrayName) {
        return 0.0;
    }

    vtkPolyData* output = this->GetOutput();

    vtkDataArray* data = output->GetPointData()->GetArray(arrayName);
    // Make sure the array with the given name exists
    if (data) {
        double range[2];
        data->GetRange(range, 0);
        return range[0];
    }

    return 0.0;
}

double vtkjoPolyDataRobustBETTraining::GetMax(const char *arrayName)
{
    if (!arrayName) {
        return 0.0;
    }

    vtkPolyData* output = this->GetOutput();

    vtkDataArray* data = output->GetPointData()->GetArray(arrayName);
    // Make sure the array with the given name exists
    if (data) {
        double range[2];
        data->GetRange(range, 0);
        return range[1];
    }

    return 0.0;
}

//=========================================================================

void vtkjoPolyDataRobustBETTraining::ComputeStatistics(vtkDataArray* data,
        vtkDataArray* means,
        vtkDataArray* vars)
{
    vtkIdType nPts = data->GetNumberOfTuples();
    if (nPts != means->GetNumberOfTuples() || nPts != vars->GetNumberOfTuples())
    {
        vtkErrorMacro("Data arrays do not have the same size.");
        return;
    }

    int numComps = data->GetNumberOfComponents();
    double sum=0.0, sumSqrs=0.0;
    double value, mean, var;
    double tol = 1E-05;

    for (int i=0; i<nPts; i++)
    {
        sum = 0.0, sumSqrs = 0.0;
        for (int n=0; n<numComps; n++)
        {
            value = data->GetComponent(i, n);
            sum += value;
            sumSqrs += value*value;
        }
        mean = sum/double(numComps);
        means->SetComponent(i, 0, mean);

        var = mean*mean + (sumSqrs - 2.0*mean*sum)/double(numComps);
        // Check for very small variances and just set these to zero
        if (var < tol) {
            var = 0.0;
        }
//        vars->SetComponent(i, 0, sqrt(var));
        vars->SetComponent(i, 0, var);
    }
}

//=========================================================================

vtkImageData* vtkjoPolyDataRobustBETTraining::GetOutputAsImage()
{
    // Check to make sure we have all the object we need to work with.
    if (this->SrcImage == NULL) {
        vtkErrorMacro("SrcImage cannot be null.");
        return NULL;
    }

    vtkIdType nPts = this->SrcImage->GetNumberOfPoints();

    vtkImageData* ones = vtkImageData::New();
    ones->CopyStructure(this->SrcImage);
    ones->SetNumberOfScalarComponents(1);
    ones->SetScalarTypeToFloat();
    ones->AllocateScalars();
    ones->GetPointData()->GetScalars()->FillComponent(0, 1.0);

    vtkPolyDataToImageStencil* stencil = vtkPolyDataToImageStencil::New();
    stencil->SetInput(this->GetOutput());
    stencil->SetInformationInput(ones);
    stencil->Update();

    vtkImageReslice* reslice = vtkImageReslice::New();
    reslice->SetInput(ones);
    reslice->SetStencil(stencil->GetOutput());
    reslice->SetBackgroundColor(0.0, 0.0, 0.0, 1.0);
    reslice->Update();

    stencil->Delete();
    ones->Delete();

    vtkImageContinuousDilate3D* dilate = vtkImageContinuousDilate3D::New();
    dilate->SetInputConnection(reslice->GetOutputPort());
    dilate->SetKernelSize(3,3,3);
    dilate->Update();

    vtkImageContinuousErode3D* erode = vtkImageContinuousErode3D::New();
    erode->SetInputConnection(reslice->GetOutputPort());
    erode->SetKernelSize(3,3,3);
    erode->Update();

    reslice->Delete();

    vtkImageData* outline = vtkImageData::New();
    outline->ShallowCopy(dilate->GetOutput());

    vtkDataArray* outlineDA = outline->GetPointData()->GetScalars();
    vtkDataArray* eroded = erode->GetOutput()->GetPointData()->GetScalars();

    // Subtract the eroded brain mask from the dilated mask
    for (int i = 0; i < nPts; i++) {
        if (eroded->GetComponent(i, 0) > 0.0) {
            outlineDA->SetComponent(i, 0, 0.0);
        }
    }

    erode->Delete();
    dilate->Delete();

    // Update the remaining surface pixels to reflect values from
    // the field data at the closest point
    vtkPolyData* surface = this->GetOutput();

    // Create a new image with the correct number of components
    int numArrays = surface->GetPointData()->GetNumberOfArrays();
    vtkImageAppendComponents* appender = vtkImageAppendComponents::New();
    for (int i=0; i<numArrays; i++) {
        appender->AddInput(outline);
        appender->Update();
    }

    outline->Delete();

    vtkImageData* output = vtkImageData::New();
    output->ShallowCopy(appender->GetOutput());
    vtkDataArray* outDA = output->GetPointData()->GetScalars();

    appender->Delete();

    // Now fill in the output frames with the field data values
    vtkDataArray* fieldData;
    double point[3];
    double value = 0.0;

    for (int n=0; n < numArrays; n++) {
        fieldData = surface->GetPointData()->GetArray(n);

        for (int i=0; i < nPts; i++) {
            if (outDA->GetComponent(i, n) > 0.0) {
                output->GetPoint(i, point);
                vtkIdType id = surface->FindPoint(point);
                if (id != -1) {
                    value = fieldData->GetComponent(id, 0);
                    outDA->SetComponent(i, n, value);
                }
            }
        }
    }

    // Cleanup...
    return output;
}

//=========================================================================
/*
vtkDoubleArray* vtkjoPolyDataRobustBETTraining::ComputeF2(vtkPolyData* surface) {

    // Check to make sure we have a surface
    if (surface == NULL) {
        vtkErrorMacro("surface cannot be null.");
        return NULL;
    }

    vtkIdType nPts = surface->GetNumberOfPoints();

    vtkDoubleArray* f2s = vtkDoubleArray::New();
    f2s->SetNumberOfComponents(1);
    f2s->SetNumberOfTuples(nPts);

    vtkPolyDataNormals* normalFilter = vtkPolyDataNormals::New();
    normalFilter->SplittingOff();
    normalFilter->FlipNormalsOn();
    normalFilter->ComputePointNormalsOn();
    normalFilter->SetInput(surface);
    normalFilter->Update();

    vtkjoBETImageFilter* bet = vtkjoBETImageFilter::New();
    bet->SetSurface(surface);

    vtkDataArray* normals = normalFilter->GetOutput()->GetPointData()->GetNormals();

    vtkDataArray* points = surface->GetPoints()->GetData();

    double const RMin = 3.33333;
    double const RMax = 10.0;
    double E = 0.5*((1/RMin) + (1/RMax));
    double F = 6.0/((1/RMin) - (1/RMax));
    double abs_dot, dot, r_inv, f_2;

//    double L = this->ComputeMeanVertexSpacing(surface);
    double L = bet->ComputeMeanVertexSpacing();
    double L2 = L*L;

    double point[3], normal[3], mean[3], s[3], st[3], sn[3];

    for (int i=0; i<nPts; i++) {

        points->GetTuple(i, point);
        normals->GetTuple(i, normal);

//        this->ComputeMeanPosition(surface, points, this->NeighborList[i], mean);
        bet->ComputeMeanPositionOfNeighbors(i, mean);

        for (int j=0; j<3; j++)
            s[j] = mean[j] - point[j];

        dot = s[0]*normal[0] + s[1]*normal[1] + s[2]*normal[2];
        for (int j=0; j<3; j++)
        {
            sn[j] = dot*normal[j];
            st[j] = s[j] - sn[j];
        }

        // Compute component update 2
        abs_dot = dot;
        if (abs_dot < 0.0)
            abs_dot *= -1.0;

        r_inv = (2.0*abs_dot)/L2;
        f_2 = 0.5*(1.0 + tanh(F*(r_inv - E)));

        f2s->SetComponent(i, 0, f_2);
    }

    normalFilter->Delete();
    bet->Delete();

    return f2s;
}
*/
//=========================================================================

int vtkjoPolyDataRobustBETTraining::RequestData(
        vtkInformation* vtkNotUsed(request),
        vtkInformationVector** inputVector,
        vtkInformationVector* outputVector)
{
    // TODO: this is bad to do this here, but it will do for now...
    fprintf(stderr, "getting data from the appender...\n");
    this->SrcImage = this->Appender->GetOutput();
    fprintf(stderr, "SrcImage has num comps = %d\n",
            this->SrcImage->GetNumberOfScalarComponents());

    // Get the info objects
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    // Get the input and output
    vtkPolyData *input = vtkPolyData::SafeDownCast(
            inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkPolyData *output = vtkPolyData::SafeDownCast(
            outInfo->Get(vtkDataObject::DATA_OBJECT()));

    vtkDataArray* points = input->GetPoints()->GetData();
    vtkIdType nPts = points->GetNumberOfTuples();

    // Copy the data through
    output->DeepCopy(input);

    vtkPolyDataNormals* normalFilter = vtkPolyDataNormals::New();
    normalFilter->SetInput(input);
    normalFilter->SplittingOff();
    normalFilter->FlipNormalsOn();
    normalFilter->ComputePointNormalsOn();
    normalFilter->Update();
    vtkDataArray* normals = normalFilter->GetOutput()->GetPointData()->GetNormals();

    // Get the source image
    int numComps = this->SrcImage->GetNumberOfScalarComponents();

    // Create a temporary storage array for calculating the mean
    // and variance of f3
    vtkDoubleArray* f3StatsArray = vtkDoubleArray::New();
    f3StatsArray->SetNumberOfComponents(numComps);
    f3StatsArray->SetNumberOfTuples(nPts);

    vtkFloatArray* meanBETVal = vtkFloatArray::New();
    meanBETVal->SetName("BET_mean");
    meanBETVal->SetNumberOfComponents(1);
    meanBETVal->SetNumberOfTuples(nPts);

    vtkFloatArray* varBETVal = vtkFloatArray::New();
    varBETVal->SetName("BET_var");
    varBETVal->SetNumberOfComponents(1);
    varBETVal->SetNumberOfTuples(nPts);

    // Create a temporary storage array for calculating the mean
    // and variance of bt
    vtkDoubleArray* btStatsArray = vtkDoubleArray::New();
    btStatsArray->SetNumberOfComponents(numComps);
    btStatsArray->SetNumberOfTuples(nPts);

    vtkFloatArray* meanBtVal = vtkFloatArray::New();
    meanBtVal->SetName("bt_mean");
    meanBtVal->SetNumberOfComponents(1);
    meanBtVal->SetNumberOfTuples(nPts);

    vtkFloatArray* varBtVal = vtkFloatArray::New();
    varBtVal->SetName("bt_var");
    varBtVal->SetNumberOfComponents(1);
    varBtVal->SetNumberOfTuples(nPts);

    // Create a temporary storage array for calculating the mean and
    // variance of bt with compensation for f2 update term
//    vtkFloatArray* meanBt2Val = vtkFloatArray::New();
//    meanBt2Val->SetName("bt2_mean");
//    meanBt2Val->SetNumberOfComponents(1);
//    meanBt2Val->SetNumberOfTuples(nPts);
//
//    vtkFloatArray* varBt2Val = vtkFloatArray::New();
//    varBt2Val->SetName("bt2_var");
//    varBt2Val->SetNumberOfComponents(1);
//    varBt2Val->SetNumberOfTuples(nPts);


    vtkImageExtractComponents* extractor = vtkImageExtractComponents::New();
    extractor->SetInput(this->SrcImage);
    vtkImageData* currentImage;

    vtkjoBETHistogram* bet = vtkjoBETHistogram::New();

    double maxDist =
        (this->MinIntensitySearchDist > this->MaxIntensitySearchDist)?
                this->MinIntensitySearchDist : this->MaxIntensitySearchDist;
    double localThreshold = pow(this->LocalThresholdConst, 0.275);

    for (int n=0; n<numComps; n++) {
        fprintf(stderr, "processing image %d...\n", n);
        extractor->SetComponents(n);
        extractor->Update();

        vtkDataArray* imgData = extractor->GetOutput()->GetPointData()->GetScalars();
        double range[2];
        imgData->GetRange(range);
        fprintf(stderr, "  img %d range: [%f, %f]\n", n, range[0], range[1]);

        bet->SetInputConnection(extractor->GetOutputPort());
        bet->Update();
        double Threshold = bet->GetThreshold();
        double MinIntensityThresh = bet->GetMinIntensity();
        double MaxIntensityThresh = bet->GetMaxIntensity();
        double MedianIntensity = bet->GetMedianIntensity();

        // Loop through all the surface points and write the BET update
        // coefficient value to the statistics array at the corresponding
        // location
        vtkIdType index;
        double intensity=0.0, Imin=0.0, Imax=0.0, t_l=0.0, f_3=0.0;
        double f_2 = 0.0;
        double b_t=0.0;
        double point[3], normal[3], probePt[3];
        for (int i=0; i<nPts; i++)
        {
            points->GetTuple(i, point);
            normals->GetTuple(i, normal);

            // Compute the update component according to BET
            Imin = MedianIntensity;
            Imax = Threshold;

            for (double d=1.0; d<maxDist; d = d + 1.0)
            {
                if (!this->SearchOutwards) {
                    for (int j=0; j<3; j++)
                        probePt[j] = point[j] - d*normal[j];
                } else {
                    for (int j=0; j<3; j++)
                        probePt[j] = point[j] + d*normal[j];
                }

                index = this->SrcImage->FindPoint(probePt);
                if (index != -1) {
                    intensity = imgData->GetComponent(index, 0);
                    if (d<this->MinIntensitySearchDist && intensity < Imin)
                        Imin = intensity;
                    if (d<this->MaxIntensitySearchDist && intensity > Imax)
                        Imax = intensity;
                }
            }

            if (MinIntensityThresh > Imin)
                Imin = MinIntensityThresh;
            if (MedianIntensity < Imax)
                Imax = MedianIntensity;

            t_l = (Imax - MinIntensityThresh)*localThreshold + MinIntensityThresh;
            if ((Imax - MinIntensityThresh) > 0.0)
                f_3 = 2.0*(Imin - t_l)/(Imax - MinIntensityThresh);
            else
                f_3 = 2.0*(Imin - t_l);

            // Store the value in our temporary array
            f3StatsArray->SetComponent(i, n, f_3);

            // Store the analytical value of bt in our surface
//            b_t = (Imin - MinIntensityThresh)/(Imax - MinIntensityThresh);
            // Store the normalized analytical value of bt in our surface
            b_t = (Imin - MinIntensityThresh)/(Imax - MinIntensityThresh);
            b_t /= localThreshold;
            btStatsArray->SetComponent(i, n, b_t);

        }
    }

    this->ComputeStatistics(f3StatsArray, meanBETVal, varBETVal);
    this->ComputeStatistics(btStatsArray, meanBtVal, varBtVal);
//    this->ComputeStatistics(bt2StatsArray, meanBt2Val, varBt2Val);

    // Compute the value of bt2
//    vtkFloatArray* f2 = this->ComputeF2(input);
//    for (int i=0; i<nPts; i++) {
//        meanBt2Val->SetComponent(i, 0,
//                meanBtVal->GetComponent(i, 0) + 0.5*f2->GetComponent(i, 0));
//    }
//    varBt2Val->DeepCopy(varBtVal);

    // Get the point data, this is what we'll be setting.
    vtkPointData* outPD = output->GetPointData();
    outPD->AddArray(meanBETVal);
    outPD->AddArray(varBETVal);
    outPD->AddArray(meanBtVal);
    outPD->AddArray(varBtVal);
//    outPD->AddArray(meanBt2Val);
//    outPD->AddArray(varBt2Val);

    meanBETVal->Delete();
    varBETVal->Delete();
    meanBtVal->Delete();
    varBtVal->Delete();
//    meanBt2Val->Delete();
//    varBt2Val->Delete();
    f3StatsArray->Delete();
    btStatsArray->Delete();
    bet->Delete();
    extractor->Delete();

    return 1;
}


