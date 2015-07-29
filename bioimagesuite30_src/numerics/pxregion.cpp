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

//
//
//
//
//  This file derives from Amit Chakraborty's original work and is almost completely obsolete!
//
//---------------------------------------------------------------------------------------------------------


#include "pxregion.h"
#include "nrutil.h"
#include "pxfloatmatrix.h"
#include "pxfloatimage.h"
#include "nr.h"
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris February 1996   papad@noodle.med.yale.edu

   see pxregion.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
// Defining namespace for old code



PX2DRegion::PX2DRegion()
{
  hasderivatives=0;
  hasoutput=0;
}
// ***********************************************************************

PX2DRegion::~PX2DRegion()
{
  if (hasoutput)
      delete outputimage;
  if (hasderivatives==1)
      {
	delete gradx;
	delete grady;
      }
}
// ***********************************************************************
void PX2DRegion::CreateOutputStructures(PXImage* pximage)
{
  if (hasoutput)
      delete outputimage;
  outputimage=new PXImage(pximage->getwidth(),pximage->getheight());
  outputimage->Zero();
}
// ***********************************************************************
void PX2DRegion::GenerateDerivatives(float sigma)
{
  if (hasderivatives)
      {
	delete gradx;
	delete grady;
      }
  
  PXFloatImage* PotentialMapX=new PXFloatImage(outputimage->getsizex(),outputimage->getsizey());
  PXFloatImage* PotentialMapY=new PXFloatImage(outputimage->getsizex(),outputimage->getsizey());
  integrate(outputimage,PotentialMapX,PotentialMapY);

  int size=7;
  if (sigma>1.0) size=6*int(sigma+0.5)+1;
  PXFloatMatrix* kernelx=new PXFloatMatrix(size,3);
  PXFloatMatrix* kernely=new PXFloatMatrix(3,size);
  kernelx->GenGradGaussian1Dx(sigma);
  kernely->GenGradGaussian1Dy(sigma);
		
  gradx = new PXFloatImage(PotentialMapX,kernelx);
  grady = new PXFloatImage(PotentialMapY,kernely);
  delete kernelx;
  delete kernely;
  hasderivatives=1;
  
  delete PotentialMapX;
  delete PotentialMapY;
}
// ***********************************************************************
float PX2DRegion::getgradx(float x,float y)
{
  if (hasderivatives==1)
      return gradx->getinterp(x,y);
  else
      return 0.0;
}
// ***********************************************************************
float PX2DRegion::getgrady(float x,float y)
{
  if (hasderivatives==1)
      return grady->getinterp(x,y);
  else
      return 0.0;
}
// ***********************************************************************
//       ACTUAL BAYESIAN SEGMENTATION FOR 2 ZONES                               
// ***********************************************************************

