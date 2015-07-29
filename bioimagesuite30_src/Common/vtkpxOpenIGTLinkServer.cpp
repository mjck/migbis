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







// OpenIGTLink specific includes
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkpxOpenIGTLinkServer.h"
#include "vtkSocketController.h"
#include "vtkMath.h"
#include "vtkPointData.h"
#include "vtkCriticalSection.h"
#include "vtkMutexLock.h"
#include "vtkCommand.h"
#include "igtl_header.h"
#include "igtl_transform.h"
#include "igtl_image.h"
#include "igtl_util.h"
#include "igtl_types.h"
#include <string.h>


vtkpxOpenIGTLinkServer* vtkpxOpenIGTLinkServer::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxOpenIGTLinkServer");
  if(ret)
    {
      return (vtkpxOpenIGTLinkServer*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxOpenIGTLinkServer;
}

// Construct object to extract all of the input data.
vtkpxOpenIGTLinkServer::vtkpxOpenIGTLinkServer()
{
  this->Connector=NULL;
  this->Observer=NULL;
  this->ClientSocket=NULL;

  this->IsServerActive=0;
  this->IsServerConnected=0;
  this->ThreadActive=0;
  this->Port=18946;

  this->LastReceivedImage=NULL;
  this->TempImage=NULL;


  this->ImageSemaphore = vtkCriticalSection::New();
  this->LastReceivedImageTime.Modified();

  this->LastReceivedImageTransform=vtkTransform::New();
  this->LastReceivedImageTransform->Identity();
  this->TempImageTransform=vtkTransform::New();
  this->TempImageTransform->Identity();


  this->LastReceivedTransform=vtkTransform::New();
  this->LastReceivedTransform->Identity();
  this->TempTransform=vtkTransform::New();
  this->TempTransform->Identity();


  this->TransformSemaphore = vtkCriticalSection::New();
  this->LastReceivedTransformTime.Modified();

  this->ServerThreader=vtkMultiThreader::New();
  this->ActivelyReceiving=0;
  this->ThreadReceivingData=0;
  this->ThreadReceivingDataId=-1;

  // Get Windows networking initialized!
  vtkSocketController* dummy=vtkSocketController::New();
  dummy->Initialize();
  dummy->Delete();

}

// ----------------------------------------------------------------------

vtkpxOpenIGTLinkServer::~vtkpxOpenIGTLinkServer()
{
  if (this->Connector!=NULL)
    this->Connector->Delete();

  if (this->Observer!=NULL)
    this->Observer->Delete();

  this->ServerThreader->Delete();

  if (this->LastReceivedImage!=NULL)
    this->LastReceivedImage->Delete();
  if (this->TempImage!=NULL)
    this->TempImage->Delete();
  this->ImageSemaphore->Delete();

  this->LastReceivedImageTransform->Delete();
  this->TempImageTransform->Delete();


  this->LastReceivedTransform->Delete();
  this->TempTransform->Delete();


  this->TransformSemaphore->Delete();

}
// ---------------------------------------------------------------------------
vtkImageData* vtkpxOpenIGTLinkServer::GetLastReceivedImage()
{
  vtkImageData* retval=NULL;

  this->ImageSemaphore->Lock();
  // ++++++++++++++++++++
  if (this->TempImage!=NULL)
    {
      if (this->LastReceivedImage==NULL)
	this->LastReceivedImage=vtkImageData::New();
      this->LastReceivedImage->ShallowCopy(this->TempImage);
      retval=this->LastReceivedImage;
    }
  // ++++++++++++++++++++
  this->ImageSemaphore->Unlock();
  return retval;
}


vtkTransform* vtkpxOpenIGTLinkServer::GetLastReceivedImageTransform()
{
  this->ImageSemaphore->Lock();
  // ++++++++++++++++++++
  this->LastReceivedImageTransform->Identity();
  this->LastReceivedImageTransform->Concatenate(this->TempImageTransform);
  // ++++++++++++++++++++
  this->ImageSemaphore->Unlock();

  return this->LastReceivedImageTransform;
}

 
vtkTransform* vtkpxOpenIGTLinkServer::GetLastReceivedTransform()
{
  this->TransformSemaphore->Lock();
  // ++++++++++++++++++++
  this->LastReceivedTransform->Identity();
  this->LastReceivedTransform->Concatenate(this->TempTransform);
  // ++++++++++++++++++++
  this->TransformSemaphore->Unlock();

  return this->LastReceivedTransform;
}

void vtkpxOpenIGTLinkServer::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkObject::PrintSelf(os,indent);

  os << indent << "  IsServerActive " << this->IsServerActive << std::endl;
  os << indent << "  IsServerConnected " << this->IsServerConnected << std::endl;
  os << indent << "  Port " << this->Port << std::endl;

  os << indent << "  LastReceivedImageTime " << this->LastReceivedImageTime << std::endl;
  os << indent << "  LastReceivedImage " << this->LastReceivedImage << std::endl;
  os << indent << "  TempImage " << this->TempImage << std::endl;
  os << indent << "  ImageSemaphore " << this->ImageSemaphore << std::endl;

  os << indent << "  LastReceivedTransformTime " << this->LastReceivedTransformTime << std::endl;
  os << indent << "  LastReceivedTransform " << this->LastReceivedTransform << std::endl;
  os << indent << "  TempTransform " << this->TempTransform << std::endl;
  os << indent << "  TransformSemaphore " << this->TransformSemaphore << std::endl;

  os << indent << "  Connector " << this->Connector << std::endl;
  os << indent << "  Observer " << this->Observer << std::endl;
  os << indent << "  ClientSocket " << this->ClientSocket << std::endl;
}

