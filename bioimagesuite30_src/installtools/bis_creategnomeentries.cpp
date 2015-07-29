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



void usage(char* cmdname)
{
  fprintf(stdout,"\n%s usage\n %s pathname [ version ] \n\n",
	  cmdname);
  exit(0);
}



int creategnomefile(std::string fname,std::string pathname,std::string command,std::string realname,std::string version)
{
  std::string buffer="";

  buffer+="[Desktop Entry]\n";
  buffer+="Encoding=UTF-8\n";
  buffer+="Name="+ realname + "v" + version +"\n";
  buffer+="Comment=Start " + realname +"\n";
  buffer+="Exec=" + pathname +"/" + command + "\n";
  buffer+="Terminal=true\n";
  buffer+="Type=Application\n";
  buffer+="Icon=" + pathname + "/images/bioimagesuite.png\n";
  buffer+="StartupNotify=true\n";
  buffer+="Categories=Application;Yale BioImage Suite\n";
  buffer+="X-Desktop-File-Install-Version="+version+"\n";
  buffer+="MimeType=\n";

  std::ofstream fout(fname.c_str());
  if (fout.is_open())
    {
      fout << buffer;
      fout.close();
      fprintf(stderr,"Created %s\n",fname.c_str());
    }
  else
    {
      fprintf(stdout,"Failed to open %s\n",fname.c_str());
    }
  return 1;
}

int main(int argc,char *argv[])
{
  if (argc<=1)
    {
      fprintf(stdout,"Not Enough Arguments");
      usage(argv[0]);
    }

  std::string pathname=argv[1];
  std::string version=BIOIMAGESUITE_STRING_VERSION;
  
  if (argc>2)
    version=argv[2];

  int sz=sizeof(void*);

  if (sz==8)
    version+="_64-bit";

  creategnomefile("/usr/share/applications/bis_console_"+version+".desktop",pathname,"bis_console","BioImage Suite Console ",version);
  creategnomefile("/usr/share/applications/bis_menu_"+version+".desktop",pathname,"bis_console_menu","BioImage Suite Menu ",version);
  
  std::string fname=pathname+"/cleanmenuentries.sh";
  std::ofstream fout(fname.c_str());
  std::string buffer="";
  buffer+="rm /usr/share/applications/bis_console_"+version+".desktop\n";
  buffer+="rm /usr/share/applications/bis_menu_"+version+".desktop\n";
  fout << buffer;
  fout.close();
  fprintf(stderr,"Created cleanup file %s\n",fname.c_str());

  std::string cmd="chmod +x "+fname;
  system(cmd.c_str());

}
    

