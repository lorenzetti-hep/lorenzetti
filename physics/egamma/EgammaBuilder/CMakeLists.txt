
file(GLOB SOURCES src/*.cxx)
file(GLOB_RECURSE HEADERS src/ElectronMaker.h )
file(GLOB_RECURSE HEADERS src/PhotonMaker.h )

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../events/Particle)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../events/CaloCluster)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../events/CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../events/CaloRings)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../core/G4Kernel)



ROOT_GENERATE_DICTIONARY(EgammaBuilderDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE EgammaBuilder)
                                      
add_library(EgammaBuilder  OBJECT ${SOURCES} EgammaBuilderDict.cxx)
install(FILES ${HEADERS}  DESTINATION EgammaBuilder)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python EgammaBuilder)