// -------------------------------------------------------------------------
unsigned long int vtkpxOpenIGTLinkServer::GetLastReceivedImageTime() 
{
  this->ImageSemaphore->Lock();
  // ++++++++++++++++++++
  unsigned long a=this->LastReceivedImageTime.GetMTime();
  // ++++++++++++++++++++
  this->ImageSemaphore->Unlock();
  return a;
}

unsigned long int vtkpxOpenIGTLinkServer::GetLastReceivedTransformTime() 
{
  this->TransformSemaphore->Lock();
  // ++++++++++++++++++++
  unsigned long a=this->LastReceivedTransformTime.GetMTime();
  // ++++++++++++++++++++
  this->TransformSemaphore->Unlock();
  return a;
}

// ---------------------------------------------------------------------------

void vtkpxOpenIGTLinkServer::SetLastReceivedTimeAndType(int tp)
{
  if (tp==0)
    {
      this->TransformSemaphore->Lock();
      // ++++++++++++++++++++
      this->LastReceivedTransformTime.Modified();
      // ++++++++++++++++++++
      this->TransformSemaphore->Unlock();
    }
  else
    {
      this->ImageSemaphore->Lock();
      // ++++++++++++++++++++
      this->LastReceivedImageTime.Modified();
      // ++++++++++++++++++++
      this->ImageSemaphore->Unlock();
    }
  this->InvokeEvent(vtkCommand::UserEvent,NULL);
}

