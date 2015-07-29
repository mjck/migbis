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
 * vtkbisRtoPvalue.cpp
 *
 *  Created on: May 18, 2009
 *      Author: Isabella Murphy,Xilin Shen
 *      take behavior correlation R value and number of study subjects as input,
 *      calculate 1-p value
 */

#include <vtkbisRtoPvalue.h>
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkImageCast.h"
#include "vtkbisRtoTmap.h"
#include "vtkpxComputeTmap.h"
#include "vtkbisImageUtil.h"
#include "math.h"
#include "pxisinf.h"
// This creates the "New" Function
vtkStandardNewMacro(vtkbisRtoPvalue);


vtkbisRtoPvalue::vtkbisRtoPvalue() {
	this->NumSubject=6;
	this->ScaleFactor=1.0;
	this->DOF=5;

}




int vtkbisRtoPvalue::RequestInformation(vtkInformation*,vtkInformationVector**,vtkInformationVector* outputVector)
{
  vtkInformation* outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
  return 1;
}

void vtkbisRtoPvalue::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
	cout<<"start vtkbisRtoPvalue"<<endl;

	vtkImageCast* cast=vtkImageCast::New();
	cast->SetInput(input);
	cast->SetOutputScalarTypeToFloat();
	cast->Update();

	vtkImageData* temp=vtkImageData::New();
	temp->ShallowCopy(cast->GetOutput());
	cast->Delete();

	vtkDataArray*   inp=temp->GetPointData()->GetScalars();
	int nt=inp->GetNumberOfTuples();

	vtkDataArray*   out=output->GetPointData()->GetScalars();
	// Cleanup first -- fill output with zeros
	out->FillComponent(0,0.0);

	vtkbisRtoTmap* r2t = vtkbisRtoTmap::New();
	r2t->SetNumSubject(this->GetNumSubject());

	int maxcount=int(nt/10);
	int count=0;
	float pog=0.0;

	this->UpdateProgress(0.01);

	for (int t=0;t<nt;t++)
	{
		out->SetComponent(t,0,r2t->RtoTmap(inp->GetComponent(t,0)));

		++count;
		if (count==maxcount)
		{
			pog=float(t)/float(nt);
			this->UpdateProgress(pog);
			count=0;
			//fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(numdone)/float(nt));
		}
	}
	//calculate t to p
	vtkImageData* pmap=vtkImageData::New();
	vtkpxComputeTmap* t2p =vtkpxComputeTmap::New();
	pmap=t2p->TmapToPvalueConversion(output,this->DOF,this->ScaleFactor);

	//calculate 1-p
	vtkImageData* oneminuspvalue=vtkImageData::New();
	oneminuspvalue=vtkbisImageUtil::OneminusPvalue(pmap);
	vtkDataArray* onempval=oneminuspvalue->GetPointData()->GetScalars();

	//check if r < 0
	for (int k=0;k<nt;k++)
	{
		out->SetComponent(k,0,onempval->GetComponent(k,0));
		if(inp->GetComponent(k,0)<0)
		{
			if(onempval->GetComponent(k,0)>0)
			{
				out->SetComponent(k,0,-1*onempval->GetComponent(k,0));
			}
		}
	}

	this->UpdateProgress(1.0);
	temp->Delete();

}

