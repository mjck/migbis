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

// --------------------------------------------------------------------------------------

int fixscript(std::string fname,std::string pathname,std::string buffer,
	      std::string vtk_dir,std::string itk_dir)
{
  buffer="";

  std::ifstream myfile (fname.c_str());
  if (!myfile.is_open())
    {
      fprintf(stdout,"File %s does not exist\n",fname.c_str());
      return 0;
    }

  int isbatchfile=0;
  int iscshfile=0;

  if (fname.rfind(".bat")!=std::string::npos)
    isbatchfile=1;
  else if (fname.rfind(".csh")!=std::string::npos)
    iscshfile=1;

  fprintf(stdout,"-- Fixing Paths for filename %s (windows batchfile=%d,cshfile=%d)\n",fname.c_str(),isbatchfile,iscshfile);

  std::string line;
  int foundendnr=0;
  while ( !myfile.eof())
    {
      getline(myfile,line);
      int first=0;
      if (line.find("bis_fix_script_begin")==std::string::npos || foundendnr==1)
	{
	  buffer+=line;
	  buffer+="\n";
	}
      else
	{
	  if (iscshfile==1)
	    {
	      buffer+="#bis_fix_script_begin\n";
	      buffer+="setenv BASE " +  pathname +"\n";
	      buffer+="setenv BIS_VTK_DIR " + vtk_dir + "\n";
	      buffer+="setenv BINPATH " + pathname + "/bin\n";
	      buffer+="setenv BASELIB " + pathname + "/lib\n";
	      buffer+="#bis_fixscript_end\n";
	    }
	  else if (isbatchfile==1)
	    {
	      buffer +="REM bis_fix_script_begin\n";
	      buffer +="@set BIOIMAGESUITE=" + pathname + "\n";
	      buffer +="@set BIS_VTK_DIR=" + vtk_dir +"\n";
	      buffer +="REM bis_fixscript_end\n";
	    }
	  else
	    {
#ifdef _WIN32
	      std::string tmp=pathname;	      pathname=fixpathformsys(tmp);
	      tmp=vtk_dir;	      vtk_dir=fixpathformsys(tmp);
#endif
	      buffer+="#bis_fix_script_begin\n";
	      buffer+="BASE=" +  pathname +"\n";
	      buffer+="BIS_VTK_DIR=" + vtk_dir + "\n";
	      buffer+="BINPATH=" + pathname + "/bin\n";
	      buffer+="BASELIB=" + pathname + "/lib\n";

	      buffer+="#bis_fixscript_end\n";
	    }
	  

	  while ( foundendnr == 0 )
	    {
	      getline(myfile,line);
	      if (line.find("bis_fixscript_end")!=std::string::npos)
		foundendnr=1;
	    }
	}
    }
  myfile.close();

  std::ofstream fout(fname.c_str());
  if (fout.is_open())
    {
      fout << buffer;
      fout.close();
    }
  else
    {
      fprintf(stdout,"Failed to open %s\n",fname.c_str());
    }
  return 1;
}


void usage(char* cmdname)
{
  fprintf(stdout,"This script is used to modify shell scripts in BioImage Suite\n\t to use correct paths after installation or as a post-packaging step");
  fprintf(stdout,"\n%s usage\n %s  mode scriptlist.txt [ newpath ]\n",
	  cmdname,cmdname);
  fprintf(stdout,"mode is one of -install or -package\n");
  fprintf(stdout,"if mode is -package then the newpath argument is mandatory\n");
  fprintf(stdout,"scriptlist.txt usually sits in bioimagesuite30/lib after installation\n");
  exit(0);
}
// ----------------------------------------------------------------------------------------------------------

int createregistryfile(std::string fname,std::string pathname,std::string buffer)
{
  fprintf(stdout," ++ Fixing registry path %s -->",pathname.c_str());
  std::string newpath=fixpathforwindows(pathname);
  fprintf(stdout,"%s\n",newpath.c_str());

  buffer="";
  buffer+="REGEDIT\n";
  buffer+="HKEY_CLASSES_ROOT\\.tcl = TclScript\n";
  buffer+="HKEY_CLASSES_ROOT\\TclScript = BioImage Suite tcl script file\n";
  buffer+="HKEY_CLASSES_ROOT\\TclScript\\DefaultIcon =" + newpath + "\\images\\bioimagesuite.ico\n";
  buffer+="HKEY_CLASSES_ROOT\\TclScript\\shell\\open\\command = " + newpath + "\\bin\\bisvtk.exe";
  buffer+=" \"%1\" %* \n";

  fprintf(stdout,"++ Creating Registry File %s\n",fname.c_str());
      

  std::ofstream fout(fname.c_str());
  if (fout.is_open())
    {
      fout << buffer;
      fout.close();
    }
  else
    {
      fprintf(stdout,"Failed to open %s\n",fname.c_str());
    }
  return 1;
}

// ----------------------------------------------------------------------------------------------------------

