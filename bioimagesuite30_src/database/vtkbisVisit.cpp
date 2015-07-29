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

#include "vtkbisVisit.h"
#include "vtkObjectFactory.h"



vtkbisVisit* vtkbisVisit::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisVisit");
  if(ret)
    {
      return (vtkbisVisit*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisVisit;
}


vtkbisVisit::vtkbisVisit()
{
  this->VisitID=new char[10];
  this->PatientID=new char[10];
  this->ProtocolID=new char[10];
  this->VisitDate=new char[10];
  this->Age=0;
  this->Height=0.0;
  this->Weight=0.0;
  this->Notes=new char[10];


  this->SetVisitID("");
  this->SetPatientID("");
  this->SetProtocolID("100");
  this->SetVisitDate("");
  this->SetNotes("");
}

vtkbisVisit::~vtkbisVisit()
{
  if (this->VisitID !=NULL ) delete [] this->VisitID;
  if (this->PatientID !=NULL ) delete [] this->PatientID;
  if (this->ProtocolID !=NULL ) delete [] this->ProtocolID;
  if (this->VisitDate !=NULL ) delete [] this->VisitDate;
  if (this->Notes !=NULL ) delete [] this->Notes;
}

void vtkbisVisit::DebugPrint()
{
  cout<<"******Print Visit Infomation******"<<endl;
  cout<<"VisitID: "<<VisitID<<endl;
  cout<<"PatientID: "<<PatientID<<endl;
  cout<<"ProtocolID: "<<ProtocolID<<endl;
  cout<<"VisitDate: "<<VisitDate<<endl;
  cout<<"Age: "<<Age<<endl;
  cout<<"Height: "<<Height<<endl;
  cout<<"Weight: "<<Weight<<endl;
  cout<<"Notes: "<<Notes<<endl;
  cout<<"******End Visit Infomation******"<<endl;
  
}

int vtkbisVisit::InsertVisit()
{
  int ok=false;
  std::string sqlstring;
  if (this->Connector->GetMode()==0)
    {
      sqlstring="Insert into Visit(PatientID, ProtocolID,VisitDate)";
      sqlstring+="Value('";
    }
  else
    {
      sqlstring="Insert into Visit(VisitID, PatientID, ProtocolID,VisitDate)";
      sqlstring+="VALUES('";
      sqlstring+=VisitID;
      sqlstring+="','";
    }

  sqlstring+=PatientID;
  sqlstring+="','";
  sqlstring+=ProtocolID;
  sqlstring+="','";
  sqlstring+=VisitDate;
  sqlstring+="')";

  if(!this->Connect())
    {
      cout<<"Cannot connect to database. Exit."<<endl;
      return ok;
    }
  vtkStdString s=this->Connector->RunUpdate(sqlstring.c_str());
  if (s=="ERROR" || s=="" )
    {
      cerr << "Failed to insert Visit: " << endl;
    }
  else
    {
      ok=1;
    }
  return ok;
}

const char* vtkbisVisit::SearchVisitID(const char* cPatientID,const char* cProtocolID,const char* cVisitDate)
{
  std::string visitid="";
  std::string sqlstring;
  this->SetPatientID(cPatientID);
  this->SetProtocolID(cProtocolID);
  this->SetVisitDate(cVisitDate);

  sqlstring="Select VisitID From Visit Where PatientID='";
  sqlstring+=cPatientID;
  sqlstring+="' and ProtocolID='";
  sqlstring+=cProtocolID;
  sqlstring+="' and VisitDate='";
  sqlstring+=cVisitDate;
  sqlstring+="'";
  
  if (!this->Connect())
    {
      cout<<"Cannot connect to database. Exit."<<endl;
      return visitid.c_str();
    }

 this->ResultString=this->Connector->RunQuery(sqlstring.c_str());
  return this->ResultString.c_str();
}


  

// ------------------------------------------------------------------------------------

int vtkbisVisit::FindEmptyVisitID()
{
  int done=0;

  //  fprintf(stderr,"Beginning Visit ID=%s\n",this->VisitID);

  while (done == 0)
    {
      std::string sqlstring;
      sqlstring="Select PatientID From Visit Where VisitID='";
      sqlstring+=VisitID;
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
	  int newp=atoi(this->VisitID);
	  newp=newp+1+int(100.0*vtkMath::Random());
	  sprintf(line,"%d",newp);
	  this->SetVisitID(line);
	}
    }
  return done;
}
// ------------------------------------------------------------------------------------
int vtkbisVisit::AddVisit()
{
  int ok=false;
  std::string visitid=this->SearchVisitID(PatientID,ProtocolID,VisitDate);
  if(visitid.length()!=0)
    {
      cout<<"Visit exists as visitid "<<visitid<<endl;
      cout<<"Not adding visit"<<endl;
      return ok;
    }

  if (this->Connector->GetMode()==1)
    {
      this->SetVisitID("100");
      int allset=this->FindEmptyVisitID();
      if (allset==0)
	{
	  cout << "Failed to find unique ID";
	  return ok;
	}
    }
  
  return this->InsertVisit();
  
}

	
  


