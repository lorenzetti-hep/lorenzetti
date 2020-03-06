
file(GLOB SOURCES Root/*.cxx)
file(GLOB_RECURSE HEADERS CaloRec/*.h )

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloCluster)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/EventInfo)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/Truth)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../external/cpk/include)



ROOT_GENERATE_DICTIONARY(CaloRecDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/Root/LinkDef.h  MODULE CaloRec)
                                      
add_library(CaloRec  OBJECT ${SOURCES} CaloRecDict.cxx)
install(FILES ${HEADERS}  DESTINATION CaloRec)










