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
  Module:    $RCSfile: vtkpxProcessRawBrukerFIDImage.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:02:52 $
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
#include "vtkObjectFactory.h"
#include "vtkpxProcessRawBrukerFIDImage.h"
#include "vtkpxUtil.h"
#include "vtkImageRFFT.h"
#include "vtkImageFFT.h"
#include "vtkImageFourierCenter.h"
#include "vtkImageShiftScale.h"
#include "vtkImageCast.h"
#include "vtkpxImagePolarToEuclidean.h"
#include "vtkpxImageExtract.h"
#include "vtkbisImageReslice.h"
#include "vtkImagePermute.h"
#include "vtkTransform.h"
#include "pxutil.h"
#include "vtkpxImageAddPhaseCorrection.h"
#include "vtkpxMatrix.h"
#include "vtkpxMath.h"
#include "vtkImageMedian3D.h"

#ifndef M_PI
#define M_PI 3.1415926854
#endif

//------------------------------------------------------------------------------
vtkpxProcessRawBrukerFIDImage* vtkpxProcessRawBrukerFIDImage::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxProcessRawBrukerFIDImage");
  if(ret)
    {
    return (vtkpxProcessRawBrukerFIDImage*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxProcessRawBrukerFIDImage;
}

// Construct object with no children.
vtkpxProcessRawBrukerFIDImage::vtkpxProcessRawBrukerFIDImage()
{
  this->EchoPlanarMode=0;
  this->PolarComplexMode=1;
  this->ShiftSpectrum=1;
  this->ConventionalAppendMultiSliceMode=0;
}
// ----------------------------------------------------------------------------
void vtkpxProcessRawBrukerFIDImage::ExecuteInformation()
{
  vtkImageData *input=this->GetImageDataInput(0);

  if (input==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }

  vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();
  output->SetScalarType(VTK_FLOAT);
}


void vtkpxProcessRawBrukerFIDImage::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  vtkImageData* tmp=vtkImageData::New();
  tmp->CopyStructure(input);
  int numscalars=input->GetNumberOfScalarComponents();

  // If multi-echo put echos last to make slices 
  if (numscalars!=2)
    {
      int dim[3]; input->GetDimensions(dim);
      tmp->SetDimensions(dim[0],dim[1],dim[2]*(numscalars/2));
      tmp->SetNumberOfScalarComponents(2);
      tmp->AllocateScalars();
      vtkpxProcessRawBrukerFIDImage::ReOrderMultipleEchos(input,tmp);
    }
  else
    {
      tmp->AllocateScalars();
    }

  
  if (this->EchoPlanarMode==0)
    {
      if (this->ConventionalAppendMultiSliceMode==1)
	{
	  vtkDebugMacro(<<"Simple Deep Copy\n");
	  output->DeepCopy(input);
	}
      else
	{
	  vtkpxProcessRawBrukerFIDImage::ConvertConventional(input,tmp);
	  vtkDebugMacro(<<"Convert Conventional\n");
	}
    }
  else
    {
      vtkDebugMacro(<<"Convert Echoplanar\n");
      vtkpxProcessRawBrukerFIDImage::ConvertEchoPlanar(input,tmp);
    }
  
  if (this->PolarComplexMode)
    {
      vtkDebugMacro(<<"IFFT\n");
      vtkpxProcessRawBrukerFIDImage::PerformInverseFourierTransform(tmp,output,this->ShiftSpectrum);
    }
  else
    {
      vtkImageCast* cast=vtkImageCast::New();
      cast->SetInput(tmp);
      cast->SetOutputScalarTypeToFloat();
      cast->Update();

      if (this->ShiftSpectrum)
	{
	  vtkImageData* tmp2=vtkImageData::New();
	  vtkDebugMacro(<<"Shift Spectrum\n");
	  vtkpxProcessRawBrukerFIDImage::DoShiftSpectrum(cast->GetOutput(),tmp2);
	  output->ShallowCopy(tmp2);
	  tmp2->Delete();
	}
      else
	{
	  output->ShallowCopy(cast->GetOutput());
	}

      cast->Delete();
    }

  tmp->Delete();

}
// ----------------------------------------------------------------------------
int vtkpxProcessRawBrukerFIDImage::ConvertConventional(vtkImageData* input,vtkImageData* output)
{
  
  vtkIntArray* out=(vtkIntArray*)output->GetPointData()->GetScalars();
  vtkIntArray* inp=(vtkIntArray*)input ->GetPointData()->GetScalars();

  int* in=(int*)inp->GetVoidPointer(0);
  int* ou=(int*)out->GetVoidPointer(0);


  int dim[3];
  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();

  int outindex=0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      {
	int inindex=2*(j*dim[2]+k)*dim[0];
	for (int i=0;i<dim[0];i++)
	  for (int c=0;c<nc;c++)
	    {
	      ou[outindex]=in[inindex];
	      ++outindex;
	      ++inindex;
	    }
      }

  return 1;
}
// ---------------------------------------------------------------------------- 
int vtkpxProcessRawBrukerFIDImage::ConvertEchoPlanar(vtkImageData* input,vtkImageData*output)
{
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* inp=input ->GetPointData()->GetScalars();

  int* in=(int*)inp->GetVoidPointer(0);
  int* ou=(int*)out->GetVoidPointer(0);

  int dim[3];
  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();
  int slicesize=dim[0]*dim[1];

  int revi=0;
  int half=dim[2]/2;
  if (half*2<dim[2])
    half+=1;

  for (int k=0;k<dim[2];k++)
    {
      int outslice=0;
      if (k<half)
	outslice=k*2;
      else
	outslice=(k-half)*2+1;

      for (int j=0;j<dim[1];j++)
	{
	  for (int ti=0;ti<dim[0];ti++)
	    {
	      int i=ti;
	      if (revi==1)
		i=dim[0]-1-ti;
	      for (int c=0;c<nc;c++)
		{
		  int outindex=2*(outslice*slicesize+j*dim[0]+i)+c;
		  int inpindex=2*(k*slicesize+j*dim[0]+ti)+c;
		  ou[outindex]=in[inpindex];
		  //out->SetComponent(outindex,c,inp->GetComponent(inpindex,c));
		}
	    }
	  revi=1-revi;
	}
    }
  return 1;
}
// ----------------------------------------------------------------------------
int vtkpxProcessRawBrukerFIDImage::PerformInverseFourierTransform(vtkImageData* input,
								  vtkImageData*output,int shiftspectrum)
{
  vtkImageCast* cast=vtkImageCast::New();
  cast->SetInput(input);
  cast->SetOutputScalarTypeToFloat();
  cast->Update();

  vtkImageRFFT* rfft=vtkImageRFFT::New();
  rfft->SetDimensionality(2);

  if (shiftspectrum)
    {
      vtkImageData* tmp=vtkImageData::New();
      vtkpxProcessRawBrukerFIDImage::DoShiftSpectrum(cast->GetOutput(),tmp); 
      rfft->SetInput(tmp);
      tmp->Delete();
    }
  else
    rfft->SetInput(cast->GetOutput());


  vtkTransform* tr=vtkTransform::New();
  double sp[3];  input->GetSpacing(sp);
  int   dim[3]; input->GetDimensions(dim);
  tr->Identity();
  tr->Translate(0.5*dim[0]*sp[0],0.5*dim[1]*sp[1],0);
      
  vtkbisImageReslice* resl=vtkbisImageReslice::New(); resl->OptimizationOff();
  resl->SetInput(rfft->GetOutput());
  resl->SetInformationInput(input);
  resl->SetResliceTransform(tr);
  tr->Delete();
  resl->WrapOn();
  resl->SetInterpolationMode(0);
  

  vtkpxImagePolarToEuclidean* pol=vtkpxImagePolarToEuclidean::New();
  pol->SetInput(resl->GetOutput());
  pol->SetInverse(1);
  pol->Update();
  output->ShallowCopy(pol->GetOutput());

  pol->Delete();
  resl->Delete();
  rfft->Delete();
  cast->Delete();


  return 1;
}

