

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
#include "vtkmpjStreamlineMapQuantization.h"
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkFloatArray.h"

//------------------------------------------------------------------------------
vtkmpjStreamlineMapQuantization* vtkmpjStreamlineMapQuantization::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjStreamlineMapQuantization");
  if(ret)
    {
      return (vtkmpjStreamlineMapQuantization*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjStreamlineMapQuantization;
}

// ----------------------------------------------------------------------------
vtkmpjStreamlineMapQuantization::vtkmpjStreamlineMapQuantization()
{
  this->Image = NULL;
  this->QuantizationField = NULL;
  this->BackgroundValue = 0;
} 

vtkmpjStreamlineMapQuantization::~vtkmpjStreamlineMapQuantization()
{
  if (this->Image != NULL)
    this->Image->Delete();
} 

// ----------------------------------------------------------------------------
void vtkmpjStreamlineMapQuantization::Execute()
{ 
  vtkPolyData *input=(vtkPolyData *)this->GetInput();
  vtkPointData *pdpoly=input->GetPointData();
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

  vtkDataArray *map = pdpoly->GetArray(this->QuantizationField);

  //printf("before zeroing name=  %s\n", this->QuantizationField);

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
	  double x1[3],x2[3],p[3],d[3],md,grad[3];
	  double maxdist=0.0;
	  int maxindex=0;
	  int k;
	  
	  double m1=0;
	  double m2=0;
	  double value=0;

	  inPts->GetPoint(pts[i-1],x1); m1 = map->GetTuple1(pts[i-1]);
	  inPts->GetPoint(pts[i], x2);  m2 = map->GetTuple1(pts[i]);
	  
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
		  value = m1;
		}
	      else
		{
		  for (k=0;k<=2;k++)
		    p[k]=x1[k]+(dt/fabs(maxdist))*d[k];
		  value=m1+(dt/fabs(maxdist))*(m2-m1);
		}
	      
	      //trans->TransformPoint(p,tx);

	      // find point location
	      index = this->Image->FindPoint(p);
	      
	      if (index >= 0) {
		val = out->GetComponent(index, 0);
		//if (val == this->BackgroundValue) {
		  count++;
		  out->SetComponent(index, 0, value);
		  //printf("index=%d, value=%f\n",index,value);
		  //}
	      }
	      
	    }
	}
    }

  this->NumberOfVoxels = count;
}

// ----------------------------------------------------------------------------
