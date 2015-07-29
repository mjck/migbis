

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


#include "vtkmpjTensorStreamlineWriter.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyLine.h"

vtkStandardNewMacro(vtkmpjTensorStreamlineWriter);

// Construct object 
vtkmpjTensorStreamlineWriter::vtkmpjTensorStreamlineWriter()
{
  this->FileName = NULL;
}

void vtkmpjTensorStreamlineWriter::Execute()
{
  vtkPolyData *input = this->GetInput();
  vtkPointData *pd=input->GetPointData();
  
  vtkDataArray *fa = pd->GetArray("fractani");
  vtkDataArray *md = pd->GetArray("meandiff");
  //vtkDataArray *dist = pd->GetArray("distance");
  vtkDataArray *angle = pd->GetArray("angle");  
  vtkDataArray *other = pd->GetArray("other");

  vtkDataArray *torsion = NULL;
  if (pd->HasArray("torsion"))
    torsion = pd->GetArray("torsion");

  //vtkPointData *outPD=output->GetPointData();
  vtkCellArray *inLines = NULL;
  
  vtkPoints *inPts;
  vtkIdType numPts = 0;
  vtkIdType numLines;
  
  vtkIdType npts=0, *pts=NULL;
  int abort=0;
  vtkIdType inCellId;

  if (this->FileName == NULL)
    return;

  FILE *fp = fopen(this->FileName, "w");
  if (fp == NULL) 
    return;

  // Check input and initialize
  //  
  if ( !(inPts=input->GetPoints()) || 
       (numPts = inPts->GetNumberOfPoints()) < 1 ||
       !(inLines = input->GetLines()) || 
       (numLines = inLines->GetNumberOfCells()) < 1 )
    {
      return;
    }
  
  //float* fractani_array = new float[numLines];
  //float* meandiff_array = new float[numLines];
  //float* distance_array = new float[numLines];
  //float* angle_array = new float[numLines];
  //float* other_array = new float[numLines]; 

  fprintf(fp, "%d\n",numLines);

  for (inCellId=0, inLines->InitTraversal(); 
       inLines->GetNextCell(npts,pts) && !abort; inCellId++)
    {
      this->UpdateProgress((float)inCellId/numLines);
      abort = this->GetAbortExecute();

      if (npts > 1) {
	
	fprintf(fp, "\n%d\n",npts);

	// compute statistics for fiber
	for(int i=0; i<npts; i++) {	
	  
	  double p[3];
	  double vfa,vmd,vangle,vtorsion,vother;
	  inPts->GetPoint(pts[i], p);
	  vfa = fa->GetTuple1(pts[i]);
	  vmd = md->GetTuple1(pts[i]);
	  vangle = angle->GetTuple1(pts[i]);
	  vother = other->GetTuple1(pts[i]);
	  if (torsion == NULL)
	    vtorsion = 0.0;
	  else
	    vtorsion = torsion->GetTuple1(pts[i]);
	  fprintf(fp, "%d  %f %f %f  %f %f  %f %f  %f \n", i, 
		  p[0], p[1], p[2], 
		  vfa, vmd, vangle, vtorsion, vother);
	}
	
      } else {
	printf("npts=%d\n",npts);
      }
      
    }
  
  fclose(fp);

  }
  