// this function runs in an alternate thread to asyncronously grab frames
// ---------------------------------------------------------------------------
static void vtkpxOpenIGTLinkServerWaitForConnectionThread(vtkMultiThreader::ThreadInfo *data)
{
  vtkpxOpenIGTLinkServer *self = (vtkpxOpenIGTLinkServer *)(data->UserData);
  self->WaitForConnection();
}
// ---------------------------------------------------------------------------
int vtkpxOpenIGTLinkServer::HandleErrorFromSocket()
{
  // This is to keep the vtkErrorMacro from being called ....
  vtkDebugMacro(<<"++ No data to read .., ignoring \n");
  //  fprintf(stderr,"No More Data\n");
  return 0;
}
// ---------------------------------------------------------------------------
int vtkpxOpenIGTLinkServer::WaitForConnection()
{
  if (this->Connector==NULL)
    return 0;

  this->ThreadActive=1;
  vtkDebugMacro(<<"++ OpenIGTLinkServer waiting for connection\n");

  this->IsServerActive=1;
  this->IsServerConnected=0;


  vtkClientSocket* in_socket=this->Connector->WaitForConnection(0);
  if (in_socket==NULL)
    {
      int r=this->Connector->CreateServer(this->Port);
      if (r!=0)
	{
	  std::cerr << "Failed to create  server on " << this->Port  << std::endl;
	  return 0;
	}
      in_socket=this->Connector->WaitForConnection(0);
    }
  
  int isdone=0;

  if (in_socket!=NULL)
    {
      //      this->InvokeEvent(vtkCommand::NextDataEvent ,NULL);
      vtkDebugMacro(<<"++ Connection Received all set ... now waiting for data\n");
      this->IsServerConnected=1;

      this->Observer->AttachSocketAndServer(in_socket,this);
      this->ClientSocket=in_socket;
    }
  this->ThreadActive=0;
  return 1;
}
//----------------------------------------------------------------------------
int   vtkpxOpenIGTLinkServer::InitializeConnection(int port)
{
  if (ThreadActive==1)
    {
      std::cerr << "Server Thread still active can not initialize connection " << std::endl;
      return 0;
    }
  
  if (this->IsServerConnected==1)
    this->CloseConnection();
  
  int oldport=this->Port;
  if (port!=-1)
    this->Port=port;
  
  if (this->Port!=oldport  || this->Connector==NULL)
    {
      if (this->Connector==NULL)
	this->Connector=vtkServerSocket::New();
      else
	this->Connector->CloseSocket();

      //      fprintf(stderr,"Proceeding to Create Server\n");
      int r=this->Connector->CreateServer(this->Port);
      
      if (r!=0)
	{
	  std::cerr << "Failed to create  server on " << port  << std::endl;
	  return 0;
	}
    }

  if (this->Observer==NULL)
    this->Observer=vtkpxOpenIGTLinkServerObserver::New();  
  return this->RestartConnection();
}
// ---------------------------------------------------------------------------
int   vtkpxOpenIGTLinkServer::RestartConnection()
{
  if (this->Connector==NULL || this->Observer==NULL)
    return 0;

  std::cerr << "Simply Restarting Connection" << this->Port  << std::endl;
  this->ServerThreader->SpawnThread((vtkThreadFunctionType)		\
				    &vtkpxOpenIGTLinkServerWaitForConnectionThread,this);
  return 1;
}
// ---------------------------------------------------------------------------
int   vtkpxOpenIGTLinkServer::CloseConnection()
{
  //  fprintf(stderr,"Begin Close Connection\n");
  if (this->IsServerConnected==0)
    {
      std::cerr << "No Connection ... cannot close connection " << std::endl;
      return 1;
    }

  if (ThreadActive==1)
    {
      std::cerr << "Server Thread still active can not close connection " << std::endl;
      // Need a hack here that connects and closes ...
      return 0;
    }

  if (this->Connector==NULL)
    {
      std::cerr << "Connector is NULL cannot disconnect " << std::endl;
      return 1;
    }

  if (this->ThreadReceivingData>0)
    {
      std::cerr << "Waiting to terminate thread prior to disconnection " << std::endl;
      this->ServerThreader->TerminateThread(this->ThreadReceivingDataId);
    }


  this->ClientSocket=NULL;

  this->IsServerActive=0;
  this->ThreadReceivingData=0;
  this->IsServerConnected=0;
  //  fprintf(stderr,"End Close Connection\n");
  return 1;
}
// -------------------------------------------------------------------------
static void vtkpxOpenIGTLinkServerKeepReceivingData(vtkMultiThreader::ThreadInfo *data)
{
  vtkpxOpenIGTLinkServer *self = (vtkpxOpenIGTLinkServer *)(data->UserData);
  self->ThreadReceiveAllData();
}