void PX2DRegion::segment_two_regions(PXImage* in,int mi,
				     float mean1,float mean2,
				     float sig, float sm)
{
  CreateOutputStructures(in);
  PXImage* out=outputimage;
  PXFloatImage *arr2,*arrm, *arr;
  
  // Scale Parameters to be in Image range as opposed to 0-1
  
  /*mean1=mean1*float(in->getmaxgrayscale());
  mean2=mean2*float(in->getmaxgrayscale());
  sig=sig*float(in->getmaxgrayscale());
  sm=sm*float(in->getmaxgrayscale());*/

  int i,j;
  int ii;

  float mid;

  float te,te1, te2, te3, te4, te5, te6, te7, test1, test2;
  float t4,t5,t6,t7;
  
  int width=in->getsizex();
  int height=in->getsizey();
  
  mid=(mean1+mean2)/2.0;

  arr2 = new PXFloatImage(width,height);
  arrm = new PXFloatImage(width,height);
  arr  = new PXFloatImage(width,height);

  for (i=0; i <width; i++)
      for (j=0; j <height; j++)
	  {
	    arr->setpixel(i,j,float(in->getpixel(i,j)));
	    arr2->setpixel(i,j,arr->getpixel(i,j));
	  }
  //  fprintf(stderr,"Done Initialization\n");
  for(ii=1;ii<=(abs(mi));ii++)
      {
	//fprintf(stderr,"Iteration %d\n",ii);
	
	if(ii==1) 
	    mid=(mean1+mean2)/2.0;
	else 
	    {
	      mid=(100.0+200.0)/2.0;
	      for (i=0; i < width; i++)
		  for (j=0; j < height ; j++)
		      arr->setpixel(i,j,arrm->getpixel(i,j));
	    }
	
	for (i=1; i < width-1; i++)
	    for (j=1; j <height-1; j++)
		{
		  arrm->setpixel(i,j,0.0);
		  /*		  if(arr2->getpixel(i,j) != 0.0)
		      {*/
			te1=arr2->getpixel(i,j) - mean1;
			te2=arr2->getpixel(i-1,j)+arr2->getpixel(i+1,j) - 2*mean1;
			te3=arr2->getpixel(i,j-1)+arr2->getpixel(i,j+1) - 2*mean1;
			te=te1-0.2*(te2+te3);
			te=te*te/(2.0*sig*sig);
			if( arr->getpixel(i-1,j) < mid) te4= -sm;
			else te4=sm;
			if( arr->getpixel(i+1,j) < mid) te5= -sm;
			else te5=sm;
			if( arr->getpixel(i,j-1) < mid) te6= -sm;
			else te6=sm;
			if( arr->getpixel(i,j+1) < mid) te7= -sm;
			else te7=sm;
			
			if( arr->getpixel(i-1,j+1) < mid) t4= -sm;
			else t4=sm;
			if( arr->getpixel(i+1,j+1) < mid) t5= -sm;
			else t5=sm;
			if( arr->getpixel(i+1,j-1) < mid) t6= -sm;
			else t6=sm;
			if( arr->getpixel(i-1,j-1) < mid) t7= -sm;
			else t7=sm;
			if(ii==0) te4=te5=te6=te7=0.0; 
			if(ii==0) t4=t5=t6=t7=0.0; 
			test1=te+te4+te5+te6+te7+2.0*log(sig*sig);
			test1=test1+t4+t5+t6+t7;
			
			
			te1=arr2->getpixel(i,j) - mean2;
			te2=arr2->getpixel(i-1,j)+arr2->getpixel(i+1,j) - 2*mean2;
			te3=arr2->getpixel(i,j-1)+arr2->getpixel(i,j+1) - 2*mean2;
			te=te1-0.2*(te2+te3);
			te=te*te/(2.0*sig*sig);
			if( arr->getpixel(i-1,j) > mid) te4= -sm;
			else te4=sm;
			if( arr->getpixel(i+1,j) > mid) te5= -sm;
			else te5=sm;
			if( arr->getpixel(i,j-1) > mid) te6= -sm;
			else te6=sm;
			if( arr->getpixel(i,j+1) > mid) te7= -sm;
			else te7=sm;
			
			if( arr->getpixel(i-1,j+1) < mid) t4= -sm;
			else t4=sm;
			if( arr->getpixel(i+1,j+1) < mid) t5= -sm;
			else t5=sm;
			if( arr->getpixel(i+1,j-1) < mid) t6= -sm;
			else t6=sm;
			if( arr->getpixel(i-1,j-1) < mid) t7= -sm;
			else t7=sm;
			
			if(ii==0) te4=te5=te6=te7=0.0; 
			if(ii==0) t4=t5=t6=t7=0.0; 
			test2=te+te4+te5+te6+te7+2.0*log(sig*sig);
			test2=test2+t4+t5+t6+t7;
			
			if(test1 < test2) 
			    arrm->setpixel(i,j,100.0);
			else 
			    arrm->setpixel(i,j,200.0);
		      //}
		}
      }
  
  for (i=0;i<width;i++)
      for (j=0;j<height;j++)
	  out->setpixel(i,j,int(arrm->getpixel(i,j)));
  
  delete arrm;
  delete arr2;
  delete arr;
}
/* ----------------------------------------------------------------------- */
void PX2DRegion::segment_three_regions(PXImage* image1,
				       int mi,float mean1,float mean2,float mean3,float sig )

