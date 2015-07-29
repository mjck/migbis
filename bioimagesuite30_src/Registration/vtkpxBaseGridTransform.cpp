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


#include "vtkpxBaseGridTransform.h"
#include "vtkObjectFactory.h"
#include "vtkMath.h"
#include "vtkDoubleArray.h"
#include "pxutil.h"
#include "math.h"
#include "vtkOutlineFilter.h"
#include "vtkTransformPolyDataFilter.h"

/// Size of lookup table for B-spline basis function values

/// Memory for lookup table for B-spline basis function values
int   vtkpxBaseGridTransform::LookupTableInitialized=0;
double vtkpxBaseGridTransform::LutSize=0.0;
double vtkpxBaseGridTransform::LookupTable[LookupTableSize+1][4];
double vtkpxBaseGridTransform::dLookupTable[LookupTableSize+1][4];

/*
#define ULT 1
*/

//----------------------------------------------------------------------------
inline int dround(double x)
{
  return x > 0 ? int(x + 0.5) : int(x - 0.5);
}

//----------------------------------------------------------------------------
vtkpxBaseGridTransform* vtkpxBaseGridTransform::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxBaseGridTransform");
  if(ret)
    {
    return (vtkpxBaseGridTransform*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxBaseGridTransform;
}
//----------------------------------------------------------------------------
const char *vtkpxBaseGridTransform::GetInterpolationModeAsString()
{
  switch (this->InterpolationMode)
    {
    case VTK_GRID_NEAREST:
      return "NearestNeighbor";
    case VTK_GRID_LINEAR:
      return "Linear";
    case VTK_GRID_CUBIC:
      return "Cubic";
    case VTK_GRID_BSPLINE:
      return "B-Spline";
    default:
      return "";
    }
}  
//----------------------------------------------------------------------------
double vtkpxBaseGridTransform::B(int i, double t)
{
  switch (i) 
    {
    case 0:
      return (1-t)*(1-t)*(1-t)/6.0;
      break;
      
    case 1:
      return (3*t*t*t - 6*t*t + 4)/6.0;
    case 2:
      return (-3*t*t*t + 3*t*t + 3*t + 1)/6.0;
      
    case 3:
      return (t*t*t)/6.0;
    }
  return 0;
}
//----------------------------------------------------------------------------
inline double vtkpxBaseGridTransform::B0(double t)
{
  return (1-t)*(1-t)*(1-t)/6.0;
}
//----------------------------------------------------------------------------
inline double vtkpxBaseGridTransform::B1(double t)
{
  return (3*t*t*t - 6*t*t + 4)/6.0;
}
//----------------------------------------------------------------------------
inline double vtkpxBaseGridTransform::B2(double t)
{
  return (-3*t*t*t + 3*t*t + 3*t + 1)/6.0;
}
//----------------------------------------------------------------------------
inline double vtkpxBaseGridTransform::B3(double t)
{
  return (t*t*t)/6.0;
}
//----------------------------------------------------------------------------
double vtkpxBaseGridTransform::dB(int i, double t)
{
  switch (i) 
    {
    case 0:
      return dB0(t);
    case 1:
      return dB1(t);
    case 2:
      return dB3(t);
    case 3:
      return dB3(t);
    }
  return 0;
}
//----------------------------------------------------------------------------
inline double vtkpxBaseGridTransform::dB0(double t)
{
  return -(1-t)*(1-t)/2.0;
}
//----------------------------------------------------------------------------
inline double vtkpxBaseGridTransform::dB1(double t)
{
  return (9*t*t - 12*t)/6.0;
}
//----------------------------------------------------------------------------
inline double vtkpxBaseGridTransform::dB2(double t)
{
  return (-9*t*t + 6*t + 3)/6.0;
}
//----------------------------------------------------------------------------
inline double vtkpxBaseGridTransform::dB3(double t)
{
  return (t*t)/2.0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Do B-Spline interpolation of the input data 'gridPtr' of extent 'gridExt' 
// at the 'point'.  The result is placed at 'outPtr'.  
// The number of scalar components in the data is 'numscalars'

// The tricubic interpolation ensures that both the intensity and
// the first derivative of the intensity are smooth across the
// image.  The first derivative is estimated using a 
// centered-difference calculation.


// helper function: set up the lookup indices and the interpolation 
// coefficients

// tricubic interpolation of a warp grid with derivatives
// (set derivatives to NULL to avoid computing them).

static inline int GridIrange(int a,int minv,int   maxv)
{
  if (a<minv) 
    return minv;
  if (a>maxv) 
    return maxv;
  return a;
}


template <class Type>
static inline void vtkBSplineHelper(double displacement[3], 
				    double derivatives[3][3],
				    int l,int m,int n,
				    int S,int T,int U,
				    double s,double t,double u,
				    Type *gridPtr, vtkIdType gridInc[3],int ext[6])
{
  int i, j, k,K,J,I,offset;
  double B_K, B_J, B_I;
  double v, dx,dy,dz;



  displacement[0]=0;
  displacement[1]=0;
  displacement[2]=0;

  if (!derivatives) 
    {
       for (k = 0; k < 4; k++)
	{
	  K = GridIrange(k + n - 1,ext[4],ext[5]);
	  B_K   = vtkpxBaseGridTransform::LookupTable[U][k];
	  for (j = 0; j < 4; j++)
	    {
	      J = GridIrange(j + m - 1,ext[2],ext[3]);
	      B_J   = vtkpxBaseGridTransform::LookupTable[T][j];
	      for (i = 0; i < 4; i++)
		{
		  I = GridIrange(i + l - 1,ext[0],ext[1]);

		  B_I   = vtkpxBaseGridTransform::LookupTable[S][i];
		  
		  offset=I*gridInc[0]+J*gridInc[1]+K*gridInc[2];
		  dx=((double) gridPtr[offset  ]);
		  dy=((double) gridPtr[offset+1]);
		  dz=((double) gridPtr[offset+2]);

		  v = B_I * B_J * B_K;
		  displacement[0] += dx * v;
		  displacement[1] += dy * v;
		  displacement[2] += dz * v;
		  
		} 
	    }
	}
    } 
  else 
    {
      double dB_K, dB_J, dB_I;
      for (i=0; i<3; i++) 
	for (j=0; j<3; j++) 
	  derivatives[i][j]=0.0;
    
      for (k = 0; k < 4; k++)
	{
	  K = GridIrange(k + n - 1,ext[4],ext[5]);
	  B_K   = vtkpxBaseGridTransform::LookupTable[U][k];
	  dB_K = vtkpxBaseGridTransform::dLookupTable[U][k];
	  for (j = 0; j < 4; j++)
	    {
	      J = GridIrange(j + m - 1,ext[2],ext[3]);
	      B_J   = vtkpxBaseGridTransform::LookupTable[T][j];
	      dB_J = vtkpxBaseGridTransform::dLookupTable[T][j];
	      for (i = 0; i < 4; i++)
		{
		I = GridIrange(i + l - 1,ext[0],ext[1]);
		B_I   = vtkpxBaseGridTransform::LookupTable[S][i];
		dB_I = vtkpxBaseGridTransform::dLookupTable[S][i];
		
		offset=I*gridInc[0]+J*gridInc[1]+K*gridInc[2];
		dx=((double) gridPtr[offset  ]);
		dy=((double) gridPtr[offset+1]);
		dz=((double) gridPtr[offset+2]);
		
		v = B_I * B_J * B_K;
		displacement[0] += dx * v;
		displacement[1] += dy * v;
		displacement[2] += dz * v;

		v = B_I * B_J * dB_K;
		derivatives[0][2] += dx * v;
		derivatives[1][2] += dy * v;
		derivatives[2][2] += dz * v;

		v = B_I * dB_J * B_K;
		derivatives[0][1] += dx * v;
		derivatives[1][1] += dy * v;
		derivatives[2][1] += dz * v;

		v = dB_I * B_J * B_K;
		derivatives[0][0] += dx * v;
		derivatives[1][0] += dy * v;
		derivatives[2][0] += dz * v;
		} 
	    }
	} 
    }
}


// point: point position in the coordinate system of the grid
// displacement: result of this function
// derivatives: in the first step NULL
// gridPtr: pointer to the displacement vectors of the grid
// gridType: Type of the grid, e.g. VTK_CHAR, VTK_DOUBLE
// gridExt: min and max index of the control points (can be negative)
// gridInc: grid increments
static void vtkBSplineInterpolation(double point[3],
				    double displacement[3], 
				    double derivatives[3][3],
				    void *gridPtr, int gridType, 
				    int gridExt[6], vtkIdType gridInc[3])
{

  double s, t, u;
  // double v, B_K, B_J, B_I;
  int l, m, n, S, T, U;

  double LUTSIZE=vtkpxBaseGridTransform::LutSize;

  // Xenios Papademetris
  // Fix floor BUG on Linux
  l = int(floor(point[0]+0.00001));
  m = int(floor(point[1]+0.00001));
  n = int(floor(point[2]+0.00001));

  // End of Fix
  s = point[0] - l;
  t = point[1] - m;
  u = point[2] - n;
  l -= gridExt[0];
  m -= gridExt[2];
  n -= gridExt[4];
  S = dround(LUTSIZE*s);
  T = dround(LUTSIZE*t);
  U = dround(LUTSIZE*u);

  int ext[6];

    
  ext[0] = -gridExt[0];
  ext[1] = gridExt[1];
  ext[2] = -gridExt[2];
  ext[3] = gridExt[3];
  ext[4] = -gridExt[4];
  ext[5] = gridExt[5];

  switch (gridType) 
    {
    case VTK_CHAR:
      vtkBSplineHelper(displacement, derivatives, 
		       l,m,n,S,T,U,s,t,u,
		       (char *)gridPtr,gridInc,ext);
      break;
    case VTK_UNSIGNED_CHAR:
      vtkBSplineHelper(displacement, derivatives, 
		       l,m,n,S,T,U,s,t,u,
		       (unsigned char *)gridPtr,gridInc,ext);
      break;
    case VTK_SHORT:
      vtkBSplineHelper(displacement, derivatives, 
		       l,m,n,S,T,U,s,t,u,
		       (short *)gridPtr,gridInc,ext);
      break;
    case VTK_UNSIGNED_SHORT:
      vtkBSplineHelper(displacement, derivatives, 
		       l,m,n,S,T,U,s,t,u, 
		     (unsigned short *)gridPtr,gridInc,ext);
      break;
    case VTK_DOUBLE:
      vtkBSplineHelper(displacement, derivatives, 
		       l,m,n,S,T,U,s,t,u,
		       (double *)gridPtr,gridInc,ext);
      break;
    }

}		  

//----------------------------------------------------------------------------
vtkpxBaseGridTransform::vtkpxBaseGridTransform()
{
  if (vtkpxBaseGridTransform::LookupTableInitialized==0)
    {
      vtkpxBaseGridTransform::LutSize=double(LookupTableSize);
      double LUTSIZE=vtkpxBaseGridTransform::LutSize;
      // Initialize lookup table
      // Arrays are of size LookupTableSize+1
      for (int i = 0; i <= LookupTableSize; i++)
	{
	  double s=double(i)/LUTSIZE;
	  vtkpxBaseGridTransform::LookupTable[i][0] = this->B0(s);
	  vtkpxBaseGridTransform::LookupTable[i][1] = this->B1(s);
	  vtkpxBaseGridTransform::LookupTable[i][2] = this->B2(s);
	  vtkpxBaseGridTransform::LookupTable[i][3] = this->B3(s);
	  vtkpxBaseGridTransform::dLookupTable[i][0] = this->dB0(s);
	  vtkpxBaseGridTransform::dLookupTable[i][1] = this->dB1(s);
	  vtkpxBaseGridTransform::dLookupTable[i][2] = this->dB2(s);
	  vtkpxBaseGridTransform::dLookupTable[i][3] = this->dB3(s);
	}
      vtkpxBaseGridTransform::LookupTableInitialized=1;
    }
  this->SetInterpolationModeToBSpline();


}

//----------------------------------------------------------------------------
void vtkpxBaseGridTransform::SetInterpolationMode(int mode)
{
  if (mode == this->InterpolationMode)
    {
      return;
    }
  this->InterpolationMode = mode;
  switch(mode)
    {
    case VTK_GRID_BSPLINE:
      this->InterpolationFunction = &vtkBSplineInterpolation;
      break;
    default:
      this->vtkGridTransform::SetInterpolationMode(mode);
    }
  this->Modified();
}
//----------------------------------------------------------------------------
vtkAbstractTransform *vtkpxBaseGridTransform::MakeTransform()
{
  return vtkpxBaseGridTransform::New();
}
//----------------------------------------------------------------------------
int vtkpxBaseGridTransform::Load(const char* fname)
{  
  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  int ok=Load(fin);
  gzclose(fin);

  return ok;
}

int vtkpxBaseGridTransform::Load(gzFile fin,int skipheader,const char* header)
{
  char pxtk_buffer[255];
  
  //  printf("hirohito:  int vtkpxBaseGridTransform::Load\n");
  if (skipheader==0)
    gzgets(fin,pxtk_buffer,100);
  else
    strncpy(pxtk_buffer,header,255);

  //  fprintf(stderr,"Header Line =%s\n",pxtk_buffer);

  int interpmode=0;
  if (gzfindstringinstring(pxtk_buffer,"#vtkpxBaseGridTransform File")==0)
    {
      if (gzfindstringinstring(pxtk_buffer,"#vtkpxBaseGridTransform2 File")==0)
	return 0;
      else
	interpmode=1;
    }

  double ori[3],spa[3];
  int   dim[3];
  gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
  sscanf(pxtk_buffer,"%lf %lf %lf",&ori[0],&ori[1],&ori[2]);
  gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
  sscanf(pxtk_buffer,"%lf %lf %lf",&spa[0],&spa[1],&spa[2]);
  gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
  sscanf(pxtk_buffer,"%d %d %d",&dim[0],&dim[1],&dim[2]);
  
  if (interpmode)
    {
      gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
      int tmp;
      sscanf(pxtk_buffer,"%d",&tmp);
      this->SetInterpolationMode(tmp);
    }
  else
    this->SetInterpolationModeToBSpline();

  //  printf("hirohito:  int vtkpxBaseGridTransform::Load setting grid\n");
  vtkImageData *grid = vtkImageData::New();
  grid->SetScalarType(VTK_DOUBLE);
  grid->SetSpacing(spa);
  grid->SetOrigin(ori);
  grid->SetNumberOfScalarComponents(3);
  grid->SetExtent(0, dim[0]-1, 0, dim[1]-1, 0, dim[2]-1);
  grid->SetWholeExtent(grid->GetExtent());
  grid->SetUpdateExtent(grid->GetWholeExtent());
  grid->AllocateScalars();
  
  // pointer to the start address of the whole image

  gzgets(fin,pxtk_buffer,100);  
  vtkDoubleArray* scalars=(vtkDoubleArray*)grid->GetPointData()->GetScalars();

  for (int ia=0;ia<=2;ia++)
    scalars->FillComponent(ia,0.0);

  int np=scalars->GetNumberOfTuples();
  int tmp=0;
  double dx[3];
  for (int i=0;i<np;i++)
    {
      gzgets(fin,pxtk_buffer,100);
      sscanf(pxtk_buffer,"%d %lf %lf %lf",&tmp,&dx[0],&dx[1],&dx[2]);
      scalars->SetTuple(i,dx);
    }


  this->SetDisplacementGrid(grid);
  return 1;
}
//----------------------------------------------------------------------------
int vtkpxBaseGridTransform::Save(const char* fname)
{
  return this->Save(fname,0);
}

int vtkpxBaseGridTransform::Save(const char* fname,int compression)
{
  if (compression==0)
    {
      FILE* fout=fopen(fname,"w");
      if (!fout)
	return 0;
      
      int ok=Save(fout);
      fclose(fout);
      return ok;
    } 

  gzFile fout=gzsuffixopen(fname,"wb",compression);
  if (!fout)
    return 0;
  int ok=Save(fout);
  gzclose(fout);
  return ok;
  
}

int vtkpxBaseGridTransform::Save(FILE* fout)
{

  vtkImageData *grid=this->GetDisplacementGrid();
  if (grid==NULL)
    return 0;
  double ori[3],spa[3];
  int   dim[3];

  grid->GetOrigin(ori);
  grid->GetSpacing(spa);
  grid->GetDimensions(dim);
  vtkDoubleArray* scalars=(vtkDoubleArray*)grid->GetPointData()->GetScalars();

  if (scalars==NULL)
    return 0;

  int np=scalars->GetNumberOfTuples();


  fprintf(fout,"#vtkpxBaseGridTransform2 File\n");
  fprintf(fout,"#Origin\n %.4f %.4f %.4f\n",ori[0],ori[1],ori[2]);
  fprintf(fout,"#Spacing\n %.4f %.4f %.4f\n",spa[0],spa[1],spa[2]);
  fprintf(fout,"#Dimensions\n %d %d %d\n",dim[0],dim[1],dim[2]);
  fprintf(fout,"#Interpolation Mode\n%d\n",this->InterpolationMode);
  fprintf(fout,"#Displacements\n");

  for (int i = 0; i < np;i++)
    fprintf(fout,"%d %.4f %.4f %.4f\n",i,
	     scalars->GetComponent(i,0),
	     scalars->GetComponent(i,1),
	     scalars->GetComponent(i,2));

  return 1;
}
//----------------------------------------------------------------------------
int vtkpxBaseGridTransform::Save(gzFile fout)
{

  vtkImageData *grid=this->GetDisplacementGrid();
  if (grid==NULL)
    return 0;
  double ori[3],spa[3];
  int   dim[3];

  grid->GetOrigin(ori);
  grid->GetSpacing(spa);
  grid->GetDimensions(dim);
  vtkDoubleArray* scalars=(vtkDoubleArray*)grid->GetPointData()->GetScalars();

  if (scalars==NULL)
    return 0;

  int np=scalars->GetNumberOfTuples();


  gzprintf(fout,"#vtkpxBaseGridTransform2 File\n");
  gzprintf(fout,"#Origin\n %.4f %.4f %.4f\n",ori[0],ori[1],ori[2]);
  gzprintf(fout,"#Spacing\n %.4f %.4f %.4f\n",spa[0],spa[1],spa[2]);
  gzprintf(fout,"#Dimensions\n %d %d %d\n",dim[0],dim[1],dim[2]);
  gzprintf(fout,"#Interpolation Mode\n%d\n",this->InterpolationMode);
  gzprintf(fout,"#Displacements\n");

  for (int i = 0; i < np;i++)
    gzprintf(fout,"%d %.4f %.4f %.4f\n",i,
	     scalars->GetComponent(i,0),
	     scalars->GetComponent(i,1),
	     scalars->GetComponent(i,2));

  return 1;
}
//----------------------------------------------------------------------------
void vtkpxBaseGridTransform::InitializeDisplacementGridTransform(double in_bounds[6],double spacing,double offset,
								 vtkAbstractTransform* tr)
{
  double bounds[6];

  if ( tr == NULL ) 
    {
      for (int ib=0;ib<=5;ib++)
	bounds[ib]=in_bounds[ib];
    }
  else
    {
      vtkImageData* img=vtkImageData::New();
      img->SetDimensions(2,2,2);
      img->SetOrigin(in_bounds[0],in_bounds[2],in_bounds[4]);
      img->SetSpacing(in_bounds[1],in_bounds[3],in_bounds[5]);
      img->AllocateScalars();

      vtkOutlineFilter* outline=vtkOutlineFilter::New();
      outline->SetInput(img);
      
      vtkTransformPolyDataFilter* transformFilter=vtkTransformPolyDataFilter::New();
      transformFilter->SetInput(outline->GetOutput());
      transformFilter->SetTransform(tr);
      transformFilter->Update();
      transformFilter->GetOutput()->GetBounds(bounds);
      transformFilter->Delete();
      outline->Delete();
      img->Delete();
    }

  for (int ia=0;ia<=2;ia++)
    {
      double diff=(bounds[2*ia+1]-bounds[2*ia]);
      bounds[2*ia]=bounds[2*ia]-offset*diff;
      bounds[2*ia+1]=bounds[2*ia]+(diff*(1.0+2.0*offset));
    }

  this->InitializeDisplacementGridBounds(bounds,spacing,spacing,spacing);
}
//----------------------------------------------------------------------------
void vtkpxBaseGridTransform::InitializeDisplacementGridBounds(double bounds[6],double dx, double dy, double dz)
{

  double _x1=bounds[0];  
  double _y1=bounds[2];  
  double _z1=bounds[4];
  double _x2=bounds[1];
  double _y2=bounds[3];
  double _z2=bounds[5];

  //fprintf(stderr,"\t ** Bounds %.1f:%.1f %.1f:%.1f %.1f:%.1f\n",_x1,_x2,_y1,_y2,_z1,_z2);

  // Intialize control point dimensions
  int _x = int((_x2 - _x1)/dx+1.5);
  int _y = int((_y2 - _y1)/dy+1.5);
  int _z = int((_z2 - _z1)/dz+1.5);

  if (_x<4) _x=4;
  if (_y<4) _y=4;
  if (_z<4) _z=4;
  
  //  fprintf(stderr,"\t ** Dim %d x %d x %d (dx=%.1f,dy=%.1f,dz=%.1f)\n",_x,_y,_z,dx,dy,dz);
  
  // Intialize control point spacing
  double _dx =  double(_x2 - _x1) / double(_x-1);
  double _dy =  double(_y2 - _y1) / double(_y-1) ;
  double _dz =  double(_z2 - _z1) / double(_z-1) ;

  /*  fprintf(stderr,"\t ** Spacing  (%.1f,%.1f,%.1f)\n",_dx,_dy,_dz);
      fprintf(stderr,"\t ** Origin  (%.1f,%.1f,%.1f)\n",_x1,_y1,_z1);*/

  vtkImageData *grid = vtkImageData::New();
  grid->SetScalarType(VTK_DOUBLE);
  grid->SetSpacing(_dx,_dy,_dz);
  grid->SetOrigin(_x1 , _y1 , _z1 );
  grid->SetNumberOfScalarComponents(3);
  grid->SetExtent(0, _x-1, 0, _y-1, 0, _z-1); 
  grid->SetUpdateExtent(grid->GetWholeExtent());
  grid->AllocateScalars();
  this->SetDisplacementGrid(grid);

  vtkDataArray* scal=(this->DisplacementGrid->GetPointData()->GetScalars());
  for (int i=0;i<=2;i++)
    scal->FillComponent(i,0.0);

}

int vtkpxBaseGridTransform::Export(const char* fname,const char* vname)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
    return 0;
  
    vtkImageData *grid=this->GetDisplacementGrid();
  if (grid==NULL)
    return 0;
  double ori[3],spa[3];
  int   dim[3];

  grid->GetOrigin(ori);
  grid->GetSpacing(spa);
  grid->GetDimensions(dim);
  vtkDoubleArray* scalars=(vtkDoubleArray*)grid->GetPointData()->GetScalars();

  if (scalars==NULL)
    return 0;

  int np=scalars->GetNumberOfTuples();


  fprintf(fout,"#vtkpxBaseGridTransform2 File\n");
  fprintf(fout,"# name: %s\n# type: matrix\n",vname);
  fprintf(fout,"# rows: %d\n# columns: 1\n",np*3);

  for (int i = 0; i < np;i++)
    fprintf(fout,"%.4f\n%.4f\n%.4f\n",
	     scalars->GetComponent(i,0),
	     scalars->GetComponent(i,1),
	     scalars->GetComponent(i,2));

 
  fclose(fout);
  return 1;
}

int vtkpxBaseGridTransform::Import(const char* fname)
{
  vtkImageData *grid=this->GetDisplacementGrid();
  if (grid==NULL)
    return 0;
  
  int dim[3]; grid->GetDimensions(dim);
  vtkDoubleArray* scalars=(vtkDoubleArray*)grid->GetPointData()->GetScalars();

  if (scalars==NULL)
    return 0;


  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  
  int np=scalars->GetNumberOfTuples();
  char pxtk_buffer[255];
  gzgets(fin,pxtk_buffer,100);
  gzgets(fin,pxtk_buffer,100);
  gzgets(fin,pxtk_buffer,100);
  gzgets(fin,pxtk_buffer,100);
  gzgets(fin,pxtk_buffer,100);

  for (int i = 0; i < np;i++)
    for (int j = 0 ; j <=2;j++)
      {
	  gzgets(fin,pxtk_buffer,100);
	  double v;
	  sscanf(pxtk_buffer,"%lf",&v);
	  scalars->SetComponent(i,j,v);
      }

  gzclose(fin);
  return 1;
}


#ifdef ULT 
#undef ULT 
#endif


