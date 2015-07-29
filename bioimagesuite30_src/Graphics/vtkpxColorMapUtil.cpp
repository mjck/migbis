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
  Module:    $RCSfile: vtkpxColorMapUtil.cpp,v $
  Language:  C++
  Date:      $Date: 2002/05/07 16:07:56 $
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
#include "vtkpxUtil.h"
#include "vtkImageData.h"
#include "vtkLookupTable.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkpxColorMapUtil.h"
#include "vtkDataArray.h"
#include "vtkImageQuantizeRGBToIndex.h"
#include "vtkpxUtil.h"

using namespace bioimagesuite_legacy;

// ---------------------------

vtkpxColorMapUtil* vtkpxColorMapUtil::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxColorMapUtil");
  if(ret)
      {
	return (vtkpxColorMapUtil*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxColorMapUtil;
}


void vtkpxColorMapUtil::SetGammaColorMap(vtkLookupTable *cmap,float gamma,float min_value,float max_value,
				       int num_colors,int vol,int opaque)
{

  if (min_value==max_value)
    ++max_value;
  gamma=Frange(gamma,0.001,100.0);
  if (num_colors!=cmap->GetNumberOfColors())
      cmap->SetNumberOfTableValues(num_colors);
  cmap->SetTableRange(min_value,max_value);

  double gm=1.0/gamma;

  for (int i=0;i<num_colors;i++)
  {
  	double v=(float(i)/float(num_colors-1));

  	float vl=pow(v,gm);

  	if (vol)
  	  cmap->SetTableValue(i,vl,vl,vl,vl);
  	else if (opaque || i>0)
  	  cmap->SetTableValue(i,vl,vl,vl,1.0);
  	else
  	  cmap->SetTableValue(i,vl,vl,vl,0.0);
  }

  
  
}
// ---------------------------------------------------------------------------------------------------------
void vtkpxColorMapUtil::SetConstantHueColorMap(vtkLookupTable* cmap,float intensity,float hue1,float hue2,float cutoff,
					     float min_value,float max_value,int num_colors,int vol,int opaque)
{

  if (num_colors!=cmap->GetNumberOfColors() && num_colors>0)
      cmap->SetNumberOfTableValues(num_colors);
  //  fprintf(stderr,"\t ConstHue Number of Colors=%d\n",num_colors);
  float numc=float(num_colors-1.0);
  float inormal=numc/2.0;
  if (cutoff==0.0)
    cutoff=1.0;


  cmap->SetTableRange(-cutoff,cutoff);


  double fraction=1.0/inormal;

  for (int j=0;j<num_colors;j++)
      {
	float s=float(j-inormal)/inormal;
	float v=intensity;
	float h=hue1;

	if (s<0.0)
	  {
	    s=-s;
	    h=hue2;
	  }
	
	h*=6.0;
	int i=int(h);
	float aa,bb,cc,f,r=0,g=0,b=0;

	f=h-float(i);

	aa= v*(1.0-s);
	bb= v*(1.0- (s*f));
	cc= v*(1.0- (s* (1.0-f)));

	switch (i)
	    {
	    case 0:
	      r=v; g=cc; b=aa;
	      break;
	    case 1:
	      r=bb; g=v; b=aa;
	      break;
	    case 2:
	      r=aa; g=v; b=cc;
	      break;
	    case 3:
	      r=aa; g=bb; b=v;
	      break;
	    case 4:
	      r=cc; g=aa; b=v;
	      break;
	    case 5:
	      r=v;  g=aa; b=bb;
	      break;
	    }

	if (vol)
	    cmap->SetTableValue(j,r,g,b,float(j)/float(num_colors));
	else if (opaque || fabs(s)>fraction)
	  cmap->SetTableValue(j,r,g,b,1.0);
	else
	  cmap->SetTableValue(j,r,g,b,0.0);
      }
}
// ---------------------------------------------------------------------------------------------------------
void vtkpxColorMapUtil::SetRainbowColorMap(vtkLookupTable *cmap,float min,float max,float min_value,float max_value,
				      int num_colors,int vol,int opaque)
{
  double gap=fabs(min_value-max_value)*0.1;

  if (gap<1e-7)
    {
      max=min+1e-7;
      gap=(min_value-max_value)*0.1;
    }

  min=Frange(min,min_value,max_value-gap);
  max=Frange(max,min_value+gap,max_value);

  if (max_value<(min_value+gap))
      max_value+=gap;

  float range=max_value-min_value;

  if (num_colors!=cmap->GetNumberOfColors() && num_colors>0)
      cmap->SetNumberOfTableValues(num_colors);

  cmap->SetTableRange(min_value,max_value);

  float numc=float(num_colors-1);

  int imin=int(0.5+numc*float(min-min_value)/range);
  int imax=int(0.5+numc*float(max-min_value)/range);

  float rgb[3];
  
  for (int i=0;i<num_colors;i++)
    {
      float v=0.0;
      if (i<=imin)
	v=0.0;
      else if (i>=imax)
			v=1.0;
      else
	v=float(i-imin)/float(imax-imin);
      
      float H,S,V;
      //H = (double)i/(double)num_colors * 340.0;
      H = v * 340.0;
      S = V = 1.0;
      vtkpxColorMapUtil::HSVtoRGB(&rgb[0], &rgb[1], &rgb[2], H,S,V);
      
      if(vol)
	cmap->SetTableValue(i, rgb[0], rgb[1], rgb[2], .5 *v);
      else if (opaque || i>0)
	cmap->SetTableValue(i, rgb[0], rgb[1], rgb[2], 1.0);
      else 
      	cmap->SetTableValue(i, rgb[0], rgb[1], rgb[2], 0.0);
    }
}

// ---------------------------------------------------------------------------------------------------------
void vtkpxColorMapUtil::SetReverseRainbowColorMap(vtkLookupTable *cmap,float min,float max,float min_value,float max_value,
				      int num_colors,int vol,int opaque)
{
  double gap=fabs(min_value-max_value)*0.1;

  if (gap<1e-7)
    {
      max=min+1e-7;
      gap=(min_value-max_value)*0.1;
    }

  min=Frange(min,min_value,max_value-gap);
  max=Frange(max,min_value+gap,max_value);

  if (max_value<(min_value+gap))
    max_value+=gap;
  
  float range=max_value-min_value;

  if (num_colors!=cmap->GetNumberOfColors() && num_colors>0)
      cmap->SetNumberOfTableValues(num_colors);

  cmap->SetTableRange(min_value,max_value);

  float numc=float(num_colors-1);

  int imin=int(0.5+numc*float(min-min_value)/range);
  int imax=int(0.5+numc*float(max-min_value)/range);

  float rgb[3];

  for (int i=0;i<num_colors;i++)
    {
      float v=0.0;
      if (i<=imin)
	v=0.0;
      else if (i>=imax)
	v=1.0;
      else
	v=float(i-imin)/float(imax-imin);
      
      float H,S,V;
      //H = 340.0 - ((double)i/(double)num_colors * 340.0);
      H = v * 340.0;
      S = V = 1.0;
      vtkpxColorMapUtil::HSVtoRGB(&rgb[0], &rgb[1], &rgb[2], H,S,V);
      
      if(vol)
	cmap->SetTableValue(i, rgb[0], rgb[1], rgb[2], .5 *v);
      else if (opaque || i>0)
	cmap->SetTableValue(i, rgb[0], rgb[1], rgb[2], 1.0);
      else
	cmap->SetTableValue(i, rgb[0], rgb[1], rgb[2], 0.0);
    }
}

// ---------------------------------------------------------------------------------------------------------
void vtkpxColorMapUtil::SetBlackBodyColorMap(vtkLookupTable *cmap,float min,float max,float min_value,float max_value,
				      int num_colors,int vol,int opaque)
{
  double gap=fabs(min_value-max_value)*0.1;

  if (gap<1e-7)
    {
      max=min+1e-7;
      gap=(min_value-max_value)*0.1;
    }

  min=Frange(min,min_value,max_value-gap);
  max=Frange(max,min_value+gap,max_value);

  if (max_value<(min_value+gap))
      max_value+=gap;

  float range=max_value-min_value;

  if (num_colors!=cmap->GetNumberOfColors() && num_colors>0)
      cmap->SetNumberOfTableValues(num_colors);

  cmap->SetTableRange(min_value,max_value);

  float numc=float(num_colors-1);

  int imin=int(0.5+numc*float(min-min_value)/range);
  int imax=int(0.5+numc*float(max-min_value)/range);
  
  float rgb[3], rgbmin[3], rgbmax[3]; ;
  float blackbody[9][3] = {{255, 255, 229} ,
			   {255, 247, 188},
			   {254, 227, 145},
			   {254, 196, 79},
			   {254, 153, 41},
			   {236, 112, 20},
			   {204, 76, 2},
			   {153, 52, 4},
			   {102, 37, 6}};
    
    
    for (int i=0;i<num_colors;i++)
      {
	float v=0.0;
	if (i<=imin)
	  v=0.0;
	else if (i>=imax)
	  v=1.0;
	else
	  v=float(i-imin)/float(imax-imin);
	
	int table_index_min = int(floor(8.0 * (1.0-v)));
	int table_index_max = int(ceil(8.0 * (1.0-v)));
	
	float actual_value = (8.0 * (1.0-v)) - table_index_min;
	
	rgbmin[0] = blackbody[table_index_min][0]/255.0f;
	rgbmin[1] = blackbody[table_index_min][1]/255.0f;
	rgbmin[2] = blackbody[table_index_min][2]/255.0f;
	
	rgbmax[0] = blackbody[table_index_max][0]/255.0f;
	rgbmax[1] = blackbody[table_index_max][1]/255.0f;
	rgbmax[2] = blackbody[table_index_max][2]/255.0f;
	
	rgb[0] = rgbmin[0] + actual_value * (rgbmax[0] - rgbmin[0]);
	rgb[1] = rgbmin[1] + actual_value * (rgbmax[1] - rgbmin[1]);
	rgb[2] = rgbmin[2] + actual_value * (rgbmax[2] - rgbmin[2]);
	
	if(vol)
	  cmap->SetTableValue(i, rgb[0], rgb[1], rgb[2], .5 *v);
	else if (opaque || i>0)
	  cmap->SetTableValue(i, rgb[0], rgb[1], rgb[2], 1.0);
	else
	  cmap->SetTableValue(i, rgb[0], rgb[1], rgb[2], 0.0);
      }
}
// ---------------------------------------------------------------------------------------------------------
void vtkpxColorMapUtil::SetStepColorMap(vtkLookupTable *cmap,float min,float max,float min_value,float max_value,
				      int num_colors,int vol,int opaque)
{
  double gap=fabs(min_value-max_value)*0.1;
  //  fprintf(stderr,"Gap=%f min=%f,max=%f,minv=%f maxv=%f\n",gap,min,max,min_value,max_value);
  if (gap<1e-7)
    {
      max=min+1e-7;
      gap=(min_value-max_value)*0.1;
    }
  //  fprintf(stderr,"New Gap=%f min=%f,max=%f,minv=%f maxv=%f\n",gap,min,max,min_value,max_value);
  min=Frange(min,min_value,max_value-gap);
  max=Frange(max,min_value+gap,max_value);

  if (max_value<(min_value+gap))
      max_value+=gap;

  float range=max_value-min_value;

  if (num_colors!=cmap->GetNumberOfColors() && num_colors>0)
      cmap->SetNumberOfTableValues(num_colors);
  //  fprintf(stderr,"\t Step Number of Colors=%d\n",num_colors);
  cmap->SetTableRange(min_value,max_value);

  float numc=float(num_colors-1);

  int imin=int(0.5+numc*float(min-min_value)/range);
  int imax=int(0.5+numc*float(max-min_value)/range);

   // fprintf(stderr,"imin=%d imax=%d num_colors=%d\n",imin,imax,num_colors);

  for (int i=0;i<num_colors;i++)
  {
    float v=0.0;
    if (i<=imin)
      v=0.0;
    else if (i>=imax)
      v=1.0;
    else
      v=float(i-imin)/float(imax-imin);
    if (vol)
      cmap->SetTableValue(i,v,v,v,v*0.5);
    else if (opaque || v>0.0)
      cmap->SetTableValue(i,v,v,v,1.0);
    else
      cmap->SetTableValue(i,v,v,v,0.0);
  }
}
//------------------------------------------------------------------------------
void vtkpxColorMapUtil::CreateOverlayColorMap(vtkLookupTable *cmap,int mode,float min,float max,float min_value,float max_value,
					      float intensity,float hue1,float hue2,int vol,int updatefunction,int opaque)
{
  int numc=cmap->GetNumberOfColors();

  if (mode!=64)
    mode=256;

  if (numc!=mode)
    {
      numc=mode;
      cmap->SetNumberOfTableValues(numc);
    }

  int midrange=192;
  int crange=32;

  if (numc==64)
    {
      midrange=56;
      crange=4;
    }

  int imin=int(0.5+numc*float(min-min_value)/float(midrange));
  int imax=int(0.5+numc*float(max-min_value)/float(midrange));


  cmap->SetTableRange(0,numc-1);

  // Bottom Grayscale range
  for (int i=0;i<midrange;i++)
    {
	float v=0.0;
	if (i<=imin)
	  v=0.0;
	else if (i>=imax)
	  v=1.0;
	else
	  v=float(i-imin)/float(imax-imin);

	if (vol)
	  cmap->SetTableValue(i,v,v,v,v*0.5);
	else if (opaque || v>0.0)
	  cmap->SetTableValue(i,v,v,v,1.0);
	else
	  cmap->SetTableValue(i,v,v,v,0.0);
    }


  if (updatefunction==1)
    {
      for (int j=0;j<2*crange;j++)
	{
	  float s=0.5*float(j-crange)/float(crange);
	  float v=intensity;
	  float h=hue1;

	  if (s<0.0)
	    {
	      s=-s;
	      h=hue2;
	    }

	  if (s<0.0)
	    s-=0.5;
	  else
	    s+=0.5;

	  h*=6.0;
	  int i=int(h);
	  float aa,bb,cc,f,r=0,g=0,b=0;

	  f=h-float(i);

	  aa= v*(1.0-s);
	  bb= v*(1.0- (s*f));
	  cc= v*(1.0- (s* (1.0-f)));

	  switch (i)
	    {
	    case 0:
	      r=v; g=cc; b=aa;
	      break;
	    case 1:
	      r=bb; g=v; b=aa;
	      break;
	    case 2:
	      r=aa; g=v; b=cc;
	      break;
	    case 3:
	      r=aa; g=bb; b=v;
	      break;
	    case 4:
	      r=cc; g=aa; b=v;
	      break;
	    case 5:
	      r=v;  g=aa; b=bb;
	      break;
	    }

	  if (vol)
	    cmap->SetTableValue(j+midrange,r,g,b,fabs(s));
	  else
	    cmap->SetTableValue(j+midrange,r,g,b,1.0);
	}
    }
}
//------------------------------------------------------------------------------
void vtkpxColorMapUtil::AutoUpdateColormapRange(vtkLookupTable* cmap,vtkImageData* img,int opaque)
{
  if (img==NULL)
      return;

  double range[2];
  img->GetPointData()->GetScalars()->GetRange(range);
  //  fprintf(stderr,"Image Range %f:%f\n",range[0],range[1]);

  //if (range[0]>=0.0)
  vtkpxColorMapUtil::SetStepColorMap(cmap,range[0],range[1],range[0],range[1],256,opaque);
  /*else
    vtkpxColorMapUtil::SetConstantHueColorMap(cmap,0.5,0.02,0.58,
			     Fmax(fabs(range[0]),fabs(range[1])),
			     range[0],range[1],256);*/
}
//------------------------------------------------------------------------------
void vtkpxColorMapUtil::ComputeStepBounds(vtkImageData* image,vtkDataArray* output,int np,float nsigma)
{
  if (image==NULL || output == NULL)
    return;

  nsigma=Frange(nsigma,0.01,10.0);

  //  output->SetDataType(VTK_FLOAT);
  output->SetNumberOfTuples(2);

  double range[2];
  vtkDataArray* scal=image->GetPointData()->GetScalars();
  scal->GetRange(range);
  int n=scal->GetNumberOfTuples(),inc=1;
  if (n>np)
    inc=int(n/np);

  float mean=0.0,mean2=0.0;
  int num=0;
  for (int i=0;i<n;i+=inc)
    {
      float v=scal->GetComponent(i,0);
      if (v!=0.0)
	{
	  mean+=v;
	  mean2+=v*v;
	  ++num;
	}
    }

  if (num==0)
    return;

  mean/=float(num);
  mean2/=float(num);
  float std=sqrt(mean2-mean*mean);

  output->SetComponent(0,0,Frange(mean-nsigma*std,range[0],range[1]-1.0));
  output->SetComponent(1,0,Frange(mean+nsigma*std,range[0]+1.0,range[1]));
}

void vtkpxColorMapUtil::HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
	int i;
	float f, p, q, t;

	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}

	h /= 60;			// sector 0 to 5
	i = int(floor( h ));
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}

}



