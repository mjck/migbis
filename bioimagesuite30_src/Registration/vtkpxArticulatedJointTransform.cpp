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





#include <stdlib.h>
#include "vtkpxArticulatedJointTransform.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkpxUtil.h"
#include "vtkTransform.h"
#include "pxisinf.h"

//----------------------------------------------------------------------------
vtkpxArticulatedJointTransform* vtkpxArticulatedJointTransform::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxArticulatedJointTransform");
  if(ret)
    {
      return (vtkpxArticulatedJointTransform*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxArticulatedJointTransform;
}

//----------------------------------------------------------------------------
vtkpxArticulatedJointTransform::vtkpxArticulatedJointTransform()
{
  this->DetailMode=0;
  this->BlendedMode=0;
  this->CubicBlending=0;
  this->RegionLabelIndex=-1;
  this->ChildRegionLabelIndex=-1000;
  this->ChildRegionLabelIndex2=-1000;
  this->MinimumSectorAngle=5.0;
  this->BlendingWindow=1.0;
  this->SmoothingRadius=1.0;
  this->SmoothingMode=0;
  this->RegionLabelImage=NULL;
  this->SimilarityTransform=vtkpxSimilarityTransform::New();
  this->ZSimilarityTransform=vtkpxSimilarityTransform::New();
  this->SetModeToRotationOnly();
  this->AngleAxisTransform=vtkTransform::New();
  this->AlignmentTransform=NULL;
  this->AlignmentTransformIndex=-1;
  this->SeparateZRotation=0;
  this->ZBlendingSize=5.0;
  this->JointName=NULL; this->SetJointName("Joint");
  this->Active=1;
}

//----------------------------------------------------------------------------
vtkpxArticulatedJointTransform::~vtkpxArticulatedJointTransform()
{
  if (this->RegionLabelImage!=NULL)
    this->SetRegionLabelImage(NULL);

  if (this->AlignmentTransform!=NULL)
    this->SetAlignmentTransform(NULL);


  this->AngleAxisTransform->Delete();
  this->SimilarityTransform->Delete();
  this->ZSimilarityTransform->Delete();

  if (this->JointName!=NULL)
    delete [] JointName;

}


void vtkpxArticulatedJointTransform::Identity()
{
  this->SimilarityTransform->Identity();
  this->ZSimilarityTransform->Identity();
}
//----------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::ExternalSetRegionLabelImage(vtkImageData* img)
{
  this->SetRegionLabelImage(img);
  if (this->RegionLabelImage!=NULL)
    {
      float mean=0.0;
      for (int kk=0;kk<=2;kk++)
	mean+=img->GetSpacing()[kk]/3.0;
      this->SimilarityTransform->SetTranslationScale(mean);
      this->ZSimilarityTransform->SetTranslationScale(mean);
    }
  else
    {
      this->SimilarityTransform->SetTranslationScale(1.0);
      this->ZSimilarityTransform->SetTranslationScale(1.0);
    }
}
//----------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "### " << this->GetClassName() << " ### \n";
  os << indent << "Joint Name : " << this->JointName << "\n";
  os << indent << "SeparateZRotation: " << this->SeparateZRotation << "\n";
  os << indent << "BlendedMode: " << this->BlendedMode << "\n";
  os << indent << "BlendingWindow: " << this->BlendingWindow << "\n";
  os << indent << "ZBlendingSize: " << this->ZBlendingSize << "\n";
  os << indent << "SmoothingMode: " << this->SmoothingMode << "\n";
  os << indent << "SmoothingRadius: " << this->SmoothingRadius << "\n";
  os << indent << "MinimumSectorAngle: " << this->MinimumSectorAngle << "\n";
  os << indent << "CubicBlending: " << this->CubicBlending << "\n";
  os << indent << "RegionLabelImage:      " << this->RegionLabelImage << "\n";
  os << indent << "RegionLabelIndex:      " << this->RegionLabelIndex << "\n";
  os << indent << "ChildRegionLabelIndex:      " << this->ChildRegionLabelIndex << "\n";
  os << indent << "ChildRegionLabelIndex2:      " << this->ChildRegionLabelIndex2 << "\n";
  os << indent << "SimilarityTransform: " << this->SimilarityTransform << "\n";
  os << indent << "ZSimilarityTransform: " << this->ZSimilarityTransform << "\n";
  os << indent << "AlignmentTransformIndex:" << this->AlignmentTransformIndex << "\n";
  os << indent << "AlignmentTransform:" << this->AlignmentTransform << "\n";
}

vtkAbstractTransform *vtkpxArticulatedJointTransform::MakeTransform()
{
  return vtkpxArticulatedJointTransform::New(); 
}

//----------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::SetMode(int m)
{
  int newm=1;
  if (m>=-1 && m<=5)
    newm=m;
  
  if (m==1)
    {
      this->SeparateZRotation=1;
      this->SimilarityTransform->SetMode(0);
      this->ZSimilarityTransform->SetMode(9);
    }
  else
    {
      this->SeparateZRotation=0;
      this->SimilarityTransform->SetMode(m);
      this->ZSimilarityTransform->SetMode(-1);
    }
  if (m!=0 && m!=1 && m!=5)
    this->SetBlendedMode(0);

}
//----------------------------------------------------------------------------  
void vtkpxArticulatedJointTransform::SetActive(int m)
{
  m=(m>0);
  if (this->Active==m)
    return;

  if (this->Active==1)
    {
      this->OldSimilarityMode=this->SimilarityTransform->GetMode();
      this->OldZSimilarityMode=this->SimilarityTransform->GetMode();
      this->OldBlendedMode=this->BlendedMode;
      this->OldSeparateZRotation=this->SeparateZRotation;

      this->SetModeToFixedParameters();
      this->Active=0;
    }
  else
    {
      this->SeparateZRotation=this->OldSeparateZRotation;
      this->SimilarityTransform->SetMode(this->OldSimilarityMode);
      this->ZSimilarityTransform->SetMode(this->OldZSimilarityMode);
      this->BlendedMode=this->OldBlendedMode;
      this->Active=1;
    }
}
//----------------------------------------------------------------------------  
int vtkpxArticulatedJointTransform::Load(const char* fname)
{
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  int ok=this->Load(fin);
  gzclose(fin);
  return ok;
}

