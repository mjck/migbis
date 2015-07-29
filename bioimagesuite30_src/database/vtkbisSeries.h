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

#ifndef SERIES_H
#define SERIES_H
#include <vtkstd/string>
#include "vtkbisBaseProcedure.h"


class vtkbisSeries : public vtkbisBaseProcedure
{
public:
  static vtkbisSeries *New();
  vtkTypeMacro(vtkbisSeries,vtkbisBaseProcedure);


  virtual void DebugPrint();
  virtual int   AddSeries();  
  virtual const char* SearchSeriesID(const char* StudyID,const char* InjectionID);
  virtual const char* SearchSeriesUID(const char* SeriesID);
  
  // Macros for all Members
  vtkSetStringMacro(SeriesID);
  vtkGetStringMacro(SeriesID);

  vtkSetStringMacro(StudyID);
  vtkGetStringMacro(StudyID);

  vtkSetStringMacro(InjectionID);
  vtkGetStringMacro(InjectionID);

  vtkSetStringMacro(SeriesUID);
  vtkGetStringMacro(SeriesUID);

  vtkSetStringMacro(SeriesFileName);
  vtkGetStringMacro(SeriesFileName);

  vtkSetStringMacro(SeriesDataPath);
  vtkGetStringMacro(SeriesDataPath);


protected:  
  vtkbisSeries();
  virtual ~vtkbisSeries();
  
  vtkbisSeries(const vtkbisSeries&) {};
  void operator=(const vtkbisSeries&) {};

  char* SeriesID;
  char* StudyID;
  char* InjectionID;
  char* SeriesUID;
  char* SeriesFileName;
  char* SeriesDataPath;

  virtual int InsertSeries();
  virtual int  FindEmptySeriesID();  
};

#endif 

