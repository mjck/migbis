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

  Program:   vtknrcontrib library file
  Module:    $RCSfile: vtknrImageCluster.h,v $
  Language:  C++
  Date:      $Date: 2003/07/25 10:59:12 $
  Version:   $Revision: 1.0 $


Copyright (c) 2003 Nallakkandi Rajeevan, Ph.D. n.rajeevan@yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef __vtknrImageCluster_h
#define __vtknrImageCluster_h

#include "vtkIntArray.h"
#include "vtkpxSimpleImageToImageFilter.h"

class  vtknrImageCluster : public vtkpxSimpleImageToImageFilter
{
public:

  static vtknrImageCluster *New();
  vtkTypeMacro(vtknrImageCluster, vtkpxSimpleImageToImageFilter);


  // Description:
  // This is the intensity Threshold
  vtkGetMacro(IntensityThreshold, float);
  vtkSetMacro(IntensityThreshold, float);
  
  // This is the connectivity of the cluster
  vtkGetMacro(OneConnected, int);
  vtkSetClampMacro(OneConnected, int,0,1);

  // This is the connectivity of the cluster
  vtkGetMacro(CubeConnected, int);
  vtkSetClampMacro(CubeConnected, int,0,1);
    
  // This is the Maximum Cluster Size
  vtkGetMacro(MaxClusterSize, int);
  vtkSetMacro(MaxClusterSize, int);

  // This is the Double Array for storing clusters
  vtkSetObjectMacro(Clusters,vtkIntArray);
  vtkGetObjectMacro(Clusters,vtkIntArray);

  // This is the Double Array for storing clusters
  vtkSetObjectMacro(ClusterHistogram,vtkIntArray);
  vtkGetObjectMacro(ClusterHistogram,vtkIntArray);

  // This is the Cluster Number Image
  vtkGetObjectMacro(ClusterNumberImage, vtkImageData);

  // Description:
  // Get Cluster Centroids -- image has values 0..N where N=number of clusters
  static vtkImageData* ColorInClusterCentroids(vtkImageData* clustermap);

  // Description:
  // Added by XP May 5th 2008
  // Goal of this function is to have surviving clusters numbered 1..N
  static int RenumberClusterImage(vtkImageData* img,int sizethreshold);
  


protected:

   vtknrImageCluster();
   virtual ~vtknrImageCluster();
   vtknrImageCluster(const vtknrImageCluster&) {};
   void operator=(const vtknrImageCluster&) {};

  virtual int RequestInformation(vtkInformation *vtkNotUsed(request),  vtkInformationVector **inputVector, 
				 vtkInformationVector *outputVector);
  virtual void SimpleExecute(vtkImageData *inData, vtkImageData *outData);


  int    OneConnected;
  int    CubeConnected;
  int    MaxClusterSize;
  float  IntensityThreshold;


  vtkIntArray*  Clusters;
  vtkIntArray*  ClusterHistogram;
  vtkImageData* ClusterNumberImage;

  // Description:
  // Method to Integrate Clusters
  virtual int IntegrateHistogram();
  virtual int ComputeClusters(vtkImageData* input,vtkImageData* output);
  virtual int ComputeOutputImage(vtkImageData* clusternumber,vtkImageData* output);
};
#endif