int vtkpxArticulatedJointTransform::Load(gzFile fin)
{
  char pxtk_buffer[101];
  gzgets(fin,pxtk_buffer,100);

  int mode=1;
  int tmp; float ftmp;

  if (gzfindstringinstring(pxtk_buffer,"#vtkpxArticulatedJointTransform3")==0)
    {
      return 0;
    }

  gzgets(fin,pxtk_buffer,101); 
  int a=strlen(pxtk_buffer);  pxtk_buffer[a-1]=(char)0;
  this->SetJointName(pxtk_buffer);


  this->SimilarityTransform->Load(fin);
  this->ZSimilarityTransform->Load(fin);
  gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100); sscanf(pxtk_buffer,"%d",&tmp);   this->SetSeparateZRotation(tmp);
  gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100); sscanf(pxtk_buffer,"%f",&ftmp);  this->SetZBlendingSize(ftmp);

  gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100); sscanf(pxtk_buffer,"%d",&tmp);   this->SetBlendedMode(tmp);
  gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100); sscanf(pxtk_buffer,"%f",&ftmp);  this->SetBlendingWindow(ftmp);
  gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100); sscanf(pxtk_buffer,"%d",&tmp);   this->SetSmoothingMode(tmp);
  gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100); sscanf(pxtk_buffer,"%f",&ftmp);  this->SetSmoothingRadius(ftmp);
  gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100); sscanf(pxtk_buffer,"%f",&ftmp);  this->SetMinimumSectorAngle(ftmp);
  gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100); sscanf(pxtk_buffer,"%d",&tmp);   this->SetCubicBlending(tmp);
  gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100); sscanf(pxtk_buffer,"%d",&tmp);   this->SetRegionLabelIndex(tmp);
  gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100); sscanf(pxtk_buffer,"%d",&tmp);   this->SetChildRegionLabelIndex(tmp);
  gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100); sscanf(pxtk_buffer,"%d",&tmp);   this->SetChildRegionLabelIndex2(tmp);
  gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100); sscanf(pxtk_buffer,"%d",&tmp);   this->SetAlignmentTransformIndex(tmp);

  this->Active=1;

  return 1;
}
//----------------------------------------------------------------------------
int vtkpxArticulatedJointTransform::Save(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
    return 0;
  
  int ok=this->Save(fout);
  fclose(fout);
  return ok;
}

int vtkpxArticulatedJointTransform::Save(FILE* fout)
{
  fprintf(fout,"#vtkpxArticulatedJointTransform3\n");
  fprintf(fout,"%s\n",this->JointName);
  this->SimilarityTransform->Save(fout);
  this->ZSimilarityTransform->Save(fout);
  fprintf(fout,"#SeparateZRotation\n%d\n",this->SeparateZRotation);
  fprintf(fout,"#ZBlendingSize\n%.3f\n",this->ZBlendingSize);
  fprintf(fout,"#BlendedMode\n%d\n",this->BlendedMode);
  fprintf(fout,"#BlendingWindow\n%.2f\n",this->BlendingWindow);
  fprintf(fout,"#SmoothingMode\n%d\n",this->SmoothingMode);
  fprintf(fout,"#SmoothingRadius\n%.2f\n",this->SmoothingRadius);
  fprintf(fout,"#MiniumSectorAngle\n%.2f\n",this->MinimumSectorAngle);
  fprintf(fout,"#CubicBlending\n%d\n",this->CubicBlending);
  fprintf(fout,"#RegionLabelIndex\n%d\n",this->RegionLabelIndex);
  fprintf(fout,"#ChildRegionLabelIndex\n%d\n",this->ChildRegionLabelIndex);
  fprintf(fout,"#ChildRegionLabelIndex2\n%d\n",this->ChildRegionLabelIndex2);
  fprintf(fout,"#AlignmentTransformIndex\n%d\n",this->AlignmentTransformIndex);
  return 1;
}
//----------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::Copy(vtkpxArticulatedJointTransform* other)
{
  if (other==NULL)
    return;
  
  this->SetJointName(other->GetJointName());
  this->SimilarityTransform->Copy(other->GetSimilarityTransform());
  this->ZSimilarityTransform->Copy(other->GetZSimilarityTransform());
  this->SetSeparateZRotation(other->GetSeparateZRotation());
  this->SetBlendedMode(other->GetBlendedMode());
  this->SetBlendingWindow(other->GetBlendingWindow());
  this->SetSmoothingMode(other->GetSmoothingMode());
  this->SetSmoothingRadius(other->GetSmoothingRadius());
  this->SetMinimumSectorAngle(other->GetMinimumSectorAngle());
  this->SetCubicBlending(other->GetCubicBlending());
  this->SetRegionLabelIndex(other->GetRegionLabelIndex());
  this->SetChildRegionLabelIndex(other->GetChildRegionLabelIndex());
  this->SetChildRegionLabelIndex2(other->GetChildRegionLabelIndex2());
  this->SetMode(other->GetMode());
  this->SetRegionLabelImage(other->GetRegionLabelImage());
  this->SetAlignmentTransform(other->GetAlignmentTransform());
  this->SetAlignmentTransformIndex(other->GetAlignmentTransformIndex());
}
//----------------------------------------------------------------------------
int vtkpxArticulatedJointTransform::IsInsideRegion(const double x[3])
{
  if (this->RegionLabelImage==NULL)
    return -1;

  int p=this->RegionLabelImage->FindPoint(x[0],x[1],x[2]);
  if (p==-1)
    return -1;
  
  int l=(int)this->RegionLabelImage->GetPointData()->GetScalars()->GetComponent(p,0);
  if (l==-1)
    return -1;

  int md=0;
  if (l==this->RegionLabelIndex)
    md=1;
  else if (l>=this->ChildRegionLabelIndex && l <= this->ChildRegionLabelIndex2 && this->ChildRegionLabelIndex<=this->ChildRegionLabelIndex2)
    md=1;
  

  if (this->DetailMode)
    fprintf(stderr,"p=%.1f,%.1f,%.1f l=%d mode=%d\n",x[0],x[1],x[2],l,md);

  return md;
}
//----------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::CreateAngleAxisTransformation(double theta,vtkpxSimilarityTransform* trans)
{
  
  float* origin=trans->GetOrigin();
  float*   axis=trans->GetRotationAxis();

  this->AngleAxisTransform->Identity();
  this->AngleAxisTransform->PostMultiply();
  this->AngleAxisTransform->Translate(-origin[0],-origin[1],-origin[2]);
  if (isnan(theta))
    {
      fprintf(stderr,"Bad Theta *");
      theta=0.0;
    }
  float magn=axis[0]*axis[0]+axis[1]*axis[1]+axis[2]*axis[2];
  if (magn<0.1)
    {
      fprintf(stderr,"* Bad Axis %.2f %.2f %.2f\n",axis[0],axis[1],axis[2]);
      axis[0]=1.0;
      axis[1]=0.0;
      axis[2]=0.0;
      theta=0.0;
    }

  this->AngleAxisTransform->RotateWXYZ(theta,axis[0],axis[1],axis[2]);
  this->AngleAxisTransform->Translate(origin);
}
// -----------------------------------------------------------------------------------
double vtkpxArticulatedJointTransform::AdjustForZBlending(const double point[3],vtkpxSimilarityTransform* trans,float max)
{
  float* origin=trans->GetOrigin();
  float*   axis=trans->GetRotationAxis();

  double sum=0.0;
  for (int ia=0;ia<=2;ia++)
    sum+=(point[ia]-origin[ia])*axis[ia];

  double r=fabs(sum/max);
  if (r>1.0)
    r=1.0;

  if (this->CubicBlending==0)
    return r;
  
  return 3.0*pow(r,2.0)-2.0*pow(r,3.0);
}

