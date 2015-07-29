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

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxImageAddGaussianNoise.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:02:52 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
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
#include "vtkObjectFactory.h"
#include "vtkpxImageAddGaussianNoise.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "math.h"


//------------------------------------------------------------------------------
vtkpxImageAddGaussianNoise* vtkpxImageAddGaussianNoise::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageAddGaussianNoise");
  if(ret)
    {
    return (vtkpxImageAddGaussianNoise*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageAddGaussianNoise;
}

// Construct object with no children.
vtkpxImageAddGaussianNoise::vtkpxImageAddGaussianNoise()
{
  this->NoiseLevel=10.0;
  this->ForcePositive=0;
}
// ----------------------------------------------------------------------------
vtkpxImageAddGaussianNoise::~vtkpxImageAddGaussianNoise()
{
}
// --------------------------------------------------------------------------------------
float vtkpxImageAddGaussianNoise::GaussianRandom()
{
  static int iset=0;
  static float gset;
  float fac,rsq,v1,v2;

  if  (iset == 0) 
    {
      do 
	{
	  v1=2.0*vtkMath::Random()-1.0;
	  v2=2.0*vtkMath::Random()-1.0;
	  rsq=v1*v1+v2*v2;
	} 
      while (rsq >= 1.0 || rsq == 0.0);
      
      fac=sqrt(-2.0*log(rsq)/rsq);
      gset=v1*fac;
      iset=1;
      return v2*fac;
    } 
  else
    {
      iset=0;
      return gset;
    }
}
// --------------------------------------------------------------------------------------
// New Code 
// --------------------------------------------------------------------------------------
void vtkpxImageAddGaussianNoise::SimpleExecute(vtkImageData* input,vtkImageData *output)
{
  output->DeepCopy(input);

  vtkDataArray* in =input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();

  int numtuples=out->GetNumberOfTuples();
  int numframes=out->GetNumberOfComponents();

  int tenth=int(numtuples/10);
  int count=0;

  this->UpdateProgress(0.02);

  for (int vx=0;vx<numtuples;vx++)
    {
      for (int fr=0;fr<numframes;fr++)
	{
	  float vl=in->GetComponent(vx,fr);
	  vl=vl+this->NoiseLevel*this->GaussianRandom();
	  if (this->ForcePositive && vl<=0)
	    vl=0.0;
	  out->SetComponent(vx,fr,vl);
	}
      ++count;
      if (count==tenth)
	{
	  this->UpdateProgress(float(vx)/float(numtuples));
	  count=0;
	}
    }
  this->UpdateProgress(1.0);
}

