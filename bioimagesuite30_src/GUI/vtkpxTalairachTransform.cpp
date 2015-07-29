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
  Module:    $RCSfile: vtkpxTalairachTransform.cpp,v $
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
#include "vtkpxTalairachTransform.h"
#include "vtkpxTalairachTransformHelper.h"
#include "vtkMath.h"
#include "vtkTransform.h"
#include "vtkObjectFactory.h"
#include "vtkpxBaseCurve.h"
#include "vtkTransformFilter.h"
#include "vtkPolyData.h"
#include "vtkPointSet.h"
#include "vtkCellArray.h"
#include "vtkDataArray.h"
#include "vtkPoints.h"
#include "vtkUnsignedCharArray.h"
#include "vtkImageData.h"
#include "vtkbisImageReslice.h"
#include "vtkImageFlip.h"
#include "vtkpxAnalyzeImageSource.h"
//----------------------------------------------------------------------------
vtkpxTalairachTransform* vtkpxTalairachTransform::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxTalairachTransform");
  if(ret)
    {
      return (vtkpxTalairachTransform*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxTalairachTransform;
}

//------------------------------------------------------------------------
vtkpxTalairachTransform::vtkpxTalairachTransform()
{
  this->Landmarks=NULL;
  this->InverseTolerance = 0.001;
  this->InverseIterations = 500;
  this->LinearTransform=NULL;
  this->ColinMode=0;
  this->ColinYSize=0;

  // Some Random values for ac,pc, extents !!
  for (int ia=0;ia<=2;ia++)
    {
      minc[ia]=0.0;
      maxc[ia]=1.0;
      pc[ia]=0.3;
      ac[ia]=0.2;
    }

  // X-coordinate 
  RIGHT_AC=69.0;
  AC_LEFT=69.0;
  
  // Y-Coordinate 
  BACK_PC=104.0-24.0;
  PC_AC=24.0;
  AC_FRONT=69.0;
   
  // Z-Coordinate 
  BOTTOM_AC=43.0;
  AC_TOP=74.0;

  this->Offset=6;
  this->AdditionalZOffset=24;
  this->SetStandardModeOn();
  
}

//------------------------------------------------------------------------
vtkpxTalairachTransform::~vtkpxTalairachTransform()
{
  if (this->Landmarks)
    {
      this->Landmarks->Delete();
    }

  if (this->LinearTransform)
    {
      this->LinearTransform->Delete();
    }


}

//------------------------------------------------------------------------
void vtkpxTalairachTransform::SetLandmarks(vtkPoints *source)
{
  if (source==NULL)
    return;

  if (this->Landmarks != NULL)
    this->Landmarks->Delete();

  this->Landmarks=vtkPoints::New();
  this->Landmarks ->DeepCopy(source);
  this->StandardMode=0;
  this->Modified();
}
//------------------------------------------------------------------------
unsigned long vtkpxTalairachTransform::GetMTime()
{
  unsigned long result = this->vtkWarpTransform::GetMTime();
  unsigned long mtime;

  if (this->Landmarks)
    {
      mtime = this->Landmarks->GetMTime(); 
      if (mtime > result)
	{
	  result = mtime;
	}
    }
  return result;
}
//------------------------------------------------------------------------
void vtkpxTalairachTransform::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkWarpTransform::PrintSelf(os,indent);
  
  os << indent << "Landmarks: " << this->Landmarks << "\n";
  if (this->Landmarks)
    {
      this->Landmarks->PrintSelf(os,indent.GetNextIndent());
    }
}

//----------------------------------------------------------------------------
vtkAbstractTransform *vtkpxTalairachTransform::MakeTransform()
{
  return vtkpxTalairachTransform::New(); 
}

