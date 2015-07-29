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
#include "bioimagesuite_settings.h"

std::string fixpathforwindows(std::string path)
{
  int done=0;
  std::string newpath=path;

  while ( done==0)
    {
      int f=newpath.rfind("/");
      if (f==std::string::npos)
	{
	  done=1;
	}
      else
	{
	  newpath.replace(f,1,"\\");
	}
    }
  return newpath;
}

std::string fixpathformsys(std::string path)
{
  int done=0;
  std::string newpath=path;

  while ( done==0)
    {
      int f=newpath.rfind(":");
      if (f==std::string::npos)
	{
	  done=1;
	}
      else
	{
	  newpath.replace(f,1,"");
	}
    }

  newpath="/"+newpath;
  return newpath;
}


int fixscript(std::string scriptname,std::string in_newpath,std::string variablename,
	      std::string buffer)
{
  fprintf(stdout,"\n++ Scriptname = %s (path=%s) (variablename=%s)\n",
	  scriptname.c_str(),
	  in_newpath.c_str(),
	  variablename.c_str());


  std::string newpath=in_newpath;





  buffer="";

  std::ifstream myfile (scriptname.c_str());
  if (!myfile.is_open())
    {
      fprintf(stdout,"File %s does not exist\n",scriptname.c_str());
      return 0;
    }

  int isbatchfile=0;
  int iscshfile=0;

  std::string tolookfor= variablename+"=";

  if (scriptname.rfind(".bat")!=std::string::npos)
    {
      isbatchfile=1;
      tolookfor= "@SET "+variablename+"=";
    }
  else if (scriptname.rfind(".csh")!=std::string::npos)
    {
      iscshfile=1;
      tolookfor= "setenv "+variablename;
    }

  fprintf(stdout,"-- Looking for %s (windows batchfile=%d,cshfile=%d)\n",tolookfor.c_str(),isbatchfile,iscshfile);

  std::string line;
  int foundvariable=0;
  while ( !myfile.eof())
    {
      getline(myfile,line);
      int first=0;

      if (line.find(tolookfor)==std::string::npos || foundvariable==1)
	{
	  buffer+=line;
	  buffer+="\n";
	}
      else
	{
	  if (iscshfile==1)
	    {
	      buffer+="setenv " + variablename + " " + newpath +"\n";
	    }
	  else if (isbatchfile==1)
	    {
	      std::string wnewpath=fixpathforwindows(in_newpath);
	      buffer +="@SET " + variablename + "=" + wnewpath + "\n";
	    }
	  else
	    {
#ifdef _WIN32
	      newpath=fixpathformsys(in_newpath);
#endif
	      buffer+= variablename +"=" +  newpath +"\n";
	    }

	  foundvariable=1;
	}
    }
  myfile.close();

  std::ofstream fout(scriptname.c_str());
  if (fout.is_open())
    {
      fout << buffer;
      fout.close();
      return 1;
    }

  
  fprintf(stdout,"Failed to open %s\n",scriptname.c_str());
  return(0);
}

// ----------------------------------------------------------------------------------------
int fixcmakepaths(std::string in_newpath,std::string oldpath,std::string cmakename)
{
  std::string scriptname=in_newpath+"/"+cmakename;
  std::string searchString=oldpath;
  //  std::string replaceString=in_newpath;


  std::string replaceString =fixpathformsys(in_newpath);


  fprintf(stdout,"\n++ Build Config File Name = %s (newpath=%s) (oldpath=%s)\n",
	  scriptname.c_str(),
	  in_newpath.c_str(),
	  oldpath.c_str());

  if ( searchString == replaceString )
    {
      fprintf(stdout,"\n ++++++  Nothing to do (%s=%s)\n",in_newpath.c_str(),oldpath.c_str());
      return 1;
    }
  

  
  std::string buffer="";

  std::ifstream myfile (scriptname.c_str());
  if (!myfile.is_open())
    {
      fprintf(stdout,"File %s does not exist\n",scriptname.c_str());
      return 0;
    }

  std::string line;
  while ( !myfile.eof())
    {
      getline(myfile,line);
      buffer+=line;
      buffer+="\n";
    }
  myfile.close();


  std::string::size_type pos = 0;
  while ( (pos = buffer.find(searchString, pos)) != std::string::npos ) 
    {
      buffer.replace( pos, searchString.size(), replaceString );
      pos++;
    }
  
  std::ofstream fout(scriptname.c_str());
  if (fout.is_open())
    {
      fout << buffer;
      fout.close();
      return 1;
    }

  fprintf(stdout,"Failed to open %s\n",scriptname.c_str());
  return(0);
}
// ----------------------------------------------------------------------------------------

