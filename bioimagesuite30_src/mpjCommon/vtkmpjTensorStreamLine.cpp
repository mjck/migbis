/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkmpjTensorStreamLine.cpp

  Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkmpjTensorStreamLine.h"

#include "vtkCellArray.h"
#include "vtkDataSet.h"
#include "vtkFloatArray.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyLine.h"
#include <vtkstd/algorithm>

vtkStandardNewMacro(vtkmpjTensorStreamLine);

// Construct object with step size set to 1.0.
vtkmpjTensorStreamLine::vtkmpjTensorStreamLine()
{
  this->StepLength = 1.0;
  this->NumberOfStreamers = 0;

  this->ComputeStatistics = false;
  this->ComputeNormals = false;
  this->ComputeParameters = false;

  this->DirectionalColormap = 0;
  this->DirectionalColorScheme = 0;
  this->DirectionalColorSaturation = 0.5;
  this->DirectionalColorScaling = 1.0;
  this->DirectionalColorModulate = 0;
  this->DirectionalColorAlphaBlending = 0;
}

int vtkmpjTensorStreamLine::RequestData(
  vtkInformation *,
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  vtkInformation *tensorInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  vtkInformation *sourceInfo = inputVector[1]->GetInformationObject(0);
  vtkInformation *maskInfo = inputVector[2]->GetInformationObject(0);
  vtkInformation *scalarsInfo = inputVector[3]->GetInformationObject(0);

  vtkDataSet *tensor = vtkDataSet::SafeDownCast(
    tensorInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkDataSet *mask = 0;
  if (maskInfo) {
      mask = vtkDataSet::SafeDownCast(
        maskInfo->Get(vtkDataObject::DATA_OBJECT()));
  }

  vtkDataSet *scalars = 0;
  if (scalarsInfo) {
      scalars = vtkDataSet::SafeDownCast(
        scalarsInfo->Get(vtkDataObject::DATA_OBJECT()));
  }

  vtkDataSet *source = 0;
  if (sourceInfo) {
      source = vtkDataSet::SafeDownCast(
        sourceInfo->Get(vtkDataObject::DATA_OBJECT()));
  }

  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkmpjTensorStreamer::TensorStreamPoint *sPtr, *sPtr2;

  //vtkFloatArray *newVectors = 0;
  vtkFloatArray *newScalars = 0;
  vtkCellArray *newCells = 0;

  vtkPoints *newPts = 0;
  vtkFloatArray *normals = 0;
  vtkFloatArray *binormals = 0;
  vtkUnsignedCharArray *dec = 0;

  vtkIdType ptId, i, id;
  //int j;
  //double r; //x[3], v[3], r;
  double vec[3], rgb[4];
  double deltau, deltav, uv[2];

  int iv = this->IntegrationEigenvector;
  int ix = this->NormalEigenvector;
  int iy = this->BinormalEigenvector;

  // Integrate tensor streamlines
  //this->SavePointInterval = this->StepLength;
  this->vtkmpjTensorStreamer::Integrate(tensor, source, mask, scalars);
  if ( this->NumberOfStreamers <= 0 ) { return 1; }

  //
  //  Convert streamer into lines. Lines may be dashed.
  //
  newPts  = vtkPoints::New();
  newCells = vtkCellArray::New();
  //newPts->Allocate(500);
  newScalars = vtkFloatArray::New();
  newScalars->SetNumberOfComponents(3);
  
  vtkFloatArray *fa = vtkFloatArray::New();
  fa->SetName("fractani");
  fa->SetNumberOfComponents(1);

  vtkFloatArray *md = vtkFloatArray::New();
  md->SetName("meandiff");
  md->SetNumberOfComponents(1);

  vtkFloatArray *dist = vtkFloatArray::New();
  dist->SetName("distance");
  dist->SetNumberOfComponents(1);
    
  vtkFloatArray *angle = vtkFloatArray::New();
  angle->SetName("angle");
  angle->SetNumberOfComponents(1);
  
  vtkFloatArray *torsion = vtkFloatArray::New();
  torsion->SetName("torsion");
  torsion->SetNumberOfComponents(1);

  vtkFloatArray *roi = vtkFloatArray::New();
  roi->SetName("roi");
  roi->SetNumberOfComponents(1);

  vtkFloatArray *other = vtkFloatArray::New();
  other->SetName("other");
  other->SetNumberOfComponents(1);

  // set up stats 
  vtkFloatArray *minangle = 0;
  vtkFloatArray *maxangle = 0;
  vtkFloatArray *medangle = 0;
  vtkFloatArray *meanangle = 0;
  vtkFloatArray *varangle = 0;

  vtkFloatArray *mintorsion = 0;
  vtkFloatArray *maxtorsion = 0;
  vtkFloatArray *medtorsion = 0;
  vtkFloatArray *meantorsion = 0;
  vtkFloatArray *vartorsion = 0;

  vtkFloatArray *minother = 0;
  vtkFloatArray *maxother = 0;
  vtkFloatArray *medother = 0;
  vtkFloatArray *meanother = 0;
  vtkFloatArray *varother = 0;

  vtkFloatArray *params = 0;


  if (this->ComputeStatistics) 
  {
    minangle = vtkFloatArray::New();
    minangle->SetName("minangle");
    minangle->SetNumberOfComponents(1);

    maxangle = vtkFloatArray::New();
    maxangle->SetName("maxangle");
    maxangle->SetNumberOfComponents(1);

    medangle = vtkFloatArray::New();
    medangle->SetName("medangle");
    medangle->SetNumberOfComponents(1);

    meanangle = vtkFloatArray::New();
    meanangle->SetName("meanangle");
    meanangle->SetNumberOfComponents(1);

    varangle = vtkFloatArray::New();
    varangle->SetName("varangle");
    varangle->SetNumberOfComponents(1);

    mintorsion = vtkFloatArray::New();
    mintorsion->SetName("mintorsion");
    mintorsion->SetNumberOfComponents(1);

    maxtorsion = vtkFloatArray::New();
    maxtorsion->SetName("maxtorsion");
    maxtorsion->SetNumberOfComponents(1);

    medtorsion = vtkFloatArray::New();
    medtorsion->SetName("medtorsion");
    medtorsion->SetNumberOfComponents(1);

    meantorsion = vtkFloatArray::New();
    meantorsion->SetName("meantorsion");
    meantorsion->SetNumberOfComponents(1);

    vartorsion = vtkFloatArray::New();
    vartorsion->SetName("vartorsion");
    vartorsion->SetNumberOfComponents(1);

    minother = vtkFloatArray::New();
    minother->SetName("minother");
    minother->SetNumberOfComponents(1);

    maxother = vtkFloatArray::New();
    maxother->SetName("maxother");
    maxother->SetNumberOfComponents(1);

    medother = vtkFloatArray::New();
    medother->SetName("medother");
    medother->SetNumberOfComponents(1);

    meanother = vtkFloatArray::New();
    meanother->SetName("meanother");
    meanother->SetNumberOfComponents(1);

    varother = vtkFloatArray::New();
    varother->SetName("varother");
    varother->SetNumberOfComponents(1);
  }

  // set up vectors
  vtkFloatArray *vectors = vtkFloatArray::New();
  vectors->SetNumberOfComponents(3);

  // set up normals and binormals
  if (this->ComputeNormals)
  {
    normals = vtkFloatArray::New();
    normals->SetNumberOfComponents(3);

    binormals = vtkFloatArray::New();
    binormals->SetName("binormal");
    binormals->SetNumberOfComponents(3);
  }

  if (this->ComputeParameters)
  {
    params = vtkFloatArray::New();
    params->SetName("parameters");
    params->SetNumberOfComponents(2);
  }

  // set up color encoding
  if (this->DirectionalColormap)
  {
    dec = vtkUnsignedCharArray::New();
    dec->SetName("rgb");
    dec->SetNumberOfComponents(this->DirectionalColorAlphaBlending ? 4 : 3);
  }

  deltav = 1.0 / (this->NumberOfStreamers / 2);
  uv[0] = uv[1] = 0.0;

  //
  // Loop over all streamers generating points
  //
  //printf( "number of streamers=%d\n",this->NumberOfStreamers);
  for (ptId=0; ptId < this->NumberOfStreamers; ptId+=2)
    {
      int numPts = this->Streamers[ptId].GetNumberOfPoints();
      int numPts2 = this->Streamers[ptId+1].GetNumberOfPoints();
      int numTotal = numPts + numPts2;
      //printf("numTotal=%d, numPts=%d, numPts2=%d\n",numTotal,numPts,numPts2);

      uv[1] += deltav;

      if (!(numPts >= 3 && numPts2 >= 3))   
        // the first point is repeated 
        // the last point is invalid, so there must be at least 3
        // points in each streamer in order to yield at least one
        // segment.
        continue;

      //printf("here!!!\n");
      sPtr = this->Streamers[ptId].GetTensorStreamPoint(numPts-2);
      sPtr2 = this->Streamers[ptId+1].GetTensorStreamPoint(numPts2-2);

      //printf("after\n");
      if ( (sPtr->d + sPtr2->d) < this->MinimumDistance ) 
        continue;

      // compute stats for the two segments
      double min_angle, min_torsion, min_other;
      double max_angle, max_torsion, max_other;
      double med_angle, med_torsion, med_other;
      double mean_angle, mean_torsion, mean_other;
      double var_angle, var_torsion, var_other;

      if (this->ComputeStatistics) 
      {
        std::vector<double> angles;
        std::vector<double> torsions;
        std::vector<double> others;
        double sumangle = 0;
        double sumother = 0;
        double sumtorsion = 0;
        double sumangle2 = 0;
        double sumtorsion2 = 0;
        double sumother2 = 0;

        // compute stats
        for(i=numPts-2; i>=0; i--) {
          sPtr = this->Streamers[ptId].GetTensorStreamPoint(i);
          angles.push_back(sPtr->angle);
          torsions.push_back(sPtr->torsion);
          others.push_back(sPtr->s);
          sumangle += sPtr->angle;
          sumangle2 += sPtr->angle * sPtr->angle;
          sumtorsion += sPtr->torsion;
          sumtorsion2 += sPtr->torsion * sPtr->torsion;
          sumother += sPtr->s;
          sumother2 += sPtr->s * sPtr->s;
        }

        // compute stats
        for(i=1; i<=numPts2-2; i++) {
          sPtr = this->Streamers[ptId+1].GetTensorStreamPoint(i);
          angles.push_back(sPtr->angle);
          torsions.push_back(sPtr->torsion);
          others.push_back(sPtr->s);
          sumangle += sPtr->angle;
          sumangle2 += sPtr->angle * sPtr->angle;
          sumtorsion += sPtr->torsion;
          sumtorsion2 += sPtr->torsion * sPtr->torsion;
          sumother += sPtr->s;
          sumother2 += sPtr->s * sPtr->s;
        }

        // sort them
        std::sort(angles.begin(), angles.end());
        std::sort(torsions.begin(), torsions.end());
        std::sort(others.begin(), others.end());

        // calculate statistics
        int total = numTotal-3;
        min_angle = angles[0];
        max_angle = angles[total-1];
        med_angle = angles[total / 2];
        mean_angle = sumangle / (double)total;
        var_angle = (sumangle2 - ((double)total*mean_angle*mean_angle)) / ((double)total-1);

        min_torsion = torsions[0];
        max_torsion = torsions[total-1];
        med_torsion = torsions[total / 2];
        mean_torsion = sumtorsion / (double)total;
        var_torsion = (sumtorsion2 - ((double)total*mean_torsion*mean_torsion)) / ((double)total-1);

        min_other = others[0];
        max_other = others[total-1];
        med_other = others[total / 2];
        mean_other = sumother / (double)total;
        var_other = (sumother2 - ((double)total*mean_other*mean_other)) / ((double)total-1);

      }

      // store values for the two segments
      newCells->InsertNextCell(numTotal-3);

      deltau = 1.0 / (numTotal - 3);

      //fprintf(stderr,"first segment\n");
      //printf("after stats\n");

      uv[0] = 0.0;

      // first segment
      for(i=numPts-2; i>=0; i--) 
      {
        sPtr = this->Streamers[ptId].GetTensorStreamPoint(i);
        //sPrev = i<numPts-1 ? this->Streamers[ptId].GetTensorStreamPoint(i+1) : sPtr;

        id = newPts->InsertNextPoint(sPtr->x);

        if (this->ComputeParameters)
        {
          //printf("uv: %lf, %lf\n", uv[0], uv[1]);
          params->InsertNextTuple(uv);
          uv[0] += deltau;
        }

        //fprintf(stderr, "%lf %lf %lf\n", sPtr->x[0] ,sPtr->x[1], sPtr->x[2]);
        newCells->InsertCellPoint(id);

        // insert scalars
        if (this->ComputeNormals)
          newScalars->InsertNextTuple3(sPtr->w[0], sPtr->w[1], sPtr->w[2]);
        else
          newScalars->InsertNextTuple3(1, 1, 1);

        // insert fields
        fa->InsertNextTuple(&sPtr->fa);
        md->InsertNextTuple(&sPtr->md);
        dist->InsertNextTuple(&sPtr->d);
        angle->InsertNextTuple(&sPtr->angle);
        torsion->InsertNextTuple(&sPtr->torsion);
        roi->InsertNextTuple(&sPtr->mask);
        other->InsertNextTuple(&sPtr->s);

        if (this->ComputeStatistics) 
        {
          minangle->InsertNextTuple(&min_angle);
          maxangle->InsertNextTuple(&max_angle);
          medangle->InsertNextTuple(&med_angle);
          meanangle->InsertNextTuple(&mean_angle);
          varangle->InsertNextTuple(&var_angle);
          
          mintorsion->InsertNextTuple(&min_torsion);
          maxtorsion->InsertNextTuple(&max_torsion);
          medtorsion->InsertNextTuple(&med_torsion);
          meantorsion->InsertNextTuple(&mean_torsion);
          vartorsion->InsertNextTuple(&var_torsion);

          minother->InsertNextTuple(&min_other);
          maxother->InsertNextTuple(&max_other);
          medother->InsertNextTuple(&med_other);
          meanother->InsertNextTuple(&mean_other);
          varother->InsertNextTuple(&var_other);           
        }

        vec[0] = sPtr->v[0][iv];
        vec[1] = sPtr->v[1][iv];
        vec[2] = sPtr->v[2][iv];
        vectors->InsertNextTuple(vec);
        //printf("vec=%lf,%lf,%lf\n",vec[0],vec[1],vec[2]);

        if (this->DirectionalColormap)
        {
          switch(this->DirectionalColorScheme) {
            case 1:
              this->RotationalSymmetryScheme(sPtr->fa, vec, rgb);
              break;
            case 2:
              this->NoSymmetryScheme(sPtr->fa, vec, rgb);
              break;
            case 3:
              this->MirrorSymmetryScheme(sPtr->fa, vec, rgb);
              break;
            default:
              this->AbsoluteValueScheme(sPtr->fa, vec, rgb);
              break;
          }

          if (this->DirectionalColorAlphaBlending)
            rgb[3] = sPtr->fa * 255;

          dec->InsertNextTuple(rgb);
        }

        if (this->ComputeNormals) 
        {
          ix = this->NormalEigenvector;
          vec[0] = sPtr->v[0][ix];
          vec[1] = sPtr->v[1][ix];
          vec[2] = sPtr->v[2][ix];
          normals->InsertNextTuple(vec);

          iy = this->BinormalEigenvector;
          vec[0] = sPtr->v[0][iy];
          vec[1] = sPtr->v[1][iy];
          vec[2] = sPtr->v[2][iy];
          binormals->InsertNextTuple(vec);
        }
        
      } // first segment

      //fprintf(stderr,"second segment\n");
      for(i=1; i<=numPts2-2; i++)
      {
        sPtr = this->Streamers[ptId+1].GetTensorStreamPoint(i);
        //sPrev = this->Streamers[ptId+1].GetTensorStreamPoint(i-1); 

        id = newPts->InsertNextPoint(sPtr->x);

        if (this->ComputeParameters)
        {
          //printf("uv: %lf, %lf\n", uv[0], uv[1]);
          params->InsertNextTuple(uv);
          uv[0] += deltau;
        }

        //fprintf(stderr, "%lf %lf %lf\n", sPtr->x[0] ,sPtr->x[1], sPtr->x[2]);
        newCells->InsertCellPoint(id);

        // insert scalars
        if (this->ComputeNormals)
          newScalars->InsertNextTuple3(sPtr->w[0], sPtr->w[1], sPtr->w[2]);
        else
          newScalars->InsertNextTuple3(1, 1, 1);

        // insert fields
        fa->InsertNextTuple(&sPtr->fa);
        md->InsertNextTuple(&sPtr->md);
        dist->InsertNextTuple(&sPtr->d);
        angle->InsertNextTuple(&sPtr->angle);
        torsion->InsertNextTuple(&sPtr->torsion);
        roi->InsertNextTuple(&sPtr->mask);
        other->InsertNextTuple(&sPtr->s);

        if (this->ComputeStatistics) 
        {
          minangle->InsertNextTuple(&min_angle);
          maxangle->InsertNextTuple(&max_angle);
          medangle->InsertNextTuple(&med_angle);
          meanangle->InsertNextTuple(&mean_angle);
          varangle->InsertNextTuple(&var_angle);
          
          mintorsion->InsertNextTuple(&min_torsion);
          maxtorsion->InsertNextTuple(&max_torsion);
          medtorsion->InsertNextTuple(&med_torsion);
          meantorsion->InsertNextTuple(&mean_torsion);
          vartorsion->InsertNextTuple(&var_torsion);

          minother->InsertNextTuple(&min_other);
          maxother->InsertNextTuple(&max_other);
          medother->InsertNextTuple(&med_other);
          meanother->InsertNextTuple(&mean_other);
          varother->InsertNextTuple(&var_other);           
        }

        iv = this->IntegrationEigenvector;
        vec[0] = sPtr->v[0][iv];
        vec[1] = sPtr->v[1][iv];
        vec[2] = sPtr->v[2][iv];
        vectors->InsertNextTuple(vec);
        //printf("vec=%lf,%lf,%lf\n",vec[0],vec[1],vec[2]);

        if (this->DirectionalColormap)
        {
          switch(this->DirectionalColorScheme) {
            case 1:
              this->RotationalSymmetryScheme(sPtr->fa, vec, rgb);
              break;
            case 2:
              this->NoSymmetryScheme(sPtr->fa, vec, rgb);
              break;
            case 3:
              this->MirrorSymmetryScheme(sPtr->fa, vec, rgb);
              break;
            default:
              this->AbsoluteValueScheme(sPtr->fa, vec, rgb);
              break;
          }

          if (this->DirectionalColorAlphaBlending)
            rgb[3] = sPtr->fa * 255;

          dec->InsertNextTuple(rgb);
        }

        if (this->ComputeNormals) 
        {
          ix = this->NormalEigenvector;
          vec[0] = sPtr->v[0][ix];
          vec[1] = sPtr->v[1][ix];
          vec[2] = sPtr->v[2][ix];
          normals->InsertNextTuple(vec);

          iy = this->BinormalEigenvector;
          vec[0] = sPtr->v[0][iy];
          vec[1] = sPtr->v[1][iy];
          vec[2] = sPtr->v[2][iy];
          binormals->InsertNextTuple(vec);
        }

      } // second segment

    } // stremers

  //printf("almost done\n");
  vtkPointData *outPD = output->GetPointData();

  outPD->SetScalars(newScalars);
  newScalars->Delete();

  outPD->AddArray(fa);
  fa->Delete();

  outPD->AddArray(md);
  md->Delete();

  outPD->AddArray(dist);
  dist->Delete();

  outPD->AddArray(angle);
  angle->Delete();

  outPD->AddArray(roi);
  roi->Delete();

  outPD->AddArray(other);
  other->Delete();

  outPD->AddArray(torsion);
  torsion->Delete();

  // insert statistics
  if (this->ComputeStatistics) 
  {
    outPD->AddArray(minangle);
    minangle->Delete(); 

    outPD->AddArray(maxangle);
    maxangle->Delete(); 

    outPD->AddArray(medangle);
    medangle->Delete();   

    outPD->AddArray(meanangle);
    meanangle->Delete(); 

    outPD->AddArray(varangle);
    varangle->Delete();  

    outPD->AddArray(mintorsion);
    mintorsion->Delete(); 

    outPD->AddArray(maxtorsion);
    maxtorsion->Delete(); 

    outPD->AddArray(medtorsion);
    medtorsion->Delete();   

    outPD->AddArray(meantorsion);
    meantorsion->Delete(); 

    outPD->AddArray(vartorsion);
    vartorsion->Delete(); 

    outPD->AddArray(minother);
    minother->Delete(); 

    outPD->AddArray(maxother);
    maxother->Delete(); 

    outPD->AddArray(medother);
    medother->Delete();   

    outPD->AddArray(meanother);
    meanother->Delete(); 

    outPD->AddArray(varother);
    varother->Delete(); 
  }
   
  if (this->DirectionalColormap)
  {
    outPD->AddArray(dec);
    dec->Delete();
  }

  if (this->ComputeNormals)
  {
    outPD->SetNormals(normals);
    normals->Delete();
    outPD->AddArray(binormals);
    binormals->Delete();
  }

  if (this->ComputeParameters)
  {
    outPD->AddArray(params);
    params->Delete();
  }

  outPD->SetVectors(vectors);
  vectors->Delete();

  output->SetPoints(newPts);
  newPts->Delete();

  output->SetLines(newCells);
  newCells->Delete();

  //printf("over\n");

  // Delete the streamers since they are no longer needed
  delete[] this->Streamers;
  this->Streamers = 0;
  this->NumberOfStreamers = 0;

  output->Squeeze();

  return 1;
}

void vtkmpjTensorStreamLine::AbsoluteValueScheme(double FA, double *v, double *nrgb)
{
  double mgn;
  double rgb[3];
  double scale = this->DirectionalColorScaling;

  rgb[0] = rgb[1] = rgb[2] = 0;
  
  rgb[0] = fabs(v[0]);
  rgb[1] = fabs(v[1]);
  rgb[2] = fabs(v[2]);
  
  /* compute color */
  mgn = (this->DirectionalColorModulate ? FA : 1.0);

  rgb[0] *= mgn;
  rgb[1] *= mgn;
  rgb[2] *= mgn;
  
  /* RGBs have to be in [0,255] */
  nrgb[0] = rgb[0]*255*scale;
  nrgb[1] = rgb[1]*255*scale;
  nrgb[2] = rgb[2]*255*scale;
  
  nrgb[0] = nrgb[0] < 256 ? nrgb[0] : 255;
  nrgb[1] = nrgb[1] < 256 ? nrgb[1] : 255;
  nrgb[2] = nrgb[2] < 256 ? nrgb[2] : 255;
  
}

void vtkmpjTensorStreamLine::RotationalSymmetryScheme(double FA, double *v, double *nrgb)
{

  double rgb[3], hsv[3];
  double theta, phi;
  double vlenxy, vlen;
  double pi = vtkMath::Pi();
  double ps = this->DirectionalColorSaturation;
  double scale = this->DirectionalColorScaling;

  rgb[0] = rgb[1] = rgb[2] = 0;
  
  nrgb[0] = v[0];
  nrgb[1] = v[1];
  nrgb[2] = v[2];
  
  /* normalize vector */
  vlenxy = vtkMath::Norm2D(v);

  /* compute hue */
  if (v[0] > 0)
    phi = asin(v[1] / vlenxy);
  else
    phi = pi - asin(v[1] / vlenxy);
  hsv[0] = fmod((phi+2*pi+pi/2)*2,2*pi) / (2*pi);
  if (hsv[0] > 1.0) hsv[0] = 1.0;
  
  /* compute saturation */
  vlen = vtkMath::Norm(v);
  theta = asin(vlenxy / vlen);
  if (theta > 1) theta = 1.0;
  hsv[1] = sin(ps*theta) / sin(ps*pi/2);
  
  /* compute value */
  hsv[2] = (this->DirectionalColorModulate ? FA : 1.0);
  
  vtkMath::HSVToRGB(hsv, rgb);

  /* RGBs have to be in [0,255] */
  nrgb[0] = rgb[0]*255+scale*nrgb[0];
  nrgb[1] = rgb[1]*255+scale*nrgb[1];
  nrgb[2] = rgb[2]*255+scale*nrgb[2];
  
  nrgb[0] = nrgb[0] < 256 ? nrgb[0] : 255;
  nrgb[1] = nrgb[1] < 256 ? nrgb[1] : 255;
  nrgb[2] = nrgb[2] < 256 ? nrgb[2] : 255;
}


void vtkmpjTensorStreamLine::NoSymmetryScheme(double FA, double *v, double *nrgb)
{
  double rgb[3], hsv[3];
  double theta, phi;
  double vlenxy, vlen;
  double pi = vtkMath::Pi();
  double ps = this->DirectionalColorSaturation;
  double scale = this->DirectionalColorScaling;
  
  rgb[0] = rgb[1] = rgb[2] = 0;
  
  nrgb[0] = v[0];
  nrgb[1] = v[1];
  nrgb[2] = v[2];

  /* normalize vector */
  vlenxy = vtkMath::Norm2D(v);
  
  /* compute hue */
  if (v[0] > 0)
    phi = asin(v[1] / vlenxy);
  else
    phi = pi - asin(v[1] / vlenxy);
  hsv[0] = fmod(phi+2*pi,2*pi) / (2*pi);
  if (hsv[0] > 1.0) hsv[0] = 1.0;
  
  /* compute saturation */
  vlen = vtkMath::Norm(v);
  theta = asin(vlenxy / vlen);
  if (theta > 1) theta = 1.0;
  hsv[1] = sin(ps*theta) / sin(ps*pi/2);
    
  /* compute value */
  hsv[2] = (this->DirectionalColorModulate ? FA : 1.0);
  
  vtkMath::HSVToRGB(hsv, rgb);
  
  /* RGBs have to be in [0,255] */
  nrgb[0] = rgb[0]*255+scale*nrgb[0];
  nrgb[1] = rgb[1]*255+scale*nrgb[1];
  nrgb[2] = rgb[2]*255+scale*nrgb[2];
  
  nrgb[0] = nrgb[0] < 256 ? nrgb[0] : 255;
  nrgb[1] = nrgb[1] < 256 ? nrgb[1] : 255;
  nrgb[2] = nrgb[2] < 256 ? nrgb[2] : 255;
  
}


void vtkmpjTensorStreamLine::MirrorSymmetryScheme(double FA, double *v, double *nrgb)
{
  double rgb[3], hsv[3];
  double theta, phi;
  double vlenxy, vlen;
  double pi = vtkMath::Pi();
  double ps = this->DirectionalColorSaturation;
  double scale = this->DirectionalColorScaling;

  rgb[0] = rgb[1] = rgb[2] = 0;
  
  nrgb[0] = v[0];
  nrgb[1] = v[1];
  nrgb[2] = v[2];
  
  /* normalize vector */
  vlenxy = vtkMath::Norm2D(v);
  
  /* compute hue */
  if (v[0] > 0)
    phi = asin(v[1] / vlenxy);
  else
    phi = pi + asin(v[1] / vlenxy);
  hsv[0] = 2 * fmod(phi + pi, pi) / (2*pi);
  if (hsv[0] > 1.0) hsv[0] = 1.0;
  
  /* compute saturation */
  vlen = vtkMath::Norm(v);
  theta = asin(vlenxy / vlen);
  if (theta > 1) theta = 1.0;
  hsv[1] = sin(ps*theta) / sin(ps*pi/2);
  
  /* compute value */
  hsv[2] = (this->DirectionalColorModulate ? FA : 1.0);
  
  vtkMath::HSVToRGB(hsv, rgb);
  
  /* RGBs have to be in [0,255] */
  nrgb[0] = rgb[0]*255+scale*nrgb[0];
  nrgb[1] = rgb[1]*255+scale*nrgb[1];
  nrgb[2] = rgb[2]*255+scale*nrgb[2];
  
  nrgb[0] = nrgb[0] < 256 ? nrgb[0] : 255;
  nrgb[1] = nrgb[1] < 256 ? nrgb[1] : 255;
  nrgb[2] = nrgb[2] < 256 ? nrgb[2] : 255;
}

void vtkmpjTensorStreamLine::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Step Length: " << this->StepLength << "\n";

}
