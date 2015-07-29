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
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkbisComputeGLM.h"
#include "vtkpxAnalyzeImageWriter.h"

vtkbisComputeGLM* vtkbisComputeGLM::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisComputeGLM");
  if(ret)
    {
      return (vtkbisComputeGLM*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisComputeGLM;
}

vtkbisComputeGLM::vtkbisComputeGLM()
{
  this->DebugName=NULL;
  this->ImageMask=NULL;
  this->ResidualImage=NULL;
  this->TmapImage=NULL;
  this->UnfitTimeSeries=NULL;
  this->DriftBetaValues=NULL;
  this->RegressorMatrix=NULL;
  this->NULLRegressorMatrix=vtkpxMatrix::New();
  this->OrderOfFstatDriftPolynomial=3;
  this->NumberOfTasks=-1;
  this->TempMatrix1=vtkpxMatrix::New();
  this->TempMatrix2=vtkpxMatrix::New();
  this->TempMatrix3=vtkpxMatrix::New();
  this->TempMatrix4=vtkpxMatrix::New();

  this->DebugIndex=-1;
  //  this->DebugIndex=51232;
  //  this->SetDebugName("test43");
}

vtkbisComputeGLM::~vtkbisComputeGLM()
{
  if (this->DebugName!=NULL)
    delete [] this->DebugName;

  this->SetImageMask(NULL);
  this->SetResidualImage(NULL);
  this->SetTmapImage(NULL);
  this->SetRegressorMatrix(NULL);
  this->SetUnfitTimeSeries(NULL);
  this->SetDriftBetaValues(NULL);
  this->TempMatrix1->Delete();
  this->TempMatrix2->Delete();
  this->TempMatrix3->Delete();
  this->TempMatrix4->Delete();
  this->NULLRegressorMatrix->Delete();


}

// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisComputeGLM::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();

  int nt=this->NumberOfTasks;
  if (this->RegressorMatrix!=NULL)
    {
      int sz[2]; this->RegressorMatrix->GetSize(sz);
      if (nt>sz[1] || nt<1)
	nt=sz[1];
    }

  if (nt<0)
    nt=1;

  output->SetScalarTypeToFloat();
  output->SetNumberOfScalarComponents(nt+1);
}
// ---------------------------------------------------------------------------------------------------------------------------
/*
  Problem =  A x = B  where X is unknown

 First premultiply by A':  A'A x = A'B
 Solution x = (A'A)^{-1}.A'.B

  LSQ = (A'A)^{-1}.A'
*/
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisComputeGLM::CreateNULLRegressorMarix(vtkpxMatrix* output,vtkpxMatrix* regressor,int order)
{
  int sz[2]; regressor->GetSize(sz);
  output->Zeros(sz[0],order+1);

  double t0=0.0;
  double t1=double(sz[0]-1);
  double dt=0.5*(t1-t0);


  for (int row=0;row<sz[0];row++)
    {
      double r=double(row);
      double t=(r-t0)/dt-1.0;
      double t2=t*t;
      double t3=t*t2;

      for (int pol=0;pol<=order;pol++)
	{
	  switch(pol)
	    {
	    case 0: // P_0(x)=1;
	      output->SetDirectElement(row,pol,1.0);
	      break;
	    case 1: // P_1(x)=x;
	      output->SetDirectElement(row,pol,t);
	      break;
	    case 2: // P_2(x)=0.5*(3x^2-1)
	      output->SetDirectElement(row,pol,1.5*t2-0.5);
	      break;
	    case 3: // P_3(x) =0.5*(5x^3-3x)
	      output->SetDirectElement(row,pol,2.5*t3-1.5*t);
	      break;
	    }
	}
    }
}
// ---------------------------------------------------------------------------------------------------------------------------

