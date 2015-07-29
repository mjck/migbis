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

#include "vtkbisInjection.h"
#include "vtkbisBaseProcedure.h"
#include "vtkObjectFactory.h"


vtkbisInjection* vtkbisInjection::New()
{
  return new vtkbisInjection;
}



vtkbisInjection::vtkbisInjection()
{
  this->VisitID=new char[10];
  this->InjectionID=new char[10];
  this->InjectionDateTime=new char[10];
  
  this->SetVisitID("");
  this->SetInjectionID("");
  this->SetInjectionDateTime("");
}

vtkbisInjection::~vtkbisInjection()
{
  if (this->VisitID!=NULL) delete [] this->VisitID;
  if (this->InjectionID!=NULL) delete [] this->InjectionID;
  if (this->InjectionDateTime!=NULL) delete [] InjectionDateTime;
}


void vtkbisInjection::DebugPrint()
{
  cout<<"******Print Injection Infomation******"<<endl;
  cout<<"InjectionID: "<<InjectionID<<endl;
  cout<<"VisitID: "<<VisitID<<endl;
  cout<<"InjectionDateTime: "<<InjectionDateTime<<endl;
  cout<<"******End Injection Infomation******"<<endl;
  
}

int vtkbisInjection::InsertInjection()
{
  int ok=false;

  
  if(!this->Connect())
  {
    cout<<"Cannot connect to database. Exit."<<endl;
    return ok;
  }

  std::string sqlstring;
  if (this->Connector->GetMode()==0)
    {
      sqlstring="Insert into Injection(VisitID, InjectionDateTime)";
      sqlstring+="Value('";
      sqlstring+=VisitID;
      sqlstring+=+"',NOW() )";
    }
  else
    {
      vtkStdString dt=this->Connector->RunQuery("SELECT datetime('now')");
      sqlstring="Insert into Injection(InjectionID,VisitID,InjectionDateTime)";
      sqlstring+="VALUES('";
      sqlstring+=InjectionID;
      sqlstring+="','";
      sqlstring+=VisitID;
      sqlstring+="','";
      sqlstring+=dt;
      sqlstring+="')";
    }

  //  fprintf(stderr,"adding injection %s\n",sqlstring.c_str());
  vtkStdString s=this->Connector->RunUpdate(sqlstring.c_str());
  if (s=="ERROR")
    {
      cerr << "Failed to insert Injection: " << endl;
    }
  else
    {
      ok=1;
    }
  return ok;
}

const char* vtkbisInjection::SearchInjectionID(const char* VisitID)
{
  std::string sqlstring;
  sqlstring="Select max(InjectionID) From Injection Where VisitID='";
  sqlstring+=VisitID;
  sqlstring+="'";
  
  if (!this->Connect())
    {
      cout<<"Cannot connect to database. Exit."<<endl;
      return "";
    }

  this->ResultString=this->Connector->RunQuery(sqlstring.c_str());
  return this->ResultString.c_str();  
}

const char* vtkbisInjection::SearchInjectionDateTime(const char* InjectionID)
{
  std::string sqlstring;
  sqlstring="Select InjectionDateTime From Injection Where InjectionID='";
  sqlstring+=InjectionID;
  sqlstring+="'";
  
  if (!this->Connect())
    {
      cout<<"Cannot connect to database. Exit."<<endl;
      return "";
    }

  this->ResultString=this->Connector->RunQuery(sqlstring.c_str());
  return this->ResultString.c_str();  

}
// ------------------------------------------------------------------------------------
int vtkbisInjection::FindEmptyInjectionID()
{
  int done=0;

  //  fprintf(stderr,"Beginning Injection ID=%s\n",this->InjectionID);

  while (done == 0)
    {
      std::string sqlstring;
      sqlstring="Select VisitID From Injection Where InjectionID='";
      sqlstring+=InjectionID;
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
	  int newp=atoi(this->InjectionID);
	  newp=newp+1+int(100.0*vtkMath::Random());
	  sprintf(line,"%d",newp);
	  this->SetInjectionID(line);
	}
    }
  return done;
}
// ------------------------------------------------------------------------------------
int vtkbisInjection::AddInjection()
{
  int ok=false;
  std::string injectionid=this->SearchInjectionID(this->VisitID);
  if(injectionid.length()!=0)
    {
      cout<<"Injection exists as injectionid "<<injectionid<<endl;
      cout<<"Not adding injection"<<endl;
      return ok;
    }

  if (this->Connector->GetMode()==1)
    {
      this->SetInjectionID("100");
      int allset=this->FindEmptyInjectionID();
      if (allset==0)
	{
	  cout << "Failed to find unique ID";
	  return ok;
	}
      //      fprintf(stderr,"Proceeding, unique ID = %s\n",this->InjectionID);
    }
  
  return this->InsertInjection();
  
}

	
  


