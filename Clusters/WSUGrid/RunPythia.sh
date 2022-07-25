#!/bin/bash

####################################################################################################
echo "Setting up WAC"
####################################################################################################
source /home/victorG/PROJECTS/ROOT/root_install/bin/thisroot.sh
export PYTHIA8=/home/victorG/PROJECTS/PYTHIA8/pythia8307
export WAC_ROOT=/home/victorG/PROJECTS/WAC
export WAC_SOURCE="$WAC_ROOT"
export WAC_BIN="$WAC_ROOT/bin"
export WAC_LIB="$WAC_ROOT/lib"

export PATH="$WAC_BIN:$PATH"
export DYLD_LIBRARY_PATH="$WAC_LIB:$PYTHIA8/lib:$DYLD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$WAC_LIB:$PYTHIA8/lib:$LD_LIBRARY_PATH"

TASKIX=$SLURM_ARRAY_TASK_ID
SEED=$(( SLURM_ARRAY_TASK_ID + SLURM_ARRAY_JOB_ID*1000 ))

# Execute application code
RunPythiaSimulationTwoParticlesDiff $TASKIX $SEED