//------------------------------------------------------------------------------
 // r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//              if s == 0, then h = -1 (undefined)

void vtkpxColorMapUtil::RGBtoHSV( float r, float g, float b, float &h, float &s, float &v )
{
  float min, max, delta;

  min = Fmin(r,Fmin(g,b));
  max = Fmax(r,Fmax(g,b));
  v = max;                               // v

  delta = max - min;

  if( max != 0 )
      {
	s = delta / max;               // s
      }
  else
      {
	// r = g = b = 0                // s = 0, v is undefined
	s = 0;
	h = -1;
	return;
      }

  if( r == max )
      h = ( g - b ) / delta;         // between yellow & magenta
  else if( g == max )
      h = 2 + ( b - r ) / delta;     // between cyan & yellow
  else
      h = 4 + ( r - g ) / delta;     // between magenta & cyan

  h = 60;                               // degrees
  if( h < 0 )
      h += 360;
  h=h/360.0;

}

int vtkpxColorMapUtil::QuantizeColorImage(vtkImageData* destination,vtkImageData* source,vtkLookupTable* cmap)
{
  vtkImageQuantizeRGBToIndex* quantizeFilter=vtkImageQuantizeRGBToIndex::New();
  quantizeFilter->SetInput(source);
  quantizeFilter->SetNumberOfColors(2048);
  quantizeFilter->Update();

  vtkLookupTable* oldlookup=quantizeFilter->GetLookupTable();
  cmap->DeepCopy(oldlookup);
  destination->ShallowCopy(quantizeFilter->GetOutput());

  fprintf(stderr,"Done with quantize\n");

  return 1;
  /*

  int numc=oldlookup->GetNumberOfColors();
  cmap->SetNumberOfTableValues(numc);
  double dat[2];
  oldlookup->GetTableRange(dat);
  cmap->SetTableRange(dat);

		      minval=V;
		      best=c2;
		    }
		  else if (H==minhue)
		    {
		      if (V<minval)
			{
			  minval=V;
			  best=c2;
			}
		    }
		}
	    }
	map[best]=color;
	double val[4];
	oldlookup->GetTableValue(best,val);
	cmap->SetTableValue(color,val);
	done[best]=1;
      }

  destination->ShallowCopy(quantizeFilter->GetOutput());

  vtkDataArray* destScalars=destination->GetPointData()->GetScalars();
  for (int j=0;j<destScalars->GetNumberOfTuples();j++)
    {
      int v=(int)destScalars->GetComponent(j,0);
      destScalars->SetComponent(j,0,map[v]);
    }
  quantizeFilter->Delete();
  delete [] map;
  delete [] done;
  return 1;*/
}

