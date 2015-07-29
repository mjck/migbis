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
 * vtkbisAlgorithmTable.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: Isabella Murphy
 */

#include <vtkbisAlgorithmTable.h>
#include <vtkObjectFactory.h>
#include <vtkSQLQuery.h>
#include <vtkStdString.h>
#include <vtkVariant.h>

vtkbisAlgorithmTable* vtkbisAlgorithmTable::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisAlgorithmTable");
  if(ret)
    {
      return (vtkbisAlgorithmTable*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisAlgorithmTable;
}
// ----------------------------------------------------------------------------------------
vtkbisAlgorithmTable::vtkbisAlgorithmTable() 
{

  this->AlgorithmName=NULL;
  this->PresetName=NULL;
  this->Description=NULL;
  this->Date=NULL;
  this->Parameter=NULL;
  
  this->SetAlgorithmName("");
  this->SetPresetName("");
  this->SetDescription("");
  this->SetDate("");
  this->SetParameter("");
  
}
// ----------------------------------------------------------------------------------------
vtkbisAlgorithmTable::~vtkbisAlgorithmTable() 
{
  if (this->AlgorithmName==NULL)  delete [] this->AlgorithmName;
  if (this->PresetName==NULL)  delete [] this->PresetName;
  if (this->Description==NULL)  delete [] this->Description;
  if (this->Date==NULL)  delete [] this->Date;
  if (this->Parameter==NULL)  delete [] this->Parameter;
}
// ----------------------------------------------------------------------------------------
int vtkbisAlgorithmTable::CreateTable()
{
  if (!this->Connect())
    {
      vtkDebugMacro(<<"Cannot connect to database. Exit."<<endl);
      return 0;
    }
  vtkStdString createQuery("CREATE TABLE IF NOT EXISTS bis_algorithm_pset_table (algorithmName TEXT, presetName TEXT, description TEXT, date TEXT, parameter TEXT)");
  //  cout << createQuery << endl;
  vtkSQLQuery* query = this->Connector->RunComplexQuery(createQuery.c_str());
  
  if (query == NULL)
    return 0;

  
  query->Delete();
  return 1;
}
// ----------------------------------------------------------------------------------------
const char* vtkbisAlgorithmTable::SearchItem(int exact)
{
  if (!this->Connect())
    {
      vtkDebugMacro(<<"Cannot connect to database. Exit."<<endl);
      return "";
    }



  vtkStdString sqlstring;
  vtkStdString qualifier="=";
  if (exact==0)
    qualifier="like";


  // Somewhere here check if exact = 1, if it is this is fine,
  // if not replace = with "Like" or "Matches" whatever sql needs

  sqlstring="SELECT * from bis_algorithm_pset_table WHERE algorithmName =";
  sqlstring+="'";
  sqlstring+=this->AlgorithmName;
  sqlstring+="'";

  if(strlen(this->PresetName)>0)
    {
      sqlstring+=" AND presetName " + qualifier + " ";
      sqlstring+="'";
      sqlstring+=this->PresetName;
      sqlstring+="'";
    }
  
  if(strlen(this->Description)>0)
    {
      sqlstring+=" AND description " + qualifier + " ";
      sqlstring+="'";
      sqlstring+=this->Description;
      sqlstring+="'";
    }

  if(strlen(this->Date)>0)
    {
      sqlstring+=" AND date " + qualifier + " ";
      sqlstring+="'";
      sqlstring+=this->Date;
      sqlstring+="'";
    }
  
  if(strlen(this->Parameter)>0)
    {
      sqlstring+=" AND parameter " + qualifier + " ";
      sqlstring+="'";
      sqlstring+=this->Parameter;
      sqlstring+="'";
    }
  //  std::cout<<sqlstring<<endl;
 
 
  vtkSQLQuery* query = this->Connector->RunComplexQuery(sqlstring.c_str());
  if (query==NULL)
    {
      vtkErrorMacro(<< "Query failed " <<query->GetLastErrorText()<< endl);
      return "";
    }
 
  this->ResultString= "";
  while ( query->NextRow())
    {
      for ( int field = 0; field < query->GetNumberOfFields(); field++)
	{
	  if ( field > 0)
	      this->ResultString+= ",";
	  this->ResultString+=query->DataValue( field ).ToString().c_str();
	}
      this->ResultString+="\n";
    }
  query->Delete();
  return this->ResultString.c_str();
}
// ----------------------------------------------------------------------------------------
const char* vtkbisAlgorithmTable::SearchItem(const char* algorithmName, const char* presetName, const char* description, const char* date, const char* parameter,int exact)
{
  if(strlen(algorithmName)<1)
    {
      cerr << "Please set algorithm Name\n"<<endl;
      return "";
    }
  this->SetAlgorithmName(algorithmName);
  this->SetPresetName(presetName);
  this->SetDescription(description);
  this->SetDate(date);
  this->SetParameter(parameter);
  return this->SearchItem(exact);

}
// ----------------------------------------------------------------------------------------
int vtkbisAlgorithmTable::AddItem(const char* algorithmName, const char* presetName, const char* description, const char* date, const char* parameter)
{
  if (!this->Connect())
    {
      vtkDebugMacro(<<"Cannot connect to database. Exit."<<endl);
      return 0;
    }
  
  if ( strlen(this->SearchItem(algorithmName,presetName,"","","")) > 0 )
    {
      cerr<<this->AlgorithmName<<":"<<this->PresetName<<":"<<" is in the menu already. AddItem failed"<<endl;
      return 0;
    }
  
  this->SetAlgorithmName(algorithmName);
  this->SetPresetName(presetName);
  this->SetDescription(description);
  this->SetDate(date);
  this->SetParameter(parameter);
  
  vtkStdString sqlstring="INSERT INTO bis_algorithm_pset_table ( algorithmName, presetName, description, date, parameter) ";
  sqlstring+="VALUES('";
  sqlstring+=this->AlgorithmName;
  sqlstring+="','";
  sqlstring+=this->PresetName;
  sqlstring+="','";
  sqlstring+=this->Description;
  sqlstring+="','";
  sqlstring+=this->Date;
  sqlstring+="','";
  sqlstring+=this->Parameter;
  sqlstring+="')";
  
  vtkSQLQuery* query = this->Connector->RunComplexQuery(sqlstring.c_str());
  if (query==NULL)
    {
      vtkErrorMacro(<< "Insert query failed\n "<< query->GetLastErrorText()<<"\n");
      return 0;
    }
  query->Delete();
  return 1;
}
// ----------------------------------------------------------------------------------------
int vtkbisAlgorithmTable::DeleteItem(const char* algorithmName, const char* presetName)
{
  if (!this->Connect())
    {
      vtkDebugMacro(<< "Cannot connect to database. Exit."<<endl);
      
      return 0;
    }
  
  if(strlen(this->SearchItem(algorithmName,presetName,"","",""))<1)
    {
      //cout<<"This item does not exist in the menu. DeleteItem failed."<<endl;
      return 0;
    }
  
  vtkStdString sqlstring= "DELETE from bis_algorithm_pset_table WHERE algorithmName = ";
  sqlstring+="'";
  sqlstring+=algorithmName;
  sqlstring+="'";
  sqlstring+=" AND presetName = ";
  sqlstring+="'";
  sqlstring+=presetName;
  sqlstring+="'";
  vtkSQLQuery* query = this->Connector->RunComplexQuery(sqlstring.c_str());
  
  //	cerr << endl << "Running query: " << query->GetQuery() << endl;
  
  if (query==NULL)
    {
      vtkDebugMacro("Query " << sqlstring.c_str() << " failed\n"<<query->GetLastErrorText());
      //cerr << "Query failed" <<query->GetLastErrorText()<< endl;
      return 0;
    }
  query->Delete();
  return 1;
}
// ----------------------------------------------------------------------------------------
const char* vtkbisAlgorithmTable::QueryAll(const char* algorithmName)
{
  this->SetAlgorithmName(algorithmName);

  vtkStdString sqlstring = "SELECT * from bis_algorithm_pset_table WHERE algorithmName = ";
  sqlstring+="'";
  sqlstring+=this->AlgorithmName;
  sqlstring+="'";
  vtkSQLQuery* query = this->Connector->RunComplexQuery(sqlstring.c_str());
  
  if (query==NULL)
    {
      vtkDebugMacro(<< "Query " << sqlstring.c_str() << " failed\n"<<query->GetLastErrorText());
      //cerr << "Query failed" <<query->GetLastErrorText()<< endl;
      return 0;
    }
  
  this->ResultString= "";
  while ( query->NextRow())
    {
      for ( int field = 0; field < query->GetNumberOfFields(); field++)
	{
	  if ( field > 0)
	    this->ResultString+= ",";
	  this->ResultString+=query->DataValue( field ).ToString().c_str();
	}
      this->ResultString+="\n";
    }
  query->Delete();
  return this->ResultString.c_str();
}
// ----------------------------------------------------------------------------------------
int vtkbisAlgorithmTable::DeleteAll(const char* algorithmName)
{
  if (!this->Connect())
    {
      vtkDebugMacro(<<"Cannot connect to database. Exit."<<endl);
      return 0;
    }
  
  vtkStdString sqlstring= "DELETE from bis_algorithm_pset_table WHERE algorithmName = ";
  sqlstring+="'";
  sqlstring+=algorithmName;
  sqlstring+="'";
  vtkSQLQuery* query = this->Connector->RunComplexQuery(sqlstring.c_str());
  
  //	cerr << endl << "Running query: " << query->GetQuery() << endl;
  
  if (query==NULL)
    {
      vtkDebugMacro("Query " << sqlstring.c_str() << " failed\n"<<query->GetLastErrorText());
      //cerr << "Query failed" <<query->GetLastErrorText()<< endl;
      return 0;
    }
  query->Delete();
  return 1;
}
// ----------------------------------------------------------------------------------------

const char* vtkbisAlgorithmTable::PrintTable()
{
  if (!this->Connect())
    {
      vtkDebugMacro(<<"Cannot connect to database. Exit."<<endl);
      return 0;
    }
  vtkStdString sqlquery = "SELECT * from bis_algorithm_pset_table ";
  vtkSQLQuery* query = this->Connector->RunComplexQuery(sqlquery.c_str());
  
  this->ResultString= "ERROR";
  
  if (query!=NULL)
    {
      this->ResultString= "";
      while ( query->NextRow())
	{
	  for ( int field = 0; field < query->GetNumberOfFields(); field++)
	    {
	      if ( field > 0)
		this->ResultString+= ",";

	      this->ResultString+=query->DataValue( field ).ToString().c_str();
	    }
	  this->ResultString+="\n";
	}
    }
  
  query->Delete();
  return this->ResultString.c_str();
}
// ----------------------------------------------------------------------------------------
int vtkbisAlgorithmTable::Cleanup()
{
  this->SetAlgorithmName("");
  this->SetPresetName("");
  this->SetDescription("");
  this->SetDate("");
  this->SetParameter("");
  return 1;
}

