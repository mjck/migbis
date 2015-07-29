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





/*
 * File: OmsVoxelStack.cpp
 * Author: Skrinjar Oskar
 * Date: October 19, 1998
 * Project: Surgical Navigation System
 * -----------------------------------------------
 * This file is the implementation of the OmsVoxelStack class.
 */

#include "vtkpxBrainPreSegment.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkDataArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkPointData.h"

// INCLUDE FILES
#include <stdio.h>
#include <stdlib.h>

//***********************************************************************
//   This came via Oskar's Code 
//***********************************************************************
/*
 * File: OmsVoxelStack.h
 * Author: Skrinjar Oskar
 * Date: October 19, 1998
 * Project: Surgical Navigation System
 * ------------------------------------------------------------
 * This file is the declaration of the OmsVoxelStack class.
 * It is a stack of voxel positions.
 */

// new data types
struct OmsVoxelStackEntry {
  int x, y, z;
  struct OmsVoxelStackEntry *previous;
  struct OmsVoxelStackEntry *next;
};


/*
 * CLASS: OmsVoxelStack
 */

class OmsVoxelStack {

private:

  OmsVoxelStackEntry *first, *last;

  void EmptyStack(void);

public:

  OmsVoxelStack(void); // constructor
  ~OmsVoxelStack();  // destructor

  void Push(int x, int y, int z);
  int Pop(int &x, int &y, int &z);
  int IsEmpty(void) { if (first == NULL) return 1;  return 0; }
  void SetEmpty(void);

};

//***********************************************************************
/*
 * Public method: OmsVoxelStack
 * Usage: OmsVoxelStack stack;
 * -----------------------------------------------------
 * This method is a constructor of the OmsVoxelStack class.
 */

OmsVoxelStack::OmsVoxelStack(void)
{
  first = NULL;
  last = NULL;
}


/*
 * Public method: ~OmsVoxelStack
 * -------------------------------------------------------
 * This method is the destructor of the OmsVoxelStack class.
 */

OmsVoxelStack::~OmsVoxelStack()
{
  EmptyStack();
}


/*
 * Public method: Push
 * Usage: stack.Push(x, y, z);
 * ------------------------------------------------------
 * This method pushes an element to the stack.
 */

void OmsVoxelStack::Push(int x, int y, int z)
{
  OmsVoxelStackEntry *tmp = new OmsVoxelStackEntry;
  if (tmp == 0)
      {
	printf("OmsVoxelStack::Puch:  Not enough memory !\n");
	exit(0);
      }
  //  printf("Pusing %d %d %d\n",x,y,z);
  // set the entry elements
  tmp->x = x;
  tmp->y = y;
  tmp->z = z;
  tmp->next = NULL;
  tmp->previous = last;

  // link it to the stack
  if (first == NULL)
      {
	first = tmp;
	last = tmp;
      }
  else
      {
	last->next = tmp;
	last = tmp;
      }
}


/*
 * Public method: Pop
 * Usage: if (stack.Pop(x,y,z) == 1) ...
 * -----------------------------------------------------------------
 * This method pops the last element from the stack and returns 1.
 * If there are no elements 0 is returned.
 */

int OmsVoxelStack::Pop(int &x, int &y, int &z)
{
  if (first == NULL) return 0;

  x = last->x;
  y = last->y;
  z = last->z;

  if (last->previous == NULL)
      {
	delete last;
	first = NULL;
	last = NULL;
      }
  else
      {
	last = last->previous;
	delete last->next;
	last->next = NULL;
      }

  return 1;
}


/*
 * Public method: SetEmpty
 * Usage: Stack.SetEmpty();
 * -----------------------------------------------------
 * This method reinitializes the stack to its initial state, i.e.,
 * deletes all the elements in the stack.
 */

void OmsVoxelStack::SetEmpty(void)
{
  EmptyStack();
  first = NULL;
  last = NULL;
}


/*
 * Private method: EmptyStack
 * Usage: EmptyStack();
 * --------------------------------------------------------------
 * This method deletes all of the stack elements.
 */

void OmsVoxelStack::EmptyStack(void)
{
  OmsVoxelStackEntry *tmp = first, *tmp_del;

  // delete entry by entry
  while (tmp != NULL)
      {
	tmp_del = tmp;
	tmp = tmp->next;
	delete tmp_del;
      }
}


//***********************************************************************
//   This also came via Oskar's Code 
//***********************************************************************
/*
 * Private method: BrainTissueAlgorithm
 * Usage: BrainTissueAlgorithm(x_seed, y_seed, z_seed)
 * ---------------------------------------------------------------------------
 * This algorithm performs the brain tissue segmentation.
 */

