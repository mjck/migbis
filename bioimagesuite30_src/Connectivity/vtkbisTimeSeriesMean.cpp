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
#include "vtkPointData.h"
#include "vtkbisTimeSeriesMean.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"


vtkbisTimeSeriesMean* vtkbisTimeSeriesMean::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisTimeSeriesMean");
  if(ret)
    {
      return (vtkbisTimeSeriesMean*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisTimeSeriesMean;
}

// ---------------------------------------------------------------------------------------------------------------------------
int vtkbisTimeSeriesMean::RequestInformation (vtkInformation * vtkNotUsed( request ),
					      vtkInformationVector ** vtkNotUsed( inputVector ),
					      vtkInformationVector * outputVector)
{

  vtkDataObject::SetPointDataActiveScalarInfo(
    outputVector->GetInformationObject(0), -1, 1);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT , -1);
  return 1;
}


void vtkbisTimeSeriesMean::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{
  if (input==NULL || output==NULL)
    {
      fprintf(stderr,"Bad Image Input to vtkbisTimeSeriesMean SimpleExecute\n");
      return;
    }

  vtkDataArray* marr = output->GetPointData()->GetScalars();
  vtkDataArray* inp = input->GetPointData()->GetScalars();


  int nt=inp->GetNumberOfTuples();
  int nc=inp->GetNumberOfComponents();

  for (int i=0;i<nt;i++)
    {
      double sum=0.0;
      for (int j=0;j<nc;j++)
	sum+=inp->GetComponent(i,j);
      sum/=double(nc);
      marr->SetComponent(i,0,sum);
    }
}

