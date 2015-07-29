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
 * vtkjoPolyDataRobustBETTraining.h
 *
 *  Created on: Feb 6, 2009
 *      Author: jonofrey
 */

#ifndef VTKJOPOLYDATAROBUSTBETTRAINING_H_
#define VTKJOPOLYDATAROBUSTBETTRAINING_H_

#include "vtkFloatArray.h"
#include "vtkImageAppendComponents.h"
#include "vtkImageData.h"
#include "vtkPolyData.h"
#include "vtkPolyDataAlgorithm.h"

class vtkjoPolyDataRobustBETTraining : public vtkPolyDataAlgorithm {
public:
    static vtkjoPolyDataRobustBETTraining *New();
    vtkTypeMacro(vtkjoPolyDataRobustBETTraining, vtkPolyDataAlgorithm);

    void PrintSelf(ostream &os, vtkIndent indent);

    // Description:
    // Get/Set the source image.
    virtual void SetSrcImage(vtkImageData* image);

    // Description:
    // Add source image to the stack of training images.  The added
    // image will be appended as a component to the end of the
    // current image stack.  Use the NormalizeInputs flag to turn
    // normalization on/off.
    virtual void AddInput(vtkImageData* image);

    virtual vtkImageData* GetInputImage();

    // Description:
    // Get the calculated information as an image.
    virtual vtkImageData* GetOutputAsImage();

    virtual double GetMin(const char* arrayName);
    virtual double GetMax(const char* arrayName);

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
    // MinSearchDist - the distance searched for the minimum intensity
    // from a surface point (d1 in the BET paper).  Default d1 = 7mm.
    vtkSetMacro(OutMinIntensitySearchDist, double);
    vtkGetMacro(OutMinIntensitySearchDist, double);
    // Description:
    // MaxSearchDist - the distance searched for the maximum intensity
    // from a surface point (d2 in the BET paper).  Default d2 = 0.5*d1.
    vtkSetMacro(OutMaxIntensitySearchDist, double);
    vtkGetMacro(OutMaxIntensitySearchDist, double);
    // Description:
    // LocalThresholdConst - fractional constant (b_t in the BET paper)
    // used when computing the locally appropriate intensity threshold t_l.
    // Default b_t = 0.5;
    vtkSetClampMacro(LocalThresholdConst, double, 0.0, 1.0);
    vtkGetMacro(LocalThresholdConst, double);

    // Description:
    // Normalize the input images as they are added to the input image
    // stack (default is On).
    vtkSetClampMacro(NormalizeInputs, int, 0, 1);
    vtkGetMacro(NormalizeInputs, int);
    vtkBooleanMacro(NormalizeInputs, int);

    // Description:
    // Normalize the input images as they are added to the input image
    // stack (default is On).
    vtkSetClampMacro(SearchOutwards, int, 0, 1);
    vtkGetMacro(SearchOutwards, int);
    vtkBooleanMacro(SearchOutwards, int);

protected:
    vtkImageData* SrcImage;
    vtkImageAppendComponents* Appender;

    double MinIntensitySearchDist;
    double MaxIntensitySearchDist;
    double OutMinIntensitySearchDist;
    double OutMaxIntensitySearchDist;
    double LocalThresholdConst;

    int NormalizeInputs;
    int SearchOutwards;

    virtual void ComputeStatistics(vtkDataArray* data,
            vtkDataArray* means,
            vtkDataArray* vars);

//    virtual vtkFloatArray* ComputeF2(vtkPolyData* surface);

    vtkjoPolyDataRobustBETTraining();
    virtual ~vtkjoPolyDataRobustBETTraining();

    virtual int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*);
};

#endif /* VTKJOPOLYDATAROBUSTBETTRAINING_H_ */

