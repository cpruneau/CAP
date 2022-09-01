################################################################################################
# Project CAP/Exec
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Exec RunAnalysis.hpp LINKDEF ExecLinkDef.h)

################################################################################################
# Create a shared library with generated dictionary
################################################################################################
add_library(Exec SHARED RunAnalysis.cpp G__Exec.cxx)
target_link_libraries(Exec Base Global Particle  Pair NuDyn Performance SubSample CAPPythia Herwig Hijing BasicGen  Ampt Epos HadronGas Urqmd ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Exec  PUBLIC Base Exec Global Particle  Pair NuDyn Performance SubSample CAPPythia Herwig Hijing BasicGen  Ampt Epos HadronGas Urqmd ${EXTRA_INCLUDES})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libExec.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libExec_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Exec  LIBRARY DESTINATION "$ENV{CAP_LIB}") 

