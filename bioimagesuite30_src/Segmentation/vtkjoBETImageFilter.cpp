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
 * vtkjoBETImageFilter.cpp
 *
 *  Created on: Sep 12, 2008
 *      Author: jonofrey
 */

#include "vtkCellArray.h"
#include "vtkDoubleArray.h"
#include "vtkIdList.h"
#include "vtkImageReslice.h"
#include "vtkImageThreshold.h"
#include "vtkjoBETImageFilter.h"
#include "vtkjoSphereSource.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyDataToImageStencil.h"

//-------------------------------------------------------------------------------

vtkjoBETImageFilter* vtkjoBETImageFilter::New()
{
    // First try to create the object from the vtkObjectFactory
    vtkObject* ret = vtkObjectFactory::CreateInstance("vtkjoBETImageFilter");
    if (ret)
    {
        return (vtkjoBETImageFilter*) ret;
    }
    // If the factory was unable to create the object, then create it here.
    return new vtkjoBETImageFilter;
}

//-------------------------------------------------------------------------------

vtkjoBETImageFilter::vtkjoBETImageFilter()
{
    this->BETHistogram = vtkjoBETHistogram::New();

    // Get initial values from the vtkjoBETHistogram instance
    this->MinIntensityPercent = this->BETHistogram->GetMinIntensityPercent();
    this->MaxIntensityPercent = this->BETHistogram->GetMaxIntensityPercent();
    this->Bins = this->BETHistogram->GetBins();
    this->MinIntensityThresh = this->BETHistogram->GetMinIntensity();
    this->MaxIntensityThresh = this->BETHistogram->GetMaxIntensity();
    this->Threshold = this->BETHistogram->GetThreshold();
    this->BETHistogram->GetCenterOfGravity(this->CenterOfGravity);
    this->Radius = this->BETHistogram->GetRadius();
    this->MedianIntensity = this->BETHistogram->GetMedianIntensity();

    this->ManualRadiusAndCOGOff();

    this->RadiusScaleFactor = 0.5;
    this->SurfaceResolution = 4;
    this->MaxIterations = 1000;
    this->MaxPasses = 10;
    this->MinChangeTolerance = 0.01;
    this->MinIntensitySearchDist = 7.0;
    this->MaxIntensitySearchDist = this->MinIntensitySearchDist*0.5;
    this->LocalThresholdConst = 0.5;
    this->RMin = 3.33333;
    this->RMax = 10.0;
    this->IncreasedSmoothing = 0;
    this->SelfIntersectionThreshold = 4000;

    this->Surface = NULL;
    this->SurfaceInput = NULL;
    this->EdgeTable = NULL;
}

//-------------------------------------------------------------------------------

vtkjoBETImageFilter::~vtkjoBETImageFilter()
{
    if (this->BETHistogram != NULL)
        this->BETHistogram->Delete();
}

//-------------------------------------------------------------------------------

void vtkjoBETImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkSimpleImageToImageFilter::PrintSelf(os, indent);

    os << indent << "  MinIntensityPercent " << this->MinIntensityPercent << "\n";
    os << indent << "  MaxIntensityPercent " << this->MaxIntensityPercent << "\n";
    os << indent << "  Bins " << this->Bins << "\n";
    os << indent << "  MinIntensityThresh " << this->MinIntensityThresh << "\n";
    os << indent << "  MaxIntensityThresh " << this->MaxIntensityThresh << "\n";
    os << indent << "  Threshold " << this->Threshold << "\n";
    os << indent << "  COG.x " << this->CenterOfGravity[0] << "\n";
    os << indent << "  COG.y " << this->CenterOfGravity[1] << "\n";
    os << indent << "  COG.z " << this->CenterOfGravity[2] << "\n";
    os << indent << "  Radius " << this->Radius << "\n";
    os << indent << "  MedianIntensity " << this->MedianIntensity << "\n";
    os << indent << "  ManualRadiusAndCOG  " << this->ManualRadiusAndCOG << "\n";

    os << indent << "  RadiusScaleFactor " << this->RadiusScaleFactor << "\n";
    os << indent << "  SurfaceResolution " << this->SurfaceResolution << "\n";
    os << indent << "  MaxIterations " << this->MaxIterations << "\n";
    os << indent << "  MaxPasses " << this->MaxPasses << "\n";
    os << indent << "  MinChangeTolerance " << this->MinChangeTolerance << "\n";
    os << indent << "  MinIntensitySearchDist " << this->MinIntensitySearchDist << "\n";
    os << indent << "  MaxIntensitySearchDist " << this->MaxIntensitySearchDist << "\n";
    os << indent << "  LocalThresholdConst " << this->LocalThresholdConst << "\n";
    os << indent << "  RMin " << this->RMin<< "\n";
    os << indent << "  RMax " << this->RMax << "\n";
    os << indent << "  IncreasedSmoothing " << this->IncreasedSmoothing << "\n";
    os << indent << "  SelfIntersectionThreshold " << this->SelfIntersectionThreshold << "\n";

}

