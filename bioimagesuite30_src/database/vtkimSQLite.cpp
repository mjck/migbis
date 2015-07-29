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
 * vtkimSQLite.cpp
 *
 *  Created on: Jan 20, 2009
 *      Author: Isabella Murphy
 */

#include <vtkimSQLite.h>
#include <vtkObjectFactory.h>
#include <vtkSQLiteDatabase.h>
#include <vtkStdString.h>
#include <vtkSQLQuery.h>
#include <vtkVariant.h>

vtkimSQLite::vtkimSQLite() {
	this->Connection=NULL;
	this->FileName=NULL;
	this->Description=NULL;

	this->SetFileName("");
	this->SetDescription("");

}

vtkimSQLite::~vtkimSQLite() {
	this->Connection->Delete();
	if (this->FileName==NULL)  delete [] this->FileName;
	if (this->Description==NULL)  delete [] this->Description;
}

vtkimSQLite* vtkimSQLite::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkimSQLite");
  if(ret)
    {
      return (vtkimSQLite*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkimSQLite;
}

int vtkimSQLite::Connect(const char* url)
{
	if (this->Connection!=NULL)
	{
		this->Disconnect();
	    this->Connection->Delete();
	}
	//this->Connection=vtkSQLiteDatabase::New();
	this->Connection = vtkSQLiteDatabase::SafeDownCast( vtkSQLDatabase::CreateFromURL( url ) );
#if ((VTK_MAJOR_VERSION >= 5)&&(VTK_MINOR_VERSION >= 4))
        bool status = this->Connection->Open("");
#else
        bool status = this->Connection->Open();
#endif

	if ( ! status )
	{
		cerr << "Couldn't open database.\n";
		return 0;
	}
	return 1;
}

void vtkimSQLite::Initialize(const char* filename, const char* description)
{
	this->SetFileName(filename);
	this->SetDescription(description);
}

void vtkimSQLite::CreateTable()
{
	vtkStdString createQuery("CREATE TABLE IF NOT EXISTS imsqlite_test (filename TEXT, description TEXT)");
	cout << createQuery << endl;
	vtkSQLQuery* query = this->Connection->GetQueryInstance();
	query->SetQuery( createQuery.c_str());
	if (!query->Execute())
	{
		cerr << "Create query failed" << query->GetLastErrorText()<<endl;
	}
}

void vtkimSQLite::InsertQuery()
{
	vtkStdString sqlstring="INSERT INTO imsqlite_test (filename, description) ";
	sqlstring+="VALUES('";
	sqlstring+=FileName;
	sqlstring+="','";
	sqlstring+=Description;
	sqlstring+="')";

	if (!this->Connection->IsOpen())
	{
	    cout<<"Cannot connect to database. Exit."<<endl;
	}

	vtkSQLQuery* query = this->Connection->GetQueryInstance();
	query->SetQuery(sqlstring.c_str());
	cout<<sqlstring<<endl;
	if (!query->Execute())
	{
		cerr << "Insert query failed: " << query->GetLastErrorText()<<endl;
	}
}

void vtkimSQLite::DeleteQuery(const char* filename)
{
	vtkStdString sqlstring= "DELETE from imsqlite_test WHERE filename = ";
	sqlstring+="'";
	sqlstring+=filename;
	sqlstring+="'";
	vtkSQLQuery* query = this->Connection->GetQueryInstance();
	cout<<sqlstring<<endl;
	query->SetQuery( sqlstring);
	cerr << endl << "Running query: " << query->GetQuery() << endl;

	if (!query->Execute())
	{
		cerr << "Query failed" <<query->GetLastErrorText()<< endl;
	}
}
void vtkimSQLite::Disconnect()
{
  if (this->Connection==NULL)
    return;

  if (this->Connection->IsOpen())
    this->Connection->Close();
}
void vtkimSQLite::PrintTable()
{
	vtkStdString queryText = "SELECT * from imsqlite_test ";
	vtkSQLQuery* query = this->Connection->GetQueryInstance();
	query->SetQuery( queryText );
	if (!query->Execute())
	{
		cerr << "Query failed" <<query->GetLastErrorText()<< endl;
	}
	for ( int col = 0; col < query->GetNumberOfFields(); col++)
	{
		if ( col > 0)
		{
		    cerr << ", ";
		}
		cerr << query->GetFieldName( col );
	}
	cerr << endl;
	while ( query->NextRow())
	{
		for ( int field = 0; field < query->GetNumberOfFields(); field++)
		{
		    if ( field > 0)
		    {
		    	cerr << ", ";
		    }
		    cerr << query->DataValue( field ).ToString().c_str();
		}
		    cerr << endl;
	}
}