void usage(char* cmdname)
{
  fprintf(stdout,"\n%s usage\n %s newpath \n or %s scriptname newpath variablename\n",
	  cmdname,cmdname,cmdname);
  exit(0);
}

int main(int argc,char *argv[])
{
  if (argc<2)
    {
      usage(argv[0]);
    }

  std::string buffer="";

  if (argc>4)
    {
      std::string scriptname=argv[1];
      std::string newpath=argv[2];
      std::string variablename=argv[3];
      fixscript(scriptname,newpath,variablename,buffer);
      return(0);
    }

  std::string newpath=argv[1];

  int onwin=0;
  int onmac=0;
  int onunix=1;
  
#ifdef _WIN32
  onwin=1;
  onunix=0;
#endif

#ifdef __APPLE__
  onmac=1;
  onunix=1;
#endif

  int sixtyfourbitlinux=0;
  if (onmac==0 && onwin==0)
    {
      int sz=sizeof(void*);
      fprintf(stdout,"Sz=%d\n",sz);
      if (sz==8)
	sixtyfourbitlinux=1;
    }


  fprintf(stdout,"\n++ Fixing Base Scripts: onunix=%d onmac=%d onwin=%d 	sixtyfourbitlinux=%d\n",
	  onunix,onmac,onwin,sixtyfourbitlinux);


  if (sixtyfourbitlinux)
    {
      fixscript(newpath+"/bioimagesuite3_base64/setpaths.csh",newpath+"/bioimagesuite3_base64","BASEROOT",buffer);
      fixscript(newpath+"/bioimagesuite3_base64/setpaths.sh", newpath+"/bioimagesuite3_base64","BASEROOT",buffer);
    }
  else if (onunix)
    {
      fixscript(newpath+"/bioimagesuite3_base/setpaths.csh",newpath+"/bioimagesuite3_base","BASEROOT",buffer);
      fixscript(newpath+"/bioimagesuite3_base/setpaths.sh", newpath+"/bioimagesuite3_base","BASEROOT",buffer);
    }
  else
    {
      fixscript(newpath+"/bioimagesuite3_base/setpaths.bat",newpath+"/bioimagesuite3_base","BASEROOT",buffer);
      // This is for msys
      fixscript(newpath+"/bioimagesuite3_base/setpaths.sh",newpath+"/bioimagesuite3_base","BASEROOT",buffer);
    }


  std::string cmakefiles[11];
  cmakefiles[0]="gdcm2/lib/gdcm-2.0/GDCMBuildSettings.cmake";
  cmakefiles[1]="gdcm2/lib/gdcm-2.0/GDCMConfig.cmake";
  cmakefiles[2]="gdcm2/lib/gdcm-2.0/GDCMLibraryDepends.cmake";
  cmakefiles[3]="itk310/lib/InsightToolkit/ITKBuildSettings.cmake";
  cmakefiles[4]="itk310/lib/InsightToolkit/ITKConfig.cmake";
  cmakefiles[5]="vtk52/lib/vtk-5.2/VTKBuildSettings.cmake";
  cmakefiles[6]="vtk52/lib/vtk-5.2/VTKConfig.cmake";
  cmakefiles[7]="vtk52/lib/vtk-5.2/VTKLibraryDepends.cmake";
  if (!onmac)
    {
      cmakefiles[8]="tcl84/lib/itclConfig.sh";
      cmakefiles[9]="tcl84/lib/tclConfig.sh";
      cmakefiles[10]="tcl84/lib/tkConfig.sh";
    }
  else
    {
      cmakefiles[8]="tcl84_x11/lib/itclConfig.sh";
      cmakefiles[9]="tcl84_x11/lib/tclConfig.sh";
      cmakefiles[10]="tcl84_x11/lib/tkConfig.sh";
    }


  /*  for (int i=0;i<11;i++)
    {
      if (sixtyfourbitlinux)
	{
	  fixcmakepaths(newpath+"/bioimagesuite3_base64","/usr/local/bioimagesuite3_base_64",cmakefiles[i]);
	}
      else if (onunix)
	{
	  fixcmakepaths(newpath+"/bioimagesuite3_base","/usr/local/bioimagesuite3_base",cmakefiles[i]);
	}
      else
	{
	  fixcmakepaths(newpath+"/bioimagesuite3_base","c:/yale/bioimagesuite3_base",cmakefiles[i]);
	  fixcmakepaths(newpath+"/bioimagesuite3_base","c:/unix/msys/1.0/yale/bioimagesuite3_base",cmakefiles[i]);
	}
	}*/
}

