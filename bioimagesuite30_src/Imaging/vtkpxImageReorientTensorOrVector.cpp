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
  Module:    $RCSfile: vtkpxImageReorinetTensorOrVector.cpp,v $
  Language:  C++  Date:      $Date: 2002/02/20 18:06:52 $
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
#include "vtkObjectFactory.h"
#include "vtkpxImageReorientTensorOrVector.h"
#include "vtkMatrix4x4.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkLandmarkTransform.h"
#include "vtkTransformToGrid.h"
#include "stdio.h"
//------------------------------------------------------------------------------
vtkpxImageReorientTensorOrVector* vtkpxImageReorientTensorOrVector::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageReorientTensorOrVector");
  if(ret)
    {
    return (vtkpxImageReorientTensorOrVector*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageReorientTensorOrVector;
}

// Construct object with no children.
vtkpxImageReorientTensorOrVector::vtkpxImageReorientTensorOrVector()
{
  this->Transformation=NULL;
  this->DisplacementField=NULL;
  this->GlobalMatrix=NULL;
  this->Use7Points=0;

  // A bunch of temp structures so we don't allocate and free memory all the time!
  this->TempMatrix1=vtkMatrix4x4::New();
  this->TempMatrix2=vtkMatrix4x4::New();
  this->TempMatrix3=vtkMatrix4x4::New();
  this->TempPoints1=vtkPoints::New();
  this->TempPoints2=vtkPoints::New();
  this->TempLandmarkTransform=vtkLandmarkTransform::New();

  this->FlipX=0;  this->FlipY=0;  this->FlipZ=0;
  this->Flips[0]=1.0;  this->Flips[1]=1.0;  this->Flips[2]=1.0;
}
// ----------------------------------------------------------------------------
vtkpxImageReorientTensorOrVector::~vtkpxImageReorientTensorOrVector()
{
  this->SetTransformation(NULL);
  this->SetDisplacementField(NULL);

  // Clean up Global Matrix if it exists
  if (this->GlobalMatrix!=NULL)
    this->GlobalMatrix->Delete();

  // Clean up Temp Structures
  this->TempMatrix1->Delete();
  this->TempMatrix2->Delete();
  this->TempMatrix3->Delete();
  this->TempPoints1->Delete();
  this->TempPoints2->Delete();
  this->TempLandmarkTransform->Delete();
}
// ----------------------------------------------------------------------------
void r_print_mat44(vtkMatrix4x4* mat,const char* s)
{
  fprintf(stdout,"\n\n %s = \n",s);
  for (int ia=0;ia<3;ia++)
    {
      fprintf(stdout,"\t [ ");
      for (int ib=0;ib<3;ib++)
	fprintf(stdout," %+5.3f  ", mat->GetElement(ia,ib));
      fprintf(stdout,"]\n");
    }
}


// ----------------------------------------------------------------------------
void vtkpxImageReorientTensorOrVector::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  // Check Input Image
  if (input==NULL)
    {
      vtkErrorMacro(<<"No Input to vtkpxImageReorientTensorOrVector");
      return;
    }

  int nc=input->GetNumberOfScalarComponents();
  if (nc!=3 && nc!=6)
    {
      vtkErrorMacro(<<"Bad Input to vtkpxImageReorientTensorOrVector, not a tensor or vector image\n");
      return;
    }
      

  // Either Transformation or Displacement Field must be set!
  if (this->Transformation==NULL && this->DisplacementField==NULL)
    {
      vtkErrorMacro(<<"Specify either a DisplacementField or a Transformation as inputs vtkpxImageReorientTensorOrVector");
      return;
    }

  // If Transformation == NULL  then Displacement Field is used,
  // It must have 3 components and the same number of voxels as the input image
  if (this->Transformation==NULL)
    {
      int dim[3],dim2[3];
      input->GetDimensions(dim);
      this->DisplacementField->GetDimensions(dim2);
      int nc=this->DisplacementField->GetNumberOfScalarComponents();
  
      int sum=0;
      for (int ia=0;ia<=2;ia++)
	sum+=abs(dim[ia]-dim2[ia]);
      if (sum!=0 || nc!=3)
	{
	  vtkErrorMacro(<<"Bad Displacement Field Dimensions");
	  return;
	}
    }
    
  
  if (this->Debug)
    fprintf(stderr,"\n\n This Far All OK\n");

  if (this->FlipX)
    this->Flips[0]=-1.0;
  else
    this->Flips[0]=1.0;

  if (this->FlipY)
    this->Flips[1]=-1.0;
  else
    this->Flips[1]=1.0;

  if (this->FlipZ)
    this->Flips[2]=-1.0;
  else
    this->Flips[2]=1.0;
  
  if (this->Debug)
    fprintf(stderr,"Flip factors : X = %.0f, Y=%.0f, Z=%.0f\n",this->Flips[0],this->Flips[1],this->Flips[2]);

  vtkMatrix4x4* local=vtkMatrix4x4::New();
  
  // Process the transformation if it is not null
  // If linear then this->GlobalMatrix is created else it is set to NULL
  //    and we compute this->DisplacementField
  if (this->Transformation!=NULL)
    this->ProcessInputTransformation(input);


  if (this->GlobalMatrix==NULL)
    {
      // Set the defaults for fitting in the case of a 
      // displacement field
      if (this->Use7Points)
	{
	  this->TempPoints1->SetNumberOfPoints(7);
	  this->TempPoints2->SetNumberOfPoints(7);
	}
      else
	{
	  this->TempPoints1->SetNumberOfPoints(27);
	  this->TempPoints2->SetNumberOfPoints(27);
	}
      this->TempLandmarkTransform->SetSourceLandmarks(this->TempPoints1);
      this->TempLandmarkTransform->SetTargetLandmarks(this->TempPoints2);
      this->TempLandmarkTransform->SetModeToRigidBody();
    }
  else
    {
      // Store local which is the matrix to be computed at each voxel
      // in the case of a displacement field once and for all
      // if the transformation is linear
      local->DeepCopy(this->GlobalMatrix);
    }
      
  int dim[3]; input->GetDimensions(dim);
  int istensor=0; 
  if (input->GetNumberOfScalarComponents()==6)
    istensor=1;

  this->UpdateProgress(0.01);

  // Zero out output image
  vtkDataArray* out=output->GetPointData()->GetScalars();
  for (int ia=0;ia<out->GetNumberOfComponents();ia++)
    out->FillComponent(ia,0.0);

  double origin[3]; input->GetOrigin(origin);
  double spacing[3]; input->GetSpacing(spacing);

  // For each voxel
  for (int k=1;k<dim[2]-1;k++)
    {
      for (int j=1;j<dim[1]-1;j++)
	for (int i=1;i<dim[0]-1;i++)
	  {
	    if (this->GlobalMatrix==NULL)
	      this->ComputeLocalRotation(this->DisplacementField,i,j,k,origin,spacing,local);
	    
	    if (istensor)
	      this->WarpTensor(input,output,i,j,k,local);
	    else
	      this->WarpVector(input,output,i,j,k,local);
	  }
      this->UpdateProgress(double(k+1)/double(dim[2]));
    }
	      
  local->Delete();
}

