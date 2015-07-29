

//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
//BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


#include "vtkmpjTensorGlyphs.h"

//#include <stdlib.h>

#include "vtkCell.h"
#include "vtkCellArray.h"
#include "vtkDataSet.h"
#include "vtkFloatArray.h"
#include "vtkDataArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkTransform.h"
#include "vtkmpjToroidalSource.h"
#include "vtkmpjToroidalExtSource.h"
#include "vtkmpjEllipsoidSource.h"
#include "vtkSphereSource.h"
#include "vtkmpjSuperquadricsSource.h"
#include "vtkmpjSuperToroidalSource.h"
#include "vtkmpjSuperToroidalExtSource.h"
#include "vtkTransformPolyDataFilter.h"

vtkStandardNewMacro(vtkmpjTensorGlyphs);

// Construct object with scaling on and scale factor 1.0. Eigenvalues are 
// extracted, glyphs are colored with input scalar data, and logarithmic
// scaling is turned off.
vtkmpjTensorGlyphs::vtkmpjTensorGlyphs()
{
  this->Scaling = 1;
  this->ScaleFactor = 1.0;
  this->ClampScaling = 0;
  this->MaxScaleFactor = 100;
  this->EigenvalueThreshold = 0.1;
  this->GlyphType = vtkmpjTensorGlyphs::GLYPH_SOURCE;
  this->UResolution = 10;
  this->VResolution = 10;
  this->N1 = 1;
  this->N2 = 1;
  this->Smoothness = 4.0;
  this->Smoothness2 = 4.0;
  this->UseConstantSmoothness = 1;
  this->UseConstantSmoothness2 = 1;
  this->Tubeness = 0.5;
  this->Inequality = 0;
  this->SwapPowers = 0;
  this->SmoothnessImage = NULL;
  this->SmoothnessImage2 = NULL;
}

vtkmpjTensorGlyphs::~vtkmpjTensorGlyphs()
{
  this->SetSource(NULL);
}

// ----------------------------------------------------------------------------
int vtkmpjTensorGlyphs::ClassifyEigenvalues(double w[3], double thresh)
{	
	double l1 = fabs(w[0]);
	double l2 = fabs(w[1]);
	double l3 = fabs(w[2]);

	//double l1l2 = l1-l2;
	//double l2l3 = l2-l3;

	double d12p = l2 / l1;
	double d23p = l3 / l2;

	int dissoc = 0;

	if (l1 > 0 && l2 > 0 && l3 > 0) {

		if (d12p >= thresh) {
			// L1 == L2
			if (d23p >= thresh) {
				// L1 == L2 == L3
				dissoc = 4;
			} else {
				// L1 == L2 > L3
				dissoc = 3;
			}
		} else
		{
			// L2 > L3
			if (d23p >= thresh) {
				// L1 > L2 == L3
				dissoc = 2;
			} else {
				// L1 > L2 > L3
				dissoc = 1;
			}
		}

	  } else 
		  dissoc = 0;

	  return dissoc;
}


vtkPolyData *vtkmpjTensorGlyphs::CreateNewSuperToroidExtGlyph(double alpha, double beta, double gamma, 
														   double XRadius, double YRadius, double ZRadius,
														   double N1, double N2, double Xi, int swap = 0)
{
  vtkmpjSuperToroidalExtSource *torus_src = vtkmpjSuperToroidalExtSource::New();  
  //vtkmpjToroidSource *torus_src = vtkmpjToroidSource::New();  
  torus_src->SetPhiResolution(this->UResolution);
  torus_src->SetThetaResolution(this->VResolution);
  torus_src->SetAlpha(alpha);
  torus_src->SetBeta(beta);	
  torus_src->SetGamma(gamma);
  torus_src->SetSize(1.0);
  torus_src->SetScale(XRadius,YRadius,ZRadius);
  torus_src->SetN1(N1);
  torus_src->SetN2(N2);
  torus_src->SetXi(Xi);
  torus_src->SetSwapAxes(swap);
  torus_src->SetNewSuperToroid(1);
	
  vtkTransform *torus_xfrm = vtkTransform::New();
  torus_xfrm->RotateY(90.0);

  vtkTransformPolyDataFilter *torus = vtkTransformPolyDataFilter::New();
  torus->SetInput(torus_src->GetOutput());
  torus->SetTransform(torus_xfrm);
  torus->Update();

  torus_xfrm->Delete();
  torus_src->Delete();

  return torus->GetOutput();
}

