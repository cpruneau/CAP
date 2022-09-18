#!/bin/bash
if [ $# -gt 3 ]; then
  echo "usage: PATH  NMAINJOBS NSUBJOBS"
  exit 0
fi

if [ $# -lt 3 ]; then
  echo "usage: PATH  NMAINJOBS NSUBJOBS"
  exit 0
fi
echo Arg1: $1
echo Arg2: $2

PATH=$1
NMAINJOBS=$2
NSUBJOBS=$3

if [ -d $PATH   ]
then
  echo Attempting clean up of directory $PATH
fi

if [ ! -d $PATH  ]
then
  echo The directory $PATH does not exist!!!!!
  exit 0
fi

for ijob in $(seq 1 $NMAINJOBS)
do
  CAP_WORKINGDIRECTORY=$PATH/$(printf "BUNCH%02d" $ijob)

  for iSubJob in $(seq 1 $NSUBJOBS)
  do
    CAP_WORKINGSUBDIRECTORY=$CAP_WORKINGDIRECTORY/$(printf "Output/%02d/" $iSubJob)
    echo Examining directory $CAP_WORKINGSUBDIRECTORY
    ls -al $CAP_WORKINGSUBDIRECTORY
  done
done
