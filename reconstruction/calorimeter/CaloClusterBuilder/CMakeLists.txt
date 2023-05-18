
file(GLOB SOURCES src/*.cxx )
file(GLOB_RECURSE HEADERS src/C*.h src/ShowerShapes.h )

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../events/CaloCluster)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../events/CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../events/EventInfo)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../events/TruthParticle)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../events/CaloRings)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../core/G4Kernel)



ROOT_GENERATE_DICTIONARY(CaloClusterBuilderDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE CaloClusterBuilder)

add_library(CaloClusterBuilder  OBJECT ${SOURCES} CaloClusterBuilderDict.cxx)
install(FILES ${HEADERS}  DESTINATION CaloClusterBuilder)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python CaloClusterBuilder)







