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

// VVLink specific includes
#include <VVLConnection.h>
#include <VVLQGetPatientName.h>
#include <VVLQGetPoints.h>
#include <VVLQGetTrajectories.h>
#include <VVLQGetSliceSets.h>
#include <VVLQSetView.h>
#include <VVLNTrackingData.h>
#include <common/VVLCoordinateSystem.h>
#include <common/VVLTrajectory.h>

#include "vtkImageData.h"
#include "vtkMatrixToLinearTransform.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkRenderWindow.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkpxVVLinkConnectorNew.h"
#include <vtkCharArray.h>
#include <vtkConeSource.h>
#include <vtkbisCylinderSource.h>
#include <vtkSphereSource.h>
#include <vtkpxCrossHairSource.h>
#include <vtkCamera.h>
#include <vtkDoubleArray.h>
#include <vtkbisImageReslice.h>
#include <vtkMath.h>
#include <vtkTransform.h>
#include <vtkUnsignedCharArray.h>
#include <vtkWindowToImageFilter.h>
#include <vtkLookupTable.h>
#include <vtkCellArray.h>

// std includes
#include <list>
#include <sstream>
#include <string>


// small helper class
//BTX
template<typename T>
class vtkObjectContainer
{
public:
	vtkObjectContainer( T * pointer )
   :_pointer(pointer)
	{}
	~vtkObjectContainer(){ _pointer->Delete(); }

	T * operator->(){ return _pointer; }
	operator T* () { return _pointer; }

private:
	T * _pointer;

	// not implemented, forbidden
	vtkObjectContainer(const vtkObjectContainer &);
	const vtkObjectContainer &operator=(const vtkObjectContainer &);
};
//ETX

// -------------------------------------------------------------------
//       Initialize Static Members to NULL
//
// All instances of VVLinkConnectorNew share a single connection to
// VectorVisionCranial using the static
//
// vtkpxVVLinkConnectorNew::VVLinkServer  proxy object
//
// The channel number for each connection is incremented automatically
// -------------------------------------------------------------------
vvlink::VVLConnection*         vtkpxVVLinkConnectorNew::VVLinkServer         = NULL;
char *                         vtkpxVVLinkConnectorNew::VVLinkPatientName    = NULL;
vvlink::VVLQGetSliceSets *     vtkpxVVLinkConnectorNew::VVLSliceSets         = NULL;
char*                          vtkpxVVLinkConnectorNew::VVLinkImageListNames = NULL;
int                            vtkpxVVLinkConnectorNew::VVLinkNumberOfImages = 0;
vvlink::VVLNTrackingData *     vtkpxVVLinkConnectorNew::VVLNTrackingData     = NULL;
std::list<std::string>         vtkpxVVLinkConnectorNew::VVLToolsAndOrder     = std::list<std::string>();

vtkpxVVLinkConnectorNew* vtkpxVVLinkConnectorNew::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxVVLinkConnectorNew");
  if(ret)
  {
    return (vtkpxVVLinkConnectorNew*)ret;
  }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxVVLinkConnectorNew;
}


// Construct object to extract all of the input data.
vtkpxVVLinkConnectorNew::vtkpxVVLinkConnectorNew()
{
  // The following lines determine the instruments that will be considered in GetVVLinkPointer
  // and determine its priority
  if ( !this->VVLToolsAndOrder.size() )
  {

    // TODO: sort
    this->VVLToolsAndOrder.push_back("Softouch");
    this->VVLToolsAndOrder.push_back("Pointer");
    this->VVLToolsAndOrder.push_back("InstrumentXL");
    this->VVLToolsAndOrder.push_back("InstrumentL");
    this->VVLToolsAndOrder.push_back("InstrumentML");
    this->VVLToolsAndOrder.push_back("InstrumentM");
    this->VVLToolsAndOrder.push_back("InstrumentS");
    this->VVLToolsAndOrder.push_back("BiopsyInstrument");
    this->VVLToolsAndOrder.push_back("BiopsyGuidingTubeInstrument");
    this->VVLToolsAndOrder.push_back("Microscope");
    this->VVLToolsAndOrder.push_back("Ultrasound");
  }

  this->VVLinkChannel=-1;
  this->VVLinkVersion=7.5;
  this->VVLCoordinateSystem = vvlink::VVLCoordinateSystem::New( vvlink::VVLCoordinateSystem::PATIENT );
}

vtkpxVVLinkConnectorNew::~vtkpxVVLinkConnectorNew()
{
  this->VVLCoordinateSystem->Delete();
}
// ---------------------------------------------------------------------------

void vtkpxVVLinkConnectorNew::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkObject::PrintSelf(os,indent);

  os << indent << "  VVLinkChannel " << this->VVLinkChannel << "\n";
  os << indent << "  VVLinkServer " << this->VVLinkServer << "\n";
  os << indent << "  VVLinkPatientName " << this->VVLinkPatientName << "\n";
  os << indent << "  VVLinkImageListNames " << this->VVLinkImageListNames << "\n";
  os << indent << "  VVLinkNumberOfImages " << this->VVLinkNumberOfImages << "\n";
}