// --------------------------------------------------------------------------------------------
void vtkpxImageReorientTensorOrVector::WarpTensor(vtkImageData* input,vtkImageData* output,int i,int j,int k,vtkMatrix4x4* matrix)
{
  //
  //  Multiplcation is R^t * H * R  (R is the inverse of usual matrix!)
  //
  //  TempMatrix 1 = R^t
  //  matrix = R
  // Image Data is H

  int doprint=0; 
  if (i==64 && j==69 && k==13 && this->Debug)
    {
      fprintf(stderr,"i=%d j=%d k=%d\n",i,j,k);
      doprint=1;
    }


  if (doprint)
    r_print_mat44(matrix,"Input matrix");

  this->TempMatrix1->Invert(matrix,this->TempMatrix1);

  if (doprint)
    r_print_mat44(this->TempMatrix1,"matrix^-1");


  this->TempMatrix2->Identity();
  this->TempMatrix2->SetElement(0,0,input->GetScalarComponentAsDouble(i,j,k,0));
  
  double tmp;

  tmp=input->GetScalarComponentAsDouble(i,j,k,1)*this->Flips[0]*this->Flips[1];
  this->TempMatrix2->SetElement(0,1,tmp);   this->TempMatrix2->SetElement(1,0,tmp);

  tmp=input->GetScalarComponentAsDouble(i,j,k,2)*this->Flips[0]*this->Flips[2];
  this->TempMatrix2->SetElement(0,2,tmp);   this->TempMatrix2->SetElement(2,0,tmp);

  tmp=input->GetScalarComponentAsDouble(i,j,k,3);  this->TempMatrix2->SetElement(1,1,tmp);  

  tmp=input->GetScalarComponentAsDouble(i,j,k,4)*this->Flips[1]*this->Flips[2];
  this->TempMatrix2->SetElement(1,2,tmp);   this->TempMatrix2->SetElement(2,1,tmp);

  tmp=input->GetScalarComponentAsDouble(i,j,k,5);  this->TempMatrix2->SetElement(2,2,tmp);  


  if (doprint)
    r_print_mat44(this->TempMatrix2,"Input Tensor");


  vtkMatrix4x4::Multiply4x4(this->TempMatrix2,this->TempMatrix1,this->TempMatrix3);
  vtkMatrix4x4::Multiply4x4(matrix,this->TempMatrix3,this->TempMatrix2);

  if (doprint)
    r_print_mat44(this->TempMatrix2,"Output Tensor");

  output->SetScalarComponentFromDouble(i,j,k,0,this->TempMatrix2->GetElement(0,0));
  output->SetScalarComponentFromDouble(i,j,k,1,this->TempMatrix2->GetElement(0,1));
  output->SetScalarComponentFromDouble(i,j,k,2,this->TempMatrix2->GetElement(0,2));
  output->SetScalarComponentFromDouble(i,j,k,3,this->TempMatrix2->GetElement(1,1));
  output->SetScalarComponentFromDouble(i,j,k,4,this->TempMatrix2->GetElement(1,2));
  output->SetScalarComponentFromDouble(i,j,k,5,this->TempMatrix2->GetElement(2,2));
}