// ----------------------------------------------------------------------------
int vtkpxProcessRawBrukerFIDImage::DoShiftSpectrum(vtkImageData* input,vtkImageData* output)
{
  int dim[3];
  input->GetDimensions(dim);

  output->CopyStructure(input);
  output->AllocateScalars();

  vtkDataArray* out=output->GetPointData()->GetScalars();

  for (int sl=0;sl<dim[2];sl++)
    {
      float maxmagn=0.0;
      int i=0,j=0,besti=0,bestj=0;
      
      for (j=0;j<dim[1];j++)
	for (i=0;i<dim[0];i++)
	  {
	    float re=input->GetScalarComponentAsDouble(i,j,sl,0);
	    float im=input->GetScalarComponentAsDouble(i,j,sl,1);
	    float magn=re*re+im*im;
	    if (magn>maxmagn)
	      {
		maxmagn=magn;
		besti=i;
		bestj=j;
	      }
	  }
      
      /*besti=0;
	bestj=0;*/
	  
      int index=sl*dim[0]*dim[1];
      for (j=0;j<dim[1];j++)
	{
	  int oj=Icyclic(j+bestj,dim[1]);
	  
	  for (i=0;i<dim[0];i++)
	    {
	      int oi=Icyclic(i+besti,dim[0]);
	      float a=input->GetScalarComponentAsDouble(oi,oj,sl,0);
	      float b=input->GetScalarComponentAsDouble(oi,oj,sl,1);
	      out->SetComponent(index,0,a);
	      out->SetComponent(index,1,b);
	      ++index;
	    }
	}
    }

  /*if (this->EchoPlanarMode)
    vtkpxImageAddPhaseCorrection::DoGhostCorrection(output,0.25);*/

  return 1;
}
// ----------------------------------------------------------------------------
int vtkpxProcessRawBrukerFIDImage::ReOrderMultipleEchos(vtkImageData* input,vtkImageData* output)
{
  vtkIntArray* out=(vtkIntArray*)output->GetPointData()->GetScalars();
  vtkIntArray* inp=(vtkIntArray*)input ->GetPointData()->GetScalars();

  int* in=(int*)inp->GetVoidPointer(0);
  int* ou=(int*)out->GetVoidPointer(0);

  int dim[3];
  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();

  int nechos=nc/2;

  fprintf(stderr,"In Order Echoes dim=%dx%dx%d nc=%d (%d)\n",dim[0],dim[1],dim[2],nc,nechos);

  
  int slicesize=dim[0]*dim[1];
  int volsize=slicesize*dim[2];

  int inindex=0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  int outindex= k*slicesize+j*dim[0]+i;
	  
	  for (int echo=0;echo<nechos;echo++)
	    {
	      for (int c=0;c<2;c++)
		{
		  ou[outindex]=in[inindex];
		  ++outindex;
		  ++inindex;
		}
	      outindex+=volsize;
	    }
	}
  return 1;
}

