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


#include "vtkObjectFactory.h"
#include "vtkpxGenericUndoStack.h"
// ---------------------------------------------------------------------------------------------
vtkpxGenericUndoStack::vtkpxGenericUndoStack()
{
  this->CurrentIndex=-1;
  this->CurrentUndoIndex=-1;
  this->CurrentRedoIndex=-1;
  this->MaxIndex=-1;
  this->NumberOfElements=0;

  this->CurrentlyUndoing=0;
  this->CurrentlyRedoing=0;


  for (int i=0;i<pxGenericArrayMAXSIZE;i++)
    Elements[i]=NULL;
}
/* -------------------------------------------------------------------------*/
vtkpxGenericUndoStack::~vtkpxGenericUndoStack()
{
  for (int i=0;i<pxGenericArrayMAXSIZE;i++)
    if (Elements[i]!=NULL)
      Elements[i]->Delete();

}
// ---------------------------------------------------------------------------------------------
void vtkpxGenericUndoStack::Initialize(int numelements)
{
  if (numelements<10)
    numelements=10;
  else if (numelements>pxGenericArrayMAXSIZE)
    numelements=pxGenericArrayMAXSIZE;

  for (int i=0;i<pxGenericArrayMAXSIZE;i++)
    if (Elements[i]!=NULL)
      {
	Elements[i]->Delete();
	Elements[i]=NULL;
      }

  this->NumberOfElements=numelements;

  for (int i=0;i<this->NumberOfElements;i++)
    {
      this->Elements[i]=this->CreateNewElement();
      this->InitializeElement(i);
    }

  this->CurrentIndex=-1;
  this->MaxIndex=-1;

  this->CurrentUndoIndex=-1;
  this->CurrentRedoIndex=-1;
  this->CurrentlyUndoing=0;
  this->CurrentlyRedoing=0;

}
// ---------------------------------------------------------------------------------------------
void vtkpxGenericUndoStack::Reset()
{
  this->CurrentIndex=-1;
  this->CurrentUndoIndex=-1;
  this->CurrentRedoIndex=-1;
  this->MaxIndex=-1;
  this->CurrentlyUndoing=0;
  this->CurrentlyRedoing=0;


  for (int i=0;i<this->NumberOfElements;i++)
    this->Initialize(i);
}

// ---------------------------------------------------------------------------------------------
vtkObject* vtkpxGenericUndoStack::GetCurrentObject()
{
  if (this->CurrentIndex<0)
    return this->GetNextObject();

  //fprintf(stderr,"\t\t\t Current Object = %d\n",CurrentIndex);
  this->CurrentUndoIndex=-1;
  this->CurrentRedoIndex=-1;
  this->CurrentlyUndoing=0;
  this->CurrentlyRedoing=0;


  return this->Elements[CurrentIndex];
}

vtkObject* vtkpxGenericUndoStack::GetNextObject()
{
  if (this->CurrentIndex<-1)
    this->CurrentIndex=-1;
  else if (this->CurrentIndex==this->NumberOfElements-1)
    this->ShuffleElements();

  this->CurrentIndex+=1;
  this->CurrentUndoIndex=-1;
  this->CurrentRedoIndex=-1;
  this->CurrentlyUndoing=0;
  this->CurrentlyRedoing=0;
  
  this->MaxIndex=this->CurrentIndex;

  //fprintf(stderr,"\n ************************ Next Object = %d (emptying)\n",CurrentIndex);

  this->InitializeElement(this->CurrentIndex);
  return this->Elements[this->CurrentIndex];
}


vtkObject* vtkpxGenericUndoStack::GetNextUndoObject()
{
  if (this->CurrentlyUndoing==0)
    {
      this->CurrentlyRedoing=0;
      if (this->CurrentIndex>=0)
	{
	  this->CurrentUndoIndex=this->CurrentIndex;
	  //fprintf(stderr,"Returning Undo Object %d\n",this->CurrentUndoIndex);
	  this->CurrentlyUndoing=1;
	  return Elements[this->CurrentUndoIndex];
	}
      this->CurrentlyUndoing=0;
      //fprintf(stderr,"Returning NULL Undo Object\n");
      return NULL;
    }

  // From Here on Currently Undoing = 1
  this->CurrentlyRedoing=0;
  if (this->CurrentUndoIndex>0)
    {
      this->CurrentUndoIndex-=1;
      this->CurrentIndex=this->CurrentUndoIndex;
      //fprintf(stderr,"Returning Undo Object %d\n",this->CurrentUndoIndex);
      return Elements[this->CurrentUndoIndex];
    }
  
  //fprintf(stderr,"Returning NULL Undo Object\n");
  return NULL;
}

vtkObject* vtkpxGenericUndoStack::GetNextRedoObject()
{
  if (this->CurrentlyRedoing==0)
    {
      if (this->CurrentlyUndoing==0)
	{
	  //fprintf(stderr,"Returning NULL Redo Object Not Undoing\n");
	  return NULL;
	}
      
      this->CurrentlyUndoing=0;
      if (this->CurrentIndex>=0)
	{
	  this->CurrentRedoIndex=this->CurrentIndex;
	  //fprintf(stderr,"Returning Redo Object %d\n",this->CurrentUndoIndex);
	  this->CurrentlyRedoing=1;
	  return Elements[this->CurrentRedoIndex];
	}
      this->CurrentlyRedoing=0;
      //    //fprintf(stderr,"Returning NULL Redo Object\n");
      return NULL;
    }


  // From Here on Currently Redoing = 1
  this->CurrentlyUndoing=0;
  this->CurrentRedoIndex+=1;
  if (this->CurrentRedoIndex<= this->MaxIndex)
    {
	this->CurrentIndex=this->CurrentRedoIndex;
	//fprintf(stderr,"Returning Redo Object %d\n",this->CurrentUndoIndex);
	return Elements[this->CurrentRedoIndex];
    }
  
  //fprintf(stderr,"Returning NULL Redo Object\n");
  return NULL;
}
// ---------------------------------------------------------------------------------------------

void vtkpxGenericUndoStack::ShuffleElements()
{
  if (this->CurrentIndex!=this->NumberOfElements-1)
    return;

  int shift=1;
  //fprintf(stderr,"Shuffling Elements %d\n",shift);

  for (int ia=shift;ia<this->NumberOfElements;ia++)
    {
      vtkObject* tmp=this->Elements[ia-shift];
      this->Elements[ia-shift]=this->Elements[ia];
      this->Elements[ia]=tmp;
    }
  
  this->CurrentIndex-=shift;
  if (this->CurrentUndoIndex>=shift)
    this->CurrentUndoIndex-=shift;
  else
    this->CurrentUndoIndex=-1;

  this->CurrentUndoIndex=-1;
  
  if (this->CurrentRedoIndex>=0)
    this->CurrentRedoIndex-=shift;
  else
    this->CurrentRedoIndex=-1;

  this->MaxIndex-=shift;
  this->CurrentlyUndoing=0;
  this->CurrentlyRedoing=0;

}




