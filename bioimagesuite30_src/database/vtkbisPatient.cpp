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

#include "vtkbisPatient.h"
#include "vtkObjectFactory.h"
#include <sstream>
using namespace std;

vtkbisPatient* vtkbisPatient::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisPatient");
  if(ret)
    {
      return (vtkbisPatient*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisPatient;
}



vtkbisPatient::vtkbisPatient()
{
  this->PatientID=new char[10];
  this->FirstName=new char[10];
  this->LastName=new char[10];
  this->BirthDate=new char[10];
 
  this->SetPatientID("");
  this->SetFirstName("");
  this->SetLastName("");
  this->SetBirthDate("");

  
}

vtkbisPatient::~vtkbisPatient()
{
  if (this->PatientID==NULL)  delete [] this->PatientID;
  if (this->FirstName==NULL)  delete [] this->FirstName;
  if (this->LastName==NULL)   delete [] this->LastName; 
  if (this->BirthDate==NULL)  delete [] this->BirthDate;
}

void vtkbisPatient::Initialize(const char* FirstName, const char* LastName, const char* BirthDate)
{
  this->SetPatientID("");
  this->SetFirstName(FirstName);
  this->SetLastName(LastName);
  this->SetBirthDate(BirthDate);
}

void vtkbisPatient::DebugPrint()
{
  cout<<"******Print Patient Infomation******"<<endl;
  cout<<"PatientID: "<<PatientID<<endl;
  cout<<"FirstName: "<<FirstName<<endl;
  cout<<"LastName: "<<LastName<<endl;
  cout<<"BirthDate: "<<BirthDate<<endl;
  cout<<"******End Patient Infomation******"<<endl;
}
// ----------------------------------------------------------------------------------
int vtkbisPatient::InsertPatient()
{
  int ok=false;
  string sqlstring="Insert into Patient(PatientID, FirstName, LastName, BirthDate) ";
  if (this->Connector->GetMode()==0)
    sqlstring+="Value('";
  else
    sqlstring+="VALUES('";
  sqlstring+=PatientID;
  sqlstring+="','";
  sqlstring+=FirstName;
  sqlstring+="','";
  sqlstring+=LastName;
  sqlstring+="','";
  sqlstring+=BirthDate;
  sqlstring+="')";
  

  if (!this->Connect())
  {
    cout<<"Cannot connect to database. Exit."<<endl;
    return ok;
  }
  
  vtkStdString s=this->Connector->RunUpdate(sqlstring.c_str());
  if (s=="ERROR" || s =="" )
    {
      cerr << "Failed to insert Patient: " << endl;
    }
  else
    {
      ok=1;
    }
  return ok;
}  
// ------------------------------------------------------------------------------------
const char* vtkbisPatient::SearchPatientID(const char* FirstName,const char* LastName,const char* BirthDate)
{
  string patientid="";
  string sqlstring;
  sqlstring="Select PatientID From Patient Where FirstName='";
  sqlstring+=FirstName;
  sqlstring+="' and LastName='";
  sqlstring+=LastName;
  sqlstring+="' and BirthDate='";
  sqlstring+=BirthDate;
  sqlstring+="'";
  
  if (!this->Connect())
    {
      cout<<"Cannot connect to database. Exit."<<endl;
      return patientid.c_str();
    }

 this->ResultString=this->Connector->RunQuery(sqlstring.c_str());
  return this->ResultString.c_str();
}

// --------------------------------------------------------------------------------------
const char* vtkbisPatient::GetNewPatientID(const char* initial,int number)
{
  stringstream ss;
  ss<<initial<<number;
  string patientid=ss.str();
  string sqlstring="Select Firstname from Patient where PatientID='";
  sqlstring+=patientid;
  sqlstring+="'";
  string fname="";

  if (!this->Connect())
    {
      //      fprintf(stderr, "Get New Patient ID  .... Cannot connect to database. Exit.\n");
      return patientid.c_str();
    }
  
  vtkSQLQuery* query=this->Connector->RunComplexQuery(sqlstring.c_str());
  if (query==NULL)
    return patientid.c_str();

  while ( query->NextRow() )
    {
      vtkStdString fname=query->DataValue(0).ToString();
      if(fname.length()!=0)
	{
	  number++;  
	  patientid=GetNewPatientID(initial,number);
	}
    }
  query->Delete();
  return patientid.c_str();
}

// ----------------------------------------------------------------------------------
int vtkbisPatient::AddPatient()
{
  int ok=false;
  string patientid=SearchPatientID(FirstName,LastName,BirthDate);
  if(patientid.length()!=0)
    {
      cout<<"Patient exists as patientid "<<patientid<<endl;
      cout<<"Not add patient"<<endl;
      return ok;
    }

  string initial="";
  initial.append(1,FirstName[0]);
  initial.append(1,LastName[0]);

  const char* newid=this->GetNewPatientID(initial.c_str(),100);
  this->SetPatientID(newid);
  if(this->InsertPatient()==1)
    {
      ok=true;
    }
  return true;
}
	


