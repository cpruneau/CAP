################################################################################################
# Project CAP/Base
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__Base Timer.hpp IdentifiedObject.hpp  Configuration.hpp ConfigurationManager.hpp VectorField.hpp Parser.hpp TextParser.hpp XmlParser.hpp XmlDocument.hpp XmlVectorField.hpp Factory.hpp Filter.hpp Collection.hpp   HistogramCollection.hpp HistogramGroup.hpp HistogramManager.hpp RandomGenerators.hpp Task.hpp TaskIterator.hpp  MessageLogger.hpp StateManager.hpp    SelectionGenerator.hpp   DerivedHistoIterator.hpp
LINKDEF BaseLinkDef.h)  

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Base SHARED Exceptions.cpp PhysicsConstants.cpp Timer.cpp Crc32.cpp IdentifiedObject.cpp NameManager.cpp Configuration.cpp ConfigurationManager.cpp VectorField.cpp  Parser.cpp  TextParser.cpp XmlParser.cpp XmlDocument.cpp  XmlVectorField.cpp  Factory.cpp HistogramCollection.cpp  HistogramGroup.cpp  HistogramManager.cpp  RandomGenerators.cpp  Task.cpp TaskIterator.cpp MessageLogger.cpp StateManager.cpp     SelectionGenerator.cpp     DerivedHistoIterator.cpp
 G__Base.cxx)
#BidimGaussFitResult.cpp BidimGaussFitConfiguration.cpp BidimGaussFitter.cpp

target_link_libraries(Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Base  PUBLIC Base Math ${EXTRA_INCLUDES} )

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libBase.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libBase_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Base  LIBRARY DESTINATION "$ENV{CAP_LIB}")

