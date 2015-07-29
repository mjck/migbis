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





/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

 see vtkpxElectrodeGridToSurfaceFilter.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkCellArray.h"
#include "vtkCollection.h"
#include "vtkDiskSource.h"
#include "vtkUnsignedCharArray.h"
#include "vtkDoubleArray.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkThinPlateSplineTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkpxElectrodeGridToSurfaceFilter.h"
#include "vtkpxElectrodeSource.h"
#include "vtkProbeFilter.h"
//------------------------------------------------------------------------------
vtkpxElectrodeGridToSurfaceFilter* vtkpxElectrodeGridToSurfaceFilter::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxElectrodeGridToSurfaceFilter");
  if(ret)
    {
      return (vtkpxElectrodeGridToSurfaceFilter*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxElectrodeGridToSurfaceFilter;
}

// Construct object with no children.
vtkpxElectrodeGridToSurfaceFilter::vtkpxElectrodeGridToSurfaceFilter()
{
  this->Grid=NULL;
  this->BaseImage=NULL;
  this->Transformation=NULL;
  this->Sigma=1.0;
  this->Lambda=1.0;
  this->CTMode=1;
  this->BaseSurface=NULL;
  this->GridPointSurface=NULL;
  this->SmoothImage=NULL;

  this->SampleRate=4;
}

/* -------------------------------------------------------------------------*/
 
vtkpxElectrodeGridToSurfaceFilter::~vtkpxElectrodeGridToSurfaceFilter()
{
  if (this->Transformation!=NULL)
    this->Transformation->Delete();
  this->SetBaseImage(NULL);
  this->SetGrid(NULL);

  if (this->BaseSurface!=NULL)
    this->BaseSurface->Delete();

  if (this->GridPointSurface!=NULL)
    this->GridPointSurface->Delete();
  
  if (this->SmoothImage!=NULL)
    this->SmoothImage->Delete();

}
/* -------------------------------------------------------------------------*/
void vtkpxElectrodeGridToSurfaceFilter::SetElectrodeGrid(vtkpxElectrodeGridSource* grd)
{
  this->SetGrid(grd);
  if (this->BaseSurface!=NULL)
    {
      this->BaseSurface->Delete();
      this->BaseSurface=NULL;
    }

  if (this->GridPointSurface!=NULL)
    {
      this->GridPointSurface->Delete();
      this->GridPointSurface=NULL;
    }
  
  if (this->Transformation!=NULL)
    {
      this->Transformation->Delete();
      this->Transformation=NULL;
    }
}

void vtkpxElectrodeGridToSurfaceFilter::SetImage(vtkImageData* img)
{
  this->SetBaseImage(img);
  if (this->SmoothImage==NULL)
    return;
    
  this->SmoothImage->Delete();
  this->SmoothImage=NULL;
}
/* -------------------------------------------------------------------------*/
//
// Optimization Stuff
//
/* -------------------------------------------------------------------------*/

double vtkpxElectrodeGridToSurfaceFilter::CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output)
{
  return ((vtkpxElectrodeGridToSurfaceFilter*)self)->OptimizerGradient(position,output);
}

double vtkpxElectrodeGridToSurfaceFilter::CalculateFunction(vtkObject* self,vtkDoubleArray* position)
{
  return ((vtkpxElectrodeGridToSurfaceFilter*)self)->OptimizerValue(position);
}

