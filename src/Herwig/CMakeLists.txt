################################################################################################
# Project CAP/Herwig
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Herwig HerwigEventReader.hpp LINKDEF HerwigLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(Herwig SHARED HerwigEventReader.cpp G__Herwig.cxx)
target_link_libraries(Herwig Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Herwig  PUBLIC Base Herwig ${EXTRA_INCLUDES})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libHerwig.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libHerwig_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Herwig  LIBRARY DESTINATION "$ENV{CAP_LIB}") 

