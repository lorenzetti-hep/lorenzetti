
file(GLOB SOURCES Root/*.cxx )
file(GLOB_RECURSE HEADERS EventInfo/*.h)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCluster)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Truth)

ROOT_GENERATE_DICTIONARY(EventInfoDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/Root/LinkDef.h  MODULE EventInfo)
                                      
add_library(EventInfo  OBJECT ${SOURCES} EventInfoDict.cxx)
install(FILES ${HEADERS}  DESTINATION EventInfo)










