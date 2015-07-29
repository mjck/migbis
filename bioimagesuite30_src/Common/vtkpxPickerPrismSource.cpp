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





#include "vtkpxPickerPrismSource.h"
#include "vtkObjectFactory.h"
#include "pxutil.h"

//-------------------------------------------------------------------------------------
// Prism Data Structure from Larry Staib
//-------------------------------------------------------------------------------------

/*@(#)iibdef.txt	1.8 8/1/90*/
/* L. Staib */
class picker_iib
{	/* Beginning of PRISM IIB Structure */


public:
  /***************************/
  /**  Manufacturer's data  **/
  /***************************/
#define PRISM1  1           /* IIB Type Code for P1 Prism System */
unsigned char   itc;        /* IIB Type Code */

#define IIBVER  2           /* IIB Version Number */
unsigned char   ivc;        /* IIB Version Code */

char            mmd[10];    /* Machine Model; 8. char */

char            msn[6];     /* Machine (Gantry) Serial number; 4. char */

unsigned short  rbl;        /* Released Software Baseline Level */
unsigned short  rtl;        /* Released Software TSKGEN Level */
unsigned short  rpl;        /* Released Software Partial Level */

	/*File Data Types*/
#define IIBPRJ  01          /* Projection (raw) data */
#define IIBCPR  02          /* Compressed data */
#define IIBIMG  04          /* Image data */
#define IIBFIL  010         /* File data */
#define IIBLBI  020         /* XImage bitmap data */
unsigned short  typ;        /* Type of data in this file */

	/********************/
	/**  Patient Data  **/
	/********************/
char            pn[22];     /* Patient Name; 20. char */

char            pi[22];     /* Patient Identification; 20. char */

unsigned char   pbm;        /* Patient birth month (1=Jan - 12=Dec) */
unsigned char   pbd;        /* Patient birth day of month (1 - 31) */
unsigned short  pby;        /* Patient birth year (e.g.: 1987) */

char            sex[2];     /* Patient Sex; 1 character (M, F, O) */

	/*********************/
	/**  Hospital Data  **/
	/*********************/

char            rdn[22];    /* Radiologist's Name; 20. char */

char            rpn[22];    /* Referring Physician's Name; 20. char */

char            oi[4];      /* Operator Identification; 3 char (initials) */

char            hn[22];     /* Hospital Name; 20. char */
char            hi[22];     /* Hospital Identification; 20. char */

	/***********************/
	/**  Acquisition Data **/
	/***********************/
char            obs0[10];   /* unused space due to obsoleted fields */
unsigned short  aqu;        /* Acquisition Number (for this patient) */
unsigned short  obs1;       /* unused space due to obsoleted fields */

unsigned char   atm;        /* Acquisition start month (1=Jan - 12=Dec) */
unsigned char   atd;        /* Acquisition start day of month (1 - 31) */
unsigned short  aty;        /* Acquisition start year (e.g.: 1987) */
char            ati[10];    /* Acquisition start time (HH:MM:SS) */

unsigned char   ctm;        /* Acquisition completion month (1=Jan - 12=Dec) */
unsigned char   ctd;        /* Acquisition completion day of month (1 - 31) */
unsigned short  cty;        /* Acquisition completion year (e.g.: 1987) */
char            cti[10];    /* Acquisition completion time (HH:MM:SS) */

	/*Acquisition types*/
#define IIBINJ  04          /* injector synchronized scan */
#define IIBEKG  010         /* ekg-gated scan */
#define IIBRES  020         /* respiratory-gated scan */
#define IIBDUA  0400        /* dual isotope scan */
#define IIBSTP  01000       /* step-and-shoot */
#define IIBCON  02000       /* continuous */
#define IIBFLD  04000       /* Flood */
#define IIBPLN  010000      /* Planar */
#define IIBDYN  020000      /* Dynamic */
#define IIBWBA  040000      /* Whole Body */
unsigned short  at;         /* Acquisition type */

short           ttp;        /* Absolute table traverse position in 0.1 mm */
short           tth;        /* Absolute table height in 0.1 mm */

