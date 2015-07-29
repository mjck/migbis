

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


#include "vtkmpjTensorStreamlineHistogramWriter.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyLine.h"

vtkStandardNewMacro(vtkmpjTensorStreamlineHistogramWriter);

// Construct object 
vtkmpjTensorStreamlineHistogramWriter::vtkmpjTensorStreamlineHistogramWriter()
{
  this->FileName = NULL;
  this->NumberOfBins = 100;
}

void vtkmpjTensorStreamlineHistogramWriter::Execute()
{
  vtkPolyData *input = this->GetInput();
  vtkPointData *pd=input->GetPointData();
  
  vtkDataArray *fa = pd->GetArray("fractani");
  vtkDataArray *md = pd->GetArray("meandiff");
  vtkDataArray *dist = pd->GetArray("distance");
  vtkDataArray *angle = pd->GetArray("angle");  
  vtkDataArray *other = pd->GetArray("other");

  vtkDataArray *torsion = NULL;
  if (pd->HasArray("torsion"))
    torsion = pd->GetArray("torsion");

  // angle stats
  vtkDataArray *minangle = NULL;
  if (pd->HasArray("minangle"))
    minangle = pd->GetArray("minangle");

  vtkDataArray *maxangle = NULL;
  if (pd->HasArray("maxangle"))
    maxangle = pd->GetArray("maxangle");

  vtkDataArray *medangle = NULL;
  if (pd->HasArray("medangle"))
    medangle = pd->GetArray("medangle");

  vtkDataArray *meanangle = NULL;
  if (pd->HasArray("meanangle"))
    meanangle = pd->GetArray("meanangle");

  vtkDataArray *varangle = NULL;
  if (pd->HasArray("varangle"))
    varangle = pd->GetArray("varangle");

  // torsion stats
  vtkDataArray *mintorsion = NULL;
  if (pd->HasArray("mintorsion"))
    mintorsion = pd->GetArray("mintorsion");

  vtkDataArray *maxtorsion = NULL;
  if (pd->HasArray("maxtorsion"))
    maxtorsion = pd->GetArray("maxtorsion");

  vtkDataArray *medtorsion = NULL;
  if (pd->HasArray("medtorsion"))
    medtorsion = pd->GetArray("medtorsion");

  vtkDataArray *meantorsion = NULL;
  if (pd->HasArray("meantorsion"))
    meantorsion = pd->GetArray("meantorsion");

  vtkDataArray *vartorsion = NULL;
  if (pd->HasArray("vartorsion"))
    vartorsion = pd->GetArray("vartorsion");

  // other stats
  vtkDataArray *minother = NULL;
  if (pd->HasArray("minother"))
    minother = pd->GetArray("minother");

  vtkDataArray *maxother = NULL;
  if (pd->HasArray("maxother"))
    maxother = pd->GetArray("maxother");

  vtkDataArray *medother = NULL;
  if (pd->HasArray("medother"))
    medother = pd->GetArray("medother");

  vtkDataArray *meanother = NULL;
  if (pd->HasArray("meanother"))
    meanother = pd->GetArray("meanother");

  vtkDataArray *varother = NULL;
  if (pd->HasArray("varother"))
    varother = pd->GetArray("varother");
  

  //vtkPointData *outPD=output->GetPointData();
  vtkCellArray *inLines = NULL;
  
  vtkPoints *inPts;
  vtkIdType numPts = 0;
  vtkIdType numLines;
  
  vtkIdType i, npts=0, *pts=NULL;
  int abort=0;
  vtkIdType inCellId;

  if (this->FileName == NULL)
    return;

  FILE *fp = fopen(this->FileName, "w");
  if (fp == NULL) 
    return;

  // Check input and initialize
  //  
  if ( !(inPts=input->GetPoints()) || 
       (numPts = inPts->GetNumberOfPoints()) < 1 ||
       !(inLines = input->GetLines()) || 
       (numLines = inLines->GetNumberOfCells()) < 1 )
    {
      return;
    }

  double factor;
  vtkIdType index = 0;
  
  double p[3];
  double vfa,vmd,vangle,vtorsion,vother;
  double vminangle, vmaxangle, vmedangle, vmeanangle, vvarangle;
  double vmintorsion, vmaxtorsion, vmedtorsion, vmeantorsion, vvartorsion;
  double vminother, vmaxother, vmedother, vmeanother, vvarother;

  fprintf(fp, "%lld\n",numLines);

  if (this->NumberOfBins < 5)
    this->NumberOfBins = 5;

  for (inCellId=0, inLines->InitTraversal(); 
       inLines->GetNextCell(npts,pts) && !abort; inCellId++)
    {
      this->UpdateProgress((float)inCellId/numLines);
      abort = this->GetAbortExecute();

      if (npts > 1) {
	
         factor = npts / (double)this->NumberOfBins;
         fprintf(fp, "\n%d\n",this->NumberOfBins);

         for(i = 0; i < this->NumberOfBins; i++)
         {

            index = vtkMath::Floor(factor * i);

            if (index >= npts) {
              printf("index is %lld, setting to %lld\n",index, npts-1);
              index = npts-1;
            }

            inPts->GetPoint(pts[index], p);
            vfa = fa->GetTuple1(pts[index]);
            vmd = md->GetTuple1(pts[index]);
            vangle = angle->GetTuple1(pts[index]);
            vother = other->GetTuple1(pts[index]);

            if (torsion == NULL)
              vtorsion = 0.0;
            else
              vtorsion = torsion->GetTuple1(pts[index]);

            // angle stats
            if (minangle == NULL)
              vminangle = 0.0;
            else
              vminangle = minangle->GetTuple1(pts[index]);

            if (maxangle == NULL)
              vmaxangle = 0.0;
            else
              vmaxangle = maxangle->GetTuple1(pts[index]);

            if (medangle == NULL)
              vmedangle = 0.0;
            else
              vmedangle = medangle->GetTuple1(pts[index]);

            if (meanangle == NULL)
              vmeanangle = 0.0;
            else
              vmeanangle = meanangle->GetTuple1(pts[index]);

            if (varangle == NULL)
              vvarangle = 0.0;
            else
              vvarangle = varangle->GetTuple1(pts[index]);

            // torsion stats
            if (mintorsion == NULL)
              vmintorsion = 0.0;
            else
              vmintorsion = mintorsion->GetTuple1(pts[index]);

            if (maxtorsion == NULL)
              vmaxtorsion = 0.0;
            else
              vmaxtorsion = maxtorsion->GetTuple1(pts[index]);

            if (medtorsion == NULL)
              vmedtorsion = 0.0;
            else
              vmedtorsion = medtorsion->GetTuple1(pts[index]);

            if (meantorsion == NULL)
              vmeantorsion = 0.0;
            else
              vmeantorsion = meantorsion->GetTuple1(pts[index]);

            if (vartorsion == NULL)
              vvartorsion = 0.0;
            else
              vvartorsion = vartorsion->GetTuple1(pts[index]);

            // other stats
           if (minother == NULL)
              vminother = 0.0;
            else
              vminother = minother->GetTuple1(pts[index]);

            if (maxother == NULL)
              vmaxother = 0.0;
            else
              vmaxother = maxother->GetTuple1(pts[index]);

            if (medother == NULL)
              vmedother = 0.0;
            else
              vmedother = medother->GetTuple1(pts[index]);

            if (meanother == NULL)
              vmeanother = 0.0;
            else
              vmeanother = meanother->GetTuple1(pts[index]);

            if (varother == NULL)
              vvarother = 0.0;
            else
              vvarother = varother->GetTuple1(pts[index]);


            
            fprintf(fp, "%lld  %f %f %f  %f %f  %f %f  %f   %f %f %f %f %f  %f %f %f %f %f  %f %f %f %f %f\n", i, 
            p[0], p[1], p[2], 
            vfa, vmd, vangle, vtorsion, vother,
            vminangle, vmaxangle, vmedangle, vmeanangle, vvarangle,
            vmintorsion, vmaxtorsion, vmedtorsion, vmeantorsion, vvartorsion,
            vminother, vmaxother, vmedother, vmeanother, vvarother);
         }
       }
    }

  fclose(fp);

  }
  