void vtkpxColorMapUtil::SetFmriMergeColormap(vtkLookupTable* cmap,int mode,int vol,int opaque)
{
  // Mode 0 is old default 64 colors no flips
  // Mode 1 is new 256 colors
  // Mode 2 is new 256 colors flipped
  // Mode 3 is Spect 64
  // Mode 4 is Spect 256
  // Mode 5 is Pawel
  // Mode 6 is Pawl flipped
  // Mode 7 is Rainbow color map
  mode=Irange(mode,0,6);

  if (mode==3 || mode ==4)
    {
      // Mode 3 is spect 64   colors green/blue
      // Mode 4 is spect 256  colors green/blue
      switch (mode)
	{
	case 3:
	  CreateOverlayColorMap(cmap,64,0.0,55.0,0.0,55.0,1.0,0.35,0.58,vol,opaque);
	  break;
	case 4:
	  CreateOverlayColorMap(cmap,256,0.0,191.0,0.0,191.0,1.0,0.35,0.58,vol,opaque);
	  break;


	}
      return;
    }

  int numc=64;
  int numanat=56;
  if (mode==1 || mode ==2)
    {
      numc=256;
      numanat=192;
    }

  if (mode==5 || mode ==6)
    {
      numc=256;
      numanat=238;
    }


  if (cmap->GetNumberOfColors()!=numc)
    cmap->SetNumberOfTableValues(numc);
  cmap->SetTableRange(0,numc-1);

  if (vol==0)
    {
      for (int i=0;i<numanat;i++)
	{
	  float v=float(i)/float(numanat-1);
	  if (opaque || i>0)
	    cmap->SetTableValue(i,v,v,v,1.0);
	  else
	    cmap->SetTableValue(i,v,v,v,0.0);
	}
    }
  else if (vol==1)
    {
      for (int i=0;i<numanat;i++)
	{
	  float v=0.75*float(i)/float(numanat-1);
	  float vt=0.25*float(i)/float(numanat-1);
	  if (i>5)
	    v+=0.2;
	  cmap->SetTableValue(i,v,v,v,vt);
	}
    }
  else if (vol==2)
    {
      for (int i=0;i<numanat;i++)
	{
	  float v=0.5*float(i)/float(numanat-1);
	  float v2=0.0;
	  if (i>5)
	    v2=0.1*v+0.7;

	  cmap->SetTableValue(i,v2,v2,v2,v);
	}
    }

  if (mode == 0 )
    {
      if (vol>0)
	{
	  // Matlab based cool(4), then hot(4)
	  cmap->SetTableValue(56, 0      , 1.0000 , 1.0000,1.00);
	  cmap->SetTableValue(57, 0.3333 , 0.6667 , 1.0000,0.95);
	  cmap->SetTableValue(58, 0.6667 , 0.3333 , 1.0000,0.90);
	  cmap->SetTableValue(59, 1.0000 , 0      , 1.0000,0.85);
	  cmap->SetTableValue(60, 1.0000 , 0      , 0     ,0.85);
	  cmap->SetTableValue(61, 1.0000 , 1.0000 , 0.    ,0.90 );
	  cmap->SetTableValue(62, 1.0000 , 1.0000 , 0.3333,0.95);
	  cmap->SetTableValue(63, 1.0000 , 1.0000 , 0.6667,1.00);
	}
      else
	{
	  cmap->SetTableValue(56, 0      , 1.0000 , 1.0000);
	  cmap->SetTableValue(57, 0.3333 , 0.6667 , 1.0000);
	  cmap->SetTableValue(58, 0.6667 , 0.3333 , 1.0000);
	  cmap->SetTableValue(59, 1.0000 , 0      , 1.0000);
	  cmap->SetTableValue(60, 1.0000 , 0      , 0     );
	  cmap->SetTableValue(61, 1.0000 , 1.0000 , 0.    );
	  cmap->SetTableValue(62, 1.0000 , 1.0000 , 0.3333);
	  cmap->SetTableValue(63, 1.0000 , 1.0000 , 0.6667);
	}
    }
  else if (mode == 1)
    {
      for (int i=0;i<32;i++)
	{
	  if (vol>0)
	    cmap->SetTableValue(223-i,float(i)*0.032258,0.0,1.0,
				0.66+float(32-i)/32.0*0.34);
	  else
	    cmap->SetTableValue(223-i,float(i)*0.032258,0.0,1.0,1.0);
	}

      // set hot temp ramp - 32 tones from red to yellow
      for (int i=0;i<32;i++)
	{
	  if (vol>0)
	    {
	      if (i <= 24)
		cmap->SetTableValue(224+i,1.0,float(i)*0.032258,0.0,0.66+float(i)/32.0*0.34);
	      else
		cmap->SetTableValue(224+i,1.0,float(i)*0.032258,(8-32+i)*0.125,0.66+float(i)/32.0*0.34);
	    }
	  else
	    {
	      if (i<8)
		{
		  double frac=double(i)/8.0;
		  cmap->SetTableValue(224+i, 1.0000 , frac      , 0     );
		}
	      else
		{
		  double frac=double(i-8)/24.0*0.667;
		  cmap->SetTableValue(224+i, 1.0000 , 1.0000 , frac    );
		}
	      /*
	      if (i <= 24)
		cmap->SetTableValue(224+i,1.0,float(i)*0.032258,0.0,1.0);
	      else
	      cmap->SetTableValue(224+i,1.0,float(i)*0.032258,(8-32+i)*0.125,1.0);*/
	    }
	}
    }
  else if (mode == 2 )
    {
      for (int i=0;i<32;i++)
	{
	  if (vol>0)
	    cmap->SetTableValue(224+i,float(i)*0.032258,0.0,1.0,
				    0.66+float(32-i)/32.0*0.34);
	  else
	    cmap->SetTableValue(224+i,float(i)*0.032258,0.0,1.0,1.0);
	}

      // set hot temp ramp - 32 tones from red to yellow
      for (int i=0;i<32;i++)
	{
	  if (vol>0)
	    {
	      if (i <= 24)
		cmap->SetTableValue(223-i,1.0,float(i)*0.032258,0.0,0.66+float(i)/32.0*0.34);
	      else
		cmap->SetTableValue(223-i,1.0,float(i)*0.032258,(8-32+i)*0.125,0.66+float(i)/32.0*0.34);
	    }
	  else
	    {
	      if (i <= 24)
		cmap->SetTableValue(223-i,1.0,float(i)*0.032258,0.0,1.0);
	      else
		cmap->SetTableValue(223-i,1.0,float(i)*0.032258,(8-32+i)*0.125,1.0);
	    }
	}
    }
  else if (mode == 5 )
    {
      if (vol>0)
	{
	  // Matlab based cool(4), then hot(4)
	  cmap->SetTableValue(238,  51.0/255.0, 204.0/255.0, 1.0,0.99);
	  cmap->SetTableValue(239,  68.0/255.0, 187.0/255.0, 1.0,0.97);
	  cmap->SetTableValue(240,  85.0/255.0, 170.0/255.0, 1.0,0.95);
	  cmap->SetTableValue(241, 102.0/255.0, 153.0/255.0, 1.0,0.93);
	  cmap->SetTableValue(242, 119.0/255.0, 136.0/255.0, 1.0,0.91);
	  cmap->SetTableValue(243, 136.0/255.0, 119.0/255.0, 1.0,0.89);
	  cmap->SetTableValue(244, 153.0/255.0, 102.0/255.0, 1.0,0.87);
	  cmap->SetTableValue(245, 170.0/255.0,  85.0/255.0, 1.0,0.85);
	  cmap->SetTableValue(246, 187.0/255.0,  68.0/255.0, 1.0,0.83);

	  cmap->SetTableValue(247, 175.0/255.0,         0.0, 0.0,0.83);
	  cmap->SetTableValue(248, 204.0/255.0,         0.0, 0.0,0.85);
	  cmap->SetTableValue(249, 255.0/255.0,         0.0, 0.0,0.87);
	  cmap->SetTableValue(250, 255.0/255.0,  51.0/255.0, 0.0,0.89);
	  cmap->SetTableValue(251, 255.0/255.0, 102.0/255.0, 0.0,0.91);
	  cmap->SetTableValue(252, 255.0/255.0, 158.0/255.0, 0.0,0.93);
	  cmap->SetTableValue(253, 255.0/255.0, 204.0/255.0, 0.0,0.95);
	  cmap->SetTableValue(254, 255.0/255.0, 255.0/255.0, 0.0,0.97);
	  cmap->SetTableValue(255, 255.0/255.0, 255.0/255.0, 68.0/255.0,0.99);
	}
      else
	{
	  cmap->SetTableValue(238,  51.0/255.0, 204.0/255.0, 1.0);
	  cmap->SetTableValue(239,  68.0/255.0, 187.0/255.0, 1.0);
	  cmap->SetTableValue(240,  85.0/255.0, 170.0/255.0, 1.0);
	  cmap->SetTableValue(241, 102.0/255.0, 153.0/255.0, 1.0);
	  cmap->SetTableValue(242, 119.0/255.0, 136.0/255.0, 1.0);
	  cmap->SetTableValue(243, 136.0/255.0, 119.0/255.0, 1.0);
	  cmap->SetTableValue(244, 153.0/255.0, 102.0/255.0, 1.0);
	  cmap->SetTableValue(245, 170.0/255.0,  85.0/255.0, 1.0);
	  cmap->SetTableValue(246, 187.0/255.0,  68.0/255.0, 1.0);

	  cmap->SetTableValue(247, 175.0/255.0,         0.0, 0.0);
	  cmap->SetTableValue(248, 204.0/255.0,         0.0, 0.0);
	  cmap->SetTableValue(249, 255.0/255.0,         0.0, 0.0);
	  cmap->SetTableValue(250, 255.0/255.0,  51.0/255.0, 0.0);
	  cmap->SetTableValue(251, 255.0/255.0, 102.0/255.0, 0.0);
	  cmap->SetTableValue(252, 255.0/255.0, 158.0/255.0, 0.0);
	  cmap->SetTableValue(253, 255.0/255.0, 204.0/255.0, 0.0);
	  cmap->SetTableValue(254, 255.0/255.0, 255.0/255.0, 0.0);
	  cmap->SetTableValue(255, 255.0/255.0, 255.0/255.0, 68.0/255.0);
	}
    }
  else if (mode == 6 )
    {
      if (vol>0)
	{
	  // Matlab based cool(4), then hot(4)
	  cmap->SetTableValue(255,  51.0/255.0, 204.0/255.0, 1.0,0.99);
	  cmap->SetTableValue(254,  68.0/255.0, 187.0/255.0, 1.0,0.97);
	  cmap->SetTableValue(253,  85.0/255.0, 170.0/255.0, 1.0,0.95);
	  cmap->SetTableValue(252, 102.0/255.0, 153.0/255.0, 1.0,0.93);
	  cmap->SetTableValue(251, 119.0/255.0, 136.0/255.0, 1.0,0.91);
	  cmap->SetTableValue(250, 136.0/255.0, 119.0/255.0, 1.0,0.89);
	  cmap->SetTableValue(249, 153.0/255.0, 102.0/255.0, 1.0,0.87);
	  cmap->SetTableValue(248, 170.0/255.0,  85.0/255.0, 1.0,0.85);
	  cmap->SetTableValue(247, 187.0/255.0,  68.0/255.0, 1.0,0.83);

	  cmap->SetTableValue(246, 175.0/255.0,         0.0, 0.0,0.83);
	  cmap->SetTableValue(245, 204.0/255.0,         0.0, 0.0,0.85);
	  cmap->SetTableValue(244, 255.0/255.0,         0.0, 0.0,0.87);
	  cmap->SetTableValue(243, 255.0/255.0,  51.0/255.0, 0.0,0.89);
	  cmap->SetTableValue(242, 255.0/255.0, 102.0/255.0, 0.0,0.91);
	  cmap->SetTableValue(241, 255.0/255.0, 158.0/255.0, 0.0,0.93);
	  cmap->SetTableValue(240, 255.0/255.0, 204.0/255.0, 0.0,0.95);
	  cmap->SetTableValue(239, 255.0/255.0, 255.0/255.0, 0.0,0.97);
	  cmap->SetTableValue(238, 255.0/255.0, 255.0/255.0, 68.0/255.0,0.99);
	}
      else
	{
	  cmap->SetTableValue(255,  51.0/255.0, 204.0/255.0, 1.0);
	  cmap->SetTableValue(254,  68.0/255.0, 187.0/255.0, 1.0);
	  cmap->SetTableValue(253,  85.0/255.0, 170.0/255.0, 1.0);
	  cmap->SetTableValue(252, 102.0/255.0, 153.0/255.0, 1.0);
	  cmap->SetTableValue(251, 119.0/255.0, 136.0/255.0, 1.0);
	  cmap->SetTableValue(250, 136.0/255.0, 119.0/255.0, 1.0);
	  cmap->SetTableValue(249, 153.0/255.0, 102.0/255.0, 1.0);
	  cmap->SetTableValue(248, 170.0/255.0,  85.0/255.0, 1.0);
	  cmap->SetTableValue(247, 187.0/255.0,  68.0/255.0, 1.0);

	  cmap->SetTableValue(246, 175.0/255.0,         0.0, 0.0);
	  cmap->SetTableValue(245, 204.0/255.0,         0.0, 0.0);
	  cmap->SetTableValue(244, 255.0/255.0,         0.0, 0.0);
	  cmap->SetTableValue(243, 255.0/255.0,  51.0/255.0, 0.0);
	  cmap->SetTableValue(242, 255.0/255.0, 102.0/255.0, 0.0);
	  cmap->SetTableValue(241, 255.0/255.0, 158.0/255.0, 0.0);
	  cmap->SetTableValue(240, 255.0/255.0, 204.0/255.0, 0.0);
	  cmap->SetTableValue(239, 255.0/255.0, 255.0/255.0, 0.0);
	  cmap->SetTableValue(238, 255.0/255.0, 255.0/255.0, 68.0/255.0);
	}
    }
}


