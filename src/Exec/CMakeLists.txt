################################################################################################
# Project CAP/Exec
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Exec RunAnalysis.hpp RunDerivedCalculation.hpp RunSubsample.hpp  LINKDEF ExecLinkDef.h)

################################################################################################
# Create a shared library with generated dictionary
################################################################################################
add_library(Exec SHARED RunAnalysis.cpp RunDerivedCalculation.cpp RunSubsample.cpp G__Exec.cxx)

target_link_libraries(Exec Base  Particles  Global ParticleSingle  ParticlePair NuDyn Performance SubSample CAPPythia BasicGen  Ampt ThermalGas   Therminator  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Exec  PUBLIC Base  Particles  Exec Global ParticleSingle  ParticlePair NuDyn Performance SubSample CAPPythia   BasicGen  Ampt  HadronGas  Therminator ${EXTRA_INCLUDES})

#target_link_libraries(Exec Base  Particles  Global ParticleSingle  ParticlePair NuDyn Performance SubSample  BasicGen  Ampt  ThermalGas  Therminator  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
#target_include_directories(Exec  PUBLIC Base  Particles  Exec Global ParticleSingle  ParticlePair NuDyn Performance SubSample  BasicGen  Ampt  HadronGas  Therminator ${EXTRA_INCLUDES})


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libExec.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libExec_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Exec  LIBRARY DESTINATION "$ENV{CAP_LIB}") 

