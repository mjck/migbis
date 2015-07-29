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
  Module:    $RCSfile: vtkpxCSIStackImages.cpp,v $
  Language:  C++
  Date:      $Date: 2002/09/26 18:09:25 $
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
#include "vtkSource.h"
#include "vtkpxCSIStackImages.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkbisImageReslice.h"
#include "vtkTransform.h"
//------------------------------------------------------------------------------
vtkpxCSIStackImages* vtkpxCSIStackImages::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxCSIStackImages");
  if(ret)
    {
      return (vtkpxCSIStackImages*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxCSIStackImages;
}

// Construct object with no children.
vtkpxCSIStackImages::vtkpxCSIStackImages()
{
  this->AnatomicalOutput=NULL;
  this->AnatomicalInput=NULL;
  this->ResolutionXY=1.0;
  this->ResolutionZ=1.0;
}
// ----------------------------------------------------------------------------
vtkpxCSIStackImages::~vtkpxCSIStackImages()
{
  if (this->AnatomicalOutput!=NULL)
    this->AnatomicalOutput->Delete();
  this->SetAnatomicalInput(NULL);
}
// ----------------------------------------------------------------------------
void vtkpxCSIStackImages::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  if (this->GetNumberOfInputPorts()<1 || this->AnatomicalInput == NULL)
    {
      vtkErrorMacro(<<"Need At least one input and an Anatomical Input\n");
      return;
    }

  // Compute Anatomical Extent
  double anat_ori[3]; this->AnatomicalInput->GetOrigin(anat_ori);
  double anat_spa[3]; this->AnatomicalInput->GetSpacing(anat_spa);
  int    anat_dim[3]; this->AnatomicalInput->GetDimensions(anat_dim);
  double  anat_ext[3][2] = { 0.0,1.0,0.0,1.0,0.0,1.0};

  vtkImageData* CSI=this->GetImageDataInput(0);
  int    csi_dim[3]; CSI->GetDimensions(csi_dim);
  double csi_spa[3]; CSI->GetSpacing(csi_spa);
  double csi_ori[3]; CSI->GetOrigin(csi_ori);

  double  csi_ext[3][2] = { 0.0,1.0,0.0,1.0,0.0,1.0};


  for (int ia=0;ia<=2;ia++)
    {
      anat_ext[ia][0]=anat_ori[ia];
      anat_ext[ia][1]=anat_ext[ia][0]+double(anat_dim[ia]-1)*anat_spa[ia];
      csi_ext[ia][0]=csi_ori[ia];
      csi_ext[ia][1]=csi_ext[ia][0]+double(csi_dim[ia]-1)*csi_spa[ia];
    }
  anat_ext[2][0]=-0.5*double(anat_dim[2]-1)*anat_spa[2];
  anat_ext[2][1]=+0.5*double(anat_dim[2]-1)*anat_spa[2];


  fprintf(stderr,"CSI Extent 0 = %.2f:%.2f  %.2f:%.2f  %.2f:%.2f\n",
	  csi_ext[0][0],csi_ext[0][1],csi_ext[1][0],csi_ext[1][1],csi_ext[2][0],csi_ext[2][1]);

  if (this->GetNumberOfInputPorts()>1)
    {
      for (int j=1;j<this->GetNumberOfInputPorts();j++)
	{
	  double z=this->GetImageDataInput(j)->GetOrigin()[2];

	  if (csi_ext[2][0]>z)
	    csi_ext[2][0]=z;
	  if (csi_ext[2][1]<z)
	    csi_ext[2][1]=z;
	  fprintf(stderr,"CSI Extent %d = %.2f:%.2f  %.2f:%.2f  %.2f:%.2f\n",j,
		  csi_ext[0][0],csi_ext[0][1],csi_ext[1][0],csi_ext[1][1],csi_ext[2][0],csi_ext[2][1]);
	}
    }

  fprintf(stderr,"CSI Center = (%.2f,%.2f,%.2f)\n",
	  0.5*(csi_ext[0][0]+csi_ext[0][1]),
	  0.5*(csi_ext[1][0]+csi_ext[1][1]),
	  0.5*(csi_ext[2][0]+csi_ext[2][1]));
  
  fprintf(stderr,"Anatomical Extent = %.2f:%.2f  %.2f:%.2f  %.2f:%.2f\n",
	  anat_ext[0][0],anat_ext[0][1],anat_ext[1][0],anat_ext[1][1],anat_ext[2][0],anat_ext[2][1]);
  fprintf(stderr,"Anatomical Center = (%.2f,%.2f,%.2f)\n",
	  0.5*(anat_ext[0][0]+anat_ext[0][1]),
	  0.5*(anat_ext[1][0]+anat_ext[1][1]),
	  0.5*(anat_ext[2][0]+anat_ext[2][1]));


  double half=0.5*(csi_spa[2])-0.5*this->ResolutionZ;

  double minz=csi_ext[2][0]-half;
  while (minz>anat_ext[2][0])
    minz-=this->ResolutionZ;

  double maxz=csi_ext[2][1]+half;
  while (maxz<anat_ext[2][1])
    maxz+=this->ResolutionZ;

  fprintf(stderr,"Final Z-Range = %.2f:%.2f\n",minz,maxz);

  int wholeExtent[6]= { 0,1,0,1,0,1};
  wholeExtent[1] = int(csi_dim[0]*csi_spa[0]/this->ResolutionXY)-1;
  wholeExtent[3] = int(csi_dim[1]*csi_spa[1]/this->ResolutionXY)-1;
  wholeExtent[5] = int((maxz-minz)/this->ResolutionZ+0.5);
  
  fprintf(stderr,"Output Dimensions = %d,%d,%d\n",wholeExtent[1]+1,wholeExtent[3]+1,wholeExtent[5]+1);

  vtkImageData *output=this->GetOutput();
  output->DebugOn();
  output->SetSpacing(this->ResolutionXY,this->ResolutionXY,this->ResolutionZ);
  output->SetOrigin(0.0,0.0,minz);
  output->SetWholeExtent( wholeExtent );
  output->DebugOff();
  output->SetUpdateExtent( wholeExtent );

  output->SetScalarType(CSI->GetScalarType());
  output->SetNumberOfScalarComponents(CSI->GetNumberOfScalarComponents());

}