void vtkpxColorMapUtil::SetScalarBarColormap(vtkLookupTable* cmap,int mode,double minv,double maxv,int positive)
{
  // Mode 0 is old default 64 colors no flips
  // Mode 1 is new 256 colors
  // Mode 2 is new 256 colors flipped
  // Mode 3 is Spect 64
  // Mode 4 is Spect 256
  // Mode 5 is Pawel
  // Mode 6 is Pawl flipped
  mode=Irange(mode,0,6);

  if (positive>=1)
    positive=1;
  else
    positive=0;

  minv=fabs(minv);
  maxv=fabs(maxv);
  if (minv==maxv)
    maxv=minv+1.0;
  if (minv>maxv)
    {
      double a=minv;
      minv=maxv;
      maxv=a;
    }

  cmap->SetTableRange(minv,maxv);

  if (mode==2)
    {
      positive=1-positive;
      mode=1;
    }

  if (mode==6)
    {
      positive=1-positive;
      mode=5;
    }

  if (mode == 0 )
    {
      cmap->SetNumberOfTableValues(4);
      if (!positive)
	{
	  cmap->SetTableValue(3, 0      , 1.0000 , 1.0000);
	  cmap->SetTableValue(2, 0.3333 , 0.6667 , 1.0000);
	  cmap->SetTableValue(1, 0.6667 , 0.3333 , 1.0000);
	  cmap->SetTableValue(0, 1.0000 , 0      , 1.0000);
	}
      else
	{
	  cmap->SetTableValue(0, 1.0000 , 0      , 0     );
	  cmap->SetTableValue(1, 1.0000 , 1.0000 , 0.    );
	  cmap->SetTableValue(2, 1.0000 , 1.0000 , 0.3333);
	  cmap->SetTableValue(3, 1.0000 , 1.0000 , 0.6667);
	}
    }
  else if (mode == 1)
    {
      cmap->SetNumberOfTableValues(32);
      if (!positive)
	{
	  for (int i=0;i<32;i++)
	    cmap->SetTableValue(i,float(i)*0.032258,0.0,1.0,1.0);
	}
      else
	{
	  for (int i=0;i<32;i++)
	    {
	      if (i<8)
		{
		  double frac=double(i)/8.0;
		  cmap->SetTableValue(i, 1.0000 , frac      , 0     );
		}
	      else
		{
		  double frac=double(i-8)/24.0*0.667;
		  cmap->SetTableValue(i, 1.0000 , 1.0000 , frac    );
		}
	    }
	}
    }
  else if (mode == 5 )
    {
      cmap->SetNumberOfTableValues(9);
      if (!positive)
	{
	  cmap->SetTableValue(0,  51.0/255.0, 204.0/255.0, 1.0,0.99);
	  cmap->SetTableValue(1,  68.0/255.0, 187.0/255.0, 1.0,0.97);
	  cmap->SetTableValue(2,  85.0/255.0, 170.0/255.0, 1.0,0.95);
	  cmap->SetTableValue(3, 102.0/255.0, 153.0/255.0, 1.0,0.93);
	  cmap->SetTableValue(4, 119.0/255.0, 136.0/255.0, 1.0,0.91);
	  cmap->SetTableValue(5, 136.0/255.0, 119.0/255.0, 1.0,0.89);
	  cmap->SetTableValue(6, 153.0/255.0, 102.0/255.0, 1.0,0.87);
	  cmap->SetTableValue(7, 170.0/255.0,  85.0/255.0, 1.0,0.85);
	  cmap->SetTableValue(8, 187.0/255.0,  68.0/255.0, 1.0,0.83);
	}
      else
	{
	  cmap->SetTableValue(0, 175.0/255.0,         0.0, 0.0,0.83);
	  cmap->SetTableValue(1, 204.0/255.0,         0.0, 0.0,0.85);
	  cmap->SetTableValue(2, 255.0/255.0,         0.0, 0.0,0.87);
	  cmap->SetTableValue(3, 255.0/255.0,  51.0/255.0, 0.0,0.89);
	  cmap->SetTableValue(4, 255.0/255.0, 102.0/255.0, 0.0,0.91);
	  cmap->SetTableValue(5, 255.0/255.0, 158.0/255.0, 0.0,0.93);
	  cmap->SetTableValue(6, 255.0/255.0, 204.0/255.0, 0.0,0.95);
	  cmap->SetTableValue(7, 255.0/255.0, 255.0/255.0, 0.0,0.97);
	  cmap->SetTableValue(8, 255.0/255.0, 255.0/255.0, 68.0/255.0,0.99);
	}
    }
  else if (mode==3 || mode ==4)
    {
      // Mode 3 is spect 64   colors green/blue
      // Mode 4 is spect 256  colors green/blue
      vtkLookupTable* tmp=vtkLookupTable::New();
      int numc=4,maxc=64;
      switch (mode)
	{
	case 3:
	  CreateOverlayColorMap(tmp,64,0.0,55.0,0.0,55.0,1.0,0.35,0.58,0);
	  break;
	case 4:
	  numc=32;
	  maxc=256;
	  CreateOverlayColorMap(tmp,256,0.0,191.0,0.0,191.0,1.0,0.35,0.58,0);
	  break;
	}

      cmap->SetNumberOfTableValues(numc);
      for (int i=0;i<numc;i++)
	{
	  int index=(maxc-numc)+i;
	  if (!positive)
	    index=(maxc-numc-1)-i;
	  double val[4]; tmp->GetTableValue(index,val);
	  cmap->SetTableValue(i,val);
	}
     }
}


