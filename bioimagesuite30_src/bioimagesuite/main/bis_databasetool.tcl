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

#---------------------------------------------------------------------------------------

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]


package provide bis_databasetool 1.0

package require vtkpxcontrib 1.0
package require pxvtable 1.0
package require pxtclhelp  1.0
package require pxtclutil  1.0
package require pxitclimage  1.0
package require Iwidgets 4.0
package require pxitclbasecontrol 
package require  labelframe 1.0
# This loads the database library


# --------------------------------------------------------------------------

itcl::class bis_databasetool {
    
    inherit pxitclbasecontrol


    protected variable database 0
    protected variable initialized 0
    protected variable standalone  0
    protected variable orthoviewer 0
    protected variable seriesnameindex -1
    protected variable seriespathindex -1
    protected common   thisparam

    constructor { par } {
	::pxitclbasecontrol::constructor $par
    } {
	$this Initialize
    }

    public method GetThisPointer { } { return $this }
    public method InitializeGUI { parent { standalone 0 } }
    
    public method Initialize { }
    public method ResetValues { }
    public method MySQLConnect { }
    public method SQLiteConnect { { fname "" } }
    public method SQLiteCreate  { { fname "" } }
    public method Disconnect { }
    public method loadImage { } 
    public method loadImage2 { } 
    public method loadImageFileName { fname }
    public method loadDICOMImage { fpath }
    public method searchImage {} 
    public method addPatient {} 
    public method clearPatient {} 

    public method addProtocol {} 
    public method clearProtocol {} 


    public method searchPatient {} 
    public method selectPatient { } 
    public method searchProtocol {} 
    public method selectProtocol { }
    public method addVisit {} 
    public method searchVisit {} 
    public method selectVisit { } 
    public method addSeries {} 
    public method browse { } 

    # New Query Stuff 31 Oct 2008
    #


    protected method beuatifyQuery { inp }
    public method searchSavedQuery { }
    public method selectSavedQuery { }
    public method runSavedQuery { } 
    public method abbreviation { input }

    # ----------------------------------
    # GUI Stuff
    # ----------------------------------
    protected method CreateServerGUI { base } 
    protected method CreateQueryGUI { base } 
    protected method CreatePatientGUI { base } 
    protected method CreateProtocolGUI { base } 
    protected method CreateVisitGUI { base } 
    protected method CreateSeriesGUI { base } 
    protected method CreateQueryQueryGUI { qframe } 

};

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::Initialize { } {

    # Server Info
    # ---------------

    set thisparam($this,username) ""
    set thisparam($this,password) ""
    set thisparam($this,host) ""
    set thisparam($this,database) "haven"
    set thisparam($this,port) "3306"
    set thisparam($this,connected) 0
    set thisparam($this,connectionstatus) "Not Connected"
    set thisparam($this,sqlitefile) ""
    set thisparam($this,debugdbase) 0

    global env

    set fname [ file join $env(HOME) ".my.cnf" ]
    if { [ file exists $fname ] } {
	set fin [ open $fname r ]
	while { [ gets $fin line ] >=0 } {
	    set lst [ split $line = ]
	    if { [ llength $lst ] == 2 } {
		set first [ string trim [ lindex $lst 0 ]]
		set second [ string trim [ lindex $lst 1 ]]
		if { $first == "user" } {
		    set thisparam($this,username) $second
		} elseif { $first == "password" } {
		    set thisparam($this,password) $second
		} elseif { $first == "host" } {
		    set thisparam($this,host) $second
		    if { $second == "centos5db" || $second == "192.168.0.111" } {
			set thisparam($this,database) "YaleTest" 
		    }
		}
	    }
	}
	close $fin
    }
    $this ResetValues
}
# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::ResetValues {} {

    set thisparam($this,firstname) ""
    set thisparam($this,lastname)  ""
    set thisparam($this,birthdate) ""
    set thisparam($this,protocolname) ""
    set thisparam($this,pi) ""
    set thisparam($this,visitdate) ""
    set thisparam($this,seriesdatapath) ""
    set thisparam($this,seriesfilename) ""
}

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::MySQLConnect {} {
    

    if { $thisparam($this,connected) == 1 } {
	tk_messageBox -type ok -message "Already Connected to a database" -title "Error" -icon error
	return
    }

    set dbname    $thisparam($this,database) 
    set host      $thisparam($this,host) 
    set port      $thisparam($this,port)
    set username  $thisparam($this,username)
    set password  $thisparam($this,password)

    if { $database == 0 } {
	set database [ vtkbisDatabase New ]
    }
    $database Initialize $dbname $host $username $password $port

    set connect [ $database Connect ]

    if { $connect == "1" } {
	tk_messageBox -type ok -message "Login to database successfully" -title "OK" -icon info
	set thisparam($this,connectionstatus) "Connected to server ${host}:$port as $username"
	set thisparam($this,connected) 1
    } else {
	tk_messageBox -type ok -message "Fail to login to database" -title "Error" -icon error
	set thisparam($this,connected) 0
	set thisparam($this,connectionstatus) "Not Connected"
    }
    wm title $basewidget "BioImage Suite::Database Control $thisparam($this,connectionstatus)"

    if { $thisparam($this,debugdbase) == 1 } {
	puts stderr "Setting Debug Mode = ON"
	$database DebugOn

    } else {
	$database DebugOff
	puts stderr "Setting Debug Mode = OFF"
    }
}

itcl::body bis_databasetool::SQLiteCreate { { fname "" } } { 

    if { $thisparam($this,connected) == 1 } {
	tk_messageBox -type ok -message "Already Connected to a database" -title "Error" -icon error
	return
    }

    if { $fname == "" } {
	set typelist { {"SQLite Files" { .sqlite }}}
	set fname [tk_getSaveFile -title "Specify an SQLite Filename" -filetypes $typelist -initialfile $thisparam($this,sqlitefile) ]
    }
    if { [ string length $fname ] < 1 } {
	return 
    }


    set iname [ file join $::pxtclvtkpxcontrib::imagedir sampledatabase.sqlite ]
    set ok [ file copy -force $iname $fname ]

    if { $ok == 0 } {
	tk_messageBox -type ok -message "Cannot create $fname" -title "Error" -icon error
	return 0
    }
    puts stderr "ok=$ok"

    return [ $this SQLiteConnect $fname ]

}