// -----------------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::InternalTransformPoint(const double point[3], 
							    double output[3])
{
  if (this->BlendedMode==0)
    {
      if (this->SeparateZRotation)
	{
	  this->ZSimilarityTransform->TransformPoint(point,output);
	  this->SimilarityTransform->TransformPoint(output,output);
	}
      else
	{
	  this->SimilarityTransform->TransformPoint(point,output);
	}
      if (this->AlignmentTransform!=NULL)
	this->AlignmentTransform->TransformPoint(output,output);
      if (this->DetailMode)
	fprintf(stderr,"Bl 0 ");

      return;
    }

  int mode=this->IsInsideRegion(point);

  if (mode==-1)
    {
      if (this->SeparateZRotation)
	{
	  this->ZSimilarityTransform->TransformPoint(point,output);
	  this->SimilarityTransform->TransformPoint(output,output);
	}
      else
	{
	  this->SimilarityTransform->TransformPoint(point,output);
	}
      if (this->AlignmentTransform!=NULL)
	this->AlignmentTransform->TransformPoint(output,output);
      if (this->DetailMode)
	fprintf(stderr,"Bl -1");
      return;
    }
  else if (mode==0)
    {
      for (int ia=0;ia<=2;ia++)
	output[ia]=point[ia];
      if (this->AlignmentTransform!=NULL)
	this->AlignmentTransform->TransformPoint(output,output);
      if (this->DetailMode)
	fprintf(stderr,"Bl 0");
      return;
    }

  // Fun part begins here

  // First Apply Z-Transform
  double newpoint[3];
  if (this->SeparateZRotation)
    {
      double phi=this->GetSmoothRotationAngle(point,this->ZSimilarityTransform);
      // Scale phi somewhat
      phi=phi*this->AdjustForZBlending(point,this->ZSimilarityTransform,this->ZBlendingSize*this->ZSimilarityTransform->GetTranslationScale());


      this->CreateAngleAxisTransformation(phi,this->ZSimilarityTransform);
      this->AngleAxisTransform->TransformPoint(point,newpoint);
      if (this->DetailMode)
	fprintf(stderr,"bl phi* =%.2f",phi);


    }
  else
    {
      for (int i=0;i<=2;i++)
	newpoint[i]=point[i];
    }

  double theta=this->GetSmoothRotationAngle(newpoint,this->SimilarityTransform);
  this->CreateAngleAxisTransformation(theta,this->SimilarityTransform);
  this->AngleAxisTransform->TransformPoint(newpoint,output);
  if (this->DetailMode)
    fprintf(stderr,", theta=%.2f\n",theta);
     
  if (this->AlignmentTransform!=NULL)
    this->AlignmentTransform->TransformPoint(output,output);
}
//------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::InternalTransformDerivative(const double point[3],
								 double output[3],
								 double derivative[3][3])
{
  for (int ia=0;ia<=2;ia++)
    for (int ja=0;ja<=2;ja++)
      derivative[ia][ja]=double(ia==ja);
	
  this->InternalTransformPoint(point,output);
  return;
}
 // -----------------------------------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::InternalTransformPoint(const float point[3],float output[3])
{
  double p1[3],p2[3]; 
  for (int i=0;i<=2;i++) 
    p1[i]=point[i];
  this->InternalTransformPoint(p1,p2);
  for (int i=0;i<=2;i++) 
    output[i]=p2[i];
}
// -----------------------------------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::InternalTransformDerivative(const float point[3],
								 float output[3],
								 float derivative[3][3])
{
  double p1[3],p2[3],der[3][3];
  for (int i=0;i<=2;i++) 
    p1[i]=point[i];

  this->InternalTransformDerivative(p1,p2,der);
  for (int i=0;i<=2;i++) 
    {
      output[i]=p2[i];
      for (int j=0;j<=2;j++)
	derivative[i][j]=der[i][j];
    }
}
//----------------------------------------------------------------------------
double vtkpxArticulatedJointTransform::SearchForInterface(const double x[3],double maxtheta,double stepsize,vtkpxSimilarityTransform* trans)
{
  double tx[3];
  double dtheta=stepsize;
  double theta=0.0;
  int found=0;
  
  while (found==0 && fabs(theta)<fabs(maxtheta))
    {
      theta+=dtheta;
      this->CreateAngleAxisTransformation(theta,trans);
      this->AngleAxisTransform->TransformPoint(x,tx);
      if (this->DetailMode==1)
	fprintf(stderr," *** %.2f ",theta);
      if (this->IsInsideRegion(tx)==0)
	found=1;
    }
  return theta;

}
//----------------------------------------------------------------------------
//
//   Implement Causal Blending, Assume Convex Interface
//
//----------------------------------------------------------------------------
double vtkpxArticulatedJointTransform::GetActualRotationAngle(const double x[3],vtkpxSimilarityTransform* trans)
{

  // Step 1:
  // Check if we need to blend at all by rotating by (2*theta), 

  double tx[3];
  
  double RotationAngle=trans->GetRotationAngle();

  if (RotationAngle==0.0)
    return 0.0;

  double threshold[2] = { 1.0+this->BlendingWindow,this->BlendingWindow };

  int    dobisection[2]={ 1, 1 };
  double upperangle[2]= { threshold[0]*RotationAngle, -threshold[1]*RotationAngle };
  
  int maxside=this->BlendedMode; // i.e. 1 or 2
  
  if (this->DetailMode)
    fprintf(stderr,"\n\nInit x=(%.1f,%.1f,%.1f) thr=(%.1f,%.1f) dobis=%d,%d, upperangle=%.2f %.2f (Angle=%.2f)\n",
	    x[0],x[1],x[2],
	    threshold[0],threshold[1],
	    dobisection[0],dobisection[1],
	    upperangle[0],upperangle[1],
	    RotationAngle);

  for (int side=0;side<maxside;side++)
    {
      if (this->MinimumSectorAngle>fabs(upperangle[side]))
	{
	  if (this->DetailMode)
	    fprintf(stderr,"Fast Search: side=%d upper=%.2f\n",side,upperangle[side]);

	  this->CreateAngleAxisTransformation(upperangle[side],trans);
	  this->AngleAxisTransform->TransformPoint(x,tx);

	  if (this->IsInsideRegion(tx)!=0)
	    dobisection[side]=0;
	}
      else
	{
	  double step=this->MinimumSectorAngle;
	  if (upperangle[side]<0.0)
	    {
	      step=-this->MinimumSectorAngle;
	      if (step>-1.0)
		step=-1.0;
	    }
	  else
	    {
	      if (step<1.0)
		step=1.0;
	    }
	  if (this->DetailMode)
	    fprintf(stderr,"Slow Search side=%d upper=%.2f step=%.2f\n",side,upperangle[side],step);

	  double th=this->SearchForInterface(x,upperangle[side],step,trans);
	  if (fabs(th)>fabs(upperangle[side]))
	    dobisection[side]=0;
	  else
	    upperangle[side]=th;
	}
    }

  if (this->DetailMode)
    fprintf(stderr," dobis=%d,%d, upperangle=%.2f %.2f (Angle=%.2f)\n",
	    dobisection[0],dobisection[1],
	    upperangle[0],upperangle[1],
	    RotationAngle);


  if (( dobisection[0]+dobisection[1] == 0 ) ||
      ( dobisection[0] == 0 && this->BlendedMode !=2 ))
      return RotationAngle;

  double ratio[2] = { threshold[0]+0.01,threshold[1]+0.01};

  for (int side=0;side<maxside;side++)
    {
      if (dobisection[side])
	{
	  // Do Bisection to establish angle of change 
	  float outsidetheta=upperangle[side];
	  float insidetheta=0.0;
	  
	  float thr=fabs(0.01);// *RotationAngle);
	  float theta=0.5*(outsidetheta+insidetheta);
	  
	  while (fabs(outsidetheta-insidetheta)>thr)
	    {
	      this->CreateAngleAxisTransformation(theta,trans);
	      this->AngleAxisTransform->TransformPoint(x,tx);
	      if (this->DetailMode)
		fprintf(stderr,"Bisection Side=%d, in=%.2f out=%.2f theta=%.2f \t",side,insidetheta,outsidetheta,theta);
	      
	      int isinside=this->IsInsideRegion(tx);
	      
	      if (isinside!=0)
		insidetheta=theta;
	      else
		outsidetheta=theta;
	      
	      theta=0.5*(outsidetheta+insidetheta);
	      
	    }
	  ratio[side]=fabs(theta/RotationAngle);
	  if (this->DetailMode)
	    fprintf(stderr,"Bisection Side=%d, theta=%.2f ratio=%.4f\n\n",side,theta,ratio[side]);
	}
    }

  // Three cases
  // Case 1 -- folding issue, no smoothing
  double newangle=RotationAngle;
  double sumratio=fabs(ratio[1])+fabs(ratio[0]);

  if (this->DetailMode)
    fprintf(stderr,"Blending, ratios=%.2f,%.2f  thresholds=%.2f,%.2f sumratio=%.2f\n",
	    ratio[0],ratio[1],threshold[0],threshold[1],sumratio);

  if  (ratio[0]<threshold[0] && ratio[1]>threshold[1])
    {
      // Fold No Blend
      double x=fabs(ratio[0]);
      double w=fabs(threshold[0]);

      if (CubicBlending)
	{
	  newangle=(x-(2.0*pow(x,3.0)/pow(w,3.0)-3.0*pow(x,2.0)/pow(w,2.0)+x))*RotationAngle;
	}
      else
	{
	  newangle=fabs(ratio[0]/threshold[0])*RotationAngle;
	}
      if (this->DetailMode)
	fprintf(stderr,"Fold cubic=%d, newangle=%.2f\n",this->CubicBlending,newangle);
    }
  else if (ratio[1] < threshold[1] && ratio[0]>threshold[0])
    {
      // Blend No Fold
      double x=fabs(ratio[1]);
      double w=fabs(threshold[1]);
      if (CubicBlending)
	{
	  newangle=(x-(2.0*pow(x,3.0)/pow(w,3.0)-3.0*pow(x,2.0)/pow(w,2.0)+x))*RotationAngle;
	}
      else
	{
	  newangle=(ratio[1]/threshold[1])*RotationAngle;
	}
      if (this->DetailMode)
	fprintf(stderr,"Smooth cubic=%d, newangle=%.2f\n",this->CubicBlending,newangle);
	
    }
  else
    {
      double midpoint=(threshold[1]*sumratio)/(threshold[0]+threshold[1]);
      int double0=-1;

      if (CubicBlending)
	{
	  newangle=fabs(ratio[0]/threshold[0])*RotationAngle;
	}
      else 
	{
	  if (midpoint<ratio[1])
	    {
	      newangle=fabs(ratio[0]/threshold[0])*RotationAngle;
	      double0=0;
	    }
	  else
	    {
	      newangle=(ratio[1]/threshold[1])*RotationAngle;
	      double0=1;
	    }
	}

      if (this->DetailMode)
	fprintf(stderr,"Double Blend  midpoint=%.2f double0=%d newangle=%.2f\n",
		midpoint,double0,newangle);
  
    }


  return newangle;

}