//-------------------------------------------------------------------------------

vtkImageData* vtkjoBETImageFilter::GetOutputMask()
{
    vtkDebugMacro(<<"Creating binary mask...");

    vtkImageData* input = this->GetImageDataInput(0);

    if (this->Surface == NULL || input == NULL)
        vtkErrorMacro(<<"Either surface points or source image (or both) is NULL.");

    vtkImageData* output = vtkImageData::New();

    vtkPolyDataToImageStencil* stencil = vtkPolyDataToImageStencil::New();
    stencil->SetInput(this->Surface);
    stencil->SetInformationInput(input);
    stencil->Update();

    vtkImageReslice* reslice = vtkImageReslice::New();
    reslice->SetInput(input);
    reslice->SetStencil(stencil->GetOutput());
    reslice->SetBackgroundColor(0.0, 0.0, 0.0, 1.0);

    vtkImageThreshold* threshold = vtkImageThreshold::New();
    threshold->SetInputConnection(reslice->GetOutputPort());
    threshold->SetOutputScalarTypeToShort();
    threshold->ThresholdByLower(0.0);
    threshold->SetInValue(0.0);
    threshold->SetOutValue(1.0);
    threshold->Update();

    output->ShallowCopy(threshold->GetOutput());

    stencil->Delete();
    reslice->Delete();
    threshold->Delete();

    return output;
}

//-------------------------------------------------------------------------------

vtkPolyData* vtkjoBETImageFilter::CreateInitialSurface()
{
    vtkDebugMacro(<<"Creating initial surface as a sphere...");

    vtkPolyData* surface = vtkPolyData::New();

    // Create the initial sphere source based on the radius and COG
    vtkjoSphereSource* sphereSrc = vtkjoSphereSource::New();
    sphereSrc->SetRadius(this->Radius*this->RadiusScaleFactor);
    sphereSrc->SetCenter(this->CenterOfGravity[0],
            this->CenterOfGravity[1],
            this->CenterOfGravity[2]);
    sphereSrc->IcosahedralTessellationOn();
    sphereSrc->SetSubdivisionLevels(this->SurfaceResolution);
    sphereSrc->Update();
    surface->ShallowCopy(sphereSrc->GetOutput());
    sphereSrc->Delete();

    vtkDebugMacro(<<"Finished creating sphere.");

    return surface;
}

//-------------------------------------------------------------------------------

void vtkjoBETImageFilter::FindNeighbors()
{
    // Get the neighbors for each vertex
    vtkDebugMacro(<<"Computing neighbor list...");

    if (this->Surface == NULL) {
        vtkErrorMacro(<<"Surface is NULL!");
        return;
    }

    int numPoints = this->Surface->GetNumberOfPoints();
    this->NeighborList = new vtkIdList*[numPoints];
    for (int i=0; i<numPoints; i++) {
        this->NeighborList[i] = vtkIdList::New();
    }

    int numCells = this->Surface->GetNumberOfCells();
    vtkCellArray* polys = this->Surface->GetPolys();
    polys->InitTraversal();

    vtkIdType nPts;
    // Each point can have either 5 or 6 neighbors
    vtkIdType  pts0[6];
    vtkIdType* pts = &pts[0];

    vtkIdType p1 = 0;
    vtkIdType p2 = 0;

    for (int i=0; i<numCells; i++) {
        polys->GetNextCell(nPts, pts);
        // Fill the unique neighbor list
        for (int j=0; j<nPts-1; j++) {
            p1 = pts[j];
            for (int k=j+1; k<nPts; k++) {
                p2 = pts[k];
                this->NeighborList[p1]->InsertUniqueId(p2);
                this->NeighborList[p2]->InsertUniqueId(p1);
            }
        }
    }

    vtkDebugMacro(<<"Finished computing neighbor list.");
}

