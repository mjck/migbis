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

// pxutil .h 

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : pxxanalyse.h

_Description : Various Utility Routines

(1) Library routines to read analyse file headers
(2) Filename parsing utilities etc                         
	       
	       

_Call : 
   int getparams(char* filename,int& x,int& y,int &z);
     
_References : 
              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in August 22nd 1995
                    2. Updated Binary File Reading 31 Aug 95
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
#ifndef _PXAnalyse
#define  _PXAnalyse

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bis_znzlib.h"
#include <math.h>

#ifndef _WIN32
	#include <unistd.h>
#endif

#ifndef M_PI
#define M_PI 3.1415926854
#endif


/*
 *
 * (c) Copyright, 1986-1993
 * Biomedical Imaging Resource
 * Mayo Foundation
 *
 * dbh.h
 *
 */

struct header_key               /* header_key        */
{                                   /* off + size*/
  int sizeof_hdr;                 /* 0 + 4     */
  char data_type[10];             /* 4 + 10    */
  char db_name[18];               /* 14 + 18   */
  int extents;                    /* 32 + 4    */
  short int session_error;        /* 36 + 2    */
  char regular;                   /* 38 + 1    */
  char hkey_un0;                  /* 39 + 1    */
};                                  /* total=40  */

struct image_dimension          /* image_dimension   */
{                                   /* off + size*/
  short int dim[8];               /* 0 + 16    */
  char vox_units[4];              /* 16 + 4    */
  char cal_units[8];              /* 20 + 4    */
  short int unused1;              /* 24 + 2    */
  short int datatype;             /* 30 + 2    */
  short int bitpix;               /* 32 + 2    */
  short int dim_un0;              /* 34 + 2    */
  float pixdim[8];                /* 36 + 32   */
  /*
     pixdim[] specifies the voxel dimensions:
     pixdim[1] - voxel width
     pixdim[2] - voxel height
     pixdim[3] - interslice distance
     ..etc
	 */
  float vox_offset;               /* 68 + 4    */
  float funused1;                 /* 72 + 4    */
  float funused2;                 /* 76 + 4    */
  float funused3;                 /* 80 + 4    */
  float cal_max;                  /* 84 + 4    */
  float cal_min;                  /* 88 + 4    */
  int compressed;                 /* 92 + 4    */
  int verified;                   /* 96 + 4    */
  int glmax, glmin;               /* 100 + 8   */
};

struct data_history             /* data_history       */
{                                   /* off + size*/
  char descrip[80];               /* 0 + 80    */
  char aux_file[24];              /* 80 + 24   */
  char orient;                    /* 104 + 1   */
  short int origin[3];            /* 105 + 10  */
  char generated[10];             /* 115 + 10  */
  char scannum[10];               /* 125 + 10  */
  char patient_id[10];            /* 135 + 10  */
  char exp_date[10];              /* 145 + 10  */
  char exp_time[10];              /* 155 + 10  */
  char hist_un0[3];               /* 165 + 3   */
  int views;                      /* 168 + 4   */
  int vols_added;                 /* 172 + 4   */
  int start_field;                /* 176 + 4   */
  int field_skip;                 /* 180 + 4   */
  int omax,omin;                  /* 184 + 8   */
  int smax,smin;                  /* 192 + 8   */
};                                  /* total=200 */

struct dsr                      /* dsr               */
{                                   /* off + size*/
  struct header_key hk;           /* 0 + 40    */
  struct image_dimension dime;    /* 40 + 108  */
  struct data_history hist;       /* 148 + 200 */
};                                  /* total=348 */

/* Acceptable values for hdr.dime.datatype */

#define DT_UNKNOWN              0
#define DT_BINARY               1
#define DT_UNSIGNED_CHAR        2
#define DT_SIGNED_SHORT         4
#define DT_SIGNED_INT           8
#define DT_FLOAT                16
#define DT_COMPLEX              32
#define DT_DOUBLE               64
#define DT_RGB                  128


// -------------------------------------------------------------------
// gelx.h
// -------------------------------------------------------------------

#define RASPOINT float
#define REFCHANTYPE short int
#define IMATRIXTYPE short int
#define DIMXYTYPE float
#define PIXSIZETYPE float
#define BLOCK char /* not null terminated */

/* long should be 4 bytes */
#define LONG4 int
/*#define ATOMIC long */
#define ATOMIC int

typedef struct LHS_VARTYP {
unsigned LONG4 length; /* length of the data */
char * data;
/* pointer to the data */
} LHS_VARTYPE;
/* typedef struct LHS_VARTYP LHS_VARTYPE */

typedef struct SUITEDATATYPE {
BLOCK su_id [4]; /*Suite ID*/
short int su_uniq; /*(internal use) Make Unique Flag*/
char su_diskid; /*(internal use) Disk ID*/
char prodid [13]; /*(internal use) Product ID*/
BLOCK su_verscre [2]; /*(internal use) Genesis Version of Record*/
BLOCK su_verscur [2]; /*(internal use) Genesis Version of Record*/
unsigned LONG4 su_checksum; /*(internal use) Suite Record Checksum*/
BLOCK su_padding [85]; /*(internal use) Spare Space*/
} SUITEDATATYPE ;

