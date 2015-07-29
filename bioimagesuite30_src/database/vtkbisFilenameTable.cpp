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

/*
 * vtkbisFilenameTable.cpp
 *
 *  Created on: Jan 22, 2009
 *      Author: isabella
 */

#include <vtkbisFilenameTable.h>
#include <vtkObjectFactory.h>
#include <vtkSQLQuery.h>
#include <vtkStdString.h>
#include <vtkVariant.h>

// -------------------------------------------------------------------------------

vtkbisFilenameTable* vtkbisFilenameTable::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisFilenameTable");
  if(ret)
    {
      return (vtkbisFilenameTable*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisFilenameTable;
}

// -------------------------------------------------------------------------------
vtkbisFilenameTable::vtkbisFilenameTable() 
{
  this->Type=NULL;
  this->FileName=NULL;
  this->Description=NULL;
  
  this->SetType("");
  this->SetFileName("");
  this->SetDescription("");
  
}
// -------------------------------------------------------------------------------
vtkbisFilenameTable::~vtkbisFilenameTable() 
{
  if (this->Type==NULL)  delete [] this->Type;
  if (this->FileName==NULL)  delete [] this->FileName;
  if (this->Description==NULL)  delete [] this->Description;
}
// -------------------------------------------------------------------------------
int vtkbisFilenameTable::CreateTable()
{
  if (!this->Connect())
    {
      vtkDebugMacro(<<"Cannot connect to database. Exit."<<endl);
      return 0;
    }

  vtkStdString createQuery("CREATE TABLE IF NOT EXISTS imSQLite_FilenameTable (type TEXT, filename TEXT, description TEXT)");
  vtkSQLQuery* query = this->Connector->RunComplexQuery(createQuery.c_str());
  
  if (query==NULL)
    {
      cerr << "Create query failed ............. \n";
      return 0;
    }
  query->Delete();
  return 1;
}
// -------------------------------------------------------------------------------
int vtkbisFilenameTable::SearchItem()
{

  if (!this->Connect())
    {
      vtkDebugMacro(<<"Cannot connect to database. Exit."<<endl);
      return 0;
    }


  vtkStdString sqlstring="SELECT * from imSQLite_FilenameTable WHERE type = ";
  sqlstring+="'";
  sqlstring+=this->Type;
  sqlstring+="'";
  sqlstring+="AND filename = ";
  sqlstring+="'";
  sqlstring+=this->FileName;
  sqlstring+="'";
  sqlstring+="AND description = ";
  sqlstring+="'";
  sqlstring+=this->Description;
  sqlstring+="'";
  
  vtkSQLQuery* query = this->Connector->RunComplexQuery(sqlstring.c_str());
  if (query==NULL)
    {
      cerr << "Query failed\n" ;
      return 0;
    }
  
  int status=0;

  if (query->NextRow())
    status=1;
  
  query->Delete();
  return status;
}
// ------------------------------------------------------------------------------------
int vtkbisFilenameTable::SearchItem(const char* type, const char* filename, const char* description)
{
  this->SetType(type);
  this->SetFileName(filename);
  this->SetDescription(description);
  return this->SearchItem();

}
// -----------------------------------------------------------------------------------
int vtkbisFilenameTable::AddItem(const char* type, const char* filename, const char* description)
{
  if (!this->Connect())
    {
      vtkDebugMacro(<<"Cannot connect to database. Exit."<<endl);
      return 0;
    }

  this->SetType(type);
  this->SetFileName(filename);
  this->SetDescription(description);
  
  
  if (this->SearchItem())
    return 0;

  vtkStdString sqlstring="INSERT INTO imSQLite_FilenameTable (type, filename, description) ";
  sqlstring+="VALUES('";
  sqlstring+=this->Type;
  sqlstring+="','";
  sqlstring+=this->FileName;
  sqlstring+="','";
  sqlstring+=this->Description;
  sqlstring+="')";
  

  if (!this->Connect())
    {
      vtkDebugMacro(<<"Cannot connect to database. Exit."<<endl);
      return 0;
    }

  
  vtkSQLQuery* query = this->Connector->RunComplexQuery(sqlstring.c_str());
  if (query==NULL)
    {
      vtkDebugMacro(<< "Insert query failed\n ");
      return 0;
    }
  query->Delete();
  return 1;
}

// --------------------------------------------------------------------------------------------------

int vtkbisFilenameTable::DeleteItem(const char* type, const char* filename, const char* description)
{
  if (!this->Connect())
    {
      vtkDebugMacro(<< "Cannot connect to database. Exit."<<endl);
      return 0;
    }

  this->SetType(type);
  this->SetFileName(filename);
  this->SetDescription(description);
  
  if(!this->SearchItem())
      return 0;

  
  vtkStdString sqlstring= "DELETE from imSQLite_FilenameTable WHERE type = ";
  sqlstring+="'";
  sqlstring+=this->Type;
  sqlstring+="'";
  sqlstring+="AND filename = ";
  sqlstring+="'";
  sqlstring+=this->FileName;
  sqlstring+="'";
  sqlstring+="AND description = ";
  sqlstring+="'";
  sqlstring+=this->Description;
  sqlstring+="'";

  vtkSQLQuery* query = this->Connector->RunComplexQuery(sqlstring.c_str());
  
  if (query==NULL)
    {
      vtkDebugMacro("Query " << sqlstring.c_str() << " failed\n");
      return 0;
    }
  query->Delete();
  return 1;
}
// --------------------------------------------------------------------------
const char* vtkbisFilenameTable::QueryAll(const char* type)
{
  if (!this->Connect())
    {
      vtkDebugMacro(<<"Cannot connect to database. Exit."<<endl);
      return 0;
    }

  vtkStdString sqlstring = "SELECT * from imSQLite_FilenameTable WHERE type = ";
  sqlstring+="'";
  sqlstring+=type;
  sqlstring+="'";
  vtkSQLQuery* query = this->Connector->RunComplexQuery(sqlstring.c_str());

  if (query==NULL)
    {
      vtkDebugMacro(<< "Query " << sqlstring.c_str() << " failed\n");
      return 0;
    }
  
  this->ResultString= "";
  while ( query->NextRow())
    {
      for ( int field = 0; field < query->GetNumberOfFields(); field++)
	{
	  if ( field > 0)
	    {
	      this->ResultString+= ",";
	    }
	  this->ResultString+=query->DataValue( field ).ToString().c_str();
	}
      this->ResultString+="|";
    }
  query->Delete();
  return this->ResultString.c_str();
}
// -----------------------------------------------------------------------------
int vtkbisFilenameTable::DeleteAll(const char* type)
{
  if (!this->Connect())
    {
      vtkDebugMacro(<<"Cannot connect to database. Exit."<<endl);
      return 0;
    }

  vtkStdString sqlstring= "DELETE from imSQLite_FilenameTable WHERE type = ";
  sqlstring+="'";
  sqlstring+=type;
  sqlstring+="'";
  vtkSQLQuery* query = this->Connector->RunComplexQuery(sqlstring.c_str());
  if (query==NULL)
    {
      vtkDebugMacro("Query " << sqlstring.c_str() << " failed\n");
      return 0;
    }
  query->Delete();
  return 1;
}

// ----------------------------------------------------------------------------
const char* vtkbisFilenameTable::PrintTable()
{
  if (!this->Connect())
    {
      vtkDebugMacro(<<"Cannot connect to database. Exit."<<endl);
      return 0;
    }

  vtkStdString sqlquery = "SELECT * from imSQLite_FilenameTable ";
  vtkSQLQuery* query = this->Connector->RunComplexQuery(sqlquery.c_str());

  this->ResultString= "ERROR";

  if (query!=NULL)
    {
      this->ResultString= "";
      while ( query->NextRow())
	{
	  if (this->ResultString !="")
	    this->ResultString+="\n";
	  for ( int field = 0; field < query->GetNumberOfFields(); field++)
	    {
	      if ( field > 0)
		{
		  this->ResultString+= ":";
		}
	      this->ResultString+=query->DataValue( field ).ToString().c_str();
	    }

	}
    }
  query->Delete();
  return this->ResultString.c_str();
}
// --------------------------------------------------------------------------

