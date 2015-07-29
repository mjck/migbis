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
#include "vtkpxOpenIGTLinkClient.h"
#include "vtkMath.h"
#include "vtkPointData.h"
#include "igtl_header.h"
#include "igtl_transform.h"
#include "igtl_image.h"
#include "igtl_util.h"
#include <string.h>

vtkpxOpenIGTLinkClient* vtkpxOpenIGTLinkClient::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxOpenIGTLinkClient");
  if(ret)
    {
      return (vtkpxOpenIGTLinkClient*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxOpenIGTLinkClient;
}

// Construct object to extract all of the input data.
vtkpxOpenIGTLinkClient::vtkpxOpenIGTLinkClient()
{
  this->Connector=vtkClientSocket::New();
  this->ToolName=NULL;
  this->SetToolName("BioImageSuite");
  this->IsConnected=0;
  this->InternalPoints=vtkPoints::New();
  this->InternalPoints->SetNumberOfPoints(2);
  vtkSocketController* dummy=vtkSocketController::New();
  dummy->Initialize();
  dummy->Delete();
  this->Observer=vtkpxOpenIGTLinkClientObserver::New();
}

vtkpxOpenIGTLinkClient::~vtkpxOpenIGTLinkClient()
{
  if (this->ToolName!=NULL)
    delete [] this->ToolName;

  this->CloseConnection();
  //  fprintf(stderr,"Deleting Connector\n");
  if (this->Connector)
    this->Connector->Delete();

  // fprintf(stderr,"Deleting Internal Points\n");
  this->InternalPoints->Delete();

  if (this->Observer!=NULL)
    this->Observer->Delete();
}
// ---------------------------------------------------------------------------

void vtkpxOpenIGTLinkClient::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkObject::PrintSelf(os,indent);

  os << indent << "  IsConnected " << this->IsConnected << "\n";
  os << indent << "  ToolName " << this->ToolName << "\n";
  os << indent << "  Connector " << this->Connector << "\n";
}
// ---------------------------------------------------------------------------
int   vtkpxOpenIGTLinkClient::InitializeConnection(char* server,int port)
{
  if (this->IsConnected)
    this->CloseConnection();

  int r=this->Connector->ConnectToServer(server,port);
  if (r!=0)
    {
      this->IsConnected=0;
      //vtkErrorMacro("Cannot connect to the server." << server << ":" << port << std::endl);
      return 0;
    }
  //  fprintf(stderr,"r=%d\n",r);


  this->Observer->AttachSocketAndClient(this->Connector,this);
  this->IsConnected=1;
  return 1;
}

