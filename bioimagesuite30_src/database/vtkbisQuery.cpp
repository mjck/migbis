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

#include "vtkbisQuery.h"
#include "vtkObjectFactory.h"

using namespace std;

vtkbisQuery* vtkbisQuery::New()
{

  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisQuery");
  if(ret)
    {
      return (vtkbisQuery*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisQuery;
}


vtkbisQuery::vtkbisQuery()
{
  this->QueryName=new char[2];
  this->QueryCategory=new char[2];
  this->QueryDescription=new char[2];
  this->QueryArg=new char[2];
  this->QueryTable=new char[2];
  this->QueryLink=new char[2];
  this->QueryField=new char[2];
  this->QueryWhere=new char[2];
  this->QueryOrder=new char[2];
  this->QueryGroup=new char[2];
  this->QueryHaving=new char[2];


  this->SetQueryName("");
  this->SetQueryCategory("");
  this->SetQueryDescription("");
  this->SetQueryArg("");
  this->SetQueryTable("");
  this->SetQueryLink("");
  this->SetQueryField("");
  this->SetQueryWhere("");
  this->SetQueryOrder("");
  this->SetQueryGroup("");
  this->SetQueryHaving("");
}

vtkbisQuery::~vtkbisQuery()
{
  if (this->QueryName !=NULL ) delete [] this->QueryName;
  if (this->QueryCategory !=NULL ) delete [] this->QueryCategory;
  if (this->QueryDescription !=NULL ) delete [] this->QueryDescription;
  if (this->QueryArg !=NULL ) delete [] this->QueryArg;
  if (this->QueryTable !=NULL ) delete [] this->QueryTable;
  if (this->QueryLink !=NULL ) delete [] this->QueryLink;
  if (this->QueryField !=NULL ) delete [] this->QueryField;
  if (this->QueryWhere !=NULL ) delete [] this->QueryWhere;
  if (this->QueryOrder !=NULL ) delete [] this->QueryOrder;
  if (this->QueryGroup !=NULL ) delete [] this->QueryGroup;
  if (this->QueryHaving !=NULL ) delete [] this->QueryHaving;
}


void vtkbisQuery::DebugPrint()
{
  cout<<"******Print Query Infomation******"<<endl;
  cout<<"QueryName: "<<QueryName<<endl;
  cout<<"QueryCategory: "<<QueryCategory<<endl;
  cout<<"QueryDescription: "<<QueryDescription<<endl;
  cout<<"QueryArg: "<<QueryArg<<endl;
  cout<<"QueryTable: "<<QueryTable<<endl;
  cout<<"QueryLink: "<<QueryLink<<endl;
  cout<<"QueryField: "<<QueryField<<endl;
  cout<<"QueryWhere: "<<QueryWhere<<endl;
  cout<<"QueryOrder: "<<QueryOrder<<endl;
  cout<<"QueryGroup: "<<QueryGroup<<endl;
  cout<<"QueryHaving: "<<QueryHaving<<endl;
  cout<<"******End Query Infomation******"<<endl;
  
}
int vtkbisQuery::SearchQuery(const char* QueryName, const char* QueryCategory)
{

  std::string sqlstring;
  sqlstring="select QueryDescription,QueryArg,QueryTable,QueryLink,QueryField,QueryWhere,QueryOrder,QueryGroup,QueryHaving from Query where QueryName = '";
  sqlstring+=QueryName;
  sqlstring+="' and QueryCategory = '";
  sqlstring+=QueryCategory;
  sqlstring+="'"; 
//cout<<sqlstring<<endl;
  
  if(!this->Connect())
  {
    cout<<"Cannot connect to database. Exit."<<endl;
    return 0;
  }

  this->QueryName    	= (char*)QueryName;
  this->QueryCategory   = (char*)QueryCategory;

  vtkSQLQuery* query=this->Connector->RunComplexQuery(sqlstring.c_str());
  if (query==NULL)
    return 0;

  while (query->NextRow())
    {
      this->SetQueryDescription(query->DataValue(0).ToString().c_str());
      this->SetQueryArg(query->DataValue(1).ToString().c_str());
      this->SetQueryTable(query->DataValue(2).ToString().c_str());
      this->SetQueryLink(query->DataValue(3).ToString().c_str());
      this->SetQueryField(query->DataValue(4).ToString().c_str());
      this->SetQueryWhere(query->DataValue(5).ToString().c_str());
      this->SetQueryOrder(query->DataValue(6).ToString().c_str());
      this->SetQueryGroup(query->DataValue(7).ToString().c_str());
      this->SetQueryHaving(query->DataValue(8).ToString().c_str());
    }
  
  query->Delete();
  return 1;
}

const char* vtkbisQuery::BuildQuery()
{
  std::string sqlstring;
  string ttt;
  sqlstring="select ";
  sqlstring+=QueryField;
  sqlstring+=" from ";
  sqlstring+=QueryTable;
 
  string parsedQueryWhere = QueryWhere;
  if(strlen(QueryLink)>0)
  {
    parsedQueryWhere+=" | ";
    parsedQueryWhere+=QueryLink;
  }

  ttt=parsedQueryWhere;
  fprintf(stderr,"ttt (C++) =%s\n",ttt.c_str());
  vector<string> v;
  Tokenize(ttt,v,"|");
  if(v.size()>0) 
   sqlstring+=" where ";
  for(int i=0;i<v.size();i++)
  {
    if(i>0)
      sqlstring+=" and ";
    sqlstring+=v[i];
  }    
  
  if(strlen(QueryOrder)>0)
  {
    sqlstring+=" order by ";
    sqlstring+=QueryOrder;  
  }
  
  if(strlen(QueryHaving)>0)
  {
    sqlstring+=" having ";
    sqlstring+=QueryHaving;  
  }

  if(strlen(QueryGroup)>0)
  {
    sqlstring+=" group by ";
    sqlstring+=QueryGroup;  
  }

  ttt=QueryArg;
  vector<string> arg_name;
  vector<string> arg_value;  
  Tokenize(ttt,arg_name,"|");
  for(int i=0;i<arg_name.size();i++)
  {
    cout<<"Please input the value of argument "<<arg_name[i]<<endl;
    cin>>ttt;
    arg_value.push_back(ttt);
  }
  for(int i=0;i<arg_value.size();i++)
  {
    string s;
    string t="$";
    stringstream out;
    out<<i+1;
    s=out.str();
    sqlstring=StringReplace(sqlstring,t+s,arg_value[i]);
  }

  return sqlstring.c_str();

}


const char* vtkbisQuery::BuildQuery(const char* inputstring)
{
 std::string sqlstring;
  string ttt;
  sqlstring="select ";
  sqlstring+=QueryField;
  sqlstring+=" from ";
  sqlstring+=QueryTable;
  
  string parsedQueryWhere = QueryWhere;
  if(strlen(QueryLink)>0)
    {
      parsedQueryWhere+=" and ";
      parsedQueryWhere+=QueryLink;
    }
  
  ttt=parsedQueryWhere;
  

  vector<string> v;
  Tokenize(ttt,v,"|");
  if(v.size()>0) 
    sqlstring+=" where ";
  for(int i=0;i<v.size();i++)
    {
      if(i>0)
	sqlstring+=" and ";
      sqlstring+=v[i];
    }    
  
  if(strlen(QueryOrder)>0)
    {
      sqlstring+=" order by ";
      sqlstring+=QueryOrder;  
    }
  
  if(strlen(QueryHaving)>0)
    {
      sqlstring+=" having ";
      sqlstring+=QueryHaving;  
    }
  
  if(strlen(QueryGroup)>0)
    {
      sqlstring+=" group by ";
      sqlstring+=QueryGroup;  
    }
  
  ttt=QueryArg;
  vector<string> arg_name;
  vector<string> arg_value;  
  Tokenize(ttt,arg_name,"|");

  string ttt2=inputstring;
  vector<string> param_name;
  Tokenize(ttt2,param_name,"|");

  if (param_name.size()!=arg_name.size())
    {
      fprintf(stderr,"Not Enough Arguments %d vs %d \n",param_name.size(),arg_name.size());
      sqlstring="Error";
      return sqlstring.c_str();
    }
  
  for(int i=0;i<param_name.size();i++)
    {
      string s;
      string t="$";
      stringstream out;
      out<<i+1;
      s=out.str();
      if (param_name[i]!="*")
	sqlstring=StringReplace(sqlstring,t+s,param_name[i]);
      else
	sqlstring=StringReplace(sqlstring,t+s,"");
    }

  return sqlstring.c_str();
}


void vtkbisQuery::Tokenize(const std::string& str, vector<std::string>& tokens, const std::string& delimiters )
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

std::string vtkbisQuery::StringReplace(std::string input,std::string old_pattern,std::string new_pattern)
{
  string output = input;
  string::size_type found_at = output.find( old_pattern );
  while( string::npos != found_at )
  {
    output.replace( found_at, old_pattern.length(), new_pattern );
    found_at = output.find( old_pattern, found_at + old_pattern.length() );
  }
 
  return output;
}

