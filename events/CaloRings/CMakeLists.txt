
file(GLOB SOURCES src/*.cxx )
file(GLOB_RECURSE HEADERS CaloRings/*.h)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/G4Kernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCluster)

ROOT_GENERATE_DICTIONARY(CaloRingsDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE CaloRings)
                                      
add_library(CaloRings  OBJECT ${SOURCES} CaloRingsDict.cxx)
install(FILES ${HEADERS}  DESTINATION CaloRings)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python CaloRings)