// ----------------------------------------------------------------------------
void vtkpxProcessRawBrukerFIDImage::FitFIDIntoGnuplot(char* fname1,char* fname2,int width,int height,int depth,int components)
{
  gzFile fp=gzsuffixopen(fname1,"rb");//FILE* fp = fopen(fname1,"rb");
  if ( !fp)
    {
      fprintf(stderr,"Cannot Open File %s\n", fname1 );
      return;
    }
  
  fprintf(stderr,"Size of long = %d\n",sizeof(long));

  int numelements=2*width*height*depth*components;
  long* data=new long[numelements];
  //fread(&data[0],sizeof(long),numelements,fp);
  //fclose(fp);
  gzread(fp,&data[0],sizeof(long)*numelements);
  gzclose(fp);

  FILE* fout=fopen(fname2,"w");
  fprintf(fout,"# ZIndex \t Components \n");
  for (int k=0;k<numelements;k+=2)
    fprintf(fout,"%d %d %d %f",k,(int)data[k],(int)data[k+1],(float)sqrt(float(data[k]*data[k]+data[k+1]*data[k+1])));

  fclose(fout);
  delete [] data;
}

vtkImageData* vtkpxProcessRawBrukerFIDImage::ReadFIDSpectrum(char* fname,int width,int height,int sliceno,int depth,int components)
{
  //FILE* fp = fopen(fname,"rb");
  gzFile fp = gzsuffixopen(fname,"rb");
  if ( !fp)
    {
      fprintf(stderr,"Cannot Open File %s\n", fname );
      return NULL;
    }
  
  fprintf(stderr,"Size of long = %d\n",sizeof(long));

  int numelements=2*width*height*depth*components;
  long* data=new long[numelements];
  /*  int np=fread(&data[0],sizeof(long),numelements,fp);
      fclose(fp);*/
  int np=gzread(fp,&data[0],sizeof(long)*numelements);
  gzclose(fp);


  fprintf(stderr,"np=%d numelements=%d  bytes=%d\n",np,numelements,np*sizeof(long));

  int depth_components=depth*components;
  int depth_components_width=depth_components*width;

  fprintf(stderr,"Read %d values (%d)\n",np,numelements);

  vtkImageData* img=vtkImageData::New();
  img->SetScalarTypeToFloat();
  img->SetNumberOfScalarComponents(2);
  img->SetDimensions(width,height,components);
  img->SetWholeExtent(0,width-1,0,height-1,0,components-1);
  img->SetExtent(0,width-1,0,height-1,0,components-1);
  img->SetSpacing(1.0,1.0,float(height)/float(components));
  img->AllocateScalars();


  vtkFloatArray* array=(vtkFloatArray*)(img->GetPointData()->GetScalars());

  fprintf(stderr,"sliceno=%d components=%d depth_components=%d depth_components_width=%d\n",
	  sliceno,components,depth_components,depth_components_width);

  int index=0;
  for (int c=0;c<components;c++)
    for (int j=0;j<height;j++)
      for (int i=0;i<width;i++)
	{
	  int offset= ((components-1)-c) + sliceno*components +  i * depth_components +  j * depth_components_width;

	  offset=2*offset;
	  array->SetComponent(index,0,data[offset]);
	  array->SetComponent(index,1,data[offset+1]);
	  ++index;
	}

  delete [] data;

  return img;
}