// ---------------------------------------------------------------------------
int vtkpxOpenIGTLinkClient::HandleErrorFromSocket()
{
  // This is to keep the vtkErrorMacro from being called ....
  //  vtkDebugMacro(<<"++ Failed to send data .., ignoring \n");
  fprintf(stderr,"++ Failed to send data .., disconnecting \n");
  this->CloseConnection();
  return 0;
}
// ---------------------------------------------------------------------------
int   vtkpxOpenIGTLinkClient::CloseConnection()
{
  if (this->IsConnected && this->Connector!=NULL)
    {
      //      vtkDebugMacro(<<"Closing Socket\n");
      fprintf(stderr,"Closing Socket\n");
      this->Connector->CloseSocket();
      this->IsConnected=0;
      //      fprintf(stderr,"Closing Socket Done\n");
      return 1;
    }
  this->IsConnected=0;
  return 0;
}
// ---------------------------------------------------------------------------
int vtkpxOpenIGTLinkClient::SendToolLocation(vtkPoints* pts,vtkImageData* img,int slicermode)
{
  if (img==NULL || pts==0)
    {
      vtkErrorMacro(<<"Bad Inputs to Send Tool Location\n");
      return 0;
    }


  double spa[3]; img->GetSpacing(spa);
  double ori[3]; img->GetOrigin(ori);
  int    dim[3]; img->GetDimensions(dim);

  int np=pts->GetNumberOfPoints();

  if (  this->InternalPoints->GetNumberOfPoints()!=np)
    this->InternalPoints->SetNumberOfPoints(np);

  double mx[3];
  for (int ia=0;ia<=2;ia++)
    mx[ia]=double(dim[ia]-1)*0.5;

  for (int i=0;i<np;i++)
    {
      double cx[3]; pts->GetPoint(i,cx);
      for (int ia=0;ia<=2;ia++)
	{
	  if (slicermode>0)
	    {
	      // flip LPS->RAS
	      //	      fprintf(stderr,"\n\n\nBeginning ia=%d,cx[%d]=%.2f\n",ia,ia,cx[ia]);
	      // cx[ia]=cx[ia]-ori[ia];
	      //	      fprintf(stderr,"Origin shift cx[%d]=%.2f\n",ia,cx[ia]);
	      cx[ia]=cx[ia]/spa[ia];
	      //	      fprintf(stderr," / spa cx[%d]=%.2f\n",ia,cx[ia]);
	      if (ia<2)
		cx[ia]=double(dim[ia]-1)-cx[ia];
		    
	      if (slicermode==2)
		cx[ia]=cx[ia]-mx[ia];
	  
	      cx[ia]=cx[ia]*spa[ia];
	      //	      fprintf(stderr," *spa cx[%d]=%.2f\n",ia,cx[ia]);
	      //cx[ia]=cx[ia]+ori[ia];
	      //	      fprintf(stderr," +ori cx[%d]=%.2f\n",ia,cx[ia]);
	    }
	}
      //      fprintf(stderr,"**** Setting Point %d (%.2f %.2f %.2f)\n",i,cx[0],cx[1],cx[2]);
      this->InternalPoints->SetPoint(i,cx);
    }

  return this->SendToolLocation(this->InternalPoints);
}
// ---------------------------------------------------------------------------
int vtkpxOpenIGTLinkClient::SendToolLocation(vtkPoints* pts)
{
  vtkDebugMacro(<<"In Send Tool Location " << this->IsConnected << "\n");

  if (this->IsConnected==0 || this->Connector==NULL)
    return 0;

  if (pts==NULL)
    return 0;

  if (pts->GetNumberOfPoints()<1)
    return 0;

	


  igtl_float32 transform[12];    
  if (pts->GetNumberOfPoints()<2)
    {
      double x[3]; pts->GetPoint(0,x);
      transform[0] = 1.0;   transform[3] = 0;     transform[6] = 0;   transform[9] =  x[0];
      transform[1] = 0;     transform[4] = 1.0;   transform[7] = 0;   transform[10] = x[1];  
      transform[2] = 0;     transform[5] = 0;     transform[8] = 1.0; transform[11] = x[2];
      vtkDebugMacro(<<"Sending x= " << x[0] << "," << x[1] << "," << x[2] << "\n");
    }
  else
    {
      // ----------------------- Rotation ------------------
      // Create rotation matrix
      // Goal is to map
      // [ 0 0 1 ]'   to direction p1-p2
      //
      //
      double m[3],p[3];
      
      double base[3]; pts->GetPoint(0,base);
      double top[3];  pts->GetPoint(1,top);
      //	  fprintf(stderr,"base=%.2f,%.2f,%.2f\n",base[0],base[1],base[2]);
      //  fprintf(stderr,"top=%.2f,%.2f,%.2f\n",top[0],top[1],top[2]);

      
      double n[3];
      for (int i=0;i<=2;i++)
	{
	  // m is along z-axis
	  m[i]=base[i]-top[i];
	  // n[i] is a random vector not parallel to m -- essentially rotation about
	  // y-axis is a free parameter!
	  n[i]=m[i]+0.2*double(i);
	}
      

      // n is fake y-axis
      // m is real z-axis
      //
      // nxm = p (x-axis)
      // mxp = n

      //      n[0]=0.0; n[1]=1.0; n[2]=0.0;
      double dist=vtkMath::Normalize(n);      vtkMath::Normalize(m);
      vtkMath::Cross(n,m,p);      vtkMath::Normalize(p); 
      // --> p is perpendicular to both n and m
      vtkMath::Cross(m,p,n);      vtkMath::Normalize(n); 
      // --> m is perpendicular to both p and n
      // Now p n and m are a right handed orthogonal set
      
      /*      p[0]=1.0;  p[1]=0.0;      p[2]=0.0;
      n[0]=0.0;  n[1]=1.0;      n[2]=0.0;
      m[0]=0.0;  m[1]=0.0;      m[2]=1.0;*/
     
	


      transform[0] = p[0];     transform[3] = n[0];     transform[6] = m[0];   transform[9] =  base[0];
      transform[1] = p[1];     transform[4] = n[1];     transform[7] = m[1];   transform[10] = base[1];  
      transform[2] = p[2];     transform[5] = n[2];     transform[8] = m[2];   transform[11] = base[2];

      //      fprintf(stderr," [ %.2f %.2f %.2f %.2f ]\n",transform[0],transform[3],transform[6],transform[9]);
      //      fprintf(stderr," [ %.2f %.2f %.2f %.2f ]\n",transform[1],transform[4],transform[7],transform[10]);
      //      fprintf(stderr," [ %.2f %.2f %.2f %.2f ]\n",transform[2],transform[5],transform[8],transform[11]);
    }

 

  igtl_transform_convert_byte_order(transform);  /* convert endian if necessary */
 
  /********** general header **********/  
  igtl_header header;
  igtl_uint64 crc = crc64(0, 0, 0LL);          /* initial crc */
  header.version   = IGTL_HEADER_VERSION;
  header.timestamp = 0;
  header.body_size = IGTL_TRANSFORM_SIZE;
  header.crc       = crc64((unsigned char*)transform, IGTL_TRANSFORM_SIZE, crc);
  
  strncpy(header.name, "TRANSFORM", 12);       /* Device Type: should be "TRANSFORM" */
  strncpy(header.device_name, this->ToolName, 20);    /* Device name */
  
  igtl_header_convert_byte_order(&header);   /* convert endian if necessary */

  //  Then send package (in case of BSD socket)
  this->Connector->Send((void*) &header, IGTL_HEADER_SIZE);
  this->Connector->Send((void*) transform, IGTL_TRANSFORM_SIZE);
  return 1;
}
// ---------------------------------------------------------------------------
int vtkpxOpenIGTLinkClient::SendImage(vtkImageData* img)
{
  if (this->IsConnected==0  || this->Connector==NULL)
    return 0;

  if (img==NULL)
    return 0;

  enum {    COORDINATE_LPS,    COORDINATE_RAS  };
  enum {    ENDIAN_BIG,    ENDIAN_LITTLE  };
  enum {    DTYPE_SCALAR = 1,    DTYPE_VECTOR = 2  };

  // This matches VTK -- see vtkType.h !
  //#define VTK_CHAR            2
  //#define VTK_UNSIGNED_CHAR   3
  //#define VTK_SHORT           4
  //#define VTK_UNSIGNED_SHORT  5
  //#define VTK_INT             6
  //#define VTK_UNSIGNED_INT    7

  enum {    TYPE_INT8   = 2,    TYPE_UINT8  = 3,    TYPE_INT16  = 4,    TYPE_UINT16 = 5,    TYPE_INT32  = 6,    TYPE_UINT32 = 7  };

  int dimensions[3]; img->GetDimensions(dimensions);
  int imagesize=dimensions[0]*dimensions[1]*dimensions[2];

  int s = IGTL_IMAGE_HEADER_SIZE + imagesize;
  unsigned char* m_ImageHeader = new unsigned char [s];
  unsigned char* m_Body   = &m_ImageHeader[IGTL_IMAGE_HEADER_SIZE];
  int m_PackSize = s;

  igtl_image_header* image_header = (igtl_image_header*)m_ImageHeader;
  image_header->version     = IGTL_IMAGE_HEADER_VERSION;
  image_header->data_type   = DTYPE_SCALAR;

  // We are focing everything to UINT8 ... see scaling below! (VVLink is almost always UINT8)
  image_header->scalar_type = TYPE_UINT8;
  image_header->endian      = ENDIAN_BIG;
  image_header->coord       = COORDINATE_RAS;
  image_header->size[0]     = dimensions[0];
  image_header->size[1]     = dimensions[1];
  image_header->size[2]     = dimensions[2];
  image_header->subvol_offset[0] = 0;
  image_header->subvol_offset[1] = 0;
  image_header->subvol_offset[2] = 0;
  image_header->subvol_size[0] = dimensions[0];
  image_header->subvol_size[1] = dimensions[1];
  image_header->subvol_size[2] = dimensions[2];

  float origin[3], norm_i[3], norm_j[3], norm_k[3], spacing[3];

  

  for (int i = 0; i < 3; i ++) 
    {
      norm_i[i] = -1.0*float (i==0);
      norm_j[i] = -1.0*float (i==1);
      norm_k[i] = 1.0*float (i==2);
      spacing[i] = float(img->GetSpacing()[i]);
      origin[i] = 0.0;//(float(dimensions[i]-1)*0.5*spacing[i]);
      //      if (i==0 || i==2)
      //	origin[i]=-origin[i];
    }


  // First Header
  igtl_image_set_matrix(spacing, origin,
                        norm_i, norm_j, norm_k,
                        image_header);

  /*  fprintf(stderr,"****************\nSpacing = %.3f %.3f %.3f\n",spacing[0],spacing[1],spacing[2]);
      fprintf(stderr,"Origin  = %.3f %.3f %.3f\n",origin[0],origin[1],origin[2]);
      fprintf(stderr,"Norm_i  = %.3f %.3f %.3f\n",norm_i[0],norm_i[1],norm_i[2]);
      fprintf(stderr,"Norm_j  = %.3f %.3f %.3f\n",norm_j[0],norm_j[1],norm_j[2]);
      fprintf(stderr,"Norm_k  = %.3f %.3f %.3f\n",norm_k[0],norm_k[1],norm_k[2]);*/

  igtl_image_convert_byte_order(image_header);

  
  // Next Data

  vtkDataArray* dat=img->GetPointData()->GetScalars();
  int nt=dat->GetNumberOfTuples();
  double r[2]; dat->GetRange(r);
  double scale=255.0/(r[1]-r[0]);
  if (scale>=1.0)
    scale=1.0;
  double shift=r[0];

  for (int i=0;i<nt;i++)
    m_Body[i]=(unsigned char)(scale*dat->GetComponent(i,0)+shift);
  

  // 

  //  Sending
  //  First the IMAGE_HEADER
  //
  igtl_header header;
  igtl_uint64 crc = crc64(0, 0, 0LL);          /* initial crc */
  header.version   = IGTL_HEADER_VERSION;
  header.timestamp = 0;
  header.body_size = IGTL_IMAGE_HEADER_SIZE+imagesize;

  header.crc       = crc64(m_ImageHeader, IGTL_IMAGE_HEADER_SIZE+imagesize, crc);
  strncpy(header.name, "IMAGE", 12);       /* Device Type: should be "TRANSFORM" */
  strncpy(header.device_name, "VVLinkImage", 20);    /* Device name */
  igtl_header_convert_byte_order(&header);   /* convert endian if necessary */

  //  Then send IGTL Header (in case of BSD socket)
  this->Connector->Send((void*) &header, IGTL_HEADER_SIZE);

  // This sends image header + image
  this->Connector->Send((void*) m_ImageHeader, IGTL_IMAGE_HEADER_SIZE+imagesize);

  delete [] m_ImageHeader;
  return 1;
}