// -------------------------------------------------------------------------------
//           Vector Vision Related Code
// -------------------------------------------------------------------------------
int vtkpxVVLinkConnectorNew::InitializeVVLConnection(char* server,char* password,int channel)
{

  if (vtkpxVVLinkConnectorNew::VVLinkServer!=NULL)
  {
    //      cerr << "Alrady Connected \n";
    this->VVLinkChannel=channel;
    return 2;
  }

  //  cerr << "Creating VVLink Server \n";
  vtkpxVVLinkConnectorNew::VVLinkServer= vvlink::VVLConnection::New();
  //  cout << "Connecting to \"" << server <<  "\" ... ";

  try {
    int i;
    std::string sClientName;
    std::stringstream ss;
    ss << "Client Nr. " << channel;
    sClientName = ss.str();


    vtkpxVVLinkConnectorNew::VVLinkServer->ConnectTo( server, sClientName, password );
    this->VVLinkChannel=channel;
    //cout << "!!!done channel = " << this->VVLinkChannel << " !!!" << endl << endl;

    // getting the patient data
    vvlink::VVLQGetPatientName *cmdPatient = vvlink::VVLQGetPatientName::New();
    cmdPatient->Execute( vtkpxVVLinkConnectorNew::VVLinkServer );
    const std::string &pat_name = cmdPatient->GetName();
    const std::string &pat_id   = cmdPatient->GetID();

    if (vtkpxVVLinkConnectorNew::VVLinkPatientName!=NULL)
      delete [] vtkpxVVLinkConnectorNew::VVLinkPatientName;

    vtkpxVVLinkConnectorNew::VVLinkPatientName=new char[pat_name.length()+pat_id.length()+10];
    sprintf(vtkpxVVLinkConnectorNew::VVLinkPatientName,"%s.%s",pat_name.c_str(),pat_id.c_str());


    // getting all slice sets
    if ( vtkpxVVLinkConnectorNew::VVLSliceSets != NULL )
      vtkpxVVLinkConnectorNew::VVLSliceSets->Delete();

    vtkpxVVLinkConnectorNew::VVLSliceSets = vvlink::VVLQGetSliceSets::New();
    vtkpxVVLinkConnectorNew::VVLSliceSets->SetCoordinateSystem( this->VVLCoordinateSystem );
    vtkpxVVLinkConnectorNew::VVLSliceSets->Execute( vtkpxVVLinkConnectorNew::VVLinkServer );


    int ok=this->UpdateImageList();

    // prepare tracking
    if ( vtkpxVVLinkConnectorNew::VVLNTrackingData )
      vtkpxVVLinkConnectorNew::VVLNTrackingData->Delete();

    vtkpxVVLinkConnectorNew::VVLNTrackingData = vvlink::VVLNTrackingData::New();
    vtkpxVVLinkConnectorNew::VVLNTrackingData->SetCoordinateSystem( this->VVLCoordinateSystem );
    vtkpxVVLinkConnectorNew::VVLNTrackingData->RegisterOnServer( vtkpxVVLinkConnectorNew::VVLinkServer );

    return 1;
  }
  catch ( vvlink::VVLEx &a )
  {

    if ( vtkpxVVLinkConnectorNew::VVLinkServer )
      vtkpxVVLinkConnectorNew::VVLinkServer->Delete();
    vtkpxVVLinkConnectorNew::VVLinkServer = NULL;

    if ( vtkpxVVLinkConnectorNew::VVLNTrackingData )
      vtkpxVVLinkConnectorNew::VVLNTrackingData->Delete();
    vtkpxVVLinkConnectorNew::VVLNTrackingData = NULL;

    if ( vtkpxVVLinkConnectorNew::VVLSliceSets )
      vtkpxVVLinkConnectorNew::VVLSliceSets->Delete();
    vtkpxVVLinkConnectorNew::VVLSliceSets = NULL;

    //      cerr << "Connection Failed\n";
    vtkErrorMacro(<<"Connection Failed: " << a.What().c_str() << "\n");
    return 0;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int vtkpxVVLinkConnectorNew::CloseVVLConnection()
{

  if (vtkpxVVLinkConnectorNew::VVLinkServer!=NULL)
	{
	  if ( vtkpxVVLinkConnectorNew::VVLNTrackingData )
    {
      vtkpxVVLinkConnectorNew::VVLNTrackingData->UnregisterFromServer();
      vtkpxVVLinkConnectorNew::VVLNTrackingData->Delete();
    }
	  vtkpxVVLinkConnectorNew::VVLNTrackingData = NULL;

	  if ( vtkpxVVLinkConnectorNew::VVLSliceSets != NULL )
	    vtkpxVVLinkConnectorNew::VVLSliceSets->Delete();
	  vtkpxVVLinkConnectorNew::VVLSliceSets = NULL;

	  vtkpxVVLinkConnectorNew::VVLinkServer->Disconnect();
	  vtkpxVVLinkConnectorNew::VVLinkServer->Delete();
	  vtkpxVVLinkConnectorNew::VVLinkServer=NULL;

	  if (vtkpxVVLinkConnectorNew::VVLinkImageListNames!=NULL)
	    delete [] vtkpxVVLinkConnectorNew::VVLinkImageListNames;

	  vtkpxVVLinkConnectorNew::VVLinkImageListNames=NULL;
	  vtkpxVVLinkConnectorNew::VVLinkNumberOfImages=0;

	  return 1;
	}

  return 0;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int vtkpxVVLinkConnectorNew::UpdateImageList()
{
  int length=0;

  if ( vtkpxVVLinkConnectorNew::VVLSliceSets == NULL )
    return 0;

  for(int i=0; i<vtkpxVVLinkConnectorNew::VVLSliceSets->GetNumberOfSliceSets(); i++ )
    length += vtkpxVVLinkConnectorNew::VVLSliceSets->GetSliceSet(i)->GetName().length() + 10;

  if (vtkpxVVLinkConnectorNew::VVLinkImageListNames!=NULL)
    delete [] vtkpxVVLinkConnectorNew::VVLinkImageListNames;
  vtkpxVVLinkConnectorNew::VVLinkImageListNames=new char[length];

  strcpy(vtkpxVVLinkConnectorNew::VVLinkImageListNames,"");

  vtkpxVVLinkConnectorNew::VVLinkNumberOfImages = vtkpxVVLinkConnectorNew::VVLSliceSets->GetNumberOfSliceSets();

  for(int i=0; i<vtkpxVVLinkConnectorNew::VVLSliceSets->GetNumberOfSliceSets(); i++ )
    sprintf( vtkpxVVLinkConnectorNew::VVLinkImageListNames, "%s%s,",
	     vtkpxVVLinkConnectorNew::VVLinkImageListNames, vtkpxVVLinkConnectorNew::VVLSliceSets->GetSliceSet(i)->GetName().c_str());

  return vtkpxVVLinkConnectorNew::VVLSliceSets->GetNumberOfSliceSets();
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int vtkpxVVLinkConnectorNew::SendViewerToVVLChannel(vtkRenderWindow* renwin)
{
  if (vtkpxVVLinkConnectorNew::VVLinkServer==NULL)
    return 0;

  renwin->Render();

  vtkObjectContainer<vtkWindowToImageFilter> renSrc( vtkWindowToImageFilter::New() );
  renSrc->SetInput(renwin);
  renSrc->Update();

	try
	{
    std::stringstream channel;
    channel << "Channel " << VVLinkChannel;

		vtkObjectContainer<vvlink::VVLQSetView> cmd( vvlink::VVLQSetView::New() );
		cmd->SetChannel( channel.str() );
		cmd->SetImage( renSrc->GetOutput() );
		cmd->Execute( vtkpxVVLinkConnectorNew::VVLinkServer );
	}
	catch( vvlink::VVLEx &e )
	{
		vtkErrorMacro( << e.What().c_str() << "\n" );
		return 0;
	}

  return 1;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int   vtkpxVVLinkConnectorNew::GetNumberOfVVLinkImages()
{
  return vtkpxVVLinkConnectorNew::VVLinkNumberOfImages;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
char* vtkpxVVLinkConnectorNew::GetVVLinkImageNameList()
{
  return vtkpxVVLinkConnectorNew::VVLinkImageListNames;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
char* vtkpxVVLinkConnectorNew::GetVVLinkPatientName()
{
  return vtkpxVVLinkConnectorNew::VVLinkPatientName;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int vtkpxVVLinkConnectorNew::GetVVLinkImage(char* imagename,vtkImageData* img,vtkMatrix4x4* matrix,vtkLookupTable* lut)
{
  if (vtkpxVVLinkConnectorNew::VVLinkServer==NULL)
	{
		vtkErrorMacro(<<"No Connection");
		return 0;
	}

	if ( vtkpxVVLinkConnectorNew::VVLSliceSets == NULL )
	{
		vtkErrorMacro(<<"connection but no slice set object created yet");
		return 0;
	}

  if (img==NULL || matrix==NULL || lut==NULL)
	{
		vtkErrorMacro(<<"Pointers must be initilialized prior to GetVVLInkImage");
		return 0;
	}

  try
	{
		const vvlink::VVLSliceSet *sliceSet = vtkpxVVLinkConnectorNew::VVLSliceSets->GetSliceSet( imagename );

		matrix->DeepCopy( sliceSet->GetOrthogonalTransformation() );
		img->ShallowCopy( sliceSet->GetOrthogonalVolume() );
		lut->DeepCopy( sliceSet->GetLookupTable() );
		return 1;
	}
  catch ( vvlink::VVLEx &e )
	{
		vtkErrorMacro(<< "Image Download Failed: " << e.What().c_str() );
		return 0;
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int vtkpxVVLinkConnectorNew::GetVVLabeledPoints(vtkPoints* points)
{
  if (vtkpxVVLinkConnectorNew::VVLinkServer==NULL)
	{
		vtkErrorMacro(<<"No Connection");
		return 0;
	}

  if (points==NULL)
	{
		vtkErrorMacro(<<"Pointers must be initilialized prior to GetVVLInkImage");
		return 0;
	}

  try
	{
  	vtkObjectContainer<vvlink::VVLQGetPoints>cmd( vvlink::VVLQGetPoints::New() );
		cmd->SetCoordinateSystem( this->VVLCoordinateSystem );
		cmd->SetPointGroup( vvlink::VVLQGetPoints::GROUP_LABELED_POINTS );
		cmd->Execute( vtkpxVVLinkConnectorNew::VVLinkServer );

		for( int i=0; i<cmd->GetNumberOfPoints(); i++ )
		{
			points->InsertNextPoint( cmd->GetPoint(i)->GetPoint() );
		}
	}
  catch ( vvlink::VVLEx &a )
	{
		vtkErrorMacro(<<"Point Download Failed: " << a.What().c_str());
    return 0;
	}

  return 1;
}
//-------
int vtkpxVVLinkConnectorNew::GetVVNamesOfTrajectories(int numberOfTrajectories)
{
	if (vtkpxVVLinkConnectorNew::VVLinkServer==NULL)
		{
			vtkErrorMacro(<<"No Connection");
			return 0;
		}

	  try
		{
			vtkObjectContainer<vvlink::VVLQGetTrajectories>cmd( vvlink::VVLQGetTrajectories::New() );

			cmd->SetCoordinateSystem( this->VVLCoordinateSystem );
			cmd->SetTrajectoryGroup( vvlink::VVLQGetTrajectories::GROUP_INTRAOP_TRAJECTORY);
			cmd->Execute( vtkpxVVLinkConnectorNew::VVLinkServer );

			fprintf(stderr, "num traj intraop is %d\n", (int)cmd->GetNumberOfTrajectories()); 
			int temp = (int)cmd->GetNumberOfTrajectories(); 
						
			//for(int i=0;i<numberOfTrajectories; i++)
			for(int i=0;i<temp; i++)
			{
				const char * c = cmd->GetTrajectory(i)->GetName().c_str();
				fprintf(stderr, "++++++++++++ traj num %d name is %s\n", i, c);
				
				const std::wstring s2 = cmd->GetTrajectory(i)->GetNameWide();
				char *str = new char[255];
				
				std::wcout << s2 << std::endl;

				sprintf(str,"%ls",s2.data());
				fprintf(stderr, "wide name is %s and size is %d\n", str, sizeof(s2)); 
				std::cout << "test size is " << s2.length() << std::endl ; 
				
				 /*for(size_t i = 0, iMax = wcslen(wtext); i < iMax; ++i)
				   {
				      std::cout << " " << static_cast<unsigned int>(static_cast<unsigned short>(wtext[i])) ;
				   }
				   */

				 std::cout << std::endl << std::endl ;
			}
			//int test = cmd->GetNumberOfTrajectories();

			// fprintf(stderr, "+++++++++++++ number of trajectories is %d +++++++++++++++++++ ", test);
			return 1;
		}
	  catch ( vvlink::VVLEx &a )
		{
			vtkErrorMacro(<<"Point Download Failed: " << a.What().c_str());
	    	return 0;
		}

	  return -1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int vtkpxVVLinkConnectorNew::GetVVNumberOfTrajectories()
{
	if (vtkpxVVLinkConnectorNew::VVLinkServer==NULL)
		{
			vtkErrorMacro(<<"No Connection");
			return 0;
		}

	  try
		{
	  		//vtkObjectContainer<vvlink::VVLQGetPoints>cmd( vvlink::VVLQGetPoints::New() );

			vtkObjectContainer<vvlink::VVLQGetTrajectories>cmd( vvlink::VVLQGetTrajectories::New() );

			cmd->SetCoordinateSystem( this->VVLCoordinateSystem );
			//cmd->SetPointGroup( vvlink::VVLQGetPoints::GROUP_LABELED_POINTS );
			cmd->SetTrajectoryGroup( vvlink::VVLQGetTrajectories::GROUP_TRAJECTORY);
			cmd->Execute( vtkpxVVLinkConnectorNew::VVLinkServer );
			int test = cmd->GetNumberOfTrajectories();

			// fprintf(stderr, "+++++++++++++ number of trajectories is %d +++++++++++++++++++ ", test);
			return test;
		}
	  catch ( vvlink::VVLEx &a )
		{
			vtkErrorMacro(<<"Point Download Failed: " << a.What().c_str());
	    return 0;
		}

	  return -1;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int vtkpxVVLinkConnectorNew::GetVVTrajectoryPoints(vtkPoints* points, vtkTransform* xform)
{
  if (vtkpxVVLinkConnectorNew::VVLinkServer==NULL)
	{
		vtkErrorMacro(<<"No Connection");
		return 0;
	}

  if (points==NULL)
	{
		vtkErrorMacro(<<"Pointers must be initilialized prior to GetVVLInkImage");
		return 0;
	}

  try
	{
  		//vtkObjectContainer<vvlink::VVLQGetPoints>cmd( vvlink::VVLQGetPoints::New() );

		vtkObjectContainer<vvlink::VVLQGetTrajectories>cmd( vvlink::VVLQGetTrajectories::New() );

		cmd->SetCoordinateSystem( this->VVLCoordinateSystem );
		//cmd->SetPointGroup( vvlink::VVLQGetPoints::GROUP_LABELED_POINTS );
		cmd->SetTrajectoryGroup( vvlink::VVLQGetTrajectories::GROUP_TRAJECTORY);
		cmd->Execute( vtkpxVVLinkConnectorNew::VVLinkServer );

		//vtkObjectContainer<vvlink::VVLTrajectory>traj( vvlink::VVLTrajectory::New() );

		//traj = cmd->GetTrajectory(0);
		/*vtkFloatingPointType startpoint[3];
		cmd->GetTrajectory(0)->GetStartPoint(startpoint);

		vtkFloatingPointType endpoint[3];
		cmd->GetTrajectory(0)->GetEndPoint(endpoint);*/

		vtkPoints* oldPoints= vtkPoints::New();
		oldPoints->InsertNextPoint( cmd->GetTrajectory(4)->GetStartPoint() );
		oldPoints->InsertNextPoint( cmd->GetTrajectory(4)->GetEndPoint() );
		
		fprintf(stderr, "get trajectory - number 1 ----- %d \n", (int)cmd->GetNumberOfTrajectories()); 


		  //int np=oldPoints->GetNumberOfPoints();
		  //newPoints->SetNumberOfPoints(np);
		int np = 2;
		points->SetNumberOfPoints(np);

		  //  fprintf(stderr,"Obtained np=%d points\n",np);

		  for (int i=0;i<np;i++)
		    {
		      double x[3],tx[3];
		      oldPoints->GetPoint(i,x);
		      xform->GetLinearInverse()->TransformPoint(x,tx);
		      points->SetPoint(i,tx);
		    }

		  //return 1;


		//std::cout << traj->GetName();

		//traj->GetStartPoint( point );

		//for( int i=0; i<cmd->GetNumberOfPoints(); i++ )
		//{
			//points->InsertNextPoint( cmd->GetPoint(i)->GetPoint() );
		//}
	}
  catch ( vvlink::VVLEx &a )
	{
		vtkErrorMacro(<<"Point Download Failed: " << a.What().c_str());
    return 0;
	}

  return 1;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int vtkpxVVLinkConnectorNew::GetVVLinkImage(char* imagename,vtkImageData* img,vtkTransform* xform,
                                            vtkLookupTable* lut,int orientation,int sample)
{

  vtkImageData* img2=vtkImageData::New();

  vtkMatrix4x4* matrix=vtkMatrix4x4::New();
  int ok=this->GetVVLinkImage(imagename,img2,matrix,lut);
  if (ok==0)
	{
		img2->Delete();
		return 0;
	}

  if (sample<1)
    sample=1;
  else if (sample>8)
    sample=8;

  int dim[3];    img2->GetDimensions(dim);
  double spa[3]; img2->GetSpacing(spa);

  //  fprintf(stderr,"Dimensions =%d %d %d\n",dim[0],dim[1],dim[2]);

  vtkbisImageReslice* resl=vtkbisImageReslice::New();

  int outdim[3];
  double outspa[3];
  for (int ia=0;ia<=2;ia++)
	{
		outdim[ia]=dim[ia];
		outspa[ia]=spa[ia];
		if (outdim[ia]>256)
		{
			outdim[ia]=outdim[ia]/sample;
			outspa[ia]=outspa[ia]*double(sample);
		}
	}

  resl->SetOutputExtent(0,outdim[0]-1,
												0,outdim[1]-1,
												0,outdim[2]-1);
  resl->SetOutputOrigin(img2->GetOrigin());
  resl->SetOutputSpacing(outspa);
  resl->SetInterpolationModeToNearestNeighbor();
  resl->SetInput(img2);


  vtkMatrix4x4* mat=vtkMatrix4x4::New();
  mat->Identity();
  if ( orientation !=0 )
	{
		// Coronal or Saggital
		mat->SetElement(1,1,-1.0);
		mat->SetElement(1,3,double(dim[1]-1)*spa[1]);
	}
  else
	{
		// Axial
		mat->SetElement(2,2,-1.0);
		mat->SetElement(2,3,double(dim[2]-1)*spa[2]);
	}

  if (orientation!=2)
	{
		// Coronal or Axial Flip Left/Right
		mat->SetElement(0,0,-1.0);
		mat->SetElement(0,3,double(dim[0]-1)*spa[0]);
	}

  vtkMatrix4x4* final=vtkMatrix4x4::New();
  vtkMatrix4x4::Multiply4x4(matrix,mat,final);

  xform->Identity();
  xform->Concatenate(final);
  final->Delete();

  vtkMatrixToLinearTransform* tr=vtkMatrixToLinearTransform::New();
  tr->SetInput(mat);
  tr->Update();

  resl->SetResliceTransform(tr);
  resl->Update();

  img->ShallowCopy(resl->GetOutput());
  resl->Delete();
  img2->Delete();
  tr->Delete();

  return 1;
}
// ---------------------------------------------------------------------------------------------

int  vtkpxVVLinkConnectorNew::GetVVLabeledPoints(vtkPoints* newPoints,vtkTransform* xform)
{
  if (vtkpxVVLinkConnectorNew::VVLinkServer==NULL)
	{
	  vtkErrorMacro(<<"No Connection");
	  return 0;;
	}

  if (newPoints==NULL || xform==NULL)
	{
	  vtkErrorMacro(<<"Pointers must be initilialized prior to GetVVLabeledPoints");
	  return 0;
	}

  vtkPoints* oldPoints= vtkPoints::New();
  if ( !GetVVLabeledPoints( oldPoints ) )
    {
      vtkErrorMacro(<< "Point Download Failed");
      oldPoints->Delete();
      return 0;
    }

  int np=oldPoints->GetNumberOfPoints();
  newPoints->SetNumberOfPoints(np);

  //  fprintf(stderr,"Obtained np=%d points\n",np);

  for (int i=0;i<np;i++)
    {
      double x[3],tx[3];
      oldPoints->GetPoint(i,x);
      xform->GetLinearInverse()->TransformPoint(x,tx);
      newPoints->SetPoint(i,tx);
    }

  return 1;
}
// ---------------------------------------------------------------------------------------------
int  vtkpxVVLinkConnectorNew::GetVVLinkPointer(vtkPoints* newPoints,vtkTransform* xform)
{
  return this->GetVVLinkPointer(newPoints,xform,20.0);
}

int  vtkpxVVLinkConnectorNew::GetVVLinkPointer(vtkPoints* newPoints,vtkTransform* xform,double length)
{
  if (vtkpxVVLinkConnectorNew::VVLinkServer==NULL)
    {
      vtkErrorMacro(<<"No Connection");
      return 0;
    }

  if (newPoints==NULL || xform==NULL)
    {
      vtkErrorMacro(<<"Pointers must be initilialized prior to GetVVLInkPointer");
      return 0;
    }

  int index=0;

  if (newPoints->GetNumberOfPoints()!=2)
    {
      //      fprintf(stderr,"Setting Points to 2 \n");
      newPoints->SetNumberOfPoints(2);
    }

  try
    {
      vtkObjectContainer<vvlink::VVLTrackingData> data( vvlink::VVLTrackingData::New() );
      vtkpxVVLinkConnectorNew::VVLNTrackingData->GetTrackingData( data );

      for( std::list<std::string>::const_iterator itToolNames = this->VVLToolsAndOrder.begin();
	   itToolNames != this->VVLToolsAndOrder.end();
         itToolNames++ )
	{
	  try
	    {
	      const vvlink::VVLInstrument *instr = data->GetInstrument(*itToolNames);
	      if ( instr->IsValid() )
		{
		  double tip[3]; instr->GetTip(tip);
		  double dir[3]; instr->GetDirection(dir);
		  newPoints->SetPoint(0, instr->GetTip() );
		  newPoints->SetPoint(1, instr->GetDirection() );
		  ++index;
		  break;
		}
	    }
	  catch( vvlink::VVLNoSuchObjectErrEx & ){} // this tool is not available, just skip
	}
    }
  catch( vvlink::VVLEx &e ) // a real error occured
    {
      vtkErrorMacro( << "Error getting tools: " << e.What().c_str() << "\n" );
      return 0;
    }


  if (index==0)
    {
      return 0;
    }

  double tip[3]; newPoints->GetPoint(0,tip);
  double dir[3]; newPoints->GetPoint(1,dir);
  double base[3];
  for(int i=0;i<3;i++)
    base[i]=tip[i]-length*dir[i];

  /*  fprintf(stderr,"Get::Getting Points tip=(%.2f,%.2f,%.2f) base=(%.2f,%.2f,%.2f)\n",
	  tip[0],tip[1],tip[2],
	  base[0],base[1],base[2]);*/


  xform->GetLinearInverse()->TransformPoint(tip,tip);
  newPoints->SetPoint(0,tip);

  xform->GetLinearInverse()->TransformPoint(base,base);
  newPoints->SetPoint(1,base);

  /*  fprintf(stderr,"Get::Getting Points Mapped\n\t tip=(%.2f,%.2f,%.2f) \n\t base=(%.2f,%.2f,%.2f)\n",
	  tip[0],tip[1],tip[2],
	  base[0],base[1],base[2]);*/

  return 1;

}
// ---------------------------------------------------------------------------------------------
//   Get As Poly Data
// ---------------------------------------------------------------------------------------------
int  vtkpxVVLinkConnectorNew::GetVVLabeledPointsAsPolyData(vtkPolyData* newdata,vtkTransform* xform)
{
  if (vtkpxVVLinkConnectorNew::VVLinkServer==NULL)
    {
      vtkErrorMacro(<<"No Connection");
      return 0;;
    }

  if (newdata==NULL || xform==NULL)
    {
      vtkErrorMacro(<<"Pointers must be initilialized prior to GetVVLabeledPointsAsPolyData");
      return 0;
    }

  vtkObjectContainer<vtkPoints> points( vtkPoints::New() );
  if ( !this->GetVVLabeledPoints( points, xform ) )
    {
      vtkErrorMacro(<< "Point Download Failed");
      return 0;
    }

  // setting points
  newdata->SetPoints( points );

  // create corresponding cell array
  vtkCellArray* verts = vtkCellArray::New();
  newdata->SetVerts( verts );
  verts->Delete();
  for( vtkIdType i=0; i<points->GetNumberOfPoints(); i++ )
    newdata->GetVerts()->InsertNextCell( 1, &i );

  return 1;
}

// ---------------------------------------------------------------------------------------------
int  vtkpxVVLinkConnectorNew::GetVVLinkTrackedToolsAsPolyData(vtkPolyData* newdata,vtkTransform* xform)
{
  if (vtkpxVVLinkConnectorNew::VVLinkServer==NULL)
  {
    vtkErrorMacro(<<"No Connection");
    return 0;
  }

  if (newdata==NULL || xform==NULL)
  {
    vtkErrorMacro(<<"Pointers must be initilialized prior to GetVVLInkImage");
    return 0;
  }

  vtkObjectContainer<vtkPolyData> polydata( vtkPolyData::New() );
  // setting points array
  vtkObjectContainer<vtkPoints> points( vtkPoints::New() );
  polydata->SetPoints( points );

  // create cell array
  vtkObjectContainer<vtkCellArray> verts( vtkCellArray::New() );
  newdata->SetVerts( verts );

  try
  {
    vtkObjectContainer<vvlink::VVLTrackingData> data( vvlink::VVLTrackingData::New() );
    vtkpxVVLinkConnectorNew::VVLNTrackingData->GetTrackingData( data );

    for( int i=0; i<data->GetNumberOfInstruments(); i++ )
    {
      const vvlink::VVLInstrument *instr = data->GetInstrument( i );
      if ( instr->IsValid() )
	{
	  vtkIdType pid = points->InsertNextPoint( instr->GetTip() );
	  verts->InsertNextCell( 1, &pid );
	}
    }
  }
  catch( vvlink::VVLEx &e ) // a real error occured
  {
    vtkErrorMacro( << "Error getting tools: " << e.What().c_str() << "\n" );
    return 0;
  }

  vtkTransformPolyDataFilter* polyDataFilter=vtkTransformPolyDataFilter::New();
  polyDataFilter->SetInput(polydata);
  polyDataFilter->SetTransform(xform->GetLinearInverse());
  polyDataFilter->Update();

  newdata->ShallowCopy(polyDataFilter->GetOutput());
  polyDataFilter->Delete();
  return 1;
}
// ---------------------------------------------------------------------------------------------
int vtkpxVVLinkConnectorNew::SaveVVLinkLabeledPoints(char* filename,vtkTransform* xform)
{
  if (vtkpxVVLinkConnectorNew::VVLinkServer==NULL)
  {
    vtkErrorMacro(<<"No Connection");
    return 0;
  }


  FILE* fout=fopen(filename,"w");
  if (!fout)
    return 0;

  try
  {
    vtkObjectContainer<vvlink::VVLQGetPatientName> cmdPatient( vvlink::VVLQGetPatientName::New() );
    cmdPatient->Execute( vtkpxVVLinkConnectorNew::VVLinkServer );

    fprintf(fout, "#VVLinkLabeled Points for Patient %s, ID: %s\n",
            cmdPatient->GetName().c_str(), cmdPatient->GetID().c_str());
  }
  catch( vvlink::VVLCommandExecutionErrEx& ){} // possibly no patient loaded => do nothing
  catch( vvlink::VVLEx &e )
  {
    vtkErrorMacro( << e.What().c_str() << "\n" );
    fclose(fout);
    return 0;
  }

  try
  {
    vtkObjectContainer<vvlink::VVLQGetPoints> cmd( vvlink::VVLQGetPoints::New() );
    cmd->SetCoordinateSystem( this->VVLCoordinateSystem );
    cmd->SetPointGroup( vvlink::VVLQGetPoints::GROUP_LABELED_POINTS );
    cmd->Execute( vtkpxVVLinkConnectorNew::VVLinkServer );

    fprintf(fout,"#Total Number Of Labeled Points = %d\n", cmd->GetNumberOfPoints());
    fprintf(fout,"#Point No \t Location \t\t Color \t\t Label\n");
    fprintf(fout,"#---------------------------------------------------------------\n");

    for( int i=0; i<cmd->GetNumberOfPoints(); i++ )
    {
      double x[3];
      xform->GetLinearInverse()->TransformPoint( cmd->GetPoint(i)->GetPoint(), x );

      double fcolor[4];
      cmd->GetPoint(i)->GetColor(fcolor);

      fprintf(fout, "Point %d\t (%.3f,%.3f,%.3f) \t (%.2f,%.2f,%.2f,%.2f) \t %s\n",
              i+1,
              x[0],x[1],x[2],
              fcolor[0],fcolor[1],fcolor[2],fcolor[3],
              cmd->GetPoint(i)->GetName().c_str() );
    }
  }
  catch ( vvlink::VVLEx &a )
  {
    vtkErrorMacro(<<"Point Download Failed: " << a.What().c_str() << "\n");
    fclose(fout);
    return 0;
  }

  fclose(fout);
  return 1;
}
// ---------------------------------------------------------------------------------------------
int vtkpxVVLinkConnectorNew::SaveVVLinkTrackedTools(char* filename,vtkTransform* xform,char *comment)
{
  if (vtkpxVVLinkConnectorNew::VVLinkServer==NULL)
  {
    vtkErrorMacro(<<"No Connection");
    return 0;
  }

  if (xform==NULL)
  {
    vtkErrorMacro(<<"Pointers must be initilialized prior to SaveVVLinkTrackedTools");
    return 0;
  }


  FILE* fout=fopen(filename,"w");
  if (!fout)  {  return 0;   }

  try
  {
    vtkObjectContainer<vvlink::VVLQGetPatientName> cmdPatient( vvlink::VVLQGetPatientName::New() );
    cmdPatient->Execute( vtkpxVVLinkConnectorNew::VVLinkServer );

    fprintf(fout, "#VVLinkTrackedTools for Patient %s, ID: %s\n",
            cmdPatient->GetName().c_str(), cmdPatient->GetID().c_str());
  }
  catch( vvlink::VVLCommandExecutionErrEx& ){} // possibly no patient loaded => do nothing
  catch( vvlink::VVLEx &e )
  {
    vtkErrorMacro( << e.What().c_str() << "\n" );
    fclose(fout);
    return 0;
  }


  try
  {
    vtkObjectContainer<vvlink::VVLTrackingData> data( vvlink::VVLTrackingData::New() );
    vtkpxVVLinkConnectorNew::VVLNTrackingData->GetTrackingData( data );

    if (comment!=NULL)
      fprintf(fout,"#%s\n",comment);
    fprintf(fout,"#Total Number Of Tracked Tools = %d\n", data->GetNumberOfInstruments());

    for( int i=0; i<data->GetNumberOfInstruments(); i++ )
    {
      const vvlink::VVLInstrument *instr = data->GetInstrument(i);

      if ( instr->IsValid() )
	    {
	      fprintf(fout,"# -------------------------------------------------\n");
	      fprintf(fout,"Instrument: %s\n", instr->GetName().c_str());
	      fprintf(fout,"Tip = (%.3f,%.3f,%.3f)\t", instr->GetTip()[0], instr->GetTip()[1], instr->GetTip()[2]);
	      fprintf(fout,"Direction: (%.3f,%.3f,%.3f)\n", instr->GetDirection()[0], instr->GetDirection()[1], instr->GetDirection()[2]);

	      double totrackertrafo[16];
	      vtkMatrix4x4::DeepCopy( totrackertrafo, instr->GetTransformation() );
	      fprintf(fout,"To-tracker transformation matrix:\n");
	      fprintf(fout,"\t [ %.3f \t %.3f \t %.3f \t %.3f ]\n", totrackertrafo[ 0] ,totrackertrafo[ 1] ,totrackertrafo[ 2] ,totrackertrafo[ 3] );
	      fprintf(fout,"\t [ %.3f \t %.3f \t %.3f \t %.3f ]\n", totrackertrafo[ 4] ,totrackertrafo[ 5] ,totrackertrafo[ 6] ,totrackertrafo[ 7] );
	      fprintf(fout,"\t [ %.3f \t %.3f \t %.3f \t %.3f ]\n", totrackertrafo[ 8] ,totrackertrafo[ 9] ,totrackertrafo[10] ,totrackertrafo[11] );
	      fprintf(fout,"\t [ %.3f \t %.3f \t %.3f \t %.3f ]\n", totrackertrafo[12] ,totrackertrafo[13] ,totrackertrafo[14] ,totrackertrafo[15] );
	      fprintf(fout,"\n");
	    }
    }
  }
  catch( vvlink::VVLEx &e )
  {
    vtkErrorMacro( << e.What().c_str() << "\n" );
    fclose(fout);
    return 0;
  }

  fclose(fout);
  return 1;
}
// ---------------------------------------------------------------------------------------------
int  vtkpxVVLinkConnectorNew::GetAndDrawVVLinkPointer(vtkPoints* newPoints,vtkTransform* xform,
                                                      double length,double radius,double extension,
                                                      vtkConeSource* source, vtkSphereSource* tipsphere, vtkpxCrossHairSource* tipsphere2)
{
  int i;

  int ok=this->GetVVLinkPointer(newPoints,xform,length);
  if (ok==0)
    {
      return 0;
      /*
	newPoints->SetNumberOfPoints(2);
	newPoints->SetPoint(1,119.54, 55.803, 160.745);
	newPoints->SetPoint(0,119.54, 22.1486, 170.829);*/
    }

  double base[3],tip[3];

  if (newPoints->GetNumberOfPoints()<2 || source==NULL)
    {
      vtkErrorMacro(<<"Failed to Get Pointer Coordinates");
      return 0;
    }

  newPoints->GetPoint(0,tip);
  newPoints->GetPoint(1,base);

  /*  fprintf(stderr,"GetAndDraw::Getting Points tip=(%.2f,%.2f,%.2f) base=(%.2f,%.2f,%.2f)\n",
	  tip[0],tip[1],tip[2],
	  base[0],base[1],base[2]);*/

  double n[3],center[3];
  for (i=0;i<=2;i++)
    n[i]=tip[i]-base[i];
  vtkMath::Normalize(n);

  // Make Tip Longer by extension
  for (int ia=0;ia<=2;ia++)
    {
      tip[ia]=tip[ia]+extension*n[ia];
      base[ia]=base[ia]+extension*n[ia];
    }


  for (i=0;i<=2;i++)
    center[i]=tip[i]-0.5*length*n[i];

  source->SetHeight(length);
  source->SetRadius(radius);
  source->SetCenter(center);
  source->SetDirection(n);
  source->CappingOn();
  source->Update();

  tipsphere->SetCenter(tip);
  tipsphere->SetRadius(5.0); 

//  vtkCamera *myCamera = vtkCamera::New(); 
 // myCamera->Get


  tipsphere2->SetPosition(tip);
//  tipsphere2->SetLength(3.0); 
  tipsphere2->SetThickness(0.3);
  tipsphere2->SetShowSphere(0); 


  newPoints->SetPoint(0,tip);
  newPoints->SetPoint(1,base);

  return 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int vtkpxVVLinkConnectorNew::ReOrientMatrix(vtkImageData* source,int inputorient,int outputorient,vtkMatrix4x4* tmat,double sp_out[3], int dim_out[3])
{
  if (tmat==NULL)
    return 0;

  if (inputorient<0 || inputorient>2)
    inputorient=0;
  if (outputorient<0 || outputorient>2)
    outputorient=0;

  int   dim[3];  source->GetDimensions(dim);
  double sp[3];  source->GetSpacing(sp);
  double ori[3]; source->GetOrigin(ori);

  vtkMatrix4x4* mat=vtkMatrix4x4::New();
  mat->Identity();



  if (inputorient==1 && outputorient==0)
  {
    // Coronal->Axial
    mat->SetElement(1, 1, 0); mat->SetElement( 1, 2, -1);
    mat->SetElement(2, 1, 1); mat->SetElement( 2, 2,  0);
  }

  if (inputorient==0 && outputorient==1)
  {
    // Axial->Coronal
    mat->SetElement(1, 1,  0); mat->SetElement(1, 2, 1);
    mat->SetElement(2, 1, -1); mat->SetElement(2, 2, 0);
  }

  if (inputorient==1 && outputorient==2)
  {
    // Coronal->Sagittal
    mat->SetElement(0, 0, 0) ;  mat->SetElement(1, 0, 0 );  mat->SetElement(2, 0, 1);
    mat->SetElement(1, 1, -1);
    mat->SetElement(0, 2, -1 ); mat->SetElement(1, 2, 0 ); mat->SetElement(2, 2, 0);
  }

  if (inputorient==2 && outputorient==1)
  {
    // Sagittal->Coronal
    mat->SetElement(0, 0, 0) ;  mat->SetElement(0, 1, 0 );  mat->SetElement(0, 2, 1);
    mat->SetElement(1, 1, -1);
    mat->SetElement(2, 0, -1 ); mat->SetElement(2, 1, 0 ); mat->SetElement(2, 2, 0);
  }


  if (inputorient==0 && outputorient==2)
  {
    // Axial->Sagittal
    mat->SetElement(0, 0,  0) ;  mat->SetElement(1, 0, 1) ;  mat->SetElement(2, 0, 0);
    mat->SetElement(0, 1,  0) ;  mat->SetElement(1, 1, 0) ;  mat->SetElement(2, 1, 1);
    mat->SetElement(0, 2, -1) ;  mat->SetElement(1, 2, 0) ;  mat->SetElement(2, 2, 0);
  }

  if (inputorient==2 && outputorient==0)
  {
    // Sagittal->Axial
    mat->SetElement(0, 0,  0) ;  mat->SetElement(0, 1, 1) ;  mat->SetElement(0, 2, 0);
    mat->SetElement(1, 0,  0) ;  mat->SetElement(1, 1, 0) ;  mat->SetElement(1, 2, 1);
    mat->SetElement(2, 0, -1) ;  mat->SetElement(2, 1, 0) ;  mat->SetElement(2, 2, 0);
  }


  vtkpxVVLinkConnectorNew::ReOrientDimensions(sp,dim,inputorient,outputorient,sp_out,dim_out);
  float shift_y[3],shift_x[3];
  for (int i=0;i<=2;i++)
  {
    shift_y[i]= 0.5*sp[i]*float(dim[i]-1);
    shift_x[i]=-0.5*sp_out[i]*float(dim_out[i]-1);
  }

  vtkTransform* tr=vtkTransform::New();
  tr->Identity();
  tr->PostMultiply();
  tr->Translate(shift_x);
  tr->Concatenate(mat);
  tr->Translate(shift_y);
  tmat->DeepCopy(tr->GetMatrix());
  tr->Delete();

  return 1;
}
//------------------------------------------------------------------------------
int vtkpxVVLinkConnectorNew::ReOrientDimensions(double sp_in[3],int dim_in[3],int inputorient,int outputorient,
                                                double sp_out[3],int dim_out[3])
{

  if (inputorient<0 || inputorient>2)
    inputorient=0;
  if (outputorient<0 || outputorient>2)
    outputorient=0;


  if (inputorient==outputorient)
  {
    for (int ia=0;ia<=2;ia++)
    {
      dim_out[ia]=dim_in[ia];
      sp_out[ia]=sp_in[ia];
    }
  }

  if (inputorient==1 && outputorient==0)
  {
    // Coronal->Axial
    dim_out[0]=dim_in[0]; dim_out[1]=dim_in[2]; dim_out[2]=dim_in[1];
    sp_out[0]=sp_in[0];   sp_out[1]=sp_in[2];   sp_out[2]=sp_in[1];
  }

  if (inputorient==0 && outputorient==1)
  {
    // Axial->Coronal
    dim_out[0]=dim_in[0]; dim_out[1]=dim_in[2]; dim_out[2]=dim_in[1];
    sp_out[0]=sp_in[0];   sp_out[1]=sp_in[2];   sp_out[2]=sp_in[1];
  }

  if (inputorient==1 && outputorient==2)
  {
    // Coronal->Sagittal
    dim_out[0]=dim_in[2]; dim_out[1]=dim_in[1]; dim_out[2]=dim_in[0];
    sp_out[0]=sp_in[2];   sp_out[1]=sp_in[1];   sp_out[2]=sp_in[0];
  }

  if (inputorient==2 && outputorient==1)
  {
    // Sagittal->Coronal
    dim_out[0]=dim_in[2]; dim_out[1]=dim_in[1]; dim_out[2]=dim_in[0];
    sp_out[0]=sp_in[2];   sp_out[1]=sp_in[1];   sp_out[2]=sp_in[0];
  }


  if (inputorient==0 && outputorient==2)
  {
    // Axial->Sagittal
    dim_out[0]=dim_in[1]; dim_out[1]=dim_in[2]; dim_out[2]=dim_in[0];
    sp_out[0]=sp_in[1];   sp_out[1]=sp_in[2];   sp_out[2]=sp_in[0];
  }

  if (inputorient==2 && outputorient==0)
  {
    // Sagittal->Axial
    dim_out[0]=dim_in[2]; dim_out[1]=dim_in[0]; dim_out[2]=dim_in[1];
    sp_out[0]=sp_in[2];   sp_out[1]=sp_in[0];   sp_out[2]=sp_in[1];
  }

  return 1;
}
// ---------------------------------------------------------------------------------------------
void LinPrint2(vtkMatrix4x4* matrix,char* name)
{
  fprintf(stdout,"\n Printing %s\n",name);
  for(int i=0;i<=3;i++)
  {
    for (int j=0;j<=3;j++)
      fprintf(stdout,"%7.3f ",matrix->GetElement(i,j));
    fprintf(stdout,"\n");
  }
  return;
}


int vtkpxVVLinkConnectorNew::GetVVLinkImage(char* imagename,vtkImageData* img,vtkTransform* xform,
                                            vtkLookupTable* lut,int orientation,
                                            int flipleftright,int flipanteriorposterior,int flipsuperiorinferior,
                                            int outputorientation,int sample)
{

  vtkImageData* img2=vtkImageData::New();

  vtkMatrix4x4* matrix=vtkMatrix4x4::New();
  int ok=this->GetVVLinkImage(imagename,img2,matrix,lut);

  if (ok==0)
  {
    img2->Delete();
    return 0;
  }

  if (sample<1)
    sample=1;
  else if (sample>8)
    sample=8;

  if (orientation == -1 )
  {
    double m1=fabs(matrix->GetElement(1,1));
    double m2=fabs(matrix->GetElement(1,2));
    if (m1>m2)
      orientation=0;
    else
      orientation=1;
    fprintf(stderr,"Auto Orientation Set to %d (%.2f,%.2f)\n",orientation,m1,m2);
    flipleftright=1;
    if (orientation==0)
    {
      flipanteriorposterior=0;
      flipsuperiorinferior=1;
    }
    else
    {
      flipanteriorposterior=1;
      flipsuperiorinferior=0;
    }
  }


  if (orientation<0 || orientation>2)
    orientation=0;

  if (outputorientation< 0 || outputorientation>2)
    outputorientation=orientation;

  fprintf(stderr,"Parameters = orientations %d -> %d , flips =%d,%d,%d, sample=%d\n",
          orientation,outputorientation,flipleftright,flipanteriorposterior,flipsuperiorinferior,sample);

  double sp_out[3];int dim_out[3];
  vtkMatrix4x4* reorient=vtkMatrix4x4::New();
  vtkpxVVLinkConnectorNew::ReOrientMatrix(img2,orientation,outputorientation,reorient,sp_out,dim_out);


  int dim[3];    img2->GetDimensions(dim);
  double spa[3]; img2->GetSpacing(spa);
  double ori[3]; img2->GetOrigin(ori);

  fprintf(stderr,"Input Dimensions =%d %d %d, Spacing=%.2f,%.2f,%.2f Origin=%.2f,%.2f,%.2f\n",
					dim[0],dim[1],dim[2],spa[0],spa[1],spa[2],ori[0],ori[1],ori[2]);

  int doflips[3] = { 0 , 0,0 };

  switch (orientation)
	{
	case 0: // Axial
	  if (flipleftright)
	    doflips[0]=1;
	  if (flipanteriorposterior)
	    doflips[1]=1;
	  if (flipsuperiorinferior)
	    doflips[2]=1;
	  break;
	case 1: // Coronal
	  if (flipleftright)
	    doflips[0]=1;
	  if (flipanteriorposterior)
	    doflips[2]=1;
	  if (flipsuperiorinferior)
	    doflips[1]=1;
	  break;
	case 2: // Sagittal
	  if (flipleftright)
	    doflips[2]=1;
	  if (flipanteriorposterior)
	    doflips[0]=1;
	  if (flipsuperiorinferior)
	    doflips[1]=1;
	  break;
	}

  vtkMatrix4x4* flipmat=vtkMatrix4x4::New();
  flipmat->Identity();

  for (int ia=0;ia<=2;ia++)
	{
	  if (doflips[ia]==1)
    {
      flipmat->SetElement(ia,ia,-1.0);
      flipmat->SetElement(ia,3,double(dim[ia]-1)*spa[ia]);
    }
	}
  //  LinPrint2(flipmat,"FlipMat");
  //  LinPrint2(reorient,"Reorient");


  vtkMatrix4x4* mat=vtkMatrix4x4::New();
  vtkMatrix4x4* final=vtkMatrix4x4::New();
  vtkMatrix4x4::Multiply4x4(flipmat,reorient,mat);

  //  LinPrint2(mat,"Mat");

  // First The world to image connection
  vtkMatrix4x4::Multiply4x4(matrix,mat,final);

  //  LinPrint2(final,"Final");

  xform->Identity();
  xform->Concatenate(final);
  final->Delete();

  vtkMatrixToLinearTransform* tr=vtkMatrixToLinearTransform::New();
  tr->SetInput(mat);
  tr->Update();

  vtkbisImageReslice* resl=vtkbisImageReslice::New();
  resl->OptimizationOff();
  resl->SetOutputExtent(0,(dim_out[0])/sample-1,
                        0,(dim_out[1])/sample-1,
                        0,(dim_out[2]-1));
  resl->SetOutputOrigin(img2->GetOrigin());
  resl->SetOutputSpacing(double(sample)*sp_out[0],double(sample)*sp_out[1],sp_out[2]);
  resl->SetInterpolationModeToNearestNeighbor();
  resl->SetInput(img2);
  resl->SetResliceTransform(tr);
  resl->Update();

  img->ShallowCopy(resl->GetOutput());
  resl->Delete();
  img2->Delete();
  tr->Delete();


  mat->Delete();
  reorient->Delete();
  matrix->Delete();
  flipmat->Delete();

  return 1;

}
// ---------------------------------------------------------------------------------------------

