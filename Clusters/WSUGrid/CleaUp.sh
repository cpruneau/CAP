#!/bin/bash


if [ $# -gt 3 ]; then
  echo "usage: PathToClean NMAINJOBS NSUBJOBS"
  exit 0
fi

if [ $# -lt 3 ]; then
  echo "usage: PathToClean  NMAINJOBS NSUBJOBS"
  exit 0
fi
echo Arg1: $1
echo Arg2: $2
echo Arg2: $3

CAP_PathToClean=$1
NMAINJOBS=$2
NSUBJOBS=$3

if [ -d $CAP_PathToClean   ]
then
  echo Attempting clean up of directory $CAP_PathToClean
fi

if [ ! -d $CAP_PathToClean  ]
then
  echo The directory $CAP_PathToClean does not exist!!!!!
  exit 0
fi

for ijob in $(seq 1 $NMAINJOBS)
do
  CAP_WORKINGDIRECTORY=$CAP_PathToClean/$(printf "BUNCH%02d" $ijob)

  for iSubJob in $(seq 1 $NSUBJOBS)
  do
    CAP_WORKINGSUBDIRECTORY=$CAP_WORKINGDIRECTORY/$(printf "Output/%02d/" $iSubJob)
    echo Examining directory $CAP_WORKINGSUBDIRECTORY
    if [ -e $CAP_WORKINGSUBDIRECTORY/PairGen.root  ]
    then
      echo found file PairGen.root
    fi
    if [ ! -f $CAP_WORKINGSUBDIRECTORY/PairGen.root  ]
    then
      rm $CAP_WORKINGDIRECTORY/Job$(printf "Job*_%d.*" $iSubJob)
      echo would delete $CAP_WORKINGSUBDIRECTORY
    fi
  done
done
