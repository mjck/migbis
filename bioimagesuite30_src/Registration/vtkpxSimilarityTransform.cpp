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
#include "vtkpxSimilarityTransform.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkpxUtil.h"
#include "vtkTransform.h"
//----------------------------------------------------------------------------
vtkpxSimilarityTransform* vtkpxSimilarityTransform::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxSimilarityTransform");
  if(ret)
    {
      return (vtkpxSimilarityTransform*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxSimilarityTransform;
}

//----------------------------------------------------------------------------
vtkpxSimilarityTransform::vtkpxSimilarityTransform()
{
  this->Mode=0;
  this->Initialize();
  this->LocalZAxis[0] = 0.0;
  this->LocalZAxis[1] = 0.0;
  this->LocalZAxis[2] = 1.0;
  this->LocalXAxis[0] = 1.0;
  this->LocalXAxis[1] = 0.0;
  this->LocalXAxis[2] = 0.0;
  this->TranslationScale=1.0;
  this->LocalAxisTransform=vtkMatrixToLinearTransform::New();

}

//----------------------------------------------------------------------------
vtkpxSimilarityTransform::~vtkpxSimilarityTransform()
{
  this->LocalAxisTransform->Delete();
}

//----------------------------------------------------------------------------
void vtkpxSimilarityTransform::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "### " << this->GetClassName() << " ### \n";
  os  << indent << "Transformation Parameters: \n";
  os << indent << "Mode: " << this->Mode << "\n";
  os << indent << "Origin:      " << this->Origin[0] << " " << this->Origin[1]  << " " << this->Origin[2] << " "  << "\n";
  os << indent << "Angle:      " << this->Angle[0] << " " << this->Angle[1]  << " " << this->Angle[2] << " "  << "\n";
  os << indent << "Translation: " << this->Translation[0] << " " << this->Translation[1]  << " " << this->Translation[2] << " "  << "\n";
  os << indent << "Scaling: " << this->ScaleFactor << "\n";
  os << indent << "FreezeUpdates: " << this->FreezeUpdates << "\n";
  os << indent << "RotationAxis:      " << this->RotationAxis[0] << " " << this->RotationAxis[1]  << " " << this->RotationAxis[2] << " "  << "\n";
  os << indent << "RotationAngle: " << this->RotationAngle << "\n";
  os << indent << "LocalXAxis:      " << this->LocalXAxis[0] << " " << this->LocalXAxis[1]  << " " << this->LocalXAxis[2] << " "  << "\n";
  os << indent << "LocalZAxis:      " << this->LocalZAxis[0] << " " << this->LocalZAxis[1]  << " " << this->LocalZAxis[2] << " "  << "\n";
  os << indent << "LocalAxisTransform: " << this->LocalAxisTransform << "\n";
  os << indent << "TranslationScale: " << this->TranslationScale << "\n";
  this->vtkMatrixToLinearTransform::PrintSelf(os, indent.GetNextIndent());
}


