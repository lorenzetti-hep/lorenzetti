
file(GLOB SOURCES src/*.cxx)
file(GLOB_RECURSE HEADERS src/CaloRingerBuilder.h )

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloCluster)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloRings)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/G4Kernel)



ROOT_GENERATE_DICTIONARY(CaloRingerBuilderDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE CaloRingerBuilder)
                                      
add_library(CaloRingerBuilder  OBJECT ${SOURCES} CaloRingerBuilderDict.cxx)
install(FILES ${HEADERS}  DESTINATION CaloRingerBuilder)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python CaloRingerBuilder)







