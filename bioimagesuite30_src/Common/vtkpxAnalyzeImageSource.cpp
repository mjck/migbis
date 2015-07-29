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


#include "vtkpxAnalyzeImageSource.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkbisImageHeader.h"
#include "vtkImagePermute.h"
#include "vtkErrorCode.h"
#include "nifti1.h"
#include "nifti1_io.h"
#include "bis_znzlib.h"
#include "vtkMatrix4x4.h"

#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkStreamingDemandDrivenPipeline.h"


//------------------------------------------------------------------------------
vtkpxAnalyzeImageSource* vtkpxAnalyzeImageSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxAnalyzeImageSource");
  if(ret)
    {
      return (vtkpxAnalyzeImageSource*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxAnalyzeImageSource;
}

// Construct object with NULL file prefix; file pattern "%s.%d"; image range 
// set to (1,1); data origin (0,0,0); data spacing (1,1,1); no data mask;
// header size 0; and byte swapping turned off.

vtkpxAnalyzeImageSource::vtkpxAnalyzeImageSource()
{
  this->FilePrefix = NULL;
  this->DataFlip[0] = this->DataFlip[1] = this->DataFlip[2]=0;
  this->Transpose=0;
  this->read_volume_ok=0;
  this->Orientation=0; // Axial
  this->IntShift=0.0;
  this->IntScale=1.0;
  this->ImageHeader=vtkbisImageHeader::New();
  this->ForceStandardOrientation=1;
  this->SetNumberOfInputPorts(0);

}


vtkpxAnalyzeImageSource::~vtkpxAnalyzeImageSource()
{
  if (this->FilePrefix)
    delete [] this->FilePrefix;

  if (this->ImageHeader)
    this->ImageHeader->Delete();
}

//------------------------------------------------------------------------------
// Specify the input data or filter.

// Read an analyze header file
// ---------------------------
int vtkpxAnalyzeImageSource::ReadHeaderFile()
{
  int ok=this->ImageHeader->ReadHeader(this->FilePrefix);
  if (ok==0)
    return 0;

  this->Orientation=this->ImageHeader->GetOrientation();
  this->DataFlip[0] = this->DataFlip[1] = this->DataFlip[2]=0; this->Transpose=0;
  this->IntShift=0.0;  this->IntScale=1.0;
  this->read_volume_ok=1;
  //  this->ImageHeader->PrintHeader();
  return 1;

}
//------------------------------------------------------------------------------
// From VTK vtkByteSwap originally, templated etc.
//------------------------------------------------------------------------------
template<class T>
void vtkpxAnalyzeImageSourceSwapBytes(T* dt,int slicesize,int sz)
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
template<class T>
int vtkpxAnalyzeImageSourceReadSlices(vtkpxAnalyzeImageSource *self,
				      vtkImageData* output,
				      znzFile fp,
				      T*)
{

  vtkDataArray* dat=output->GetPointData()->GetScalars();
  T* dat_point=(T*)dat->GetVoidPointer(0);
  
  unsigned long sz=output->GetPointData()->GetScalars()->GetDataTypeSize();

  int numcomponents=dat->GetNumberOfComponents();
  int dim[3];output->GetDimensions(dim);
  int slicesize=dim[0]*dim[1];
  T* temp = new T[slicesize];
  double oldpog=0.0;

  int DataFlip[3]; self->GetDataFlip(DataFlip);
  int Transpose[3]; self->GetDataFlip(Transpose);
  //  double intScale=self->GetIntScale();
  //  double intShift=self->GetIntShift();
  //  fprintf(stderr,"\nUsing Data Flip=%d,%d,%d \n",DataFlip[0],DataFlip[1],DataFlip[2]);
  //  fprintf(stderr,"Data Size %d\n",dat->GetDataType());

  int sliceNumber=0;
  int totalslices=numcomponents*dim[2];
  for (int frame=0;frame<numcomponents;frame++)
    for (int slice = 0; slice < dim[2]; slice++) 
      {
	double pog=float(sliceNumber)/float(totalslices);
	if ( (pog-oldpog)>0.1)
	  {
	    self->UpdateProgress(pog);
	    oldpog=pog;
	  }
	//	fprintf(stderr,"Reading slice %d/%d frame %d/%d\n",slice,dim[2],frame,numcomponents);
	znzread(&temp[0],sz,slicesize,fp);
	
	if (self->GetImageHeader()->GetSwapBytes())
	  vtkpxAnalyzeImageSourceSwapBytes(temp,slicesize,sz);
	
	int index=0;
	for (int ib=0;ib<dim[1];ib++)
	  {
	    int offset=0;
	    if (DataFlip[2]==0)
	      offset+=slice*slicesize;
	    else
	      offset+=(dim[2]-1-slice)*slicesize;
	    
	    if (DataFlip[1]==0)
	      offset+=ib*dim[0];
	    else
	      offset+=(dim[1]-1-ib)*dim[0];
	    
	    offset=offset*numcomponents+frame;
	    
	    if (DataFlip[0]==1)
	      offset+=(dim[0]-1)*numcomponents;
	    
	    for (int ia=0;ia<dim[0];ia++)
	      {
		dat_point[offset]=(T)((double)temp[index]);// *intScale+intShift);
		if (DataFlip[0]==0)
		  offset+=numcomponents;
		else
		  offset-=numcomponents;
		++index;
	      }
	  }
	++sliceNumber;
      }
  
  delete [] temp;
  return 1;
}
//------------------------------------------------------------------------------
int vtkpxAnalyzeImageSource::ReadMetaData(vtkInformation *outInfo)
{
  this->SetErrorCode( vtkErrorCode::NoError );

  int ok=1;

  if (this->read_volume_ok==0)
    ok=this->ReadHeaderFile();

  if (ok==0)
    {
      vtkErrorMacro(<<"Bad Header!");
      this->SetErrorCode( vtkErrorCode::UnrecognizedFileTypeError );
      return 1;
    }


  int dim[3]; this->ImageHeader->GetDimensions(dim);
  int nc=this->ImageHeader->GetNumberOfComponents();
  double spa[3]; this->ImageHeader->GetSpacing(spa);
  int dt=this->ImageHeader->GetVTKDataType();


  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
	       0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  outInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_EXTENT(),
	       0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);

  outInfo->Set(vtkDataObject::SPACING(), spa, 3);


  double origin[3]={0.0,0.0,0.0};

  outInfo->Set(vtkDataObject::ORIGIN(), origin, 3);


  

  vtkDataObject::SetPointDataActiveScalarInfo( outInfo, 
					       dt,nc);
  return 1;
}