//----------------------------------------------------------------------------
void vtkpxTalairachTransform::InternalDeepCopy(vtkAbstractTransform *transform)
{
  vtkpxTalairachTransform *t = (vtkpxTalairachTransform *)transform;

  this->SetInverseTolerance(t->InverseTolerance);
  this->SetInverseIterations(t->InverseIterations);
  this->SetLandmarks(t->Landmarks);

  if (this->InverseFlag != t->InverseFlag)
    {
      this->InverseFlag = t->InverseFlag;
      this->Modified();
    }
}
//------------------------------------------------------------------------
void vtkpxTalairachTransform::InternalUpdate()
{
  if (this->StandardMode==-1 || this->ColinMode==1 || this->ColinMode==-1)
    {
      vtkDebugMacro(<<"Nothing to do, loaded transform\n");
      return;
    }

  if (this->StandardMode==1)
    {
      if (this->LinearTransform==NULL)
	this->LinearTransform=vtkTransform::New();

      vtkMatrix4x4* mat=vtkMatrix4x4::New();
      mat->Identity();
      

      this->LinearTransform->SetMatrix(mat);
      mat->Delete();

      // Define the bounds 
      this->scalex0=1.0;
      this->scalex1=1.0;

      this->scalez0=1.0;
      this->scalez1=1.0;

      this->scaley0=1.0;
      this->scaley1=1.0;
      this->scaley2=1.0;
      return;
    }


  // Non-Standard Mode 
  int i,ia;

  if (this->Landmarks == NULL)
    {
      return;
    }

  if (this->Landmarks->GetNumberOfPoints() < 4 )
    {
      vtkErrorMacro("Update: At least 4 points needed to define talairach coordinate system");
      return;
    }

  //  fprintf(stderr,"Here in Talairach Transform");

  this->StandardMode=0;
  // ----------------------------------- 1. Find Linear Transform ---------------------------

  // First Get the AC and the PC 
  this->Landmarks->GetPoint(0,ac);
  this->Landmarks->GetPoint(1,pc);

  // Now convert Create Rotation to AC/PC Line
  if (this->LinearTransform==NULL)
    this->LinearTransform=vtkTransform::New();
  this->LinearTransform->Identity();

  double x1[3],x2[3],x3[3];

  //  Make ac the origin (0,0,0) by adding a -ac translation 
  this->LinearTransform->Translate(-ac[0],-ac[1],-ac[2]); 

  this->Landmarks->GetPoint(2,x1);
  this->Landmarks->GetPoint(3,x3);
  
  // Find ac pc line and store in x2 to become the y-axis 
  for (ia=0;ia<=2;ia++)
    {
      x2[ia]=ac[ia]-pc[ia];
      x3[ia]=x3[ia]-x1[ia];
    }
  vtkMath::Normalize(x2);
  vtkMath::Normalize(x3);


  // Find the x-axis x1 as the cross-product of x2 and x3
  // Double cross-product to orthogonalize x2 and x3 !!
  vtkMath::Cross(x2,x3,x1);
  vtkMath::Normalize(x1);

  vtkMath::Cross(x1,x2,x3);
  vtkMath::Normalize(x3);

  // These creates a rotation with the AC/PC Line as the y-axis 
  // By Setting the 3x3 Section of mat
  vtkMatrix4x4* mat=vtkMatrix4x4::New();
  mat->Identity();

  for (i=0;i<=2;i++)
    {
      mat->SetElement(0,i,x1[i]);
      mat->SetElement(1,i,x2[i]);
      mat->SetElement(2,i,x3[i]);
    }

  // Combine mat with translation to get complete transform in AC/PC Space 
  this->LinearTransform->PostMultiply();
  this->LinearTransform->Concatenate(mat);

  // ----------------------------------- 2. Find Bounding Box --------------------------------------
  // Find Bounding Box in original coordinate system 
  vtkPolyData* temppoly=vtkPolyData::New();
  temppoly->SetPoints(this->Landmarks);

  vtkTransformFilter* tfilt=vtkTransformFilter::New();
  tfilt->SetTransform(this->LinearTransform);
  tfilt->SetInput(temppoly);
  tfilt->Update();
  
  vtkPointSet* newpoly=tfilt->GetOutput();  
  newpoly->ComputeBounds();
  double bounds[6];

  newpoly->GetBounds(bounds);

  for (i=0;i<=2;i++)
    {
      minc[i]=bounds[i*2];
      maxc[i]=bounds[i*2+1];
    }

  tfilt->Delete();
  temppoly->Delete();

  // Avoid division by zero stuff 
  for (int ib=0;ib<=2;ib++)
    {
      if (maxc[ib]==minc[ib])
	maxc[ib]=minc[ib]+1.0;
    }

  // ----------------------------------- 3. Transform PC  ---------------------------
  // Transform PC into this space   
  double tpc[3];
  for (int id=0;id<=2;id++)
    tpc[id]=pc[id];
  LinearTransform->TransformPoint(tpc,pc);


  // ----------------------------------- 4. Scaling Constants  ---------------------------
  this->SetScalingConstants();
  return;
}
//------------------------------------------------------------------------
void vtkpxTalairachTransform::SetScalingConstants()
{
  // Geared Towards inverse transform;
  //  float scalex0,scalex1,scaley0,scaley1,scaley2,scalez0,scalez1;
  this->scalex0=fabs(minc[0]/RIGHT_AC);
  this->scalex1=fabs(maxc[0]/AC_LEFT);
  this->scalez0=fabs(minc[2]/BOTTOM_AC);
  this->scalez1=fabs(maxc[2]/AC_TOP);

  this->scaley0=fabs((minc[1]-pc[1])/BACK_PC);
  this->scaley1=fabs(pc[1]/PC_AC);
  this->scaley2=fabs(maxc[1]/AC_FRONT);

  return;
}
//------------------------------------------------------------------------
int vtkpxTalairachTransform::GetMNICoordinates(double x[3],double mni[3])
{
  if (this->GetColinMode()!=0 && vtkpxTalairachTransformHelper::ColinLookup!=NULL)
    {
      if (this->ColinYSize==216)
	{
	  // Flip X
	  mni[0]=(180-x[0])-90.0;
	  mni[2]=(x[2]-72.0);

	  // Flip Y
	  if (this->GetColinMode()==-1)
	    {
	      mni[1]=x[1]-127;

	      // Also Flip Right/Left
	      mni[0]=-mni[0];
	    }
	  else
	    {
	      mni[1]=(216-x[1])-127;
	    }

	  // 1 offset !!!
	  mni[1]+=1.0;


	  return 1;
	}
    }
  return 0;
}
template<class T>
static inline void vtkpxTalairachForwardTransformPoint(vtkpxTalairachTransform *self,
						       const T point[3], T output[3],
						       vtkTransform* LinearTransform,
						       const double ac[3],const double pc[3],const double minc[3],const double maxc[3],
						       const double RIGHT_AC,const double AC_LEFT,
						       const double BACK_PC,const double PC_AC,const double AC_FRONT,
						       const double BOTTOM_AC,const double AC_TOP)
{

  if (self->GetColinMode()!=0 && vtkpxTalairachTransformHelper::ColinLookup!=NULL)
    {
      int pt[3];
      for (int i=0;i<=2;i++)
	pt[i]=(int)point[i];
      if (self->GetColinMode()==-1)
	pt[1]=self->GetColinYSize()-pt[1];
      
      if (vtkpxTalairachTransformHelper::ColinLookup->GetScalarType()==VTK_SHORT)
	{
	  //	  fprintf(stderr, "In Short mode=%d, size=%d %d %d %d\n",self->GetColinMode(),self->GetColinYSize(),pt[0],pt[1],pt[2]);
	  for (int ia=0;ia<=2;ia++)
	    output[ia]=vtkpxTalairachTransformHelper::ColinLookup->GetScalarComponentAsDouble(pt[0],pt[1],pt[2],ia)*0.1;
	} 
      else
	{
	  for (int ia=0;ia<=2;ia++)
	    output[ia]=vtkpxTalairachTransformHelper::ColinLookup->GetScalarComponentAsDouble(pt[0],pt[1],pt[2],ia)-128;
	}

      if (self->GetColinMode()==-1)
	output[0]=-output[0];

      return;
    }

  if (self->GetStandardMode()==1)
    {
      for (int i=0;i<=2;i++)
	output[i]=point[i];
    }



  if (LinearTransform==NULL)
    {
      for (int ia=0;ia<=2;ia++)
	output[ia]=point[ia];
      return;
    }

  T tempoutput[3];

  //  fprintf(stderr,"Point = %f %f %f\n",point[0],point[1],point[2]);

  // Output x,y,z now in AC/PC space with ac as origin
  LinearTransform->TransformPoint(point,tempoutput);

  
  //  fprintf(stderr,"Post Linear = %f %f %f\n",tempoutput[0],tempoutput[1],tempoutput[2]);


  // Now we need to scale appropriately
  // First x-coordinate 
  if (tempoutput[0]<0.0)
    tempoutput[0]=-RIGHT_AC*fabs(tempoutput[0]/minc[0]);
  else
    tempoutput[0]= AC_LEFT *fabs(tempoutput[0]/maxc[0]);

  //  fprintf(stderr,"Post X = %f %f %f\n",tempoutput[0],tempoutput[1],tempoutput[2]);

  // Then y-coordinate in three steps
  if (tempoutput[1]<pc[1])
    tempoutput[1]=-1.0*PC_AC-BACK_PC*fabs( (tempoutput[1]-pc[1])/(minc[1]-pc[1]) );
  else if (tempoutput[1]>=pc[1] && tempoutput[1]<=0.0)
    tempoutput[1]=-1.0*PC_AC*fabs( tempoutput[1]/pc[1]);
  else
    tempoutput[1]=AC_FRONT*fabs(tempoutput[1]/maxc[1]);

  //  fprintf(stderr,"Post Y = %f %f %f\n",tempoutput[0],tempoutput[1],tempoutput[2]);

  // Then z-coordinate 
  if (tempoutput[2]<0.0)
    tempoutput[2]=-BOTTOM_AC*fabs(tempoutput[2]/minc[2]);
  else
    tempoutput[2]= AC_TOP*fabs(tempoutput[2]/maxc[2]);
 
  //fprintf(stderr,"Post Z = %f %f %f\n",tempoutput[0],tempoutput[1],tempoutput[2]);

  for (int ia=0;ia<=2;ia++)
    output[ia]=tempoutput[ia];

  //  fprintf(stderr,"\n\n");

}
//------------------------------------------------------------------------