itcl::body bis_databasetool::SQLiteConnect { { fname "" } } { 
    
    if { $thisparam($this,connected) == 1 } {
	tk_messageBox -type ok -message "Already Connected to a database" -title "Error" -icon error
	return
    }

    if { $fname == "" } {
	set typelist { {"SQLite Files" { .sqlite }}}
	lappend typelist {"All Files" {*}}

	set fname [tk_getOpenFile -title "Specify an SQLite Filename" -filetypes $typelist -initialfile $thisparam($this,sqlitefile) ]
    }
    if { [ string length $fname ] < 1 } {
	return 
    }

    if { [ file exists $fname ] == 0 } {
	return
    }

    if { $database == 0 } {
	set database [ vtkbisDatabase New ]
    }

    if { $thisparam($this,debugdbase) == 1 } {
	puts stderr "Setting Debug Mode = ON"
	$database DebugOn
    } else {
	puts stderr "Setting Debug Mode = OFF"
	$database DebugOff
    }


    $database InitializeSQLite $fname

    set connect [ $database Connect ]

    if { $connect == "1" } {
	tk_messageBox -type ok -message "Connected to database successful" -title "OK" -icon info
	set thisparam($this,connectionstatus) "Connected to sqlite file $fname"
	set thisparam($this,connected) 1
    } else {
	tk_messageBox -type ok -message "Fail to to access database file" -title "Error" -icon error
	set thisparam($this,connected) 0
	set thisparam($this,connectionstatus) "Not Connected"
    }
    set thisparam($this,sqlitefile) $fname

    wm title $basewidget "BioImage Suite::Database Control $thisparam($this,sqlitefile)"
}

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::Disconnect {} {
    

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Not connected to a database" -title "Error" -icon error
	return 
    }

    catch { $database Disconnect }
    set thisparam($this,connected) 0
    set thisparam($this,connectionstatus) "Not Connected"
    wm title $basewidget "BioImage Suite::Database Control $thisparam($this,connectionstatus)"

    return

}

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::loadDICOMImage { filepath } {

    
    set namelist [ lsort [ glob [ file join ${filepath} *.dcm ]] ]
    if { [ llength $namelist ] < 1 } {
	return 0
    }
    
    set fname [ lindex $namelist 0 ]

    if { $standalone == 1 } {
	set timg [ [ pxitclimage \#auto ] GetThisPointer ]
	$timg LoadDICOM $fname 1 $parent 
	
	set img [ $timg GetImage ]
	if { [ $img  GetNumberOfPoints ] >  1 } {
	    $orthoviewer SetImage $img [  $timg GetOrientation ]
	    wm title $basewidget [ file tail $fname ]
	    $notebook view "Viewer"
	}
	itcl::delete obj $timg
	return 1
    }  else {
	return [ $parent LoadImage $fname ]
    }
    
}
# ------------------------------------------------------------------------------------

itcl::body bis_databasetool::loadImage2 { } {

    set name [ $thisparam($this,savequeryframe2_querylist) getcurselection ]

    set tmp  [ split $name | ]
    set n    [ llength $tmp ]

    if { $seriesnameindex == -1 } {
	set filename [ lindex $tmp [ expr $n -2 ]]
    } else {
	set filename [ lindex $tmp $seriesnameindex ]
    }

    if { $seriespathindex == -1 } {
	set filepath [ lindex $tmp [ expr $n -1 ]]
    } else {
	set filepath [ lindex $tmp $seriespathindex ] 
    }

    puts stderr "Filename = $filename, filepath=$filepath"

 
    if { $filename == "" } {
	puts stderr "DICOM ... empty filename"
	return [ $this loadDICOMImage $filepath ]
    }

    set fname [ file join $filepath $filename ]
   
    return [ $this loadImageFileName $fname ]
}

itcl::body bis_databasetool::loadImage { } {

    set name [ $thisparam($this,optframe_querylist) getcurselection ]
    
    set tmp  [ split $name | ]
    set n    [ llength $tmp ]

    set filename [ lindex $tmp [ expr $n -2 ]]
    set filepath [ lindex $tmp [ expr $n -1 ]]

    if { $filename == "" } {
	#puts stderr "DICOM ... empty filename"
	return [ $this loadDICOMImage $filepath ]
    }

    set fname [ file join $filepath $filename ]


    return [ $this loadImageFileName $fname  ]
}

itcl::body bis_databasetool::loadImageFileName { fname } {

    if { $standalone == 1 } {
	set ana  [ vtkpxAnalyzeImageSource New ]
	$ana Load $fname
	
	set img [ $ana GetOutput ]
	if { [ $img  GetNumberOfPoints ] >  1 } {
	    $orthoviewer SetImage [ $ana GetOutput ] [ $ana GetOrientation ]
	    wm title $basewidget [ file tail $fname ]
	    $notebook view "Viewer"
	}
	$ana Delete
	return
    }
    
    $parent LoadImage $fname
}


# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::searchImage {} {

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Please Connect To Server First Before trying to Execute a Query" -title "Error!" -icon error
	return
    }
    
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
    }

    set firstname $thisparam($this,firstname)
    set lastname $thisparam($this,lastname)  
    set birthdate $thisparam($this,birthdate)
    set protocolname $thisparam($this,protocolname)
    set pi $thisparam($this,pi)
    set visitdate $thisparam($this,visitdate)
    set seriesdatapath $thisparam($this,seriesdatapath)
    set seriesfilename $thisparam($this,seriesfilename)

    # Create Database Connector
    set  sqlstring "select Patient.Firstname, Patient.Lastname, Patient.Birthdate, Protocol.protocolname, "
    append sqlstring "Protocol.Pi, Visit.Visitdate, Series.SeriesFileName, Series.seriesdatapath "
    append sqlstring "from Series,Injection,Visit,Protocol,Patient "
    append sqlstring "where Series.injectionid=Injection.injectionid and Injection.visitid=Visit.visitid "
    append sqlstring "and Visit.patientid=Patient.patientid and Visit.protocolid=Protocol.protocolid "
    append sqlstring "and Patient.firstname like '%"
    append sqlstring $firstname
    append sqlstring "%' "
    append sqlstring "and Patient.lastname like '%"
    append sqlstring $lastname
    append sqlstring "%' "
    append sqlstring "and Patient.birthdate like '%"
    append sqlstring $birthdate
    append sqlstring "%' "
    append sqlstring "and Protocol.protocolname like '%"
    append sqlstring $protocolname
    append sqlstring "%' "
    append sqlstring "and Protocol.pi like '%"
    append sqlstring $pi
    append sqlstring "%' "
    append sqlstring "and Visit.visitdate like '%"
    append sqlstring $visitdate
    append sqlstring "%' "
    append sqlstring "and Series.seriesdatapath like '%"
    append sqlstring $seriesdatapath
    append sqlstring "%' "
    append sqlstring "and (Series.seriesfilename like '%"
    append sqlstring $seriesfilename
    append sqlstring "%' or Series.seriesfilename is null) "

    if { ![ $database Connect ]} {
  	puts "Cannot connect to database. Exit."
  	pxtkexit
    }

    $thisparam($this,optframe_querylist) delete 0 end

    set query [ $database RunComplexQuery $sqlstring ]
    set i 0
    while {[$query NextRow]} {
  	set i [expr $i +1 ]
  	set firstname [ $database GetQueryValueAsString $query 0 ] 
  	set lastname [ $database GetQueryValueAsString $query 1 ] 
  	set birthdate [ $database GetQueryValueAsString $query 2 ] 
  	set protocolname [ $database GetQueryValueAsString $query 3 ] 
  	set pi [ $database GetQueryValueAsString $query 4 ] 
  	set visitdate [ $database GetQueryValueAsString $query 5 ] 
  	set seriesfilename [ $database GetQueryValueAsString $query 6 ] 
	set seriesdatapath [ $database GetQueryValueAsString $query 7 ] 

  
	set record "$i|$firstname|$lastname|$birthdate|$protocolname|$pi|$visitdate|$seriesfilename|$seriesdatapath"
	set rf  $thisparam($this,optframe_rf)
	$rf.result insert end $record
    }
}

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::addPatient {} {

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Please Connect To Server First Before trying to Execute a Query" -title "Error!" -icon error
	return
    }

    set thisparam($this,namelist) { { FirstName firstname } { LastName lastname } { BirthDate birthdate } }

    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	if { $thisparam($this,$field) == "" } {
	    tk_messageBox -type ok -message "Parameter $label not set" -title "Error!" -icon error
	    return
	}
    }

    set firstname $thisparam($this,firstname)
    set lastname $thisparam($this,lastname)  
    set birthdate $thisparam($this,birthdate)

    set match [ regexp {\d\d\d\d-\d\d-\d\d} $thisparam($this,birthdate) ]
    if { $match == "0" } {
      tk_messageBox -type ok -message "BirthDate in wrong format yyyy-yy-yy" -title "Error!" -icon error
      return
    }

    # Create Database Connector
    set patient [ vtkbisPatient New ]
    $patient Initialize $firstname $lastname $birthdate 

    puts stderr "Setting Connector [ $database GetClassName ], [ $database GetMode ]"

    $patient SetConnector $database
    set ok [ $patient AddPatient ]
    if { $ok == "0" } {
      tk_messageBox -type ok -message "Patient exists, not add again." -title "Error!" -icon error
      return
    }

    puts stderr "\n\n\n --------------------------- this far OK ----------------------------------\n"

    set patientid [ $patient SearchPatientID $firstname $lastname $birthdate ]
    $patient Delete
    #    $patient SetPatientID $patientid
    tk_messageBox -type ok -message "Patient was added as patientid $patientid" -title "OK" -icon info
    
}


