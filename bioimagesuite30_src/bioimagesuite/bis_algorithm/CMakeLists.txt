#BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
#BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
#BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
#BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
#BIOIMAGESUITE_LICENSE  Medicine, http:#www.bioimagesuite.org.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
#BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
#BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
#BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
#BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
#BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
#BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#BIOIMAGESUITE_LICENSE  See also  http:#www.gnu.org/licenses/gpl.html
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
#BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
#BIOIMAGESUITE_LICENSE 
#BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------



#
# Source files
#
# Here is where you can add the name of your local common classes.
#

SET(KITSCRIPTS
    pkgIndex.tcl
    bis_common.tcl
    bis_util.tcl
    bis_object.tcl
    bis_option.tcl
    bis_optiondialog.tcl
    bis_optiondialog.tcl
    bis_algorithmdbaseinterface.tcl
    bis_basealgorithm.tcl
    bis_algorithm.tcl
    bis_imagetoimagealgorithm.tcl
    bis_imagetosurfacealgorithm.tcl
    bis_basethresholdimage.tcl
    bis_dualimagealgorithm.tcl
    bis_dualimagetransformationalgorithm.tcl
    bis_dualimagereslicetransformationalgorithm.tcl
    bis_imageandsurfacetosurfacealgorithm.tcl
    bis_surfacetosurfacealgorithm.tcl
    bis_landmarkstolandmarksalgorithm.tcl
    bis_baseintensityregistration.tcl
    bis_basepointbasedregistration.tcl
    bis_pointbasedregistrationviewer.tcl
    bis_spectbase.tcl
    bis_baselevelset.tcl
    bis_graphvis.tcl
    bis_guicontainer.tcl
    bis_guiimageutility.tcl
    bis_guisurfacecontrol.tcl
    bis_guisurfaceutility.tcl
    bis_guioverlayutility.tcl 
    bis_guicompareutility.tcl 
    bis_guirpmcontainer.tcl 
    bis_guitreeimageutility.tcl
    bis_guitreecreate4d.tcl
    bis_datamanager.tcl
    bis_treewidget.tcl
    bis_viewer.tcl
    bis_polygonaldisplaymanager.tcl
    bis_xmlalgorithm.tcl
    bis_electrodeeditor.tcl
    bis_landmarkeditor.tcl
    bis_imagelisttoimagealgorithm.tcl
    bis_displayroimean.tcl
    bis_basecontrolhelper.tcl
)

# These are the scripts that are to be tested

SET(KITAPPSTEST
#
# fMRI Test Stuff
#
bis_multisubjectfmri.tcl
bis_computetensor.tcl
bis_tensoranalysis.tcl
#
bis_blendimage.tcl
bis_castimage.tcl
bis_csfwmmotioncorr.tcl
bis_createoverlayimage.tcl
bis_checkerboardimage.tcl
bis_cleansurface.tcl
bis_clipsurface.tcl
bis_clusterthresholdimage.tcl
bis_colorblendimage.tcl
bis_combineframes.tcl
bis_combineimages.tcl
bis_combinetransformations.tcl
bis_computedisplacementfield.tcl
bis_computejointhistogram.tcl
bis_computeglm.tcl
bis_computenormals.tcl
bis_computeoverlap.tcl
bis_computesimilarity.tcl
bis_connect.tcl
bis_convolveimage.tcl
bis_create4dimage.tcl
bis_cropimage.tcl
bis_curvatures.tcl
bis_cylindricalcropimage.tcl
bis_decimate.tcl
bis_delaunay3D.tcl
bis_distancemapimage.tcl
bis_skeletonizeimage.tcl
bis_edgedetectimage.tcl
bis_extractobjectmap.tcl 
bis_extractsurface.tcl
bis_fdr.tcl
bis_fftimage.tcl
bis_flipimage.tcl
bis_imagecompare.tcl
bis_imagedilate.tcl
bis_imagemathoperations.tcl
bis_imagemedian.tcl
bis_imageremoveframes.tcl
bis_intsegment.tcl
bis_linearintensityregister.tcl
bis_linearpointregister.tcl
bis_listthresholdimage.tcl
bis_logimage.tcl
bis_manualregistration.tcl
bis_manualreslice.tcl
bis_maskimage.tcl
bis_matrixcorrelation.tcl
bis_mediantemporalsmoothimage.tcl
bis_nonlineardistortioncorrection.tcl
bis_nonlinearintensityregister.tcl
bis_nonlinearpointregister.tcl
bis_nonlinearsmoothimage.tcl
bis_piecewiseimagemap.tcl
bis_preprocessimage.tcl
bis_proportionalscale.tcl
bis_qianvesselness.tcl
bis_regularizeobjectmap.tcl
bis_relabelimage.tcl
bis_removeslicemean.tcl     
bis_removetemporaldrift.tcl 
bis_reorientimage.tcl
bis_newreorientimage.tcl
bis_resampleimage.tcl
bis_resamplelandmarks.tcl
bis_resliceimage.tcl
bis_rfftimage.tcl
bis_roimean.tcl
bis_rtopvalue.tcl
bis_rtotmap.tcl
bis_seedcorrelation.tcl
bis_shiftscaleimage.tcl
bis_singlejacobian.tcl
bis_singleimagemathoperations.tcl
bis_sliceinhomogeneity.tcl
bis_smoothimage.tcl
bis_exponentialfitimage.tcl
bis_exponentialfitt1.tcl
bis_smoothlandmarks.tcl
bis_smoothsurface.tcl
bis_spectrf.tcl
bis_stitchimage.tcl
bis_stripskull.tcl
bis_subdivide.tcl
bis_temporalsmoothimage.tcl
bis_thresholdimage.tcl
bis_thresholdpoints.tcl
bis_tmaptopvalue.tcl
bis_tmaptozscore.tcl
bis_transformsurface.tcl
bis_triangulate.tcl
bis_ttest.tcl
bis_visualizetransformation.tcl
bis_zscoretopvalue.tcl
bis_ISAS.tcl
bis_ISASHN.tcl
#
bis_singlesubjectfmri.tcl
bis_frangivesselness.tcl
bis_computecorrelations.tcl
bis_biasfield.tcl
bis_levelset.tcl
bis_interleave.tcl
bis_reslicetensor.tcl
#
bis_intrinsicconnectivity.tcl
bis_fmritoolbox.tcl

)

