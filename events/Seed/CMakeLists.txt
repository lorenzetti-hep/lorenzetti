
file(GLOB SOURCES src/*.cxx )
file(GLOB_RECURSE HEADERS Seed/*.h)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCluster)

ROOT_GENERATE_DICTIONARY(SeedDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE Seed)
                                      
add_library(Seed  OBJECT ${SOURCES} SeedDict.cxx)
install(FILES ${HEADERS}  DESTINATION Seed)










