#!/bin/bash

if [ $# -lt 1 ]; then
  echo "usage: batchMergeScannedUrQMD outputdir"
  exit 1
fi


OUTDIR=/lustre/nyx/alice/users/vgonzale/UrQMDparser/$1
if [ -d $OUTDIR ]
then
  echo "-----------------------------------------------------"
  echo " It seems target directory for the merge already     "
  echo " exists. Please, fix it                              "
  echo "-----------------------------------------------------"
  exit 1
fi

mkdir -p $OUTDIR


CENTRALITY='0001 0102 0203 0304 0405 0005 0510 1020 2030 3040 4050 5060 6070 7080'
DATADIRPATTERN=/lustre/nyx/alice/users/vgonzale/MergeUrQMD/QMD201
WORKINGDIRECTORY=/lustre/nyx/alice/users/vgonzale/UrQMDparser/SANDBOX

for CENT in $CENTRALITY
do
  echo submitting merge job for centrality $CENT

  sbatch -J b_mScanUrQMD --workdir=$WORKINGDIRECTORY --mem-per-cpu=8000 --time=02:00:00 -o $OUTDIR/Job_%A.out -e $OUTDIR/Job_%A.err $WORKINGDIRECTORY/mergeScannedUrQMD.sh $OUTDIR $DATADIRPATTERN $CENT

  sleep 2s
done

