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

#include <stdio.h>
#include <math.h>
#include "vtkjoBETHistogram.h"
#include "vtkDataArray.h"
#include "vtkDataObject.h"
#include "vtkFloatArray.h"
#include "vtkObjectFactory.h"
#include "vtkImageAccumulate.h"
#include "vtkImageData.h"
#include "vtkImageMask.h"
#include "vtkImageMedian3D.h"
#include "vtkImageThreshold.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkPointData.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkSortDataArray.h"
#include "vtkStreamingDemandDrivenPipeline.h"



vtkjoBETHistogram* vtkjoBETHistogram::New()
{
	// First try to create the object from the vtkObjectFactory
	vtkObject* ret = vtkObjectFactory::CreateInstance("vtkjoBETHistogram");
	if (ret)
	{
		return (vtkjoBETHistogram*) ret;
	}
	// If the factory was unable to create the object, then create it here.
	return new vtkjoBETHistogram;
}


void vtkjoBETHistogram::PrintSelf(ostream& os, vtkIndent indent)
{
	vtkSimpleImageToImageFilter::PrintSelf(os, indent);

	os << indent << "  MinIntensityPercent " << this->MinIntensityPercent << "\n";
	os << indent << "  MaxIntensityPercent " << this->MaxIntensityPercent << "\n";
	os << indent << "  Bins " << this->Bins << "\n";
	os << indent << "  MinIntensity " << this->MinIntensity << "\n";
	os << indent << "  MaxIntensity " << this->MaxIntensity << "\n";
	os << indent << "  Threshold " << this->Threshold << "\n";
	os << indent << "  COG.x " << this->CenterOfGravity[0] << "\n";
	os << indent << "  COG.y " << this->CenterOfGravity[1] << "\n";
	os << indent << "  COG.z " << this->CenterOfGravity[2] << "\n";
	os << indent << "  Radius " << this->Radius << "\n";
	os << indent << "  MeanIntensity " << this->MeanIntensity << "\n";
	os << indent << "  MedianIntensity " << this->MedianIntensity << "\n";
}


vtkjoBETHistogram::vtkjoBETHistogram()
{
	this->MinIntensityPercent = 0.02;
	this->MaxIntensityPercent = 0.98;

	this->Bins = 1000;

	this->MinIntensity = 0.0;
	this->MaxIntensity = 0.0;
	this->Threshold = 0.0;

	// TODO: Is this the proper initialization?
	this->CenterOfGravity[0] = 0.0;
	this->CenterOfGravity[1] = 0.0;
	this->CenterOfGravity[2] = 0.0;

	this->Radius = 0.0;
	this->MeanIntensity = 0.0;
	this->MedianIntensity = 0.0;
}


vtkjoBETHistogram::~vtkjoBETHistogram()
{
}


void vtkjoBETHistogram::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
	vtkDebugMacro(<<"Entering SimpleExecute");

	if (input == NULL)
	{
		vtkErrorMacro(<<"Input is NULL");
		return;
	}

	// Get the input image's range in order to determine spacing of the
	// histogram
	double range[2];
	input->GetPointData()->GetScalars()->GetRange(range);

	double spacing = (range[1] - range[0])/float(this->Bins-1);

	// Create the histogram
	vtkSmartPointer<vtkImageAccumulate> accumulator =
		vtkSmartPointer<vtkImageAccumulate>::New();
	accumulator->SetInput(input);
	accumulator->SetComponentExtent(0, this->Bins-1, 0, 0, 0, 0);
	accumulator->SetComponentOrigin(range[0], 0, 0);
	accumulator->SetComponentSpacing(spacing, 1, 1);
	accumulator->Update();

	vtkDataArray* data = accumulator->GetOutput()->GetPointData()->GetScalars();
	vtkSmartPointer<vtkFloatArray> cum =
		vtkSmartPointer<vtkFloatArray>::New();
	cum->SetNumberOfTuples(this->Bins);

	// Compute the cumulative distribution of the histogram
	cum->SetComponent(0, 0, data->GetComponent(0,0));
	for (int i=1; i<this->Bins; i++)
		cum->SetComponent(i, 0, data->GetComponent(i,0) + cum->GetComponent(i-1,0));

	// Get the total number of voxels
	double total = cum->GetComponent(this->Bins-1, 0);

	// Compute the "robust" thresholds
	bool foundMinThresh = false;
	bool foundMaxThresh = false;
	double value = 0.0;

	for (int i=0; i<this->Bins; i++)
	{
		value = cum->GetComponent(i, 0);
		if (!foundMinThresh && value/total > this->MinIntensityPercent)
		{
			foundMinThresh = true;
			this->MinIntensity = range[0] + i*spacing;
		}

		if (!foundMaxThresh && value/total > this->MaxIntensityPercent)
		{
			foundMaxThresh = true;
			this->MaxIntensity = range[0] + i*spacing;
		}
	}
	vtkDebugMacro(<<"MinIntensity="<<this->MinIntensity<<" MaxIntensity="<<this->MaxIntensity);

	// Find threshold 10% of the way between the min and max thresholds
//    this->Threshold = (this->MaxIntensity - this->MinIntensity)*0.10;
    this->Threshold = (this->MaxIntensity - this->MinIntensity)*0.10 + this->MinIntensity;
	vtkDebugMacro(<<"Threshold="<<this->Threshold);

	// Compute the CoG and the mean radius
	ComputeCoGandRadius(input, output);
}


// TODO: remove the output image from here...
/**
 * Computes the COG, the "mean" radius and the median intensity.  Voxels with
 * intensity greater than Theshold and less than MaxIntensity are used to compute
 * the COG.  All voxels with intensity greater than Threshold are used to find
 * the radius.  The median intensity is found for all points within a sphere
 * of the radius centered on the COG.
 */