template<class T>
static inline void vtkpxTalairachInverseTransformPoint(const T point[3], T output[3],
						       vtkTransform* LinearTransform,
						       const double PC_AC,const double pc,
						       const double scalex0,const double scalex1,
						       const double scaley0,const double scaley1,const double scaley2,
						       const double scalez0,const double scalez1)
{
  if (LinearTransform==NULL)
    {
      for (int ia=0;ia<=2;ia++)
	output[ia]=point[ia];
      return;
    }

  // Then x-coordinate this is easy 
  if (point[0]<0.0)
    output[0]=point[0]*scalex0;
  else
    output[0]=point[0]*scalex1;

  // Then y-coordinate in three steps
  if (point[1]<-PC_AC)
    output[1]=(point[1]+PC_AC)*scaley0+pc;
  else if (point[1]<=0.0)
    output[1]=point[1]*scaley1;
  else
    output[1]=point[1]*scaley2;

  // Then z-coordinate this is again easy 
  if (point[2]<0.0)
    output[2]= point[2]*scalez0;
  else
    output[2]= point[2]*scalez1;

  // Output x,y,z now in AC/PC space with ac as origin
  (LinearTransform->GetLinearInverse())->TransformPoint(output,output);
}

//------------------------------------------------------------------------
void vtkpxTalairachTransform::ForwardTransformPoint(const double point[3], 
						    double output[3])
{
  if (this->InverseFlag)
    {
      vtkpxTalairachInverseTransformPoint(point,output,this->LinearTransform,PC_AC,pc[1],
					  scalex0,scalex1,scaley0,scaley1,scaley2,scalez0,scalez1);
      return;
    }

  vtkpxTalairachForwardTransformPoint(this,point,output,this->LinearTransform,this->ac,this->pc,this->minc,this->maxc,
				      RIGHT_AC,AC_LEFT,BACK_PC,PC_AC,AC_FRONT,BOTTOM_AC,AC_TOP);

}

