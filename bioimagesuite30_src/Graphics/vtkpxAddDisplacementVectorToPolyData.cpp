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
  Module:    $RCSfile: vtkpxAddDisplacementVectorToPolyData.cpp,v $
  Language:  C++
  Date:      $Date: 2002/07/15 14:14:00 $
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

#include "vtkpxAddDisplacementVectorToPolyData.h"
#include "pxutil.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataNormals.h"
#include "vtkFloatArray.h"
#include "vtkPoints.h"
#include "vtkCleanPolyData.h"
#include "vtkPointLocator.h"
//--------------------------------------------------------------------------
vtkpxAddDisplacementVectorToPolyData* vtkpxAddDisplacementVectorToPolyData::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxAddDisplacementVectorToPolyData");
  if(ret)
    {
    return (vtkpxAddDisplacementVectorToPolyData*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxAddDisplacementVectorToPolyData;
}

vtkpxAddDisplacementVectorToPolyData::vtkpxAddDisplacementVectorToPolyData()
{
  this->SamplingDistance = 0.5;
  this->Scale=1.0;
  this->Displacements=NULL;
}

void vtkpxAddDisplacementVectorToPolyData::SetDisplacements(vtkFloatArray* disp)
{
  this->Displacements=disp;
}

void vtkpxAddDisplacementVectorToPolyData::Execute()
{
  vtkPolyData *source = this->GetInput();
  vtkPolyData *destination= this->GetOutput();

  fprintf(stderr,"Here\n");
  if (this->Displacements==NULL || source==NULL || destination==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs\n");
      return;
    }
  
  if (this->Displacements->GetNumberOfTuples()!=source->GetNumberOfPoints())
    {
      vtkErrorMacro(<<"Not Enough Displacemnts\n");
      return;
    }

  
  vtkDebugMacro(<<"Beginning\n "<<source->GetNumberOfPoints()<<","<<this->Displacements->GetNumberOfTuples()<<"\n");

  vtkCleanPolyData* clean=vtkCleanPolyData::New();
  clean->SetToleranceIsAbsolute(1);
  clean->SetAbsoluteTolerance(this->SamplingDistance);
  clean->SetInput(source);
  clean->Update();

  int np=clean->GetOutput()->GetNumberOfPoints();
  vtkPoints* oldpoints=clean->GetOutput()->GetPoints();

  vtkPoints* points=vtkPoints::New();
  points->SetNumberOfPoints(np*2);

  vtkPointLocator* locator=vtkPointLocator::New();
  locator->SetDataSet(clean->GetOutput());
  locator->BuildLocator();

  vtkCellArray* lines=vtkCellArray::New();
  lines->Allocate(np,10);

  vtkIdType pt[2];
  double x[3],dx[3];
  for (int i=0;i<np;i++)
    {
      oldpoints->GetPoint(i,x);
      points->SetPoint(i,x);

      int id=locator->FindClosestPoint(x);
      this->Displacements->GetTuple(id,dx);
      
      for (int ia=0;ia<=2;ia++)
	dx[ia]=x[ia]+this->Scale*dx[ia];
      points->SetPoint(i+np,dx);
      pt[0]=i;
      pt[1]=pt[0]+np;
      lines->InsertNextCell(2,pt);
    }

  destination->SetPoints(points);
  destination->SetPolys(clean->GetOutput()->GetPolys());
  destination->SetLines(lines);

  points->Delete();
  lines->Delete();
  clean->Delete();
  locator->Delete();
}



