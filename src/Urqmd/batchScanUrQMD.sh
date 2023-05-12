#!/bin/bash

if [ $# -gt 1 ]; then
  echo "usage: batchScanUrQMD sourcedatapattern"
  exit 1
fi

if [ $# -lt 1 ]; then
  echo "usage: batchScanUrQMD sourcedatapattern"
  exit 1
fi

DATAPATTERN=$1
WORKINGDIRECTORY=/lustre/nyx/alice/users/vgonzale/UrQMDparser/SANDBOX
NEVENTS=1000000
PRODUCTIONDIRECTORY=OUT`date +%Y%m%d%H%M`

if [ -d $WORKINGDIRECTORY/$PRODUCTIONDIRECTORY ]
then
  echo Production directory $PRODUCTIONDIRECTORY does exist. WAIT AT LEAST ONE MINUTE!!!!
  exit 1	
fi

mkdir $WORKINGDIRECTORY/$PRODUCTIONDIRECTORY
echo Production $PRODUCTIONDIRECTORY

for PATTERN in $DATAPATTERN
do
  for DIRECTORY in /lustre/nyx/alice/users/vgonzale/MergeUrQMD/$PATTERN
  do
    # launching the sub-jobs for the whole set of centralities
    mkdir $DIRECTORY/$PRODUCTIONDIRECTORY

    sbatch -J batch__scanUrQMD --array=0-13 --workdir=$WORKINGDIRECTORY --mem-per-cpu=8000 --time=02:00:00 -o $DIRECTORY/$PRODUCTIONDIRECTORY/Job_%A_%a.out -e $DIRECTORY/$PRODUCTIONDIRECTORY/Job_%A_%a.err /lustre/nyx/alice/users/vgonzale/UrQMDparser/SANDBOX/scanUrQMD.sh $DIRECTORY $PRODUCTIONDIRECTORY $NEVENTS 

    sleep 2s
  done
done

