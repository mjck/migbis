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





#include "vtkafBVImageWriter.h"
#include "vtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkpxUtil.h"
#include "vtkImageImport.h"
#include "pxutil.h"
#include <assert.h>


//------------------------------------------------------------------------------
vtkafBVImageWriter* vtkafBVImageWriter::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkafBVImageWriter");
  if(ret)
      {
	return (vtkafBVImageWriter*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkafBVImageWriter;
}
//------------------------------------------------------------------------------
vtkafBVImageWriter::vtkafBVImageWriter()
{
  this->NumFrames=1;
 

  this->resolution=3;
  this->XStart=57;
  this->XEnd=231;
  this->YStart=52;
  this->YEnd=172;
  this->ZStart=59;
  this->ZEnd=197;
  this->ByteSwap=0;
  this->Orientation=0; // Axial
  this->MultipleComponentsAsFrames=1;
  
}
//----------------------------------------------------------------------------
char * vtkafBVImageWriter::GetFilename(char *which)
{
  if (which == "prt" || which == "PRT")
    return this->PRTfilename;
  //if (which == "fmr" || which == "FMR")
    return this->FMRfilename;
}


//----------------------------------------------------------------------------
void vtkafBVImageWriter::SetFilenames(const char *fmr,const char* prt)
{
  this->FMRfilename = new char[strlen (fmr)+1];
  assert(this->FMRfilename !=0);
  
  this->PRTfilename = new char[strlen (prt)+1];
  assert(this->PRTfilename !=0);
  

  //strcpy(this->FMRfilename,fmr);

  strncpy(this->FMRfilename,fmr,strlen(fmr)+1);
  strncpy(this->PRTfilename,prt,strlen(prt)+1);
   cout << "SET FILENAME" << this->FMRfilename <<endl;
  cout << "SET FILENAME" << fmr <<endl;
 
  //strcat(this->FMRfilename,'\0');
 
  //strcat(this->PRTfilename,'\0');
  
}

//----------------------------------------------------------------------------
void vtkafBVImageWriter::WriteData()
{
  vtkImageData *input=this->GetInput(0);

 cout << "DEBUG- WRITE DATA" <<endl;

if (input==NULL)
      {
	vtkErrorMacro(<<"No Input Image To Save");
	return;
      }
  
 

  int dim[3];  

  input->GetDimensions(dim);

 
  int maxnumcomp=1;
 
  if (this->NumFrames==1 && this->MultipleComponentsAsFrames==1)
    {
      maxnumcomp=input->GetNumberOfScalarComponents();
 
    }
  
  //int bits=16;

  
  // Find Min and Max Values 

  vtkDataArray* int_scalars=input->GetPointData()->GetScalars();

  

  char* fname=this->FileName;
  
  int sz=dim[0]*dim[1];
  long bytes=0;

  void *fdf;

  fdf=(FILE *)fopen(fname,"wb"); 
  

 short int *headerinfo1= new short int[8];
 float *headerinfo2= new float[2];
 short int *headerinfo3= new short int[1];
 
 this->NumFrames=input->GetNumberOfScalarComponents();
 short int version=(short int)2;
 //this->NrOfVolumes = this->NumFrames/this->slices;
 headerinfo1[3-3]=(short int)this->NumFrames; //NrOfVolumes;
 headerinfo1[4-3]=(short int)this->resolution;
 headerinfo1[5-3]=(short int)this->XStart;
 headerinfo1[6-3]=(short int)this->XEnd;
 headerinfo1[7-3]=(short int)this->YStart;
 headerinfo1[8-3]=(short int)this->YEnd;
 headerinfo1[9-3]=(short int)this->ZStart;
 headerinfo1[10-3]=(short int)this->ZEnd;
 headerinfo1[11-3]=(short int)this->hemodynamicDelay;

 headerinfo2[0]=(short int)this->TR;
 headerinfo2[1]=(short int)this->deltaParam;
 headerinfo2[2]=(short int)this->tauParam;
 headerinfo3[0]=(short int)this->segment_sz;
 headerinfo3[1]=(short int)this->segment_offset;

 bytes += fwrite(&version,1,2,(FILE *)fdf);
 //cout << "size " << sizeof(this->FMRfilename) <<endl;
 //cout << "size " << sizeof(this->PRTfilename) <<endl;
 bytes += fwrite(this->FMRfilename,1,strlen(this->FMRfilename)+1,(FILE *)fdf);

 bytes += fwrite(this->PRTfilename,1,strlen(this->PRTfilename)+1,(FILE *)fdf);

 bytes += fwrite(&headerinfo1[0],1,sizeof(short int)*9,(FILE *)fdf);
 bytes += fwrite(&headerinfo2[0],1,sizeof(float)*3,(FILE *)fdf);
 bytes += fwrite(&headerinfo3[0],1,sizeof(short int)*2,(FILE *)fdf);

 cout << "BYTES WROTE  HEADER : " << bytes <<endl;
 
 cout << input->GetNumberOfScalarComponents() <<endl;
 cout << "DIMENSIONS" << dim[0] <<dim[1] <<dim[2] << " " << NumFrames << endl;
 
 int total_voxels=dim[1]*dim[0]*dim[2];
 //WRITE THE DATA!
 if (fdf)
    {
      
      for (int voxel=0;voxel<total_voxels;voxel++)
	{
	  unsigned short* temp = new unsigned short[sz];
	  for (int comp=0;comp<this->NumFrames;comp++)
	    {
	      temp[0]=(unsigned short)int_scalars->GetComponent(voxel,comp);
	      bytes+=fwrite(&temp[0],1,2,(FILE *)fdf);
	      // cout << "within the loop" <<endl;
	      // int offset=voxel*sz;
	      //for (int k=0;k<sz;k++)
	      //temp[k]=(unsigned short)int_scalars->GetComponent(k+offset,comp);
	      
	      // bytes += fwrite(&temp[0],1,sz*2,(FILE *)fdf);
	    } 
	  delete [] temp;
	}
    }
 
 fclose((FILE *)fdf);
 

 cout << "DEBUG- end WRITE DATA- bytes wrote: " << bytes <<endl;
 delete [] fname;
  //delete hdr;

}

//==============================================================================================
void vtkafBVImageWriter::Save(const char* fname)
{ 
  cout << "WRITING...." <<endl;

  if (fname!=NULL)
      {
	 cout << "DEBUG- save" <<endl;
	this->SetFileName(fname);
	this->WriteData();
      }
}



