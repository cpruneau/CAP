################################################################################################
# Project CAP/AMPT
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Ampt AmptEventReader.hpp LINKDEF AmptLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(Ampt SHARED AmptEventReader.cpp G__Ampt.cxx)
target_link_libraries(Ampt Base  Particles ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Ampt  PUBLIC Base  Particles  Ampt ${EXTRA_INCLUDES})
##target_include_directories(Ampt  PUBLIC Base Cluster Ampt ${EXTRA_INCLUDES})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libAmpt.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libAmpt_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Ampt  LIBRARY DESTINATION "$ENV{CAP_LIB}") 

