################################################################################################
# Project CAP/Global
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Global GlobalAnalyzer.hpp GlobalHistos.hpp GlobalDerivedHistos.hpp    TransverseSpherocityHistos.hpp TransverseSpherocityAnalyzer.hpp  
LINKDEF GlobalLinkDef.h)  


################################################################################################
# Create a shared library with generated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Global SHARED GlobalAnalyzer.cpp GlobalHistos.cpp GlobalDerivedHistos.cpp  TransverseSpherocityHistos.cpp TransverseSpherocityAnalyzer.cpp 
 G__Global.cxx)

target_link_libraries(Global Base  Particles  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Global  PUBLIC Global Base  Particles  ${EXTRA_INCLUDES} )

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libGlobal.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libGlobal_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Global  LIBRARY DESTINATION "$ENV{CAP_LIB}")

