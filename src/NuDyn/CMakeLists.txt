################################################################################################
# Project CAP/NuDyn
################################################################################################

ROOT_GENERATE_DICTIONARY(G__NuDyn NuDynAnalyzer.hpp NuDynDerivedHistogramCalculator.hpp NuDynDerivedHistos.hpp NuDynHistos.hpp LINKDEF NuDynLinkDef.h)


################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(NuDyn SHARED NuDynAnalyzer.cpp NuDynDerivedHistogramCalculator.cpp NuDynDerivedHistos.cpp NuDynHistos.cpp G__NuDyn.cxx)

target_link_libraries(NuDyn Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(NuDyn  PUBLIC Base NuDyn ${EXTRA_INCLUDES} ) 

# optimization for big histogram access within the pair inner loop
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libNuDyn.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libNuDyn_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS NuDyn  LIBRARY DESTINATION "$ENV{CAP_LIB}")