vtkpxMatrix* vtkbisComputeGLM::ComputeLSQMatrix(vtkpxMatrix* A,vtkpxMatrix* S2BMatrix)
{
  vtkpxMatrix* At=vtkpxMatrix::New();
  At->Copy(A);
  At->Transpose();

  vtkpxMatrix* AtA=vtkpxMatrix::New();
  if (S2BMatrix!=NULL)
    AtA=S2BMatrix;
  else
    AtA=vtkpxMatrix::New();

  vtkpxMatrix::Multiply(At,A,AtA);
  AtA->Invert();


  vtkpxMatrix* LSQ=vtkpxMatrix::New();
  vtkpxMatrix::Multiply(AtA,At,LSQ);

  if (S2BMatrix==NULL)
    AtA->Delete();
  At->Delete();

  return LSQ;

}
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisComputeGLM::StoreVoxelIndexIntoColumn(vtkDataArray* dat,int index,vtkpxMatrix* colvect)
{
  int nc=dat->GetNumberOfComponents();
  colvect->Zeros(nc,1);
  for (int i=0;i<nc;i++)
    colvect->SetDirectElement(i,0,dat->GetComponent(index,i));
}

void vtkbisComputeGLM::StoreColumnIntoVoxelIndex(vtkpxMatrix* vect,vtkDataArray* dat,int index)
{
  int nc=dat->GetNumberOfComponents();
  for (int i=0;i<nc;i++)
    dat->SetComponent(index,i,vect->GetDirectElement(i,0));
}

// ---------------------------------------------------------------------------------------------------------------------------
//    r = || A.beta - colvect ||
// ---------------------------------------------------------------------------------------------------------------------------

double vtkbisComputeGLM::ComputeFStats(vtkpxMatrix* A,vtkpxMatrix* ReducedA,vtkpxMatrix* beta,vtkpxMatrix* nullbeta,
				       vtkpxMatrix* colvect,double factor,double tmapdof,vtkDataArray* res,vtkDataArray* unfit,int index,int debug)
{
  vtkpxMatrix::Multiply(A,beta,this->TempMatrix1);
  vtkpxMatrix::Add(1.0,this->TempMatrix1,-1.0,colvect,this->TempMatrix2);

  int nc=unfit->GetNumberOfComponents();
  for (int i=0;i<nc;i++)
    unfit->SetComponent(index,i,-1.0*this->TempMatrix2->GetDirectElement(i,0));
  double SSEF=this->TempMatrix2->SumSquares();

  if (debug)
    {
      A->Save("A.matr");
      ReducedA->Save("RA.matr");
      beta->Save("beta.matr");
      colvect->Save("col.matr");
      this->TempMatrix1->Save("t1.matr");
      this->TempMatrix2->Save("t2.matr");
      fprintf(stdout,"SSEF = %f\n",SSEF);
    }


  double MSE=(SSEF/tmapdof);


  vtkpxMatrix::Multiply(ReducedA,nullbeta,this->TempMatrix3);
  vtkpxMatrix::Add(1.0,this->TempMatrix3,-1.0,colvect,this->TempMatrix4);
  double SSEB=this->TempMatrix4->SumSquares();

  res->SetComponent(index,0,MSE);

  if (debug)
    {
      this->TempMatrix1->Save("rbeta.matr");
      this->TempMatrix3->Save("t3.matr");
      this->TempMatrix4->Save("t4.matr");
      fprintf(stdout,"SSEB = %f\n",SSEB);
      fprintf(stdout,"index=%d\n",index);
      fprintf(stdout,"MSE =%f\n",res->GetComponent(index,0));
      fprintf(stdout,"R^2  = %f\n",1.0-SSEF/SSEB);
      beta->Print("B=");
    }


  if (SSEF<0.0001)
    return -1.0;

  double f = factor*(SSEB-SSEF)/SSEF;

  if (debug)
    fprintf(stdout,"F-Stat=%f\n",f);

  return f;

}





