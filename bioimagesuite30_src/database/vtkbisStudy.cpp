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

#include "vtkbisStudy.h"
#include "vtkObjectFactory.h"

using namespace std;

vtkbisStudy* vtkbisStudy::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisStudy");
  if(ret)
    {
      return (vtkbisStudy*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisStudy;
}


vtkbisStudy::vtkbisStudy()
{
  this->StudyID=new char[10];
  this->VisitID=new char[10];
  this->PatientID=new char[10];
  this->StudyUID=new char[10];

  this->SetVisitID("");
  this->SetPatientID("");
  this->SetStudyID("");
  this->SetStudyUID("");
}

vtkbisStudy::~vtkbisStudy()
{
  if (this->VisitID !=NULL ) delete [] this->VisitID;
  if (this->PatientID !=NULL ) delete [] this->PatientID;
  if (this->StudyID !=NULL ) delete [] this->StudyID;
  if (this->StudyUID !=NULL ) delete [] this->StudyUID;
}


void vtkbisStudy::DebugPrint()
{
  cout<<"******Print Study Infomation******"<<endl;
  cout<<"StudyID: "<<StudyID<<endl;
  cout<<"VisitID: "<<VisitID<<endl;
  cout<<"PatientID: "<<PatientID<<endl;
  cout<<"StudyUID: "<<StudyUID<<endl;
  cout<<"******End Study Infomation******"<<endl;
  
}

int vtkbisStudy::InsertStudy()
{
  int ok=false;
  std::string sqlstring;
  if(!this->Connect())
  {
    cout<<"Cannot connect to database. Exit."<<endl;
    return ok;
  }

  if (this->Connector->GetMode()==0)
    {
      sqlstring="Insert into Study(VisitID,PatientID,StudyUID)";
      sqlstring+="Value('";
      sqlstring+=VisitID;
      sqlstring+="','";
      sqlstring+=PatientID;
      sqlstring+="',NOW() )";
    }
  else
    {
      vtkStdString dt=this->Connector->RunQuery("SELECT datetime('now')");
      
      sqlstring="Insert into Study(StudyID,VisitID,PatientID,StudyUID)";
      sqlstring+="VALUES('";
      sqlstring+=StudyID;
      sqlstring+="','";
      sqlstring+=VisitID;
      sqlstring+="','";
      sqlstring+=PatientID;
      sqlstring+="','";
      sqlstring+=dt;
      sqlstring+="')";
    }
  
  //  fprintf(stderr,"adding study %s\n",sqlstring.c_str());

  vtkStdString s=this->Connector->RunUpdate(sqlstring.c_str());
  if (s=="ERROR" || s=="")
    {
      cerr << "Failed to insert Study: " << endl;
    }
  else
    {
      ok=1;
    }
  return ok;
}

const char* vtkbisStudy::SearchStudyID(const char* VisitID, const char* PatientID)
{
  std::string studyid="";
  std::string sqlstring;
  sqlstring="Select max(StudyID) From Study Where PatientID='";
  sqlstring+=PatientID;
  sqlstring+="' and VisitID='";
  sqlstring+=VisitID;
  sqlstring+="'";
  
  if (!this->Connect())
    {
      cout<<"Cannot connect to database. Exit."<<endl;
      return studyid.c_str();
    }

 this->ResultString=this->Connector->RunQuery(sqlstring.c_str());
  return this->ResultString.c_str();  
}

const char* vtkbisStudy::SearchStudyUID(const char* StudyID)
{
  std::string studyuid="";
  std::string sqlstring;
  sqlstring="Select StudyUID From Study Where StudyID='";
  sqlstring+=StudyID;
  sqlstring+="'";
  //cout<<"sqlstring= "<<sqlstring<<endl;
  
  if (!this->Connect())
    {
      cout<<"Cannot connect to database. Exit."<<endl;
      return studyuid.c_str();
    }

 this->ResultString=this->Connector->RunQuery(sqlstring.c_str());
  return this->ResultString.c_str();
}



// ------------------------------------------------------------------------------------
int vtkbisStudy::FindEmptyStudyID()
{
  int done=0;

  //  fprintf(stderr,"Beginning Study ID=%s\n",this->StudyID);

  while (done == 0)
    {
      std::string sqlstring;
      sqlstring="Select PatientID From Study Where StudyID='";
      sqlstring+=StudyID;
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
	  int newp=atoi(this->StudyID);
	  newp=newp+1+int(100.0*vtkMath::Random());
	  sprintf(line,"%d",newp);
	  this->SetStudyID(line);
	}
    }
  return done;
}
// ------------------------------------------------------------------------------------
int vtkbisStudy::AddStudy()
{
  int ok=false;
  std::string studyid=this->SearchStudyID(this->VisitID,this->PatientID);
  if(studyid.length()!=0)
    {
      cout<<"Study exists as studyid "<<studyid<<endl;
      cout<<"Not adding study"<<endl;
      return ok;
    }

  if (this->Connector->GetMode()==1)
    {
      this->SetStudyID("100");
      int allset=this->FindEmptyStudyID();
      if (allset==0)
	{
	  cout << "Failed to find unique ID";
	  return ok;
	}
      //      fprintf(stderr,"Proceeding, unique ID = %s\n",this->StudyID);
    }
  
  return this->InsertStudy();
  
}

	
  