int main(int argc,char *argv[])
{
  int error=0,mode=0;
  std::string modestr;

  if (argc<2)
    {
      error=1;
    }
  else
    {
      modestr=argv[1];
      if (modestr=="-package") 
	{
	  mode=1;
	  if (argc==2)
	    error=1;
	}
    }

  if (error==1)
    {
      fprintf(stdout,"Not Enough Arguments");
      usage(argv[0]);
    }

  std::string vtk_dir="";
  std::string itk_dir="";
  std::string pathname="";
  std::string buffer;

  fprintf(stdout,"\n++ Fixing Script Paths argc=%d, this=%s\n",argc,argv[0]);

  int onwin=0, onmac=0, onunix=1, usingvs2005=0;
  
#ifdef _WIN32
  onwin=1;
  onunix=0;
#  ifdef CMAKE_COMPILER_2005
  usingvs2005=1;
#  endif
#endif

#ifdef __APPLE__
  onmac=1;
  onunix=1;
#endif

  int sixtyfourbitlinux=0;
  if (onmac==0 && onwin==0)
    {
      int sz=sizeof(void*);
      if (sz==8)
	sixtyfourbitlinux=1;
    }
  // End of Configuration


  
    

  fprintf(stdout,"++ Reading Filelist from %s\n",argv[2]);
  std::ifstream myfile (argv[2]);
  if (!myfile.is_open())
    {
      fprintf(stdout,"File %s does not exist -- perhaps in make package mode\n",argv[2]);
      exit(0);
    }
  
  std::string line;
  std::string tmp;
  
  getline(myfile,line);  getline(myfile,line);  getline(myfile,line);
  if (argc>3)
    pathname=argv[3];
  else
    pathname=line;

  
  getline(myfile,line);  getline(myfile,line);  vtk_dir=line;
  getline(myfile,line);  getline(myfile,line);  itk_dir=line;
  getline(myfile,line);
  
  if (mode==1)
    {
      //fprintf(stdout,"\n++ Fixing Scripts package mode: onunix=%d onmac=%d onwin=%d usingvs2005=%d sixtyfourbitlinux=%d\n",
      //	      onunix,onmac,onwin,usingvs2005,sixtyfourbitlinux);
      vtk_dir=pathname+"/../bioimagesuite3_base/vtk52/lib/vtk-5.2";
      itk_dir=pathname+"/../bioimagesuite3_base/itk30/lib/InsightToolkit";  
      
      if (sixtyfourbitlinux)
	{
	  vtk_dir=pathname+"/../bioimagesuite3_base64/vtk52/lib/vtk-5.2";
	  itk_dir=pathname+"/../bioimagesuite3_base64/itk30/lib/InsightToolkit";  
	}
      
      //      fprintf(stderr,"vtk_dir=%s\nitk_dir=%s\npathname=%s\n",vtk_dir.c_str(),itk_dir.c_str(),pathname.c_str());
    }
  
  if (modestr!="-package")
    modestr="-install";

  fprintf(stdout,"++ Mode = %s \n",modestr.c_str());
  fprintf(stdout,"++ Using Pathname %s\n",pathname.c_str());
  fprintf(stdout,"++ Using VTK Pathname %s\n",vtk_dir.c_str());


  while ( !myfile.eof())
    {
      std::string fname;
      fname=pathname;
      fname+="/";
      getline(myfile,line);
      if (line.length()>1)
	{
	  fname+=line;
	  fixscript(fname,pathname,buffer,vtk_dir,itk_dir);
	}
    }
  myfile.close();
  
#ifdef _WIN32
  createregistryfile(pathname+"/bin/bisvtkreg.reg",pathname,buffer);
#endif
  
  exit(0);
}
/*
  // Auto mode based on path -- used for cpack postinstall
  pathname=argv[1];


  
  if (!onwin)
    {
      fixscript(pathname+"/setpaths.sh",pathname,buffer,vtk_dir,itk_dir);
      fixscript(pathname+"/setpaths.csh",pathname,buffer,vtk_dir,itk_dir);
      fixscript(pathname+"/start_bioimagesuite",pathname,buffer,vtk_dir,itk_dir);
      fixscript(pathname+"/setpaths_console.sh",pathname,buffer,vtk_dir,itk_dir);
      fixscript(pathname+"/bis_console",pathname,buffer,vtk_dir,itk_dir);
      fixscript(pathname+"/bis_console_menu",pathname,buffer,vtk_dir,itk_dir);
      if (!onmac)
	fixscript(pathname+"/createmenuentries.sh",pathname,buffer,vtk_dir,itk_dir);
    }
  else
    {
      createregistryfile(pathname+"/bin/bisvtkreg.reg",pathname,buffer);
      fixscript(pathname+"/setpaths.bat",pathname,buffer,vtk_dir,itk_dir);
      fixscript(pathname+"/bioimagesuite_console.bat",pathname,buffer,vtk_dir,itk_dir);
      fixscript(pathname+"/bioimagesuite.bat",pathname,buffer,vtk_dir,itk_dir);
      fixscript(pathname+"/setassociations.bat",pathname,buffer,vtk_dir,itk_dir);
      fixscript(pathname+"/bisexec.bat",pathname,buffer,vtk_dir,itk_dir);
      fixscript(pathname+"/setpaths.sh",pathname,buffer,vtk_dir,itk_dir);
    }
}
*/