typedef struct EXAMDATATYPE {
BLOCK ex_suid [4]; /*Suite ID for this Exam*/
short int ex_uniq; /*(internal use) The Make± Unique Flag*/
char ex_diskid; /*(internal use) Disk ID for this Exam*/
unsigned short int ex_no; /*Exam Number*/
char hospname [33]; /*Hospital Name*/
short int detect; /*Detector Type*/
int numcells; /*Number of cells in det*/
float zerocell; /*Cell number at theta*/
float cellspace; /*Cell spacing*/
float srctodet; /*Distance from source to detector*/
float srctoiso; /*Distance from source to iso*/
short int tubetyp; /*Tube type*/
short int dastyp; /*DAS type*/
short int num_dcnk; /*Number of Decon Kernals*/
short int dcn_len; /*Number of elements in Decon Kernal*/
short int dcn_density; /*Decon Kernal density*/
short int dcn_stepsize; /*Decon Kernal stepsize*/
short int dcn_shiftcnt; /*Decon Kernal Shift Count*/
int magstrength; /*Magnet strength (in gauss)*/
char patid [13]; /*Patient ID for this Exam*/
char patname [25]; /*Patient Name*/
short int patage; /*Patient Age (years, months or days)*/
short int patian; /*Patient Age Notation*/
short int patsex; /*Patient Sex*/
int patweight; /*Patient Weight*/
short int trauma; /*Trauma Flag*/
char hist [61]; /*Patient History*/
char reqnum [13]; /*Requisition Number*/
int ex_datetime; /*Exam date/time stamp*/
char refphy [33]; /*Referring Physician*/
char diagrad [33]; /*Diagnostician/Radiologist*/
char op [4]; /*Operator*/
char ex_desc [23]; /*Exam Description*/
char ex_typ [3]; /*Exam Type*/
short int ex_format; /*Exam Format*/
char firstaxtime[20]; /*Start time(secs) of first axial in exam: double 
		      doesn't work on linux */
char ex_sysid [9]; /*Creator Suite and Host*/
int ex_lastmod; /*Date/Time of Last Change*/
short int protocolflag; /*Non± Zero indicates Protocol Exam*/
char ex_alloc_key [13]; /*Process that allocated this record*/
ATOMIC ex_delta_cnt; /*(internal use) Number of updates to header*/
BLOCK ex_verscre [2]; /*Genesis Version ± Created*/
BLOCK ex_verscur [2]; /*Genesis Version ± Now*/
unsigned LONG4 ex_checksum; /*(internal use) Exam Record Checksum*/
ATOMIC ex_complete; /*(internal use) Exam Complete Flag*/
ATOMIC ex_seriesct; /*(internal use) Last Series Number Used*/
ATOMIC ex_numarch; /*(internal use) Number of Series Archived*/
ATOMIC ex_numseries; /*(internal use) Number of Series Existing*/
LHS_VARTYPE ex_series; /*(internal use) Series Keys for this Exam*/
ATOMIC ex_numunser; /*(internal use) Number of Unstored Series*/
LHS_VARTYPE ex_unseries; /*(internal use) Unstored Sers Keys in Exam*/
ATOMIC ex_toarchcnt; /*(internal use) # of Unarchived Series*/
LHS_VARTYPE ex_toarchive; /*(internal use) Unarchived Sers Keys in Ex*/
ATOMIC ex_prospcnt; /*(internal use) # of Prosp/Scout Series*/
LHS_VARTYPE ex_prosp; /*(internal use) Prosp/Scout Sers Keys in Ex */
ATOMIC ex_modelnum; /*(internal use) Last Model Number used*/
ATOMIC ex_modelcnt; /*(internal use) Number of ThreeD Models*/
LHS_VARTYPE ex_models; /*(internal use) ThreeD Model Keys for Exam*/
short int ex_stat; /*Patient Status*/
BLOCK uniq_sys_id [16]; /*Unique System ID*/
BLOCK service_id [16]; /*Unique Service ID*/
BLOCK mobile_loc [4]; /*Mobile Location Number*/
BLOCK study_uid [32]; /*Study Entity Unique ID*/
short int study_status; /*indicates if study has complete info (DICOM/genesis)*/
BLOCK ex_padding [516]; /*Spare Space*/
} EXAMDATATYPE ;

typedef struct SERIESDATATYPE {
BLOCK se_suid [4]; /*Suite ID for this Series*/
short int se_uniq; /*(internal use) The Make± Unique Flag*/
char se_diskid; /*(internal use) Disk ID for this Series*/
unsigned short int se_exno; /*Exam Number*/
short int se_no; /*Series Number*/
int se_datetime; /*Allocation Series Data/Time stamp*/
int se_actual_dt; /*Actual Series Data/Time stamp*/
char se_desc [30]; /*Series Description*/
char pr_sysid [9]; /*Primary Receiver Suite and Host*/
char pansysid [9]; /*Archiver Suite and Host*/
short int se_typ; /*Series Type*/
short int se_source; /*Series from which prescribed*/
short int se_plane; /*Most like Plane (for L/S)*/
short int scan_type; /*Scout or Axial (for CT)*/
int position; /*Patient Position*/
int entry; /*Patient Entry*/
char anref [3]; /*Anatomical reference*/
float lmhor; /*Horizontal Landmark*/
char prtcl [25]; /*Scan Protocol Name*/
short int se_contrast; /*Non± zero if > 0 image used contrast(L/S)*/
char start_ras; /*RAS letter for first scan location (L/S)*/
float start_loc; /*First scan location (L/S)*/
char end_ras; /*RAS letter for last scan location (L/S)*/
float end_loc; /*Last scan location (L/S)*/
short int se_pseq; /*Last Pulse Sequence Used (L/S)*/
short int se_sortorder; /*(internal use) Image Sort Order (L/S)*/
int se_lndmrkcnt; /*Landmark Counter*/
short int se_nacq; /*Number of Acquisitions*/
short int xbasest; /*Starting number for baselines*/
short int xbaseend; /*Ending number for baselines*/
short int xenhst; /*Starting number for enhanced scans*/
short int xenhend; /*Ending number for enhanced scans*/
int se_lastmod; /*Date/Time of Last Change*/
char se_alloc_key [13]; /*Process that allocated this record*/
ATOMIC se_delta_cnt; /*(internal use) # of updates to header*/
BLOCK se_verscre [2]; /*Genesis Version ± Created*/
BLOCK se_verscur [2]; /*Genesis Version ± Now*/
float se_pds_a; /*PixelData size ± as stored*/
float se_pds_c; /*PixelData size ± Compressed*/
float se_pds_u; /*PixelData size ± UnCompressed*/
unsigned LONG4 se_checksum; /*(internal use) Series Record checksum*/
ATOMIC se_complete; /*(internal use) Series Complete Flag*/
ATOMIC se_numarch; /*(internal use) Number of Images Archived*/
ATOMIC se_imagect; /*(internal use) Last Image Number Used*/
ATOMIC se_numimages; /*(internal use) Number of Images Existing*/
LHS_VARTYPE se_images; /*(internal use) Image Keys for this Series*/
ATOMIC se_numunimg; /*(internal use) Number of Unstored Images*/
LHS_VARTYPE se_unimages; /*(internal use) Unstored Img Keys in Series*/
ATOMIC se_toarchcnt; /*(internal use) # of Unarchived Images*/
LHS_VARTYPE se_toarchive; /*(internal use) Unarchived Img Keys in Sers*/
float echo1_alpha; /*Echo 1 Alpha Value*/
float echo1_beta; /*Echo 1 Beta Value*/
unsigned short int echo1_window; /*Echo 1 Window Value*/
short int echo1_level; /*Echo 1 Level Value*/
float echo2_alpha; /*Echo 2 Alpha Value*/
float echo2_beta; /*Echo 2 Beta Value*/
unsigned short int echo2_window; /*Echo 2 Window Value*/
short int echo2_level; /*Echo 2 Level Value*/
float echo3_alpha; /*Echo 3 Alpha Value*/
float echo3_beta; /*Echo 3 Beta Value*/
unsigned short int echo3_window; /*Echo 3 Window Value*/
short int echo3_level; /*Echo 3 Level Value*/
float echo4_alpha; /*Echo 4 Alpha Value*/
float echo4_beta; /*Echo 4 Beta Value*/
unsigned short int echo4_window; /*Echo 4 Window Value*/
short int echo4_level; /*Echo 4 Level Value*/
float echo5_alpha; /*Echo 5 Alpha Value*/
float echo5_beta; /*Echo 5 Beta Value*/
unsigned short int echo5_window; /*Echo 5 Window Value*/
short int echo5_level; /*Echo 5 Level Value*/
float echo6_alpha; /*Echo 6 Alpha Value*/
float echo6_beta; /*Echo 6 Beta Value*/
unsigned short int echo6_window; /*Echo 6 Window Value*/
short int echo6_level; /*Echo 6 Level Value*/
float echo7_alpha; /*Echo 7 Alpha Value*/
float echo7_beta; /*Echo 7 Beta Value*/
unsigned short int echo7_window; /*Echo 7 Window Value*/
short int echo7_level; /*Echo 7 Level Value*/
float echo8_alpha; /*Echo 8 Alpha Value*/
float echo8_beta; /*Echo 8 Beta Value*/
unsigned short int echo8_window; /*Echo 8 Window Value*/
short int echo8_level; /*Echo 8 Level Value*/
BLOCK series_uid [32]; /*Series Entity Unique ID*/
BLOCK landmark_uid [32]; /*Landmark Unique ID*/
BLOCK equipmnt_uid [32]; /*Equipment Unique ID*/
BLOCK se_padding [588]; /*Spare Space*/
} SERIESDATATYPE ;