// --------------------------------------------------------------------------------------------
void vtkpxImageReorientTensorOrVector::WarpVector(vtkImageData* inp,vtkImageData* output,int i,int j,int k,vtkMatrix4x4* matrix)
{
  double vec[4],vecout[4]; vec[3]=1.0;
  for (int ia=0;ia<=2;ia++)
    vec[ia]=inp->GetScalarComponentAsDouble(i,j,k,ia);

  matrix->MultiplyPoint(vec,vecout);

  if (output->GetScalarType()!=VTK_UNSIGNED_CHAR)
    {
      for (int ia=0;ia<=2;ia++)
	output->SetScalarComponentFromDouble(i,j,k,ia,vecout[ia]);
    }
  else
    {
      for (int ia=0;ia<=2;ia++)
	output->SetScalarComponentFromDouble(i,j,k,ia,fabs(vecout[ia]));
    }
}
// --------------------------------------------------------------------------------------------
void vtkpxImageReorientTensorOrVector::ProcessInputTransformation(vtkImageData* input)
{
  vtkDebugMacro(<<"Processing Input Transformation\n");

  if (this->Transformation->IsA("vtkHomogeneousTransform")==0)
    {
      vtkDebugMacro(<<"Computing Displacement Field");
      vtkTransformToGrid* tgr=vtkTransformToGrid::New();
      tgr->SetGridOrigin(input->GetOrigin());
      tgr->SetGridSpacing(input->GetSpacing());
      tgr->SetGridExtent(input->GetExtent());
      tgr->SetGridScalarTypeToFloat();
      tgr->SetInput(this->Transformation);
      tgr->Update();
      
      vtkDebugMacro(<<"Done Computing\n");

      if (this->DisplacementField!=NULL)
	this->DisplacementField->Delete();
      
      this->DisplacementField=vtkImageData::New();
      this->DisplacementField->ShallowCopy(tgr->GetOutput());
      tgr->Delete();

      vtkDebugMacro(<<"Copied Field\n");

      if (this->GlobalMatrix!=NULL)
	this->GlobalMatrix->Delete();
      this->GlobalMatrix=NULL;

      vtkDebugMacro(<<"Cleanup Global Matrix\n");
      return;
    }

  vtkDebugMacro(<<"Fiting Global Rigid Transformation");

  vtkPoints* tmp1=vtkPoints::New();
  vtkPoints* tmp2=vtkPoints::New();

  tmp1->SetNumberOfPoints(8);
  tmp2->SetNumberOfPoints(8);

  tmp1->SetPoint(0,-5.0,-5.0,-5.0);
  tmp1->SetPoint(1, 5.0,-5.0,-5.0);
  tmp1->SetPoint(2,-5.0, 5.0,-5.0);
  tmp1->SetPoint(3, 5.0, 5.0,-5.0);
  tmp1->SetPoint(4,-5.0,-5.0, 5.0);
  tmp1->SetPoint(5, 5.0,-5.0, 5.0);
  tmp1->SetPoint(6,-5.0, 5.0, 5.0);
  tmp1->SetPoint(7, 5.0, 5.0, 5.0);
  
  vtkDebugMacro(<<"On To Transform Points\n");

  for (int i=0;i<=7;i++)
    {
      double x[3];       tmp1->GetPoint(i,x);
      if (this->Debug)
	fprintf(stderr,"Mapping (%.2f,%.2f,%.2f) \t ",x[0],x[1],x[2]);

      this->Transformation->TransformPoint(x,x);
      if (this->Debug)
	fprintf(stderr,"to (%.2f,%.2f,%.2f) \n ",x[0],x[1],x[2]);
      tmp2->SetPoint(i,x);
    }

  if (this->Debug)
    fprintf(stderr,"Number Of Points %d vs %d\n",
	    tmp1->GetNumberOfPoints(),
	    tmp2->GetNumberOfPoints());

  vtkLandmarkTransform* templan=vtkLandmarkTransform::New();
  templan->DebugOn();
  templan->SetSourceLandmarks(tmp1);
  templan->SetTargetLandmarks(tmp2);
  templan->SetModeToRigidBody();
  templan->Modified();
  templan->Update();
  templan->DebugOff();
  this->GlobalMatrix=vtkMatrix4x4::New();
  templan->GetMatrix(this->GlobalMatrix);
  templan->Delete();
  tmp1->Delete();
  tmp2->Delete();

  for (int ia=0;ia<=2;ia++)
    this->GlobalMatrix->SetElement(ia,3,0.0);


  r_print_mat44(this->GlobalMatrix,"global matrix");
  
}