//-------------------------------------------------------------------------------

vtkIdType vtkjoBETImageFilter::CreateEdgeTable()
{
    // Get the neighbors for each vertex
    vtkDebugMacro(<<"Creating edge table...");

    if (this->Surface == NULL) {
        vtkErrorMacro(<<"Cannot create edge table, surface is NULL!");
        return 0;
    }

    if (this->EdgeTable != NULL) {
        vtkDebugMacro(<<"Edge table is not empty - deleting old table.");
        this->EdgeTable->Delete();
    }

    vtkIdType numCells = this->Surface->GetNumberOfCells();

    vtkCellArray* polys = this->Surface->GetPolys();
    polys->InitTraversal();

    this->EdgeTable = vtkEdgeTable::New();
    this->EdgeTable->InitEdgeInsertion(this->Surface->GetNumberOfPoints(), 0);

    vtkIdType nPts;
    // Each point can have either 5 or 6 neighbors
    vtkIdType  pts0[3];
    vtkIdType* pts = &pts[0];

    vtkIdType p1 = 0;
    vtkIdType p2 = 0;

    for (int i=0; i<numCells; i++) {
        polys->GetNextCell(nPts, pts);
        for (int idx=0; idx<nPts; idx++) {
            int next = (idx+1)%nPts;
            vtkIdType id = this->EdgeTable->IsEdge(pts[idx], pts[next]);
            // Add the edge to the table if it does not already exist
            if (id == -1)
                this->EdgeTable->InsertEdge(pts[idx], pts[next]);
        }
    }

    vtkDebugMacro(<<"Finished creating edge table.");

    return this->EdgeTable->GetNumberOfEdges();
}

//-------------------------------------------------------------------------------

double vtkjoBETImageFilter::ComputeMeanVertexSpacing(vtkPolyData* surface)
{
    vtkDebugMacro(<<"Computing mean vertex spacing for the surface.");

    if (surface == NULL || this->EdgeTable == NULL) {
        vtkErrorMacro(<<"Surface points are NULL");
        return 0.0;
    }

    vtkDoubleArray* pointsDA = static_cast<vtkDoubleArray*>(surface->GetPoints()->GetData());
    double* points = pointsDA->GetPointer(0);

    int numComps = pointsDA->GetNumberOfComponents();

    vtkIdType numEdges = this->EdgeTable->GetNumberOfEdges();
    this->EdgeTable->InitTraversal();

    double meanSpacing = 0.0;

    vtkIdType p1, p2;

    for (int i=0; i<numEdges; i++) {
        this->EdgeTable->GetNextEdge(p1, p2);
        meanSpacing += sqrt(vtkMath::Distance2BetweenPoints(&points[p1*numComps], &points[p2*numComps]));
    }

    return meanSpacing/numEdges;
}

//-------------------------------------------------------------------------------
/*
double vtkjoBETImageFilter::ComputeMeanVertexSpacing()
{
    return this->ComputeMeanVertexSpacing(this->Surface);
}
*/
// -------------------------------------------------------------------------------------------

void vtkjoBETImageFilter::ComputeMeanPosition(
        const double* points,
        vtkIdList* neighbors,
        double mean[3])
{
    for (int ia=0; ia<=2; ia++)
        mean[ia] = 0.0;

    int numNeighbors = neighbors->GetNumberOfIds();
    double scale = 1.0/(double(numNeighbors));

    int offset=0;
    for (int j=0; j<numNeighbors; j++)
    {
        offset = neighbors->GetId(j)*3;
        for (int idx=0; idx<=2; idx++)
            mean[idx] += points[offset+idx];
    }

    for (int idx=0; idx<=2; idx++)
        mean[idx] = mean[idx]*scale;
}

// -------------------------------------------------------------------------------------------
/*
void vtkjoBETImageFilter::ComputeMeanPositionOfNeighbors(vtkIdType id, double mean[3])
{
    for (int i=0; i<3; i++)
        mean[i] = 0.0;

    vtkIdList* neighbors = this->NeighborList[id];
    int n = neighbors->GetNumberOfIds();
    double scale = 1.0/(double(n));

    vtkDoubleArray* points = static_cast<vtkDoubleArray*>(
            this->Surface->GetPoints()->GetData());

    vtkIdType nb;
    for (int j=0; j<n; j++)
    {
        nb = neighbors->GetId(j);
        for (int i=0; i<3; i++)
            mean[i] += points->GetComponent(nb, i);
    }

    for (int i=0; i<3; i++)
        mean[i] = mean[i]*scale;
}
*/
// -------------------------------------------------------------------------------------------