typedef struct MRIMAGEDATATYPE {
BLOCK im_suid [4]; /*Suite id for this image*/
short int im_uniq; /*(internal use) The Make± Unique Flag*/
char im_diskid; /*(internal use) Disk ID for this Image*/
unsigned short int im_exno; /*Exam number for this image*/
short int im_seno; /*Series Number for this image*/
short int im_no; /*Image Number*/
int im_datetime; /*Allocation Image date/time stamp*/
int im_actual_dt; /*Actual Image date/time stamp*/
float sctime; /*Duration of scan (secs)*/
float slthick; /*Slice Thickness (mm)*/
IMATRIXTYPE imatrix_X; /*Image matrix size ± X*/
IMATRIXTYPE imatrix_Y; /*Image matrix size ± Y*/
float dfov; /*Display field of view ± X (mm)*/
float dfov_rect; /*Display field of view ± Y (if different)*/
DIMXYTYPE dim_X; /*Image dimension ± X*/
DIMXYTYPE dim_Y; /*Image dimension ± Y*/
PIXSIZETYPE pixsize_X; /*Image pixel size ± X*/
PIXSIZETYPE pixsize_Y; /*Image pixel size ± Y*/
BLOCK pdid [14]; /*(internal use) Pixel Data ID*/
char contrastIV [17]; /*IV Contrast Agent*/
char contrastOral [17]; /*Oral Contrast Agent*/
short int contmode; /*Image Contrast Mode*/
short int serrx; /*Series from which prescribed*/
short int imgrx; /*Image from which prescribed*/
short int screenformat; /*Screen Format(8/16 bit)*/
short int plane; /* Plane Type */
float scanspacing; /*Spacing between scans (mm?)*/
short int im_compress; /*Image compression type for allocation*/
short int im_scouttype; /*Scout Type (AP or lateral)*/
char loc_ras; /*RAS letter of image location*/
float loc; /*Image location*/
RASPOINT ctr_R; /*Center R coord of plane image*/
RASPOINT ctr_A; /*Center A coord of plane image*/
RASPOINT ctr_S; /*Center S coord of plane image*/
RASPOINT norm_R; /*Normal R coord*/
RASPOINT norm_A; /*Normal A coord*/
RASPOINT norm_S; /*Normal S coord*/
RASPOINT tlhc_R; /*R Coord of Top Left Hand Corner*/
RASPOINT tlhc_A; /*A Coord of Top Left Hand Corner*/
RASPOINT tlhc_S; /*S Coord of Top Left Hand Corner*/
RASPOINT trhc_R; /*R Coord of Top Right Hand Corner*/
RASPOINT trhc_A; /*A Coord of Top Right Hand Corner*/
RASPOINT trhc_S; /*S Coord of Top Right Hand Corner*/
RASPOINT brhc_R; /*R Coord of Bottom Right Hand Corner*/
RASPOINT brhc_A; /*A Coord of Bottom Right Hand Corner*/
RASPOINT brhc_S; /*S Coord of Bottom Right Hand Corner*/
char forimgrev [4]; /*Foreign Image Revision*/
int tr; /*Pulse repetition time(usec)*/
int ti; /*Pulse inversion time(usec)*/
int te; /*Pulse echo time(usec)*/
int te2; /*Second echo echo (usec)*/
short int numecho; /*Number of echoes*/
short int echonum; /*Echo Number*/
float tbldlta; /*Table Delta*/
float nex; /*Number of Excitations*/
short int contig; /*Continuous Slices Flag*/
short int hrtrate; /*Cardiac Heart Rate (bpm)*/
int tdel; /*Delay time after trigger (msec)*/
float saravg; /*Average SAR*/
float sarpeak; /*Peak SAR*/
short int monsar; /*Monitor SAR flag*/
short int trgwindow; /*Trigger window (% of R± R interval)*/
float reptime; /*Cardiac repetition time*/
short int imgpcyc; /*Images per cardiac cycle*/
short int xmtgain; /*Actual Transmit Gain (.1 db)*/
short int rcvgain1; /*Actual Receive Gain Analog (.1 db)*/
short int rcvgain2; /*Actual Receive Gain Digital (.1 db)*/
short int mr_flip; /*Flip Angle for GRASS scans (deg.)*/
int mindat; /*Minimum Delay after Trigger (uSec)*/
short int cphase; /*Total Cardiac Phase prescribed*/
short int swappf; /*Swap Phase/Frequency Axis*/
short int pauseint; /*Pause Interval (slices)*/
float pausetime; /*Pause Time*/
int obplane; /*Oblique Plane*/
int slocfov; /*Slice Offsets on Freq axis*/
int xmtfreq; /*Center Frequency (0.1 Hz)*/
int autoxmtfreq; /*Auto Center Frequency (0.1 Hz)*/
short int autoxmtgain; /*Auto Transmit Gain (0.1 dB)*/
short int prescan_r1; /*PreScan R1 ± Analog*/
short int prescan_r2; /*PreScan R2 ± Digital*/
int user_bitmap; /*Bitmap defining user CVs*/
short int cenfreq; /*Center Frequency Method*/
short int imode; /*Imaging Mode*/
int iopt; /*Imaging Options*/
short int pseq; /*Pulse Sequence*/
short int pseqmode; /*Pulse Sequence Mode*/
char psdname [33]; /*Pulse Sequence Name*/
int psd_datetime; /*PSD Creation Date and Time*/
char psd_iname [13];/*PSD name from inside PSD*/
short int ctyp; /*Coil Type*/
char cname [17]; /*Coil Name*/
short int surfctyp; /*Surface Coil Type*/
short int surfcext; /*Extremity Coil Flag*/
int rawrunnum; /*RawData Run Number*/
unsigned LONG4 cal_fldstr; /*Calibrated Field Strength (x10 uGauss)*/
short int supp_tech; /*SAT fat/water/none*/
float vbw; /*Variable Bandwidth (Hz)*/
short int slquant; /*Number of slices in this scan group*/
short int gpre; /*Graphically prescribed*/
int intr_del; /*Interimage/interloc delay (uSec)*/
float user0; /*User Variable 0*/
float user1; /*User Variable 1*/
float user2; /*User Variable 2*/
float user3; /*User Variable 3*/
float user4; /*User Variable 4*/
float user5; /*User Variable 5*/
float user6; /*User Variable 6*/
float user7; /*User Variable 7*/
float user8; /*User Variable 8*/
float user9; /*User Variable 9*/
float user10; /*User Variable 10*/
float user11; /*User Variable 11*/
float user12; /*User Variable 12*/
float user13; /*User Variable 13*/
float user14; /*User Variable 14*/
float user15; /*User Variable 15*/
float user16; /*User Variable 16*/
float user17; /*User Variable 17*/
float user18; /*User Variable 18*/
float user19; /*User Variable 19*/
float user20; /*User Variable 20*/
float user21; /*User Variable 21*/
float user22; /*User Variable 22*/
float user23; /*Projection Angle*/
float user24; /*Concat Sat Type Flag*/
char im_alloc_key [13]; /*Process that allocated this record*/
int im_lastmod; /*Date/Time of Last Change*/
BLOCK im_verscre [2]; /*Genesis Version ± Created*/
BLOCK im_verscur [2]; /*Genesis Version ± Now*/
int im_pds_a; /*PixelData size ± as stored*/
int im_pds_c; /*PixelData size ± Compressed*/
int im_pds_u; /*PixelData size ± UnCompressed*/
unsigned LONG4 im_checksum; /*(internal use) AcqRecon record checksum*/
ATOMIC im_archived; /*(internal use) Image Archive Flag*/
ATOMIC im_complete; /*(internal use) Image Complete Flag*/
short int satbits; /*Bitmap of SAT selections*/
short int scic; /*Surface Coil Intensity Correction Flag*/
short int satxloc1; /*R± side SAT pulse loc rel to lndmrk*/
short int satxloc2; /*L± side SAT pulse loc rel to lndmrk*/
short int satyloc1; /*A± side SAT pulse loc rel to lndmrk*/
short int satyloc2; /*P± side SAT pulse loc rel to lndmrk*/
short int satzloc1; /*S± side SAT pulse loc rel to lndmrk*/
short int satzloc2; /*I± side SAT pulse loc rel to lndmrk*/
short int satxthick; /*Thickness of X± axis SAT pulse*/
short int satythick; /*Thickness of Y± axis SAT pulse*/
short int satzthick; /*Thickness of Z± axis SAT pulse*/
short int flax; /*Phase contrast flow axis*/
short int venc; /*Phase contrast velocity encoding*/
short int thk_disclmr; /*Slice Thickness*/
short int ps_flag; /*Auto/Manual Prescan flag*/
short int ps_status; /*Bitmap of changed values*/
short int image_type; /*Magnitude, Phase, Imaginary, or Real*/
short int vas_collapse; /*Collapse Image*/
float user23n; /*User Variable 23*/
float user24n; /*User Variable 24*/
short int proj_alg; /*Projection Algorithm*/
char proj_name [13]; /*Projection Algorithm Name*/
float x_axis_rot; /*X Axis Rotation*/
float y_axis_rot; /*Y Axis Rotation*/
float z_axis_rot; /*Z Axis Rotation*/
int thresh_min1; /*Lower Range of Pixels 1*/
int thresh_max1; /*Upper Range of Pixels 1*/
int thresh_min2; /*Lower Range of Pixels 2*/
int thresh_max2; /*Upper Range of Pixels 2*/
short int echo_trn_len; /*Echo Train Length for Fast Spin Echo*/
short int frac_echo; /*Fractional Echo ± Effective TE Flag*/
short int prep_pulse; /*Preparatory Pulse Option*/
short int cphasenum; /*Cardiac Phase Number*/
short int var_echo; /*Variable Echo Flag*/
char ref_img; /*Reference Image Field*/
char sum_img; /*Summary Image Field*/
unsigned short int img_window; /*Window Value*/
short int img_level; /*Level Value*/
int slop_int_1; /*Integer Slop Field 1*/
int slop_int_2; /*Integer Slop Field 2*/
int slop_int_3; /*Integer Slop Field 3*/
int slop_int_4; /*Integer Slop Field 4*/
int slop_int_5; /*Integer Slop Field 5*/
float slop_float_1; /*Float Slop Field 1*/
float slop_float_2; /*Float Slop Field 2*/
float slop_float_3; /*Float Slop Field 3*/
float slop_float_4; /*Float Slop Field 4*/
float slop_float_5; /*Float Slop Field 5*/
char slop_str_1 [16]; /*String Slop Field 1*/
char slop_str_2 [16]; /*String Slop Field 2*/
short int scanactno; /*Scan Acquisition Number*/
short int vasflags; /*Magnitude Weighting Flag*/
float vencscale; /*Scale Weighted Venc*/
short int integrity; /*GE Image Integrity*/
int fphase; /*Number Of Phases*/
short int freq_dir; /*Frequency Direction*/
short int vas_mode; /*Vascular Mode*/
BLOCK image_uid [32]; /*Image Unique ID*/
BLOCK sop_uid [32]; /*Service Obj Class Unique ID*/
short int dont_use_1; /*This field is not used*/
short int dont_use_2; /*This field is not used*/
short int dont_use_3; /*This field is not used*/
short int pscopts; /*bitmap of prescan options*/
short int asoffsetx; /*gradient offset in X± direction*/
short int asoffsety; /*gradient offset in Y± direction*/
short int asoffsetz; /*gradient offset in Z± direction*/
short int unoriginal; /*identifies image as original or unoriginal*/
short int interleaves; /*number of EPI shots*/
short int effechospace; /*effective echo spacing for EPI*/
short int viewsperseg; /*views per segment*/
short int rbpm; /*respiratory rate, breaths per min*/
short int rtpoint; /*respiratory trigger point as percent of max.*/
short int rcvrtype; /*type of receiver used*/
float dbdt; /*peak rate of change of gradient field, tesla/sec*/
float dbdtper; /*limit in units of percent of theoretical curve*/
float estdbdtper; /*PSD estimated limit in units of percent*/
float estdbdtts; /*PSD estimated limit in Teslas/sec*/
float saravghead; /*Avg head SAR*/
float neg_scanspacing; /*Negative scan spacing for overlap slices*/
int offsetfreq; /*Offset Frequency ± Mag.Transfer*/
unsigned LONG4 user_usage_tag; /*Defines how following user CVs are to be filled in*/
/*Default value = 0x00000000*/
/*GE range = 0x00000001 ± 0x7fffffff*/
/*Research = 0x80000000 ± 0xffffffff*/
unsigned LONG4 user_fill_mapMSW; /*Define what process fills in the user CVs, ifcc or TIR*/
unsigned LONG4 user_fill_mapLSW; /*Define what process fills in the user CVs, ifcc or TIR*/
float user25; /*User Variable 25*/
float user26; /*User Variable 26*/
float user27; /*User Variable 27*/
float user28; /*User Variable 28*/
float user29; /*User Variable 29*/
float user30; /*User Variable 30*/
float user31; /*User Variable 31*/
float user32; /*User Variable 32*/
float user33; /*User Variable 33*/
float user34; /*User Variable 34*/
float user35; /*User Variable 35*/
float user36; /*User Variable 36*/
float user37; /*User Variable 37*/
float user38; /*User Variable 38*/
float user39; /*User Variable 39*/
float user40; /*User Variable 40*/
float user41; /*User Variable 41*/
float user42; /*User Variable 42*/
float user43; /*User Variable 43*/
float user44; /*User Variable 44*/
float user45; /*User Variable 45*/
float user46; /*User Variable 46*/
float user47; /*User Variable 47*/
float user48; /*User Variable 48*/
int slop_int_6; /*Integer Slop Field 6*/
int slop_int_7; /*Integer Slop Field 7*/
int slop_int_8; /*Integer Slop Field 8*/
int slop_int_9; /*Integer Slop Field 9*/
BLOCK mr_padding [32]; /*Spare Space*/
} MRIMAGEDATATYPE ;


