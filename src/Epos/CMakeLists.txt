################################################################################################
# Project CAP/Epos
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Epos EposEventReader.hpp LINKDEF EposLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(Epos SHARED EposEventReader.cpp G__Epos.cxx)
target_link_libraries(Epos Base Particles ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Epos  PUBLIC Base  Particles  Cluster Epos ${EXTRA_INCLUDES})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libEpos.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libEpos_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Epos  LIBRARY DESTINATION "$ENV{CAP_LIB}")

