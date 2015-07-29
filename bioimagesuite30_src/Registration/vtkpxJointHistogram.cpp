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

// ------------------------------------------------------------------------
//
// This file derives from original work
// in the vtkCISG Toolkit
// that was released under the terms of 
// the GNU General Public License (either version 2,
// of the License, or any later version.)
// 
// The code has been modified for inclusion in BioImage Suite
//
// It may consist of work derived from more than one source
// file in the vtkCISG Toolkit. The original copyright notice
// was:
//
//  Authors   : Thomas Hartkens <thomas@hartkens.de>
//              Daniel Rueckert <d.rueckert@doc.ic.ac.uk>
//	        Julia Schnabel  <julia.schnabel@kcl.ac.uk>
//	        under the supervision of Dave Hawkes and Derek Hill
//  Web       : http://www.image-registration.com
//  Copyright : King's College London
//              Div. of Radiological Sciences, 
//              Computational Imaging Science Group, 1997 - 2000
//	        http://www-ipg.umds.ac.uk/cisg
//
// ------------------------------------------------------------------------


/*=========================================================================*/
#include "vtkpxJointHistogram.h"
#include "vtkObjectFactory.h"
#include "vtkShortArray.h"
#include "vtkImageData.h"
#include "vtkpxRegistration.h"
#include "vtkImageCast.h"
#include "pxutil.h"
// ----------------------------------------------------------------------------------

int** NewIntMatrix(int x[2]) 
{
  int** m = new int*[x[0]];
  for(int i = 0; i < x[0]; i++) 
    m[i] = new int[x[1]];
  return m;
}

void DeleteIntMatrix(int** m, int x[2] ) 
{
  for(int i = 0; i < x[0]; i++) 
    delete [] m[i]; // OK, we don't actually need y
  delete [] m;
}
// ----------------------------------------------------------------------------------


