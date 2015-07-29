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





#include "vtkpxImageExtractVOI.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkbisImageReslice.h"
#include "pxutil.h"

vtkpxImageExtractVOI* vtkpxImageExtractVOI::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageExtractVOI");
  if(ret)
    {
      return (vtkpxImageExtractVOI*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageExtractVOI;
}


// Construct object to extract all of the input data.
vtkpxImageExtractVOI::vtkpxImageExtractVOI()
{
  this->VOI[0] = this->VOI[2] = this->VOI[4] = 0;
  this->VOI[1] = this->VOI[3] = this->VOI[5] = 1;
  this->SampleRate[0] = this->SampleRate[1] = this->SampleRate[2] = 1;
}

// ---------------------------------------------------------------------------

void vtkpxImageExtractVOI::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkpxSimpleImageToImageFilter::PrintSelf(os,indent);

  os << indent << "  VOI " << this->VOI[0] << ":" << this->VOI[1] << " ";
  os << indent << "  VOI " << this->VOI[2] << ":" << this->VOI[3] << " ";
  os << indent << "  VOI " << this->VOI[4] << ":" << this->VOI[5] << "\n";
  os << indent << "  SampleRate "<<this->SampleRate[0]<< ","<<this->SampleRate[1]<<","<<this->SampleRate[2]<<"\n";
}
// ---------------------------------------------------------------------------
void vtkpxImageExtractVOI::SetRegion(double center[3],double gridspacing[3],double scale)
{
  if (this->GetImageDataInput(0)==NULL)
    {
      vtkErrorMacro(<<"Set Input First before Calling SetRegion\n");
      fprintf(stderr,"Set Input First before Calling SetRegion\n");
      return;
    }

  

  int p1[3],p2[3];
  double origin[3];   this->GetImageDataInput(0)->GetOrigin(origin);
  double spacing[3];  this->GetImageDataInput(0)->GetSpacing(spacing);
  int  dim[3];        this->GetImageDataInput(0)->GetDimensions(dim);
  int extent[6];

  for (int ia=0;ia<=2;ia++)
    {
      double x1=center[ia]-scale*gridspacing[ia];
      x1=(x1-origin[ia])/spacing[ia];
      p1[ia]=Irange(int(x1),0,dim[ia]-1);

      double x2=center[ia]+scale*gridspacing[ia];
      x2=(x2-origin[ia])/spacing[ia];
      p2[ia]=Irange(int(x2),0,dim[ia]-1);
      
      extent[ia*2]=p1[ia];  extent[ia*2+1]=p2[ia];
    }

  this->SetVOI(extent);
  this->SetSampleRate(1,1,1);

  /*  fprintf(stderr,"::SetRegion:: Input (%.2f,%.2f,%.2f), sp=(%.2f,%.2f,%.2f) sc=%.2f --> (%d:%d,%d:%d,%d:%d)\n",
	  center[0],center[1],center[2],gridspacing[1],gridspacing[1],gridspacing[2],scale,
	  extent[0],extent[1],extent[2],extent[3],extent[4],extent[5]);*/
}

void vtkpxImageExtractVOI::ExecuteInformation()
{
  int i, outDims[3], voi[6];
  int rate[3];
  int wholeExtent[6];

  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  vtkImageData* input=this->GetImageDataInput(0);
  vtkImageData* output=this->GetOutput();
  
  if (this->GetImageDataInput(0) == NULL)
    {
      vtkErrorMacro("Missing input");
      return;
    }
  input->GetWholeExtent( wholeExtent );
  
  for ( i=0; i < 6; i++ )
    {
      voi[i] = this->VOI[i];
    }

  for ( i=0; i < 3; i++ )
    {
      if ( voi[2*i+1] > wholeExtent[2*i+1] )
	{
	  voi[2*i+1] = wholeExtent[2*i+1];
	}
      else if ( voi[2*i+1] < wholeExtent[2*i] )
	{
	  voi[2*i+1] = wholeExtent[2*i];
	}
      if ( voi[2*i] < wholeExtent[2*i] )
	{
	  voi[2*i] = wholeExtent[2*i];
	}
      else if ( voi[2*i] > wholeExtent[2*i+1] )
	{
	  voi[2*i] = wholeExtent[2*i+1];
	}
      
      if ( voi[2*i] > voi[2*i+1] )
	{
	  voi[2*i] = voi[2*i+1];
	}
      
      if ( (rate[i] = this->SampleRate[i]) < 1 )
	{
	  rate[i] = 1;
	}
      
      outDims[i] = (voi[2*i+1] - voi[2*i]) / rate[i] + 1;
      if ( outDims[i] < 1 )
	{
	  outDims[i] = 1;
	}
    }
  
  // This makes sense for sample rates of 1, 1, 1.
  wholeExtent[0] = 0;
  wholeExtent[1] = outDims[0] - 1;
  wholeExtent[2] = 0;
  wholeExtent[3] = outDims[1] - 1;
  wholeExtent[4] = 0;
  wholeExtent[5] = outDims[2] - 1;
  
  output->SetDimensions(outDims);
  output->SetWholeExtent( wholeExtent );

  double spacing[3]; input->GetSpacing(spacing);
  double origin[3];  input->GetOrigin(origin);

  output->SetSpacing(spacing[0]*rate[0],spacing[1]*rate[1],spacing[2]*rate[2]);
  output->SetOrigin(voi[0]*spacing[0]+origin[0],
		    voi[2]*spacing[1]+origin[1],
		    voi[4]*spacing[2]+origin[2]);




}
// ---------------------------------------------------------------------------
void vtkpxImageExtractVOI::SimpleExecute(vtkImageData* input, vtkImageData* output) 
{
  if (input==NULL)
    {
      vtkErrorMacro(<< "Bad Input");
      return;
    }

  int dim[3]; output->GetDimensions(dim);


  vtkbisImageReslice* resl=vtkbisImageReslice::New();
  resl->SetInput(input);
  resl->SetOutputOrigin(output->GetOrigin());
  resl->SetOutputSpacing(output->GetSpacing());
  resl->SetInterpolationMode(0);
  resl->SetOutputExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  resl->OptimizationOff();
  resl->Update();

  output->ShallowCopy(resl->GetOutput());
  resl->Delete();
}




