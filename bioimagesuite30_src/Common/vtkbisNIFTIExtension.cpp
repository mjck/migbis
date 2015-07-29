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

#include "vtkObjectFactory.h"
#include "vtkbisNIFTIExtension.h"
#include "sstream"
using namespace std;
//------------------------------------------------------------------------------
vtkbisNIFTIExtension* vtkbisNIFTIExtension::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisNIFTIExtension");
  if(ret)
    {
      return (vtkbisNIFTIExtension*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisNIFTIExtension;
}
// ----------------------------------------------------
vtkbisNIFTIExtension::vtkbisNIFTIExtension()
{
  this->ESize=0;
  this->ECode=0;
  this->Edata="Dummy Extension";
}
// ----------------------------------------------------
vtkbisNIFTIExtension::~vtkbisNIFTIExtension()
{
}
// ----------------------------------------------------
void vtkbisNIFTIExtension::SetData(const char* extension,int ecode)
{
  this->SetXMLData(extension);
  this->ECode=ecode;
}

void vtkbisNIFTIExtension::SetXMLData(const char* extension)
{
  if (extension==NULL)
    return;

  std::stringstream s;
  s << extension;

  //this->Edata=extension;
  int inplen=strlen(extension);
  int temp=inplen+8;
  int newlen=int(temp/16)*16;
  if (newlen<temp)
    newlen+=16;

  //  for (int i=inplen;i<this->ESize-8;i++)
  //    s << ".";
  this->Edata=s.str();
  this->ESize=newlen;
  //  fprintf(stderr,"Added %d (%s)\n",this->ESize,this->Edata.c_str());
  return;
}

const char* vtkbisNIFTIExtension::GetText()
{
  return this->Edata.c_str();
}


int vtkbisNIFTIExtension::Copy(vtkbisNIFTIExtension* old)
{
  if (old==NULL)
    return 0;
  
  const char* txt=old->GetText();
  if (txt==NULL)
    return 0;

  this->ESize=old->GetESize();
  this->ECode=old->GetECode();
  this->SetXMLData(txt);

  return 1;
}


void vtkbisNIFTIExtension::PrintAll()
{
  fprintf(stderr,"Extension: %s  (%d,%d) \n",this->Edata.c_str(),this->ESize,this->ECode);
}

int vtkbisNIFTIExtension::WriteExtension(znzFile fdf)
{
  int total=0;
  total += znzwrite(&this->ESize,4,1,fdf)*sizeof(this->ESize);
  total += znzwrite(&this->ECode,4,1,fdf)*sizeof(this->ECode);
  //  fprintf(stderr,"Total Header = %d %d\n",total,sizeof(this->ESize));
  int n=this->Edata.length();
  total += znzwrite(this->Edata.c_str(),1,this->Edata.length(),fdf);
  //  fprintf(stderr,"Writing edata=%d \"%s\"\n total=%d vs %d",n,this->Edata.c_str(),total,this->ESize);

  char s[1];  s[0]=(char)0;

  while(total<this->ESize)
    //    {
      total+=znzwrite(&s,1,1,fdf);
  //fprintf(stderr,"Adding zeros total=%d vs %d\n",total,this->ESize);
  //}

  if (total!=this->ESize)
    fprintf(stderr,"Error not enough bytes %d vs %d\n",total,this->ESize);

  
  return total;
}

