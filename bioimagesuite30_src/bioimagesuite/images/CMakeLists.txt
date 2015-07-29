

#BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
#BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
#BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
#BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
#BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
#BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


#
# Source files
#
# Here is where you can add the name of your local common classes.
#

SET (KITSCRIPTS
#
# This are various logos and icons
#
  bioimagesuite.ico
  bioimagesuite.png
  bioimagesuite.xbm
  bioimagesuite_setuplogo.bmp
  bioimagesuite_logo_small_www.gif
  bioimagesuite_logo_www.gif
  mig_logo_www.gif
# This is the MNI to Talairach Lookup table for MNI Space
#
  colin_talairach_lookup.nii.gz
#
# This is legacy stuff ignore
#
  colinlookup.hdr
  colinlookup.img.gz

#
# This is the real MNI stuff
#
  MNI_T1_1mm.nii.gz
  MNI_T1_1mm_mask.nii.gz
  MNI_T1_1mm_stripped.nii.gz
  MNI_T1_1mm_graywhite.nii.gz
  MNI_T1_2mm.nii.gz
  MNI_T1_2mm_mask.nii.gz
  MNI_T1_2mm_stripped.nii.gz
  MNI_T1_2mm_graywhite.nii.gz
  avg152T1_LR_nifti.nii.gz
#
# This is the Yale Broadmann Areas Atlas
#
  yale_broadmann.nii.gz
  yale_broadmann.txt
  MNI_T1_1mm_graywhite.txt
  bioimagesuite_atlas.txt
  README.txt
#
# These are SPECT related
#
  ISASHN_Standard_Deviation.nii.gz
  ISAS_SPECT_Template.nii.gz 
  ISAS_Standard_Deviation.nii.gz
  ISAS_Mean_Diff_SPECT.nii.gz 
  ISAS_SPECT_Mask.nii.gz
#
# This is a sample sqlite database file
#
sampledatabase.sqlite
)

SET(KITAPPS )

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line
# FILELIST4 and 5 are dummy file pointers to make sure that the fixlicense script
# does not parse these in any way -- this causes problems with the Yale Atlas stuff
SET (FILELIST4 ${BIOIMAGESUITE3_BINARY_DIR}/imagelist.txt)
SET (FILELIST5 ${BIOIMAGESUITE3_BINARY_DIR}/imagelist.txt)
bis_complex_tcl(bioimagesuite/images ${FILELIST4} ${FILELIST5})

#MESSAGE("Appending to ${FILELIST}")
FILE(APPEND ${FILELIST} "# ---------------------------------\n# ** images\n#-----------------------------------------\n")
FILE(APPEND ${FILELIST} "${BIOIMAGESUITE_INSTALL_EXTRAPATH}/images/CMakeLists.txt\n")
