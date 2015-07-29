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


#include "vtkpxTalairachTransformHelper.h"
#include "vtkObjectFactory.h"
#include "vtkpxBaseCurve.h"
#include "vtkImageData.h"
#include "vtkpxAnalyzeImageSource.h"
//----------------------------------------------------------------------------
vtkImageData* vtkpxTalairachTransformHelper::ColinLookup=NULL;
char* vtkpxTalairachTransformHelper::ColinLookupFileName=NULL;
char* vtkpxTalairachTransformHelper::ColinLookupFileName2=NULL;

//----------------------------------------------------------------------------
vtkpxTalairachTransformHelper* vtkpxTalairachTransformHelper::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxTalairachTransformHelper");
  if(ret)
    {
      return (vtkpxTalairachTransformHelper*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxTalairachTransformHelper;
}

//------------------------------------------------------------------------
int vtkpxTalairachTransformHelper::SetColinLookupFileName(char* fname,int mode)
{
  if (fname==NULL)
    {
      fprintf(stderr,"No FileName Specified\n");
      return 0;
    }

  // fprintf(stderr,"In Set Lookup File Name %s %d\n",fname,mode);

  if (mode<1)
    mode=0;
  else
    mode=1;

  if (mode==0)
    {
      if (vtkpxTalairachTransformHelper::ColinLookupFileName!=NULL)
	{
	  if (strcmp(vtkpxTalairachTransformHelper::ColinLookupFileName,fname)==0)
	    {
	      // fprintf(stderr,"Same Name =%s\n",fname);
	      return 1;
	    }
	delete [] vtkpxTalairachTransformHelper::ColinLookupFileName;
	}
      vtkpxTalairachTransformHelper::ColinLookupFileName=new char[strlen(fname)+1];
      strcpy(vtkpxTalairachTransformHelper::ColinLookupFileName,fname);
      // fprintf(stderr,"Lookup Table Filename =%s\n",vtkpxTalairachTransformHelper::ColinLookupFileName);
    }
  else
    {
      if (vtkpxTalairachTransformHelper::ColinLookupFileName2!=NULL)
	{
	  if (strcmp(vtkpxTalairachTransformHelper::ColinLookupFileName2,fname)==0)
	    {
	      // fprintf(stderr,"Same Name =%s\n",fname);
	      return 1;
	    }
	  delete [] vtkpxTalairachTransformHelper::ColinLookupFileName2;
	}
      vtkpxTalairachTransformHelper::ColinLookupFileName2=new char[strlen(fname)+1];
      strcpy(vtkpxTalairachTransformHelper::ColinLookupFileName2,fname);
      // fprintf(stderr,"Lookup Table Filename 2=%s\n",vtkpxTalairachTransformHelper::ColinLookupFileName2);
    }


  return 1;
}

int vtkpxTalairachTransformHelper::LoadColinLookup(int numberofvoxels)
{
  //  fprintf(stderr,"Number of Voxels=%d\n",numberofvoxels);
  int mode=-1;

  if (numberofvoxels==902629)
    numberofvoxels=7109137;

  switch(numberofvoxels)
    {
    case 7109137:
      if (vtkpxTalairachTransformHelper::ColinLookupFileName2!=NULL)
	{
	  mode=1;
	  // fprintf(stderr,"Setting Filename to %s (numberofvoxels=%d)\n",ColinLookupFileName2,numberofvoxels);
	}
      break;
    case 5903040:
      if (vtkpxTalairachTransformHelper::ColinLookupFileName!=NULL)
	{
	  mode=0;
	  // fprintf(stderr,"Setting Filename to %s (numberofvoxels=%d)",ColinLookupFileName,numberofvoxels);
	}
      break;
    }

  if (mode==-1)
    {
      // fprintf(stderr,"Cannot Load Colin Lookup Table (No FileName)\n");
      return 0;
    }

  if(vtkpxTalairachTransformHelper::ColinLookup!=NULL)
    {
      int np=vtkpxTalairachTransformHelper::ColinLookup->GetNumberOfPoints();
      if (np==numberofvoxels)
	{
	  //  fprintf(stderr,"Proper Colin Lookup Table %d already loaded\n",np);
	  return 1;
	}
    }


  //  fprintf(stderr,"TH::Loading Colin Lookup Table mode=%d\n",mode);
  vtkpxAnalyzeImageSource* ana=vtkpxAnalyzeImageSource::New();
  if (mode==0)
    ana->Load(vtkpxTalairachTransformHelper::ColinLookupFileName);
  else
    ana->Load(vtkpxTalairachTransformHelper::ColinLookupFileName2);
  int vox=ana->GetOutput()->GetNumberOfPoints();
  int ok=0;
  if (vox==5903040 || vox==7109137)
    {
      ok=1;
      if(vtkpxTalairachTransformHelper::ColinLookup==NULL)
	vtkpxTalairachTransformHelper::ColinLookup=vtkImageData::New();
      vtkpxTalairachTransformHelper::ColinLookup->ShallowCopy(ana->GetOutput());
    }
  ana->Delete();
  //fprintf(stderr,"Done Loading Colin Lookup Table %d\n",ok);
  return ok;
}



