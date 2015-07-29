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
 * vtkjoColoredSphereSource.h
 *
 *  Created on: Oct 28, 2008
 *      Author: jonofrey
 */
// .NAME vtkjoColoredSphereSource - Create an image of a sphere with
// colored faces.
// .SECTION Description
// vtkjoColoredSphereSource produces an image of a sphere with individually
// integer colored faces.  These colored faces extend to the sphere's center creating
// colored triangular wedges.  The sphere is tessellated as an icosahedron,
// with the SubdivisionLevels variable controlling the number of times the
// faces of the icosahedron are subdivided into smaller triangles.

#ifndef VTKJOCOLOREDSPHERESOURCE_H_
#define VTKJOCOLOREDSPHERESOURCE_H_

#include "vtkImageAlgorithm.h"

class vtkjoColoredSphereSource : public vtkImageAlgorithm
{
public:
    static vtkjoColoredSphereSource *New();
    vtkTypeMacro(vtkjoColoredSphereSource, vtkImageAlgorithm);

    void PrintSelf(ostream& os, vtkIndent indent);

    vtkSetMacro(UseNewWedges, int);
    vtkGetMacro(UseNewWedges, int);

    // Description:
    // Set the color value within each wedge volume.  Note, this value
    // will only be used if the UseSingleFrame parameter is turned off.
    // Default is 100.
    vtkSetMacro(ColorValue, short);
    vtkGetMacro(ColorValue, short);

    // Description:
    // Set radius of the sphere.  Default is 32.0.
    void SetRadius(double r);
    vtkGetMacro(Radius, double);

    // Description:
    // Set the number of subdivisions to perform on the icosahedron when creating
    // the sphere.  0 subdivions will return the initial icosahedron.
    vtkSetClampMacro(SubdivisionLevels, int, 0, 1);
    vtkGetMacro(SubdivisionLevels, int);

    // Description:
    // Set/Get the center of the sphere.
    vtkSetVector3Macro(Center, double);
    vtkGetVector3Macro(Center, double);

    // Description:
    // If on, create the output using a single vtkImageData component.  This coloring
    // results in wedges being colored 1,2,3,...,N, where N is the number of wedges
    // (or faces), in a single image structure.  Note, wegde volumes may not overlap
    // so the volumes may vary with using this scheme.  Alternatively, if off, create the
    // output using N components, where N is the number of wedges (or faces).  This
    // way, each wedge is its own component and wedge volumes are more uniform.  The
    // wedge color is then the same value for each wedge (see [Get/Set]ColorValue).
    // Default is Off.
    vtkSetClampMacro(UseSingleFrame, int, 0, 1);
    vtkGetMacro(UseSingleFrame, int);
    vtkBooleanMacro(UseSingleFrame, int);

protected:
    vtkjoColoredSphereSource();
    ~vtkjoColoredSphereSource() {};

    int UseNewWedges;
    short ColorValue;
    double Radius;
    int SubdivisionLevels;
    int WholeExtent[6];
    double Center[3];
    int UseSingleFrame;

    virtual int RequestInformation(vtkInformation*, vtkInformationVector**, vtkInformationVector*);
    virtual int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*);

};

#endif /* VTKJOCOLOREDSPHERE_H_ */

