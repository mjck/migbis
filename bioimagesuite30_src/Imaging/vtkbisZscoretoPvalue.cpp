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

/*
 * vtkbisZscoretoPvalue.cpp
 *
 *  Created on: Apr 16, 2009
 *      Author: Isabella Murphy
 */

#include <vtkbisZscoretoPvalue.h>
#include "vtkObjectFactory.h"
#include <vtkpxMath.h>
#include <vtkImageCast.h>
#include <vtkImageData.h>
#include <vtkDataArray.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkPointData.h>

using namespace std;

// This creates the "New" Function
vtkStandardNewMacro(vtkbisZscoretoPvalue);


//----------------------------------------------------------------------------
int vtkbisZscoretoPvalue::RequestInformation(vtkInformation*,
                                           vtkInformationVector**,
                                           vtkInformationVector* outputVector)
{
  vtkInformation* outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
  return 1;
}
void vtkbisZscoretoPvalue::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
	cout<<"start vtkbisZscoretoPvalue"<<endl;

	vtkImageCast* cast=vtkImageCast::New();
	cast->SetInput(input);
	cast->SetOutputScalarTypeToFloat();
	cast->Update();

	vtkImageData* temp=vtkImageData::New();
	temp->ShallowCopy(cast->GetOutput());
	cast->Delete();

	vtkDataArray*   inp=temp->GetPointData()->GetScalars();
	int nt=inp->GetNumberOfTuples();
	int nc=inp->GetNumberOfComponents();

	vtkDataArray*   out=output->GetPointData()->GetScalars();
	// Cleanup first -- fill output with zeros
	for (int ia=0;ia<nc;ia++)
	  out->FillComponent(ia,0);

	vtkpxMath* math=vtkpxMath::New();
	int maxcount=int(nt/10);
	int count=0;
	double pog=0.0;

	this->UpdateProgress(0.01);

	for (int t=0;t<nt;t++)
	{
		for (int c=0;c<nc;c++)
		{
			out->SetComponent(t,c,math->ZscoreToPvalue(inp->GetComponent(t,c)));

		}
		++count;
		if (count==maxcount)
		{
			pog=double(t)/double(nt);
		    this->UpdateProgress(pog);
		    count=0;
		    //fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,double(numdone)/double(nt));
		}
	}
	this->UpdateProgress(1.0);

	math->Delete();
	temp->Delete();


}