double vtkjoBETImageFilter::ComputeSefIntersection(vtkPolyData* originalSurface)
{
    if (this->Surface == NULL || originalSurface == NULL)
        vtkErrorMacro(<<"Surface points or original points or both are NULL");

    double intersection = 0.0;

    // Get the points
    vtkPoints* currentPts = this->Surface->GetPoints();
    vtkPoints* originalPts = originalSurface->GetPoints();
    if (currentPts->GetNumberOfPoints() != originalPts->GetNumberOfPoints())
    {
        vtkErrorMacro(<<"Original surface points do not correspond the those of the current points");
        return 0.0;
    }

    // Compute the mean neighbor distance for each vertex
    double ml= this->ComputeMeanVertexSpacing(this->Surface);
    double mlo = this->ComputeMeanVertexSpacing(originalSurface);
    double ml2 = ml*ml;

    vtkIdType nPts = currentPts->GetNumberOfPoints();
    for (int i=0; i<nPts; i++) {
        for (int j=0; j<nPts; j++) {
            if (i != j && this->EdgeTable->IsEdge(i, j) == -1) {
                double currPt[3], currNPt[3];
                currentPts->GetPoint(i, currPt);
                currentPts->GetPoint(j, currNPt);

                if (vtkMath::Distance2BetweenPoints(currPt, currNPt) < ml2) {
                    double origPt[3], origNPt[3];
                    originalPts->GetPoint(i, origPt);
                    originalPts->GetPoint(j, origNPt);

                    double currDist = sqrt(vtkMath::Distance2BetweenPoints(currPt, currNPt))/ml;
                    double origDist = sqrt(vtkMath::Distance2BetweenPoints(origPt, origNPt))/mlo;
                    intersection += (currDist - origDist)*(currDist - origDist);
                }
            }
        }
    }

    return intersection;
}

// -------------------------------------------------------------------------------------------

void vtkjoBETImageFilter::CreateStrippedImage(
        vtkPolyData* surface,
        vtkImageData* input,
        vtkImageData* output)
{
    vtkDebugMacro(<<"Creating extracted image...");

    if (surface == NULL || input == NULL)
        vtkErrorMacro(<<"Either surface points or source image (or both) is NULL.");

    vtkPolyDataToImageStencil* stencil = vtkPolyDataToImageStencil::New();
    stencil->SetInput(surface);
    stencil->SetInformationInput(input);
    stencil->Update();

    vtkImageReslice* reslice = vtkImageReslice::New();
    reslice->SetInput(input);
    reslice->SetStencil(stencil->GetOutput());
    reslice->SetBackgroundColor(0.0, 0.0, 0.0, 1.0);
    reslice->Update();

    output->ShallowCopy(reslice->GetOutput());

    stencil->Delete();
    reslice->Delete();
}

// -------------------------------------------------------------------------------------------

