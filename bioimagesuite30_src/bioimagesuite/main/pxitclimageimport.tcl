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

package provide pxitclimageimport 1.0

# 	$Id: pxitclimageimport.tcl,v 1.4 2003/08/20 20:46:37 xenios Exp xenios $	





package require  pxitclbaseimagecontrol 1.0

package require  pxtclreadsprheader 1.0
package require  pxtcluserprefs 1.0
package require  Iwidgets 4.0

itcl::class  pxitclimageimport {

    inherit pxitclbaseimagecontrol
    
    private common thisparam

    public variable need_load 0 
    variable selection_box 0
    
    private variable dimframe   0
    private variable otherframe 0
    private variable multiframe 0
    private variable byteframe  0

    private common filetypes [ list "DICOM" "Analyze"  "Signa LX" "Prism (SPECT)" "Binary"  "Signa Spr"  "TIFF" "PNM" "BMP" "JPEG" ] 
    private common masks [ list  "*" "*.hdr*" "e*s*i*" "*.psm*" "*" "*.spr"  "*.tif*" "*.p?m" "*.bmp" "*.jp*" ]

    private common orientations [ list "Axial" "Coronal" "Sagittal" ]
    private common flips  [ list "None" "Slice Order(Z)" "X" "Y" "YZ" "Transpose" "FlipTranspose"]
    private common mr_modes [ list "Conventional" "Conventional (Append)" "EchoPlanar" ]
    
    constructor { par } {
	pxitclbaseimagecontrol::constructor $par
    } {	
	InitializeImageImport 

    }
    

    # ---------------------------------------------------------------------------------
    #   Create Actual GUI
    # ---------------------------------------------------------------------------------


    private   method InitializeImageImport { }
    public    method Initialize { inpwidg } 
    protected method CreateFilenameControl { base }
    protected method CreatePropertiesControl { base }
    protected proc   ProcessSignaFilename { f } 
    public    method ReadHeader { } 
    public    method ImportImage { } 
    public    method UpdateParent { }
    public    method UpdateFileName { } 
    
    public    method UpdateFileType { } 
    public    method GetFileType    { }
    public    method SetFileType    { ftype }

    public method SetCurrentDirectory { fname} 


    public method ShowWindow { args }
    public method Debug { } { puts stdout $thisparam($this,import_numbytes) }


    public method ImportTIFF     { fname minslice maxslice comment }
    public method ImportJPEG     { fname minslice maxslice comment }
    public method ImportPNM      { fname minslice maxslice comment }
    public method ImportBMP      { fname minslice maxslice comment }
    public method ImportSignaSpr { fname minslice maxslice comment }
    public method ImportSignaLX  { fname minslice maxslice comment }
    public method ImportBinary   { fname minslice maxslice comment }
    public method ImportAnalyze  { fname minslice maxslice comment }
    public method ImportNmr47    { fname minslice maxslice comment }
    public method ImportRawMR    { fname minslice maxslice comment }
    public method ImportPrism    { fname minslice maxslice comment }
    public method ImportDICOM    { fname minslice maxslice comment }
}
# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::InitializeImageImport { } {

    global pxtcl_pref_array
    
    #  ---------------------------------------
    #  GUI Related Parameters
    #  ---------------------------------------
    set thisparam($this,import_bottomslice) -1 ; set thisparam($this,import_topslice) -1 ; 
    set thisparam($this,import_orient) "Axial"
    set thisparam($this,import_xsp) 1.5;      set thisparam($this,import_ysp) 1.5;     
    set thisparam($this,import_zsp) 1.5;
    set thisparam($this,import_width) 128 ;   set thisparam($this,import_height) 128 ;  
    set thisparam($this,import_numbytes) 4;
    set thisparam($this,import_skip) 0; set thisparam($this,import_flip) "None"
    set thisparam($this,import_filename) "" ; set thisparam($this,image_type) "Signa LX"
    set thisparam($this,num_frames) 1;
    set thisparam($this,num_slices) 1;
    set thisparam($this,import_swapbytes) 0
    set thisparam($this,image_type)       $pxtcl_pref_array(DefaultImportMode)
    set thisparam($this,import_flip) "None"
    set thisparam($this,import_mrmode) "Conventional"
}

# ---------------------------------------------------------------------------------
#
# Create Import Control
#
# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::Initialize { widg } {
	
    if { $initialized == 1 } { return $basewidget }

    set basewidget $widg
    toplevel $basewidget ; wm withdraw $basewidget
    wm geometry $basewidget 450x420
    wm title $basewidget "Import Image Control"

    set notebook $basewidget.notebook
    iwidgets::tabnotebook $notebook -width 5i -height 3i -tabpos n
    
    CreateFilenameControl     [ $notebook add -label "Filename"  ]

    set bottom [ $notebook add -label "Properties"  ]

    set w [ frame $bottom.bottom -relief ridge -width 800]
    pack $w -side bottom -fill x -expand false
    pack $notebook -side top -fill both -expand true

    eval "button $w.cp2 -text \"Load + Update\"  -command {  $this UpdateParent }"
    eval "button $w.close   -text Close -command { $this HideWindow }"
    pack $w.cp2 $w.close -side left -fill x -padx 15 -pady 10

    
    CreatePropertiesControl  $bottom
    $notebook view "Filename"

    set initialized 1
    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"


    set canvas [ [ $notebook component tabset ] component canvas ]
    set l [ AddLogoLabel $canvas ]
    pack $l -side right -padx 1 


    return $basewidget

}

# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::CreateFilenameControl {  base } {
	
    
    label $base.o -bg black -fg white -text "Select Filename and Image Type"
    pack  $base.o   -side top -expand f -fill x  -pady 2
    
    set selection_box $base.b
    set filetype_box  $base.c

    iwidgets::fileselectionbox $selection_box -width 100i -height 100i
    set w [ [ iwidgets::labeledwidget  $filetype_box -labeltext "Image File Type:"] childsite ]

    pack $filetype_box -side bottom -fill x -expand  f
    pack $selection_box -side top -fill both -expand true
    eval "$selection_box  configure -selectfilecommand { $this UpdateFileName }"
   
    set optmenu [ tk_optionMenu $w.m [ itcl::scope thisparam($this,image_type) ] [lindex $filetypes 0 ] ]
    for { set k 1 } { $k < [ llength $filetypes ] } { incr k } {
    	$optmenu add radio -variable [ itcl::scope thisparam($this,image_type) ]  -label [ lindex $filetypes $k ]
    }
    
    for { set k 0 } { $k < [ llength $filetypes ] } { incr k } {
    	eval "$optmenu entryconfigure $k -command { $this UpdateFileType }"
    }

    pack  $w.m -side left -padx 10
}

# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::SetCurrentDirectory { fname}  {

    $selection_box configure -directory $fname
}

# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::UpdateFileName { } {

    set fname [ $selection_box get ]
    if { [ string length $fname ] > 1 } {
	set thisparam($this,currentfilename) $fname
	ReadHeader  
	set need_load 1
    }
}

itcl::body pxitclimageimport::UpdateFileType { } {

    set ind [ lsearch -exact $filetypes $thisparam($this,image_type) ]
    set f2  [ lindex $masks $ind ]  
    $selection_box configure -mask $f2
    $selection_box filter
    update idletasks

    set ft $thisparam($this,image_type) 


    if { $ft == "Analyze" || $ft == "Nmr47"  || $ft == "Prism (SPECT)" ||  $ft == "DICOM" ||  $ft == "Varian" } {
	DisableUI [ list $byteframe $multiframe $dimframe ]
    } elseif { $ft == "Signa Spr" } {
	DisableUI [ list $multiframe $dimframe ]
	EnableUI $byteframe
    } elseif { $ft == "Signa LX" } {
	DisableUI [ list $dimframe $byteframe ]
	EnableWidget $multiframe
    } elseif { $ft == "Binary" || $ft == "Raw MR" } {
	EnableUI [ list $dimframe $byteframe ]
	DisableWidget $multiframe
    } else {
	EnableUI [ list $byteframe $multiframe $dimframe ]
    }
    	
}

# ---------------------------------------------------------------------------------

::itcl::body pxitclimageimport::GetFileType {  } { return $thisparam($this,image_type)  }
# ---------------------------------------------------------------------------------
::itcl::body pxitclimageimport::SetFileType { ft } {

    set ind [ lsearch -exact $filetypes $ft ] 
    if { $ind >=0 } {
	set thisparam($this,image_type) $ft
    }

}
# ---------------------------------------------------------------------------------
::itcl::body pxitclimageimport::ShowWindow { args } {

    UpdateFileType
    return [ pxitclbasecontrol::ShowWindow "Filename" ]
}
# ---------------------------------------------------------------------------------