// --------------------------------------------------------------------------------------------
void vtkpxImageReorientTensorOrVector::ComputeLocalRotation(vtkImageData* dispfield,int i,int j,int k,
							    double origin[3],double spacing[3],vtkMatrix4x4* matrix)
{
  
  /*  const int offset[7][3] = { 
    { 0,0,0 }, 
    { -1,0,0},{1,0,0},
    {0,-1,0}, {0,1,0},
    { 0,0,-1},{0,0,1}};*/

  int maxpoints=27;
  if (this->Use7Points)
    maxpoints=7;

  const double offset[27][3] = { 
    { 0.0,0.0,0.0 }, 
    { -1.0,0.0,0.0},{1.0,0.0,0.0},
    {0.0,-1.0,0.0}, {0.0,1.0,0.0},
    { 0.0,0.0,-1.0},{0.0,0.0,1.0},
    { -1.0,-1.0,-1.0 } , { -1.0 ,0.0 , -1.0 } , { -1.0 , 1.0 -1.0 } , { 0.0,-1.0,-1.0} , 
    { 0.0,1.0,-1.0}, { 1.0,-1.0,-1.0} , { 1.0,0.0,-1.0},{1.0,1.0,-1.0},
    { -1.0,-1.0,0.0}   , { -1.0,1.0,0.0} , { 1.0,-1.0,0.0}, { 1.0,1.0,0.0},
    { -1.0,-1.0,1.0 } , { -1.0 ,0.0 , 1.0 } , { -1.0 , 1.0, 1.0 } , { 0.0,-1.0,1.0} , 
    { 0.0,1.0,1.0}, { 1.0,-1.0,1.0} , { 1.0,0.0,-1.0},{1.0,1.0,1.0}
  };

  int doprint=0; 
  if (i==14 && j==15 && k==14 && this->Debug)
    {
      fprintf(stderr,"i=%d j=%d k=%d\n",i,j,k);
      doprint=1;
    }


  int ijk[3];  ijk[0]=i;  ijk[1]=j;  ijk[2]=k;

  double x[3],y[3],d[3];
  for (int ia=0;ia<maxpoints;ia++)
    {
      for (int ib=0;ib<=2;ib++)
	{
	  //	  x[ib]=origin[ib]+double(ijk[ib]+offset[ia][ib])*spacing[ib];
	  x[ib]=offset[ia][ib]*spacing[ib];
	  d[ib]=dispfield->GetScalarComponentAsDouble(i+(int)offset[ia][0],j+(int)offset[ia][1],k+(int)offset[ia][2],ib);
	  y[ib]=x[ib]+d[ib];
	}
      this->TempPoints1->SetPoint(ia,x);
      this->TempPoints2->SetPoint(ia,y);
      if (doprint)
	fprintf(stderr,"point %d (%.2f,%.2f,%.2f) + (%.2f,%.2f,%.2f) --> (%.2f,%.2f,%.2f)\n",
		ia,x[0],x[1],x[2],d[0],d[1],d[2],y[0],y[1],y[2]);
    }
  this->TempLandmarkTransform->Modified();
  this->TempLandmarkTransform->Update();
  this->TempLandmarkTransform->GetMatrix(matrix);
  for (int ia=0;ia<=2;ia++)
    matrix->SetElement(ia,3,0.0);
  
  if (doprint)
    r_print_mat44(matrix,"Fitted matrix");
}