itcl::body bis_databasetool::clearPatient {} {
    set thisparam($this,firstname) ""
    set thisparam($this,lastname)   ""
    set thisparam($this,birthdate) ""

}

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::addProtocol {} {

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Please Connect To Server First Before trying to Execute a Query" -title "Error!" -icon error
	return
    }

    set thisparam($this,namelist) { { ProtocolName protocolname } { Pi pi } }

    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	if { $thisparam($this,$field) == "" } {
	    tk_messageBox -type ok -message "Parameter $label not set" -title "Error!" -icon error
	    return
	}
    }

    set protocolname $thisparam($this,protocolname)
    set pi $thisparam($this,pi)  

    
    set protocol [ vtkbisProtocol New ]
    $protocol SetConnector $database
    $protocol SetProtocolName $protocolname
    $protocol SetPI $pi

    set protocolid [ $protocol SearchProtocolID $protocolname $pi ]
    $protocol SetProtocolID $protocolid
    if { [ string length $protocolid ] ==0 } {
	puts stderr "Going to Add Protocol"
	$protocol AddProtocol
	puts stderr " Protocol added going back to get the true id"
   	set protocolid [ $protocol SearchProtocolID $protocolname $pi ]
        tk_messageBox -type ok -message "Protocol was added as protocolid $protocolid" -title "OK" -icon info
    } else {
        tk_messageBox -type ok -message "Protocol exists, not add again" -title "Error!" -icon error
    }
    $protocol Delete
}

itcl::body bis_databasetool::clearProtocol {} {
    set thisparam($this,protocolname) ""
    set thisparam($this,pi)   ""
}
# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::searchPatient {} {

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Please Connect To Server First Before trying to Execute a Query" -title "Error!" -icon error
	return
    }
    
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
    }
    set firstname $thisparam($this,firstname)
    set lastname $thisparam($this,lastname)  
    set birthdate $thisparam($this,birthdate)
    set patientid ""

    
    set  sqlstring "select PatientID, Firstname, Lastname, Birthdate from Patient "
    append sqlstring "where firstname like '%"
    append sqlstring $firstname
    append sqlstring "%' "
    append sqlstring "and lastname like '%"
    append sqlstring $lastname
    append sqlstring "%' "
    append sqlstring "and birthdate like '%"
    append sqlstring $birthdate
    append sqlstring "%' "

    $thisparam($this,visitframe1_querylist) delete 0 end
    set query [ $database RunComplexQuery $sqlstring ]
    set i 0
    while {[$query NextRow]} {
  	set i [expr $i +1 ]
 	set patientid [ $database GetQueryValueAsString $query 0 ] 
  	set firstname [ $database GetQueryValueAsString $query 1 ] 
  	set lastname [ $database GetQueryValueAsString $query 2 ] 
  	set birthdate [ $database GetQueryValueAsString $query 3 ] 
	set record "$i|$patientid|$firstname|$lastname|$birthdate"
	set rf $thisparam($this,visitframe1_rf)
	$rf.result insert end $record
    }
}

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::selectPatient { } {

    set name [ $thisparam($this,visitframe1_querylist) getcurselection ]
    set tmp  [ split $name | ]
    set patientid [ lindex $tmp 1 ]
    set thisparam($this,patientid) $patientid
    tk_messageBox -type ok -message "You select PatientID $patientid" -title "OK" -icon info
    set thisparam($this,patientid_selected) 1
}

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::searchProtocol {} {

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Please Connect To Server First Before trying to Execute a Query" -title "Error!" -icon error
	return
    }
    
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
    }
    set protocolname $thisparam($this,protocolname)
    set pi $thisparam($this,pi)  
    set protocolid ""

    
    set  sqlstring "select ProtocolID, ProtocolName, PI from Protocol "
    append sqlstring "where protocolname like '%"
    append sqlstring $protocolname
    append sqlstring "%' "
    append sqlstring "and pi like '%"
    append sqlstring $pi
    append sqlstring "%' "

    $thisparam($this,visitframe2_querylist) delete 0 end
    set query [ $database RunComplexQuery $sqlstring ]
    set i 0
    while {[$query NextRow]} {
  	set i [expr $i +1 ]
 	set protocolid [ $database GetQueryValueAsString $query 0 ] 
  	set protocolname [ $database GetQueryValueAsString $query 1 ] 
  	set pi [ $database GetQueryValueAsString $query 2 ] 
	set record "$i|$protocolid|$protocolname|$pi"
	set rf $thisparam($this,visitframe2_rf)
	$rf.result insert end $record
    }
}

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::selectProtocol { } {

    set name [ $thisparam($this,visitframe2_querylist) getcurselection ]
    set tmp  [ split $name | ]
    set protocolid [ lindex $tmp 1 ]
    set thisparam($this,protocolid) $protocolid
    tk_messageBox -type ok -message "You select ProtocolID $protocolid" -title "OK" -icon info
    set thisparam($this,protocolid_selected) 1
}


# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::addVisit {} {

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Please Connect To Server First Before trying to Execute a Query" -title "Error!" -icon error
	return
    }
    if { $thisparam($this,patientid_selected) == 0 } {
	tk_messageBox -type ok -message "Please Select Patient first" -title "Error!" -icon error
	return
    }
    if { $thisparam($this,protocolid_selected) == 0 } {
	tk_messageBox -type ok -message "Please Select Protocol first" -title "Error!" -icon error
	return
    }

    set thisparam($this,namelist) { { VisitDate visitdate } }
    
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	if { $thisparam($this,$field) == "" } {
	    tk_messageBox -type ok -message "Parameter $label not set" -title "Error!" -icon error
	    return
	}
    }
    set match [ regexp {\d\d\d\d-\d\d-\d\d} $thisparam($this,visitdate) ]
    if { $match == "0" } {
      tk_messageBox -type ok -message "VisitDate in wrong format" -title "Error!" -icon error
      return
    }

    set patientid $thisparam($this,patientid)
    set protocolid $thisparam($this,protocolid)  
    set visitdate $thisparam($this,visitdate)

    set visit [ vtkbisVisit New ]
    $visit SetConnector $database
    $visit SetPatientID $patientid
    $visit SetProtocolID $protocolid
    $visit SetVisitDate $visitdate
    set visitid [ $visit SearchVisitID $patientid $protocolid $visitdate ]
    $visit SetVisitID $visitid
    if { [  string length $visitid ] ==0 } {
	$visit AddVisit 
  	set visitid [ $visit SearchVisitID $patientid $protocolid $visitdate ]
 	$visit SetVisitID $visitid
	tk_messageBox -type ok -message "Visit was added as visitid $visitid" -title "OK" -icon info

    } else {
        tk_messageBox -type ok -message "Visit exists, not add again" -title "Error!" -icon error
    }

}

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::searchVisit {} {

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Please Connect To Server First Before trying to Execute a Query" -title "Error!" -icon error
	return
    }
    
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
    }
    set firstname $thisparam($this,firstname)
    set lastname $thisparam($this,lastname)
    set birthdate $thisparam($this,birthdate)
    set protocolname $thisparam($this,protocolname)
    set pi $thisparam($this,pi)  
    set visitdate $thisparam($this,visitdate)
    set patientid ""
    set visitid ""

    set  sqlstring "select Visit.VisitID, Patient.PatientID, Patient.Firstname, Patient.Lastname, Patient.Birthdate, "
    append sqlstring "Protocol.protocolname, Protocol.Pi, Visit.Visitdate "
    append sqlstring "from Visit,Protocol,Patient "
    append sqlstring "where Visit.patientid=Patient.patientid and Visit.protocolid=Protocol.protocolid "
    append sqlstring "and Patient.firstname like '%"
    append sqlstring $firstname
    append sqlstring "%' "
    append sqlstring "and Patient.lastname like '%"
    append sqlstring $lastname
    append sqlstring "%' "
    append sqlstring "and Patient.birthdate like '%"
    append sqlstring $birthdate
    append sqlstring "%' "
    append sqlstring "and Protocol.protocolname like '%"
    append sqlstring $protocolname
    append sqlstring "%' "
    append sqlstring "and Protocol.pi like '%"
    append sqlstring $pi
    append sqlstring "%' "
    append sqlstring "and Visit.visitdate like '%"
    append sqlstring $visitdate
    append sqlstring "%' "

    $thisparam($this,seriesframe1_querylist) delete 0 end
    set query [ $database RunComplexQuery $sqlstring ]
    set i 0
    while {[$query NextRow]} {
  	set i [expr $i +1 ]
 	set visitid [ $database GetQueryValueAsString $query 0 ] 
 	set patientid [ $database GetQueryValueAsString $query 1 ] 
  	set firstname [ $database GetQueryValueAsString $query 2 ] 
 	set lastname [ $database GetQueryValueAsString $query 3 ] 
 	set birthdate [ $database GetQueryValueAsString $query 4 ] 
 	set protocolname [ $database GetQueryValueAsString $query 5 ] 
  	set pi [ $database GetQueryValueAsString $query 6 ] 
 	set visitdate [ $database GetQueryValueAsString $query 7 ] 
	set record "$i|$visitid|$patientid|$firstname|$lastname|$birthdate|$protocolname|$pi|$visitdate"
	set rf $thisparam($this,seriesframe1_rf)
	$rf.result insert end $record
    }
}

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::selectVisit { } {

    set name [ $thisparam($this,seriesframe1_querylist) getcurselection ]
    set tmp  [ split $name | ]
    set visitid [ lindex $tmp 1 ]
    set patientid [ lindex $tmp 2 ]
    set thisparam($this,visitid) $visitid
    set thisparam($this,patientid) $patientid
    tk_messageBox -type ok -message "You select VisitID $visitid" -title "OK" -icon info
    set thisparam($this,visitid_selected) 1
}

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::addSeries {} {

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Please Connect To Server First Before trying to Execute a Query" -title "Error!" -icon error
	return
    }
    if { $thisparam($this,visitid_selected) == 0 } {
	tk_messageBox -type ok -message "Please Select Visit first" -title "Error!" -icon error
	return
    }
    append imagefile $thisparam($this,seriesdatapath) "/" $thisparam($this,seriesfilename) 
    if {![file exist $imagefile]} {
      tk_messageBox -type ok -message "Imagefile $imagefile not exist" -title "Error!" -icon error
      return
    }

    set thisparam($this,namelist) { { ImagePath seriesdatapath } { ImageFileName seriesfilename} }
    
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	if { $thisparam($this,$field) == "" } {
	    tk_messageBox -type ok -message "Parameter $label not set" -title "Error!" -icon error
	    return
	}
    }

    set visitid $thisparam($this,visitid)
    set patientid $thisparam($this,patientid)
    set seriesdatapath $thisparam($this,seriesdatapath)
    set seriesfilename $thisparam($this,seriesfilename)
    


    set study [ vtkbisStudy New ]
    $study SetConnector $database
    $study SetVisitID $visitid
    $study SetPatientID $patientid
    $study AddStudy 
    set studyid [ $study SearchStudyID $visitid $patientid ]
    $study SetStudyID $studyid
    set studyuid [ $study SearchStudyUID $studyid ]
    $study SetStudyUID $studyuid
    
    set  injection [ vtkbisInjection New ]
    $injection SetConnector $database
    $injection SetVisitID $visitid
    $injection AddInjection 
    set injectionid [ $injection SearchInjectionID $visitid ]
    $injection SetInjectionID $injectionid
    $injection SetInjectionDateTime [ $injection SearchInjectionDateTime $injectionid ]
    
    set series [  vtkbisSeries New ]
    $series SetConnector $database
    $series SetStudyID $studyid
    $series SetInjectionID $injectionid
    $series SetSeriesDataPath $seriesdatapath
    $series SetSeriesFileName $seriesfilename
    set seriesid [  $series SearchSeriesID $studyid $injectionid ]
    $series SetSeriesID $seriesid
    if { [ string length $seriesid ] ==0 } {
	$series AddSeries 
	set seriesid [ $series SearchSeriesID $studyid $injectionid ]
	$series SetSeriesID $seriesid
	$series SetSeriesUID [ $series SearchSeriesUID $seriesid ]
	tk_messageBox -type ok -message "Series was added as seriesid $seriesid" -title "OK" -icon info
    }

}

# ------------------------------------------------------------------------------------
itcl::body bis_databasetool::browse { } {

    set f2tail $thisparam($this,seriesfilename)
    set f2path $thisparam($this,seriesdatapath)

    set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
    set fname  [tk_getOpenFile -title "Get Filename Series" -filetypes $typelist -initialfile $f2tail -initialdir $f2path ]   
    if { [ string length $fname ] < 1 } {
	return 0
    }

    set thisparam($this,seriesdatapath) [ file dirname $fname ]
    set thisparam($this,seriesfilename) [ file tail $fname ]
    return 1
}

