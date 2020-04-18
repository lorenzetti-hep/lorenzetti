
file(GLOB SOURCES src/*.cxx)
file(GLOB_RECURSE HEADERS src/CaloNtupleMaker.h )

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloCluster)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/EventInfo)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/TruthParticle)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloRings)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)



ROOT_GENERATE_DICTIONARY(CaloNtupleDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE CaloNtuple)
                                      
add_library(CaloNtuple  OBJECT ${SOURCES} CaloNtupleDict.cxx)
install(FILES ${HEADERS}  DESTINATION CaloNtuple)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python CaloNtuple)







