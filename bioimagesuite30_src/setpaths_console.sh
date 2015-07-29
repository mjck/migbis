#Script file to set paths for bioimagesuite

# --------------------------------------------------------------------------------------
# This file is part of the BioImage Suite Software Package. (seee www.bioimagesuite.org)
# --------------------------------------------------------------------------------------
#bis_fix_script_begin
BASE=/Users/mjack/src3/bioimagesuite30_src
#bis_fixscript_end


source ${BASE}/setpaths.sh
alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'
alias ll='ls -l'                              
alias la='ls -A'                              
PS1="\h:\w>"
export PS1

echo ""
echo ""
echo "----------------------"
echo "BioImage Suite Console"
echo "----------------------"
echo ""
echo "BioimageSuite CommandLine applications ready to run. Type bis.tcl to get the starting menu ($BASE)"
echo ""

