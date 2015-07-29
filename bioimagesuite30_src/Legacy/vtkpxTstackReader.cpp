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
  Module:    $RCSfile: vtkpxSurfaceUtil.h,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
  Version:   $Revision: 1.1 $


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
#include "vtkpxTstackReader.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "pxutil.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
//------------------------------------------------------------------------------
vtkpxTstackReader* vtkpxTstackReader::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxTstackReader");
  if(ret)
    {
    return (vtkpxTstackReader*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxTstackReader;
}


vtkpxTstackReader::vtkpxTstackReader()
{
  this->Filename=NULL;
}

vtkpxTstackReader::~vtkpxTstackReader()
{
  if (this->Filename)
      delete this->Filename;
}


// Specify file name of vtk polygonal data file to read.
void vtkpxTstackReader::SetFileName(const char* name) 
{
  if (this->Filename)
      delete this->Filename;

  this->Filename=new char[strlen(name)+1];
  strcpy(this->Filename,name);
}
char *vtkpxTstackReader::GetFileName() 
{
  return this->Filename;
}

void vtkpxTstackReader::Execute()
{
  vtkPolyData *output = this->GetOutput();
  vtkDebugMacro(<<"Reading tstack data...");


  gzFile fin=gzsuffixopen(this->Filename,"r");
  if (!fin)
    {
      vtkErrorMacro(<<"Failed to read tstack from "<<this->Filename);
      return;
    }

  vtkDebugMacro(<<"Reading "<<this->Filename);

  char line[400];
  gzgets(fin,line,200);
  if (gzfindstringinstring(line, "#TriangulatedStack")==0)
      {
	gzclose(fin);
	vtkErrorMacro(<<"Bad Header in "<<this->Filename);
	return;
      }


  int npoints,ntriangles,connect,hasnormals,hascurvatures,ncntr;
  gzgets(fin,line,200); sscanf(line,"#%d\n",&ncntr);
  gzgets(fin,line,200); sscanf(line,"#%d\n",&npoints);
  gzgets(fin,line,200); sscanf(line,"#%d\n",&ntriangles);
  gzgets(fin,line,200); sscanf(line,"#%d\n",&connect);
  gzgets(fin,line,200); sscanf(line,"#%d\n",&hasnormals);
  gzgets(fin,line,200); sscanf(line,"#%d\n",&hascurvatures);

  //init(npoints,ntriangles);

  vtkDebugMacro(<<"Points = "<<npoints<<", Triangles = "<<ntriangles);
  vtkDebugMacro(<<"Hasnormals="<<hasnormals<<", hascurvatures="<<hascurvatures);
  if (npoints<1 || ntriangles<1)
      {
	vtkErrorMacro("Insufficient Data:: Points = "<<npoints<<", Triangles = "<<ntriangles);
	gzclose(fin);
	return;
      }

  if (ncntr>0)
      {
	gzgets(fin,line,200); 
	for (int i=0;i<ncntr;i++)
	    gzgets(fin,line,200);
      }

  
  vtkPoints *points = vtkPoints::New();
  points->SetNumberOfPoints(npoints);


  vtkFloatArray* normals=NULL;
  vtkFloatArray* curvatures=NULL;

  if (hasnormals)
    {
      normals=vtkFloatArray::New();
      normals->SetNumberOfComponents(3);
      normals->SetNumberOfTuples(npoints);
    }
  if (hascurvatures)
    {
      curvatures=vtkFloatArray::New();
      curvatures->SetNumberOfComponents(2);
      curvatures->SetNumberOfTuples(npoints);
    }

  int i;
  gzgets(fin,line,200); 
  for (i=0;i<npoints;i++)
      {
	gzgets(fin,line,200);
	int index;
	float x[3],nx[3],k[2];
	sscanf(line,"%d %f %f %f %f %f %f %f %f\n",
	       &index,&x[0],&x[1],&x[2],
	       &nx[0],&nx[1],&nx[2],&k[0],&k[1]);
	points->SetPoint(i,x);
	if (hasnormals)
	  normals->SetTuple(i,nx);
	if (hascurvatures)
	  curvatures->SetTuple(i,k);
      }
  
  vtkCellArray *polys = vtkCellArray::New();
  gzgets(fin,line,200);
  for (i=0;i<ntriangles;i++)
      {
	gzgets(fin,line,200);
	int index,p[3];
	sscanf(line,"%d %d %d %d\n",&index,&p[0],&p[1],&p[2]);
	vtkIdType pp[3];
	for (int ia=0;ia<=2;ia++)
	  pp[ia]=p[ia];
	polys->InsertNextCell(3,pp);
      }
  
  if (output==NULL)
      {
	vtkErrorMacro("Null Output\n");
	return;
      }

  vtkDebugMacro ("Setting Points\n");
  
  output->SetPoints(points);
  points->Delete();
  output->SetPolys(polys);
  polys->Delete();

  if (hasnormals)
    {
      output->GetPointData()->SetNormals(normals);
      normals->Delete();
    }

  if (hascurvatures)
    {
      output->GetPointData()->SetScalars(curvatures);
      curvatures->Delete();
    }



}


void vtkpxTstackReader::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkPolyDataSource::PrintSelf(os,indent);
}

