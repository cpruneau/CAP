################################################################################################
# Project CAP/Blastwave
################################################################################################


ROOT_GENERATE_DICTIONARY(G__Blastwave  BWModelEventGenerator.hpp  LINKDEF BWLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(Blastwave SHARED  BWModelEventGenerator.cpp G__Blastwave.cxx)
target_link_libraries(Blastwave Base  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Blastwave  PUBLIC Base Blastwave ${EXTRA_INCLUDES} ) 

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libBlastwave.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libBlastwave_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Blastwave  LIBRARY DESTINATION "$ENV{CAP_LIB}") 
#install(TARGETS RunBWModelSimulation DESTINATION "$ENV{CAP_BIN}") 

