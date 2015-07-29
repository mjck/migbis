

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


#include "vtkmpjTensorStreamlineRegionFilter.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyLine.h"

vtkStandardNewMacro(vtkmpjTensorStreamlineRegionFilter);

// Construct object 
vtkmpjTensorStreamlineRegionFilter::vtkmpjTensorStreamlineRegionFilter()
{
  this->RegionNumber1 = 0;
  this->RegionNumber2 = 0;
  this->Clipping = 0;
}

void vtkmpjTensorStreamlineRegionFilter::Execute()
{
  vtkPolyData *input = this->GetInput();
  vtkPolyData *output = this->GetOutput();
  vtkPointData *pd=input->GetPointData();

  vtkPointData *outPD=output->GetPointData();
  vtkCellArray *inLines = NULL;

  vtkPoints *inPts;
  vtkIdType numPts = 0;
  vtkIdType numLines;
    
  float faval;
  float mdval;
  float distval;
  float angleval;
  float roival;
  float otherval;

  vtkIdType npts=0, *pts=NULL;
  int id, abort=0;
  vtkIdType inCellId;

  vtkDataArray *fa = pd->GetArray("fractani");
  vtkDataArray *md = pd->GetArray("meandiff");
  vtkDataArray *dist = pd->GetArray("distance");
  vtkDataArray *angle = pd->GetArray("angle");  
  vtkDataArray *roi = pd->GetArray("roi");
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
  
  // Create output data
  vtkPoints *newPts = vtkPoints::New();
  vtkCellArray *newCells = vtkCellArray::New();

  vtkFloatArray *newfa = vtkFloatArray::New();
  newfa->SetName("fractani");
  newfa->SetNumberOfComponents(1);

  vtkFloatArray *newmd = vtkFloatArray::New();
  newmd->SetName("meandiff");
  newmd->SetNumberOfComponents(1);

  vtkFloatArray *newdist = vtkFloatArray::New();
  newdist->SetName("distance");
  newdist->SetNumberOfComponents(1);

  vtkFloatArray *newangle = vtkFloatArray::New();
  newangle->SetName("angle");
  newangle->SetNumberOfComponents(1);

  vtkFloatArray *newroi = vtkFloatArray::New();
  newroi->SetName("roi");
  newroi->SetNumberOfComponents(1);

  vtkFloatArray *newother = vtkFloatArray::New();
  newother->SetName("other");
  newother->SetNumberOfComponents(1);

  for (inCellId=0, inLines->InitTraversal(); 
       inLines->GetNextCell(npts,pts) && !abort; inCellId++)
    {
      this->UpdateProgress((float)inCellId/numLines);
      abort = this->GetAbortExecute();
      
      if (npts < 2)
	{
	  vtkWarningMacro(<< "Less than two points in line!");
	  continue; //skip filtering this line
	}

      //      int index=-1;
      
      // traverse polyline and search for roi crossing
      /*for(int i=0; i<npts; i++) {	  
 	roival = roi->GetTuple1(pts[i]);	  
	if (roival == this->RegionNumber)
	{ index=i; break; }
	}*/
      
      // back-traverse polyline searching for ROI crossing
      //for(int i=npts-1; i>=0; i--) {	  
      //roival = roi->GetTuple1(pts[i]);	  
      //if (roival == this->RegionNumber)
      //  { index=i; break; }
      //}
      
      int index1=-1, index2=-1;

      // navigate polylines
      for(int i=0; i<npts; i++) {	  
 	roival = roi->GetTuple1(pts[i]);	  
	if (roival == this->RegionNumber1)
	  { index1=i; }
	if (roival == this->RegionNumber2)
	  { index2=i; }
      }

      // check if fiber crosses both ROIs
      if ((index1 >= 0) && (index2 >= 0)) {
	
	int startindex = 0;
	int endindex = npts-1;
	int NumNewPts = npts;
	
	if (this->Clipping) {
	  if (index1 > index2) {
	    startindex=index2; endindex=index1;
	    NumNewPts = index1 - index2 + 1;
	  } else {
	    startindex=index1; endindex=index2;	    
	    NumNewPts = index2 - index1 + 1;
	  }	    
	}	
	
	// allocate next cell
	newCells->InsertNextCell(NumNewPts);
	
	// create new polyline
	for(int i=startindex; i<=endindex; i++) {
	  
	  faval = fa->GetTuple1(pts[i]);
	  mdval = md->GetTuple1(pts[i]);
	  distval = dist->GetTuple1(pts[i]);
	  angleval = angle->GetTuple1(pts[i]);
	  roival = roi->GetTuple1(pts[i]);
	  otherval = other->GetTuple1(pts[i]);
	  
	  id = newPts->InsertNextPoint(inPts->GetPoint(pts[i]));
	  
	  // insert next cell
	  newCells->InsertCellPoint(id);
	  
	  // insert fields
	  newfa->InsertNextTuple(&faval);
	  newmd->InsertNextTuple(&mdval);
	  newdist->InsertNextTuple(&distval);
	  newangle->InsertNextTuple(&angleval);
	  newroi->InsertNextTuple(&roival);
	  newother->InsertNextTuple(&otherval);	 
	}
	
      } // end if(index >= 0) ...

    }

  outPD->AddArray(newfa);
  newfa->Delete();
  
  outPD->AddArray(md);
  newmd->Delete();
  
  outPD->AddArray(newdist);
  newdist->Delete();

  outPD->AddArray(newangle);
  newangle->Delete();

  outPD->AddArray(newroi);
  newroi->Delete();
  
  outPD->AddArray(newother);
  newother->Delete();

  output->SetPoints(newPts);
  newPts->Delete();

  output->SetLines(newCells);
  newCells->Delete();

}

