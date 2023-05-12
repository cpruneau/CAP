################################################################################################
# Project CAP/Particles
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__Particles  Event.hpp EventProperties.hpp EventFilter.hpp EventCountHistos.hpp  EventTask.hpp    Particle.hpp ParticleDecayMode.hpp ParticleDecayer.hpp ParticleDecayerTask.hpp  ParticleType.hpp  ParticleDb.hpp ParticleDbManager.hpp ParticleFilter.hpp   ParticlePairFilter.hpp     Nucleus.hpp  NucleusType.hpp   MomentumGenerator.hpp ParticleDigit.hpp  RootTreeReader.hpp
LINKDEF ParticlesLinkDef.h)


################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Particles SHARED  Event.cpp EventProperties.cpp EventFilter.cpp EventCountHistos.cpp   EventTask.cpp     Particle.cpp ParticleDecayMode.cpp ParticleDecayer.cpp ParticleDecayerTask.cpp  ParticleType.cpp  ParticleDb.cpp ParticleDbManager.cpp ParticleFilter.cpp   ParticlePairFilter.cpp
Nucleus.cpp  NucleusType.cpp   MomentumGenerator.cpp ParticleDigit.cpp  RootTreeReader.cpp EventTask.cpp
 G__Particles.cxx)

target_link_libraries(Particles Base  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Particles  PUBLIC Base Particles ${EXTRA_INCLUDES} )

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libParticles.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libParticles_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Particles  LIBRARY DESTINATION "$ENV{CAP_LIB}")

