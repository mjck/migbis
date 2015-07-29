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

// ------------------------------------------------------------------------
//
// This file derives from original work
// in the vtkCISG Toolkit
// that was released under the terms of 
// the GNU General Public License (either version 2,
// of the License, or any later version.)
// 
// The code has been modified for inclusion in BioImage Suite
//
// It may consist of work derived from more than one source
// file in the vtkCISG Toolkit. The original copyright notice
// was:
//
//  Authors   : Thomas Hartkens <thomas@hartkens.de>
//              Daniel Rueckert <d.rueckert@doc.ic.ac.uk>
//	        Julia Schnabel  <julia.schnabel@kcl.ac.uk>
//	        under the supervision of Dave Hawkes and Derek Hill
//  Web       : http://www.image-registration.com
//  Copyright : King's College London
//              Div. of Radiological Sciences, 
//              Computational Imaging Science Group, 1997 - 2000
//	        http://www-ipg.umds.ac.uk/cisg
//
// ------------------------------------------------------------------------



#include <stdlib.h>
#include "vtkpxLinearTransform.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "pxutil.h"
#include "vtkImageData.h"
#include "pxutil.h"
#include "vtkpxUtil.h"
#include "vtkLandmarkTransform.h"
#include "vtkPoints.h"
#include "vtkLinearTransform.h"
//----------------------------------------------------------------------------
vtkpxLinearTransform* vtkpxLinearTransform::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxLinearTransform");
  if(ret)
    {
    return (vtkpxLinearTransform*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxLinearTransform;
}

//----------------------------------------------------------------------------
vtkpxLinearTransform::vtkpxLinearTransform()
{
  /// Scaling along the x-axis, y-axis, z-axis
  _sx=100.0;
  _sy=100.0;
  _sz=100.0;
  /// Skew angle in the x-y ...  plane (in degrees)
  _sxy=0.0;
  _syx=0.0;
  _syz=0.0;
  _szy=0.0;
  _szx=0.0;
  _sxz=0.0;
  /// Translation along the x-axis (in mm)
  _tx=0.0;
  _ty=0.0;
  _tz=0.0;
  /// Rotation around the x-axis (in degrees)
  _rx=0.0;
  _ry=0.0;
  _rz=0.0;

  FlipX=0;
  FlipY=0;
  FlipZ=0;

  this->DistortionMode=0;
  this->FreezeUpdates=0;
}

//----------------------------------------------------------------------------
vtkpxLinearTransform::~vtkpxLinearTransform()
{
}

//----------------------------------------------------------------------------
void vtkpxLinearTransform::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "### " << this->GetClassName() << " ### \n";
  os  << indent << "Transformation Parameters: \n";
  os << indent << "Translation: " << _tx << " " << _ty  << " " << _tz << " "  << "\n";
  os << indent << "Rotation: " << _rx  << " " << _ry  << " " << _rz << "\n";
  os << indent << "Scaling: " << _sx  << " " << _sy  << " " << _sz << "\n";
  os << indent << "Shearing 1: " << _sxy  << " " << _syx  << " " << _syz << "\n";
  os << indent << "Shearing 2: " << _szy  << " " << _szx  << " " << _sxz << "\n";
  os << indent << "Flip: " << FlipX << " " << FlipY  << " " << FlipZ << "\n";
  os << indent << "DistotionMode: " << DistortionMode << "\n";
  
  this->vtkTransform::PrintSelf(os, indent.GetNextIndent());
}


