
#include "vtkmpjSuperToroidalSource.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"

#include <math.h>

static const double SQ_SMALL_OFFSET = 0.01;

vtkStandardNewMacro(vtkmpjSuperToroidalSource);

//----------------------------------------------------------------------------
double vtkmpjSuperToroidalSource::Power ( double x, double n )
{
  if ( x == 0 )
    {
    return 0;
    }
  if ( x < 0 )
    {
    return -pow(-x,n);
    }
  else
    {
    return pow(x,n);
    }
}

//----------------------------------------------------------------------------
void vtkmpjSuperToroidalSource::Evaluate(double theta, double phi,  
                             double dtheta, double dphi,                       
                             double dims[3],                            
                             double xyz[3],      
                             double nrm[3])     
{
  double cu = cos(phi);
  double su = sin(phi);
  double cv = cos(theta);
  double sv = sin(theta);
  double t = this->Alpha + this->Beta * this->Power(cv,this->N2);

  double cu2 = cos(phi+dphi);
  double cv2 = cos(theta+dtheta);
  double su2 = sin(phi+dphi);
  double sv2 = sin(theta+dtheta);

  double Du[3];
  double Dv[3];

  xyz[0] = t * this->Power(cu,this->N1);
  xyz[1] = t * this->Power(su,this->N1);
  xyz[2] = this->Gamma * this->Power(sv,this->N2);

  // The derivatives are:
  //Du[0] = -t * su;
  //Du[1] = t * cu;
  //Du[2] = 0;
  //Dv[0] = -this->Beta * sv * cu;
  //Dv[1] = -this->Beta * sv * su;
  //Dv[2] = this->Gamma * cv;

  //vtkMath::Cross(Du,Dv,nrm);

  nrm[0] = nrm[1] = nrm[2] = 0;
  
  nrm[0] = 1.0 / (this->Alpha + this->Beta) * cv2 * cu2;
  nrm[1] = 1.0 / (this->Alpha + this->Beta) * cv2 * su2;
  nrm[2] = 1.0 / this->Gamma * sv2;
}


vtkmpjSuperToroidalSource::vtkmpjSuperToroidalSource(int res)
{
  res = res < 4 ? 4 : res;

  this->Center[0] = this->Center[1] = this->Center[2] = 0.0;
  this->Scale[0] = this->Scale[1] = this->Scale[2] = 1.0;
  this->Size = .5;
  this->ThetaResolution = 0;
  this->SetThetaResolution(res);
  this->PhiResolution = 0;
  this->SetPhiResolution(res);
	
  this->Alpha = 1;
  this->Beta = 0.5;
  this->Gamma = 0.5;

  this->N1 = 1;
  this->N2 = 1;
}

void vtkmpjSuperToroidalSource::SetPhiResolution(int i)
{
  if(i < 4)
    {
    i = 4;
    }
  i = (i+3)/4*4;  // make it divisible by 4
  if(i > VTK_MAX_SUPERQUADRIC_RESOLUTION)
    {
    i =  VTK_MAX_SUPERQUADRIC_RESOLUTION;
    }
  
  if (this->PhiResolution != i)
    {
    this->PhiResolution = i;
    this->Modified ();
    }
}

void vtkmpjSuperToroidalSource::SetThetaResolution(int i)
{
  if(i < 8)
    {
    i = 8;
    }
  i = (i+7)/8*8; // make it divisible by 8
  if(i > VTK_MAX_SUPERQUADRIC_RESOLUTION)
    {
    i =  VTK_MAX_SUPERQUADRIC_RESOLUTION;
    }
  
  if (this->ThetaResolution != i)
    {
    this->ThetaResolution = i;
    this->Modified ();
    }
}

