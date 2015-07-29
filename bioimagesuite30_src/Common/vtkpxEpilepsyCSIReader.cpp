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
  Module:    $RCSfile: vtkpxEpilepsyCSIReader.cpp,v $
  Language:  C++
  Date:      $Date: 2002/08/09 20:40:19 $
  Version:   $Revision: 1.3 $


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

#include "vtkpxEpilepsyCSIReader.h"
#include "vtkFloatArray.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "pxutil.h"
//------------------------------------------------------------------------------
vtkpxEpilepsyCSIReader* vtkpxEpilepsyCSIReader::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxEpilepsyCSIReader");
  if(ret)
    {
      return (vtkpxEpilepsyCSIReader*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxEpilepsyCSIReader;
}

// Construct object with NULL file prefix; file pattern "%s.%d"; image range 
// set to (1,1); data origin (0,0,0); data spacing (1,1,1); no data mask;
// header size 0; and byte swapping turned off.

vtkpxEpilepsyCSIReader::vtkpxEpilepsyCSIReader()
{
  this->Status=0;
  this->FlipZOffset=1;
  this->ScaleFactors=vtkFloatArray::New();
  this->CategoryNames=new char[300];
}


vtkpxEpilepsyCSIReader::~vtkpxEpilepsyCSIReader()
{
  this->ScaleFactors->Delete();
  delete [] this->CategoryNames;
}
//------------------------------------------------------------------------------
int vtkpxEpilepsyCSIReader::Load(const char* ana)
{
  this->SetFileName(ana);
  this->Update();
  return this->Status;
}
//------------------------------------------------------------------------------
void vtkpxEpilepsyCSIReader::Execute()
{
  this->Status=0;

  if (this->FileName == NULL) 
    {
      vtkErrorMacro(<< "FileName is NULL");
      return;
    }



  FILE* fin=fopen(this->FileName,"r");
  if (!fin)
    {
      vtkErrorMacro(<< "Bad FileName " << this->FileName);
      return;
    } 
  
  char* line=new char[200];

  // Read Header Line
  fgets(line,200,fin);
  if (gzfindstringinstring(line, "#AECOM MRS CSI Data File")==0)
    {
      vtkErrorMacro(<< "Bad File Header " << line);
      delete [] line;
      fclose(fin);
      return;
    } 

  // Read Next Line
  // Read Dimensions
  fgets(line,200,fin);  fgets(line,200,fin); // fprintf(stderr,"line=%s\n",line);
  int width=24,height=24,ncomponents=1;
  sscanf(line,"%d %d %d\n",&width,&height,&ncomponents);
  // fprintf(stderr,"line=%s\n %d %d %d\n",line,width,height,ncomponents);

  // Read Spacing  
  fgets(line,200,fin);  fgets(line,200,fin);  // fprintf(stderr,"line=%s\n",line);
  double spa[3] = { 8.0,8.0,10.0 };
  sscanf(line,"%lf %lf %lf\n",&spa[0],&spa[1],&spa[2]);

  fgets(line,200,fin);  fgets(line,200,fin);  // fprintf(stderr,"line=%s\n",line);
  double ori[3] = { 0.0,0.0,0};
  sscanf(line,"%lf \n",&ori[2]);
  if (this->FlipZOffset)
    ori[2]=-1.0*ori[2];


  // Read Flip Modes 
  fgets(line,200,fin);  fgets(line,200,fin);
  int  flip[3] = { 0,0,0 };
  sscanf(line,"%d %d %d \n",&flip[0],&flip[1],&flip[2]);
  fprintf(stderr,"Flips = %d %d %d\n",flip[0],flip[1],flip[2]);
  
  
  // Read Actual Number of Voxels
  fgets(line,200,fin);  fgets(line,200,fin);
  int nvoxels=1;
  sscanf(line,"%d\n",&nvoxels); 
  fprintf(stderr,"Number Of Voxels = %d\n",nvoxels);
  fgets(line,200,fin);
  fprintf(stderr,"line=%s\n",line);

  this->ScaleFactors->SetNumberOfComponents(1);
  this->ScaleFactors->SetNumberOfTuples(ncomponents);
  for (int i=0;i<ncomponents;i++)
    {
      float sc;  fscanf(fin,"%f ",&sc);
      this->ScaleFactors->SetComponent(i,0,sc);
    }
  fgets(line,200,fin);
  strcpy(this->CategoryNames,line);

  // Allocate Image and Put Data in place
  vtkImageData* tmp=vtkImageData::New();
  tmp->SetNumberOfScalarComponents(ncomponents+1);
  tmp->SetDimensions(width,height,1); 
  int dimensions[3]; tmp->GetDimensions(dimensions);
  tmp->SetWholeExtent(0,dimensions[0]-1,0,dimensions[1]-1,0,dimensions[2]-1);
  tmp->SetExtent(0,dimensions[0]-1,0,dimensions[1]-1,0,dimensions[2]-1);
  tmp->SetUpdateExtent(0,dimensions[0]-1,0,dimensions[1]-1,0,dimensions[2]-1);
  tmp->SetOrigin(ori);
  tmp->SetSpacing(spa);
  tmp->SetScalarTypeToFloat();
  tmp->AllocateScalars();
  
  for (int c=0;c<=ncomponents;c++)
    tmp->GetPointData()->GetScalars()->FillComponent(c,0);

  //  fprintf(stderr,"Height=%d Width=%d\n",height,width);

  for (int voxel=0;voxel<nvoxels;voxel++)
    {
      int row,column; fscanf(fin,"%d %d ",&row,&column);
      //  fprintf(stderr,"voxel=%d row=%d column=%d ",voxel,row,column);
      row=row-1;
      column=column-1;
      //      fprintf(stderr," ---> row=%d column=%d ",row,column);

      if (flip[0]==1)
	column=(width-1)-column;

      if (flip[1]==1)
	row=(height-1)-row;
      
      if (flip[2]==1)
	{
	  int t=row;
	  row=column;
	  column=t;
	}

      //      fprintf(stderr,"--->  row=%d column=%d\n",row,column);

      tmp->SetScalarComponentFromDouble(column,row,0,0,1000.0);

      for (int i=0;i<ncomponents;i++)
	{
	  float v;  fscanf(fin,"%f ",&v);
	  tmp->SetScalarComponentFromDouble(column,row,0,i+1,v*this->ScaleFactors->GetComponent(i,0));
	}
    }



  vtkImageData* output=this->GetOutput();
  output->ShallowCopy(tmp);
  tmp->Delete();

  fclose(fin);
  delete [] line;
  this->Status=1;
}