//----------------------------------------------------------------------------
void vtkpxSimilarityTransform::SetLocalXZAxis(float x[3],float z[3])
{
  vtkMath::Normalize(x);
  vtkMath::Normalize(z);

  for (int i=0;i<=2;i++)
    {
      this->LocalZAxis[i]=z[i];
      this->LocalXAxis[i]=x[i];
    }

  // Construct LocalAxisTransform and we are done.
  float y[3];  vtkMath::Cross(z,x,y);

  vtkMatrix4x4* mat=vtkMatrix4x4::New();
  mat->Identity();
  for (int i=0;i<=2;i++)
    {
      mat->SetElement(i,0,x[i]);
      mat->SetElement(i,1,y[i]);
      mat->SetElement(i,2,z[i]);
    }
  mat->Invert();

  this->LocalAxisTransform->SetInput(mat);
  this->LocalAxisTransform->Update();
  mat->Delete();

}
//----------------------------------------------------------------------------
/*void vtkpxSimilarityTransform::UpdateMatrix()
{
  if (this->FreezeUpdates)
    return;


  // Generate Rotation Transformation

  vtkTransform* rot=vtkTransform::New();
  rot->Identity();
  rot->PostMultiply();

  // Explicit Angle Axis Representation
  // In Local Coordinate System
  // First Take Local x-axis --> rotate it via Z
  // 
  double p1[3]= { 1,0,0} ;
  if (this->Mode==9)
    {
      p1[0]=0.0;
      p1[1]=0.0;
      p1[2]=1.0;
    }
  else
    {
      rot->RotateY(this->Angle[2]);
      rot->TransformVector(p1,p1);
      
      //  fprintf(stderr,"Post Angle 2 (Y-Rotate %.2f) New X Axis = %.2f %.2f %.2f\n",this->Angle[2],p1[0],p1[1],p1[2]);
      
      rot->Identity();
      rot->RotateZ(this->Angle[1]);
      rot->TransformVector(p1,p1);
      //  fprintf(stderr,"Post Angle 1 (Z-Rotate %.2f) New X Axis = %.2f %.2f %.2f\n",this->Angle[1],p1[0],p1[1],p1[2]);
      rot->Delete();
    }

  float scale=this->ScaleFactor*0.01;

  vtkTransform* tr=vtkTransform::New();

  int maxpass=2;
  if (this->Angle[0]!=0.0)
    maxpass=1;
  
  for (int pass=0;pass<maxpass;pass++)
    {
      tr->Identity();
      tr->PostMultiply();
      
      // First Get it to a Local Coordinate System
      tr->Translate(-this->Origin[0],-this->Origin[1],-this->Origin[2]);
      tr->Concatenate(this->LocalAxisTransform);
      
      // Now Apply Scale, Rotations
      tr->Scale(scale,scale,scale);
      
      if (maxpass==2 && pass==0)
	tr->RotateWXYZ(1.0,p1[0],p1[1],p1[2]);
      else
	tr->RotateWXYZ(this->Angle[0],p1[0],p1[1],p1[2]);

      // Back to Global Coordinate System
      tr->Concatenate(this->LocalAxisTransform->GetLinearInverse());
      tr->Translate(this->Origin);
      
      // Add Translation 
      tr->Translate(this->Translation);
      tr->Update();

      double wxyz[4];  
      tr->GetOrientationWXYZ(wxyz);

      if (pass==0)
	{
	  for (int i=0;i<=2;i++)
	    this->RotationAxis[i]=wxyz[i+1];
	}

      if (pass==maxpass-1)
	{
	  this->RotationAngle=wxyz[0];
	  if (this->RotationAngle*this->Angle[0] < 0 )
	    {
	      this->RotationAngle=-wxyz[0];
	      for (int i=0;i<=2;i++)
		this->RotationAxis[i]=-wxyz[i+1];
	    }
	  while (this->RotationAngle<-180.0)
	    this->RotationAngle+=360.0;
	  while (this->RotationAngle>180.0)
	    this->RotationAngle-=360.0;
	}
    }


  this->SetInput(tr->GetMatrix());
  this->Update();
  tr->Delete();
}*/
//----------------------------------------------------------------------------
void vtkpxSimilarityTransform::UpdateMatrix()
{
  if (this->FreezeUpdates)
    return;


  // Generate Rotation Transformation

  float scale=this->ScaleFactor*0.01;

  vtkTransform* tr=vtkTransform::New();
  tr->Identity();
  tr->PostMultiply();
  
  // First Get it to a Local Coordinate System
  tr->Translate(-this->Origin[0],-this->Origin[1],-this->Origin[2]);
  tr->Concatenate(this->LocalAxisTransform);
  
  // Now Apply Scale, Rotations
  tr->Scale(scale,scale,scale);
  
  if (this->Mode==9)
    {
      tr->RotateZ(this->Angle[0]);
    }
  else
    {
      tr->RotateZ(this->Angle[2]);
      tr->RotateY(this->Angle[1]);
      tr->RotateX(this->Angle[0]);
    }
  
  // Back to Global Coordinate System
  tr->Concatenate(this->LocalAxisTransform->GetLinearInverse());
  tr->Translate(this->Origin);
  
  // Add Translation 
  tr->Translate(this->Translation);
  tr->Update();

  double wxyz[4];  
  tr->GetOrientationWXYZ(wxyz);

  for (int i=0;i<=2;i++)
    this->RotationAxis[i]=wxyz[i+1];

  this->RotationAngle=wxyz[0];
  if (this->RotationAngle*this->Angle[0] < 0 )
    {
      this->RotationAngle=-wxyz[0];
      for (int i=0;i<=2;i++)
	this->RotationAxis[i]=-wxyz[i+1];
    }

  while (this->RotationAngle<-180.0)
    this->RotationAngle+=360.0;
  while (this->RotationAngle>180.0)
    this->RotationAngle-=360.0;


  this->SetInput(tr->GetMatrix());
  this->Update();
  tr->Delete();
}
//----------------------------------------------------------------------------
void vtkpxSimilarityTransform::SetMode(int m)
{
  if (m<-1)
    this->Mode=-1;
  else if (m>9)
    this->Mode=9;
  else 
    this->Mode=m;
}
//----------------------------------------------------------------------------
int vtkpxSimilarityTransform::GetNumberOfParameters()
{
  switch (this->Mode)
    {
    case -1: // Fixed Parameters
      return 0;
      break;
    case 0:  // XY Rotation Only
      return 2;
      break;
    case 1:  // Rotation Only
      return 3;
      break;
    case 2: // Rotation+Scale Only
      return 4;
      break;
    case 3: // Rotation + Translation
      return 6;
      break;
    case 4: // Rotation + Scale + Translation
      return 7;
      break;
    case 5: // Rotation + Origin
      return 6;
      break;
    case 6: // Rotation + Scale + Origin
      return 7;
      break;
    case 7: // Rotation + Translation + Origin
      return 9;
    case 8: // All of them 
      return 10;
      break;
    case 9: // Z-Rot Only
      return 1;
      break;
    }
  return 0;
}