// ----------------------------------------------------------------------------
void vtkpxCSIStackImages::SimpleExecute(vtkImageData* ,vtkImageData* output)
{

  if (this->GetNumberOfInputPorts()==0 || this->GetImageDataInput(0)==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }

  int dim[3];    output->GetDimensions(dim);
  double spa[3]; output->GetSpacing(spa);
  double ori[3]; output->GetOrigin(ori);
  fprintf(stderr,"Output Origin = %.2f %.2f %.2f\n",ori[0],ori[1],ori[2]);

  double spa_csi[3]; this->GetImageDataInput(0)->GetSpacing(spa_csi);
  int ratiox=int(spa_csi[0]/spa[0]);
  int ratioy=int(spa_csi[1]/spa[1]);
  int* index=new int[dim[2]];

  for (int i=0;i<dim[2];i++)
    {
      index[i]=-1;
      double zlevel=ori[2]+double(i)*spa[2];

      int j=0;
      while (j<this->GetNumberOfInputPorts() && index[i]==-1)
	{
	  double z=this->GetImageDataInput(j)->GetOrigin()[2];
	  
	  if (zlevel>=(z-0.5*spa_csi[2]) && zlevel<(z+0.5*spa_csi[2]))
	    index[i]=j;
	  ++j;
	}

      if (index[i]!=-1)
	fprintf(stderr,"zlevel = %.2f \t  index=%d\n",zlevel,index[i]);
      else
	fprintf(stderr,"zlevel = %.2f \t  index= *\n",zlevel);
    }

  this->UpdateProgress(0.2);

  int nc=output->GetNumberOfScalarComponents();

  for (int k=0;k<dim[2];k++)
    {
      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {
	    if (index[k]==-1)
	      {
		for (int c=0;c<nc;c++)
		  output->SetScalarComponentFromDouble(i,j,k,c,0.0);
	      }
	    else
	      {
		vtkImageData* input=this->GetImageDataInput(index[k]);
		int in_i=i/ratiox;
		int in_j=j/ratioy;
		for (int c=0;c<nc;c++)
		  output->SetScalarComponentFromDouble(i,j,k,c,input->GetScalarComponentAsDouble(in_i,in_j,0,c));
	      }
	  }
    }

  delete [] index;


  this->UpdateProgress(0.8);

  double anat_ori[3]; this->AnatomicalInput->GetOrigin(anat_ori);
  double anat_spa[3]; this->AnatomicalInput->GetSpacing(anat_spa);
  int    anat_dim[3]; this->AnatomicalInput->GetDimensions(anat_dim);
  
  double shift[3] = { 0.0,0.0,0.0};

  fprintf(stderr,"Origin = %.2f %.2f %.2f\n",ori[0],ori[1],ori[2]);
  fprintf(stderr,"Spacing = %.2f %.2f %.2f\n",spa[0],spa[1],spa[2]);
  fprintf(stderr,"Dimensions = %d %d %d\n",dim[0],dim[1],dim[2]);

  for (int i=0;i<=1;i++)
    {
      double c0=ori[i]+0.5*(double(dim[i]-1))*spa[i];
      double c1=anat_ori[i]+0.5*(double(anat_dim[i]-1))*anat_spa[i];
      shift[i]=c1-c0;
      fprintf(stderr,"i=%d c0=%.2f c1=%.2f shift[i]=%.2f\n",
	      i,c0,c1,shift[i]);
    }
  shift[2]=+0.5*double(anat_dim[2]-1)*anat_spa[2];

  fprintf(stderr,"Shift = %.2f %.2f %.2f\n",shift[0],shift[1],shift[2]);

  vtkTransform* tr=vtkTransform::New();
  tr->Identity();
  tr->DebugOn();
  tr->Translate(shift);
  tr->DebugOff();

  vtkbisImageReslice* resl=vtkbisImageReslice::New(); resl->OptimizationOff();
  resl->SetInput(this->AnatomicalInput);
  resl->SetInformationInput(this->GetOutput());
  resl->SetResliceTransform(tr);
  resl->SetInterpolationMode(1);
  resl->SetBackgroundLevel(0.0);
  resl->Update();
  
  if (this->AnatomicalOutput!=NULL)
    this->AnatomicalOutput->Delete();

  this->AnatomicalOutput=vtkImageData::New();
  this->AnatomicalOutput->ShallowCopy(resl->GetOutput());
  resl->Delete();
  tr->Delete();

  this->UpdateProgress(1.0);
}