// ----------------------------------------------------------------------------------------------
vtkImageData* vtkpxArticulatedJointTransform::ComputeAllAngles()
{
  if (this->RegionLabelImage==NULL)
    return NULL;

  vtkImageData* output=vtkImageData::New();
  output->CopyStructure(this->RegionLabelImage);
  output->SetScalarTypeToFloat();
  output->SetNumberOfScalarComponents(1);
  output->AllocateScalars();

  vtkDataArray* out=output->GetPointData()->GetScalars();
  out->FillComponent(0,0.0);
  int np=out->GetNumberOfTuples();


  for (int i=0;i<np;i++)
    {
      double x[3]; output->GetPoint(i,x);
      
      if (this->IsInsideRegion(x)==1)
	{
	  double theta=this->GetSmoothRotationAngle(x,this->SimilarityTransform);
	  out->SetComponent(i,0,theta);
	}
    }

  return output;
}
// ----------------------------------------------------------------------------------------------      
double vtkpxArticulatedJointTransform::GetSmoothRotationAngle(const double x[3],vtkpxSimilarityTransform* trans)
{
  double RotationAngle=trans->GetRotationAngle();
  //  if (this->BlendedMode==0)
  //return RotationAngle;

  double theta=this->GetActualRotationAngle(x,trans);
  if (!this->SmoothingMode)
    return theta;

  float origin[3]; trans->GetOrigin(origin);
  float axis[3];   trans->GetRotationAxis(axis);
  double r[3],sum=0.0;
  for (int i=0;i<=2;i++)
    {
      r[i]=x[i]-origin[i];
      sum+=r[i]*axis[i];
    }

  if (this->DetailMode) fprintf(stderr,"axis=%.2f,%.2f,%.2f\n",axis[0],axis[1],axis[2]);
  if (this->DetailMode) fprintf(stderr,"r   =%.2f,%.2f,%.2f\n",r[0],r[1],r[2]);

  
  double magn=0.0;
  for (int i=0;i<=2;i++)
    {
      r[i]-=sum*axis[i];
      magn+=r[i]*r[i];
    }

  if (this->DetailMode) fprintf(stderr,"r   =%.2f,%.2f,%.2f\n",r[0],r[1],r[2]);

  for (int i=0;i<=2;i++)
    r[i]/=sqrt(magn);
  
  if (this->DetailMode) fprintf(stderr,"r   =%.2f,%.2f,%.2f\n",r[0],r[1],r[2]);

  double thetasum=0.0;
  for (int i=0;i<4;i++)
    {
      double newx[3] = { x[0],x[1],x[2] };
      switch(i)
	{
	case 0:
	  for (int j=0;j<=2;j++)
	    newx[j]=x[j]-this->SmoothingRadius*axis[j];
	  break;
	case 1:
	  for (int j=0;j<=2;j++)
	    newx[j]=x[j]+this->SmoothingRadius*axis[j];
	  break;
	case 3:
	  for (int j=0;j<=2;j++)
	    newx[j]=x[j]-this->SmoothingRadius*r[j];
	  break;
	case 4:
	  for (int j=0;j<=2;j++)
	    newx[j]=x[j]+this->SmoothingRadius*r[j];
	  break;
	}
      thetasum+=0.25*this->GetActualRotationAngle(newx,trans);
    }
      
  return 0.5*(theta+thetasum);
}
// ----------------------------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::AdjustOriginToBeOutside(double gap)
{
  
  float origin[3]; this->SimilarityTransform->GetOrigin(origin);
  float zaxis[3];  this->SimilarityTransform->GetLocalZAxis(zaxis);
  double px[3];

  // do bisection assume we are < 20.0 from boundary and go
  // ------------------------------------------------------


  int dim[3]; this->RegionLabelImage->GetDimensions(dim);
  double sp[3]; this->RegionLabelImage->GetSpacing(sp);
  double ori[3]; this->RegionLabelImage->GetOrigin(ori);
  // fprintf(stderr,"Dimensions =%dx%dx%d spacing=%.2fx%.2fx%.2f\n",dim[0],dim[1],dim[2],sp[0],sp[1],sp[2]);


  for (int k=0;k<=2;k++)
    px[k]=origin[k];

  int p=this->RegionLabelImage->FindPoint(px[0],px[1],px[2]);
  int pv=(int)this->RegionLabelImage->GetPointData()->GetScalars()->GetComponent(p,0);

  this->RegionLabelImage->GetPoint(p,px);
  for (int i=0;i<=2;i++)
    px[i]=(px[i]-ori[i])/sp[i];

  int newp=int(px[2]*(dim[0]*dim[1])+px[1]*dim[0]+px[0]);
  int p2=(int)this->RegionLabelImage->GetScalarComponentAsDouble(int(0.5+px[0]),int(0.5+px[1]),int(0.5+px[2]),0);

  /*  fprintf(stderr,"Testing %.2f %.2f %.2f (%d,%d,%d)  p=%d(%d)  newp=%d p2=%d    (ind=%d, children=%d,%d)\n",
	  origin[0],origin[1],origin[2],
	  int(0.5+px[0]),int(0.5+px[1]),int(0.5+px[2]),
	  p,pv,
	  newp,p2,
	  this->RegionLabelIndex,
	  this->ChildRegionLabelIndex,this->ChildRegionLabelIndex2);*/


  //  this->SetDetailMode(1);

  for (int k=0;k<=2;k++)
    px[k]=origin[k];
  
  int isinside=this->IsInsideRegion(px);

  //  fprintf(stderr,"Ininside (%.2f,%.2f,%.2f) = %d\n",px[0],px[1],px[2],isinside);


  float inside =30.0;
  float outside=-30.0;

  float thr=0.2;
  float location=0.5*(outside+inside);
	  
  while (fabs(outside-inside)>thr)
    {
      fprintf(stderr,"Bisection %.2f : %.2f  testing %.2f ",inside,outside,location);
      for (int i=0;i<=2;i++)
	px[i]=origin[i]+location*zaxis[i];
      int isinside=this->IsInsideRegion(px);

      int p=this->RegionLabelImage->FindPoint(px[0],px[1],px[2]);
      int pv=(int)this->RegionLabelImage->GetPointData()->GetScalars()->GetComponent(p,0);

      fprintf(stderr,"px=(%.2f %.2f %.2f) isinside=%d (p=%d pv=%d)\n",px[0],px[1],px[2],isinside,p,pv);
	      
      if (isinside!=0)
	inside=location;
      else
	outside=location;
      
      location=0.5*(outside+inside);
	      
    }

  for (int i=0;i<=2;i++)
    px[i]=origin[i]+location*zaxis[i];

  /*  fprintf(stderr,"Distance of (%.2f,%.2f,%.2f) from boundary = %.2f (%.2f,%.2f,%.2f) \t new origin (%.2f) = ",
      origin[0],origin[1],origin[2],location,px[0],px[1],px[2],gap);*/

  // New z-axis
  this->SetFreezeUpdates(1);
  
  for (int i=0;i<=2;i++)
    {
      origin[i]=px[i]-fabs(gap)*zaxis[i];
      if (i==2)
	this->SetFreezeUpdates(0);
      this->Put(3+i,origin[i]);
    }
  this->SimilarityTransform->GetOrigin(origin);
  fprintf(stderr,"%.2f %.2f %.2f\n",origin[0],origin[1],origin[2]);
    
}

