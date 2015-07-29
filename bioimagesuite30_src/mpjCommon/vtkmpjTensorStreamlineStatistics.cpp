

//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
//BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


#include "vtkmpjTensorStreamlineStatistics.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyLine.h"

vtkStandardNewMacro(vtkmpjTensorStreamlineStatistics);

// Construct object 
vtkmpjTensorStreamlineStatistics::vtkmpjTensorStreamlineStatistics()
{
  this->NumberOfFibers = 0;

  // min/max/mean/variance
  for(int i=0;i<4;i++) {
    this->Anisotropy[i] = 0.0;
    this->Diffusivity[i] = 0.0;
    this->Distance[i] = 0.0;
    this->Angle[i] = 0.0;
    this->Other[i] = 0.0;
  }
  
}

void vtkmpjTensorStreamlineStatistics::Execute()
{
  vtkPolyData *input = this->GetInput();
  //vtkPolyData *output = this->GetOutput();
  vtkPointData *pd=input->GetPointData();
  
  //vtkPointData *outPD=output->GetPointData();
  vtkCellArray *inLines = NULL;
  
  vtkPoints *inPts;
  vtkIdType numPts = 0;
  vtkIdType numLines;
  
  float fractanisum, fractanisumsqr;
  float meandiffsum, meandiffsumsqr;
  float distancesum, distancesumsqr;
  float anglesum, anglesumsqr;
  float othersum, othersumsqr;

  vtkIdType npts=0, *pts=NULL;
  int abort=0;
  vtkIdType inCellId;

  vtkDataArray *fa = pd->GetArray("fractani");
  vtkDataArray *md = pd->GetArray("meandiff");
  vtkDataArray *dist = pd->GetArray("distance");
  vtkDataArray *angle = pd->GetArray("angle");  
  vtkDataArray *other = pd->GetArray("other");
 
  // Check input and initialize
  //  
  if ( !(inPts=input->GetPoints()) || 
       (numPts = inPts->GetNumberOfPoints()) < 1 ||
       !(inLines = input->GetLines()) || 
       (numLines = inLines->GetNumberOfCells()) < 1 )
    {
      return;
    }
  
  float* fractani_array = new float[numLines];
  float* meandiff_array = new float[numLines];
  float* distance_array = new float[numLines];
  float* angle_array = new float[numLines];
  float* other_array = new float[numLines]; 
  
  for (inCellId=0, inLines->InitTraversal(); 
       inLines->GetNextCell(npts,pts) && !abort; inCellId++)
    {
      this->UpdateProgress((float)inCellId/numLines);
      abort = this->GetAbortExecute();

      if (npts > 1) {
	
	fractanisum = 0.0; 
	meandiffsum = 0.0; 
	anglesum = 0.0; 
	othersum = 0.0;
	
  //printf("n=%d\n",npts);
	// compute statistics for fiber
	for(int i=0; i<npts; i++) {	
		  
	  fractanisum += fa->GetTuple1(pts[i]);
	  meandiffsum += md->GetTuple1(pts[i]);
	  anglesum += angle->GetTuple1(pts[i]);
    if (std::isfinite(other->GetTuple1(pts[i])))
	     othersum += other->GetTuple1(pts[i]);	  

    //printf("%d fasum=%lf, mdsum=%lf, anglesm=%lf, othersum=%lf\n", i, fractanisum, meandiffsum, anglesum, othersum);
	  //	  printf("fasum=%f, osum=%f\n",fractanisum, othersum);

	  //	  printf("anglesum=%f,%f\n",anglesum,angle->GetTuple1(pts[i]));

	}
	
	// compute mean values for fiber
	fractanisum /= (float)npts;
	meandiffsum /= (float)npts;
	anglesum /= (float)npts;
	othersum /= (float)npts;
	
	// store values
	fractani_array[inCellId] = fractanisum;
	meandiff_array[inCellId] = meandiffsum;
	distance_array[inCellId] = dist->GetTuple1(pts[npts-1]);
	angle_array[inCellId] = anglesum;
	other_array[inCellId] = othersum;

      } else {
	printf("npts=%d\n",npts);
      }
      
    }
  
  this->NumberOfFibers = numLines;

  fractanisum = 0.0; fractanisumsqr = 0.0;
  meandiffsum = 0.0; meandiffsumsqr = 0.0;
  distancesum = 0.0; distancesumsqr = 0.0;
  anglesum = 0.0; anglesumsqr = 0.0;
  othersum = 0.0; othersumsqr = 0;
  
  this->Anisotropy[0] = fractani_array[0];  // min FA
  this->Anisotropy[1] = fractani_array[0];  // max FA
  
  this->Diffusivity[0] = meandiff_array[0];  // min MD
  this->Diffusivity[1] = meandiff_array[0];  // max MD
  
  this->Distance[0] = distance_array[0];  // min distance
  this->Distance[1] = distance_array[0];  // max distance
  
  this->Angle[0] = angle_array[0];  // min angle
  this->Angle[1] = angle_array[0];  // max angle

  this->Other[0] = other_array[0];  // min other
  this->Other[1] = other_array[0];  // max other

  // compute statistics for the bundle
  for(int i=0; i<numLines; i++) {
    
    // min/max
    if ( fractani_array[i] < this->Anisotropy[0]) {
      this->Anisotropy[0] = fractani_array[i];
    } 
    if (fractani_array[i] > this->Anisotropy[1]) {
      this->Anisotropy[1] = fractani_array[i];
    }
    
    // min/max
    if ( meandiff_array[i] < this->Diffusivity[0]) {
      this->Diffusivity[0] = meandiff_array[i];
    } 
    if (meandiff_array[i] > this->Diffusivity[1]) {
      this->Diffusivity[1] = meandiff_array[i];
    }

    // min/max
    if ( distance_array[i] < this->Distance[0]) {
      this->Distance[0] = distance_array[i];
    } 
    if (distance_array[i] > this->Distance[1]) {
      this->Distance[1] = distance_array[i];
    }
    
    // min/max
    if (angle_array[i] < this->Angle[0]) {
      this->Angle[0] = angle_array[i];
    } 
    if (angle_array[i] > this->Angle[1]) {
      this->Angle[1] = angle_array[i];
    }
    
    // min/max
    if ( other_array[i] < this->Other[0]) {
      this->Other[0] = other_array[i];
    } 
    if (other_array[i] > this->Other[1]) {
      this->Other[1] = other_array[i];
    }
    
    // sum/sumsqr
    fractanisum += fractani_array[i];
    fractanisumsqr += fractani_array[i]*fractani_array[i];
    //printf("fractanisum=%f,%f\n",fractanisum,fractani_array[i]);

    // sum/sumsqr
    meandiffsum += meandiff_array[i];
    meandiffsumsqr += meandiff_array[i]*meandiff_array[i];
    //printf("meandiff=%f,%f\n",meandiffsum,meandiff_array[i]);

    // sum/sumsqr
    distancesum += distance_array[i];
    distancesumsqr += distance_array[i]*distance_array[i];
    //printf("fractanisum=%f,%f\n",distancesum,distance_array[i]);
    
    // sum/sumsqr
    anglesum += angle_array[i];
    anglesumsqr += angle_array[i]*angle_array[i];
    //printf("anglesum=%f,%f\n",anglesum,angle_array[i]);
 
    // sum/sumsqr
    othersum += other_array[i];
    othersumsqr += other_array[i]*other_array[i];
  }
  
  delete [] fractani_array;
  delete [] meandiff_array;
  delete [] distance_array;
  delete [] angle_array;
  delete [] other_array;

  // compute mean
  this->Anisotropy[2] = fractanisum / (float)numLines;
  this->Diffusivity[2] = meandiffsum / (float)numLines;
  this->Distance[2] = distancesum / (float)numLines;
  this->Angle[2] = anglesum / (float)numLines;
  this->Other[2] = othersum / (float)numLines;

  //printf("numLines=%d\n",numLines);
  //printf("stats1: %f %f %f %f %f\n",fractanisum,meandiffsum,distancesum,anglesum,othersum);
  //printf("stats2: %f %f %f %f %f\n",fractanisumsqr,meandiffsumsqr,distancesumsqr,anglesumsqr,othersumsqr);
  
  // compute variance
  if (numLines > 1) {
    this->Anisotropy[3] = (fractanisumsqr - 
			   (((float)numLines)*this->Anisotropy[2]*this->Anisotropy[2]))/((float)numLines-1);
    this->Diffusivity[3] = (meandiffsumsqr - 
			    (((float)numLines)*this->Diffusivity[2]*this->Diffusivity[2]))/((float)numLines-1);
    this->Distance[3] = (distancesumsqr - 
			 (((float)numLines)*this->Distance[2]*this->Distance[2]))/((float)numLines-1);
    this->Angle[3] = (anglesumsqr - 
		      (((float)numLines)*this->Angle[2]*this->Angle[2]))/((float)numLines-1);
    this->Other[3] = (othersumsqr - 
		      (((float)numLines)*this->Other[2]*this->Other[2]))/((float)numLines-1);
  }
 
}

