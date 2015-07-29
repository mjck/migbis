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
 * vtkbisDijkstraGraphGeodesicPath.cpp
 *
 *  Created on: Apr 29, 2010
 *      Author: Isabella Murphy, Xilin Shen
 *      We modified the vtkDijkstraGraphGeodesicPath in vtk5.2, 2 modifications:
 *      1. the edge weight is always 1
 *      2. The condition that there is no path between the given vertices is considered
 */
/*=========================================================================

  Program:   Visualization Toolkit
  Module:  $RCSfile: vtkDijkstraGraphGeodesicPath.cxx,v $
  Language:  C++
  Date:    $Date: 2008-07-21 19:34:55 $
  Version:   $Revision: 1.6.2.1 $

  Made by Rasmus Paulsen
  email:  rrp(at)imm.dtu.dk
  web:    www.imm.dtu.dk/~rrp/VTK

  This class is not mature enough to enter the official VTK release.
=========================================================================*/
#include "vtkbisDijkstraGraphGeodesicPath.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkFloatArray.h"
#include "vtkObjectFactory.h"
#include "vtkExecutive.h"
#include "vtkMath.h"
#include "vtkIdList.h"
#include "vtkIntArray.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkCellArray.h"
vtkbisDijkstraGraphGeodesicPath* vtkbisDijkstraGraphGeodesicPath::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisDijkstraGraphGeodesicPath");
  if(ret)
    {
      return (vtkbisDijkstraGraphGeodesicPath*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisDijkstraGraphGeodesicPath;
}
//----------------------------------------------------------------------------
vtkbisDijkstraGraphGeodesicPath::vtkbisDijkstraGraphGeodesicPath()
{
  this->IdList = vtkIdList::New();
  this->d    = vtkFloatArray::New();
  this->pre  = vtkIntArray::New();
  this->f    = vtkIntArray::New();
  this->s    = vtkIntArray::New();
  this->Heap = vtkIntArray::New();
  this->p    = vtkIntArray::New();
  this->HeapSize  = 0;
  this->StartVertex = 0;
  this->EndVertex   = 0;
  this->StopWhenEndReached = 0;
  this->UseScalarWeights = 0;
  this->Adjacency = NULL;
  this->NumberOfVertices = 0;
  this->AdjacencyGraphSize = 0;
}

//----------------------------------------------------------------------------
vtkbisDijkstraGraphGeodesicPath::~vtkbisDijkstraGraphGeodesicPath()
{
  if (this->IdList)
    {
    this->IdList->Delete();
    }
  if (this->d)
    {
    this->d->Delete();
    }
  if (this->pre)
    {
    this->pre->Delete();
    }
  if (this->f)
    {
    this->f->Delete();
    }
  if (this->s)
    {
    this->s->Delete();
    }
  if (this->Heap)
    {
    this->Heap->Delete();
    }
  if (this->p)
    {
    this->p->Delete();
    }

  this->DeleteAdjacency();
}

//----------------------------------------------------------------------------
int vtkbisDijkstraGraphGeodesicPath::RequestData(
  vtkInformation *           vtkNotUsed( request ),
  vtkInformationVector **    inputVector,
  vtkInformationVector *     outputVector)
{
  vtkInformation * inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo =   outputVector->GetInformationObject(0);

  vtkPolyData *input = vtkPolyData::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!input)
    {
    return 0;
    }

  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!output)
    {
    return 0;
    }

  if ( this->AdjacencyBuildTime.GetMTime() < input->GetMTime() )
    {
    this->Initialize();
    }
  else
    {
    this->Reset();
    }

  if (this->NumberOfVertices == 0)
    {
    return 0;
    }

  int trace =this->ShortestPath(this->StartVertex, this->EndVertex);
  //std::cout<<"we are here!!!!!!!!!"<<"trace:"<<trace<<endl;
  //if(trace==1)
  //{
	  //std::cout<<"we are in trace!!!!!!!!"<<endl;
	  this->TraceShortestPath(input, output, this->StartVertex, this->EndVertex);
  //}

  return 1;
}

