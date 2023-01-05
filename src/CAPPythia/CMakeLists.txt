################################################################################################
# Project CAP/Pythia
################################################################################################

ROOT_GENERATE_DICTIONARY(G__CAPPythia PythiaEventGenerator.hpp PythiaEventGeneratorRho.hpp PythiaEventReader.hpp LINKDEF CAPPythiaLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(CAPPythia SHARED PythiaEventGenerator.cpp PythiaEventGeneratorRho.cpp PythiaEventReader.cpp  G__CAPPythia.cxx)
target_link_libraries(CAPPythia  Base PYTHIA8_LIB EGPYTHIA8  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(CAPPythia  PUBLIC  Base CAPPythia ${EXTRA_INCLUDES})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libCAPPythia.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libCAPPythia_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS CAPPythia  LIBRARY DESTINATION "$ENV{CAP_LIB}") 