void   BrainTissueAlgorithm(vtkpxBrainPreSegment* self,
			    int seed[3],
			    float LowThreshold,
			    float HighThreshold,
			    vtkImageData* input,
			    vtkImageData* output,
			    int erode,
			    int regrow,
			    int geodilate,
			    int numerode)
		  
{
  /*printf("\nBrain tissue segmentation:  seed=(%d,%d,%d), levels=%f:%f\n",
    seed[0],seed[1],seed[2],LowThreshold,HighThreshold);*/
  
  // sizes of the bounding box

  int dim[3];
  input->GetDimensions(dim);

  int xsize = dim[0];
  int ysize = dim[1];
  int zsize = dim[2];

  int slicesize=xsize*ysize;
  int volsize=slicesize*zsize;

  unsigned char *data1 = NULL;
  data1 = new unsigned char [volsize];
  if (data1 == 0)
      {
	printf("BrainTissueAlgorithm:  Not enough memory !\n");
	return;
      }

  vtkDataArray* outScalars=output->GetPointData()->GetScalars();
  unsigned char* data2=((vtkUnsignedCharArray *)outScalars)->WritePointer(0, volsize);
  
  // THRESHOLDING
  //printf("Double thresholding ... ");
  vtkDataArray* inScalars=input->GetPointData()->GetScalars();
  int x, y, z;
  
  for (int ia = 0; ia < volsize; ia++) 
    {
      float v=inScalars->GetComponent(ia,0);
      if (v>=LowThreshold && v<=HighThreshold)
	{
	  data1[ia] = 1;
	  data2[ia] = 1;
	}
      else
	{
	  data1[ia] = 0;
	  data2[ia] = 0;
	}
    }

  //  printf(" Done !\n");
  self->UpdateProgress(0.2);

  int n;

  if (erode==1)
    {
      // Erosion
      //      printf("Erosion: ");
      for (n = 0; n < numerode; n++)
	{
	  //	  printf(" %d ", n);
	  for (x = 1; x < xsize-1; x++) 
	    for (y = 1; y < ysize-1; y++) 
	      for (z = 1; z < zsize-1; z++) 
		{
		  int index=x+y*xsize+z*slicesize;
		  if (data1[index] == 1)
		    {
		      if ( (data1[index-1] == 0) || 
			   (data1[index+1] == 0) || 
			   (data1[index+xsize] == 0) || 
			   (data1[index-xsize] == 0) || 
			   (data1[index+slicesize] == 0) || 
			   (data1[index-slicesize] == 0) ) 
			data2[index] = 0;
		    }
		}
	  for (int i=0;i<volsize;i++)
	    data1[i]=data2[i];
	  if (numerode>1)
	    self->UpdateProgress(float(n+1)/float(numerode-1)*0.3+0.1);
	}
      //printf("Done\n");
      
    }
  

  if (regrow==1)
    {
      // REGROW
      //      printf("Regrow .... ");
      
      int xc = seed[0];
      int yc = seed[1];
      int zc = seed[2];
      
      OmsVoxelStack stack;
      stack.Push(xc,yc,zc);
      int index=xc+yc*xsize+zc*slicesize;
      data2[index] = 2;

      int iter=0;
      
      while ( stack.Pop(xc, yc, zc) == 1 )
	{
	  iter++;
	  int index=xc+yc*xsize+zc*slicesize;
	  if (xc > 0)
	    {
	      if ( data2[index-1] == 1 )
		{
		  data2[index-1] = 2;
		  stack.Push(xc-1, yc, zc);
		}
	    }
	  
	  if (xc < xsize-1)
	    {
	      if ( data2[index+1] == 1 )
		{
		  data2[index+1] = 2;
		  stack.Push(xc+1, yc, zc);
		}
	    }
	  
	  if (yc > 0)
	    {
	      if ( data2[index-xsize] == 1 )
		{
		  data2[index-xsize] = 2;
		  stack.Push(xc, yc-1, zc);
		}
	    }
	  
	  if (yc < ysize-1)
	    {
	      if ( data2[index+xsize] == 1 )
		{
		  data2[index+xsize] = 2;
		  stack.Push(xc, yc+1, zc);
		}
	    }
	  
	  if (zc > 0)
	    {
	      if ( data2[index-slicesize] == 1 )
		{
		  data2[index-slicesize] = 2;
		  stack.Push(xc, yc, zc-1);
		}
	    }
	  
	  if (zc < zsize-1)
	    {
	      if ( data2[index+slicesize] == 1 )
		{
		  data2[index+slicesize] = 2;
		  stack.Push(xc, yc, zc+1);
		}
	    }
	}
      
      //      printf("Done Regrow iter=%d\n",iter);

      if (iter>2)
	{
	  //printf("Reset ... ");
	  for (int index = 0; index < volsize; index++) 
	    {
	      if ( data2[index] == 2) 
		data2[index] = 1;
	      else 
		data2[index] = 0;
	    }
	  //printf(" Done !\n");
	}
      else
	{
	  printf("****** Bad Seed No Regrow  will be attempted! ***** \n");
	}

      self->UpdateProgress(0.6);
    }

  // GEODILATE
  if (geodilate)
    {
      //printf("Geodilate in process ...");
      for (n = 0; n < numerode+1; n++)
	{
	  //printf(" %d", n);
	  
	  for (x = 1; x < xsize-1; x++) 
	    for (y = 1; y < ysize-1; y++) 
	      for (z = 1; z < zsize-1; z++) 
		{
		  int index=x+y*xsize+z*slicesize;
		  if ( data2[index] == 1 )
		    {
		      if ( (data2[index-1]==0) && (data1[index-1]==1) ) 
			data2[index-1] = 2;
		      //index = INDEX(x+1,y,z);
		      if ( (data2[index+1]==0) && (data1[index+1]==1) ) 
			data2[index+1] = 2;
		      //index = INDEX(x,y-1,z);
		      if ( (data2[index-xsize]==0) && (data1[index-xsize]==1) ) 
			data2[index-xsize] = 2;
		      //index = INDEX(x,y+1,z);
		      if ( (data2[index+xsize]==0) && (data1[index+xsize]==1) ) 
			data2[index+xsize] = 2;
		      //index = INDEX(x,y,z-1);
		      if ( (data2[index-slicesize]==0) && (data1[index-slicesize]==1) ) 
			data2[index-slicesize] = 2;
		      //index = INDEX(x,y,z+1);
		      if ( (data2[index+slicesize]==0) && (data1[index+slicesize]==1) ) 
			data2[index+slicesize] = 2;
		    }
		}
	  
	  for (int i=0;i<volsize;i++)
	    {
	      if ( data2[i] == 2 ) 
		data2[i] = 1;
	    }

	  self->UpdateProgress(0.6+float(n+1)/float(numerode)*0.4);
	}
      //printf("\n");
    }
  
  // free the occupied memory
  if (data1 != NULL) 
    delete [] data1;
  

      self->UpdateProgress(1.0);
  //  printf("Brain segmentation algorithm completed !\n");

}



