################################################################################################
# Project CAP/Pair
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__Pair 
ParticlePairDerivedHistos.hpp ParticlePairHistos.hpp ParticlePairAnalyzer.hpp  ParticlePairDerivedHistogramCalculator.hpp BalanceFunctionCalculator.hpp LINKDEF PairLinkDef.h)  


################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Pair SHARED ParticlePairDerivedHistos.cpp ParticlePairHistos.cpp ParticlePairAnalyzer.cpp  ParticlePairDerivedHistogramCalculator.cpp BalanceFunctionCalculator.cpp G__Pair.cxx)

target_link_libraries(Pair Base Particle ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Pair  PUBLIC Base Particle Pair ${EXTRA_INCLUDES} ) 

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libPair.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libPair_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Pair  LIBRARY DESTINATION "$ENV{CAP_LIB}")