// ----------------------------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::SetOriginLocalAxis(double ori[3],double p1[3],double p2[3])
{
  fprintf(stderr,"\n------------------------------------------------------\norigin=%.2f,%.2f,%.2f\n",ori[0],ori[1],ori[2]);
  fprintf(stderr,"p1=%.2f,%.2f,%.2f\n",p1[0],p1[1],p1[2]);
  fprintf(stderr,"p2=%.2f,%.2f,%.2f\n",p2[0],p2[1],p2[2]);

  for (int j=0;j<=2;j++)
    {
      p1[j]=p1[j]-ori[j];
      p2[j]=p2[j]-ori[j];
    }
  
  vtkMath::Normalize(p1);
  vtkMath::Normalize(p2);

  double a=vtkMath::Dot(p1,p2);
  for (int ia=0;ia<=2;ia++)
    p2[ia]=p2[ia]-a*p1[ia];
  
  double p3[3];
  vtkMath::Cross(p1,p2,p3);


  fprintf(stderr,"p1=%.2f,%.2f,%.2f\n",p1[0],p1[1],p1[2]);
  fprintf(stderr,"p2=%.2f,%.2f,%.2f\n",p2[0],p2[1],p2[2]);
  fprintf(stderr,"p3=%.2f,%.2f,%.2f\n",p3[0],p3[1],p3[2]);


  int isorigininside=this->IsInsideRegion(ori);
  fprintf(stderr,"Origin isinside=%d\n",isorigininside);

  if (this->BlendedMode!=0)
    {
      // Check which way to go ....
      
      double inside=-5.0,outside=5.0;
      
      double tp[3];
      double test=inside;
      
      for (int ia=0;ia<=2;ia++)
	tp[ia]=ori[ia]+test*p3[ia];
      
      
      int isinside=this->IsInsideRegion(tp);

      if (!isinside)
	{
	  outside=-5.0;
	  inside=5.0;
	}

      fprintf(stderr,"ins=%.2f outs=%.2f tp=%.2f %.2f %.2f test=%.2f insid=%d\n",
	      inside,outside,0.5*tp[0],0.5*tp[1],0.5*tp[2],test,isinside);

      test=0.0;
      while ( fabs(outside-inside)>0.1)
	{
	  for (int ia=0;ia<=2;ia++)
	    tp[ia]=ori[ia]+test*p3[ia];

	  isinside=this->IsInsideRegion(tp);
	  fprintf(stderr,"ins=%.2f outs=%.2f tp=%.2f %.2f %.2f test=%.2f insid=%d\n",
		  inside,outside,0.5*tp[0],0.5*tp[1],0.5*tp[2],test,isinside);
	  
	  if (!isinside)
	    outside=test;
	  else
	    inside=test;
	  test=0.5*(outside+inside);

	}
      
      double shift=0.0;
      if (isorigininside)
	{
	  shift=fabs(test)+3.0;
	  if (test<0.0)
	    shift=-shift;
	}
      else
	{
	  if (fabs(test)<3.0)
	    {
	      shift=-(3.0-fabs(test));
	      if (test<0.0)
		shift=-shift;
	    }
	}

      fprintf(stderr,"\t Test=%.2f shift=%.2f \n",test,shift);
      
      for (int ia=0;ia<=2;ia++)
	ori[ia]=ori[ia]+shift*p3[ia];
    }

  this->SetFreezeUpdates(1);
  for (int i=0;i<=2;i++)
    {
      if (i==2)
	this->SetFreezeUpdates(0);
      this->Put(3+i,ori[i]);
    }
       
  int flag=this->IsInsideRegion(ori);
  fprintf(stderr,"Origin=%.2f,%.2f,%.2f (%.2f,%.2f,%.2f) inside=%d (***)\n",ori[0],ori[1],ori[2],0.5*ori[0],0.5*ori[1],0.5*ori[2],flag);
  fprintf(stderr,"X- Axis=%.2f,%.2f,%.2f\n\n",p1[0],p1[1],p1[2]);
  fprintf(stderr,"Z- Axis=%.2f,%.2f,%.2f\n\n",p3[0],p3[1],p3[2]);

  float x[3],z[3];
  for (int ia=0;ia<=2;ia++)
    {
      x[ia]=p1[ia];
      z[ia]=p3[ia];
    }
  this->SetLocalXZAxis(x,z);
}