//----------------------------------------------------------------------------
void vtkpxLinearTransform::UpdateParameters()
{
  if (this->FreezeUpdates)
    return;

  float pi = vtkMath::Pi();

  /*  float cosrx = cos(_rx*pi/180.0);
      float cosry = cos(_ry*pi/180.0);
      float cosrz = cos(_rz*pi/180.0);
      float sinrx = sin(_rx*pi/180.0);
      float sinry = sin(_ry*pi/180.0);
      float sinrz = sin(_rz*pi/180.0);*/



  // Update affine transformation: Add shearing
  vtkMatrix4x4 *skewx= vtkMatrix4x4::New();
  skewx->Identity();
  skewx->SetElement(2, 1,tan(_szy*(pi/180.0)));
  skewx->SetElement(1, 2,tan(_syz*(pi/180.0))); 
  vtkMatrix4x4 *skewy= vtkMatrix4x4::New();
  skewy->Identity();
  skewy->SetElement(2, 0,tan(_szx*(pi/180.0)));
  skewy->SetElement(0, 2,tan(_sxz*(pi/180.0)));
  vtkMatrix4x4 *skewz= vtkMatrix4x4::New();
  skewz->Identity();
  skewz->SetElement(1, 0,tan(_sxy*(pi/180.0)));  
  skewz->SetElement(0, 1,tan(_syx*(pi/180.0))); 

  float sx=_sx*0.01; if (this->FlipX) sx=-1.0*sx;
  float sy=_sy*0.01; if (this->FlipY) sy=-1.0*sy;
  float sz=_sz*0.01; if (this->FlipZ) sz=-1.0*sz;

  vtkTransform* tr=vtkTransform::New();

  tr->Identity();
  tr->PostMultiply();
  if (this->DistortionMode==0)
    {
      tr->Concatenate(skewx);
      tr->Concatenate(skewy);
      tr->Concatenate(skewz);
      tr->Scale(sx,sy,sz);
      tr->RotateX(_rx);
      tr->RotateY(_ry);
      tr->RotateZ(_rz);
      tr->Translate(_tx,_ty,_tz);
    }
  else
    {
      tr->RotateX(_rx);
      tr->RotateY(_ry);
      tr->RotateZ(_rz);
      tr->Translate(_tx,_ty,_tz);
      tr->Concatenate(skewx);
      tr->Concatenate(skewy);
      tr->Concatenate(skewz);
      tr->Scale(sx,sy,sz);
    }

  skewx->Delete();
  skewy->Delete();
  skewz->Delete();


  vtkTransform::Identity();
  this->Concatenate(tr->GetMatrix());
  tr->Delete();
  this->vtkTransform::InternalUpdate();

}


//----------------------------------------------------------------------------
void vtkpxLinearTransform::GetParameters(vtkDoubleArray *parameters)
{
  this->GetParameters(parameters,0);
}

void vtkpxLinearTransform::SetParameters(vtkDoubleArray *scalars)
{
  this->SetParameters(scalars,0);
}
//----------------------------------------------------------------------------
void vtkpxLinearTransform::GetParameters(vtkDoubleArray *parameters,int frame)
{
  if (parameters->GetNumberOfComponents()>=this->GetNumberOfParameters() && parameters->GetNumberOfTuples()>frame)
    {
      for (int i=0;i<this->GetNumberOfParameters();i++)
	parameters->SetComponent(frame,i,this->Get(i));
    }
}

void vtkpxLinearTransform::SetParameters(vtkDoubleArray *scalars,int frame)
{
  if (scalars->GetNumberOfComponents()<this->GetNumberOfParameters() || frame>=scalars->GetNumberOfTuples())
    {
      fprintf(stderr,"Wrong Input in comp=%d,nump=%d in frame=%d tuples=%d\n",
	      scalars->GetNumberOfComponents(),this->GetNumberOfParameters(),
	      frame,scalars->GetNumberOfTuples());
      return;
    }

  for (int i=0;i<this->GetNumberOfParameters();i++)
    this->Put(i,scalars->GetComponent(frame,i));
  this->UpdateParameters();
}

