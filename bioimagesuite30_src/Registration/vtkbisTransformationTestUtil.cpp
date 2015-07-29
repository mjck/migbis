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




#include "vtkObjectFactory.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkMath.h"
#include "vtkbisTransformationTestUtil.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkCellArray.h"


// ---------------------------

vtkbisTransformationTestUtil* vtkbisTransformationTestUtil::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisTransformationTestUtil");
  if(ret)
      {
	return (vtkbisTransformationTestUtil*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisTransformationTestUtil;
}

vtkbisTransformationTestUtil::vtkbisTransformationTestUtil()
{
  this->ClearStatus();
}

const char* vtkbisTransformationTestUtil::ReturnStatus() 
{
  return this->Status.c_str();
}

void vtkbisTransformationTestUtil::ClearStatus()
{
  this->Status="";
}


// ----------------------------------------------------------------------------------------------------
//
//  Core Method
//
// ----------------------------------------------------------------------------------------------------
int vtkbisTransformationTestUtil::CompareTransformations(vtkAbstractTransform* tr1,vtkAbstractTransform* tr2)
{
  return this->CompareTransformations(tr1,tr2,0.001);
}

int vtkbisTransformationTestUtil::CompareTransformations(vtkAbstractTransform* tr1,vtkAbstractTransform* tr2,double threshold)
{
  if (tr1==NULL && tr2==NULL)
    return 1;

  if (tr1==NULL || tr2==NULL)
    return 0;


  const char* name1=tr1->GetClassName();
  const char* name2=tr2->GetClassName();
  if (strcmp(name1,name2)!=0)
    return 0;

  if (tr1->IsA("vtkHomogeneousTransform")==1)
    return this->CompareHomogeneousTransformations((vtkHomogeneousTransform*)tr1,
					      (vtkHomogeneousTransform*)tr2,
					      threshold);

  if (tr1->IsA("vtkThinPlateSplineTransform"))
    return this->CompareThinPlateSplineTransformations((vtkThinPlateSplineTransform*)tr1,
						       (vtkThinPlateSplineTransform*)tr2,
						       threshold);

  if (tr1->IsA("vtkpxComboTransform"))
    return this->CompareComboTransformations((vtkpxComboTransform*)tr1,
								     (vtkpxComboTransform*)tr2,
								     threshold);
  if (tr1->IsA("vtkGridTransform"))
    return this->CompareGridTransformations((vtkGridTransform*)tr1,
								    (vtkGridTransform*)tr2,
								    threshold);
  
  return 0;

}
// ----------------------------------------------------------------------------------------------------
int vtkbisTransformationTestUtil::CompareHomogeneousTransformations(vtkHomogeneousTransform* tr1,
								vtkHomogeneousTransform* tr2,
								double threshold)
{
  vtkMatrix4x4* mat1=tr1->GetMatrix();
  vtkMatrix4x4* mat2=tr2->GetMatrix();

  double maxd=0.0;
  for (int ia=0;ia<=3;ia++)
    for (int ib=0;ib<=3;ib++)
      {
	double v=fabs(mat1->GetElement(ia,ib)-mat2->GetElement(ia,ib));
	if (v>maxd)
	  maxd=v;
      }

  sprintf(this->Buffer," Comparing 4x4 Matrix max=%.5f, thr=%.5f",maxd,threshold);
  this->Status+=Buffer;
  if (maxd>threshold)
    return 0;

  return 1;
}

// ----------------------------------------------------------------------------------------------------
int vtkbisTransformationTestUtil::CompareThinPlateSplineTransformations(vtkThinPlateSplineTransform* tr1,
								    vtkThinPlateSplineTransform* tr2,
								    double threshold)
{
  if (tr1->GetBasis()!=tr2->GetBasis())
    return 0;

  vtkPoints* pt1=tr1->GetSourceLandmarks();
  vtkPoints* pt2=tr2->GetSourceLandmarks();

  vtkbisTestUtil* ut=vtkbisTestUtil::New();
  ut->ClearStatus();
  int ok=ut->ComparePoints(pt1,pt2,threshold);
  this->Status+=ut->ReturnStatus();
  ut->Delete();
  return ok;
}
// ----------------------------------------------------------------------------------------------------
int vtkbisTransformationTestUtil::CompareGridTransformations(vtkGridTransform* tr1,
							     vtkGridTransform* tr2,
							     double threshold)
{
  if (tr1->GetInterpolationMode()!=tr2->GetInterpolationMode())
    return 0;

  vtkImageData* im1=tr1->GetDisplacementGrid();
  vtkImageData* im2=tr2->GetDisplacementGrid();
  vtkbisTestUtil* ut=vtkbisTestUtil::New();
  ut->ClearStatus();
  int ok=ut->CompareImages(im1,im2,threshold);
  this->Status+=ut->ReturnStatus();
  ut->Delete();
  return ok;
}
// ----------------------------------------------------------------------------------------------------
int vtkbisTransformationTestUtil::CompareComboTransformations(vtkpxComboTransform* tr1,
							      vtkpxComboTransform* tr2,
							      double threshold)
{
  if (tr1->GetNonLinearFirst()!=tr2->GetNonLinearFirst())
    return 0;

  int ok2=this->CompareHomogeneousTransformations(tr1->GetLinearTransform(),
									  tr2->GetLinearTransform(),
									  threshold);
  if (ok2==0)
    return 0;


  int n1=tr1->GetNumberOfGridTransformations();
  int n2=tr2->GetNumberOfGridTransformations();
  if (n1!=n2)
    return 0;



  for (int ia=0;ia<n1;ia++)
    {
      int ok=this->CompareGridTransformations(tr1->GetGridTransformByIndex(ia),
					      tr2->GetGridTransformByIndex(ia),
					      threshold);
      if (ok==0)
	return 0;
    }

  return 1;
}