itcl::body pxitclimageimport::CreatePropertiesControl { base } { 
	
    label $base.o -bg black -fg white -text "Image Header Info" -width 50 
    frame $base.mid ; 

    set dimframe   [ frame $base.mid.bar0 ]
    set otherframe [ frame $base.mid.bar1 ]

    
    pack  $base.o -side top -expand f -fill x  -pady 2
    pack  $base.mid -side top -expand t -fill both -pady 2 -padx 2
    pack  $dimframe   -side left -expand f -fill y   -pady 2 -padx 2
    pack  $otherframe  -side left -expand true -fill both  -pady 2 -padx 2


    label $dimframe.0a -text "Image Dimensions"
    iwidgets::entryfield $dimframe.0 -labeltext "Width:" -fixed 10 -width 6 -validate numeric -textvariable [ itcl::scope  thisparam($this,import_width) ]
    iwidgets::entryfield $dimframe.1 -labeltext "Height:" -fixed 10 -width 6 -validate numeric -textvariable [ itcl::scope  thisparam($this,import_height) ]
    iwidgets::entryfield $dimframe.2 -labeltext "Depth:" -fixed 10 -width 6 -validate numeric -textvariable [ itcl::scope  thisparam($this,num_slices) ]
    iwidgets::entryfield $dimframe.3 -labeltext "Frames:" -fixed 10 -width 6 -validate numeric -textvariable [ itcl::scope  thisparam($this,num_frames) ]
    pack $dimframe.0a  $dimframe.0 $dimframe.1 $dimframe.2 $dimframe.3   -side top -fill x -expand t


    label $dimframe.4 -text "Voxel Dimensions"
    iwidgets::entryfield $dimframe.a0 -labeltext "x (mm):" -fixed 10 -width 6 -validate real -textvariable [ itcl::scope  thisparam($this,import_xsp) ]
    iwidgets::entryfield $dimframe.a1 -labeltext "y (mm):" -fixed 10 -width 6 -validate real -textvariable [ itcl::scope  thisparam($this,import_ysp) ]
    iwidgets::entryfield $dimframe.a2 -labeltext "z (mm):" -fixed 10 -width 6 -validate real -textvariable [ itcl::scope  thisparam($this,import_zsp) ]
    pack  $dimframe.4 $dimframe.a0 $dimframe.a1 $dimframe.a2  -side top -fill x -expand t


    # Series Information
    set multiframe  [ iwidgets::Labeledframe $otherframe.top -labelpos n -labeltext "File Series"].childsite
    pack $otherframe.top -side top -fill x -expand f
    iwidgets::entryfield $multiframe.0 -labeltext "Begin File:" -fixed 10 -width 6 -validate numeric -textvariable [ itcl::scope  thisparam($this,import_bottomslice) ]
    iwidgets::entryfield $multiframe.1 -labeltext "End File:"   -fixed 10 -width 6 -validate numeric -textvariable [ itcl::scope  thisparam($this,import_topslice) ]
    pack  $multiframe.0 $multiframe.1 -side top -fill x -expand f 
    
    
    set byteframe  [ iwidgets::Labeledframe $otherframe.mid -labelpos n -labeltext "File Info"].childsite
    pack $otherframe.mid -side top -fill x -expand f

    iwidgets::entryfield $byteframe.1 -labeltext "Header Offset:" -fixed 10 -width 6 -validate numeric -textvariable [ itcl::scope  thisparam($this,import_skip) ]
    set w [ [ iwidgets::labeledwidget  $byteframe.2 -labeltext "Bytes/Voxel:"] childsite ]
    tk_optionMenu $w.m [ itcl::scope thisparam($this,import_numbytes) ] "1" "2" "4"
    pack $w.m -side left
    set w [ [ iwidgets::labeledwidget  $byteframe.3 -labeltext "Swap Voxels:"] childsite ]
    checkbutton $w.3 -variable [ itcl::scope  thisparam($this,import_swapbytes) ] -text ""
    pack $w.3 -side left

    set w [ [ iwidgets::labeledwidget  $byteframe.4 -labeltext "MR Mode:"] childsite ]
    tk_optionMenu $w.m [ itcl::scope thisparam($this,import_mrmode)]  [ lindex $mr_modes 0 ] 
    pack $w.m -side left
    for { set k 0 } { $k < [ llength $mr_modes ] } { incr k } {
	if { $k > 0 } {
	    $w.m.menu add radio -variable [ itcl::scope thisparam($this,import_mrmode)] -label [ lindex $mr_modes $k ]
	}
	eval "$w.m.menu entryconfigure $k -command { $this configure -need_load 1 }"
    }


    pack $byteframe.1 $byteframe.2 $byteframe.3  $byteframe.4 -side top -fill x -expand f
    
    set mframe  [ iwidgets::Labeledframe $otherframe.bot -labelpos n -labeltext "Orientation"].childsite
    pack $otherframe.bot -side top -fill x -expand f
    set w [ [ iwidgets::labeledwidget  $mframe.1 -labeltext "Orientation:"] childsite ]
    tk_optionMenu $w.m [ itcl::scope  thisparam($this,import_orient) ] "Axial" "Coronal" "Sagittal" 
    for { set k 0 } { $k < 3 } { incr k } {
	eval "$w.m.menu entryconfigure $k -command { $this configure -need_load 1 }"
    }
    pack $w.m -side left

    set w [ [ iwidgets::labeledwidget  $mframe.2 -labeltext "Flip:"] childsite ]
    tk_optionMenu $w.m [ itcl::scope thisparam($this,import_flip)]  [ lindex $flips 0 ] 
    pack $w.m -side left
    for { set k 0 } { $k < [ llength $flips ] } { incr k } {
	if { $k > 0 } {
	    $w.m.menu add radio -variable [ itcl::scope thisparam($this,import_flip)] -label [ lindex $flips $k ]
	}
	eval "$w.m.menu entryconfigure $k -command { $this configure -need_load 1 }"
    }


    
    pack  $mframe.1 $mframe.2  -side top -fill x -expand f

    

    
}

# -------------------------------------------------------------------------------------------
#  
#    Image Load Stuff
#
# -------------------------------------------------------------------------------------------

# ---------------------------------------------------------------------------------
#   Signa LX Stuff
# ---------------------------------------------------------------------------------

itcl::body pxitclimageimport::ProcessSignaFilename {  f } {
	
    set pa(name) f
    set pa(0) 1
    set pa(1) 1
    
    set first [ string last "i" $f ] 
    set pa(name) [ string range $f 0 $first ]
    
    set flag 0
    
    while { $flag == 0 } {
	set tmpname "$pa(name)$pa(1)"
	set tmpname2 "$pa(name)$pa(1).gz"
	if { [ file exists $tmpname ] ==1 || [ file exists $tmpname2 ]==1 } {
	    incr pa(1)
	} else {
	    set flag 1 
	    set pa(1) [ expr $pa(1) -1 ]
	}
    }
    
    set pa_list [ array get  pa ]
    return $pa_list
}

# ---------------------------------------------------------------------------------
#   Read Header
# ---------------------------------------------------------------------------------