//------------------------------------------------------------------------------
int vtkpxAnalyzeImageSource::Load(const char *ana)
{
  this->SetFilePrefix(ana);
  int ok=this->ReadHeaderFile();
  if (ok!=0)
      this->Update();

  int dim[3]; this->GetOutput()->GetDimensions(dim);
  int total=dim[0]*dim[1]*dim[2];
  
  if (total>=1)
    return 1;
  return 0;
}
//------------------------------------------------------------------------------
int vtkpxAnalyzeImageSource::RequestData(vtkInformation *,
					 vtkInformationVector **,
					 vtkInformationVector *outputVector)
{

  if (this->FilePrefix == NULL) 
    {
      vtkErrorMacro(<< "FilePrefix is NULL");
      return 1;
    }
    
  if (this->read_volume_ok==0)
    {
      vtkDebugMacro(<<"Not updating");
      return 1;
    }
  
  if (!this->FilePrefix)
    return 1;
  
  char* filename;
  // Analyze Mode 0 = Binary, 1= Analyze, 2= NIFTY (.hdr,.img), 3=NIFTY(.nii)
  if (this->ImageHeader->GetMode()==0 || this->ImageHeader->GetMode()==1)
    {
      filename=vtkbisImageHeader::CreateAnalyzeIMGFilename(this->FilePrefix);
    }
  else
    {
      filename=new char[(strlen(this->FilePrefix))+1];
      strcpy(filename,this->FilePrefix);
    }
  
  //  fprintf(stderr,"Reading data from %s\n",filename);

  znzFile fp = vtkbisImageHeader::SuffixOpen(filename,"rb");
  
  if (znz_isnull(fp))
    {
      char* filename2=vtkbisImageHeader::CreateFilename(filename,".IMG",".img");
      fp=vtkbisImageHeader::SuffixOpen(filename2,"rb"); 
      //      fprintf(stderr,"Reading data from %s\n",filename2);
      delete [] filename2;
	
      if (znz_isnull(fp))
	{
	  vtkErrorMacro(<< "Cannot Open File " << filename );
	  delete [] filename;
	  this->SetErrorCode( vtkErrorCode::UnrecognizedFileTypeError );
	  return 1;
	}
    }

  this->UpdateProgress(0.0);
  float oldpog=0.0;

  int offsetposition=0;



  this->DataFlip[0] = this->DataFlip[1] = this->DataFlip[2]=0; this->Transpose=0;
  if (this->ImageHeader->GetMode()>0)
    {
      if (this->ForceStandardOrientation)
	{
	  //	  fprintf(stderr,"Forcing Orientation\n");
	  this->FlipNIFTIImageAndUpdateMatrixToBioImageSuiteCoordinates();
	}
    }

  if (this->ImageHeader->GetMode()==2)
    {
      znzseek (fp, this->ImageHeader->GetHeaderOffset(), 0);
    }

  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  this->SetErrorCode( vtkErrorCode::NoError );
  vtkStructuredPoints *output = vtkStructuredPoints::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // ImageSource superclass does not do this.
  output->ReleaseData();

  this->ImageHeader->SetImageStructureFromHeader(output);
  output->AllocateScalars();

  int d[3]; output->GetDimensions(d);

  switch (output->GetScalarType())
    {
      vtkTemplateMacro4(vtkpxAnalyzeImageSourceReadSlices, this, output,
			fp,static_cast<VTK_TT *>(0));
    default:
      vtkGenericWarningMacro("Unknown input ScalarType in Write Data");
      return 1;
    }

  // Look at Flips and modify tensor data if needed!
  // The only changes to the raw data have to do with DataFlip

  /*double spa[3];
  output->GetSpacing(spa);
  output->SetSpacing(spa[1],spa[0],spa[2]);
  fprintf(stderr,"Output spacing =%.2f,%.2f,%.2f\n",spa[0],spa[1],spa[2]);*/

  if (this->Transpose)
    {
      
      vtkImageData* tmp=vtkImageData::New();
      tmp->ShallowCopy(output);

      vtkImagePermute* perm=vtkImagePermute::New();
      perm->SetFilteredAxes(1,0,2);
      perm->SetInput(tmp);
      perm->Update();

      tmp->Delete();
      output->ShallowCopy(perm->GetOutput());
      output->SetOrigin(0.0,0.0,0.0);
    }



  /*  output->GetSpacing(spa);
  output->SetSpacing(spa[1],spa[0],spa[2]);
  fprintf(stderr,"Output spacing =%.2f,%.2f,%.2f\n",spa[0],spa[1],spa[2]);*/

  return 1;

}

