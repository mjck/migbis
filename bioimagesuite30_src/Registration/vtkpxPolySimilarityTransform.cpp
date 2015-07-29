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
  Module:    $RCSfile: vtkpxPolySimilarityTransform.cpp,v $
  Language:  C++
  Date:      $Date: 2003/04/11 20:00:25 $
  Version:   $Revision: 1.2 $


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
#include "vtkAppendPolyData.h"
#include "vtkContourFilter.h"
#include "vtkDataArray.h"
#include "vtkDecimatePro.h"
#include "vtkGlyph3D.h"
#include "vtkImageExtractComponents.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageMathematics.h"
#include "vtkLinearTransform.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkSphereSource.h"
#include "vtkpxAnalyzeImageSource.h"
#include "vtkpxAnalyzeImageWriter.h"
#include "vtkpxArticulatedJointTransform.h"
#include "vtkpxAppendComponents.h"
#include "vtkpxOptimizer.h"
#include "vtkpxPolySimilarityTransform.h"
#include "vtkpxUtil.h"
#include "vtkpxTransformationUtil.h"
#include "vtkImageDataGeometryFilter.h"
#include "vtkThresholdPoints.h"
#include "vtkGridTransform.h"
#include "vtkImageThreshold.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkCleanPolyData.h"
#include "vtkPolyDataWriter.h"
#include "vtkThinPlateSplineTransform.h"
#include "vtkPolyDataConnectivityFilter.h"
//----------------------------------------------------------------------------
vtkpxPolySimilarityTransform* vtkpxPolySimilarityTransform::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxPolySimilarityTransform");
  if(ret)
    {
      return (vtkpxPolySimilarityTransform*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxPolySimilarityTransform;
}

//------------------------------------------------------------------------
vtkpxPolySimilarityTransform::vtkpxPolySimilarityTransform()
{
  vtkpxArticulatedJointTransform* tr=vtkpxArticulatedJointTransform::New();
  tr->SetModeToRotationScaleTranslation();
  this->SetTransform(0,tr);
  tr->Delete();
  this->RegionLabelImageName=NULL; this->SetRegionLabelImageName("Joint");

}
//------------------------------------------------------------------------
vtkpxPolySimilarityTransform::~vtkpxPolySimilarityTransform()
{
  if (this->RegionLabelImageName!=NULL)
    delete [] RegionLabelImageName;

}
//------------------------------------------------------------------------
void vtkpxPolySimilarityTransform::SetCylinderPreset(int numparts)
{
  if (numparts<2)
    numparts=2;
  else if (numparts>3)
    numparts=3;

  char line[40];

  this->TransformationCollection->RemoveAllItems();
  vtkpxArticulatedJointTransform* tr[2];

  for (int i=0;i<numparts;i++)
    {
      tr[i]=vtkpxArticulatedJointTransform::New();
      if (i==0)
	{
	  tr[i]->SetModeToRotationTranslation();
	  tr[i]->SetBlendedMode(0);
	}
      else 
	{
	  tr[i]->SetModeToRotationOnly();
	  fprintf(stderr,"Setting Mode to Rotation Only\n");
	  tr[i]->SetBlendedMode(1);
	  tr[i]->SetAlignmentTransform(tr[i-1]);
	  tr[i]->SetAlignmentTransformIndex(i-1);
	}

      tr[i]->SetChildRegionLabelIndex(i+1);
      tr[i]->SetChildRegionLabelIndex2(numparts-1);
      tr[i]->ExternalSetRegionLabelImage(this->RegionLabel);
      tr[i]->SetRegionLabelIndex(i);
      sprintf(line,"Joint_%d",i+1);
      tr[i]->SetJointName(line);
    }

  for (int i=0;i<numparts;i++)
    {
      this->SetTransform(i,tr[i]);
      tr[i]->Delete();
    }

}


//------------------------------------------------------------------------
void vtkpxPolySimilarityTransform::SetWholeBodyPreset(int doscale,int hasfeet,
						      int hashindlimbs,int hasforelimbs,int hashead, int taildivisions,
						      int spinedivisions)
{
  this->TransformationCollection->RemoveAllItems();

  if (spinedivisions>10)
    spinedivisions=10;
  else if (spinedivisions<1)
    spinedivisions=0;

  hasfeet=(hasfeet>0);
  hashead=(hashead>0);
  hashindlimbs=(hashindlimbs>0);
  hasforelimbs=(hasforelimbs>0);
  hashead=(hashead>0);

  char line[100],partname[50];
  
  if (taildivisions<1)
    taildivisions=0;
  else if (taildivisions>5)
    taildivisions=5;

  int legparts=2+hasfeet;
  
  int totalparts=
    1 + 2*legparts*(hashindlimbs+hasforelimbs) +
    hashead +
    spinedivisions +
    taildivisions;


  // Create Defaults
  vtkpxArticulatedJointTransform* tr[30];
  for (int i=0;i<totalparts;i++)
    {
      tr[i]=vtkpxArticulatedJointTransform::New();
      tr[i]->SetModeToRotationOnly();
      tr[i]->SetBlendedMode(2);
      tr[i]->SetRegionLabelIndex(i);
      tr[i]->ExternalSetRegionLabelImage(this->RegionLabel);
      tr[i]->SetRegionLabelIndex(i);
      this->SetTransform(i,tr[i]);
      tr[i]->Delete();
    }

  for (int i=0;i<totalparts;i++)
    tr[i]=this->GetTransform(i);

  // One part at a time --- first the abdomen core part
  tr[0]->SetBlendedMode(0);
  if (doscale)
    tr[0]->SetModeToRotationScaleTranslation();
  else
    tr[0]->SetModeToRotationTranslation();
  tr[0]->SetChildRegionLabelIndex(1);
  tr[0]->SetChildRegionLabelIndex2(totalparts);
  tr[0]->SetJointName("Global_0");

  int shoulderjoint = totalparts-(1+hashead);

  if (hashindlimbs == 1 || hasforelimbs == 1 ) 
    {
      int minleg=0;
      int maxleg=3;
      if (hashindlimbs==0)
	minleg=2;
      if (hasforelimbs ==0)
	maxleg=1;
    
      for (int leg=minleg;leg<=maxleg;leg++)
	{
	  if (leg==0 || leg==2)
	    sprintf(partname,"Left");
	  else
	    sprintf(partname,"Right");
	  
	  int baseindex=leg*legparts+1;
	  int maxindex =baseindex+legparts-1;
	  int index=baseindex;
	  
	  for (int part=0;part<legparts;part++)
	    {
	      if (part==0)
		{
		  tr[index]->SetModeToRotationOnly();
		  if (leg<2)
		    {
		      tr[index]->SetAlignmentTransform(tr[0]);
		      tr[index]->SetAlignmentTransformIndex(0);
		    }
		  else
		    {
		      tr[index]->SetAlignmentTransform(tr[shoulderjoint]);
		      tr[index]->SetAlignmentTransformIndex(shoulderjoint);
		    }

		  if (leg<2)
		    sprintf(line,"%s-Hipp_%d",partname,index);
		  else
		    sprintf(line,"%s-Shoulder_%d",partname,index);
		  tr[index]->SetJointName(line);

		}
	      else // either abdomen parts or knee/ankle
		{
		  tr[index]->SetModeToRotationXYOnly();
		  tr[index]->SetAlignmentTransform(tr[index-1]);
		  tr[index]->SetAlignmentTransformIndex(index-1);
		  
		  if (leg<2)
		    {
		      if (part==1)
			sprintf(line,"%s-Knee_%d",partname,index);
		      else
			sprintf(line,"%s-Ankle_%d",partname,index);
		    }
		  else
		    {
		      if (part==1)
			sprintf(line,"%s-Elbow_%d",partname,index);
		      else
			sprintf(line,"%s-Wrist_%d",partname,index);
		    }
		  tr[index]->SetJointName(line);
		}
	      tr[index]->SetChildRegionLabelIndex(index+1);
	      tr[index]->SetChildRegionLabelIndex2(maxindex);
	      ++index;
	    }
	}
    }

  if ((spinedivisions+hashead)> 0 || taildivisions > 0 ) 
    {
      int cut[3];
      cut[0]=legparts*2*(hashindlimbs+hasforelimbs)+1;
      cut[1]=cut[0]+taildivisions;
      cut[2]=totalparts;

      int minregion=0,maxregion=1;
      if (taildivisions==0)
	minregion=1;
      if( (spinedivisions+hashead)==0)
	maxregion=0;

      for (int region=minregion;region<=maxregion;region++)
	{
	  int baseindex=cut[region];
	  int maxindex=cut[region+1]-1;
	  int index=baseindex;
	  int numparts=maxindex-baseindex+1;

	  for (int part=0;part<numparts;part++)
	    {
	      if (region==1)
		tr[index]->SetModeToRotationOnly(); // Abdomen is fully flexed
	      else
		tr[index]->SetModeToRotationXYOnly(); // Tail is mostly circular anyway

	      if (part==0)
		{
		  tr[index]->SetAlignmentTransform(tr[0]);
		  tr[index]->SetAlignmentTransformIndex(0);
		}
	      else
		{
		  tr[index]->SetAlignmentTransform(tr[index-1]);
		  tr[index]->SetAlignmentTransformIndex(index-1);
		}
	      tr[index]->SetChildRegionLabelIndex(index+1);
	      tr[index]->SetChildRegionLabelIndex2(maxindex);


	      if (region==0)
		sprintf(line,"Tail_%d",index);
	      else if (hashead==1 && index==maxindex)
		sprintf(line,"Neck_%d",index);
	      else
		sprintf(line,"Spine_%d",index);

	      tr[index]->SetJointName(line);
	      ++index;
	    }
	}
    }
}

//------------------------------------------------------------------------
void vtkpxPolySimilarityTransform::SetRegionLabel(vtkImageData* h)
{
  vtkpxPolyAbstractTransform::SetRegionLabel(h);
  int np=this->TransformationCollection->GetNumberOfItems();
  for (int i=0;i<np;i++)
    {
      vtkpxArticulatedJointTransform* tr=this->GetTransform(i);
      tr->ExternalSetRegionLabelImage(this->RegionLabel);
      tr->SetRegionLabelIndex(i);
      tr->Modified();
    }

}

//------------------------------------------------------------------------
void vtkpxPolySimilarityTransform::SetTransform(int i,vtkpxArticulatedJointTransform* tr)
{
  int np=this->TransformationCollection->GetNumberOfItems();
  if (i>=np)
    {
      this->TransformationCollection->AddItem(tr);
    }
  else
    {
      this->TransformationCollection->ReplaceItem(i,tr);
    }

}
vtkpxArticulatedJointTransform* vtkpxPolySimilarityTransform::GetTransform(int i)
{
  int np=this->TransformationCollection->GetNumberOfItems();
  if (np==0 || i>=np || i<0)
    return NULL;

  return (vtkpxArticulatedJointTransform*)this->TransformationCollection->GetItemAsObject(i);
}
// ---------------------------------------------------------------------------
int vtkpxPolySimilarityTransform::GetNumberOfTransformations()
{
  return this->TransformationCollection->GetNumberOfItems();
 
}
// ---------------------------------------------------------------------------
int vtkpxPolySimilarityTransform::Load(const char* fname)
{
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;

  char pxtk_buffer[255];
  gzgets(fin,pxtk_buffer,100);
  if (gzfindstringinstring(pxtk_buffer,"#vtkpxPolySimilarityTransform2")==0)
    {
      gzclose(fin);
      return 0;
    }

  gzgets(fin,pxtk_buffer,100);
  gzgets(fin,pxtk_buffer,200);
  int numtransforms;      
  sscanf(pxtk_buffer,"%d",&numtransforms);
  if (numtransforms<1)
    {
      gzclose(fin);
      return 0;
    }
  
  gzgets(fin,pxtk_buffer,200);
  gzgets(fin,pxtk_buffer,200);
  int a=strlen(pxtk_buffer);  pxtk_buffer[a-1]=(char)0;
  this->SetRegionLabelImageName(pxtk_buffer);

  this->TransformationCollection->RemoveAllItems();

  for (int i=0;i<numtransforms;i++)
    {
      gzgets(fin,pxtk_buffer,200);
      gzgets(fin,pxtk_buffer,200);
      vtkpxArticulatedJointTransform* tr=vtkpxArticulatedJointTransform::New();
      tr->Load(fin);
      this->SetTransform(i,tr);
      tr->Delete();
    }

  if (this->ThinPlateSplineTransform==NULL)
    this->ThinPlateSplineTransform=vtkThinPlateSplineTransform::New();
  
  int ok=vtkpxTransformationUtil::LoadThinPlateSplineTransform(this->ThinPlateSplineTransform,fin);
  if (ok==0)
    {
      this->ThinPlateSplineTransform->Delete();
      this->ThinPlateSplineTransform=NULL;
    }

  gzclose(fin);

  // Fix Alignment Transform Stuff -- i.e. re-establish hierarchy
  for (int i=0;i<numtransforms;i++)
    {
      vtkpxArticulatedJointTransform* tr=this->GetTransform(i);
      int ind=tr->GetAlignmentTransformIndex();
      if (ind>=0 && ind<numtransforms && ind!=i)
	tr->SetAlignmentTransform(this->GetTransform(ind));
    }



  vtkpxAnalyzeImageSource* w=vtkpxAnalyzeImageSource::New();
  w->Load(this->RegionLabelImageName);

  this->SetRegionLabel(w->GetOutput());
  w->Delete();
  
  return 1;
}
// ---------------------------------------------------------------------------
int vtkpxPolySimilarityTransform::Save(const char* fname)
{
  if (this->IsValid()==0)
    return 0;

  /*  vtkpxAnalyzeImageWriter* w=vtkpxAnalyzeImageWriter::New();
  w->SetInput(this->RegionLabel);
  w->SetCompression(9);
  w->Save(fname);*/
  
  int np=this->TransformationCollection->GetNumberOfItems();


  FILE* fout=fopen(fname,"w");
  fprintf(fout,"#vtkpxPolySimilarityTransform2\n");
  fprintf(fout,"#Number Of Transformations\n");
  fprintf(fout,"%d\n",np);
  fprintf(fout,"#RegionLabelImageName\n%s\n",this->RegionLabelImageName);
  for (int i=0;i<np;i++)
    {
      fprintf(fout,"#*****************************\n#Transformation %d\n",i+1);
      vtkpxArticulatedJointTransform* tr=this->GetTransform(i);
      tr->Save(fout);
    }

  if (this->ThinPlateSplineTransform==NULL)
    fprintf(fout,"No Thin Plate Spline Transform\n");
  else
    vtkpxTransformationUtil::SaveThinPlateSplineTransform(this->ThinPlateSplineTransform,fout);

  fclose(fout);
  return 1;
}
// ---------------------------------------------------------------------------
int  vtkpxPolySimilarityTransform::GetNumberOfParameters()
{
  int n=0;
  int np=this->TransformationCollection->GetNumberOfItems();
  for (int i=0;i<np;i++)
    n+= this->GetTransform(i)->GetNumberOfParameters();

  return n;
}
// ---------------------------------------------------------------------------
void vtkpxPolySimilarityTransform::SetCurrentParameters(vtkDoubleArray* params)
{
  int nt=params->GetNumberOfTuples();
  int np=this->GetNumberOfParameters();
  
  if (nt<np)
    {
      vtkErrorMacro(<<"Insufficient number of parameters " << nt << "," << np);
      return;
    }

  int nc=this->TransformationCollection->GetNumberOfItems();
  int offset=0;
  for (int j=0;j<nc;j++)
    {
      vtkpxArticulatedJointTransform* tr=this->GetTransform(j);
      int nump=tr->GetNumberOfParameters();
	if (nump>0)
	  {
	    tr->SetParameters(params,offset);
	    offset+= nump;
	  }
    }
}
// ---------------------------------------------------------------------------
void vtkpxPolySimilarityTransform::GetCurrentParameters(vtkDoubleArray* params)
{
 int nt=params->GetNumberOfTuples();
 int np=this->GetNumberOfParameters();
  
  if (nt<np)
    {
      vtkErrorMacro(<<"Insufficient number of parameters " << nt << "," << np);
      return;
    }

  int nc=this->TransformationCollection->GetNumberOfItems();
  int offset=0;
  for (int j=0;j<nc;j++)
    {
      vtkpxArticulatedJointTransform* tr=this->GetTransform(j);
      int nump=tr->GetNumberOfParameters();
	if (nump>0)
	  {
	    tr->GetParameters(params,offset);
	    offset+= nump;
	  }
    }
}


// ---------------------------------------------------------------------------
// Optimization Stuff
// ---------------------------------------------------------------------------
// Description:
// These are needed for the Optimizer 
void vtkpxPolySimilarityTransform::OptimizerFeedback(vtkObject* self,const char* line,double val)
{
  
  fprintf(stderr,"%s %f\n",line,val);
}

double vtkpxPolySimilarityTransform::CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output)
{
  return ((vtkpxPolySimilarityTransform*)self)->OptimizerGradient(position,output);
}

