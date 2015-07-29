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
  Module:    $RCSfile: vtkpxSignaLXImageSource.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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
#include "vtkpxSignaLXImageSource.h"
#include "vtkUnsignedShortArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkShortArray.h"
#include "vtkFloatArray.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "pxutil.h"

//------------------------------------------------------------------------------
vtkpxSignaLXImageSource* vtkpxSignaLXImageSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxSignaLXImageSource");
  if(ret)
    {
      return (vtkpxSignaLXImageSource*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxSignaLXImageSource;
}

// Construct object with NULL file prefix; file pattern "%s.%d"; image range 
// set to (1,1); data origin (0,0,0); data spacing (1,1,1); no data mask;
// header size 0; and byte swapping turned off.

vtkpxSignaLXImageSource::vtkpxSignaLXImageSource()
{
  this->SetFilePattern ("%s%d");
}

//------------------------------------------------------------------------------
// Read signa header file
// ---------------------------
int vtkpxSignaLXImageSource::ReadHeaderFile()
{

  anaimagedata *hdr=new anaimagedata;

  char filename[1024];
  if (this->FilePattern)
    {
      sprintf (filename, this->FilePattern, this->FilePrefix, ImageRange[0]);
    }
  else
    {
      sprintf (filename, this->FilePattern, ImageRange[0]);
    }

  int bits=readlxhdr(filename,hdr);

  if (bits==0)
      {
	delete hdr;
	return 0;
      }

  this->read_volume_ok=1;
  this->SetDataDimensions(hdr->xsize,hdr->ysize);
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
    this->SwapBytes = 1;
  else
    this->SwapBytes = 0;
  
  this->Orientation=hdr->orient;
  this->FileType=2;
  
  delete hdr;

  
  vtkDebugMacro(<<"Read Header " << this->FilePrefix << " Dime="<<this->DataDimensions[0]<<"*"<<this->DataDimensions[0]<<" Orient= "<<this->Orientation<< " Swap= " << this->SwapBytes);
  
  return bits;
}

vtkDataArray* vtkpxSignaLXImageSource::ReadVolume(int first, int last)
{
  
  this->MultipleFramesAsComponents=0;
  if (this->read_volume_ok==0)
      {
	vtkDebugMacro(<<"Not updating");
	return NULL;      
      }

  if (!this->FilePrefix)
    return NULL;

  
  int numPts = this->DataDimensions[0] * this->DataDimensions[1];
  int numberSlices = last - first+1;

  vtkShortArray* scalars = vtkShortArray::New();
  scalars->SetNumberOfComponents(1);
  scalars->SetNumberOfTuples(numPts * numberSlices);
  short *slice = ((vtkShortArray *)scalars)->WritePointer(0, numPts*numberSlices);

  this->UpdateProgress(0.0);
  float oldpog=0.0;

  int offsetposition=0;
  int skip=0;
  for (int sliceNumber = first; sliceNumber <= last; sliceNumber++) 
    {
      char filename[1024];
      if (this->FilePattern)
	{
	  sprintf (filename, this->FilePattern, this->FilePrefix, sliceNumber);
	}
      else
	{
	  sprintf (filename, this->FilePattern, sliceNumber);
	}
      
      if (sliceNumber == first)
	{ 
	  // Figure Skip stuff once and for all 
	  gzFile fp2 = gzsuffixopen(filename,"rb");
	  if (fp2==0)
	    {
	      vtkErrorMacro(<<"Can't find file: " << filename);
	      scalars->Delete();
	      return NULL;
	    }
	  char t;
	  int flag=0,total=0;
	  while (flag==0)
	    {
	      if (gzread(fp2,&t,1)!=0)
		++total;
	      else
		flag=1;
	    }
	  gzclose(fp2);
	  skip=total-numPts*2;
	  vtkDebugMacro(<< "File Size = " << total << " Skip = " << skip << " (numPts="<<numPts<<")");
	}
      
      gzFile fp = gzsuffixopen(filename,"rb");
      if (fp==0)
	{
	  vtkErrorMacro(<<"Can't find file: " << filename);
	  scalars->Delete();
	  return NULL;
	}
      
      vtkDebugMacro ( << "Reading " << filename );
      
      float pog=float(sliceNumber-first)/float(last-first+1);
      if ( (pog-oldpog)>0.1)
	{
	  this->UpdateProgress(pog);
	  oldpog=pog;
	}
      
      ReadBinaryImage (fp, &slice[offsetposition], 
		       this->DataDimensions[0], 
		       this->DataDimensions[1], 
		       skip,this->SwapBytes);
      offsetposition+=numPts;	      
      gzclose (fp);
    }
  
  
  this->UpdateProgress(1.0);
  this->read_volume_ok=0;

  /*  if (Debug)
      {*/
      double rv[2];
      scalars->GetRange(rv);
      int bits2=16;
               
      char line[180];
      sprintf(line,"\n%s\n Dimensions %d x %d x %d (%d) (%5.2f %5.2f %5.2f)\n Bits=%d (%d) Swap=%d Range=%f:%f\n",
	      this->FilePrefix,this->DataDimensions[0],this->DataDimensions[1],this->ImageRange[1]-this->ImageRange[0]+1,numberSlices,
	      this->DataSpacing[0],this->DataSpacing[1],this->DataSpacing[2],
	      8*FileType,bits2,this->SwapBytes,rv[0],rv[1]);
      vtkDebugMacro(<<line);
      //}
  return scalars;
}
//------------------------------------------------------------------------------
int vtkpxSignaLXImageSource::Load(const char *ana)
{
  return this->Load(ana,1,1);
}

int vtkpxSignaLXImageSource::Load(const char *ana,int begin,int end)
{
  this->SetFilePrefix(ana);
  this->SetImageRange(begin,end);
  int ok=this->ReadHeaderFile();
  if (ok!=0)
      this->Update();
  return ok;
}
//------------------------------------------------------------------------------
void vtkpxSignaLXImageSource::Execute()
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

