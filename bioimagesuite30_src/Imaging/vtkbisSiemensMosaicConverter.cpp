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





#include "vtkbisSiemensMosaicConverter.h"
#include "vtkImageExtractComponents.h"
#include "vtkbisImageReslice.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include <stdio.h>
#include "pxutil.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "nifti1_io.h"


vtkbisSiemensMosaicConverter* vtkbisSiemensMosaicConverter::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisSiemensMosaicConverter");
  if(ret)
    {
      return (vtkbisSiemensMosaicConverter*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisSiemensMosaicConverter;
}


// Construct object to extract all of the input data.
vtkbisSiemensMosaicConverter::vtkbisSiemensMosaicConverter()
{
  this->MosaicWidth=64;
  this->MosaicHeight=64;
  this->MosaicDepth=20;
}

// ---------------------------------------------------------------------------

void vtkbisSiemensMosaicConverter::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkSimpleImageToImageFilter::PrintSelf(os,indent);
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
int vtkbisSiemensMosaicConverter::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector, 
					  vtkInformationVector *outputVector)
{
  vtkInformation *inInfo =  inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  int wholeExtent[6]; inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExtent);
  double sp[3];  inInfo->Get(vtkDataObject::SPACING(), sp);
  double origin[3];  inInfo->Get(vtkDataObject::ORIGIN(),  origin);

  int nframes=wholeExtent[5]-wholeExtent[4]+1;

  int oldwidth=wholeExtent[3]-wholeExtent[2]+1;
  int oldheight=wholeExtent[1]-wholeExtent[0]+1;

  int nd=(oldwidth*oldheight)/(this->MosaicWidth*this->MosaicHeight);

  if (nd>this->MosaicDepth)
    nd=this->MosaicDepth;
  else
    this->MosaicDepth=nd;
  /*  fprintf(stderr,"oldwidth=%d, oldheight=%d nd=%d,nf=%d\n",
      oldwidth,oldheight,nd,nframes);*/
  
  wholeExtent[0]=0;
  wholeExtent[1]=this->MosaicWidth-1;
  wholeExtent[2]=0;
  wholeExtent[3]=this->MosaicHeight-1;
  wholeExtent[4]=0;
  wholeExtent[5]=nd-1;


  origin[0]=0.0;
  origin[1]=0.0;
  origin[2]=0.0;

  outInfo->Set(vtkDataObject::ORIGIN(), origin, 3);
  outInfo->Set(vtkDataObject::SPACING(), sp, 3);
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),wholeExtent,6);
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, nframes);
  return 1;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void vtkbisSiemensMosaicConverter::SimpleExecute(vtkImageData* input, vtkImageData* output) 
{
  

  int indim[3]; input->GetDimensions(indim);
  int odim[3]; output->GetDimensions(odim);
  int ncout=output->GetNumberOfScalarComponents();
  int ncin=input->GetNumberOfScalarComponents();

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();

  /*  fprintf(stderr,"odim=%dx%dx%d,%d idim=%dx%dx%d,%d\n",
	  odim[0],odim[1],odim[2],ncout,
	  indim[0],indim[1],indim[2],ncin);*/
  /*  double a[3]; output->GetOrigin(a);
  fprintf(stderr,"Out Ori=%f,%f,%f\n",a[0],a[1],a[2]);
   input->GetOrigin(a);
   fprintf(stderr,"In Ori=%f,%f,%f\n",a[0],a[1],a[2]);*/

  int inmatrix=indim[0]*indim[1];
  int numimageswide=indim[0]/odim[0];
  int numimagestall=indim[1]/odim[1];

  //  fprintf(stderr,"Numimageswide=%d\n",numimageswide);
  //  int outindex=0;

  for (int ia=0;ia<ncout;ia++)
    out->FillComponent(0,0.0);


  for (int newz=0;newz<odim[2];newz++)
    {
      int Bigy=int(newz/numimageswide);
      int Bigx=newz-int(Bigy*numimageswide);
      Bigy=(numimagestall-1)-Bigy;
      int outindex_z=newz*odim[0]*odim[1];
      //      fprintf(stderr,"\n ********* newz=%d BigY=%d BigX=%d\n",newz,Bigy,Bigx);
      for (int ny=0;ny<odim[1];ny++)
	{
	  //  int index=(Bigy+newy)*indim[0]+Bigx*odim[0];
	  int newy=(odim[1]-1)-ny;
	  int bigj=Bigy*odim[1]+newy;
	  int outindex=outindex_z+newy*odim[0];
	  int inindex =bigj*indim[0]+Bigx*odim[0];
	  for (int newx=0;newx<odim[0];newx++)
	    {
	      //	      int bigi=Bigx*odim[0]+newx;
	      for (int c=0;c<ncout;c++)
		out->SetComponent(outindex,c,inp->GetComponent(inindex+c*inmatrix,0));
		//		out->SetComponent(outindex,c,input->GetScalarComponentAsDouble(bigi,bigj,c,0));
		//		output->SetScalarComponentFromDouble(newx,newy,nz,c,input->GetScalarComponentAsDouble(bigi,bigj,c,0));
	      outindex++;
	      inindex++;
	    }
	}
    }
 
  //  this->OutputImageHeader->ReconcileImage(output,0);
}


