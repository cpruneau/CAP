################################################################################################
# Project CAP/Hijing
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Hijing HijingEventReader.hpp LINKDEF HijingLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(Hijing SHARED HijingEventReader.cpp G__Hijing.cxx)
target_link_libraries(Hijing Base  Particles  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Hijing  PUBLIC Base  Particles Hijing ${EXTRA_INCLUDES})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libHijing.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libHijing_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Hijing  LIBRARY DESTINATION "$ENV{CAP_LIB}") 

