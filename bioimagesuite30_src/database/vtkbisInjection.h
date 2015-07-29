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

#ifndef INJECTION_HPP
#define INJECTION_HPP

#include "vtkbisBaseProcedure.h"
#include "vtkObject.h"

class vtkbisInjection : public vtkbisBaseProcedure
{
public:
  static vtkbisInjection *New();
  vtkTypeMacro(vtkbisInjection,vtkbisBaseProcedure);

  virtual void DebugPrint();

  virtual int   AddInjection();  
  const char*  SearchInjectionID(const char* VisitID);
  const char*  SearchInjectionDateTime(const char* InjectionID);

  vtkSetStringMacro(InjectionID);
  vtkGetStringMacro(InjectionID);

  vtkSetStringMacro(VisitID);
  vtkGetStringMacro(VisitID);

  vtkSetStringMacro(InjectionDateTime);
  vtkGetStringMacro(InjectionDateTime);

protected:  

  vtkbisInjection();
  virtual ~vtkbisInjection();

  vtkbisInjection(const vtkbisInjection&) {};
  void operator=(const vtkbisInjection&) {};

  char* InjectionID;
  char* VisitID;
  char* InjectionDateTime;

  virtual int  InsertInjection();
  virtual int  FindEmptyInjectionID();  
};

#endif 

