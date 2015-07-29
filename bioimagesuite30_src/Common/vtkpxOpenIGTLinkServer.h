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




#ifndef __vtkpxOpenIGTLinkServer_h
#define __vtkpxOpenIGTLinkServer_h

#include "vtkObject.h"
#include "vtkPoints.h"
#include "vtkImageData.h"
#include "vtkServerSocket.h"
#include "vtkClientSocket.h"
#include "vtkMultiThreader.h"
#include "vtkTransform.h"
#include "vtkImageData.h"
#include "vtkpxOpenIGTLinkServerObserver.h"
#include "vtkTimeStamp.h"

class vtkCriticalSection;

class vtkpxOpenIGTLinkServer : public vtkObject
{
public:
  static vtkpxOpenIGTLinkServer *New();
  vtkTypeMacro(vtkpxOpenIGTLinkServer,vtkObject);

  // Description:
  // Print Self
  void PrintSelf(ostream& os, vtkIndent indent);  

  // Description:
  // Initialize and Close Connection
  virtual int   InitializeConnection() { return this->InitializeConnection(-1);}
  virtual int   InitializeConnection(int port);
  virtual int   RestartConnection();
  virtual int   CloseConnection();


  // Description:
  // Receive All Data
  virtual int BeginReceivingDataInThread();

  // Function to Receive Data in Thread
  virtual int ThreadReceiveAllData();


  // Description
  // Get The Actual Sockets if needed
  vtkGetObjectMacro(Connector,vtkServerSocket);



  // Description:
  // Get Info About Server
  vtkGetMacro(IsServerActive,int);
  vtkGetMacro(IsServerConnected,int);
  vtkGetMacro(ThreadActive,int);
  vtkGetMacro(Port,int);
  vtkGetMacro(ThreadReceivingData,int);


  // Description:
  // Callback for Handling Error from Receive Data
  virtual int HandleErrorFromSocket();


  // Description:
  // Wait for Connection -- DO NOT USE this directly
  virtual int WaitForConnection();

  // Description:
  // Time when last data was received
  virtual unsigned long GetLastReceivedTransformTime();
  virtual unsigned long GetLastReceivedImageTime();

  // Description:
  // Get Last Good Stuff
  virtual vtkImageData* GetLastReceivedImage();
  virtual vtkTransform* GetLastReceivedImageTransform();
  virtual vtkTransform* GetLastReceivedTransform();


protected:

  vtkpxOpenIGTLinkServer();
  ~vtkpxOpenIGTLinkServer();
  vtkpxOpenIGTLinkServer(const vtkpxOpenIGTLinkServer&) {};
  void operator=(const vtkpxOpenIGTLinkServer&) {};

  // Use BTX..ETX to avoid parsing for tcl
  //BTX
  vtkServerSocket* Connector;
  vtkClientSocket* ClientSocket;
  vtkMultiThreader *ServerThreader;
  vtkpxOpenIGTLinkServerObserver* Observer;

  
  // Key Variables
  int  IsServerActive;
  int  IsServerConnected;
  int  Port;

  char buffer[2000];
  int  ThreadActive;


  vtkTransform* LastReceivedTransform;
  vtkTransform* TempTransform;

  vtkImageData* LastReceivedImage;
  vtkImageData* TempImage;
  vtkTransform* LastReceivedImageTransform;
  vtkTransform* TempImageTransform;


  vtkTimeStamp  LastReceivedImageTime;   // Time Last Data was received
  vtkTimeStamp  LastReceivedTransformTime;   // Time Last Data was received
  int           ActivelyReceiving;
  int           ThreadReceivingData;
  int           ThreadReceivingDataId;
  vtkCriticalSection *ImageSemaphore;
  vtkCriticalSection *TransformSemaphore;

  //ETX
  // Description:
  // Internal "Object Factory" Methods
  virtual int ReceiveTransform(vtkClientSocket* ReceiveConnector);
  virtual int ReceiveImage(vtkClientSocket* ReceiveConnector);
  

  // Description:
  // Receive Stuff
  virtual int ReceiveData(vtkClientSocket* ReceiveConnector);


  // Description:
  // Update Last Received Time and trigger USER Event
  virtual void SetLastReceivedTimeAndType(int tp);




};


#endif