// ----------------------------------------------------------------------------------------------
int vtkpxArticulatedJointTransform::GetNumberOfParameters()
{
  int nt=this->SimilarityTransform->GetNumberOfParameters();
  if (this->SeparateZRotation==1)
    nt+=this->ZSimilarityTransform->GetNumberOfParameters();
  return nt;
}
// ----------------------------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::SetParameters(vtkDoubleArray *parameters,int InputOffset)
{
  this->SimilarityTransform->SetParameters(parameters,InputOffset);
  if (this->SeparateZRotation==1)
    {
      int nt=this->SimilarityTransform->GetNumberOfParameters();
      this->ZSimilarityTransform->SetParameters(parameters,InputOffset+nt);
    }
  return;
}
// ----------------------------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::GetParameters(vtkDoubleArray *parameters,int InputOffset)
{
  this->SimilarityTransform->GetParameters(parameters,InputOffset);
  if (this->SeparateZRotation==1)
    {
      int nt=this->SimilarityTransform->GetNumberOfParameters();
      this->ZSimilarityTransform->GetParameters(parameters,InputOffset+nt);
    }
}
// ----------------------------------------------------------------------------------------------
void vtkpxArticulatedJointTransform::SetLocalXZAxis(float x[3],float z[3])
{
  this->SimilarityTransform->SetLocalXZAxis(x,z);
  this->ZSimilarityTransform->SetLocalXZAxis(x,z);
}
// ----------------------------------------------------------------------------------------------
int vtkpxArticulatedJointTransform::GetMode()
{
  if (this->SeparateZRotation==1)
    return this->SimilarityTransform->GetMode();
  else
    return 1;
}
// ----------------------------------------------------------------------------------------------
// Description:
// Puts a transformation parameter (transformation matrix is updated)
void  vtkpxArticulatedJointTransform::Put(int a, float v)
{
  if (this->SeparateZRotation==0)
    {
      this->SimilarityTransform->Put(a,v); 
      return;
    }

  if (a<6 || a>8)
    {
      this->SimilarityTransform->Put(a,v);  
      this->ZSimilarityTransform->Put(a,v);  
    }
  else if (a==8)
    {
      this->ZSimilarityTransform->Put(6,v);  
    }
  else
    {
      this->SimilarityTransform->Put(a,v);  
    }
}