itcl::body pxitclimageimport::ReadHeader { }  {

    global tcl_platform
    
    set orient 0
    set thisparam($this,import_bottomslice) -1 ; set thisparam($this,import_topslice) -1 ; set thisparam($this,import_orient) "Axial"
    set thisparam($this,import_xsp) 1.5;      set thisparam($this,import_ysp) 1.5;      set thisparam($this,import_zsp) 1.5;
    set thisparam($this,import_width) 128 ;   set thisparam($this,import_height) 128 ;  set thisparam($this,import_swapbytes) 0 ; set thisparam($this,import_numbytes) 2;
    set thisparam($this,import_skip) 0
    set thisparam($this,import_flip) "None"
    
    switch -exact $thisparam($this,image_type) {
	"Analyze" {
	    set ana_2 [  vtkpxAnalyzeImageSourceOld [ pxvtable::vnewobj ] ]
	    $ana_2 SetFilePrefix $thisparam($this,currentfilename)
	    
	    set bits [ $ana_2 ReadHeaderFile ]
	    if { $bits != 0 } {
			set thisparam($this,import_topslice)    -1
			set thisparam($this,import_bottomslice) -1
			set thisparam($this,import_xsp)         [ lindex [$ana_2 GetDataSpacing ] 0 ]
			set thisparam($this,import_ysp)         [ lindex [$ana_2 GetDataSpacing ] 1 ]
			set thisparam($this,import_zsp)         [ lindex [$ana_2 GetDataSpacing ] 2 ]
			set thisparam($this,import_width)       [ lindex [$ana_2 GetDataDimensions ] 0 ]
			set thisparam($this,import_height)       [ lindex [$ana_2 GetDataDimensions ] 1 ]
			set thisparam($this,num_frames) [ $ana_2 GetNumTimeFrames ]
			set thisparam($this,num_slices) [ $ana_2 GetNumSlices ]
			set orient [ $ana_2 GetOrientation ] 
			if { $bits < 0 } {
				set thisparam($this,import_swapbytes) 1 
				set bits [ expr -1.0 *bits ]
			} else {
				set thisparam($this,import_swapbytes) 0
			}
		set thisparam($this,import_numbytes) [ expr $bits /8 ]
	    }
	    catch {$ana_2 Delete }
	}
	"Varian" {
		set varian [ vtkbisVarianSource [ pxvtable::vnewobj ] ] 
		$varian SetFilePrefix $thisparam($this,currentfilename)
		set varianreader [ $varian ReadHeaderFile ] 
		if { $varianreader !=0 } {
			set thisparam($this,import_topslice)    -1
			set thisparam($this,import_bottomslice) -1
			set thisparam($this,import_xsp)         [ lindex [$varian GetDataSpacing ] 0 ]
			set thisparam($this,import_ysp)         [ lindex [$varian GetDataSpacing ] 1 ]
			set thisparam($this,import_zsp)         [ lindex [$varian GetDataSpacing ] 2 ]
			set thisparam($this,import_width)       [ lindex [$varian GetDataDimensions ] 0 ]
			set thisparam($this,import_height)       [ lindex [$varian GetDataDimensions ] 1 ]
			set thisparam($this,num_frames) [ $varian GetNumTimeFrames ]
			set thisparam($this,num_slices) [ $varian GetNumSlices ]
			set orient [ $varian GetOrientation ] 
			if { $varianreader < 0 } {
			    set thisparam($this,import_swapbytes) 1 
			    set varianreader [ expr -1.0 *varianreader ]
			} else {
			    set thisparam($this,import_swapbytes) 0
			}
		set thisparam($this,import_numbytes) [ expr $varianreader /8 ]
		}	    
	    	catch {$varian Delete }					
	}
	"Prism (SPECT)" {
	    set ana_2 [  vtkpxPickerPrismSource [ pxvtable::vnewobj ] ]
	    $ana_2 SetFilePrefix $thisparam($this,currentfilename)
	    
	    set bits [ $ana_2 ReadHeaderFile ]
	    if { $bits != 0 } {
		set thisparam($this,import_topslice)    -1
		set thisparam($this,import_bottomslice) -1
		set thisparam($this,import_xsp)         [ lindex [$ana_2 GetSpacing ] 0 ]
		set thisparam($this,import_ysp)         [ lindex [$ana_2 GetSpacing ] 1 ]
		set thisparam($this,import_zsp)         [ lindex [$ana_2 GetSpacing ] 2 ]
		set thisparam($this,import_width)       [ lindex [$ana_2 GetDimensions ] 0 ]
		set thisparam($this,import_height)       [ lindex [$ana_2 GetDimensions ] 1 ]
		set thisparam($this,num_frames) [ $ana_2 GetNumTimeFrames ]
		set thisparam($this,num_slices) [ $ana_2 GetNumSlices ]
		set orient [ $ana_2 GetOrientation ] 
		if { $bits < 0 } {
		    set thisparam($this,import_swapbytes) 1 
		    set bits [ expr -1.0 *bits ]
		} else {
		    set thisparam($this,import_swapbytes) 0
		}
		set thisparam($this,import_numbytes) [ expr $bits /8 ]
	    }
	    catch {$ana_2 Delete }
	    set thisparam($this,import_flip) "Slice Order(Z)"
	}
	"Signa Spr" {
	    set spr  [ pxvtable::vnewobj ]
	    
	    if { [ ::pxtclreadsprheader::ReadHeader  $spr $thisparam($this,currentfilename) ] > 0 } {
		
		set thisparam($this,num_frames) [ pxvtable::getvariable $spr numframes ]
		set thisparam($this,num_slices) [ pxvtable::getvariable $spr numslices ]
		set thisparam($this,import_topslice)    -1
		
		set thisparam($this,import_bottomslice) -1
		set thisparam($this,import_xsp)         [ pxvtable::getvariable $spr spacingx]
		set thisparam($this,import_ysp)         [ pxvtable::getvariable $spr spacingy]
		set thisparam($this,import_zsp)         [ pxvtable::getvariable $spr spacingz]
		set thisparam($this,import_width)       [ pxvtable::getvariable $spr width]
		set thisparam($this,import_height)      [ pxvtable::getvariable $spr height]
		set thisparam($this,import_swapbytes) 1
		if { [ pxvtable::getvariable $spr orient] == "COR" } { 
		    set orient 1 
		} 
	    }
	}
	
	"Nmr47" {
	    set header [ pxvtable::vnewobj ]
	    if { [ ::pxtclbrukerheader::ReadHeader $header $thisparam($this,currentfilename) ] > 0 } {
		set thisparam($this,num_frames) [ pxvtable::getvariable $header dim3 ]
		set thisparam($this,num_slices) [ pxvtable::getvariable $header dim2 ]
		set thisparam($this,import_topslice)    -1
		
		set thisparam($this,import_bottomslice) -1
		set thisparam($this,import_xsp)         [ pxvtable::getvariable $header sp0]
		set thisparam($this,import_ysp)         [ pxvtable::getvariable $header sp1]
		set thisparam($this,import_zsp)         [ pxvtable::getvariable $header sp2]
		set thisparam($this,import_width)       [ pxvtable::getvariable $header dim0]
		set thisparam($this,import_height)      [ pxvtable::getvariable $header dim1]
		set orient 0
	    } 
	    unset header
	}
	
	"DICOM" {
	    set thisparam($this,import_flip) "YZ"
	}
	
	
	"Signa LX" {
	    set lx_reader_2 [ vtkpxSignaLXImageSource [ pxvtable::vnewobj ]  ]
	    set f $thisparam($this,currentfilename)
	    $lx_reader_2 SetFilePrefix [ string range $f 0 [ string last "i" $f ]]
	    if { [ $lx_reader_2 ReadHeaderFile ] !=0 } {
		set thisparam($this,import_xsp)         [ lindex [$lx_reader_2 GetDataSpacing ] 0 ]
		set thisparam($this,import_ysp)         [ lindex [$lx_reader_2 GetDataSpacing ] 1 ]
		set thisparam($this,import_zsp)         [ lindex [$lx_reader_2 GetDataSpacing ] 2 ]
		set thisparam($this,import_width)       [ lindex [$lx_reader_2 GetDataDimensions ] 0 ]
		set thisparam($this,import_height)      [ lindex [$lx_reader_2 GetDataDimensions ] 1 ]
		set pa_list [ pxitclimageimport::ProcessSignaFilename $f ] 
		array set pa $pa_list
		set thisparam($this,import_topslice)    $pa(1)
		set thisparam($this,import_bottomslice) $pa(0)
		set thisparam($this,num_slices) [ expr $pa(1) - $pa(0) + 1 ]
		set thisparam($this,num_frames) 1
		set orient [ $lx_reader_2 GetOrientation ] 
	    }
	    catch { $lx_reader_2 Delete }
	}

	"Raw MR" {
	    set thisparam($this,num_frames) 1
	    set thisparam($this,import_numbytes) 4
	    if { $tcl_platform(byteOrder) == "littleEndian" } { 
		set thisparam($this,import_swapbytes) 1
	    } else {
		set thisparam($this,import_swapbytes) 0
	    }

	    set sz [ file size $thisparam($this,currentfilename) ]
	    set thisparam($this,num_slices) 0
	    set sq 256
	    while { $thisparam($this,num_slices) < 1 && $sq > 8 } {
		set thisparam($this,import_width)  $sq 
		set thisparam($this,import_height) $sq
		set thisparam($this,num_slices) [ expr $sz / ($sq*$sq*8) ]
		set sq [ expr $sq /2 ]
	    }
	}
    }
    
    if { $orient < 0 } { set orient 0 } 
    if { $orient > 2 } { set orient 2 }
    set thisparam($this,import_orient) [ lindex $orientations $orient ]
    $notebook view "Properties"
}

# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::ImportTIFF { fname minslice maxslice comment } {

    set tif_reader [ vtkTIFFReader [ pxvtable::vnewobj ]  ]
    
    if { $minslice == - 1 || $maxslice == - 1 } {
	$tif_reader SetFileName $fname
    } else {
	set suffix ".tiff"
	if { [ string first ".tif" $fname ] > 0 } {
	    set suffix ".tif" 
	}

	set first [ string first "_" $fname ] 
	if { $first > 0 } {
	    set prefix [ string range $fname 0 [expr $first - 1] ]
	    $tif_reader SetFilePattern "%s_%04d$suffix"
	    $tif_reader SetFilePrefix $prefix
	} else  {
	    set first [ string last "." $fname ] 
	    set prefix [ string range $fname 0 [expr $first - 1] ]
	    $tif_reader SetFilePattern "%s.%d"
	    $tif_reader SetFilePrefix $prefix
	}

	#	puts stderr "Prefix=$prefix, slices=$minslice:$maxslice"
	
	$tif_reader SetDataExtent 0 511 0 511 $minslice $maxslice
    }
    
    set thisparam($this,num_frames) 1
    set thisparam($this,num_slices) [ expr $maxslice- $minslice + 1]
    SetFilterCallbacks $tif_reader $comment
    $tif_reader Update

    #    set dm [ [ $tif_reader GetOutput ] GetDimensions ]
    #    puts stdout "dm=$dm"

    set inf [ vtkImageChangeInformation New ]
    $inf SetInput [ $tif_reader GetOutput ]
    $inf SetOutputExtentStart 0 0 0
    $inf Update

    $currentresults ShallowCopyImage [$inf GetOutput]
    $tif_reader Delete
    $inf Delete
    return 1
}
# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::ImportPNM  { fname minslice maxslice comment } {

    set pnm_reader [   vtkPNMReader [ pxvtable::vnewobj ] ]
    if { $minslice == - 1 || $maxslice == - 1 } {
	$pnm_reader SetFileName $fname
    } else {
	set suffix ".pnm"
	if { [ string first ".pbm" $fname ] > 0 } {
	    set suffix ".pbm" 
	} else {
	    if { [ string first ".ppm" $fname ] > 0 } {
		set suffix ".pbm" 
	    } else {
		if { [ string first ".pgm" $fname ] > 0 } {
		    set suffix ".pgm" 
		}
	    }
	}
	
	set first [ string first "_" $fname ] 
	if { $first > 0 } {
	    set prefix [ string range $fname 0 [expr $first - 1] ]
	    $pnm_reader SetFilePattern "%s_%04d$suffix"
	    $pnm_reader SetFilePrefix $prefix
	} else  {
	    set first [ string last "." $fname ] 
	    set prefix [ string range $fname 0 [expr $first - 1] ]
	    $pnm_reader SetFilePattern "%s.%d"
	    $pnm_reader SetFilePrefix $prefix
	}
	$pnm_reader SetDataExtent 0 511 0 511 $minslice $maxslice
    }
    SetFilterCallbacks $pnm_reader $comment
    $pnm_reader Update
    set thisparam($this,num_frames) 1
    set thisparam($this,num_slices) [ expr $maxslice- $minslice + 1]

    set inf [ vtkImageChangeInformation New ]
    $inf SetInput [ $png_reader GetOutput ]
    $inf SetOutputExtentStart 0 0 0
    $inf Update

    
    $currentresults ShallowCopyImage [$inf_reader GetOutput]
    $inf Delete
    $pnm_reader Delete
    return 1
}
# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::ImportBMP  { fname minslice maxslice comment } {

    set bmp_reader [  vtkBMPReader [ pxvtable::vnewobj ] ]
    if { $minslice == - 1 || $maxslice == - 1 } {
	$bmp_reader SetFileName $fname
    } else {
	set suffix ".BMP"
	if { [ string first ".bmp" $fname ] > 0 } {
	    set suffix ".bmp"
	}
	set first [ string first "_" $fname ] 
	if { $first > 0 } {
	    set prefix [ string range $fname 0 [expr $first - 1] ]
	    $bmp_reader SetFilePattern "%s_%04d$suffix"
	    $bmp_reader SetFilePrefix $prefix
	} else {
	    set first [ string last "." $fname ] 
	    set prefix [ string range $fname 0 [expr $first - 1] ]
	    $bmp_reader SetFilePattern "%s.%d"
	    $bmp_reader SetFilePrefix $prefix
	}
	$bmp_reader SetDataExtent 0 511 0 511 $minslice $maxslice
    }
    $bmp_reader Update
    set thisparam($this,num_frames) 1
    set thisparam($this,num_slices) [ expr $maxslice- $minslice + 1]
    
    SetFilterCallbacks $bmp_reader $comment
    $currentresults ShallowCopyImage [$bmp_reader GetOutput]
    $bmp_reader Delete
    return 1
}
# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::ImportJPEG { fname minslice maxslice comment } {

    set jpg_reader [ vtkJPEGReader [ pxvtable::vnewobj ]  ]
    
    if { $minslice == - 1 || $maxslice == - 1 } {
	$jpg_reader SetFileName $fname
    } else {
	set suffix ".jpeg"
	if { [ string first ".jpg" $fname ] > 0 } {
	    set suffix ".jpg" 
	}
	
	set first [ string last "_" $fname ] 

	if { $first > 0 } {
	    set prefix [ string range $fname 0 [expr $first - 1] ]
	    $jpg_reader SetFilePattern "%s_%04d$suffix"
	    puts stderr "%s_%04d$suffix, $prefix"
	    $jpg_reader SetFilePrefix $prefix
	} else  {
	    set first [ string last "." $fname ] 
	    set prefix [ string range $fname 0 [expr $first - 1] ]
	    $jpg_reader SetFilePattern "%s.%d"
	    $jpg_reader SetFilePrefix $prefix
	}
	
	puts stderr "slices = $minslice:$maxslice"
	$jpg_reader SetDataExtent 0 1023 0 1023 $minslice $maxslice
    }
    
    set thisparam($this,num_frames) 1
    set thisparam($this,num_slices) [ expr $maxslice- $minslice + 1]
    SetFilterCallbacks $jpg_reader $comment
    $jpg_reader Update
    $currentresults ShallowCopyImage [$jpg_reader GetOutput]
    $jpg_reader Delete
    return 1
}
# ---------------------------------------------------------------------------------