vtkpxBrainPreSegment* vtkpxBrainPreSegment::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxBrainPreSegment");
  if(ret)
      {
	return (vtkpxBrainPreSegment*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxBrainPreSegment;
}


// Construct object to extract all of the input data.
vtkpxBrainPreSegment::vtkpxBrainPreSegment()
{
  this->LowThreshold=0.0;
  this->HighThreshold=255.0;
  this->Erode=1;
  this->Regrow=1;
  this->Geodilate=1;
  this->NumErosions=2;
  for (int i=0;i<=2;i++)
    this->Seed[i]=0;
}

void vtkpxBrainPreSegment::SimpleExecute(vtkImageData *input,vtkImageData *output)
{  
  vtkDebugMacro(<< "Extracting Slice");

  int dim[3];   input->GetDimensions(dim);
  double ori[3]; input->GetOrigin(ori);
  double sp[3];  input->GetSpacing(sp);

  output->SetSpacing(sp);
  output->SetScalarType(VTK_UNSIGNED_CHAR);
  output->SetNumberOfScalarComponents(1);
  output->SetOrigin(ori);
  output->SetDimensions(dim);
  output->SetWholeExtent(0,dim[0]-1,
			 0,dim[1]-1,
			 0,dim[2]-1);
  output->AllocateScalars();
  BrainTissueAlgorithm(this,
		       this->Seed,
		       this->LowThreshold,
		       this->HighThreshold,
		       input,output,
		       this->Erode,
		       this->Regrow,
		       this->Geodilate,
		       this->NumErosions);
}


void vtkpxBrainPreSegment::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkpxSimpleImageToImageFilter::PrintSelf(os,indent);

  os << indent << "  Seed " << this->Seed[0] << ", "<< this->Seed[1] << ", " << this->Seed[2]  << "\n";
  os << indent << "  Thresholds " << this->LowThreshold << ", " << this->HighThreshold << "\n";
}




