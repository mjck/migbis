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




/*=========================================================================*/
#include "vtkpxAnalyzeImageWriter.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkbisImageHeader.h"

int vtkpxAnalyzeImageWriter::ForceOutputFileFormat=0;
  //------------------------------------------------------------------------------
vtkpxAnalyzeImageWriter* vtkpxAnalyzeImageWriter::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxAnalyzeImageWriter");
  if(ret)
      {
	return (vtkpxAnalyzeImageWriter*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxAnalyzeImageWriter;
}
//------------------------------------------------------------------------------
vtkpxAnalyzeImageWriter::vtkpxAnalyzeImageWriter()
{
  this->Compression=0;
  this->ByteSwap=0;
  this->Orientation=0; // Axial
  this->SaveStatus=0;
  this->ImageHeader=NULL;

  // Messy Initialization for DLLS
  if (vtkpxAnalyzeImageWriter::ForceOutputFileFormat!=1 && vtkpxAnalyzeImageWriter::ForceOutputFileFormat!=2)
    vtkpxAnalyzeImageWriter::ForceOutputFileFormat=0;
}

vtkpxAnalyzeImageWriter::~vtkpxAnalyzeImageWriter()
{
  if (this->ImageHeader!=NULL)
    this->ImageHeader->Delete();
}

void vtkpxAnalyzeImageWriter::SetImageHeader(vtkbisImageHeader* header)
{
  if (header==NULL)
    return;

  if (this->ImageHeader==NULL)
    this->ImageHeader=vtkbisImageHeader::New();

  this->ImageHeader->CopyHeader(header);
}

void vtkpxAnalyzeImageWriter::Save(const char* fname)
{
  if (fname!=NULL)
    {
      this->SetFileName(fname);
      this->WriteData();
    }
}

//----------------------------------------------------------------------------
// From VTK vtkByteSwap originally, templated etc.
//------------------------------------------------------------------------------
template<class T>
void vtkpxAnalyzeImageWriterSwapBytes(T* dt,int slicesize,int sz)
{
  if (sz<1)
    return;

  char* chardata=(char*)dt;
  char one_byte=0;
  int   totalnumbytes=slicesize*sz;
  for (int i=0;i<totalnumbytes;i+=sz)
    {
      char* data=&chardata[i];
      switch(sz)
	{
	case 2:
	  one_byte = data[0]; data[0] = data[1]; data[1] = one_byte;
	  break;
	case 4:
	  one_byte = data[0]; data[0] = data[3]; data[3] = one_byte;
	  one_byte = data[1]; data[1] = data[2]; data[2] = one_byte; 
	  break;
	case 8:
	  one_byte = data[0]; data[0] = data[7]; data[7] = one_byte;
	  one_byte = data[1]; data[1] = data[6]; data[6] = one_byte;
	  one_byte = data[2]; data[2] = data[5]; data[5] = one_byte;
	  one_byte = data[3]; data[3] = data[4]; data[4] = one_byte; 
	  break;
	}
    }
}
// ----------------------------------------------------------------------------

// Templated Function to write out image
template<class T>
int vtkpxAnalyzeImageWriterWritePointer(vtkpxAnalyzeImageWriter* self,vtkImageData* img,
					znzFile fdf,T *)
{
  int dim[3];  img->GetDimensions(dim);

  //  fprintf(stderr,"Dimensions = %d,%d,%d\n",dim[0],dim[1],dim[2]);

  int slicesize=dim[0]*dim[1];
  T* temp = new T[slicesize];
  int maxnumcomp=img->GetNumberOfScalarComponents();



  vtkDataArray* dat=img->GetPointData()->GetScalars();
  T* dat_point=(T*)dat->GetVoidPointer(0);

  int bytes=0;
  int sz=sizeof(T);
  int writesize=sz*slicesize;

  //  fprintf(stderr,"Writing compressed=%d maxnumcomp=%d datatypesize=%d, slicesize=%d, type=%d\n",
  //	  compressed,maxnumcomp,sz,slicesize,dat->GetDataType());
  
  for (int comp=0;comp<maxnumcomp;comp++)
    for (int slice=0;slice<dim[2];slice++)
      {
	int index=0;
	for (int ib=0;ib<dim[1];ib++)
	  {
	    int offset=(slice*slicesize+ib*dim[0])*maxnumcomp+comp;
	    for (int ia=0;ia<dim[0];ia++)
	      {
		temp[index]=dat_point[offset];
		++index;
		offset+=maxnumcomp;
	      }
	  }
	if (self->GetByteSwap())
	  vtkpxAnalyzeImageWriterSwapBytes(temp,slicesize,sz);
	bytes += znzwrite(temp,1,writesize,fdf);
	//	fprintf(stderr,"Writing Slice %d (bytes=%d)\n",slice,bytes);
      }
  delete [] temp;
  return bytes;
}
//----------------------------------------------------------------------------
void vtkpxAnalyzeImageWriter::WriteData()
{
  this->SaveStatus=0;
  vtkImageData *input=this->GetInput(0);
  if (input==NULL)
    {
      vtkErrorMacro(<<"No Input Image To Save");
      return;
    }
  
  // Step 2 --- Create Header if it does not exist
  if (this->ImageHeader==NULL)
    this->CreateImageHeader(0);

  //  fprintf(stderr,"\n\n\n  Original Header mode=%d\n\n\n",this->ImageHeader->GetMode());
  //  this->ImageHeader->PrintHeader(0);

  // Fix Header for Dimensions/Spacing/Numframes/voxel sizes etc.
  // This is a last resort
  this->ImageHeader->ReconcileHeader(input,0);

  //  fprintf(stderr,"\n\n\n Reconciled Header mode=%d\n\n\n",this->ImageHeader->GetMode());
  //  this->ImageHeader->PrintHeader(1);


  if (vtkpxAnalyzeImageWriter::ForceOutputFileFormat==1 && this->ImageHeader->GetMode()!=0)
    {
      this->ImageHeader->SetModeToAnalyze();
    }
  else if (vtkpxAnalyzeImageWriter::ForceOutputFileFormat==2 && this->ImageHeader->GetMode()==0)
    {
      if (vtkbisImageHeader::FindExtension(this->FileName,".nii",".nii.gz")==1)
	this->ImageHeader->SetModeToNIFTI_NII();
      else
	this->ImageHeader->SetModeToNIFTI_HDRIMG();
    }
  else if (vtkbisImageHeader::FindExtension(this->FileName,".nii",".nii.gz")==1 && 
	   vtkpxAnalyzeImageWriter::ForceOutputFileFormat!=1 )
    {
      this->ImageHeader->SetModeToNIFTI_NII();
    }

  /*  if (this->ImageHeader->GetMode()==2 && this->Compression==0)
    {
      this->Compression=9;
      }*/




  if (this->ImageHeader->GetMode()==0)
    this->WriteDataAsAnalyze();
  else
    this->WriteDataAsNifty();

  //fprintf(stderr,"\n\n\n Final Header mode=%d\n\n\n",this->ImageHeader->GetMode());
  //  this->ImageHeader->PrintHeader(2);

}
//----------------------------------------------------------------------------
void vtkpxAnalyzeImageWriter::CreateImageHeader(int print)
{
  vtkbisImageHeader* newhead=vtkbisImageHeader::New();
  newhead->CreateHeader(this->GetInput(0),0);
  newhead->SetOrientation(this->Orientation);
  this->SetImageHeader(newhead);
  newhead->Delete();
  if (print)
    this->ImageHeader->PrintHeader(0);
}

//----------------------------------------------------------------------------
void vtkpxAnalyzeImageWriter::WriteDataAsAnalyze()
{
  char* headername=vtkbisImageHeader::CreateAnalyzeHDRFilename(this->FileName);

  

  int ok=this->ImageHeader->WriteAnalyzeHeader(headername);

  if (ok==0)
    {
      fprintf(stderr,"Failed to save analyze header in %s\n",headername);
      delete [] headername;
      return;
    }

  char* imgname=vtkbisImageHeader::CreateAnalyzeIMGFilename(headername);

  znzFile fdf=vtkbisImageHeader::SuffixOpen(imgname,"wb",this->Compression);

  switch (this->GetInput(0)->GetScalarType())
    {
      vtkTemplateMacro4(vtkpxAnalyzeImageWriterWritePointer, this, this->GetInput(0),
			fdf,static_cast<VTK_TT *>(0));
    default:
      vtkGenericWarningMacro("Unknown input ScalarType in Write Data");
      return;
    }
  
  znzclose(fdf);
  
  delete [] imgname;
  delete [] headername;
  this->SaveStatus=1;
}

void vtkpxAnalyzeImageWriter::WriteDataAsNifty()
{

  //  this->ImageHeader->PrintHeader(1);
  if (vtkbisImageHeader::FindExtension(this->FileName,".nii",".nii.gz")==1)
    this->ImageHeader->SetModeToNIFTI_NII();
  else
    this->ImageHeader->SetModeToNIFTI_HDRIMG();

  //  this->ImageHeader->SetModeToNIFTI_HDRIMG();
  
  // Check for hdr/img first
  znzFile fdf;
  if (this->ImageHeader->GetMode()==1)
    {
      char* headername=vtkbisImageHeader::CreateAnalyzeHDRFilename(this->FileName);
      int ok=this->ImageHeader->WriteNIFTIHeader(headername,0);
      if (ok==0)
	{
	  fprintf(stderr,"Failed to save NIFTI header in %s\n",headername);
	  delete [] headername;
	  return;
	}
      char* imgname=vtkbisImageHeader::CreateAnalyzeIMGFilename(headername);
      fdf=vtkbisImageHeader::SuffixOpen(imgname,"wb",this->Compression);
      delete [] imgname;
      delete [] headername;
    }
  else
    {
      fdf=vtkbisImageHeader::SuffixOpen(this->FileName,"wb",this->Compression);
      int ok=this->ImageHeader->WriteNIFTIHeader(fdf);
      if (ok==0)
	{
	  fprintf(stderr,"Failed to save NIFTI header in %s\n",this->FileName);
	  return;
	}
      this->ImageHeader->SetFileName(this->FileName);
    }

  //  this->ImageHeader->PrintHeader(1);

  //  fprintf(stderr,"On to saving image %d\n",this->GetInput(0));


  switch (this->GetInput(0)->GetScalarType())
    {
      vtkTemplateMacro4(vtkpxAnalyzeImageWriterWritePointer, this, this->GetInput(0),
			fdf,static_cast<VTK_TT *>(0));
    default:
      vtkGenericWarningMacro("Unknown input ScalarType in Write Data");
      return;
    }
  
  znzclose(fdf);
  this->SaveStatus=1;
  //  this->ImageHeader->PrintHeader(1);
}

int vtkpxAnalyzeImageWriter::SetForceOutputFileFormat(int mode)
{
  if (mode>=0 && mode <= 2)
    {
      vtkpxAnalyzeImageWriter::ForceOutputFileFormat=mode;
    }
  return vtkpxAnalyzeImageWriter::ForceOutputFileFormat;
}

int vtkpxAnalyzeImageWriter::GetForceOutputFileFormat()
{
  return vtkpxAnalyzeImageWriter::ForceOutputFileFormat;
}