	/*Data Modes*/
#define IIBBYT  1           /* Byte-mode data */
#define IIBWOR  2           /* Word-mode data */
short           dmd;        /* Data mode flag */
	/*Gantry Rotation Direction*/
#define IIBCW   01          /* Clockwise Rotation */
#define IIBCCW  02          /* Counter Clockwise Rotation */
unsigned short  rod;        /* Rotation direction */

unsigned short  ns;         /* Number of scans */

short           sa;         /* Acquisition start angle in 0.1 degrees */
short           ar;         /* Acquisition angle range in 0.1 degrees */
short           npi;        /* Number of projections in this acquisition */

unsigned short  tv;         /* Maximum time per step in 0.01 seconds */

unsigned short  cps[4];     /* Kcounts per step per window (all heads) */

	/*Rotation Orbit Types*/
#define IIBCIR  01          /* circular */
#define IIBELL  02          /* non-circular */
#define IIBOOT  04          /* other */
unsigned short  orb;        /* Rotation orbit type */

unsigned short  orm[4];     /* Orbital radial map in 0.1 mm units */

#define IIBPC1  1           /* Version 1 Prism camera 9.4x15.75 */
#define IIBCAMTECHRECT 2    /* 14.5x20 camera with cropped corners */
#define IIBCAMTECHHEX 3     /* Hex head */
#define IIBCAMGENESYS 4     /* 15x20 camera with cropped corners */
#define IIBCAMDYNAMO 5      /* 10 inch circular */
unsigned short  cam;        /* Camera type */

	/*Active Head and Window Bit Masks*/
	/*Bit Number:    |15|14|13|12|11|10|9|8|7|6|5|4|3|2|1|0|*/
	/*Head Number:   |  | 3| 2| 1|  | 3|2|1| |3|2|1| |3|2|1|*/
	/*Window Number: | 4| 4| 4| 4| 3| 3|3|3|2|2|2|2|1|1|1|1|*/
#define IIBHE1  010421      /* Head 1 active (0x1111) */
#define IIBHE2  021042      /* Head 2 active (0x2222) */
#define IIBHE3  042104      /* Head 3 active (0x4444) */
unsigned short  hac;        /* Heads active */

	/*Window Type Flags*/
#define IIBWTK  01          /* keV window */
#define IIBWCH  02          /* channel window */
unsigned short  wtf;        /* Window type flag */

	/*window center accessed from C as wce[3][4] or wce[head][window]*/
unsigned short  wce[12];    /* window center */

	/*window width in % accessed from C as wwi[3][4] or wwi[head][window]*/
unsigned char   wwi[12];    /* window width in % */
short           hvs[3];     /* High Voltage Settings */

char            apx[12];    /* Acquisition pixel size x axis in mm (floating pt.) */
char            apy[12];    /* Acquisition pixel size y axis in mm (floating pt.) */

int             amx;        /* Acquisition x axis pixels */
int             amy;        /* Acquisition y axis pixels */

char            rnu[12];    /* radiopharmaceutical; 10 char */
unsigned short  rnv;        /* radiopharmaceutical volume in 0.1 cc */
unsigned short  rna;        /* radiopharmaceutical activity in 0.1 mCi units */
unsigned char   rnm;        /* radiopharmaceutical injection month (1=Jan - 12=Dec) */
unsigned char   rnd;        /* radiopharmaceutical injection day of month (1 - 31) */
unsigned short  rny;        /* radiopharmaceutical injection year (e.g.: 1987) */
char            rni[6];     /* radiopharmaceutical injection time (HH:MM) */

	/*Isotope Half-Life*/
#define IIBTMI  00          /* units of 0.01 minutes */
#define IIBTHR  01          /* units of 0.01 hours */
#define IIBTDA  02          /* units of 0.01 days */
#define IIBTYR  03          /* units of 0.01 years */
#define IIBTUK  04          /* units unknown */
unsigned short  rhu;        /* isotope half-life time units */
unsigned short  rhl;        /* isotope half-life in units shown above */