double vtkpxPolySimilarityTransform::CalculateFunction(vtkObject* self,vtkDoubleArray* position)
{
  return ((vtkpxPolySimilarityTransform*)self)->OptimizerValue(position);
}

// ---------------------------------------------------------------------------
double vtkpxPolySimilarityTransform::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)
{
  if (grad->GetNumberOfComponents()!=1 || grad->GetNumberOfTuples()!=OptimizationStructure.maxdofs)
    {
      grad->SetNumberOfComponents(1);
      grad->SetNumberOfTuples(OptimizationStructure.maxdofs);
    }

  grad->FillComponent(0,0.0);
  //double old_similarity=this->OptimizerValue(params);
  double GradientNorm = 0;

  for (int i=0;i<OptimizationStructure.maxdofs;i++)
    {
      double orig=params->GetComponent(i,0);
      params->SetComponent(i, 0, orig + this->OptimizationStructure.step);
      double a=this->OptimizerValue(params);
      
      params->SetComponent(i, 0, orig - this->OptimizationStructure.step);
      double b=this->OptimizerValue(params);

      params->SetComponent(i, 0, orig);
      this->SetCurrentParameters(params);
      
      double g=-0.5*(b-a)/this->OptimizationStructure.step;
      grad->SetComponent(i,0,g);
      GradientNorm+=pow(g,2.0);
    }
  GradientNorm = (sqrt(GradientNorm));


  // -------------------------
  // Hirohito Found this One!!
  // -------------------------
  for (int j=0;j<OptimizationStructure.maxdofs;j++)
    grad->SetComponent(j,0,grad->GetComponent(j,0)/GradientNorm);
      
  return GradientNorm;
}
// ---------------------------------------------------------------------------
double vtkpxPolySimilarityTransform::OptimizerValue(vtkDoubleArray* params)
{
  this->SetCurrentParameters(params);
  return this->Evaluate();
}
// ---------------------------------------------------------------------------

