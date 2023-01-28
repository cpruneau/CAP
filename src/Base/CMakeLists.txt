################################################################################################
# Project CAP/Base
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__Base Timer.hpp Configuration.hpp  Factory.hpp Filter.hpp Collection.hpp   HistogramCollection.hpp HistogramGroup.hpp RandomGenerators.hpp Task.hpp TaskIterator.hpp RootTreeReader.hpp MessageLogger.hpp StateManager.hpp Event.hpp EventProperties.hpp EventFilter.hpp EventCountHistos.hpp     Particle.hpp ParticleDecayMode.hpp ParticleDecayer.hpp ParticleDecayerTask.hpp  ParticleType.hpp  ParticleTypeCollection.hpp ParticleTypeTableLoader.hpp ParticleFilter.hpp   ParticlePairFilter.hpp     Nucleus.hpp  NucleusType.hpp   MomentumGenerator.hpp SelectionGenerator.hpp  ParticleDigit.hpp  DerivedHistoIterator.hpp DerivedHistogramCalculator.hpp
LINKDEF BaseLinkDef.h)  

### tempo: BidimGaussFitResult.hpp BidimGaussFitConfiguration.hpp BidimGaussFitter.hpp

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Base SHARED Timer.cpp NameManager.cpp Configuration.cpp Factory.cpp HistogramCollection.cpp  HistogramGroup.cpp RandomGenerators.cpp  Task.cpp TaskIterator.cpp RootTreeReader.cpp  MessageLogger.cpp StateManager.cpp  Event.cpp EventProperties.cpp EventFilter.cpp   EventCountHistos.cpp      Particle.cpp  ParticleDecayMode.cpp ParticleDecayer.cpp ParticleDecayerTask.cpp   ParticleType.cpp ParticleTypeCollection.cpp ParticleTypeTableLoader.cpp  ParticleFilter.cpp  ParticlePairFilter.cpp   Nucleus.cpp   NucleusType.cpp     MomentumGenerator.cpp  SelectionGenerator.cpp    ParticleDigit.cpp DerivedHistoIterator.cpp  DerivedHistogramCalculator.cpp
 G__Base.cxx)

#BidimGaussFitResult.cpp BidimGaussFitConfiguration.cpp BidimGaussFitter.cpp

target_link_libraries(Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Base  PUBLIC Base ${EXTRA_INCLUDES} ) 

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libBase.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libBase_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Base  LIBRARY DESTINATION "$ENV{CAP_LIB}")