int vtkpxOpenIGTLinkServer::BeginReceivingDataInThread()
{
  if (this->ThreadReceivingData==1)
    return 0;

  if (this->IsServerConnected==0)
    return 0;
  
  this->ThreadReceivingDataId=this->ServerThreader->SpawnThread((vtkThreadFunctionType) \
								&vtkpxOpenIGTLinkServerKeepReceivingData,this);
  return this->ThreadReceivingDataId;
}

int vtkpxOpenIGTLinkServer::ThreadReceiveAllData()
{
  if (this->ClientSocket==NULL)
    return 0;

  if (this->ThreadReceivingData==1)
    return 0;

  this->ThreadReceivingData=1;
  //  fprintf(stderr,"Receiving All Data\n");

  int ok=1,count=0;
  while (ok>0)
    {
      ok=this->ReceiveData(this->ClientSocket);     
      ++count;
      //      fprintf(stderr,"\t\t Received Packet %d %d\n",ok,count);
    }

  this->ThreadReceivingData=0;
  this->CloseConnection();

  //  fprintf(stderr,"Done Thread Receiving Data .. client disconnected\n");
  return count;
}
// -------------------------------------------------------------------------
int vtkpxOpenIGTLinkServer::ReceiveData(vtkClientSocket* in_socket)
{
  if (this->ActivelyReceiving==1)
    return 1;

  if (this->IsServerActive==0)
    return -1;

  if (this->IsServerConnected==0)
    return -1;

  int len=1;
  int n=0;


  this->ActivelyReceiving=1;

  vtkDebugMacro(<<"++ In Receiving Data\n");
  len=in_socket->Receive((void*)buffer,58);
  if (len!=58)
    {
      this->ActivelyReceiving=0;
      return 0;
    }

  igtl_header* header=  (igtl_header*)buffer;
  igtl_header_convert_byte_order(header);   /* convert endian if necessary */

  vtkDebugMacro(<<"Received packed name= " << header->name << " device_name= " << header->device_name << "\n");
  //  std::cerr<<"Received packed name= " << header->name << " device_name= " << header->device_name << "\n";



  int st=0;

  if (strncmp(header->name,"TRANSFORM",9)==0)
    {
      st=this->ReceiveTransform(in_socket);
    }
  else if (strncmp(header->name,"IMAGE",5)==0)
    {
      st=this->ReceiveImage(in_socket);
    }
  else
    {
      st=0;
    }

  this->ActivelyReceiving=0;
  return st;
}
// -------------------------------------------------------------------------

