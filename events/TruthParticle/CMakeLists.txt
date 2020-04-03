
file(GLOB SOURCES src/*.cxx )
file(GLOB_RECURSE HEADERS TruthParticle/*.h)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCluster)

ROOT_GENERATE_DICTIONARY(TruthParticleDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE TruthParticle)
                                      
add_library(TruthParticle  OBJECT ${SOURCES} TruthParticleDict.cxx)
install(FILES ${HEADERS}  DESTINATION TruthParticle)










