#!/bin/bash

DIRECTORY=$1
PRODUCTIONDIRECTORY=$2
NEVENTS=$3

CENTIX=$SLURM_ARRAY_TASK_ID

# Execute application code
source /etc/profile.d/modules.sh
module use /cvmfs/it.gsi.de/modulefiles/
module load root/v6.06-06
export ROOTSYS=/cvmfs/it.gsi.de/root/v6-06-06

root -q -l -b "runUrqmd.c(\"$DIRECTORY/UrQMD_generated.root\",\"$DIRECTORY\",\"$PRODUCTIONDIRECTORY\",$CENTIX,$NEVENTS)";