//----------------------------------------------------------------------------
void vtkpxSimilarityTransform::GetParameters(vtkDoubleArray *parameters,int InputOffset)
{
  int np=this->GetNumberOfParameters();
  if (np==-1)
    return;
  
  if (parameters->GetNumberOfTuples()-InputOffset<np)
    return;

  if (this->Mode==8)
    {
      for (int i=0;i<np;i++)
	parameters->SetComponent(InputOffset+i,0,this->Get(i));
    }
  else 
    {
      int offset=3;
      if (this->Mode==0)
	offset=2;
      else if (this->Mode==9)
	offset=1;
      
      for (int i=0;i<offset;i++)
	parameters->SetComponent(InputOffset+i,0,this->Angle[i]);
            
      if (this->Mode == 2 || this->Mode == 4 || this->Mode == 6)
	{
	  parameters->SetComponent(InputOffset+offset,0,this->ScaleFactor);
	  ++offset;
	}
      
      if (this->Mode == 3 || this->Mode == 4 || this->Mode == 7)
	{
	  for (int i=0;i<=2;i++)
	    parameters->SetComponent(InputOffset+offset+i,0,this->Translation[i]*this->TranslationScale);
	  offset+=3;
	}
      
      if (this->Mode == 5 || this->Mode == 6 || this->Mode == 7 )
	{
	  for (int i=0;i<=2;i++)
	    parameters->SetComponent(InputOffset+offset+i,0,this->Origin[i]);
	}
    }
  return;
}

