#!/bin/bash
conda init bash
conda activate CProot
echo `which root` 
#export CAP_ROOT="/wsu/home/aa/aa75/aa7526/CAP/"
#export CAP_SRC="$CAP_ROOT/src/"
#export CAP_BIN="$CAP_ROOT/bin/"
#export CAP_LIB="$CAP_ROOT/lib/"
#export CAP_DATA="$CAP_ROOT/data/"
#export CAP_MACROS="$CAP_SRC/Macros/"
#export CAP_Clusters="$CAP_ROOT/Clusters/"
#export CAP_WSUGrid="$CAP_Clusters/WSUGrid/"
#export CAP_INPUT_PATH="$CAP_DATA/InputFiles/"
#export CAP_OUTPUT_PATH="$CAP_WSUGrid"
#export PATH="$CAP_BIN:$PATH"
#export DYLD_LIBRARY_PATH="$CAP_LIB:$DYLD_LIBRARY_PATH"
#export LD_LIBRARY_PATH="$CAP_LIB:$LD_LIBRARY_PATH"
#echo "Environment variables are now defined for CAP"
module load  pythia
TASKIX=$SLURM_ARRAY_TASK_ID
SEED=$(( SLURM_ARRAY_TASK_ID + SLURM_ARRAY_JOB_ID*1000 ))
CAP_HISTOPATH=$CAP_WORKINGDIRECTORY_Output/$(printf "%02d/" $TASKIX )
echo "========================================================================================"
echo SLURM_ARRAY_TASK_ID is  $SLURM_ARRAY_TASK_ID
echo SLURM_ARRAY_JOB_ID is  $SLURM_ARRAY_JOB_ID
echo TASKIX is  $TASKIX
echo SEED is $SEED
echo Will save histogram at $CAP_HISTOPATH
mkdir $CAP_HISTOPATH
echo "Calling root w/ RunAna"
echo "========================================================================================"
root -b "$CAP_MACROS/RunAna.C("\"$CAP_MACROS/$CAP_JOB_CONFIGURATION\"","\"$CAP_HISTOPATH\"",$TASKIX,$SEED)"