SET(KITLIBS
  vtkpxCommon
  vtkpxCommonTCL
  numerics
  ${BIOIMAGESUITE3_COMMON_LIBS}
)

# Excluded TCL Code 

IF(BIOIMAGESUITE3_MOUSEREG)
SET(KITAPPSTEST
${KITAPPSTEST}
bis_serialdemonmousereg.tcl
)
ENDIF(BIOIMAGESUITE3_MOUSEREG)



SET (KITAPPS
${KITAPPSTEST}
bis_testviewer.tcl
bis_testmatrix.tcl
bis_testdatabase.tcl
# Add applications that have no tests
bis_treeapp.tcl
bis_headerinfo.tcl
bis_makebatch.tcl
bis_split4dimage.tcl
bis_help.tcl
bis_surfaceinfo.tcl
bis_dicom2nifti.tcl
bis_varian2nifti.tcl
#
# These ought to have tests but do not
#
# bis_serialdemonmousereg.tcl
#
)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

bis_complex_tcl(bioimagesuite/bis_algorithm ${FILELIST2} ${FILELIST})



# ---------------------------------------------------------------------------------------------------------------------
# Create scripts and help files
# ---------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------
# Create Wrapper Scripts 
# --------------------------------------------------------------------------------------

SET (BASEDIR ${CMAKE_INSTALL_PREFIX}/${BIOIMAGESUITE3_INSTALL_EXTRAPATH})
SET (TMPBINDIR ${BIOIMAGESUITE3_BINARY_DIR}/bioimagesuite/bis_algorithm)
SET (DOSH 0)
SET (SHSUFFIX "")

IF (UNIX)
  SET (DOSH 1)
 
ELSEIF (BIOIMAGESUITE_USING_MINGW)
  SET (DOSH 1)
  SET (SHSUFFIX ".sh")
ENDIF (UNIX)


# ---------------------------------------------------------
# Create a list of scripts
SET (F33 ${BIOIMAGESUITE3_BINARY_DIR}/bisalgorithmscriptlist.txt)
FILE (WRITE ${F33} "#List of scripts\n")
FOREACH (item ${KITAPPS})
  GET_FILENAME_COMPONENT(TAIL ${item} NAME_WE)
  FILE (APPEND ${F33} "${TAIL}\n")
ENDFOREACH (item ${KITAPPS})
install(FILES ${F33} DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/lib)

# ---------------------------------------------------------
#MESSAGE("USING DOSH=${DOSH}")

IF (DOSH)
  FOREACH (item2 ${KITAPPS})

    GET_FILENAME_COMPONENT(item ${item2} NAME_WE)
    set(item3 ${item}${SHSUFFIX})
    
    configure_file( ${BIOIMAGESUITE3_SOURCE_DIR}/bioimagesuite/origscripts/runscript.in
      ${EXECUTABLE_OUTPUT_PATH}/${item3} @ONLY )

    EXECUTE_PROCESS (COMMAND chmod +x  ${EXECUTABLE_OUTPUT_PATH}/${item3})
    
    install(FILES ${EXECUTABLE_OUTPUT_PATH}/${item3}  DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/bin
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE )
    FILE(APPEND ${SCRIPTFILELIST} "bin/${item}\n")
  ENDFOREACH (item2 ${KITAPPS})
ENDIF (DOSH)
  
IF (WIN32)
  FOREACH (item2 ${KITAPPS})

    GET_FILENAME_COMPONENT(item ${item2} NAME_WE)
    set(item3 ${item}.bat)

    configure_file( ${BIOIMAGESUITE3_SOURCE_DIR}/bioimagesuite/origscripts/runscript.bat.in
         ${EXECUTABLE_OUTPUT_PATH}/${item3} @ONLY )
  
    install(FILES ${EXECUTABLE_OUTPUT_PATH}/${item3}  DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/bin
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE )
    FILE(APPEND ${SCRIPTFILELIST} "bin/${item3}\n")
  ENDFOREACH (item2 ${KITAPPS})
ENDIF(WIN32)

add_custom_command(OUTPUT
  ${BIOIMAGESUITE3_BINARY_DIR}/html/index.html
  DEPENDS ${KITAPPSTEST} vtkbisConnectivityTCL vtkpxGUITCL
  POST_BUILD
  COMMENT     "Creating HTML Help Pages"
  COMMAND 
  ${BIOIMAGESUITE3_BINARY_DIR}/bisexec ${BIOIMAGESUITE3_SOURCE_DIR}/bioimagesuite/bis_algorithm/bis_help.tcl ${BIOIMAGESUITE3_BINARY_DIR}/bisalgorithmscriptlist.txt ${BIOIMAGESUITE3_BINARY_DIR}/html)


add_custom_target(bis_help 
  DEPENDS  ${BIOIMAGESUITE3_BINARY_DIR}/html/index.html 
)

install (DIRECTORY ${BIOIMAGESUITE3_BINARY_DIR}/html DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH})


# ---------------------------------------------------------------------------------------------------------------------
# Testing Stuff -- exclude from release
# ---------------------------------------------------------------------------------------------------------------------
# Excluded TCL Code 

