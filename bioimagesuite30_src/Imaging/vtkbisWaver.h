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





#ifndef __vtkbisWaver_h
#define __vtkbisWaver_h

#include "vtkObject.h"
#include "vtkDoubleArray.h"

class vtkpxMatrix;
class vtkIntArray;

class vtkbisWaver : public vtkObject
{
public:
  static vtkbisWaver *New();
  vtkTypeMacro(vtkbisWaver,vtkObject);


  // Description:
  // Compute Waveform commands

  // Waveform for a single run, input is matrix of 1s and 0s, 1 column per task
  virtual int CreateWaveform(vtkpxMatrix* input,vtkpxMatrix* output);

  // Waveform for multiple runs, input is matrix of 1s and 0s, 1 column per task
    virtual int CreateStackedWaveform(vtkpxMatrix* input,vtkpxMatrix* output,vtkIntArray* breaks);

  // Add drift terms to stacked waveform output, input is output of stacked waveform
  virtual int AddDriftTerms(vtkpxMatrix* input,vtkpxMatrix* output,vtkIntArray* breaks,int polorder=3);

  // Add rate terms (linear, quadratic etc) for tasks
  virtual int AddDriftAndRateTerms(vtkpxMatrix* input,vtkpxMatrix* output,vtkIntArray* breaks,int polorder,int numrates=1);

  // Description:
  // Generate Response Function
  vtkDoubleArray* GenerateResponseFunction();
  vtkpxMatrix*    GenerateResponseFunctionForAllOptions();

  // Description:
  // Status is final result
  vtkGetStringMacro(Status);
  vtkSetStringMacro(Status);



  // Description:
  // Set Parameters for Waver things -- see  Waver documentation for details
  // 0 = Cox Type, 1 = GAM
  vtkGetMacro(Waveform_type,int);
  vtkSetClampMacro(Waveform_type,int,0,5);
  virtual void SetWaveformToNone() { this->SetWaveform_type(0);}
  virtual void SetWaveformToWAV() { this->SetWaveform_type(1);}
  virtual void SetWaveformToGAM() { this->SetWaveform_type(2);}
  virtual void SetWaveformToDoubleGamma() { this->SetWaveform_type(3);}
  virtual void SetWaveformToTripleGamma() { this->SetWaveform_type(4);}
  virtual void SetWaveformToSPMCanonical() { this->SetWaveform_type(5);}


  
  vtkGetMacro(WAV_delay_time,double);
  vtkSetMacro(WAV_delay_time,double);

  vtkGetMacro(WAV_rise_time,double);
  vtkSetMacro(WAV_rise_time,double);

  vtkGetMacro(WAV_fall_time,double);
  vtkSetMacro(WAV_fall_time,double);

  vtkGetMacro(WAV_undershoot,double);
  vtkSetMacro(WAV_undershoot,double);

  vtkGetMacro(WAV_restore_time,double);
  vtkSetMacro(WAV_restore_time,double);

  vtkGetMacro(Peak,double);
  vtkSetMacro(Peak,double);

  vtkGetMacro(TR,double);
  vtkSetMacro(TR,double);

  vtkGetMacro(GAM_power,double);
  vtkSetMacro(GAM_power,double);

  vtkGetMacro(GAM_time,double);
  vtkSetMacro(GAM_time,double);

  vtkGetMacro(GAM_ampl,double);
  vtkSetMacro(GAM_ampl,double);
  
  vtkGetMacro(GAM_delay_time,double);
  vtkSetMacro(GAM_delay_time,double);

protected:

  vtkbisWaver();
  ~vtkbisWaver();
  vtkbisWaver(const vtkbisWaver&) {};
  void operator=(const vtkbisWaver&) {};

  // Set all to defaults
  virtual void ResetParameters();
  virtual double ComputeGammaDuration(double G_power,double G_time);
  virtual double ComputeDuration();


  // Internal Functions from afni waver
  double gammafunction( double t,double gpower,double gtime,double gpeak);
  virtual double ztone( double x ) ;
  virtual double waveform_GAM( double t ) ;
  virtual double waveform_WAV( double t ) ;
  virtual double waveform_doublegamma( double t ) ;
  virtual double waveform_triplegamma( double t ) ;
  virtual double waveform_spm( double t ) ;
  virtual double waveform( double t ) ;


  int    Waveform_type;
  
  double WAV_delay_time;
  double WAV_rise_time;
  double WAV_fall_time;
  double WAV_undershoot;
  double WAV_restore_time;

  double Peak;
  double TR;

  double GAM_power;
  double GAM_time;
  double GAM_ampl;
  double GAM_delay_time;

  char* Status;

};
#endif

