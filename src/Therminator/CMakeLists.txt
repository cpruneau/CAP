###############################################################################################
# Project CAP/Therminator
###############################################################################################
# CMakeLists.txt for Therminator package imbedded in CAP 3.0.
#
################################################################################################
# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED True)
# configure_file(TutorialConfig.h.in TutorialConfig.h)

ROOT_GENERATE_DICTIONARY(G__Therminator Chemistry.hpp Entropy.hpp  Viscosity.hpp  Pressure.hpp SoundVelocity.hpp Temperature.hpp  Energy.hpp  Thermodynamics.hpp  Model.hpp Model_BWA.hpp Model_BlastWave.hpp  Model_Lhyquid2DBI.hpp  Model_Lhyquid3D.hpp  Model_KrakowSFO.hpp  Model_HadronGas.hpp   Hypersurface.hpp Hypersurface_Lhyquid2D.hpp  Hypersurface_Lhyquid3D.hpp  TherminatorGenerator.hpp
LINKDEF TherminatorLinkDef.h)

add_compile_options(-Wall -Wextra -pedantic)
add_library(Therminator SHARED Chemistry.cpp Entropy.cpp  Viscosity.cpp  Pressure.cpp SoundVelocity.cpp Temperature.cpp  Energy.cpp  Thermodynamics.cpp  Model.cpp Model_BWA.cpp Model_BlastWave.cpp  Model_Lhyquid2DBI.cpp  Model_Lhyquid3D.cpp  Model_KrakowSFO.cpp Model_HadronGas.cpp    Hypersurface.cpp Hypersurface_Lhyquid2D.cpp  Hypersurface_Lhyquid3D.cpp  TherminatorGenerator.cpp   G__Therminator.cxx)
target_link_libraries(Therminator Base Particles  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Therminator  PUBLIC Therminator Base Particles ${EXTRA_INCLUDES} )

#add_executable(TherminatorExe therm2_events.cpp )
#target_include_directories(TherminatorExe  PUBLIC TherminatorLib ${EXTRA_INCLUDES} )
#target_link_libraries(TherminatorExe PUBLIC TherminatorLib ${ROOT_LIBRARIES} ${EXTRA_LIBS} )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libTherminator.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libTherminator_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS Therminator  LIBRARY DESTINATION "$ENV{CAP_LIB}")

## install(TARGETS TherminatorExe  LIBRARY DESTINATION "$ENV{THERMINATOR_EXE}")


