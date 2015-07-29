

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


#include "vtkmpjTensorStreamlineAngleFilter.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyLine.h"

vtkStandardNewMacro(vtkmpjTensorStreamlineAngleFilter);

// Construct object 
vtkmpjTensorStreamlineAngleFilter::vtkmpjTensorStreamlineAngleFilter()
{
  this->MinimumAngle = 0;
  this->MaximumAngle = 0;
  this->MinimumTorsion = 0;
  this->MaximumTorsion = 0;
  this->AngleOutside = 0;
  this->TorsionOutside = 0;
}

int vtkmpjTensorStreamlineAngleFilter::TestAngle(double angleval,
						 double minangle,
						 double maxangle,
						 int outside)
{
  if (outside == 0) {
    // angle within desired interval
    if ((angleval >= minangle) && (angleval <= maxangle))
      return 1;
    else
      return 0;
  } else {
    if ((angleval < minangle) || (angleval > maxangle))
      return 1;
    else 
      return 0;
  }
}

void vtkmpjTensorStreamlineAngleFilter::Execute()
{
  vtkPolyData *input = this->GetInput();
  vtkPolyData *output = this->GetOutput();
  vtkPointData *pd=input->GetPointData();

  vtkPointData *outPD=output->GetPointData();
  vtkCellArray *inLines = NULL;

  vtkPoints *inPts;
  vtkIdType numPts = 0;
  vtkIdType numLines;
    
  double val[3];
  double normal[3];
  double binormal[3];
  double vector[3];
  float faval;
  float mdval;
  float distval;
  float angleval;
  float torsionval;
  float roival;
  float otherval;

  vtkIdType npts=0, *pts=NULL;
  int id, abort=0;
  vtkIdType inCellId;

  vtkDataArray *scalars = pd->GetScalars();
  vtkDataArray *normals = pd->GetNormals();
  vtkDataArray *vectors = pd->GetVectors();
  vtkDataArray *binormals = pd->GetArray("binormal");

  vtkDataArray *fa = pd->GetArray("fractani");
  vtkDataArray *md = pd->GetArray("meandiff");
  vtkDataArray *dist = pd->GetArray("distance");
  vtkDataArray *angle = pd->GetArray("angle");  
  vtkDataArray *torsion = pd->GetArray("torsion");
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

  vtkFloatArray *newScalars = vtkFloatArray::New();
  newScalars->SetNumberOfComponents(scalars->GetNumberOfComponents());

  vtkFloatArray *newNormals = vtkFloatArray::New();
  newNormals->SetNumberOfComponents(3);

  vtkFloatArray *newBinormals = vtkFloatArray::New();
  newBinormals->SetName("binormal");
  newBinormals->SetNumberOfComponents(3);

  vtkFloatArray *newVectors = vtkFloatArray::New();
  newVectors->SetNumberOfComponents(3);

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

  vtkFloatArray *newtorsion = vtkFloatArray::New();
  newtorsion->SetName("torsion");
  newtorsion->SetNumberOfComponents(1);

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
      
      int count = 0;
      int start = 0;
      int end = 0;
      
      // create new polyline
      for(int i=0; i < npts; i++) {
	
	if (normals) {
	  normals->GetTuple(pts[i], normal);
	}
	if (binormals) {
	  binormals->GetTuple(pts[i], binormal);
	}
	vectors->GetTuple(pts[i], vector);
	scalars->GetTuple(pts[i], val);
	faval = fa->GetTuple1(pts[i]);
	mdval = md->GetTuple1(pts[i]);
	distval = dist->GetTuple1(pts[i]);
	angleval = angle->GetTuple1(pts[i]);
	torsionval = torsion->GetTuple1(pts[i]);
	roival = roi->GetTuple1(pts[i]);
	otherval = other->GetTuple1(pts[i]);
	  
	// angle within desired interval
	if (TestAngle(angleval,this->MinimumAngle,this->MaximumAngle,this->AngleOutside) &&
            TestAngle(torsionval,this->MinimumTorsion,this->MaximumTorsion,this->TorsionOutside))
	  {
	    count = 0;
	    start = i;
	    end = i;
	    
	    // detect starting and ending point
	    while (end < npts) {
	      angleval = angle->GetTuple1(pts[end]);
	      torsionval = torsion->GetTuple1(pts[end]);
	      if (TestAngle(angleval,this->MinimumAngle,this->MaximumAngle,this->AngleOutside) &&
		  TestAngle(torsionval,this->MinimumTorsion,this->MaximumTorsion,this->TorsionOutside))
		count++;
	      else
		break;
	      end++;
	    }
	    
	    printf("count=%d\n",count);

	    // in case of more than two points create a cell and insert points
	    if (count >= 2) 
	      {
		// insert new cell
		newCells->InsertNextCell(count);
		
		// construct line and scalar fields
		for(int j=start; j<end; j++) 
		  {
		    if (normals) {
		      normals->GetTuple(pts[j], normal);
		    }
		    if (binormals) {
		      binormals->GetTuple(pts[j], binormal);
		    }
		    //printf("normal=%f,%f,%f\n",normal[0],normal[1],normal[2]);
		    scalars->GetTuple(pts[j], val);
		    vectors->GetTuple(pts[j], vector);
		    faval = fa->GetTuple1(pts[j]);
		    mdval = md->GetTuple1(pts[j]);
		    distval = dist->GetTuple1(pts[j]);
		    angleval = angle->GetTuple1(pts[j]);
		    torsionval = torsion->GetTuple1(pts[j]);
		    roival = roi->GetTuple1(pts[j]);
		    otherval = other->GetTuple1(pts[j]);
		    
		    // insert new point
		    id = newPts->InsertNextPoint(inPts->GetPoint(pts[j]));
		    
		    // insert next cell
		    newCells->InsertCellPoint(id);
		    
		    // insert fields
		    if (normals) {
		      newNormals->InsertNextTuple(normal);
		    }
		    if (binormals) {
		      newBinormals->InsertNextTuple(binormal);
		    }
		    newScalars->InsertNextTuple(val);
		    newVectors->InsertNextTuple(vector);
		    newfa->InsertNextTuple(&faval);
		    newmd->InsertNextTuple(&mdval);
		    newdist->InsertNextTuple(&distval);
		    newangle->InsertNextTuple(&angleval);
		    newtorsion->InsertNextTuple(&torsionval);
		    newroi->InsertNextTuple(&roival);
		    newother->InsertNextTuple(&otherval);
		  }
	      }
	    
	    // go to the end of current line and repeat the process
	    i = end;
	  }
      }
    } 
  
  outPD->SetScalars(newScalars);
  newScalars->Delete();

  outPD->SetVectors(newVectors);
  newVectors->Delete();

  if (normals)
    outPD->SetNormals(newNormals);
  newNormals->Delete();

  if (binormals)
    outPD->AddArray(newBinormals);
  newBinormals->Delete();

  outPD->AddArray(newfa);
  newfa->Delete();
  
  outPD->AddArray(md);
  newmd->Delete();
  
  outPD->AddArray(newdist);
  newdist->Delete();

  outPD->AddArray(newangle);
  newangle->Delete();

  outPD->AddArray(newtorsion);
  newtorsion->Delete();

  outPD->AddArray(newroi);
  newroi->Delete();
  
  outPD->AddArray(newother);
  newother->Delete();

  output->SetPoints(newPts);
  newPts->Delete();

  output->SetLines(newCells);
  newCells->Delete();

}

