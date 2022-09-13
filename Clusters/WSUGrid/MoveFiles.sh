#!/bin/bash
####################################################################################
# Script MoveFiles
#
# Move files from their production location to a result repository
#
# $1 :  Production Directory
# $2 :  Repository Sub-Directory (where the precious histos are moved to)
# $3 :  whether to erase the production directory after the move
#
# The production OUT#### is assumed to have the canonical structure
# $CAP_ROOT/Clusters/WSUGrid/OUT######
# $CAP_ROOT/Clusters/WSUGrid/OUT######/BUNCH##
# $CAP_ROOT/Clusters/WSUGrid/OUT######/BUNCH##/PairGen.root
# $CAP_ROOT/Clusters/WSUGrid/OUT######/BUNCH##/PartGen.root
# $CAP_ROOT/Clusters/WSUGrid/OUT######/BUNCH##/ (and so on)
# $CAP_ROOT/Clusters/WSUGrid/OUT######/PairGenSum#TO#.root
# $CAP_ROOT/Clusters/WSUGrid/OUT######/PairGenDerivedSum#TO#.root
# $CAP_ROOT/Clusters/WSUGrid/OUT######/PairGenBalFctSum#TO#.root
#
# the Repository Sub-Directory is assumed to be a sub-directory of  $CAP_REPOSITORY
#
####################################################################################


if [ $# -gt 3 ]; then
  echo "usage: ProductionDirectory RepositorySubDirectory OPTION"
  exit 0
fi

if [ $# -lt 2 ]; then
  echo "usage: ProductionDirectory RepositoryDirectory OPTION"
  exit 0
fi

if [ ! -d $CAP_ROOT  ]
then
  echo "The environment variable CAP_ROOT is not defined"
  exit 0
fi

if [ ! -d $CAP_REPOSITORY  ]
then
  echo "The environment variable CAP_REPOSITORY is not defined"
  exit 0
fi


ProductionDirectory=$1
RepositorySubDirectory=$2
Repository="$CAP_REPOSITORY/$RepositorySubDirectory"
echo "ROOT and Script Files will be moved from $ProductionDirectory to $Repository"

if [ ! -d $CAP_REPOSITORY   ]
then
  echo Creating $CAP_REPOSITORY
  mkdir $CAP_REPOSITORY
fi


if [ ! -d $Repository   ]
then
  echo Creating $Repository
  mkdir "$Repository"
fi

cp "$CAP_ROOT"/Clusters/WSUGrid/"$ProductionDirectory"/*.C "$Repository"/
cp "$CAP_ROOT"/Clusters/WSUGrid/"$ProductionDirectory"/*.root "$Repository"/

if [ $# -eq 3 ]; then
if [ $3 -eq 1 ]
then
rm -r -f "$CAP_ROOT"/Clusters/WSUGrid/"$ProductionDirectory"/
fi
fi
