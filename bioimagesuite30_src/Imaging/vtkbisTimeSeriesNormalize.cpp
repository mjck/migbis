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
#include "vtkbisTimeSeriesNormalize.h"
#include "vtkpxAnalyzeImageWriter.h"

vtkbisTimeSeriesNormalize* vtkbisTimeSeriesNormalize::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisTimeSeriesNormalize");
  if(ret)
    {
      return (vtkbisTimeSeriesNormalize*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisTimeSeriesNormalize;
}

vtkbisTimeSeriesNormalize::vtkbisTimeSeriesNormalize()
{
  this->NormalizedThreshold=1;
  this->Threshold=0.05;
  this->DebugName=NULL;
  this->ComputeMeanOnly=0;
}

vtkbisTimeSeriesNormalize::~vtkbisTimeSeriesNormalize()
{
  if (this->DebugName!=NULL)
    delete [] this->DebugName;
}

// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisTimeSeriesNormalize::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();

  output->SetScalarTypeToFloat();
  if (this->ComputeMeanOnly==1)
    output->SetNumberOfScalarComponents(1);
}

void vtkbisTimeSeriesNormalize::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{
  if (input==NULL || output==NULL)
    {
      fprintf(stderr,"Bad Image Input to vtkbisTimeSeriesNormalize SimpleExecute\n");
      return;
    }

  
  vtkImageData* mean=vtkImageData::New();
  mean->CopyStructure(input);
  mean->SetNumberOfScalarComponents(1);
  mean->SetScalarTypeToDouble();
  mean->AllocateScalars();

  vtkDataArray* marr = mean->GetPointData()->GetScalars();
  vtkDataArray* inp = input->GetPointData()->GetScalars();
  vtkDataArray* out =output->GetPointData()->GetScalars();

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

  if (this->ComputeMeanOnly)
    {
      out->CopyComponent(0,marr,0);
    }
  else
    {
      double thr=this->Threshold;
      if (this->NormalizedThreshold>0)
	{
	  double range[2]; marr->GetRange(range);
	  thr=this->Threshold*(range[1]-range[0])+range[0];
	}
      
      //      fprintf(stderr,"Using Threhsold = %.2f (%.3f)\n",thr,this->Threshold);

      for (int i=0;i<nt;i++)
	{
	  double m=marr->GetComponent(i,0);
	  if (m>thr)
	    {
	      double scale=100.0/m;
	      for (int j=0;j<nc;j++)
		out->SetComponent(i,j,inp->GetComponent(i,j)*scale);
	    }
	  else
	    {
	      for (int j=0;j<nc;j++)
		out->SetComponent(i,j,0.0);
	    }
	}
      
      
      if (this->DebugName!=NULL)
	{
	  vtkpxAnalyzeImageWriter* w=vtkpxAnalyzeImageWriter::New();
	  w->SetInput(mean);
	  w->Save(this->DebugName);
	  fprintf(stderr,"Mean Image saved in %s\n",this->DebugName);
	  w->Delete();
	}
    }
  mean->Delete();
}

