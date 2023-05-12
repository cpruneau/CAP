################################################################################################
# Project CAP/Pythia
################################################################################################

ROOT_GENERATE_DICTIONARY(G__CAPPythia PythiaEventGenerator.hpp PythiaEventReader.hpp LINKDEF CAPPythiaLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(CAPPythia SHARED PythiaEventGenerator.cpp PythiaEventReader.cpp  G__CAPPythia.cxx)
target_link_libraries(CAPPythia  Base  Particles  PYTHIA8_LIB  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(CAPPythia  PUBLIC  Base  Particles CAPPythia $ENV{PYTHIA8}/include" "$ENV{PYTHIA8}/include/Pythia8 ${EXTRA_INCLUDES})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libCAPPythia.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libCAPPythia_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS CAPPythia  LIBRARY DESTINATION "$ENV{CAP_LIB}") 