void vtkjoBETHistogram::ComputeCoGandRadius(vtkImageData* image, vtkImageData* output)
{
	vtkDebugMacro(<<"Computing the CoG and Radius...");

	int dim[3];
	image->GetDimensions(dim);
	double spacing[3];
	image->GetSpacing(spacing);

	int slicesize = dim[0]*dim[1];

	vtkDataArray* data = image->GetPointData()->GetScalars();

	double range[2];
	data->GetRange(range);

	int countIn = 0;
	double totalMass = 0.0;

	double value = 0.0;
	double tempThresh = this->Threshold - this->MinIntensity;
	for (int z=0; z<dim[2]; z++)
		for (int y=0; y<dim[1]; y++)
			for (int x=0; x<dim[0]; x++)
			{
			    // TODO: BET uses the MinIntensity as a zero basis, so
			    // we added tempThresh here
//                value = data->GetComponent(x+y*dim[0]+z*slicesize, 0);
                value = data->GetComponent(x+y*dim[0]+z*slicesize, 0) - this->MinIntensity;

//                if (value > this->Threshold)
                if (value > tempThresh)
				{
					countIn++;
					// NOTE: BET does not use this upper limit even though
					// it is reported in the literature
					// TODO: probably add this back into our alg.
//					if (value <= this->MaxIntensity)
					{
						this->CenterOfGravity[0] += x*value;
						this->CenterOfGravity[1] += y*value;
						this->CenterOfGravity[2] += z*value;
						totalMass += value;
					}
				}
			}

	// Check to make sure the total mass is > 0 in order to avoid dividing by zero
	if (totalMass > 0.0)
	{
		this->CenterOfGravity[0] /= totalMass;
		this->CenterOfGravity[1] /= totalMass;
		this->CenterOfGravity[2] /= totalMass;
	}
	else
	{
		this->CenterOfGravity[0] = 0;
		this->CenterOfGravity[1] = 0;
		this->CenterOfGravity[2] = 0;
	}

	// use V=4/3*pi*r^3 to find the mean radius
	const double PI = vtkMath::DoublePi();
	// TODO: incorporate spacing information into the final calc?
	this->Radius = pow((0.75*double(countIn))/PI, 1.0/3.0);

	vtkDebugMacro(<<"COG.x="<<this->CenterOfGravity[0]<<" COG.y="<<this->CenterOfGravity[1]<<" COG.z="<<this->CenterOfGravity[2]);
	vtkDebugMacro(<<"Mean Radius="<<this->Radius);

	// Finally compute the median intensity within the mean radius
	// from the COG

	// TODO: remove me
	vtkDataArray* outData = output->GetPointData()->GetScalars();

	// Note, rather than using r > ((x-x0)^2+(y-y0)^2+...)^0.5
	// Use: r^2 > (x-x0)^2 + (y-y0)^2 in order to save on calculating the
	// square root every time
	double radiusThresh = this->Radius*this->Radius;
	double dist = 0.0;
	int spherePoints = 0;
	double sphereIntensity = 0.0;

	vtkDataArray* intensityArray = vtkDataArray::CreateDataArray(data->GetDataType());

	for (int z=0; z<dim[2]; z++)
		for (int y=0; y<dim[1]; y++)
			for (int x=0; x<dim[0]; x++)
			{
				dist = (x - this->CenterOfGravity[0])*(x - this->CenterOfGravity[0]) +
					   (y - this->CenterOfGravity[1])*(y - this->CenterOfGravity[1]) +
					   (z - this->CenterOfGravity[2])*(z - this->CenterOfGravity[2]);
				if (dist < radiusThresh)
				{
					value = data->GetComponent(x+y*dim[0]+z*slicesize, 0);

					// BET also performs a check for the intensity to be between
					// the min and max intensity thresholds.  NOTE, this is note
					// reported in the literature.
					if (value > this->MinIntensity && value < this->MaxIntensity)
					{
	                    intensityArray->InsertNextTuple1(value);
	                    sphereIntensity += value;
	                    spherePoints++;
					}
					outData->SetComponent(x+y*dim[0]+z*slicesize, 0, 1.0);
				}
				else
				{
					outData->SetComponent(x+y*dim[0]+z*slicesize, 0, 0.0);
				}
			}

	this->MeanIntensity = sphereIntensity/double(spherePoints);
	vtkDebugMacro(<<"MeanIntensity="<<this->MeanIntensity);

	vtkSortDataArray::Sort(intensityArray);

	// If we have an odd number of points take the middle tuple, else take the
	// average of the two middle tuples.
	int numElements = intensityArray->GetNumberOfTuples();
	if (numElements%2 != 0)
	{
		this->MedianIntensity = intensityArray->GetTuple1((numElements-1)/2);
	}
	else
	{
		int medianIndex = (numElements-1)/2;
		this->MedianIntensity = (intensityArray->GetTuple1(medianIndex-1) +
				intensityArray->GetTuple1(medianIndex))/2.0;
	}
	vtkDebugMacro(<<"MedianIntensity="<<this->MedianIntensity);


	double sum=0.0;
	for (int i=0;i<=2;i++)
	  {
	    this->CenterOfGravity[i]*=spacing[i];
	    sum+=spacing[i];
	  }
	sum=sum/3.0;
	this->Radius*=sum;
	vtkDebugMacro(<<"COG.x="<<this->CenterOfGravity[0]<<" COG.y="<<this->CenterOfGravity[1]<<" COG.z="<<this->CenterOfGravity[2]);
	vtkDebugMacro(<<"Mean Radius="<<this->Radius);

}