# --------------------------------------------------------------------------------------------------------
itcl::body bis_databasetool::searchSavedQuery {} {

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Please Connect To Server First Before trying to Execute a Query" -title "Error!" -icon error
	return
    }
    
    set namelist { { QueryName queryname } { QueryCategory querycategory } }

    foreach elem $namelist {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
    }
    set queryname $thisparam($this,queryname)
    set querycategory $thisparam($this,querycategory)
    set querydescription ""
    set queryarg ""
    set querytable ""
    set querylink ""
    set queryfield ""
    set querywhere ""
    set queryorder ""
    set querygroup ""
    set queryhaving ""

    # Create Database Connector
    
    set  sqlstring "select QueryName, QueryCategory,QueryDescription,QueryArg, QueryTable,"
    append sqlstring "QueryLink,QueryField,QueryWhere,QueryOrder,QueryGroup,QueryHaving "
    append sqlstring "from Query "
    append sqlstring "where queryname like '%"
    append sqlstring $queryname
    append sqlstring "%' "
    append sqlstring "and querycategory like '%"
    append sqlstring $querycategory
    append sqlstring "%' "

    $thisparam($this,savedqueryframe1_querylist) delete 0 end
    set query [ $database RunComplexQuery $sqlstring ]
    set i 0
    while {[$query NextRow]} {
  	set i [expr $i +1 ]
 	set queryname [ $database GetQueryValueAsString $query 0 ] 
  	set querycategory [ $database GetQueryValueAsString $query 1 ] 
 	set querydescription [ $database GetQueryValueAsString $query 2 ] 
 	set queryarg [ $database GetQueryValueAsString $query 3 ] 
 	set querytable [ $database GetQueryValueAsString $query 4 ] 
  	set querylink [ $database GetQueryValueAsString $query 5 ] 
 	set queryfield [ $database GetQueryValueAsString $query 6 ] 
	set querywhere [ $database GetQueryValueAsString $query 7 ] 
  	set queryorder [ $database GetQueryValueAsString $query 8 ] 
 	set querygroup [ $database GetQueryValueAsString $query 9 ] 
 	set queryhaving [ $database GetQueryValueAsString $query 10 ] 
	
	set record "$i|$queryname|$querycategory|$querydescription"
	set rf $thisparam($this,savedqueryframe1_rf)
	$rf.result insert end $record
    }
}

itcl::body bis_databasetool::abbreviation { input } {
 

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Please Connect To Server First Before trying to Execute a Query" -title "Error!" -icon error
	return
    }

    set  sqlstring "select Abbrev, AbbrevTranslation from Abbreviations "

    set query [ $database RunComplexQuery $sqlstring ]
    set i 0
    while {[$query NextRow]} {
 	set tmp [ $database GetQueryValueAsString $query 0 ] 
    	if { [ string index $tmp 0 ] == "$" } {
		set abbrev($i) "\\$tmp"
        } else {
		set abbrev($i) $tmp
	}
	set abbrevTranslation($i) [ $database GetQueryValueAsString $query 1 ] 
	set search($i) $abbrev($i)
	set replace($i) $abbrevTranslation($i)

	set i [expr $i +1 ]
    }

    set delimiter "(^|\\W|$)"
    set separator "~~!~~"
    
    for {set index 0} {$index < [array size search]} {incr index} {
      	set search($index) "$delimiter$search($index)$delimiter"
	set replace($index) "\\1$separator$replace($index)\\2"
    }
    
    set new_text $input
    set old_text ""
    set step 0
    #here, the while loop always runs, why?
    while { [string equal new_text old_text]==0 } {
        #puts "step=$step"
	set step [expr $step +1 ]
	if { $step > 25 } {
	     break
	}
	for {set index 0} {$index < [array size search]} {incr index} {
		set old_text $new_text
		regsub -all -nocase $search($index) $old_text $replace($index) new_text
		regsub -all -nocase $separator $new_text "" new_text
	}
    }	
	    
    return $new_text

}


