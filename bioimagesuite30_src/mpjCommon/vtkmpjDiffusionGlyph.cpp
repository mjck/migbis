

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


#include "vtkmpjDiffusionGlyph.h"
#include "vtkPointSet.h"
#include "vtkDelaunay3D.h"
#include "vtkUnstructuredGrid.h"

vtkStandardNewMacro(vtkmpjDiffusionGlyph);

// Construct object with scaling on and scale factor 1.0. Eigenvalues are 
// extracted, glyphs are colored with input scalar data, and logarithmic
// scaling is turned off.
vtkmpjDiffusionGlyph::vtkmpjDiffusionGlyph()
{
  this->ScaleFactor = 1.0;
  this->ColorGlyphs = 1;
  this->ColorMode = COLOR_BY_SCALARS;
  this->ClampScaling = 0;
  this->MaxScaleFactor = 100;
  this->NumberOfDirections = 6;
  this->Gradients = NULL;
  this->DiffusionName = NULL;
}

vtkmpjDiffusionGlyph::~vtkmpjDiffusionGlyph()
{
  //  this->SetSource(NULL);
  if (this->Gradients != NULL)
    this->Gradients->Delete();
  if (this->DiffusionName)
    delete [] DiffusionName;
}

void vtkmpjDiffusionGlyph::InterpolateH(int number, vtkIdType param[20][20], int *param_count,
					vtkPoints *newPts, vtkCellArray *newCells,
					vtkFloatArray *newScalars)
{
  int numPts = newPts->GetNumberOfPoints();
  int very_first_col = 0;
  int first_col = 0;
  int next_col = 0;
  int last_col = 0;
  vtkIdType index_first = 0;
  vtkIdType index_last = 0;
  float lastP[3], firstP[3];
  int stop;
  
  // for all rows
  for(int i=0; i<number; i++) {
    
    printf("i=%d\n",i);

    // Case of a single point
    if (param_count[i] == 1) {
      
      // find first column
      for(int j=0;j<number;j++)
	if (param[j][i] >= 0) {
	  very_first_col = j;
	  index_first = param[j][i];
	  break;
	}      

      // repeat for all columns
      for(int j=0;j<number;j++) 
	param[j][i] = index_first;

      return;
    }

    if (param_count[i] > 1) {
      
      // Step 1: Interpolate between fisrt and last columns
      
      // find first column
      for(int j=0;j<number;j++)
	if (param[j][i] >= 0) {
	  first_col = j;
	  index_first = param[j][i];
	  break;
	}
      
      // find last column
      for(int j=number-1;j>=0;j--)
	if (param[j][i] >= 0) {
	  last_col = j;
	  index_last = param[j][i];
	  break;
	}
      
      int total = (number-1) - last_col + first_col;
      
      // in case there are points to interpolate
      if (total > 0) {
	
	int count = 1;
	float w = 1.0 / total;
	float *firstP = newPts->GetPoint(index_first);
	float *lastP = newPts->GetPoint(index_last);
	float newP[3];
	
	for(int k=0; k<3; k++) 
	  lastP[k] =- firstP[k];
	
	count = 1;
	// interpolate at the end
	for(int j=last_col; j<number; j++) {
	  for(int k=0;k<3;k++) 
	    newP[k] = firstP[k] + (w*((float)count)) * lastP[k];
	  param[j][i] = newPts->InsertNextPoint(newP);
	  count++;
	}
	
	// interpolate at the beginning
	for(int j=0;j<first_col;j++) {
	  for(int k=0;k<3;k++) 
	    newP[k] = firstP[k] + (w*((float)count)) * lastP[k];
	  param[j][i] = newPts->InsertNextPoint(newP);
	  count++;
	}
      }

      printf("jere!\n");

      // interpolate middle
      do {
	
	stop=1;
	
	printf("z0\n");

	// find next column
	for(int j=first_col+1;j<last_col;j++)
	  if (param[j][i] >= 0) {
	    next_col = j;
	    stop = 0;
	    break;
	  }
	
	if (!stop) {
	  
	  // interpolate 
	  if ((next_col - first_col) > 1) {
	    
	    printf("a-1\n");

	    index_first = param[first_col][i];
	    index_last = param[next_col][i];

	    printf("a0\n");

	    int count = 1;
	    int total = (next_col - first_col) - 1;
	    float w = 1.0 / total;
	    float *firstP = newPts->GetPoint(index_first);
	    float *nextP = newPts->GetPoint(index_last);
	    float newP[3];
	    
	    printf("a %x %x\n",firstP,nextP);
	    printf("first=%d\n",index_first);
	    printf("last=%d\n",index_last);

	    for(int k=0; k<3; k++) 
	      nextP[k] =- firstP[k];

	    printf("after-a\n");

	    count = 1;

	    printf("next_col=%d\n",next_col);
	    printf("first_col=%d\n",first_col);

	    
	    // interpolate at the end
	    for(int j=first_col+1; j<next_col; j++) {
	      for(int k=0;k<3;k++) 
		newP[k] = firstP[k] + (w*((float)count)) * nextP[k];
	      param[j][i] = newPts->InsertNextPoint(newP);
	      count++;
	    }

	    printf("b\n");
	  }
	  
	  first_col = next_col;
	}
	
      } while(!stop);            
    }
  }
  
}