itcl::body pxitclimageimport::ImportSignaSpr  { fname minslice maxslice comment } {

    set ana_reader  [  vtkpxAnalyzeImageSourceOld [ pxvtable::vnewobj ]  ]
    set spr [ pxvtable::vnewobj ]
    ::pxtclreadsprheader::ReadHeader  $spr $fname
    ::pxtclreadsprheader::SetAnalyzeImageSource $spr $ana_reader
    $ana_reader SetSwapBytes $thisparam($this,import_swapbytes)
    SetFilterCallbacks $ana_reader $comment
    $ana_reader Update
    $currentresults SetFromAnalyzeSource $ana_reader
    [ $currentresults GetImage ] SetOrigin 0.0 0.0 0.0
    $ana_reader Delete
    return 1
	
}
# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::ImportSignaLX  { fname minslice maxslice comment } {

    set ok 1
    set lx_reader [ vtkpxSignaLXImageSource [ pxvtable::vnewobj ]  ]
    $lx_reader SetFilePrefix [ string range $fname 0 [ string last "i" $fname ]]
    set a [ $lx_reader ReadHeaderFile ]
    if { $a != 0 } {
	set pa_list [ pxitclimageimport::ProcessSignaFilename $fname ] 
	array set pa $pa_list
	
	SetFilterCallbacks $lx_reader $comment
	$lx_reader Load $pa(name) $minslice $maxslice 
	set thisparam($this,num_frames) 1
	set thisparam($this,num_slices) [ expr $maxslice- $minslice + 1]
	$currentresults SetFromAnalyzeSource $lx_reader 
    } else {
	set ok 0 
    }
    $lx_reader Delete
    return $ok
	
}
# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::ImportBinary  { fname minslice maxslice comment } {

    set ana_reader2  [  vtkpxAnalyzeImageSourceOld [ pxvtable::vnewobj ]  ]
    $ana_reader2 SetHeaderInfo  $thisparam($this,import_width) $thisparam($this,import_height) 1 1 $thisparam($this,import_swapbytes) $thisparam($this,import_numbytes)
    $ana_reader2 SetDataOrigin 0.0 0.0 0.0
    
    if { $thisparam($this,import_xsp) < 0.0 } {
	set thisparam($this,import_xsp) [ expr abs(10.0*$thisparam($this,import_xsp)/$thisparam($this,import_width)) ]
	set thisparam($this,import_ysp) [ expr abs(10.0*$thisparam($this,import_ysp)/$thisparam($this,import_height)) ]
	set thisparam($this,import_zsp) [ expr abs(10.0*$thisparam($this,import_zsp)/$thisparam($this,num_slices)) ]
	$ana_reader2 SetDataSpacing $thisparam($this,import_xsp) $thisparam($this,import_ysp) $thisparam($this,import_zsp)
	puts stdout "Spacing = [ $ana_reader2 GetDataSpacing ]"
    } else {
	$ana_reader2 SetDataSpacing $thisparam($this,import_xsp) $thisparam($this,import_ysp) $thisparam($this,import_zsp)
	puts stdout "Spacing = [ $ana_reader2 GetDataSpacing ]"
    }
    $ana_reader2 SetHeaderSize $thisparam($this,import_skip)
    
    if { $minslice != - 1 && $maxslice != - 1 } {
	$ana_reader2 SetImageRange 1 $thisparam($this,num_slices)
    } else {
	set bt [ file size $fname ]
	catch { set maxslice [ expr $bt / ( $thisparam($this,import_width)*$thisparam($this,import_height)*$thisparam($this,import_numbytes) ) ] }
	set minslice 1
	$ana_reader2 SetImageRange $minslice $maxslice
    }
    
    set thisparam($this,num_frames) 1
    set thisparam($this,num_slices) [ expr $maxslice- $minslice + 1]
    $ana_reader2 SetFilePrefix $fname
    SetFilterCallbacks $ana_reader2 $comment
    $ana_reader2 Update
    $currentresults SetFromAnalyzeSource  $ana_reader2 
    $ana_reader2 Delete
    return 1
}
# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::ImportNmr47  { fname minslice maxslice comment } {

    set ok 0
    set header [ pxtclbrukerheader::init ]
    ::pxtclbrukerheader::ReadHeader  $header $fname
    set fname [ ::pxtclbrukerheader::GetFileName $header ]
    set a [ ::pxtclbrukerheader::LoadPolarComplexProgress $header $fname [$currentresults GetImage] "Auto" "$this ProgressCallback" "$this ProgressCallback" ]
    if { $a > 0 } {
	set ok 1
    }
    return $ok
}
# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::ImportAnalyze  { fname minslice maxslice comment } {

    set ok 1
    set ana_reader  [  vtkpxAnalyzeImageSourceOld  [ pxvtable::vnewobj ]  ]
    $ana_reader SetFilePrefix $fname
    set a [ $ana_reader ReadHeaderFile ]
    if { $a != 0 } {
	SetFilterCallbacks $ana_reader $comment
	$ana_reader Update
	$currentresults SetFromAnalyzeSource $ana_reader
    } else {
	set ok 0 
    }
    
    $ana_reader Delete
}

# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::ImportDICOM  { fname minslice maxslice comment } {

    set ok 1
    set ana_reader  [  vtkDICOMImageReader  [ pxvtable::vnewobj ]  ]
    set dname [ file dirname $fname ]
    $ana_reader SetDirectoryName $dname
    SetFilterCallbacks $ana_reader $comment
    $ana_reader Update

    set dim [ [ $ana_reader GetOutput ] GetDimensions ]

    if { [ lindex $dim 2 ] > 0 } {
	$currentresults ShallowCopyImage [  $ana_reader GetOutput ]
	$currentresults CopyImageHeader 0
	$currentresults configure -filename [ file join $dname  "[ $ana_reader GetPatientName ]_[ $ana_reader GetStudyID ]" ]
	pxtkprint "Name = [ $currentresults cget -filename ]"
	set ok 1
    } else {
	set ok 0 
    }
    
    $ana_reader Delete
    return $ok
}
# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::ImportPrism  { fname minslice maxslice comment } {

    set ok 1
    set ana_reader  [  vtkpxPickerPrismSource  [ pxvtable::vnewobj ]  ]
    $ana_reader SetFilePrefix $fname
    set a [ $ana_reader ReadHeaderFile ]
    if { $a != 0 } {
	SetFilterCallbacks $ana_reader $comment
	$ana_reader Update
	$currentresults SetFromAnalyzeSource $ana_reader
    } else {
	set ok 0 
    }
    
    $ana_reader Delete
    return $ok
}
# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::ImportRawMR  { fname minslice maxslice comment } {

    if { [ file exists $fname ] == 0 } { return 0 }
    set wd $thisparam($this,import_width)
    set ht $thisparam($this,import_height)
    set sl $thisparam($this,num_slices)
    set sz [ expr $wd*$ht*$sl*$thisparam($this,import_numbytes)*2 ]
    if { [ file size $fname ] != $sz } {   return 0 }

    set re [ vtkImageReader2 [ pxvtable::vnewobj ]]
    $re SetFileName $fname
    switch -exact $thisparam($this,import_numbytes) {
	"1" { $re SetDataScalarTypeToChar  }
	"2" { $re SetDataScalarTypeToShort }
	"4" { $re SetDataScalarTypeToInt   }
    }
    $re SetDataExtent 0 [ expr $wd*$ht*$sl -1 ] 0 0 0 0
    $re SetNumberOfScalarComponents 2
    $re SetSwapBytes $thisparam($this,import_swapbytes)
    $re SetHeaderSize $thisparam($this,import_skip)
    $re Update
	
    set img [ vtkImageData [ pxvtable::vnewobj ]]
    $img CopyStructure [ $re GetOutput ]
    $img SetWholeExtent 0 [ expr $wd -1 ] 0 [ expr $ht -1 ] 0 [ expr $sl - 1 ]
    $img SetExtent 0 [ expr $wd -1 ] 0 [ expr $ht -1 ] 0 [ expr $sl - 1 ]
    $img SetDimensions  $wd $ht $sl
    $img SetSpacing     $thisparam($this,import_xsp) $thisparam($this,import_ysp) $thisparam($this,import_zsp)
    [ $img  GetPointData ] SetScalars  [[ [ $re GetOutput ] GetPointData ] GetScalars ]
    $re Delete
	
    set pr [ vtkpxProcessRawBrukerFIDImage [ pxvtable::vnewobj ]]
    $pr SetInput $img
    $pr PolarComplexModeOn
    
    if { $thisparam($this,import_mrmode) == "EchoPlanar" } {
	$pr SetEchoPlanarMode 1
    } elseif { $thisparam($this,import_mrmode) == "Conventional (Append)" } {
	$pr SetConventionalAppendMultiSliceMode 1
    }
    $pr SetShiftSpectrum 0
    
    set magn [ vtkImageExtractComponents [ pxvtable::vnewobj ]]
    $magn SetInput [ $pr GetOutput ]
    $magn SetComponents 1
    $magn Update
	
    set r  [[[[ $magn GetOutput] GetPointData ] GetScalars ] GetRange ]
    set r1 [ expr abs ([lindex $r 0 ])]
    set r2 [ expr abs ([lindex $r 1 ])]
    if { $r1 >= $r2 } { set r2 $r1 }
    
    set sc [ vtkImageShiftScale [ pxvtable::vnewobj ]]
    $sc SetInput [ $magn GetOutput ]
    $sc SetScale [ expr 4096/($r2+1.0) ]
    $sc SetOutputScalarTypeToShort 
    $sc Update

    $currentresults ShallowCopyImage [ $sc GetOutput  ]
    $magn Delete
    $pr   Delete
    $img  Delete
    $sc   Delete
    return 1
}