itcl::body bis_databasetool::beuatifyQuery { inp } {

#    puts stderr "Beginning with $inp"

    regsub -all {[\n\t ]} $inp "" inp
    set ok 1
    while { $ok > 0 } {
	set old $inp
	set ok [ regsub -all {\([^\(\)]*\)} $old "+" inp ]
	#puts stderr "old=$old\t new=$inp\t ok=$ok\n"
    }

    set ok 1
    while { $ok > 0 } {
	set old $inp
	set ok [ regsub -all {\"[^\"]*\"} $old "*" inp ]
	#	puts stderr "old=$old\t new=$inp\t ok=$ok\n"
    }

      #  puts stderr "Final text=$inp"
    return $inp
}

itcl::body bis_databasetool::selectSavedQuery { } {
    

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Please Connect To Server First Before trying to Execute a Query" -title "Error!" -icon error
	return
    }

    set name [ $thisparam($this,savedqueryframe1_querylist) getcurselection ]
    set tmp  [ split $name | ]
    set queryname [ lindex $tmp 1 ]
    set querycategory [ lindex $tmp 2 ]
    set thisparam($this,queryname) $queryname
    set thisparam($this,querycategory) $querycategory
    set querydescription ""
    set queryarg ""
    set querytable ""
    set querylink ""
    set queryfield ""
    set querywhere ""
    set queryorder ""
    set querygroup ""
    set queryhaving ""
    # Create Database Connector
    
    set  sqlstring "select QueryName, QueryCategory,QueryDescription,QueryArg, QueryTable,"
    append sqlstring "QueryLink,QueryField,QueryWhere,QueryOrder,QueryGroup,QueryHaving "
    append sqlstring "from Query "
    append sqlstring "where queryname='"
    append sqlstring $queryname
    append sqlstring "'"
    append sqlstring " and querycategory='"
    append sqlstring $querycategory
    append sqlstring "'"

    set query [ $database RunComplexQuery $sqlstring ]
    set i 0
    while {[$query NextRow]} {
  	set i [expr $i +1 ]
 	set queryname [ $database GetQueryValueAsString $query 0 ] 
  	set querycategory [ $database GetQueryValueAsString $query 1 ] 
 	set querydescription [ $database GetQueryValueAsString $query 2 ] 
 	set queryarg [ $database GetQueryValueAsString $query 3 ] 
 	set querytable [ $database GetQueryValueAsString $query 4 ] 
  	set querylink [ $database GetQueryValueAsString $query 5 ] 
 	set queryfield [ $database GetQueryValueAsString $query 6 ] 
	set querywhere [ $database GetQueryValueAsString $query 7 ] 
  	set queryorder [ $database GetQueryValueAsString $query 8 ] 
 	set querygroup [ $database GetQueryValueAsString $query 9 ] 
 	set queryhaving [ $database GetQueryValueAsString $query 10 ] 
    }

    set querytable [ $this abbreviation $querytable ]
    set querylink [ $this abbreviation $querylink ]
    set queryfield [ $this abbreviation $queryfield ]
    set querywhere [ $this abbreviation $querywhere ]
    set queryorder [ $this abbreviation $queryorder ]
    set querygroup [ $this abbreviation $querygroup ]
    set queryhaving [ $this abbreviation $queryhaving ]

     

    if { [ string first "Series" $querytable ] < 0 } {
	tk_messageBox -type ok -message "You selected a query that is not valid for searching for images" -title "Bad Query" -icon error
	return 0
    }

    set seriespathindex -1
    set seriesnameindex -1

    set fielddivision [ split [ beuatifyQuery $queryfield ] "," ]


    set thisparam($this,queryfieldnumber) [ llength $fielddivision ]

    for { set i 0 } { $i < [ llength $fielddivision ] } { incr i } {
	set qt [ string trim [ lindex $fielddivision $i ]]
	#	puts stderr "Element $i \t $qt"
	if { [ string first "Series.SeriesFileName" $qt ] >=0 } {
	    set seriesnameindex [ expr $i  + 1]
	} elseif { [ string first "Series.SeriesDataPath" $qt ] >= 0 } {
	    set seriespathindex [ expr $i + 1 ]
	}
    }


    if { $seriespathindex == -1 } {
	append queryfield ",Series.SeriesDataPath"
    }
    if { $seriesnameindex == -1 } {
	append queryfield ",Series.SeriesFileName"
    } 


    set fielddivision [ split [ beuatifyQuery $queryfield ] "," ]
    for { set i 0 } { $i < [ llength $fielddivision ] } { incr i } {
	set qt [ string trim [ lindex $fielddivision $i ]]
	#	puts stderr "Element $i \t $qt"
	if { [ string first "Series.SeriesFileName" $qt ] >=0 } {
	    set seriesnameindex [ expr $i  + 1]
	} elseif { [ string first "Series.SeriesDataPath" $qt ] >= 0 } {
	    set seriespathindex [ expr $i + 1 ]
	}
    }

    set thisparam($this,queryfieldnumber) [ llength $fielddivision ]
    tk_messageBox -type ok -message "You selected query $queryname in category $querycategory" -title "OK" -icon info

    set thisparam($this,queryname) $queryname
    set thisparam($this,querycategory) $querycategory
    set thisparam($this,querydescription) $querydescription
    set thisparam($this,queryarg) $queryarg
    set thisparam($this,querytable) $querytable
    set thisparam($this,querylink) $querylink
    set thisparam($this,queryfield) $queryfield
    set thisparam($this,querywhere) $querywhere
    set thisparam($this,queryorder) $queryorder
    set thisparam($this,querygroup) $querygroup
    set thisparam($this,queryhaving) $queryhaving
    set argname [ split $queryarg | ]

    set f $thisparam($this,ontheflyframe) 
#    puts stderr "Removing $f"
    pack forget $f
    destroy $f
    frame $f  
    pack $f -side top -expand true -fill both

    set entrylist ""
    set thisparam($this,ontheflyentries) ""
    
    foreach elem $argname {

      set e [ string trim $elem ]
      #regsub -all  " " $e "_" newelem
      regsub -all  {[^a-zA-Z0-9]} $e "_" newelem
      set label  $e
      set field  parameters_${newelem}
      set thisparam($this,$field) ""
      iwidgets::entryfield $f.$field  -labeltext "${label}:" -width 20 -textvariable [ itcl::scope thisparam($this,$field) ] -relief sunken
      pack $f.$field -side top -expand false -fill x
      set entrylist "$entrylist $f.$field"
      lappend thisparam($this,ontheflyentries) $field
    }
    eval "iwidgets::Labeledwidget::alignlabels $entrylist"
    #puts stderr "vars=$thisparam($this,ontheflyentries)"

    $thisparam($this,queryquerynotebook) view "2. Input arguments"

}


itcl::body bis_databasetool::runSavedQuery { } {

    if { $thisparam($this,connected) == 0 } {
	tk_messageBox -type ok -message "Please Connect To Server First Before trying to Execute a Query" -title "Error!" -icon error
	return
    }

#    foreach elem $thisparam($this,ontheflyentries) {
#	puts stderr "Value of $elem = $thisparam($this,$elem)"
#    }
    # Create Database Connector
    
    set queryname $thisparam($this,queryname)
    set querycategory $thisparam($this,querycategory)
    set p [ vtkbisQuery New ]
    $p SetConnector $database
    set ok [ $p SearchQuery $queryname $querycategory ]
    #$p Print

   # set ttt [ $p GetQueryField ]
    set ttt $thisparam($this,queryfield) 
    $p SetQueryField $ttt
    
    set l ""
    for { set i 0 } { $i < [ llength $thisparam($this,ontheflyentries) ] } { incr i } {
	set pp [ lindex $thisparam($this,ontheflyentries) $i ]
	set q $thisparam($this,$pp)
	if { [ string length $q ] > 0 } {
	    set l "${l}${q}|"
	} else {
	    set l "${l}*|"
	}
    }
    set sql [ $p BuildQuery $l]
    set sql [ $this abbreviation $sql ]
   # puts $sql
    set query [ $database RunComplexQuery $sql ]
    
    $thisparam($this,savequeryframe2_querylist) delete 0 end
    set ok 1
    set i  0
    while { $ok !=0 } {
	set ok [ $query NextRow ]
	if { $ok > 0 } {
	    set line ""
	    incr i
	    for { set j 0 } { $j < $thisparam($this,queryfieldnumber) } { incr j } {
		set line "$line|[ $database GetQueryValueAsString $query $j ]"
	    }
	    set record "$i$line"
	    #	    puts stderr $record
	    set rf $thisparam($this,savequeryframe2_rf)
	    $rf.result insert end $record
	}
    }
}

# --------------------------------------------------------------------------------------------------------
itcl::body bis_databasetool::CreateServerGUI { pw } {

    set serverframe  [ LabelFrame:create $pw.a -text "My SQL Server"]
    set sqliteframe  [ LabelFrame:create $pw.b -text "SQLite File"]

    checkbutton $pw.c -variable [ itcl::scope thisparam($this,debugdbase)  ] -text "Debug SQL" 

    pack $pw.c -side bottom -expand false -fill x



    set fr [ frame ${serverframe}.info ]
    set thisparam($this,namelist) { { Username username } { Password password } { Host host }
	{ Database database } { Port port } }
    
    set entrylist ""
    foreach elem $thisparam($this,namelist) {
	
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	
	iwidgets::entryfield $fr.$field  -labeltext "${label}:" -width 20 -textvariable [ itcl::scope thisparam($this,$field) ] -relief sunken
	pack $fr.$field -side top -expand false -fill x -padx 10
	
	if { $field == "password" } {
	    $fr.$field configure -show "*"
	}
	set entrylist "$entrylist $fr.$field"
    }
    
    eval "iwidgets::Labeledwidget::alignlabels $entrylist"
    
    set bf [ frame ${serverframe}.bf ]
    
    eval "button $bf.login -text \"Connect to Database\" -command { $this MySQLConnect }"
    eval "button $bf.cancel -text \"Disconnect\"  -command { $this Disconnect }"
    pack $bf.login $bf.cancel -side left
    
    pack $fr $bf -side top -expand true -fill both


    set db 0
    catch { 
	set db [ vtkbisMySQLDatabase New ]
	$db Delete
    }
    
    if { $db == 0  } {
#	puts stderr "Eliminating MYSQL Stuff"
	pack $pw.b -side top -expand true -fill x
		
    } else {
	pack $pw.b -side bottom -expand true -fill x
	pack $pw.a  -side top -expand true -fill both
    }




    eval "button $sqliteframe.3 -text \"Disconnect\"  -command { $this Disconnect }"
    eval "button $sqliteframe.2 -text \"Connect to File\" -command { $this SQLiteConnect }"
    eval "button $sqliteframe.1 -text \"Create New File\" -command { $this SQLiteCreate }"

    pack $sqliteframe.3 -side right -expand false -padx 2
    pack $sqliteframe.2 $sqliteframe.1 -side left -expand true -fill x -padx 2
}
# --------------------------------------------------------------------------------------------------------
itcl::body bis_databasetool::CreateQueryGUI { optframe } {

    set fr [ frame ${optframe}.info ]
    
    set thisparam($this,namelist) { { FirstName firstname } { LastName lastname } { BirthDate birthdate }
	{ ProtocolName protocolname } { Pi pi } { VisitDate visitdate } { ImagePath seriesdatapath }
	{ ImageFileName seriesfilename} }
    
    set entrylist ""
    
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	
	iwidgets::entryfield $fr.$field  -labeltext "${label}:" -width 20 -textvariable [ itcl::scope thisparam($this,$field) ] -relief sunken
	pack $fr.$field -side top -expand false -padx 10 -fill x
	
	set entrylist "$entrylist $fr.$field"
    }

    # Eliminate this at some point
    # ----------------------------
    set thisparam($this,firstname) "John"
    set thisparam($this,lastname) "Smith"
    set thisparam($this,birthdate) "1990-01-01"
    set thisparam($this,visitdate) "2008-08-20"
    # ----------------------------
    
    eval "iwidgets::Labeledwidget::alignlabels $entrylist"
    
    set bf [ frame ${optframe}.bf ]
    
    eval "button $bf.search -text \"Search for Image\" -command { $this searchImage }"
    eval "button $bf.load -text \"Load Image\" -command { $this loadImage }"
    pack $bf.search $bf.load -side left
    set rf [ frame ${optframe}.rf ]
    set thisparam($this,optframe_querylist) [ iwidgets::scrolledlistbox $rf.result  ]
    pack $rf.result -side left -expand true -fill both
    
    pack $fr $rf $bf -side top -expand true -fill both
    set thisparam($this,optframe_rf) $rf
}
# --------------------------------------------------------------------------------------------------------
itcl::body bis_databasetool::CreatePatientGUI { patientframe } {

    set fr [ frame ${patientframe}.info ]
    
    set thisparam($this,namelist) { { FirstName firstname } { LastName lastname } { BirthDate birthdate } }
    set entrylist ""
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	iwidgets::entryfield $fr.$field  -labeltext "${label}:" -width 20 -textvariable [ itcl::scope thisparam($this,$field) ] -relief sunken
	pack $fr.$field -side top -expand false -fill x
	set entrylist "$entrylist $fr.$field"
    }
    eval "iwidgets::Labeledwidget::alignlabels $entrylist"

    set bf [ frame ${patientframe}.bf ]
    
    eval "button $bf.add -text Add -command { $this addPatient }"
    eval "button $bf.clear -text Clear -command { $this clearPatient }"
    pack $bf.add $bf.clear -side left
    pack $fr $bf -side top -expand true -fill both
    
}