double vtkpxPolySimilarityTransform::Evaluate()
{
  
  vtkPoints* p1=OptimizationStructure.sourcePoints;
  vtkPoints* p2=OptimizationStructure.targetPoints;

  int useweights=0;
  int np=p1->GetNumberOfPoints();
  if (OptimizationStructure.weights!=NULL)
    {
      if (OptimizationStructure.weights->GetSize()[0]==np)
	useweights=1;
    }

  double p[3],q[3];
  double sum=0.0,nsum=0.00001;
  for (int i=0;i<np;i++)
    {
      p1->GetPoint(i,p);
      p2->GetPoint(i,q);
      this->TransformPoint(p,p);

      double w=1.0;
      if (useweights)
	w=OptimizationStructure.weights->GetElement(i,0);
      
      sum+=w*vtkMath::Distance2BetweenPoints(p,q);
      nsum+=w;
    }

  return sum/nsum;
}
// ---------------------------------------------------------------------------
double vtkpxPolySimilarityTransform::ApproximatePoints(vtkPoints* SourceLandmarks,vtkPoints* TargetLandmarks,vtkpxMatrix* Weights,
						       int numsteps,float stepsize,float tolerance)
{
  if (SourceLandmarks==NULL || TargetLandmarks==NULL)
    {
      vtkErrorMacro(<<"One or more Point Sets=NULL in input to ApproximatePoints\n");
      return 0.0;
    }
  
  int np1=SourceLandmarks->GetNumberOfPoints();
  int np2=TargetLandmarks->GetNumberOfPoints();

  if (np1!=np2)
    {
      vtkErrorMacro(<<"Bad point sets " << np1 << " vs " << np2 << " in input to ApproximatePoints\n");
      return 0.0;
    } 

  vtkpxMatrix* optweights=NULL;
  if (Weights!=NULL)
    {
      if (Weights->GetSize()[0]==np1)
	optweights=Weights;
    }

  int nparam=this->GetNumberOfParameters();

  vtkpxOptimizer* optimizer=vtkpxOptimizer::New();
  vtkDoubleArray* position=vtkDoubleArray::New();
  position->SetNumberOfTuples(nparam);
  vtkDoubleArray* temp=vtkDoubleArray::New();
  temp->SetNumberOfTuples(nparam);
  
  this->GetCurrentParameters(position);

  optimizer->SetMasterObject(this);
  optimizer->SetEvaluateFunctionMethod(&vtkpxPolySimilarityTransform::CalculateFunction);
  optimizer->SetEvaluateGradientMethod(&vtkpxPolySimilarityTransform::CalculateGradient);
  optimizer->SetFeedbackMethod(&vtkpxPolySimilarityTransform::OptimizerFeedback);
  
  double v=0.0;
  numsteps=Irange(numsteps,1,3);

  fprintf(stderr,"\n Approximating Transformation using %d points numsteps=%d step=%.3f tolerance=%.3f\n",np1,
	  numsteps,stepsize,tolerance);

  this->OptimizationStructure.SetLandmarksAndWeights(SourceLandmarks,TargetLandmarks,optweights);

  for (int step=1;step<=numsteps;step++)
    {
      this->OptimizationStructure.SetValues(nparam,stepsize,step);
      v=optimizer->ComputeConjugateGradient(position,20,tolerance);
      stepsize*=0.5;
      tolerance*=0.5;
    }

  //  temp->Delete();
  //  position->Delete();
  //  optimizer->Delete();
  
  return v;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void vtkpxPolySimilarityTransform::FixOrigins(double gap)
{
  int nt=this->GetNumberOfTransformations();
  if (nt<2)
    return;

  for (int i=1;i<nt;i++)
    {
      vtkpxArticulatedJointTransform* tr=this->GetTransform(i);
      tr->AdjustOriginToBeOutside(gap);
    }
}


void vtkpxPolySimilarityTransform::SetOriginAxes(vtkPoints* points)
{
  if (points==NULL)
    {
      vtkErrorMacro(<<"Bad Points (NULL)\n");
      return;
    }

  int nt=this->GetNumberOfTransformations();
  if (points->GetNumberOfPoints()<nt*3 || nt==0)
    {
      vtkErrorMacro(<<"Bad Points (Not Enough or not enough transformations)\n");
      return;
    }
  
  for (int i=0;i<nt;i++)
    {
      double ori[3],p1[3],p2[3];
      points->GetPoint(i*3,ori);
      points->GetPoint(i*3+1,p1);
      points->GetPoint(i*3+2,p2);
      vtkpxArticulatedJointTransform* tr=this->GetTransform(i);
      tr->SetOriginLocalAxis(ori,p1,p2);
    }
}

vtkPolyData* vtkpxPolySimilarityTransform::DrawAxis(float scale,float length)
{
  // Combination of
  // a sphere at origin and p1 and a line from -p1 to p2

  int nt=this->GetNumberOfTransformations();
  if (nt<1)
    return NULL;

  vtkPoints    *c_points    = vtkPoints::New();
  vtkPoints    *o_points    = vtkPoints::New();
  vtkCellArray *c_lines     = vtkCellArray::New();
  
  o_points->Allocate(nt);
  c_points->Allocate(nt*3);
  c_lines->Allocate(nt*2,5);
  
  vtkIdType index=0,pt[2];
  float x[3],x2[3],x3[3],n[3];

  for (int i=1;i<nt;i++)
    {
      vtkpxSimilarityTransform* tr=this->GetTransform(i)->GetSimilarityTransform();
      tr->GetOrigin(x);
      tr->GetRotationAxis(n);

      for (int ia=0;ia<=2;ia++)
	{
	  x2[ia]=x[ia]-n[ia]*length;
	  x3[ia]=x[ia]+n[ia]*length;
	}
      /*      vtkAbstractTransform* align=this->GetTransform(i)->GetAlignmentTransform();
      if (align!=NULL)
	{
	  align->TransformPoint(x,x);
	  align->TransformPoint(x2,x2);
	  align->TransformPoint(x3,x3);
	  }*/
      
      c_points->InsertNextPoint(x2);
      c_points->InsertNextPoint(x);
      c_points->InsertNextPoint(x3);

      o_points->InsertNextPoint(x);
      o_points->InsertNextPoint(x3);
      pt[0]=index;  pt[1]=index+1;
      c_lines->InsertNextCell(2,pt);
      pt[0]=index+1;  pt[1]=index+2;
      c_lines->InsertNextCell(2,pt);
      index+=3;
    }
  
  vtkPolyData* curve=vtkPolyData::New();
  curve->SetPoints(c_points);
  curve->SetLines(c_lines);
  c_points->Delete();
  c_lines->Delete();

  vtkPolyData* centers=vtkPolyData::New();
  centers->SetPoints(o_points);
  o_points->Delete();

  vtkTubeFilter* tube=vtkTubeFilter::New();
  tube->SetInput(curve);
  tube->SetRadius(scale*0.5);
  tube->SetNumberOfSides(8);
  tube->SetVaryRadiusToVaryRadiusOff();
  tube->Update();

  curve->Delete();

  vtkGlyph3D* glyph=vtkGlyph3D::New();
  vtkSphereSource* sphere=vtkSphereSource::New();
  sphere->SetRadius(scale);
  sphere->Update();
  glyph->SetSource(sphere->GetOutput());
  glyph->SetInput(centers);
  glyph->Update();

  centers->Delete();
  sphere->Delete();
  
  vtkAppendPolyData* append=vtkAppendPolyData::New();
  append->AddInput(glyph->GetOutput());
  append->AddInput(tube->GetOutput());
  append->Update();

  vtkDecimatePro* deciFilter=vtkDecimatePro::New();
  deciFilter->SetInput(append->GetOutput());
  deciFilter->SetTargetReduction(0.9);
  deciFilter->PreserveTopologyOn();
  deciFilter->Update();
  

  vtkPolyData* output=vtkPolyData::New();
  output->ShallowCopy(deciFilter->GetOutput());
  tube->Delete();
  glyph->Delete();
  deciFilter->Delete();
  return output;
}
// ----------------------------------------------------------------------
int vtkpxPolySimilarityTransform::InitializeRearLimbPresetRotations(vtkPoints* points)
{
  return this->InitializeRearLimbPresetRotations(points,NULL);
}
// ----------------------------------------------------------------------
int vtkpxPolySimilarityTransform::InitializeRearLimbPresetRotations(vtkPoints* orig_points,
								    vtkLinearTransform* lin_tr)
{
  if (orig_points==NULL)
    return 0;

  // Somehow use tr->Inverse to move the points back so that we can compute angles
  // -----------------------------------------------------------------------------


  int nt=this->TransformationCollection->GetNumberOfItems();
  int np=orig_points->GetNumberOfPoints();
  
  if (np< nt-1)
    {
      fprintf(stderr,"Not Enough Points\n");
      return 0;
    }

  vtkPoints* points=vtkPoints::New();
  points->DeepCopy(orig_points);

  if (lin_tr!=NULL)
    {
      for (int i=0;i<np;i++)
	{
	  double x[3]; points->GetPoint(i,x);
	  lin_tr->GetLinearInverse()->TransformPoint(x,x);
	  points->SetPoint(i,x);
	}
    }
	
  vtkTransform* concat=vtkTransform::New();
  
  for (int i=1;i<nt;i++)
    {
      vtkpxArticulatedJointTransform* tr=this->GetTransform(i);
      int ch=tr->GetChildRegionLabelIndex();
      if (ch >=0)
	{
	  vtkpxArticulatedJointTransform* trchild=this->GetTransform(ch);
	  float x1[3]; tr->GetOrigin(x1);
	  float x2[3]; trchild->GetOrigin(x2);
	  double x[3];

	  double p1[3]; points->GetPoint(i-1,p1);
	  double p2[3]; points->GetPoint(ch-1,p2);

	  // Back Shift p1 and p2 ......
	  concat->Identity();
	  concat->PostMultiply();
	  
	  vtkpxArticulatedJointTransform* current=tr;

	  int done=0,numt=0;
	  while (done==0)
	    {
	      vtkAbstractTransform* trparent0=current->GetAlignmentTransform();
	      done=1;

	      if (trparent0!=NULL)
		{
		  if (trparent0->IsA("vtkpxArticulatedJointTransform")==1)
		    {
		      vtkpxArticulatedJointTransform* joint=(vtkpxArticulatedJointTransform*)trparent0;

		      if (joint->GetSeparateZRotation()>0)
			concat->Concatenate(joint->GetZSimilarityTransform());
		      concat->Concatenate(joint->GetSimilarityTransform());
		      current=joint;
		      done=0;
		      ++numt;
		    }
		}
	    }

	  fprintf(stderr,"\n\n\n +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ \n");
	  fprintf(stderr,"Tr=%d ch=%d  origins=(%.2f,%.2f,%.2f),(%.2f,%.2f,%.2f) ----> (%.2f,%.2f,%.2f), (%.2f,%.2f,%.2f)\n",
		  i,ch,
		  x1[0],x1[1],x1[2],x2[0],x2[1],x2[2],
		  p1[0],p1[1],p1[2],p2[0],p2[1],p2[2]);
		

	  concat->Inverse();
	  concat->TransformPoint(p1,p1);
	  concat->TransformPoint(p2,p2);

	  fprintf(stderr,"\t\t Transform points numt=%d  ----> (%.2f,%.2f,%.2f), (%.2f,%.2f,%.2f)\n",
		  numt,p1[0],p1[1],p1[2],p2[0],p2[1],p2[2]);
		  
	  for (int ia=0;ia<=2;ia++)
	    {
	      p2[ia]=p2[ia]-p1[ia];
	      x[ia]=x2[ia]-x1[ia];
	    }

	  fprintf(stderr,"\t\t\t vectors (%.2f,%.2f,%.2f)  and (%.2f,%.2f,%.2f)\n\n\n\n",
		  x[0],x[1],x[2],p2[0],p2[1],p2[2]);

	  tr->InitializeAngles(x,p2);
	}
    }

  points->Delete();
  return 1;
}
      

vtkPoints* vtkpxPolySimilarityTransform::CreateTestPoints()
{
  vtkPoints* pt=vtkPoints::New();
  int nt=this->TransformationCollection->GetNumberOfItems();

  pt->SetNumberOfPoints(nt-1);
  for (int i=1;i<nt;i++)
    {
      vtkpxArticulatedJointTransform* tr=this->GetTransform(i);
      float x1[3]; tr->GetOrigin(x1);
      float x2[3];
      this->TransformPoint(x1,x2);
      fprintf(stderr,"x1=(%.2f,%.2f,%.2f) --> (%.2f,%.2f,%.2f)\n",
	      x1[0],x1[1],x1[2],
	      x2[0],x2[1],x2[2]);
      pt->SetPoint(i-1,x2[0],x2[1],x2[2]);
    }

  return pt;
}

void vtkpxPolySimilarityTransform::Identity()
{
  int np=this->TransformationCollection->GetNumberOfItems();
  for (int i=0;i<np;i++)
    this->GetTransform(i)->Identity();
}
// ----------------------------------------------------------------------
int vtkpxPolySimilarityTransform::UpdateThinPlateSplineTransform()
{
  return this->CreateThinPlateSplineTransform(-150);
}
// ----------------------------------------------------------------------
vtkPoints* vtkpxPolySimilarityTransform::CreateSamplePoints(int numberofpoints,float smooth,float ratio,float startsample)
{
  if (this->RegionLabel==NULL)
    {
      vtkErrorMacro(<<"No Region Label , Cannot create Sample Points\n");
      return NULL;
    }


  vtkPoints* sourcePoints=vtkPoints::New();


  if (numberofpoints<40)
    numberofpoints=40;
  else if (numberofpoints>10000)
    numberofpoints=10000;
  
  if (smooth<0.5)
    smooth=0.5;
  else if (smooth>4.0)
    smooth=4.0;
  
  if (ratio<1.1)
    ratio=1.1;
  else if (ratio>3.0)
    ratio=3.0;
  
  if (startsample<2.0)
    startsample=2.0;
  else if (startsample>10.0)
    startsample=10.0;
  
  fprintf(stderr,"SamplePoints :: Thresholding nc=%d smooth=%.2f ratio=%.2f startsample=%.2f\n",
	  numberofpoints,smooth,ratio,startsample);
  
  vtkImageThreshold* thr=vtkImageThreshold::New();
  thr->SetInput(this->RegionLabel);
  thr->ThresholdBetween(0.0,100);
  thr->SetInValue(100);
  thr->SetOutValue(0);
  thr->ReplaceInOn();
  thr->ReplaceOutOn();
      
  fprintf(stderr,"SamplePoints :: Smoothing\n");
  vtkImageGaussianSmooth* smoothFilter=vtkImageGaussianSmooth::New();
  smoothFilter->SetInput(thr->GetOutput());
  smoothFilter->SetStandardDeviations(smooth,smooth,smooth);
  smoothFilter->Update();
  thr->Delete();
      
      
  fprintf(stderr,"SamplePoints :: Extracting\n");
  vtkContourFilter* skinExtractor=vtkContourFilter::New();
  skinExtractor->SetInput(smoothFilter->GetOutput());
  skinExtractor->SetNumberOfContours(1);
  skinExtractor->SetValue(0,99.0);
  skinExtractor->ComputeScalarsOff();
  skinExtractor->ComputeGradientsOff();
  skinExtractor->ComputeNormalsOff();
  skinExtractor->Update();
  
  vtkPolyData* poly=vtkPolyData::New();
  poly->ShallowCopy(skinExtractor->GetOutput());
  skinExtractor->Delete();
  
  fprintf(stderr,"SamplePoints:: Resampling\n");
  
  
  double sampling=startsample*this->RegionLabel->GetSpacing()[0];
  
  vtkCleanPolyData* cleanerFilter=vtkCleanPolyData::New();
  cleanerFilter->SetInput(poly);
  cleanerFilter->SetToleranceIsAbsolute(1);
  
  int np1=poly->GetNumberOfPoints();
  fprintf(stderr,"SamplePoints:: Original Number of Points =%d sampling=%.2f\n",np1,sampling);
  
  int totalnp=np1,iter=1;
  
  while (totalnp>numberofpoints*1.1 && iter<20)
    {
      cleanerFilter->SetAbsoluteTolerance(sampling);
      cleanerFilter->Modified();
      cleanerFilter->Update();
      
      totalnp=cleanerFilter->GetOutput()->GetNumberOfPoints();
      fprintf(stderr,"\t\t\t SamplePoints: iter=%d \t Sampling %.2f \t  np=%d\n",iter,sampling,totalnp);
      sampling*=ratio;
      ++iter;
    }
  
  
  poly->ShallowCopy(cleanerFilter->GetOutput());
  
  int finalnp=poly->GetNumberOfPoints();
  fprintf(stderr,"\t\t\t SamplePoints: Final reduction = %d (%.2f)\n",finalnp,sampling);
  cleanerFilter->Delete();
  sourcePoints->DeepCopy(poly->GetPoints());
  poly->Delete();
  return sourcePoints;
}
// ----------------------------------------------------------------------
int vtkpxPolySimilarityTransform::CreateThinPlateSplineTransform(int numberofcontrolpoints,float smooth,float ratio,float startsample)
{
  if (this->RegionLabel==NULL)
    {
      vtkErrorMacro(<<"No Region Label , Cannot create TPS Transform\n");
      return 0;
    }

  int reinitialize=1;

  if (numberofcontrolpoints<0)
    {
      if (this->ThinPlateSplineTransform!=NULL)
	{
	  if (this->ThinPlateSplineTransform->GetSourceLandmarks()->GetNumberOfPoints()>5)
	    {
	      reinitialize=0;
	    }
	}

      if (reinitialize==1)
	numberofcontrolpoints=abs(numberofcontrolpoints);
    }

  vtkPoints* sourcePoints=NULL;


  if (reinitialize == 1 )
    {
      if (numberofcontrolpoints>400)
	numberofcontrolpoints=400;
      sourcePoints=CreateSamplePoints(numberofcontrolpoints,smooth,ratio,startsample);
    }
  else
    {
      sourcePoints=vtkPoints::New();
      sourcePoints->DeepCopy(this->ThinPlateSplineTransform->GetSourceLandmarks());
      fprintf(stderr,"Not Reinitializing, just recomputing displacements nc=%d\n",
	      sourcePoints->GetNumberOfPoints());

    }
  
  int totalnp=sourcePoints->GetNumberOfPoints();

  fprintf(stderr,"Transforming Points\n");
  vtkPoints* targetPoints=vtkPoints::New();
  targetPoints->DeepCopy(sourcePoints);
  for (int i=0;i<totalnp;i++)
    {
      double px[3],py[3];
      sourcePoints->GetPoint(i,px);
      
      this->TransformPoint(px,py);
      // This needs to be checked
      // int p=this->RegionLabel->FindPoint(px);
      targetPoints->SetPoint(i,py);
    }
  
  fprintf(stderr,"Creating TPS\n");

  if (this->ThinPlateSplineTransform==NULL)
    this->ThinPlateSplineTransform = vtkThinPlateSplineTransform::New();
  this->ThinPlateSplineTransform->SetSourceLandmarks(sourcePoints);
  this->ThinPlateSplineTransform->SetTargetLandmarks(targetPoints);
  this->ThinPlateSplineTransform->SetBasisToR();
  this->ThinPlateSplineTransform->Update();

  sourcePoints->Delete();
  targetPoints->Delete();

  return totalnp;
}

// ----------------------------------------------------------------------
void vtkpxPolySimilarityTransform::InitializeComboTransform(vtkpxComboTransform* combotransform,
						   int numpoints,int numsteps,double smoothness)
{
  if (combotransform->GetGridTransform()->GetNumberOfControlPoints()<50)
    {
      fprintf(stderr,"Bad Combo Transform, cannot initialize\n");
      return;
    }

  // First the linear part
  
  if (combotransform->GetNonLinearFirst()!=1)
    combotransform->SetNonLinearFirst(1);
  
  vtkpxSimilarityTransform* sim=vtkpxSimilarityTransform::New();
  sim->Copy(this->GetTransform(0)->GetSimilarityTransform());
  combotransform->GetLinearTransform()->Identity();
  combotransform->GetLinearTransform()->Concatenate(sim);


  // For now set global part to identity this is covered by the linear
  // part of vtkpxComboTransform
  this->GetTransform(0)->GetSimilarityTransform()->Identity();

  // Next Create a thin plate spline transformation ....
  vtkPoints* sourcePoints=this->CreateSamplePoints(numpoints);

  if (sourcePoints==NULL)
    {
      fprintf(stderr,"Bad Poly Similarity Transformation");
      return;
    }

  double sp[3]; this->GetRegionLabel()->GetSpacing(sp);
  double voxelsize=sp[0];

  // Now approximate points in multiple steps
  vtkPoints* tmpPoints=vtkPoints::New();

  int np=sourcePoints->GetNumberOfPoints();
  tmpPoints->SetNumberOfPoints(np);

  fprintf(stderr,"Number of Sampled Points=%d\n",np);

  vtkDataArray* disp=combotransform->GetGridTransform()->GetDisplacementGrid()->GetPointData()->GetScalars();
  for (int i=0;i<=2;i++)
    disp->FillComponent(i,0.0);

  if (numsteps<1)
    numsteps=1;
  else if (numsteps>50)
    numsteps=50;
    
  vtkCollection* collection=vtkCollection::New();
  combotransform->GetGridTransform()->FormCollectionStructure(sourcePoints,collection,0.01);
  

  

  for (int i=1;i<=numsteps;i++)
    {
      fprintf(stderr,"Approximation, step %d/%d\n",i,numsteps);
      double fraction=double(i)/double(numsteps);
      for (int j=0;j<np;j++)
	{
	  double x[3]; sourcePoints->GetPoint(j,x);
	  double y[3]; this->TransformPoint(x,y);
	  double t[3];
	  for (int ia=0;ia<=2;ia++)
	    t[ia]=fraction*(y[ia]-x[ia])+x[ia];
	  tmpPoints->SetPoint(j,t);
	}
      combotransform->GetGridTransform()->ApproximatePoints(sourcePoints,tmpPoints,collection,voxelsize*0.1,
							    20,1,voxelsize*0.2,smoothness,0);
    }
  collection->Delete();
  tmpPoints->Delete();

  // Restore polysimilarity transform to initial stage
  this->GetTransform(0)->GetSimilarityTransform()->Copy(sim);
  sim->Delete();
}


