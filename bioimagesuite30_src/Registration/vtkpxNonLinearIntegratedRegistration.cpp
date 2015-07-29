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





/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxNonLinearIntegratedRegistration.cpp,v $
  Language:  C++
  Date:      $Date: 2002/04/16 17:51:27 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkpxNonLinearIntegratedRegistration.h"
#include "vtkObjectFactory.h"
#include "pxutil.h"
#include "vtkbisImageReslice.h"
#include "vtkpxUtil.h"
#include "vtkpxImageExtractVOI.h"
#include "vtkpxOptimizer.h"
#include "vtkMath.h"

vtkpxNonLinearIntegratedRegistration* vtkpxNonLinearIntegratedRegistration::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxNonLinearIntegratedRegistration");
  if(ret)
    {
    return (vtkpxNonLinearIntegratedRegistration*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxNonLinearIntegratedRegistration;
  
}


vtkpxNonLinearIntegratedRegistration::vtkpxNonLinearIntegratedRegistration()
{
  this->RPMEstimator=vtkpxRPMCorrespondenceFinder::New();
  this->PointsWeight=0.01;
  this->WholeImage=1;
  this->MarginSize=0.1;
  this->FixedCorrespondences=1;
}

vtkpxNonLinearIntegratedRegistration::~vtkpxNonLinearIntegratedRegistration() 
{
  if (this->RPMEstimator!=NULL)
    this->RPMEstimator->Delete();
}

//-------------------------------------------------------------------------

int vtkpxNonLinearIntegratedRegistration::InitializeAll() 
{
  vtkpxNonLinearRegistration::InitializeAll();
  if (this->WholeImage==1)
    {
      fprintf(stderr,"Using Whole Image\n");
      return 1;
    }

  
  double bounds[6];
  vtkPolyData* source=this->RPMEstimator->GetSource();
  if (source!=NULL)
    source->GetPoints()->GetBounds(bounds);

  fprintf(stderr,"Source Bounds = %.1f:%.1f %.1f:%.1f %.1f:%.1f\n",
	  bounds[0],bounds[1],
	  bounds[2],bounds[3],
	  bounds[4],bounds[5]);

  vtkImageData* tmp=vtkImageData::New();
  tmp->CopyStructure(this->ReferenceImage);
  tmp->SetScalarTypeToUnsignedChar();
  tmp->AllocateScalars();

  bounds[0]=bounds[0]-this->MarginSize*(bounds[1]-bounds[0]);
  bounds[1]=bounds[1]+this->MarginSize*(bounds[1]-bounds[0]);
  bounds[2]=bounds[2]-this->MarginSize*(bounds[3]-bounds[2]);
  bounds[3]=bounds[3]+this->MarginSize*(bounds[3]-bounds[2]);
  bounds[4]=bounds[4]-this->MarginSize*(bounds[5]-bounds[4]);
  bounds[5]=bounds[5]+this->MarginSize*(bounds[5]-bounds[4]);

  fprintf(stderr,"Enlarged Bounds (%.2f) = %.1f:%.1f %.1f:%.1f %.1f:%.1f\n",
	  this->MarginSize,
	  bounds[0],bounds[1],
	  bounds[2],bounds[3],
	  bounds[4],bounds[5]);

  double ori[3]; tmp->GetOrigin(ori);
  double sp[3];  tmp->GetSpacing(sp);


  bounds[0]=(bounds[0]-ori[0])/sp[0];
  bounds[1]=(bounds[1]-ori[0])/sp[0];
  bounds[2]=(bounds[2]-ori[1])/sp[1];
  bounds[3]=(bounds[3]-ori[1])/sp[1];
  bounds[4]=(bounds[4]-ori[2])/sp[2];
  bounds[5]=(bounds[5]-ori[2])/sp[2];

  fprintf(stderr,"Voxel Bounds (%.2f) = %.1f:%.1f %.1f:%.1f %.1f:%.1f\n",
	  this->MarginSize,
	  bounds[0],bounds[1],
	  bounds[2],bounds[3],
	  bounds[4],bounds[5]);


  int   dim[3]; tmp->GetDimensions(dim);

  vtkDataArray* scal=tmp->GetPointData()->GetScalars();
  int index=0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  if (i>bounds[0] && i<bounds[1] && j>bounds[2] && j<bounds[3] && k>bounds[4] && k<bounds[5])
	    scal->SetComponent(index,0,1);
	  else
	    scal->SetComponent(index,0,0);
	  ++index;
	}

  this->SetImageMask(tmp);

  tmp->Delete();
  return 1;

}
//-------------------------------------------------------------------------
int vtkpxNonLinearIntegratedRegistration::Initialize(int level) 
{
  // Different Procedure for toplevel and remaining levels
  // In top level initialize using InitialTransform
  // in other levels initialize using previous grid transform

  vtkpxNonLinearRegistration::Initialize(level);

  if (level==this->NumberOfLevels)
    RPMEstimator->Initialize(NULL);
  this->GridTransform->FormCollectionStructure(RPMEstimator->GetPointsX(),RPMEstimator->GetCollection(),0.001);
  return 1;
}
//-------------------------------------------------------------------------
double vtkpxNonLinearIntegratedRegistration::Evaluate()
{
  if (this->LastTargetImage==NULL)
    this->LastTargetImage=vtkImageData::New();

  vtkpxRegistration::ResliceImage(this->LastTargetImage,this->SampledTransformImage,
				  this->SampledReferenceImage,this->ComboTransform,1);
  this->Histogram->FillHistogram(this->SampledReferenceImage,this->LastTargetImage,1);  
  this->LastSimilarity=this->Similarity(this->Histogram);

  if (this->Lambda>0.000001)
    this->LastSmoothness=this->ComputeInternalEnergy(this->GridTransform);
  else
    this->LastSmoothness=0.0;
  //  fprintf(stderr,"Last sim=%.4f lastsmo=%.4f \t",this->LastSimilarity,this->LastSmoothness);
  
  this->LastFitError=this->ComputeTotalApproximationError(this->LastFitAbsError,0);
  /*double v2=0.0;
    double v1=this->GridTransform->TestWeightedApproximation(RPMEstimator->GetPointsX(),
							   RPMEstimator->GetPointsVY(),
							   RPMEstimator->GetM_Outliers_Column(),
							   v2);
  
    fprintf(stderr,"Comparing errors direct (%.3f,%.3f)  new (%.3f,%.3f)\n",
    v1,v2,this->LastFitAbsError,this->LastFitError);*/


  return this->LastSimilarity-this->LastSmoothness*this->Lambda-this->PointsWeight*this->LastFitError;
}
//-------------------------------------------------------------------------