void vtkpxTalairachTransform::ForwardTransformPoint(const float point[3], 
						    float output[3])
{
  if (this->InverseFlag)
    {
      vtkpxTalairachInverseTransformPoint(point,output,this->LinearTransform,PC_AC,pc[1],
					  scalex0,scalex1,scaley0,scaley1,scaley2,scalez0,scalez1);
      return;
    }

  vtkpxTalairachForwardTransformPoint(this,point,output,this->LinearTransform,this->ac,this->pc,this->minc,this->maxc,
				      RIGHT_AC,AC_LEFT,BACK_PC,PC_AC,AC_FRONT,BOTTOM_AC,AC_TOP);

}

void vtkpxTalairachTransform::InverseTransformPoint(const float point[3], float output[3])
{
  if (this->InverseFlag)
    {
      vtkpxTalairachInverseTransformPoint(point,output,this->LinearTransform,PC_AC,pc[1],
					  scalex0,scalex1,scaley0,scaley1,scaley2,scalez0,scalez1);
      return;
    }

  vtkpxTalairachForwardTransformPoint(this,point,output,this->LinearTransform,this->ac,this->pc,this->minc,this->maxc,
				      RIGHT_AC,AC_LEFT,BACK_PC,PC_AC,AC_FRONT,BOTTOM_AC,AC_TOP);
}

void vtkpxTalairachTransform::InverseTransformPoint(const double point[3], double output[3])
{
  if (this->InverseFlag)
    {
      vtkpxTalairachInverseTransformPoint(point,output,this->LinearTransform,PC_AC,pc[1],
					  scalex0,scalex1,scaley0,scaley1,scaley2,scalez0,scalez1);
      return;
    }

  vtkpxTalairachForwardTransformPoint(this,point,output,this->LinearTransform,this->ac,this->pc,this->minc,this->maxc,
				      RIGHT_AC,AC_LEFT,BACK_PC,PC_AC,AC_FRONT,BOTTOM_AC,AC_TOP);
}
//------------------------------------------------------------------------
void vtkpxTalairachTransform::ForwardTransformDerivative(const double point[3],
							 double output[3],
							 double derivative[3][3])
{
  if (this->LinearTransform!=NULL)
    {
      for (int ia=0;ia<=2;ia++)
	for (int ib=0;ib<=2;ib++)
	  derivative[ia][ib]=this->LinearTransform->GetMatrix()->GetElement(ia,ib);
    }
  
}

