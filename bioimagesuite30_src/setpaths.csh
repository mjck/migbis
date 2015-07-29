# --------------------------------------------------------------------------------------
# This file is part of the BioImage Suite Software Package. (seee www.bioimagesuite.org)
# --------------------------------------------------------------------------------------
#bis_fix_script_begin
setenv BASE /Users/mjack/src3/bioimagesuite30_src/bioimagesuite
setenv BIS_VTK_DIR /usr/local/bioimagesuite3_base64/vtk510/lib/vtk-5.10
setenv BIOIMAGESUITE3_BINARY_DIR /Users/mjack/src3/bioimagesuite30_src
setenv BINPATH /Users/mjack/src3/bioimagesuite30_src/bin
setenv BASELIB /Users/mjack/src3/bioimagesuite30_src/lib
#bis_fixscript_end

setenv BSCRIPT ${BIS_VTK_DIR}/../../../setpaths.csh
if (-r $BSCRIPT) then
    source  $BSCRIPT
else
    setenv VV `which vtk`
    if ( -r $VV ) then
	echo "using vtk from $VV"
    else		
        echo "VTK or $BSCRIPT do not exist"
    endif
    unsetenv VV
endif

unsetenv BSCRIPT
unsetenv BIS_VTK_DIR

# -------------------------------------------------------------------------------------
# Compatibility with other packages
# -------------------------------------------------------------------------------------
setenv FSLDIR /usr/local/fsl
setenv AFNIDIR /usr/local/afni
setenv FSLOUTPUTTYPE NIFTI_PAIR

setenv PATH ${PATH}:${FSLDIR}/bin:${AFNIDIR}
# -------------------------------------------------------------------------------------------
setenv PATH ${BASE}:${BASE}/main:${BINPATH}:${BASE}/apps:${BASE}/mjack:${BASE}/datatree:${BASE}/bis_algorithm:"${PATH}"

setenv OS `uname`
if ( $OS == "Darwin") then
    if ($?DYLD_LIBRARY_PATH) then
	setenv DYLD_LIBRARY_PATH ${BASELIB}:"${DYLD_LIBRARY_PATH}"
    else
	setenv DYLD_LIBRARY_PATH ${BASELIB}
    endif
else
    if ($?LD_LIBRARY_PATH) then
	setenv LD_LIBRARY_PATH ${BASELIB}:"${LD_LIBRARY_PATH}"
    else
	setenv LD_LIBRARY_PATH ${BASELIB}
    endif
endif
unsetenv OS

setenv BIOIMAGESUITE ${BASE}
setenv BIOIMAGESUITE_DIR ${BASE}/include

echo "BioimageSuite ready to run. Type bis.tcl to get the starting menu ($BIOIMAGESUITE)"
unsetenv BASE
unsetenv BASELIB





