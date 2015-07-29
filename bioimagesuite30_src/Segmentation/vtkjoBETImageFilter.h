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
 * vtkjoBETImageFilter.h
 *
 *  Created on: Sep 12, 2008
 *      Author: jonofrey
 */

#ifndef VTKJOBETIMAGEFILTER_H_
#define VTKJOBETIMAGEFILTER_H_

#include "vtkEdgeTable.h"
#include "vtkImageData.h"
#include "vtkjoBETHistogram.h"
#include "vtkPolyData.h"
#include "vtkPolyDataNormals.h"
#include "vtkSimpleImageToImageFilter.h"

class vtkjoBETImageFilter : public vtkSimpleImageToImageFilter
{
public:
    static vtkjoBETImageFilter *New();
    vtkTypeMacro(vtkjoBETImageFilter, vtkSimpleImageToImageFilter);

    void PrintSelf(ostream& os, vtkIndent indent);

    // Description:
    // Set the initial surface geometry from which the BET
    // algorithm will start running.  If no surface is provided here
    // BET proceeds as normally by creating an initial sphere surface
    // based on parameters found creating the histogram.
    vtkSetObjectMacro(SurfaceInput, vtkPolyData);

    // Description:
    // Returns the BET brain surface geometry mesh.
    vtkGetObjectMacro(Surface, vtkPolyData);

    // Description:
    // Returns the binary image mask of the extracted brain.
    vtkImageData* GetOutputMask();

    // Description:
    // MinIntensityPercent - the lower percentage of the cumulative histogram
    // used to determine MinIntensity.  We limit the minimum intensity
    // percentage to be between 0.0 and 1.0.  The default is 0.02.
    vtkSetClampMacro(MinIntensityPercent, double, 0.0, 1.0);
    vtkGetMacro(MinIntensityPercent, double);

    // Description:
    // MaxIntensityPercent - the upper percentage of the cumulative histogram
    // used to determine MaxIntensity.  We limit the maximum intensity
    // percentage to be between 0.0 and 1.0.  The default is 0.98.
    vtkSetClampMacro(MaxIntensityPercent, double, 0.0, 1.0);
    vtkGetMacro(MaxIntensityPercent, double);

    // Description:
    // MinIntensityThresh - the robust minimum intensity threshold below which
    // lies MinIntensityPercent percent of the cumulative histogram.
    vtkGetMacro(MinIntensityThresh, double);
    // Description:
    // MaxIntensityThresh - the robust maximum intensity threshold above which
    // lies MaxIntensityPercent percent of the cumulative histogram.
    vtkGetMacro(MaxIntensityThresh, double);
    // Description:
    // Threshold - set to be 10% of the way between MinIntensityThresh
    // and MaxIntensityThresh.
    vtkGetMacro(Threshold, double);

    // Description:
    // Bins - the number of histogram bins.  We limit the number of bins to be
    // between 1 and 1024.
    vtkSetClampMacro(Bins, int, 1, 1024);
    vtkGetMacro(Bins, int);

    // Description:
    // Center Of Gravity - the center of gravity (COG) of the brain/head in the
    // image.  Only voxels with intensity greater than Threshold and less than
    // MaxIntensityThresh are used to compute this.
    vtkGetVectorMacro(CenterOfGravity, double, 3);
    vtkSetVector3Macro(CenterOfGravity, double);

    // Description:
    // Radius - the estimated "mean' radius of the brain/head volume.  All voxels
    // with intensity greater than Threshold are counted.
    vtkSetMacro(Radius, double);
    vtkGetMacro(Radius, double);

    // Description:
    // MedianIntensity - the median intensity within a sphere of the estimated
    // radius and centered on the COG.
    vtkGetMacro(MedianIntensity, double);

    // Description:
    // ManualRadiusAndCOG - the filter uses the radius and center of gravity
    // manually specified by the user instead of those automatically found
    // during image preprocessing.
    vtkSetClampMacro(ManualRadiusAndCOG, int, 0, 1);
    vtkGetMacro(ManualRadiusAndCOG, int);
    vtkBooleanMacro(ManualRadiusAndCOG, int);

    // Description:
    // RadiusScaleFactor - constant factor to scale the InitialRadius by.  Default
    // value set to 0.5;
    vtkSetMacro(RadiusScaleFactor, double);