typedef struct PixHdr{
int img_magic; /* magic number */
int img_hdr_length; /* length of pixel header in bytes, can be used as
a byte displacement to the pixel data from
the start of the Pixel Header*/
int img_width; /* width (pixels) of image */
int img_height; /* height (pixels) of image */
int img_depth; /* depth (1, 8, 16, or 24 bits) of pixel */
int img_compress; /* type of compression; see IC_* below
1=IC_RECT Non± compressed, normal rectangular image.
2=IC_PACKED Image is line length map packed.
3=IC_COMPRESSED Image compressed via DCPM only.
4=IC_COMPACK Image is compressed and packed. */
int img_dwindow; /* default window setting */
int img_dlevel; /* default level setting */
int img_bgshade; /* background shade to use for non± image */
int img_ovrflow; /* overflow value */
int img_undflow; /* underflow value */
int img_top_offset; /* number of blank lines at image top */
int img_bot_offset; /* number of blank lines at image bottom */
short img_version; /* version of the header structure
and a word to maintain 32 bit alignment */
unsigned short img_checksum; /* 16 bit end_around_carry sum of pixels */
int img_p_id; /* a byte disp to unique image identifier */
int img_l_id; /* byte length of unique image identifier */
int img_p_unpack; /* a byte disp to 'unpack control' */
int img_l_unpack; /* byte length of 'unpack control' */
int img_p_compress; /* a byte disp to 'compression control' */
int img_l_compress; /* byte length of 'compression control' */
int img_p_histo; /* a byte disp to 'histogram control' */
int img_l_histo; /* byte length of 'histogram control' */
int img_p_text; /* a byte disp to 'text plane data' */
int img_l_text; /* byte length of 'text plane data' */
int img_p_graphics; /* a byte disp to 'graphics plane data' */
int img_l_graphics; /* byte length of 'graphics plane data' */
int img_p_dbHdr; /* a byte disp to 'data base header data' */
int img_l_dbHdr; /* byte length of 'data base header data' */
int img_levelOffset;/* value to add to stored Pixel Data values
to get the correct presentation value */
int img_p_user; /* byte displacement to user defined data */
int img_l_user; /* byte length of user defined data */
} PixHdr;