void vtkmpjDiffusionGlyph::InterpolateV(int number, vtkIdType param[20][20], int *param_count,
					vtkPoints *newPts, vtkCellArray *newCells,
					vtkFloatArray *newScalars)
{
  int numPts = newPts->GetNumberOfPoints();

}

void vtkmpjDiffusionGlyph::Execute()
{
  // vtkDataArray *inScalars;
//   vtkIdType numPts, numSourcePts, numSourceCells, inPtId, i;
//   int j;
//   vtkPoints *sourcePts;
//   vtkDataArray *sourceNormals;
//   vtkCellArray *sourceCells, *cells;  
//   vtkPoints *newPts;
//   vtkFloatArray *newScalars=NULL;
//   vtkFloatArray *newNormals=NULL;
//   float *x, s;
//   vtkTransform *trans;
//   vtkCell *cell;
//   vtkIdList *cellPts;
//   int npts;
//   vtkIdType *pts;
//   vtkIdType ptIncr, cellId;
//   vtkIdType subIncr;
//   int numDirs, dir, eigen_dir, symmetric_dir;
//   vtkMatrix4x4 *matrix;
//   float *m[3], w[3], *v[3];
//   float m0[3], m1[3], m2[3];
//   float v0[3], v1[3], v2[3];
//   float xv[3], yv[3], zv[3];
//   float maxScale;
//   vtkPointData *pd, *outPD;
  
  vtkTransform *trans = vtkTransform::New();

  float *vector;
  vtkDataSet *input = this->GetInput();
  vtkPolyData *output = this->GetOutput();
  
  //if (this->GetSource() == NULL)
  // {
  //vtkErrorMacro("No source.");
  //return;
  //}

  //numDirs = 1;//(this->ThreeGlyphs?3:1)*(this->Symmetric+1);

  
  printf("before\n");

  vtkPointData *pd = input->GetPointData();
  vtkPointData *outPD = output->GetPointData();
  vtkDataArray *inScalars = pd->GetScalars();

  vtkIdType numPts = input->GetNumberOfPoints();

  vtkIdType numComp = inScalars->GetNumberOfComponents();
  vtkIdType numTuples = inScalars->GetNumberOfTuples();

  printf("number of points=%d\n",numPts);
  printf("number of tuples=%d\n",numTuples);
  printf("number of components=%d\n",numComp);

  //printf("number of components=%d\n",numTuples);
  //  printf("number of vector points=%d\n",numTuples);

  // printf("number of components=%d\n",numComp);

  // Allocate storage for output PolyData
  //
  // sourcePts = this->GetSource()->GetPoints();
  //numSourcePts = sourcePts->GetNumberOfPoints();
  //numSourceCells = this->GetSource()->GetNumberOfCells();

  vtkDelaunay3D *delaunay = vtkDelaunay3D::New();
  delaunay->SetOffset(2.0);
  //delaunay->SetTolerance(0.001);
  //delaunay->SetAlpha(1.0);

  vtkPolyData *pointset = vtkPolyData::New();

  vtkPoints *newPts = vtkPoints::New();  
  vtkPoints *srcPts = vtkPoints::New();
  vtkCellArray *newCells = vtkCellArray::New();
  vtkFloatArray *newScalars = vtkFloatArray::New();
  newScalars->SetNumberOfComponents(1);
  //  newScalars->SetNumberOfTuples(numPts*2);

  //newPts->Allocate(numDirs*numPts*numSourcePts);
  //
  // Traverse all Input points, transforming glyph at Source points
  //
  //trans->PreMultiply();

  int index=0;
  float max_adc = -VTK_LARGE_FLOAT;
  float *norm_adc = new float[numComp-1];
  vtkIdType param[20][20];
  int param_count[20];
  int num_points=0;

  for (vtkIdType inPtId=0; inPtId < numPts; inPtId++)    
    {	  
      // populate vector
      vector = inScalars->GetTuple(inPtId);

      float *x = input->GetPoint(inPtId);

      // reset parametric map
      for (int i=0;i<20;i++) 
	for (int j=0;j<20;j++) {
	  param_count[i]=0;
	  param[i][j]=-1;
	};
      
      // determine max & make a new copy of adcs
      max_adc = -VTK_LARGE_FLOAT;
      for (int i=1; i<numComp; i++) {
	norm_adc[i-1] = vector[i];
	if (vector[i] > max_adc) {
	  max_adc = vector[i];
	}	
      }
      
      // compute normalized adc
      for (int i=0; i<(numComp-1); i++) {
	norm_adc[i] /= max_adc;
      }
      
      float p0[3]={x[0],x[1],x[2]};
      float p1[3]={1,1,1};
      float phi, theta, S;
      int row, col;
      
      // For every direction in this voxel
      for (int i=0; i<(numComp-1); i++) {
	
	// compute profile point
	float *dir = this->Gradients->GetTuple(i);
	(void) vtkMath::Normalize(dir);
	
	p1[0] = dir[0] * norm_adc[i] + x[0];
	p1[1] = dir[1] * norm_adc[i] + x[1];
	p1[2] = dir[2] * norm_adc[i] + x[2];
	
	// calculate spherical coordinates
	theta = atan(dir[1] / dir[0]);
	if (dir[0] < 0) {
	  theta += vtkMath::Pi();
	} else {
	  if (dir[1] < 0) {
	    theta = vtkMath::Pi()*2 + theta;
	  }
	}
	phi = acos(dir[2]);
	row = (int) ((19.0 / vtkMath::Pi()) * phi);
	col = (int) ((19.0 / (2*vtkMath::Pi())) * theta);
	
	//printf("%d: (%f,%f) - (%f %f %f) (%d %d)\n",i,theta,phi,dir[0],dir[1],dir[2],col,row);

	if (param[col][row] < 0) {
	  newPts->InsertNextPoint(p1); 
	  newScalars->InsertNextTuple(&vector[0]);
	  param[col][row] = num_points++;
	  param_count[row]++;
	}


	// allocate next cell
	//newCells->InsertNextCell(2);
	
	// insert next point
	//newCells->InsertCellPoint(newPts->InsertNextPoint(p0)); num_points++;
	//newScalars->InsertNextTuple(&vector[0]);
	
	//newCells->InsertCellPoint(newPts->InsertNextPoint(p1)); num_points++;
	//newScalars->InsertNextTuple(&vector[0]);

	p1[0] = -dir[0] * norm_adc[i] + x[0];
	p1[1] = -dir[1] * norm_adc[i] + x[1];
	p1[2] = -dir[2] * norm_adc[i] + x[2];

	// calculate spherical coordinates
	dir[2] = (dir[2] != 0.0) ? -dir[2] : dir[2];
	dir[1] = (dir[1] != 0.0) ? -dir[1] : dir[1];
	dir[0] = (dir[0] != 0.0) ? -dir[0] : dir[0];

	phi = acos(dir[2]);
	theta = atan(dir[1] / dir[0]);
	if (dir[0] < 0) {
	  theta += vtkMath::Pi();
	} else {
	  if (dir[1] < 0) {
	    theta = vtkMath::Pi()*2 + theta;
	  }
	}	
	row = (int) ((19.0 / vtkMath::Pi()) * phi);
	col = (int) ((19.0 / (2*vtkMath::Pi())) * theta);
	
	if (param[col][row] < 0) {
	  newPts->InsertNextPoint(p1); 
	  newScalars->InsertNextTuple(&vector[0]);
	  param[col][row] = num_points++;
	  param_count[row]++;
	}

	//printf("%d: (%f,%f)-(%f %f %f):(%d %d)\n",i,theta,phi,dir[0],dir[1],dir[2],col,row);
	
	// allocate next cell
	//newCells->InsertNextCell(2);
	
	// insert next point
	//newCells->InsertCellPoint(newPts->InsertNextPoint(p0));
	//newScalars->InsertNextTuple(&vector[0]);
	
	//newCells->InsertCellPoint(newPts->InsertNextPoint(p1));
	//newScalars->InsertNextTuple(&vector[0]);
		
      }

      this->InterpolateH(20, param, param_count, newPts, newCells, newScalars);
      this->InterpolateV(20, param, param_count, newPts, newCells, newScalars);

      // create connectivity information
      for(int i=0;i<20;i++) {
	printf("count[%d]=%d\n",i,param_count[i]);
	if (param_count[i] > 0) {
	  
	  newCells->InsertNextCell(21);
	  
	  for(int j=0;j<20;j++) 
	    if (param[j][i] >= 0)
	      newCells->InsertCellPoint(param[j][i]);
	  
	  newCells->InsertCellPoint(param[0][i]);
	  
	}
      }
      
    }      
  
  
  delete norm_adc;
  
  outPD->SetScalars(newScalars);
  newScalars->Delete();
  
  output->SetPoints(newPts);
  newPts->Delete();
  
  output->SetLines(newCells);
  newCells->Delete();
  
  //output->Squeeze();
  trans->Delete();
  delaunay->Delete();
 
  //matrix->Delete();
}

void vtkmpjDiffusionGlyph::SetSource(vtkPolyData *source)
{
  this->vtkProcessObject::SetNthInput(1, source);
}

vtkPolyData *vtkmpjDiffusionGlyph::GetSource()
{
  if (this->NumberOfInputs < 2)
    {
      return NULL;
    }
  return (vtkPolyData *)(this->Inputs[1]);
}

void vtkmpjDiffusionGlyph::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  
  os << indent << "Scale Factor: " << this->ScaleFactor << "\n";
  os << indent << "Color Glyphs: " << (this->ColorGlyphs ? "On\n" : "Off\n");
  os << indent << "Color Mode: " << this->ColorMode << endl;
  os << indent << "Clamp Scaling: " << (this->ClampScaling ? "On\n" : "Off\n");
  os << indent << "Max Scale Factor: " << this->MaxScaleFactor << "\n";
}
