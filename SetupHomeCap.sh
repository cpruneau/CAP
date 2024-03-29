#!/bin/bash

####################################################################################################
#
# Use this file to setup envirionment variable to run CAP applications
#
# The build is controlled by cmake and make. Both are needed for this to work.
#
# The following directory structure is assumed/build
#
#  'pwd'                   : Current directory
#  'pwd'/build             : Where cmake & make will compile&build APPs
#  'pwd'/data              : data read/written by CAP executables and MACROs
#  'pwd'/documentation     : documentation output (DOXYGEN)
#  'pwd'/src               : Source code main directory
#  'pwd'/src/AMPT          : Code to read AMPT events.
#  'pwd'/src/Base          : Base classes of the CAP framework.
#  'pwd'/src/Blastwave     : Classes to simulate Blastwave (depracated, use Base/BWGenerator).
#  'pwd'/src/Cluster       : Original "cluster" code (depracated).
#  'pwd'/src/EOS           : Configuration file for the Hadron Gas module.
#  'pwd'/src/Epos          : Code to read EPOS events.
#  'pwd'/src/HadronGas     : Code to carry out hadron gas calculation and event generation.
#  'pwd'/src/Music         : Code to read MUSIC events.
#  'pwd'/src/StatStudies   : Code to carry various stat studies.
#  'pwd'/src/Urqmd         : Code to read UrQMD events.
#  'pwd'/src/CAPPythia     : Code to read or produce PYTHIA events.
#
####################################################################################################
echo "========================================================================================"
echo "Creating (if needed) build, InputFiles, and OutputFiles directories"
if [[ ! (-d build) ]]
then
  mkdir build;
fi

echo "========================================================================================"
echo "Setting up CAP environment variables"
echo "========================================================================================"
export CAP_ROOT=`pwd`
export CAP_SRC="$CAP_ROOT/src/"
export CAP_BIN="$CAP_ROOT/binHome/"
export CAP_LIB="$CAP_ROOT/libHome/"
export CAP_DATA="/Volumes/ClaudeDisc4/OutputFiles/"
export CAP_CONFIG="$CAP_ROOT/projects/"
export CAP_DATABASE="$CAP_ROOT/DB/"
export PATH="$CAP_BIN:$PATH"
export DYLD_LIBRARY_PATH="$CAP_LIB:$DYLD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$CAP_LIB:$LD_LIBRARY_PATH"
echo "All done"
echo "========================================================================================"