void vtkpxTalairachTransform::ForwardTransformDerivative(const float point[3],
							 float output[3],
							 float derivative[3][3])
{
  if (this->LinearTransform!=NULL)
    {
      for (int ia=0;ia<=2;ia++)
	for (int ib=0;ib<=2;ib++)
	  derivative[ia][ib]=this->LinearTransform->GetMatrix()->GetElement(ia,ib);
    }
}
//------------------------------------------------------------------------
vtkPolyData* vtkpxTalairachTransform::GetTalairachCube(int inverse)
{

  if (inverse)
    this->Inverse();

  vtkPoints* gridpoints=vtkPoints::New();
  int c_dim[3];
  c_dim[0]=9;	  c_dim[1]=12;	  c_dim[2]=13;

  gridpoints->SetNumberOfPoints(c_dim[0]*c_dim[1]*c_dim[2]);

  vtkUnsignedCharArray* scal=vtkUnsignedCharArray::New();
  scal->SetNumberOfComponents(3);
  scal->SetNumberOfTuples(c_dim[0]*c_dim[1]*c_dim[2]);

  float x[3];
  int index=0,i=0;

  for (i=0;i<c_dim[0];i++)
    {
      if (i<4)
	x[0]=-0.25*float(4-i)*this->GetRIGHT_AC();
      else if (i>4)
	x[0]=+0.25*float(i-4)*this->GetAC_LEFT();
      else
	x[0]=0.0;

      for (int j=0;j<c_dim[1];j++)
	{
	  if (j<4)
	    x[1]=-0.25*float(4-j)*this->GetBACK_PC()-this->GetPC_AC();
	  else if (j==4)
	    x[1]=-this->GetPC_AC();
	  else if (j==5)
	    x[1]=-2.0/3.0*this->GetPC_AC();
	  else if (j==6)
	    x[1]=-1.0/3.0*this->GetPC_AC();
	  else if (j==7)
	    x[1]=0.0;
	  else
	    x[1]=0.25*float(j-7)*this->GetAC_FRONT();
	

	  for (int k=0;k<c_dim[2];k++)
	    {
	      if ( k<4)
		x[2]=-0.25*float(4-k)*this->GetBOTTOM_AC();
	      else if (k==4)
		x[2]=0.0;
	      else 
		x[2]=0.125*float(k-4)*this->GetAC_TOP();
	      
	      float c[3];
	      c[0]=0; c[1]=0; c[2]=0;

	      float tx[3];
	      this->TransformPoint(x,tx);

	      if (this->StandardMode==1)
		{
		  tx[1]=(AC_FRONT+this->Offset)-x[1];
		  tx[0]=(AC_LEFT+this->Offset)-x[0];
		}
      	      
	      
	      if (i==4)
		{
		  c[0]=255.0;
		  c[1]=255.0;
		  c[2]=0.0;
		}
	      else
		{
		  c[0]=64-float(i-4)*12.0;

		  if (j==4)
		    {
		      c[0]=128.0;
		      c[1]=128.0;
		      c[2]=128.0;
		    }
		  else if (j==7)
		    {
		      c[0]=255.0;
		      c[1]=255.0;
		      c[2]=255.0;
		    }
		  else if (j==5 || j==6)
		    {
		      c[0]=192.0;
		      c[1]=192.0;
		      c[2]=192.0;
		    }
		  else 
		    {
		      if (j<4)
			c[1]=abs(4-j)*12;
		      else if (j>7)
			c[1]=abs(j-7)*12;
		      
		      if (k==4)
			c[2]=255.0;
		      else
			c[2]=0.0;
		    }
		}
	      gridpoints->SetPoint(index,tx);
	      scal->SetTuple(index,c);
	      index++;
	    }
	}
    }

  if (inverse)
    this->Inverse();

  // Do Lines Now
  vtkCellArray *c_lines     = vtkCellArray::New();
  c_lines->Allocate(3*c_dim[0]*c_dim[1]*c_dim[2],10);
  int c_dim12=c_dim[1]*c_dim[2];
  for (i=0;i<c_dim[0];i++)
    for (int j=0;j<c_dim[1];j++)
      for (int k=0;k<c_dim[2];k++)
	{
	  int index=k+j*c_dim[2]+i*c_dim12;

	  if (k<c_dim[2]-1)
	    {
	      c_lines->InsertNextCell(2);
	      c_lines->InsertCellPoint(index);
	      c_lines->InsertCellPoint(index+1);
	    }

	  if (j<c_dim[1]-1)
	    {
	      c_lines->InsertNextCell(2);
	      c_lines->InsertCellPoint(index);
	      c_lines->InsertCellPoint(index+c_dim[2]);
	    }

	  if (i<c_dim[0]-1)
	    {
	      c_lines->InsertNextCell(2);
	      c_lines->InsertCellPoint(index);
	      c_lines->InsertCellPoint(index+c_dim12);
	    }
	}
  
	  
  //vtkStructuredGrid* sgrid=vtkStructuredGrid::New();
  //  sgrid->SetDimensions(c_dim[2],c_dim[1],c_dim[0]);
  //sgrid->SetPoints(gridpoints);
  //sgrid->GetPointData()->SetScalars(scal);

  vtkPolyData* sgrid=vtkPolyData::New();
  sgrid->SetPoints(gridpoints);
  sgrid->SetLines(c_lines);
  sgrid->GetPointData()->SetScalars(scal);
  

  scal->Delete();
  gridpoints->Delete();
  c_lines->Delete();
  return sgrid;
}
//------------------------------------------------------------------------
void vtkpxTalairachTransform::SetStandardModeOn()
{
  this->ColinMode=0;
  this->StandardMode=1;
  this->Modified();
}

