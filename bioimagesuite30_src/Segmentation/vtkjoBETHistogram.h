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

#ifndef __vtkjoBETHistogram_h
#define __vtkjoBETHistogram_h

#include "vtkSimpleImageToImageFilter.h"


class vtkjoBETHistogram : public vtkSimpleImageToImageFilter
{
public:
	static vtkjoBETHistogram *New();
	vtkTypeMacro(vtkjoBETHistogram, vtkSimpleImageToImageFilter);

	void PrintSelf(ostream& os, vtkIndent indent);

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
	// MinIntensity - the robust minimum intensity below which lies
	// MinIntensityPercent percent of the cumulative histogram.
	vtkGetMacro(MinIntensity, double);
	// Description:
	// MaxIntensity - the robust maximum intensity above which lies
	// MaxIntensityPercent percent of the cumulative histogram.
	vtkGetMacro(MaxIntensity, double);
	// Description:
	// Threshold - set to be 10% of the way between MinIntensity
	// and MaxIntensity.
	vtkGetMacro(Threshold, double);

	// Description:
	// Bins - the number of histogram bins.  We limit the number of bins to be
	// between 1 and 512.
	vtkSetClampMacro(Bins, int, 1, 1024);
	vtkGetMacro(Bins, int);

	// Description:
	// Center Of Gravity - the center of gravity (COG) of the brain/head in the
	// image.  Only voxels with intensity greater than Threshold and less than
	// MaxIntensity are used to compute this.
	vtkGetVectorMacro(CenterOfGravity, double, 3);

	// Description:
	// Radius - the estimated "mean' radius of the brain/head volume.  All voxels
	// with intensity greater than Threshold are counted.
	vtkGetMacro(Radius, double);

	// Description:
	// MeanIntensity - the mean intensity within a sphere of the estimated
	// radius and centered on the COG.
	vtkGetMacro(MeanIntensity, double);
	// Description:
	// MedianIntensity - the median intensity within a sphere of the estimated
	// radius and centered on the COG.
	vtkGetMacro(MedianIntensity, double);

protected:

	double MinIntensityPercent;
	double MaxIntensityPercent;

	double Threshold;
	double MinIntensity;
	double MaxIntensity;

	int Bins;

	double Radius;
	double CenterOfGravity[3];
	double MeanIntensity;
	double MedianIntensity;

	vtkjoBETHistogram();
	virtual ~vtkjoBETHistogram();

	virtual void SimpleExecute(vtkImageData* input, vtkImageData *output);

//	virtual int RequestInformation(vtkInformation *vtkNotUsed(request),
//			vtkInformationVector **inputVector,
//			vtkInformationVector *outputVector);

//	void ComputeCenterOfGravity(vtkImageData* input);
	void ComputeCoGandRadius(vtkImageData* image, vtkImageData* output);
};

#endif


