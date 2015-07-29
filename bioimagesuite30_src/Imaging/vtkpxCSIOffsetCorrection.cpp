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
  Module:    $RCSfile: vtkpxCSIOffsetCorrection.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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
#include "vtkpxCSIOffsetCorrection.h"
#include "vtkpxUtil.h"
#include "vtkFloatArray.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkMath.h"
//------------------------------------------------------------------------------

vtkpxCSIOffsetCorrection* vtkpxCSIOffsetCorrection::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxCSIOffsetCorrection");
  if(ret)
    {
      return (vtkpxCSIOffsetCorrection*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxCSIOffsetCorrection;
}



vtkpxCSIOffsetCorrection::vtkpxCSIOffsetCorrection()
{
  this->Threshold=0.05;
  this->WaterPeak=-1;
  this->FatPeak=-1;
  this->Input2=NULL;
  this->FatPercentageOutput=NULL;
  this->GlobalFatPercentageOutput=NULL;
  this->OffsetMap=NULL;
  this->GlobalOutput=NULL;
  this->Spectrum=NULL;
  this->Scale=0.000001;
  this->Lambda=1.0;
  this->Iterations=10;
  this->SpectralSigma=1.0;
}
// ----------------------------------------------------------------------------
vtkpxCSIOffsetCorrection::~vtkpxCSIOffsetCorrection()
{
  this->SetInput2(NULL);

  if (this->FatPercentageOutput!=NULL)
    this->FatPercentageOutput->Delete();
  this->FatPercentageOutput=NULL;

  if (this->GlobalFatPercentageOutput!=NULL)
    this->GlobalFatPercentageOutput->Delete();
  this->GlobalFatPercentageOutput=NULL;
  
  if (this->Spectrum!=NULL)
    this->Spectrum->Delete();
  this->Spectrum=NULL;
 
  if (this->OffsetMap!=NULL)
    this->OffsetMap->Delete();
  this->OffsetMap=NULL;

  if (this->GlobalOutput!=NULL)
    this->GlobalOutput->Delete();
  this->GlobalOutput=NULL;

}

// ---------------------------------------------------------------------------------------------
vtkImageData* vtkpxCSIOffsetCorrection::GenerateFatPercentageOutput(vtkImageData* input)
{
  if (input==NULL)
    return NULL;
  float thr=Frange(this->Threshold,0.0,0.5);

  if (input->GetNumberOfScalarComponents()<2)
    return NULL;

  vtkImageData* fat_out=vtkImageData::New();
  fat_out->CopyStructure(input);
  fat_out->SetNumberOfScalarComponents(1);
  fat_out->SetScalarTypeToShort();
  fat_out->AllocateScalars();

  vtkDataArray* data_in=input->GetPointData()->GetScalars();
  vtkDataArray* data_out=fat_out->GetPointData()->GetScalars();

  int nt=data_in->GetNumberOfTuples();
  double range1[2]; data_in->GetRange(range1,0);
  double range2[2]; data_in->GetRange(range2,1);
  float threshold1=thr*(range1[1]-range1[0])+range1[0];
  float threshold2=thr*(range2[1]-range2[0])+range2[0];

  fprintf(stderr,"Threshold1=%.3f (%.2f:%.2f)\n",threshold1,range1[0],range1[1]);
  fprintf(stderr,"Threshold2=%.3f (%.2f:%.2f)\n",threshold2,range2[0],range2[1]);

  for (int i=0;i<nt;i++)
    {
      float water=data_in->GetComponent(i,0);
      float fat  =data_in->GetComponent(i,1);

      if (water < threshold1 && fat < threshold2)
	data_out->SetComponent(i,0,0.0);
      else
	{
	  float p=100.0*fat/(fat+water);
	  if (p>=1.0)
	    data_out->SetComponent(i,0,p);
	  else
	    data_out->SetComponent(i,0,0.0);
	}
    }

  return fat_out;
}
// ---------------------------------------------------------------------------------------------
vtkImageData* vtkpxCSIOffsetCorrection::GenerateSmoothInputSpectrum(vtkImageData* realimg,vtkImageData* imagimg,float sigma,float scale)
{
  float s[5];
  static int   dt[5] = { -2,-1,0,1,2};

  if (sigma<0.1)
    sigma=0.1;

  float sum=0.0;
  float sigma2=sigma*sigma;
  for (int i=0;i<5;i++)
    {
      float ds=float(dt[i]);
      s[i]=exp(-0.5*(ds*ds)/sigma2);
      sum+=s[i];
    }
  for (int i=0;i<5;i++)
    {
      s[i]/=sum;
      fprintf(stderr,"i=%d dt=%d s[%d]=%.2f\n",i,dt[i],i,s[i]);
    }


  vtkImageData* magn=vtkImageData::New();
  magn->CopyStructure(realimg);
  magn->AllocateScalars();

  int dim[3]; realimg->GetDimensions(dim);
  int nc=realimg->GetNumberOfScalarComponents();
  for (int k=0;k<dim[2];k++)
    {
      fprintf(stderr,"Smoothing slice %d /%d\n",k,dim[2]);
      for (int i=0;i<dim[0];i++)
	for (int j=0;j<dim[1];j++)
	  for (int c=0;c<nc;c++)
	    {
	      float sum=0.0;
	      for (int t=0;t<5;t++)
		{
		  int tc=Irange(c+dt[t],0,nc-1);
		  float re=realimg->GetScalarComponentAsDouble(i,j,k,tc);
		  float im=imagimg->GetScalarComponentAsDouble(i,j,k,tc);
		  sum+=s[t]*sqrt(re*re+im*im);
		}
	      magn->SetScalarComponentFromDouble(i,j,k,c,sum*scale);
	    }
    }
  return magn;
}

// ---------------------------------------------------------------------------------------------
vtkImageData* vtkpxCSIOffsetCorrection::ComputeInitialOffsetmap(vtkImageData* magnitude)
{
  if (magnitude==NULL)
    return NULL;

  vtkImageData* peaks=vtkImageData::New();

  peaks->CopyStructure(magnitude);
  peaks->SetScalarTypeToFloat();
  peaks->SetNumberOfScalarComponents(1);
  peaks->AllocateScalars();

  int dim[3]; magnitude->GetDimensions(dim);
  int nc=magnitude->GetNumberOfScalarComponents();

  
  float fat_peak=this->FatPeak;
  float water_peak=this->WaterPeak;

  int gap=int(0.4*fabs(fat_peak-water_peak)+0.5);

  fprintf(stderr,"\n\n Input looking for water peak in %.1f:%.1f (%.1f) and fat_peak in %.1f:%.1f (%.1f) gap=%d\n",
	  water_peak-gap,water_peak+gap,water_peak,
	  fat_peak-gap,fat_peak+gap,fat_peak,gap);


  for (int k=0;k<dim[2];k++)
    {
      fprintf(stderr,"\tSlice %d/%d \n",k+1,dim[2]);
      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {
	    //
	    //  Identify Peaks via shift 
	    //
	    float max_sum=0.0;
	    int shift=0;

	    for (int s=-gap;s<=gap;s++)
	      {
		int c[2];
		c[0]=Irange(int(water_peak)+s,0,nc-1);
		c[1]=Irange(int(fat_peak)+s,0,nc-1);
		
		float v1=magnitude->GetScalarComponentAsDouble(i,j,k,c[0]);
		float v2=magnitude->GetScalarComponentAsDouble(i,j,k,c[1]);
		float v=v1+v2;
		
		if (v>max_sum)
		  {
		    max_sum=v;
		    shift=s;
		  }
	      }
	    peaks->SetScalarComponentFromDouble(i,j,k,0,shift);
	  }
    }

  return peaks;

}
// ---------------------------------------------------------------------------------------------
void vtkpxCSIOffsetCorrection::IntegrateMagnitude(vtkImageData* realimg,vtkImageData* imagimg,vtkImageData* peaks,
						  vtkImageData* output,vtkFloatArray* volint)

						  
{
  float water_peak=this->WaterPeak;
  float fat_peak=this->FatPeak;

  int dim[3]; realimg->GetDimensions(dim);
  int nc=realimg->GetNumberOfScalarComponents();
  float scale=1.0/(dim[0]*dim[1]*dim[2]);

  volint->SetNumberOfComponents(3);
  volint->SetNumberOfTuples(nc);
  volint->FillComponent(0,0.0);
  volint->FillComponent(1,0.0);
  volint->FillComponent(2,0.0);

  if (this->GlobalOutput!=NULL)
    this->GlobalOutput->Delete();
  this->GlobalOutput=vtkImageData::New();
  this->GlobalOutput->CopyStructure(output);
  this->GlobalOutput->AllocateScalars();

  vtkFloatArray* data=(vtkFloatArray*)output->GetPointData()->GetScalars();
  vtkFloatArray* gdata=(vtkFloatArray*)this->GlobalOutput->GetPointData()->GetScalars();
  
  float gap=0.5*fabs(fat_peak-water_peak);
  float lw=water_peak-gap;
  float uw=water_peak+gap;
  float lf=fat_peak-gap;
  float uf=fat_peak+gap;

  int c2high=nc-int(gap);
  int c2low=int(gap);
  float nt=float(dim[2]*dim[1]*dim[0]);
  int  globshift=int(water_peak-nc/2);


  fprintf(stderr,"Input looking for water peak in %.1f:%.1f (%.1f) and fat_peak in %.1f:%.1f (%.1f) globshift=%d scale=%.4f\n",
	  lw,uw,water_peak,lf,uf,fat_peak,globshift,scale);


  int voxel=0;

  fprintf(stderr,"slice :");

  for (int k=0;k<dim[2];k++)
    {
      fprintf(stderr,"%d/%d ",k+1,dim[2]);
      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {
	    float shift=peaks->GetScalarComponentAsDouble(i,j,k,0);
	    float wat_p=water_peak+shift;
	    float fat_p=fat_peak+shift;

	    int  dshift=int(wat_p-nc/2);
	    
	    float gp=fabs(fat_p-wat_p)*0.5;
	    float l1=wat_p-gp;
	    float m1=wat_p+gp;
	    float u1=fat_p+gp;

	    float lg1=water_peak-gp;
	    float mg1=water_peak+gp;
	    float mu1=fat_peak  +gp;

	    
	    if (i==dim[0]/2 && j==dim[0]/2)
	      {
		fprintf(stderr,"k=%d,water = %.1f:%.1f fat=%.1f:%.1f   offset=%.0f\n",k,
			l1,m1,
			m1,u1,shift);
	      }
	    
	    float wat=0.0,fat=0.0,gwat=0.0,gfat=0.0;
	    for (int c=0;c<nc;c++)
	      {
		float v1=realimg->GetScalarComponentAsDouble(i,j,k,c);
		float v2=imagimg->GetScalarComponentAsDouble(i,j,k,c);
		float v=sqrt(v1*v1+v2*v2);

		int c2=c-dshift;
		if (c2>c2low && c2<c2high)
		  volint->SetComponent(c2,0,volint->GetComponent(c2,0)+v);

		int c3=c-globshift;
		if (c3>c2low && c3<c2high)
		  volint->SetComponent(c3,1,volint->GetComponent(c3,1)+v);

		volint->SetComponent(c,2,volint->GetComponent(c,2)+v);


		if (c>=l1 && c<=m1)
		  wat+=v;
		else if (c>=m1 && c<=u1)
		  fat+=v;
		

		if (c>=lg1 && c<=mg1)
		  gwat+=v;
		else if (c>=mg1 &&  c <= mu1)
		  gfat+=v;

		

	      }
	    
	    data->SetComponent(voxel,0,wat*scale);
	    data->SetComponent(voxel,1,fat*scale);
	    
	    gdata->SetComponent(voxel,0,gwat*scale);
	    gdata->SetComponent(voxel,1,gfat*scale);
	    

	    if (i==dim[0]/2 && j==dim[0]/2)
	      {
		fprintf(stderr,"k=%d, local (%.1f %.1f) global (%.1f,%.1f) \n",k,
			data->GetComponent(voxel,0),
			data->GetComponent(voxel,1),
			gdata->GetComponent(voxel,0),
			gdata->GetComponent(voxel,1));

	      }
	    ++voxel;
	  }
    }


  for (int c=0;c<nc;c++)
    for (int d=0;d<=2;d++)
      volint->SetComponent(c,d,volint->GetComponent(c,d)/nt);



}

// ----------------------------------------------------------------------------
int  vtkpxCSIOffsetCorrection::UpdateVoxel(int eveni,int evenj,int evenk,int pass)
{
  int update_voxel=0;
  if (eveni == evenj) update_voxel=1;
  if (evenk)   	  update_voxel=1-update_voxel;
  if (pass==1) 	  update_voxel=1-update_voxel;
  
  return update_voxel;
}

// ----------------------------------------------------------------------------
float vtkpxCSIOffsetCorrection::OptimalOffsetForVoxel(int i,int j,int k,int dim[3],float weights[6],float lambda,float current,int nc)
{
  static int shift[6][3] = { { -1 ,0,0}, { 1,0,0},{ 0,-1,0 },{0,1,0} , { 0,0,-1},{0,0,1}};

  float best=0.0;
  float bestshift=current;
  int   flag=0;
  /*  if (i==dim[0]/ 2 && j==dim[1]/2 && k==dim[2]/2)
      flag=1;*/
  for (int s=-3;s<=3;s++)
    {
      int c[2];
      c[0]=Irange(int(this->WaterPeak+current+s),0,nc-1);
      c[1]=Irange(int(this->FatPeak  +current+s),0,nc-1);
		
      float s1=this->SmoothMagnitude->GetScalarComponentAsDouble(i,j,k,c[0]);
      float s2=this->SmoothMagnitude->GetScalarComponentAsDouble(i,j,k,c[1]);
      float this_voxel=s1+s2;

      if (flag)
	fprintf(stderr,"\n shift=%.1f (%d) s1=%.1f s2=%.1f, \t ",current+s,s,s1,s2);
	
      float sum=0.0;
      float other_voxels=0.0;
      for (int t=0;t<6;t++)
	{
	  int newi=Irange(i+shift[t][0],0,dim[0]-1);
	  int newj=Irange(j+shift[t][1],0,dim[1]-1);
	  int newk=Irange(k+shift[t][2],0,dim[2]-1);
	  float sp=this->OffsetMap->GetScalarComponentAsDouble(newi,newj,newk,0);
	  
	  c[0]=Irange(int(this->WaterPeak+sp),0,nc-1);
	  c[1]=Irange(int(this->FatPeak  +sp),0,nc-1);
	  float s1=this->SmoothMagnitude->GetScalarComponentAsDouble(newi,newj,newk,c[0]);
	  float s2=this->SmoothMagnitude->GetScalarComponentAsDouble(newi,newj,newk,c[1]);
	  other_voxels+=weights[t]*(s1+s2);
	  
	  float dv=-weights[t]*pow(sp-(current+s),float(2.0));
	  sum+=dv;
	  if (flag)
	    fprintf(stderr,"(t=%d s1=%.1f s2=%.1f sp=%.1f) ",t,s1,s2,sp,dv);
	}

      
      float a=this_voxel/(this_voxel+other_voxels);
      float b=1.0-a;
      sum=a*this_voxel+b*lambda*sum;

      if (flag)
	fprintf(stderr,"\n\n\t a=%.1f b=%.1f sum=%.1f\n",a,b,sum);

      if (s==-1.0 || sum>best)
	{
	  best=sum;
	  bestshift=float(s+current);
	}
    }
  
  if (flag)
    fprintf(stderr,"**** bestshift=%.1f best=%.1f\n\n",bestshift,best);

  return bestshift;
}

// ----------------------------------------------------------------------------

int vtkpxCSIOffsetCorrection::OptimizeOffsetMap()
{
  int iter=1;
  int maxiter=this->Iterations;
	  
  int dim[3];   this->OffsetMap->GetDimensions(dim);
  double sp[3]; this->OffsetMap->GetSpacing(sp);
  
  int nc=this->SmoothMagnitude->GetNumberOfScalarComponents();
  float wgt[6];
  wgt[5]=1.0/sp[2];   wgt[4]=1.0/sp[2];
  wgt[3]=1.0/sp[1];   wgt[2]=1.0/sp[1];
  wgt[1]=1.0/sp[0];   wgt[0]=1.0/sp[0];
  
   float sum=0.0;
  for (int l=0;l<=5;l++)
    sum+=wgt[l];

  for (int k=0;k<=5;k++)
    wgt[k]/=sum;

  int done=0;

  int nt=dim[0]*dim[1]*dim[2];
  int tenth=nt/11;

  while (done==0 && iter<maxiter)
    {
      float changed=0,total=0.0;
      
      int order=(vtkMath::Random()>0.5);
  
      fprintf(stderr,"\t **** MRF Regularization (%.2f) iter=%d :",this->Lambda,iter+1);
      int count=0;

      for (int pass=0;pass<=1;pass++)
	{
	  int realpass=pass;
	  if (order==1)
	    realpass=1-pass;

	  for (int k=1;k<dim[2]-1;k++)
	    {
	      int evenk= ( ( k % 2 )  == 0);
	      for (int j=1;j<dim[1]-1;j++)
		{
		  int evenj= ( ( j % 2 ) == 0 );
		  for (int i=1;i<dim[0]-1;i++)
		    {
		      int eveni= ( ( i % 2 ) == 0 );
		      ++count;
		      if (count==tenth)
			{
			  fprintf(stderr,".");
			  count=0;
			}

		      // ----------------------------
		      // Updating Voxel
		      // ----------------------------
		      
		      if ( this->UpdateVoxel(eveni,evenj,evenk,pass)==1)
			{
			  float current=this->OffsetMap->GetScalarComponentAsDouble(i,j,k,0);
			  float best   =this->OptimalOffsetForVoxel(i,j,k,dim,wgt,this->Lambda,current,nc);
			  if (fabs(current-best)>0.5)
			    changed+=1.0;
			  this->OffsetMap->SetScalarComponentFromDouble(i,j,k,0,best);
			  total+=1.0;
			}
		    }
		}
	    }
	}
      
      changed=100.0*changed/total;
      fprintf(stderr,"changed=%.2f\n\n",changed);
      if (changed<1)
	done=1;
      ++iter;
    }
 return 1;
}
// ----------------------------------------------------------------------------
void vtkpxCSIOffsetCorrection::ExecuteInformation()
{
  if (this->GetImageDataInput(0)==NULL)
    return;

  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();
  output->SetNumberOfScalarComponents(2);
  output->SetScalarType(VTK_FLOAT);

}
// ----------------------------------------------------------------------------
void vtkpxCSIOffsetCorrection::SimpleExecute(vtkImageData* input,vtkImageData* output)
{

 
  if (input==NULL    || this->Input2==NULL )
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxCSIOffsetCorrection");
      return;
    }


  int dim[3],dim2[3];
  input->GetDimensions(dim);
  this->Input2->GetDimensions(dim2);
  
  int sum=0;
  for (int ia=0;ia<=2;ia++)
    sum+=abs(dim[ia]-dim2[ia]);
  if (sum!=0)
    {
      vtkErrorMacro(<<"Bad Input Dimensions");
      return;
    }

  if (this->WaterPeak==-1 || this->FatPeak==-1)
    {
      int nc=input->GetNumberOfScalarComponents();
      this->WaterPeak=nc/2;
      this->FatPeak=int(0.65*nc);
    }


  // First Smooth Input Spectrum
  this->SmoothMagnitude=this->GenerateSmoothInputSpectrum(input,this->Input2,SpectralSigma,this->Scale);
  this->UpdateProgress(0.1);
  fprintf(stderr,"Done with Smooth Magnitude -- works \n");

  // Then Compute Initial Offsets
  this->OffsetMap=this->ComputeInitialOffsetmap(this->SmoothMagnitude);
  this->UpdateProgress(0.2);
  fprintf(stderr,"Done with Initial Offset Map\n");

  
  // Then Horror of Horrors Optimize Offset Map
  if (this->Iterations>0)
    this->OptimizeOffsetMap();
  this->UpdateProgress(0.9);
  fprintf(stderr,"Done with Final Offset Map\n");
  
  // Then Compute Integrated Output map
  this->Spectrum=vtkFloatArray::New();
  this->IntegrateMagnitude(input,this->Input2,this->OffsetMap,output,this->Spectrum);


  fprintf(stderr,"Done with Int Magn Map\n");
  this->FatPercentageOutput=this->GenerateFatPercentageOutput(output);
  this->GlobalFatPercentageOutput=this->GenerateFatPercentageOutput(this->GlobalOutput);
  fprintf(stderr,"Done with Fat Percentage\n");

  this->UpdateProgress(1.0);
}