	/*Isotope*/
#define IIBITC  01          /* Tc-99m */
#define IIBICO  02          /* Co-57 */
#define IIBITL  04          /* Tl-201 */
#define IIBIGA  010         /* Ga-97 */
#define IIBIXE  020         /* Xe-133 */
#define IIBI23  040         /* I-123 */
#define IIBI31  0100        /* I-131 */
#define IIBIN1  0200        /* In-111 */
#define IIBUKI  0100000     /* unknown isotope or combination */
unsigned short  iso;        /* isotope */

	/*Collimator Descriptions*/
#define IIBHF   1           /* 140keV HR fan */
#define IIBHP   2           /* 140keV HR parallel */
#define IIBPG   3           /* 140keV parallel gap */
#define IIBFG   4           /* 140keV fan gap */
#define IIBUF   5           /* 140keV UHR fan */
#define IIBUP   6           /* 140keV UHR parallel */
#define IIBMEF  7           /* 300keV fan */
#define IIBMEP  8           /* 300keV parallel */
#define IIBHEP  9           /* 400keV parallel */
unsigned short  col;        /* collimator description */
short           fbf[3];     /* fan beam focal distance in 0.1 mm units (per head) */
short           fbx[3];     /* fan beam x-axis focus in 0.1 pixel units (256 mtx) */

	/*patient direction in gantry values: H=headfirst, F=feetfirst*/
char            pdg[2];     /* patient direction in gantry; 1 char */

	/*patient orientation in gantry values:*/
	/*S=supine, P=prone, L=leftdown, R=rightdown*/
char            pog[2];     /* patient orientation in gantry; 1 char (S,P,L,R) */

	/*Interval Information*/
short           inn;        /* Interval Number */
short           noi;        /* Number of Intervals */

	/*View Termination Information:	Type (parameter)*/
#define VTTIME  01          /* Time (0.01 seconds) */
#define VTTRIG  02          /* Trigger (number of triggers) */
#define VTTCOU  04          /* Counts (Kcounts per projection) */
#define VTTDEN  010         /* Density (counts per pixel) */
#define VTTCLO  020         /* Close on byte overflow */
#define VTTWRA  040         /* Wraparound on byte overflow */
unsigned short  vtt;        /* View Termination Type */
int             vtp;        /* View Termination Parameter */

	/*Beat Rejection Information*/
#define NOBRT   00          /* No Beat Rejection (no parameters) */
	/*Beat Rejection Types:	Type (parameters)*/
#define BRT3P   01          /* 3 Bin Percentage (% and 0) */
#define BRT3I   02          /* 3 Bin Interval (begin and end in msec) */
#define BRT1P   03          /* 1 Bin Percentage (% and 0) */
	/*Beat Rejection Masks*/
#define BRTTYP  0377        /* Type Mask */
#define BRTNOR  0400        /* Normal Bin */
#define BRTSHO  01000       /* Short Bin */
#define BRTLON  02000       /* Long Bin */
unsigned short  brt;        /* Beat Rejection Type */
short           brp[2];     /* Beat Rejection Parameters */
	/*Acquisition Orientation*/
unsigned short  ori;        /* Acquisition Orientation */

	/*Gantry Type*/
#define IIBGANPRISM 0       /* Prism gantry */
#define IIBGANUNK 1         /* unknown gantry */
#define IIBGANGEMINI 2      /* Gemini gantry */
#define IIBGANOMEGA 3       /* Omega gantry */
#define IIBGANDYNAMO 4      /* DynaMo gantry */
#define IIBGENESYS 5        /* Genesys gantry */
unsigned short  gan;        /* Gantry Type */

	/*Corrections*/
char            fn1[8];     /* 6 character flood name */
char            fn2[8];     /* 6 character flood name, head 2 */
char            fn3[8];     /* 6 character flood name, head 3 */

#define IIBCORENERGY 01     /* Energy correction enabled */
#define IIBCORFLOOD 02      /* Flood correction enabled */
#define IIBCORLIN 04        /* Linearity correction enabled */
#define IIBCORIRC 010       /* Image registration correction enabled */
unsigned char   cor[4];     /* Acquisition corrections, 1 per head */