vtkImageData* vtkpxProcessRawBrukerFIDImage::ProcessFIDSpectrum(char* fname,int width,int height,int sliceno,int depth,
								int components,float scalefactor)
{
  
  vtkImageData* img=vtkpxProcessRawBrukerFIDImage::ReadFIDSpectrum(fname,width,height,sliceno,depth,components);

  if (img==NULL)
    return NULL;

  vtkImageFFT* sh=vtkImageFFT::New();
  sh->SetDimensionality(3);
  sh->SetInput(img);

  vtkImageFourierCenter* ct2=vtkImageFourierCenter::New();
  ct2->SetDimensionality(3);
  ct2->SetInput(sh->GetOutput());

  vtkImageShiftScale* sc=vtkImageShiftScale::New();
  sc->SetInput(ct2->GetOutput());
  sc->SetScale(scalefactor);
  sc->Update();

  vtkImageData* output=vtkpxProcessRawBrukerFIDImage::PhaseCorrection(sc->GetOutput(),2.0,0.0);

  sc->Delete();
  ct2->Delete();
  sh->Delete();
  img->Delete();

  return output;


}


int vtkpxProcessRawBrukerFIDImage::PlotImageComponentToGnuplot(vtkImageData* image,int x,int y,char* output)
{
  FILE* fout=fopen(output,"w");
  if (fout==NULL)
    return 0;

  int dim[3]; image->GetDimensions(dim);

  fprintf(fout,"# ZIndex \t Components \n");
  for (int k=0;k<dim[2];k++)
    {
      fprintf(fout,"%d ",k);
      for (int c=0;c<image->GetNumberOfScalarComponents();c++)
	fprintf(fout,"%.3f ",image->GetScalarComponentAsDouble(x,y,k,c));
      fprintf(fout,"\n");
    }

  fclose(fout);
  return 1;
}

