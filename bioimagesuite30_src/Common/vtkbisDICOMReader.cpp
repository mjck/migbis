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

#include "vtkbisDICOMReader.h"
#include "vtkMatrix4x4.h"
#include "vtkMath.h"
#include "gdcmImageReader.h"
#include "gdcmDataElement.h"
#include "gdcmByteValue.h"
#include "gdcmSwapper.h"
#include "gdcmUnpacker12Bits.h"
#include "gdcmRescaler.h"
#include "gdcmOrientation.h"
#include "gdcmImageChangePlanarConfiguration.h"
#include <vtkObjectFactory.h>
#include <vtkstd/vector>
#include <vtkstd/string>
#include "igtl_types.h"
#include "sstream"

vtkCxxRevisionMacro(vtkbisDICOMReader, "$Revision: 1.18.2.4 $");
vtkStandardNewMacro(vtkbisDICOMReader);

inline const char *GetStringValueFromTag_BIS(const gdcm::Tag& t, const gdcm::DataSet& ds)
{
  static std::string buffer;
  buffer = "";  // cleanup previous call


  if( ds.FindDataElement( t ) )
    {
      const gdcm::DataElement& de = ds.GetDataElement( t );
      const gdcm::ByteValue *bv = de.GetByteValue();

      if( bv ) // Can be Type 2
	{
	  buffer = std::string( bv->GetPointer(), bv->GetLength() );
	}
    }

  // Since return is a const char* the very first \0 will be considered
  return buffer.c_str();
}

inline double GetValueFromTag_BIS(const gdcm::Tag& t, const gdcm::DataSet& ds)
{
  static std::string buffer;
  buffer = "";  // cleanup previous call


  if( ds.FindDataElement( t ) )
    {
    const gdcm::DataElement& de = ds.GetDataElement( t );
    const gdcm::ByteValue *bv = de.GetByteValue();
    if( bv ) // Can be Type 2
      {
      buffer = std::string( bv->GetPointer(), bv->GetLength() );
      // Will be padded with at least one \0
      }
    }


  // Since return is a const char* the very first \0 will be considered
  return atof(buffer.c_str());
}
// -------------------------------------------------------------------------------------------------------------------
inline int GetIntValueFromTag_BIS(const gdcm::Tag& t, const gdcm::DataSet& ds)
{
  int val=0;

  if( ds.FindDataElement( t ) )
    {
    const gdcm::DataElement& de = ds.GetDataElement( t );
    const gdcm::ByteValue *bv = de.GetByteValue();
    if( bv ) // Can be Type 2
      {
	if (bv->GetLength()<4)
	  {
	    const char* cc=bv->GetPointer();
	    for (int i=0;i<bv->GetLength();i++)
	      {
		val+=int(int(cc[i])*pow(double(256.0),i));
		//		fprintf(stderr,"i=%d, int(cc[i])=%d val=%d\n",i,int(cc[i]),val);
	      }
	  }
      }
    }

  return val;
}

inline double GetDoubleValueFromTag_BIS(const gdcm::Tag& t, const gdcm::DataSet& ds,int offset)
{
  int val=0;

  if( ds.FindDataElement( t ) )
    {
      const gdcm::DataElement& de = ds.GetDataElement( t );
      const gdcm::ByteValue *bv = de.GetByteValue();
      if( bv ) // Can be Type 2
	{
	  const char* cc=bv->GetPointer();
	  double a=(double)cc[offset*4];
	  return a;
	}
    }

  return 0.0;
}

// -------------------------------------------------------------------------------------------------------------------

vtkbisDICOMReader::vtkbisDICOMReader()
{
  this->ReferenceTime=NULL;
  this->StudyUID=NULL;
  this->SeriesUID=NULL;
  this->PName=NULL;
  this->ReferenceTime=NULL;
  this->SetReferenceTime("");
  this->SetStudyUID("None");
  this->SetSeriesUID("None");
  this->SetPName("None");
  this->CreateNIFTIHeader=1;
  this->ImageHeader=NULL;
  this->ZPosition=0.0;
  this->TimeFrame=0.0;
  this->FlipZ=0;
  this->IsMosaic=0;
  this->MosaicWidth=0;
  this->MosaicHeight=0;
  this->MosaicDepth=0;
  this->AddExtensions=1;
  this->DWIMode=0;
  this->DWIBValue=0.0;
  this->DWIDirections[0]=0.0;
  this->DWIDirections[1]=0.0;
  this->DWIDirections[2]=0.0;
}