vtkPolyData *vtkmpjTensorGlyphs::CreateSuperToroidExtGlyph(double alpha, double beta, double gamma, 
														   double XRadius, double YRadius, double ZRadius,
														   double N1, double N2, double Xi, int swap = 0)
{
  vtkmpjSuperToroidalExtSource *torus_src = vtkmpjSuperToroidalExtSource::New();  
  //vtkmpjToroidSource *torus_src = vtkmpjToroidSource::New();  
  torus_src->SetPhiResolution(this->UResolution);
  torus_src->SetThetaResolution(this->VResolution);
  torus_src->SetAlpha(alpha);
  torus_src->SetBeta(beta);	
  torus_src->SetGamma(gamma);
  torus_src->SetSize(1.0);
  torus_src->SetScale(XRadius,YRadius,ZRadius);
  torus_src->SetN1(N1);
  torus_src->SetN2(N2);
  torus_src->SetXi(Xi);
  torus_src->SetSwapAxes(swap);
	
  vtkTransform *torus_xfrm = vtkTransform::New();
  torus_xfrm->RotateY(90.0);

  vtkTransformPolyDataFilter *torus = vtkTransformPolyDataFilter::New();
  torus->SetInput(torus_src->GetOutput());
  torus->SetTransform(torus_xfrm);
  torus->Update();

  torus_xfrm->Delete();
  torus_src->Delete();

  return torus->GetOutput();
}

vtkPolyData *vtkmpjTensorGlyphs::CreateSuperToroidGlyph(double alpha, double beta, double gamma, double N1, double N2)
{
  vtkmpjSuperToroidalSource *torus_src = vtkmpjSuperToroidalSource::New();  
  //vtkmpjToroidSource *torus_src = vtkmpjToroidSource::New();  
  torus_src->SetPhiResolution(this->UResolution);
  torus_src->SetThetaResolution(this->VResolution);
  torus_src->SetAlpha(alpha);
  torus_src->SetBeta(beta);	
  torus_src->SetGamma(gamma);
  torus_src->SetSize(1.0);
  torus_src->SetN1(N1);
  torus_src->SetN2(N2);
	
  vtkTransform *torus_xfrm = vtkTransform::New();
  torus_xfrm->RotateY(90.0);

  vtkTransformPolyDataFilter *torus = vtkTransformPolyDataFilter::New();
  torus->SetInput(torus_src->GetOutput());
  torus->SetTransform(torus_xfrm);
  torus->Update();

  torus_xfrm->Delete();
  torus_src->Delete();

  return torus->GetOutput();
}

vtkPolyData *vtkmpjTensorGlyphs::CreateToroidExtGlyph(double XRadius, double YRadius, double ZRadius, 
													  double alpha, double beta, double gamma)
{
  vtkmpjToroidalExtSource *torus_src = vtkmpjToroidalExtSource::New();  
  torus_src->SetPhiResolution(this->UResolution);
  torus_src->SetThetaResolution(this->VResolution);
  torus_src->SetAlpha(alpha);
  torus_src->SetBeta(beta);	
  torus_src->SetGamma(gamma);
  torus_src->SetScale(XRadius,YRadius,ZRadius);
  torus_src->SetSize(1.0);
	
  vtkTransform *torus_xfrm = vtkTransform::New();
  torus_xfrm->RotateY(90.0);

  vtkTransformPolyDataFilter *torus = vtkTransformPolyDataFilter::New();
  torus->SetInput(torus_src->GetOutput());
  torus->SetTransform(torus_xfrm);
  torus->Update();

  torus_xfrm->Delete();
  torus_src->Delete();

  return torus->GetOutput();
}

vtkPolyData *vtkmpjTensorGlyphs::CreateToroidGlyph(double alpha, double beta, double gamma)
{
  vtkmpjToroidalSource *torus_src = vtkmpjToroidalSource::New();  
  torus_src->SetPhiResolution(this->UResolution);
  torus_src->SetThetaResolution(this->VResolution);
  torus_src->SetAlpha(alpha);
  torus_src->SetBeta(beta);	
  torus_src->SetGamma(gamma);
  torus_src->SetSize(1.0);
	
  vtkTransform *torus_xfrm = vtkTransform::New();
  torus_xfrm->RotateY(90.0);

  vtkTransformPolyDataFilter *torus = vtkTransformPolyDataFilter::New();
  torus->SetInput(torus_src->GetOutput());
  torus->SetTransform(torus_xfrm);
  torus->Update();

  torus_xfrm->Delete();
  torus_src->Delete();

  return torus->GetOutput();
}

