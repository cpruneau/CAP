################################################################################################
# Project CAP/Particle
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__Particle ParticleHistos.hpp ParticleDerivedHistos.hpp  ParticleAnalyzer.hpp 
LINKDEF ParticleLinkDef.h)  


################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Particle SHARED  ParticleHistos.cpp ParticleDerivedHistos.cpp  ParticleAnalyzer.cpp   G__Particle.cxx)

target_link_libraries(Particle Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Particle  PUBLIC Base Particle ${EXTRA_INCLUDES} ) 

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libParticle.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libParticle_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Particle  LIBRARY DESTINATION "$ENV{CAP_LIB}")

