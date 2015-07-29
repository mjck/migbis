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





#include "vtkpxImageCorrectNonUniformZ.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include <stdio.h>
#include "pxutil.h"
#include "vtkpxBSpline.h"

vtkpxImageCorrectNonUniformZ* vtkpxImageCorrectNonUniformZ::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageCorrectNonUniformZ");
  if(ret)
    {
      return (vtkpxImageCorrectNonUniformZ*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageCorrectNonUniformZ;
}


// Construct object to extract all of the input data.
vtkpxImageCorrectNonUniformZ::vtkpxImageCorrectNonUniformZ()
{
  this->ZLevels=NULL;
  this->Spacing=1.0;
}

// ---------------------------------------------------------------------------
vtkpxImageCorrectNonUniformZ::~vtkpxImageCorrectNonUniformZ()
{
  this->SetZLevels(NULL);
}

// ---------------------------------------------------------------------------
void vtkpxImageCorrectNonUniformZ::ExecuteInformation()
{
  if (this->ZLevels==NULL)
    {
      vtkErrorMacro(<<"No Z-Levels Specified\n");
      return;
    }

  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  vtkImageData *input=this->GetImageDataInput(0);
  vtkImageData *output=this->GetOutput();

  if (input==NULL)
    {
      output->SetSpacing(1,1,1);
      output->SetOrigin(0,0,0);
      output->SetWholeExtent(0,0,0,0,0,0);

      return;
    }

  double sp[3]; input->GetSpacing(sp);

  int wholeExtent[6];
  int dims[3]; 
  input->GetDimensions(dims);
  
  wholeExtent[0]=0;  
  wholeExtent[2]=0;
  wholeExtent[4]=0;  
    
  wholeExtent[1] = dims[0]-1;
  wholeExtent[3] = dims[1]-1;

  float z1=this->ZLevels->GetComponent(0,0);
  float z2=this->ZLevels->GetComponent(dims[2]-1,0);
  float dz=fabs(z2-z1);

  wholeExtent[5]= int(dz/this->Spacing+1);

  output->SetSpacing(sp[0],sp[1],this->Spacing);
  output->SetOrigin(input->GetOrigin());
  output->SetWholeExtent( wholeExtent );
  output->SetScalarType(input->GetScalarType());
  output->SetNumberOfScalarComponents(input->GetNumberOfScalarComponents());
}
// ---------------------------------------------------------------------------
void vtkpxImageCorrectNonUniformZ::SimpleExecute(vtkImageData* input, vtkImageData* output) 
{
  if (this->ZLevels==NULL)
    {
      vtkErrorMacro(<<"No Z-Levels Specified\n");
      return;
    }


  fprintf(stderr,"Input Components\n--------------------------\n");
  int numtuples=this->ZLevels->GetNumberOfTuples();
  for (int i=0;i<numtuples;i++)
    fprintf(stderr,"i=%d, z=%.2f\n",i,this->ZLevels->GetComponent(i,0));
  fprintf(stderr,"\n\n");

  vtkDebugMacro(<< "Resampling CT");
  this->Spacing=Frange(this->Spacing,0.1,100.0);
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  int dim_o[3]; output->GetDimensions(dim_o);
  int dim_i[3]; input->GetDimensions(dim_i);

  double spa[3]; output->GetSpacing(spa);
  double sp_in[3]; input->GetSpacing(sp_in);  

  float z1=this->ZLevels->GetComponent(0,0);
  float z2=this->ZLevels->GetComponent(dim_i[2]-1,0);
  float dz=fabs(z2-z1);

  fprintf(stderr,"Dims = %d,%d,%d --> %d,%d,%d,  nom=%.2fx%.2fx%.2f (dz=%.2f)->%.2fx%.2fx%.2f\n",
	  dim_i[0],dim_i[1],dim_i[2],
	  dim_o[0],dim_o[1],dim_o[2],
	  sp_in[0],sp_in[1],sp_in[2],dz,
	  spa[0],spa[1],spa[2]);

  int slicesize=dim_o[0]*dim_o[1];

  vtkFloatArray* data=vtkFloatArray::New();
  data->SetNumberOfTuples(dim_i[2]);
  
  double ori[3]; output->GetOrigin(ori);

  int numc=input->GetNumberOfScalarComponents();
  
  for (int ka=0;ka<dim_o[2];ka++)
    {
      float wgt[2],zl[2];

      int baseindex=0;
      int nextindex=1;
      wgt[0]=1.0; 
      wgt[1]=0.0; 
      zl[0]=this->ZLevels->GetComponent(0,0);
      zl[1]=this->ZLevels->GetComponent(1,0);
      float dzl=zl[1]-zl[0];

      float z=ori[2]+ka*spa[2];
      int reached_top=0;
      int firstpass=0;
      /*      fprintf(stderr,"\n ---------------------------- \n Starting z=%.2f zl=%.2f, %.2f indices=%d,%d\n",
	      z,zl[0],zl[1],baseindex,nextindex);*/
	
      

	while (z>zl[1] && reached_top==0)
	  {
	    baseindex+=1;
	    if (baseindex>numtuples-2)
	      {
		reached_top=1;
		baseindex=numtuples-2;
	      }
	    
	    zl[0]=this->ZLevels->GetComponent(baseindex,0);
	    nextindex=baseindex+1;
	    zl[1]=this->ZLevels->GetComponent(nextindex,0);
	    dzl=zl[1]-zl[0];
	    /*	    fprintf(stderr,"\t  zl=%.2f, %.2f indices=%d,%d\n",
	      zl[0],zl[1],baseindex,nextindex);*/
	}
	  
      if (z>=zl[0] && z<zl[1])
	{
	  wgt[0]=(zl[1]-z)/dzl;
	  wgt[1]=1.0-wgt[0];
	}
      else if (z<zl[0])
	{
	  wgt[0]=1.0;
	  wgt[1]=0.0;
	}
      else
	{
	  wgt[0]=0.0;
	  wgt[1]=1.0;
	}

      fprintf(stderr,"ka=%d z=%.2f ( base=%d,zl[0]=%.2f,wgt[0]=%.3f \t next=%d zl[1]=%.2f,wgt[1]=%.3f) dzl=%.2f\n",
	      ka,z,
	      baseindex,zl[0],wgt[0],
	      nextindex,zl[1],wgt[1],dzl);

      int index=ka*slicesize;
      for (int j=0;j<dim_o[1];j++)
	for (int i=0;i<dim_o[0];i++)
	  {
	    for (int comp=0;comp<numc;comp++)
	      out->SetComponent(index,comp,
				wgt[0]*input->GetScalarComponentAsDouble(i,j,baseindex,comp)+
				wgt[1]*input->GetScalarComponentAsDouble(i,j,nextindex,comp));
	    ++index;
	  }
    }
  fprintf(stderr,"\nDone\n");
  
  data->Delete();

  
}
  

