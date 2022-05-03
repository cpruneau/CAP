################################################################################################
# Project WAC/AMPT
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Ampt AmptEventReader.hpp LINKDEF AmptLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(Ampt SHARED AmptEventReader.cpp G__Ampt.cxx)
target_link_libraries(Ampt Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Ampt  PUBLIC Base Cluster Ampt ${EXTRA_INCLUDES})

add_executable(RunAmptSimulation RunAmptSimulation.cpp)
target_include_directories(RunAmptSimulation PUBLIC Base Ampt ${EXTRA_INCLUDES})
target_link_libraries(RunAmptSimulation Base Ampt ${CMAKE_DL_LIBS})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libAmpt.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libAmpt_rdict.pcm" DESTINATION "$ENV{WAC_LIB}")
install(TARGETS Ampt  LIBRARY DESTINATION "$ENV{WAC_LIB}") 
install(TARGETS RunAmptSimulation DESTINATION "$ENV{WAC_BIN}") 

