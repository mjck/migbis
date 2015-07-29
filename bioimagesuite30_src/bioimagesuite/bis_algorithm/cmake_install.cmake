# Install script for directory: /Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bis_algorithm" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/pkgIndex.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_common.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_util.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_object.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_option.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_optiondialog.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_optiondialog.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_algorithmdbaseinterface.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_basealgorithm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_algorithm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_imagetoimagealgorithm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_imagetosurfacealgorithm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_basethresholdimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_dualimagealgorithm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_dualimagetransformationalgorithm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_dualimagereslicetransformationalgorithm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_imageandsurfacetosurfacealgorithm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_surfacetosurfacealgorithm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_landmarkstolandmarksalgorithm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_baseintensityregistration.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_basepointbasedregistration.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_pointbasedregistrationviewer.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_spectbase.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_baselevelset.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_graphvis.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_guicontainer.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_guiimageutility.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_guisurfacecontrol.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_guisurfaceutility.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_guioverlayutility.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_guicompareutility.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_guirpmcontainer.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_guitreeimageutility.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_guitreecreate4d.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_datamanager.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_treewidget.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_viewer.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_polygonaldisplaymanager.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_xmlalgorithm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_electrodeeditor.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_landmarkeditor.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_imagelisttoimagealgorithm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_displayroimean.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_basecontrolhelper.tcl"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bis_algorithm" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_multisubjectfmri.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_computetensor.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_tensoranalysis.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_blendimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_castimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_csfwmmotioncorr.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_createoverlayimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_checkerboardimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_cleansurface.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_clipsurface.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_clusterthresholdimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_colorblendimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_combineframes.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_combineimages.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_combinetransformations.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_computedisplacementfield.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_computejointhistogram.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_computeglm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_computenormals.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_computeoverlap.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_computesimilarity.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_connect.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_convolveimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_create4dimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_cropimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_curvatures.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_cylindricalcropimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_decimate.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_delaunay3D.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_distancemapimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_skeletonizeimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_edgedetectimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_extractobjectmap.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_extractsurface.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_fdr.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_fftimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_flipimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_imagecompare.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_imagedilate.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_imagemathoperations.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_imagemedian.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_imageremoveframes.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_intsegment.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_linearintensityregister.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_linearpointregister.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_listthresholdimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_logimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_manualregistration.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_manualreslice.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_maskimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_matrixcorrelation.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_mediantemporalsmoothimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_nonlineardistortioncorrection.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_nonlinearintensityregister.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_nonlinearpointregister.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_nonlinearsmoothimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_piecewiseimagemap.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_preprocessimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_proportionalscale.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_qianvesselness.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_regularizeobjectmap.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_relabelimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_removeslicemean.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_removetemporaldrift.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_reorientimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_newreorientimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_resampleimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_resamplelandmarks.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_resliceimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_rfftimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_roimean.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_rtopvalue.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_rtotmap.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_seedcorrelation.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_shiftscaleimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_singlejacobian.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_singleimagemathoperations.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_sliceinhomogeneity.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_smoothimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_exponentialfitimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_exponentialfitt1.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_smoothlandmarks.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_smoothsurface.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_spectrf.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_stitchimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_stripskull.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_subdivide.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_temporalsmoothimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_thresholdimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_thresholdpoints.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_tmaptopvalue.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_tmaptozscore.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_transformsurface.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_triangulate.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_ttest.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_visualizetransformation.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_zscoretopvalue.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_ISAS.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_ISASHN.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_singlesubjectfmri.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_frangivesselness.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_computecorrelations.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_biasfield.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_levelset.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_interleave.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_reslicetensor.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_intrinsicconnectivity.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_fmritoolbox.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_testviewer.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_testmatrix.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_testdatabase.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_treeapp.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_headerinfo.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_makebatch.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_split4dimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_help.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_surfaceinfo.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_dicom2nifti.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/bis_algorithm/bis_varian2nifti.tcl"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/lib" TYPE FILE FILES "/Users/mjack/src3/bioimagesuite30_src/bisalgorithmscriptlist.txt")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_multisubjectfmri")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_computetensor")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_tensoranalysis")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_blendimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_castimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_csfwmmotioncorr")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_createoverlayimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_checkerboardimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_cleansurface")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_clipsurface")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_clusterthresholdimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_colorblendimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_combineframes")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_combineimages")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_combinetransformations")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_computedisplacementfield")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_computejointhistogram")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_computeglm")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_computenormals")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_computeoverlap")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_computesimilarity")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_connect")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_convolveimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_create4dimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_cropimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_curvatures")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_cylindricalcropimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_decimate")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_delaunay3D")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_distancemapimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_skeletonizeimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_edgedetectimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_extractobjectmap")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_extractsurface")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_fdr")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_fftimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_flipimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_imagecompare")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_imagedilate")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_imagemathoperations")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_imagemedian")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_imageremoveframes")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_intsegment")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_linearintensityregister")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_linearpointregister")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_listthresholdimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_logimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_manualregistration")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_manualreslice")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_maskimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_matrixcorrelation")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_mediantemporalsmoothimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_nonlineardistortioncorrection")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_nonlinearintensityregister")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_nonlinearpointregister")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_nonlinearsmoothimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_piecewiseimagemap")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_preprocessimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_proportionalscale")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_qianvesselness")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_regularizeobjectmap")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_relabelimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_removeslicemean")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_removetemporaldrift")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_reorientimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_newreorientimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_resampleimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_resamplelandmarks")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_resliceimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_rfftimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_roimean")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_rtopvalue")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_rtotmap")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_seedcorrelation")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_shiftscaleimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_singlejacobian")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_singleimagemathoperations")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_sliceinhomogeneity")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_smoothimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_exponentialfitimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_exponentialfitt1")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_smoothlandmarks")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_smoothsurface")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_spectrf")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_stitchimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_stripskull")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_subdivide")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_temporalsmoothimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_thresholdimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_thresholdpoints")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_tmaptopvalue")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_tmaptozscore")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_transformsurface")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_triangulate")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_ttest")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_visualizetransformation")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_zscoretopvalue")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_ISAS")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_ISASHN")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_singlesubjectfmri")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_frangivesselness")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_computecorrelations")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_biasfield")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_levelset")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_interleave")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_reslicetensor")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_intrinsicconnectivity")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_fmritoolbox")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_testviewer")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_testmatrix")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_testdatabase")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_treeapp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_headerinfo")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_makebatch")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_split4dimage")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_help")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_surfaceinfo")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_dicom2nifti")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/bin" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/mjack/src3/bioimagesuite30_src/bin/bis_varian2nifti")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30" TYPE DIRECTORY FILES "/Users/mjack/src3/bioimagesuite30_src/html")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

