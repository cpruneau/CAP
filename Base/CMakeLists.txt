################################################################################################
# Project CAP/Base
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__Base Timer.hpp CanvasCollection.hpp CanvasConfiguration.hpp Configuration.hpp  EnhancedGraph.hpp DataGraph.hpp Factory.hpp Filter.hpp Collection.hpp  FunctionCollection.hpp GraphConfiguration.hpp HistogramCollection.hpp  Histograms.hpp RandomGenerators.hpp RapidityGenerator.hpp Task.hpp TaskIterator.hpp FileTaskIterator.hpp RootTreeReader.hpp MessageLogger.hpp Plotter.hpp StatStudyHistograms.hpp StatStudyPlotter.hpp StatStudyModel.hpp  Event.hpp EventProperties.hpp EventFilter.hpp EventCountHistos.hpp     Particle.hpp ParticleDecayMode.hpp ParticleDecayer.hpp ParticleDecayerTask.hpp  ParticleType.hpp  ParticleTypeCollection.hpp ParticleFilter.hpp ParticleFilterEfficiency.hpp  ParticleSmearer.hpp      ParticlePairFilter.hpp   NuDynAnalyzer.hpp  NuDynHistos.hpp NuDynDerivedHistos.hpp  NuDynDerivedHistogramCalculator.hpp  NuDynPlotter.hpp  CollisionGeometry.hpp   CollisionGeometryAnalyzer.hpp CollisionGeometryGenerator.hpp CollisionGeometryHistograms.hpp CollisionGeometryGradientHistograms.hpp CollisionGeometryMoments.hpp  Nucleus.hpp NucleusGenerator.hpp NucleonNucleonCollisionGenerator.hpp  RadialBoostHistos.hpp RadialBoostTask.hpp TotalXsection.hpp SubSampleStatCalculator.hpp GlobalAnalyzer.hpp GlobalHistos.hpp BidimGaussFitResult.hpp BidimGaussFitConfiguration.hpp BidimGaussFitter.hpp MomentumGenerator.hpp SelectionGenerator.hpp EventPlaneRandomizerTask.hpp EventVertexRandomizerTask.hpp  DetectorFastSimulatorTask.hpp PartitionCalculator.hpp ParticleHistos.hpp ParticleDerivedHistos.hpp  ParticleAnalyzer.hpp ParticleDerivedHistogramCalculator.hpp ParticlePairCombinedHistos.hpp ParticlePairDerivedHistos.hpp ParticlePairHistos.hpp ParticlePairAnalyzer.hpp  ParticlePairDerivedHistogramCalculator.hpp BalanceFunctionCalculator.hpp ParticleDigit.hpp  ResonanceGenerator.hpp TransverseSpherocityHistos.hpp TransverseSpherocityAnalyzer.hpp
LINKDEF BaseLinkDef.h)


################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Base SHARED Timer.cpp  CanvasCollection.cpp Configuration.cpp CanvasConfiguration.cpp  EnhancedGraph.cpp DataGraph.cpp  Factory.cpp  FunctionCollection.cpp  GraphConfiguration.cpp HistogramCollection.cpp Histograms.cpp RandomGenerators.cpp RapidityGenerator.cpp Task.cpp TaskIterator.cpp FileTaskIterator.cpp RootTreeReader.cpp  MessageLogger.cpp Plotter.cpp  StatStudyHistograms.cpp StatStudyPlotter.cpp StatStudyModel.cpp  Event.cpp EventProperties.cpp EventFilter.cpp   EventCountHistos.cpp      Particle.cpp  ParticleDecayMode.cpp ParticleDecayer.cpp ParticleDecayerTask.cpp   ParticleType.cpp ParticleTypeCollection.cpp  ParticleFilter.cpp ParticleFilterEfficiency.cpp ParticleSmearer.cpp   ParticlePairCombinedHistos.cpp ParticlePairDerivedHistos.cpp ParticlePairFilter.cpp ParticlePairHistos.cpp   NuDynAnalyzer.cpp NuDynHistos.cpp NuDynDerivedHistos.cpp NuDynDerivedHistogramCalculator.cpp   NuDynPlotter.cpp CollisionGeometry.cpp  CollisionGeometryAnalyzer.cpp CollisionGeometryGenerator.cpp CollisionGeometryHistograms.cpp CollisionGeometryGradientHistograms.cpp  CollisionGeometryMoments.cpp  Nucleus.cpp NucleusGenerator.cpp NucleonNucleonCollisionGenerator.cpp   RadialBoostHistos.cpp RadialBoostTask.cpp TotalXsection.cpp SubSampleStatCalculator.cpp GlobalAnalyzer.cpp GlobalHistos.cpp  BidimGaussFitResult.cpp BidimGaussFitConfiguration.cpp BidimGaussFitter.cpp  MomentumGenerator.cpp  SelectionGenerator.cpp EventPlaneRandomizerTask.cpp  EventVertexRandomizerTask.cpp   DetectorFastSimulatorTask.cpp PartitionCalculator.cpp ParticleHistos.cpp ParticleDerivedHistos.cpp  ParticleAnalyzer.cpp ParticleDerivedHistogramCalculator.cpp ParticlePairCombinedHistos.cpp ParticlePairDerivedHistos.cpp ParticlePairHistos.cpp ParticlePairAnalyzer.cpp  ParticlePairDerivedHistogramCalculator.cpp BalanceFunctionCalculator.cpp  ParticleDigit.cpp  ParticleDigit.cpp  ResonanceGenerator.cpp TransverseSpherocityHistos.cpp TransverseSpherocityAnalyzer.cpp G__Base.cxx)

target_link_libraries(Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Base  PUBLIC Base ${EXTRA_INCLUDES} ) 

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libBase.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libBase_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Base  LIBRARY DESTINATION "$ENV{CAP_LIB}")

