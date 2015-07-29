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
  Module:    $RCSfile: vtkpxAbstractRPMRegistration.cpp,v $
  Language:  C++
  Date:      $Date: 2003/03/06 16:34:20 $
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

#include "vtkpxAbstractRPMRegistration.h"
#include "vtkObjectFactory.h"
//-----------------------------------------------------------------------------------
vtkpxAbstractRPMRegistration* vtkpxAbstractRPMRegistration::New()
{
  // First try to create the object from the vtkObjectFactory

  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxAbstractRPMRegistration");
  if(ret)
    {
      return (vtkpxAbstractRPMRegistration*)ret;
    }

  // If the factory was unable to create the object, then create it here.

  return new vtkpxAbstractRPMRegistration;
}

//----------------------------------------------------------------------------

vtkpxAbstractRPMRegistration::vtkpxAbstractRPMRegistration()
{

  this->CurrentlyUpdating=0;
  this->InitialTemperature=0.2;
  this->FinalTemperature=0.2/100;
  this->AnnealRate=0.93;
  this->InitialSmoothnessFactor=1.0;
  this->FinalSmoothnessFactor=0.1;
  this->UseSymmetricCorrespondence=0;
  this->SymmetryWeight=0.25;
}

//----------------------------------------------------------------------------

void vtkpxAbstractRPMRegistration::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkpxRPMCorrespondenceFinder::PrintSelf(os,indent);

  os << indent << "AnnealRate: " << this->AnnealRate << "\n";
  os << indent << "InitialTemperature: " << this->InitialTemperature << "\n";
  os << indent << "FinalTemperature: " << this->FinalTemperature << "\n";
  os << indent << "InitialSmoothnessFactor: " << this->InitialSmoothnessFactor << "\n";
  os << indent << "FinalSmoothnessFactor: " << this->FinalSmoothnessFactor << "\n";
  
}

//----------------------------------------------------------------------------

void vtkpxAbstractRPMRegistration::Run()
{
  // Check source, target
  if (this->CurrentlyUpdating==1)
    {
      return;
    }

  if (this->Source == NULL || !this->Source->GetNumberOfPoints())
    {
      vtkErrorMacro(<<"Can't execute with NULL or empty input");
      return;
    }
  
  if (this->Target == NULL || !this->Target->GetNumberOfPoints())
    {
      vtkErrorMacro(<<"Can't execute with NULL or empty target");
      return;
    }

  this->AbortExecute=0;
  this->CurrentlyUpdating=1;
  if (this->UseSymmetricCorrespondence)
    this->SymmetricAnneal();
  else
    this->Anneal();
  this->CurrentlyUpdating=0;
}

void vtkpxAbstractRPMRegistration::SymmetricAnneal()
{
  vtkErrorMacro(<<"No Symmetric Anneal Defined! Running Standard Annealing\n");
  this->UseSymmetricCorrespondence=0;
  this->Anneal();
  this->UseSymmetricCorrespondence=1;
}

void vtkpxAbstractRPMRegistration::Anneal()
{
  vtkErrorMacro(<<"No  Anneal Defined! \n");
}

//----------------------------------------------------------------------------
vtkAbstractTransform* vtkpxAbstractRPMRegistration::GetCurrentTransformation()
{
  return NULL;
}
//----------------------------------------------------------------------------

