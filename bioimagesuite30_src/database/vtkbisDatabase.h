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


#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <vtkObject.h>
#include <vtkSQLDatabase.h>
#include <vtkStdString.h>
#include "vtkSQLQuery.h"
#include <vtkVariant.h>

class vtkbisDatabase : public vtkObject
{
public:

  static vtkbisDatabase *New();
  vtkTypeMacro(vtkbisDatabase,vtkObject);


  // Description:
  // Mode = 0 = mySQL, 1 = SQLite
  // If BIOIMAGESUITE3_USE_MYSQL_DATABASE_MODULE not defined then it is always = 1
  vtkGetMacro(Mode,int);
  virtual int UsingSQLite() { return (this->Mode==1); }


  // Description:
  // Initialize as MySQL


  virtual void Initialize(char* name, char* host, char* user, char* password, unsigned int port);

  // Initialize as SQLLite
  virtual void InitializeSQLite(char* url);

  virtual void DebugPrint();

  // Connect/Disconnect
  // ------------------
  virtual int  Connect();
  virtual void Disconnect();

  // Queries
  // -------
  virtual vtkStdString RunQuery(const char* searchstring);
  virtual vtkStdString RunUpdate(const char* searchstring);
  virtual vtkSQLQuery*  RunComplexQuery(const char* searchstring);


  // Query
  // Get Query Value as String
  virtual vtkStdString GetQueryValueAsString(vtkSQLQuery* query,int value);

protected:

  
  vtkbisDatabase();
  virtual ~vtkbisDatabase();
  vtkbisDatabase(const vtkbisDatabase&) {};
  void operator=(const vtkbisDatabase&) {};


  // Internal Stuff
  virtual int ConnectMySQL();
  virtual int ConnectSQLite();

  //BTX
  vtkStdString name;
  vtkStdString host;
  vtkStdString user;
  vtkStdString password;
  vtkStdString sqliteurl;
  unsigned int port;
  vtkSQLDatabase* Connection;
  int Mode;
  //ETX
};

#endif // DATABASE_HPP

