#/usr/local
MESSAGE("++ Executing BioImage Suite Postinstall Script")

execute_process(
COMMAND /Users/mjack/src3/bioimagesuite30_src/bisexec
/Users/mjack/src3/bioimagesuite30_src/installtools/bis_fix.tcl /Users/mjack/src3/bioimagesuite30_src/cppfilelist.tcl /Users/mjack/src3/bioimagesuite30_src/tclfilelist.tcl /Users/mjack/src3/bioimagesuite30_src/scriptfiles.txt on
WORKING_DIRECTORY /usr/local)

execute_process(
COMMAND /Users/mjack/src3/bioimagesuite30_src/bin/bis_fixscripts
-install /Users/mjack/src3/bioimagesuite30_src/scriptfiles.txt
WORKING_DIRECTORY /usr/local)