int vtkpxTalairachTransform::Load(const char* fname)
{
  char line[200];
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
      {
	vtkErrorMacro(<<"Failed to Read Talairach Transform from "<< fname << ". Bad filename.");
	return 0;
      }
  
  gzgets(fin,line,100);
  // fprintf(stderr,"line=%s\n",line);
  if (gzfindstringinstring(line,"#Talairach Transform")==0)
    {
      vtkErrorMacro(<<"Failed to Read Talairach Transform from "<< fname << ". Bad Header.");
      gzclose(fin);
      return 0;
    }

  gzgets(fin,line,100); // Linear Transform Header
  if (this->LinearTransform==NULL)
    this->LinearTransform=vtkTransform::New();
  this->LinearTransform->Identity();
  
  vtkMatrix4x4* matrix=vtkMatrix4x4::New();
  matrix->Identity();
  float tmp[4];
  for (int i=0;i<=3;i++)
    {
      gzgets(fin,line,100); 
      // fprintf(stderr,"line=%s\n",line);
      sscanf(line,"%f %f %f %f",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
      for (int j=0;j<=3;j++)
	{
	  //	  fprintf(stderr,"Setting Elment %d, %d to %.1f\n",i,j,tmp[j]);
	  matrix->SetElement(i,j,tmp[j]);
	}
    }
      
  this->LinearTransform->Identity();
  this->LinearTransform->PostMultiply();
  this->LinearTransform->Concatenate(matrix);
  matrix->Delete();
  
      
  gzgets(fin,line,100);  // AC Header
  gzgets(fin,line,100); 
  // fprintf(stderr,"line=%s\n",line);
  sscanf(line,"%lf %lf %lf",&ac[0],&ac[1],&ac[2]);
  // fprintf(stderr,"Setting Ac %.1f %.1f %.1f\n",ac[0],ac[1],ac[2]);
  
  gzgets(fin,line,100);  // PC Header
  gzgets(fin,line,100); 
  // fprintf(stderr,"line=%s\n",line);
  sscanf(line,"%lf %lf %lf",&pc[0],&pc[1],&pc[2]);
  // fprintf(stderr,"Setting Pc %.1f %.1f %.1f\n",pc[0],pc[1],pc[2]);
  
  gzgets(fin,line,100);  // MINC Header
  gzgets(fin,line,100); 
  // fprintf(stderr,"line=%s\n",line);
  sscanf(line,"%lf %lf %lf",&minc[0],&minc[1],&minc[2]);
  // fprintf(stderr,"Setting Minc %.1f %.1f %.1f\n",minc[0],minc[1],minc[2]);
  
  gzgets(fin,line,100);  // MAXC Header
  gzgets(fin,line,100); 
  // fprintf(stderr,"line=%s\n",line);
  sscanf(line,"%lf %lf %lf",&maxc[0],&maxc[1],&maxc[2]);
  // fprintf(stderr,"Setting Maxc %.1f %.1f %.1f\n",maxc[0],maxc[1],maxc[2]);
  
  gzgets(fin,line,100);  // RIGHT_AC,LEFT_AC Header
  gzgets(fin,line,100); 
  // fprintf(stderr,"line=%s\n",line);
  sscanf(line,"%lf %lf", &RIGHT_AC, &AC_LEFT);
  
  gzgets(fin,line,100);  // BACK_PC,PC_AC,AC_FRONT Header
  gzgets(fin,line,100); 
  //fprintf(stderr,"line=%s\n",line);
  sscanf(line,"%lf %lf %lf",&BACK_PC,&PC_AC,&AC_FRONT);
  
  gzgets(fin,line,100);  // BOTTOM_AC,LEFT_AC Header
  gzgets(fin,line,100); 
  // fprintf(stderr,"line=%s\n",line);
  sscanf(line,"%lf %lf", &BOTTOM_AC, &AC_TOP);
  
  gzclose(fin);
  
  this->ColinMode=0;
  this->StandardMode=-1;
  this->SetScalingConstants();
  this->Modified();
  return(1);
}

int vtkpxTalairachTransform::Save(const char* fname)
{
  if (this->LinearTransform==NULL)
    {
      vtkErrorMacro(<<"Cannot Save Talairach Transform, no linear transform");
      return 0;
    }

  FILE* fout=fopen(fname,"w");
  if (!fout)
    {
      vtkErrorMacro(<<"Cannot Save Talairach Transform, bad filename " << fname);
      return 0;
    }

  fprintf(fout,"#Talairach Transform\n");

  fprintf(fout,"#Linear Transform\n");
  vtkMatrix4x4* matrix=this->LinearTransform->GetMatrix();
  for (int i=0;i<=3;i++)
    {
      for (int j=0;j<=3;j++)
	fprintf(fout,"%7.3f ",matrix->GetElement(i,j));
      fprintf(fout,"\n");
    }
  fprintf(fout,"#AC\n");
  fprintf(fout,"%.2f %.2f %.2f\n",ac[0],ac[1],ac[2]);
  fprintf(fout,"#PC\n");
  fprintf(fout,"%.2f %.2f %.2f\n",pc[0],pc[1],pc[2]);
  fprintf(fout,"#Minc\n");
  fprintf(fout,"%.2f %.2f %.2f\n",minc[0],minc[1],minc[2]);
  fprintf(fout,"#Maxc\n");
  fprintf(fout,"%.2f %.2f %.2f\n",maxc[0],maxc[1],maxc[2]);
  fprintf(fout,"#RIGHT_AC,LEFT_AC\n%.1f %.1f\n", RIGHT_AC, AC_LEFT);
  fprintf(fout,"#BACK_PC,PC_AC,AC_FRONT\n%.1f %.1f %.1f\n",BACK_PC,PC_AC,AC_FRONT);
  fprintf(fout,"#BOTTOM_AC, AC_TOP\n%.1f %.1f\n",BOTTOM_AC,AC_TOP);

  fclose(fout);
  return 1;
}
//------------------------------------------------------------------------
//   Talairach Reslice Image 
//------------------------------------------------------------------------
vtkImageData* vtkpxTalairachTransform::ResliceImage(vtkImageData* image,int linearOnly)
{
  return ResliceImage(image,linearOnly,NULL);
}

vtkImageData* vtkpxTalairachTransform::ResliceImage(vtkImageData* image,int linearOnly,vtkbisImageReslice* resl)
{
  if (image==NULL)
    {
      vtkErrorMacro(<<"NULL Input Cannot Reslice");
      return NULL;
    }

  if (LinearTransform==NULL)
    {
      vtkErrorMacro(<<"No Transform Cannot Reslice");
      return NULL;
    }

  vtkbisImageReslice* reslice;

  if (resl!=NULL)
    {
      reslice=resl;
    }
  else
    {
      reslice=vtkbisImageReslice::New();
      reslice->SetInterpolationModeToLinear();
      reslice->SetBackgroundLevel(10.0) ;
      reslice->OptimizationOff();
    }
  
  reslice->SetInput(image);

  int dim[3];
  dim[0]=this->Offset*2+int(RIGHT_AC+AC_LEFT)+1;
  dim[1]=this->Offset*2+int(BACK_PC+PC_AC+AC_FRONT);
  dim[2]=this->Offset*2+int(BOTTOM_AC+AC_TOP)+this->AdditionalZOffset;
   
  double ori[3];
  ori[0]=-(float(this->Offset)+RIGHT_AC);
  ori[1]=-(float(this->Offset)+BACK_PC+PC_AC);
  ori[2]=-(float(this->Offset+this->AdditionalZOffset)+BOTTOM_AC);
  
  if (linearOnly)
    {
      vtkTransform *tr=vtkTransform::New();
      tr->PostMultiply();
      tr->Identity();
      tr->Translate(-ac[0],-ac[1],-ac[2]);
      tr->Concatenate(this->LinearTransform->GetLinearInverse());
      
      reslice->SetResliceTransform(tr);
      tr->Delete();
      reslice->SetInformationInput(image);
    }
  else
    {
      reslice->SetOutputOrigin(ori);
      reslice->SetOutputExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
      reslice->SetOutputSpacing(1.0,1.0,1.0);
      reslice->SetResliceTransform(this);
      this->InverseFlag=1;
    }



  vtkImageFlip* flip=vtkImageFlip::New();
  flip->SetInput(reslice->GetOutput());
  flip->SetFilteredAxis(1);
  flip->Update();

  this->InverseFlag=0;

  vtkImageData* output=vtkImageData::New();
  output->ShallowCopy(flip->GetOutput());
  output->SetOrigin(0,0,0);
  
  flip->Delete();

  if (resl==NULL)
    reslice->Delete();
  return output;

}

//------------------------------------------------------------------------
void vtkpxTalairachTransform::SetColinMode(int colinmode,vtkImageData* img)
{
  int debug=0;
  if(debug>0) 
    fprintf(stderr,"Setting Colin Mode=%d\n",colinmode);

  int ok=0;
  int badimage=0;
  int np=0;
  if (img==NULL)
    {
      badimage=1;
    }
  else
    {
      np=img->GetNumberOfPoints();
      if (np!=5903040 && np!=7109137 && np!=902629)
	badimage=1;
    }
  
  if (badimage==1)
    {
      if(debug>0)      fprintf(stderr,"Not the Colin Brain\n");
      this->ColinMode=0;
      this->SetStandardModeOn();
      this->Modified();
      return;
    }

  if(debug>0) 
    fprintf(stderr,"Still Setting Colin Mode=%d np=%d\n",colinmode,np);

  if (colinmode!=0)
    {
      if(debug>0)
	fprintf(stderr,"Loading Colin %d\n",np);
      ok=vtkpxTalairachTransformHelper::LoadColinLookup(np);
    }
  
  if(debug>0)
    fprintf(stderr,"Ok=%d\n",ok);
  
  if (ok==0)
    {
      if(debug>0)
	fprintf(stderr,"Setting Colin Mode to 0\n");
      this->ColinMode=0;
      this->SetStandardModeOn();
      this->Modified();
      return;
    }
  else if (colinmode<0)
    {
      this->SetMontrealMode(0);
      this->ColinMode=-1;
      if(debug>0)
	fprintf(stderr,"Setting Colin Mode to -1\n");
    }
  else if (colinmode>0)
    {
      this->SetMontrealMode(1);
      this->ColinMode=1;
      if(debug>0)
	fprintf(stderr,"Setting Colin Mode to 1\n");
    }

  if(debug>0)
    fprintf(stderr,"before talairach helper\n");

  int d[3]; vtkpxTalairachTransformHelper::ColinLookup->GetDimensions(d);

  if(debug>0)
    fprintf(stderr,"after talairach helper\n");

  this->ColinYSize=d[1]-1;

  if(debug>0)
    fprintf(stderr,"Final Colin Mode=%d\n",this->ColinMode);
  this->Modified();
  this->Update();
}

void vtkpxTalairachTransform::SetMontrealMode(int yaxisfronttoback)
{
  static double pts[10][3]= { 
    {  87.00, 91.00, 66.00 },
    {  87.00,121.00, 66.00 },
    {  87.00,121.00, 66.00 },
    {  87.00,121.00,139.00 },
    {  97.00,131.00,155.00 },
    { 103.00, 20.00, 74.00 },
    {  76.00,199.00, 67.00 },
    { 158.00,120.00, 57.00 },
    {  14.00,124.00, 71.00 },
    {  58.00,100.00, 17.00 }};
  /*
    { 87.00,  91.00,  66.00},
    { 87.00, 121.00,  66.00},
    { 87.00, 121.00,  66.00},
    { 87.00, 121.00, 139.00},
    { 97.00, 131.00, 153.00},
    {103.00,  20.00,  74.00},
    { 76.00, 199.00,  67.00},
    {158.00, 120.00,  57.00},
    { 14.00, 124.00,  71.00},
    { 58.00,  98.00,  21.00}};*/


  vtkPoints* points=vtkPoints::New();
  points->SetNumberOfPoints(10);
  for (int ia=0;ia<10;ia++)
    {
      if (yaxisfronttoback)
	points->SetPoint(ia,pts[ia][0],pts[ia][1],pts[ia][2]);
      else
	points->SetPoint(ia,pts[ia][0],219.0-pts[ia][1],pts[ia][2]);
    }

  this->SetLandmarks(points);
  points->Delete();
  this->ColinMode=0;
  return;
	
  /*  
  if (this->LinearTransform==NULL)
    this->LinearTransform=vtkTransform::New();
  this->LinearTransform->Identity();
  
  vtkMatrix4x4* matrix=vtkMatrix4x4::New();
  matrix->Identity();


    {
      matrix->SetElement(0,0,-1);
      matrix->SetElement(1,1,-1);
      matrix->SetElement(0,3,87);
      matrix->SetElement(1,3,90);
      matrix->SetElement(2,3,-64);
      
      this->LinearTransform->Identity();
      this->LinearTransform->PostMultiply();
      this->LinearTransform->Concatenate(matrix);
      matrix->Delete();
      
      //#AC
      ac[0]=87.00; ac[1]=90.00; ac[2]=64.00;
      //#PC
      pc[0]=0.00; pc[1]= -32.0; pc[2]=0.00;
      //#Minc
      minc[0]=-72.0; minc[1]=-110.0; minc[2]=-44.0;
      //#Maxc
      maxc[0]=75.00; maxc[1]=71.00; maxc[2]=90.0;
    } 
  else
    {
      matrix->SetElement(0,3,-87.0);
      matrix->SetElement(1,3,-129.0);
      matrix->SetElement(2,3,-64);
      
      this->LinearTransform->Identity();
      this->LinearTransform->PostMultiply();
      this->LinearTransform->Concatenate(matrix);
      matrix->Delete();
      
      //#AC
      ac[0]=87.00; ac[1]=129.00; ac[2]=64.00;
      //#PC
      pc[0]=0.00; pc[1]= -32.0; pc[2]=0.00;
      //#Minc
      minc[0]=-75.0; minc[1]=-110.0; minc[2]=-44.0;
      //#Maxc
      maxc[0]=72.00; maxc[1]=71.00; maxc[2]=90.0;
    }
    
  //#RIGHT_AC,LEFT_AC
  RIGHT_AC=69.0; AC_LEFT=69.0;
  //#BACK_PC,PC_AC,AC_FRONT
  BACK_PC=80.0; PC_AC=24.0; AC_FRONT=69.0;
  //#BOTTOM_AC, AC_TOP
  BOTTOM_AC=43.0; AC_TOP=74.0;
  
  this->StandardMode=-1;
  this->SetScalingConstants();5
  this->Modified();
  return;*/
}

int vtkpxTalairachTransform::SetColinLookupFileName(char* fname,int mode)
{
  //  fprintf(stderr,"Setting Filename from vtkpxTalairachTransform %s (mode=%d)\n",fname,mode);
  return vtkpxTalairachTransformHelper::SetColinLookupFileName(fname,mode);
}


vtkImageData* vtkpxTalairachTransform::GenerateLookupTable(vtkImageData* img,vtkAbstractTransform* tr)
{
  if (img==NULL)
    {
      vtkErrorMacro(<<"Bad Input\n");
      return NULL;
    }

  vtkImageData* out=vtkImageData::New();
  out->CopyStructure(img);
  out->SetScalarTypeToFloat();
  out->SetNumberOfScalarComponents(3);
  out->AllocateScalars();

  vtkDataArray* dat=out->GetPointData()->GetScalars();
  int np=dat->GetNumberOfTuples();
  for (int i=0;i<np;i++)
    {
      double x[3];
      img->GetPoint(i,x);
      if (tr!=NULL)
	tr->TransformPoint(x,x);
      this->TransformPoint(x,x);

      double x2[3];
      for (int ia=0;ia<=2;ia++)
	{
	  //	  int a=int(x[ia]+0.5);
	  x2[ia]=x[ia]*10.0;//a+128;
	}
      dat->SetTuple(i,x2);
    }

  return out;
}

int vtkpxTalairachTransform::ReorientCoordinates(vtkImageData* img, int inputorientation,double x[3],double y[3])
{
  if ( (inputorientation!=1 && inputorientation!=2) || (img == NULL))
    {
      for (int ia=0;ia<=2;ia++)
	y[ia]=x[ia];
      return 0;
    }


  int dim[3]; img->GetDimensions(dim);
  double spa[3]; img->GetSpacing(spa);
  double ori[3]; img->GetOrigin(ori);
  

  if (inputorientation==1)
    {
      // x->x
      y[0]=x[0];
      // z->y
      y[1]=x[2];
      // -y->z
      y[2]=x[1]-ori[1];
      y[2]=(double(dim[1]-1.0)*spa[2])-y[2];
      y[2]+=ori[1];

      // z->y

      return 1;
    }

  // Sagital
  //     mat->SetElement(0, 0,  0) ;  mat->SetElement(0, 1, 1) ;  mat->SetElement(0, 2, 0);
  y[1]=x[0];
  //      mat->SetElement(1, 0,  0) ;  mat->SetElement(1, 1, 0) ;  mat->SetElement(1, 2, 1);
  y[2]=x[1];
  //      mat->SetElement(2, 0, -1) ;  mat->SetElement(2, 1, 0) ;  mat->SetElement(2, 2, 0);
  y[0]=x[2]-ori[2];
  y[0]=(double(dim[2]-1.0)*spa[2])-y[0];
  y[0]+=ori[2];
    
  return 1;

}

