
file(GLOB SOURCES src/*.cxx )
file(GLOB_RECURSE HEADERS CaloCluster/*.h)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/G4Kernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCell)

ROOT_GENERATE_DICTIONARY(CaloClusterDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE CaloCluster)
                                      
add_library(CaloCluster  OBJECT ${SOURCES} CaloClusterDict.cxx)
install(FILES ${HEADERS}  DESTINATION CaloCluster)
gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python CaloCluster)