//----------------------------------------------------------------------------
float vtkpxLinearTransform::Get(int i) const
{
  switch (i) 
    {
    case 0:
      return _tx;
      break;
    case 1:
      return _ty;
      break;
    case 2:
      return _tz;
      break;
    case 3:
      return _rx;
      break;
    case 4:
      return _ry;
      break;
    case 5:
      return _rz;
      break;
    case 6:
      return _sx;
      break;
    case 7:
      return _sy;
      break;
    case 8:
      return _sz;
      break;
    case 9:
      return _sxy; 
      break;
    case 10:
      return _syx;
      break;
    case 11:
      return _syz;
      break;
    case 12:
      return _szy;
      break;
    case 13:
      return _szx;
      break;
    case 14:
      return _sxz;
      break;
    case 15:
      return FlipX;
      break;
    case 16:
      return FlipY;
      break;
    case 17:
      return FlipZ;
      break;
    case 18:
      return DistortionMode;
      break;
    default:
      cerr << "No such dof" << i << endl;
    return 0;
    }
}
//----------------------------------------------------------------------------
void vtkpxLinearTransform::Put(int i, float x)
{
  switch (i) 
    {
    case 0:
      _tx = x;
      break;
    case 1:
      _ty = x;
      break;
    case 2:
      _tz = x;
      break;
    case 3:
      _rx = x;
      break;
    case 4:
      _ry = x;
      break;
    case 5:
      _rz = x;
      break;
    case 6:
      _sx = x;
      if (_sx<1.01)
	_sx=100.0;
      break;
    case 7:
      _sy = x;
      if (_sy<1.01)
	_sy=100.0;
      break;
    case 8:
      _sz = x;
      if (_sz<1.01)
	_sz=100.0;
      break;
    case 9:
      _sxy = x; 
      break;
    case 10:
      _syx = x;
      break;
    case 11:
      _syz = x;
      break;
    case 12:
      _szy = x;
      break;
    case 13:
      _szx = x;
      break;
    case 14:
      _sxz = x;
    break;
    case 15:
      FlipX = ((int) x) %2;
      break;
    case 16:
      FlipY = ((int) x) %2;
      break;
    case 17:
      FlipZ = ((int) x) %2;
      break;
    case 18:
      DistortionMode=int(x>0);
      break;
    default:
      cerr << "Put(): No such dof" << i << endl;
      return;
    }
  // Update transformation matrix
  this->UpdateParameters();
}
//----------------------------------------------------------------------------
void vtkpxLinearTransform::SetFlipX(int f)
{
  this->FlipX=f;
  this->UpdateParameters();
}
//----------------------------------------------------------------------------
void vtkpxLinearTransform::SetFlipY(int f)
{
  this->FlipY=f;
  this->UpdateParameters();
}
//----------------------------------------------------------------------------
void vtkpxLinearTransform::SetFlipZ(int f)
{
  this->FlipZ=f;
  this->UpdateParameters();
}

//----------------------------------------------------------------------------
void vtkpxLinearTransform::SetDistortionMode(int f)
{
  this->DistortionMode=(f>0);
  this->UpdateParameters();
}
//----------------------------------------------------------------------------


void vtkpxLinearTransform::Inverse() 
{
    /// Scaling along the x-axis, y-axis, z-axis
  _sx=10000.0/_sx; _sy=10000.0/_sy; _sz=10000.0/_sz;
  /// Skew angle in the x-y ...  plane (in degrees)
  _sxy=-_sxy;
  _syx=-_syx;
  _syz=-_syz;
  _szy=-_szy;
  _szx=-_szx;
  _sxz=-_sxz;

  /// Translation along the x-axis (in mm)
  _tx *= -1;
  _ty *= -1;
  _tz *= -1;
  /// Rotation around the x-axis (in degrees)
  _rx *= -1;
  _ry *= -1;
  _rz *= -1;

  this->GetMatrix()->Invert();
  this->Update();
  
  this->Concatenation->Inverse();
  this->Modified();
}
//----------------------------------------------------------------------------
int vtkpxLinearTransform::Load(char* fname)
{
  char pxtk_buffer[255];

  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  gzgets(fin,pxtk_buffer,100);
  
  if (gzfindstringinstring(pxtk_buffer,"#vtkpxLinearTransform File")==0)
    {
      gzclose(fin);
      return 0;
    }

  gzgets(fin,pxtk_buffer,100);
  float dx[4];

  vtkMatrix4x4* matrix=this->GetMatrix();
  
  for (int ia=0;ia<=3;ia++)
    {
      gzgets(fin,pxtk_buffer,200);
      sscanf(pxtk_buffer,"%f %f %f %f",&dx[0],&dx[1],&dx[2],&dx[3]);
      for (int j=0;j<=3;j++)
	matrix->SetElement(ia,j,dx[j]);
    }

  gzgets(fin,pxtk_buffer,200); 
  int tmp;
  float v;
  for (int i=0;i<this->GetNumberOfParameters();i++)
    {
      gzgets(fin,pxtk_buffer,200);
      sscanf(pxtk_buffer,"%d %f",&tmp,&v);
      this->Put(i,v);
    }
  gzclose(fin);
  return 1;
}
//----------------------------------------------------------------------------
int vtkpxLinearTransform::Save(char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
    return 0;

  fprintf(fout,"#vtkpxLinearTransform File\n");
  fprintf(fout,"#4x4 Matrix\n");

  vtkMatrix4x4* matrix=this->GetMatrix();

  for(int ia=0;ia<=3;ia++)
    {
      for (int j=0;j<=3;j++)
	fprintf(fout,"%.4f ",matrix->GetElement(ia,j));
      fprintf(fout,"\n");
    }

  fprintf(fout,"#Actual Parameters\n");
  for (int i=0;i<this->GetNumberOfParameters();i++)
    fprintf(fout,"%d %.4f\n",i+1,this->Get(i));
  
  fclose(fout);
  return 1;
}
//----------------------------------------------------------------------------
void vtkpxLinearTransform::Identity()
{
  /// Scaling along the x-axis, y-axis, z-axis
  _sx=100.0;
  _sy=100.0;
  _sz=100.0;
  /// Skew angle in the x-y ...  plane (in degrees)
  _sxy=0.0;
  _syx=0.0;
  _syz=0.0;
  _szy=0.0;
  _szx=0.0;
  _sxz=0.0;
  /// Translation along the x-axis (in mm)
  _tx=0.0;
  _ty=0.0;
  _tz=0.0;
  /// Rotation around the x-axis (in degrees)
  _rx=0.0;
  _ry=0.0;
  _rz=0.0;

  FlipX=0;
  FlipY=0;
  FlipZ=0;
  this->UpdateParameters();
}
//----------------------------------------------------------------------------
void vtkpxLinearTransform::Copy(vtkpxLinearTransform* other)
{
  if (other==NULL)
    return;

  for (int i=0;i<this->GetNumberOfParameters();i++)
    {
      this->Put(i,other->Get(i));
    }
  this->UpdateParameters();
}

