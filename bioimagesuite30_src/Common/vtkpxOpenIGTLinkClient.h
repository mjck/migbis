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




#ifndef __vtkpxOpenIGTLinkClient_h
#define __vtkpxOpenIGTLinkClient_h

#include "vtkObject.h"
#include "vtkPoints.h"
#include "vtkImageData.h"
#include "vtkClientSocket.h"
#include "vtkSocketController.h"
#include "vtkpxOpenIGTLinkClientObserver.h"

class vtkpxOpenIGTLinkClient : public vtkObject
{
public:
  static vtkpxOpenIGTLinkClient *New();
  vtkTypeMacro(vtkpxOpenIGTLinkClient,vtkObject);

  // Description:
  // Print Self
  void PrintSelf(ostream& os, vtkIndent indent);  

  // Description:
  // Initialize and Close Connection
  virtual int   InitializeConnection(char* server,int port);
  virtual int   CloseConnection();


  // Description:
  // Callback for Handling Error from Sending Data
  virtual int HandleErrorFromSocket();

  // Description:
  // Set Name of this "device"
  vtkSetStringMacro(ToolName);
  vtkGetStringMacro(ToolName);
  vtkGetMacro(IsConnected,int);

  // Description:
  // Send Tool Location
  virtual int SendToolLocation(vtkPoints* pts);
  virtual int SendToolLocation(vtkPoints* pts,vtkImageData* img,int slicermode);

  // Description:
  // Send Image
  virtual int SendImage(vtkImageData* img);
  
  // Description:
  // 


protected:

  vtkpxOpenIGTLinkClient();
  ~vtkpxOpenIGTLinkClient();
  vtkpxOpenIGTLinkClient(const vtkpxOpenIGTLinkClient&) {};
  void operator=(const vtkpxOpenIGTLinkClient&) {};

  // Use BTX..ETX to avoid parsing for tcl
  //BTX
  vtkClientSocket* Connector;
  vtkPoints* InternalPoints;
  char* ToolName;
  int  IsConnected;
  vtkpxOpenIGTLinkClientObserver* Observer;
  //ETX

};


#endif

