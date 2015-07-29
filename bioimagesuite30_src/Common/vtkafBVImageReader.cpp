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





#include "vtkafBVImageReader.h"
#include "vtkUnsignedShortArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkShortArray.h"
#include "vtkFloatArray.h"
#include "vtkObjectFactory.h"
#include "pxutil.h"
#include "vtkpxImageComponentsToFrames.h"
#include "vtkImageToStructuredPoints.h"
#include "vtkStructuredPoints.h"
#include "vtkPointData.h"
//------------------------------------------------------------------------------
vtkafBVImageReader* vtkafBVImageReader::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkafBVImageReader");
  if(ret)
    {
      return (vtkafBVImageReader*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkafBVImageReader;
}

// Construct object with NULL file prefix; file pattern "%s.%d"; image range 
// set to (1,1); data origin (0,0,0); data spacing (1,1,1); no data mask;
// header size 0; and byte swapping turned off.
//------------------------------------------------------------------------------
vtkafBVImageReader::vtkafBVImageReader()
{
  this->DataDimensions[0] = this->DataDimensions[1] = this->DataDimensions[2] =1;
  this->DataOrigin[0] = this->DataOrigin[1] = this->DataOrigin[2] = 0.0;
  this->DataSpacing[0] = this->DataSpacing[1] = this->DataSpacing[2] = 1.0;
  this->NumTimeFrames=1;
  
  this->HeaderSize = 0;
  this->FileType=2;
  
  this->read_volume_ok=0;
  

  this->Orientation=0; // Axial
  this->ForceOutputToShort=0;
}
//------------------------------------------------------------------------------
// Read the header information
// ---------------------------
int vtkafBVImageReader::ReadHeaderInfo()
{
 void *fdf;

  
 int bytes =0; 
 short int *headerinfo= new short int[14];
 float *headerinfo1= new float[2];
 
 fdf=(FILE *)fopen(this->Filename,"rb"); 

 
 bytes+=fread(&version,1,sizeof(short int),(FILE *)fdf);
 
 
 cout << "version: " << this->version <<endl;
 
 
 char temp[]=" ";

 int counter=0;
 char *filename = new char[100];

 bytes+=fread(&temp[0],1,sizeof(char),(FILE *)fdf); 
 filename[counter]=temp[0];
 
 
 while( true)
    {
      counter++; 
      bytes+=fread(&temp[0],1,sizeof(char),(FILE *)fdf); 
      
      filename[counter]=temp[0];
   
     
      if (counter > 3 && temp[0]== '\0') 
      	break;
      
     
   }
 this->FMRfilename = new char [counter];
 
 strncpy(this->FMRfilename, filename,counter);
 this->FMRfilename[counter]='\0';
 cout <<this->FMRfilename  <<" " << strlen(this->FMRfilename)+1 <<endl;
 

 counter=0;
 bytes+=fread(&temp[0],1,sizeof(char),(FILE *)fdf); 
 filename[counter]=temp[0];

   
   while(true )
   {
      counter++; 
      bytes+=fread(&temp[0],1,sizeof(char),(FILE *)fdf); 
      filename[counter]=temp[0];
      if (counter > 3 && temp[0] == '\0') 
	{ 
	  break;
	}
   }

 this->PRTfilename= new char [counter];
 
 strncpy(this->PRTfilename, filename,counter);
 this->PRTfilename[counter]='\0';


 cout <<this->PRTfilename << "  " << strlen(this->PRTfilename)+1 <<endl;


 bytes+=fread(&headerinfo[1],1,sizeof(short int)*9,(FILE *)fdf); 
 bytes+=fread(&headerinfo1[0],1,sizeof(float)*3,(FILE *)fdf); 
 bytes+=fread(&headerinfo[10],1,sizeof(short int)*2,(FILE *)fdf); 

 this->NumTimeFrames=headerinfo[1];
 this->NrOfVolumes=headerinfo[1];
 this->resolution=headerinfo[2];
 this->XStart=headerinfo[3];
 this->XEnd=headerinfo[4];
 this->YStart=headerinfo[5];
 this->YEnd=headerinfo[6];
 this->ZStart=headerinfo[7]; 
 this->ZEnd=headerinfo[8];
 this->hemodynamicDelay=headerinfo[9];
 this->TR=headerinfo1[0];
 this->deltaParam=headerinfo1[1];
 this->tauParam=headerinfo1[2];
 this->segment_sz=headerinfo[10];
 this->segment_offset=headerinfo[11];

 this->HeaderSize=bytes;
 
 this->DataDimensions[0]= (this->XEnd - this->XStart)/this->resolution;
 this->DataDimensions[1]= (this->YEnd - this->YStart)/this->resolution;
 this->DataDimensions[2]= (this->ZEnd - this->ZStart)/this->resolution;

 this->DataSpacing[0]=this->resolution;
 this->DataSpacing[1]=this->resolution;
 this->DataSpacing[2]=this->resolution;
 
 for (int ii = 0;ii<12;ii++) {
   cout << ii << " " << headerinfo[ii] <<endl ;
 }

 for (int i = 0;i<3;i++) {
   cout << "float information " <<headerinfo1[i] <<endl ;
 }
 
 cout <<"total bytes in header file " <<bytes <<endl;
 fclose((FILE *)fdf);

 this->read_volume_ok=1;

 delete [] filename;
 delete [] headerinfo;
 
 delete [] headerinfo1;
 return bytes;
}


//------------------------------------------------------------------------------
int  vtkafBVImageReader::GetNumTimeFrames()
{
  return this->NumTimeFrames;
}
//------------------------------------------------------------------------------
vtkDataArray* vtkafBVImageReader::ReadVolume()
{ 
  cout << "Reading VOlume" << endl;

  if (this->read_volume_ok==0)
    {
      vtkDebugMacro(<<"Not updating");
      cout << "NOT UPDATING! "<< endl;
      return NULL;      
    }
  
  if (!this->Filename)
    return NULL;
  
  char* filename=new char[(strlen(this->Filename))+1];
  strcpy(filename,this->Filename);
  
  int numPts = this->DataDimensions[0] * this->DataDimensions[1] * this->DataDimensions[2];
  
  vtkDataArray *scalars = NULL;
  
  if (FileType==2)
    {
      scalars = vtkDataArray::CreateDataArray(VTK_SHORT);
      scalars->SetNumberOfComponents(NumTimeFrames);
      scalars->SetNumberOfTuples(numPts);
    }
  
  
  gzFile fp = gzsuffixopen(filename,"rb");
  
  if ( fp==Z_NULL)
      {
	vtkErrorMacro(<< "Cannot Open File " << filename );
	
	return NULL;
      }

  this->UpdateProgress(0.0);
  //  float oldpog=0.0;

  
  //if (skip>0)
    gzseek (fp,this->HeaderSize, 0);

  short test;

  for (int voxel=0;voxel<numPts;voxel++)
    for (int frame=0;frame<NumTimeFrames;frame++)
      {
	gzread(fp,&test,2);
	scalars->SetComponent(voxel,frame,test);
      }

  gzclose (fp);
  delete [] filename;
  
  this->UpdateProgress(1.0);
  this->read_volume_ok=0;
  return scalars;
}
//------------------------------------------------------------------------------
void vtkafBVImageReader::Execute()
{

  if (this->Filename == NULL) 
    {
      vtkErrorMacro(<< "FilePrefix is NULL");
      return;
    }
  
  
  if (this->HeaderSize < 0) 
    {
      vtkErrorMacro(<< "HeaderSize " << this->HeaderSize << " must be >= 0");
      return;
    }
  
  int* dim = this->DataDimensions;
  
  if (dim[0] <= 0 || dim[1] <= 0) 
    {
      vtkErrorMacro(<< "x, y dimensions " << dim[0] << ", " << dim[1] 
		    << "must be greater than 0.");    
      return;
    } 
  
  vtkDataArray* tmpScalars = this->ReadVolume();
  vtkDataArray* newScalars=tmpScalars;
  int deletenewscalars=0;
 

  if (!tmpScalars)
    {
      cout << "RETURNING" << endl;
      return;
    }

  if (this->ForceOutputToShort==1)
    {
      if (newScalars->GetDataType()==VTK_FLOAT)
	{
	  newScalars=vtkDataArray::CreateDataArray(VTK_SHORT);
	  newScalars->DeepCopy(tmpScalars);
	  deletenewscalars=1;
	}
    }

  vtkStructuredPoints* output=this->GetOutput();
  if (!output) {
    cout << "output is null" <<endl;
    return;
  }
  int dimensions[3]; 
  dimensions[0] = this->DataDimensions[0];
  dimensions[1] = this->DataDimensions[1];
  dimensions[2] = this->DataDimensions[2];
  output->SetDimensions(dimensions);
  output->SetWholeExtent(0,dimensions[0]-1,0,dimensions[1]-1,0,dimensions[2]-1);
  output->SetExtent(0,dimensions[0]-1,0,dimensions[1]-1,0,dimensions[2]-1);
  
  output->SetSpacing((double*)this->DataSpacing);
  output->SetOrigin((double*)this->DataOrigin);

  output->SetScalarType(newScalars->GetDataType());
  output->SetNumberOfScalarComponents(newScalars->GetNumberOfComponents());
  cout << " num of components " << newScalars->GetNumberOfComponents() <<endl;
  output->GetPointData()->SetScalars(newScalars);
  
  if (deletenewscalars)
    newScalars->Delete();

  tmpScalars->Delete();

  //  int d[3];
  cout << "END OF EXECUTE!!!" <<endl;
}
//------------------------------------------------------------------------------
void vtkafBVImageReader::ExecuteInformation()
{
 
  vtkStructuredPoints *output = this->GetOutput();
  
  int dim[3];
  dim[0]=this->DataDimensions[0];
  dim[1]=this->DataDimensions[1];
  dim[2]=this->DataDimensions[2];
  output->SetWholeExtent(0, dim[0]-1, 0, dim[1]-1, 0, dim[2]-1);
  output->SetExtent(0, dim[0]-1, 0, dim[1]-1, 0, dim[2]-1);
  output->SetNumberOfScalarComponents(NumTimeFrames);

  if (this->FileType!=4)
    output->SetScalarType(VTK_SHORT);
  else
    output->SetScalarType(VTK_FLOAT);
  

}

//------------------------------------------------------------------------------
int vtkafBVImageReader::Load(const char *file)
{
 
  cout <<"LOADING....." << file <<endl;

  
  this->Filename=new char[strlen(file) +1];
  strcpy(this->Filename,file);
  int ok=this->ReadHeaderInfo();
 
  if (ok!=0){
    
    this->Update();
      cout << "DEBUG POINT - after update" << endl;
  }
  
  
  return ok;
 
  
}
//------------------------------------------------------------------------------

