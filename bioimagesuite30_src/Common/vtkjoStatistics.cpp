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
 * vtkjoStatistics.cpp
 *
 *  Created on: Nov 19, 2008
 *      Author: jonofrey
 */

#include <vtkjoStatistics.h>

#include "vtkDataArray.h"
#include "vtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"


vtkjoStatistics* vtkjoStatistics::New()
{
    // First try to create the object from the vtkObjectFactory
    vtkObject* ret = vtkObjectFactory::CreateInstance("vtkjoStatistics");
    if(ret)
      {
        return (vtkjoStatistics*) ret;
      }
    // If the factory was unable to create the object, then create it here.
    return new vtkjoStatistics;
}


double vtkjoStatistics::JaccardCoefficient(vtkImageData* imageA, vtkImageData* imageB)
{
    int dimA[3]; imageA->GetDimensions(dimA);
    int dimB[3]; imageB->GetDimensions(dimB);

    // TODO: Should we also compare spacing and origin?
    if ((dimA[0] != dimB[0]) &&
            (dimA[1] != dimB[1]) &&
            (dimA[2] != dimB[2]))
    {
        fprintf(stderr, "Images are not the same size.");
        return 0.0;
    }

    vtkDataArray* dataA = imageA->GetPointData()->GetScalars();
    vtkDataArray* dataB = imageB->GetPointData()->GetScalars();

    vtkIdType nPts = dataA->GetNumberOfTuples();
    double valueA = 0.0, valueB = 0.0;
    int intersectAB = 0, unionAB = 0;

    for (int i=0; i<nPts; i++) {
        valueA = dataA->GetComponent(i, 0);
        valueB = dataB->GetComponent(i, 0);

        // Find the intersecting voxels in the image sets
        if (valueA > 0.0 && valueB > 0.0)
            intersectAB++;
        // Find the unions of the image sets voxels
        if (valueA > 0.0 || valueB > 0.0)
            unionAB++;
    }

    return double(intersectAB)/double(unionAB);
}