typedef struct ImgID {
int id_version;
/* version number of text area */
char id_text[80]; /* 80 character descriptive text string */
} ImgID;

#define ID_VERSION 1 /* version number of this structure */

#define V_ADIN(_x) (_x >= 1 && _x <= 2)
/* 1 ±> DAS Filter Card */
/* 2 ->> Aux */
#define V_AUTOSCANFLAG(_x) (_x >=0 && _x <= 2)
/* 0 ->> Autoscan off */ 
/* 1 ->> Autoscan second */
/* 2 ->> Autoscan all */ 
#define V_BIOP_POS(_x) (_x == 1 || _x == 2 || _x == 4)
/* 1 ->> Superior */ 
/* 2 ->> Centered */ 
/* 4 ->> Inferior */
#define V_CALFREQ(_x) (_x >= 0 && _x <= 1)
#define V_CALMODE(_x) (_x >= 1 && _x <= 3)
/* 1 ->> DC Das Cal */ 
/* 2 ->> AC Das Cal */
/* 3 ->> No Das Cal */ 
#define V_COMPUTETYP(_x) (_x >= 1 && _x <= 5)
/* 1 ->> axial */
/* 2 ->> calcheck */ 
/* 3 ->> scout */
/* 4 ->> ppscan */ 
/* 5 ->> viewsvschannels */
#define V_CONTIG(_x) (_x >= 0 && _x <= 1)  */ 
/*~ 0 ->> No:/I */ 
/*~ 1 ->> Yes:/C */ 
#define V_CONTMODE(_x) (_x >= 0 && _x <= 3)
/* 0 ->> No Contrast: */
/* 1 ->> oral: */ 
/* 2 ->> intravenous:+C */
/* 3 ->> oralandintravenous:+C */ 
#define V_CTYP(_x) (_x >= 1 && _x <= 3)
/* 1 ->> Head */
/* 2 ->> Body */ 
/* 3 ->> Surface */
#define V_DASTYP(_x) (_x >= 1 && _x <= 2)
/* 1 ->> CDAS */ 
/* 2 ->> EDAS */
#define V_DATAMOD(_x) (_x >= 1 && _x <= 3)
/* 1 ->> Raw */ 
/* 2 ->> Offset Corrected */
/* 3 ->> Normalized */ 
#define V_DETECT(_x) (_x >= 1 && _x <= 2)
/* 1 ->> Xenon Det */
/* 2 ->> Hilight Det */ 
#define V_ENTRY(_x) (_x >= 1 && _x <= 2)
/* 1 ->> Head First */
/* 2 ->> Feet First */ 
#define V_EX_FORMAT(_x) (_x >= 0 && _x <= 14)
/* 0 ->> Foreign */
/* 1 ->> Genesis Image */ 
/* 2 ->> Technicare HPS1440 */
/* 3 ->> Technicare Delta Scan 2010 */ 
/* 4 ->> Technicare Delta Scan 2020 */
/* 5 ->> Technicare Delta Scan 2060 */ 
/* 6 ->> Signa */
/* 7 ->> CT9800 */ 
/* 8 ->> CT9800 Q */
/* 9 ->> CT9800 QHL */ 
/* 10 ->> EMI_9800 */
/* 11 ->> CT_8800_9800 */ 
/* 12 ->> CT9600 */
/* 13 ->> Jupiter */ 
/* 14 ->> Zeus:CT HiSpeed Adv */
#define V_EX_NO(_x) (_x >= 1 && _x <= 65535)
#define V_EX_STAT(_x) (_x >= 0 && _x <= 3)
/* 0 ->> In Patient:IN */ 
/* 1 ->> Out Patient:OUT */
/* 2 ->> Emergency:EM */ 
/* 3 ->> Referral:REF */
#define V_FILTTYP(_x) (_x >= 1 && _x <= 5)
/* 1 ->> Air Filter */ 
/* 2 ->> Body Filter */
/* 3 ->> Bowtie Flat Filter:Adult Head */ 
/* 4 ->> Flat Filter */
/* 5 ->> High Filter:Adult Head/HF */ 
#define V_FLAX(_x) (_x >= 0 && _x <= 7)
/* 0 ->> FLAX_NONE */
/* 1 ->> FLAXSI:S/I */ 
/* 2 ->> FLAXAP:A/P */
/* 4 ->> FLAXRL:R/L */ 
/* 7 ->> FLAXALL:ALL */
#define V_FPAGAIN(_x) (_x == 0 || _x == 1 || _x == 8 || _x == 64)
/* 0 ->> auto */ 
/* 1 ->> X1 */
/* 8 ->> X8 */ 
/* 64 ->> X64 */
#define V_FRAC_ECHO(_x) (_x >= 0 && _x <= 3)
/* 0 ->> FRACT_ECHO_OFF: */ 
/* 1 ->> FRACT_ECHO_FRACTIONAL:Fr */
/* 2 ->> FRACT_ECHO_EFFECTIVE:Ef */ 
/* 3 ->> FRACT_ECHO_FRACT_EFF:FE */
#define V_GANDIR(_x) (_x >= 1 && _x <= 2)
/* 1 ->> Clockwise:CW */ 
/* 2 ->> Counter Clockwise:CCW */
#define V_GENMODE(_x) (_x >= 1 && _x <= 3)
/* 1 ->> Surface Only */ 
/* 2 ->> Projection Only */
/* 3 ->> Surface and Projection */ 
#define V_GPRE(_x) (_x >= 0 && _x <= 2)
/* 0 ->> No GRX */
/* 1 ->> GRX 1 localizer */ 
/* 2 ->> GRX 2 localizers */
#define V_IBONEFLAG(_x) (_x >= 0 && _x <= 1)
/*~ 0 ->> No: */ 
/*~ 1 ->> Yes:/I */
#define V_IMODE(_x) (_x >= 1 && _x <= 8)
/* 1 ->> Two D:2D */ 
/* 2 ->> Three D Volume:3D */
/* 3 ->> Three D Fourier: */ 
/* 4 ->> Cine:Cine */
/* 5 ->> Angiography:ANGIO */ 
/* 6 ->> Spectroscopy:SPECT */
/* 7 ->> Fluoroscopy:FLUOR */ 
/* 8 ->> Research IMode:RM */
#define V_IMSCOUTTYPE(_x) (_x >= 1 && _x <= 2)
/* 1 ->> Scout Vertical */ 
/* 2 ->> Scout Horizontal */
#define V_IM_NO(_x) (_x >= 1 && _x <= 999)
#define V_IMAGE_TYPE(_x) (_x >= 0 && _x <= 3)
/* 0 ->> IMAGE_TYPE_0 */ 
/* 1 ->> IMAGE_TYPE_1:PHASE */
/* 2 ->> IMAGE_TYPE_2:REAL */ 
/* 3 ->> IMAGE_TYPE_3:IMAGINARY */
#define V_IOPT(_x) (_x >= 0 && _x <= 1073741824)
/*~ 0 ->> No: */ 
/* 1 ->> EG */
/* 2 ->> RESP */ 
/* 4 ->> RC */
/* 8 ->> FC */ 
/* 16 ->> CL */
/* 32 ->> ST */ 
/* 64 ->> PG */
/* 128 ->> NP */ 
/* 256 ->> NF */
/* 512 ->> RT */ 
/* 1024 ->> VB */
/* 2048 ->> ED */ 
/* 4096 ->> PM */
/* 8192 ->> SQ */ 
/* 16384 ->> cs */
/* 32768 ->> MP */ 
/* 65536 ->> SQPIX */
/* 131072 ->> MT */ 
/* 262144 ->> RAMPED_SI */
/* 524288 ->> RAMPED_IS */ 
/* 1048576 ->> RAMPED_AP */
/* 2097152 ->> RAMPED_PA */ 
/* 4194304 ->> RAMPED_RL */
/* 8388608 ->> RAMPED_LR */ 
/* 16777216 ->> FC_FREQ_AXIS */
/* 33554432 ->> FC_SLICE_AXIS */ 
/* 67108864 ->> EPI */
/* 134217728 ->> TRF */ 
/* 268435456 ->> RTR */
/* 536870912 ->> FT */ 
/* 1073741824 ->> SS_FSE */
#define V_OUTSRC(_x) (_x >= 1 && _x <= 2)
/* 1 ->> AID */ 
/* 2 ->> XM */
#define V_OVERRNG_CORR(_x) (_x == 0 || _x == 1)
/*~ 0 ->> No: */ 
/*~ 1 ->> Yes: */
#define V_PATIAN(_x) (_x >= 0 && _x <= 3)
/* 0 ->> Years:Y */ 
/* 1 ->> Months:M */
/* 2 ->> Days:D */ 
/* 3 ->> Weeks:W */
#define V_PATSEX(_x) (_x >= 1 && _x <= 2)
/* 1 ->> Male:M */ 
/* 2 ->> Female:F */
#define V_PERISFLAG(_x) (_x >= 0 && _x <= 1)
/*~ 0 ->> No: */ 
/*~ 1 ->> Yes:/P */
#define V_PHANTSIZE(_x) (_x >= 1 && _x <= 4)
/* 1 ->> No Psize */ 
/* 2 ->> Small Psize */
/* 3 ->> Medium Psize */ 
/* 4 ->> Large Psize */
#define V_PHANTTYP(_x) (_x >= 1 && _x <= 4)
/* 1 ->> None */ 
/* 2 ->> Air */
/* 3 ->> Water */ 
/* 4 ->> Poly */
#define V_PLANE(_x) (_x == 1 || _x == 2 || _x == 4 || _x == 8 || _x == 16 || _x == 18 || _x == 20 || _x == 24 || _x == 32 || _x == 64 || _x == 128 || _x == 256)
/* 1 ->> Scout Plane:SCT */ 
/* 2 ->> Axial Plane:Ax */
/* 4 ->> Sagittal Plane:Sag */ 
/* 8 ->> Coronal Plane:Cor */
/* 16 ->> Oblique Plane:O */ 
/* 18 ->> Oblique Axial Plane:OAx */
/* 20 ->> Oblique Sagittal Plane:OSag */ 
/* 24 ->> Oblique Coronal Plane:OCor */
/* 32 ->> ParAxial:PAX */ 
/* 64 ->> Reformatted Plane:RFMT */
/* 128 ->> Projected Plane:PJN */ 
/* 256 ->> Mixed Plane:MIXED */
#define V_POSITION(_x) (_x == 1 || _x == 2 || _x == 4 || _x == 8)
/* 1 ->> Supine */ 
/* 2 ->> Prone */
/* 4 ->> Decubitus Left */ 
/* 8 ->> Decubitus Right */
#define V_PREP_PULSE(_x) (_x >= 0 && _x <= 2) 
/* 0 ->> PREP_PULSE_NONE: */
/* 1 ->> PREP_PULSE_IR:IR */ 
/* 2 ->> PREP_PULSE_DE:DE */
#define V_PROJ_ALG(_x) (_x >= 0 && _x <= 3)
/* 0 ->> NO_PROJ_ALG: */ 
/* 1 ->> PROTOTYPE_ALG: */
/* 2 ->> MIN_PIXEL_ALG:Min */ 
/* 3 ->> MAX_PIXEL_ALG:Max */
#define V_PS_FLAG(_x) (_x >= 0 && _x <= 5)
/* 0 ->> PS_FLAG_NONE: */ 
/* 1 ->> PS_FLAG_AF:a */
/* 2 ->> PS_FLAG_AS:A */ 
/* 3 ->> PS_FLAG_M:M */
/* 4 ->> PS_FLAG_AFM:aM */ 
/* 5 ->> PS_FLAG_ASM:AM */
#define V_PS_STATUS(_x) (_x == 1 || _x == 2 || _x == 4 || _x == 8)
/* 1 ->> PS_STATUS_CF:CF */ 
/* 2 ->> PS_STATUS_TA:TA */
/* 4 ->> PS_STATUS_R1:R1 */ 
/* 8 ->> PS_STATUS_R2:R2 */
#define V_PSC(_x) (_x ==0 || _x ==1)
/*~ 0 ->> No: */ 
/*~ 1 ->> Yes: */
#define V_PSEQ(_x) (_x >= 0 && _x <= 25)
/* 0 ->> SE */ 
/* 1 ->> IR */
/* 2 ->> RM:RM: */ 
/* 3 ->> RMGE: */
/* 4 ->> GRE:GR */ 
/* 5 ->> MPGR */
/* 6 ->> MPIRS:IR/s */ 
/* 7 ->> MPIRI:IR */
/* 8 ->> VOGRE:3D/GR */ 
/* 9 ->> CINEGRE:Cine/GR */
/* 10 ->> SPGR */ 
/* 11 ->> SSFP */
/* 12 ->> TF:TOF */ 
/* 13 ->> PC */
/* 14 ->> CINSPGR:Cine/SPGR */ 
/* 15 ->> TOFGR:TOF/GR */
/* 16 ->> TOFSPGR:TOF/SPGR */ 
/* 17 ->> PCGR:PC/GR */
/* 18 ->> PCSPGR:PC/SPGR */ 
/* 19 ->> FSE */
/* 20 ->> FGR */ 
/* 21 ->> FMPGR */
/* 22 ->> FSPGR */ 
/* 23 ->> FMPSPGR */
/* 24 ->> SPECT */ 
/* 25 ->> PSEQ_MIXED:MIXED */
#define V_PSEQMODE(_x) (_x >= 1 && _x <= 3)
/* 1 ->> Product */ 
/* 2 ->> Research Mode */
/* 3 ->> Research Mode GE */ 
#define V_RECONALG(_x) (_x == 1 || _x == 2 || _x == 4 || _x == 8 || _x == 16 || _x == 32 || _x == 64 || _x == 128)
/* 1 ->> smooth:SMTH */
/* 2 ->> soft:SOFT */ 
/* 4 ->> stand:STND */
/* 8 ->> detail:DETL */ 
/* 16 ->> bone:BONE */
/* 32 ->> edge:EDGE */ 
/* 64 ->> sharp:SHRP */
/* 128 ->> experimental 2:EXP2 */ 
#define V_RECONDATASET(_x) (_x == 1 || _x == 2 || _x == 4 || _x == 8)
/* 1 ->> half scan views */
/* 2 ->> normal scan views */ 
/* 4 ->> over scan views */
/* 8 ->> variable views */ 
#define V_REF_CHAN(_x) (_x == 0 || _x == 1)
/*~ 0 ->> no: */
/*~ 1 ->> yes: */ 
#define V_REGXM(_x) (_x >= 1 && _x <= 2)
/* 1 ->> hold */
/* 2 ->> shift */ 
#define V_ROTTYP(_x) (_x == 1 || _x == 2 || _x == 4 || _x == 8)
/* 1 ->> half scan:rotating */
/* 2 ->> normal scan:rotating */ 
/* 4 ->> over scan:rotating */
/* 8 ->> stationary:stationary */ 
#define V_ROTORSPEED(_x) (_x >= 1 && _x <= 4)
/* 1 ->> Rotor off */
/* 2 ->> Rotor low */ 
/* 3 ->> Rotor medium */
/* 4 ->> Rotor high */ 
#define V_SATBITS(_x) (_x >= 0 && _x <= 11)
/* 0 ->> SATBITS_S:S */
/* 1 ->> SATBITS_I:I */ 
/* 2 ->> SATBITS_R:R */
/* 4 ->> Scout mode */ 
/* 8 ->> Axial Xron mode */
/* 16 ->> Axial Xroff mode */ 
/* 32 ->> Static Xron mode */
/* 64 ->> Static Xroff mode */ 
/* 128 ->> Tube Heat mode */
/* 256 ->> Das mode */ 
/* 512 ->> Tube Cal mode */
/* 1024 ->> Biopsy mode */ 
/* 2048 ->> Cine mode */
/* 4096 ->> Helical mode */ 
/* 8192 ->> Rotgencal mode */
#define V_SCAN_TYPE(_x) (_x >= 1 && _x <= 3)
/* 1 ->> Scout Scan:SCT */ 
/* 2 ->> Axial Scan:AX */
/* 3 ->> Screensave Scan:SS */ 
#define V_SCIC(_x) (_x == 0 || _x == 1)
/* 0 ->> SCIC_OFF */
/* 1 ->> SCIC_ON:IIC */ 
#define V_SE_NO(_x) (_x >= 1 && _x <= 999)
#define V_SE_TYP(_x) (_x >= 1 && _x <= 9)
/* 1 ->> Prospective:PROSP */
/* 2 ->> Retrospective:RETRO */ 
/* 3 ->> Scout Series:SCOUT */
/* 4 ->> Reformatted:REFMT */ 
/* 5 ->> Scrnsave:SSAVE */
/* 6 ->> Xenon:XENON */ 
/* 7 ->> Three D:3D */
/* 8 ->> Service:SERV */ 
/* 9 ->> Projected:PJN */
#define V_SFOVTYP(_x) (_x == 1 || _x == 2 || _x == 4 || _x == 8 || _x == 16 || _x == 32) 
/* 1 ->> Ped Head */
/* 2 ->> Adult Head */ 
/* 4 ->> Small */
/* 8 ->> Medium */ 
/* 16 ->> Large */
/* 32 ->> No Sfovtype */ 
#define V_SPOT_POS(_x) (_x >= 1 && _x <= 3)
/* 1 ->> spot left */
/* 2 ->> spot center */ 
/* 3 ->> spot right */
#define V_SPOT_SIZE(_x) (_x >= 1 && _x <= 2) 
/* 1 ->> small spot */
/* 2 ->> large spot */ 
#define V_SUPP_TECH(_x) (_x >= 0 && _x <= 2)
/* 0 ->> SUPP_NONE: */
/* 1 ->> SUPP_FAT:F */ 
/* 2 ->> SUPP_WATER:W */
#define V_SWAPPF(_x) (_x >= 0 && _x <= 1)
/*~ 0 ->> No: */ 
/*~ 1 ->> Yes:SPF */
#define V_THK_DISCLMR(_x) (_x == 0 || _x == 1)
/* 0 ->> THK_DISCLMR_NO: */ 
/* 1 ->> THK_DISCLMR_YES:& */
#define V_TRAUMA(_x) (_x == 0 || _x == 1)
/* 0 ->> Trauma flag off */ 
/* 1 ->> Trauma flag on */
#define V_TRIGMODE(_x) (_x >= 1 && _x <= 4)
/* 1 ->> Normal Trigger:XTTX */ 
/* 2 ->> Xrayoff Trigger:TT */
/* 3 ->> TXXT */ 
/* 4 ->> XXTT */
#define V_TRIGSRC(_x) (_x >= 1 && _x <= 2)
/* 1 ->> internal */ 
/* 2 ->> external */
#define V_TUBETYP(_x) (_x >= 0 && _x <= 14)
/* 0 ->> MX100_TUBE */ 
/* 1 ->> MX125_TUBE */
/* 2 ->> TT9800:9800_TUBE */ 
/* 3 ->> TT9100:9100_TUBE */
/* 4 ->> HPRI_TUBE */ 
/* 5 ->> HPRII_TUBE */
/* 6 ->> ADV_TUBE */ 
/* 6 ->> ADV650_TUBE */
/* 7 ->> ADV681_TUBE */ 
/* 8 ->> ST2000CT_6_TUBE */
/* 9 ->> ST2000CT_10_TUBE */ 
/* 12 ->> MX_135CT_TUBE */
/* 13 ->> MX_165CT_TUBE */ 
/* 14 ->> MX_165CT_I_TUBE */
#define V_VAR_ECHO(_x) (_x == 0 || _x == 1)
/* 0 ->> VAR_ECHO_NO: */ 
/* 1 ->> VAR_ECHO_YES:/V */
#define V_VAS_COLLAPSE(_x) (_x >= 0 && _x <= 7)
/* 0 ->> VAS_COLLAPSE_OFF */ 
/* 1 ->> VAS_COLLAPSE_ON_1:COL */
/* 2 ->> VAS_COLLAPSE_ON_2:MAG */ 
/* 3 ->> VAS_COLLAPSE_ON_3:R/L */
/* 4 ->> VAS_COLLAPSE_ON_4:A/P */ 
/* 5 ->> VAS_COLLAPSE_ON_5:S/I */
/* 6 ->> VAS_COLLAPSE_ON_6:PJN */ 
/* 7 ->> VAS_COLLAPSE_ON_7:ALL */