void vtkmpjSuperToroidalSource::Execute()
{
  // Get the output
  vtkPolyData *output = this->GetOutput();

  int i, j;
  vtkIdType numPts;
  vtkPoints *newPoints;
  vtkFloatArray *newNormals;
  vtkFloatArray *newTCoords;
  vtkCellArray *newPolys;
  vtkIdType *ptidx;
  double pt[3], nv[3], dims[3];
  double len;
  double deltaPhi, deltaTheta, phi, theta;
  double phiLim[2], thetaLim[2];
  double deltaPhiTex, deltaThetaTex;
  int base, pbase;
  vtkIdType numStrips;
  int ptsPerStrip;
  int phiSubsegs, thetaSubsegs, phiSegs, thetaSegs;
  int iq, jq, rowOffset;
  double thetaOffset, phiOffset;
  double texCoord[2];
  double tmp;

  dims[0] = this->Scale[0]*this->Size;
  dims[1] = this->Scale[1]*this->Size;
  dims[2] = this->Scale[2]*this->Size;

  phiLim[0] = -vtkMath::Pi();
  phiLim[1] =  vtkMath::Pi();
  thetaLim[0] = -vtkMath::Pi();
  thetaLim[1] =  vtkMath::Pi();

  deltaPhi = (phiLim[1] - phiLim[0]) / this->PhiResolution;
  deltaPhiTex = 1.0 / this->PhiResolution;
  deltaTheta = (thetaLim[1] - thetaLim[0]) / this->ThetaResolution;
  deltaThetaTex = 1.0 / this->ThetaResolution;

  phiSegs = 4;
  thetaSegs = 8;

  phiSubsegs = this->PhiResolution / phiSegs;
  thetaSubsegs = this->ThetaResolution / thetaSegs;

  numPts = (this->PhiResolution + phiSegs)*(this->ThetaResolution + thetaSegs);
  // creating triangles
  numStrips = this->PhiResolution * thetaSegs;
  ptsPerStrip = thetaSubsegs*2 + 2;

  //
  // Set things up; allocate memory
  //
  newPoints = vtkPoints::New();
  newPoints->Allocate(numPts);
  newNormals = vtkFloatArray::New();
  newNormals->SetNumberOfComponents(3);
  newNormals->Allocate(3*numPts);
  newNormals->SetName("Normals");
  newTCoords = vtkFloatArray::New();
  newTCoords->SetNumberOfComponents(2);
  newTCoords->Allocate(2*numPts);
  newTCoords->SetName("TextureCoords");

  newPolys = vtkCellArray::New();
  newPolys->Allocate(newPolys->EstimateSize(numStrips,ptsPerStrip));

  // generate!
  for(iq = 0; iq < phiSegs; iq++)
    {
    for(i = 0; i <= phiSubsegs; i++)
      {
      phi = phiLim[0] + deltaPhi*(i + iq*phiSubsegs);
      texCoord[1] = deltaPhiTex*(i + iq*phiSubsegs);

      // SQ_SMALL_OFFSET makes sure that the normal vector isn't
      // evaluated exactly on a crease;  if that were to happen,
      // large shading errors can occur.
      if(i == 0)
        {
        phiOffset =  SQ_SMALL_OFFSET*deltaPhi;
        }
      else if (i == phiSubsegs)
        {
        phiOffset = -SQ_SMALL_OFFSET*deltaPhi;
        }
      else
        {
        phiOffset =  0.0;
        }

      for(jq = 0; jq < thetaSegs; jq++)
        {
        for(j = 0; j <= thetaSubsegs; j++)
          {
          theta = thetaLim[0] + deltaTheta*(j + jq*thetaSubsegs);
          texCoord[0] = deltaThetaTex*(j + jq*thetaSubsegs);

          if(j == 0)
            {
            thetaOffset =  SQ_SMALL_OFFSET*deltaTheta;
            }
          else if (j == thetaSubsegs)
            {
            thetaOffset = -SQ_SMALL_OFFSET*deltaTheta;
            }
          else
            {
            thetaOffset =  0.0;
            }

          //This give you superquadric with axis of symmetry: z
          this->Evaluate(theta, phi,
                         thetaOffset, phiOffset,                       
                         dims, pt, nv);

          
          if((len = vtkMath::Norm(nv)) == 0.0)
            {
            len = 1.0;
            }
          nv[0] /= len; nv[1] /= len; nv[2] /= len;

          //if(!this->Toroidal &&
            // ((iq == 0 && i == 0) || (iq == (phiSegs-1) && i == phiSubsegs)))
            //{

            // we're at a pole:
            // make sure the pole is at the same location for all evals
            // (the superquadric evaluation is numerically unstable
            // at the poles)

            //pt[0] = pt[2] = 0.0;
          //}

          pt[0] += this->Center[0];
          pt[1] += this->Center[1];
          pt[2] += this->Center[2];

          newPoints->InsertNextPoint(pt);
          newNormals->InsertNextTuple(nv);
          newTCoords->InsertNextTuple(texCoord);
          }
        }
      }
    }

  // mesh!
  // build triangle strips for efficiency....
  ptidx = new vtkIdType[ptsPerStrip];

  rowOffset = this->ThetaResolution+thetaSegs;

  for(iq = 0; iq < phiSegs; iq++)
    {
    for(i = 0; i < phiSubsegs; i++)
      {
      pbase = rowOffset*(i +iq*(phiSubsegs+1));
      for(jq = 0; jq < thetaSegs; jq++)
        {
        base = pbase + jq*(thetaSubsegs+1);
        for(j = 0; j <= thetaSubsegs; j++)
          {
          ptidx[2*j] = base + rowOffset + j;
          ptidx[2*j+1] = base + j;
          }
        newPolys->InsertNextCell(ptsPerStrip, ptidx);
        }
      }
    }
  delete[] ptidx;

  output->SetPoints(newPoints);
  newPoints->Delete();

  output->GetPointData()->SetNormals(newNormals);
  newNormals->Delete();

  output->GetPointData()->SetTCoords(newTCoords);
  newTCoords->Delete();

  output->SetStrips(newPolys);
  newPolys->Delete();
}

void vtkmpjSuperToroidalSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Size: " << this->Size << "\n";
  os << indent << "Theta Resolution: " << this->ThetaResolution << "\n";
  os << indent << "Phi Resolution: " << this->PhiResolution << "\n";
  os << indent << "Center: (" << this->Center[0] << ", "
     << this->Center[1] << ", " << this->Center[2] << ")\n";
  os << indent << "Scale: (" << this->Scale[0] << ", "
     << this->Scale[1] << ", " << this->Scale[2] << ")\n";
  os << indent << "Squareness in the z-axis: " << this->N1 << "\n";
  os << indent << "Squareness in the x-y plane: " << this->N2 << "\n";

}