void vtkpxNonLinearIntegratedRegistration::GenerateOutput(int level, int step,int iter,double stepsize,
							  double actualSimilarity)
{
  if (iter>=100)
    iter=99;

  sprintf(this->OutputStatus,"Level=%2d Step=%2d iter=%2d stepsize=%.3f: sim=%8.6f (int=%8.6f pt2=%8.6f, l*sm=%8.6f (sm=%9.6f, ptd=%8.6f))\n",
	  level,step,iter,stepsize,actualSimilarity,
	  this->LastSimilarity,this->LastFitError,this->LastSmoothness*this->Lambda,
	  this->LastSmoothness,this->LastFitAbsError);
  this->UpdateProgress(this->LastProgress);
  //  vtkpxAbstractIntensityRegistration::GenerateOutput(level,step,iter,stepsize,actualSimilarity);
  return;
}


//-------------------------------------------------------------------------
// Optimizer Stuff 
//-------------------------------------------------------------------------
double vtkpxNonLinearIntegratedRegistration::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)

{

  if (grad->GetNumberOfComponents()!=1 || grad->GetNumberOfTuples()!=this->NumberOfControlPoints*3)
    {
      grad->SetNumberOfComponents(1);
      grad->SetNumberOfTuples(this->NumberOfControlPoints*3);
    }

  OptimizationStructure.old_similarity=this->OptimizerValue(params);

  // Calculate gradient 
  double GradientNorm = 0;

  int per=this->NumberOfControlPoints/10;
  int count=0;
  int count2=0;
  
  grad->FillComponent(0,0.0);

  
  //  this->GenerateOutput(OptimizationStructure.level,
  //		       -1,-1,OptimizationStructure.step,
  //		       OptimizationStructure.old_similarity);
  fprintf(stderr,"\nOptimizer Computing Gradients : (%5.3f)",OptimizationStructure.step);

  //int mid=this->NumberOfControlPoints;

  RPMEstimator->GetCollection()->InitTraversal();

  int debug=0;
  for (int i = 0; i < this->NumberOfControlPoints; i++)
    {
      vtkFloatArray* indices=(vtkFloatArray*)RPMEstimator->GetCollection()->GetNextItemAsObject();

      ++count;
      if (count==per)
	{
	  fprintf(stderr,"%.1f ",double(count2)*10.0);
	  count=0;
	  ++count2;
	  if (count2==10)
	    fprintf(stderr,"\n");
	}

      if (this->Status->GetComponent(i,0)+
	  this->Status->GetComponent(i,1)+
	  this->Status->GetComponent(i,2) > 0 )
	{
	  double p[3]; this->GridTransform->GetControlPointLocation(i,p);
	  // Determine the area in which we have to update our similarity measure
	  // p1 = upper left , p2=lower right

	  // Extract Relevant Portion of the Image
	  vtkpxImageExtractVOI* voi=vtkpxImageExtractVOI::New();
	  voi->SetInput(this->SampledReferenceImage);
	  voi->SetRegion(p,OptimizationStructure.gridspacing,this->WindowSize);
	  voi->Update();

	  vtkpxImageExtractVOI* voi2=vtkpxImageExtractVOI::New();
	  voi2->SetInput(this->LastTargetImage);
	  voi2->SetRegion(p,OptimizationStructure.gridspacing,this->WindowSize);
	  voi2->Update();

	  for (int j=0;j<=2;j++)
	    {
	      if (this->Status->GetComponent(i,j)>0)
		{
		  this->GridTransform->ShiftControlPoint(i,j,-OptimizationStructure.step);
		  
		  double e1 = this->EvaluatePiece(voi->GetOutput(),
						  this->SampledTransformImage,
						  voi2->GetOutput(),NULL,NULL,i);
		  double pe1_2=0.0;
		  if (this->PointsWeight>0.00001)
		    pe1_2=this->ComputeApproximationErrorAtControlPoint(indices,debug);

		  
		  double l1=0.0;
		  if (this->Lambda>0.0001)
		    l1 = this->ComputeInternalEnergy(this->GridTransform,i);
		  double s1=e1-this->Lambda*l1-this->PointsWeight*pe1_2;
		  
		  this->GridTransform->ShiftControlPoint(i,j,+2.0*OptimizationStructure.step);
		  double e2 =this->EvaluatePiece(voi->GetOutput(),
						 this->SampledTransformImage,
						 voi2->GetOutput(),NULL,NULL,i);
		  double pe2_2=0.0;
		  if (this->PointsWeight>0.00001)
		    if (this->PointsWeight>0.00001)
		      pe2_2=this->ComputeApproximationErrorAtControlPoint(indices,debug);
		  
		  double l2=0.0;
		  if (this->Lambda>0.0001)
		    l2 = this->ComputeInternalEnergy(this->GridTransform,i);
		  double s2=e2-l2*this->Lambda-this->PointsWeight*pe2_2;

		  this->GridTransform->ShiftControlPoint(i,j,-OptimizationStructure.step);
		  grad->SetComponent(i*3+j,0,-(s2-s1));
		  GradientNorm+=pow(s2-s1,2.0);
		}
	    }
	  voi->Delete();
	  voi2->Delete();
	}
    }

  GradientNorm = (sqrt(GradientNorm));
  double sc=OptimizationStructure.step/GradientNorm;

  // Normalize vector
  if (GradientNorm > 0)
    this->GridTransform->ScaleGradients(grad,sc);
  else 
    grad->FillComponent(0,0.0);
  fprintf(stderr,"Gradient Norm=%.5f scale=%.5f\n",GradientNorm,sc);
  return GradientNorm;

}
//-------------------------------------------------------------------------
int vtkpxNonLinearIntegratedRegistration::ComputeRegistration(int level)
{  
  /*  this->RPMEstimator->WarpAndUpdateCorrespondence(this->GridTransform,this->RPMEstimator->GetTemperature());  
  vtkpxNonLinearRegistration::ComputeRegistration(level);
  return 1;*/

  fprintf(stderr,"Computing Registration level=%d\n",level);
  fprintf(stderr,"Updating RPM Estimator Correspondence stuff\n");
  this->RPMEstimator->WarpAndUpdateCorrespondence(this->GridTransform,this->RPMEstimator->GetTemperature());  

  fprintf(stderr,"On to estimation\n");
  double step=this->CurrentStepSize;
  double origin[3],spacing[3],gridspacing[3];
  int   dimensions[3];
  this->GridTransform->GetDisplacementGrid()->GetSpacing(gridspacing);
  this->SampledReferenceImage->GetOrigin(origin);
  this->SampledReferenceImage->GetSpacing(spacing);
  this->SampledReferenceImage->GetDimensions(dimensions);
  
  vtkDoubleArray* grad=vtkDoubleArray::New();
  grad->SetNumberOfComponents(3);
  grad->SetNumberOfTuples(this->NumberOfControlPoints);




  double origptsweights=this->PointsWeight;

  for (int st=1;st<=this->NumberOfSteps;st++)
    {
      double old_similarity = this->Evaluate();
      this->PointsWeight=origptsweights*double(st)/double(this->NumberOfSteps);
      fprintf(stderr,"\n ---------------------------------------------------- \n Points Weight=%.5f\n\n",this->PointsWeight);
      double new_similarity=this->Optimize(grad,old_similarity,step,level,
					  origin,spacing,dimensions,gridspacing);
      this->GenerateOutput(level,st,this->NumberOfIterations,step,new_similarity);
    }

  grad->Delete();
  return 1;
}
//-------------------------------------------------------------------------
double vtkpxNonLinearIntegratedRegistration::OnBeginIteration(vtkDoubleArray* )
{
  if (this->FixedCorrespondences==0)
    {
      fprintf(stderr,"Updating Corresspondences!");
      this->RPMEstimator->WarpAndUpdateCorrespondence(this->GridTransform,this->RPMEstimator->GetTemperature());
    }
  return 1.0;
}