//----------------------------------------------------------------------------
void vtkbisDijkstraGraphGeodesicPath::Initialize()
{
  vtkPolyData *input = vtkPolyData::SafeDownCast(
      this->GetExecutive()->GetInputData(0, 0));
//cout<<"A\n";
  this->BuildAdjacency( input );
  //cout<<"B\n";
  this->NumberOfVertices = input->GetNumberOfPoints();

  this->d->SetNumberOfComponents(1);
  this->d->SetNumberOfTuples(this->NumberOfVertices);
  this->pre->SetNumberOfComponents(1);
  this->pre->SetNumberOfTuples(this->NumberOfVertices);
  this->f->SetNumberOfComponents(1);
  this->f->SetNumberOfTuples(this->NumberOfVertices);
  this->s->SetNumberOfComponents(1);
  this->s->SetNumberOfTuples(this->NumberOfVertices);
  this->p->SetNumberOfComponents(1);
  this->p->SetNumberOfTuples(this->NumberOfVertices);

  // The heap has elements from 1 to n
  this->Heap->SetNumberOfComponents(1);
  this->Heap->SetNumberOfTuples(this->NumberOfVertices+1);
}

//----------------------------------------------------------------------------
void vtkbisDijkstraGraphGeodesicPath::Reset()
{
  this->IdList->Reset();
  this->HeapSize = 0;
}

//----------------------------------------------------------------------------
void vtkbisDijkstraGraphGeodesicPath::DeleteAdjacency()
{
  const int npoints = this->AdjacencyGraphSize;

  if (this->Adjacency)
    {
    for (int i = 0; i < npoints; i++)
      {
      this->Adjacency[i]->Delete();
      }
    delete [] this->Adjacency;
    }
  this->Adjacency = NULL;
}

//----------------------------------------------------------------------------
// The edge cost function should be implemented as a callback function to
// allow more advanced weighting
double vtkbisDijkstraGraphGeodesicPath::CalculateEdgeCost(
     vtkPolyData *pd, vtkIdType u, vtkIdType v)
{
  double p1[3];
  pd->GetPoint(u,p1);
  double p2[3];
  pd->GetPoint(v,p2);

  double w = sqrt(vtkMath::Distance2BetweenPoints(p1, p2));

  if (this->UseScalarWeights)
    {
    // Note this edge cost is not symmetric!
    vtkFloatArray *scalars =
      static_cast<vtkFloatArray*>(pd->GetPointData()->GetScalars());
    //    float s1 = scalars->GetValue(u);
    double s2 = static_cast<double>(scalars->GetValue(v));

    double wt = s2*s2;
    if (wt != 0.0)
      {
      w  /= wt;
      }
    }
  return w;
}


//----------------------------------------------------------------------------
// This is probably a horribly inefficient way to do it.
void vtkbisDijkstraGraphGeodesicPath::BuildAdjacency(vtkPolyData *pd)
{
	int i;

	int npoints = pd->GetNumberOfPoints();
	int ncells = pd->GetNumberOfCells();
//cout<<"npoints:"<<npoints<<"\t";
	this->DeleteAdjacency();
	//cout<<"b\t";
	this->Adjacency = new vtkIdList*[npoints];
	//cout<<"c\t";
	// Remember size, so it can be deleted again
	this->AdjacencyGraphSize = npoints;
	//cout<<"d\t";
	for (i = 0; i < npoints; i++)
    {
		this->Adjacency[i] = vtkIdList::New();
    }

	for (i = 0; i < ncells; i++)
    {//cout<<"f\t";
		// Possible types
		//    VTK_VERTEX, VTK_POLY_VERTEX, VTK_LINE,
		//    VTK_POLY_LINE,VTK_TRIANGLE, VTK_QUAD,
		//    VTK_POLYGON, or VTK_TRIANGLE_STRIP.

		vtkIdType ctype = pd->GetCellType(i);

		// Until now only handle polys and triangles
		// TODO: All types
		if (ctype == VTK_POLYGON || ctype == VTK_TRIANGLE || ctype == VTK_LINE)
		{//cout<<"g\n";
			vtkIdType *pts;
			vtkIdType npts;
			pd->GetCellPoints (i, npts, pts);

			vtkIdType u = pts[0];
			vtkIdType v = pts[npts-1];

			this->Adjacency[u]->InsertUniqueId(v);
			this->Adjacency[v]->InsertUniqueId(u);
			for (int j = 0; j < npts-1; j++)
			{
				vtkIdType u1 = pts[j];
				vtkIdType v1 = pts[j+1];
				this->Adjacency[u1]->InsertUniqueId(v1);
				this->Adjacency[v1]->InsertUniqueId(u1);
			}
		}
    }
	this->AdjacencyBuildTime.Modified();
}

