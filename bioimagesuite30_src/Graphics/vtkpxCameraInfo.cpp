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





/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxCameraInfo.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/28 03:51:32 $
  Version:   $Revision: 1.2 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkObjectFactory.h"
#include "vtkpxCameraInfo.h"
#include "pxutil.h"
//------------------------------------------------------------------------------
vtkpxCameraInfo* vtkpxCameraInfo::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxCameraInfo");
  if(ret)
    {
      return (vtkpxCameraInfo*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxCameraInfo;
}
vtkpxCameraInfo::vtkpxCameraInfo()
{
  this->CameraParameters=vtkDoubleArray::New();
  this->CameraParameters->SetNumberOfComponents(13);
  this->CameraParameters->Allocate(10,5);
  // Focal Point[3]
  params[0]=0.0; params[1]=0.0; params[2]=1.0;
  // Position[3]
  params[3]=0.0; params[4]=0.0; params[5]=1.0;
  // ViewUp[3]
  params[6]=0.0; params[7]=0.0; params[8]=1.0;
  // ClippingRange[2]
  params[9]=0.0; params[10]=100.0;
  // Parallel Projection
  params[11]=1.0;
  // Parallel Scale
  params[12]=1.0;

  // this->CameraParameters->InsertNextTuple(params);
}
//------------------------------------------------------------------------------
// Destruct a volume
vtkpxCameraInfo::~vtkpxCameraInfo()
{
  this->CameraParameters->Delete();
}
//------------------------------------------------------------------------------
int vtkpxCameraInfo::GetNumberOfCameras()
{
  return this->CameraParameters->GetNumberOfTuples();
}
//------------------------------------------------------------------------------
int vtkpxCameraInfo::UpdateRenderer(vtkRenderer* ren,int index)
{
  if (ren==NULL)
    return 0;
  return this->UpdateCamera(ren->GetActiveCamera());
}
//------------------------------------------------------------------------------
int vtkpxCameraInfo::UpdateCamera(vtkCamera* camera,int index)
{
  if (camera==NULL || this->GetNumberOfCameras()==0)
    return 0;
  
  index=Irange(index,0,this->GetNumberOfCameras()-1);

  this->CameraParameters->GetTuple(index,params);

  camera->SetFocalPoint(params[0],params[1],params[2]);
  camera->SetPosition(params[3],params[4],params[5]);
  camera->SetViewUp(params[6],params[7],params[8]);
  camera->ComputeViewPlaneNormal();
  camera->SetClippingRange(params[9],params[10]);
  camera->SetParallelProjection((int)params[11]);
  camera->SetParallelScale(params[12]);
  
  /*  fprintf(stderr,"\t\t Setting params index=%d = %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n\n",
	 params[0],params[1],params[2],
	 params[3],params[4],params[5],
	 params[6],params[7],params[8],
	 params[9],params[10],params[11],
	 params[12]);*/
  
  return 1;
}
//------------------------------------------------------------------------------
int vtkpxCameraInfo::CopyRenderer(vtkRenderer* ren,int index)
{
 if (ren==NULL)
   {
     fprintf(stderr,"Bad Renderer\n");
     return 0;
   }
  return this->CopyCamera(ren->GetActiveCamera());
}
//------------------------------------------------------------------------------
int vtkpxCameraInfo::CopyCamera(vtkCamera* camera,int index)
{
 if (camera==NULL)
   {
     fprintf(stderr,"Bad Camera\n");
     return 0;
   }

 if (index>=0)
   {
     index=Irange(index,0,this->GetNumberOfCameras()-1);
   }
 else
   {
     // Allocate a new one if (-1)
     this->CameraParameters->InsertNextTuple(params);
     index=this->GetNumberOfCameras()-1;
   }
    
 for (int ia=0;ia<=2;ia++)
   {
     params[0+ia]=camera->GetFocalPoint()[ia];
     params[3+ia]=camera->GetPosition()[ia];
     params[6+ia]=camera->GetViewUp()[ia];
     if (ia<2)
       params[9+ia]=camera->GetClippingRange()[ia];
   }
 params[11]=(double)camera->GetParallelProjection();
 params[12]=(double)camera->GetParallelScale();
 
 /* fprintf(stderr,"Setting tuple=%d / %d\n",index,this->GetNumberOfCameras());
 fprintf(stderr,"\t\t params = %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n",
	 params[0],params[1],params[2],
	 params[3],params[4],params[5],
	 params[6],params[7],params[8],
	 params[9],params[10],params[11],
	 params[12]);*/
 this->CameraParameters->SetTuple(index,params);  
 this->CameraParameters->GetTuple(index,params);
 /* fprintf(stderr,"\t\t Checking params = %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n\n",
	 params[0],params[1],params[2],
	 params[3],params[4],params[5],
	 params[6],params[7],params[8],
	 params[9],params[10],params[11],
	 params[12]);

	 this->Save("/tmp/camera.list");*/

 return 1;

}

//------------------------------------------------------------------------------
int vtkpxCameraInfo::Load(const char* fname)
{
  gzFile fin=gzsuffixopen(fname,"rb");
  if (fin)
    {
      int ok=this->Load(fin);
      gzclose(fin);
      return ok;
    }
  return 0;
}

//------------------------------------------------------------------------------
int vtkpxCameraInfo::Save(const char* fname)
{
  FILE* fout=fopen(fname,"w");
  if (fout)
    {
      int ok=this->Save(fout);
      fclose(fout);
      return ok;
    }
  return 0;
}
//------------------------------------------------------------------------------

int vtkpxCameraInfo::Load(gzFile fin)
{
  char line[400];
  gzgets(fin,line,100);
  if (gzfindstringinstring(line,"#vtkpxCameraInfo File")==0)
    return 0;
  
  gzgets(fin,line,100); 	
  gzgets(fin,line,100);
  int num=0;
  sscanf(line,"%d ",&num);
  this->CameraParameters->Delete();
  this->CameraParameters=vtkDoubleArray::New();
  this->CameraParameters->SetNumberOfComponents(13);
  this->CameraParameters->Allocate(num,5);
  for (int i=0;i<num;i++)
    {
      gzgets(fin,line,399);
      sscanf(line,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	     &params[0],&params[1],&params[2],
	     &params[3],&params[4],&params[5],
	     &params[6],&params[7],&params[8],
	     &params[9],&params[10],&params[11],
	     &params[12]);
      this->CameraParameters->InsertNextTuple(params);
    }
  return 1;
}
//------------------------------------------------------------------------------
int vtkpxCameraInfo::Save(FILE*  fout)
{
  fprintf(fout,"#vtkpxCameraInfo File\n");
  fprintf(fout,"#Number Of Camers\n%d\n",this->GetNumberOfCameras());
  int nc=this->CameraParameters->GetNumberOfComponents();
  for (int i=0;i<this->GetNumberOfCameras();i++)
    {
      this->CameraParameters->GetTuple(i,params);
      for (int j=0;j<nc;j++)
	fprintf(fout,"%.3f ",params[j]);
      fprintf(fout,"\n");
    }
  return 1;
}