void vtkpxSimilarityTransform::SetParameters(vtkDoubleArray *parameters,int InputOffset)
{
  int np=this->GetNumberOfParameters();
  if (np==-1)
    return;

  if (parameters->GetNumberOfTuples()-InputOffset<np)
    return;
  
  this->FreezeUpdates=1;
  if (this->Mode==8)
    {
      for (int i=0;i<np;i++)
	this->Put(i,parameters->GetComponent(InputOffset+i,0));
    }
  else 
    {
      int offset=3;
      if (this->Mode==0)
	offset=2;
      else if (this->Mode==9)
	offset=1;

      for (int i=0;i<offset;i++)
	this->Angle[i]=parameters->GetComponent(InputOffset+i,0);
      
      if (this->Mode == 2 || this->Mode == 4 || this->Mode == 6)
	{
	  this->ScaleFactor=parameters->GetComponent(InputOffset+offset,0);
	  ++offset;
	}
      
      if (this->Mode == 3 || this->Mode == 4 || this->Mode == 7)
	{
	  for (int i=0;i<=2;i++)
	    this->Translation[i]=parameters->GetComponent(InputOffset+offset+i,0)/this->TranslationScale;
	  offset+=3;
	}
      
      if (this->Mode == 5 || this->Mode == 6 || this->Mode == 7 )
	{
	  for (int i=0;i<=2;i++)
	    this->Origin[i]=parameters->GetComponent(InputOffset+offset+i,0);
	}
    }
  this->FreezeUpdates=0;
  this->UpdateMatrix();
}
//----------------------------------------------------------------------------
float vtkpxSimilarityTransform::Get(int i) const
{
  switch (i) 
    {
    case 0:
      return this->Translation[0];
      break;
    case 1:
      return this->Translation[1];
      break;
    case 2:
      return this->Translation[2];
      break;
    case 3:
      return this->Origin[0];
      break;
    case 4:
      return this->Origin[1];
      break;
    case 5:
      return this->Origin[2];
      break;
    case 6:
      return this->Angle[0];
      break;
    case 7:
      return this->Angle[1];
      break;
    case 8:
      return this->Angle[2];
      break;
    case 9:
      return this->ScaleFactor;
      break;
    default:
      cerr << "No such dof" << i << endl;
      return 0;
    }
}
//----------------------------------------------------------------------------
void vtkpxSimilarityTransform::Put(int i, float x)
{
  switch (i) 
    {
    case 0:
      this->Translation[0] = x;
      break;
    case 1:
      this->Translation[1] = x;
      break;
    case 2:
      this->Translation[2] = x;
      break;
    case 3:
      this->Origin[0] = x;
      break;
    case 4:
      this->Origin[1] = x;
      break;
    case 5:
      this->Origin[2] = x;
      break;
    case 6:
      this->Angle[0]=x;
      break;
    case 7:
      this->Angle[1]=x;
      break;
    case 8:
      this->Angle[2]=x;
      break;
    case 9:
      this->ScaleFactor=x;
      if (this->ScaleFactor<1.01)
	this->ScaleFactor=100.0;
      break;
    default:
      cerr << "Put(): No such dof" << i << endl;
      return;
    }
  this->UpdateMatrix();
}
//----------------------------------------------------------------------------
int vtkpxSimilarityTransform::Load(const char* fname)
{
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  int ok=this->Load(fin);
  gzclose(fin);
  return ok;
}

int vtkpxSimilarityTransform::Load(gzFile fin)
{
  char pxtk_buffer[101];
  gzgets(fin,pxtk_buffer,100);
  
  if (gzfindstringinstring(pxtk_buffer,"#vtkpxSimilarityTransform")==0)
    return 0;

  gzgets(fin,pxtk_buffer,100);

  int tmp;
  float v;
  this->FreezeUpdates=1;

  gzgets(fin,pxtk_buffer,200);   gzgets(fin,pxtk_buffer,200); 
  sscanf(pxtk_buffer,"%f %f %f",&this->Angle[0],&this->Angle[1],&this->Angle[2]);

  gzgets(fin,pxtk_buffer,200);   gzgets(fin,pxtk_buffer,200); 
  sscanf(pxtk_buffer,"%f %f %f",&this->Translation[0],&this->Translation[1],&this->Translation[2]);

  gzgets(fin,pxtk_buffer,200);   gzgets(fin,pxtk_buffer,200); 
  sscanf(pxtk_buffer,"%f %f %f",&this->Origin[0],&this->Origin[1],&this->Origin[2]);

  float x[3],z[3];   
  gzgets(fin,pxtk_buffer,200);   gzgets(fin,pxtk_buffer,200);   sscanf(pxtk_buffer,"%f %f %f",&x[0],&x[1],&x[2]);
  gzgets(fin,pxtk_buffer,200);   gzgets(fin,pxtk_buffer,200);   sscanf(pxtk_buffer,"%f %f %f",&z[0],&z[1],&z[2]);
  this->SetLocalXZAxis(x,z);

  gzgets(fin,pxtk_buffer,200);   gzgets(fin,pxtk_buffer,200); 
  sscanf(pxtk_buffer,"%f",&this->ScaleFactor);

  gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
  sscanf(pxtk_buffer,"%d",&tmp);
  this->SetMode(tmp);
  this->FreezeUpdates=0;

  this->UpdateMatrix();

  return 1;
}
//----------------------------------------------------------------------------
int vtkpxSimilarityTransform::Save(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
    return 0;
  
  int ok=this->Save(fout);
  fclose(fout);
  return ok;
}

