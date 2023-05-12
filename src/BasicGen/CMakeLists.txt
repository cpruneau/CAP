################################################################################################
# Project CAP/BasicGen
################################################################################################

ROOT_GENERATE_DICTIONARY(G__BasicGen GaussianGeneratorTask.hpp RadialBoostHistos.hpp RadialBoostTask.hpp RapidityGenerator.hpp ResonanceGenerator.hpp LINKDEF BasicGenLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(BasicGen SHARED GaussianGeneratorTask.cpp RadialBoostHistos.cpp RadialBoostTask.cpp RapidityGenerator.cpp ResonanceGenerator.cpp G__BasicGen.cxx)
target_link_libraries(BasicGen Base Particles ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(BasicGen  PUBLIC Base Particles BasicGen ${EXTRA_INCLUDES})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libBasicGen.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libBasicGen_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS BasicGen  LIBRARY DESTINATION "$ENV{CAP_LIB}")

