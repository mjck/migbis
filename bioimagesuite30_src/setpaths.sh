# --------------------------------------------------------------------------------------
# This file is part of the BioImage Suite Software Package. (seee www.bioimagesuite.org)
# --------------------------------------------------------------------------------------

#bis_fix_script_begin
BASE=/Users/mjack/src3/bioimagesuite30_src/bioimagesuite
BIS_VTK_DIR=/usr/local/bioimagesuite3_base64/vtk510/lib/vtk-5.10
BIOIMAGESUITE3_BINARY_DIR=/Users/mjack/src3/bioimagesuite30_src
BINPATH=/Users/mjack/src3/bioimagesuite30_src/bin
BASELIB=/Users/mjack/src3/bioimagesuite30_src/lib
#bis_fixscript_end

BSCRIPT=${BIS_VTK_DIR}/../../../setpaths.sh

if [ -r $BSCRIPT ]; then
     source $BSCRIPT
else
   VV=`which vtk`
   if [ -r $VV ]; then
	echo "using vtk from $VV"
    else		
        echo "VTK or $BSCRIPT do not exist"
    fi
fi
# -------------------------------------------------------------------------------------
# Compatibility with other packages
# -------------------------------------------------------------------------------------
FSLDIR=/usr/local/fsl
AFNIDIR=/usr/local/afni
FSLOUTPUTTYPE=NIFTI_PAIR

export FSLDIR
export AFNIDIR
export FSLOUTPUTTYPE

PATH=${PATH}:${FSLDIR}/bin:${AFNIDIR}
# -------------------------------------------------------------------------------------




OS=`uname`
if [ $OS == "Darwin" ]; then    
    if [ -n "$DYLD_LIBRARY_PATH" ]; then
	DYLD_LIBRARY_PATH=${BASELIB}:"${DYLD_LIBRARY_PATH}"
    else
	DYLD_LIBRARY_PATH=${BASELIB}
    fi
    export DYLD_LIBRARY_PATH
else
    if [ -n "$LD_LIBRARY_PATH" ]; then
	LD_LIBRARY_PATH=${BASELIB}:"${LD_LIBRARY_PATH}"
    else
	LD_LIBRARY_PATH=${BASELIB}
    fi
    export LD_LIBRARY_PATH
fi

export PATH=${BASE}/main:${BINPATH}:${BASE}/apps:${BASE}/mjack:${BASE}/datatree:${BASE}/bis_algorithm:"${PATH}"

export BIOIMAGESUITE=${BASE}

if [ -n  "$BIOIMAGESUITE3_BINARY_DIR" ]; then
    export BIOIMAGESUITE3_BINARY_DIR	
fi


echo "BioimageSuite ready to run. Type bis.tcl to get the starting menu ($BASE)"