    // Description:
    // SurfaceResolution - sets the number of surface points to use.  This value
    // represents the number of times an icosahedron's faces are subdivided into
    // 4 triangles.
    vtkSetClampMacro(SurfaceResolution, int, 0, 8);

    // Description:
    // MaxIterations - the maximum number of iterations for the algorithm
    // to run.
    vtkSetMacro(MaxIterations, int);

    // Description:
    // MaxPasses - the maximum number of passes for the algorithm
    // to run.
    vtkSetMacro(MaxPasses, int);

    // Description:
    // MinChangeTolerance - the minimum allowable maximum change in point distance
    // shift before the algorithm stops.
    vtkSetMacro(MinChangeTolerance, double);

    // Description:
    // MinSearchDist - the distance searched for the minimum intensity
    // from a surface point (d1 in the BET paper).  Default d1 = 7mm.
    vtkSetMacro(MinIntensitySearchDist, double);
    vtkGetMacro(MinIntensitySearchDist, double);
    // Description:
    // MaxSearchDist - the distance searched for the maximum intensity
    // from a surface point (d2 in the BET paper).  Default d2 = 0.5*d1.
    vtkSetMacro(MaxIntensitySearchDist, double);
    vtkGetMacro(MaxIntensitySearchDist, double);

    // Description:
    // LocalThresholdConst - fractional constant (b_t in the BET paper)
    // used when computing the locally appropriate intensity threshold t_l.
    // Default b_t = 0.5;
    vtkSetClampMacro(LocalThresholdConst, double, 0.0, 1.0);
    vtkGetMacro(LocalThresholdConst, double);

    // Description:
    // RMin - the minimum local radius used for smoothing (default 3.33mm).
    vtkSetMacro(RMin, double);
    // Description:
    // RMax - the maximum local radius used for smoothing (default 10mm).
    vtkSetMacro(RMax, double);

    // Description:
    // IncreasedSmoothing - tells the algorithm to used increased smoothing.
    vtkSetMacro(IncreasedSmoothing, int);
    vtkGetMacro(IncreasedSmoothing, int);
    vtkBooleanMacro(IncreasedSmoothing, int);

    // Description:
    // SelfIntersectionThreshold - threshold for the surface self intersections.
    vtkSetMacro(SelfIntersectionThreshold, double);

//    virtual double ComputeMeanVertexSpacing();
//    virtual void ComputeMeanPositionOfNeighbors(vtkIdType id, double mean[3]);

protected:

    // Stuff from vtkjoBETHistogram...
    // ======================================================
    double MinIntensityPercent;
    double MaxIntensityPercent;
    double Threshold;
    double MinIntensityThresh;
    double MaxIntensityThresh;
    int Bins;
    double Radius;
    double CenterOfGravity[3];
    double MedianIntensity;

    int ManualRadiusAndCOG;
    vtkjoBETHistogram* BETHistogram;

    // Stuff from vtkjoBETSurface...
    // ======================================================
    double RadiusScaleFactor;
    int SurfaceResolution;
    vtkPolyData* SurfaceInput;
    vtkPolyData* Surface;
    vtkIdList** NeighborList;
    vtkEdgeTable* EdgeTable;

    int MaxIterations;
    int MaxPasses;
    double MinChangeTolerance;

    double MaxIntensitySearchDist;
    double MinIntensitySearchDist;
    double LocalThresholdConst;

    double RMin;
    double RMax;

    int IncreasedSmoothing;
    double SelfIntersectionThreshold;

    vtkjoBETImageFilter();
    virtual ~vtkjoBETImageFilter();

    // Setup methods
    virtual vtkPolyData* CreateInitialSurface();
    virtual void FindNeighbors();
    virtual vtkIdType CreateEdgeTable();

    // BET-related methods
    virtual double ComputeMeanVertexSpacing(vtkPolyData* surface);
    virtual void ComputeMeanPosition(
            const double* points,
            vtkIdList* neighbors,
            double mean[3]);
    virtual double ComputeSefIntersection(
            vtkPolyData* originalSurface);
    virtual void CreateStrippedImage(
            vtkPolyData* surface,
            vtkImageData* input,
            vtkImageData* output);
    virtual double SingleIteration(
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
            double smoothingFactor);

    virtual void SimpleExecute(vtkImageData* input, vtkImageData *output);
};

#endif /* VTKJOBETIMAGEFILTER_H_ */

