################################################################################################
# Project CAP/CollGeom
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__CollGeom 
CollisionGeometry.hpp CollisionGeometryAnalyzer.hpp CollisionGeometryGenerator.hpp CollisionGeometryHistograms.hpp CollisionGeometryGradientHistograms.hpp CollisionGeometryMoments.hpp NucleusGenerator.hpp NucleonNucleonCollisionGenerator.hpp LINKDEF CollGeomLinkDef.h)  


################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(CollGeom SHARED CollisionGeometry.cpp CollisionGeometryAnalyzer.cpp  CollisionGeometryGenerator.cpp CollisionGeometryHistograms.cpp CollisionGeometryGradientHistograms.cpp 
 CollisionGeometryMoments.cpp NucleusGenerator.cpp NucleonNucleonCollisionGenerator.cpp 
 G__CollGeom.cxx)

target_link_libraries(CollGeom Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(CollGeom  PUBLIC Base CollGeom ${EXTRA_INCLUDES} ) 

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libCollGeom.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libCollGeom_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS CollGeom  LIBRARY DESTINATION "$ENV{CAP_LIB}")