# --------------------------------------------------------------------------------------------------------
itcl::body bis_databasetool::CreateProtocolGUI { protocolframe } {

    set fr [ frame ${protocolframe}.info ]
    
    set thisparam($this,namelist) { { ProtocolName protocolname } { PI pi } }
    set entrylist ""
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	iwidgets::entryfield $fr.$field  -labeltext "${label}:" -width 20 -textvariable [ itcl::scope thisparam($this,$field) ] -relief sunken
	pack $fr.$field -side top -expand false -fill x
	set entrylist "$entrylist $fr.$field"
    }
    
    eval "iwidgets::Labeledwidget::alignlabels $entrylist"
    
    set bf [ frame ${protocolframe}.bf ]
    
    eval "button $bf.add -text Add -command { $this addProtocol }"
    eval "button $bf.clear -text Clear -command { $this clearProtocol }"
    pack $bf.add $bf.clear -side left
    pack $fr $bf -side top -expand true -fill both
}

# --------------------------------------------------------------------------------------------------------
itcl::body bis_databasetool::CreateVisitGUI { visitframe } {

    set nt [ iwidgets::tabnotebook $visitframe.1  -tabpos n ]

    # 1. Identify Patient
    set visitframe1 [ $nt add -label "1. Identify Patient" ]
    set fr [ frame ${visitframe1}.info ]
    set thisparam($this,namelist) { { FirstName firstname } { LastName lastname } { BirthDate birthdate } }
    set entrylist ""
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	iwidgets::entryfield $fr.$field  -labeltext "${label}:" -width 20 -textvariable [ itcl::scope thisparam($this,$field) ] -relief sunken
	pack $fr.$field -side top -expand false -fill x
	set entrylist "$entrylist $fr.$field"
    }
    eval "iwidgets::Labeledwidget::alignlabels $entrylist"
    set rf [ frame ${visitframe1}.rf ]
    set thisparam($this,visitframe1_querylist) [ iwidgets::scrolledlistbox $rf.result  ]
    pack $rf.result -side left -expand true -fill both
    set bf [ frame ${visitframe1}.bf ]
    eval "button $bf.search -text Search -command { $this searchPatient }"
    eval "button $bf.select -text Select -command { $this selectPatient }"
    pack $bf.search $bf.select -side left
    pack $fr $rf $bf -side top -expand true -fill both
    set thisparam($this,visitframe1_rf) $rf
    set thisparam($this,patientid_selected) 0

    # 2. Identify Protocol
    set visitframe2 [ $nt add -label "2. Identify Protocol" ]
    set fr [ frame ${visitframe2}.info ]
    set thisparam($this,namelist) { { ProtocolName protocolname } { PI pi } }
    set entrylist ""
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	iwidgets::entryfield $fr.$field  -labeltext "${label}:" -width 20 -textvariable [ itcl::scope thisparam($this,$field) ] -relief sunken
	pack $fr.$field -side top -expand false -fill x
	set entrylist "$entrylist $fr.$field"
    }
    eval "iwidgets::Labeledwidget::alignlabels $entrylist"
    set rf [ frame ${visitframe2}.rf ]
    set thisparam($this,visitframe2_querylist) [ iwidgets::scrolledlistbox $rf.result  ]
    pack $rf.result -side left -expand true -fill both
    set bf [ frame ${visitframe2}.bf ]
    eval "button $bf.search -text Search -command { $this searchProtocol }"
    eval "button $bf.select -text Select -command { $this selectProtocol }"
    pack $bf.search $bf.select -side left
    pack $fr $rf $bf -side top -expand true -fill both
    set thisparam($this,visitframe2_rf) $rf
    set thisparam($this,protocolid_selected) 0
    
    # 3. Add Visit

    set visitframe3 [ $nt add -label "3. Add Visit" ]
    set fr [ frame ${visitframe3}.info ]
    
    set thisparam($this,namelist) { {VisitDate visitdate } }
    set entrylist ""
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	iwidgets::entryfield $fr.$field  -labeltext "${label}:" -width 20 -textvariable [ itcl::scope thisparam($this,$field) ] -relief sunken
	pack $fr.$field -side top -expand false -fill x
	set entrylist "$entrylist $fr.$field"
    }
    eval "iwidgets::Labeledwidget::alignlabels $entrylist"
    set bf [ frame ${visitframe3}.bf ]
    eval "button $bf.add -text Add -command { $this addVisit }"
    pack $bf.add -side left
    pack $fr $bf -side top -expand true -fill both
    
    $nt view "1. Identify Patient"
    pack $nt -side top -expand true -padx 4  -fill both
    

}
# --------------------------------------------------------------------------------------------------------
itcl::body bis_databasetool::CreateSeriesGUI { seriesframe } {

    set nt [ iwidgets::tabnotebook $seriesframe.1  -tabpos n ]
    set seriesframe1 [ $nt add -label "1. Identify Visit" ]
    set fr [ frame ${seriesframe1}.info ]
    set thisparam($this,namelist) { { FirstName firstname } { LastName lastname } { BirthDate birthdate }
	{ ProtocolName protocolname } { PI pi } {VisitDate visitdate} }
    set entrylist ""
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	iwidgets::entryfield $fr.$field  -labeltext "${label}:" -width 20 -textvariable [ itcl::scope thisparam($this,$field) ] -relief sunken
	pack $fr.$field -side top -expand false -fill x
	set entrylist "$entrylist $fr.$field"
    }
    eval "iwidgets::Labeledwidget::alignlabels $entrylist"
    set rf [ frame ${seriesframe1}.rf ]
    set thisparam($this,seriesframe1_querylist) [ iwidgets::scrolledlistbox $rf.result  ]
    pack $rf.result -side left -expand true -fill both
    set bf [ frame ${seriesframe1}.bf ]
    eval "button $bf.search -text Search -command { $this searchVisit }"
    eval "button $bf.select -text Select -command { $this selectVisit }"

    pack $bf.search $bf.select -side left
    pack $fr $rf $bf -side top -expand true -fill both
    set thisparam($this,seriesframe1_rf) $rf
    set thisparam($this,visitid_selected) 0
    
    set seriesframe2 [ $nt add -label "2. Add Series" ]
    set fr [ frame ${seriesframe2}.info ]
    
    set thisparam($this,namelist) { {SeriesDataPath seriesdatapath } {SeriesFileName seriesfilename} }
    set entrylist ""
    foreach elem $thisparam($this,namelist) {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	iwidgets::entryfield $fr.$field  -labeltext "${label}:" -width 20 -textvariable [ itcl::scope thisparam($this,$field) ] -relief sunken
	pack $fr.$field -side top -expand false -fill x
	set entrylist "$entrylist $fr.$field"
    }
    eval "iwidgets::Labeledwidget::alignlabels $entrylist"
    set bf [ frame ${seriesframe2}.bf ]

    eval "button $bf.add -text Add -command { $this addSeries }"
    eval "button $bf.browse -text Browse -command { $this browse }"

    pack $bf.add $bf.browse -side left
    pack $fr $bf -side top -expand true -fill both
    
    $nt view "1. Identify Visit"
    pack $nt -side top -expand true -padx 4  -fill both
}