# ---------------------------------------------------------------------------------
#   Import Image and Update Parent
# ---------------------------------------------------------------------------------
itcl::body pxitclimageimport::ImportImage { } {
 
    if { $thisparam($this,currentfilename) == "" } { 
	::pxtclutil::Warning "No filename selected\n Cannot load image!"
	return 
    }
    
    set ok 1
    set fname $thisparam($this,currentfilename)
    set minslice $thisparam($this,import_bottomslice)
    set maxslice $thisparam($this,import_topslice)
    set comment "Loading Image from [ file tail $fname ] ($minslice:$maxslice)"
    
    
    puts stderr "$comment"

    switch -exact $thisparam($this,image_type) {
	"TIFF"      { set ok [ ImportTIFF     $fname $minslice $maxslice $comment ] }
	"JPEG"      { set ok [ ImportJPEG     $fname $minslice $maxslice $comment ] }
	"BMP"       { set ok [ ImportBMP      $fname $minslice $maxslice $comment ] }
	"PNM"       { set ok [ ImportPNM      $fname $minslice $maxslice $comment ] }
	"Signa Spr" { set ok [ ImportSignaSpr $fname $minslice $maxslice $comment ] }
	"Signa LX"  { set ok [ ImportSignaLX  $fname $minslice $maxslice $comment ] }
	"Binary"    { set ok [ ImportBinary   $fname $minslice $maxslice $comment ] }
	"Nmr47"     { set ok [ ImportNmr47    $fname $minslice $maxslice $comment ] }
	"Analyze"   { set ok [ ImportAnalyze  $fname $minslice $maxslice $comment ] }
	"Varian"    { set ok [ ImportVarian   $fname $minslice $maxslice $comment ] }
	"DICOM"     { set ok [ ImportDICOM    $fname $minslice $maxslice $comment ] }
	"Raw MR"    { set ok [ ImportRawMR    $fname $minslice $maxslice $comment ] }
	"Prism (SPECT)" { set ok [ ImportPrism    $fname $minslice $maxslice $comment ] }
    }

    if { $ok == 0 } {
	::pxtclutil::Warning "Failed to Import image $fname"
	return
    } else {
	set need_load 0
    }
}

itcl::body pxitclimageimport::UpdateParent { } { 

    if { $thisparam($this,currentfilename) == "" } {
	::pxtclutil::Warning "No Image Filename has been selected!"
	return
    }
    
    if { $need_load == 1 } {
	ImportImage 
	set orient [ lsearch -exact $orientations $thisparam($this,import_orient) ]
	$currentresults CopyImageHeader $orient
	$currentresults configure -filename $thisparam($this,currentfilename)
	if { $thisparam($this,image_type) == "Signa LX" } {
	    set fname $thisparam($this,currentfilename)
	    $currentresults configure -filename [ string range $fname 0 [ expr [string last "s" $fname ] +1 ]]
	}
	$currentresults FlipImage $thisparam($this,import_flip)
    }
	
    SendImageToParent
}


