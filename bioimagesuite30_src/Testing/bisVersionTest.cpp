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

#include "bioimagesuite_settings.h"
#include <itkVersion.h>
#include <vtkVersion.h>
#include <gdcmVersion.h>
#include <tcl.h>
#include <tk.h>


int main(int argc, char* argv[])
{
  // Tcl
  //
  std::cout << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "Tcl Version    = "
            << TK_PATCH_LEVEL << std::endl;
  std::cout << "Major Version  = "
            << TK_MAJOR_VERSION << std::endl;
  std::cout << "Minor Version  = "
            << TK_MINOR_VERSION << std::endl;
  std::cout << "Patch Version  = "
            << TK_RELEASE_SERIAL << std::endl;


  // Tk
  //
  std::cout << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "Tk Version     = "
            << TCL_PATCH_LEVEL << std::endl;
  std::cout << "Major Version  = "
            << TCL_MAJOR_VERSION << std::endl;
  std::cout << "Minor Version  = "
            << TCL_MINOR_VERSION << std::endl;
  std::cout << "Patch Version  = "
            << TCL_RELEASE_SERIAL << std::endl;


  // GDCM
  //
  std::cout << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "GDCM Version   = "
            << gdcm::Version::GetVersion() << std::endl;
  std::cout << "Major Version  = "
            << gdcm::Version::GetMajorVersion()  << std::endl;
  std::cout << "Minor Version  = "
            << gdcm::Version::GetMinorVersion()  << std::endl;
  std::cout << "Patch Version  = "
            << gdcm::Version::GetBuildVersion()  << std::endl;


  // ITK
  // 
  std::cout << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "ITK Version    = " 
            << itk::Version::GetITKVersion()       << std::endl;
  std::cout << "Major Version  = " 
            << itk::Version::GetITKMajorVersion()  << std::endl;
  std::cout << "Minor Version  = " 
            << itk::Version::GetITKMinorVersion()  << std::endl;
  std::cout << "Patch Version  = " 
            << itk::Version::GetITKBuildVersion()  << std::endl;
  std::cout << "Source Version = " 
            << itk::Version::GetITKSourceVersion() << std::endl;


  // VTK
  // 
  std::cout << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "VTK Version    = " 
            << vtkVersion::GetVTKVersion()       << std::endl;
  std::cout << "Major Version  = " 
            << vtkVersion::GetVTKMajorVersion()  << std::endl;
  std::cout << "Minor Version  = " 
            << vtkVersion::GetVTKMinorVersion()  << std::endl;
  std::cout << "Patch Version  = " 
            << vtkVersion::GetVTKBuildVersion()  << std::endl;
  std::cout << "Source Version = " 
            << vtkVersion::GetVTKSourceVersion() << std::endl;
  std::cout << std::endl;

  std::cout << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;
#ifdef BIOIMAGESUITE3_USE_CUDA
  std::cout << "CUDA Version    = " << BIOIMAGESUITE3_CUDA_VERSION << std::endl;
#else
  std::cout << "CUDA Version    = NONE " << std::endl; 
#endif


  // BIOIMAGESUITE
  std::cout << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "BioImage Suite Package Version    = " << BIOIMAGESUITE_PACK_VERSION  << std::endl;

  return 0;
}