int vtkpxProcessRawBrukerFIDImage::ReadFIDSpectrumAndSpitIntoFile(char* fname,char* outname,int width,int height,int depth,int components)
{
  gzFile fp = gzsuffixopen(fname,"rb");//  FILE* fp = fopen(fname,"rb");
  if ( !fp)
    {
      fprintf(stderr,"Cannot Open File %s\n", fname );
      return 0;
    }
  
  fprintf(stderr,"Size of long = %d\n",sizeof(long));
  
  int numelements=2*width*height*depth*components;
  long* data=new long[numelements];
  /*  int np=fread(&data[0],sizeof(long),numelements,fp);
      fclose(fp);*/
  int np=gzread(fp,&data[0],sizeof(long)*numelements);
  gzclose(fp);


  fprintf(stderr,"np=%d numelements=%d  bytes=%d\n",np,numelements,np*sizeof(long));

  FILE* fout=fopen(outname,"w");
  for (int i=0;i<numelements;i+=2)
    fprintf(fout,"%d %d %d\n",i/2,(int)data[i],(int)data[i+1]);
  fclose(fout);
      
  return 1;

}


vtkImageData* vtkpxProcessRawBrukerFIDImage::PhaseCorrection(vtkImageData* complexinput,float theta,float theta2)
{
  vtkImageData* output=vtkImageData::New();
  output->DeepCopy(complexinput);

  int dim[3]; output->GetDimensions(dim);

  vtkFloatArray* data=(vtkFloatArray*)output->GetPointData()->GetScalars();

  int index=0;
  
  float t1=theta;
  float t2=theta2;

  fprintf(stderr,"t1=%.2f, t2=%.2f (theta=%.2f,theta2=%.2f)\n",t1,t2,theta,theta2);

  for (int k=0;k<dim[2];k++)
    {
      float offset =t1*float(k);
      float offset2=t2;

      if (k==dim[2]/2)
	fprintf(stderr,"Offsets = %f %f\n",offset,offset2);

      for (int i=0;i<dim[0];i++)
	for (int j=0;j<dim[1];j++)
	  {
	    float X = data->GetComponent(index,0);
	    float Y = data->GetComponent(index,1);
	    
	    float Theta = atan2(Y, X);
	    float R = sqrt(X*X + Y*Y);
	      
	    float Theta2= Theta + offset + offset2;
	    float X2=R*cos(Theta2);
	    float Y2=R*sin(Theta2);
	      
	    data->SetComponent(index,0,X2);
	    data->SetComponent(index,1,Y2);

	    /*	    if (i==16 && j==16 && k==256)
	      {
		fprintf(stderr,"X=%f Y=%f --> (R=%f,T=%.f + %.f -->%f) --> X2=%f Y2=%f   magn=%.2f\n",
			X,Y,R,Theta,offset,Theta2,X2,Y2,sqrt(X2*X2+Y2*Y2));
			}*/
  
	    ++index;
	  }
    }

  return output;
}

float vtkpxProcessRawBrukerFIDImage::AutoPhaseCorrection(vtkImageData* complexinput)
{
  int dim[3]; complexinput->GetDimensions(dim);

  int midi=dim[0]/2+1;
  int midj=dim[1]/2+1;
  float maxmagn=0.0;
  float sumreal=0.0,sumimag=0.0;
  int bestk=dim[2]/2;

  for (int k=0;k<dim[2];k++)
    {
      
      float v1=complexinput->GetScalarComponentAsDouble(midi,midj,k,0);
      sumreal+=v1;
      float v2=complexinput->GetScalarComponentAsDouble(midi,midj,k,1);
      sumimag+=v2;
      float magn=v1*v1+v2*v2;
      
      if (magn>maxmagn)
	{
	  maxmagn=magn;
	  bestk=k;
	}
    }

  fprintf(stderr,"\t\tOffset = %d sums=%.2f,%.2f\n",bestk,sumreal,sumimag);

  return fabs(float(dim[2]/2-bestk));

}