int vtkpxNonLinearIntegratedRegistration::FinalizeAll()
{
  return vtkpxNonLinearRegistration::FinalizeAll();
}

//-------------------------------------------------------------------------
double vtkpxNonLinearIntegratedRegistration::ComputeApproximationErrorAtControlPoint(vtkFloatArray* indices,int debug)
{
  
  /*  double other=0.0;
  this->GridTransform->TestWeightedApproximation(RPMEstimator->GetPointsX(),
						 RPMEstimator->GetPointsVY(),
						 RPMEstimator->GetM_Outliers_Column(),
						 other);*/

  vtkPoints* points=RPMEstimator->GetPointsX();
  vtkPoints* points2=RPMEstimator->GetPointsVY();
  vtkpxMatrix* lsqweights=RPMEstimator->GetM_Outliers_Column();
  int useweights=0;
  if (lsqweights!=NULL)
    {
      if (lsqweights->GetSize()[0]>=points->GetNumberOfPoints())
	useweights=1;
    }

  int np=indices->GetNumberOfTuples();
  double nump=double(points->GetNumberOfPoints());
  double x[3],tx[3],sum2=0.0;

  if (debug>0)
    fprintf(stderr,"\nApproximation error debug=%d, np=%d\t",debug,np);

  if (np>0)
    {
      for (int ki=0;ki<np;ki++)
	{
	  int i=(int)indices->GetComponent(ki,0);
	  double w=indices->GetComponent(ki,1);
	  points->GetPoint(i,x);
	  points2->GetPoint(i,tx);
	  this->GridTransform->TransformPoint(x,x);
	  double dist= (vtkMath::Distance2BetweenPoints(x, tx));
	  double w2=1.0;
	  if (useweights)
	    w2=lsqweights->GetDirectElement(i,0);
	  sum2+=w*w2*dist;
	}
      sum2/=(nump);
      if (debug>0)
	fprintf(stderr,"Error=%.8f\n",sum2);
      return sum2;
    }
  return 0.0;
}