# --------------------------------------------------------------------------------------------------------
itcl::body bis_databasetool::CreateQueryQueryGUI { qframe } {


    set nt [ iwidgets::tabnotebook $qframe.1  -tabpos n ]

    set thisparam($this,queryquerynotebook) $nt

    set savedqueryframe1 [ $nt add -label "1. Identify Saved Query" ]
    set fr [ frame ${savedqueryframe1}.info ]
    set namelist { { QueryName queryname } { QueryCategory querycategory } }
    set entrylist ""
    foreach elem $namelist {
	set label [ lindex $elem 0 ]
	set field [ lindex $elem 1 ]
	iwidgets::entryfield $fr.$field  -labeltext "${label}:" -width 20 -textvariable [ itcl::scope thisparam($this,$field) ] -relief sunken
	pack $fr.$field -side top -expand false -fill x
	set entrylist "$entrylist $fr.$field"
    }
    eval "iwidgets::Labeledwidget::alignlabels $entrylist"
    set rf [ frame ${savedqueryframe1}.rf ]
    set thisparam($this,savedqueryframe1_querylist) [ iwidgets::scrolledlistbox $rf.result  ]
    pack $rf.result -side left -expand true -fill both
    set bf [ frame ${savedqueryframe1}.bf ]
    eval "button $bf.search -text \"Execute Query\" -command { $this searchSavedQuery }"
    eval "button $bf.select -text \"Select\" -command { $this selectSavedQuery }"
    pack $bf.search $bf.select -side left
    pack $fr $rf $bf -side top -expand true -fill both
    set thisparam($this,savedqueryframe1_rf) $rf
    set thisparam($this,visitid_selected) 0

    set savedqueryframe2 [ $nt add -label "2. Input arguments" ]
    set fr [ frame ${savedqueryframe2}.info ]
    set bf [ frame ${savedqueryframe2}.bf ]
    eval "button $bf.run -text \"Execute Query\" -command { $this runSavedQuery }"
    eval "button $bf.load -text \"Load Image\" -command { $this loadImage2 }"

    pack $bf.run $bf.load -side left
    pack $fr $bf -side top -expand false -fill x
    
    set ontheflyframe [ frame $savedqueryframe2.tt  ]
    pack $ontheflyframe -side top -expand true -fill both

    set thisparam($this,ontheflyframe) $ontheflyframe
    
    set rf [ frame ${savedqueryframe2}.rf ]
    set thisparam($this,savequeryframe2_querylist) [ iwidgets::scrolledlistbox $rf.result  ]
    pack $rf.result -side left -expand true -fill both
    pack $rf -side top -expand true -fill both
    set thisparam($this,savequeryframe2_rf) $rf
    

    $nt view "1. Identify Saved Query"
    pack $nt -side top -expand true -padx 4  -fill both
}
# --------------------------------------------------------------------------------------------------------

itcl::body bis_databasetool::InitializeGUI { base { standal 0 } } {

    if { $initialized == 1 } {
	return $basewidget
    }

    set standalone [ expr $standal > 0 ]
    set basewidget $base
    toplevel $basewidget
    wm withdraw $basewidget

    wm title $basewidget "Search information from database"



    set buttonbar [ frame $basewidget.0 ]
    pack $buttonbar -side bottom -expand false -fill x -padx 2 -pady 2
    


    if { $standalone > 0 } {
        button $buttonbar.quit -text Quit -command pxtkexit
    } else {
	eval "button $buttonbar.quit -text Close -command { wm withdraw $basewidget }"
    }
    eval "button $buttonbar.reset -text Reset -command { $this ResetValues }"

    pack $buttonbar.quit $buttonbar.reset -side right -expand false -padx 5 

    

    entry $buttonbar.st -width 30  -textvariable [ itcl::scope thisparam($this,connectionstatus) ] -relief sunken  -bg black -fg white 
    pack $buttonbar.st -side left -expand true -fill x

    # ------------------------------------------------
    
    set notebook [ iwidgets::tabnotebook $basewidget.1  -tabpos w ]
    pack $notebook -side top -expand true -fill both -padx 2 -pady 2
    
    $this CreateServerGUI [ $notebook add -label "Server" ]
    $notebook view "Server"


    $this CreateQueryQueryGUI [ $notebook add -label "Saved Query" ]

    $this CreateQueryGUI  [ $notebook add -label "Query" ]
    if { $standalone > 0 } {
	set viewframe [ $notebook add -label "Viewer" ]
	set orthoviewer [ vtkpxGUIOrthogonalViewer [ pxvtable::vnewobj ]]
	$orthoviewer Initialize $viewframe 1
	wm geometry $basewidget 600x500
    } else  {
	wm geometry $basewidget 600x450
    }


    $this CreatePatientGUI  [ $notebook add -label "Add Patient" ]
    $this CreateProtocolGUI [ $notebook add -label "Add Protocol" ]
    $this CreateVisitGUI    [ $notebook add -label "Add Visit" ]
    $this CreateSeriesGUI   [ $notebook add -label "Add Series" ]


    wm title $basewidget "BioImage Suite::Database Control $thisparam($this,connectionstatus)"


}

# ----------------------------------------------------------------------------------------
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    wm withdraw .

    set br  .[ pxvtable::vnewobj ]
    set element [bis_databasetool \#auto 0 ]
    update idletasks
    $element InitializeGUI $br 1
    $element ShowWindow

    if { [ llength $argv ] > 0 } {
	$element SQLiteConnect [ lindex $argv 0 ]
    }
}
    

