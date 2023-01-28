################################################################################################
# Project CAP/Plotting
################################################################################################
ROOT_GENERATE_DICTIONARY(G__Plotting CanvasCollection.hpp CanvasConfiguration.hpp DataGraph.hpp FunctionCollection.hpp GraphConfiguration.hpp  Plotter.hpp  LegendConfiguration.hpp
LINKDEF PlottingLinkDef.h)

### tempo: BidimGaussFitResult.hpp BidimGaussFitConfiguration.hpp BidimGaussFitter.hpp

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Plotting SHARED CanvasCollection.cpp CanvasConfiguration.cpp  DataGraph.cpp   FunctionCollection.cpp  GraphConfiguration.cpp Plotter.cpp LegendConfiguration.cpp
 G__Plotting.cxx)

#BidimGaussFitResult.cpp BidimGaussFitConfiguration.cpp BidimGaussFitter.cpp

target_link_libraries(Plotting Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Plotting  PUBLIC Plotting Base ${EXTRA_INCLUDES} )

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libPlotting.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libPlotting_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Plotting  LIBRARY DESTINATION "$ENV{CAP_LIB}")

