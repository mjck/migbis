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
SET(KITBASE GUI)
SET(KIT vtkpx${KITBASE})

INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/${KITBASE} /opt/X11/include)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Common)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/pxtklib)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Graphics)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Legacy)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Registration)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Imaging)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Nmrlib)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/mpjGraphics)



SET (KITSRCS
# ------------------------------------------------------------
# Base GUI Stuff.cpp
# ------------------------
vtkpxGUIComponent.cpp
vtkbisGUINIFTIHeaderEditor.cpp
# -----------------------
# Gui Stuff -- 1 Renderers
# -----------------------
vtkpxGUIBaseRenderer.cpp
vtkpxGUIRenderer.cpp
vtkpxGUI4DRenderer.cpp
# ------------------------
# Gui Stuff 2 -- viewers
# ------------------------
vtkpxGUIBaseImageViewer.cpp
vtkpxGUIBaseCurveEditor.cpp
vtkpxGUI2DImageViewer.cpp
vtkpxGUIMosaicViewer.cpp
vtkpxGUIObjectmapMosaicViewer.cpp
vtkpxGUIMultipleMosaicViewer.cpp
vtkpxGUICurveImageEditor.cpp
vtkpxGUIContourStackEditor.cpp
vtkpxGUIOrthogonalViewer.cpp
vtkpxGUIObjectmapOrthogonalViewer.cpp
vtkpxGUIOrthogonal4DViewer.cpp
vtkpxGUI2DSplineObjectmapViewer.cpp
vtkpxGUISplineEditor.cpp
# ----------------------------
# Objectmap Editor Attachments
# ----------------------------
vtkpxGUIAbstractMultisliceEditor.cpp
vtkpxGUIObjectmapMosaicEditor.cpp
vtkpxGUIObjectmapOrthogonalEditor.cpp
vtkpxGUI2DImageEditor.cpp
# ---------------------------------
# GYU Stuff 3 - auxiliary controls
# ---------------------------------
vtkpxGUIAnalyzeHeaderEditor.cpp
vtkpxGUIColormapEditor.cpp
vtkpxGUIBaseCurveControl.cpp
vtkpxGUIPolyDataControl.cpp
vtkpxGUIObjectmapSplineControl.cpp
vtkpxGUISplineControl.cpp
vtkpxGUISplineStackControl.cpp
vtkpxGUIObjectmapSplineStackControl.cpp
vtkpxGUI4DSplineStackControl.cpp
vtkpxGUITriangulatedStackControl.cpp
vtkpxGUI4DTriangulatedStackControl.cpp
vtkpxGUIFemPostControl.cpp
#vtkpxGUITransformControl.cpp
vtkpxGUIVolumeControl.cpp
vtkpxGUIObliqueImageSliceControl.cpp
#
pxvtkgadgets.cpp
pxvtktkdialog.cpp
pxtkanaheadercontrol.cpp
vtkpxGUICallback.cpp
vtkpxTalairachTransform.cpp
vtkpxTalairachTransformHelper.cpp
#
vtkmpjX3DExporter.cpp
)

#IF (BIOIMAGESUITE3_EXPERIMENTAL)
#SET (KITSRCS
#    ${KITSRCS}
#    vtkbisFMRITools.cpp
#}

SET (KITTCLSRCS 
  vtkpxtkGUIlib.cpp
  )

SET (KIIPYTHONSRCS
#  vtkpxPythonGUIlib.cpp
)

SET_SOURCE_FILES_PROPERTIES(
 vtkpxGUIBaseImageViewer.cpp
 vtkpxGUIBaseCurveEditor.cpp
 vtkpxGUIBaseRenderer.cpp
 vtkpxGUIAbstractMultisliceEditor.cpp
ABSTRACT
)

SET_SOURCE_FILES_PROPERTIES(
 pxtkanaheadercontrol.cpp
 pxvtkgadgets.cpp
 pxvtktkdialog.cpp
 vtkpxGUICallback.cpp
 vtkpxTalairachTransformHelper.cpp
 WRAP_EXCLUDE
)

SET(KITLIBS
  numerics
  vtkpxRegistration
  vtkpxRegistrationTCL
  vtkpxLegacy
  vtkpxLegacyTCL
  vtkpxGraphics
  vtkpxGraphicsTCL
  vtkpxImaging
  vtkpxCommon
  vtkpxCommonTCL
  vtkCommonTCL
  ${BIOIMAGESUITE3_COMMON_LIBS}
  vtkmpjGraphics
  vtkmpjGraphicsTCL
  pxtklib
)

	


IF (CYGWIN)
SET (KITLIBS
    ${KITLIBS}
    -ltk84
    )
ENDIF(CYGWIN)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line
bis_complex("GUI" ${KIT} ${FILELIST})