int vtkpxOpenIGTLinkServer::ReceiveTransform(vtkClientSocket* in_socket)
{
  int len=in_socket->Receive((void*)buffer,48);
  if (len!=48)
    return 0;

  vtkDebugMacro(<<"Received Transform size=" << len <<"\n");

  igtl_float32* tr=(igtl_float32*)buffer;
  igtl_transform_convert_byte_order(tr);   /* convert endian if necessary */

  /*  for (int i=0;i<=11;i++)
    fprintf(stderr,"Tr[%d]=%.2f ",i,tr[i]);
    fprintf(stderr,"\n");*/

  /*  double p[3];
  p[0]=tr[9];   p[1]=tr[10];   p[2]=tr[11];
  fprintf(stderr,"Point (%.2f %.2f %.2f)\n",p[0],p[1],p[2]);*/

  double elements[16];
  for (int j=0;j<=2;j++)
    for (int i=0;i<=3;i++)
      elements[j*4+i]=tr[i*3+j];
  elements[12]=0;  elements[13]=0;  elements[14]=0;  elements[15]=1;

  int index=0;
  /*  for (int j=0;j<=3;j++)
    {
      fprintf(stderr,"[ ");
      for (int i=0;i<=3;i++)
	{
	  fprintf(stderr,"%f ",elements[index]);
	  ++index;
	}
      fprintf(stderr,"]\n");
      }*/

  this->TransformSemaphore->Lock();
  // ++++++++++++++++++++
  this->TempTransform->Identity();
  this->TempTransform->Concatenate(elements);
  // ++++++++++++++++++++
  this->TransformSemaphore->Unlock();


  this->SetLastReceivedTimeAndType(0);

  return 1;
}
// -------------------------------------------------------------------------
int vtkpxOpenIGTLinkServer::ReceiveImage(vtkClientSocket* in_socket)
{
  int len=in_socket->Receive((void*)buffer,72);
  if (len!=72)
    return 0;


  igtl_image_header* image_header=(igtl_image_header*)buffer;
  
  igtl_image_convert_byte_order(image_header);
  igtl_uint64 imgsize=igtl_image_get_data_size(image_header);



  // Begin Critical Section
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  this->ImageSemaphore->Lock();

  int sum=0;
  float spacing[3],origin[3],norm_i[3],norm_j[3],norm_k[3];
  igtl_image_get_matrix(spacing,origin,norm_i,norm_j,norm_k,image_header);

  if (this->TempImage!=NULL)
    {
      int dim[3]; this->TempImage->GetDimensions(dim);
      for (int ia=0;ia<=2;ia++)
	sum+=abs(image_header->size[ia]-dim[ia]);
      if (sum!=0)
	{
	  fprintf(stderr,"Sum is not zero allocating memory %d,%d,%d vs %d,%d,%d\n",
		  image_header->size[0],
		  image_header->size[1],
		  image_header->size[2],
		  dim[0],dim[1],dim[2]);
	  this->TempImage->Delete();
	  this->TempImage=vtkImageData::New();
	}
    } 
  else 
    {
      sum=-1;
      fprintf(stderr,"Pointer is zero allocating memory\n");
      this->TempImage=vtkImageData::New();
    }

  image_header->version     = IGTL_IMAGE_HEADER_VERSION;

  //  this->LastReceivedImage->SetScalarType(image_header->data_type);
  if (sum!=0)
    {
      this->TempImage->SetScalarType(VTK_UNSIGNED_CHAR);
      this->TempImage->SetDimensions(image_header->size[0],image_header->size[1],image_header->size[2]);
    }


  // Store the normals in a transformation
  double elements[16];
  elements[0]=norm_i[0]; elements[4]=norm_j[0]; elements[8]=norm_k[0];
  elements[1]=norm_i[1]; elements[5]=norm_j[1];	elements[9]=norm_k[1];
  elements[2]=norm_i[2]; elements[6]=norm_j[2];	elements[10]=norm_k[2];
  elements[3]=0.0;       elements[7]=0.0;      	elements[11]=0.0;      

  elements[12]=0.0; elements[13]=0.0; elements[14]=0.0; elements[15]=1.0;
  this->TempImageTransform->Identity();
  this->TempImageTransform->Concatenate(elements);
  // -----------------------------------------------
  // On to image
  // -----------------------------------------------

  for (int ia=0;ia<=2;ia++)
    spacing[ia]=fabs(spacing[ia]);

  if (sum!=0) 
    {
      this->TempImage->SetSpacing(spacing[0],spacing[1],spacing[2]);
      this->TempImage->SetOrigin(origin[0],origin[1],origin[2]);
      this->TempImage->AllocateScalars();
    }
  
  int dim[3]; this->TempImage->GetDimensions(dim);

  /*  fprintf(stderr,"Dim = %d x %d x %d  spa = %.2f x %.2f x %.2f size=%d\n",
	  dim[0],dim[1],dim[2],
	  spacing[0],spacing[1],spacing[2],imgsize);*/

  unsigned char* dpointer=new unsigned char[imgsize];
  int totalsize=in_socket->Receive((void*)dpointer,imgsize);

  vtkDataArray* arr=this->TempImage->GetPointData()->GetScalars();
  //  std::cerr <<"Received Image size= " << imgsize << "actual size=" << totalsize << "\n";
  unsigned char* ptr= (unsigned char*)arr->GetVoidPointer(0);
  memcpy(ptr,dpointer,imgsize);
  delete [] dpointer;

  //vtkDebugMacro(<<"Received Image size= " << imgsize << "actual size=" << totalsize << "\n");
  

  // End Critical Section
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  this->ImageSemaphore->Unlock();

  this->SetLastReceivedTimeAndType(1);
  return 1;
}


