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

package provide pxitclsubjectentry 1.0

# 	$Id: pxitclsubjectentry.tcl,v 1.1 2002/06/11 01:21:41 papad Exp papad $	





package require  labelframe 1.0
package require  pxtclutil  1.0
package require  pxvtable   1.0
package require  vtkpxcontrib 1.1
package require  pxitclbasecontrol 1.0


# ---------------------------------------------------------------------------
#  Ancilliary Class pxitclsubject
# ---------------------------------------------------------------------------
itcl::class pxitclsubject {

    public    variable anatomical_image  0
    public    variable conventional_image  0
    public    variable functional_image 0
    public    variable echoplanar_image 0
    public    variable fieldmap_image 0
    public    variable reference_transform 0
    public    variable internal_transform 0
    public    variable distortion_transform 0
    public    common   util 0
    public    variable subject_id ""

    constructor { } {
	set anatomical_image      [  [ pxitclimage \#auto ] GetThisPointer ]
	set conventional_image    [  [ pxitclimage \#auto ] GetThisPointer ]
	set functional_image       [  [ pxitclimage \#auto ] GetThisPointer ]
	set echoplanar_image       [  [ pxitclimage \#auto ] GetThisPointer ]
	set fieldmap_image       [  [ pxitclimage \#auto ] GetThisPointer ]
	
	set internal_transform  [ [ pxitcltransform \#auto ] GetThisPointer ]
	set reference_transform [ [ pxitcltransform \#auto ] GetThisPointer ]
	set distortion_transform [ [ pxitcltransform \#auto ] GetThisPointer ]

	if { $util == 0 } { set util [ vtkpxUtil [ pxvtable::vnewobj ]] }
    }

    destructor  { 

	catch { delete object $anatomical_image }
	catch { delete object $conventional_image }
	catch { delete object $functional_image }
	catch { delete object $echoplanar_image }
	catch { delete object $fieldmap_image }

	catch { delete object $internal_transform }
	catch { delete object $reference_transform }
	catch { delete object $distortion_transform }
    }

    public method  GetThisPointer  { } { return $this }
    public method  GetDescription { }
    public method  RenameFunctional { oldsuffix newsuffix }

    public proc    GetFileElement { fileid }
    public method  LoadFromSetupFile { fileid { mode 0 } }
    public method  SaveToSetupFile   { fileid }
    public method  SaveToDirectory   { newdir { keepnewfunctionalname 1 } { notfunctionalonly 0  } }




    public method  GetBaseName {  }
    public method  GetNameList {  }

    public method  UnloadImages { } 
}


# ---------------------------------------------------------------------------
# Get Description
# ---------------------------------------------------------------------------
itcl::body pxitclsubject::GetDescription {  } {

    set name [ $anatomical_image cget -filename ]
    set len [ string length $name ]
    if { [ $len > 40 ] } {
	set name [ string range $name [ expr $len - 40 ] [ expr $len -1 ] ]
    }
    
    set desc [ format "%40s : A %12s C %12s F %12s TR %10s TI %10s" $name [ $anatomical_image cget -status ] [ $conventional_image cget - status ] [ $functional_image cget -status ] [ $reference_transform GetClassName ] [ $internal_transform GetClassName ]]
    return $desc
}
# ---------------------------------------------------------------------------
itcl::body pxitclsubject::RenameFunctional { oldsuffix newsuffix } {

    set f [ $functional_image cget -filename ]
    $functional_image UnLoad
    set ind [ string last $oldsuffix $f ]
    if { $ind >=0 } {
	regsub -start $ind $oldsuffix $f $newsuffix thisparam(${nm}filename)
	$functional_image configure -filename $f
	return 1
    }
    return 0
}
# ---------------------------------------------------------------------------
itcl::body pxitclsubject::UnloadImages { }  {

    set imlist [ list $anatomical_image $conventional_image $functional_image $echoplanar_image $fieldmap_image]
    for { set i 0 } { $i<=2 } { incr i } {
	set img [ lindex $imlist $i]
	set  f [ $img cget -filename ]
	$img UnLoad
	$img configure -filename $f
    }
}
# ---------------------------------------------------------------------------
itcl::body pxitclsubject::GetFileElement { fileid } {

    gets $fileid line 
    set index [string first ":" $line]
    if { $index > 0 }  {
	set len   [string length $line]
	set value [string trim [string range $line [expr $index +1] [expr $len -1]]]
	return $value
    }
    return ""
}

itcl::body pxitclsubject::LoadFromSetupFile { fileid {mode 0} } {

    UnloadImages
    $reference_transform Clear
    $internal_transform Clear
    $distortion_transform Clear
    if { $mode > 1 } {
	set subject_id [ GetFileElement $fileid ]
    }
    $anatomical_image configure -filename [ GetFileElement $fileid ]
    $conventional_image configure -filename [ GetFileElement $fileid ]
    $functional_image configure -filename [ GetFileElement $fileid ]
    $reference_transform configure -filename [ GetFileElement $fileid ]
    $internal_transform configure -filename [ GetFileElement $fileid ]
    if { $mode > 0 } {
	$echoplanar_image configure -filename [ GetFileElement $fileid ]
	$distortion_transform configure -filename [ GetFileElement $fileid ]
	$fieldmap_image configure -filename [ GetFileElement $fileid ]
    }
    gets $fileid line
}
# ---------------------------------------------------------------------------
itcl::body pxitclsubject::SaveToDirectory   { newdir { keepnewfunctionalname 1 } { notfunctionalonly 0 }  } {


    if { $notfunctionalonly > 0 } {
	set itlist [ list $functional_image $anatomical_image  $conventional_image    $reference_transform  $internal_transform  $echoplanar_image  $distortion_transform  $fieldmap_image ]
    } else {
	set itlist [ list $functional_image ]
    }

    for { set i 0 } { $i < [ llength $itlist ] } { incr i } {
	set obj [ lindex $itlist $i ]
	set fn [ $obj cget -filename ]
	if { [ string length $fn ] > 1 } {
	    set newname [ file join $newdir [ file tail $fn ]]
	    $obj configure -filename $newname
	    $obj Save
	    pxtkprint "Mapping $fn --> $newname\n"
	    if { $keepnewfunctionalname == 0 && $i == 0 } {
		# Restore functional name back so that it can be renamed for next task
		$obj configure -filename $fn
	    }
	}
    }
}

# ---------------------------------------------------------------------------
itcl::body pxitclsubject::SaveToSetupFile   { fileid } {

    puts $fileid "Subject Id : $subject_id"
    puts $fileid "Anatomical Image : [ $anatomical_image cget -filename ]"
    puts $fileid "Conventional Image : [ $conventional_image cget -filename ]"
    puts $fileid "Functional Image : [ $functional_image cget -filename ]"
    puts $fileid "Reference Transformation : [ $reference_transform cget -filename ]"
    puts $fileid "Internal Transformation :  [ $internal_transform cget -filename ]"
    puts $fileid "Echoplanar Image : [ $echoplanar_image cget -filename ]"
    puts $fileid "Distortion Transformation :  [ $distortion_transform cget -filename ]"
    puts $fileid "Fieldmap Image : [ $fieldmap_image cget -filename ]"
    puts $fileid "-------------------------------------------------------"
}

# ---------------------------------------------------------------------------
itcl::body pxitclsubject::GetBaseName {  } {

    set b [ $anatomical_image cget -filename ]
    if { [ string length $b ] < 1 } {
	set b "New_Subject"
    }

    set out [ file root [ file tail $b ] ]

    if { $subject_id == "" } {
	return  $out
    } 
    return ${subject_id}
}

itcl::body pxitclsubject::GetNameList {  } {

    set rlist "" 
    lappend rlist [ list $this.1  "Anat:[ file tail [ $anatomical_image cget -filename ]]" ] 
    lappend rlist [ list $this.2  "Conv:[ file tail [ $conventional_image cget -filename ]]" ]
    lappend rlist [ list $this.3  "Func:[ file tail [ $functional_image cget -filename ]]" ]
    lappend rlist [ list $this.4  "RefX:[ file tail [ $reference_transform cget -filename]]" ]
    lappend rlist [ list $this.5  "IntX:[ file tail [ $internal_transform cget -filename]]"  ]
    lappend rlist [ list $this.6  "EPIm:[ file tail [ $echoplanar_image cget -filename]]"  ]
    lappend rlist [ list $this.7  "DistX:[ file tail [ $distortion_transform cget -filename]]"  ]
    lappend rlist [ list $this.8  "Fldmp:[ file tail [ $fieldmap_image cget -filename]]"  ]
    return $rlist
}
# ---------------------------------------------------------------------------
#
#
#
#                     pxitcltask
#
#
#
# ---------------------------------------------------------------------------
#  Ancilliary Class pxitclsubject
# ---------------------------------------------------------------------------
itcl::class pxitcltask {

    public    variable   task_name ""
    public    variable   task_suffix ""

    constructor { } {
    }

    public method  LoadFromSetupFile { fileid }
    public method  SaveToSetupFile   { fileid }
    public method  GetThisPointer { } { return $this }
    public method  GetDescription { } { return "Task : $task_name\nSuffix=$task_suffix\n"}
    public method  GetBaseName {  }
    public method  GetNameList {  }



}
# ---------------------------------------------------------------------------
itcl::body pxitcltask::LoadFromSetupFile { fileid } {


    set task_name [ pxitclsubject::GetFileElement $fileid ]
    set task_suffix [ pxitclsubject::GetFileElement $fileid ]
    gets $fileid line
}
# ---------------------------------------------------------------------------
itcl::body pxitcltask::SaveToSetupFile   { fileid } {

    puts $fileid "Task Name   : $task_name"
    puts $fileid "Task Suffix : $task_suffix"
    puts $fileid "-------------------------------------------------------"
}

itcl::body pxitcltask::GetBaseName {  } {
    set b $task_name 
    if { [ string length $task_name ] < 1 } {
	set b "New_Task"
    }
    return $b
}

itcl::body pxitcltask::GetNameList {  } {
    set rlist ""
    lappend rlist  [ list $this.1 "Suffix: $task_suffix" ]
    return $rlist
}


# -------------------------------------------------------------------------------------
#
#
#
#         task GUI stuff
#
#
# -------------------------------------------------------------------------------------

itcl::class pxitcltaskGUI {

    public    variable  itcl_task 0
    protected variable  basewidget  0
    protected variable  initialized 0
    private   common    thisparam
    protected variable  owns_task 1
    public variable     description "Task"
    public variable     callback 0

    constructor { } {
	Reset
    }

    destructor {
	if { $owns_task == 1} {
	    catch { itcl::delete object $itcl_task }
	}
    }

    public    method GetThisPointer { } { return $this }
    public    method GetTask { } { return $itcl_task  }
    public    method SetTask { task }
    public    method Reset { } 
    public    method Initialize { basewidg } 
    public    method Update { }
    public    method UpdateEntry { }
}
# ----------------------------------------------------------
itcl::body pxitcltaskGUI::Reset { } {

    if { $itcl_task !=0 && $owns_task == 1} {
	itcl::delete object $itcl_task
    }
    
    set itcl_task  [ [  pxitcltask \#auto ] GetThisPointer ]
    set owns_task 1
    Update
}

itcl::body pxitcltaskGUI::Update { } {
    
    set thisparam($this,name)        [ $itcl_task cget -task_suffix ]
    set thisparam($this,description) [ $itcl_task cget -task_name ]
}

itcl::body pxitcltaskGUI::UpdateEntry { } {
    
    $itcl_task configure -task_name $thisparam($this,description)
    $itcl_task configure -task_suffix $thisparam($this,name)
    if { $callback !=0 } {
	eval $callback
    }
}


itcl::body pxitcltaskGUI::SetTask { trn } {
    
    if { [ $trn isa pxitcltask ] == 0 } {
	return 0
    }
	
    if { $owns_task == 1} {
	catch { itcl::delete object $itcl_task }
    }

    set itcl_task $trn
    set owns_task 0
    Update
}

itcl::body pxitcltaskGUI::Initialize { basewidg } {

    if { $initialized == 1 } { return $basewidget }
    
    set basewidget [  iwidgets::Labeledframe $basewidg -labelpos n -labeltext $description ]
    set base [ $basewidget childsite ]
    frame $base.mid

    iwidgets::entryfield $base.0 -labeltext "Task Description:"  -width 30  -textvariable [ itcl::scope thisparam($this,description) ] -relief sunken 
    iwidgets::entryfield $base.1 -labeltext "Task Suffix:"  -width 10  -textvariable [ itcl::scope thisparam($this,name) ] -relief sunken 
    pack $base.0 $base.1 -side top -expand f -fill x

    set w [ frame $base.bot ]
    eval "button $w.upd -text Update -command {  $this UpdateEntry }"
    pack  $w.upd  -side left -fill x -expand f
    pack  $base.0 $base.1 $base.bot -side top -expand f -fill x -padx 5
    set initialized 1
    Update

    return $basewidget
}
# ----------------------------------------------------------

