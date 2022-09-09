#!/bin/bash
conda init bash
conda activate CProot
echo `which root` 
export CAP_ROOT="/wsu/home/aa/aa75/aa7526/CAP/"
export CAP_SRC="$CAP_ROOT/src/"
export CAP_BIN="$CAP_ROOT/bin/"
export CAP_LIB="$CAP_ROOT/lib/"
export CAP_DATA="$CAP_ROOT/data/"
export PATH="$CAP_BIN:$PATH"
export DYLD_LIBRARY_PATH="$CAP_LIB:$DYLD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$CAP_LIB:$LD_LIBRARY_PATH"
export CAP_INPUT_PATH="$CAP_DATA/InputFiles/"
#export CAP_OUTPUT_PATH="$CAP_DATA/OutputFiles/"
echo "Environment variables are now defined for CAP"
module load  pythia
TASKIX=1 #$SLURM_ARRAY_TASK_ID
SEED=$(( SLURM_ARRAY_TASK_ID + SLURM_ARRAY_JOB_ID*1000 ))
echo "========================================================================================"
echo "Calling root w/ RunAnalysis"
echo "========================================================================================"
root -b "/wsu/home/aa/aa75/aa7526/CAP/src/Macros/RunAna.C($TASKIX,$SEED)"  