// -----------------------------------------------------------------------------------
void vtkpxAnalyzeImageSource::FlipNIFTIImageAndUpdateMatrixToBioImageSuiteCoordinates()
{
  //  fprintf(stderr,"In Flip NIFTI\n");
  this->Orientation=this->ImageHeader->ForceHeaderToBioImageSuiteCoordinatesAndReturnFlips(this->DataFlip,this->Transpose);
  //  this->Orientation=this->ImageHeader->GetOrientation();
  //  fprintf(stderr,"Done %d,%d,%d tr=%d proent=%d was or=%d\n",this->DataFlip[0],this->DataFlip[1],this->DataFlip[2],this->Transpose,this->ImageHeader->GetOrientation(),ori);
  return;
}
/*

  // Step 1 : Establish actual rotation matrix

  nifti_1_header* hdr=this->ImageHeader->GetRawHeader();
  this->DataFlip[0] = this->DataFlip[1] = this->DataFlip[2]=0;
  this->Orientation=0;
    
  //   METHOD 1 (the "old" way, used only when qform_code = 0):
  if (hdr->qform_code == 0 && hdr->sform_code==0)
      return;


  // Method 2 and 3 Rely on First Creating a 4x4 Matrix
  
  mat44 original;
  if (hdr->qform_code==0)
    {
      for (int ia=0;ia<=3;ia++)
	{
	  original.m[0][ia]=hdr->srow_x[ia];
	  original.m[1][ia]=hdr->srow_y[ia];
	  original.m[2][ia]=hdr->srow_z[ia];
	  original.m[3][ia]=float(ia==3);
	}
    }
  else
    {
      original=nifti_quatern_to_mat44(hdr->quatern_b,hdr->quatern_c,hdr->quatern_d,
				      hdr->qoffset_x,hdr->qoffset_y,hdr->qoffset_z,
				      fabs(hdr->pixdim[1]),fabs(hdr->pixdim[2]),fabs(hdr->pixdim[3]),
				      hdr->pixdim[0]);
    }

  //  vtkbisImageHeader::NIFTI_print_mat44("original",original);

  int icod,jcod,kcod;  nifti_mat44_to_orientation(original,&icod,&jcod,&kcod);
  
  //  fprintf(stderr,"Orientation icod=%d (%s), jcod=%d (%s), kcod=%d (%s)\n",	  icod,nifti_orientation_string(icod),	  jcod,nifti_orientation_string(jcod),	  kcod,nifti_orientation_string(kcod));
  



  // Step 1 -- establish orientation -- this is the direction where z axis is at

  int needs_transpose=0;
  mat44 transpose_matrix;       vtkbisImageHeader::NIFTI_identity_mat44(&transpose_matrix);

  int newdim[3]; this->ImageHeader->GetDimensions(newdim);
  int dim[3];    this->ImageHeader->GetDimensions(dim);

  int do_transpose=0;
  
  for (int ia=0;ia<=3;ia++)
    for (int ib=0;ib<=3;ib++)

  if ( (kcod==NIFTI_I2S || kcod==NIFTI_S2I) && (icod==NIFTI_L2R || icod==NIFTI_R2L) )
    {
      // Axial: Z-axis along k!
      this->Orientation=10;
      if (icod==NIFTI_L2R)	this->DataFlip[0]=1;
      if (jcod==NIFTI_P2A)	this->DataFlip[1]=1;
      if (kcod==NIFTI_S2I)	this->DataFlip[2]=1;
    }
  else if ( (jcod==NIFTI_I2S || jcod == NIFTI_S2I) && (icod==NIFTI_L2R || icod==NIFTI_R2L))
    {
      // Coronal
      this->Orientation=20;
      if (icod==NIFTI_L2R)	this->DataFlip[0]=1;
      if (jcod==NIFTI_I2S)	this->DataFlip[1]=1;
      if (kcod==NIFTI_P2A)	this->DataFlip[2]=1;
    }
  else if ((jcod==NIFTI_I2S || jcod == NIFTI_S2I) && (kcod==NIFTI_L2R || kcod==NIFTI_R2L))
    {
      this->Orientation=30;
      if (icod==NIFTI_P2A)	this->DataFlip[0]=1;
      if (jcod==NIFTI_S2I)	this->DataFlip[1]=1;
      if (kcod==NIFTI_R2L)	this->DataFlip[2]=1;
    } 
  // From Here we are in non_standard orientations and need transpose and flip !!!
  else if ( (kcod==NIFTI_I2S || kcod==NIFTI_S2I) && (jcod==NIFTI_L2R || jcod==NIFTI_R2L) )
    {
      this->Orientation=10;
      // This is a transposed axial ....
      // Step 1 -- switch x and y
      transpose_matrix.m[0][0]=0;      transpose_matrix.m[0][1]=1;
      transpose_matrix.m[1][0]=1;      transpose_matrix.m[1][1]=0;
      newdim[1]=dim[0]; newdim[0]=dim[1];
      
      // Step 2 -- check coordinates as before
      if (jcod==NIFTI_L2R)	this->DataFlip[0]=1;
      if (icod==NIFTI_P2A)	this->DataFlip[1]=1;
      if (kcod==NIFTI_S2I)	this->DataFlip[2]=1;

      do_transpose=1;
    }
  else  if ( (jcod==NIFTI_I2S || jcod == NIFTI_S2I) && (kcod==NIFTI_L2R || kcod==NIFTI_R2L))
    {
      this->Orientation=20;
      // This is a transposed coronal ....
      // Step 1 -- switch x and z
      transpose_matrix.m[0][0]=0;      transpose_matrix.m[0][2]=1;
      transpose_matrix.m[2][0]=1;      transpose_matrix.m[2][2]=0;
      newdim[0]=dim[2]; newdim[2]=dim[0];
      
      // Step 2 -- check coordinates as before
      if (kcod==NIFTI_L2R)	this->DataFlip[0]=1;
      if (jcod==NIFTI_I2S)	this->DataFlip[1]=1;
      if (icod==NIFTI_P2A)	this->DataFlip[2]=1;

      do_transpose=1;
    }
  else  if ((kcod==NIFTI_I2S || kcod == NIFTI_S2I) && (jcod==NIFTI_L2R || jcod==NIFTI_R2L))
    {
      this->Orientation = 30;
      // This is a transposed sagital ....
      // Step 1 -- switch y and z
      transpose_matrix.m[1][1]=0;      transpose_matrix.m[1][2]=1;
      transpose_matrix.m[2][1]=1;      transpose_matrix.m[2][2]=0;
      newdim[1]=dim[2]; newdim[2]=dim[1];

      // Step 2 -- check coordinates as before
      if (icod==NIFTI_P2A)	this->DataFlip[0]=1;
      if (kcod==NIFTI_S2I)	this->DataFlip[1]=1;
      if (jcod==NIFTI_R2L)	this->DataFlip[2]=1;

      do_transpose=1;
    }
  else
    {
      fprintf(stderr,"Can't handle this orientation ... not sure what to do\n");
      fprintf(stderr,"Orientation icod=%d (%s), jcod=%d (%s), kcod=%d (%s)\n",
	      icod,nifti_orientation_string(icod),
	      jcod,nifti_orientation_string(jcod),
	      kcod,nifti_orientation_string(kcod));
      return;
    }

  //  fprintf(stderr,"On to computing flips etc, Orientation=%d (flips=%d,%d,%d) do_transpose=%d\n",
  //	  this->Orientation,this->DataFlip[0],this->DataFlip[1],this->DataFlip[2],do_transpose);

  // Now we have the data flips we need to figure out what to do
  // Original matrix is  xyz= orig * ijk
  // Need     new matrix xyz= new  * i'j'k'
  // Which is            ijk= (flip * transpose) i'j'k'
  //
  // new = orig * flip * transpose
  // Key is to find flip and transpose matrices !!!!!
  
  mat44 flip;  vtkbisImageHeader::NIFTI_identity_mat44(&flip);
  //  vtkbisImageHeader::NIFTI_print_mat44("flip matrix 1",flip);
  for (int ia=0;ia<=2;ia++)
    {
      if (this->DataFlip[ia]==1)
	{
	  flip.m[ia][ia]=-1.0;
	  flip.m[ia][3] = newdim[ia]-1;
	}
    }
  //  vtkbisImageHeader::NIFTI_print_mat44("flip matrix",flip);
  mat44 mapping=vtkbisImageHeader::NIFTI_mat44_mmul(flip,transpose_matrix);
  this->ImageHeader->ModifyOrientation(this->Orientation,mapping,this->DataFlip,do_transpose);
}
										     
*/









