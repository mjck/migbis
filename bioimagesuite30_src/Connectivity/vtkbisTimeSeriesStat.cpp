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
#include "vtkbisTimeSeriesStat.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"


vtkbisTimeSeriesStat* vtkbisTimeSeriesStat::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisTimeSeriesStat");
  if(ret)
    {
      return (vtkbisTimeSeriesStat*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisTimeSeriesStat;
}
// ---------------------------------------------------------------------------------------------------------------------------
vtkbisTimeSeriesStat::vtkbisTimeSeriesStat() 
{
  this->Mode=0;
}
// ---------------------------------------------------------------------------------------------------------------------------
int vtkbisTimeSeriesStat::RequestInformation (vtkInformation * vtkNotUsed( request ),
					      vtkInformationVector ** vtkNotUsed( inputVector ),
					      vtkInformationVector * outputVector)
{

  vtkDataObject::SetPointDataActiveScalarInfo(
    outputVector->GetInformationObject(0), -1, 1);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    if (this->Mode<2)
      vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT , -1);
  return 1;
}


void vtkbisTimeSeriesStat::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{
  if (input==NULL || output==NULL)
    {
      fprintf(stderr,"Bad Image Input to vtkbisTimeSeriesStat SimpleExecute\n");
      return;
    }

  vtkDataArray* marr = output->GetPointData()->GetScalars();
  vtkDataArray* inp = input->GetPointData()->GetScalars();


  int nt=inp->GetNumberOfTuples();
  int nc=inp->GetNumberOfComponents();

  /*
  virtual void SetModeToMean() { this->SetMode(0); }
  virtual void SetModeToSum() { this->SetMode(1); }
  virtual void SetModeToMin() { this->SetMode(2); }
  virtual void SetModeToMax() { this->SetMode(3); }

  */
  for (int i=0;i<nt;i++)
    {
      double sum=0.0;
      double sum2=0.0;
      double vmin=inp->GetComponent(i,0);
      double vmax=vmin;
      for (int j=0;j<nc;j++)
	{
	  double v=inp->GetComponent(i,j);
	  sum+=v;
	  sum2+=v*v;
	  if (vmax<v)
	    vmax=v;
	  else if (vmin>v)
	    vmin=v;
	}
      
      double mean=sum/double(nc);
      double magn=sqrt(sum2);
      
      if (this->Mode==0)
	marr->SetComponent(i,0,mean);
      else if (this->Mode == 1 )
	marr->SetComponent(i,0,sum);
      else if (this->Mode == 2 )
	marr->SetComponent(i,0,vmin);
      else if (this->Mode == 3 )
	marr->SetComponent(i,0,vmax);
      else if (this->Mode == 4 )
	marr->SetComponent(i,0,magn);
    }
}

