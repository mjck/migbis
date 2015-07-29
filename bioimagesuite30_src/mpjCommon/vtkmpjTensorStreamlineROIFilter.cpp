

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


#include "vtkmpjTensorStreamlineROIFilter.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyLine.h"

vtkStandardNewMacro(vtkmpjTensorStreamlineROIFilter);

// Construct object 
vtkmpjTensorStreamlineROIFilter::vtkmpjTensorStreamlineROIFilter()
{
  this->Center1[0] = 0;
  this->Center1[1] = 0;
  this->Center1[2] = 0;

  this->Radius1 = 1.0;

  this->Center2[0] = 0;
  this->Center2[1] = 0;
  this->Center2[2] = 0;

  this->Radius2 = 1.0;

  this->Clipping = 0;

  this->ComputeStatistics = 0;
}

void vtkmpjTensorStreamlineROIFilter::Execute()
{
  vtkPolyData *input = this->GetInput();
  vtkPolyData *output = this->GetOutput();
  vtkPointData *pd=input->GetPointData();

  vtkPointData *outPD=output->GetPointData();
  vtkCellArray *inLines = NULL;

  vtkPoints *inPts;
  vtkIdType numPts = 0;
  vtkIdType numLines;
    
  double faval, mdval, distval, angleval, torsionval, roival, otherval;
  double scalarval[3], paramsval[2], decval[4];
  double normalval[3], vectorval[3], binormalval[3];
  double minangleval, maxangleval, medangleval, meanangleval, varangleval;
  double mintorsionval, maxtorsionval, medtorsionval, meantorsionval, vartorsionval;
  double minotherval, maxotherval, medotherval, meanotherval, varotherval;
  double pt[3];

  vtkIdType npts=0, *pts=NULL;
  int id, abort=0;
  vtkIdType inCellId;

  vtkDataArray *scalars = pd->GetScalars();
  vtkDataArray *params = pd->GetArray("parameters");

  vtkDataArray *fa = pd->GetArray("fractani");
  vtkDataArray *md = pd->GetArray("meandiff");
  vtkDataArray *dist = pd->GetArray("distance");
  vtkDataArray *angle = pd->GetArray("angle");  
  vtkDataArray *torsion = pd->GetArray("torsion");
  vtkDataArray *roi = pd->GetArray("roi");
  vtkDataArray *other = pd->GetArray("other");

  vtkDataArray *minangle = pd->GetArray("minangle");
  vtkDataArray *maxangle = pd->GetArray("maxangle");
  vtkDataArray *medangle = pd->GetArray("medangle");
  vtkDataArray *meanangle = pd->GetArray("meanangle");
  vtkDataArray *varangle = pd->GetArray("varangle");

  vtkDataArray *mintorsion = pd->GetArray("mintorsion");
  vtkDataArray *maxtorsion = pd->GetArray("maxtorsion");
  vtkDataArray *medtorsion = pd->GetArray("medtorsion");
  vtkDataArray *meantorsion = pd->GetArray("meantorsion");
  vtkDataArray *vartorsion = pd->GetArray("vartorsion");

  vtkDataArray *minother = pd->GetArray("minother");
  vtkDataArray *maxother = pd->GetArray("maxother");
  vtkDataArray *medother = pd->GetArray("medother");
  vtkDataArray *meanother = pd->GetArray("meanother");
  vtkDataArray *varother = pd->GetArray("varother");

  vtkDataArray *normals = pd->GetNormals();
  vtkDataArray *binormals = pd->GetArray("binormals");
  vtkDataArray *vectors = pd->GetVectors();

  vtkDataArray *dec = pd->GetArray("rgb");
 
  // Check input and initialize
  //  
  if ( !(inPts=input->GetPoints()) || 
       (numPts = inPts->GetNumberOfPoints()) < 1 ||
       !(inLines = input->GetLines()) || 
       (numLines = inLines->GetNumberOfCells()) < 1 )
    {
      return;
    }
  
  // Create output data
  vtkPoints *newPts = vtkPoints::New();
  vtkCellArray *newCells = vtkCellArray::New();

  vtkFloatArray *newscalars = vtkFloatArray::New();
  newscalars->SetNumberOfComponents(scalars->GetNumberOfComponents());

  vtkFloatArray *newparams = NULL;
  if (params != NULL) {
    newparams = vtkFloatArray::New();
    newparams->SetNumberOfComponents(params->GetNumberOfComponents());
  }

  vtkFloatArray *newfa = vtkFloatArray::New();
  newfa->SetName("fractani");
  newfa->SetNumberOfComponents(1);

  vtkFloatArray *newmd = vtkFloatArray::New();
  newmd->SetName("meandiff");
  newmd->SetNumberOfComponents(1);

  vtkFloatArray *newdist = vtkFloatArray::New();
  newdist->SetName("distance");
  newdist->SetNumberOfComponents(1);

  vtkFloatArray *newangle = vtkFloatArray::New();
  newangle->SetName("angle");
  newangle->SetNumberOfComponents(1);

  vtkFloatArray *newtorsion = vtkFloatArray::New();
  newtorsion->SetName("torsion");
  newtorsion->SetNumberOfComponents(1);

  vtkFloatArray *newroi = vtkFloatArray::New();
  newroi->SetName("roi");
  newroi->SetNumberOfComponents(1);

  vtkFloatArray *newother = vtkFloatArray::New();
  newother->SetName("other");
  newother->SetNumberOfComponents(1);

  vtkFloatArray *newminangle = NULL;  
  vtkFloatArray *newmaxangle = NULL;
  vtkFloatArray *newmedangle = NULL;
  vtkFloatArray *newmeanangle = NULL;
  vtkFloatArray *newvarangle = NULL;

  vtkFloatArray *newmintorsion = NULL;
  vtkFloatArray *newmaxtorsion = NULL;
  vtkFloatArray *newmedtorsion = NULL;
  vtkFloatArray *newmeantorsion = NULL;
  vtkFloatArray *newvartorsion = NULL;

  vtkFloatArray *newminother = NULL;
  vtkFloatArray *newmaxother = NULL;
  vtkFloatArray *newmedother = NULL;
  vtkFloatArray *newmeanother = NULL;
  vtkFloatArray *newvarother = NULL;

  vtkUnsignedCharArray *newdec = NULL;

  vtkFloatArray *newnormals = NULL;
  vtkFloatArray *newbinormals = NULL;
  vtkFloatArray *newvectors = NULL;

  // set up stats fields
  if (this->ComputeStatistics) 
  {
    newminangle = vtkFloatArray::New();
    newminangle->SetName("minangle");
    newminangle->SetNumberOfComponents(1);
    
    newmaxangle = vtkFloatArray::New();
    newmaxangle->SetName("maxangle");
    newmaxangle->SetNumberOfComponents(1);

    newmedangle = vtkFloatArray::New();
    newmedangle->SetName("medangle");
    newmedangle->SetNumberOfComponents(1);
  
    newmeanangle = vtkFloatArray::New();
    newmeanangle->SetName("meanangle");
    newmeanangle->SetNumberOfComponents(1);

    newvarangle = vtkFloatArray::New();
    newvarangle->SetName("varangle");
    newvarangle->SetNumberOfComponents(1);
  
    newmintorsion = vtkFloatArray::New();
    newmintorsion->SetName("mintorsion");
    newmintorsion->SetNumberOfComponents(1);

    newmaxtorsion = vtkFloatArray::New();
    newmaxtorsion->SetName("maxtorsion");
    newmaxtorsion->SetNumberOfComponents(1);

    newmedtorsion = vtkFloatArray::New();
    newmedtorsion->SetName("medtorsion");
    newmedtorsion->SetNumberOfComponents(1);
  
    newmeantorsion = vtkFloatArray::New();
    newmeantorsion->SetName("meantorsion");
    newmeantorsion->SetNumberOfComponents(1);

    newvartorsion = vtkFloatArray::New();
    newvartorsion->SetName("vartorsion");
    newvartorsion->SetNumberOfComponents(1);

    newminother = vtkFloatArray::New();
    newminother->SetName("minother");
    newminother->SetNumberOfComponents(1);

    newmaxother = vtkFloatArray::New();
    newmaxother->SetName("maxother");
    newmaxother->SetNumberOfComponents(1);
  
    newmedother = vtkFloatArray::New();
    newmedother->SetName("medother");
    newmedother->SetNumberOfComponents(1);

    newmeanother = vtkFloatArray::New();
    newmeanother->SetName("meanother");
    newmeanother->SetNumberOfComponents(1);
  
    newvarother = vtkFloatArray::New();
    newvarother->SetName("varother");
    newvarother->SetNumberOfComponents(1);
    
  }

  if (normals != NULL)
  {
    newnormals = vtkFloatArray::New();
    newnormals->SetNumberOfComponents(3);
  }

  if (binormals != NULL)
  {
    newbinormals = vtkFloatArray::New();
    newbinormals->SetNumberOfComponents(3);
  }

  if (vectors != NULL)
  {
    newvectors = vtkFloatArray::New();
    newvectors->SetNumberOfComponents(3);
  }

  if (dec != NULL)
   {
    newdec = vtkUnsignedCharArray::New();
    newdec->SetName("rgb");
    newdec->SetNumberOfComponents(dec->GetNumberOfComponents());
  }

  for (inCellId=0, inLines->InitTraversal(); 
       inLines->GetNextCell(npts,pts) && !abort; inCellId++)
    {
      this->UpdateProgress((double)inCellId/numLines);
      abort = this->GetAbortExecute();
      
      if (npts < 2)
    	{
    	  vtkWarningMacro(<< "Less than two points in line!");
    	  continue; //skip filtering this line
    	}

    vtkIdType i; 
    vtkIdType enter1=-1;
    vtkIdType leave1=-1;
    vtkIdType enter2=-1;
    vtkIdType leave2=-1;
    int roi1=-1, roi2=-1;
    double dist1, dist2;
    int state = 0;

    double pt[3];

    for(i=0; i<npts; i++) {

        inPts->GetPoint(pts[i], pt);

        dist1 = (pt[0] - this->Center1[0]) * (pt[0] - this->Center1[0]) +
                (pt[1] - this->Center1[1]) * (pt[1] - this->Center1[1]) +
                (pt[2] - this->Center1[2]) * (pt[2] - this->Center1[2]);
        dist2 = (pt[0] - this->Center2[0]) * (pt[0] - this->Center2[0]) +
                (pt[1] - this->Center2[1]) * (pt[1] - this->Center2[1]) +
                (pt[2] - this->Center2[2]) * (pt[2] - this->Center2[2]);

        switch(state)
        {
          case 0: 
            // outside ROIS
            if (dist1 <= this->Radius1*this->Radius1)
            {
              enter1 = i; roi1 = 1; state=1;
            } else if (dist2 <= this->Radius2*this->Radius2)
            {
              enter1 = i; roi1 = 2; state=1;
            }
            break;
          case 1:
            // inside initial ROI
            if (roi1 == 1 && dist1 > this->Radius1*this->Radius1)
            {
              leave1 = i; state = 2;
            } else if (roi1 == 2 && dist2 > this->Radius2*this->Radius2)
            {
              leave1 = i; state = 2;
            }
            break;
          case 2: 
            // passed one ROI
            if (dist1 <= this->Radius1*this->Radius1)
            {
              enter2 = i; roi2 = 1; state=3;
            } else if (dist2 <= this->Radius2*this->Radius2)
            {
              enter2 = i; roi2 = 2; state=3;
            }
            break;
          case 3:
            // inside second ROI
            if (roi2 == 1 && dist1 > this->Radius1*this->Radius1)
            {
              leave2 = i-1; state = 4;
            } else if (roi2 == 2 && dist2 > this->Radius2*this->Radius2)
            {
              leave2 = i-1; state = 4;
            }
            break;

            default:
            break;
        }

      }

      // terminou dentro da ROI
      if (state == 3) {
          leave2 = npts-1;
      }

      // keep fiber
      if (state >= 3 && fabs(roi1-roi2) == 1) {

        vtkIdType startindex = 0;
        vtkIdType endindex = npts-1;
        vtkIdType NumNewPts = npts;
        int inci = 1;

        if (this->Clipping) {
            startindex = enter1;
            endindex = leave2;
            NumNewPts = (leave2 - enter1) + 1;
        }

        //printf("roi1=%d roi2=%d, numpts=%d\n",roi1,roi2,NumNewPts);
        if (roi1 > roi2) {
          vtkIdType temp = startindex;
          startindex = endindex;
          endindex = temp;
          inci = -1;
        }

      	// allocate next cell
      	newCells->InsertNextCell(NumNewPts);
      	
      	// create new polyline
      	for(i = startindex; i != endindex + inci; i = i + inci) {
      	  
          //printf("fetching data1 %d\n",i);
          scalars->GetTuple(pts[i], scalarval);

          if (params != NULL)
            params->GetTuple(pts[i], paramsval);

      	  faval = fa->GetTuple1(pts[i]);
      	  mdval = md->GetTuple1(pts[i]);
      	  distval = dist->GetTuple1(pts[i]);
      	  angleval = angle->GetTuple1(pts[i]);
          torsionval = torsion->GetTuple1(pts[i]);
      	  roival = roi->GetTuple1(pts[i]);
      	  otherval = other->GetTuple1(pts[i]);

          if (this->ComputeStatistics) {
            //printf("fetching stats\n");
            minangleval = minangle->GetTuple1(pts[i]);
            maxangleval = maxangle->GetTuple1(pts[i]);
            medangleval = medangle->GetTuple1(pts[i]);
            meanangleval = meanangle->GetTuple1(pts[i]);
            varangleval = varangle->GetTuple1(pts[i]);

            mintorsionval = mintorsion->GetTuple1(pts[i]);
            maxtorsionval = maxtorsion->GetTuple1(pts[i]);
            medtorsionval = medtorsion->GetTuple1(pts[i]);
            meantorsionval = meantorsion->GetTuple1(pts[i]);
            vartorsionval = vartorsion->GetTuple1(pts[i]);

            minotherval = minother->GetTuple1(pts[i]);
            maxotherval = maxother->GetTuple1(pts[i]);
            medotherval = medother->GetTuple1(pts[i]);
            meanotherval = meanother->GetTuple1(pts[i]);
            varotherval = varother->GetTuple1(pts[i]);
          }

          if (normals != NULL)
            normals->GetTuple(pts[i], normalval);

          if (binormals != NULL)
            binormals->GetTuple(pts[i], binormalval);

          if (vectors != NULL)
            vectors->GetTuple(pts[i], vectorval);

          if (dec != NULL)
            dec->GetTuple(pts[i], decval);


          // begin insertion
          inPts->GetPoint(pts[i], pt);
      	  id = newPts->InsertNextPoint(pt);
      	  
      	  // insert next cell
      	  newCells->InsertCellPoint(id);
      	  
          //printf("inserting data1\n");
      	  // insert fields
          newscalars->InsertNextTuple(scalarval);

          if (params != NULL)
            newparams->InsertNextTuple(paramsval);

          // TEST TEST TEST TEST
          //otherval = (double) i;

      	  newfa->InsertNextTuple(&faval);
      	  newmd->InsertNextTuple(&mdval);
      	  newdist->InsertNextTuple(&distval);
      	  newangle->InsertNextTuple(&angleval);
          newtorsion->InsertNextTuple(&torsionval);
      	  newroi->InsertNextTuple(&roival);
      	  newother->InsertNextTuple(&otherval);	 

          if (this->ComputeStatistics) {
            //printf("inserting stats\n");
            newminangle->InsertNextTuple(&minangleval);
            newmaxangle->InsertNextTuple(&maxangleval);
            newmedangle->InsertNextTuple(&medangleval);
            newmeanangle->InsertNextTuple(&meanangleval);
            newvarangle->InsertNextTuple(&varangleval);

            newmintorsion->InsertNextTuple(&mintorsionval);
            newmaxtorsion->InsertNextTuple(&maxtorsionval);
            newmedtorsion->InsertNextTuple(&medtorsionval);
            newmeantorsion->InsertNextTuple(&meantorsionval);
            newvartorsion->InsertNextTuple(&vartorsionval);

            newminother->InsertNextTuple(&minotherval);
            newmaxother->InsertNextTuple(&maxotherval);
            newmedother->InsertNextTuple(&medotherval);
            newmeanother->InsertNextTuple(&meanotherval);
            newvarother->InsertNextTuple(&varotherval);

          }

          if (normals != NULL)
            newnormals->InsertNextTuple(normalval);

          if (binormals != NULL)
            newbinormals->InsertNextTuple(binormalval);

          if (vectors != NULL)
            newvectors->InsertNextTuple(vectorval);
          
          if (dec != NULL)
            newdec->InsertNextTuple(decval);

      	}
	
      }

    }

  //printf("wrapping up\n");
  outPD->SetScalars(newscalars);
  newscalars->Delete();

  if (params != NULL)
    outPD->AddArray(newparams);

  if (normals != NULL)
    outPD->SetNormals(newnormals);

  if (binormals != NULL)
    outPD->AddArray(newbinormals);

  if (vectors != NULL)
    outPD->SetVectors(newvectors);

  outPD->AddArray(newfa);
  newfa->Delete();
  
  outPD->AddArray(md);
  newmd->Delete();
  
  outPD->AddArray(newdist);
  newdist->Delete();

  outPD->AddArray(newangle);
  newangle->Delete();

  outPD->AddArray(newtorsion);
  newtorsion->Delete();

  outPD->AddArray(newroi);
  newroi->Delete();
  
  outPD->AddArray(newother);
  newother->Delete();

  if (this->ComputeStatistics) {
    outPD->AddArray(newminangle);
    newminangle->Delete(); 

    outPD->AddArray(newmaxangle);
    newmaxangle->Delete(); 

    outPD->AddArray(newmedangle);
    newmedangle->Delete();   

    outPD->AddArray(newmeanangle);
    newmeanangle->Delete(); 

    outPD->AddArray(newvarangle);
    newvarangle->Delete();  

    outPD->AddArray(newmintorsion);
    newmintorsion->Delete(); 

    outPD->AddArray(newmaxtorsion);
    newmaxtorsion->Delete(); 

    outPD->AddArray(newmedtorsion);
    newmedtorsion->Delete();   

    outPD->AddArray(newmeantorsion);
    newmeantorsion->Delete(); 

    outPD->AddArray(newvartorsion);
    newvartorsion->Delete(); 

    outPD->AddArray(newminother);
    newminother->Delete(); 

    outPD->AddArray(newmaxother);
    newmaxother->Delete(); 

    outPD->AddArray(newmedother);
    newmedother->Delete();   

    outPD->AddArray(newmeanother);
    newmeanother->Delete(); 

    outPD->AddArray(newvarother);
    newvarother->Delete(); 
  }

  if (dec != NULL)
    outPD->AddArray(newdec);

  output->SetPoints(newPts);
  newPts->Delete();

  output->SetLines(newCells);
  newCells->Delete();

}