/******************************************************************************

Utils  Lawrence Staib 10/90, 2/92

******************************************************************************/

struct anaimagedata{/* data associated with an image */
  short zsize,ysize,xsize,tsize;
  float zasp,yasp,xasp;
  float zpix,ypix,xpix;
  int minpix,maxpix;
  int trueminpix,truemaxpix;
  char orient;
  int datatype;
} ;


int writeanahdr(const char* filename,anaimagedata *imdata,int bits);
int readanahdr(const char* filename, anaimagedata *imdata);
int readlxhdr(const char* filename, anaimagedata *imdata);
char *eatsuffix(const char* filename);
char *nothisextension(const char* filename,const char* extension);

/******************************************************************************

Xenios Papademetris 8/95 - Addition of PXImage code

******************************************************************************/

int  Anagetparams(const char* filename,int& x,int& y,int &z);
int  Anagetparams(const char* filename,int& x,int& y,int &z,int &bytes);
int  Anagetparams(const char* filename,int& x,int& y,int &z,int &t,int &bytes);

/******************************************************************************

Xenios Papademetris 8/95 - Addition of PXImage code

Filename Manipulation Utilities

******************************************************************************/

char* anaextractpath(const char* fname);
char* anaeatsuffix(const char* fname);
char* anaeatpath(const char* fname);
char* anacreateheaderfilename(const char* fname);
char* anacreateimagefilename(const char* fname);
char* anacreatefilename(const char* fname,const char* wanted,const char* unwanted);


