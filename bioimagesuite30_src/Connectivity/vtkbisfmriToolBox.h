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
 * vtkbisfmriToolBox.cpp
 *
 *  Created on: May 21, 2010
 *      Author: Isabella Murphy,Xilin Shen
 *      This class compute complex network measures
 *      1. Adjacency matrix (brain network)
 *      2. Degrees
 *      3. Reachability matrix
 *      4. Shortest path length matrix
 *      5. Number of triangles
 *      6. Characteristic path length
 *      7. Global efficiency
 *      8. Clustering coefficient
 *      9. Transicity
 *      10. Local efficiency
 *      11. Closeness Centrality
 *      12. Betweeness centrality
 *      @[in]:image1 Functional Image.
 *      @[in]:image2 ROI Correlation Image
 *      @[in]:image3 ROI Map Image
 *      @[out]:image1 shortest path length. # of tuples=# of components=# of nodes
 *      @[out]:image2 network measurements. # of tuples=# of nodes, # of components=12
 *
 */

#ifndef VTKBISFMRITOOLBOX_H_
#define VTKBISFMRITOOLBOX_H_

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkIntArray.h"
#include "vtkpxMatrix.h"
#include "vtkFloatArray.h"
#include <string>
#include <iostream>

class vtkInformation;
class vtkInformationVector;

class vtkbisfmriToolBox : public vtkSimpleImageToImageFilter {
public:
	static vtkbisfmriToolBox *New();
	vtkTypeMacro(vtkbisfmriToolBox,vtkSimpleImageToImageFilter);

	//FunctionalImage Image
	vtkSetObjectMacro(FunctionalImage,vtkImageData);
	vtkGetObjectMacro(FunctionalImage,vtkImageData);

	//roiMap Image
	vtkSetObjectMacro(roiMap,vtkImageData);
	vtkGetObjectMacro(roiMap,vtkImageData);

	//OutData2 Image
	vtkSetObjectMacro(OutData2,vtkImageData);
	vtkGetObjectMacro(OutData2,vtkImageData);

	//Threshold
	vtkSetMacro(Threshold,float);
	vtkGetMacro(Threshold,float);

	// File name
	virtual void SetFilename(const char* filename);
	virtual const char* GetFilename();
	// File name
	virtual void SetFilename2(const char* filename2);
	virtual const char* GetFilename2();
	// File name
	virtual void SetFilename3(const char* filename3);
	virtual const char* GetFilename3();

	/*
	 * ComputeBinaryNetwork
	 * @[in]:FunctionalImage. Output of vtkbisTimeSeriesCorrelation, an ROI*ROI*1*1 image
	 * @[in]:threshold. If value of (ROI1,ROI2)>threshold, (ROI1,ROI2)of network=1
	 * @[out]:matrix. An binary matrix describe the connectiveness between ROIs
	 */
	static vtkFloatArray* ComputeBrainNetwork(vtkImageData* roiCorrelation, float threshold, int binary);
	static vtkIntArray* ComputeDegreesUnd(vtkFloatArray* brainNetwork);
	static float ComputeNumberOfTriangles(vtkFloatArray* brainNetwork,int tuple);
	static vtkpxMatrix* ComputeBetweenessCentrality(vtkFloatArray* brainNetwork);
	static vtkFloatArray* ComputeLocalEfficiency(vtkFloatArray* brainNetwork, vtkIntArray* degrees);
	static vtkFloatArray* ComputeNetworkMeasures(vtkFloatArray* brainNetwork,vtkImageData* brain, vtkImageData* ROI, vtkIntArray* degrees, vtkDataArray* out);
	static void GenerateOutputForAlarkViewer(vtkFloatArray* localE, vtkpxMatrix * BC, vtkFloatArray* otherData,const char* Filename2, vtkDataArray* out2);
	static void GenerateAdjMatrixOutputForAlarkViewer(vtkFloatArray* brainNetwork, vtkFloatArray* otherData, const char* Filename);
	static void GenerateROICorrelationOutputForAlarkViewer(vtkImageData* roiCorrelation, vtkFloatArray* otherData, const char* Filename3);

protected:

  vtkbisfmriToolBox();
  virtual ~vtkbisfmriToolBox();
  virtual int RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector,
    					  vtkInformationVector *outputVector);
  // Execute Function -- main function
  virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);

private:
	vtkbisfmriToolBox(const vtkbisfmriToolBox& src) {};
	vtkbisfmriToolBox& operator=(const vtkbisfmriToolBox& rhs) {};
	vtkImageData* FunctionalImage;
	vtkImageData* roiMap;
	vtkImageData* OutData2;
	float Threshold;
	//BTX
	std::string Filename;
	std::string Filename2;
	std::string Filename3;
	//ETX
};

#endif /* VTKBISFMRITOOLBOX_H_ */

