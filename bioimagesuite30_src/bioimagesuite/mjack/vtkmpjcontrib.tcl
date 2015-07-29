package provide vtkmpjcontrib 1.2

# 	$Id: vtkmpjcontrib.tcl,v 1.1 2005/12/01 16:42:26 xenios Exp xenios $	


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



lappend auto_path [ file dirname [ info script ]]
lappend auto_path [ file join [ file join [ file join  [ file dirname [ info script ]] .. ] extensions ] lib ]

package require vtkpxcontrib 1.2

::pxtclvtkpxcontrib::LoadLibraries { vtkmpjCommonTCL vtkmpjGraphicsTCL vtkmpjDiffusionTCL vtkmpjLevelsetTCL }
