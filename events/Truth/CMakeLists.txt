
file(GLOB SOURCES Root/*.cxx )
file(GLOB_RECURSE HEADERS Truth/*.h)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCluster)

ROOT_GENERATE_DICTIONARY(TruthDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/Root/LinkDef.h  MODULE Truth)
                                      
add_library(Truth  OBJECT ${SOURCES} TruthDict.cxx)
install(FILES ${HEADERS}  DESTINATION Truth)










