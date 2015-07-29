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


#include "vtkbisProtocol.h"
#include "vtkObjectFactory.h"

using namespace std;


vtkbisProtocol* vtkbisProtocol::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisProtocol");
  if(ret)
    {
      return (vtkbisProtocol*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisProtocol;
}


vtkbisProtocol::vtkbisProtocol()
{
  this->ProtocolID=new char[10]; this->SetProtocolID("100");
  this->ProtocolName=new char[10]; this->SetProtocolName("");
  this->PI=new char[10]; this->SetPI("");
}

vtkbisProtocol::~vtkbisProtocol()
{
  if (this->ProtocolID!=NULL) delete [] this->ProtocolID;
  if (this->ProtocolName!=NULL) delete [] this->ProtocolName;
  if (this->PI!=NULL) delete [] this->PI;
}


void vtkbisProtocol::DebugPrint()
{
  cout<<"******Print Protocol Infomation******"<<endl;
  cout<<"ProtocolID: "<<ProtocolID<<endl;
  cout<<"ProtocolName: "<<ProtocolName<<endl;
  cout<<"PI: "<<PI<<endl;
  cout<<"******End Protocol Infomation******"<<endl;
  
}
// ----------------------------------------------------------------------------------
int vtkbisProtocol::InsertProtocol()
{
  int ok=false;
  string sqlstring;
  if (this->Connector->GetMode()==0)
    {
      sqlstring="Insert into Protocol(ProtocolName,PI)";
      sqlstring+="Value('";
    }
  else
    {
      sqlstring="Insert into Protocol(ProtocolID,ProtocolName,PI)";
      sqlstring+="VALUES('";
      sqlstring+=ProtocolID;
      sqlstring+="','";
    }
  sqlstring+=ProtocolName;
  sqlstring+="','";
  sqlstring+=PI;
  sqlstring+="')";
  
  if (!this->Connect())
    {
      cout<<"Cannot connect to database. Exit."<<endl;
      return ok;
    }
  
  vtkStdString s=this->Connector->RunUpdate(sqlstring.c_str());
  if (s=="ERROR" || s =="" )
    {
      cerr << "Failed to insert Protocol: " << endl;
    }
  else
    {
      ok=1;
    }
  return ok;
}
// ------------------------------------------------------------------------------------
const char* vtkbisProtocol::SearchProtocolID(const char* Protocolname,const char* PI)
{
  string protocolid="";
  string sqlstring;
  sqlstring="Select ProtocolID From Protocol Where ProtocolName='";
  sqlstring+=ProtocolName;
  sqlstring+="' and PI='";
  sqlstring+=PI;
  sqlstring+="'";
  
  if (!this->Connect())
    {
      cout<<"Cannot connect to database. Exit."<<endl;
      return protocolid.c_str();
    }
  
 this->ResultString=this->Connector->RunQuery(sqlstring.c_str());
  return this->ResultString.c_str();
}

// ------------------------------------------------------------------------------------

int vtkbisProtocol::FindEmptyProtocolID()
{
  int done=0;

  //  fprintf(stderr,"Beginning Protocol ID=%s\n",this->ProtocolID);

  while (done == 0)
    {
      string sqlstring;
      sqlstring="Select ProtocolName From Protocol Where ProtocolID='";
      sqlstring+=ProtocolID;
      sqlstring+="'";
  
      if (!this->Connect())
	{
	  cout<<"Cannot connect to database. Exit."<<endl;
	  return 0;
	}
      
     this->ResultString=this->Connector->RunQuery(sqlstring.c_str());
      if (this->ResultString.length()==0)
	{
	  done=1;
	}
      else
	{
	  char line[10];
	  int newp=atoi(this->ProtocolID);
	  newp=newp+1+int(100.0*vtkMath::Random());
	  sprintf(line,"%d",newp);
	  this->SetProtocolID(line);
	}
    }
  return done;
}
// ------------------------------------------------------------------------------------
int vtkbisProtocol::AddProtocol()
{
  int ok=false;
  string protocolid=this->SearchProtocolID(ProtocolName,PI);
  if(protocolid.length()!=0)
    {
      cout<<"Protocol exists as protocolid "<<protocolid<<endl;
      cout<<"Not adding protocol"<<endl;
      return ok;
    }

  if (this->Connector->GetMode()==1)
    {
      this->SetProtocolID("100");
      int allset=this->FindEmptyProtocolID();
      if (allset==0)
	{
	  cout << "Failed to find unique ID";
	  return ok;
	}
      //      fprintf(stderr,"Proceeding, unique ID = %s\n",this->ProtocolID);
    }
  
  return this->InsertProtocol();
  
}

	
  