vtkPolyData *vtkmpjTensorGlyphs::CreateEllipsoidGlyph(double XRadius, double YRadius, double ZRadius)
{
  vtkSphereSource *ellipsoid_src = vtkSphereSource::New();
  ellipsoid_src->SetPhiResolution(this->UResolution);
  ellipsoid_src->SetThetaResolution(this->VResolution);
  ellipsoid_src->SetRadius(1.0);
	
  vtkTransform *ellipsoid_xfrm = vtkTransform::New();
  ellipsoid_xfrm->Scale(XRadius, YRadius, ZRadius);

  vtkTransformPolyDataFilter *ellipsoid = vtkTransformPolyDataFilter::New();
  ellipsoid->SetInput(ellipsoid_src->GetOutput());
  ellipsoid->SetTransform(ellipsoid_xfrm);
  ellipsoid->Update();

  ellipsoid_xfrm->Delete();
  ellipsoid_src->Delete();

  return ellipsoid->GetOutput();
}

vtkPolyData *vtkmpjTensorGlyphs::CreateSuperQuadricGlyph(double XRadius, double YRadius, double ZRadius,
														double Alpha, double Beta, int swap = 0)
{	
  vtkmpjSuperquadricsSource *squadric_src = vtkmpjSuperquadricsSource::New();
  squadric_src->SetPhiResolution(this->UResolution);
  squadric_src->SetThetaResolution(this->VResolution);
  squadric_src->SetPhiRoundness(Beta);
  squadric_src->SetThetaRoundness(Alpha);
  squadric_src->SetScale(XRadius,YRadius,ZRadius);
  squadric_src->SetSwapAxes(swap);
  squadric_src->SetSize(1.0);

  vtkTransform *squadric_xfrm = vtkTransform::New();
  //squadric_xfrm->RotateY(90.0);

  vtkTransformPolyDataFilter *squadric = vtkTransformPolyDataFilter::New();
  squadric->SetInput(squadric_src->GetOutput());
  squadric->SetTransform(squadric_xfrm);
  squadric->Update();

  squadric_xfrm->Delete();
  squadric_src->Delete();

  return squadric->GetOutput();
}

// ----------------------------------------------------------------------------
void vtkmpjTensorGlyphs::SortEigenvectorsByMagnitude(double w[3], double *v[3])
{
  int i, j, k;
  double tmp, tmp2;
  
  // sort eigenfunctions
  for (j=0; j<3; j++) 
    {
      k = j;
      tmp = w[k];
      tmp2 = fabs(tmp);
      for (i=j; i<3; i++)
	{
	  if (fabs(w[i]) >= tmp2) 
	    {
	      k = i;
	      tmp = w[k];
	      tmp2 = fabs(tmp);
	    }
	}
      if (k != j) 
	{
	  w[k] = w[j];
	  w[j] = tmp;
	  for (i=0; i<3; i++) 
	    {
	      tmp = v[i][j];
	      v[i][j] = v[i][k];
	      v[i][k] = tmp;
	    }
	}
    }
}

