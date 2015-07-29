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





// ------------------------------------------------------------------------
//
//         Some of this code derives from AFNI Source and is released
// 
// ------------------------------------------------------------------------

/*****************************************************************************
   Major portions of this software are copyrighted by the Medical College
   of Wisconsin, 1994-2000, and are released under the Gnu General Public
   License, Version 2.  See the file README.Copyright for details.
******************************************************************************/

#include "vtkObjectFactory.h"
#include "vtkIntArray.h"
#include "vtkpxMatrix.h"
#include "vtkbisWaver.h"
#include "vtkstd/string"

vtkbisWaver* vtkbisWaver::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisWaver");
  if(ret)
    {
      return (vtkbisWaver*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisWaver;
}

// ---------------------------------------------------------------------------------------------------------------------------

vtkbisWaver::vtkbisWaver() 
{
  this->Status=NULL;
  this->ResetParameters();
}

vtkbisWaver::~vtkbisWaver() 
{
  if (this->Status!=NULL)
    delete [] this->Status;
}

void vtkbisWaver::ResetParameters() 
{
  // This is GAM
  this->Waveform_type    = 1;

  this->WAV_delay_time   =  2.0 ;
  this->WAV_rise_time    =  4.0 ;
  this->WAV_fall_time    =  6.0 ;
  this->WAV_undershoot   =  0.2 ;
  this->WAV_restore_time =  2.0 ;

  this->Peak=1.0;
  this->TR=1.0 ;

  this->GAM_power        = 8.6 ;
  this->GAM_time         = 0.547 ;
  this->GAM_ampl         = 0.0 ;
  this->GAM_delay_time   = 0.0 ;
  this->SetStatus("None");
}


/*----------------------------------------------------------------
  Function that transitions from 0 to 1 over input x in [0,1].
------------------------------------------------------------------*/


double vtkbisWaver::ztone( double x )
{
#ifndef PI
#  define PI 3.14159265358979323846
#endif

#define ZT_FAC 0.50212657
#define ZT_ADD 0.99576486
   double y , z ;

   if( x <= 0.0 ) return 0.0 ;
   if( x >= 1.0 ) return 1.0 ;

   y = (0.5*PI) * ( 1.6 * x - 0.8 ) ;
   z = ZT_FAC * ( tanh(tan(y)) + ZT_ADD ) ;
   return z ;
#undef ZT_FAC
#undef ZT_ADD
#undef PI
}

double vtkbisWaver::waveform( double t )
{
  
  switch (this->Waveform_type)
    {
    case 0:
      if (t==0.0)
	return 1.0;
      else
	return 0.0;
      break;
    case 2:
      return waveform_GAM(t) ;
    case 3:
      return waveform_doublegamma(t);
    case 4:
      return waveform_triplegamma(t);
    case 5:
      return waveform_spm(t);
    default:
      return waveform_WAV(t);
    }
}

/*----------------------------------------------------------------*/

// Rajeevan Notation from vtknrGammaSourceNew ->
//  this->Alpha -> GAM_Power
//  this->Beta  -> GAM_Time
//  0           -> this->GAM_DelayTime
//  1.0/MaxAmpl -> GAM_Ampl

double vtkbisWaver::gammafunction( double t,double gpower,double gtime,double gpeak)
{
  if (t<0.0)
    return 0.0;

  double ampl= gpeak*exp(gpower) / pow(gpower*gtime,gpower) ;
  return ampl * pow(t,gpower) * exp(-t/gtime) ;
}
/*----------------------------------------------------------------*/
// The following three functions come vtknrHrfSource (N. Rajeevan)
/*----------------------------------------------------------------*/
double vtkbisWaver::waveform_doublegamma( double t ) 
{
  double v=this->gammafunction(t,5.0,1.1,1.0);
  return v+this->gammafunction(t,12.0,0.9,-0.4);
}
/*----------------------------------------------------------------*/
double vtkbisWaver::waveform_triplegamma( double t ) 
{
  double v=this->waveform_doublegamma(t);
  return v+this->gammafunction(t,3.0,0.4,-0.25);
}
/*----------------------------------------------------------------*/
double vtkbisWaver::waveform_spm( double t ) 
{
  double v=this->gammafunction(t, 5.0,1.0,1.0);
  return v+this->gammafunction(t,16.0,1.0,-0.1667);
}
/*----------------------------------------------------------------*/

double vtkbisWaver::waveform_GAM( double t )
{
  if( GAM_ampl == 0.0 ){
    GAM_ampl = exp(GAM_power) / pow(GAM_power*GAM_time,GAM_power) ;
  }
  
  if( t-GAM_delay_time <= 0.0 ) return 0.0 ;
  
  return GAM_ampl * pow((t-GAM_delay_time),GAM_power) * exp(-(t-GAM_delay_time)/GAM_time) ;
}

/*----------------------------------------------------------------*/

double vtkbisWaver::waveform_WAV( double t )
{

  double      WAV_rise_start   = WAV_delay_time ;
  double      WAV_fall_start   = WAV_rise_start + WAV_rise_time ;
  double      WAV_fall_end     = WAV_fall_start + WAV_fall_time ;
  double      WAV_restore_end  = WAV_fall_end   + WAV_restore_time ;

   if( t < WAV_rise_start )
      return 0.0 ;

   if( t < WAV_fall_start )
      return ztone( (t-WAV_rise_start)/WAV_rise_time ) ;

   if( t < WAV_fall_end )
      return (1.0+WAV_undershoot) * ztone( (WAV_fall_end-t)/WAV_fall_time )
             - WAV_undershoot ;

   if( t < WAV_restore_end )
      return -WAV_undershoot * ztone( (WAV_restore_end-t)/WAV_restore_time ) ;

   return 0.0 ;
}

double vtkbisWaver::ComputeGammaDuration(double G_power,double G_time)
{
  double bal = 5.5/G_power + 1.0 ;
  double al  = bal ;
  double lal = log(al) ;
      
  while( al < bal + lal ){ al = bal + 1.1*lal ; lal = log(al) ; }
  
  return al * G_power * G_time ;
}
/*----------------------------------------------------------------*/
double vtkbisWaver::ComputeDuration()
{
  double   WAV_duration = -666.0 ;

  //  fprintf(stderr,"Waveform_type=%d\n",this->Waveform_type);

  switch (this->Waveform_type)
    {
    case 2:
      WAV_duration=this->ComputeGammaDuration(GAM_power,GAM_time);
      break;
    case 3:
    case 4:
      WAV_duration=this->ComputeGammaDuration(5.0,1.1)*1.5;
      break;
    case 5:
      WAV_duration=this->ComputeGammaDuration(5.0,1.0)*1.5;
      break;
    default:
      WAV_duration = WAV_delay_time + WAV_rise_time + WAV_fall_time
	+ ( (WAV_undershoot != 0.0) ? WAV_restore_time : 0.0 ) ;
    }
  return WAV_duration;
}

/*----------------------------------------------------------------*/
vtkDoubleArray* vtkbisWaver::GenerateResponseFunction()
{
  double   WAV_duration = this->ComputeDuration();
  //  fprintf(stderr,"Waveform_type=%d\n",this->Waveform_type);
  
  int WAV_npts = (int)(1 + ceil(WAV_duration/this->TR)) ;
  //fprintf(stderr,"WAV_Npts=%d, %d, %f\n",WAV_npts,WAV_duration,this->TR);
  vtkDoubleArray* arr=vtkDoubleArray::New();
  arr->SetNumberOfComponents(1);
  arr->SetNumberOfTuples(WAV_npts);

  for(int  ii=0 ; ii < WAV_npts ; ii++ )
    arr->SetComponent(ii,0, this->Peak * this->waveform(this->TR*double(ii) ));

  char line[200];
  

  if (this->Waveform_type==1)
    {
      sprintf(line,"Cox Waveform: del=%.3f rist=%.3f fall=%.3f under=%.3f restore=%.3f peak=%.3f TR=%.3f",
	      this->WAV_delay_time,this->WAV_rise_time,this->WAV_fall_time,this->WAV_undershoot,this->WAV_restore_time,
	      this->Peak,this->TR);
      this->SetStatus(line);
    }
  else if (this->Waveform_type==3)
    {
      this->SetStatus("Canonical Double Gamma");
    }
  else if (this->Waveform_type==4)
    {
      this->SetStatus("Canonical Triple Gamma");
    }
  else if (this->Waveform_type==5)
    {
      this->SetStatus("SPM Double Gamma");
    }
  else
    {
      sprintf(line,"Gamma Waveform: power=%.3f time=%.3f ampl=%.3f delay_time=%.3f peak=%.3f TR=%.3f",
	      this->GAM_power,this->GAM_time,this->GAM_ampl,this->GAM_delay_time,
	      this->Peak,this->TR);
      this->SetStatus(line);
    }

  return arr;
}

/*----------------------------------------------------------------*/
vtkpxMatrix* vtkbisWaver::GenerateResponseFunctionForAllOptions()
{
  int old_waveform_type=this->Waveform_type;

  double duration=0.0;
  for (int i=1;i<=5;i++)
    {
      this->Waveform_type=i;
      double v=this->ComputeDuration();
      if (v>duration)
	duration=v;
    }

  int WAV_npts = (int)(1 + ceil(duration/this->TR)) ;
  vtkpxMatrix* plots=vtkpxMatrix::New();
  plots->Zeros(WAV_npts,5);

  for (int i=1;i<=5;i++)
    {
      this->Waveform_type=i;
      for(int  ii=0 ; ii < WAV_npts ; ii++ )
	plots->SetDirectElement(ii, i -1, this->Peak * this->waveform(this->TR*double(ii) ));
    }

  this->Waveform_type=old_waveform_type;
  return plots;
}
/*----------------------------------------------------------------*/
int vtkbisWaver::CreateWaveform(vtkpxMatrix* input,vtkpxMatrix* output)
{

  if (input==NULL || output==NULL)
    {
      fprintf(stderr,"Bad Matrix Input to vtkbisWaver::CreateWaveform\n");
      return 0;
    }

  
   /*---- if no input timeseries, just output waveform ----*/

  int size[2]; input->GetSize(size);

  int  IN_npts=size[0];

  vtkDoubleArray* resp=this->GenerateResponseFunction();
  int WAV_npts=resp->GetNumberOfTuples();

  /*  fprintf(stderr,"Response Function\n");
    for (int i=0;i<WAV_npts;i++)
    fprintf(stderr,"%d \t %.5f\n",i,resp->GetComponent(i,0));*/
  
  int OUT_npts = IN_npts + WAV_npts ;



  output->Zeros(OUT_npts,size[1]);

  for (int column=0;column<size[1];column++)
    {
      for(int jj=0 ; jj < IN_npts ; jj++ )
	{
	  double val = input->GetDirectElement(jj,column);
	  if( val == 0.0 || fabs(val) >= 33333.0 ) 
	    continue ;
	  for(int ii=0 ; ii < WAV_npts && ii+jj < OUT_npts ; ii++ )
	    output->AddToElement(ii+jj,column,val * resp->GetComponent(ii,0));
	}
      
      for(int jj=0 ; jj < IN_npts ; jj++ )
	{
	  double val = input->GetDirectElement(jj,column);
	  if( fabs(val) >= 33333.0 ) 
	    output->SetDirectElement(jj,column,99999.0);
	}
      
    }
  resp->Delete();
  return 1;
}

int vtkbisWaver::CreateStackedWaveform(vtkpxMatrix* input,vtkpxMatrix* output,vtkIntArray* breaks)
{
  if (input==NULL || output==NULL || breaks==NULL)
    {
      fprintf(stderr,"Bad Matrix Input to vtkbisWaver::CreateStacked Waveform\n");
      return 0;
    }
  

  int nruns=breaks->GetNumberOfTuples();
  if (nruns<1)
    {
      fprintf(stderr,"Bad Breaks Input to vtkbisWaver::CreateStacked Waveform\n");
      return 0;
    }

  vtkpxMatrix* subinput=vtkpxMatrix::New();
  vtkpxMatrix* suboutput=vtkpxMatrix::New();

  int size[2]; input->GetSize(size);
  output->Zeros(size[0],size[1]);

  for (int i=0;i<nruns;i++)
    {
      int minrow=(int)breaks->GetComponent(i,0);
      int maxrow=size[0]-1;
      if (i<(nruns-1))
	maxrow=(int)(breaks->GetComponent(i+1,0))-1;

      fprintf(stderr,"Waver Matrix Run %d rows=%d:%d\n",i+1,minrow,maxrow);

      subinput->Copy(input,minrow,maxrow,-1,-1);
      
      this->CreateWaveform(subinput,suboutput);

      //      char line[300];
      /*      sprintf(line,"submatrix_%d.matr",i+1);
      suboutput->Save(line);
      sprintf(line,"subinput_%d.matr",i+1);
      subinput->Save(line);*/
      
      // Copy Stuff back into main matrix
      for (int row=minrow;row<=maxrow;row++)
	for (int col=0;col<size[1];col++)
	  output->SetDirectElement(row,col,suboutput->GetDirectElement(row-minrow,col));
    }
  subinput->Delete();
  suboutput->Delete();

  return 1;


}
/*----------------------------------------------------------------*/
int vtkbisWaver::AddDriftTerms(vtkpxMatrix* input,vtkpxMatrix* output,vtkIntArray* breaks,int polorder)
{
  if (input==NULL || output == NULL || breaks==NULL)
    {
      fprintf(stderr,"Bad Input to bisWaver::AddDriftTerms\n");
      return 0;
    }

  if (polorder<-1)
    {
      output->Copy(input);
      return 1;
    }

  if (polorder>3)
    polorder=3;


  int nruns=breaks->GetNumberOfTuples();
  fprintf(stderr,"Breaks = %d : ",nruns);
  for (int i=0;i<nruns;i++)
    fprintf(stderr,"%d ",(int)breaks->GetComponent(i,0));
  fprintf(stderr,"\n");


  int size[2]; input->GetSize(size);
  int numterms=polorder+1;
  int numextra=nruns*numterms;
  int numcols= size[1]+numextra;

  output->Zeros(size[0],numcols);

  for (int r=0;r<size[0];r++)
    for (int c=numextra;c<numcols;c++)
      output->SetDirectElement(r,c,input->GetDirectElement(r,c-numextra));
  //  output->Save("simple_copy_afni.matr");

  for (int i=0;i<nruns;i++)
    {
      int minrow=(int)breaks->GetComponent(i,0);
      int maxrow=size[0]-1;
      if (i<(nruns-1))
	maxrow=int(breaks->GetComponent(i+1,0))-1;
      
      if ( (polorder>-1) && (maxrow>minrow))
	{
	  for (int row=minrow;row<=maxrow;row++)
	    {
	      // Next the Legendre Stuff
	      int mincol=i*numterms;
	      int maxcol=mincol+numterms;

	      double t0=double(minrow);
	      double t1=double(maxrow);
	      double dt=0.5*(t1-t0);
	      double r=double(row);
	      double t=(r-t0)/dt-1.0;
	      double t2=t*t;
	      double t3=t*t2;

	      // This is needed to match AFNI Exactly
	      double afni_offset=0.5/dt;
	      
	      for (int pol=0;pol<numterms;pol++)
		{
		  if (row==minrow)
		    fprintf(stderr,"run=%d (rows=%d:%d) \t\t pol=%d col=%d (mincol=%d)\n",i,minrow,maxrow,pol,mincol+pol,mincol);

		  switch(pol)
		    {
		    case 0: // P_0(x)=1;
		      output->SetDirectElement(row,mincol+pol,1.0); 
		      break;
		    case 1: // P_1(x)=x;
		      output->SetDirectElement(row,mincol+pol,t); 
		      break;
		    case 2: // P_2(x)=0.5*(3x^2-1)
		      output->SetDirectElement(row,mincol+pol,1.5*t2-0.5-afni_offset); 
		      break;
		    case 3: // P_3(x) =0.5*(5x^3-3x)
		      output->SetDirectElement(row,mincol+pol,2.5*t3-1.5*t); 
		      break;
		    }
		}
	    }
	}
    }
  return 1;
}

// ---------------------------------------------------------------------------
// More complex GLM with rates
// ---------------------------------------------------------------------------

int vtkbisWaver::AddDriftAndRateTerms(vtkpxMatrix* input,vtkpxMatrix* output,vtkIntArray* breaks,int polorder,int numrates)
{
  if (numrates<0)
    numrates=0;
  else if (numrates>3)
    numrates=3;


  int ok=vtkbisWaver::AddDriftTerms(input,output,breaks,polorder);

  if (ok==0)
    return 0;

  if (numrates==0)
    return ok;
  
  int insize[2]; input->GetSize(insize);
  int outsize[2]; output->GetSize(outsize);

  vtkpxMatrix* temp=vtkpxMatrix::New();
  temp->Copy(output);

  int outcolumns=outsize[1]+insize[1]*numrates;
  output->Zeros(outsize[0],outcolumns);
  for (int i=0;i<outsize[0];i++)
    for (int j=0;j<outsize[1];j++)
      output->SetDirectElement(i,j,temp->GetDirectElement(i,j));

  // On to creating normalized versions for higher orders .....

  
  int maxdrift=outsize[1]-insize[1];
  for (int task=0;task<insize[1];task++)
    {
      temp->Zeros(outsize[0],1+numrates);
      int tindex=task+maxdrift;
      double magn=0.0;
      for (int i=0;i<outsize[0];i++)
	{
	  double v=output->GetDirectElement(i,tindex);
	  magn+=v*v;
	  temp->SetDirectElement(i,0,v);
	}
      magn=sqrt(magn);
      temp->NormalizeColumns(0,0);
      
      for (int order=1;order<=numrates;order++)
	{
	  double t=double(order);
	  for (int i=0;i<outsize[0];i++)
	    {
	      double x=double(i);
	      temp->SetDirectElement(i,order,pow(x,t)*output->GetDirectElement(i,tindex));
	    }
	  temp->NormalizeColumns(order,order);

	  // Orthogonalize to all previous columns of temp matrix
	  for (int orthog=0;orthog<order;orthog++)
	    {
	      double dot=0.0;
	      for (int i=0;i<outsize[0];i++)
		dot+=(temp->GetDirectElement(i,orthog)*temp->GetDirectElement(i,order));
	      //	      fprintf(stderr,"Orthogonalizing order=%d to column=%d, dot=%.5f\n",order,orthog,dot);

	      for (int i=0;i<outsize[0];i++)
		temp->SetDirectElement(i,order,temp->GetDirectElement(i,order)-dot*temp->GetDirectElement(i,orthog));
	      temp->NormalizeColumns(order,order);
	    }
	  
      int outcol=outsize[1]+(order-1)*insize[1]+task;
	  fprintf(stderr,"Storing rate terms for task=%d (rate=%d) which lies in column=%d --> stored in %d\n",task,order,tindex,outcol);

	  for (int i=0;i<outsize[0];i++)
	    output->SetDirectElement(i,outcol,magn*(temp->GetDirectElement(i,order)));
	}
    }

  return 1;
}

