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

/*
 * vtkbisDisplayROIMean.cpp
 *
 *  Created on: Sep 1, 2009
 *      Author: Isabella Murphy
 */

#include <vtkbisDisplayROIMean.h>
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkObject.h"
#include "vtkPointData.h"
#include "vtkbisImageTimeSeriesNormalize.h"
#include "vtkFloatArray.h"

using namespace std;

vtkbisDisplayROIMean::vtkbisDisplayROIMean() 
{
  this->ImageROI=NULL;
  this->AverageROI=NULL;
  
}

vtkbisDisplayROIMean::~vtkbisDisplayROIMean() 
{
  this->SetImageROI(NULL);
  this->SetAverageROI(NULL);
}

vtkbisDisplayROIMean* vtkbisDisplayROIMean::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisDisplayROIMean");
  if(ret)
    {
      return (vtkbisDisplayROIMean*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisDisplayROIMean;
}
/*-------------------------------------------------------------------------------------
 * force the output data type to float, need to change extend information to let
 * the pipeline know the difference
 * @Set: output data type = float
 */
int vtkbisDisplayROIMean::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector,
					  vtkInformationVector *outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
  return 1;
}
// ---------------------------------------------------------------------------------------------------------------------------
//   Main Function
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisDisplayROIMean::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{
  fprintf(stderr,"Beginning vtkbisDisplayROIMean\n");

  if (this->AverageROI==NULL || this->ImageROI==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs to vtkbisDisplayROIMean\n");
      return;
    }


  if (this->AverageROI->GetNumberOfScalarComponents()!=input->GetNumberOfScalarComponents())
    {
      fprintf(stderr,"Bad ROI mean Image in vtkbisDisplayROIMean SimpleExecute\n");
      return;
    }

  int dim[3],dim2[3];
  this->ImageROI->GetDimensions(dim);
  input->GetDimensions(dim2);
  int sum=0;
  for (int ia=0;ia<=2;ia++)
    sum+=abs(dim[ia]-dim2[ia]);
  if (sum>0)
    {
      fprintf(stderr,"Bad ROI Input to vtkbisROIMean SimpleExecute, wrong dimensions ROI=%d,%d,%d input=%d,%d,%d.\n",
	      dim[0],dim[1],dim[2],
	      dim2[0],dim2[1],dim2[2]);
      return;
    }

  double r[2]; this->ImageROI->GetPointData()->GetScalars()->GetRange(r);
  int ntr=this->AverageROI->GetPointData()->GetScalars()->GetNumberOfTuples();

  int numr=int(r[1]);
  if (ntr!=numr)
    {
      fprintf(stderr,"Average ROI Map does not agree with ROI Image ntr=%d numr=%d\n",ntr,numr);
      return;
    }


  vtkDataArray* inp=input->GetPointData()->GetScalars();
  int nt=inp->GetNumberOfTuples();//number of voxels
  int nc=inp->GetNumberOfComponents();//number of frames

  vtkDataArray* out=output->GetPointData()->GetScalars();
  for (int ia=0;ia<nc;ia++)
    out->FillComponent(ia,0.0);

  vtkDataArray* roi=roi=this->ImageROI->GetPointData()->GetScalars();
  vtkDataArray* roimean=this->AverageROI->GetPointData()->GetScalars();



  for(int i=0;i<nt;i++)
    {
      int roiregion = (int)roi->GetComponent(i,0);
      if(roiregion>0)
	{
	  for(int j=0;j<nc;j++)
	      out->SetComponent(i,j,roimean->GetComponent(roiregion-1,j));
	}
    }
}

