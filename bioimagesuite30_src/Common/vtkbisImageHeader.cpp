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



/* =========================================================================*/

#include "vtkbisImageHeader.h"
#include "vtkbisNIFTIExtensionsList.h"
#include "vtkbisNIFTIExtension.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkImageShiftScale.h"
#include "vtkXMLDataElement.h"
#include "vtkMath.h"
#include "vtkMatrix4x4.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkTransform.h"
#include <vtkInformation.h>
#include "vtkImageReslice.h"
#include "vtkpxUtil.h"
#include "pxutil.h"

#include <string>

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
vtkbisImageHeader* vtkbisImageHeader::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisImageHeader");
  if(ret)
    {
      return (vtkbisImageHeader*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisImageHeader;
}
// ----------------------------------------------------
vtkbisImageHeader::vtkbisImageHeader()
{
  this->Header=new nifti_1_header;
  this->Extensions=vtkbisNIFTIExtensionsList::New();
  this->FileName=new char[500];
  strcpy(this->FileName,"");
  this->ResetFields();
  this->Buffer="";
}
// ----------------------------------------------------
vtkbisImageHeader::~vtkbisImageHeader()
{
  if (this->Header)
    delete this->Header;

  if (this->FileName)
    delete [] this->FileName;

  this->Extensions->Delete();
}
// ----------------------------------------------------
int vtkbisImageHeader::ResetFields()
{
  this->Extensions->Cleanup();
  this->Mode=0;
  strcpy(FileName,"none.hdr");
  // Description:
  // Other Flags
  this->IsHeaderValid=0;
  this->HeaderOffset=0;
  this->SwapBytes=0;
  this->Orientation=0;
  this->OriginBuffer[0]=  this->OriginBuffer[1]=  this->OriginBuffer[2]=0.0;

  vtkbisImageHeader::InitializeHeader(this->Header);
  return 1;
}


int vtkbisImageHeader::CopyHeader(const nifti_1_header* input,nifti_1_header* output)
{
  if (input==NULL || output==NULL)
    return 0;
  memcpy(output, input, sizeof(nifti_1_header));
  return 1;
}


int vtkbisImageHeader::InitializeHeader(nifti_1_header* header)
{
  if (header==NULL)
    return 0;

  // NIFTI1 -- Initialize
  header->sizeof_hdr=348;;          /*!< MUST be 348           */  /* int sizeof_hdr;      */
  strcpy(header->data_type,"");     /*!< ++UNUSED++            */  /* char data_type[10];  */
  strcpy(header->db_name,"none");   /*!< ++UNUSED++            */  /* char db_name[18];    */
  header->extents=0;        /*!< ++UNUSED++            */  /* int extents;         */
  header->session_error=0; /*!< ++UNUSED++            */  /* short session_error; */
  header->regular=0;       /*!< ++UNUSED++            */  /* char regular;        */
  header->dim_info=0;      /*!< MRI slice ordering.   */  /* char hkey_un0;       */

  /*--- was image_dimension substruct ---*/
  /*!< Data array dimensions.*/  /* short dim[8];        */
  for (int ia=0;ia<8;ia++) header->dim[ia]=0;

  header->intent_p1=0 ;    /*!< 1st intent parameter. */  /* short unused8;       */
  /* short unused9;       */
  header->intent_p2=0 ;    /*!< 2nd intent parameter. */  /* short unused10;      */
  /* short unused11;      */
  header->intent_p3=0 ;    /*!< 3rd intent parameter. */  /* short unused12;      */
  /* short unused13;      */
  header->intent_code=0 ;  /*!< NIFTI_INTENT_* code.  */  /* short unused14;      */
  header->datatype=4;      /*!< Defines data type!    */  /* short datatype;      */
  header->bitpix=16;        /*!< Number bits/voxel.    */  /* short bitpix;        */
  header->slice_start=0;   /*!< First slice index.    */  /* short dim_un0;       */

    /*!< Grid spacings.        */  /* float pixdim[8];     */
  for (int ia=0;ia<8;ia++) header->pixdim[ia]=1.0;

  header->vox_offset=0.0;    /*!< Offset into .nii file */  /* float vox_offset;    */
  header->scl_slope=1.0;    /*!< Data scaling: slope.  */  /* float funused1;      */
  header->scl_inter=0.0;    /*!< Data scaling: offset. */  /* float funused2;      */
  header->slice_end=0;     /*!< Last slice index.     */  /* float funused3;      */
  header->slice_code=0;   /*!< Slice timing order.   */
  /*!< Units of pixdim[1..4] */
  header->xyzt_units=NIFTI_UNITS_MM;
  header->cal_max=255;       /*!< Max display intensity */  /* float cal_max;       */
  header->cal_min=0;       /*!< Min display intensity */  /* float cal_min;       */
  header->slice_duration=1.0;/*!< Time for 1 slice.     */  /* float compressed;    */
  header->toffset=0.0;       /*!< Time axis shift.      */  /* float verified;      */
  header->glmax=255;         /*!< ++UNUSED++            */  /* int glmax;           */
  header->glmin=0;         /*!< ++UNUSED++            */  /* int glmin;           */
  /*--- was data_history substruct ---*/
  strcpy(header->descrip,"default bisImageHeader");   /*!< any text you like.    */  /* char descrip[80];    */
  strcpy(header->aux_file,"none");  /*!< auxiliary filename.   */  /* char aux_file[24];   */

  header->qform_code=0 ;   /*!< NIFTI_XFORM_* code.   */  /*-- all ANALYZE 7.5 ---*/
  header->sform_code=1 ;   /*!< NIFTI_XFORM_* code.   */  /*   fields below here  */
  /*   are replaced       */
  header->quatern_b=0.0 ;    /*!< Quaternion b param.   */
  header->quatern_c=0.0 ;    /*!< Quaternion c param.   */
  header->quatern_d=0.0 ;    /*!< Quaternion d param.   */
  header->qoffset_x=0.0 ;    /*!< Quaternion x shift.   */
  header->qoffset_y=0.0 ;    /*!< Quaternion y shift.   */
  header->qoffset_z=0.0 ;    /*!< Quaternion z shift.   */

  /*!< 1st row affine transform.   */
  header->srow_x[0]=-1.0;  header->srow_x[1]= 0.0;  header->srow_x[2]= 0.0;  header->srow_x[3]= 0.0;
  /*!< 2nd row affine transform.   */
  header->srow_y[0]= 0.0;  header->srow_y[1]=-1.0;  header->srow_y[2]= 0.0;  header->srow_y[3]= 0.0;
  /*!< 3rd row affine transform.   */
  header->srow_z[0]= 0.0;  header->srow_z[1]= 0.0;  header->srow_z[2]= 1.0;  header->srow_z[3]= 0.0;

  header->intent_name[0]=(char)0;
  strcpy(header->magic,"ni1\0");
  return 1;
}

// -------------------------------------------------------------------------------------
double vtkbisImageHeader::GetIntensityScale()
{
  nifti_1_header *hdr=this->Header;
  return hdr->scl_slope;
}
// -------------------------------------------------------------------------------------
double vtkbisImageHeader::GetIntensityOffset()
{
  nifti_1_header *hdr=this->Header;
  return hdr->scl_inter;

}

// -------------------------------------------------------------------------------------
void vtkbisImageHeader::GetDimensions(int dim[3])
{
  for (int ia=0;ia<=2;ia++)
    dim[ia]=this->Header->dim[1+ia];
}

int vtkbisImageHeader::GetNumberOfComponents()
{
  return this->Header->dim[4];
}

void vtkbisImageHeader::GetSpacing(double spa[3])
{
  int flag=0;
  if (this->Header->qform_code==0 && this->Header->sform_code>0) 
    {
      //      fprintf(stderr,"\t\t\t\t Incoming q_spacing spacing header=(%.2f,%.2f,%.2f)\n",
      //	      this->Header->pixdim[1],this->Header->pixdim[2],this->Header->pixdim[3]);
  
      for (int ia=0;ia<=2;ia++)
	spa[ia]=fabs(this->Header->pixdim[ia+1]);
      
      mat44 mat;  NIFTI_identity_mat44(&mat);
      for (int ia=0;ia<=3;ia++)
	{
	  mat.m[0][ia]=this->Header->srow_x[ia];
	  mat.m[1][ia]=this->Header->srow_y[ia];
	  mat.m[2][ia]=this->Header->srow_z[ia];
	}

      float quatern_b,quatern_c,quatern_d,qoffset_x,qoffset_y,qoffset_z,pixdim[4];
      nifti_mat44_to_quatern(mat,
			     &quatern_b,&quatern_c,&quatern_d,
			     &qoffset_x,&qoffset_y,&qoffset_z,
			     &pixdim[1],&pixdim[2],&pixdim[3],
			     &pixdim[0]);
      for (int ia=0;ia<=3;ia++)
	this->Header->pixdim[ia]=pixdim[ia];
      flag=1;
    }

  for (int ia=0;ia<=2;ia++)
    spa[ia]=fabs(this->Header->pixdim[ia+1]);

  /*  if (flag)
    fprintf(stderr,"\t\t\t\t Post q_spacing spacing header=(%.2f,%.2f,%.2f)\n",
    spa[0],spa[1],spa[2]);*/



}


int vtkbisImageHeader::GetVTKDataType()
{
  if (this->IsHeaderValid==0)
    return VTK_SHORT;

  return vtkbisImageHeader::GetVTKDataType(this->Header->datatype);
  //return this->Header->datatype;
  
}
// -------------------------------------------------------------------------------------
int vtkbisImageHeader::GetNIFTIDataType(int vtkdatatype)
{
  switch(vtkdatatype)
    {
    case  (VTK_UNSIGNED_CHAR):
      return DT_UINT8;
      break;

    case (VTK_CHAR):
      return DT_INT8;
      break;

    case (VTK_UNSIGNED_SHORT):
      return DT_UINT16;
      break;

    case (VTK_SHORT):
      return DT_INT16;
      break;

    case (VTK_UNSIGNED_INT):
      return DT_UINT32;
      break;

    case (VTK_INT):
    return DT_INT32;
      break;

    case (VTK_FLOAT):
      return DT_FLOAT;
      break;

    case (VTK_DOUBLE):
      return DT_DOUBLE;
      break;
    }

  return DT_INT16;
}

int vtkbisImageHeader::GetBitPix(int vtkdatatype)
{
  switch(vtkdatatype)
    {
    case  (VTK_UNSIGNED_CHAR):
    case (VTK_CHAR):
      return 8;
      break;

    case (VTK_UNSIGNED_SHORT):
    case (VTK_SHORT):
      return 16;
      break;

    case (VTK_UNSIGNED_INT):
    case (VTK_INT):
    case (VTK_FLOAT):
      return 32;
      break;

    case (VTK_DOUBLE):
      return 64;
      break;
    }

  return 16;
}


int vtkbisImageHeader::GetVTKDataType(int niftidatatype)
{
  //	fprintf(stderr, "in GetVTKDataType %d\n", niftidatatype); 
  switch(niftidatatype)
    {
    case DT_UINT8:
      return (VTK_UNSIGNED_CHAR);
      break;

    case DT_INT8:
      return (VTK_CHAR);
      break;

    case DT_UINT16:
      return (VTK_UNSIGNED_SHORT);
      break;

    case DT_INT16:
      return (VTK_SHORT);
      break;

    case DT_UINT32:
      return (VTK_UNSIGNED_INT);
      break;

    case DT_INT32:
      return (VTK_INT);
      break;

    case DT_FLOAT:
      return (VTK_FLOAT);
      break;

    case DT_DOUBLE:
      return (VTK_DOUBLE);
      break;
    }

  return VTK_SHORT;
}
// -----------------------------------------------------------------------------------
int vtkbisImageHeader::GetIndexedDataType()
{
  int niftidatatype=this->Header->datatype;

  switch(niftidatatype)
    {
    case DT_UINT8:
      return 0;
      break;

    case DT_INT8:
      return 1;
      break;

    case DT_UINT16:
      return 2;
      break;

    case DT_INT16:
      return 3;
      break;

    case DT_UINT32:
      return 4;
      break;

    case DT_INT32:
      return 5;
      break;

    case DT_FLOAT:
      return 6;
      break;

    case DT_DOUBLE:
      return 7;
      break;
    }

  return 3;
}
// -----------------------------------------------------------------------------------
void vtkbisImageHeader::SetIndexedDataType(int indexedtype)
{
  switch(indexedtype)
    {
    case 0:
      this->Header->datatype=DT_UINT8;
      this->Header->bitpix=8;
      break;

    case 1:
      this->Header->datatype=DT_INT8;
      this->Header->bitpix=8;
      break;

    case 2:
      this->Header->datatype=DT_UINT16;
      this->Header->bitpix=16;
      break;


    case 4:
      this->Header->datatype=DT_UINT32;
      this->Header->bitpix=32;
      break;

    case 5:
      this->Header->datatype=DT_INT32;
      this->Header->bitpix=32;
      break;

    case 6:
      this->Header->datatype=DT_FLOAT;
      this->Header->bitpix=32;
      break;

    case 7:
      this->Header->datatype=DT_DOUBLE;
      this->Header->bitpix=64;
      break;

    case 3:
    default:
      this->Header->datatype=DT_INT16;
      this->Header->bitpix=16;
      break;
    }

}
// -----------------------------------------------------------------------------------
int vtkbisImageHeader::CreateHeader(vtkImageData* img,int useorigin)
{
  if (img==NULL)
    return 0;

  this->ResetFields();
  this->Mode=0;
  int dim[3]; img->GetDimensions(dim);
  double spa[3]; img->GetSpacing(spa);

  this->Header->dim[1]=dim[0];   this->Header->dim[2]=dim[1];   this->Header->dim[3]=dim[2];
  this->Header->dim[4]=img->GetNumberOfScalarComponents();
  this->Header->dim[0]=(img->GetNumberOfScalarComponents()>1);
  this->Header->datatype=vtkbisImageHeader::GetNIFTIDataType(img->GetScalarType());
  this->Header->bitpix=vtkbisImageHeader::GetBitPix(img->GetScalarType());

  this->Header->pixdim[1]=spa[0];      this->Header->srow_x[0]=-spa[0];
  this->Header->pixdim[2]=spa[1];      this->Header->srow_y[1]=-spa[1];
  this->Header->pixdim[3]=spa[2];      this->Header->srow_z[2]= spa[2];


  double range[2]; img->GetPointData()->GetScalars()->GetRange(range);
  this->Header->cal_min=range[0];
  this->Header->glmin=(int)range[0];
  this->Header->glmax=(int)range[1];

  this->Orientation=0;
  if (useorigin)
    this->SetOrigin(img->GetOrigin());

  this->Extensions->Cleanup();
  this->IsHeaderValid=1;
  //  this->PrintAll(0);
  //  this->PrintHeader();
  return 1;
}

// -----------------------------------------------------------------------------------
int vtkbisImageHeader::CopyHeader(vtkbisImageHeader* other)
{
  if (other==NULL)
    return 0;
  this->Mode=other->GetMode();
  this->SetFileName(other->GetFileName());
  vtkbisImageHeader::CopyHeader(other->GetRawHeader(),this->Header);
  this->IsHeaderValid=other->GetIsHeaderValid();
  this->HeaderOffset=other->GetHeaderOffset();
  this->SwapBytes=other->GetSwapBytes();
  this->Orientation=other->GetOrientation();
  this->Extensions->Copy(other->GetExtensions());
  return 1;
}
// -----------------------------------------------------------------------------------
int vtkbisImageHeader::CopyNIFTIHeader(const nifti_1_header* header)
{
  if (header==NULL)
    return 0;
  this->ResetFields();
  vtkbisImageHeader::CopyHeader(header,this->Header);
  this->Mode=1;
  this->Orientation=0;
  this->IsHeaderValid=1;
  this->SetFileName("");
  return 1;
}

int vtkbisImageHeader::StoreInNIFTIHeader(nifti_1_header* header)
{
  if (header==NULL)
    return 0;

  vtkbisImageHeader::CopyHeader(this->Header,header);
  return 1;
}

// -----------------------------------------------------------------------------------
int vtkbisImageHeader::AddComment(const char* comment)
{
  this->Extensions->AddExtension(comment,4);
  return 1;
}

// -----------------------------------------------------------------------------------
int vtkbisImageHeader::AddComment(const char* comment,int ecode)
{
  this->Extensions->AddExtension(comment,ecode);
  return 1;
}

// -----------------------------------------------------------------------------------
void vtkbisImageHeader::SetImageStructureFromHeader(vtkImageData* img)
{
  if (this->IsHeaderValid==0 || img==NULL)
  {
	  fprintf(stderr, "HeaderValid = %d and img null = %d \n", this->IsHeaderValid, (img==NULL)); 
    return;
  }

  img->SetOrigin(0.0,0.0,0.0);
  //fprintf(stderr, "in bisImageHeader %d %d %d\n", this->Header->dim[1], this->Header->dim[2], this->Header->dim[3]); 
  //fprintf(stderr, "in bisImageHeader %d %d\n", this->GetVTKDataType(), this->Header->datatype); 

  img->SetDimensions(this->Header->dim[1],this->Header->dim[2],this->Header->dim[3]);
  img->SetNumberOfScalarComponents(this->Header->dim[4]);
  img->SetSpacing(fabs(this->Header->pixdim[1]),
		  fabs(this->Header->pixdim[2]),
		  fabs(this->Header->pixdim[3]));
  img->SetScalarType(this->GetVTKDataType());
}

vtkImageData* vtkbisImageHeader::AllocateImageFromHeaderInfo()
{

  if (this->IsHeaderValid==0)
    return NULL;

  vtkImageData* img=vtkImageData::New();
  img->SetDimensions(this->Header->dim[1],this->Header->dim[2],this->Header->dim[3]);
  img->SetNumberOfScalarComponents(this->Header->dim[4]);
  img->SetSpacing(fabs(this->Header->pixdim[1]),
		  fabs(this->Header->pixdim[2]),
		  fabs(this->Header->pixdim[3]));
  img->SetScalarType(this->GetVTKDataType());
  img->AllocateScalars();

  for (int ia=0;ia<this->Header->dim[4];ia++)
    img->GetPointData()->GetScalars()->FillComponent(ia,0.0);
  return img;
}

// -----------------------------------------------------------------------------------
// Handling the Origin Flag from some Analyze Format Stuff ...........................
// -----------------------------------------------------------------------------------
double vtkbisImageHeader::GetOriginComponent(int a)
{
  if (a<0) a=0;
  if (a>2) a=2;
  double ori[3]; this->GetOrigin(ori);
  return ori[a];
}
void vtkbisImageHeader::GetOrigin(double origin[3])
{
  origin[0]=origin[1]=origin[2]=0.0;
  if (this->IsHeaderValid==0)
    return;

  if (this->Header->qform_code>0)
    {
      origin[0]=this->Header->qoffset_x;
      origin[1]=this->Header->qoffset_y;
      origin[2]=this->Header->qoffset_z;
      return;
    }

  if (this->Header->sform_code>0)
    {
      origin[0]=this->Header->srow_x[3];
      origin[1]=this->Header->srow_y[3];
      origin[2]=this->Header->srow_z[3];
    }

}

/*void double* vtkbisImageHeader::GetOrigin()
{
  this->GetOrigin(this->OriginBuffer);
  return &this->OriginBuffer[0];
  }*/

void vtkbisImageHeader::SetOrigin(double x,double y,double z)
{
  this->OriginBuffer[0]=x;
  this->OriginBuffer[1]=y;
  this->OriginBuffer[2]=z;

  if (this->Header->qform_code>0)
    {
      this->Header->qoffset_x=x;
      this->Header->qoffset_y=y;
      this->Header->qoffset_z=z;
      return;
    }

  this->Header->srow_x[3]=x;
  this->Header->srow_y[3]=y;
  this->Header->srow_z[3]=z;
  if (this->Header->sform_code==0)
    {
      this->Header->sform_code=1;
      this->Header->srow_x[0]=-1.0;  this->Header->srow_x[1]= 0.0;  this->Header->srow_x[2]= 0.0;
      this->Header->srow_y[0]= 0.0;  this->Header->srow_y[1]=-1.0;  this->Header->srow_y[2]= 0.0;
      this->Header->srow_z[0]= 0.0;  this->Header->srow_z[1]= 0.0;  this->Header->srow_z[2]= 1.0;
    }
}

void vtkbisImageHeader::SetOrigin(double ori[3])
{
  this->SetOrigin(ori[0],ori[1],ori[2]);
}

// -----------------------------------------------------------------------------------
// Handling the Origin Flag from some Analyze Format Stuff ...........................
// -----------------------------------------------------------------------------------
short vtkbisImageHeader::GetVOIValue()
{
  return (this->Header->extents);
}

void vtkbisImageHeader::SetVOIValue(short a)
{
  this->Header->extents=a;
}


char* vtkbisImageHeader::GetDBName()
{
  return this->Header->db_name;
}

void vtkbisImageHeader::SetDBName(const char* name)
{
  if (name!=NULL)
    strncpy(this->Header->db_name,name,17);
}
// -----------------------------------------------------------------------------------
int vtkbisImageHeader::FindExtension(const char* orig,const char* lookfor,const char* lookfor2)
{
  if (lookfor==NULL)
    return 0;


  int l_orig=strlen(orig);
  int l_lookfor=strlen(lookfor);

  //fprintf(stderr,"\n\n Looking for %s in %s (%s) \n",lookfor, orig,&orig[l_orig-l_lookfor]);

  if (l_lookfor>l_orig)
      return 0;

  if (strcmp(&orig[l_orig-l_lookfor],lookfor)==0)
    {
      //fprintf(stderr,"Found %s in %s\n",lookfor,orig);
      return 1;
    }

  if (lookfor2==NULL)
    return 0;

  int l_lookfor2=strlen(lookfor2);
  //fprintf(stderr,"\t\t Take 2 looking for %s in %s (%s)\n",lookfor2, orig,&orig[l_orig-l_lookfor2]);

  if (l_lookfor2>l_orig)
      return 0;


  //  fprintf(stderr,"Looking for %s in %s\n",lookfor2, &orig[l_orig-l_lookfor2]);


  if (strcmp(&orig[l_orig-l_lookfor2],lookfor2)==0)
    {
      //fprintf(stderr,"Found %s in %s\n",lookfor2,orig);
      return 1;
    }

  return 0;
}

// -----------------------------------------------------------------------------------
char* vtkbisImageHeader::EatSuffix(const char *orig_filename)
{
  int   len = (int)strlen(orig_filename);
  char* newname = new char[len+10];
  char* filename= new char[len +10];
  strcpy(newname,orig_filename);
  strcpy(filename,orig_filename);

  char* ext = strrchr(filename,'.');
  char* sla = strrchr(filename,'/');

  if (ext != NULL)
    {
      if ((sla == NULL) || (strlen(sla) >= strlen(ext)))
	{ /* file.ext or ../file.ext */
	  int lenext = (int)strlen(ext);
	  strncpy(newname,filename,len-lenext);
	  newname[len-lenext] = '\0';
	}
    }

  delete [] filename;
  return(newname);
}
// -----------------------------------------------------------------------------------
char* vtkbisImageHeader::CreateFilename(const char* orig_filename,const char* wanted,const char *unwanted)
{
  char* tmp,*hdrfile;
  char* filename=new char[strlen(orig_filename)+1 ];
  strcpy(filename,orig_filename);

  char* ext = strrchr(filename,'.');

  if (ext)
    {
      if ( (strcmp(ext,unwanted) == 0 ) ||  ( strcmp(ext,".") == 0 ) )
	{
	  tmp=vtkbisImageHeader::EatSuffix(filename);
	  hdrfile=new char[strlen(tmp)+strlen(wanted)+5];
	  sprintf(hdrfile,"%s%s",tmp,wanted);
	  delete [] tmp;
	  delete [] filename;
	  return hdrfile;
	}

      if (strcmp(ext,wanted) == 0)
	{
	  hdrfile=new char[strlen(filename)+5];
	  strcpy(hdrfile,filename);
	  delete [] filename;
	  return hdrfile;
	}

      hdrfile=new char[strlen(filename)+strlen(wanted)+5];
      sprintf(hdrfile,"%s%s",filename,wanted);
      delete [] filename;
      return hdrfile;
    }
  hdrfile=new char[strlen(filename)+strlen(wanted)+5];
  sprintf(hdrfile,"%s%s",filename,wanted);
  delete [] filename;
  return hdrfile;
}
// -----------------------------------------------------------------------------------
char* vtkbisImageHeader::CreateAnalyzeHDRFilename(const char* fname)
{
  char* temp=vtkbisImageHeader::CreateFilename(fname,"",".gz");
  char* temp1=vtkbisImageHeader::CreateFilename(temp,"",".HDR");
  char* temp2=vtkbisImageHeader::CreateFilename(temp1,".hdr",".img");
  delete [] temp;
  delete [] temp1;
  return temp2;
}

char* vtkbisImageHeader::CreateAnalyzeIMGFilename(const char* fname)
{
  char* temp=vtkbisImageHeader::CreateFilename(fname,"",".gz");
  char* temp2=vtkbisImageHeader::CreateFilename(temp,"",".HDR");
  char* temp3=vtkbisImageHeader::CreateFilename(temp2,".img",".hdr");

  delete [] temp;
  delete [] temp2;
  return temp3;

}


// -----------------------------------------------------------------------------------
znzFile vtkbisImageHeader::SuffixOpen(const char* path, const char* mode,int complevel)
{
  // Default

  if (mode==NULL)
    return Z_NULL;

  if (mode[0]=='r')
      {
	znzFile s=znzopen (path, mode,1);
	if (!znz_isnull(s))
	  return s;

	// Take 2 check for .gz suffix if missing
	char* newpath=new char[strlen(path)+5];
	sprintf(newpath,"%s.gz",path);
	s=znzopen(newpath,mode,1);
	delete [] newpath;
	return s;
      }

  // Write or append
  int foundgz=0;

  if (complevel<0)
    complevel=0;
  else if (complevel>9)
    complevel=9;

  if (complevel==0)
    {
      if (vtkbisImageHeader::FindExtension((char* )path,".gz")==1)
	{
	  complevel=6;
	  foundgz=1;
	}
    }

  if (complevel==0 || foundgz==1)
    {
      //  fprintf(stdout,"Opening %s (mode=%s) comp=%d\n",path,mode,complevel);
      znzFile fd=znzopen(path,mode,complevel);
      if (znz_isnull(fd))
	fprintf(stdout,"Failed to open %s\n",path);
      return fd;
    }

  char* newpath=vtkbisImageHeader::CreateFilename((char*)path,".gz",".gz");
  //  fprintf(stdout,"Opening %s (mode=%s) comp=%d\n",newpath,mode,complevel);
  znzFile s=znzopen(newpath,mode,complevel);
  delete [] newpath;
  if (znz_isnull(s))
    fprintf(stdout,"Failed to open %s\n",newpath);
  return s;
}


// -----------------------------------------------------------------------------------
int vtkbisImageHeader::ReadHeader(char* fname)
{
  int ok=this->ReadNIFTIHeader(fname);

  if (ok>0)
    return 1;

  //  fprintf(stderr,"Reading as Analyze\n");

  return this->ReadAnalyzeHeader(fname);
}
// -------------------------------------------------------------------------------------
int vtkbisImageHeader::WriteHeader(char* fname,int compression)
{

  if (this->Mode==0)
    return this->WriteAnalyzeHeader(fname);


  return this->WriteNIFTIHeader(fname,compression);
}
// ------------------------------------------------------------------------------------
int vtkbisImageHeader::SetModeToNIFTI_NII()
{
  this->Mode=2;
  double spa[3]; this->GetSpacing(spa);
  //  fprintf(stdout,"Converting to NIFTI (%.2f,%.2f,%.2f)\n",spa[0],spa[1],spa[2]);
  strncpy(this->Header->magic,"n+1\0",4);
  int total=this->Extensions->GetTotalByteSize();
  this->Header->vox_offset=352+total;
  //  fprintf(stdout,"Done Converting to NIFTI (%.2f,%.2f,%.2f)\n",spa[0],spa[1],spa[2]);
  return 1;
}
// ------------------------------------------------------------------------------------
int vtkbisImageHeader::SetModeToNIFTI_HDRIMG()
{
  this->Mode=1;
  this->Header->vox_offset=0;
  strncpy(this->Header->magic,"ni1\0",4);
  return 1;
}
// ------------------------------------------------------------------------------------
int vtkbisImageHeader::WriteNIFTIHeader(char* filename,int compression)
{
  if (filename==NULL)
    return 0;

  znzFile fdf=vtkbisImageHeader::SuffixOpen(filename,"wb",compression);

  if (znz_isnull(fdf))
    {
      fprintf(stdout,"Cannot write %s\n",filename);
      return 0;
    }

  int total=this->WriteNIFTIHeader(fdf);
  znzclose(fdf);
  if (total>0)
      this->SetFileName(filename);

  return total;
}

// -----------------------------------------------------------------------------------
int vtkbisImageHeader::WriteAnalyzeHeader(char* filename)
{
  //  fprintf(stderr,"Writing Analyze Header ... \n");
  //  fprintf(stderr,"Valid=%d\n",this->IsHeaderValid);

  if (filename==NULL)   return 0;
  if (this->IsHeaderValid==0) return 0;


  FILE* fp;
  if((fp=fopen(filename,"wb"))==0)
    {
      fprintf(stdout,"vtkbisImageHeader::WriteAnalyzeHeader: unable to create: %s\n",filename);
      return 0;
    }
  //  fprintf(stdout,"Diagnostics OK ... \n");

  struct dsr hdr;
  memset(&hdr,0,sizeof(struct dsr));
  hdr.hk.sizeof_hdr = sizeof(struct dsr);
  hdr.hk.extents = 16384;
  hdr.hk.regular = 'r';

  if (this->Header->dim[3]>1)
    hdr.dime.dim[0] = 4;
  else
    hdr.dime.dim[0] = 3;
  hdr.dime.dim[1] = this->Header->dim[1];
  hdr.dime.dim[2] = this->Header->dim[2];
  hdr.dime.dim[3] = this->Header->dim[3];
  hdr.dime.dim[4] = this->Header->dim[4];

  /*  fprintf(stdout,"Dimensions = %d x %d x %d x %d\n",
	  hdr.dime.dim[1],
	  hdr.dime.dim[2],
	  hdr.dime.dim[3],
	  hdr.dime.dim[4]);*/

  strcpy(hdr.dime.vox_units,"mm");
  strcpy(hdr.dime.cal_units," ");

  hdr.dime.datatype=this->Header->datatype;
  hdr.dime.bitpix=this->Header->bitpix;
  hdr.dime.cal_max = this->Header->cal_max;
  hdr.dime.cal_min = this->Header->cal_min;
  hdr.dime.glmax  =  this->Header->glmax;
  hdr.dime.glmin  =  this->Header->glmin;
  hdr.dime.pixdim[1] = this->Header->pixdim[1];
  hdr.dime.pixdim[2] = this->Header->pixdim[2];
  hdr.dime.pixdim[3] = this->Header->pixdim[3];
  hdr.dime.pixdim[4] = this->Header->pixdim[4];
  hdr.dime.vox_offset = 0.0;
  hdr.hist.orient = this->Orientation;

  hdr.dime.unused1=this->GetVOIValue();
  //  double ori[3]; this->GetOrigin(ori);
  hdr.dime.funused1=this->GetIntensityScale();
  hdr.dime.funused2=this->GetIntensityOffset();
  hdr.dime.funused3=0.0;
  strcpy(hdr.hist.exp_date,"");
  strcpy(hdr.hist.exp_time,"");
  strcpy(hdr.hist.patient_id,"");
  strcpy(hdr.hist.descrip,"created by vtkbisImageHeader");
  strncpy(hdr.hk.db_name,this->GetDBName(),17);
  fwrite(&hdr,sizeof(struct dsr),1,fp);
  fclose(fp);
  this->SetFileName(filename);

  return 348;
}
// -----------------------------------------------------------------------------------
int vtkbisImageHeader::ReadNIFTIHeader(char* filename)
{

  znzFile fd=vtkbisImageHeader::SuffixOpen(filename,"rb");
  int mode=0,swap=0;
  if (!znz_isnull(fd))
    {
      znzread(this->Header,sizeof(nifti_1_header),1,fd);
      znzclose(fd);
      //this->Header = nifti_read_header(filename,&swap,true);
      //      fprintf(stdout,"Setting NIFTI Read Filename to %s\n",filename);
      this->SetFileName(filename);
    }
  else
    {
      char *hdrfile= vtkbisImageHeader::CreateAnalyzeHDRFilename(filename);
      fd=vtkbisImageHeader::SuffixOpen(hdrfile,"rb");

      if (!znz_isnull(fd))
	{
	  znzread(this->Header,sizeof(nifti_1_header),1,fd);
	  znzclose(fd);
	  //	  fprintf(stdout,"Setting NIFTI Read Filename to %s\n",hdrfile);
	  //	  this->Header = nifti_read_header(hdrfile,&swap,true);
	}
      else
	{
	  char* hdrfile2=vtkbisImageHeader::CreateFilename(filename,".HDR",".hdr");
	  fd=vtkbisImageHeader::SuffixOpen(hdrfile2,"rb");
	  if (!znz_isnull(fd))
	    {
	      znzread(this->Header,sizeof(nifti_1_header),1,fd);
	      znzclose(fd);
	      //	      this->Header = nifti_read_header(hdrfile2,&swap,true);
	      //	      fprintf(stdout,"Setting NIFTI Read Filename to %s\n",hdrfile2);
	      this->SetFileName(hdrfile2);
	    }
	  delete [] hdrfile2;
	}
      delete [] hdrfile;
    }

  // Now some testing and optionally byte swapping
  this->SwapBytes=0;
  if (strcmp(this->Header->magic,"n+1\0")!=0 &&
      strcmp(this->Header->magic,"ni1\0")!=0)
    {
      //      fprintf(stdout,"Bad Magic String in Header \n");
      this->ResetFields();
      return 0;
    }
  //  fprintf(stdout,"Read NIFTI Header from %s (magic=%s)\n",this->FileName,this->Header->magic);
  //  fprintf(stdout,"Size of Header = %d, %d (%d)\n",sizeof(nifti_1_header),sizeof(int),this->Header->sizeof_hdr);

  // Now do swap if needed
  if (this->Header->sizeof_hdr>65536)
    {
      this->SwapBytes=1;
      swap_nifti_header(this->Header,1);
    }

  //  fprintf(stdout,"Size of Header = %d, %d (%d)\n",sizeof(nifti_1_header),sizeof(int),this->Header->sizeof_hdr);

  if (strncmp(this->Header->magic,"n+1",3)==0)
    {
      this->Mode=2;
    }
  else if (strncmp(this->Header->magic,"ni1",3)==0)
    {
      this->Mode=1;
    }
  else
    {
      //      fprintf(stdout,"Bad Magic Code %s\n",this->Header->magic);
      return 0;
    }

  if (this->Header->scl_slope==0.0)
    this->Header->scl_slope=1.0;


  //  fprintf(stdout,"Theoretical Spacing %f,%f,%f\n",this->Header->pixdim[1],this->Header->pixdim[2],this->Header->pixdim[3]);

  // If quaternion is bad create it while we are it, it is good to have set
  // especially the pixdim stuff
  if (this->Header->qform_code == 0 && this->Header->sform_code==0)
    {
      this->Header->srow_x[0]=-fabs(this->Header->pixdim[1]);
      this->Header->srow_y[1]=-fabs(this->Header->pixdim[2]);
      this->Header->srow_z[2]= fabs(this->Header->pixdim[3]);
    }

  // Next up
  // Compute the Orientation
  this->Orientation=this->GetOrientationFromNIFTIHeader();

  // Now read extensions
  // First Compute how many bytes of room are there
  this->HeaderOffset=(int)this->Header->vox_offset;
  int numbytesleft=this->HeaderOffset-348;
  if (this->Mode==1)
    {
      this->HeaderOffset=0;
      // go to the end
      //      fprintf(stdout,"Reading %s\n",this->FileName);
      znzFile ffd=vtkbisImageHeader::SuffixOpen(this->FileName,"rb");
      int n=1,count=0;
      char line[100];
      while (n>0)
	{
	  n=znzread(line,1,100,ffd);
	  count+=n;
	}
      numbytesleft=count-348;
      znzclose(ffd);
    }

  int numberofdimensions=this->Header->dim[0];

  if (this->Header->dim[0]>4)
    {
      int fourthdimension=1;
      for (int ia=4;ia<8;ia++)
	{
	  if (this->Header->dim[ia]>0)
	    fourthdimension=fourthdimension*this->Header->dim[ia];
	}
      this->Header->dim[4]=fourthdimension;
      for (int ia=5;ia<8;ia++)
	this->Header->dim[ia]=1;

      this->Header->dim[0]=4;
    }





  //  fprintf(stdout,"Reading %s bytesleft=%d\n",this->FileName,numbytesleft);
  //  fprintf(stdout,"Read NIFTI Header from %s (magic=%s) left=%d\n",this->FileName,this->Header->magic,numbytesleft);

  /// Now read the Extensions
  znzFile ffd=vtkbisImageHeader::SuffixOpen(this->FileName,"rb");
  znzseek (ffd, 348, 0);
  this->Extensions->ReadExtensions(ffd,numbytesleft);


  if (this->Header->dim[0]!=numberofdimensions)
    {
      char line[200];
      sprintf(line,"BIS::vtkbisImageHeader::Changed Image Dimension from %d to %d",numberofdimensions,this->Header->dim[0]);
      this->AddComment(line,0);
    }

  znzclose(ffd);

  // Set Headet to Valid
  this->IsHeaderValid=1;
  //  fprintf(stdout," Extensions Number of Bytes = %d, next=%d\n", this->Extensions->GetTotalByteSize(),this->Extensions->GetNumberOfExtensions());


  //  fprintf(stdout,"Setting NIFTI Header Filename to %s\n",filename);
  //  this->PrintHeader();

  return 1;
}

// --------------------------------------------------------------------------------
template<class T>
void vtkbisImageHeaderSwap(T* dt,int arraysize=1)
{
  int sz=sizeof(T);
  if (sz<2)
    return;

  char* chardata=(char*)dt;
  char one_byte=0;
  int   totalnumbytes=arraysize*sz;
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

// -----------------------------------------------------------------------
int vtkbisImageHeader::CreateVarianHeader(vtkXMLDataElement* element)
{
  if (element==NULL)
    return 0;
  
  this->ResetFields();
  
  // Step 1 .. byte size
  std::string dtp = element->GetAttribute("storage");
  if (dtp=="float")
      this->Header->datatype = DT_FLOAT;
  else
    this->Header->datatype = DT_INT16; 



  this->Header->dim[0]=3;
  this->Header->dim[1]=atoi(element->GetAttribute("bis_Dimension0"));
  this->Header->dim[2]=atoi(element->GetAttribute("bis_Dimension1"));
  this->Header->dim[3]=atoi(element->GetAttribute("bis_Dimension2"));

  /*  this->Header->srow_x[0]=-atof(element->GetAttribute("bis_Orient0"));
  this->Header->srow_y[0]=-atof(element->GetAttribute("bis_Orient1"));
  this->Header->srow_z[0]=atof(element->GetAttribute("bis_Orient2"));
  this->Header->srow_x[1]=-atof(element->GetAttribute("bis_Orient3"));
  this->Header->srow_y[1]=-atof(element->GetAttribute("bis_Orient4"));
  this->Header->srow_z[1]=atof(element->GetAttribute("bis_Orient5"));
  this->Header->srow_x[2]=-atof(element->GetAttribute("bis_Orient6"));
  this->Header->srow_y[2]=-atof(element->GetAttribute("bis_Orient7"));
  this->Header->srow_z[2]=atof(element->GetAttribute("bis_Orient8"));*/

  this->Header->srow_x[0]=-atof(element->GetAttribute("bis_Orient0"));
  this->Header->srow_x[1]=-atof(element->GetAttribute("bis_Orient3"));
  this->Header->srow_x[2]=-atof(element->GetAttribute("bis_Orient6"));

  this->Header->srow_y[0]=-atof(element->GetAttribute("bis_Orient1"));
  this->Header->srow_y[1]=-atof(element->GetAttribute("bis_Orient4"));
  this->Header->srow_y[2]=-atof(element->GetAttribute("bis_Orient7"));

  this->Header->srow_z[0]=atof(element->GetAttribute("bis_Orient2"));
  this->Header->srow_z[1]=atof(element->GetAttribute("bis_Orient5"));
  this->Header->srow_z[2]=atof(element->GetAttribute("bis_Orient8"));



  this->Header->srow_x[3] = this->Header->srow_y[3] = this->Header->srow_z[3] = 0.0;

  this->IsHeaderValid=1;
  this->Mode=2;


  double sp[3];
  sp[0]=atof(element->GetAttribute("bis_Spacing0"));
  sp[1]=atof(element->GetAttribute("bis_Spacing1"));
  sp[2]=atof(element->GetAttribute("bis_Spacing2"));

  // Convert spans and srows into something meaningful
  this->Header->srow_x[0]*=sp[0];
  this->Header->srow_x[1]*=sp[1];
  this->Header->srow_x[2]*=sp[2];

  this->Header->srow_y[0]*=sp[0];
  this->Header->srow_y[1]*=sp[1];
  this->Header->srow_y[2]*=sp[2];

  this->Header->srow_z[0]*=sp[0];
  this->Header->srow_z[1]*=sp[1];
  this->Header->srow_z[2]*=sp[2];
  
  /*  mat44 inmat,rotmat,pmat;
  for (int ia=0;ia<=3;ia++)
    {
      inmat.m[0][ia]=this->Header->srow_x[ia];
      inmat.m[1][ia]=this->Header->srow_y[ia];
      inmat.m[2][ia]=this->Header->srow_z[ia];
      inmat.m[3][ia]=0.0;
    }
    inmat.m[3][3]=1.0;*/

  /*  NIFTI_print_mat44("input",inmat);
  fprintf(stderr,"orient=%.2f ",atof(element->GetAttribute("bis_Orient0")));
  fprintf(stderr,"%.2f ",atof(element->GetAttribute("bis_Orient3")));
  fprintf(stderr,"%.2f ",atof(element->GetAttribute("bis_Orient6")));

  fprintf(stderr,"%.2f ",atof(element->GetAttribute("bis_Orient1")));
  fprintf(stderr,"%.2f ",atof(element->GetAttribute("bis_Orient4")));
  fprintf(stderr,"%.2f ",atof(element->GetAttribute("bis_Orient7")));

  fprintf(stderr,"%.2f ",atof(element->GetAttribute("bis_Orient2")));
  fprintf(stderr,"%.2f ",atof(element->GetAttribute("bis_Orient5")));
  fprintf(stderr,"%.2f\n",atof(element->GetAttribute("bis_Orient8")));*/


  /*  vtkbisImageHeader::NIFTI_identity_mat44(&rotmat);
  rotmat.m[0][0]= 0.0; rotmat.m[0][2]= -1.0;
  rotmat.m[1][1]= -1.0; 
  rotmat.m[2][0]= 1.0; rotmat.m[2][2]= 0.0;
  NIFTI_print_mat44("rot",rotmat);

  mat44 outmat=NIFTI_mat44_mmul(rotmat,inmat);
  NIFTI_print_mat44("out",outmat);
  
  for (int ia=0;ia<=3;ia++)
    {
      this->Header->srow_x[ia]=outmat.m[0][ia];
      this->Header->srow_y[ia]=outmat.m[1][ia];
      this->Header->srow_z[ia]=outmat.m[2][ia];
      }*/


  this->Header->qform_code=0 ;   /*!< NIFTI_XFORM_* code.   */  /*-- all ANALYZE 7.5 ---*/
  this->Header->sform_code=1 ;   /*!< NIFTI_XFORM_* code.   */  /*   fields below here  */
  
  this->Header->intent_name[0]=(char)0;
  strcpy(this->Header->magic,"ni1\0");
  this->MakeStoredHeaderValid();

  //  fprintf(stderr,"Orientation Code =%s\n",this->GetOrientationCode());


  return 1;
}
// -----------------------------------------------------------------------
int vtkbisImageHeader::ReadAnalyzeHeader(char* filename)
{
  char *hdrfile=vtkbisImageHeader::CreateAnalyzeHDRFilename(filename);
  znzFile fd=vtkbisImageHeader::SuffixOpen(hdrfile,"rb");
  if (znz_isnull(fd))
    {
      fprintf(stdout,"Failed to open\n",hdrfile);
      char* hdrfile2=vtkbisImageHeader::CreateFilename(hdrfile,".HDR",".hdr");
      fd=vtkbisImageHeader::SuffixOpen(hdrfile2,"rb");
      if (!znz_isnull(fd))
	this->SetFileName(hdrfile2);
      delete [] hdrfile2;
    }
  else
    {
      this->SetFileName(hdrfile);
    }

  struct dsr hdr;
  short psize=0;
  if (znz_isnull(fd))
    {
      delete [] hdrfile;
      this->SetFileName("none.hdr");
      return 0;
    }

  znzread(&hdr,sizeof(struct dsr),1,fd);

  std::string s(this->FileName);
  this->ResetFields();
  this->SetFileName(s.c_str());

  this->Header->dim[1]  = hdr.dime.dim[1];
  this->Header->dim[2]  = hdr.dime.dim[2];
  this->Header->dim[3]  = hdr.dime.dim[3];
  this->Header->dim[4]  = hdr.dime.dim[4];
  this->Header->dim[0]  = 3+(hdr.dime.dim[4]>1);
  this->Header->pixdim[0] =   hdr.dime.pixdim[0];
  this->Header->pixdim[1] =   hdr.dime.pixdim[1];
  this->Header->pixdim[2]   = hdr.dime.pixdim[2];
  this->Header->pixdim[3]   = hdr.dime.pixdim[3];
  this->Header->glmin = hdr.dime.glmin;
  this->Header->glmax = hdr.dime.glmax;
  int orient = hdr.hist.orient;
  this->Header->bitpix          = hdr.dime.bitpix;
  this->Header->datatype=hdr.dime.datatype;


  strncpy(this->Header->descrip,hdr.hist.descrip,80);
  strncpy(this->Header->db_name,hdr.hk.db_name,18);

  this->OriginBuffer[0]=0.0;
  this->OriginBuffer[1]= 0.0;
  this->OriginBuffer[2]= 0.0;

  //  fprintf(stderr,"hdr.dime.funused1=%f, hdr.dime.funused2=%f\n",hdr.dime.funused1,hdr.dime.funused2);

  if (hdr.dime.funused3==0.0)
    {
      this->Header->scl_slope=hdr.dime.funused1;
      this->Header->scl_inter=hdr.dime.funused2;
    }
  else
    {
      this->Header->scl_slope=1.0;
      this->Header->scl_inter=0.0;
    }



  short voivalue=hdr.dime.unused1;
  this->SwapBytes=0;



  // This can be a common bug sometimes ... short marked as float but 16-bit!
  if (this->Header->bitpix==16 && this->Header->datatype!=DT_UINT16)
    this->Header->datatype=DT_INT16;


  if (hdr.dime.bitpix<0 || hdr.dime.bitpix>128)
    {
      this->SwapBytes=1;
      vtkbisImageHeaderSwap(&this->Header->dim[0],8);
      vtkbisImageHeaderSwap(&this->Header->glmin);
      vtkbisImageHeaderSwap(&this->Header->glmax);
      vtkbisImageHeaderSwap(&this->Header->pixdim[0],4);
      vtkbisImageHeaderSwap(&this->Header->bitpix);
      vtkbisImageHeaderSwap(&this->Header->datatype);
      vtkbisImageHeaderSwap(&this->OriginBuffer[0],3);
      vtkbisImageHeaderSwap(&this->Header->scl_slope,4);
      vtkbisImageHeaderSwap(&this->Header->scl_inter,4);
      vtkbisImageHeaderSwap(&voivalue);
    }
  znzclose(fd);

//  fprintf(stderr,"slope=%f inter=%f swap=%d\n",this->Header->scl_slope,
//	  this->Header->scl_inter,      this->SwapBytes);

  if (fabs(this->Header->scl_slope)<1e-8)
    {
      this->Header->scl_slope=1.0;
      this->Header->scl_inter=0.0;
    }

  double spa[3],ori[3]; int dim[3];
  for (int ia=1;ia<=3;ia++)
    {
      dim[ia-1]=this->Header->dim[ia];
      spa[ia-1]=fabs(this->Header->pixdim[ia]);
      ori[ia-1]=this->OriginBuffer[ia-1];
    }

  // Check This Step in future
  vtkMatrix4x4* mat_mapping=vtkMatrix4x4::New();
  vtkpxUtil::ReOrientMatrix(ori,spa,dim,0,orient,mat_mapping);
  mat44 mapping=NIFTI_Copy_Matrix4x4_mat44(mat_mapping);
  mat_mapping->Delete();

  mat44 scale; vtkbisImageHeader::NIFTI_identity_mat44(&scale);
  for (int ia=0;ia<=2;ia++)
    {
      scale.m[ia][ia]=spa[ia];
      scale.m[ia][3]=0.0;//this->OriginBuffer[ia];
    }

  //  vtkbisImageHeader::NIFTI_print_mat44("scale",scale);

  mat44 newmat=vtkbisImageHeader::NIFTI_mat44_mmul(mapping,scale);
  //  vtkbisImageHeader::NIFTI_print_mat44("newmat",newmat);

  this->ApplyMapping(newmat);
  this->Orientation=orient;
  //  this->SetOrientation(orient);
  this->Header->qform_code=0;
  this->Header->sform_code=1;

  this->Header->srow_x[3]=this->OriginBuffer[0];
  this->Header->srow_y[3]=this->OriginBuffer[1];
  this->Header->srow_z[3]=this->OriginBuffer[2];

  //  this->SetOrigin(this->OriginBuffer);
  this->SetVOIValue(voivalue);
  this->SetDBName(hdr.hk.db_name);

  strcpy(this->Header->magic,"ANA\0");

  this->IsHeaderValid=1;
  this->Mode=0;


  //  this->PrintHeader();
  return 1;
}
// --------------------------------------------------------------------------------------------
void vtkbisImageHeader::SetOrientation(int orient)
{
  this->SetOrientationComplex(orient,0);
}

void vtkbisImageHeader::SetOrientationComplex(int orient,int nomatrixops)
{
  if (orient<0) orient=0;

  int oldorient=this->Orientation;
  this->Orientation=orient;

  if (nomatrixops==1)
    return;

  if (this->Orientation==3)
    {
      this->Header->sform_code=0;
      this->Header->qform_code=0;
      return;
    }

  double ori[3]={0.0,0.0,0.0};
  int dim[3];
  dim[0]=this->Header->dim[1];
  dim[1]=this->Header->dim[2];
  dim[2]=this->Header->dim[3];

  double spa[3];
  spa[0]=this->Header->pixdim[1];
  spa[1]=this->Header->pixdim[2];
  spa[2]=this->Header->pixdim[3];

  // Check This Step in future
  vtkMatrix4x4* mat_mapping=vtkMatrix4x4::New();
  vtkpxUtil::ReOrientMatrix(ori,spa,dim,oldorient,this->Orientation,mat_mapping);


  mat44 mapping=NIFTI_Copy_Matrix4x4_mat44(mat_mapping);
  //  vtkbisImageHeader::NIFTI_print_mat44("reorient mapping",mapping);
  mat_mapping->Delete();
  this->ApplyMapping(mapping);
  if (this->Mode>0)
    {
      char line[200];
      sprintf(line,"BIS::vtkbisImageHeader::SetOrientationComplex Changing Orientation from %d to %d",oldorient,orient);
      this->AddComment(line,0);
    }


}
// --------------------------------------------------------------------------------------------
void vtkbisImageHeader::SetOrientationAsMatrix(mat44 newmat)
{
  nifti_1_header* hdr=this->GetRawHeader();
  int orientation=0;
  hdr->qform_code=0;
  hdr->sform_code=1;
  for (int ia=0;ia<=3;ia++)
    {
      hdr->srow_x[ia]=newmat.m[0][ia];
      hdr->srow_y[ia]=newmat.m[1][ia];
      hdr->srow_z[ia]=newmat.m[2][ia];
    }

  this->Orientation=this->GetOrientationFromNIFTIHeader();
  return;

}

// --------------------------------------------------------------------------------------------
int vtkbisImageHeader::WriteNIFTIHeader(znzFile fdf)
{

  if (this->IsHeaderValid==0)
    return 0;

  //  fprintf(stdout,"Beginning WriteNIFTI Header Diagnostics OK\n");
  int numbytes=znzwrite(this->Header,sizeof(nifti_1_header),1,fdf);

  //  fprintf(stdout,"Done with Write Header %d\n",numbytes);
  numbytes+=this->Extensions->WriteExtensions(fdf);
  //fprintf(stdout,"Numbytes=%d, done with extensions ...\n",numbytes);

  return numbytes;
}
// ------------------------------------------------------------------------
void vtkbisImageHeader::PrintAll(int show_extensions)
{
  std::cerr << "Common Parameters:\n";
  std::cerr  << "Mode (0=Analyze,1 =NIFT1 .hdr/.img, 2=NIFTI .nii) = " << this->Mode << "\n";
  std::cerr  << "FileName  = " << this->FileName << "\n";
  std::cerr  << "IsHeaderValid = " << this->IsHeaderValid << "\n";
  std::cerr  << "SwapBytes = " << this->SwapBytes << "\n";
  std::cerr  << "Orientation = " << this->Orientation << "\n";

  nifti_1_header *hdr=this->Header;
  std::cerr  << "NIFTI1 Header (Header)\n --------------------------------------------\n";
  std::cerr  << "int sizeof_hdr    : " << hdr->sizeof_hdr    << "\t MUST be 348         (ANLZ7.5)int sizeof_hdr\n";
  std::cerr  << "char data_type[10]: " << hdr->data_type     << "\t ++UNUSED++          (ANLZ7.5)char data_type[10]\n";
  std::cerr  << "char db_name[18]: " << hdr->db_name       << "\t ++UNUSED++          (ANLZ7.5)char db_name[18]\n";
  if (show_extensions>1)
    {
      std::cerr  << "int extents     : " << hdr->extents       << "\t ++UNUSED++          (ANLZ7.5)int extents\n";
      std::cerr  << "short session_error : " << hdr->session_error << "\t ++UNUSED++          (ANLZ7.5)short session_error\n";
      std::cerr  << "char regular    : " << hdr->regular       << "\t ++UNUSED++          (ANLZ7.5)char regular\n";
      std::cerr  << "char dim_info   : " << hdr->dim_info      << "\t MRI slice ordering. (ANLZ7.5)char hkey_un0\n";
    }
  std::cerr  << "short dim[8]    : was image_dimension substruct -- (ANLZ7.5)short dim[8];\n\t";
  for (int ia=0;ia<8;ia++)
    std::cerr  << "Dim ("<<ia<<")="<<hdr->dim[ia]<<",";
  std::cerr << "\n";
  if (show_extensions>1)
    {
      std::cerr  << "float intent_p1 :" << hdr->intent_p1 << "\t 1st intent parameter. (ANLZ7.5)short unused8; short unused9\n";
      std::cerr  << "float intent_p2 :" << hdr->intent_p2 << "\t 1st intent parameter. (ANLZ7.5)short unused10; short unused11\n";
      std::cerr  << "float intent_p3 :" << hdr->intent_p3 << "\t 1st intent parameter. (ANLZ7.5)short unused12; short unused13\n";
      std::cerr  << "short ident_code:" << hdr->intent_code << "\tNIFTI_INTENT_* code.  (Analyze)short unused14;\n";
    }
  std::cerr  << "short dataype:   " << hdr->datatype << "\t Defines data type!    (Analyze)short datatype;\n";
  std::cerr  << "short bitpix :   " << hdr->bitpix   << "\t Number bits/voxel.    (Analyze)short bitpix;\n";
  if (show_extensions>1)
    std::cerr  << "short slice_start:" << hdr->slice_start << "\t First slice index.    (Analyze)short dim_un0;\n";
  std::cerr  << "short pixdim[8] \t    Grid spacings.   (ANLZ7.5)float pixdim[8];\n";
  for (int ia=0;ia<8;ia++)
    std::cerr  << "pixdim ("<<ia<<")="<<hdr->pixdim[ia]<<",";
  std::cerr << "\n";
  std::cerr  << "float vox_offset: " << hdr->vox_offset << "\t Offset into .nii file (Analyze)float vox_offset;\n";
  std::cerr  << "float scl_slope : " << hdr->scl_slope  << "\t Data scaling: slope.  (Analyze)float funused1 \n";
  std::cerr  << "float scl_inter : " << hdr->scl_inter  << "\t Data scaling: offset. (Analyze)float funused2;\n";
  if (show_extensions>1)
    {
      std::cerr  << "float slice_end : " << hdr->slice_end  << "\t Last slice index.     (Analyze)float funused3\n";
      std::cerr  << "char slice_code : " << hdr->slice_code << "\t Slice timing order.   (Analyze) ...\n";
      std::cerr  << "char xyzt_units : " << hdr->xyzt_units << "\t Units of pixdim[1..4]";
      std::cerr  << "float cal_max   : " << hdr->cal_max    << "\t Max display intensity (Analyze)float cal_max;\n";
      std::cerr  << "float cal_min   : " << hdr->cal_min    << "\t Min display intensity (Analyze)float cal_min;\n";
      std::cerr  << "float slice_duration"<< hdr->slice_duration << "\t Time for 1 slice.     (Analyze)float compressed;\n";
      std::cerr  << "float toffset   :   "<<hdr->toffset << "\t Time axis shift.      (Analyze)float verified;\n";
      std::cerr  << "int glmax       :   "<<hdr->glmax << "\t ++UNUSED++       (ANLZ7.5)int glmax;\n";
      std::cerr  << "int glmin       :   "<<hdr->glmin << "\t ++UNUSED++       (ANLZ7.5)int glmin;\n";
      std::cerr  << "char descrip[80]:   "<<hdr->descrip << "\t  any text you like.    (Analyze)char descrip[80];\n";
      std::cerr  << "char aux_file[24]:  "<<hdr->aux_file<< "\t auxiliary filename.   (Analyze)char aux_file[24];\n";
    }
  std::cerr  << "short qform_code :  "<<hdr->qform_code << "\t NIFTI_XFORM_* code.  -- all ANALYZE 7.5 ---\n";
  std::cerr  << "short sform_code :  "<<hdr->sform_code << "\t NIFTI_XFORM_* code.    fields below here are replaced \n";
  std::cerr  << "float quatern_b  :  " << hdr->quatern_b << "\t Quaternion b param.  \n";
  std::cerr  << "float quatern_c  :  " << hdr->quatern_c << "\t Quaternion c param.  \n";
  std::cerr  << "float quatern_d  :  " << hdr->quatern_d << "\t Quaternion d param.  \n";
  std::cerr  << "float qoffset_x  :  " << hdr->qoffset_x << "\t Quaternion x shift.  \n";
  std::cerr  << "float qoffset_y  :  " << hdr->qoffset_y << "\t Quaternion y shift.  \n";
  std::cerr  << "float qoffset_z  :  " << hdr->qoffset_z << "\t Quaternion z shift.  \n";
  std::cerr  << "float srow_x[4]  :  \t 1st row affine transform.   \n";
  std::cerr  << "\t " <<  hdr->srow_x[0] << "\t" <<  hdr->srow_x[1] << "\t" <<  hdr->srow_x[2] << "\t" <<  hdr->srow_x[3] << "\n";
  std::cerr  << "float srow_y[4]  :  \t 2nd row affine transform.   \n";
  std::cerr  << "\t " <<  hdr->srow_y[0] << "\t" <<  hdr->srow_y[1] << "\t" <<  hdr->srow_y[2] << "\t" <<  hdr->srow_y[3] << "\n";
  std::cerr  << "float srow_z[4]  :  \t 3rd row affine transform.   \n";
  std::cerr  << "\t " <<  hdr->srow_z[0] << "\t" <<  hdr->srow_z[1] << "\t" <<  hdr->srow_z[2] << "\t" <<  hdr->srow_z[3] << "\n";
  std::cerr  << "char intent_name[16] : " << hdr->intent_name << "\t 'name' or meaning of data. \n";
  std::cerr  << "char magic[4]        : " << hdr->magic << "\t MUST be ni-1 or n+1\n\n";



  if (show_extensions)
    {
      std::cerr  << "NIFTI1 Extensions (Extensions)\n --------------------------------------------\n";
      this->Extensions->PrintAll();
    }
}
// -----------------------------------------------------------------------------------------------------------------------------------
mat44 vtkbisImageHeader::NIFTI_CreateOrientationMatrix( nifti_1_header *hdr)
{
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

  return original;
}

const char* vtkbisImageHeader::NIFTI_GetOrientationString( int ii )
{
   switch( ii )
     {
     case NIFTI_L2R: return "L-to-R" ;
     case NIFTI_R2L: return "R-to-L" ;
     case NIFTI_P2A: return "P-to-A" ;
     case NIFTI_A2P: return "A-to-P" ;
     case NIFTI_I2S: return "I-to-S" ;
     case NIFTI_S2I: return "S-to-I" ;
     }
   return "Unknown" ;
}

const char* vtkbisImageHeader::NIFTI_GetOrientationStringShort( int ii )
{
   switch( ii )
     {
     case NIFTI_L2R: return "R" ;
     case NIFTI_R2L: return "L" ;
     case NIFTI_P2A: return "A" ;
     case NIFTI_A2P: return "P" ;
     case NIFTI_I2S: return "S" ;
     case NIFTI_S2I: return "I" ;
     }
   return "U" ;
}


const char* vtkbisImageHeader::GetOrientationCode()
{
  this->Buffer="";

  nifti_1_header* hdr=this->GetRawHeader();
  int orientation=0;

  //   METHOD 1 (the "old" way, used only when qform_code = 0):
  if (hdr->qform_code == 0 && hdr->sform_code==0)
    {
      this->Buffer="";
      return this->Buffer.c_str();
    }

  if (this->GetMode()==0)
    {
      this->Buffer="";
      return this->Buffer.c_str();
    }



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

  this->Buffer="(";
  this->Buffer+=NIFTI_GetOrientationStringShort(icod);
  this->Buffer+=NIFTI_GetOrientationStringShort(jcod);
  this->Buffer+=NIFTI_GetOrientationStringShort(kcod);
  this->Buffer+=")";

  return this->Buffer.c_str();
}

// -----------------------------------------------------------------------------------
// mat44 Utilities
// -----------------------------------------------------------------------------------
mat44 vtkbisImageHeader::NIFTI_mat44_mmul( mat44 A , mat44 B )  /* multiply 2 4x4 matrices */
{
   mat44 C ; int i,j ;
   for( i=0 ; i < 4 ; i++ )
    for( j=0 ; j < 4 ; j++ )
      C.m[i][j] =  A.m[i][0] * B.m[0][j]
	+ A.m[i][1] * B.m[1][j]
	+ A.m[i][2] * B.m[2][j]
	+ A.m[i][3] * B.m[3][j];
   return C ;
}

void vtkbisImageHeader::NIFTI_print_mat44(char* s, mat44 A)
{
  fprintf(stdout,"\n\n %s = \n",s);
  for (int ia=0;ia<=3;ia++)
    {
      fprintf(stdout,"\t [ ");
      for (int ib=0;ib<=3;ib++)
	fprintf(stdout," %+8.3f\t ", A.m[ia][ib]);
      fprintf(stdout,"]\n");
    }
}

void vtkbisImageHeader::NIFTI_identity_mat44(mat44* A)
{
  for (int ia=0;ia<=3;ia++)
    for (int ib=0;ib<=3;ib++)
      {
	if (ia==ib)
	  A->m[ia][ib]=1.0;
	else
	  A->m[ia][ib]=0.0;
      }
}


mat44 vtkbisImageHeader::NIFTI_Copy_Matrix4x4_mat44(vtkMatrix4x4* input)
{
  mat44 output;
  if (input!=NULL)
    {
      for (int ia=0;ia<=3;ia++)
	for (int ib=0;ib<=3;ib++)
	  output.m[ia][ib]=input->GetElement(ia,ib);
    }
  else
    {
      vtkbisImageHeader::NIFTI_identity_mat44(&output);
    }
  return output;

}

vtkMatrix4x4* vtkbisImageHeader::NIFTI_CopyMat44(mat44 input)
{
  vtkMatrix4x4* output=vtkMatrix4x4::New();
  for (int ia=0;ia<=3;ia++)
    for (int ib=0;ib<=3;ib++)
      output->SetElement(ia,ib,input.m[ia][ib]);

  return output;

}

// -----------------------------------------------------------------------------------------------------------------------------------
void vtkbisImageHeader::PrintHeader(int long_mode)
{
  char* l=this->GetHeaderDescription(long_mode);
  fprintf(stdout,"%s\n",l);
  delete [] l;
}
// -----------------------------------------------------------------------------------------------------------------------------------
char* vtkbisImageHeader::GetHeaderDescription(int long_mode)
{
  std::string output ="";
  nifti_1_header *hdr=this->Header;
  char line[20000];

  if (long_mode>=0)
    output = output +  this->FileName +" ";

  if (long_mode==-1)
    long_mode=0;

  if (long_mode)
    output += "\n\tOrientation = ";
  switch (this->Orientation)
    {
    case 0: output += " Axial "; break;
    case 1: output += " Coronal "; break;
    case 2: output += " Sagittal "; break;
    case 3: output += " Polar "; break;
    }

  mat44 original=vtkbisImageHeader::NIFTI_CreateOrientationMatrix(this->Header);
  int icod,jcod,kcod;  nifti_mat44_to_orientation(original,&icod,&jcod,&kcod);
  if (long_mode)
    {
      sprintf(line,"(%s,%s,%s)", nifti_orientation_string(icod),
	      nifti_orientation_string(jcod),
	      nifti_orientation_string(kcod));
      output+=line;
    }
  else
    {
      sprintf(line,"(%s,%s,%s)", vtkbisImageHeader::NIFTI_GetOrientationString(icod),
	      vtkbisImageHeader::NIFTI_GetOrientationString(jcod),
	      vtkbisImageHeader::NIFTI_GetOrientationString(kcod));
      output+=line;

    }


  if (long_mode)
    {
      output+="\n\tMode = ";
      switch (this->Mode)
	{
	case 0: output+=" Analyze 7.5"; break;
	case 1: output+=" NIFTI (.hdr/.img)"; break;
	case 2: output+=" NIFTI (.nii)"; break;
	}
      sprintf(line,"(magic=%s) (Valid = %d SwapBytes=%d)\n",hdr->magic,this->IsHeaderValid,this->SwapBytes);
      output+=line;
      output+="\t Dimensions =";
    }

  sprintf(line," %dx%dx%d, %d ",hdr->dim[1],hdr->dim[2],hdr->dim[3],hdr->dim[4]);
  output+=line;
  if (long_mode)
    {
      sprintf(line,"\n\tVoxel Dimensions = %.3fx%.3fx%.3f, %.2f \n",hdr->pixdim[1],hdr->pixdim[2],hdr->pixdim[3],hdr->pixdim[4]);
      output+=line;
      sprintf(line,"\tData Type = %s(%d) (Bits per Pixel=%d) Vox_Offset=%d Num Extensions=%d (Size =%d)\n",nifti_datatype_string(hdr->datatype),hdr->datatype,hdr->bitpix,
	      (int)hdr->vox_offset,this->Extensions->GetNumberOfExtensions(),this->Extensions->GetTotalByteSize());
      output+=line;

      sprintf(line,"\tScale= %.3f Shift=%.3f \n",hdr->scl_slope,hdr->scl_inter);
      output+=line;
      sprintf(line,"\tOrientation Matrix Stuff (qform_code=%d sform_code=%d)\n",hdr->qform_code,hdr->sform_code);
      output+=line;
      if (hdr->qform_code>0)
	{
	  sprintf(line,"\tQuatenion (qfac=%f) quat=(%.3f,%.3f,%.3f) offset=(%.3f,%.3f,%.3f)\n",
		  hdr->pixdim[0],
		  hdr->quatern_b,hdr->quatern_c,hdr->quatern_d,
		  hdr->qoffset_x,hdr->qoffset_y,hdr->qoffset_z);
	  output+=line;
	}

      if (hdr->sform_code>0)
	{
	  sprintf(line,"\tMatrix_x = \t( %+8.3f %+8.3f %+8.3f %+8.3f )\n",
		  hdr->srow_x[0],hdr->srow_x[1],hdr->srow_x[2],hdr->srow_x[3]);
	  output+=line;
	  sprintf(line,"\t       y = \t( %+8.3f %+8.3f %+8.3f %+8.3f )\n",
		  hdr->srow_y[0],hdr->srow_y[1],hdr->srow_y[2],hdr->srow_y[3]);
	  output+=line;
	  sprintf(line,"\t       z = \t( %+8.3f %+8.3f %+8.3f %+8.3f)\n",
		  hdr->srow_z[0],hdr->srow_z[1],hdr->srow_z[2],hdr->srow_z[3]);
	  output+=line;
	}

      if (this->Mode==0)
	{
	  double ori[3]; this->GetOrigin(ori);
	  sprintf(line,"\t (Analyze Origin) = (%.1f,%1.f,%.1f)\n",ori[0],ori[1],ori[2]);
	  output+=line;
	}
    }
  else
    {
      sprintf(line," (%.3fx%.3fx%.3f, %.2f)\n",hdr->pixdim[1],hdr->pixdim[2],hdr->pixdim[3],hdr->pixdim[4]);
      output+=line;
      sprintf(line,"\tData Type = %s (Bits=%d) NumExt=%d (Sz =%d)",nifti_datatype_string(hdr->datatype),hdr->bitpix,
	      this->Extensions->GetNumberOfExtensions(),this->Extensions->GetTotalByteSize());
      output+=line;
    }


  /*  if (long_mode>0)
    {
    sprintf(line,"\t DBName : %s , VOIValue= %d\n",this->GetDBName(),this->GetVOIValue());
    output+=line;
      
    }*/

  if (long_mode>1)
    {
      int next=this->Extensions->GetNumberOfExtensions();
      sprintf(line,"\n\t Extensions = %d , size=%d\n",
	      next,this->Extensions->GetTotalByteSize());
      output+=line;
      for (int i=0;i<next;i++)
	{
	  vtkbisNIFTIExtension* extn=this->Extensions->GetRawExtension(i);
	  sprintf(line,"*********** Extensions %d (code=%d size=%d)\n",
		  i+1,extn->GetECode(),extn->GetESize());
	  output+=line;
	  output+=extn->GetText();
	  output+="\n";
	}
    }

  char* ll=new char[output.length()+1];
  strcpy(ll,output.c_str());
  return ll;
}
// -----------------------------------------------------------------------------------------------------------------------------------
int vtkbisImageHeader::ReconcileImage(vtkImageData* img,int useorigin)
{
  if (img==NULL || this->IsHeaderValid==0)
    return 0;

  double spa[3];
  this->GetSpacing(spa);  img->SetSpacing(spa);

  if (useorigin)
    {
      double ori[3]; this->GetOrigin(ori); img->SetOrigin(ori);
    }
  return 1;
}

int vtkbisImageHeader::ReconcileHeader(vtkImageData* img,int useorigin)
{
  if (img==NULL)
    return 0;

  if (this->IsHeaderValid==0)
    {
      return this->CreateHeader(img,useorigin);
    }

  //  fprintf(stderr,"\n ---------------------------------\n OK Reconciling Header .................. \n");

  // These are a lock ...
  char line[200];
  int dim[3]; img->GetDimensions(dim);
  int olddim[3];
  double spa[3]; img->GetSpacing(spa);

  for (int ia=0;ia<=2;ia++)
    spa[ia]=fabs(spa[ia]);

  int dim_change=0;

  for (int ia=0;ia<=2;ia++)
    {
      olddim[ia]=this->Header->dim[ia+1];
      if (this->Header->dim[ia+1]!=dim[ia])
	{
	  ++dim_change;
	  this->Header->dim[ia+1]=dim[ia];
	}
    }


  if (dim_change>0)
    {
      //      sprintf(line,"DebugMessage (Ignore) BIS:vtkbisImageHeader:Recon Header:Changed Dimensions  (%d,%d,%d) -> (%d,%d,%d)",
      //	      olddim[0],olddim[1],olddim[2],dim[0],dim[1],dim[2]);
      //      this->AddComment(line,0);
    }


  int numc_change=0;
  if (this->Header->dim[4]!=img->GetNumberOfScalarComponents())
    {
      int tmp=this->Header->dim[4];
      this->Header->dim[4]=img->GetNumberOfScalarComponents();

      //      sprintf(line,"DebugMessage (Ignore) BIS:vtkbisImageHeader:Recon Header:Changed Frames %d to %d",tmp,this->Header->dim[4]);
      //      this->AddComment(line,0);

      numc_change=1;
    }

  int maxd=1;
  for (int ia=1;ia<8;ia++)
    {
      if (this->Header->dim[ia]>1)
	maxd=ia;
    }
  this->Header->dim[0]=maxd;

  int datatype_change=0;
  int dt=vtkbisImageHeader::GetNIFTIDataType(img->GetScalarType());
  if (this->Header->datatype!=dt)
    {
      //  sprintf(line,"DebugMessage (Ignore) BIS:vtkbisImageHeader:Recon Header:Changed Datatype : %d --> %d",
      //      this->Header->datatype,dt);
      //      this->AddComment(line,0);
      this->Header->datatype=dt;
      this->Header->bitpix=vtkbisImageHeader::GetBitPix(img->GetScalarType());
      datatype_change=1;
    }


  double oldspa[3]; this->GetSpacing(oldspa);
  /*  oldspa[0]=this->Header->pixdim[1];
  oldspa[1]=this->Header->pixdim[2];
  oldspa[2]=this->Header->pixdim[3];*/

  //    fprintf(stderr,"\n\n\n On to spacing img = (%.2f,%.2f,%.2f) header=(%.2f,%.2f,%.2f) qf=%d, sf=%d\n",
  //    spa[0],spa[1],spa[2],oldspa[0],oldspa[1],oldspa[2],
  //    this->Header->qform_code,this->Header->sform_code);


  int spacing_change=0;
  for (int ia=0;ia<=2;ia++)
    {
      if (fabs(spa[ia]-oldspa[ia])>0.00001)
	    ++spacing_change;
    }

  if (spacing_change!=0)
    {
      /*      this->Header->pixdim[1]=spa[0];
	      this->Header->pixdim[2]=spa[1];
	      this->Header->pixdim[3]=spa[2];*/

      mat44 mapping; vtkbisImageHeader::NIFTI_identity_mat44(&mapping);
      for (int ia=0;ia<=2;ia++)
	mapping.m[ia][ia]=spa[ia]/oldspa[ia];

      /*      for (int ia=0;ia<=3;ia++)
	{
	  fprintf(stderr,"mapping = [ ");
	  for (int ib=0;ib<=3;ib++)
	    fprintf(stderr,"%.4f ",mapping.m[ia][ib]);
	  fprintf(stderr,"]\n");
	  }*/


      this->ApplyMapping(mapping);
      //      this->PrintHeader();


      if (useorigin)
	{
	  double ori[3]; this->GetOrigin(ori);
	  double orn[3]; img->GetOrigin(orn);
	  vtkbisImageHeader::NIFTI_identity_mat44(&mapping);
	  for (int ia=0;ia<=2;ia++)
	    mapping.m[ia][3]=orn[ia]-ori[ia];
	  this->ApplyMapping(mapping);
	}
      //      sprintf(line,"DebugMessage (Ignore) BIS:vtkbisImageHeader:Recon Header:Changed Voxel spacing  (%.3f,%.3f,%.3f) -> (%.3f,%.3f,%.3f)",
      //      oldspa[0],oldspa[1],oldspa[2],spa[0],spa[1],spa[2]);
      //      this->AddComment(line,0);
      //fprintf(stderr,"%s\n",line);
      //      this->PrintHeader();
    }

  this->IsHeaderValid=1;

  //  fprintf(stderr,"Done Reconciling Header .................. \n");
  return 1;
}

// -----------------------------------------------------------------------------------
int vtkbisImageHeader::ApplyMapping(mat44 mapping,int premultiply)
{
  //  First  quaternion stuff
  int mod_flag=0;

  if (this->Header->sform_code>0)
    {
      mat44 original;
      for (int ia=0;ia<=3;ia++)
	{
	  original.m[0][ia]=this->Header->srow_x[ia];
	  original.m[1][ia]=this->Header->srow_y[ia];
	  original.m[2][ia]=this->Header->srow_z[ia];
	  original.m[3][ia]=float(ia==3);
	}
      mat44 newmat;
      if (premultiply>0)
	newmat=vtkbisImageHeader::NIFTI_mat44_mmul(mapping,original);
      else
	newmat=vtkbisImageHeader::NIFTI_mat44_mmul(original,mapping);

      for (int ia=0;ia<=3;ia++)
	{
	  this->Header->srow_x[ia]=newmat.m[0][ia];
	  this->Header->srow_y[ia]=newmat.m[1][ia];
	  this->Header->srow_z[ia]=newmat.m[2][ia];
	}
      mod_flag+=2;

      nifti_mat44_to_quatern(newmat,
			     &this->Header->quatern_b,&this->Header->quatern_c,&this->Header->quatern_d,
			     &this->Header->qoffset_x,&this->Header->qoffset_y,&this->Header->qoffset_z,
			     &this->Header->pixdim[1],&this->Header->pixdim[2],&this->Header->pixdim[3],
			     &this->Header->pixdim[0]);
    }
  else if (this->Header->qform_code>0)
    {
      mat44 original;
      original=nifti_quatern_to_mat44(this->Header->quatern_b,this->Header->quatern_c,this->Header->quatern_d,
				      this->Header->qoffset_x,this->Header->qoffset_y,this->Header->qoffset_z,
				      fabs(this->Header->pixdim[1]),fabs(this->Header->pixdim[2]),fabs(this->Header->pixdim[3]),
				      this->Header->pixdim[0]);

      //      NIFTI_print_mat44("original=",original);
      //      fprintf(stderr,"Pixdim=%f,%f,%f",this->Header->pixdim[1],this->Header->pixdim[2],this->Header->pixdim[3]);

      mat44 newmat;
      if (premultiply>0)
	newmat=vtkbisImageHeader::NIFTI_mat44_mmul(mapping,original);
      else
	newmat=vtkbisImageHeader::NIFTI_mat44_mmul(original,mapping);

      for (int ia=0;ia<=3;ia++)
	{
	  this->Header->srow_x[ia]=newmat.m[0][ia];
	  this->Header->srow_y[ia]=newmat.m[1][ia];
	  this->Header->srow_z[ia]=newmat.m[2][ia];
	}

      nifti_mat44_to_quatern(newmat,
			     &this->Header->quatern_b,&this->Header->quatern_c,&this->Header->quatern_d,
			     &this->Header->qoffset_x,&this->Header->qoffset_y,&this->Header->qoffset_z,
			     &this->Header->pixdim[1],&this->Header->pixdim[2],&this->Header->pixdim[3],
			     &this->Header->pixdim[0]);
      mod_flag+=1;
    }

  return mod_flag;
}

int vtkbisImageHeader::ModifyOrientation(mat44 mapping,int DataFlip[3],int do_transpose)
{
  int mod_flag=this->ApplyMapping(mapping,0);
  this->Orientation=this->GetOrientationFromNIFTIHeader();

  if (DataFlip[0]>0 || DataFlip[1] >0 || DataFlip[2]>0 ||
      do_transpose>0)
    {
      char line[300];
      sprintf(line,"BIS::vtkbisImageHeader :: Fixed Header to BioImage Suite Standard (still valid!) flips = %d,%d,%d transpose=%d orientation=%d (mod_qform=%d),(mod_sform=%d)",
	      DataFlip[0],
	      DataFlip[1],
	      DataFlip[2],
	      do_transpose,
	      this->Orientation,
	      ( (mod_flag==1) || (mod_flag==3)),
	      (mod_flag>1));
      this->AddComment(line,0);
      //      fprintf(stdout,"Comment=%s\n",line);
      return 1;
    }
  return 0;
}

int vtkbisImageHeader::ApplyShift(double x,double y,double z)
{

  if (fabs(x)<0.001 && fabs(y)<0.001 && fabs(z)<0.001)
    return 0;


  mat44 shift_matrix;       vtkbisImageHeader::NIFTI_identity_mat44(&shift_matrix);
  shift_matrix.m[0][3]=x;
  shift_matrix.m[1][3]=y;
  shift_matrix.m[1][3]=z;

  this->ApplyMapping(shift_matrix);
  //  char line[200];
  //  sprintf(line,"DebugMessage (Ignore) BIS:vtkbisImageHeader:Apply Shift (perhaps image cropping) %.2f %.2f %.2f",x,y,z);
  //  this->AddComment(line,0);
  return 1;
}

// ----------------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------------
int vtkbisImageHeader::CopyExtensionsOnly(vtkbisImageHeader* other)
{
  if (other==NULL)
    return 0;

  if (other->GetExtensions()==NULL)
    return 0;

  this->Extensions->Copy(other->GetExtensions());
  return 1;

}
// ----------------------------------------------------------------------------------------------
int vtkbisImageHeader::CopyLocationInformationOnly(vtkbisImageHeader* other)
{
  if (other==NULL)
    return 0;

  nifti_1_header* hdr=this->GetRawHeader();
  nifti_1_header* otr=other->GetRawHeader();

  hdr->qform_code=otr->qform_code;
  hdr->sform_code=otr->sform_code;
  /*   are replaced       */
  hdr->quatern_b=  otr->quatern_b;
  hdr->quatern_c=  otr->quatern_c;
  hdr->quatern_d=  otr->quatern_d;
  hdr->qoffset_x=  otr->qoffset_x;
  hdr->qoffset_y=  otr->qoffset_y;
  hdr->qoffset_z=  otr->qoffset_z;

  for (int ia=0;ia<=3;ia++)
    {
      hdr->srow_x[ia]=otr->srow_x[ia];
      hdr->srow_y[ia]=otr->srow_y[ia];
      hdr->srow_z[ia]=otr->srow_z[ia];
    }

  this->Orientation=this->GetOrientationFromNIFTIHeader();
  return 1;
}
// ----------------------------------------------------------------------------------------------
/*vtkImageData* vtkbisImageHeader::ReOrientDICOMImageAndFillInHeader(vtkbisDICOMReader* dicomr)
{
  if (dicomr==NULL)
    return NULL;

  if (dicomr->GetOutput()==NULL)
    return NULL;

  vtkImageData* tmp=vtkImageData::New();
  tmp->ShallowCopy(dicomr->GetOutput());

  if (tmp->GetDimensions()[0]<1)
    {
      tmp->Delete();
      return NULL;
    }

  // --- On to real work from here on ----------------
  fprintf(stderr,"Dicomr is OK\n");

  this->CreateHeader(tmp,0);
  //  this->SetFileName(dicomr->GetFileName());

  strcpy(this->Header->descrip,"from vtkImageDICOMReader");
  strcpy(this->Header->aux_file,"vtkbisImageHeader");

  // --- Key Thing now is to get the orientation and work from here .....

  double* vec=dicomr->GetImagePositionPatient();

  double v1[3],v2[3],v3[3];
  for (int ia=0;ia<=2;ia++)
    {
      v1[ia]=vec[ia];
      v2[ia]=vec[ia+3];
    }

  vtkMath::Normalize(v1);
  vtkMath::Normalize(v2);
  vtkMath::Cross(v1,v2,v3);

  fprintf(stderr,"v1 = (%.2f,%.2f,%.2f)\n",v1[0],v1[1],v1[2]);
  fprintf(stderr,"v2 = (%.2f,%.2f,%.2f)\n",v2[0],v2[1],v2[2]);
  fprintf(stderr,"v3 = (%.2f,%.2f,%.2f)\n\n",v3[0],v3[1],v3[2]);

  return tmp;

}
*/

int vtkbisImageHeader::MapOrientationToNewStyle(int oldorient)
{
  // Called by mistake
  if (oldorient>=3)
    return oldorient;

  if (oldorient==1)
    return 20;
  else if (oldorient==2)
    return 30;

  return 10;

}

int vtkbisImageHeader::MapOrientationToOldStyle(int neworient)
{
  // No Conversion needed here
  if (neworient<=3 && neworient>=0)
    return neworient;

  // Coronal
  if ( (neworient>=20 && neworient<=27 ) || (neworient>=50 && neworient<=57))
    return 1;

  // Sagital
  if ( (neworient>=30 && neworient<=37) || (neworient>=60 && neworient<=67))
    return 2;

  // Default is Axial
  return 0;
}
// -------------------------------------------------------------------------------
int vtkbisImageHeader::MakeStoredHeaderValid()
{
  this->Mode=2;
  this->Orientation= this->GetOrientationFromNIFTIHeader();
  this->SetFileName("");
  this->SetIsHeaderValid(1);

  return 1;
}
// -------------------------------------------------------------------------------
int vtkbisImageHeader::ReconcileImageIntensityRange(vtkImageData* img)
{
  if (img==NULL || this->IsHeaderValid==0)
    return 0;

  // if scale=1.0 and offset=0.0 then do nothing !
  if (fabs(this->GetIntensityScale()-1.0)<0.0001 &&
      fabs(this->GetIntensityOffset())<0.0001 )
    {
      //      fprintf(stderr,"vtkbisImageHeader::No changes to intensity needed\n");
      return 1;
    }

  if (this->GetIntensityScale()==0.0)
    return 1;


  //

  double scale=this->GetIntensityScale();
  double shift=this->GetIntensityOffset();

  // vtkImageShiftScale does
  //  *outSI = static_cast<OT>((static_cast<double>(*inSI) + shift) * scale);
  // out = (inp + shift2)* scale2
  // We want
  // out = inp*scale1 + shift1
  //
  // out= inp*scale2 + shift2*scale2
  // scale2=scale1
  // shift2=shift1/scale1

  char line[200];
  sprintf(line,"BIS::vtkbisImageHeader::Changed Image Intensity Scale and Offset from %.6f,%.6f to 1.0,0.0",
	  scale,shift);

  if  (fabs(scale)>0.00001)
    shift=shift/scale;


  vtkImageShiftScale* sc=vtkImageShiftScale::New();
  sc->SetInput(img);
  sc->SetShift(shift);
  sc->SetScale(scale);

  if (img->GetScalarType()!=VTK_DOUBLE)
    sc->SetOutputScalarTypeToFloat();
  else
    sc->SetOutputScalarTypeToDouble();

  sc->Update();

  img->ShallowCopy(sc->GetOutput());
  sc->Delete();

  int dt=vtkbisImageHeader::GetNIFTIDataType(img->GetScalarType());
  if (this->Header->datatype!=dt)
    {
      sprintf(line,"%s and changed Header Datatype : %d --> %d\n",
	      line,this->Header->datatype,dt);
      this->Header->datatype=dt;
      this->Header->bitpix=vtkbisImageHeader::GetBitPix(img->GetScalarType());
    }
  this->Header->scl_inter=0.0;
  this->Header->scl_slope=1.0;

  this->AddComment(line,0);
  //  fprintf(stderr,"line=%s\n",line);
  return 1;

}

int vtkbisImageHeader::ForceHeaderToBioImageSuiteCoordinatesAndFlipImage(vtkImageData* inout)
{
  int flips[3],transpose=1;
  int ori=this->ForceHeaderToBioImageSuiteCoordinatesAndReturnFlips(flips,transpose);

  // Flip the image
  if (flips[0]==0 && flips[1]==0 && flips[2]==0 && transpose==0)
    return ori;

  double scale[3];
  for (int ia=0;ia<=2;ia++)
    {
      if (flips[ia]>0)
	scale[ia]=-1.0;
      else
	scale[ia]=1.0;
      //      fprintf(stderr,"Scale (%d) = %.2f\n",flips[ia],scale[ia]);
    }


  vtkTransform* tr=vtkTransform::New();
  tr->Identity();
  tr->PostMultiply();
  tr->Scale(scale);

  if (transpose)
    {
      double elements[16] = { 0.0, 1.0, 0.0, 0.0, 
			      1.0, 0.0, 0.0, 0.0, 
			      0.0, 0.0, 1.0, 0.0,
			      0.0, 0.0, 0.0, 1.0 };
      tr->Concatenate(elements);
    }
  
  vtkImageReslice* resl=vtkImageReslice::New();
  resl->AutoCropOutputOn();
  resl->SetInput(inout);
  resl->SetInterpolationMode(0);
  resl->OptimizationOff();
  resl->SetResliceTransform(tr);
  resl->Update();
  
  

  tr->Delete();
  inout->ShallowCopy(resl->GetOutput());
  inout->SetOrigin(0.0,0.0,0.0);

  if (inout->GetInformation()->Has(vtkDataObject::ORIGIN()))
    inout->GetInformation()->Set(vtkDataObject::ORIGIN(),0.0,0.0,0.0);
  
  if (inout->GetPipelineInformation()->Has(vtkDataObject::ORIGIN()))
    inout->GetPipelineInformation()->Set(vtkDataObject::ORIGIN(),0.0,0.0,0.0);

  resl->Delete();


  return ori;
}

char vtkbisImageHeader::OrientationToAxisName(int neworient,int axis)
{
  if (neworient<10)
    neworient=vtkbisImageHeader::MapOrientationToNewStyle(neworient);
  char codes[7];
  char newc[1]; strcpy(newc,"?");

  if (neworient>=10 && neworient<=17)
    strcpy(codes,"LPSRAI");
  else if (neworient>=20 && neworient<=27)
    strcpy(codes,"LIPRSA");
  else if (neworient>=30 && neworient<=37)
    strcpy(codes,"PSRAIL");
  else if (neworient>=40 && neworient<=47)
    strcpy(codes,"PLSARI");
  else if (neworient>=50 && neworient<=57)
    strcpy(codes,"ILPSRA");
  else if (neworient>=60 && neworient<=67)
    strcpy(codes,"SPRIAL");
  else
    return newc[0];
      
  int subcode=neworient-10*int(neworient/10);
  int index[3] = { 0,1,2};
  if (subcode >=4)
    index[2]=5;
  if (subcode==2 || subcode==3 || subcode==6 || subcode==7)
    index[1]=4;
  if (subcode==1 || subcode==3 || subcode==5 || subcode==7)
    index[0]=3;

  if (axis<0 || axis>2)
    axis=0;

  fprintf(stderr,"axis=%d index=%d,%d,%d, subcode=%d, neworient=%d\n",
	  axis,index[0],index[1],index[2],subcode,neworient);

  return codes[index[axis]];
}
// ---------------------------------------------------------------------
//  If Generating a JPEG this will tell you if you need to flip any axis
// ---------------------------------------------------------------------
int vtkbisImageHeader::GetJPEGFlip(int plane) 
{
  mat44 original=vtkbisImageHeader::NIFTI_CreateOrientationMatrix(this->Header);
  int icod,jcod,kcod;  nifti_mat44_to_orientation(original,&icod,&jcod,&kcod);

  if (plane<0 || plane>2)
    plane=0;
  
  int oldstyleorient=vtkbisImageHeader::MapOrientationToOldStyle(this->Orientation);

  int outcode=0;
  if (oldstyleorient==0)
    {
      switch(plane)
	{
	case 2:
	  if (icod==NIFTI_L2R) outcode+=1;
	  if (jcod==NIFTI_A2P) outcode+=2;
	  break;
	case 1: 
	  if (icod==NIFTI_L2R) outcode+=1;
	  if (kcod==NIFTI_S2I) outcode+=2;
	  break;
	case 0:
	  if (icod==NIFTI_A2P) outcode+=1;
	  if (jcod==NIFTI_S2I) outcode+=2;
	}
    }
  return outcode;
}
// -----------------------------------------------------------------------------------
int vtkbisImageHeader::GetOrientationFromNIFTIHeader()
{
  // Step 1 : Establish actual rotation matrix

  nifti_1_header* hdr=this->GetRawHeader();
  int orientation=0;

  //   METHOD 1 (the "old" way, used only when qform_code = 0):
  if (hdr->qform_code == 0 && hdr->sform_code==0)
      return 0;


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

  /*  fprintf(stdout,"Orientation icod=%d (%s), jcod=%d (%s), kcod=%d (%s)\n",
	  icod,nifti_orientation_string(icod),
	  jcod,nifti_orientation_string(jcod),
	  kcod,nifti_orientation_string(kcod));*/




  // Step 1 -- establish orientation -- this is the direction where z axis is at

  int needs_transpose=0;
  mat44 transpose_matrix;       vtkbisImageHeader::NIFTI_identity_mat44(&transpose_matrix);


  int done=0;
  /* Moreover the first three are also assumed to be (as far as the viewers are concerned)

  Axial = LPS i.e. i-axis Right-to-Left, j-axis: Anterior-to-Posterior (Front-to-Back), k-axis: Inferior to Superior (Bottom-to-Top)
  Coronal = LIP (Right-to-Left, Superior-to-Inferior, Anterior-to-Posterior)
  Sagital = PSR (Anterior-to-Posterior, Inferior-to-Superior, Left-to-Right)
  */

  if  (kcod==NIFTI_I2S || kcod==NIFTI_S2I) 
    {
      // Axial: Z-axis along k!
      //      fprintf(stderr,"Axial ...");
      if  (icod==NIFTI_L2R || icod==NIFTI_R2L)
	{
	  //	  fprintf(stderr,"Normal Axial ...");
	  orientation=10;
	  if (icod==NIFTI_L2R) orientation+=1;
	  if (jcod==NIFTI_P2A) orientation+=2;
	  if (kcod==NIFTI_S2I) orientation+=4;
	  done=1;
	}
      else if (jcod==NIFTI_L2R || jcod==NIFTI_R2L) 
	{
	  //	  fprintf(stderr,"Transposed Axial ...");
	  // Transposed Axial
	  orientation=40;
	  if (icod==NIFTI_P2A) orientation+=1;
	  if (jcod==NIFTI_L2R) orientation+=2;
	  if (kcod==NIFTI_S2I) orientation+=4;
	  done=1;
	}
    }
  else if  (kcod==NIFTI_A2P || kcod==NIFTI_P2A) 
    {
      //      fprintf(stderr,"Coronal ...");
      if (icod==NIFTI_L2R || icod==NIFTI_R2L)
	{
	  // Coronal
	  //  fprintf(stderr,"Normal Coronal ...");
	  orientation=20;
	  if (icod==NIFTI_L2R) orientation+=1;
	  if (jcod==NIFTI_I2S) orientation+=2;
	  if (kcod==NIFTI_P2A) orientation+=4;
	  done=1;
	}
      else if (jcod==NIFTI_L2R || jcod==NIFTI_R2L)
	{
	  //	  fprintf(stderr,"Transposed Coronal ...");
	  orientation=50;
	  if (icod==NIFTI_I2S) orientation+=1;
	  if (jcod==NIFTI_L2R) orientation+=2;
	  if (kcod==NIFTI_P2A) orientation+=4;
	  done=1;
	}
    }
  else if (kcod==NIFTI_L2R || kcod==NIFTI_R2L)
    {
      //      fprintf(stderr,"Sagital ...");
      if (jcod==NIFTI_I2S || jcod == NIFTI_S2I)
	{
	  // Sagittal
	  //  fprintf(stderr,"Normal Sag ...");
	  orientation=30;
	  if (icod==NIFTI_P2A) orientation+=1;
	  if (jcod==NIFTI_S2I) orientation+=2;
	  if (kcod==NIFTI_R2L) orientation+=4;
	  done=1;
	}
      else if (icod==NIFTI_I2S || icod == NIFTI_S2I)
	{
	  // Transposed Sagittal
	  //	  fprintf(stderr,"Transp Sag ...");
	  orientation=60;
	  if (icod==NIFTI_S2I) orientation+=1;
	  if (jcod==NIFTI_P2A) orientation+=2;
	  if (kcod==NIFTI_R2L) orientation+=4;
	  done=1;
	}
    }

  if (done==0)
    {
      fprintf(stdout,"Confused Orientation icod=%d (%s), jcod=%d (%s), kcod=%d (%s)\n",
	      icod,nifti_orientation_string(icod),
	      jcod,nifti_orientation_string(jcod),
	      kcod,nifti_orientation_string(kcod));
      return -1;
    }

  return orientation;
}
// -------------------------------------------------------------------------------------
int vtkbisImageHeader::ForceHeaderToBioImageSuiteCoordinatesAndReturnFlips(int DataFlip[3],int& Transpose)
{
  DataFlip[0]=0;
  DataFlip[1]=0;
  DataFlip[2]=0;
  Transpose=0;

  nifti_1_header* hdr=this->GetRawHeader();
  int new_orientation=0;
    
  //   METHOD 1 (the "old" way, used only when qform_code = 0):
  if (hdr->qform_code == 0 && hdr->sform_code==0)
    return new_orientation;


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


  double spa[3];
  for (int ia=0;ia<=2;ia++)
    {
      double sum=0.0;
      for (int ib=0;ib<=2;ib++)
		sum+=pow(original.m[ib][ia],2.0f);
      spa[ia]=sqrt(sum);
    }


  //  vtkbisImageHeader::NIFTI_print_mat44("original",original);
  //  fprintf(stderr,"Spacing = %.2f %.2f %.2f\n",spa[0],spa[1],spa[2]);
	 


  int icod,jcod,kcod;  nifti_mat44_to_orientation(original,&icod,&jcod,&kcod);
  
  /*  fprintf(stderr,"Orientation icod=%d (%s), jcod=%d (%s), kcod=%d (%s)\n",
	  icod,nifti_orientation_string(icod),
	  jcod,nifti_orientation_string(jcod),
	  kcod,nifti_orientation_string(kcod));*/
  



  // Step 1 -- establish orientation -- this is the direction where z axis is at

  int needs_transpose=0;
  mat44 transpose_matrix;       vtkbisImageHeader::NIFTI_identity_mat44(&transpose_matrix);

  int newdim[3]; this->GetDimensions(newdim);
  int dim[3];    this->GetDimensions(dim);

  int do_transpose=0;
  int can_handle=0;

  if  (kcod==NIFTI_I2S || kcod==NIFTI_S2I) 
    {
      // Axial: Z-axis along k!
      //      fprintf(stderr,"Axial ...");
      if  (icod==NIFTI_L2R || icod==NIFTI_R2L)
	{
	  //	  fprintf(stderr,"Normal Axial ...");
	  new_orientation=10;
	  if (icod==NIFTI_L2R)	DataFlip[0]=1;
	  if (jcod==NIFTI_P2A)	DataFlip[1]=1;
	  if (kcod==NIFTI_S2I)	DataFlip[2]=1;
	  can_handle=1;
	}
      else if (jcod==NIFTI_L2R || jcod==NIFTI_R2L) 
	{
	  //	  fprintf(stderr,"Transposed Axial ...");
	  // Transposed Axial
	  new_orientation=10;
	  // This is a transposed axial ....
	  // Step 1 -- switch x and y
	  transpose_matrix.m[0][0]=0;      transpose_matrix.m[0][1]=1.0;
	  transpose_matrix.m[1][0]=1.0;      transpose_matrix.m[1][1]=0;
	  newdim[1]=dim[0]; newdim[0]=dim[1];
	  
	  // Step 2 -- check coordinates as before
	  if (jcod==NIFTI_L2R)	DataFlip[1]=1;
	  if (icod==NIFTI_P2A)	DataFlip[0]=1;
	  if (kcod==NIFTI_S2I)	DataFlip[2]=1;
	  
	  do_transpose=1;
	  can_handle=1;
	}
    }
  else if  (kcod==NIFTI_A2P || kcod==NIFTI_P2A) 
    {
      //      fprintf(stderr,"Coronal ...");
      if (icod==NIFTI_L2R || icod==NIFTI_R2L)
	{
	  // Coronal
	  //	  fprintf(stderr,"Normal Coronal ...");
	  new_orientation=20;
	  if (icod==NIFTI_L2R)	DataFlip[0]=1;
	  if (jcod==NIFTI_I2S)	DataFlip[1]=1;
	  if (kcod==NIFTI_P2A)	DataFlip[2]=1;
	  can_handle=1;
	}
      else if (jcod==NIFTI_L2R || jcod==NIFTI_R2L)
	{
	  //	  fprintf(stderr,"Transposed Coronal ...");
	  new_orientation=20;
	  // Step 1 -- switch x and z
	  transpose_matrix.m[0][0]=0;        transpose_matrix.m[0][1]=1.0;
	  transpose_matrix.m[1][0]=1.0;      transpose_matrix.m[1][1]=0;
	  newdim[0]=dim[2]; newdim[2]=dim[0];
	  // Step 2 -- check coordinates as before
	  if (jcod==NIFTI_L2R)	DataFlip[1]=1;
	  if (icod==NIFTI_I2S)	DataFlip[0]=1;
	  if (kcod==NIFTI_P2A)	DataFlip[2]=1;
	  do_transpose=1;
	  can_handle=1;
	}
    }
  else if (kcod==NIFTI_L2R || kcod==NIFTI_R2L)
    {
      //      fprintf(stderr,"Sagital ...");
      if (jcod==NIFTI_I2S || jcod == NIFTI_S2I)
	{
	  // Sagittal
	  //	  fprintf(stderr,"Normal Sag ...");
	  new_orientation=30;
	  if (icod==NIFTI_P2A)	DataFlip[0]=1;
	  if (jcod==NIFTI_S2I)	DataFlip[1]=1;
	  if (kcod==NIFTI_R2L)	DataFlip[2]=1;
	  can_handle=1;
	}
      else if (icod==NIFTI_I2S || icod == NIFTI_S2I)
	{
	  // Transposed Sagittal
	  //	  fprintf(stderr,"Transp Sag ...");
	  new_orientation=30;
	  // Step 1 -- switch y and z
	  transpose_matrix.m[0][1]=0;        transpose_matrix.m[0][1]=1.0;
	  transpose_matrix.m[1][0]=1.0;      transpose_matrix.m[1][1]=0.0;
	  newdim[1]=dim[2]; newdim[2]=dim[1];
	  
	  // Step 2 -- check coordinates as before
	  if (jcod==NIFTI_P2A)	DataFlip[0]=1;
	  if (icod==NIFTI_S2I)	DataFlip[1]=1;
	  if (kcod==NIFTI_R2L)	DataFlip[2]=1;
	  do_transpose=1;
	  can_handle=1;
	}
    }

  if (can_handle==0)
    {
      fprintf(stderr,"Can't handle this orientation ... not sure what to do\n");
      fprintf(stderr,"Orientation icod=%d (%s), jcod=%d (%s), kcod=%d (%s)\n",
	      icod,nifti_orientation_string(icod),
	      jcod,nifti_orientation_string(jcod),
	      kcod,nifti_orientation_string(kcod));
      return new_orientation;
    }

  //  fprintf(stderr,"On to computing flips etc, Orientation=%d (flips=%d,%d,%d) do_transpose=%d\n",
  //	  new_orientation,DataFlip[0],DataFlip[1],DataFlip[2],do_transpose);

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
      if (DataFlip[ia]==1)
	{
	  flip.m[ia][ia]=-1.0;
	  flip.m[ia][3] = newdim[ia]-1;
	}
    }
  /*  fprintf(stderr,"Data Flip=%d,%d,%d tr=%d\n",
      DataFlip[0],DataFlip[1],DataFlip[2],do_transpose);*/
  //  vtkbisImageHeader::NIFTI_print_mat44("flip",flip);
  //  vtkbisImageHeader::NIFTI_print_mat44("trans",transpose_matrix);
  mat44 mapping=vtkbisImageHeader::NIFTI_mat44_mmul(flip,transpose_matrix);
  //  vtkbisImageHeader::NIFTI_print_mat44("mapping",mapping);

  this->ModifyOrientation(mapping,DataFlip,do_transpose);



  if (do_transpose)
    Transpose=1;

  

  return this->Orientation;
}

