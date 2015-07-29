#!/bin/sh
# the next line restarts using wish \
	exec vtk "$0" "$@"

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

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".." ] base]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".." ] apps]

package require Itcl 3.2
package require Iwidgets 
# This loads the basic BioImage Suite libraries
package require vtkpxcontrib  1.1


# This loads the database library
pxtclvtkpxcontrib::LoadLibraries { vtkbisDatabaseTCL }

itcl::class bis_parameterGUI {
	private common parameters
	protected variable basewidget 0
	
	constructor { parent } { CreateGUI $parent }
	destructor { }
	
	public method Reset { ask }
	public method SetColor { widget }
	public method GetName { mode }
	public method CreateGUI { base }
	
};

itcl::body bis_parameterGUI::Reset { ask } {
	if { $ask == 1 } {
		set ok [ tk_messageBox -type yesno -message "Reseting filenames" -icon question ]
		if { $ok == "no" } { return }
	}
	set parameters($this,readname) ""
	set parameters($this,writename) ""
	set parameters($this,directory) ""
}
itcl::body bis_parameterGUI::SetColor { widget } {
	set color [ tk_chooseColor -title "Set Background Color" -parent $basewidget ]
	if { [string length $color ] > 0 } { $widget configure -bg $color }
}

itcl::body bis_parameterGUI::GetName { mode } {
	set fname ""
	set filetype1 [ list "Text Files" [ list .txt .tex ] ]
	set filetype2 [ list "All Files" "*" ]
	switch -exact $mode {
		"readname" { set fname [tk_getOpenFile -title "Load" -filetypes [ list $filetype1 $filetype2 ] ] }
		"writename" { set fname [tk_getSaveFile -title "Filename" -filetypes [ list $filetype1 ] ] }
		"directory" { set fname [tk_chooseDirectory -title "Select Directory" ] }
	}
	if { [ string length $fname ] < 1 } { return }
	set parameters($this,$mode) $fname
}

itcl::body bis_parameterGUI::CreateGUI { base } {
	set .tnb $base
	
	# Page Add
	# ----------------------------------------------------------------------
	set page [.tnb add -label "Add"]
	
	iwidgets::entryfield $page.name -labeltext "Preset Name:" 
	pack $page.name
	iwidgets::entryfield $page.addr -labeltext "Description:" 
	pack $page.addr
	iwidgets::Labeledwidget::alignlabels $page.name $page.addr
	iwidgets::buttonbox $page.bb 
	$page.bb add OK -text "Add Current" -command "puts OK" 
	pack $page.bb -expand yes -fill both
	 
	# Page Query
	# ----------------------------------------------------------------------
	set page [.tnb add -label "Query"]
	
	iwidgets::entryfield $page.presetnameQ -labeltext "Preset Name:" 
	pack $page.presetnameQ
	iwidgets::entryfield $page.descrQ -labeltext "Description:" 
	pack $page.descrQ
	iwidgets::entryfield $page.date -labeltext "Date:" 
	pack $page.date
	iwidgets::entryfield $page.para -labeltext "Parameter:" 
	pack $page.para
	iwidgets::Labeledwidget::alignlabels $page.presetnameQ $page.descrQ $page.date $page.para
	
	iwidgets::buttonbox $page.btop 
	$page.btop add querybtn -text "Query" -command "puts OK" 
	$page.btop add showbtn -text "Show All" -command "puts All"
	pack $page.btop -fill both
	
	iwidgets::scrolledlistbox $page.slb -vscrollmode dynamic -selectmode single 
	pack $page.slb -expand yes -fill x
	$page.slb insert 0 Hello Here Me
	
	iwidgets::buttonbox $page.bb 
	$page.bb add btnview -text View -command "puts OK" 
	$page.bb add btnselect -text Select -command "puts Apply"
	$page.bb add btndelete -text Delete -command "puts Cancel"
	$page.bb default btnview
	
	pack $page.bb -expand yes -fill both
	
	#iwidgets::buttonbox $page.bbtom 
	#$page.bbtom add viewbtn -text "View" -command "puts OK" 
	#$page.bbtom add selectbtn -text "Select" -command "puts All"
	#$page.bbtom add delbtn -text "Delete" -command "puts All"
	#pack $page.bbtom -expand yes -fill both
	
	.tnb view "Add"
	.tnb configure -tabpos n
	
}

option add *textBackground seashell
option add *Tabnotebook.backdrop DimGray
option add *Scale.width 8
. configure -background white

iwidgets::tabnotebook .tnb -width 500 -height 450
pack .tnb -expand yes -fill both
set element [ bis_parameterGUI \#auto .tnb ]
