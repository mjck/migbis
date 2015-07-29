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





/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxProcessFmri.cpp,v $
  Language:  C++
  Date:      $Date: 2002/08/16 15:24:36 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkImageFlip.h"
#include "vtkObjectFactory.h"
#include "vtkSource.h"
#include "vtkpxProcessFmri.h"
#include "vtkpxUtil.h"
#include <vtkImageShiftScale.h> 
#include <vtkLineSource.h>
#include <vtkPolyData.h>
#include <vtkMath.h>
#include <vtkbisImageReslice.h>
#include <vtkTransform.h>
#include <vtkDataArray.h>
#include <vtkFloatArray.h>
//------------------------------------------------------------------------------
vtkpxProcessFmri* vtkpxProcessFmri::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxProcessFmri");
  if(ret)
    {
    return (vtkpxProcessFmri*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxProcessFmri;
}

// Construct object with no children.
vtkpxProcessFmri::vtkpxProcessFmri()
{
  this->Activations=NULL;
  this->ScaleFactor=1000.0;
  this->IntensityRescaling=1;
  this->FlipLeftRight=1;
}
// ----------------------------------------------------------------------------
vtkpxProcessFmri::~vtkpxProcessFmri()
{
  if (this->Activations!=NULL)
    {
      this->Activations->UnRegister(this);
      this->Activations=NULL;
    }

}
// ----------------------------------------------------------------------------
void vtkpxProcessFmri::SetActivations(vtkDataArray* act)
{
  if (this->Activations != NULL )
    this->Activations->UnRegister(this);
  
  this->Activations=act;
  if (this->Activations!=NULL)
    this->Activations->Register(this);
}
// ----------------------------------------------------------------------------
void vtkpxProcessFmri::ExecuteInformation()
{
  vtkImageData *input=this->GetImageDataInput(0);

  if (input==NULL)
      {
	vtkErrorMacro(<<"No Input FMRI Data !!");
	return;
      }

  vtkImageData *output=this->GetOutput();

  vtkpxSimpleImageToImageFilter::ExecuteInformation();
  output->SetNumberOfScalarComponents(1);
  output->SetScalarType(VTK_SHORT);
}

// ----------------------------------------------------------------------------
void vtkpxProcessFmri::CheckActivationSeries(vtkDataArray* act,int timeframes,
					     float& Numbaseline,float& Numactivation)
{

  Numbaseline=0.0;
  Numactivation=0.0;
  for (int tm=0;tm<timeframes;tm++)
    {
      int v=(int)act->GetComponent(tm,0);
      if (v==0)
	Numbaseline+=1.0;
      else if (v==1)
	Numactivation+=1.0;
    }

  if (Numbaseline<1 || Numactivation<1)
    {
      vtkErrorMacro(<<"Bad activation scalars ("<<Numbaseline<<","<<Numactivation<<")\n");
      return;
    }
  vtkDebugMacro(<<"Activation scalars ("<<Numbaseline<<","<<Numactivation<<")\n");

}

// ----------------------------------------------------------------------------

void vtkpxProcessFmri::NormalizeIntensity(vtkImageData* input,vtkImageData* output,
					  float newmean)
{

  int dim[3];   input->GetDimensions(dim);
  
  output->SetSpacing(input->GetSpacing());
  output->SetOrigin(input->GetOrigin());
  output->SetScalarType(VTK_SHORT);
  output->SetNumberOfScalarComponents(1);
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  output->AllocateScalars();
  vtkDataArray* outScal=output->GetPointData()->GetScalars();
  vtkDataArray* inScal=input->GetPointData()->GetScalars();
 
  int slicesize=dim[0]*dim[1];
  for (int sl=0;sl<dim[2];sl++)
    {
      int offset=sl*slicesize;
      float sum=0.0;
      for (int pixel=0;pixel<slicesize;pixel++)
	sum+=inScal->GetComponent(offset+pixel,0);
      sum/=float(slicesize);
	    
      if (sum==0.0)
	sum=1.0;

      float scale=newmean/sum;

      float newsum=0.0;
      for (int px=0;px<slicesize;px++)
	{
	  int index=offset+px;
	  float v=scale*inScal->GetComponent(index,0);
	  newsum+=v;
	  outScal->SetComponent(index,0,v);
	}

      newsum/=float(slicesize);
    }
}
// ----------------------------------------------------------------------------


void vtkpxProcessFmri::SimpleExecute(vtkImageData* input,vtkImageData *output)
{
  if (this->Activations==NULL)
      {
	vtkErrorMacro(<<"No Activation Scalars Supplied\n");
	return;
      }

  int numtimeframes=input->GetNumberOfScalarComponents();
  int dim[3];
  input->GetDimensions(dim);

  if (this->Activations->GetNumberOfTuples()<numtimeframes)
      {
	vtkErrorMacro(<<"Not Enough Activation Scalars Supplied ");
	vtkErrorMacro(<< this->Activations->GetNumberOfTuples()<<" <" <<numtimeframes<< "\n");
	return;
      }
  
  // Create Output Array
  // -------------------
  this->UpdateProgress(0.01);
  float oldpog=0.00;


  // First Check Activation Series 
  float Numbaseline=0.0,Numactivation=0.0;
  this->CheckActivationSeries(this->Activations,numtimeframes,Numbaseline,Numactivation);

  vtkImageData* fmriData=this->GetImageDataInput(0);
  if (this->IntensityRescaling)
    {
      // Normalize all slices to have mean 1200 for some reason of Todd's crafting
      fmriData=vtkImageData::New();
      vtkpxProcessFmri::NormalizeIntensity(this->GetImageDataInput(0),fmriData,1200.0);
      oldpog=0.4;
      this->UpdateProgress(oldpog);
    }


  output->SetSpacing(this->GetImageDataInput(0)->GetSpacing());
  output->SetOrigin(this->GetImageDataInput(0)->GetOrigin());
  output->SetScalarType(VTK_SHORT);
  output->SetNumberOfScalarComponents(1);
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  output->AllocateScalars();

  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* fmriScalars=fmriData->GetPointData()->GetScalars();
  
  int slicesize=dim[0]*dim[1];
  for (int sl=0;sl<dim[2];sl++)
    {
      for (int i=0;i<dim[0];i++)
	{
	  int outi=i;
	  if (this->FlipLeftRight)
	    outi=(dim[0]-1)-i;
	  for (int j=0;j<dim[1];j++)
	    {
	      int bpixel=j*dim[0];
	      int pixel=bpixel+i;
	      int outpixel=bpixel+outi;
	      float sumbase=0.0,sum2base=0.0,sumact=0.0,sum2act=0.0;
	      
	      int   readindex =pixel+sl*slicesize;
	      int   writeindex=outpixel+sl*slicesize;
	      
	      for (int tm=0;tm<numtimeframes;tm++)
		{

		  float val=fmriScalars->GetComponent(readindex,tm);
		  int   act=(int)Activations->GetComponent(tm,0);
		 	  
		  if (act==0)
		    {
		      sumbase+=val;
		      sum2base+= (val*val);
		    }
		  else if (act==1)
		    {
		      sumact+=val;
		      sum2act+= (val*val);
		    }
		}

	      
	      float meanbase  =sumbase  /Numbaseline;
	      float meanact   =sumact   /Numactivation;
	      
	      float mean2base =sum2base /Numbaseline;
	      float mean2act  =sum2act  /Numactivation;
	      
	      float sigmabase=sqrt(mean2base-meanbase*meanbase);
	      float sigmaact =sqrt(mean2act -meanact*meanact);
	      
	      float tsigma=0.5*(sigmabase+sigmaact);
	      
	      if (tsigma<0.00001)
		  tsigma=0.00001;
	      

	      int  tvalue=int(0.5+this->ScaleFactor*(meanact-meanbase)/tsigma);
	      out->SetComponent(writeindex,0,float(tvalue));
	    }
	}
      float pog=(0.4+0.6*float(sl)/float(dim[2]));
      if (pog-oldpog>0.1)
	{
	  this->UpdateProgress(pog);	
	  oldpog=pog;
	}
      
    }
  
  this->UpdateProgress(oldpog+0.5);
  this->UpdateProgress(1.0);

  if (this->IntensityRescaling)
    fmriData->Delete();
}

// -------------------------------------------------------------------------------------------
//     GUI Support Functions for handling activations as stings of ranges 
// -------------------------------------------------------------------------------------------
void addScalars(vtkDataArray* scal,int num,char* specs,int val,int tshift)
{
  if (scal==NULL || specs==NULL || num<2)
    return;
  
  char* out=strtok(specs," ");  
  for (int i=0;i<num/2;i++)
    {
      int begin=0,end=0;
      if (out!=NULL)
	begin=atoi(out)+tshift;
      out=strtok(NULL," ");
      if (out!=NULL)
	end=atoi(out)+tshift;
      
      //fprintf(stderr,"Setting values %d:%d as %d\n",begin,end,val);
      for (int j=begin-1;j<end;j++)
	{
	  if (j<scal->GetNumberOfTuples() && j>=0)
	    scal->SetComponent(j,0,val);
	}
      out=strtok(NULL," ");
    }
}
// ----------------------------------------------------------------------------------------------
void vtkpxProcessFmri::CreatePolyDataFromScalars(vtkDataArray* activations,vtkPolyData* polydata)
{
  if (polydata==NULL || activations==NULL)
      return;

  int frames=activations->GetNumberOfTuples();

  vtkLineSource* linesource=vtkLineSource::New();
  linesource->SetResolution(frames);
  linesource->SetPoint1(0.0, 0.0,  0.0);
  linesource->SetPoint2(1.0, 0.0,  0.0);
  linesource->Update();

  polydata->CopyStructure(linesource->GetOutput());
  linesource->Delete();
  
  vtkFloatArray* tmpScalars=vtkFloatArray::New();
  tmpScalars->SetNumberOfTuples(frames +1);
  tmpScalars->SetComponent(0,0,0.5);

  for (int k=1;k<=frames;k++)
      {
	float val=activations->GetComponent(k-1,0);
	if (val<0.0)
	    val=0.5;
	tmpScalars->SetComponent(k,0,val);
      }
  polydata->GetPointData()->SetScalars(tmpScalars);
  tmpScalars->Delete();


}
// --------------------------------------------------------------------------------------
void vtkpxProcessFmri::CreatePolyDataFromScalars(vtkDataArray* activations,vtkPolyData* polydata,float scale)
{
  if (polydata==NULL || activations==NULL)
      return;

  int frames=activations->GetNumberOfTuples();

  vtkLineSource* linesource=vtkLineSource::New();
  linesource->SetResolution(frames);
  linesource->SetPoint1(0.0, 0.0,  0.0);
  linesource->SetPoint2(1.0, 0.0,  0.0);
  linesource->Update();

  polydata->CopyStructure(linesource->GetOutput());
  linesource->Delete();
  
  vtkFloatArray* tmpScalars=vtkFloatArray::New();
  tmpScalars->SetNumberOfTuples(frames +1);
  tmpScalars->SetComponent(0,0,0.0);

  for (int k=1;k<=frames;k++)
      {
	float val=activations->GetComponent(k-1,0);
	tmpScalars->SetComponent(k,0,val*scale);
      }
  polydata->GetPointData()->SetScalars(tmpScalars);
  tmpScalars->Delete();

}
// --------------------------------------------------------------------------------------
void vtkpxProcessFmri::ProcessScalars(vtkDataArray* activations,int frames,int tshift,
				      int numact,char* act,
				      int numbase,char* base)
{
  if (activations==NULL)
      return;

  activations->SetNumberOfTuples(frames);
  for (int i=0;i<frames;i++)
      activations->SetComponent(i,0,-1);

  char longline[300];
  strncpy(longline,act,300);   addScalars(activations,numact,longline,1,tshift);
  strncpy(longline,base,300);  addScalars(activations,numbase,longline,0,tshift);

}