/*PIXEL *in, *out;
float mean1,mean2,mean3,sig;
int width, height,mi;*/
{
  CreateOutputStructures(image1);
  PXImage* image2=outputimage;

  int width=image1->getwidth();
  int height=image2->getheight();

  PXImage* tempimage=new PXImage(image1);

  unsigned short* in=tempimage->getimagepointer();
  unsigned short* out=image2->getimagepointer();

  float *arr2,*arrm, *arr;
  int npl,nph,nrl,nrh;//ncl,nch;
  int i,j;
  int ii;

  //int nn,xsize,ysize,zsize;

  //float xsiz,ysiz,zsiz;
  float te,te1, te2, te3, te4, te5, te6, te7, test1, test2,test3;
  //float asp;
  unsigned short *ptr1;
  float *ptr2;
  float mid1,mid2;

  arr2 = new float[width*height];// (float *) calloc(width*height, sizeof(float));
  arrm = new float[width*height];//(float *) calloc(width*height, sizeof(float));
  arr =  new float[width*height];//(float *) calloc(width*height, sizeof(float));


  npl = 2;
  nph = height-2;
  nrl = 2;
  nrh = width-2;
  //ncl = 0;
  //nch = 123;
  //asp = 1.0;
  /*zsize = nch-ncl+1;
  ysize = nrh-nrl+1;
  xsize = nph-npl+1;
  xsiz = width;
  ysiz = height;
  zsiz = 124.0;*/


  // nn = 0;
  //  int temp =0.0;

  for (i=npl; i <= nph; i++)
      for (j=nrl; j <= nrh; j++)
	  {
	    if (*(in+width*i+j) < 30 ) 
		*(in+width*i+j)=0;
	    *(arr2+width*i+j) = *(arr+width*i+j)=(float) *(in+width*i+j); 
	  }
  
  for(ii=0;ii<=mi;ii++)
      {
	if(ii==0)
	    {
	      mid1=(mean1+mean2)/2.0;
	      mid2=(mean2+mean3)/2.0;
	    }
	else
	    {
	      mid1=100.0; 
	      mid2=150.0;
	    }
	
    for (i=npl; i <= nph; i++)
	for (j=nrl; j <= nrh; j++)
	    *(arr+i*width+j)=*(arrm+i*width+j);
	
    for (i=npl; i <= nph; i++)
	for (j=nrl; j <= nrh; j++)
	    {
	      *(arrm+i*width+j)=0.0;
	      if(*(arr2+i*width+j) != 0.0)
		  {
		    te1=*(arr2+i*width+j) - mean1;
		    te2=*(arr2+(i-1)*width+j)+*(arr2+(i+1)*width+j) - 2*mean1;
		    te3=*(arr2+i*width+j-1)+*(arr2+i*width+j+1) - 2*mean1;
		    te=te1-0.02*(te2+te3);
		    te=te*te/(2.0*sig*sig);
		    if(( *(arr+(i-1)*width+j) < mid1) && ( *(arr+(i-1)*width+j) > 61)) 
			te4= -0.9;
		    else 
			te4=0.9;
		    if((*(arr+(i+1)*width+j) < mid1) && ( *(arr+(i+1)*width+j) > 61)) 
			te5= -0.9;
		    else 
			te5=0.9;
		    if((*(arr+i*width+j-1) < mid1) && ( *(arr+i*width+j-1) >61)) 
			te6= -0.9;
		    else 
			te6=0.9;
		    if((*(arr+i*width+j+1) < mid1)  && ( *(arr+i*width+j-1) >61))
			te7= -0.9;
		    else 
			te7=0.9;
		    test1=te+te4+te5+te6+te7+2.0*log((double)(sig*sig));
	
		    te1=*(arr2+i*width+j) - mean2;
		    te2=*(arr2+(i-1)*width+j)+*(arr2+(i+1)*width+j) - 2*mean2;
		    te3=*(arr2+i*width+j-1)+*(arr2+i*width+j+1) - 2*mean2;
		    te=te1-0.02*(te2+te3);
		    te=te*te/(2.0*sig*sig);
		    if(( *(arr+(i-1)*width+j) < mid2) && ( *(arr+(i-1)*width+j) > mid1)) 
			te4= -0.9;
		    else 
			te4=0.9;
		    if((*(arr+(i+1)*width+j) < mid2) && ( *(arr+(i+1)*width+j) > mid1)) 
			te5= -0.9;
		    else 
			te5=0.9;
		    if((*(arr+i*width+j-1) < mid2) && ( *(arr+i*width+j-1) >mid1)) 
			te6= -0.9;
		    else 
			te6=0.9;
		    if((*(arr+i*width+j+1) < mid2)  && ( *(arr+i*width+j-1) >mid1))
			te7= -0.9;
		    else 
			te7=0.9;
		    test2=te+te4+te5+te6+te7+2.0*log((double)(sig*sig));

		    te1=*(arr2+i*width+j) - mean3;
		    te2=*(arr2+(i-1)*width+j)+*(arr2+(i+1)*width+j) - 2*mean3;
		    te3=*(arr2+i*width+j-1)+*(arr2+i*width+j+1) - 2*mean3;
		    te=te1-0.02*(te2+te3);
		    te=te*te/(2.0*sig*sig);
		    if( *(arr+(i-1)*width+j) > mid2) 
			te4= -0.9;
		    else 
			te4=0.9;
		    if( *(arr+(i+1)*width+j) > mid2) 
			te5= -0.9;
		    else 
			te5=0.9;
		    if( *(arr+i*width+j-1) > mid2) 
			te6= -0.9;
		    else 
			te6=0.9;
		    if( *(arr+i*width+j+1) > mid2) 
			te7= -0.9;
		    else 
			te7=0.9;

		    test3=te+te4+te5+te6+te7+2.0*log((double)(sig*sig));
		    
		    if((test1 < test2) && (test1<test3)) 
			*(arrm+i*width+j)=50.0;
		    else
			{
			  if((test2 < test1) && (test2<test3)) 
			      *(arrm+i*width+j)=130.0;
			  else 
			      *(arrm+i*width+j) = 200.0; }
		  }
	    }
	ptr1 = out;
	ptr2 = arrm;
	for( i = 0; i < width * height; i++ )
	    *ptr1++ = (unsigned short) *ptr2++;
      }
  delete [] arr2;
  delete [] arrm;
  delete [] arr;
  delete tempimage;
  
}
/* ----------------------------------------------------------------------- */
void PX2DRegion::integrate(PXImage *im1,PXFloatImage* imx,PXFloatImage* imy)
{
  int i,j;
  for(j=0;j<im1->getsizey();j++)
      {
      float sumx=32768.0;
      for(i=0;i<im1->getsizex();i++)
	{
	  float temp=float(im1->getpixel(i,j));
	  if (temp==200.0) 
	    temp=1.0;
	  else
	    temp=-1.0;
	  sumx=sumx+temp;
	  imx->setpixel(i,j,sumx);
	}	
    }
  

  for (i=0;i<im1->getsizex();i++)
    {
      float sumy=32768.0;
      for(j=0;j<im1->getsizey();j++)
	  {
	    float temp=float(im1->getpixel(i,j));
	    if (temp==200.0) 
		temp=1.0;
	    else
		temp=-1.0;
	    sumy=sumy+temp;
	    imy->setpixel(i,j,sumy);
	  }	
    }
}
/* ----------------------------------------------------------------------- */
void PX2DRegion::texture_segment(PXImage* image1,float beta,int iter,int window,
				 int numtext,int* xi ,int* yi)
{
  int width=image1->getwidth();
  int height=image1->getheight();
  CreateOutputStructures(image1);
  PXImage* image2=outputimage;

  unsigned short* in=image1->getimagepointer();
  unsigned short* out=image2->getimagepointer();

  float *x, *arr,*mx;
  int i,j,k,l,i1,j1;
  int N,Nm;
  unsigned short  *temp, *arrm;
  
  unsigned short val[20];
  float test[20];
  float meanf, a1f, a2f, a3f, a4f, sigmaf;
  float y1,y2,y3,y4,y0,si,sumn;

  x = new float[width*height];
  mx = new float[width*height];
  arrm = new unsigned short[width*height];
  arr = new float[width*height];
  temp = new unsigned short[width*height];
  
  for(i=0;i<height;i++)
      for(j=0;j<width;j++)
	  *(arr+i*width+j)=(float)( *(in+i*width+j));
  
  for(i=0;i<height;i++)
      for(j=0;j<width;j++)
	  {
	    sumn=0.0; Nm=0;
	    for(i1=i-5;i1<=i+5;i1++)
		for(j1=j-5;j1<=j+5;j1++){
		  if((i1>0)&&(i1<height-1)&&(j1>0)&&(j1<width-1)){
		    sumn=sumn+*(arr+i1*width+j1); Nm=Nm+1;}
		}
	    *(x+i*width+j)=*(arr+i*width+j)-sumn/((float)(Nm));
	    *(mx+i*width+j)=sumn/((float)(Nm));
	  }
  
  if (window>0)
      {
	N=numtext;
	textureparams.numzones=N;
	for(k=1;k<=N;k++)
	    {
	      fprintf(stderr,"----------------- Zone %d -(%d,%d)------------------\n",k,xi[k-1],yi[k-1]);
	      for(i=0;i<height;i++)
		  for(j=0;j<width;j++)
		      {
			temp[i*width+j]=0;
			if(j>=xi[k-1]-window &&
			   j<=xi[k-1]+window && 
			   i>=yi[k-1]-window  && 
			   i<=yi[k-1]+window )
			    temp[i*width+j]=200;
		      }
	      l_Coeff(mx,temp,width,height,&meanf,&a1f,&a2f,&a3f,&a4f,&sigmaf);
	      textureparams.mean[k]=meanf;
	      l_Coeff(x, temp,width,height,&meanf,&a1f,&a2f,&a3f,&a4f,&sigmaf);
	      textureparams.sigma[k]=sigmaf;
	      textureparams.a1[k]=a1f; 
	      textureparams.a2[k]=a2f; 
	      textureparams.a3[k]=a3f; 
	      textureparams.a4[k]=a4f;
	    }
      }
  else
      {
	fprintf(stderr,"Using Previous Parameters\n");
	N=textureparams.numzones;
      } 

  for(k=1;k<=N;k++)
      val[k]=(unsigned short)(k*255/N);
  
  
  for(l=1;l<=iter;l++)
      {
	fprintf(stderr,"----------------- Iteration %d -------------------\n",l);
  	for(i=0;i<height;i++)
	    for(j=0;j<width;j++)
		{
		  for(k=1;k<=N;k++)
		      {
			si=0.0;
			for(i1=i-1;i1<=i+1;i1++)
			    for(j1=j-1;j1<=j+1;j1++){
			      if((i1>0)&&(i1<height-1)&&(j1>0)&&(j1<width-1))
				  {
				    y0=*(x+i1*width+j1);
				    y1=*(x+i1*width+j1-1) + *(x+i1*width+j1+1) ;
				    y2=*(x+(i1-1)*width+j1) + *(x+(i1+1)*width+j1) ;
				    y3=*(x+(i1-1)*width+j1-1) + *(x+(i1+1)*width+j1+1) ;
				    y4=*(x+(i1-1)*width+j1+1) + *(x+(i1+1)*width+j1-1) ;
				    
				    si= si + (y0 - 
					      textureparams.a1[k] *y1 - textureparams.a2[k]*y2 - 
					      textureparams.a3[k]*y3 -  textureparams.a4[k]*y4 )* y0 ;
				    si= si + (*(arr+i1*width+j1)-
					      textureparams.mean[k])*(*(arr+i1*width+j1)-
								      textureparams.mean[k]);
				  }
			    }
			
			if((l==1)||(i==0)||(i==height-1)||(j==0)||(j==width-1))
			    test[k]=si;
			else
			    {
			      test[k]=si;
			      if( *(arrm+i*width+j+1)==val[k] ) 
				  test[k]=test[k] - beta;
			      else 
				  test[k]=test[k] + beta;
			      if( *(arrm+i*width+j-1)==val[k] ) 
				  test[k]=test[k] - beta;
			      else 
				  test[k]=test[k] + beta;
			      if( *(arrm+(i+1)*width+j)==val[k] ) 
				  test[k]=test[k] - beta;
			      else 
				  test[k]=test[k] + beta;
			      if( *(arrm+(i-1)*width+j)==val[k] ) 
				  test[k]=test[k] - beta;
			      else 
				  test[k]=test[k] + beta;
			    }
			
		      }
		  sort_least(test,N,&k);
		  *(arrm+i*width+j)=val[k];
		}
	
	for(i=0;i<height;i++)
	    for(j=0;j<width;j++)
		*(out+i*width+j)=*(arrm+i*width+j);
      }
  
  delete [] x; 
  delete [] mx;
  delete [] arrm; 
  delete [] arr;
  delete [] temp;
}
/* ----------------------------------------------------------------------- */
void PX2DRegion::sort_least(float* a,int n,int* k)
{
  int i;
  float min;
  *k=1; min=a[1];
  for(i=2;i<=n;i++)
      {
	if(a[i]<=min)
	    {
	      min=a[i]; *k=i;
	    }
      }
}
// ----------------------------------------------------------------------------------
void PX2DRegion::l_Coeff(float* in,unsigned short* in1,
			 int width,int height,
			 float* meanf,float* a1f,float* a2f,float* a3f,float* a4f,float *sigmaf)
{
  int i,j,k,l;
  double *x,mean;
  double sig,si,sigsq;
  double sig1,si1,sigsq1;
  double *y;
  double sum;
  double weight;
  int N;
  //double w;
  double **A, *B,d;
  int *indx;
  int n=4;
  
  x=new double[height*width];
  A=dmatrix(1,n,1,n);
  B=dvector(1,n);
  y=dvector(1,n);
  indx=ivector(1,n);
  
  for(k=1;k<=n;k++)
      for(l=1;l<=n;l++)
	  A[k][l]=B[k]=0.0;
  
  for(i=0;i<height;i++)
      for(j=0;j<width;j++)
	  *(x+i*width+j)=(double)( *(in+i*width+j));

  sum=0.0;
  weight=0.0;
  for(i=0;i<height;i++)
      for(j=0;j<width;j++)
	  {
	    if( *(in1+i*width+j)!=0 )
		{
		  sum=sum+ *(x+i*width+j);
		  weight=weight+1.0;
		}
	  }
  mean=sum/weight;
  printf("mean, num %f %f \n", mean,weight);
  
 
  for(i=0;i<height;i++)
    for(j=0;j<width;j++)
      {
	if( *(in1+i*width+j)!=0 )
	  {
	    //w=1.0;
	    
	    y[1]=*(x+i*width+j-1) + *(x+i*width+j+1) ;
	    y[2]=*(x+(i-1)*width+j) + *(x+(i+1)*width+j) ; 
	    y[3]=*(x+(i-1)*width+j-1) + *(x+(i+1)*width+j+1) ; 
	    y[4]=*(x+(i-1)*width+j+1) + *(x+(i+1)*width+j-1) ; 
	    
	    for(k=1;k<=n;k++)
	      for(l=1;l<=n;l++)
		A[k][l]=A[k][l] + y[k] * y[l];
	    
	    
	    for(k=1;k<=n;k++)
	      /*
		B[k]=B[k] + y[k] * ( *(x+i*width+j) -mean);
	      */
	      B[k]=B[k] + y[k] * ( *(x+i*width+j) );
	    
	    
	  }
      }
  
  int singular;
  ludcmp(A,n,indx,&d,singular);
  lubksb(A,n,indx,B);
  
  if( ( fabs(B[1]) > 2.0)||( fabs(B[2]) > 2.0)||( fabs(B[3]) > 2.0)||( fabs(B[4]) > 2.0) )
    B[1]=B[2]=B[3]=B[4]=0.0;
  
  printf("results %f   %f  %f   %f\n", B[1],B[2],B[3],B[4]);
  
  sigsq=0.0;
  sigsq1=0.0;
  N=0;
  for(i=0;i<height;i++)
    for(j=0;j<width;j++){
      if( *(in1+i*width+j)!=0)
	{
	  si1=*(x+i*width+j)-mean;
	  
	  y[1]=*(x+i*width+j-1) + *(x+i*width+j+1) ;
	  y[2]=*(x+(i-1)*width+j) + *(x+(i+1)*width+j) ; 
	  y[3]=*(x+(i-1)*width+j-1) + *(x+(i+1)*width+j+1) ; 
	  y[4]=*(x+(i-1)*width+j+1) + *(x+(i+1)*width+j-1) ; 
	  si=*(x+i*width+j)  - B[1]*y[1] - B[2]*y[2] - B[3]*y[3] - B[4]*y[4];
	  sigsq=sigsq+si*si; 
	  sigsq1=sigsq1+si1*si1;
	  N=N+1;
	}
    }
  sig=sqrt(sigsq/((double)N)); 
  sig1=sqrt(sigsq1/((double)N));
  printf("sig,sig1 %f %f \n", sig,sig1);
  
  printf("st dev %f \n", sig); 
  printf("st dev1 %f \n", sig1);
  printf("mean %f \n", mean);
  printf("%d \n", N); 
  *meanf=mean; *sigmaf=sig;
  *a1f=B[1]; *a2f=B[2]; *a3f=B[3]; *a4f=B[4];

  free_dmatrix(A,1,n,1,n);
  free_dvector(B,1,n);
  free_dvector(y,1,n);
  free_ivector(indx,1,n);
  delete [] x;
}



