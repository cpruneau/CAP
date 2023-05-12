################################################################################################
# Project CAP/ThermalGas
################################################################################################

ROOT_GENERATE_DICTIONARY(G__ThermalGas  ThermalGas.hpp ParticleThermalProperties.hpp ThermalGasHistograms.hpp
ThermalGasVsTempHistograms.hpp ThermalGasModel.hpp  LINKDEF ThermalGasLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic )
add_library(ThermalGas SHARED   ThermalGas.cpp ParticleThermalProperties.cpp ThermalGasHistograms.cpp
ThermalGasVsTempHistograms.cpp ThermalGasModel.cpp   G__ThermalGas.cxx)
target_link_libraries(ThermalGas  Base  Particles   ${ROOT_LIBRARIES} MathMore ${EXTRA_LIBS} )
target_include_directories(ThermalGas  PUBLIC Base  Particles  ThermalGas ${EXTRA_INCLUDES})

################################################################################################
# Install
################################################################################################

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libThermalGas.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libThermalGas_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS ThermalGas  LIBRARY DESTINATION "$ENV{CAP_LIB}")

