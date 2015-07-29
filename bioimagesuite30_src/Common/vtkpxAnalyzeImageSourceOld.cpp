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







#include "vtkpxAnalyzeImageSourceOld.h"
#include "vtkUnsignedShortArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkShortArray.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkObjectFactory.h"
#include "vtkpxImageComponentsToFrames.h"
#include "vtkPointData.h"
#include "vtkImageShiftScale.h"
#include "vtkInformation.h"
#include "vtkStreamingDemandDrivenPipeline.h"

using namespace bioimagesuite_legacy;

//------------------------------------------------------------------------------
vtkpxAnalyzeImageSourceOld* vtkpxAnalyzeImageSourceOld::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxAnalyzeImageSourceOld");
  if(ret)
    {
      return (vtkpxAnalyzeImageSourceOld*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxAnalyzeImageSourceOld;
}

// Construct object with NULL file prefix; file pattern "%s.%d"; image range 
// set to (1,1); data origin (0,0,0); data spacing (1,1,1); no data mask;
// header size 0; and byte swapping turned off.

vtkpxAnalyzeImageSourceOld::vtkpxAnalyzeImageSourceOld()
{
  this->FilePrefix = NULL;
  this->FilePattern = new char[strlen("%s.%d") + 1];
    strcpy (this->FilePattern, "%s.%d");
  this->ImageRange[0] = this->ImageRange[1] = 1;
  this->DataOrigin[0] = this->DataOrigin[1] = this->DataOrigin[2] = 0.0;
  this->DataSpacing[0] = this->DataSpacing[1] = this->DataSpacing[2] = 1.0;

  this->HeaderSize = 0;
  this->SwapBytes = 0;
  this->FileType=2;
  this->DataDimensions[0] = this->DataDimensions[1] = 0;
  this->SetFilePattern ("%s.img");
  this->read_volume_ok=0;
  this->AnalyzeMode=1;
  this->NumTimeFrames=1;
  this->Orientation=0; // Axial
  this->ForceOutputToShort=0;
  this->MultipleFramesAsComponents=1;

  this->SetNumberOfInputPorts(0);

}


vtkpxAnalyzeImageSourceOld::~vtkpxAnalyzeImageSourceOld()
{
  if (this->FilePrefix)
    delete [] this->FilePrefix;
    
  delete [] this->FilePattern;
}

//------------------------------------------------------------------------------
// Specify the input data or filter.

// Read an analyze header file
// ---------------------------
int vtkpxAnalyzeImageSourceOld::ReadHeaderFile()
{
  anaimagedata *hdr=new anaimagedata;

  int bits=readanahdr(this->FilePrefix,hdr);

  if (bits==0)
      {
	delete hdr;
	return 0;
      }

  this->read_volume_ok=1;
  this->SetDataDimensions(hdr->xsize,hdr->ysize);
  this->SetImageRange(1,hdr->zsize*hdr->tsize);
  this->SetNumTimeFrames(hdr->tsize);

  if (hdr->xpix>0.01)
      {
	this->SetDataSpacing(hdr->xpix,hdr->ypix,hdr->zpix);
      }
  else
      {
	this->SetDataSpacing(1.0,1.0,1.0);
      }
  
  if (bits<0)
    {
      bits=-bits;
      this->SwapBytes = 1;
    }
  else
      this->SwapBytes = 0;

  this->Orientation=hdr->orient;
  //  fprintf(stderr,"Reading Image Orientation =%d\n",this->Orientation);

  if (bits==1)
    this->FileType=0;
  else if (bits<=8)
    this->FileType=1;
  else if (bits==32)
    this->FileType=4;
  else if (bits==31)
    this->FileType=3;
  else 
    this->FileType=2;

  delete hdr;
  this->AnalyzeMode=1;
  return bits;
}


void vtkpxAnalyzeImageSourceOld::SetHeaderInfo(int width,int height,int depth,int swap,int bytes)
{
  this->read_volume_ok=1;
  this->SetDataDimensions(width,height);
  this->SetImageRange(1,depth);

  if (swap)
      this->SwapBytes=1;
  else
      this->SwapBytes=0;

  this->FileType=Irange(bytes,1,4);
  /*  if (this->FileType==3)
      this->FileType=1;*/

  this->AnalyzeMode=0;
}


void vtkpxAnalyzeImageSourceOld::SetHeaderInfo(int width,int height,int depth,int nframes,int swap,int bytes)
{
  if (nframes>0)
      {
	this->SetHeaderInfo(width,height,depth*nframes,swap,bytes);
	this->NumTimeFrames=nframes;
      }
  else
      {
	this->SetHeaderInfo(width,height,depth,swap,bytes);
	this->NumTimeFrames=1;
      }
}

void vtkpxAnalyzeImageSourceOld::SetVoxelDimensions(float vox1,float vox2,float vox3)
{
  this->SetDataSpacing(vox1,vox2,vox3);
}

int  vtkpxAnalyzeImageSourceOld::GetNumTimeFrames()
{
  return this->NumTimeFrames;
}

void vtkpxAnalyzeImageSourceOld::SetNumTimeFrames(int t)
{
  if (t<1)
      return;

  this->NumTimeFrames=t;
}


int vtkpxAnalyzeImageSourceOld::GetNumSlices()
{
  int range[2]; 
  this->GetImageRange(range);

  int s=range[1]-range[0]+1;
  if (s==1)
      return 1;
  else
      return s/this->NumTimeFrames;

}

vtkDataArray* vtkpxAnalyzeImageSourceOld::ReadVolume(int first, int last)
{

  if (this->read_volume_ok==0)
      {
	vtkDebugMacro(<<"Not updating");
	return NULL;      
      }

  if (!this->FilePrefix)
    return NULL;


  char* filename;
  if (this->AnalyzeMode==1)
      {
	filename=anacreateimagefilename(FilePrefix);
      }
  else
      {
	filename=new char[(strlen(FilePrefix))+1];
	strcpy(filename,FilePrefix);
      }


  int numPts = this->DataDimensions[0] * this->DataDimensions[1];
  int numberSlices = last - first+1;

  vtkDataArray *scalars = NULL;
  char*  charslice=NULL;
  short *slice=NULL;
  float *floatslice=NULL;
  int   *intslice=NULL;

  if (FileType==1 || FileType==0)
      {
	scalars = vtkDataArray::CreateDataArray(VTK_UNSIGNED_CHAR);
	scalars->SetNumberOfComponents(1);
	scalars->SetNumberOfTuples(numPts * numberSlices);
	charslice = (char *)scalars->GetVoidPointer(0);
      }
  else if (FileType==2)
      {
	scalars = vtkDataArray::CreateDataArray(VTK_SHORT);
	scalars->SetNumberOfComponents(1);
	scalars->SetNumberOfTuples(numPts * numberSlices);
	slice = (short *)scalars->GetVoidPointer(0);
      }
  else if (FileType==3)
    {
      scalars = vtkDataArray::CreateDataArray(VTK_INT);
      scalars->SetNumberOfComponents(1);
      scalars->SetNumberOfTuples(numPts * numberSlices);
      intslice = (int *)scalars->GetVoidPointer(0);
    }
  else
    {
      scalars = vtkDataArray::CreateDataArray(VTK_FLOAT);
      scalars->SetNumberOfComponents(1);
      scalars->SetNumberOfTuples(numPts * numberSlices);
      floatslice = (float *)scalars->GetVoidPointer(0);
    }
  
  //  fprintf(stderr,"Number of Tuples (%d)\n",scalars->GetNumberOfTuples());

  gzFile fp = gzsuffixopen(filename,"rb");
  
  if ( fp==Z_NULL)
      {
	char* filename2=anacreatefilename(filename,".IMG",".img");
	fp=gzsuffixopen(filename2,"rb"); 
	//	fprintf(stderr,"Trying %s instead of %s\n",filename2,filename);
	delete [] filename2;
	
	if ( fp==Z_NULL)
	  {
	    delete [] filename;
	    vtkErrorMacro(<< "Cannot Open File " << filename );
	    return NULL;
	  }
      }

  this->UpdateProgress(0.0);
  float oldpog=0.0;

  int offsetposition=0;

  for (int sliceNumber = first; sliceNumber <= last; sliceNumber++) 
    {
      
      int skip=0;
      if (sliceNumber==first)
	{
	  int sz=this->FileType;
	  if (FileType==3)
	    sz=4;
	  if (first!=1)
	    skip=this->DataDimensions[0]*this->DataDimensions[1]*(first-1)*sz;
	  skip+=this->HeaderSize;
	}
      
      
      float pog=float(sliceNumber-first)/float(last-first+1);
      //fprintf(stderr,"Progress=%f\n",pog);
      if ( (pog-oldpog)>0.1)
	{
	  this->UpdateProgress(pog);
	  oldpog=pog;
	}
      
      if (FileType==1 || FileType == 0 )
	ReadCharBinaryImage (fp, &charslice[offsetposition], this->DataDimensions[0], 
			     this->DataDimensions[1], 
			     skip);
      else if (FileType==2 )
	ReadBinaryImage (fp, &slice[offsetposition], this->DataDimensions[0], 
			 this->DataDimensions[1], 
			 skip,this->SwapBytes);
      else if (FileType==3)
	ReadIntBinaryImage (fp, &intslice[offsetposition], this->DataDimensions[0], 
			    this->DataDimensions[1], 
			    skip,this->SwapBytes);
      else
	this->ReadFloatBinaryImage (fp, &floatslice[offsetposition], this->DataDimensions[0], 
				    this->DataDimensions[1], 
				    skip,this->SwapBytes);
      offsetposition+=numPts;	      
    }
    

  gzclose (fp);
  
  delete [] filename;
  
  this->UpdateProgress(1.0);
  this->read_volume_ok=0;

  if (Debug)
    {
      double rv[2];
      scalars->GetRange(rv);
      
      int dtype=scalars->GetDataType();
      int bits2;
      if (dtype==VTK_UNSIGNED_CHAR || dtype==VTK_CHAR)
	bits2=8;
      else if (dtype==VTK_SHORT || dtype==VTK_UNSIGNED_SHORT)
	bits2=16;
      else if (dtype==VTK_INT)
	bits2=31;
      else if (dtype==VTK_FLOAT)
	bits2=32;
      
      char line[180];
      sprintf(line,"\n%s\n Dimensions %d x %d x %d (%d) (%5.2f %5.2f %5.2f)\n Bits=%d (%d) Swap=%d Range=%f:%f\n",
	      this->FilePrefix,this->DataDimensions[0],this->DataDimensions[1],this->ImageRange[1]-this->ImageRange[0]+1,numberSlices,
	      this->DataSpacing[0],this->DataSpacing[1],this->DataSpacing[2],
	      8*FileType,bits2,this->SwapBytes,rv[0],rv[1]);
      fprintf(stderr,"out=%s\n",line);
    }  
  return scalars;
}
//------------------------------------------------------------------------------
int vtkpxAnalyzeImageSourceOld:: ReadBinaryImage (gzFile fp,  short *pixels, int xsize, 
					       int ysize, int skip, int swapBytes)
{

  int numShorts = xsize * ysize;
  
  if (skip>0)
    gzseek (fp, skip, 0);
  
  gzread(fp,pixels,numShorts*2);

  if (swapBytes && FileType==2) 
      {
	unsigned char *bytes = (unsigned char *) pixels;
	unsigned char tmp;
	int i;
	for (i = 0; i < numShorts; i++, bytes += 2) 
	    {
	      tmp = *bytes; 
	      *bytes = *(bytes + 1); 
	      *(bytes + 1) = tmp;
	    }
      }


  return 1;
}
//------------------------------------------------------------------------------
int vtkpxAnalyzeImageSourceOld:: ReadCharBinaryImage (gzFile fp,  char *pixels, int xsize, 
						   int ysize, int skip)
{

  int numChars = xsize * ysize;
  
  if (skip>0)
    gzseek (fp, skip, 0);
  
  if (FileType==0)
    {
      const unsigned char mask[8]={ 1 , 2 , 4 , 8 , 16 , 32 , 64 , 128 };
      int numBits=numChars;
      int numChars2=(numBits+7)/8;
      unsigned char *charPtr=new unsigned char[numChars];
      gzread(fp,&charPtr[0],numChars2);
      for (int index=0;index<numChars2;index++)
	{
	  int i=index/8;
	  int j=7-(index-i*8);
	  pixels[index]=(char) (( charPtr[i] & mask[j] )>0);
	}
      delete [] charPtr;
    }
  else
    {
      gzread(fp,pixels,numChars);
    }


  return 1;
}
//------------------------------------------------------------------------------

int vtkpxAnalyzeImageSourceOld:: ReadFloatBinaryImage (gzFile fp,  float *pixels, int xsize, 
					      int ysize, int skip, int swapBytes)
{
  int numFloats = xsize * ysize;
  
  if (skip)
      gzseek (fp, skip, 0);
 
  gzread(fp,&pixels[0],numFloats*4);


  if (swapBytes) 
      {
	unsigned char *bytes = (unsigned char *) pixels;
	unsigned char tmp;
	int i;
	for (i = 0; i < numFloats; i++, bytes += 4) 
	    {
	      tmp = *bytes; 
	      *bytes = *(bytes + 3); 
	      *(bytes + 3) = tmp;

	      tmp = *(bytes+1);
	      *(bytes+1)=*(bytes+2);
	      *(bytes+2)=tmp;
	    }
      }
    
  return 1;
}

//------------------------------------------------------------------------------
int vtkpxAnalyzeImageSourceOld:: ReadIntBinaryImage (gzFile fp,  int *pixels, int xsize, 
						  int ysize, int skip, int swapBytes)
{
  int numInts = xsize * ysize;
  
  if (skip)
      gzseek (fp, skip, 0);
 
  gzread(fp,&pixels[0],numInts*4);


  if (swapBytes) 
      {
	unsigned char *bytes = (unsigned char *) pixels;
	unsigned char tmp;
	int i;
	for (i = 0; i < numInts; i++, bytes += 4) 
	    {
	      tmp = *bytes; 
	      *bytes = *(bytes + 3); 
	      *(bytes + 3) = tmp;

	      tmp = *(bytes+1);
	      *(bytes+1)=*(bytes+2);
	      *(bytes+2)=tmp;
	    }
      }
    
  return 1;
}

//------------------------------------------------------------------------------
void vtkpxAnalyzeImageSourceOld::Execute()
{

  if (this->FilePrefix == NULL) 
      {
	vtkErrorMacro(<< "FilePrefix is NULL");
	return;
      }
  
  if (this->HeaderSize < 0) 
      {
	vtkErrorMacro(<< "HeaderSize " << this->HeaderSize << " must be >= 0");
	return;
      }
  
  if (this->DataDimensions[0] <= 0 || this->DataDimensions[1] <= 0) 
      {
	vtkErrorMacro(<< "x, y dimensions " << this->DataDimensions[0] << ", " << this->DataDimensions[1] 
		      << "must be greater than 0.");    
	return;
      } 
  
  int first = this->ImageRange[0];
  int last = this->ImageRange[1];

  if (this->FileType==0)
    first=1;

  vtkDataArray* tmpScalars = this->ReadVolume(first, last);
  vtkDataArray* newScalars=tmpScalars;
  int deletenewscalars=0;


  //  fprintf(stderr,"Read %d x %d scalars\n",tmpScalars->GetNumberOfTuples(),tmpScalars->GetNumberOfComponents());
  
  if (!tmpScalars)
    return;

  if (this->ForceOutputToShort==1)
    {
      if (newScalars->GetDataType()==VTK_FLOAT)
	{
	  newScalars=vtkDataArray::CreateDataArray(VTK_SHORT);
	  newScalars->DeepCopy(tmpScalars);
	  deletenewscalars=1;
	}
    }

  int dimensions[3]; 
  dimensions[0] = this->DataDimensions[0];
  dimensions[1] = this->DataDimensions[1];
  dimensions[2] = this->ImageRange[1] - this->ImageRange[0] + 1;

  vtkImageData* output=this->GetOutput();
  output->SetSpacing(this->DataSpacing);
  output->SetOrigin(this->DataOrigin);
  output->SetScalarType(newScalars->GetDataType());
  
  if (this->MultipleFramesAsComponents==1 && this->NumTimeFrames>1)
    {
      int dim[3];
      for (int ia=0;ia<=2;ia++)
	dim[ia]=dimensions[ia];
      dim[2]/=this->NumTimeFrames;
  
      output->SetNumberOfScalarComponents(this->NumTimeFrames);
      output->SetDimensions(dim);
      output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
      output->AllocateScalars();
      
      for (int comp=0;comp<NumTimeFrames;comp++)
	{
	  int index=comp*dim[0]*dim[1]*dim[2];
	  for (int k=0;k<dim[2];k++)
	    for (int j=0;j<dim[1];j++)
	      for (int i=0;i<dim[0];i++)
		{
		  output->SetScalarComponentFromDouble(i,j,k,comp,newScalars->GetComponent(index,0));
		  ++index;
		}
	}
      
    }
  else
    {
      output->SetDimensions(dimensions);
      output->SetWholeExtent(0,dimensions[0]-1,0,dimensions[1]-1,0,dimensions[2]-1);
      output->SetNumberOfScalarComponents(newScalars->GetNumberOfComponents());
      output->GetPointData()->SetScalars(newScalars);

    }
     
  if (deletenewscalars)
    newScalars->Delete();
  tmpScalars->Delete();
    


}
//------------------------------------------------------------------------------
void vtkpxAnalyzeImageSourceOld::ExecuteInformation()
{
  //  fprintf(stderr,"IN Execute Information\n");
  vtkImageData *output = this->GetOutput();

  
  int dim[3];
  dim[0]=this->DataDimensions[0];
  dim[1]=this->DataDimensions[1];

  //  output->DebugOn();

  if (this->MultipleFramesAsComponents)
    {
      output->SetNumberOfScalarComponents(this->NumTimeFrames);
      dim[2]=(this->ImageRange[1]-this->ImageRange[0]+1)/this->NumTimeFrames;
    }
  else
    {
      output->SetNumberOfScalarComponents(1);
      dim[2]=(this->ImageRange[1]-this->ImageRange[0]+1);
    }

  output->SetWholeExtent(0, dim[0]-1, 0, dim[1]-1, 0, dim[2]-1);
  output->SetSpacing(this->DataSpacing);
  output->SetOrigin(this->DataOrigin);
  output->SetDimensions(dim);
  
  if (this->FileType<1)
    output->SetScalarType(VTK_UNSIGNED_CHAR);
  else if (this->FileType==2)
    output->SetScalarType(VTK_SHORT);
  else
    output->SetScalarType(VTK_FLOAT);

  //  output->DebugOff();
  //  fprintf(stderr,"IN Execute Information Done\n");
}

//------------------------------------------------------------------------------
int vtkpxAnalyzeImageSourceOld::Load(const char *ana)
{
  this->SetFilePrefix(ana);
  int ok=this->ReadHeaderFile();
  if (ok!=0)
      this->Update();
  return ok;
}
//------------------------------------------------------------------------------
int vtkpxAnalyzeImageSourceOld::FillOutputPortInformation(int port, vtkInformation* info)
{
  if(!this->Superclass::FillOutputPortInformation(port, info))
    {
    return 0;
    }
  info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkImageData");
  return 1;
}