double vtkpxNonLinearIntegratedRegistration::ComputeTotalApproximationError(double& lastfiterror,int debug)
{
  vtkPoints* points=RPMEstimator->GetPointsX();
  vtkPoints* points2=RPMEstimator->GetPointsVY();
  int nc=points->GetNumberOfPoints();

  vtkpxMatrix* lsqweights=RPMEstimator->GetM_Outliers_Column();    
  int useweights=0;
  if (lsqweights!=NULL)
    {
      if (lsqweights->GetSize()[0]>=points->GetNumberOfPoints())
	useweights=1;
    }

  lastfiterror=0.0;
  double sum2error=0.0;

  if (nc<1)
    return 0.0;

  double x[3],tx[3];
  for (int i=0;i<nc;i++)
    {
      points->GetPoint(i,x);
      points2->GetPoint(i,tx);
      this->GridTransform->TransformPoint(x,x);
      double d=vtkMath::Distance2BetweenPoints(x,tx);
      double w=1.0;
      if (useweights)
	w=lsqweights->GetDirectElement(i,0);
      lastfiterror+=w*sqrt(d);
      sum2error+=w*d;
    }
  lastfiterror/=double(nc);
  sum2error/=double(nc);

  if (debug)
    fprintf(stderr,"Total Error =%.6f (%.6f) \n",lastfiterror,sum2error);

  return sum2error;
 
}
//-------------------------------------------------------------------------
int vtkpxNonLinearIntegratedRegistration::InitializeTransformation(int level) 
{
  int proceed=0;


  if (level==1 && this->NumberOfLevels==1)
    {
      if (this->InitialTransform!=NULL)
	{
	  if (this->InitialTransform->IsA("vtkpxComboTransform")==1)
	    {
	      proceed=1;
	    }
	}
    }

  if (proceed==0)
    {
      return vtkpxNonLinearRegistration::InitializeTransformation(level);
    }

  fprintf(stderr,"In vtkpxNonLinearIntegratedRegistration::InitializeTransform\n");

  this->LinearTransform=vtkTransform::New();
  this->GridTransform=vtkpxGridTransform::New();
  this->ComboTransform=vtkpxComboTransform::New();
  
  vtkpxComboTransform* x=(vtkpxComboTransform*)(this->InitialTransform);
  this->LinearTransform->Identity();
  this->LinearTransform->Concatenate(x->GetLinearTransform()->GetMatrix());
  this->ComboTransform->SetNonLinearFirst(x->GetNonLinearFirst());
  
  vtkImageData* tmp=vtkImageData::New();
  tmp->DeepCopy(x->GetGridTransform()->GetDisplacementGrid());
  
  this->GridTransform->SetInterpolationMode(x->GetGridTransform()->GetInterpolationMode());
  this->GridTransform->SetDisplacementGrid(tmp);
  tmp->Delete();
  
  this->ComboTransform->SetTransformations(this->LinearTransform,this->GridTransform);
  this->NumberOfControlPoints=this->GridTransform->GetNumberOfControlPoints();
  fprintf(stderr,"In vtkpxNonIntegratedLinearRegistration::InitializeTransform Done\n");
  return 1;
}

