

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



#include "vtkObjectFactory.h"
#include "vtkmpjStreamlineQuantization.h"
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkFloatArray.h"

//------------------------------------------------------------------------------
vtkmpjStreamlineQuantization* vtkmpjStreamlineQuantization::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjStreamlineQuantization");
  if(ret)
    {
      return (vtkmpjStreamlineQuantization*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjStreamlineQuantization;
}

// ----------------------------------------------------------------------------
vtkmpjStreamlineQuantization::vtkmpjStreamlineQuantization()
{
  this->Image = NULL;
  this->MaskValue = 1;
  this->BackgroundValue = 0;
} 

vtkmpjStreamlineQuantization::~vtkmpjStreamlineQuantization()
{
  if (this->Image != NULL)
    this->Image->Delete();
} 

// ----------------------------------------------------------------------------
void vtkmpjStreamlineQuantization::Execute()
{ 
  vtkPolyData *input=(vtkPolyData *)this->GetInput();
  //  vtkDataSet *output=this->GetOutput();

  vtkPointData *pd=this->Image->GetPointData();
  vtkDataArray *out=pd->GetScalars();

  vtkCellArray *inLines = NULL;

  vtkPoints *inPts;
  vtkIdType numPts = 0;
  vtkIdType numLines;

  vtkIdType npts=0, *pts=NULL;
  int abort=0;
  vtkIdType inCellId;
  
  float val;

  // zero out background
  out->FillComponent(0, this->BackgroundValue);

  long count = 0;

  // Check input and initialize
  //  
  if ( !(inPts=input->GetPoints()) || 
       (numPts = inPts->GetNumberOfPoints()) < 1 ||
       !(inLines = input->GetLines()) || 
       (numLines = inLines->GetNumberOfCells()) < 1 )
    {
      return;
    }

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
      
      int index=-1;
      
      for(int i=1;i<npts;i++)
	{
	  double x1[3],x2[3],p[3],d[3],grad[3];
	  double maxdist=0.0;
	  int maxindex=0;
	  int k;
	  
	  inPts->GetPoint(pts[i-1],x1);
	  inPts->GetPoint(pts[i], x2);
	  
	  for (k=0;k<=2;k++)
	    {
	      d[k]=x2[k]-x1[k];
	      if (fabs(d[k])>fabs(maxdist))
		{
		  maxdist=d[k];
		  maxindex=k;
		}
	    }
	  
	  for (k=0;k<=2;k++)
	    {
	      if (d[k]!=0.0)
		grad[k]=d[k]/maxdist;
	      else
		grad[k]=0.0;
	    }
      
	  for (float dt=0.0;dt<fabs(maxdist);dt+=0.25)
	    {
	      if (maxdist==0)
		{
		  for (k=0;k<=2;k++)
		    p[k]=x1[k];
		}
	      else
		{
		  for (k=0;k<=2;k++)
		    p[k]=x1[k]+(dt/fabs(maxdist))*d[k];
		}
	      
	      //trans->TransformPoint(p,tx);

	      // find point location
	      index = this->Image->FindPoint(p);
	      
	      if (index >= 0) {
		val = out->GetComponent(index, 0);
		if (val != this->MaskValue) {
		  count++;
		  out->SetComponent(index, 0, this->MaskValue);
		}
	      }
	      
	    }
	}
    }

  this->NumberOfVoxels = count;
}

// ----------------------------------------------------------------------------
