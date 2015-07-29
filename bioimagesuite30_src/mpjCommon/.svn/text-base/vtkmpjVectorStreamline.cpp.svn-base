

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


#include "vtkmpjVectorStreamline.h"

#include "vtkCellArray.h"
#include "vtkDataSet.h"
#include "vtkDoubleArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkPolyData.h"
#include "vtkPolyLine.h"

//vtkCxxRevisionMacro(vtkmpjVectorStreamline, "$Revision: 1.54 $");
vtkStandardNewMacro(vtkmpjVectorStreamline);

// Construct object with step size set to 1.0.
vtkmpjVectorStreamline::vtkmpjVectorStreamline()
{
  this->StepLength = 1.0;
  this->NumberOfStreamers = 0;
}

void vtkmpjVectorStreamline::Execute()
{
  if ( !this->GetInput() )
    {
    vtkErrorMacro("Input not set");
    return;
    }

  vtkmpjStreamer::mpjStreamPoint *sPrev, *sPtr;
  vtkPoints *newPts;
  vtkDoubleArray *newVectors;
  vtkDoubleArray *newScalars=NULL;
  vtkDoubleArray *newCellScalars=NULL;
  vtkCellArray *newLines;
  vtkIdType ptId, i, id;
  int j;
  vtkIdList *pts;
  double tOffset, x[3], v[3], s, ssum, r;
  double theta;
  vtkPolyLine* lineNormalGenerator = NULL;
  vtkDoubleArray* normals = NULL;
  vtkDoubleArray* rotation = 0;
  vtkPolyData *output=this->GetOutput();

  this->SavePointInterval = this->StepLength;
  this->vtkmpjStreamer::Integrate();
  if ( this->NumberOfStreamers <= 0 ) {return;}

  pts = vtkIdList::New();
  pts->Allocate(2500);

  //
  //  Convert streamer into lines. Lines may be dashed.
  //
  newPts  = vtkPoints::New();
  newPts->Allocate(1000);
  newVectors  = vtkDoubleArray::New();
  newVectors->SetNumberOfComponents(3);
  newVectors->Allocate(3000);
  if ( this->Vorticity )
    {
      lineNormalGenerator = vtkPolyLine::New();
      normals = vtkDoubleArray::New();
      normals->SetNumberOfComponents(3);
      normals->Allocate(3000);
      rotation = vtkDoubleArray::New();
      rotation->SetNumberOfComponents(1);
      rotation->Allocate(1000);
      rotation->SetName("Thetas");
      output->GetPointData()->AddArray(rotation);
    }
  
  if ( this->GetInput()->GetPointData()->GetScalars() || this->SpeedScalars
       || this->OrientationScalars)
    {
      newScalars = vtkDoubleArray::New();
      newScalars->Allocate(1000);
      
      // Allocate cell scalars
      newCellScalars = vtkDoubleArray::New();
      newCellScalars->Allocate(1000);
    }
  
  newLines = vtkCellArray::New();
  newLines->Allocate(newLines->EstimateSize(2*this->NumberOfStreamers,
                                            VTK_CELL_SIZE));
  //
  // Loop over all streamers generating points
  //  fprintf(stderr,"Looping over all streamers generating points\n");
  //
  for (ptId=0; ptId < this->NumberOfStreamers; ptId++)
    {
      //      fprintf(stderr,"Streamer=%d\n",ptId);

      if ( this->Streamers[ptId].GetNumberOfPoints() < 2 )
	{
	  continue;
	}
      sPrev = this->Streamers[ptId].GetmpjStreamPoint(0);
      sPtr = this->Streamers[ptId].GetmpjStreamPoint(1);
      
      if ( this->Streamers[ptId].GetNumberOfPoints() == 2 && sPtr->cellId >= 0 )
	{
	  continue;
	}
      
      tOffset = sPrev->t;
      ssum = 0.0;
      
      for ( i=1; 
	    i < this->Streamers[ptId].GetNumberOfPoints() && sPtr->cellId >= 0;
	    i++, sPrev=sPtr, sPtr=this->Streamers[ptId].GetmpjStreamPoint(i) )
	{
	  //
	  // Create points for line
	  //
	  while ( tOffset >= sPrev->t && tOffset < sPtr->t )
	    {
	      r = (tOffset - sPrev->t) / (sPtr->t - sPrev->t);
	      
	      for (j=0; j<3; j++)
		{
		  x[j] = sPrev->x[j] + r * (sPtr->x[j] - sPrev->x[j]);
		  v[j] = sPrev->v[j] + r * (sPtr->v[j] - sPrev->v[j]);
		}
	      
	      // add point to line
	      id = newPts->InsertNextPoint(x);
	      //printf("%f %f %f\n",x[0],x[1],x[2]);
	      
	      pts->InsertNextId(id);
	      newVectors->InsertTuple(id,v);
	      
	      if ( newScalars ) 
		{
		  s = sPrev->s + r * (sPtr->s - sPrev->s);
		  newScalars->InsertTuple(id,&s);
		  ssum += s;
		  //printf("acc ssum=%f, %f\n",ssum, s);
		}
	      
	      if ( this->Vorticity )
		{
		  // Store the rotation values. Used after all the streamlines
		  // are generated.
		  theta = sPrev->theta + r * (sPtr->theta - sPrev->theta);
		  rotation->InsertTuple(id, &theta);
		}
	      
	      tOffset += this->StepLength;
	      
	    } // while
	} //for this streamer
      
      if ( pts->GetNumberOfIds() > 1 )
	{
	  vtkIdType cellid = newLines->InsertNextCell(pts);
	  
	  ssum = ssum / (double)pts->GetNumberOfIds();
	  
	  // insert cell scalars
	  if ( newCellScalars ) {
	    newCellScalars->InsertTuple(cellid,&ssum);				      
	    //printf("ssum=%f, count=%d\n",ssum,pts->GetNumberOfIds());
	  }
	  
	  pts->Reset();	
	}
      
    } //for all streamers

  //  fprintf(stderr,"On to next point\n");
  
  vtkDebugMacro(<<"Created " << newPts->GetNumberOfPoints() << " points, "
		<< newLines->GetNumberOfCells() << " lines");
  
  if (this->Vorticity)
    {
      //      fprintf(stderr,"In Vorticity\n");
      // Rotate the normal vectors with stream vorticity
      int nPts=newPts->GetNumberOfPoints();
      double normal[3], local1[3], local2[3], length, costheta, sintheta;
      
      lineNormalGenerator->GenerateSlidingNormals(newPts,newLines,normals);
      

      //      fprintf(stderr,"Creating Points\n");

      for(i=0; i<nPts; i++)
	{
	  normals->GetTuple(i, normal);
	  newVectors->GetTuple(i, v);
	  // obtain two unit orthogonal vectors on the plane perpendicular to
	  // the streamline

	  //	  fprintf(stderr,"Dealing with Point %d (%.2f,%.2f,%.2f)\n",normal[0],normal[1],normal[2]);

	  for(j=0; j<3; j++) { local1[j] = normal[j]; }
	  length = vtkMath::Normalize(local1);
	  vtkMath::Cross(local1, v, local2);
	  vtkMath::Normalize(local2);
	  // Rotate the normal with theta
	  rotation->GetTuple(i, &theta);
	  costheta = cos(theta);
	  sintheta = sin(theta);
	  for(j=0; j<3; j++)
	    {
	      normal[j] = length* (costheta*local1[j] + sintheta*local2[j]);
	    }
	  normals->SetTuple(i, normal);
	}
      output->GetPointData()->SetNormals(normals);
      normals->Delete();
      lineNormalGenerator->Delete();
      rotation->Delete();
    }

  //  fprintf(stderr,"Clean up time .. which where the problem is most likely to be ...\n");
  
  output->SetPoints(newPts);
  newPts->Delete();
  
  //  fprintf(stderr,"Clean up time 2\n");
  output->GetPointData()->SetVectors(newVectors);
  newVectors->Delete();
  
  if ( newScalars ) 
    {
      output->GetPointData()->SetScalars(newScalars);
      newScalars->Delete();
    }
  
  //  fprintf(stderr,"Clean up time 3\n");

  if ( newCellScalars )
    {
      output->GetCellData()->SetScalars(newCellScalars);
      newCellScalars->Delete();
    }
  
  //  fprintf(stderr,"Clean up time 4\n");
  pts->Delete();
  //  fprintf(stderr,"Clean up time 41\n");
  output->SetLines(newLines);
  //  fprintf(stderr,"Clean up time 42\n");
  newLines->Delete();
  //  fprintf(stderr,"Clean up time 43\n");

  // Delete the streamers since they are no longer needed
  delete [] this->Streamers;
  this->Streamers = 0;
  this->NumberOfStreamers = 0;
  
  //fprintf(stderr,"Clean up time 45\n");

  output->Squeeze();
  //  fprintf(stderr,"Clean up time 46\n");

}

void vtkmpjVectorStreamline::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Step Length: " << this->StepLength << "\n";

}