//----------------------------------------------------------------------------
void vtkbisDijkstraGraphGeodesicPath::TraceShortestPath(
               vtkPolyData *inPd, vtkPolyData *outPd,
               vtkIdType startv, vtkIdType endv)
{
	//std::cout<<"startv:"<<startv<<"endv:"<<endv<<" \n";
	vtkPoints   *points = vtkPoints::New();
	vtkCellArray *lines = vtkCellArray::New();

	// n is far to many. Adjusted later
	lines->InsertNextCell(this->NumberOfVertices);

	// trace backward
	int npoints = 0;
	int v = endv;
	double pt[3];
	vtkIdType id;
	while (v != startv && v != (-1))
	{//std::cout<<"v="<<v<<"startv="<<startv<<" \n";
		IdList->InsertNextId(v);

		inPd->GetPoint(v,pt);

		id = points->InsertNextPoint(pt);

		lines->InsertCellPoint(id);

		npoints++;

		v = this->pre->GetValue(v);
		//std::cout<<"after update v = "<<v<<"\n";

    }
	//std::cout<<"out of traceshortestpath while loop \n";
	this->IdList->InsertNextId(v);
	inPd->GetPoint(v,pt);
	id = points->InsertNextPoint(pt);
	lines->InsertCellPoint(id);
	npoints++;

	lines->UpdateCellCount(npoints);
	outPd->SetPoints(points);
	points->Delete();
	outPd->SetLines(lines);
	lines->Delete();

}


//----------------------------------------------------------------------------
void vtkbisDijkstraGraphGeodesicPath::InitSingleSource(int startv)
{
  for (int v = 0; v < this->NumberOfVertices; v++)
    {
    // d will be updated with first visit of vertex
    this->d->SetValue(v, -1);
    this->pre->SetValue(v, -1);
    this->s->SetValue(v, 0);
    this->f->SetValue(v, 0);
    }

  this->d->SetValue(startv, 0);
}


//----------------------------------------------------------------------------
void vtkbisDijkstraGraphGeodesicPath::Relax(int u, int v, double w)
{
  if (this->d->GetValue(v) > this->d->GetValue(u) + w)
    {
    this->d->SetValue(v, this->d->GetValue(u) + w);
    this->pre->SetValue(v, u);

    this->HeapDecreaseKey(v);
    }
}

//----------------------------------------------------------------------------
int vtkbisDijkstraGraphGeodesicPath::ShortestPath(int startv, int endv)
{
	//std::cout<<"We are in shortestpath"<<"startv:"<<startv<<"endv:"<<endv<<endl;
	vtkPolyData *input = vtkPolyData::SafeDownCast(
			this->GetExecutive()->GetInputData(0, 0));

	int i, u, v;
	this->InitSingleSource(startv);
	this->HeapInsert(startv);
	this->f->SetValue(startv, 1);
	int stop = 0;

	//u=this->HeapExtractMin();
	//for(; u>=0 ; )

	while ((u = this->HeapExtractMin()) >= 0 && !stop)
	{
			//if(stop==0)
			//{
				//std::cout<<"e"<<endl;
				// u is now in s since the shortest path to u is determined
				this->s->SetValue(u, 1);
				//std::cout<<"f"<<endl;
				// remove u from the front set
				this->f->SetValue(u, 0);
				//std::cout<<"g"<<"u:"<<u<<endl;
				if (u == endv && this->StopWhenEndReached)
				{
					//std::cout<<"h"<<endl;
					stop = 1;
				}

				// Update all vertices v adjacent to u
				for (i = 0; i < this->Adjacency[u]->GetNumberOfIds(); i++)
				{
					//std::cout<<"adj:"<<this->Adjacency[u]->GetNumberOfIds()<<"i now:"<<i<<endl;
					v = this->Adjacency[u]->GetId(i);
					//std::cout<<"what is v now?"<<v<<endl;
					//std::cout<<"j"<<endl;
					// s is the set of vertices with determined shortest path...do not use them again
					if (!this->s->GetValue(v))
					{
						//std::cout<<"k"<<endl;
						// Only relax edges where the end is not in s and edge is in the front set
						//double w = this->CalculateEdgeCost(input, u, v);
						double w = 1;
						//std::cout<<"l"<<endl;
						if (this->f->GetValue(v))
						{
							//std::cout<<"m"<<endl;
							this->Relax(u, v, w);
							//std::cout<<"n"<<endl;
						}
						// add edge v to front set
						else
						{
							this->f->SetValue(v, 1);
							this->d->SetValue(v, this->d->GetValue(u) + w);

							// Set Predecessor of v to be u
							this->pre->SetValue(v, u);

							this->HeapInsert(v);
						}
					}
				}
				//u = this->HeapExtractMin();
				//if( u==-1)
				//{
				//return -1;
				//}
			//}
	}
	return 1;
}