double vtkjoBETImageFilter::SingleIteration(
        const int iter,
        const int pass,
        vtkImageData* input,
        const double* spacing,
        const double* extent,
        double* points,
        float* normals,
        double* updatedPoints,
        const vtkIdType nPts,
        const double localThreshold,
        const double E,
        const double F,
        const double L,
        const double L2,
        double smoothingFactor)
{
    vtkDebugMacro(<<"Updating the surface: iteration: "<<iter);

    vtkDataArray* imgData = input->GetPointData()->GetScalars();

    double* point, *updatedPoint;
    float* normal;
    double mean[3], s[3], sn[3], st[3], probePt[3];
    double dot=0.0, abs_dot=0.0, r_inv=0.0, f_2=0.0, f_3=0.0,
		   Imin=0.0, Imax=0.0, intensity=0.0, t_l=0.0,
		   dist=0.0, maxDist=0.0;
    int offset = 0;
    vtkIdType index;

    // Update each point
    for (int i=0; i<nPts; i++) {
        offset = i*3;
    	point = &points[offset];
    	updatedPoint = &updatedPoints[offset];
        normal = &normals[offset];

        this->ComputeMeanPosition(points, this->NeighborList[i], mean);
//        this->ComputeMeanPositionOfNeighbors(i, mean);

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

        if (pass > 0 && dot > 0.0)
        {
        	f_2 *= smoothingFactor;
        	// f_2 = MIN(f_2, 1.0);
        	if (f_2 > 1.0)
        		f_2 = 1.0;
        }

        // Compute component update 3
        Imin = this->MedianIntensity;
        Imax = this->Threshold;

        for (int d=1; double(d)<this->MinIntensitySearchDist; d++)
        {
            for (int j=0; j<3; j++)
                probePt[j] = point[j] - double(d)*normal[j];

            index = input->FindPoint(probePt);
            if (index != -1) {
                intensity = imgData->GetComponent(index, 0);

                if (intensity < Imin)
                    Imin = intensity;

                if (double(d)<this->MaxIntensitySearchDist && intensity > Imax)
                        Imax = intensity;
            }
        }

        if (this->MinIntensityThresh > Imin)
        	Imin = this->MinIntensityThresh;
        if (this->MedianIntensity < Imax)
        	Imax = this->MedianIntensity;

        t_l = (Imax - this->MinIntensityThresh)*localThreshold + this->MinIntensityThresh;
        if ((Imax - this->MinIntensityThresh) > 0.0)
        	f_3 = 2.0*(Imin - t_l)/(Imax - this->MinIntensityThresh);
        else
         	f_3 = 2.0*(Imin - t_l);

        // Perform update
        for (int j=0; j<3; j++)
        {
            updatedPoint[j] = point[j] + 0.5*st[j] + f_2*sn[j] + 0.05*f_3*L*normal[j];
            if (updatedPoint[j] <= extent[2*j])
                updatedPoint[j] = extent[2*j] + 2;
            else if (updatedPoint[j] >= extent[2*j+1])
                updatedPoint[j] = extent[2*j+1] - 2;
        }

        dist = vtkMath::Distance2BetweenPoints(point, updatedPoint);
        if (dist > maxDist)
            maxDist = dist;
    }

    for (int i=0; i<nPts; i++)
    {
        offset = i*3;
        for (int j=0; j<3; j++)
        {
            points[offset+j] = updatedPoints[offset+j];
        }
    }

    return maxDist;
}

// -------------------------------------------------------------------------------------------

