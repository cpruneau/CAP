################################################################################################
# Project CAP/Urqmd
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Urqmd UrqmdEventReader.hpp LINKDEF UrqmdLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(Urqmd SHARED UrqmdEventReader.cpp G__Urqmd.cxx)
target_link_libraries(Urqmd Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Urqmd  PUBLIC Base  Urqmd ${EXTRA_INCLUDES})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libUrqmd.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libUrqmd_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Urqmd  LIBRARY DESTINATION "$ENV{CAP_LIB}")

