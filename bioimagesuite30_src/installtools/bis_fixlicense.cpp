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

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include "bioimagesuite_settings.h"
// ----------------------------------------------------------------------------------------------------

int findextension(const char* orig,const char* lookfor,const char* lookfor2)
{
  if (lookfor==NULL)
    return 0;

  
  int l_orig=strlen(orig);
  int l_lookfor=strlen(lookfor);

  //fprintf(stdout,"\n\n Looking for %s in %s (%s) \n",lookfor, orig,&orig[l_orig-l_lookfor]);

  if (l_lookfor>l_orig)
      return 0;
  
  if (strcmp(&orig[l_orig-l_lookfor],lookfor)==0)
    {
      //fprintf(stdout,"Found %s in %s\n",lookfor,orig);
      return 1;
    }

  if (lookfor2==NULL)
    return 0;

  int l_lookfor2=strlen(lookfor2);  
  //fprintf(stdout,"\t\t Take 2 looking for %s in %s (%s)\n",lookfor2, orig,&orig[l_orig-l_lookfor2]);

  if (l_lookfor2>l_orig)
      return 0;


  //  fprintf(stdout,"Looking for %s in %s\n",lookfor2, &orig[l_orig-l_lookfor2]);


  if (strcmp(&orig[l_orig-l_lookfor2],lookfor2)==0)
    {
      //fprintf(stdout,"Found %s in %s\n",lookfor2,orig);
      return 1;
    }

  return 0;
}

// ----------------------------------------------------------------------------------------------------
int fixandcleancodefile(const char* fname,std::string buffer,std::string licensecpp,std::string licensetcl)
{
  std::string fn=fname;
  int ignorenr=0;

  if (findextension(fname,".sqlite",".db3"))
    return 0;


  if (fn.find("bis_fixlicense")!=std::string::npos)
    {
      //      fprintf(stdout," -- Ignoring NumRecipes code in %s\n",fname);
      ignorenr=1;
    }

  int tclmode=findextension(fname,".tcl",".txt");
  if (tclmode==0)
    tclmode=findextension(fname,".cmake",".cmake");





  int cppmode=0;
  if (tclmode==0)
    {
      if (findextension(fname,".cpp",".h"))
	cppmode=1;
      else if (findextension(fname,".c",".txx"))
	cppmode=1;
      else if (findextension(fname,".h.in",".cc"))
	cppmode=1;
      else if (findextension(fname,".cxx",".hpp"))
	cppmode=1;
    }

  if (cppmode==0 && tclmode==0)
    {
      //      fprintf(stdout,"-- Ignoring file %s (not a text file most likely)\n",fname,tclmode);
      return 0;
    }



  
  buffer="";

  std::string commentcode="//";
  if (tclmode)
    commentcode="#";
  
  fprintf(stdout,"-- Fixing License for filename %s (tclmode=%d)\n",fname,tclmode);

  std::ifstream myfile (fname);
  if (!myfile.is_open())
    {
      //      fprintf(stdout,"File %s does not exist\n",fname);
      return 0;
    }


  std::string line;
  int count=0,goodcount=0;
  while ( !myfile.eof())
    {
      getline(myfile,line);
      if (count==0)
	{
	  if (line.find("#!/bin")!=std::string::npos)
	    goodcount=4;
	}
      
      if (count==goodcount)
	{
	  if (tclmode==1)
	    buffer+=licensetcl;
	  else
	    buffer+=licensecpp;
	}

      if (ok==std::string::npos)
	{
	  if (!ignorenr)
	    {
	      ok=line.find("//BEGINNR");
#ifdef BIOIMAGESUITE3_EXCLUDE_CODE
	      if (ok==std::string::npos)	  
		{
		  ok=line.find("//BEGIN_EXCLUDE");
		}
#endif
	    }
	  else
	    ok=std::string::npos;

	  if (ok==std::string::npos)
	    {
	      buffer+=line;
	      buffer+="\n";
	    } 
	  else
	    {
	      if (cppmode)
		{
		  buffer+="\t\t" + commentcode + "BEGIN OMITTED\n";
		  buffer+="\t\t" + commentcode + " Some lines of  code have been removed as they\n";
		  buffer+="\t\t" + commentcode + " derive/depend on the presence of code from the book Numerical Recipes in C\n";
		  buffer+="\t\t" + commentcode + " which can not be distributed in source form.\n";
		  buffer+="\t\t" + commentcode + " We have REPLACED the missing code with an alternative version\n";
		  buffer+="\t\t" + commentcode + " ... see below. The missing code is not mostly OBSOLETE.\n";
		  buffer+="\t\t" + commentcode + "END OMITTED\n\n";
		}
	      else
		{
		  //fprintf(stderr,"Line = %s\n",line.c_str());
		  buffer+="# Excluded TCL Code \n";
		  //fprintf(stderr,"%s\n",buffer.c_str());
		}

	      int foundendnr=0;
	      while ( foundendnr == 0 )
		{
		  getline(myfile,line);
		  if (line.find("//ENDNR")!=std::string::npos)
		    foundendnr=1;
#ifdef BIOIMAGESUITE3_EXCLUDE_CODE
		  else if (line.find("//END_EXCLUDE")!=std::string::npos)
		    foundendnr=1;
#endif
		}
	    }
	}
      ++count;
    }
  myfile.close();

  std::ofstream fout(fname);
  if (fout.is_open())
    {
      fout << buffer;
      fout.close();
    }
  else
    {
      //      fprintf(stdout,"Failed to open %s\n",fname);
    }
  return 1;
}

// ----------------------------------------------------------------------------------------------------
void usage(char* cmdname)
{
  fprintf(stdout,"\n%s usage\n %s file1 file2 file3\t or\t %s -list filelist.txt [ pathname ] \n",
	  cmdname,cmdname,cmdname);
  exit(0);
}
// ----------------------------------------------------------------------------------------------------
int main(int argc,char *argv[])
{
  std::string licensecpp;

  std::string licensetcl;


  if (argc<=1)
    {
      usage(argv[0]);
    }

  std::string buffer;
  
  if (strcmp(argv[1],"-list")!=0)
    {
      for(int i=1;i<argc;i++)
	fixandcleancodefile(argv[i],buffer,licensecpp,licensetcl);
      return(0);
    }

  if (argc<=2)
    {
      usage(argv[0]);
    }
  fprintf(stdout,"\n++ Fixing Licenses\n");
  fprintf(stdout,"++ Reading Filelist from %s\n",argv[2]);
  std::ifstream myfile (argv[2]);
  if (!myfile.is_open())
    {
      fprintf(stdout,"File %s does not exist -- perhaps in make package mode\n",argv[2]);
      exit(0);
    }

  std::string line;
  std::string pathname;
  std::string tmp;

  getline(myfile,line);
  getline(myfile,line);
  getline(myfile,line);
  if (argc>3)
    pathname=argv[3];
  else
    pathname=line;

  fprintf(stdout,"++ Path = %s\n",pathname.c_str());

  getline(myfile,line);

  while ( !myfile.eof())
    {
      std::string fname;
      fname=pathname;
      fname+="/";
      getline(myfile,line);
      if (line.find("#")!=0 && line.length()>1)
	{
	  fname+=line;
	  fixandcleancodefile(fname.c_str(),buffer,licensecpp,licensetcl);
	}
      /*      else
	{
	  fprintf(stdout,"\t ** Ignoring %s\n",line.c_str());
	  }*/
    }
  myfile.close();
  exit(0);
}