//----------------------------------------------------------------------------
void vtkbisDijkstraGraphGeodesicPath::Heapify(int i)
{
	// left node
	int l = i * 2;

	// right node
	int r = i * 2 + 1;

	int smallest = -1;

	// The value of element v is d(v)
	// the heap stores the vertex numbers
	if (   l <= this->HeapSize && (this->d->GetValue(this->Heap->GetValue(l)) < this->d->GetValue(this->Heap->GetValue(i))))
    {
		smallest = l;
    }
	else
    {
		smallest = i;
    }

	if ( r <= this->HeapSize && (this->d->GetValue(this->Heap->GetValue(r)) < this->d->GetValue(this->Heap->GetValue(smallest))))
    {
		smallest = r;
    }

	if (smallest != i)
    {
		int t = this->Heap->GetValue(i);

		this->Heap->SetValue(i, this->Heap->GetValue(smallest));

		// where is Heap(i)
		this->p->SetValue(this->Heap->GetValue(i), i);

		// Heap and p are kind of inverses
		this->Heap->SetValue(smallest, t);
		this->p->SetValue(t, smallest);

		this->Heapify(smallest);
    }
}

//----------------------------------------------------------------------------
// Insert vertex v. Weight is given in d(v)
// H has indices 1..n
void vtkbisDijkstraGraphGeodesicPath::HeapInsert(int v)
{
  if (this->HeapSize >= this->Heap->GetNumberOfTuples()-1)
    return;

  this->HeapSize++;
  int i = this->HeapSize;

  while (i > 1 &&
         (this->d->GetValue(this->Heap->GetValue(i/2))
                   > this->d->GetValue(v)))
    {
    this->Heap->SetValue(i, this->Heap->GetValue(i/2));
    this->p->SetValue(this->Heap->GetValue(i), i);
    i /= 2;
    }
  // Heap and p are kind of inverses
  this->Heap->SetValue(i, v);
  this->p->SetValue(v, i);
}

//----------------------------------------------------------------------------
int vtkbisDijkstraGraphGeodesicPath::HeapExtractMin()
{
	//std::cout<<"in HeapExtractMin"<<endl;
  if (this->HeapSize == 0)
  {
	  //std::cout<<"in HeapSize==0 "<<endl;
	  return -1;

  }

  //std::cout<<"in HeapSize!=0"<<endl;
  int minv = this->Heap->GetValue(1);
  //std::cout<<"minv="<<minv<<"\n";
  this->p->SetValue(minv, -1);

  this->Heap->SetValue(1, this->Heap->GetValue(this->HeapSize));
  //std::cout<<"set heap 1 to:"<<this->Heap->GetValue(this->HeapSize)<<endl;
  this->p->SetValue(this->Heap->GetValue(1), 1);
  //std::cout<<"set p"<<this->Heap->GetValue(1)<<"to 1\n";
  this->HeapSize--;
  //std::cout<<"updated heap size:"<<this->HeapSize<<endl;
  this->Heapify(1);
  //std::cout<<"returned minv:"<<minv<<endl;
  return minv;
}

//----------------------------------------------------------------------------
void vtkbisDijkstraGraphGeodesicPath::HeapDecreaseKey(int v)
{
  // where in Heap is vertex v
  int i = this->p->GetValue(v);
  if (i < 1 || i > this->HeapSize)
    return;

  while (i > 1 &&
      this->d->GetValue(this->Heap->GetValue(i/2)) > this->d->GetValue(v))
    {
    this->Heap->SetValue(i, this->Heap->GetValue(i/2));
    this->p->SetValue(this->Heap->GetValue(i), i);
    i /= 2;
    }

  // Heap and p are kind of inverses
  this->Heap->SetValue(i, v);
  this->p->SetValue(v, i);
}

//----------------------------------------------------------------------------
void vtkbisDijkstraGraphGeodesicPath::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "StopWhenEndReached: ";
  if (this->StopWhenEndReached)
    {
    os << "On\n";
    }
  else
    {
    os << "Off\n";
    }
  os << indent << "Verts in input mesh: " << this->NumberOfVertices << endl;

  // Add all members later
  // this->d
  // this->pre
  // this->Adjacency
  // this->IdList
  // this->p
  // this->UseScalarWeights
  // this->AdjacencyGraphSize
  // this->HeapSize
  // this->s
  // this->Heap
  // this->f
}


