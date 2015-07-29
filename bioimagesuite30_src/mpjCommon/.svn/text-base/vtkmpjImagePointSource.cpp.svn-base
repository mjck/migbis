

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


#include "vtkmpjImagePointSource.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkTransform.h"

vtkStandardNewMacro(vtkmpjImagePointSource);

vtkmpjImagePointSource::vtkmpjImagePointSource()
{
  this->Density = 1.0;

  this->Center[0] = 0.0;
  this->Center[1] = 0.0;
  this->Center[2] = 0.0;

  this->Size[0] = 3;
  this->Size[1] = 3;
  this->Size[2] = 3;

  this->Origin[0] = 0.0;
  this->Origin[1] = 0.0;
  this->Origin[2] = 0.0;

  this->Spacing[0] = 1.0;
  this->Spacing[1] = 1.0;
  this->Spacing[2] = 1.0;

  this->Mask = NULL;
}

vtkmpjImagePointSource::~vtkmpjImagePointSource()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

void vtkmpjImagePointSource::Execute()
{
  double x1, y1, z1;
  double x[3];
  int i, j, k;
  vtkPoints *newPoints; 
  vtkPolyData *output = this->GetOutput();
  newPoints = vtkPoints::New();
  vtkDataArray *mask = NULL;

  double incx = (double)this->Spacing[0] / (double)this->Density;
  double incy = (double)this->Spacing[1] / (double)this->Density;
  double incz = (double)this->Spacing[2] / (double)this->Density;

  //printf("incx=%lf, incy=%lf, incz=%lf\n",incx,incy,incz);

  double xmin, ymin, zmin;
  double xmax, ymax, zmax;
  int pos, count = 0;

  // determine bounding box of mask
  if (this->Mask != NULL) {
    
    int dim[3];
    this->Mask->GetDimensions(dim);
    mask = this->Mask->GetPointData()->GetScalars();
    
    //printf("dim=%d,dim=%d,dim=%d\n",dim[0],dim[1],dim[2]);

    xmin = dim[0]-1;
    ymin = dim[1]-1;
    zmin = dim[2]-1;
    xmax = 0;
    ymax = 0;
    zmax = 0;
    
    double val;
    int index = 0;
    
    for(k=0; k<dim[2]; k++) 
      for(j=0; j<dim[1]; j++)
	for(i=0; i<dim[0]; i++) {
	  	  
	  val = mask->GetComponent(index, 0);
	  
	  if (val > 0) {	   
	    if (i < xmin) xmin = i;
	    if (j < ymin) ymin = j;
	    if (k < zmin) zmin = k;	    
	    if (i > xmax) xmax = i;
	    if (j > ymax) ymax = j;
	    if (k > zmax) zmax = k;
	    //printf("%d %d %d = %f, min=%f,%f,%f max=%f,%f,%f\n",i,j,k,val,xmin,ymin,zmin,xmax,ymax,zmax);
	  }
	  
	  ++index;
	}
    
    //printf("min=%f,%f,%f, max=%f,%f,%f\n",xmin,ymin,zmin,xmax,ymax,zmax);

    // convert into point coords
    xmin = this->Origin[0] + this->Spacing[0]*xmin;
    ymin = this->Origin[1] + this->Spacing[1]*ymin;
    zmin = this->Origin[2] + this->Spacing[2]*zmin;    
    xmax = this->Origin[0] + this->Spacing[0]*xmax;
    ymax = this->Origin[1] + this->Spacing[1]*ymax;
    zmax = this->Origin[2] + this->Spacing[2]*zmax;
 
    //printf("min=%f,%f,%f, max=%f,%f,%f\n",xmin,ymin,zmin,xmax,ymax,zmax);

  } else {

    xmin = this->Center[0] - ((this->Size[0]-1)/2.0)*this->Spacing[0];
    ymin = this->Center[1] - ((this->Size[1]-1)/2.0)*this->Spacing[1];
    zmin = this->Center[2] - ((this->Size[2]-1)/2.0)*this->Spacing[2];
    xmax = xmin + (this->Size[0]-1)*this->Spacing[0];
    ymax = ymin + (this->Size[1]-1)*this->Spacing[1];
    zmax = zmin + (this->Size[2]-1)*this->Spacing[2];
  }
  
	for(x1 = xmin; x1 <= xmax; x1 += incx) 
	for(y1 = ymin; y1 <= ymax; y1 += incy)
	for(z1 = zmin; z1 <= zmax; z1 += incz) {
	
	x[0] = x1; 
	x[1] = y1;
	x[2] = z1;
        //printf("%lf %lf %lf\n",x[0],x[1],x[2]);	
	if (this->Mask) {
	  pos = this->Mask->FindPoint(x);
	  if (pos >= 0) {
	    if ((mask->GetComponent(pos, 0) > 0)) {
	      newPoints->InsertNextPoint(x);
	      count++;
	    }
	  }
	} else {
	  newPoints->InsertNextPoint(x);
	  count++;
	}

	//	if ((this->Mask ? this->Mask->FindPoint(x)) : 1) >= 0) {
	//newPoints->InsertNextPoint(x);
	//count++;
	//}
     }
  
  //  printf("total of seed points=%d\n",count);

  // Update ourselves and release memory
  output->SetPoints(newPoints);
  newPoints->Delete();
  
}