void vtkpxProcessRawBrukerFIDImage::ComputeSums(vtkImageData* img)
{
  int dim[3]; img->GetDimensions(dim);

  float sumreal=0.0,sumimag=0.0,sumtail=0.0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  sumreal+=img->GetScalarComponentAsDouble(i,j,k,0);
	  sumimag+=img->GetScalarComponentAsDouble(i,j,k,1);

	  if (k<dim[2]/8 || k> (4*dim[2])/8)
	    sumtail+=fabs(img->GetScalarComponentAsDouble(i,j,k,0));
	}

  fprintf(stderr,"Sumreal = %.2f, Sumimag=%.2f tail=%.2f\n",sumreal,sumimag,sumtail);
}

vtkImageData* vtkpxProcessRawBrukerFIDImage::CenterPhaseData(vtkImageData* complexinput)
{
  vtkImageData* output=vtkImageData::New();
  output->DeepCopy(complexinput);

  int dim[3]; output->GetDimensions(dim);

  vtkFloatArray* data=(vtkFloatArray*)output->GetPointData()->GetScalars();

  int dim01=dim[0]*dim[1];

  for (int i=0;i<dim[0];i++)
    for (int j=0;j<dim[1];j++)
      {
	int offset=0;
	float maxR=0.0;
	for (int k=0;k<dim[2];k++)
	  {
	    float X = complexinput->GetScalarComponentAsDouble(i,j,k,0);
	    float Y = complexinput->GetScalarComponentAsDouble(i,j,k,1);
	    float R = sqrt(X*X + Y*Y);
	      
	    if (R>maxR)
	      {
		maxR=R;
		offset=k-dim[2]/2;
	      }
	  }

	if (i>dim[0]/4 && i< 3*dim[0]/4 &&
	    j>dim[1]/4 && j< 3*dim[1]/4)
	  fprintf(stderr,"i=%d j=%d; offset=%d (%f)\n",i,j,offset,maxR);

	for (int k2=0;k2<dim[2];k2++)
	  {
	    int index=k2*dim01+j*dim[0]+i;
	    int otherk=k2+offset;
	    if (otherk>=dim[2])
	      otherk-=dim[2];
	    else if (otherk<0)
	      otherk+=dim[2];
	    
	    data->SetComponent(index,0,complexinput->GetScalarComponentAsDouble(i,j,otherk,0));
	    data->SetComponent(index,1,complexinput->GetScalarComponentAsDouble(i,j,otherk,1));
	  }
      }
  return output;
}
// ----------------------------------------------------------------------------
int vtkpxProcessRawBrukerFIDImage::FitExponentialT2Model(vtkImageData* input,vtkImageData* output,float first,float dt,
							 int mode,float threshold,float int_threshold,float max_t2)
{
  int numechos=input->GetNumberOfScalarComponents();

  output->CopyStructure(input);
  output->SetScalarTypeToFloat();
  if (mode==1)
    output->SetNumberOfScalarComponents(5);
  else
    output->SetNumberOfScalarComponents(1);
  output->AllocateScalars();

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();

  //  int numechos=6;

  vtkpxMatrix* A=vtkpxMatrix::New();
  vtkpxMatrix* At=vtkpxMatrix::New();
  vtkpxMatrix* B=vtkpxMatrix::New();
  vtkpxMatrix* F=vtkpxMatrix::New();

  A->Ones(numechos,2);
  At->Ones(2,numechos);
  for (int i=0;i<numechos;i++)
    {
      A->SetElement(i,1,-1.0*(first+float(i)*dt));
      At->SetElement(1,i,-1.0*(first+float(i)*dt));
    }

  vtkpxMatrix::Multiply(At,A,B);
  B->Invert();

  vtkpxMatrix::Multiply(B,At,F);

  A->Zeros(numechos,1);
  At->Zeros(numechos,1);
  B->Zeros(2,1);

  int nt=inp->GetNumberOfTuples();


  fprintf(stderr,"nt=%d numechos=%d\n",nt,numechos);

  int dim[3]; input->GetDimensions(dim);



  double range[2]; inp->GetRange(range);
  int_threshold=int_threshold*(range[1]);
  int voxel_index=0;

  for (int ic=0;ic<dim[2];ic++)
    for (int ib=0;ib<dim[1];ib++)
      for (int ia=0;ia<dim[0];ia++)
	{
	  int flag=0;

	  float sigma=0.0;
	  for (int c=0;c<numechos;c++)
	    {
	      float v=Frange(inp->GetComponent(voxel_index,c),1.0,1e+10);
	      sigma+=fabs(v);

	      A->SetDirectElement(c,0,log(v));
	      At->SetDirectElement(c,0,v);
	    }
	  sigma=sigma/float(numechos);
	  float sigma2=sigma*sigma;

	  vtkpxMatrix::Multiply(F,A,B);
	  float loga=B->GetDirectElement(0,0);
	  float r2=Frange(B->GetDirectElement(1,0),1.0e-7,1.0e+7);
	  float alpha=exp(loga);
	  float t2=1.0/r2;
	 
	  out->SetComponent(voxel_index,0,Frange(t2,1.0,max_t2));
	  if (mode==1)
	    out->SetComponent(voxel_index,1,alpha);

	  float error=0.0;
	  float chisq=0.0;
	  
	  if (flag)
	    fprintf(stderr,"# alpha=%.f t2=%.2f\n#Echo \t t \t Model \t Data\n",alpha,t2);


	  
	  for (int k=0;k<numechos;k++)
	    {
	      float t=first+float(k)*dt;
	      float model=alpha*exp(-t/t2);
	      float data=inp->GetComponent(voxel_index,k);
	      if (flag)
		fprintf(stderr,"%d \t %.2f \t %.2f \t %.2f\n",k+1,t,model,data);
	      error+=fabs(model-data);
	      chisq+=pow(model-data,float(2.0))/sigma2;
	    }

	  float perror=fabs(100.0*error/(float(numechos)*inp->GetComponent(voxel_index,0)));
	  if (mode==1)
	    {
	      out->SetComponent(voxel_index,2,perror);
	      out->SetComponent(voxel_index,3,chisq);
	    }

	  if (flag)
	    fprintf(stderr,"Single perror (%d,%d,%d) =%f (%5.4f) chisq=%.4f\n",ia,ib,ic,error,perror,chisq);

	  float vp=inp->GetComponent(voxel_index,0);
	  if (perror>threshold || vp<int_threshold)
	    out->SetComponent(voxel_index,0,0.0);
	  
	  float tor2=out->GetComponent(voxel_index,0);
	  if (tor2>0.0)
	    tor2=10000.0/tor2;
	  out->SetComponent(voxel_index,4,tor2);
	  ++voxel_index;
	}

  
			    
  A->Delete();
  At->Delete();
  B->Delete();
  F->Delete();

  return 1;
}
// ----------------------------------------------------------------------------
int vtkpxProcessRawBrukerFIDImage::FillMap(vtkImageData* map,vtkImageData* density,float thr,int bimap)
{
  vtkImageMedian3D* median=vtkImageMedian3D::New();
  median->SetKernelSize(3,3,3);

  double range[2]; density->GetPointData()->GetScalars()->GetRange(range);
  
  thr=thr*range[1];
  int nc=map->GetNumberOfScalarComponents();
  int dim[3]; map->GetDimensions(dim);
  int done=0,iter=1;

  if (nc<6)
    bimap=0;

  fprintf(stderr,"Bimap=%d\n",bimap);

  while (done==0 && iter<20)
    {
      median->SetInput(map);
      median->Modified();
      median->Update();
      vtkImageData* tmp=median->GetOutput();

      vtkDataArray* inp_array=map->GetPointData()->GetScalars();
      vtkDataArray* med_array=tmp->GetPointData()->GetScalars();
      vtkDataArray* den_array=density->GetPointData()->GetScalars();

      int numbad=0,dim10=dim[1]*dim[0];
      for (int k=1;k<dim[2]-1;k++)
	for (int j=1;j<dim[1]-1;j++)
	  {
	    int index=k*dim10+j*dim[0]+1;
	    for (int i=1;i<dim[0]-1;i++)
	      {
		if (bimap==0)
		  {
		    float d=den_array->GetComponent(index,0);
		    if (d>thr)
		      {
			float v=inp_array->GetComponent(index,0);
			float m=med_array->GetComponent(index,0);
			if (v<1.0 && m>=1.0)
			  {
			    for (int comp=0;comp<nc;comp++)
			      inp_array->SetComponent(index,comp,med_array->GetComponent(index,comp));
			    ++numbad;
			  }
		      }
		  }
		else
		  {
		    float d=den_array->GetComponent(index,0);
		    if (d>thr)
		      {
			float v=inp_array->GetComponent(index,5);
			float m=med_array->GetComponent(index,5);
			if (v<1.0 && m>=1.0)
			  {
			    for (int comp=4;comp<=5;comp++)
			      inp_array->SetComponent(index,comp,med_array->GetComponent(index,comp));

			    inp_array->SetComponent(index,0,med_array->GetComponent(index,4));
			    inp_array->SetComponent(index,1,med_array->GetComponent(index,5));
			    inp_array->SetComponent(index,2,med_array->GetComponent(index,4));
			    inp_array->SetComponent(index,3,0.0);
			    ++numbad;
			  }
		      }
		  }
		++index;
	      }
	  }
      fprintf(stderr,"End of Iteration %d numbad=%d\n",iter,numbad);
      ++iter;
      if (numbad==0)
	done=1;
    }
  median->Delete();
  return 1;
}
// ----------------------------------------------------------------------------
vtkImageData* vtkpxProcessRawBrukerFIDImage::GenerateSyntheticData(float d1,float d2,float t1,float t2,
								    float dte,int numechos,float noise)
{
  vtkImageData* img=vtkImageData::New();
  img->SetScalarTypeToFloat();
  img->SetNumberOfScalarComponents(numechos);
  img->SetDimensions(100,100,1);
  img->SetWholeExtent(0,99,0,99,0,0);
  img->SetExtent(0,99,0,99,0,0);
  img->SetSpacing(1.0,1.0,1.0);
  img->AllocateScalars();
  vtkFloatArray* array=(vtkFloatArray*)(img->GetPointData()->GetScalars());

  for (int c=0;c<numechos;c++)
    array->FillComponent(c,0.0);



  int index=0;
  for (int j=0;j<100;j++)
    {
      int a=int(j/10);
      float w=float(a)*0.1;
      float dw=1.0-w;
      for (int i=0;i<100;i++)
	{
	  for (int k=0;k<numechos;k++)
	    {
	      float t=float(k+1)*dte;
	      float v1=d1*exp(-t/t1);
	      float v2=d2*exp(-t/t2);
	      float n=noise*vtkpxMath::GaussianRandom();

	      float v=w*v1+dw*v2+n;
	      if (v<0.0)
		v=0.0;
	      array->SetComponent(index,k,v);

	      if (i==50 && ( j==50 || j==0 ))
		fprintf(stderr,"i=%d j=%d t=%.2f v1=%.2f v2=%.2f n=%.2f w=%.2f dw=%.2f v=%.2f\n",
			i,j,t,v1,v2,n,w,dw,v);
	      
	    }
	  ++index;
	}
    }

  return img;
}