void vtkjoBETImageFilter::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
    vtkDebugMacro(<<"Executing BET image filter");

    if (input == NULL)
    {
        vtkErrorMacro(<<"Input is NULL");
        return;
    }

    this->UpdateProgress(0.0);

    // Setup and perform image preprocessing

    this->BETHistogram->SetInput(input);
    this->BETHistogram->SetMinIntensityPercent(this->GetMinIntensityPercent());
    this->BETHistogram->SetMaxIntensityPercent(this->GetMaxIntensityPercent());
    this->BETHistogram->SetBins(this->GetBins());
    this->BETHistogram->Update();

    this->Threshold = this->BETHistogram->GetThreshold();
    this->MinIntensityThresh = this->BETHistogram->GetMinIntensity();
    this->MaxIntensityThresh = this->BETHistogram->GetMaxIntensity();
    this->MedianIntensity = this->BETHistogram->GetMedianIntensity();

    if (this->GetManualRadiusAndCOG() == 0)
    {
        this->Radius = this->BETHistogram->GetRadius();
        this->SetCenterOfGravity(this->BETHistogram->GetCenterOfGravity());
    }

    this->PrintSelf(cout, vtkIndent());

    // Initialize the surface.  Use a sphere based on calculations
    // from the histogram if no other surface is provided by the user.
    if (this->SurfaceInput == NULL)
    {
        this->SurfaceInput = this->CreateInitialSurface();
    }

    // Copy the initial/input surface so we are working with a local copy
    this->Surface = vtkPolyData::New();
    this->Surface->DeepCopy(this->SurfaceInput);

    this->FindNeighbors();
    vtkIdType numEdges = this->CreateEdgeTable();

    double localThresholdConst = pow(this->LocalThresholdConst, 0.275);

    // Stuff for increased smoothing
    vtkPolyData* originalSurface;
    if (this->IncreasedSmoothing) {
        // Make a copy of the original surface for use later
        // in calculating "spherecity"
        originalSurface = this->CreateInitialSurface();
    }
    else
        this->MaxPasses = 1;

    // Do a bunch of setup for optimal performance
    double spacing[3]; input->GetSpacing(spacing);
    int inExtent[6]; input->GetWholeExtent(inExtent);

    double extent[6];
    for (int i=0; i<3; i++)
    {
        extent[2*i] = double(inExtent[2*i])*spacing[i];
        extent[2*i+1] = double(inExtent[2*i+1])*spacing[i];
    }

    // Precompute E and F for the second update component
    double E = 0.5*((1/this->RMin) + (1/this->RMax));
    double F = 6.0/((1/this->RMin) - (1/this->RMax));
    double L = 0.0, L2 = 0.0;
    double smoothingFactor = 1.0;

    // Get the set of surface points and a copy of them and convert to pointer arrays
    vtkDoubleArray* pointsArray = (vtkDoubleArray*) this->Surface->GetPoints()->GetData();
    double* points_ptr = pointsArray->GetPointer(0);
    vtkIdType nPts = pointsArray->GetNumberOfTuples();

    vtkPoints* updatedPoints = vtkPoints::New();
    updatedPoints->SetDataTypeToDouble();
    updatedPoints->SetNumberOfPoints(nPts);
    double* updatedPoints_ptr = (double*) updatedPoints->GetData()->GetVoidPointer(0);

    // Create a single normal filter
    vtkPolyDataNormals* normalFilter = vtkPolyDataNormals::New();
    normalFilter->SplittingOff();
    normalFilter->FlipNormalsOn();
    normalFilter->ComputePointNormalsOn();

    // Record the maximum distance changed
    double maxDist=0.0;


    for (int j=0; j<this->MaxPasses; j++)
    {
        double scale = 1.0/double(this->MaxIterations);

        for (int i=0; i<this->MaxIterations; i++)
        {
            // Find the mean connection length, occasionally
            if (i == 50 || i%100 == 0)
            {
                L = this->ComputeMeanVertexSpacing(this->Surface);
                fprintf(stderr, "%d: Mean Vertex Spacing = %f\n", i, L);
                L2 = L*L;
            }

            if (j > 0)
            {
                double tempFactor = pow(10.0, double(j+1));
                if (i > double(this->MaxIterations)*0.75)
                    smoothingFactor = 4.0*(1.0 - double(i)*scale)*(tempFactor - 1.0) + 1.0;
                else
                    smoothingFactor = tempFactor;
            }

            // Compute the surface normals
            normalFilter->SetInput(this->Surface);
            normalFilter->Update();

            // TODO: probably make this a template to handle the case when normals
            // switch over to doubles in future VTK releases
            vtkDataArray* normalsArray = normalFilter->GetOutput()->GetPointData()->GetNormals();
            float* normals_ptr = (float*) (normalsArray->GetVoidPointer(0));

            maxDist = this->SingleIteration(
                    i, j,
                    input,
                    spacing,
                    extent,
                    points_ptr,
                    normals_ptr,
                    updatedPoints_ptr,
                    nPts,
                    localThresholdConst,
                    E, F, L, L2,
                    smoothingFactor);

            this->UpdateProgress(double(i+1)/double(this->MaxIterations));
        }

        if (this->IncreasedSmoothing)
        {
            double self_intersection = this->ComputeSefIntersection(originalSurface);
            fprintf(stderr, "self_intersection=%f\n", self_intersection);

            if (self_intersection <= this->SelfIntersectionThreshold)
                j = this->MaxPasses;
            else {
                vtkDebugMacro(<<"Rerunning with higher smoothness constraint");
                fprintf(stderr, "Rerunning with higher smoothness constraint\n");

//                double* origPts_ptr = (double*) originalPts->GetData()->GetVoidPointer(0);
//                int offset = 0;
//                for (int k=0; k<nPts; k++) {
//                    offset = k*3;
//                    for (int m=0; m<3; m++) {
//                        points_ptr[offset+m] = origPts_ptr[offset+m];
//                    }
//                }
            }
        }
    }

    this->UpdateProgress(1.0);

    this->CreateStrippedImage(this->Surface, input, output);

    // Cleanup...
    normalFilter->Delete();
    updatedPoints->Delete();
    if (this->IncreasedSmoothing)
        originalSurface->Delete();
}