vtkbisDICOMReader::~vtkbisDICOMReader()
{
  delete [] this->ReferenceTime;
  delete [] this->StudyUID;
  delete [] this->SeriesUID;
  delete [] this->PName;

  if (this->ImageHeader)
    this->ImageHeader->Delete();

}

void bis_Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters )
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


void vtkbisDICOMReader::FillMedicalImageInformation(const gdcm::ImageReader &reader)
{
  vtkbisGDCMImageReader::FillMedicalImageInformation(reader);

  const gdcm::File &file = reader.GetFile();
  const gdcm::DataSet &ds = file.GetDataSet();


  const char* s=GetStringValueFromTag_BIS( gdcm::Tag(0x0054,0x1300), ds);
  if (strlen(s)>0)
    this->SetReferenceTime(s);

  //  fprintf(stderr,"time=%s\n",s);
  
  this->SetStudyUID(GetStringValueFromTag_BIS( gdcm::Tag(0x0020,0x000d), ds));
  this->SetSeriesUID(GetStringValueFromTag_BIS( gdcm::Tag(0x0020,0x000e), ds));
  this->SetTimeFrame(GetValueFromTag_BIS( gdcm::Tag(0x0020,0x0012), ds));
  this->SetPName(GetStringValueFromTag_BIS( gdcm::Tag(0x0008,0x0090), ds));

  
  //   Mosaic Tag
  //  0051,1016

  /*(0008,0081) ST [1Gilbert Street, New Haven  //0EE9E0//, Boston, 06520-8043, US]         # 62,1 Institution Address
(0018,1310) US 64\0\0\64                                          # 8,4 Acquisition Matrix
(0019,100a) US 19                                                 # 2,1 NumberOfImagesInMosaic
(0028,0002) US 1                                                  # 2,1 Samples per Pixel
(0028,0010) US 320                                                # 2,1 Rows
(0028,0011) US 320                                                # 2,1 Columns
(0028,0100) US 16                                                 # 2,1 Bits Allocated
(0028,0101) US 12                                                 # 2,1 Bits Stored
(0028,0102) US 11                                                 # 2,1 High Bit
(0028,0103) US 0                                                  # 2,1 Pixel Representation
(0028,0106) US 0                                                  # 2,1 Smallest Image Pixel Value
(0028,0107)*/
    //    fprintf(stderr,"Add = %d\n",GetIntValueFromTag_BIS( gdcm::Tag(0x0019,0x100A), ds));


  //  fprintf(stderr,"mname3=%d\n",dp);

  std::string  mname=GetStringValueFromTag_BIS( gdcm::Tag(0x0051,0x1016), ds);



  std::string::size_type foundmos=mname.find("MOSAIC");
  this->IsMosaic=0;      this->MosaicWidth=0;      this->MosaicHeight=0; this->MosaicDepth=0;

  if (std::string::npos != foundmos)
    {
      std::string  mname2=GetStringValueFromTag_BIS( gdcm::Tag(0x0051,0x100b), ds);


      std::vector<std::string> v; bis_Tokenize(mname2,v,"*");
      if(v.size()==2) 
	{
	  this->MosaicWidth=atoi(v[0].c_str());
	  this->MosaicHeight=atoi(v[1].c_str());
	  this->IsMosaic=1;
	}

      int dp=GetIntValueFromTag_BIS( gdcm::Tag(0x0019,0x100a), ds);
      if (dp>0)
	this->MosaicDepth=dp;
      /*fprintf(stderr,"Mname=%s . %d x %d x %d  ismosaic=%d other=%s,\n",mname.c_str(),
	      this->MosaicWidth,this->MosaicHeight,this->MosaicDepth,
	      this->IsMosaic,mname2.c_str());*/
    }


  std::string directional=GetStringValueFromTag_BIS(gdcm::Tag(0x0019,0x100D),ds);
  std::string::size_type founddir=directional.find("DIRECTIONAL");
  std::string::size_type foundnone=directional.find("NONE");

  this->DWIMode=0;
  if (std::string::npos != foundnone)
    {
      this->DWIMode=1;
      this->DWIDirections[0]=0.0;
      this->DWIDirections[1]=0.0;
      this->DWIDirections[2]=0.0;
      //      fprintf(stderr,"Baseline  DWI=%s\n",directional.c_str());
    }
  else if (std::string::npos != founddir)
    {
      this->DWIMode=2;
      double dir[3];
      const gdcm::DataElement& dirnorm = ds.GetDataElement( gdcm::Tag(0x0019,0x100e));
      const gdcm::ByteValue *dirval = dirnorm.GetByteValue();
      double *v = (double*)dirval->GetPointer();
      for (int ia=0;ia<=2;ia++)
	this->DWIDirections[ia]=v[ia];

      std::string bvalstr=GetStringValueFromTag_BIS(gdcm::Tag(0x0019,0x100c),ds);
      this->DWIBValue=atof(bvalstr.c_str());
      //      fprintf(stderr,"BValue=%f or %s\n",this->DWIBValue,bvalstr.c_str());
    }
  else
    {
      //      fprintf(stderr,"Non DWI=%s\n",directional.c_str());
    }
  

  
  // This is the big mes from here on ....
  double pos[3]; this->GetImagePositionPatient(pos);
  double orient[6];  this->GetImageOrientationPatient(orient);
  gdcm::Orientation::OrientationType tp = gdcm::Orientation::GetType(orient);
  const char *label = gdcm::Orientation::GetLabel( tp );

  //  fprintf(stderr,"Pos=%f,%f,%f\n",pos[0],pos[1],pos[2]);
  this->X0=pos[0];
  this->Y0=pos[1];
  this->Z0=pos[2];

  double x[3],y[3],z[3];
  // Issues .....
  // DICOM is rotated 180 degrees with respect to NIFTI (i.e. x=-x, y=-y)
  // BUT VTK Flips the Y-axis
  // Hence flip x is all that is needed for axial images!
  //
  // For Coronal/Sagittal God help you
  // Sagittal
  
  for (int ia=0;ia<=2;ia++)
    {
      x[ia]=orient[ia];
      y[ia]=orient[ia+3];
    }


  vtkMath::Normalize(x);
  vtkMath::Normalize(y);
  vtkMath::Cross(x,y,z);

  // Compliance with VTK Flipping
  // Flip X, Y is flipped already
  if (tp==3)
    {
      // Saggital
      for (int ia=0;ia<=2;ia++)
	{
	  y[ia]=-y[ia];
	  x[ia]=-x[ia];
	}
      pos[0]=-pos[0];
      this->ZPosition=pos[0];
      if(z[0]<0.0)
	this->ZPosition=-pos[0];

    }
  else if (tp==2)
    {
      // Coronal
      for (int ia=0;ia<=2;ia++)
	{
	  x[ia]=-x[ia];
	  y[ia]=-y[ia];
	}
      pos[1]=-pos[1];

      this->ZPosition=pos[1];
      if (z[1]<0.0)
	this->ZPosition=-pos[1];
    }

  else
    {
      // Axial
      // Axial      
      for (int ia=0;ia<=2;ia++)
	x[ia]=-x[ia];
      this->ZPosition=pos[2];
      if (z[2]<0.0)
	this->ZPosition=-pos[2];
    }

  //  if (this->CreateNIFTIHeader)
  //    fprintf(stdout,"tp=%d label=%s (%.2f,%.2f,%.2f) x=(%.2f,%.2f,%.2f) y=(%.2f,%.2f,%.2f) z(NFT)=(%.2f,%.2f,%.2f)\n",tp,label,pos[0],pos[1],pos[2],x[0],x[1],x[2],y[0],y[1],y[2],z[0],z[1],z[2]);
  
  

  double zfactor=1;
  if (this->FlipZ)
    zfactor=-1.0;
  
  /*  if (this->CreateNIFTIHeader)
    {
      fprintf(stdout,"\n pos=%.2f,%.2f,%.2f, or=%.2f,%.2f,%.2f \n",
	      pos[0],pos[1],pos[2],
	      orient[0],orient[1],orient[2]);
      if (zfactor)
	for (int ia=0;ia<=2;ia++)
	  z[ia]=zfactor*z[ia];
      fprintf(stdout,"x=%.2f,%.2f,%.2f, y=%.2f,%.2f,%.2f z=%.2f,%.2f,%.2f  Zpos=%.4f\n",
	      x[0],x[1],x[2],
	      y[0],y[1],y[2],
	      z[0],z[1],z[2],
	      this->ZPosition);
	      }*/
  
  
  if (!this->CreateNIFTIHeader)
    return;

  if (this->ImageHeader!=NULL)
    this->ImageHeader->Delete();

  this->ImageHeader=vtkbisImageHeader::New();
  this->ImageHeader->ResetFields();

  nifti_1_header* header=this->ImageHeader->GetRawHeader();
  vtkbisNIFTIExtensionsList* extensions=this->ImageHeader->GetExtensions();

  // Set the Dimensions
  //  for (int ia=0;ia<8;ia++) header->dim[ia]=0;
  int ext[6];  this->GetDataExtent(ext);
  for (int ia=0;ia<=2;ia++)
    header->dim[ia+1]=ext[ia*2+1]-ext[ia*2]+1;

  header->dim[4]=this->GetNumberOfScalarComponents();
  
  //
  int datatype=this->GetDataScalarType();
  header->datatype=vtkbisImageHeader::GetNIFTIDataType(datatype);
  header->bitpix=vtkbisImageHeader::GetBitPix(datatype);

  header->slice_start=0;   /*!< First slice index.    */  /* short dim_un0;       */

    /*!< Grid spacings.        */  /* float pixdim[8];     */
  double sp[3]; this->GetDataSpacing(sp);

  //  if (this->IsMosaic)
    //    fprintf(stderr,"Data Spacing= %.3f,%.3f,%.3f (%.3f)\n",sp[0],sp[1],sp[2],this->ForceZSpacing);

  //if (fabs(this->ForceZSpacing)<0.00001)
    //sp[2]=this->ForceZSpacing;

  /*  for (int ia=1;ia<=3;ia++) 
      header->pixdim[ia]=1.0;//sp[ia-1];*/


  
  header->scl_slope=this->GetScale();
  header->scl_inter=this->GetShift();

  //  fprintf(stderr,"DICOM Shift=%.2f\t, scale=%.2f\n",header->scl_inter,header->scl_slope);

  strcpy(header->descrip,"bisImageHeader from DICOM");   /*!< any text you like.    */  /* char descrip[80];    */
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

  header->srow_x[0]=x[0]*sp[0];
  header->srow_x[1]=y[0]*sp[1];
  header->srow_x[2]=z[0]*sp[2];

  header->srow_y[0]=x[1]*sp[0];
  header->srow_y[1]=y[1]*sp[1];
  header->srow_y[2]=z[1]*sp[2];

  header->srow_z[0]=x[2]*sp[0];
  header->srow_z[1]=y[2]*sp[1];
  header->srow_z[2]=z[2]*sp[2];
  
  header->srow_x[3]=zfactor*pos[0];
  header->srow_y[3]=zfactor*pos[1];
  header->srow_z[3]=zfactor*pos[2];

  header->intent_name[0]=(char)0;
  strcpy(header->magic,"ni1\0");
  this->ImageHeader->MakeStoredHeaderValid();


  // Can we do this here
  if (this->AddExtensions)
    {
      std::stringstream s;
      s << "Original DICOM Header\n";
      s << "Study UID " << this->StudyUID << "\n";
      s << "Series UID " << this->SeriesUID << "\n";
      s << "PNAME " << this->PName << "\n";

      const gdcm::Image &image = reader.GetImage();
      image.Print(s);

      if (this->IsMosaic)
	{
	  s << "Original Image was Siemens Mosaic (fMRI)\n";
	  s << "Mosaic Image Dimensions " << this->MosaicWidth << "x" << this->MosaicHeight <<"\n";
	  s << "Mosaic Number of Slices " << this->MosaicDepth << "\n";
	}
      s << "End of Original DICOM Header\n";
      this->ImageHeader->AddComment(s.str().c_str());

    }
      
  return;
  //  fprintf(stdout,"\n\n");
}