void vtkmpjTensorGlyphs::Execute()
{  
  vtkDataArray *inTensors;
  double *tensor;
  vtkDataArray *inScalars;
  vtkDataArray *inSmooth;
  vtkDataArray *inSmooth2;
  vtkIdType numPts, numSourcePts, numSourceCells, inPtId, i;
  int j;
  vtkPoints *sourcePts;
  vtkDataArray *sourceNormals;
  vtkCellArray *sourceCells, *cellsPoly=NULL, *cellsStrip=NULL;  
  vtkPoints *newPts;
  vtkDataArray *newScalars=NULL;
  vtkFloatArray *newNormals=NULL;
  double *x, s, smoothness, smoothness2;
  vtkTransform *trans;
  vtkCell *cell;
  vtkIdList *cellPts;
  int npts;
  vtkIdType *pts;
  vtkIdType ptIncr, cellId;
  vtkIdType subIncr;
  int numDirs, dir, eigen_dir, symmetric_dir;
  vtkMatrix4x4 *matrix;
  double *m[3], w[3], *v[3];
  double m0[3], m1[3], m2[3];
  double v0[3], v1[3], v2[3];
  double xv[3], yv[3], zv[3];
  float maxScale;

  vtkPointData *pd, *outPD;
  vtkDataSet *input = this->GetInput();
  vtkPolyData *output = this->GetOutput();

  // number of glyphs per point
  int numMaxGlyphs = 1;

  // select toroid if needed
  if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_TOROID) {
	  this->SetSource(CreateToroidGlyph(1.0,0.5,1.0));
  }

  // select toroid if needed
  if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_TOROID_EXT) {
	  this->SetSource(CreateToroidExtGlyph(1.0,1.0,1.0,1.0,0.5,1.0));
  }

  // select supertoroid if needed
  if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERTOROID) {
	  this->SetSource(CreateSuperToroidGlyph(1.0,0.5,1.0,1.0,1.0));
  }

  // select supertoroid ext if needed
  if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERTOROID_EXT) {
	  this->SetSource(CreateSuperToroidExtGlyph(1.0,0.5,1.0,1.0,1.0,1.0,1.0,1.0,0.5));
  }

  // select supertoroid ext if needed
  if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERTOROID_EXT2) {
	  this->SetSource(CreateSuperToroidExtGlyph(1.0,0.5,1.0,1.0,1.0,1.0,1.0,1.0,0.5));
  }

  // select supertoroid ext if needed
  if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERTOROID_EXT3) {
	  this->SetSource(CreateNewSuperToroidExtGlyph(1.0,0.5,1.0,1.0,1.0,1.0,1.0,1.0,0.5));
  }
  
   // select supertoroid ext if needed
  if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERTOROID_EXT4) {
	  this->SetSource(CreateNewSuperToroidExtGlyph(1.0,0.5,1.0,1.0,1.0,1.0,1.0,1.0,0.5));
  }

  // select ellipsoid if needed
  if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_ELLIPSOID) {
    this->SetSource(CreateEllipsoidGlyph(1.0,1.0,1.0));
  }

  // select superquadric if needed
  if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERQUADRIC) {
	  this->SetSource(CreateSuperQuadricGlyph(1.0,1.0,1.0,1.0,1.0,0));
  }

  if (this->GetSource() == NULL)
  {
     vtkErrorMacro("No source.");
	 return;
  }

  // list of points
  pts = new vtkIdType[this->GetSource()->GetMaxCellSize()];
  
  trans = vtkTransform::New();
  matrix = vtkMatrix4x4::New();
  
  // set up working matrices
  m[0] = m0; m[1] = m1; m[2] = m2; 
  v[0] = v0; v[1] = v1; v[2] = v2; 

  vtkDebugMacro(<<"Generating tensor glyphs");

  pd = input->GetPointData();
  outPD = output->GetPointData();
  inTensors = pd->GetTensors();
  inScalars = pd->GetScalars();
  inSmooth = this->SmoothnessImage->GetPointData()->GetScalars();
  inSmooth2 = this->SmoothnessImage2->GetPointData()->GetScalars();
  numPts = input->GetNumberOfPoints();
 
  if ( !inTensors || numPts < 1 )
  {
    vtkErrorMacro(<<"No tensor data to glyph!");
    return;
  }

  //
  // Allocate storage for output PolyData
  //
  sourcePts = this->GetSource()->GetPoints();
  numSourcePts = sourcePts->GetNumberOfPoints();
  numSourceCells = this->GetSource()->GetNumberOfCells();

  newPts = vtkPoints::New();
  newPts->Allocate(numPts*numSourcePts, numPts);

  // Setting up for calls to PolyData::InsertNextCell()  
  if ( (sourceCells=this->GetSource()->GetPolys())->GetNumberOfCells() > 0 )
    {
      cellsPoly = vtkCellArray::New();
      cellsPoly->Allocate(numPts*sourceCells->GetSize(),numPts);
      //output->SetPolys(cells);
      //cells->Delete();
	  //printf("in cells poly\n");
    }
  if ( (sourceCells=this->GetSource()->GetStrips())->GetNumberOfCells() > 0 )
    {
    cellsStrip = vtkCellArray::New();
    cellsStrip->Allocate(numPts*sourceCells->GetSize(),numPts);
    //output->SetStrips(cells);
    //cells->Delete();
	//printf("in strips poly\n");
    }

  // only copy scalar data through
  pd = this->GetSource()->GetPointData();

  //newScalars = vtkFloatArray::New();
  //newScalars->Allocate(inScalars->GetNumberOfComponents()*numPts*numSourcePts,numPts);
  
  newScalars = inScalars->NewInstance();
  newScalars->SetNumberOfComponents(inScalars->GetNumberOfComponents());
  newScalars->Allocate(inScalars->GetNumberOfComponents()*numPts*numSourcePts,numPts);

  if ( (sourceNormals = pd->GetNormals()) )
  {
    newNormals = vtkFloatArray::New();
    newNormals->SetNumberOfComponents(3);
    newNormals->Allocate(numPts*numSourcePts,numPts);
  }

  //
  // Traverse all Input points, transforming glyph at Source points
  //
  trans->PreMultiply();

  // initialize counter
  ptIncr = 0;

  for (inPtId=0; inPtId < numPts; inPtId++)
  {

    // Controls constant or variable ring smoothness
    if (this->UseConstantSmoothness) 
      smoothness = this->Smoothness;
    else
      smoothness = inSmooth->GetComponent(inPtId, 0);
    
    // Controls constant or variable cross-section smoothness
    if (this->UseConstantSmoothness2)
      smoothness2 = this->Smoothness2;
    else
      smoothness2 = inSmooth2->GetComponent(inPtId, 0);

    // Translation is postponed
    tensor = inTensors->GetTuple(inPtId);

    // compute orientation vectors and scale factors from tensor
    for (j=0; j<3; j++)
      for (i=0; i<3; i++)
	{
	  m[i][j] = tensor[i+3*j];
	}
    
	// diagonalize
    vtkMath::Jacobi(m, w, v);
    this->SortEigenvectorsByMagnitude(w,v);
    for (i=0; i<3; i++) {
      if (w[i] < 0) {
	v[0][i] = -v[0][i];
	v[1][i] = -v[1][i];
	v[2][i] = -v[2][i];
	w[i] = -w[i];
      }	   
    }

    //copy eigenvectors
    xv[0] = v[0][0]; xv[1] = v[1][0]; xv[2] = v[2][0];
    yv[0] = v[0][1]; yv[1] = v[1][1]; yv[2] = v[2][1];
    zv[0] = v[0][2]; zv[1] = v[1][2]; zv[2] = v[2][2];           
	
	// eigenvalue classification
	int eigen_dissoc = this->ClassifyEigenvalues(w, this->EigenvalueThreshold);
    
	// number of glyphs
	int numGlyphs = 1;

	if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_TOROID || this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERTOROID) {
		numGlyphs = (eigen_dissoc > 2) ? eigen_dissoc-1 : 1;
	}
 
	//ptIncr = numGlyphs * inPtId * numSourcePts;

    // compute scale factors
    w[0] *= this->ScaleFactor;
    w[1] *= this->ScaleFactor;
    w[2] *= this->ScaleFactor;
    
    if ( this->ClampScaling )
    {
		for (maxScale=0.0, i=0; i<3; i++)
        {
			if ( maxScale < fabs(w[i]) )
				maxScale = fabs(w[i]);
        }

		if ( maxScale > this->MaxScaleFactor )
        {
			maxScale = this->MaxScaleFactor / maxScale;
			for (i=0; i<3; i++)
				w[i] *= maxScale; //preserve overall shape of glyph
        }
     }

    // make sure scale is okay (non-zero) and scale data
    for (maxScale=0.0, i=0; i<3; i++)
    {
      if ( w[i] > maxScale )
		maxScale = w[i];

	  if ( maxScale == 0.0 )
		maxScale = 1.0;
      
	  for (i=0; i<3; i++)
		if ( w[i] == 0.0 )
			w[i] = maxScale * 1.0e-06;
     }

    // Now do the real work for each glyph

    //for (dir=0; dir < numDirs; dir++) 
	for(dir=0; dir<numGlyphs; dir++)
    {
      eigen_dir = dir;
       
      // Remove previous scales ...
      trans->Identity();

      // translate Source to Input point
      x = input->GetPoint(inPtId);
      trans->Translate(x[0], x[1], x[2]);

      // normalized eigenvectors rotate object for eigen direction 0
      matrix->Element[0][0] = xv[0];
      matrix->Element[0][1] = yv[0];
      matrix->Element[0][2] = zv[0];
      matrix->Element[1][0] = xv[1];
      matrix->Element[1][1] = yv[1];
      matrix->Element[1][2] = zv[1];
      matrix->Element[2][0] = xv[2];
      matrix->Element[2][1] = yv[2];
      matrix->Element[2][2] = zv[2];
      trans->Concatenate(matrix);
        
      if (eigen_dir == 1)
        trans->RotateZ(90.0);
        
      if (eigen_dir == 2)
        trans->RotateY(-90.0);       	  

	  if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_ELLIPSOID) {

			// recreate glyph
			//this->GetSource()->Delete();
			this->SetSource(CreateEllipsoidGlyph(w[0]/2.0,w[1]/2.0,w[2]/2.0));

			// update points and normals
			sourcePts = this->GetSource()->GetPoints();
			sourceNormals = this->GetSource()->GetPointData()->GetNormals();	

	  } else if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERQUADRIC) {
		
			double mw = w[0] + w[1] + w[2];
			double cl = (w[0] - w[1]) / mw;
			double cp = (2 * (w[1] - w[2])) / mw;
			double cs = (3 * w[2]) / mw;
			double alpha, beta; 

			int swap = 0;

			if (cl >= cp) {
				alpha = pow(1.0 - cp, this->Smoothness);
				beta = pow(1.0 - cl, this->Smoothness);
				swap = 0;
			} 
			else {			
				alpha = pow(1.0 - cl, this->Smoothness);
				beta = pow(1.0 - cp, this->Smoothness);				
				swap = 1;
			}

			// recreate glyph
			//this->GetSource()->Delete();
			this->SetSource(CreateSuperQuadricGlyph(w[0]/2.0,w[1]/2.0,w[2]/2.0,alpha,beta,swap));
			
			// update points and normals
			sourcePts = this->GetSource()->GetPoints();
			sourceNormals = this->GetSource()->GetPointData()->GetNormals();	

	  } else if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_TOROID) {
		  
			// compute factors
			double alpha = (2.0 * w[1] + w[2]) / 4.0;
			double beta = w[2] / 4.0;
			double gamma = w[0] / 2.0;
				
			// recreate glyph
			//this->GetSource()->Delete();

			this->SetSource(CreateToroidGlyph(alpha,beta,gamma));
			
			// update points and normals
			sourcePts = this->GetSource()->GetPoints();
			sourceNormals = this->GetSource()->GetPointData()->GetNormals();			
	  
	  } else if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_TOROID_EXT) {
		  
			// compute factors
		    double mw = w[0] + w[1] + w[2];
			double cl = (w[0] - w[1]) / mw;
			double cp = (2 * (w[1] - w[2])) / mw;
			double cs = (3 * w[2]) / mw;
			double alpha, beta; 
	
			// recreate glyph
			//this->GetSource()->Delete();
		
			this->SetSource(CreateToroidExtGlyph(w[0]/2.0,w[1]/2.0,w[2]/2.0,cl+cp,cs,cs));
			
			// update points and normals
			sourcePts = this->GetSource()->GetPoints();
			sourceNormals = this->GetSource()->GetPointData()->GetNormals();			
	  
	  } else if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERTOROID) {
	
		    // compute factors
			double alpha = (2.0 * w[1] + w[2]) / 4.0;
			double beta = w[2] / 4.0;
			double gamma = w[0] / 2.0;

			double mw = w[0] + w[1] + w[2];
			double cl = (w[0] - w[1]) / mw;
			double cp = (2 * (w[1] - w[2])) / mw;
			double cs = (3 * w[2]) / mw;
			double N1, N2;

			int swap = 0;

			if (cl >= cp) {
				N1 = pow(1.0 - cp, smoothness);
				N2 = pow(1.0 - cl, smoothness);
				swap = 0;
			} 
			else {			
				N1 = pow(1.0 - cl, smoothness);
				N2 = pow(1.0 - cp, smoothness);				
				swap = 1;
			}

			// recreate glyph
			//this->GetSource()->Delete();

			this->SetSource(CreateSuperToroidGlyph(alpha,beta,gamma,N1,N2));

			// update points and normals
			sourcePts = this->GetSource()->GetPoints();
			sourceNormals = this->GetSource()->GetPointData()->GetNormals();			
	  
	  } else if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERTOROID_EXT) {
	
		    // compute factors
			double alpha = (2.0 * w[1] + w[2]) / 4.0;
			double beta = w[2] / 4.0;
			double gamma = w[0] / 2.0;

			double mw = w[0] + w[1] + w[2];
			double cl = (w[0] - w[1]) / mw;
			double cp = (2 * (w[1] - w[2])) / mw;
			double cs = (3 * w[2]) / mw;
			double N1, N2;

			int swap = 0;

			if (cl >= cp) {
				N1 = pow(1.0 - cp, smoothness);
				N2 = pow(1.0 - cl, smoothness);				
			} 
			else {			
				N1 = pow(1.0 - cl, smoothness);
				N2 = pow(1.0 - cp, smoothness);												
			}

			// recreate glyph
			//this->GetSource()->Delete();
			
			this->SetSource(CreateSuperToroidExtGlyph(cl+cp, cs, cs, w[0]/2.0, w[1]/2.0, w[2]/2.0, 
				N1, N2, this->Tubeness, swap));

			// update points and normals
			sourcePts = this->GetSource()->GetPoints();
			sourceNormals = this->GetSource()->GetPointData()->GetNormals();			

		} else if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERTOROID_EXT2) {
	
		    // compute factors
			double alpha = (2.0 * w[1] + w[2]) / 4.0;
			double beta = w[2] / 4.0;
			double gamma = w[0] / 2.0;

			double mw = w[0] + w[1] + w[2];
			double cl = (w[0] - w[1]) / mw;
			double cp = (2 * (w[1] - w[2])) / mw;
			double cs = (3 * w[2]) / mw;
			double N1, N2;

			int swap = 0;

			if (cl >= cp) {
				N1 = pow(1.0 - cp, smoothness);
				N2 = pow(1.0 - cl, smoothness);
				swap = 0;
			} 
			else {			
				N1 = pow(1.0 - cl, smoothness);
				N2 = pow(1.0 - cp, smoothness);				
				swap = 1;
			}

			// recreate glyph
			//this->GetSource()->Delete();
			
			this->SetSource(CreateSuperToroidExtGlyph(cl+cp, cs, cs, w[0]/2.0, w[1]/2.0, w[2]/2.0, 
				N1, N2, this->Tubeness, swap));

			// update points and normals
			sourcePts = this->GetSource()->GetPoints();
			sourceNormals = this->GetSource()->GetPointData()->GetNormals();			

	  } else if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERTOROID_EXT3) {
	
		    // compute factors
			double alpha = (2.0 * w[1] + w[2]) / 4.0;
			double beta = w[2] / 4.0;
			double gamma = w[0] / 2.0;

			double mw = w[0] + w[1] + w[2];
			double cl = (w[0] - w[1]) / mw;
			double cp = (2 * (w[1] - w[2])) / mw;
			double cs = (3 * w[2]) / mw;
			double N1, N2;

                      
			int swap = 0;

                        if (this->Inequality == 0) {

			  alpha = cl + cp;
                          beta = cs;

			} else {
			  
			  if (cp <= cs) {
			    
			    alpha = cl + cp;
			    beta = cs;

			  } else {
			    
			    alpha = cs;
			    beta = cl + cp;

			  }
			}

                        if (this->SwapPowers == 0) {

			  N1 = pow(1.0 - cp, smoothness);
			  N2 = pow(1.0 - cl, smoothness2);

                        } else {

			  N1 = pow(1.0 - cl, smoothness2);
                          N2 = pow(1.0 - cp, smoothness);
			}
				
			// recreate glyph
			//this->GetSource()->Delete();			
                        
			this->SetSource(CreateNewSuperToroidExtGlyph(alpha, beta, cs, w[0]/2.0, w[1]/2.0, w[2]/2.0, 
				N1, N2, this->Tubeness, swap));

			// update points and normals
			sourcePts = this->GetSource()->GetPoints();
			sourceNormals = this->GetSource()->GetPointData()->GetNormals();
 
	  } else if (this->GlyphType == vtkmpjTensorGlyphs::GLYPH_SUPERTOROID_EXT4) {
	
		    // compute factors
			double alpha = (2.0 * w[1] + w[2]) / 4.0;
			double beta = w[2] / 4.0;
			double gamma = w[0] / 2.0;

			double mw = w[0] + w[1] + w[2];
			double cl = (w[0] - w[1]) / mw;
			double cp = (2 * (w[1] - w[2])) / mw;
			double cs = (3 * w[2]) / mw;
			double N1, N2;
			int swap = 0;

                        if (this->Inequality == 0) {

			  alpha = cl + cp;  // inner hole
                          beta = cs;

			} else {
			  
			  if (cp <= cs) {
			    
			    if (cl == 0) {
			      N1 = pow(1.0 - cl, smoothness2);
			      N2 = pow(1.0 - cp, smoothness);
			      alpha = 0;
			      beta = 1;//cl + cp;
			      swap = 1;

			    } else {
			      
			      N1 = pow(1.0 - cp, smoothness);
			      N2 = pow(1.0 - cl, smoothness2);
			      alpha = cl + cp;   // inner hole
			      beta = cs;
			      swap = 0;
			    }

			  } else {
			    
			    alpha = cs;   // inner hole
			    beta = cl + cp;
			    
			    if (cl == 0) {

			      N1 = pow(1.0 - cl, smoothness2);
			      N2 = pow(1.0 - cp, smoothness);
			      alpha = 0;
			      beta = 1;//cl + cp;
			      swap = 1;

			    } else {
			      N1 = pow(1.0 - cp, smoothness);
			      N2 = pow(1.0 - cl, smoothness2);
			      swap = 0;
			    }
			    
			  }
			}

			// recreate glyph
			//this->GetSource()->Delete();			
                        
			this->SetSource(CreateNewSuperToroidExtGlyph(alpha, beta, cs, w[0]/2.0, w[1]/2.0, w[2]/2.0, 
				N1, N2, this->Tubeness, swap));

			// update points and normals
			sourcePts = this->GetSource()->GetPoints();
			sourceNormals = this->GetSource()->GetPointData()->GetNormals();

		} else {

			// source glyph
			trans->Scale(w[0],w[1],w[2]);
		}
  
		//
		// Copy topology (transformation independent)
		//
		for (cellId=0; cellId < numSourceCells; cellId++)
		{
			cell = this->GetSource()->GetCell(cellId);
			cellPts = cell->GetPointIds();
			npts = cellPts->GetNumberOfIds();

			for (i=0; i < npts; i++)
				pts[i] = ptIncr + cellPts->GetId(i);
			
			//output->InsertNextCell(cell->GetCellType(),npts,pts);
			if ( cellsPoly )
				cellsPoly->InsertNextCell(npts,pts);

			if ( cellsStrip )
				cellsStrip->InsertNextCell(npts,pts);
		}
		
      // multiply points (and normals if available) by resulting
      // matrix
      trans->TransformPoints(sourcePts, newPts); 

      // Apply the transformation to a series of points, 
      // and append the results to outPts.
      if ( newNormals )
      {
        trans->TransformNormals(sourceNormals, newNormals);
      }
      
      // Copy point data from source
	for (i=0; i < numSourcePts; i++) 
	  {
	    //s = inScalars->GetComponent(inPtId, i);
	    //newScalars->InsertTuple(ptIncr+i, &s);
	    outPD->CopyTuple(inScalars, newScalars, inPtId, ptIncr+i);
	  }

	  ptIncr += numSourcePts;

    }  // for each direction

  } // for each point 

  vtkDebugMacro(<<"Generated " << numPts <<" tensor glyphs");
  
  //
  // Update output and release memory
  //
  delete [] pts;

  newPts->Squeeze();
  output->SetPoints(newPts);
  newPts->Delete();

  if ( cellsStrip ) {
	cellsStrip->Squeeze();
	output->SetStrips(cellsStrip);
	cellsStrip->Delete();
  }

  if ( cellsPoly ) {
	cellsPoly->Squeeze();
	output->SetPolys(cellsPoly);
	cellsPoly->Delete();
  }

  if ( newScalars )
  {
	newScalars->Squeeze();
    outPD->SetScalars(newScalars);
    newScalars->Delete();
  }

  if ( newNormals )
  {
    newNormals->Squeeze();
	outPD->SetNormals(newNormals);
    newNormals->Delete();
  }

  trans->Delete();
  matrix->Delete();

}

void vtkmpjTensorGlyphs::SetSource(vtkPolyData *source)
{
  this->vtkProcessObject::SetNthInput(1, source);
}

vtkPolyData *vtkmpjTensorGlyphs::GetSource()
{
  if (this->NumberOfInputs < 2)
    {
    return NULL;
    }
  return (vtkPolyData *)(this->Inputs[1]);
}

void vtkmpjTensorGlyphs::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "Scaling: " << (this->Scaling ? "On\n" : "Off\n");
  os << indent << "Scale Factor: " << this->ScaleFactor << "\n";
  os << indent << "Clamp Scaling: " << (this->ClampScaling ? "On\n" : "Off\n");
  os << indent << "Max Scale Factor: " << this->MaxScaleFactor << "\n";
  os << indent << "GlyphType: " << this->GlyphType << "\n";
}
