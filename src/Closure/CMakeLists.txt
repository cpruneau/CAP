################################################################################################
# Project CAP/SubSample
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__SubSample SubSampleStatCalculator.hpp SubSampleStatIterator.hpp  LINKDEF SubSampleLinkDef.h)  


################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(SubSample SHARED SubSampleStatCalculator.cpp  SubSampleStatIterator.cpp  G__SubSample.cxx)

target_link_libraries(SubSample Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(SubSample  PUBLIC Base SubSample ${EXTRA_INCLUDES} ) 

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libSubSample.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libSubSample_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS SubSample  LIBRARY DESTINATION "$ENV{CAP_LIB}")