/* *****************************************************************

String Stuff 4 Jan 2000 to accomodate zlib compression library

***************************************************************** */

// Definitions of Boolean Type
#ifndef PXBOOL
  #ifndef _WIN32
    enum PXBool { PXFalse = 0, PXTrue };
  #else
    #define PXBool bool
    #define PXFalse false
    #define PXTrue true
    #endif
  #define PXBOOL 1
#endif

gzFile ZEXPORT gzsuffixopen (const char* path, const char* mode,int complevel=0);

int gzfindstringinstring(const char* orig,const char* lookfor);
int stringskipspace(const char* line,int startpos);
int stringskipnumber(const char* line,int startpos);
int   getgzstreaminteger(gzFile fin);
float getgzstreamfloat(gzFile fin);

 
// Create backup file for file in fname 
int anabackupfile(const char* fname);


// Byte-Swapping Stuff
// --------------------
int pxanaswapint(int input);
float pxanaswapfloat(float input);
double pxanaswapdouble(double input);
short pxanaswapshort(short input);


// -------------------------------------------------------------
//  Quantization -- quantize numbers to specified decimal places 
// -------------------------------------------------------------
float  quantize(float a,int dp=4);
double quantize(double a,int dp=4);

// -------------------------
//  Return Maxium or Minimum
// -------------------------
float Fmax(float a,float b);
float Fmin(float a,float b);
int   Imax(int a,int b);
int   Imin(int a,int b);

float Frange(float a,float minv,float maxv);
float FcyclicDistance(float p1,float p2,float circum);
int   Irange(int   a,int   minv,int   maxv);
int   Icyclic(int v,int np);
float Fcyclic(float v, float maxv);
int   Ipowerof2(int v);
// --------------------------
// Point Comparisons
// --------------------------
float Fdist(float x1,float y1,float z1,float x2,float y2,float z2);
float Fsqdist(float x1,float y1,float z1,float x2,float y2,float z2);
int   Fsamepoint(float x1,float y1,float z1,float x2,float y2,float z2,float maxdist=0.01);

namespace bioimagesuite_legacy
{
  // Empty for now
}

#endif

