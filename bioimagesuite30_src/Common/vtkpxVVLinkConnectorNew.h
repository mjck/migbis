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

  Program:   vtkpxCommon library file
  Module:    $RCSfile: vtkpxVVLinkConnectorNew.h,v $
  Language:  C++
  Date:      $Date: 2007/01/08 $
  Version:   $Revision: #1 $


Copyright (c) 1995-2005 Xenios Papademetris
xenophon.papademetris@.yale.edu
All rights reserved.

=========================================================================*/
// .NAME vtkpxVVLinkConnectorNew - a collection of static utility functions
// .SECTION Description
// This class encapsulates the vvlink::VProxy Class and
// provides functionality for the integration of VectorVisionLink with BioImage Suite

#ifndef __vtkpxVVLinkConnectorNew_h
#define __vtkpxVVLinkConnectorNew_h

#include "vtkObject.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkTransform.h"
#include "vtkImageData.h"


#include <list>
#include <string>

class vtkMatrix4x4;
class vtkPolyData;
class vtkRenderWindow;
class vtkConeSource;
class vtkbisCylinderSource;
class vtkSphereSource;
class vtkpxCrossHairSource;

//BTX
namespace vvlink
{
  class VVLConnection;
  class VVLCoordinateSystem;
  class VVLQGetSliceSets;
  class VVLNTrackingData;
}
//ETX

class vtkpxVVLinkConnectorNew : public vtkObject
{
public:
  static vtkpxVVLinkConnectorNew *New();
  vtkTypeMacro(vtkpxVVLinkConnectorNew,vtkObject);

  // Description:
  // Print Self
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Initialize and Close Connection
  virtual int   InitializeVVLConnection(char* server,char* password,int channel);
  virtual int   CloseVVLConnection();
  virtual int   UpdateImageList();


  // Description:
  // Get Channel that this object wil send images to (using SendViewerToVVLChannel Below)
  vtkGetMacro(VVLinkChannel,int);

  // Description:
  // Number of Images on VV Station
  virtual int   GetNumberOfVVLinkImages();

  // Description:
  // Comma separated string of all VV Image Names
  virtual char* GetVVLinkImageNameList();
  virtual char* GetVVLinkPatientName();

  // Description:
  // Get Image, Matrix and Lookup Table
  virtual int   GetVVLinkImage(char* imagename,vtkImageData* img,vtkMatrix4x4* matrix,vtkLookupTable* lut);

  // Description:
  // Get and Sample/Flip Image Appropriately as well as Transformation and LookupTable
  // orientation: 0=Axial, 1=Coronal, 2=Sagittal
  // Axial    (FlipX and FlipZ)
  // Coronal  (FlipX and FlipY)
  // Sagittal (FlipY only)
  //
  // Sample --> Reduce In-plane resolution   i.e. Sample=2 takes 512x512 and outputs 256x256
  virtual int   GetVVLinkImage(char* imagename,vtkImageData* img,vtkTransform* xform,vtkLookupTable* lut,int orientation,
			       int flipleftright,
			       int flipanteriorposterior,
			       int flipsuperiorinferior,
			       int outputorientation,int sample=2);
  virtual int   GetVVLinkImage(char* imagename,vtkImageData* img,vtkTransform* xform,vtkLookupTable* lut,int orientation,
			       int sample=2);

  // Description:
  // Send RGB Image obtained from a vtkRenderWindow to VVLink
  virtual int   SendViewerToVVLChannel(vtkRenderWindow* renwin);

  // Description:
  // Labeled Points
  // Get Labeled Point position and other information (in case of Poly Data)
  // and map using xform (obtained using VVLinkGetImage i.e. Image->World)
  virtual int   GetVVLabeledPoints(vtkPoints* points);
  virtual int   GetVVLabeledPoints(vtkPoints* points,vtkTransform* xform);
  virtual int   GetVVLabeledPointsAsPolyData(vtkPolyData* surface,vtkTransform* xform);

  //virtual int   GetVVTrajectoryPoints(vtkPoints* points);

  // Description:
  // Save all information in a text file (filename), points are mapped to image space using xform
  virtual int   SaveVVLinkLabeledPoints(char* filename,vtkTransform* xform);


  // Description:
  // Get Pointer Position and Orientation
  // Point 1 = Tool Tip
  // Point 2 = Tool Base ( Base=Tip-length*Direction direction=pointer orientation)
  virtual int   GetVVLinkPointer(vtkPoints* newPoints,vtkTransform* xform);
  virtual int   GetVVLinkPointer(vtkPoints* newPoints,vtkTransform* xform,double length);

  // Description:
  // Get Pointer and Draw Cone to represent in in scene (in image space coordinates as specified using xform)
  // length= Cone length
  // radius= Cone base radius
  // source= vtkConeSource object to modify
  virtual int   GetAndDrawVVLinkPointer(vtkPoints* newPoints,vtkTransform* xform,
					double length,double radius,double extension,
					vtkConeSource* source, vtkSphereSource* tipsphere, vtkpxCrossHairSource* tipsphere2);

  // Description:
  // Get All Tracked Tools
  // Get Labeled Point position and other information and either story in PolyData on in filename
  virtual int   GetVVLinkTrackedToolsAsPolyData(vtkPolyData* tools,vtkTransform* xform);
  virtual int   SaveVVLinkTrackedTools(char* filename,vtkTransform* transform,char* comment);

  // Description:
  // Trajectory Points (start point, end point)
  // Get Trajectory Points position and other information
  virtual int GetVVTrajectoryPoints(vtkPoints* points, vtkTransform* xform);
  virtual int GetVVNumberOfTrajectories();
  virtual int GetVVNamesOfTrajectories(int numberOfTrajectories); 

protected:

  vtkpxVVLinkConnectorNew();
  ~vtkpxVVLinkConnectorNew();
  vtkpxVVLinkConnectorNew(const vtkpxVVLinkConnectorNew&) {};
  void operator=(const vtkpxVVLinkConnectorNew&) {};

  // Use BTX..ETX to avoid parsing for tcl

  //BTX
  static vvlink::VVLConnection*        VVLinkServer;
  int                                  VVLinkChannel;
  float                                VVLinkVersion;

	vvlink::VVLCoordinateSystem *        VVLCoordinateSystem;

  static char*                         VVLinkPatientName;

	static vvlink::VVLQGetSliceSets *    VVLSliceSets;
  static char*                         VVLinkImageListNames;
  static int                           VVLinkNumberOfImages;

	static vvlink::VVLNTrackingData *    VVLNTrackingData;

  static std::list<std::string>        VVLToolsAndOrder;
  //ETX

  // Image Reorientation Stuff
  // ------------------------
  // Description:
  // Procedures to Re-Orient an Image from Coronal/Axial/Saggital to C/A/S
  // BTX
  static int ReOrientMatrix(vtkImageData* source,int inputorient,int outputorient,vtkMatrix4x4* tmat,double sp_out[3],int dim_out[3]);
  static int ReOrientDimensions(double sp_in[3],int dim_in[3],int inputorient,int outputorient,double sp_out[3],int dim_out[3]);
  //ETX

};


#endif

