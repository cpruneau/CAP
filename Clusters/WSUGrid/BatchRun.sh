#!/bin/bash

if [ $# -gt 4 ]; then
  echo "usage: OUTPUT_SUBPATH  JOB_CONFIGURATION NMAINJOBS NSUBJOBS"
  exit 0
fi

if [ $# -lt 4 ]; then
  echo "usage: OUTPUT_SUBPATH  JOB_CONFIGURATION NMAINJOBS NSUBJOBS"
  exit 0
fi
echo Arg1: $1
echo Arg2: $2
echo Arg3: $3
echo Arg4: $4

export CAP_ROOT="/wsu/home/aa/aa75/aa7526/CAP/"
export CAP_SRC="$CAP_ROOT/src/"
export CAP_BIN="$CAP_ROOT/bin/"
export CAP_LIB="$CAP_ROOT/lib/"
export CAP_DATA="$CAP_ROOT/data/"
export CAP_MACROS="$CAP_SRC/Macros/"
export CAP_Clusters="$CAP_ROOT/Clusters/"
export CAP_WSUGrid="$CAP_Clusters/WSUGrid/"
export CAP_INPUT_PATH="$CAP_DATA/InputFiles/"
export CAP_OUTPUT_PATH="$CAP_WSUGrid"
export PATH="$CAP_BIN:$PATH"
export DYLD_LIBRARY_PATH="$CAP_LIB:$DYLD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$CAP_LIB:$LD_LIBRARY_PATH"
echo "Environment variables are now defined for CAP"

CAP_OUTPUT_SUBPATH=$1
CAP_JOB_CONFIGURATION=$2
NMAINJOBS=$3
NSUBJOBS=$4
CAP_PRODUCTION=OUT`date +%Y%m%d%H%M`

if [ -d $CAP_OUTPUT_PATH/$CAP_OUTPUT_SUBPATH/$CAP_PRODUCTION ]
then
  echo Production directory $PRODUCTIONDIRECTORY does exist. WAIT AT LEAST ONE MINUTE!!!!
  exit 1
fi

if [ ! -d $CAP_MACROS/$CAP_JOB_CONFIGURATION]
then
  echo The configuration file $CAP_JOB_CONFIGURATION does not exist!!!!!
  exit 1
fi


mkdir $CAP_OUTPUT_PATH/$CAP_OUTPUT_SUBPATH/$CAP_PRODUCTION
echo Production $CAP_OUTPUT_PATH/$CAP_OUTPUT_SUBPATH/$CAP_PRODUCTION

# let's preserve the configuration
cp $CAP_MACROS/RunAna.C $CAP_OUTPUT_PATH/$CAP_OUTPUT_SUBPATH/$CAP_PRODUCTION
cp $CAP_MACROS/$CAP_JOB_CONFIGURATION  $CAP_OUTPUT_PATH/$CAP_OUTPUT_SUBPATH/$CAP_PRODUCTION

for ijob in $(seq 1 $NMAINJOBS)
do
  # launching the main generation jobs
  export CAP_WORKINGDIRECTORY=$CAP_OUTPUT_PATH/$CAP_OUTPUT_SUBPATH/$CAP_PRODUCTION/$(printf "BUNCH%02d" $ijob)
  export CAP_WORKINGDIRECTORY_Output=$CAP_WORKINGDIRECTORY/Output
  echo Preparing to launch job  $ijob w/  working directory $CAP_WORKINGDIRECTORY w/ output $CAP_WORKINGDIRECTORY_Output
  mkdir -p $CAP_WORKINGDIRECTORY_Output
  sbatch -J batch__CAP -q primary --array=1-$NSUBJOBS --chdir=$CAP_WORKINGDIRECTORY --time=04:00:00 --mem=2G -o $CAP_WORKINGDIRECTORY/Job_%A_%a.out -e $CAP_WORKINGDIRECTORY/Job_%A_%a.err $CAP_WSUGrid/Run.sh
  sleep 2s
done
