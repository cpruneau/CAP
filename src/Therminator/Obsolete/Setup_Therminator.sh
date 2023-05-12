#!/bin/bash

####################################################################################################
#
# Use this file to setup envirionment variable to run THERMINATOR applications
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
#
####################################################################################################
echo "========================================================================================"
echo "Creating (if needed) build, InputFiles, and OutputFiles directories"
if [[ ! (-d build) ]]
then
  mkdir build;
fi

echo "========================================================================================"
echo "Setting up THERMINATOR environment variables"
echo "========================================================================================"
export THERMINATOR_ROOT=`pwd`
export THERMINATOR_SRC="$THERMINATOR_ROOT/src/"
export THERMINATOR_BIN="$THERMINATOR_ROOT/bin/"
export THERMINATOR_LIB="$THERMINATOR_ROOT/lib/"
export THERMINATOR_DATA="$THERMINATOR_ROOT/data/"
export THERMINATOR_INPUT_PATH="$THERMINATOR_DATA/inputHistos/"
export THERMINATOR_OUTPUT_PATH="$THERMINATOR_DATA/outputHistos/"
export THERMINATOR_REPOSITORY_PATH="$THERMINATOR_DATA/repository/"
export THERMINATOR_FIGURES_PATH="$THERMINATOR_DATA/figures/"
export PATH="$THERMINATOR_BIN:$PATH"
export DYLD_LIBRARY_PATH="$THERMINATOR_LIB:$DYLD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$THERMINATOR_LIB:$LD_LIBRARY_PATH"
export THERMINATOR_INPUT_PATH="$THERMINATOR_DATA/InputFiles/"
export THERMINATOR_OUTPUT_PATH="$THERMINATOR_DATA/OutputFiles/"
export THERMINATOR_REPOSITORY_PATH="$THERMINATOR_DATA/Repository/"
echo "All done"
echo "========================================================================================"