void vtkpxColorMapUtil::CopyLookupTable(vtkLookupTable* in,vtkLookupTable* out)
{
  if (in==NULL || out ==NULL)
    {
      fprintf(stderr,"Bad LookupTable to copy\n");
      return;
    }
  //  fprintf(stderr,"Copying Lookup Table ....");
  out->DeepCopy(in);
  //  fprintf(stderr,"done\n");
  return;
  /*  out->SetNumberOfTableValues(in->GetNumberOfColors());
  out->SetTableRange(in->GetTableRange());


  for (int int i=0;i<out->GetNumberOfColors();i++)
  out->SetTableValue(i,in->GetTableValue(i));*/
}

// ---------------------------------------------------------------------------------------------------------
int vtkpxColorMapUtil::LoadColormap(vtkLookupTable* colormap,const char* name)
{
  if (colormap==NULL)
      return 0;

  gzFile fin=gzsuffixopen(name,"rb");
  if (!fin)
    return 0;

  char line[102];
  gzgets(fin,line,100);
  if (gzfindstringinstring(line,"#IPAGRGBAColormap")==0)
      {
	gzclose(fin);
	return 0;
      }

  gzgets(fin,line,100);  gzgets(fin,line,100);
  int numc;
  sscanf(line,"%d",&numc);
  if (numc!=colormap->GetNumberOfColors())
      colormap->SetNumberOfTableValues(numc);

  double dat[2];
  gzgets(fin,line,100);  gzgets(fin,line,100);
  sscanf(line,"%lf %lf",&dat[0],&dat[1]);
  colormap->SetTableRange(dat);

  gzgets(fin,line,100);  gzgets(fin,line,100);
  for (int i=0;i<colormap->GetNumberOfColors();i++)
      {
	double val[4];
	int tmp;
	gzgets(fin,line,100);
	sscanf(line,"%d %lf %lf %lf %lf",&tmp,&val[0],&val[1],&val[2],&val[3]);
	colormap->SetTableValue(tmp,val);
      }
  gzclose(fin);
  return 1;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxColorMapUtil::SaveColormap(vtkLookupTable* colormap,const char* name)
{
  if (colormap==NULL)
      return 0;

  FILE* fout=fopen(name,"w");

  if (!fout)
      return 0;

  fprintf(fout,"#IPAGRGBAColormap\n");
  fprintf(fout,"#Number of Colors\n%d\n",colormap->GetNumberOfColors());

  double dat[2];
  colormap->GetTableRange(dat);
  fprintf(fout,"#Range\n %6.3f %6.3f \n",dat[0],dat[1]);

  fprintf(fout,"#Colors\n#Index  R     G     B  Alpha\n");
  for (int i=0;i<colormap->GetNumberOfColors();i++)
      {
	double val[4];
	colormap->GetTableValue(i,val);
	fprintf(fout,"%4d %5.3f %5.3f %5.3f %5.3f\n",i,val[0],val[1],val[2],val[3]);
      }
  fclose(fout);
  return 1;

}
// ---------------------------------------------------------------------------------------------------------
int vtkpxColorMapUtil::LoadAnalyzeColormap(vtkLookupTable* colormap,const char* name,int volume_mode)
{
  if (colormap==NULL)
    return 0;

  FILE* fin=fopen(name,"r");
  if (!fin)
      return 0;

  double colors[256][3];
  for (int i=0;i<255;i++)
      for (int j=0;j<3;j++)
	  {
	    colors[i][j]=double(i)/double(255);
	    fscanf(fin,"%lf ",&colors[i][j]);
	    colors[i][j]/=255.0;
	    colors[i][j]=Frange(colors[i][j],0.0,1.0);
	  }
  fclose(fin);

  if (colormap->GetNumberOfColors()!=256)
      colormap->SetNumberOfTableValues(256);

  for (int ia=0;ia<256;ia++)
    {
      double val[4];
      if (volume_mode)
	val[3]=double(ia)/double(256);
      else
	val[3]=1.0;
      for (int j=0;j<=2;j++)
	val[j]=colors[ia][j];
      colormap->SetTableValue(ia,val);
    }
  gzclose(fin);
  return 1;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxColorMapUtil::SaveAnalyzeColormap(vtkLookupTable* colormap,const char* name)
{
  if (colormap==NULL)
      return 0;

  if (colormap->GetNumberOfColors()!=256)
    {
      fprintf(stderr,"Cannot export colormap as it does not have 256 colors\n");
      return 0;
    }

  FILE* fout=fopen(name,"w");

  if (!fout)
      return 0;

  for (int i=0;i<colormap->GetNumberOfColors();i++)
      {
	double val[4];
	colormap->GetTableValue(i,val);
	fprintf(fout,"%3d %3d %3d \n",int(val[0]*255.0),int(val[1]*255.0),int(val[2]*255.0));
      }
  fprintf(fout,"\n");
  fclose(fout);
  return 1;
}
// -----------------------------------------------------------------------------------------------------------------------
void vtkpxColorMapUtil::SetAutoStepColorMap(vtkImageData* img,vtkLookupTable* cmap,int robust,int num_colors,int volume,int opaque)
{
  if (img==NULL || cmap==NULL)
    return;

  double range[2]; img->GetPointData()->GetScalars()->GetRange(range);

  float           auto_min,auto_max;
  if (robust)
    {
      vtkpxUtil::ImageRobustRange(img,0.01,0.99,auto_min,auto_max);
    }
  else
    {
      auto_min=range[0];
      auto_max=range[1];
    }

  if (auto_min==auto_max)
    auto_max+=1.0;

  vtkpxColorMapUtil::SetStepColorMap(cmap,auto_min,auto_max,range[0],range[1],num_colors,volume,opaque);
}
// -----------------------------------------------------------------------------------------------------------------------