int vtkpxSimilarityTransform::Save(FILE* fout)
{
  fprintf(fout,"#vtkpxSimilarityTransform2\n");
  fprintf(fout,"#Actual Parameters\n");
  fprintf(fout,"#Rotation\n %.4f %.4f %.4f\n",this->Angle[0],this->Angle[1],this->Angle[2]);
  fprintf(fout,"#Translation\n %.4f %.4f %.4f\n",this->Translation[0],this->Translation[1],this->Translation[2]);
  fprintf(fout,"#Origin\n %.4f %.4f %.4f\n",this->Origin[0],this->Origin[1],this->Origin[2]);
  fprintf(fout,"#LocalXAxis\n %.4f %.4f %.4f\n",this->LocalXAxis[0],this->LocalXAxis[1],this->LocalXAxis[2]);
  fprintf(fout,"#LocalZAxis\n %.4f %.4f %.4f\n",this->LocalZAxis[0],this->LocalZAxis[1],this->LocalZAxis[2]);
  fprintf(fout,"#Scale\n %.4f\n",this->ScaleFactor);
  fprintf(fout,"#Mode\n%d\n",this->Mode);
  return 1;
}
//----------------------------------------------------------------------------
void vtkpxSimilarityTransform::Initialize()
{
  this->FreezeUpdates=1;
  this->ScaleFactor=100.0;
  for (int i=0;i<=2;i++)
    {
      this->Origin[i]=0.0; 
      this->Angle[i]=0.0;
      this->Translation[i]=0.0;
    }
  this->FreezeUpdates=0;
}
//----------------------------------------------------------------------------
void vtkpxSimilarityTransform::Identity()
{
  this->ScaleFactor=100.0;
  for (int i=0;i<=2;i++)
    {
      this->Angle[i]=0.0;
      this->Translation[i]=0.0;
    }
  this->UpdateMatrix();
}
//----------------------------------------------------------------------------
void vtkpxSimilarityTransform::Copy(vtkpxSimilarityTransform* other)
{
  if (other==NULL)
    return;

  this->FreezeUpdates=1;
  for (int i=3;i<=5;i++)
    this->Put(i,other->Get(i));
  
  this->SetLocalXZAxis(other->GetLocalXAxis(),other->GetLocalZAxis());
  this->CopyParameters(other);
}

void vtkpxSimilarityTransform::CopyParameters(vtkpxSimilarityTransform* other)
{
  if (other==NULL)
    return;

  this->FreezeUpdates=1;
  for (int i=0;i<10;i++)
    if (i<3 || i > 5)
      this->Put(i,other->Get(i));

  this->SetTranslationScale(this->GetTranslationScale());

  this->FreezeUpdates=0;
  this->UpdateMatrix();
}

