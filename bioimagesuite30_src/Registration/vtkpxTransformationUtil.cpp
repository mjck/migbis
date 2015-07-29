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
  Module:    $RCSfile: vtkpxTransformationUtil.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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

#include "pxutil.h"
#include "vtkAbstractTransform.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkGridTransform.h"
#include "vtkHomogeneousTransform.h"
#include "vtkIdentityTransform.h"
#include "vtkImageCast.h"
#include "vtkImageData.h"
#include "vtkImageData.h"
#include "vtkMath.h"
#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"
#include "vtkStructuredPoints.h"
#include "vtkStructuredPointsReader.h"
#include "vtkStructuredPointsWriter.h"
#include "vtkThinPlateSplineTransform.h"
#include "vtkTransform.h"
#include "vtkpxComboTransform.h"
#include "vtkpxGridTransform.h"
#include "vtkpxMatrix.h"
#include "vtkpxTransformationUtil.h"
#include "vtkMatrixToHomogeneousTransform.h"
#include "vtkAppendPolyData.h"
#include "vtkImageExtractComponents.h"
#include "bioimagesuite_settings.h"
#include "vtkpxAppendComponents.h"
#include "vtkContourFilter.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageMathematics.h"
#include "vtkImageDataGeometryFilter.h"
#include "vtkThresholdPoints.h"
#include "vtkPolyDataConnectivityFilter.h"


#ifdef BIOIMAGESUITE_EXPERIMENTAL
#include "vtkpxPolySimilarityTransform.h"
#include "vtkpxSimilarityTransform.h"
#endif
// ---------------------------

