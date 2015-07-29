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

#include "vtkbisSeries.h"
#include "vtkObjectFactory.h"



vtkbisSeries* vtkbisSeries::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisSeries");
  if(ret)
    {
      return (vtkbisSeries*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisSeries;
}


vtkbisSeries::vtkbisSeries()
{
  this->SeriesID=new char[10];       this->SetSeriesID("");
  this->StudyID=new char[10];        this->SetStudyID("");
  this->InjectionID=new char[10];    this->SetInjectionID("");
  this->SeriesUID=new char[10];      this->SetSeriesUID("");
  this->SeriesFileName=new char[10]; this->SetSeriesFileName("");
  this->SeriesDataPath=new char[10]; this->SetSeriesDataPath("");
  
}


vtkbisSeries::~vtkbisSeries()
{
  if (this->SeriesID==NULL)        delete [] SeriesID;
  if (this->StudyID==NULL)         delete [] StudyID;
  if (this->InjectionID==NULL)     delete [] InjectionID;
  if (this->SeriesUID==NULL)       delete [] SeriesUID;
  if (this->SeriesFileName==NULL)  delete [] SeriesFileName;
  if (this->SeriesDataPath==NULL)  delete [] SeriesDataPath;
}


void vtkbisSeries::DebugPrint()
{
  cout<<"******Print Series Infomation******"<<endl;
  cout<<"SeriesID: "<<SeriesID<<endl;
  cout<<"StudyID: "<<StudyID<<endl;
  cout<<"InjectionID: "<<InjectionID<<endl;
  cout<<"SeriesUID: "<<SeriesUID<<endl;
  cout<<"SeriesFileName: "<<SeriesFileName<<endl;
  cout<<"SeriesDatapath: "<<SeriesDataPath<<endl;
  cout<<"******End Series Infomation******"<<endl;
  
}

int vtkbisSeries::InsertSeries()
{
  int ok=false;
  if(!this->Connect())
    {
      cout<<"Cannot connect to database. Exit."<<endl;
      return ok;
    }

  std::string sqlstring;
  vtkStdString dt;
  if (this->Connector->GetMode()==0)
    {
      sqlstring="Insert into Series(StudyID, InjectionID, SeriesUID, SeriesFileName,SeriesDataPath)";
      sqlstring+="Value('";
    }
  else
    {
      dt=this->Connector->RunQuery("SELECT datetime('now')");
      sqlstring="Insert into Series(SeriesID,StudyID, InjectionID, SeriesUID,SeriesFileName,SeriesDataPath)";
      sqlstring+="VALUES('";
      sqlstring+=SeriesID;
      sqlstring+="','";
    }

  sqlstring+=StudyID;
  sqlstring+="','";
  sqlstring+=InjectionID;
  if (this->Connector->GetMode()==0)
    sqlstring+="', NOW(), '";
  else
    sqlstring+="','"+dt+"','";
  sqlstring+=SeriesFileName;
  sqlstring+="','";
  sqlstring+=SeriesDataPath;
  sqlstring+="')";

  //  fprintf(stderr,"adding series %s\n",sqlstring.c_str());
  vtkStdString s=this->Connector->RunUpdate(sqlstring.c_str());
  if (s=="ERROR" || s=="")
    {
      cerr << "Failed to insert Series: " << endl;
    }
  else
    {
      ok=1;
    }
  return ok;
}

const char* vtkbisSeries::SearchSeriesID(const char* StudyID,const char* InjectionID)
{
  std::string seriesid="";
  std::string sqlstring;
  sqlstring="Select SeriesID From Series Where StudyID='";
  sqlstring+=StudyID;
  sqlstring+="' and InjectionID='";
  sqlstring+=InjectionID;
  sqlstring+="'";

  if (!this->Connect())
    {
      cout<<"Cannot connect to database. Exit."<<endl;
      return seriesid.c_str();
    }

 this->ResultString=this->Connector->RunQuery(sqlstring.c_str());
  return this->ResultString.c_str();
}

const char* vtkbisSeries::SearchSeriesUID(const char* SeriesID)
{
  std::string seriesuid="";
  std::string sqlstring;
  sqlstring="Select SeriesUID From Series Where SeriesID='";
  sqlstring+=SeriesID;
  sqlstring+="'";
  //cout<<"sqlstring= "<<sqlstring<<endl;

  if (!this->Connect())
    {
      cout<<"Cannot connect to database. Exit."<<endl;
      return seriesuid.c_str();
    }

 this->ResultString=this->Connector->RunQuery(sqlstring.c_str());
  return this->ResultString.c_str();
}

// ------------------------------------------------------------------------------------
int vtkbisSeries::FindEmptySeriesID()
{
  int done=0;

  //  fprintf(stderr,"Beginning Series ID=%s\n",this->SeriesID);

  while (done == 0)
    {
      std::string sqlstring;
      sqlstring="Select SeriesFileName From Series Where SeriesID='";
      sqlstring+=SeriesID;
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
	  int newp=atoi(this->SeriesID);
	  newp=newp+1+int(100.0*vtkMath::Random());
	  sprintf(line,"%d",newp);
	  this->SetSeriesID(line);
	}
    }
  return done;
}
// ------------------------------------------------------------------------------------
int vtkbisSeries::AddSeries()
{
  int ok=false;
  std::string seriesid=this->SearchSeriesID(this->StudyID,this->InjectionID);
  if(seriesid.length()!=0)
    {
      cout<<"Series exists as seriesid "<<seriesid<<endl;
      cout<<"Not adding series"<<endl;
      return ok;
    }

  if (this->Connector->GetMode()==1)
    {
      this->SetSeriesID("100");
      int allset=this->FindEmptySeriesID();
      if (allset==0)
	{
	  cout << "Failed to find unique ID";
	  return ok;
	}
      //      fprintf(stderr,"Proceeding, unique ID = %s\n",this->SeriesID);
    }
  
  return this->InsertSeries();
  
}

	
  