//----------------------------------------------------------------------------
void vtkpxSimilarityTransform::ApproximatePoints(vtkPoints* SourceLandmarks,vtkPoints* TargetLandmarks,vtkpxMatrix* Weights)
{
  int j;

  if (SourceLandmarks == NULL || TargetLandmarks == NULL)
    return;

  // --- compute the necessary transform to match the two sets of landmarks ---

  /*
    The solution is based on
    Berthold K. P. Horn (1987),
    "Closed-form solution of absolute orientation using unit quaternions,"
    Journal of the Optical Society of America A, 4:629-642
  */

  // Original python implementation by David G. Gobbi
  // Weight Stuff added by Xenios Papademetris
  // Moved out of vtkpxWeightedLandamarkTransform to Support Offsets etc.
  // Center of Rotation is now the the Origin[3] not the centroid of the points
  // No alignment by matching centroids etc

  const vtkIdType N_PTS = SourceLandmarks->GetNumberOfPoints();
  if (N_PTS < 3)
    return;

  if(N_PTS != TargetLandmarks->GetNumberOfPoints())
    {
      vtkErrorMacro(<<"Update: Source and Target Landmarks contain a different number of points");
      return;
    }

  if (Weights!=NULL)
    {
      if(N_PTS != Weights->GetSize()[0] || Weights->GetSize()[1]!=1)
	{
	  vtkDebugMacro(<<"Update: Source Landmarks and Weight Matrix contain a different number of points");
	  Weights=NULL;
	}
    }


  // -- if no points, stop here


  vtkDebugMacro(<<"Using Weighted Least Squares Landmark Fitting\n");
  
  // -- find the centroid of each set --

  // No more centroid stuff, rotation is about a fixed point i.e. Origin[3]
  float source_centroid[3]={0,0,0};
  float target_centroid[3]={0,0,0};


  for(int i=0;i<N_PTS;i++)
    {
      double* p;
      p = SourceLandmarks->GetPoint(i);
      source_centroid[0] += p[0];
      source_centroid[1] += p[1];
      source_centroid[2] += p[2];
      p = TargetLandmarks->GetPoint(i);

      target_centroid[0] += p[0];
      target_centroid[1] += p[1];
      target_centroid[2] += p[2];
    }
  source_centroid[0] /= float(N_PTS);
  source_centroid[1] /= float(N_PTS);
  source_centroid[2] /= float(N_PTS);
  target_centroid[0] /= float(N_PTS);
  target_centroid[1] /= float(N_PTS);
  target_centroid[2] /= float(N_PTS);


  // -- build the 3x3 matrix M --
  float M[3][3];
  float AAT[3][3];
  for(int i=0;i<3;i++) 
    {
      AAT[i][0] = M[i][0]=0.0F; // fill M with zeros
      AAT[i][1] = M[i][1]=0.0F; 
      AAT[i][2] = M[i][2]=0.0F; 
    }

  vtkIdType pt;
  double a[3],b[3];
  float sa=0.0F,sb=0.0F;
  for(pt=0;pt<N_PTS;pt++)
    {
      float wgt=1.0;
      if (Weights!=NULL)
	wgt=Weights->GetDirectElement(pt,0);


      // get the origin-centred point (a) in the source set
      SourceLandmarks->GetPoint(pt,a);
      TargetLandmarks->GetPoint(pt,b);

      for (int ia=0;ia<=2;ia++)
	{
	  if (this->Mode < 5 )
	    {
	      a[ia]-=this->Origin[ia];
	      b[ia]-=this->Origin[ia];
	    }
	  else
	    {
	      a[ia]-=source_centroid[ia];
	      b[ia]-=target_centroid[ia];
	      this->Origin[ia]=0.0;
	    }
	}
      // accumulate the products a*T(b) into the matrix M
      for(int i=0;i<3;i++) 
	{
	  M[i][0] += wgt*a[i]*b[0];
	  M[i][1] += wgt*a[i]*b[1];
	  M[i][2] += wgt*a[i]*b[2];
	}
      // accumulate scale factors (if desired)
      sa += wgt*(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
      sb += wgt*(b[0]*b[0]+b[1]*b[1]+b[2]*b[2]);
    }
  
  // compute required scaling factor (if desired)
  float scale = (float)sqrt(sb/sa);
  
  // -- build the 4x4 matrix N --
  
  float Ndata[4][4];
  float *N[4];
  for(int i=0;i<4;i++)
    {
      N[i] = Ndata[i];
      N[i][0]=0.0F; // fill N with zeros
      N[i][1]=0.0F;
      N[i][2]=0.0F;
      N[i][3]=0.0F;
    }
  // on-diagonal elements
  N[0][0] = M[0][0]+M[1][1]+M[2][2];
  N[1][1] = M[0][0]-M[1][1]-M[2][2];
  N[2][2] = -M[0][0]+M[1][1]-M[2][2];
  N[3][3] = -M[0][0]-M[1][1]+M[2][2];
  // off-diagonal elements
  N[0][1] = N[1][0] = M[1][2]-M[2][1];
  N[0][2] = N[2][0] = M[2][0]-M[0][2];
  N[0][3] = N[3][0] = M[0][1]-M[1][0];
  
  N[1][2] = N[2][1] = M[0][1]+M[1][0];
  N[1][3] = N[3][1] = M[2][0]+M[0][2];
  N[2][3] = N[3][2] = M[1][2]+M[2][1];
  
  // -- eigen-decompose N (is symmetric) --
  
  float eigenvectorData[4][4];
  float *eigenvectors[4],eigenvalues[4];
  
  eigenvectors[0] = eigenvectorData[0];
  eigenvectors[1] = eigenvectorData[1];
  eigenvectors[2] = eigenvectorData[2];
  eigenvectors[3] = eigenvectorData[3];
  
  vtkMath::JacobiN(N,4,eigenvalues,eigenvectors);
  
  // the eigenvector with the largest eigenvalue is the quaternion we want
  // (they are sorted in decreasing order for us by JacobiN)
  double w,x,y,z;
  
  // first: if points are collinear, choose the quaternion that 
  // results in the smallest rotation.
  if (eigenvalues[0] == eigenvalues[1])
    {
      double s0[3],t0[3],s1[3],t1[3];
      SourceLandmarks->GetPoint(0,s0);
      TargetLandmarks->GetPoint(0,t0);

      SourceLandmarks->GetPoint(1,s1);
      TargetLandmarks->GetPoint(1,t1);

      double ds[3],dt[3];
      double rs = 0, rt = 0;
      for (int i = 0; i < 3; i++)
	{
	  ds[i] = s1[i] - s0[i];      // vector between points
	  rs += ds[i]*ds[i];
	  dt[i] = t1[i] - t0[i];
	  rt += dt[i]*dt[i];
	}
      
      // normalize the two vectors
      rs = sqrt(rs);
      ds[0] /= rs; ds[1] /= rs; ds[2] /= rs; 
      rt = sqrt(rt);
      dt[0] /= rt; dt[1] /= rt; dt[2] /= rt; 
      
      // take dot & cross product
      w = ds[0]*dt[0] + ds[1]*dt[1] + ds[2]*dt[2];
      x = ds[1]*dt[2] - ds[2]*dt[1];
      y = ds[2]*dt[0] - ds[0]*dt[2];
      z = ds[0]*dt[1] - ds[1]*dt[0];
      
      double r = sqrt(x*x + y*y + z*z);
      double theta = atan2(r,w);
      
      // construct quaternion
      w = cos(theta/2);
      if (r != 0)
	{
	  r = sin(theta/2)/r;
	  x = x*r;
	  y = y*r;
	  z = z*r;
	}
      else // rotation by 180 degrees: special case
	{
	  // rotate around a vector perpendicular to ds
	  vtkMath::Perpendiculars(ds,dt,0,0);
	  r = sin(theta/2);
	  x = dt[0]*r;
	  y = dt[1]*r;
	  z = dt[2]*r;
	}
    }
  else // points are not collinear
    {
      w = eigenvectors[0][0];
      x = eigenvectors[1][0];
      y = eigenvectors[2][0];
      z = eigenvectors[3][0];
    }
      
  // convert quaternion to a rotation matrix
  
  double ww = w*w;
  double wx = w*x;
  double wy = w*y;
  double wz = w*z;
  
  double xx = x*x;
  double yy = y*y;
  double zz = z*z;
  
  double xy = x*y;
  double xz = x*z;
  double yz = y*z;


  // Step 1 Get Rotation 
  // -------------------
  vtkMatrix4x4* fitMatrix=vtkMatrix4x4::New();
  fitMatrix->Identity();
  fitMatrix->Element[0][0] = ww + xx - yy - zz; 
  fitMatrix->Element[1][0] = 2.0*(wz + xy);
  fitMatrix->Element[2][0] = 2.0*(-wy + xz);
  
  fitMatrix->Element[0][1] = 2.0*(-wz + xy);  
  fitMatrix->Element[1][1] = ww - xx + yy - zz;
  fitMatrix->Element[2][1] = 2.0*(wx + yz);
  
  fitMatrix->Element[0][2] = 2.0*(wy + xz);
  fitMatrix->Element[1][2] = 2.0*(-wx + yz);
  fitMatrix->Element[2][2] = ww - xx - yy + zz;
  
  
  // First Get Angles 
  vtkTransform* tr=vtkTransform::New();
  tr->SetMatrix(fitMatrix);
  tr->GetOrientation(this->Angle);
  tr->Delete();
  
  if (this->Mode ==2 || this->Mode == 4 || this->Mode == 7)
    { 
      this->ScaleFactor=scale*100.0;
      for(int i=0;i<3;i++) 
	{
	  fitMatrix->Element[i][0] *= scale;
	  fitMatrix->Element[i][1] *= scale;
	  fitMatrix->Element[i][2] *= scale;
	}
    }
  
  // the translation is given by the difference in the transformed source
  // centroid and the target centroid
  double sx, sy, sz;
  
  sx = fitMatrix->Element[0][0] * source_centroid[0] +
    fitMatrix->Element[0][1] * source_centroid[1] +
    fitMatrix->Element[0][2] * source_centroid[2];
  sy = fitMatrix->Element[1][0] * source_centroid[0] +
    fitMatrix->Element[1][1] * source_centroid[1] +
    fitMatrix->Element[1][2] * source_centroid[2];
  sz = fitMatrix->Element[2][0] * source_centroid[0] +
    fitMatrix->Element[2][1] * source_centroid[1] +
    fitMatrix->Element[2][2] * source_centroid[2];
  
  this->Translation[0] = target_centroid[0] - sx;
  this->Translation[1] = target_centroid[1] - sy;
  this->Translation[2] = target_centroid[2] - sz;

  fitMatrix->Delete();
  this->UpdateMatrix();
}

// -------------------------------------------------------------------------------------
vtkPolyData* vtkpxSimilarityTransform::CreateBlendedCylinder(vtkPolyData* input,float angle,int mode)
{
  if (input==NULL)
    return NULL;

  double radcon=180.0/vtkMath::Pi();

  for (float a=-1.0;a<=1.0;a+=2.0)
    for (float b=-1.0;b<=1.0;b+=2.0)
      {
	fprintf(stderr,"a=%.1f b=%.1f angle=%.2f\n",a,b,
		radcon*atan2(b,a));
      }



  double bounds[6]; input->GetBounds(bounds);

  if (mode<0)
    mode=0;
  else if (mode>2)
    mode=2;

  double radangle=fabs(angle)/radcon;

  double centroid[3];
  for (int i=0;i<=2;i++)
    centroid[i]=0.5*(bounds[i*2]+bounds[i*2+1]);

  fprintf(stderr,"Centroid =%.2f %.2f %.2f\n",centroid[0],centroid[1],centroid[2]);

  vtkPolyData* output=vtkPolyData::New();
  output->DeepCopy(input);
  

  double c1= 0.25/pow(radangle,2.0);
  double c2=-0.75/radangle;

  int np=output->GetNumberOfPoints();

  if (radangle==0.0)
    np=0;

  for (int i=0;i<np;i++)
    {
      double x[3]; output->GetPoint(i,x);
      
      // First subtract centroid
      for (int ia=0;ia<=2;ia++)
	x[ia]=x[ia]-centroid[ia];

      if (i==np/2)
	fprintf(stderr,"x=(%.2f,%.2f,%.2f) \t ",x[0],x[1],x[2]);

      double r=sqrt(x[0]*x[0]+x[2]*x[2]);
      double theta=atan2(x[2],x[0]);


      if (i==np/2)
	fprintf(stderr,"x=(r=%.2f,theta=%.2f) \t ",r,theta*radcon);

      // 3 Cases
      

      if (mode==0)
	{
	  if (x[2]>0)
	    theta-=radangle;
	}
      else if (mode==1)
	{
	  if (theta>radangle)
	    theta-=radangle;
	  else if (theta<-radangle)
	    theta=theta;
	  else
	    theta= c1*pow(theta,3.0)+0.25*theta-0.5*radangle;
	}
      else if (mode ==2 )
	{
	  if (theta>2.0*radangle)
	    theta-=radangle;
	  else if (theta<=0.0)
	    theta=theta;
	  else
	    theta= c1*pow(theta,3.0)+c2*pow(theta,2.0)+theta;
	}
    
      x[0]=r*cos(theta);
      x[2]=r*sin(theta);

      if (i==np/2)
	fprintf(stderr,"(r=%.2f,theta=%.2f) (%.1f,%.1f,%.1f)\n",r,theta*radcon,x[0],x[1],x[2]);

      
      for (int ia=0;ia<=2;ia++)
	x[ia]=x[ia]+centroid[ia];
      
      output->GetPoints()->SetPoint(i,x);
    }

  return output;
}