void vtkpxElectrodeGridToSurfaceFilter::OptimizerFeedback(vtkObject* self,const char* line,double val)
{
  fprintf(stderr,"Output %s : %.3f\n",line,val);
}
//-------------------------------------------------------------------------
void vtkpxElectrodeGridToSurfaceFilter::Execute()
{
  vtkPolyData* output = this->GetOutput();

  if (this->Grid==NULL)
    {
      vtkDiskSource* disk=vtkDiskSource::New();
      disk->SetRadialResolution(2);
      disk->SetCircumferentialResolution(2);
      disk->Update();
      output->ShallowCopy(disk->GetOutput());
      disk->Delete();
      return;
    }

  if (this->BaseSurface==NULL)
    this->GenerateBaseSurface();

  if (this->Transformation==NULL)
    {
      output->ShallowCopy(this->BaseSurface);
      return;
    }

  vtkTransformPolyDataFilter* trF=vtkTransformPolyDataFilter::New();
  trF->SetInput(this->BaseSurface);
  trF->SetTransform(this->Transformation);
  trF->Update();

  output->ShallowCopy(trF->GetOutput());
  trF->Delete();
     
}
/* -------------------------------------------------------------------------*/
int vtkpxElectrodeGridToSurfaceFilter::Optimize(int iterations)
{
  if (this->Grid==NULL)
    return 0;

  if (this->SmoothImage==NULL)
    this->GenerateSmoothImage();

  if (this->BaseSurface==NULL)
    this->GenerateBaseSurface();

  if (this->Transformation==NULL)
    this->InitializeTransformation();

  int np=this->Transformation->GetSourceLandmarks()->GetNumberOfPoints();

  int dims[2]; this->Grid->GetDimensions(dims);
		 
  vtkpxOptimizer* optimizer=vtkpxOptimizer::New();
  vtkDoubleArray* position=vtkDoubleArray::New();
  vtkDoubleArray* temp=vtkDoubleArray::New();
  position->SetNumberOfTuples(np*3);
  this->GetCurrentParameters(position);
  optimizer->SetMasterObject(this);

  optimizer->SetEvaluateFunctionMethod(&vtkpxElectrodeGridToSurfaceFilter::CalculateFunction);
  optimizer->SetEvaluateGradientMethod(&vtkpxElectrodeGridToSurfaceFilter::CalculateGradient);
  optimizer->SetFeedbackMethod(&vtkpxElectrodeGridToSurfaceFilter::OptimizerFeedback);

  optimizer->ComputeConjugateGradient(position,iterations,0.001);
    
  optimizer->Delete();
  position->Delete();
  temp->Delete();
  
  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxElectrodeGridToSurfaceFilter::GenerateSmoothImage()
{
  if (this->BaseImage==NULL)
    return 0;

  vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
  smooth->SetInput(this->BaseImage);
  smooth->SetStandardDeviations(this->Sigma,this->Sigma,this->Sigma);
  smooth->Update();

  if (this->SmoothImage==NULL)
    this->SmoothImage=vtkImageData::New();
  this->SmoothImage->ShallowCopy(smooth->GetOutput());
  smooth->Delete();
  return 1;
}

int vtkpxElectrodeGridToSurfaceFilter::InitializeTransformation()
{
  if (this->Grid==NULL)
    return 0;

  if (this->BaseSurface==NULL)
    this->GenerateBaseSurface();

  vtkPoints* points1=vtkPoints::New();
  vtkPoints* points2=vtkPoints::New();
  
  int dims[2]; this->Grid->GetDimensions(dims);
  int num=dims[0]*dims[1];

  this->Grid->AutoWarp();

  points1->Allocate(num,2);
  points2->Allocate(num,2);

  double d[3],x[3];
  vtkCollection* ElectrodeCollection=this->Grid->GetElectrodeCollection();
  float sp[2]; this->Grid->GetElectrodeSpacing(sp);

  int index=0;
  ElectrodeCollection->InitTraversal();
  for (int j=0;j<dims[1];j++)
    for (int i=0;i<dims[0];i++)
      {
	d[0]=i*sp[0];
	d[1]=j*sp[1];
	d[2]=0.0;
	vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(ElectrodeCollection->GetNextItemAsObject());
	if (electrode->GetElectrodePresent())
	  {
	    electrode->GetPosition(x);
	    points1->InsertNextPoint(d);
	    points2->InsertNextPoint(x);
	    ++index;
	  }
      }
	    
  if (this->Transformation==NULL)
    this->Transformation=vtkThinPlateSplineTransform::New();
  this->Transformation->SetSourceLandmarks(points1);
  this->Transformation->SetTargetLandmarks(points2);
  this->Transformation->SetSigma(2.0);
  this->Transformation->SetBasisToR();
  points1->Delete();
  points2->Delete();
  this->Transformation->Update();
  return 1;

}

// ------------------------------------------------------------------------------------------------------------------------------------------
int vtkpxElectrodeGridToSurfaceFilter::GenerateBaseSurface()
{
  if (this->GridPointSurface==NULL)
    this->GridPointSurface=vtkPolyData::New();

  int dims[2]; this->Grid->GetDimensions(dims);
  
  vtkCollection* ElectrodeCollection=this->Grid->GetElectrodeCollection();
  float sp[2]; this->Grid->GetElectrodeSpacing(sp);

  vtkPoints    *c_points2    = vtkPoints::New();
  c_points2->Allocate(dims[0]*dims[1]*(1+this->SampleRate)*(1+this->SampleRate),20);

  ElectrodeCollection->InitTraversal();

  for (int j=0;j<dims[1];j++)
    for (int i=0;i<dims[0];i++)
      {
	vtkpxElectrodeSource* electrode=(vtkpxElectrodeSource*)(ElectrodeCollection->GetNextItemAsObject());
	if (electrode->GetElectrodePresent())
	  {
	    double d[3];
	    d[0]=i*sp[0];	d[1]=j*sp[1];	    d[2]=0.0;
	    c_points2->InsertNextPoint(electrode->GetPosition());
	  }
      }
  this->GridPointSurface->SetPoints(c_points2);
  c_points2->Delete();

  int numx=dims[0]+(dims[0]-1)*this->SampleRate;
  int numy=dims[1]+(dims[1]-1)*this->SampleRate;


  vtkPoints    *c_points    = vtkPoints::New();
  c_points->SetNumberOfPoints(numx*numy);
  vtkCellArray *c_lines     = vtkCellArray::New();
  c_lines->Allocate(numx*numy,10);
  
  double newsp[2];
  newsp[0]=sp[0]/double(this->SampleRate+1);
  newsp[1]=sp[1]/double(this->SampleRate+1);

  vtkIdType index=0;
  vtkIdType pt[2];
  for (int j=0;j<numy;j++)
    for (int i=0;i<numx;i++)
      {
	double d[3];
	d[0]=i*newsp[0];
	d[1]=j*newsp[1];
	d[2]=0.0;
	c_points->SetPoint(index,d);
	pt[0]=index;

	if (i!=numx-1)
	  {
	    pt[1]=index+1;
	    c_lines->InsertNextCell(2,pt);
	  }
	if (j!=numy-1)
	  {
	    pt[1]=index+numx;
	    c_lines->InsertNextCell(2,pt);
	  }
	++index;
      }

  if (this->BaseSurface==NULL)
    this->BaseSurface=vtkPolyData::New();
  this->BaseSurface->SetPoints(c_points);
  this->BaseSurface->SetLines(c_lines);

  c_points->Delete();
  c_lines->Delete();

  return 1;
}


/* -------------------------------------------------------------------------*/
double vtkpxElectrodeGridToSurfaceFilter::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)
{
  if (grad->GetNumberOfTuples()!=params->GetNumberOfTuples())
    grad->SetNumberOfTuples(params->GetNumberOfTuples());

  grad->FillComponent(0,0.0);
  double GradientNorm = 0;
  double step=1.0;

  int np=grad->GetNumberOfTuples();

  for (int i=0;i<np;i++)
    {
      double orig=params->GetComponent(i,0);
      params->SetComponent(i, 0, orig + step);
      double a=this->OptimizerValue(params);
      
      params->SetComponent(i, 0, orig - step);
      double b=this->OptimizerValue(params);
      
      params->SetComponent(i, 0, orig);
      this->SetCurrentParameters(params);

      double g=-0.5*(b-a)/step;
      grad->SetComponent(i,0,g);
      GradientNorm+=pow(g,2.0);
    }

  GradientNorm = (sqrt(GradientNorm));
  // -------------------------
  // Hirohito Found this One!!
  // -------------------------
  for (int j=0;j<np;j++)
    grad->SetComponent(j,0,grad->GetComponent(j,0)/GradientNorm);
      
  return GradientNorm;
}

 
double vtkpxElectrodeGridToSurfaceFilter::OptimizerValue(vtkDoubleArray* params)
{
  this->SetCurrentParameters(params);
  
  // First The Image Adherence Term
  vtkTransformPolyDataFilter* trF=vtkTransformPolyDataFilter::New();
  trF->SetInput(this->GridPointSurface);
  trF->SetTransform(this->Transformation);
  trF->Update();

  vtkProbeFilter* probe=vtkProbeFilter::New();
  probe->SetInput(trF->GetOutput());
  probe->SetSource(this->SmoothImage);
  probe->Update();
      
  vtkDataArray* data=probe->GetOutput()->GetPointData()->GetScalars();
  double sum=0.0;

  float sp[2]; this->Grid->GetElectrodeSpacing(sp);
  double newsp[2];
  newsp[0]=sp[0]/double(this->SampleRate+1);
  newsp[1]=sp[1]/double(this->SampleRate+1);

  int   np=data->GetNumberOfTuples();
  for (int i=0;i<np;i++)
    sum+=data->GetComponent(i,0);
  probe->Delete();
  trF->Delete();

  if (np>0)
    sum/=double(np);

  if (this->CTMode==0)
    sum=-sum;
  
  double ImageAdherence=sum;


  // Then Smoothness Term

  vtkTransformPolyDataFilter* trF2=vtkTransformPolyDataFilter::New();
  trF2->SetInput(this->BaseSurface);
  trF2->SetTransform(this->Transformation);
  trF2->Update();

  vtkPoints* pts=trF2->GetOutput()->GetPoints();

  int dims[2]; this->Grid->GetDimensions(dims);
  int numx=dims[0]+(dims[0]-1)*this->SampleRate;
  int numy=dims[1]+(dims[1]-1)*this->SampleRate;


  double distance=0.0;
  int   numlines=0;

  int index=0;
  double x1[3],x2[3];
  for (int j=0;j<numy;j++)
    for (int i=0;i<numx;i++)
      {
	pts->GetPoint(index,x1);
	if (i!=numx-1)
	  {
	    pts->GetPoint(index+1,x2);
	    distance+=pow( sqrt(vtkMath::Distance2BetweenPoints(x1,x2))-newsp[0],2.0);
	    ++numlines;
	  }
	if (j!=numy-1)
	  {
	    pts->GetPoint(index+numx,x2);
	    distance+=pow( sqrt(vtkMath::Distance2BetweenPoints(x1,x2))-newsp[0],2.0);
	    ++numlines;
	  }
	++index;
      }



  if (numlines>0)
    distance=distance/double(numlines);

  trF2->Delete();
  
  double total=-ImageAdherence - this->Lambda*distance;
  fprintf(stderr,"ImageAdherence=%.3f distance=%.3f \t\t Total=%.3f\n",ImageAdherence,distance,total);

  return total;
}