	/*Second Isotope/Radiopharmaceutical*/
char            rnu2[12];   /* radiopharmaceutical; 10 char */
unsigned short  rnv2;       /* radiopharmaceutical volume in 0.1 cc */
unsigned short  rna2;       /* radiopharmaceutical activity in 0.1 mCi units */
unsigned char   rnm2;       /* radiopharmaceutical injection month (1=Jan - 12=Dec) */
unsigned char   rnd2;       /* radiopharmaceutical injection day of month (1 - 31) */
unsigned short  rny2;       /* radiopharmaceutical injection year (e.g.: 1987) */
char            rni2[6];    /* radiopharmaceutical injection time (HH:MM) */
unsigned short  rhu2;       /* isotope half-life time units */
unsigned short  rhl2;       /* isotope half-life in units specified above */
unsigned short  iso2;       /* isotope */

	/*180 Recon Frame Range*/
unsigned short  lvf;        /* Lowest valid frame */
unsigned short  hvf;        /* Highest valid frame */

char            zap[12];    /* z-axis pixel size in mm (floating pt.) */

char			fill0[38];	/* UNUSED */

	/*****************************************/
	/**  Display/Reconstruction Information **/
	/*****************************************/
char            dxt[6];     /* display x-axis tag (bottom edge); 5 char */
char            dyt[6];     /* display y-axis tag (left edge); 5 char */

short           otw;        /* oblique twist (y/y) in degrees */
short           oti;        /* oblique tilt (x/z) in degrees */

	/*Processing Flags*/
#define IIBSAG  01          /* sagittal */
#define IIBCOR  02          /* coronal */
#define IIBOBL  04          /* oblique */
#define IIBOBS  010         /* oblique sagittal */
#define IIBOBC  020         /* oblique coronal */
#define IIBOBT  040         /* oblique transverse */
#define IIBSAX  0100        /* short axis cardiac */
#define IIBHLA  0200        /* horiz long axis cardiac */
#define IIBVLA  0400        /* vert long axis cardiac */
unsigned short  pf;         /* Processing Flag */

#define IIBSCA  01          /* Scatter corrected */
#define IIBADF  02          /* Adaptive filtered */
unsigned short  ppf;        /* Post Processing Flags */

char            rid[12];    /* Reconstruction ID; 10 char */

unsigned char   rcm;        /* reconstruction month (1=Jan - 12=Dec) */
unsigned char   rcd;        /* reconstruction day of month (1 - 31) */
unsigned short  rcy;        /* reconstruction year (e.g.: 1987) */
char            rci[10];    /* reconstruction time (HH:MM:SS) */

char            cfs[16];    /* Convolution filter file spec; 15. char */

	/*Pixel scaling, maximum, minimum pixel values*/
short           upv;        /* Upper pixel value */
short           lpv;        /* lower pixel value */

	/*Transverse Slice Positioning*/
unsigned short  slr;        /* Projection used as reference */
short           slo;        /* Slice offset in pixels from first projection pixel */
unsigned short  slt;        /* Reconstructed slice thickness in pixels */
unsigned short  sls;        /* Distance between slices in mm */
unsigned short  sln;        /* Number of slices */