vtkpxJointHistogram* vtkpxJointHistogram::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxJointHistogram");
  if(ret)
    {
      return (vtkpxJointHistogram*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxJointHistogram;
}
// ----------------------------------------------------------------------------------
vtkpxJointHistogram::vtkpxJointHistogram()
{

  this->NumberOfBins[0]=64;
  this->NumberOfBins[1]=64;
  this->NumberOfSamples   = 0;

  this->Bins=NULL;
  this->OldBins=NULL;
}
// ----------------------------------------------------------------------------------
vtkpxJointHistogram::~vtkpxJointHistogram()
{
  if (this->Bins!=NULL)
    {
      DeleteIntMatrix(this->Bins,this->NumberOfBins);
      DeleteIntMatrix(this->OldBins,this->NumberOfBins);
    }
}
// ----------------------------------------------------------------------------------
void vtkpxJointHistogram::Reset()
{
  if (this->Bins==NULL)
    return;

  for (int ia=0;ia<this->NumberOfBins[0];ia++)
    for (int ib=0;ib<this->NumberOfBins[1];ib++)
      this->Bins[ia][ib]=0;
  this->NumberOfSamples=0;

}
// ----------------------------------------------------------------------------------
void vtkpxJointHistogram::Allocate(int numx,int numy)
{
  int flag=0;

  int oldx[2];
  oldx[0]=this->NumberOfBins[0];
  oldx[1]=this->NumberOfBins[1];

  if (this->Bins!=NULL)
    {
      if (numx!=NumberOfBins[0])
	{
	  this->NumberOfBins[0]=Irange(numx,1,1024);
	  flag=1;
	}
      
      if (numy!=NumberOfBins[1])
	{
	  this->NumberOfBins[1]=Irange(numy,1,1024);
	  flag=1;
	}

      if (flag==1)
	{
	  DeleteIntMatrix(this->Bins,oldx);
	  DeleteIntMatrix(this->OldBins,oldx);
	}

    }
  else
    {
      flag=1;
      this->NumberOfBins[0]=Irange(numx,1,1024);
      this->NumberOfBins[1]=Irange(numy,1,1024);
    }
 
  if (flag==1)
    {
      this->Bins=NewIntMatrix(this->NumberOfBins);
      this->OldBins=NewIntMatrix(this->NumberOfBins);
    }
  
  this->Reset();
  this->BackupHistogram();

  
}
// ----------------------------------------------------------------------------------
int vtkpxJointHistogram::CheckHistogram()
{
  if (this->NumberOfSamples == 0 || this->Bins==NULL)
    {
      vtkErrorMacro(<<"No Samples In Histogram");
      return 0;
    }
  
  return 1;
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::MeanX()
{
  if (this->CheckHistogram()==0)
    return 0.0;

  
  float val=0.0;
  for (int i = 0; i < NumberOfBins[0]; i++)
    {
      float tmp=float(i);
      for (int j = 0; j < NumberOfBins[1]; j++)
	val += this->Bins[i][j] * tmp;
    }
  
  return val / (float)this->NumberOfSamples;
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::MeanY()
{
  if (this->CheckHistogram()==0)
    return 0.0;


  float val = 0.0;
  for (int j = 0; j < NumberOfBins[1]; j++)
    {
      float tmp = float(j);
      for (int i = 0; i < NumberOfBins[0]; i++)
	val += this->Bins[i][j] * tmp;
    }

  return val / (float)this->NumberOfSamples;
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::VarianceX()
{
  if (this->CheckHistogram()==0)
    return 0.0;

  float val  = 0.0;
  for (int i = 0; i < NumberOfBins[0]; i++)
    val += this->MarginalProbabilityX(i) * pow(float(i),float(2.0));

  return val - pow(this->MeanX(), 2);
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::VarianceY()
{
  if (this->CheckHistogram()==0)
    return 0.0;

  float val  = 0;
  for (int i = 0; i < NumberOfBins[1]; i++)
    val += this->MarginalProbabilityY(i) * pow(float(i),float(2.0));

  return val - pow(this->MeanY(), 2);
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::StandardDeviationX()
{
  if (this->CheckHistogram()==0)
    return 0.0;
  return sqrt(this->VarianceX());
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::StandardDeviationY()
{
  if (this->CheckHistogram()==0)
    return 0.0;
  
  return sqrt(this->VarianceY());
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::Covariance()
{
  if (this->CheckHistogram()==0)
    return 0.0;
  
  float val  = 0.0;
  float mean_x = this->MeanX(); 
  float mean_y = this->MeanY(); 
  for (int j = 0; j < NumberOfBins[1]; j++)
    for (int i = 0; i < NumberOfBins[0]; i++)
      val += this->Bins[i][j] * float (i-mean_x)*float(j-mean_y);

  return val / (float)NumberOfSamples;
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::EntropyX()
{
  if (this->CheckHistogram()==0)
    return 0.0;

  // Calculate entropy
  float val = 0;
  for (int i = 0; i < NumberOfBins[0]; i++)
    {
      float tmp=0.0;
      for (int j = 0; j < NumberOfBins[1]; j++)
	  tmp +=this->Bins[i][j];

      if (tmp > 0) 
	val += tmp * log(tmp);
    }



  float v= - val / (float)NumberOfSamples + log(float(NumberOfSamples));
  //  fprintf(stdout,"Entropy X=%f\n",v);
  return v;
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::EntropyY()
{
  if (this->CheckHistogram()==0)
    return 0.0;

  // Calculate entropy
  float val = 0;
  
  for (int j = 0; j < NumberOfBins[1]; j++)
    {
      float tmp = 0;
      for (int i = 0; i < NumberOfBins[0]; i++)
	tmp += this->Bins[i][j];

      if (tmp > 0) 
	val += tmp * log(tmp);
    }
  float v= - val / (float)NumberOfSamples + log(float(NumberOfSamples));
  //  fprintf(stdout,"Entropy Y=%f\n",v);
  return v;
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::JointEntropy()
{
  if (this->CheckHistogram()==0)
    return 0.0;

  // Calculate joint entropy
  float val = 0;
  
  for (int j = 0; j < NumberOfBins[1]; j++)
    for (int i = 0; i < NumberOfBins[0]; i++)
      {
	float v=this->Bins[i][j];
	if (v > 0.0)
	  val += v * log(v);

      }

  float v= - val / (float)NumberOfSamples + log(float(NumberOfSamples));
  //  fprintf(stdout,"Joint Entropy = %f\n",v);
  return v;
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::ConditionalMeanXY(int i)
{
  if (this->CheckHistogram()==0)
    return 0.0;

  if ((i < 0) || (i > NumberOfBins[1]-1))
    {
      cerr << "vtkpxJointHistogram::ConditionalMeanXY: No such bin " << i << endl;
      return 0;
    }
  
  float m = 0;
  float p = 0;
  for (int j = 0; j < NumberOfBins[0]; j++)
    {
      m += this->JointProbability(j, i) * float(j);
      p += this->JointProbability(j, i);
    }

  if (p > 0.0)
    return m / p;

  return 0;
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::ConditionalMeanYX(int i)
{
  if (this->CheckHistogram()==0)
    return 0.0;

  if ((i < 0) || (i > NumberOfBins[0]-1)){
    cerr << "vtkpxJointHistogram::ConditionalMeanYX: No such bin " << i << endl;
    return 0;
  }
  float m = 0;
  float p = 0;
  for (int j = 0; j < NumberOfBins[1]; j++)
    {
      m += this->JointProbability(i, j) * float(j);
      p += this->JointProbability(i, j);
    }

  if (p > 0)
    return m / p;

  return 0;
  
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::CorrelationRatioXY()
{
  if (this->CheckHistogram()==0)
    return 0.0;

  float c = 0;
  float m = this->MeanX();
  for (int i = 0; i < NumberOfBins[1]; i++)
    c += this->MarginalProbabilityY(i) * pow(this->ConditionalMeanXY(i)-m, float(2.0));
  
  return (c / this->VarianceX());
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::CorrelationRatioYX()
{
  if (this->CheckHistogram()==0)
    return 0.0;

  float c = 0;
  float m = this->MeanY();
  for (int i = 0; i < NumberOfBins[0]; i++)
    c += this->MarginalProbabilityX(i) * pow(this->ConditionalMeanYX(i)-m, float(2.0));
  return (c / this->VarianceY());
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::MutualInformation()
{
  if (this->CheckHistogram()==0)
    return 0.0;


  return this->EntropyX() + this->EntropyY() - this->JointEntropy();
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::NormalizedMutualInformation()
{
  if (this->CheckHistogram()==0)
    return 0.0;

  return (this->EntropyX() + this->EntropyY()) / this->JointEntropy()-1.0;
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::CrossCorrelation()
{
  if (this->CheckHistogram()==0)
    return 0.0;

  return this->Covariance() / (sqrt(this->VarianceX()) * sqrt(this->VarianceY()));
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::SumsOfSquaredDifferences()
{
  if (this->CheckHistogram()==0)
    return 0.0;

  float ssd   = 0;
  for (int j = 0; j < NumberOfBins[1]; j++)
    for (int i = 0; i < NumberOfBins[0]; i++)
      ssd   += this->Bins[i][j]*pow(float(i-j),float(2.0));
  
  return ssd/float(this->GetNumberOfSamples());

}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::ComputeProduct()
{
  if (this->CheckHistogram()==0)
    return 0.0;


  if (this->NumberOfSamples<1)
    return 0.0;

  float prod   = 0;
  for (int j = 0; j < NumberOfBins[1]; j++)
    for (int i = 0; i < NumberOfBins[0]; i++)
      prod   += this->Bins[i][j]*i*j;
  
  if (this->NumberOfSamples<1)
    return 0;

  return prod/float(this->GetNumberOfSamples());
}
// ----------------------------------------------------------------------------------
int vtkpxJointHistogram::GetNumberOfBinsX()
{
  return NumberOfBins[0];
}
// ----------------------------------------------------------------------------------
 int vtkpxJointHistogram::GetNumberOfBinsY()
{
  return NumberOfBins[1];
}
// ----------------------------------------------------------------------------------
int vtkpxJointHistogram::GetBinCount(int i,int j)
{
  i=Irange(i,0,this->NumberOfBins[0]-1);
  j=Irange(j,0,this->NumberOfBins[1]-1);
  return this->Bins[i][j];
}

void vtkpxJointHistogram::SetBinCount(int i,int j,int v)
{
  int ia=Irange(i,0,this->NumberOfBins[0]-1);
  int ib=Irange(j,0,this->NumberOfBins[1]-1);
  if (i!=ia || ib!=j)
    fprintf(stdout,"Mapping %d %d --> %d %d\n",i,j,ia,ib);
  this->NumberOfSamples=this->NumberOfSamples+v-this->Bins[ia][ib];
  this->Bins[ia][ib]=v;
}

 float vtkpxJointHistogram::JointProbability(int i, int j)
{
  i=Irange(i,0,this->NumberOfBins[0]-1);
  j=Irange(j,0,this->NumberOfBins[1]-1);
  return this->Bins[i][j] / (float) NumberOfSamples;
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::MarginalProbabilityX(int i)
{
  i=Irange(i,0,this->NumberOfBins[0]-1);
  float n = 0;
  for (int j = 0; j < NumberOfBins[1]; j++)
    n += this->Bins[i][j];
  return n / (float) NumberOfSamples;
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::MarginalProbabilityY(int j)
{
  j=Irange(j,0,this->NumberOfBins[1]-1);
  float n = 0;
  for (int i = 0; i < NumberOfBins[0]; i++)
    n += this->Bins[i][j];

  return n / (float) NumberOfSamples;
}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::ConditionalProbabilityXY(int i, int j)
{
  float p = this->MarginalProbabilityY(j);
  if (p > 0)
    return this->JointProbability(i, j) / p;
    
  return 0;

}
// ----------------------------------------------------------------------------------
float vtkpxJointHistogram::ConditionalProbabilityYX(int i, int j)
{
  float p = this->MarginalProbabilityX(j);
  if (p > 0)
    return this->JointProbability(j, i) / p;

  return 0;
}
// ----------------------------------------------------------------------------------
int vtkpxJointHistogram::AddToBin(short a,short b,short count)
{
  if (this->Bins==NULL)
      return 0;


  if (a>=0 && a<this->NumberOfBins[0] && b>=0 && b<this->NumberOfBins[1])
    {
      this->Bins[a][b] +=count;
      this->NumberOfSamples+=count;
      return 1;

    }
  
  return 0;

}


// ----------------------------------------------------------------------------------
int vtkpxJointHistogram::DelToBin(short a,short b,short count)
{
  if (this->Bins==NULL)
    return 0;

  if (a>=0 && a<this->NumberOfBins[0] && b>=0 && b<this->NumberOfBins[1])
    {
      this->Bins[a][b] -=count;
      this->NumberOfSamples-=count;
      return 1;
    }
  return 0;

}
// --------------------------------------------------------------------------------
int vtkpxJointHistogram::AddToBin(unsigned char a,unsigned char b,unsigned char count)
{
  if (this->Bins==NULL)
    return 0;

  if (a<this->NumberOfBins[0] &&  b<this->NumberOfBins[1])
    {
      this->Bins[a][b] +=count;
      this->NumberOfSamples+=count;
      return 1;
    }
  
  return 0;

}


// ----------------------------------------------------------------------------------
int vtkpxJointHistogram::DelToBin(unsigned char a,unsigned char b,unsigned char count)
{
  if (this->Bins==NULL)
    return 0;

  if (a<this->NumberOfBins[0] && b<this->NumberOfBins[1])
    {
      this->Bins[a][b] -=count;
      this->NumberOfSamples-=count;
      return 1;
    }
  return 0;

}
// ----------------------------------------------------------------------------------
void vtkpxJointHistogram::ResetHistogram()
{
  if (this->Bins!=NULL && this->OldBins!=NULL)
    {
      for (int i=0;i<this->NumberOfBins[0];i++)
	for (int j=0;j<this->NumberOfBins[1];j++)
	  this->Bins[i][j]=this->OldBins[i][j];
      this->NumberOfSamples=this->OldNumberOfSamples;
    }
}

void vtkpxJointHistogram::BackupHistogram()
{
  if (this->Bins!=NULL && this->OldBins!=NULL)
    {
      for (int i=0;i<this->NumberOfBins[0];i++)
	for (int j=0;j<this->NumberOfBins[1];j++)
	  this->OldBins[i][j]=this->Bins[i][j];
      this->OldNumberOfSamples=this->NumberOfSamples;
    }
}

//-------------------------------------------------------------------------
int vtkpxJointHistogram::AreBothImagesShort(vtkImageData* img1,vtkImageData* img2)
{
  if (img1->GetScalarType()==VTK_SHORT && img2->GetScalarType()==VTK_SHORT )
    return 1;
  return 0;
}
//-------------------------------------------------------------------------
int vtkpxJointHistogram::AreBothImagesUnsignedChar(vtkImageData* img1,vtkImageData* img2)
{
  if (img1->GetScalarType()==VTK_UNSIGNED_CHAR && img2->GetScalarType()==VTK_UNSIGNED_CHAR )
    return 1;
  return 0;
}
//-------------------------------------------------------------------------
int vtkpxJointHistogram::FillHistogram(vtkImageData* ref,vtkImageData* targ,int reset)
{
  
  int np1= ref->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc1= ref->GetNumberOfScalarComponents();
  int np2=targ->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc2=targ->GetNumberOfScalarComponents();

  if (np1!=np2)
    return 0;

  if (this->AreBothImagesShort(ref,targ)==1)
    {
      short *img1 = (short*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *img2 = (short*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);

      
      if (reset)
	this->Reset();

      if (nc1 ==1 && nc2==1)
	{
	  for (int i=0;i<np1;++i)
	    {
	      this->AddToBin(*img1,*img2);
	      ++img1;
	      ++img2;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
		  this->AddToBin(img1[c1],img2[c2]);
	      img1+=nc1;
	      img2+=nc2;
	    }
	}
    }
  else if (this->AreBothImagesUnsignedChar(ref,targ)==1)
    {
      unsigned char *img1 = (unsigned char*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      unsigned char *img2 = (unsigned char*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);

      if (reset)
	this->Reset();

      if (nc1 ==1 && nc2==1)
	{
	  for (int i=0;i<np1;++i)
	    {
	      this->AddToBin(*img1,*img2);
	      ++img1;
	      ++img2;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
		  this->AddToBin(img1[c1],img2[c2]);
	      img1+=nc1;
	      img2+=nc2;
	    }
	}
    }
  else
    {
      vtkDataArray* img1 =  ref->GetPointData()->GetScalars();
      vtkDataArray* img2 =  targ->GetPointData()->GetScalars();
      
      if (reset)
	this->Reset();

      for (int i=0;i<np1;i++)
	{
	  for (int c1=0;c1<nc1;c1++)
	    {
	      short a1=(short)img1->GetComponent(i,c1);
	      for (int c2=0;c2<nc2;c2++)
		{
		  short a2=(short)img2->GetComponent(i,c2);
		  this->AddToBin(a1,a2);
		}
	    }
	}
    }

  return 1;
}
//-------------------------------------------------------------------------
//
//  Weighted Fill Histogram
//
//-------------------------------------------------------------------------
int vtkpxJointHistogram::WeightedFillHistogram(vtkImageData* ref,vtkImageData* targ,vtkImageData* weights,int reset)
{
  static int count=0;
  ++count;

  if (weights==NULL)
    return this->FillHistogram(ref,targ,reset);
  
  int np1= ref->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc1= ref->GetNumberOfScalarComponents();
  int np2=targ->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc2=targ->GetNumberOfScalarComponents();
  int np3=weights->GetPointData()->GetScalars()->GetNumberOfTuples();

  if (count==1)
    fprintf(stderr,"Weighted Fill Histogram np1=%d, npc1=%d, np2=%d nc2=%d np3=%d\n",
	    np1,nc1,np2,nc2,np3);

  if (np1!=np2 || np1!=np3)
    return 0;

  if (this->AreBothImagesShort(ref,targ)==1 && this->AreBothImagesShort(ref,weights))
    {
      if (count==1)
	fprintf(stderr,"In Short Paradise ..... \n");
      short *img1 = (short*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *img2 = (short*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *wgt  = (short*) weights->GetPointData()->GetScalars()->GetVoidPointer(0);
      if (reset)
	this->Reset();

      if (nc1 ==1 && nc2==1)
	{
	  for (int i=0;i<np1;++i)
	    {
	      this->AddToBin(*img1,*img2,*wgt);
	      ++img1;
	      ++img2;
	      ++wgt;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
	      this->AddToBin(img1[c1],img2[c2],*wgt);
	      img1+=nc1;
	      img2+=nc2;
	      ++wgt;
	    }	  
	}
    }
  else if (this->AreBothImagesUnsignedChar(ref,targ)==1 && this->AreBothImagesUnsignedChar(ref,weights))
      {
      unsigned char *img1 = (unsigned char*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      unsigned char *img2 = (unsigned char*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);
      unsigned char *wgt  = (unsigned char*) weights->GetPointData()->GetScalars()->GetVoidPointer(0);

      if (reset)
	this->Reset();

      if (nc1 ==1 && nc2==1)
	{
	  for (int i=0;i<np1;++i)
	    {
	      this->AddToBin(*img1,*img2,*wgt);
	      ++img1;
	      ++img2;
	      ++wgt;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
		  this->AddToBin(img1[c1],img2[c2],*wgt);
	      img1+=nc1;
	      img2+=nc2;
	      ++wgt;
	    }
	}
    }
  else
    {
      vtkDataArray* img1 =  ref->GetPointData()->GetScalars();
      vtkDataArray* img2 =  targ->GetPointData()->GetScalars();
      vtkDataArray* wgt  =  weights->GetPointData()->GetScalars();
      
      if (reset)
	this->Reset();

      for (int i=0;i<np1;i++)
	{
	  for (int c1=0;c1<nc1;c1++)
	    {
	      short a1=(short)img1->GetComponent(i,c1);
	      for (int c2=0;c2<nc2;c2++)
		{
		  short a2=(short)img2->GetComponent(i,c2);
		  this->AddToBin(a1,a2,(short)wgt->GetComponent(i,0));
		}
	    }
	}
    }

  return 1;
}
//-------------------------------------------------------------------------
//
//  Dual Weighted Fill Histogram
//
//-------------------------------------------------------------------------
int vtkpxJointHistogram::DualWeightedFillHistogram(vtkImageData* ref,vtkImageData* targ,vtkImageData* weights,vtkImageData* weights2,int reset)
{
  static int count=0;
  ++count;

  if (weights==NULL)
    return this->FillHistogram(ref,targ,reset);

  if (weights2==NULL)
    return this->WeightedFillHistogram(ref,targ,weights,reset);
  
  int np1= ref->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc1= ref->GetNumberOfScalarComponents();
  int np2=targ->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc2=targ->GetNumberOfScalarComponents();
  int np3=weights->GetPointData()->GetScalars()->GetNumberOfTuples();
  int np4=weights2->GetPointData()->GetScalars()->GetNumberOfTuples();

  if (count==1)
    fprintf(stderr,"Dual Weighted Fill Histogram np1=%d, npc1=%d, np2=%d nc2=%d np3=%d np4=%d\n",
	    np1,nc1,np2,nc2,np3,np4);

  if (np1!=np2 || np1!=np3 || np1!=np4 )
    return 0;

  if (this->AreBothImagesShort(ref,targ)==1 && this->AreBothImagesShort(weights,weights2))
    {
      if (count==1)
	fprintf(stderr,"In Short Paradise ..... \n");
      short *img1 = (short*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *img2 = (short*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *wgt  = (short*) weights->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *wgt2 = (short*) weights2->GetPointData()->GetScalars()->GetVoidPointer(0);
      if (reset)
	this->Reset();

      if (nc1 ==1 && nc2==1)
	{
	  for (int i=0;i<np1;++i)
	    {
	      this->AddToBin(*img1,*img2,(*wgt)+(*wgt2));
	      ++img1;
	      ++img2;
	      ++wgt;
	      ++wgt2;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
		  this->AddToBin(img1[c1],img2[c2],(*wgt)+(*wgt2));
	      img1+=nc1;
	      img2+=nc2;
	      ++wgt;
	      ++wgt2;
	    }	  
	}
    }
  else if (this->AreBothImagesUnsignedChar(ref,targ)==1 && this->AreBothImagesUnsignedChar(weights,weights2))
      {
      unsigned char *img1 = (unsigned char*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      unsigned char *img2 = (unsigned char*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);
      unsigned char *wgt  = (unsigned char*) weights->GetPointData()->GetScalars()->GetVoidPointer(0);
      unsigned char *wgt2 = (unsigned char*) weights2->GetPointData()->GetScalars()->GetVoidPointer(0);

      if (reset)
	this->Reset();

      if (nc1 ==1 && nc2==1)
	{
	  for (int i=0;i<np1;++i)
	    {
	      this->AddToBin(*img1,*img2,(*wgt)+(*wgt2));
	      ++img1;
	      ++img2;
	      ++wgt;
	      ++wgt2;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
		  this->AddToBin(img1[c1],img2[c2],(*wgt)+(*wgt2));
	      img1+=nc1;
	      img2+=nc2;
	      ++wgt;
	      ++wgt2;
	    }
	}
    }
  else
    {
      vtkDataArray* img1 =  ref->GetPointData()->GetScalars();
      vtkDataArray* img2 =  targ->GetPointData()->GetScalars();
      vtkDataArray* wgt  =  weights->GetPointData()->GetScalars();
      vtkDataArray* wgt2 =  weights2->GetPointData()->GetScalars();
      
      if (reset)
	this->Reset();

      for (int i=0;i<np1;i++)
	{
	  for (int c1=0;c1<nc1;c1++)
	    {
	      short a1=(short)img1->GetComponent(i,c1);
	      for (int c2=0;c2<nc2;c2++)
		{
		  short a2=(short)img2->GetComponent(i,c2);
		  this->AddToBin(a1,a2,(short)(wgt->GetComponent(i,0)+wgt2->GetComponent(i,0)));
		}
	    }
	}
    }

  return 1;
}
//-------------------------------------------------------------------------
//
//  Use Intensity Gradient to compute histogram
//
//-------------------------------------------------------------------------
int vtkpxJointHistogram::FillGradientHistogram(vtkImageData* ref,vtkImageData* targ,vtkImageData* grad,
					       double step,int component,int reset)
{
  if (grad==NULL)
    return this->FillHistogram(ref,targ,reset);

  int np1= ref->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc1= ref->GetNumberOfScalarComponents();

  int np2=targ->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc2=targ->GetNumberOfScalarComponents();

  int np3=grad->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc3=grad->GetNumberOfScalarComponents();

  if (np1!=np2 || np1!=np3)
    return 0;

  if (component<0 || component>2)
    component=0;

  if (this->AreBothImagesShort(ref,targ)==1 && grad->GetScalarType()==VTK_DOUBLE)
    {
      short *img1 = (short*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *img2 = (short*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);
      double *grd =  (double*) grad->GetPointData()->GetScalars()->GetVoidPointer(0);
      for (int c=0;c<component;c++)
	++grd;
      
      if (reset)
	this->Reset();
      
      if (nc1 ==1 && nc2==1)
	{
	  for (int i=0;i<np1;++i)
	    {
	      this->AddToBin(*img1,short(*img2+(*grd)*step));
	      ++img1;
	      ++img2;
	      ++grd;
	      ++grd;
	      ++grd;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
		  this->AddToBin(img1[c1],short(img2[c2]+(*grd)*step));
	      img1+=nc1;
	      img2+=nc2;
	      ++grd;
	      ++grd;
	      ++grd;
  	    }
	}
    }
  else
    {
      vtkDataArray* img1 =  ref->GetPointData()->GetScalars();
      vtkDataArray* img2 =  targ->GetPointData()->GetScalars();
      vtkDataArray* grd =   grad->GetPointData()->GetScalars();
      
      if (reset)
	this->Reset();

      for (int i=0;i<np1;i++)
	{
	  for (int c1=0;c1<nc1;c1++)
	    {
	      short a1=(short)img1->GetComponent(i,c1);
	      for (int c2=0;c2<nc2;c2++)
		{
		  short a2=(short)img2->GetComponent(i,c2);
		  double g2=grd->GetComponent(i,component);
		  this->AddToBin(a1,short(a2+g2*step));
		}
	    }
	}
    }
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxJointHistogram::RemoveFromHistogram(vtkImageData* ref,vtkImageData* targ)
{
  int np1= ref->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc1= ref->GetNumberOfScalarComponents();
  int np2= targ->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc2= targ->GetNumberOfScalarComponents();

  if (np1!=np2)
    return 0;

  if (this->AreBothImagesShort(ref,targ)==1)
    {
      short *img1 = (short*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *img2 = (short*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);
      
      if (nc1==1 && nc2==1)
	{
	  for (int i=0;i<np1;i++)
	    {
	      this->DelToBin(*img1,*img2);
	      ++img1;
	      ++img2;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
		  this->DelToBin(img1[c1],img2[c2]);
	      img1+=nc1;
	      img2+=nc2;
	    }
	}
    }
  else if (this->AreBothImagesUnsignedChar(ref,targ)==1)
    {
      unsigned char *img1 = (unsigned char*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      unsigned char *img2 = (unsigned char*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);
      
      if (nc1==1 && nc2==1)
	{
	  for (int i=0;i<np1;i++)
	    {
	      this->DelToBin(*img1,*img2);
	      ++img1;
	      ++img2;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
		  this->DelToBin(img1[c1],img2[c2]);
	      img1+=nc1;
	      img2+=nc2;
	    }
	}
    }
  else
    {
      vtkDataArray* img1 =  ref->GetPointData()->GetScalars();
      vtkDataArray* img2 =  targ->GetPointData()->GetScalars();
      
      for (int i=0;i<np1;i++)
	{
	  for (int c1=0;c1<nc1;c1++)
	    {
	      short a1=(short)img1->GetComponent(i,c1);
	      for (int c2=0;c2<nc2;c2++)
		{
		  short a2=(short)img2->GetComponent(i,c2);
		  this->DelToBin(a1,a2);
		}
	    }
	}
    }

  return 1;
}
//-------------------------------------------------------------------------
int vtkpxJointHistogram::WeightedRemoveFromHistogram(vtkImageData* ref,vtkImageData* targ,vtkImageData* weights)
{
  if (weights==NULL)
    return this->RemoveFromHistogram(ref,targ);


  int np1= ref->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc1= ref->GetNumberOfScalarComponents();
  int np2= targ->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc2= targ->GetNumberOfScalarComponents();
  int np3=weights->GetPointData()->GetScalars()->GetNumberOfTuples();
  if (np1!=np2 || np1!=np3)
    return 0;

  if (this->AreBothImagesShort(ref,targ)==1 && this->AreBothImagesShort(ref,weights)==1)
    {
      short *img1 = (short*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *img2 = (short*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *wgt  = (short*) weights->GetPointData()->GetScalars()->GetVoidPointer(0);
      
      if (nc1==1 && nc2==1)
	{
	  for (int i=0;i<np1;i++)
	    {
	      this->DelToBin(*img1,*img2,*wgt);
	      ++img1;
	      ++img2;
	      ++wgt;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
		  this->DelToBin(img1[c1],img2[c2],*wgt);
	      img1+=nc1;
	      img2+=nc2;
	      ++wgt;
	    }
	}
    }
  else if (this->AreBothImagesUnsignedChar(ref,targ)==1 && this->AreBothImagesUnsignedChar(ref,weights)==1)
    {
      unsigned char *img1 = (unsigned char*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      unsigned char *img2 = (unsigned char*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);
      unsigned char *wgt  = (unsigned char*) weights->GetPointData()->GetScalars()->GetVoidPointer(0);
	    
      if (nc1==1 && nc2==1)
	{
	  for (int i=0;i<np1;i++)
	    {
	      this->DelToBin(*img1,*img2,*wgt);
	      ++img1;
	      ++img2;
	      ++wgt;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
		  this->DelToBin(img1[c1],img2[c2],*wgt);
	      img1+=nc1;
	      img2+=nc2;
	      ++wgt;
	    }
	}
    }
  else
    {
      vtkDataArray* img1 =  ref->GetPointData()->GetScalars();
      vtkDataArray* img2 =  targ->GetPointData()->GetScalars();
      vtkDataArray* wgt  =  weights->GetPointData()->GetScalars();
      
      for (int i=0;i<np1;i++)
	{
	  short w=(short)wgt->GetComponent(i,0);
	  for (int c1=0;c1<nc1;c1++)
	    {
	      short a1=(short)img1->GetComponent(i,c1);
	      for (int c2=0;c2<nc2;c2++)
		{
		  short a2=(short)img2->GetComponent(i,c2);
		  this->DelToBin(a1,a2,w);
		}
	    }
	}
    }

  return 1;
}
// -------------------------------------------------------------------------------
int vtkpxJointHistogram::DualWeightedRemoveFromHistogram(vtkImageData* ref,vtkImageData* targ,vtkImageData* weights,vtkImageData* weights2)
{
  if (weights==NULL)
    return this->RemoveFromHistogram(ref,targ);

  if (weights2==NULL)
    return this->WeightedRemoveFromHistogram(ref,targ,weights);


  int np1= ref->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc1= ref->GetNumberOfScalarComponents();
  int np2= targ->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc2= targ->GetNumberOfScalarComponents();
  int np3=weights->GetPointData()->GetScalars()->GetNumberOfTuples();
  int np4=weights2->GetPointData()->GetScalars()->GetNumberOfTuples();

  if (np1!=np2 || np1!=np3 || np1!=np4 )
    return 0;

  if (this->AreBothImagesShort(ref,targ)==1 && this->AreBothImagesShort(weights,weights2)==1)
    {
      short *img1 = (short*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *img2 = (short*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *wgt  = (short*) weights->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *wgt2  = (short*) weights2->GetPointData()->GetScalars()->GetVoidPointer(0);
      
      if (nc1==1 && nc2==1)
	{
	  for (int i=0;i<np1;i++)
	    {
	      this->DelToBin(*img1,*img2,((*wgt)+(*wgt2)));
	      ++img1;
	      ++img2;
	      ++wgt;
	      ++wgt2;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
		  this->DelToBin(img1[c1],img2[c2],((*wgt)+(*wgt2)));
	      img1+=nc1;
	      img2+=nc2;
	      ++wgt;
	      ++wgt2;
	    }
	}
    }
  else if (this->AreBothImagesUnsignedChar(ref,targ)==1 && this->AreBothImagesUnsignedChar(ref,weights)==1)
    {
      unsigned char *img1 = (unsigned char*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      unsigned char *img2 = (unsigned char*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);
      unsigned char *wgt  = (unsigned char*) weights->GetPointData()->GetScalars()->GetVoidPointer(0);
      unsigned char *wgt2 = (unsigned char*) weights2->GetPointData()->GetScalars()->GetVoidPointer(0);
	    
      if (nc1==1 && nc2==1)
	{
	  for (int i=0;i<np1;i++)
	    {
	      this->DelToBin(*img1,*img2,((*wgt)+(*wgt2)));
	      ++img1;
	      ++img2;
	      ++wgt;
	      ++wgt2;
	    }
	}
      else
	{
	  for (int i=0;i<np1;i++)
	    {
	      for (int c1=0;c1<nc1;c1++)
		for (int c2=0;c2<nc2;c2++)
		  this->DelToBin(img1[c1],img2[c2],((*wgt)+(*wgt2)));
	      img1+=nc1;
	      img2+=nc2;
	      ++wgt;
	      ++wgt2;
	    }
	}
    }
  else
    {
      vtkDataArray* img1 =  ref->GetPointData()->GetScalars();
      vtkDataArray* img2 =  targ->GetPointData()->GetScalars();
      vtkDataArray* wgt  =  weights->GetPointData()->GetScalars();
      vtkDataArray* wgt2 =  weights2->GetPointData()->GetScalars();
      
      for (int i=0;i<np1;i++)
	{
	  short w=(short)wgt->GetComponent(i,0);
	  w=w+(short)wgt2->GetComponent(i,0);
	  for (int c1=0;c1<nc1;c1++)
	    {
	      short a1=(short)img1->GetComponent(i,c1);
	      for (int c2=0;c2<nc2;c2++)
		{
		  short a2=(short)img2->GetComponent(i,c2);
		  this->DelToBin(a1,a2,w);
		}
	    }
	}
    }

  return 1;
}
// -------------------------------------------------------------------------------
float vtkpxJointHistogram::ComputeEntropyOfDifference()
{
  static float newhist[2048];

  int ncommon=this->NumberOfBins[0]+this->NumberOfBins[1]-1;
  if (ncommon>2048)
    {
      fprintf(stderr,"Difference Histogram is too big\n");
      return 0.0;
    }

  for (int ia=0;ia<ncommon;ia++)
    newhist[ia]=0.0;

  for (int j = 0; j < NumberOfBins[1]; j++)
    for (int i = 0; i < NumberOfBins[0]; i++)
      {
	int index=(j-i)+NumberOfBins[0]-1;
	newhist[index]+=this->Bins[i][j];
      }

  float total=0.0;
  for (int k=0;k<ncommon;k++)
    total+=newhist[k];

  float val = 0;
  for (int ic=0;ic<ncommon;ic++)
    {
      float a=newhist[ic]/total;
      if (a>0)
	val+=a*log(a);
    }

  return -val;
}

// -------------------------------------------------------------------------------------

const char* vtkpxJointHistogram::NameOfMeasure(int measureno)
{
  // Return the name of similarity measure
  switch (measureno) 
    {
    case 1:
      return "JointEntropy";
      break;
    case 2:
      return "Correlation";
      break;
    case 3:
      return "Gradient Correlation";
      break;
    case 4:
      return "Mutual Information";
      break;
    case 5:
      return "Normalized Mutual Information";
      break;
    case 6:
      return "Sums of Squared Differences";
      break;
    case 7:
      return "Correlation ratio C(X|Y)";
      break;
    case 8:
      return "Correlation ratio C(Y|X)";
    case 9:
      return "Product Y*x";
      break;
    case 10:
      return "Entropy of Difference";
      break;
    }
  return "Undefined!";

}
// -------------------------------------------------------------------------------------
float vtkpxJointHistogram::Similarity(int measureno)
{
  switch (measureno) 
    {
    case 1:
      return -this->JointEntropy();
      break;
    case 2:
      return +this->CrossCorrelation();
      break;
    case 3:
      return fabs(this->CrossCorrelation());
      break;
    case 4:
      return +this->MutualInformation();
      break;
    case 5:
      return +this->NormalizedMutualInformation();
      break;
    case 6:
      return -this->SumsOfSquaredDifferences();
      break;
    case 7:
      return +this->CorrelationRatioXY();
      break;
    case 8:
      return +this->CorrelationRatioYX();
      break;
    case 9:
      return this->ComputeProduct();
      break;
    case 10:
      return -this->ComputeEntropyOfDifference();
      break;
    default:
      return 0;
      break;
    }
}


// -------------------------------------------------------------------------------------
vtkImageData* vtkpxJointHistogram::GetHistogramAsImage()
{
  vtkImageData* img=vtkImageData::New();
  img->SetDimensions(this->NumberOfBins[0],this->NumberOfBins[1],1);
  img->SetScalarTypeToFloat();
  img->AllocateScalars();

  for (int ia=0;ia<this->NumberOfBins[0];ia++)
    for (int ib=0;ib<this->NumberOfBins[1];ib++)
      img->SetScalarComponentFromDouble(ia,ib,0,0,this->Bins[ia][ib]);
  return img;
}

int  vtkpxJointHistogram::SetHistogramFromImage(vtkImageData* img)
{
  if (img==NULL)
    return 0;

  int dim[3];
  img->GetDimensions(dim);
  if (dim[2]!=1)
    {
      fprintf(stderr,"Error 3D Image Specified in SetHistogramFromImage\n");
      return 0;
    }

  this->Allocate(dim[0],dim[1]);
  for (int ia=0;ia<dim[0];ia++)
    for (int ib=0;ib<dim[1];ib++)
      {
	double v=img->GetScalarComponentAsDouble(ia,ib,0,0);
	this->Bins[ia][ib]=int(v);
      }
  return 1;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxJointHistogram::FillHistogramComponent(vtkImageData* ref,vtkImageData* targ,int component1,int component2,int reset)
{
  
  int np1= ref->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc1= ref->GetNumberOfScalarComponents();
  int np2=targ->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc2=targ->GetNumberOfScalarComponents();

  if (np1!=np2)
    return 0;
  
  if (component1<0 || component1>=nc1)
    component1=0;

  if (component2<0 || component2>=nc2)
    component2=0;

  if (this->AreBothImagesShort(ref,targ)==1)
    {
      short *img1 = (short*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *img2 = (short*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);

      if (reset)
	this->Reset();

      img1+=component1;
      img2+=component2;

      for (int i=0;i<np1;i++)
	{
	  this->AddToBin(*img1,*img2);
	  img1+=nc1;
	  img2+=nc2;
	}
    }
  else
    {
      vtkDataArray* img1 =  ref->GetPointData()->GetScalars();
      vtkDataArray* img2 =  targ->GetPointData()->GetScalars();
      
      if (reset)
	this->Reset();

      for (int i=0;i<np1;i++)
	{
	  short a1=(short)img1->GetComponent(i,component1);
	  short a2=(short)img2->GetComponent(i,component2);
	  this->AddToBin(a1,a2);
	}
    }

  return 1;
}
//-------------------------------------------------------------------------
int vtkpxJointHistogram::RemoveFromHistogramComponent(vtkImageData* ref,vtkImageData* targ,int component1,int component2)
{
  int np1= ref->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc1= ref->GetNumberOfScalarComponents();
  int np2= targ->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc2= targ->GetNumberOfScalarComponents();

  if (np1!=np2)
    return 0;

  if (component1<0 || component1>=nc1)
    component1=0;

  if (component2<0 || component2>=nc2)
    component2=0;


  if (this->AreBothImagesShort(ref,targ)==1)
    {
      short *img1 = (short*)  ref->GetPointData()->GetScalars()->GetVoidPointer(0);
      short *img2 = (short*) targ->GetPointData()->GetScalars()->GetVoidPointer(0);

      img1+=component1;
      img2+=component2;
      
      for (int i=0;i<np1;i++)
	{
	  this->DelToBin(*img1,*img2);
	  img1+=nc1;
	  img2+=nc2;
	}
    }
  else
    {
      vtkDataArray* img1 =  ref->GetPointData()->GetScalars();
      vtkDataArray* img2 =  targ->GetPointData()->GetScalars();
      
      for (int i=0;i<np1;i++)
	{
	      short a1=(short)img1->GetComponent(i,component1);
	      short a2=(short)img2->GetComponent(i,component2);
	      this->DelToBin(a1,a2);
	}
    }

  return 1;
}
// -------------------------------------------------------------------------------------------------------------
void vtkpxJointHistogram::CreateHistogram(vtkImageData* ref, int numbinsref,vtkImageData* trg, int numbinstrg)
{
  short minr=0,maxr=1;
  vtkImageData* img1=vtkImageData::New();
  vtkImageData* img2=vtkImageData::New();

  for (int ia=0;ia<=1;ia++)
    {
      vtkImageCast* cst=vtkImageCast::New();
      cst->SetOutputScalarTypeToShort();
      if (ia==0)
	cst->SetInput(ref);
      else
	cst->SetInput(trg);
      cst->Update();
      
      if (ia==0)
	img1->ShallowCopy(cst->GetOutput());
      else
	img2->ShallowCopy(cst->GetOutput());
      cst->Delete();
    }


  int num_ref=vtkpxRegistration::CalculateNumberOfBinsAndAdjustImage(img1,numbinsref,
								     minr,maxr,
								     0,1,0);
  int num_tar=vtkpxRegistration::CalculateNumberOfBinsAndAdjustImage(img2,numbinstrg,
								     minr,maxr,
								     0,1,0);

  fprintf(stdout,"Computing Histogram num_ref=%d, num_tar=%d\n",num_ref,num_tar);
  this->Allocate(num_ref,num_tar);
  this->FillHistogram(img1,img2,1);
  img1->Delete();
  img2->Delete();

}


void vtkpxJointHistogram::CreateDualWeightedHistogram(vtkImageData* ref, int numbinsref,vtkImageData* trg, int numbinstrg,
							   vtkImageData* refw,vtkImageData* trgw)
{
  short minr=0,maxr=1;


  vtkImageData* img[4];

  for (int ia=0;ia<=3;ia++)
    {
      img[ia]=vtkImageData::New();
      vtkImageCast* cst=vtkImageCast::New();
      cst->SetOutputScalarTypeToShort();
      if (ia==0)
	cst->SetInput(ref);
      else if (ia==1)
	cst->SetInput(trg);
      else if (ia==2)
	cst->SetInput(refw);
      else if (ia==3)
	cst->SetInput(trgw);
      cst->Update();
      
      img[ia]->ShallowCopy(cst->GetOutput());
      cst->Delete();
    }


  int num_ref=vtkpxRegistration::CalculateNumberOfBinsAndAdjustImage(img[0],numbinsref,
								     minr,maxr,
								     0,1,0);
  int num_tar=vtkpxRegistration::CalculateNumberOfBinsAndAdjustImage(img[1],numbinstrg,
								     minr,maxr,
								     0,1,0);
  
  vtkpxRegistration::ScaleWeightImage(img[2],img[0],100.0);
  vtkpxRegistration::ScaleWeightImage(img[3],img[1],100.0);

  fprintf(stdout,"Computing Weighted Histogram num_ref=%d, num_tar=%d\n",num_ref,num_tar);
  this->Allocate(num_ref,num_tar);
  this->DualWeightedFillHistogram(img[0],img[1],img[2],img[3],1);


  img[0]->Delete();
  img[1]->Delete();
  img[2]->Delete();
  img[3]->Delete();

}