/* -------------------------------------------------------------------------*/
void vtkpxElectrodeGridToSurfaceFilter::SetCurrentParameters(vtkDoubleArray* params)
{
  int np=this->Transformation->GetSourceLandmarks()->GetNumberOfPoints();
  if (params->GetNumberOfTuples()!=np*3)
    {
      vtkErrorMacro(<<"Bad Number Of Parameters\n");
      return;
    }

  vtkPoints* points=this->Transformation->GetSourceLandmarks();

  int index=0;
  for (int i=0;i<np;i++)
    {
      double x[3];
      for (int j=0;j<=2;j++)
	{
	  x[j]=params->GetComponent(index,0);
	  ++index;
	}
      points->SetPoint(i,x);
    }
  this->Transformation->Update();
}

void vtkpxElectrodeGridToSurfaceFilter::GetCurrentParameters(vtkDoubleArray* params)
{
  int np=this->Transformation->GetSourceLandmarks()->GetNumberOfPoints();
  if (params->GetNumberOfTuples()!=np*3)
    {
      params->SetNumberOfComponents(1);
      params->SetNumberOfTuples(np*3);
    }

  vtkPoints* points=this->Transformation->GetSourceLandmarks();

  int index=0;
  for (int i=0;i<np;i++)
    {
      double x[3]; points->GetPoint(i,x);
      for (int j=0;j<=2;j++)
	{
	  params->SetComponent(index,0,x[j]);
	  ++index;
	}
    }


}



