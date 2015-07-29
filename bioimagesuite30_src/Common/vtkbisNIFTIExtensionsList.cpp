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
#include "vtkbisNIFTIExtensionsList.h"
#include "vtkbisNIFTIExtension.h"
#include "vtkObjectFactory.h"

//------------------------------------------------------------------------------
vtkbisNIFTIExtensionsList* vtkbisNIFTIExtensionsList::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisNIFTIExtensionsList");
  if(ret)
    {
      return (vtkbisNIFTIExtensionsList*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisNIFTIExtensionsList;
}
// ----------------------------------------------------
vtkbisNIFTIExtensionsList::vtkbisNIFTIExtensionsList()
{
  this->ExtensionsCollection=vtkCollection::New();
  
}
// ----------------------------------------------------
vtkbisNIFTIExtensionsList::~vtkbisNIFTIExtensionsList()
{
  this->ExtensionsCollection->Delete();
}

// ----------------------------------------------------
int vtkbisNIFTIExtensionsList::ReadExtensions(znzFile fin,int remain)
{
  // We start with the magic number !
  this->Cleanup();
  if (remain<16)
    {
      //      fprintf(stderr,"Not Enough Room for extensions remain=%d (<20)\n",remain);
      return 0;
    }

  char extension[4];
  int count=znzread(&extension[0],1,4,fin);
  if (count <4)
    {
      //      fprintf(stderr,"Failed to read Header\n");
      return 0;
    }

  if (extension[0]!=1)
    {
      //      fprintf(stderr,"Extension Header is not 1\n");
      return 0;
    }


  remain-=4;
  //  fprintf(stderr,"Remaining =%d\n",remain);
  int maxlength=10000;
  //  fprintf(stderr,"On to Memory Allocation\n");
  char* buffer=new char[maxlength];
  //  fprintf(stderr,"Did Memory Allocation Fail\n");

  while (remain>16)
    {
      //      fprintf(stderr,"Here we go %d\n",remain);
      int esize,ecode;
      znzread(&esize,4,1,fin);
      znzread(&ecode,4,1,fin);
      //      fprintf(stderr,"This Far %d %d\n",esize,ecode);
      int stringlength=esize-8;
      if (stringlength>maxlength)
	{
	  delete [] buffer;
	  maxlength=stringlength;
	  buffer=new char[maxlength];
	}
      memset(buffer,0,maxlength);
      //  fprintf(stderr,"String Length=%d\n",stringlength);
      znzread(buffer,1,stringlength,fin);
      remain=remain-(esize);
      //      fprintf(stderr,"Added Extension (%d,%d) remain=%d \n",esize,ecode,remain);
      this->AddExtension(buffer,ecode);
    }
  //  fprintf(stderr,"Deleting Buffer\n");
  delete [] buffer;
  //  fprintf(stderr,"Deleting Buffer Done\n");
  return this->GetNumberOfExtensions();

}

// --------------------------------------------------------------------------------
int vtkbisNIFTIExtensionsList::WriteExtensions(znzFile fdf)
{
  int next=this->GetNumberOfExtensions();
  int total=0;
  if (next==0)
    {
      char exten[4]= { 0,0,0,0};
      total += znzwrite(&exten[0],1,4,fdf);
      //      total += compressed ? gzwrite(fdf,&exten[0],4): 
      return total;
    }

  //  fprintf(stderr,"Writing Extensions\n");
  char exten[4]= { 1,0,0,0};
  //  total += compressed ? gzwrite(fdf,&exten[0],4): fwrite(&exten[0],1,4,(FILE *)fdf);
  total += znzwrite(&exten[0],1,4,fdf);
  //  fprintf(stderr,"Writing Extensions total=%d\n");  

  for (int i=0;i<next;i++)
    {
      vtkbisNIFTIExtension* extn=this->GetRawExtension(i);
      int esize=extn->GetESize();
      if (esize>0)
	total+=extn->WriteExtension(fdf);
      else
	fprintf(stderr,"Not writing extension %d\n",i);
    }
  return total;
}

int  vtkbisNIFTIExtensionsList::AddExtension(const char* comment,int ecode)
{
  if (comment==NULL)
    return 0;

  vtkbisNIFTIExtension* exn=vtkbisNIFTIExtension::New();
  exn->SetData(comment,ecode);
  this->ExtensionsCollection->AddItem(exn);
  //  exn->Delete();

  //  fprintf(stderr,"Num Extensions = %d\n\n\n\n",this->GetNumberOfExtensions());
  return 1;
}

const char* vtkbisNIFTIExtensionsList::GetExtension(int index)
{
  vtkbisNIFTIExtension* ext=this->GetRawExtension(index);
  if (ext==NULL)
    return NULL;

  return ext->GetText();
}

int vtkbisNIFTIExtensionsList::GetNumberOfExtensions()
{
  return this->ExtensionsCollection->GetNumberOfItems();
}
  
int vtkbisNIFTIExtensionsList::Copy(vtkbisNIFTIExtensionsList* other)
{
  if (other==NULL)
    return 0;

  this->ExtensionsCollection->RemoveAllItems();
  int num_ext=other->GetNumberOfExtensions();
  for (int i=0;i<num_ext;i++)
    {
      //      fprintf(stderr,"\n ****************** Copying extension %d/%d \n",i,num_ext);
      vtkbisNIFTIExtension* orig=other->GetRawExtension(i);
      vtkbisNIFTIExtension* exn=vtkbisNIFTIExtension::New();
      exn->Copy(orig);
      //      orig->PrintAll();
      //      exn->PrintAll();
      this->ExtensionsCollection->AddItem(exn);
      //      exn->Delete();
    }
	return num_ext;
}

// Description:
// Reset
int vtkbisNIFTIExtensionsList::Cleanup()
{
  this->ExtensionsCollection->RemoveAllItems();
  return 1;
}

vtkbisNIFTIExtension* vtkbisNIFTIExtensionsList::GetRawExtension(int i)
{
  if (i>=this->GetNumberOfExtensions() || i<0)
    return NULL;

  return (vtkbisNIFTIExtension*)this->ExtensionsCollection->GetItemAsObject(i);

}

int vtkbisNIFTIExtensionsList::GetTotalByteSize()
{
  int next=this->GetNumberOfExtensions(); 
  int total=0;
  for (int i=0;i<next;i++)
    {
      vtkbisNIFTIExtension* extn=this->GetRawExtension(i);
      total=total+extn->GetESize();
    }
  return total;
}


void vtkbisNIFTIExtensionsList::PrintAll()
{
  int next=this->GetNumberOfExtensions(); 
  //  std::cerr << "Total Number of Bytes = " << this->GetTotalByteSize() << "(" << next <<" extensions)\n";

  for (int i=0;i<next;i++)
    {
      vtkbisNIFTIExtension* extn=this->GetRawExtension(i);
      extn->PrintAll();
    }
}