// ---------------------------------------------------------------------------------------------------------------------------
//   beta= LSQ * colvect
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisComputeGLM::ComputeBetas(vtkpxMatrix* LSQ,vtkpxMatrix* beta,vtkpxMatrix* colvect)
{
  vtkpxMatrix::Multiply(LSQ,colvect,beta);
}
// ---------------------------------------------------------------------------------------------------------------------------
//   Main Function
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisComputeGLM::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{

  fprintf(stdout,"Beginning ComputeGLM\n");

  if (input==NULL || output==NULL || this->RegressorMatrix==NULL)
    {
      fprintf(stdout,"Bad Image Input to vtkbisComputeGLM SimpleExecute\n");
      return;
    }


  if (this->ImageMask!=NULL)
    {
      int dim[3]; this->ImageMask->GetDimensions(dim);
      int dim2[3]; input->GetDimensions(dim2);
      int sum=0;
      for (int ia=0;ia<=2;ia++)
	sum+=abs(dim[ia]-dim2[ia]);
      if (sum>0)
	fprintf(stdout,"Bad Mask Input to vtkbisComputeGLM SimpleExecute\n");
      return;
    }



  vtkDataArray* inp=input->GetPointData()->GetScalars();
  int nt=inp->GetNumberOfTuples();
  int nc=inp->GetNumberOfComponents();

  int sz[2]; this->RegressorMatrix->GetSize(sz);
  if (sz[0]!=nc)
    {
      fprintf(stdout,"Bad Regressor Matrix %d x %d numframes=%d\n",sz[0],sz[1],nc);
      return;
    }

  int nc2=output->GetNumberOfScalarComponents();
  int numdrift=sz[1]-this->NumberOfTasks;
  int maxtask=sz[1];

  // First Component of output is f-stat
  if ( (maxtask-numdrift)!=(nc2-1))
    {
      fprintf(stdout,"Bad Task Number Matrix %d x %d (Task columns %d:%d) vs output numframes=%d\n",sz[0],sz[1],
	      numdrift,maxtask,nc2-1);
      return;
    }


  if (this->ResidualImage!=NULL)
    this->ResidualImage->Delete();

  this->ResidualImage=vtkImageData::New();
  this->ResidualImage->CopyStructure(input);
  this->ResidualImage->SetNumberOfScalarComponents(1);
  this->ResidualImage->SetScalarTypeToFloat();
  this->ResidualImage->AllocateScalars();

  if (this->UnfitTimeSeries!=NULL)
    this->UnfitTimeSeries->Delete();

  this->UnfitTimeSeries=vtkImageData::New();
  this->UnfitTimeSeries->CopyStructure(input);
  this->UnfitTimeSeries->SetNumberOfScalarComponents(input->GetNumberOfScalarComponents());
  this->UnfitTimeSeries->SetScalarTypeToFloat();
  this->UnfitTimeSeries->AllocateScalars();

  int createnullregressormatrix=0;
  if (numdrift<1)
    {
      createnullregressormatrix=1;
      if (this->OrderOfFstatDriftPolynomial<0)
	this->OrderOfFstatDriftPolynomial=0;
      else if (this->OrderOfFstatDriftPolynomial>3)
	this->OrderOfFstatDriftPolynomial=3;
      numdrift=this->OrderOfFstatDriftPolynomial+1;
    }

  if (this->DriftBetaValues!=NULL)
    this->DriftBetaValues->Delete();

  this->DriftBetaValues=vtkImageData::New();
  this->DriftBetaValues->CopyStructure(input);
  this->DriftBetaValues->SetNumberOfScalarComponents(numdrift);
  this->DriftBetaValues->SetScalarTypeToFloat();
  this->DriftBetaValues->AllocateScalars();
  vtkDataArray* driftbeta=this->DriftBetaValues->GetPointData()->GetScalars();
  for (int ia=0;ia<driftbeta->GetNumberOfComponents();ia++)
    driftbeta->FillComponent(ia,0.0);
  fprintf(stdout,"Done allocating DriftBetaValues numc=%d (numdrift=%d)\n",driftbeta->GetNumberOfComponents(),numdrift);

  if (this->TmapImage!=NULL)
    this->TmapImage->Delete();
  this->TmapImage=vtkImageData::New();
  this->TmapImage->CopyStructure(output);
  this->TmapImage->SetNumberOfScalarComponents(output->GetNumberOfScalarComponents());
  this->TmapImage->AllocateScalars();

  fprintf(stdout,"Done Allocating Memory, %d, %d\n",
	  output->GetNumberOfScalarComponents(),
	  this->TmapImage->GetNumberOfScalarComponents());

  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* tmap=this->TmapImage->GetPointData()->GetScalars();
  for (int ia=0;ia<nc2;ia++)
    {
      out->FillComponent(ia,0.0);
      tmap->FillComponent(ia,0.0);
    }

  vtkDataArray* res=this->ResidualImage->GetPointData()->GetScalars();
  res->FillComponent(0,0.0);

  vtkDataArray* unfit=this->UnfitTimeSeries->GetPointData()->GetScalars();


  vtkDataArray* msk=NULL;
  if (this->ImageMask!=NULL)
    msk=this->ImageMask->GetPointData()->GetScalars();

  //  fprintf(stdout,"All Set to GO out numcomponents=%d\n",nc2);

  if (createnullregressormatrix==0)
    {
      fprintf(stdout,"Creating NULL Regressor Matrix from first %d columns of Regressor Matrix\n",numdrift);
      this->NULLRegressorMatrix->Copy(this->RegressorMatrix,-1,-1,0,numdrift-1);
    }
  else
    {
      fprintf(stdout,"Creating NULL Regressor Matrix with %d columns as direct polynomail fit\n",numdrift);
      this->CreateNULLRegressorMarix(this->NULLRegressorMatrix,this->RegressorMatrix,numdrift-1);
    }

  vtkpxMatrix* S2BMatrix=vtkpxMatrix::New();
  vtkpxMatrix* LSQ=this->ComputeLSQMatrix(this->RegressorMatrix,S2BMatrix);

  int sk[2]; S2BMatrix->GetSize(sk);
  for (int ia=0;ia<sk[0];ia++)
    {
      double v=S2BMatrix->GetElement(ia,ia);
      S2BMatrix->SetElement(ia,ia,sqrt(v));
    }

  vtkpxMatrix* NULL_LSQ=this->ComputeLSQMatrix(this->NULLRegressorMatrix);

  std::string debugfname;
  if (this->DebugName!=NULL)
    {
      debugfname=this->DebugName; debugfname+="_lsq.matr";
      LSQ->Save(debugfname.c_str());
      fprintf(stdout,"LSQ Matrix Saved in %s\n",debugfname.c_str());

      debugfname=this->DebugName; debugfname+="_drift.matr";
      this->NULLRegressorMatrix->Save(debugfname.c_str());
      fprintf(stdout,"NULL Regressor Matrix Saved in %s\n",debugfname.c_str());
    }


  vtkpxMatrix* colvect=vtkpxMatrix::New();
  colvect->Zeros(nc,1);
  this->TempMatrix1->Zeros(nc,1);
  this->TempMatrix2->Zeros(nc,1);
  this->TempMatrix3->Zeros(numdrift,1);
  this->TempMatrix4->Zeros(numdrift,1);


  vtkpxMatrix* betas=vtkpxMatrix::New();
  vtkpxMatrix* null_betas=vtkpxMatrix::New();
  betas->Zeros(1,sz[1]);
  null_betas->Zeros(1,sz[1]);

  int maxcount=int(nt/10);
  int count=0;
  double pog=0.0;
  this->UpdateProgress(0.01);
  fprintf(stdout,"GLM Compute : ");
  int numdone=0;

  // factor= dff/(dfb-dff)
  double factor_forfstat= double(sz[0]-sz[1])/double(this->NumberOfTasks);
  double tmapdof=(sz[0]-sz[1]);
  fprintf(stdout,"Factor for fstat=%f nummdof for tmap=%f\n",factor_forfstat,tmapdof);

  int found=0;
  for (int voxel=0;voxel<nt;voxel++)
    {
      int dodebugsave=0;
      if (this->DebugName!=NULL && voxel==this->DebugIndex)
	dodebugsave=1;

      int doglm=1;
      if (msk!=NULL)
	{
	  if (msk->GetComponent(voxel,0)<1)
	    doglm=0;
	}
      else
	{
	  this->StoreVoxelIndexIntoColumn(inp,voxel,colvect);
	  double a=colvect->SumMagnitude();
	  if (a<0.001)
	    doglm=0;
	}

      if (doglm==1)
	{
	  // Step 1


	  if (dodebugsave)
	    {
	      debugfname=this->DebugName; debugfname+="_vect.matr";
	      colvect->Save(debugfname.c_str());
	      fprintf(stdout,"Column Vector Saved in %s\n",debugfname.c_str());
	    }

	  this->ComputeBetas(LSQ,betas,colvect);
	  this->ComputeBetas(NULL_LSQ,null_betas,colvect);
	  if (dodebugsave)
	    {
	      debugfname=this->DebugName; debugfname+="_betas.matr";
	      betas->Save(debugfname.c_str());
	      fprintf(stdout,"Beta Vector Saved in %s\n",debugfname.c_str());
	    }


	  double f=this->ComputeFStats(this->RegressorMatrix,this->NULLRegressorMatrix,betas,null_betas,colvect,
				       factor_forfstat,tmapdof,res,unfit,voxel,dodebugsave);

	  out->SetComponent(voxel,0,f);
	  double MSE=res->GetComponent(voxel,0);
	  tmap->SetComponent(voxel,0,MSE);
	  double sqMSE=sqrt(MSE);

	  for (int task=numdrift;task<maxtask;task++)
	    {
	      int outind=task-numdrift+1;
	      double b=betas->GetElement(task,0);
	      out->SetComponent(voxel,outind,b);

	      if (dodebugsave)
		fprintf(stdout,"Storing voxel=%d outind=%d with beta=%f from task=%d (%d,%d)\n",voxel,outind,b,task,betas->GetSize()[0],betas->GetSize()[1]);

	      double tval= b/ (S2BMatrix->GetElement(task,task)*sqMSE);
	      if (fabs(b)>1e-4)
		tmap->SetComponent(voxel,outind,tval);
	      else
		tmap->SetComponent(voxel,outind,0.0);

	      //	      if (found==0)
	      //		{
	      //		  fprintf(stderr,"b=%f S2B=%f sqMSE=%f t=%f\n",
	      //			  b,S2BMatrix->GetElement(task,task),sqMSE,tval);
	      //		  found=1;
	      //		}
	    }

	  if (numdrift>0)
	    {
	      for (int t=0;t<numdrift;t++)
		driftbeta->SetComponent(voxel,t,betas->GetElement(t,0));
	    }
	  ++numdone;
	}
      ++count;
      if (count==maxcount)
	{
	  pog+=0.1;
	  this->UpdateProgress(pog);
	  count=0;
	  fprintf(stdout," %.2f/100 (%d) ",100.0*pog,numdone);
	}
    }

  fprintf(stdout,"\n");
  betas->Delete();
  null_betas->Delete();
  colvect->Delete();
  LSQ->Delete();
  NULL_LSQ->Delete();
  S2BMatrix->Delete();
  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------------------------------------------
vtkImageData* vtkbisComputeGLM::MatrixImageMultiply(vtkImageData* betasimage,vtkpxMatrix* glmmatrix)
{
  if (glmmatrix==NULL || betasimage==NULL)
    {
      fprintf(stdout,"Either matrix or betasimage image is NULL");
      return NULL;
    }

  vtkDataArray* betas=betasimage->GetPointData()->GetScalars();


  int nc=betas->GetNumberOfComponents();
  int nt=betas->GetNumberOfTuples();

  int sz[2]; glmmatrix->GetSize(sz);
  if (sz[1] != nc)
    {
      fprintf(stdout,"Bad Dimensions image (%d voxels x %d frames), matrix=(%d rows x %d columns)\n",
	      nt,nc,sz[0],sz[1]);
      return NULL;
    }

  vtkImageData* outimage=vtkImageData::New();
  outimage->CopyStructure(betasimage);
  outimage->SetNumberOfScalarComponents(sz[0]);
  outimage->AllocateScalars();

  vtkDataArray* outdata=outimage->GetPointData()->GetScalars();

  vtkpxMatrix* temp1=vtkpxMatrix::New();
  vtkpxMatrix* temp2=vtkpxMatrix::New();
  vtkpxMatrix* At=vtkpxMatrix::New();
  At->Copy(glmmatrix);
  At->Transpose();
  temp1->Zeros(sz[1],1);
  temp2->Zeros(sz[0],1);

  int sum=(nt/10);
  int count=0;
  for (int voxel=0;voxel<nt;voxel++)
    {
      ++count;
      if (count==sum)
	{
	  fprintf(stdout,"Computing image multiply voxel = %d/%d\n",voxel,nt);
	  count=0;
	}
      vtkbisComputeGLM::StoreVoxelIndexIntoColumn(betas,voxel,temp1);
      vtkpxMatrix::Multiply(glmmatrix,temp1,temp2);
      vtkbisComputeGLM::StoreColumnIntoVoxelIndex(temp2,outdata,voxel);
    }

  temp1->Delete();
  temp2->Delete();
  At->Delete();
  return outimage;


}

vtkImageData* vtkbisComputeGLM::ComputeSimpleGLM(vtkImageData* timeseriesimage,vtkpxMatrix* glmmatrix)
{
  if (glmmatrix==NULL || timeseriesimage==NULL)
    {
      fprintf(stdout,"Either matrix or timeseriesimage image is NULL");
      return NULL;
    }

  vtkDataArray* timeseries=timeseriesimage->GetPointData()->GetScalars();
  int nc=timeseries->GetNumberOfComponents();
  int nt=timeseries->GetNumberOfTuples();

  int sz[2]; glmmatrix->GetSize(sz);
  if (sz[0] != nc)
    {
      fprintf(stdout,"Bad Dimensions image (%d voxels x %d frames), matrix=(%d rows x %d columns)\n",
	      nt,nc,sz[0],sz[1]);
      return NULL;
    }

  vtkImageData* outimage=vtkImageData::New();
  outimage->CopyStructure(timeseriesimage);
  outimage->SetNumberOfScalarComponents(sz[1]);
  outimage->SetScalarTypeToFloat();
  outimage->AllocateScalars();

  vtkDataArray* outdata=outimage->GetPointData()->GetScalars();

  vtkpxMatrix* temp1=vtkpxMatrix::New();
  vtkpxMatrix* temp2=vtkpxMatrix::New();
  vtkpxMatrix* LSQ=vtkbisComputeGLM::ComputeLSQMatrix(glmmatrix,NULL);
  temp1->Zeros(sz[1],1);
  temp2->Zeros(sz[1],1);

  int sum=(nt/10);
  int count=0;
  for (int voxel=0;voxel<nt;voxel++)
    {
      ++count;
      if (count==sum)
	{
	  fprintf(stdout,"Computing simple glm voxel = %d/%d\n",voxel,nt);
	  count=0;
	}
      vtkbisComputeGLM::StoreVoxelIndexIntoColumn(timeseries,voxel,temp1);
      vtkpxMatrix::Multiply(LSQ,temp1,temp2);
      vtkbisComputeGLM::StoreColumnIntoVoxelIndex(temp2,outdata,voxel);
    }

  temp1->Delete();
  temp2->Delete();
  LSQ->Delete();
  return outimage;
}