vtkpxTransformationUtil* vtkpxTransformationUtil::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxTransformationUtil");
  if(ret)
      {
	return (vtkpxTransformationUtil*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxTransformationUtil;
}


/*=========================================================================


     T R A N S F O R M      C O D E


=========================================================================*/


//------------------------------------------------------------------------
// Some Matrix Code --- genrate rotation matrix from 2 or 3 points 
//------------------------------------------------------------------------
int vtkpxTransformationUtil::GenerateTransform(vtkTransform* tr,vtkPoints* points,int npoints)
{
  if (tr==NULL || points==NULL)
    return 0;

  tr->Identity();

  if (npoints==-1)
    {
      if (points->GetNumberOfPoints()<2)
	  return 0;
      
      if (points->GetNumberOfPoints()>3)
	npoints=4;
      else
	npoints=2;
    }
  else
    {
      if (points->GetNumberOfPoints()<2)
	{
	  return 0;
	}
      else 
	npoints=Irange(npoints,2,4);
    }

  // fprintf(stderr,"Generating transform npoints=%d\n",npoints);

  double x1[3],x2[3],x3[3];
  
  if (npoints==3)
    {
      points->GetPoint(0,x1);
      points->GetPoint(1,x2);
      points->GetPoint(2,x3);

      // Pick x1 as origin 
      for (int ia=0;ia<=2;ia++)
	{
	  x2[ia]=x2[ia]-x1[ia];
	  x3[ia]=x3[ia]-x1[ia];
	  x1[ia]=0.0;
	}

      /*      fprintf(stderr,"Differences:\n");
	      for (int ia=0;ia<=2;ia++)
	      fprintf(stderr,"Vectors x1[%d]=%5.2f x2[%d]=%5.2f x3[%d]=%5.2f\n", ia,x1[ia],ia,x2[ia],ia,x3[ia]);*/

      vtkMath::Normalize(x2);
      vtkMath::Normalize(x3);
      vtkMath::Cross(x2,x3,x1);
      /*fprintf(stderr,"Perpendiculars:\n");
	for (int ia=0;ia<=2;ia++)
	fprintf(stderr,"Vectors x1[%d]=%5.2f x2[%d]=%5.2f x3[%d]=%5.2f\n", ia,x1[ia],ia,x2[ia],ia,x3[ia]);*/

    }
  else
    { 
      points->GetPoint(0,x1);
      points->GetPoint(1,x2);

      // Pick x1 as origin 
      for (int ia=0;ia<=2;ia++)
	x2[ia]=x2[ia]-x1[ia];

      /*fprintf(stderr,"Differences:\n");
	for (int ia=0;ia<=2;ia++)
	fprintf(stderr,"Vectors x1[%d]=%5.2f x2[%d]=%5.2f x3[%d]=%5.2f\n", ia,x1[ia],ia,x2[ia],ia,x3[ia]);*/

      vtkMath::Normalize(x2);
      vtkMath::Perpendiculars(x2,x3,x1,0.0);
      /*fprintf(stderr,"Perpendiculars:");
	for (int ia=0;ia<=2;ia++)
	fprintf(stderr,"Vectors x1[%d]=%5.2f x2[%d]=%5.2f x3[%d]=%5.2f\n", ia,x1[ia],ia,x2[ia],ia,x3[ia]);*/
    }


  // Set the 3x3 Section with the vectors x2,x3,x1 ::: Done 
  vtkMatrix4x4* mat=tr->GetMatrix();
  for (int i=0;i<=2;i++)
    {
      mat->SetElement(0,i,x1[i]);
      mat->SetElement(1,i,x2[i]);
      mat->SetElement(2,i,x3[i]);
    }

  //  PrintMatrix(mat->Element,4,4);
  return 1;
}



//------------------------------------------------------------------------
// Fix Mmvreg Output to central coordinates 
//------------------------------------------------------------------------
void vtkpxTransformationUtil::FixMmvregOutput(vtkMatrix4x4* matrix,int permute)
{
  if (matrix==NULL)
    return;

  double mat[4][4];

  for (int i=0;i<=3;i++)
    for (int j=0;j<=3;j++)
      mat[i][j]=matrix->GetElement(i,j);

  permute=Irange(permute,0,2);
  
  for (int ia=0;ia<=3;ia++)
    for (int ib=0;ib<=3;ib++)
      {
	int newi=Icyclic(ia+permute,3);
	int newj=Icyclic(ib+permute,3);
	matrix->SetElement(ia,ib,mat[newi][newj]);
      }
}

//------------------------------------------------------------------------
// Transformation I/O
//------------------------------------------------------------------------

int vtkpxTransformationUtil::SaveAbstractTransform(vtkAbstractTransform* tr,const char* fname)
{

  if ( tr->IsA("vtkHomogeneousTransform")==1) 
    return vtkpxTransformationUtil::SaveMatrix(((vtkHomogeneousTransform*)tr)->GetMatrix(),fname);
  else if ( tr->IsA("vtkThinPlateSplineTransform")==1)
    return vtkpxTransformationUtil::SaveThinPlateSplineTransform((vtkThinPlateSplineTransform*)tr,fname);
  else if ( tr->IsA("vtkpxGridTransform")==1) 
    return ((vtkpxGridTransform*)tr)->Save(fname);
  else if ( tr->IsA("vtkpxComboTransform")==1) 
    return ((vtkpxComboTransform*)tr)->Save(fname);

#ifdef BIOIMAGESUITE_EXPERIMENTAL
  else if ( tr->IsA("vtkpxSimilarityTransform")==1)
    return ((vtkpxSimilarityTransform*)tr)->Save(fname); 
  else if ( tr->IsA("vtkpxPolySimilarityTransform")==1)
    return ((vtkpxPolySimilarityTransform*)tr)->Save(fname); 
#endif
  return 0;
}

vtkAbstractTransform* vtkpxTransformationUtil::LoadAbstractTransform(const char* fname)
{
  // Check Grid, then ThinPlate then Matrix

  vtkpxComboTransform* tg=vtkpxComboTransform::New();
  int ok=tg->Load(fname);
  if (ok)
    return tg;

  tg->Delete();

#ifdef BIOIMAGESUITE_EXPERIMENTAL
  vtkpxSimilarityTransform* stg=vtkpxSimilarityTransform::New();
  ok=stg->Load(fname);
  if (ok)
    return stg;
  stg->Delete();

  vtkpxPolySimilarityTransform* pstg=vtkpxPolySimilarityTransform::New();
  ok=pstg->Load(fname);
  if (ok)
    return pstg;
  pstg->Delete();
#endif

  vtkThinPlateSplineTransform* tr=vtkThinPlateSplineTransform::New();
  ok=vtkpxTransformationUtil::LoadThinPlateSplineTransform(tr,fname);
  if (ok)
    return tr;

  tr->Delete();
  
  vtkTransform* tm=vtkTransform::New();
  ok=LoadMatrixTransform(tm,fname);
  
  if (ok)
    return tm;

  tm->Delete();
  return NULL;
}
//------------------------------------------------------------------------
int vtkpxTransformationUtil::LoadMatrixTransform(vtkTransform* tr,const char* fname)
{
  vtkMatrix4x4* mat=vtkMatrix4x4::New();
  int ok=vtkpxTransformationUtil::LoadMatrix(mat,fname);
  if (ok)
    {
      tr->Identity();
      tr->Concatenate(mat);
    }
  mat->Delete();

  return ok;
}

int vtkpxTransformationUtil::SaveMatrixTransform(vtkTransform* tr,const char* fname)
{
  return vtkpxTransformationUtil::SaveMatrix(tr->GetMatrix(),fname);
}


int vtkpxTransformationUtil::LoadMatrix(vtkMatrix4x4* matrix,const char* fname)
{
  FILE* fin=fopen(fname,"r");
  if (fin==NULL)
    return 0;

  vtkMatrix4x4* mat_in=vtkMatrix4x4::New();
  mat_in->Identity();


  float mat;
  for (int i=0;i<=3;i++)
    for (int j=0;j<=3;j++)
      {
	fscanf(fin,"%f ",&mat);
	mat_in->SetElement(i,j,mat);
      }
  fclose(fin);

  matrix->DeepCopy(mat_in);
  mat_in->Delete();
  return 1;

}

int vtkpxTransformationUtil::SaveMatrix(vtkMatrix4x4* matrix,const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (fout==NULL)
    return 0;
  
  for(int i=0;i<=3;i++)
    {
      for (int j=0;j<=3;j++)
	fprintf(fout,"%7.3f ",matrix->GetElement(i,j));
      fprintf(fout,"\n");
    }
  fclose(fout);
  return 1;
}




int vtkpxTransformationUtil::LoadThinPlateSplineTransform(vtkThinPlateSplineTransform* tps,const char* fname)
{
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  int ok=vtkpxTransformationUtil::LoadThinPlateSplineTransform(tps,fin);

  gzclose(fin);
  return ok;
}



int vtkpxTransformationUtil::LoadThinPlateSplineTransform(vtkThinPlateSplineTransform* tps,gzFile fin)
{
  char pxtk_buffer[100];
  gzgets(fin,pxtk_buffer,100);
 
  if (gzfindstringinstring(pxtk_buffer,"#Thin-Plate Spline Transform File")==0)
    return 0;

  gzgets(fin,pxtk_buffer,100); 
  gzgets(fin,pxtk_buffer,100);
  int np;
  sscanf(pxtk_buffer,"%d",&np);
  vtkPoints* points[2];
  
  for (int j=0;j<=1;j++)
    {
      gzgets(fin,pxtk_buffer,100);
      points[j]=vtkPoints::New();
      points[j]->SetNumberOfPoints(np);

      for (int i=0;i<np;i++)
	{
	  float x[3];
	  gzgets(fin,pxtk_buffer,100);
	  sscanf(pxtk_buffer,"%f %f %f",&x[0],&x[1],&x[2]);
	  points[j]->SetPoint(i,x);
	}
    }

  
  tps->SetSourceLandmarks(points[0]);
  tps->SetTargetLandmarks(points[1]);
  tps->SetBasisToR();
  tps->Update();
  points[0]->Delete();
  points[1]->Delete();

  return 1;
}

int vtkpxTransformationUtil::SaveThinPlateSplineTransform(vtkThinPlateSplineTransform* tps,const char* fname)
{
  vtkPoints* points[2];
  
  points[0]=tps->GetSourceLandmarks();
  points[1]=tps->GetTargetLandmarks();

  if (points[0]==NULL || points[1]==NULL)
    return 0;
  
  //int np=points[0]->GetNumberOfPoints();
  
  FILE* fout=fopen(fname,"w");
  if (fout==NULL)
    return 0;

  int ok=vtkpxTransformationUtil::SaveThinPlateSplineTransform(tps,fout);
  fclose(fout);
  return ok;
}
// --------------------------------------------------------------------------------------
int vtkpxTransformationUtil::SaveThinPlateSplineTransform(vtkThinPlateSplineTransform* tps,FILE* fout)
{

  vtkPoints* points[2];
  
  points[0]=tps->GetSourceLandmarks();
  points[1]=tps->GetTargetLandmarks();

  if (points[0]==NULL || points[1]==NULL)
    return 0;
  
  int np=points[0]->GetNumberOfPoints();
    
  fprintf(fout,"#Thin-Plate Spline Transform File\n");
  fprintf(fout,"#Number of Points\n%d\n",np);
  for (int j=0;j<=1;j++)
    {
      if (j==0)
	fprintf(fout,"#Source Points\n");
      else
	fprintf(fout,"#Target Points\n");
      for (int i=0;i<np;i++)
	{
	  double x[3];
	  points[j]->GetPoint(i,x);
	  fprintf(fout,"%.2f %.2f %.2f\n",x[0],x[1],x[2]);
	}
    }
  return 1;
}
// --------------------------------------------------------------------------------------

int vtkpxTransformationUtil::SaveGridTransform(vtkpxGridTransform* mat,const char* fname)
{
  return mat->Save(fname);
}

int vtkpxTransformationUtil::LoadGridTransform(vtkpxGridTransform* tr,const char* fname)
{
  char pxtk_buffer[400];
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  gzgets(fin,pxtk_buffer,100);
  
  if (gzfindstringinstring(pxtk_buffer,"#Grid Transform File")==1)
    {
      float scale,shift;
      int interp;
      gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
      sscanf(pxtk_buffer,"%f",&scale);
      gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
      sscanf(pxtk_buffer,"%f",&shift);
      gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
      sscanf(pxtk_buffer,"%d",&interp);
      gzclose(fin);
      sprintf(pxtk_buffer,"%s.cntrl",fname);
      vtkStructuredPointsReader* rd=vtkStructuredPointsReader::New();
      rd->SetFileName(pxtk_buffer);
      rd->Update();
      tr->SetDisplacementScale(scale);
      tr->SetDisplacementShift(shift);
      tr->SetInterpolationMode(interp);
      tr->SetDisplacementGrid(rd->GetOutput());
      rd->Delete();
      return 1;
    }
  gzclose(fin);
  int ok=tr->Load(fname);
  return ok;
}

float vtkpxTransformationUtil::GetJacobian(vtkAbstractTransform* tr,vtkpxMatrix* jac,float x,float y,float z)
{
  tr->Update();

  if (tr==NULL || jac==NULL)
    return 0.0;
  jac->Zeros(3,3);

  float p[3],p2[3],der[3][3];
  p[0]=x;p[1]=y;p[2]=z;
  tr->InternalTransformDerivative(p,p2,der);
  for (int i=0;i<=2;i++)
    for (int j=0;j<=2;j++)
      jac->SetElement(i,j,der[i][j]);
  return vtkMath::Determinant3x3(der);
}
// ----------------------------------------------------------------------
// Mouse Model Stuff originally in vtkpxPolySimilarityTransform
// ----------------------------------------------------------------------

vtkImageData* vtkpxTransformationUtil::GetMaskImage(vtkImageData* input,vtkFloatArray* planes,int offset)
{
  return vtkpxTransformationUtil::GetMaskImage(input,planes,offset,0);
}

vtkImageData* vtkpxTransformationUtil::GetMaskImage(vtkImageData* input,vtkFloatArray* planes,int offset,int flip)
{
  double n1[3],c1[3];
  
  for (int ia=0;ia<=2;ia++)
    {
      n1[ia]=planes->GetComponent(offset,ia);
      c1[ia]=planes->GetComponent(offset,ia+4);
    }
  double dist1=planes->GetComponent(offset,3);
  int above1=(int)planes->GetComponent(offset,7);
  if (flip)
    above1=-above1;

  return vtkpxTransformationUtil::GetMaskImage(input,above1,n1,c1,dist1);

}
// ----------------------------------------------------------------------
vtkImageData* vtkpxTransformationUtil::GetMaskImage(vtkImageData* image,int abv,double n[3],double c[3],double offset)
{
  if (abv!=1)
    abv=-1;

  //  fprintf(stderr,"\nCenter = %.2f,%.2f,%.2f Normal=%.2f,%.2f,%.2f offset=%.2f\t abd=%d\n",c[0],c[1],c[2],n[0],n[1],n[2],offset,abv);
  



  vtkImageData* img=vtkImageData::New();
  img->CopyStructure(image);
  img->SetNumberOfScalarComponents(1);
  img->SetScalarTypeToShort();
  img->AllocateScalars();

  int np=img->GetNumberOfPoints();
  vtkDataArray* data=img->GetPointData()->GetScalars();
  vtkDataArray* inp=image->GetPointData()->GetScalars();
  
  data->CopyComponent(0,inp,0);
    

  int dim[3];  img->GetDimensions(dim);
  double spa[3]; img->GetSpacing(spa);
  double ori[3]; img->GetOrigin(ori);

  double x[3];
  
  for (int k=0;k<dim[2];k++)
    {
      x[2]=double(k)*spa[2]+ori[2];
      for (int j=0;j<dim[1];j++)
	{
	  x[1]=double(j)*spa[1]+ori[1];
	  for (int i=0;i<dim[0];i++)
	    {
	      x[0]=double(i)*spa[0]+ori[0];
	      double sum=0.0;
	      for (int ia=0;ia<=2;ia++)
		sum+=(x[ia]-c[ia])*n[ia];

	      if (sum*abv<0)	      
		img->SetScalarComponentFromDouble(i,j,k,0,0);
	    }
	}
    }
  return img;
}
// ----------------------------------------------------------------------
vtkImageData* vtkpxTransformationUtil::CreateRegionLabelImage(vtkImageData* input,float sigma)
{
  return vtkpxTransformationUtil::CreateRegionLabelImage(input,sigma,NULL);
}
// ----------------------------------------------------------------------
int vtkpxTransformationUtil::CreatePolygonalModel(vtkImageData* multiframeinput,vtkPolyData* surface,float threshold)
{
  fprintf(stderr,"\n\n Now Creating Surface Model thr=%.2f\n",threshold);
       
  int nc=multiframeinput->GetNumberOfScalarComponents();
  vtkAppendPolyData* append=vtkAppendPolyData::New();
  
  for (int i=0;i<nc;i++)
    {
      fprintf(stderr,"Part %d\n",i+1);
      vtkImageExtractComponents* frame=vtkImageExtractComponents::New();
      frame->SetInput(multiframeinput);
      frame->SetComponents(i);
      frame->Modified();
	  
      vtkContourFilter* skinExtractor=vtkContourFilter::New();
      skinExtractor->SetInput(frame->GetOutput());
      skinExtractor->SetUseScalarTree(0);
      skinExtractor->SetNumberOfContours(1);
      skinExtractor->SetValue(0,threshold);
      skinExtractor->ComputeScalarsOff();
      skinExtractor->ComputeGradientsOff();
      skinExtractor->ComputeNormalsOff();
      skinExtractor->Update();
      
      vtkPolyData* tmp=vtkPolyData::New();
      tmp->ShallowCopy(skinExtractor->GetOutput());
      
      vtkDataArray* ptScalars=vtkDataArray::CreateDataArray(VTK_SHORT);
      ptScalars->SetNumberOfComponents(1);
      ptScalars->SetNumberOfTuples(tmp->GetPoints()->GetNumberOfPoints());
      ptScalars->FillComponent(0,i);
	  
      tmp->GetPointData()->SetScalars(ptScalars);
      ptScalars->Delete();
      
      append->AddInput(tmp);
      tmp->Delete();
      
      skinExtractor->Delete();
      frame->Delete();
    }
      
  fprintf(stderr,"Concatenating\n");
  append->Update();
      
  surface->ShallowCopy(append->GetOutput());
  append->Delete();
  return 1;
}
// ----------------------------------------------------------------------
vtkImageData* vtkpxTransformationUtil::CreateRegionLabelImage(vtkImageData* input,float sigma,vtkPolyData* surface)
{
  double r[2]; input->GetPointData()->GetScalars()->GetRange(r);
  double threshold=(r[0]+r[1])*0.5;

  fprintf(stderr,"Creating Model Input nc=%d range=%.1f:%.1f threshold=%.2f sigma=%.2f\n",
	  input->GetNumberOfScalarComponents(),r[0],r[1],threshold,sigma);


  vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
  smooth->SetInput(input);
  smooth->SetStandardDeviations(sigma,sigma,sigma);
  smooth->SetRadiusFactors(3.0,3.0,3.0);
  smooth->Update();

  vtkImageData* output=vtkImageData::New();
  output->CopyStructure(input);
  output->SetNumberOfScalarComponents(1);
  output->SetScalarTypeToShort();
  output->AllocateScalars();

  vtkDataArray* inp=smooth->GetOutput()->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();

  int np=out->GetNumberOfTuples();
  int nc=input->GetNumberOfScalarComponents();

  out->FillComponent(0,0);

  fprintf(stderr,"Here Starting np=%d nc=%d :",np,nc);

  int count=0;
  int per=0;
  int tenth=np/10;

  for (int i=0;i<np;i++)
    {
      int best=-1;
      double bestv=0;

      for (int j=0;j<nc;j++)
	{
	  double v=inp->GetComponent(i,j);
	  if (v>bestv)
	    {
	      bestv=v;
	      best=j;
	    }
	}

      out->SetComponent(i,0,best);
      ++count;
      if (count==tenth)
	{
	  per+=10;
	  fprintf(stderr,"%d \%",per);
	  count=0;
	}
    }


  if (surface != NULL)
    {
      vtkImageData* smoothed=smooth->GetOutput();
      vtkpxTransformationUtil::CreatePolygonalModel(smoothed,surface,threshold);
    }
  
  smooth->Delete();
  return output;

}

// ----------------------------------------------------------------------
//
//            Whole body atlas  .... .............
//
// ----------------------------------------------------------------------
vtkImageData* vtkpxTransformationUtil::ChopLeg(vtkImageData* Leg,vtkFloatArray* planes,int offset,int numcuts,int legmode)
{
  legmode=(legmode>0);

  vtkpxAppendComponents* append=vtkpxAppendComponents::New();

  for (int piece=0;piece<numcuts;piece++)
    {
      double n[3],c[3],dist;
      for (int ia=0;ia<=2;ia++)
	{
	  n[ia]=planes->GetComponent(offset+piece,ia);
	  c[ia]=planes->GetComponent(offset+piece,ia+4);
	}
      dist=planes->GetComponent(offset+piece,3);
      int above=(int)planes->GetComponent(offset+piece,7);
      int oldabove=above;
      if (above<=0)
	above=-1;
      else
	above=1;
     
      if (piece==0)
	above=-above;

      fprintf(stderr,"Piece=%d offset+piece=%d n=%.2f,%.2f,%.2f  dist=%.2f above=%d --> %d\n",
	      piece,offset+piece,
	      n[0],n[1],n[2],dist,oldabove,above);

      vtkImageData* tmp=vtkpxTransformationUtil::GetMaskImage(Leg,above,n,c,dist);

      vtkImageMathematics* m2=vtkImageMathematics::New();
      m2->SetInput1(Leg);
      m2->SetInput2(tmp);
      m2->SetOperationToSubtract();
      m2->Update();

      
      Leg->ShallowCopy(m2->GetOutput());
      m2->Delete();

      if (piece==numcuts-1 && legmode==1)
	append->AddInput(Leg);	
      append->AddInput(tmp);

      tmp->Delete();
    }
  
  if (legmode==0)
    append->AddInput(Leg);	

  append->Update();

  vtkImageData* pieces=vtkImageData::New();
  pieces->ShallowCopy(append->GetOutput());
  append->Delete();
  return pieces;
}
// ----------------------------------------------------------------------
vtkImageData* vtkpxTransformationUtil::CreateWholeBodyRegionLabelImage(vtkImageData* input,vtkFloatArray* planes,
									    int hasfeet,int hashindlimbs,int hasforelimbs,
									    int hashead,int taildivisions, int spinedivisions)
{
  // Plane order
  // Left  Hipp 1+2, Left Knee ,Left Foot               0..3 
  // Right Hipp 1+2, Right Knee ,Right Foot             4..7
  // Left  Shoulder 1+2, Left Elbow ,Left Wrist         8..11
  // Right Shoulder 1+2, Right Elbow ,Right Wrist       12..15
  // Tail from hipp down                                16..20
  // Abdomen from hipp up                               21..25
  // Neck                                               26
  
  // Strategy
  // 1. Chop off all legs first (1..4)
  // 2. Chop off tail
  // 3. Chop up legs, tail
  // 4. Chop up abdomen and head

  //  int abdomenoffset=0;

  if (input==NULL || planes == NULL)
    {
      fprintf(stderr,"Bad Inputs to Create Region Label Image\n");
      return NULL;
    }
	
  hasfeet=(hasfeet>0);
  hashead=(hashead>0);
  hashindlimbs=(hashindlimbs>0);
  hasforelimbs=(hasforelimbs>0);

  if (spinedivisions>10)
    spinedivisions=10;
  else if (spinedivisions<1)
    spinedivisions=0;

  if (taildivisions<1)
    taildivisions=0;
  else if (taildivisions>5)
    taildivisions=5;

  int legparts=3+hasfeet;  // (hip needs two planes)

  int totalplanes=
    hashead + // head
    2*(hashindlimbs+hasforelimbs)*legparts + // 4 limbs
    spinedivisions + //
    taildivisions;
	
  int numberofplanes=planes->GetNumberOfTuples();
  if (numberofplanes!=totalplanes || planes->GetNumberOfComponents()!=8)
    {
      fprintf(stderr,"Bad Number of Planes %d (vs %d needed) or components (%d vs 8) to Create Region Label Image\n",numberofplanes,totalplanes,
	      planes->GetNumberOfComponents());
      return NULL;
    }


  vtkpxAppendComponents* append=vtkpxAppendComponents::New();
  
  // --------------------------------------------------------------------------------
  // [A]  Chop off Four Legs to leave MainTrunk -- further subdivide legs into pieces
  // --------------------------------------------------------------------------------

  int minleg=-1,maxleg=-2;

  vtkImageData* MainTrunk=vtkImageData::New();
  vtkImageData* LegPieces[4];
  for (int i=0;i<=3;i++)
    LegPieces[i]=NULL;


  if (hasforelimbs ==1 || hashindlimbs ==1 )
    {
      minleg=0;
      maxleg=3;
      if (hashindlimbs==0)
	minleg=2;
      if (hasforelimbs==0)
	maxleg=1;

      vtkImageData* Leg[4];
      
      for (int leg=minleg;leg<=maxleg;leg++)
	{
	  int offset=legparts*leg;
	  vtkImageData* tmp=vtkpxTransformationUtil::GetMaskImage(input,planes,offset);
	  Leg[leg]=vtkpxTransformationUtil::GetMaskImage(tmp,planes,offset+1);

	  tmp->Delete();
	}
      
      vtkImageMathematics* m2=vtkImageMathematics::New();
      m2->SetInput1(input);
      m2->SetOperationToSubtract();
  
      if (hasforelimbs ==1 && hashindlimbs ==1 )
	{
	  vtkImageMathematics* m1[3]; 
	  for (int p=0;p<=2;p++)
	    {
	      m1[p]=vtkImageMathematics::New();
	      if (p<2)
		{
		  m1[p]->SetInput1(Leg[2*p+1]);
		  m1[p]->SetInput2(Leg[2*p]);
		}
	      else
		{
		  m1[2]->SetInput1(m1[0]->GetOutput());
		  m1[2]->SetInput2(m1[1]->GetOutput());
		}
	      m1[p]->SetOperationToAdd();
	      m1[p]->Update();
	    }
	  m2->SetInput2(m1[2]->GetOutput());

	  //	  vtkpxUtil::SaveAnalyze("Legs",m1[2]->GetOutput(),0);


	  for (int p=0;p<=2;p++)
	    m1[p]->Delete();
	}
      else
	{
	  vtkImageMathematics* m1=vtkImageMathematics::New();
	  m1->SetInput1(Leg[minleg]);
	  m1->SetInput2(Leg[maxleg]);
	  m1->SetOperationToAdd();
	  m1->Update();
	  m2->SetInput2(m1->GetOutput());
	  //	  vtkpxUtil::SaveAnalyze("Legs",m1->GetOutput(),0);
	  m1->Delete();
	}
      m2->Update();
      MainTrunk->ShallowCopy(m2->GetOutput());
      m2->Delete();

      for (int i=minleg;i<=maxleg;i++)
	{
	  LegPieces[i]=vtkpxTransformationUtil::ChopLeg(Leg[i],planes,legparts*i+2,1+hasfeet,1);
	  Leg[i]->Delete();
	}
    }
  else
    {
      MainTrunk->ShallowCopy(input);
    }

  //  vtkpxUtil::SaveAnalyze("MainTrunk",MainTrunk,0);

  // --------------------------------------------------------------------------------
  // [B] Chop off Tail and Spine
  // --------------------------------------------------------------------------------

  // Next do tail -- and abdomen top -- two cuts 

  vtkImageData* SpineTailParts[2];
  for (int ia=0;ia<=1;ia++)
    SpineTailParts[ia]=NULL;


  if ((spinedivisions+hashead)> 0 || taildivisions > 0 ) 
    {
      fprintf(stderr,"In Spine or Tail Divisions\n");

      vtkImageData* SpineTail[2];

      int cut[3]; 
      cut[0]=legparts*2*(hashindlimbs+hasforelimbs);
      cut[1]=cut[0]+taildivisions;
      cut[2]=totalplanes;
      
      int minpart=0,maxpart=1;
      if (taildivisions==0)
	minpart=1;
      if( (spinedivisions+hashead)==0)
	maxpart=0;

      for (int part=minpart;part<=maxpart;part++)
	{
	  int offset=cut[part];
	  SpineTail[part]=vtkpxTransformationUtil::GetMaskImage(MainTrunk,planes,offset);
	}

      vtkImageMathematics* m2a=vtkImageMathematics::New();
      m2a->SetInput1(MainTrunk);
      m2a->SetOperationToSubtract();
      
      if (maxpart==1 && minpart==0)
	{
	  vtkImageMathematics* m1a=vtkImageMathematics::New();
	  m1a->SetInput1(SpineTail[0]);
	  m1a->SetInput2(SpineTail[1]);
	  m1a->SetOperationToAdd();
	  m1a->Update();
	  m2a->SetInput2(m1a->GetOutput());
	  m1a->Delete();
	}
      else if (maxpart==0)
	{
	  m2a->SetInput2(SpineTail[0]);
	}
      else
	{
	  m2a->SetInput2(SpineTail[1]);
	}
      m2a->Update();
      MainTrunk->ShallowCopy(m2a->GetOutput());
      m2a->Delete();
      
      for (int p=minpart;p<=maxpart;p++)
	{
	  int numcuts=cut[p+1]-cut[p]-1;
	  fprintf(stderr,"Tail part=%d, numcuts=%d\n",p,numcuts);

	  if (numcuts>0)
	    {
	      SpineTailParts[p]=vtkpxTransformationUtil::ChopLeg(SpineTail[p],planes,1+cut[p],numcuts,0);
	      SpineTail[p]->Delete();
	    }
	  else
	    {
	      SpineTailParts[p]=SpineTail[p];
	    }
	}
    }
  
  append->AddInput(MainTrunk); 
  MainTrunk->Delete();
  for (int ia=0;ia<=3;ia++)
    {
      if (LegPieces[ia]!=NULL)
	{
	  fprintf(stderr,"Adding LegPiece %d\n",ia);
	  append->AddInput(LegPieces[ia]); 
	  LegPieces[ia]->Delete();
	}
    }

  for (int ia=0;ia<=1;ia++)
    {
      if (SpineTailParts[ia]!=NULL)
	{
	  append->AddInput(SpineTailParts[ia]); 
	  SpineTailParts[ia]->Delete();
	}
    }
  append->Update();
  vtkImageData* model=vtkImageData::New();
  model->ShallowCopy(append->GetOutput()); 
  append->Delete();
  
  return model;
}
// ----------------------------------------------------------------------
int vtkpxTransformationUtil::GetPlaneNumber(int jointno,int hasfeet,
						 int hashindlimbs,int hasforelimbs,int hashead, int taildivisions,
						 int spinedivisions)
{
  if ( jointno < 1 ) {   return -1; }

  int legparts=2+(hasfeet>0);
  int offset=0;
  int totalhips=0;

  int numpass=(hashindlimbs>0) + (hasforelimbs>0);
  
  for (int pass=0; pass< numpass; pass++)
    {
      int legpartp = offset+legparts;
      if (jointno<=legpartp)
	{
	  return jointno+totalhips;
	}
      totalhips+=2;
    }

  return jointno + totalhips -1;
}

vtkPoints* vtkpxTransformationUtil::GetCentroids(vtkImageData* img,double axislength)
{

  int dim[3]; img->GetDimensions(dim);

  int numjoints=dim[2];

  vtkPoints* jointpoints=vtkPoints::New();
  jointpoints->SetNumberOfPoints(numjoints*3);


  //  vtkpxUtil::SaveAnalyze("tt_joint.hdr",img);

  for (int joint=0;joint<numjoints;joint++)
    {
      vtkImageDataGeometryFilter* filt=vtkImageDataGeometryFilter::New();
      filt->SetInput(img);
      filt->SetExtent(0,dim[0]-1,
		      0,dim[1]-1,
		      joint,joint);
      filt->Update();

     
      vtkThresholdPoints* thrPoints=vtkThresholdPoints::New();
      thrPoints->SetInput(filt->GetOutput());
      thrPoints->ThresholdBetween(1,250);
      thrPoints->Update();

      vtkPoints* points=points=vtkPoints::New();

      if (thrPoints->GetOutput()->GetNumberOfPoints()>0)
	{
	  vtkPolyDataConnectivityFilter* connectFilter=vtkPolyDataConnectivityFilter::New();
	  connectFilter->SetInput(thrPoints->GetOutput());
	  connectFilter->SetExtractionModeToLargestRegion();
	  connectFilter->Update();
	  
	  fprintf(stderr,"Extracted %d Connected %d --> Number of Regions=%d\n",
		  thrPoints->GetOutput()->GetNumberOfPoints(),
		  connectFilter->GetOutput()->GetNumberOfPoints(),
		  connectFilter->GetNumberOfExtractedRegions());
	  
	  
	  points->ShallowCopy(connectFilter->GetOutput()->GetPoints());	  
	  connectFilter->Delete();
	} 
      else
	{
	  points->SetNumberOfPoints(1);
	  points->SetPoint(0,0,0,0);
	}
      
      thrPoints->Delete();
      filt->Delete();
      
      int nump=points->GetNumberOfPoints();

      double centroid[3] = { 0.0,0.0,0.0};
      for (int i=0;i<nump;i++)
	{
	  double p[3]; points->GetPoint(i,p);
	  for (int ia=0;ia<=2;ia++)
	    centroid[ia]+=p[ia];
	}

      if (nump>1)
	{
	  for (int ia=0;ia<=2;ia++)
	    centroid[ia]/=double(nump);
	}

      jointpoints->SetPoint(joint*3,centroid);
      jointpoints->SetPoint(joint*3+1,centroid[0]+axislength,centroid[1],centroid[2]);
      jointpoints->SetPoint(joint*3+2,centroid[0],axislength+centroid[1],centroid[2]);


      
    }

  
  return jointpoints;
}

