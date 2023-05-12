################################################################################################
# Project CAP/HadronGas
################################################################################################

ROOT_GENERATE_DICTIONARY(G__HadronGas  HadronGas.hpp HadronGasHistograms.hpp 
HadronGasVsTempHistograms.hpp  ChemicalPotential.hpp 
HadronGasGeneratorTask.hpp  LINKDEF HadronGasLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic )
add_library(HadronGas SHARED   HadronGas.cpp HadronGasHistograms.cpp 
HadronGasVsTempHistograms.cpp Arsenal.cpp ChemicalPotential.cpp  Table2D.cpp
HadronGasGeneratorTask.cpp  G__HadronGas.cxx)
target_link_libraries(HadronGas  Base  ${ROOT_LIBRARIES} MathMore ${EXTRA_LIBS} )
target_include_directories(HadronGas  PUBLIC Base HadronGas ${EXTRA_INCLUDES})

################################################################################################
# Install
################################################################################################

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libHadronGas.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libHadronGas_rdict.pcm" DESTINATION "$ENV{CAP_LIB}")
install(TARGETS HadronGas  LIBRARY DESTINATION "$ENV{CAP_LIB}")
#install(TARGETS RunHadronGas DESTINATION "$ENV{CAP_BIN}") 

