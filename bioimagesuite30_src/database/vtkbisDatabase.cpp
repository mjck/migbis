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


#include "vtkbisDatabase.h"
#include "vtkObjectFactory.h"
#include <vtkSQLiteDatabase.h>
#include "bioimagesuite_settings.h"
#include <vtkVersion.h> // Needed for VTK_*_VERSION

#ifdef BIOIMAGESUITE3_USE_MYSQL_DATABASE_MODULE
#include <vtkbisMySQLDatabase.h>
#endif



vtkbisDatabase* vtkbisDatabase::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisDatabase");
  if(ret)
    {
      return (vtkbisDatabase*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisDatabase;
}

// -----------------------------------------------------------------------------------
vtkbisDatabase::vtkbisDatabase()
{
  name="YaleTest";
  host="";
  user="";
  password="";
  port=3306;
  sqliteurl="";
  this->Mode=0;
  this->Connection=NULL;

}
// -------------------------------------------------------------------------------------------

vtkbisDatabase::~vtkbisDatabase()
{
  this->Connection->Delete();
}

// --------------------------------------------------------------------------------------------------------
void vtkbisDatabase::Initialize(char* name, char* host, char* user, char* password, unsigned int port)
{
#ifdef BIOIMAGESUITE3_USE_MYSQL_DATABASE_MODULE
  if (this->Connection!=NULL)
    {
      this->Disconnect();
      this->Connection->Delete();
    }
  this->Connection=vtkbisMySQLDatabase::New();
  this->Mode=0;
  this->name=name;
  this->host=host;
  this->user=user;
  this->password=password;
  this->port=port;
  return;
#endif
}

void vtkbisDatabase::InitializeSQLite(char* filename)
{
  this->Mode=1;
  if (this->Connection!=NULL)
    {
      this->Disconnect();
      this->Connection->Delete();
    }


  //  this->sqliteurl="sqlite://";
  this->sqliteurl=filename;

  //  std::cout<<"SQLIte url= " << sqliteurl.c_str() << "\n";
  vtkDebugMacro(<<"SQLIte url= " << sqliteurl.c_str() << "\n");

 vtkSQLiteDatabase* tmp=vtkSQLiteDatabase::New();
 tmp->SetDatabaseFileName(this->sqliteurl);

#if ((VTK_MAJOR_VERSION >= 5)&&(VTK_MINOR_VERSION >= 4))
  if ( !tmp->Open("") )
#else
  if ( !tmp->Open() )
#endif
    {
      this->sqliteurl="";
      tmp->Delete();
    }
  else
    {
      this->Connection=tmp;
    }
}

// --------------------------------------------------------------------------------------------------------

void vtkbisDatabase::DebugPrint()
{
  cout <<"Mode = " << this->Mode <<endl;
  if (!this->UsingSQLite())
    {
      cout << " Using MySQL " << endl;
      cout<<"name: "<<name<<endl;
      cout<<"host: "<<host<<endl;
      cout<<"user: "<<user<<endl;
      cout<<"password: "<<password<<endl;
      cout<<"port: "<<port<<endl;
    }
  else
    {
      cout << "Using SQLite" << endl;
      cout << "URL = " << sqliteurl.c_str() << endl;
    }
  cout<<"Connection: "<< this->Connection << endl;
}
// --------------------------------------------------------------------------------------------------------

int vtkbisDatabase::Connect()
{

  //  vtkDebugMacro(<<"Attempting to Connect .... \n");

  if (this->Connection==NULL)
    {
      vtkDebugMacro(<<"Connection is NULL\n");
      return 0;
    }

  if (this->Connection->IsOpen())
    {
      //  vtkDebugMacro(<<"Connection is Open\n");
      return 1;
    }

  int status=0;

  if (!this->UsingSQLite())
    {
      vtkDebugMacro(<<"Attempting to Connect mySQL.... \n");
      status=this->ConnectMySQL();
    }
  else
    {
      vtkDebugMacro(<<"Attempting to Connect SQLite.... \n");
      status=this->ConnectSQLite();
    }

  vtkDebugMacro(<<"connection status = " << status << endl);
  return status;
}
// ----------------------------------------------------------------------------------------------------
int vtkbisDatabase::ConnectSQLite()
{
  if (this->Connection==NULL)
    return 0;

#if ((VTK_MAJOR_VERSION >= 5)&&(VTK_MINOR_VERSION >= 4))
  return this->Connection->Open("");
#else
  return this->Connection->Open();
#endif
}
// ----------------------------------------------------------------------------------------------------
int vtkbisDatabase::ConnectMySQL()
{
#ifndef BIOIMAGESUITE3_USE_MYSQL_DATABASE_MODULE
  return 0;
#else
  char line[20];
  sprintf(line,"%d",this->port);

  vtkStdString url="mysql://";
  url+=user.c_str();
  url+=":";
  url+=password.c_str();
  url+="@";
  url+=host.c_str();
  url+=":";
  url+=line;
  url+="/";
  url+=name.c_str();

  vtkbisMySQLDatabase* con = vtkbisMySQLDatabase::New();
  con->SetUser(user.c_str());
  con->SetPassword(password.c_str());
  con->SetServerPort(port);
  con->SetHostName(host.c_str());
  con->SetDatabaseName(name.c_str());

  this->Connection=con;
  return this->Connection->Open();
#endif

}
// ---------------------------------------------------------------------------
void vtkbisDatabase::Disconnect()
{
  if (this->Connection==NULL)
    return;

  vtkDebugMacro(<<"Attempting to Disconnect .... \n");

  if (this->Connection->IsOpen())
    this->Connection->Close();

#if ((VTK_MAJOR_VERSION >= 5)&&(VTK_MINOR_VERSION >= 4))
  if (!this->Connection->Open(""))
#else
  if (!this->Connection->Open())
#endif
    {
      vtkDebugMacro(<<" Disconnection succeeded .... \n");
    }
  else
    {
      vtkDebugMacro(<<" Disconnection failed .... \n");
    }

}
// ---------------------------------------------------------------------------
//return simple string query result
vtkStdString vtkbisDatabase::RunQuery(const char* sqlstring)
{
  if (this->Connect()==0)
    return "";

  std::string s=sqlstring;

  vtkSQLQuery* query = this->Connection->GetQueryInstance();
  query->SetQuery(s.c_str());

  vtkStdString result="";

  vtkDebugMacro(<<"Invoking Query= " << s.c_str() << "\n");

  if (!query->Execute())
    {
      vtkDebugMacro(<<"Query= " << s.c_str() << " Failed " << query->GetLastErrorText()<< endl);
      result="";
    }
  else
    {
      while (query->NextRow())
	{
	  for ( int col = 0; col < query->GetNumberOfFields(); ++ col )
	    {
	      if ( col > 0 )
		result+=",";
	      result+=query->DataValue(col).ToString().c_str() ;
	    }
	}
    }

  query->Delete();
  return result;
}
// -------------------------------------------------------------------------------------------------

//update database
//@output: error or ok
vtkStdString vtkbisDatabase::RunUpdate(const char* sqlstring)
{
  if (this->Connect()==0)
    return "ERROR";

  std::string s=sqlstring;
  vtkSQLQuery* query = this->Connection->GetQueryInstance();
  query->SetQuery(s.c_str());
  vtkStdString result="OK";
  if (!query->Execute())
    {
      vtkDebugMacro(<<"Query= " << s.c_str() << " Failed " << query->GetLastErrorText()<< endl);
      result="ERROR";
    }
  else
    {
      result="OK";
    }
  query->Delete();
  return result;
}
// -------------------------------------------------------------------------------------------------
//return pointer to query result, for complex query
vtkSQLQuery*  vtkbisDatabase::RunComplexQuery(const char* searchstring)
{
  if (this->Connect()==0){
	  return NULL;
	  std::cout<<"RunComplexQuery no connect"<<endl;
  }

  vtkDebugMacro(<<"About to Execute Query= " << searchstring << "\n");
  std::string s=searchstring;

  vtkSQLQuery *query = this->Connection->GetQueryInstance();
  query->SetQuery(s.c_str());

  if (!query->Execute())
    {
      vtkDebugMacro(<<"Query= " << searchstring << " Failed " << query->GetLastErrorText()<< endl);
      std::cout<<"Query failed "<<query->GetLastErrorText()<<endl;
      query->Delete();
      query=NULL;

    }

  vtkDebugMacro(<<"Query  RUN OK\n");
  return query;
}
// -------------------------------------------------------------------------------------------------

vtkStdString vtkbisDatabase::GetQueryValueAsString(vtkSQLQuery* query,int value)
{
  if (query!=NULL)
    return query->DataValue(value).ToString();

  return "";
}
// -------------------------------------------------------------------------------------------------

