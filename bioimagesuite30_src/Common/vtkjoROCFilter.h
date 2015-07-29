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
 * vtkjoROCFilter.h
 *
 *  Created on: Dec 1, 2008
 *      Author: jonofrey
 */

#ifndef VTKJOROCFILTER_H_
#define VTKJOROCFILTER_H_

#include "vtkSimpleImageToImageFilter.h"

class vtkjoROCFilter : public vtkSimpleImageToImageFilter
{
public:
    static vtkjoROCFilter *New();
    vtkTypeMacro(vtkjoROCFilter, vtkSimpleImageToImageFilter);

    void PrintSelf(ostream& os, vtkIndent indent);

    // Description:
    // Set the true result to compare the input against.
    void SetTrueResult(vtkImageData* truth) { this->TrueResult = truth; };
    vtkImageData* GetTrueResult() { return this->TrueResult; };

    // Description:
    // Get the number of True Positives.
    vtkGetMacro(TP, int);
    // Description:
    // Get the number of False Positives.
    vtkGetMacro(FP, int);
    // Description:
    // Get the number of True Negatives.
    vtkGetMacro(TN, int);
    // Description:
    // Get the number of False Negatives.
    vtkGetMacro(FN, int);

    // Description:
    // Get the True Positive Rate (Sensitivity) = TP/(TP + FN).
    double GetTruePositiveRate();

    // Description:
    // Get the False Positive Rate = FP/(FP + TN).
    double GetFalsePositiveRate();

    // Description:
    // Get the Accuracy = (TP + TN)/(P + N).
    double GetAccuracy();

    // Description:
    // Get the Specificity = TN/(FP + TN) = 1-FPR.
    double GetSpecificity();

    // Description:
    // Get the Positive Predictive Value (Precision) = TP/(TP + FP).
    double GetPositivePredictiveValue();

    // Description:
    // Get the Negative Predictive Value = TN/(TN + FN).
    double GetNegativePredictiveValue();

    // Description:
    // Get the False Discovery Rate = FP/(FP + TP).
    double GetFalseDiscoveryRate();

protected:
    vtkjoROCFilter();
    ~vtkjoROCFilter() {};

    vtkImageData* TrueResult;

    int TP;
    int TN;
    int FP;
    int FN;

    virtual int RequestInformation(vtkInformation*, vtkInformationVector**, vtkInformationVector*);
//    virtual void SimpleExecute(vtkInformation*, vtkInformationVector**, vtkInformationVector*);
    virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);
};

#endif /* VTKJOROCFILTER_H_ */

