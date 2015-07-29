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





#ifndef __itkLevelSetCallback_h_
#define __itkLevelSetCallback_h_
#include "itkCommand.h"
#include "vtkpxAnalyzeImageWriter.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "vtkITKUtility.h"
#include "itkImage.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"

template<class TFilter,class RFilter,class ImageType>
class CommandIterationUpdate : public itk::Command
{
public:

  // Some ITK Definitions
  typedef CommandIterationUpdate   Self;
  typedef itk::Command             Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  itkNewMacro( Self );

  RFilter* actual_filter;

  virtual int SetSaveOptions(char* fname,int orientation,int frequency)
  {
    strncpy(this->SavePrefix,fname,199);
    this->SaveOrientation=orientation;
    this->SaveFrequency=frequency;
    this->DoSave=1;
	return 1;
  };

protected:
  CommandIterationUpdate() 
  {
    this->DoSave=0;
    this->SaveOrientation=0;
    this->SaveFrequency=50;
    strcpy(this->SavePrefix,"iterations");
  };
  int    DoSave;
  char  SavePrefix[200];
  int   SaveOrientation;
  int   SaveFrequency;
  

public:

  void Execute(itk::Object *caller, const itk::EventObject & event)
    { 
      Execute( (const itk::Object *) caller, event); 
    }

  void Execute(const itk::Object * object, const itk::EventObject & event)
    {
      const TFilter * filter = dynamic_cast< const TFilter * >( object );
      if( typeid( event ) != typeid( itk::IterationEvent ) )
        { 
	  return; 
	}
      
      
      std::cerr << filter->GetElapsedIterations() << ": ";
      std::cerr << filter->GetRMSChange() << std::endl;
      int iterations=filter->GetElapsedIterations();

      if (this->DoSave==1 && this->SaveFrequency*int(iterations/this->SaveFrequency)==iterations &&
	  actual_filter->GetOutput()!=NULL)
	{
	  typename itk::VTKImageExport<ImageType>::Pointer itkExporter = itk::VTKImageExport<ImageType>::New();

	  /*typename ImageType::SizeType size=actual_filter->GetOutput()->GetLargestPossibleRegion().GetSize();
	    std::cerr << "actual_filter_output_size " << size[0] << "," << size[1] << std::endl;*/
	  itkExporter->SetInput(actual_filter->GetOutput());
	  itkExporter->Update();

	  vtkImageImport* vtkImporter=vtkImageImport::New();
	  ConnectPipelines(itkExporter, vtkImporter);
	  vtkImporter->Update();

	  if (vtkImporter->GetOutput()!=NULL)
	    {
	      char line[220];
	      sprintf(line,"%s_%03d",this->SavePrefix,iterations);
	      vtkpxAnalyzeImageWriter* anaw=vtkpxAnalyzeImageWriter::New();
	      anaw->SetInput(vtkImporter->GetOutput());
	      anaw->SetOrientation(this->SaveOrientation);
	      anaw->Save(line);
	      
	      std::cerr << " ***** Saving in " << line  << std::endl;
	      anaw->Delete();
	    }
	  vtkImporter->Delete();
	}
    }

};

#endif