void  vtkpxArticulatedJointTransform::Set(int a, float v)
{
  this->Put(a,v);
}


float vtkpxArticulatedJointTransform::Get(int a)
{
  if (this->SeparateZRotation==0 || a!=8)
    return this->SimilarityTransform->Get(a); 
  
  return this->ZSimilarityTransform->Get(6);  
}


void vtkpxArticulatedJointTransform::SetFreezeUpdates(int a)
{
  this->ZSimilarityTransform->SetFreezeUpdates(a); 
  this->SimilarityTransform->SetFreezeUpdates(a); 
}

int  vtkpxArticulatedJointTransform::GetFreezeUpdates()
{
  return this->SimilarityTransform->GetFreezeUpdates();
}

// ------------------------------------------------------------------------------------

int vtkpxArticulatedJointTransform::InitializeAngles(double source0[3],double target0[3])
{
  vtkMath::Normalize(source0);
  vtkMath::Normalize(target0);

  double source[3],target[3];
  for (int ia=0;ia<=2;ia++)
    {
      source[ia]=source0[ia];
      target[ia]=target0[ia];
    }

  vtkMath::Normalize(target);

  float z1[3]; this->GetLocalZAxis(z1);
  double z[3]; 
  for (int ia=0;ia<=2;ia++)
    z[ia]=z1[ia];

  vtkMath::Normalize(z);

  double temp[3];

  if (this->SeparateZRotation)
    {
      double p1[3],p2[3];
  
      double a1=vtkMath::Dot(z,source);
      double a2=vtkMath::Dot(z,target);

      fprintf(stderr,"\t\t --------------- Beginning Z Rotation -----------------------------\n");
      fprintf(stderr,"\t\t Source = (%.2f,%.2f,%.2f) %.2f \n",source[0],source[1],source[2],a1);
      fprintf(stderr,"\t\t Target = (%.2f,%.2f,%.2f) %.2f \n",target[0],target[1],target[2],a2);
      
      for (int ia=0;ia<=2;ia++)
	{
	  source[ia]=source[ia]-a1*z[ia];
	  target[ia]=target[ia]-a2*z[ia];
	}
      vtkMath::Normalize(source);
      vtkMath::Normalize(target);
      double cosangle=vtkMath::Dot(source,target);

      fprintf(stderr,"\t\t Source' = (%.2f,%.2f,%.2f) \n",source[0],source[1],source[2]);
      fprintf(stderr,"\t\t Target' = (%.2f,%.2f,%.2f) \n",target[0],target[1],target[2]);
      fprintf(stderr,"\t\t Z-axis  = (%.2f,%.2f,%.2f) \n",z[0],z[1],z[2]);
      
      vtkMath::Cross(source,target,temp);
      double sign=1.0;
      if (vtkMath::Dot(temp,z)<0.0)
	sign=-1.0;
      
      double phi=sign*acos(cosangle)*180.0/vtkMath::Pi();
      fprintf(stderr,"\t\t cosangle=%.3f phi=%.2f\n",cosangle,phi);

      // Now Find Residual
      this->ZSimilarityTransform->Put(6,phi);

      vtkTransform* tr=vtkTransform::New();
      tr->RotateWXYZ(phi,z[0],z[1],z[2]);
      tr->TransformPoint(source0,source);
    }
  else
    {
      for (int ia=0;ia<=2;ia++)
	source[ia]=source0[ia];
    }

  for (int ia=0;ia<=2;ia++)
    target[ia]=target0[ia];
  
  fprintf(stderr,"\t\t --------------- Beginning XY Rotation -----------------------------\n");
  fprintf(stderr,"\t\t  source=(%.2f,%.2f,%.2f)\n",source[0],source[1],source[2]);
  fprintf(stderr,"\t\t  target=(%.2f,%.2f,%.2f)\n",target[0],target[1],target[2]);
  
  float x1[3]; this->GetLocalXAxis(x1);
  double x[3]; 
  for (int ia=0;ia<=2;ia++)
    x[ia]=x1[ia];

  vtkMath::Normalize(x);

  double newaxis[3];
  vtkMath::Cross(source,target,newaxis);
  vtkMath::Normalize(newaxis);
  fprintf(stderr,"\t\t Newaxis = (%.2f,%.2f,%.2f)\n\t\t zaxis=(%.2f,%.2f,%.2f)\n",newaxis[0],newaxis[1],newaxis[2],z[0],z[1],z[2]);

  double dt=vtkMath::Dot(newaxis,z);
  for (int ia=0;ia<=2;ia++)
    newaxis[ia]=newaxis[ia]-dt*z[ia];
  vtkMath::Normalize(newaxis);
  
  fprintf(stderr,"\t\t Newaxis' = (%.2f,%.2f,%.2f) %.2f \n",newaxis[0],newaxis[1],newaxis[2],dt);
  double a1=vtkMath::Dot(source,newaxis);
  double a2=vtkMath::Dot(target,newaxis);

  for (int ia=0;ia<=2;ia++)
    {
      source[ia]=source[ia]-a1*newaxis[ia];
      target[ia]=target[ia]-a2*newaxis[ia];
    }

  vtkMath::Normalize(source);
  vtkMath::Normalize(target);
  double cosangle=vtkMath::Dot(source,target);

  vtkMath::Cross(source,target,temp);
  double sign=1.0;
  if (vtkMath::Dot(temp,newaxis)<0.0)
    sign=-1.0;
  
  double theta=sign*acos(cosangle)*180.0/vtkMath::Pi();


  fprintf(stderr,"\t\t Source' = (%.2f,%.2f,%.2f) %.2f \n",source[0],source[1],source[2],a1);
  fprintf(stderr,"\t\t Target' = (%.2f,%.2f,%.2f) %.2f \n",target[0],target[1],target[2],a2);
  fprintf(stderr,"\t\t Cosangle=%.2f theta=%.2f\n",cosangle,theta);

  double theta2=0.0,cosangle2=1.0;

  if (fabs(theta)>2.0)
    {
      vtkMath::Cross(x,newaxis,temp);
      double sign=1.0;
      if (vtkMath::Dot(temp,z)<0.0)
	sign=-1.0;
      
      fprintf(stderr,"\t\t X-axis = (%.2f,%.2f,%.2f)  \n",x[0],x[1],x[2]);
      cosangle2=vtkMath::Dot(x,newaxis);
      theta2=sign*acos(cosangle2)*180.0/vtkMath::Pi();
    }

  fprintf(stderr,"\t\t Cosangle2=%.2f theta2=%.2f\n",cosangle2,theta2);

  this->SimilarityTransform->Put(7,theta2);
  this->SimilarityTransform->Put(6,theta);


  return 1;
}

