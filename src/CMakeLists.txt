################################################################################################
#
# CMakeLists.txt for CAP package.
#
# Creates  libraries with dictionary and various executables
#
################################################################################################
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

################################################################################################
# Project CAP
################################################################################################
project(CAP VERSION 1.0 DESCRIPTION "Correlation Analysis Package" LANGUAGES CXX)

set(CAP_ROOT "${CAP_SRC}")

#option(USE_ECCENTRICITY "Use eccentricity lib" OFF)

################################################################################################
# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS EG MathCore MathMore RIO Hist Tree Net )
find_library(PYTHIA8_LIB pythia8 PATHS $ENV{PYTHIA8}/lib)
find_library(EGPYTHIA8 EGPythia8)


#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} "Base" "BasicGen" "SubSample" "Cluster" "CollGeom" "HadronGas" "Ampt" "Epos" "Urqmd" "Hijing" "CAPPythia" "Herwig" "Eccentricity" "Blastwave" "Performance" "Global" "ParticleSingle" "ParticlePair" "NuDyn" "Plotting"  "Exec" "$ENV{ROOTSYS}/include" "$ENV{PYTHIA8}/include" "$ENV{PYTHIA8}/include/Pythia8")

add_definitions(${ROOT_CXX_FLAGS})
add_library(EG SHARED IMPORTED)
add_library(EGPYTHIA8 SHARED IMPORTED)
add_library(PYTHIA8_LIB SHARED IMPORTED)

set_target_properties(PYTHIA8_LIB PROPERTIES IMPORTED_LOCATION ${PYTHIA8_LIB})
set_target_properties(EGPYTHIA8 PROPERTIES IMPORTED_LOCATION ${EGPYTHIA8})

add_subdirectory(Base)
add_subdirectory(BasicGen)
add_subdirectory(SubSample)
add_subdirectory(HadronGas)
add_subdirectory(CAPPythia)
add_subdirectory(Global)
add_subdirectory(ParticleSingle)
add_subdirectory(ParticlePair)
add_subdirectory(NuDyn)
add_subdirectory(Performance)
add_subdirectory(CollGeom)
add_subdirectory(Epos)
add_subdirectory(Urqmd)
add_subdirectory(Ampt)
add_subdirectory(Hijing)
add_subdirectory(Herwig)
add_subdirectory(Plotting)
add_subdirectory(Exec)
#add_subdirectory(Blastwave)
#add_subdirectory(LambdaAnalysis)
#add_subdirectory(Music)