//----------------------------------------------------------------------------
void LinPrint(vtkMatrix4x4* matrix,char* name)
{
  fprintf(stdout,"\n Printing %s\n",name);
  for(int i=0;i<=3;i++)
    {
      for (int j=0;j<=3;j++)
	fprintf(stdout,"%7.3f ",matrix->GetElement(i,j));
      fprintf(stdout,"\n");
    }
}


//----------------------------------------------------------------------------
/*

Let C : be centered coordinate frame to centered coordinate frame transformation
Let G : be transformation that changes orientation from reference to transform
Let Z : be actual VTK Transformation 

Let r be the centered coordinate space on the Reference Image
Let t be the centered coordinate space on the Transform Image

Let r' be the non-centered (default) coordinate space on Reference
Let t' be the non-centered (default) coordinate space on Transform


Transformation we are interested in is

     r --> t i.e.   t= Ar
     
What we need for practical application is

     r' --> t' i.e.  t'=Br'

     
Let R,T be shift operations such that : r'=Rr, t'=Tt

If no orientation change then

          
   t=Ar
   T(-1)t'=AR(-1)r'
   t'=[ TAR(-1) ] r'

Hence B=[ TAR(-1) ] 

This would be true if the orientations of B and A where the same. 

If not we need to transform B such that it accounts for the orientation dependence.

Hence we need B' = G*B


B'=G * [ TAR(-1) ] 

TAR(-1) = G(-1)*B'
TA      =  G(-1)*B'*R
A       = T(-1)*G(-1)*B'*R



*/
//----------------------------------------------------------------------------
int vtkpxLinearTransform::ManualSetParameters(float tx,float ty,float tz,
					      float rx,float ry,float rz,
					      float sx,float sy,float sz,
					      vtkImageData* reference,int ref_orientation,
					      vtkImageData* transform,int trn_orientation,
					      vtkTransform* combined)
{
  
  // This Function Generates the A Matrix Above

  if (reference==NULL || transform==NULL || combined==NULL)
    return 0;

  // Ensure No Individual Updates
  this->FreezeUpdates=1;
  this->Identity();
  this->Put(0,tx);   this->Put(1,ty);   this->Put(2,tz);
  this->Put(3,rx);   this->Put(4,ry);   this->Put(5,rz);
  this->Put(6,sx);   this->Put(7,sy);   this->Put(8,sz);
  this->SetDistortionMode(0);
  this->FreezeUpdates=0;
  // Creates Transformation from parameters 
  this->UpdateParameters();

  

  // This call modifes A to yield B'
  return this->GenerateUpdateTransform(combined,reference,ref_orientation,
				       transform,trn_orientation);
  
}
//----------------------------------------------------------------------------
int vtkpxLinearTransform::ExtractParameters(vtkAbstractTransform* tr,int nparam,
					    vtkImageData* reference,int ref_orientation,
					    vtkImageData* transform,int trn_orientation)
{
  if (tr==NULL || reference==NULL || transform ==NULL)
    {
      vtkErrorMacro(<< "Bad  Inputs to Extract Parameters\n");
      return 0;
    }

  vtkTransform *estimatedMatrix=vtkTransform::New();
  estimatedMatrix->Identity();
  
  if (tr->IsA("vtkLinearTransform")!=1)
    {
      // Step 1 : Generate B' Matrix
      // ---------------------------
      vtkPoints* p1=vtkPoints::New();
      vtkPoints* p2=vtkPoints::New();
      
      p1->SetNumberOfPoints(8);
      
      double bounds[6]; reference->GetBounds(bounds);
      int index=0;
      for (int i=0;i<2;i++)
	for (int j=0;j<2;j++)
	  for (int k=0;k<2;k++)
	    {
	      p1->SetPoint(index,bounds[i],bounds[2+j],bounds[4+k]);
	      ++index;
	    }
      
      tr->TransformPoints(p1,p2);
      
      vtkLandmarkTransform* land=vtkLandmarkTransform::New();
      land->SetSourceLandmarks(p1);
      land->SetTargetLandmarks(p2);
      land->SetModeToAffine();
      
      if (nparam<7)
	land->SetModeToRigidBody();
      else if (nparam<8)
	land->SetModeToSimilarity();
      land->Update();
      
      p1->Delete();
      p2->Delete();

      estimatedMatrix->Concatenate(land->GetMatrix());
      land->Delete();
    }
  else
    {
      vtkLinearTransform* t=(vtkLinearTransform*) tr;
      estimatedMatrix->Concatenate(t->GetMatrix());
    }
  

  // ---------------------------------------------------------
  //  Transformation = Orient * Shift * Linear * MinusShift
  //  Linear = Shift(-1) *   ( Orient(-1) * Transformation ) * MinusShift(-1)
  // ---------------------------------------------------------
  
  // Not needed vtkpxUtil takes care of this
  //  ref_orientation=Irange(ref_orientation,0,2);
  //  trn_orientation=Irange(trn_orientation,0,2);
  
  
  // Orient = G(-1)
  vtkTransform* Orient=vtkTransform::New();
  vtkMatrix4x4* mat=vtkMatrix4x4::New();
  vtkpxUtil::ReOrientMatrix(transform,ref_orientation,trn_orientation,mat,1);
  //  vtkpxUtil::ReOrientMatrix(transform,trn_orientation,ref_orientation,mat);
  Orient->Identity();
  Orient->Concatenate(mat);
  mat->Delete();
  //  Orient->Inverse();

  int   rdim[3],tdim[3];
  double rsp[3],tsp[3];

  reference->GetSpacing(rsp); reference->GetDimensions(rdim);
  transform->GetSpacing(tsp); transform->GetDimensions(tdim);
  float shift[3],minusshift[3];
  for (int ib=0;ib<=2;ib++)
    {
      shift[ib]=0.5*tsp[ib]*float(tdim[ib]-1.0);
      minusshift[ib]=-0.5*rsp[ib]*float(rdim[ib]-1.0);
    }

  vtkTransform* MinusShift=vtkTransform::New();
  MinusShift->Translate(minusshift);
  MinusShift->Inverse();

  vtkTransform* Shift=vtkTransform::New();
  Shift->Translate(shift);
  Shift->Inverse();
 
  vtkTransform* combo=vtkTransform::New();
  combo->Identity();
  combo->PostMultiply();
  combo->Concatenate(MinusShift);
  combo->Concatenate(estimatedMatrix);
  combo->Concatenate(Shift);
  combo->Concatenate(Orient);

  //  fprintf(stderr,"Extracting Stuff");
  //LinPrint(MinusShift->GetMatrix(),"MinusShift");
  //LinPrint(land->GetMatrix(),"Landmark");
  //LinPrint(Orient->GetMatrix(),"Orientation");
  //LinPrint(Shift->GetMatrix(),"Shift");
  //LinPrint(combo->GetMatrix(),"Combo");



  estimatedMatrix->Delete();
  Shift->Delete();
  Orient->Delete();
  MinusShift->Delete();


  float rot[3]; combo->GetOrientation(rot);
  float pos[3]; combo->GetPosition(pos);
  float sc[3];  combo->GetScale(sc);

  for (int ia=0;ia<=2;ia++)
    {
      if (fabs(rot[ia])<0.001)
	rot[ia]=0.0;
      if (fabs(pos[ia])<0.001)
	pos[ia]=0.0;
    }



  // Ensure No Individual Updates
  this->FreezeUpdates=1;
  this->Identity();
  this->Put(0,pos[0]);   this->Put(1,pos[1]);   this->Put(2,pos[2]);
  this->Put(3,rot[0]);   this->Put(4,rot[1]);   this->Put(5,rot[2]);
  this->Put(6,sc[0]*100.0);   this->Put(7,sc[1]*100.0);   this->Put(8,sc[2]*100.0);
  this->SetDistortionMode(0);
  this->FreezeUpdates=0;
  this->UpdateParameters();
 
  return 0;
}
//---------------------------------------------------------------------------------------------
int vtkpxLinearTransform::GenerateUpdateTransform(vtkTransform* combined,
						  vtkImageData* reference,int ref_orientation,
						  vtkImageData* transform,int trn_orientation)
{
  if (reference==NULL || transform==NULL || combined==NULL)
    {
      combined->Identity();
      fprintf(stderr,"Here\n");
      combined->Concatenate(this->GetMatrix());
      return 0;
    }

  combined->Identity();

  //  ref_orientation=Irange(ref_orientation,0,2);
  //  trn_orientation=Irange(trn_orientation,0,2);
  
  int   rdim[3],tdim[3];
  double rsp[3],tsp[3];

  reference->GetSpacing(rsp); reference->GetDimensions(rdim);
  transform->GetSpacing(tsp); transform->GetDimensions(tdim);

  float shift[3],minusshift[3];
  for (int i=0;i<=2;i++)
    {
      shift[i]=0.5*tsp[i]*float(tdim[i]-1);
      minusshift[i]=-0.5*rsp[i]*float(rdim[i]-1);
    }

  return this->GenerateUpdateTransform(combined,minusshift,ref_orientation,shift,trn_orientation);

}
//---------------------------------------------------------------------------------------------
int vtkpxLinearTransform::GenerateUpdateTransform(vtkTransform* combined,
						  float minusshift[3],int ref_orientation,
						  float shift[3],int trn_orientation)
{
  if (combined==NULL)
    return 0;

  vtkMatrix4x4* mat=vtkMatrix4x4::New();
  double ori[3],sp[3];int d[3];
  vtkpxUtil::ReOrientMatrix(ori,sp,d,ref_orientation,trn_orientation,mat,1);

  vtkTransform* Orient=vtkTransform::New();
  Orient->Identity();
  Orient->PostMultiply();
  Orient->Concatenate(mat);
  Orient->Inverse();
  

  /*  fprintf(stderr,"Reference Orientation=%d Transform=%d\n",ref_orientation,trn_orientation);
      fprintf(stderr,"Shift = %.2f %.2f %.2f\n mShift=%.2f %.2f %.2f\n",
      shift[0],shift[1],shift[2],minusshift[0],minusshift[1],minusshift[2]);*/

  combined->Identity();
  combined->PostMultiply();
  combined->Translate(minusshift);
  combined->Concatenate(this->GetMatrix());
  combined->Concatenate(Orient);
  combined->Translate(shift);

  /*  fprintf(stdout,"\n Printing Combined\n");
      LinPrint(this->GetMatrix(),"Input Matrix");
      LinPrint(mat,"Reorient Matrix");
      LinPrint(combined->GetMatrix(),"Combined Matrix");*/
  mat->Delete();
  Orient->Delete();
  return 1;
}
			      
//---------------------------------------------------------------------------------------------