	/*Zoom recon parameters*/
short           zx;         /* Zoom X ctr in mm; ctr of scan to ctr of zoom */
short           zy;         /* Zoom Y ctr in mm; ctr of scan to ctr of zoom */
unsigned short  zd;         /* Zoom recon diameter in mm */
char            rpx[12];    /* Reconstructed pixel size x axis in mm (floating pt.) */
char            rpy[12];    /* Reconstructed pixel size y axis in mm (floating pt.) */

unsigned short  vsp;        /* Reconstructed view spacing in 0.25 degree units */

int             rmx;        /* Reconstructed x axis pixels */
int             rmy;        /* Reconstructed y axis pixels */

#define ALCRC0  2           /* no-BP, ECT recon */
#define ALCRC1  3           /* 180, no-BP, ECT recon */
#define ALCRC2  4           /* standard BP, ECT recon */
#define ALCRC3  5           /* 180, BP, ECT recon */
#define ALCRC4  6           /* 360, BP, linear weighted, ECT recon */
unsigned short  alc;        /* Reconstruction algorithm code */
char            scl[12];    /* Reconstruction scale factor (floating pt.) */
unsigned short  rsa;        /* Reconstruction start angle for 180+ */

#define IIBAC0H 1           /* Automatic Head Chang AttCor Algorithm */
#define IIBAC0T 2           /* Automatic Torso Chang AttCor Algorithm */
#define IIBAC0M 3           /* Manual Chang AttCor Algorithm */
unsigned short  atc;        /* Attenuation algorithm code */
char            acv[12];    /* Attenuation coefficient (floating pt.) */
char            acs[12];    /* AttCor Scale Factor (floating pt.) */

	/*******************/
	/** MISCELLANEOUS **/
	/*******************/
char            cl1[32];    /* Comment Line; 30 char */
char            obs2[16];   /* unused space due to obsoleted field */
char            mom[16];    /* parent file; 15 char */
char            aid[12];    /* Acquisition ID; 10 char */
char            vie[12];    /* Anatomical view; 10 char */
char            org[12];    /* Organ description; 10 char */
unsigned short  cvt;        /* 1=file converted from foreign format */
unsigned int    ebo;        /* offset to first IIB extension block */
unsigned int    imo;        /* offset to first image data */

char			fill1[1268];	/* UNUSED */

	/**************************/
	/** END OF THE PRISM IIB **/
	/**************************/
};			/* End of PRISM IIB Structure */

//------------------------------------------------------------------------------
vtkpxPickerPrismSource* vtkpxPickerPrismSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxPickerPrismSource");
  if(ret)
    {
      return (vtkpxPickerPrismSource*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxPickerPrismSource;
}

//------------------------------------------------------------------------------

vtkpxPickerPrismSource::vtkpxPickerPrismSource()
{
  this->HeaderSize=2048;
  this->SetFilePattern ("%s");

}

int vtkpxPickerPrismSource::ReadHeaderFile()
{

  picker_iib* hdr=new picker_iib;

  gzFile fd=gzsuffixopen(this->FilePrefix,"rb");
  if ((long)fd==0)
    {
      return 0;
    }

  gzread(fd,hdr,2048);
  gzclose(fd);
  this->read_volume_ok=1;

  // Voxel Dimensions
  float voxel_width=atof(hdr->rpx);
  float voxel_height=atof(hdr->rpy);
  float voxel_depth=atof(hdr->zap);

  int dimx=hdr->rmx;
  int dimy=hdr->rmy;
  unsigned short dimz=hdr->sln;
  int orient=hdr->pf;


  unsigned short sls=hdr->sls;



  if (dimx<0 || dimx>2048)
    {
      this->SwapBytes=1;
      dimx=pxanaswapint(dimx);
      dimy=pxanaswapint(dimy);
      dimz=pxanaswapshort(dimz);
      orient=pxanaswapint(orient);
      sls=pxanaswapshort(sls);
    }

  


  this->SetDataDimensions(dimx,dimy);
  this->SetImageRange(1,dimz);
  this->SetDataSpacing(voxel_width,voxel_height,voxel_depth);
  this->SetNumTimeFrames(1);

  switch(orient)
    {
    case 1: // Saggital
      this->Orientation=2;
      break;
      
    case 2: // Coronal
      this->Orientation=1;
      break;

    default: // Axial
      this->Orientation=0;
    }


  if (hdr->dmd==1)
    this->FileType=1;
  else
    this->FileType=2;

  this->HeaderSize=2048;

  delete hdr;
  return 8*this->FileType;
}


