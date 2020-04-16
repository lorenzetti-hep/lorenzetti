
file(GLOB SOURCES src/*.cxx)
file(GLOB_RECURSE HEADERS src/EventNtupleMaker.h )

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloCluster)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/EventInfo)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/TruthParticle)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloRings)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)



ROOT_GENERATE_DICTIONARY(EventNtupleDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE EventNtuple)
                                      
add_library(EventNtuple  OBJECT ${SOURCES} EventNtupleDict.cxx)
install(FILES ${HEADERS}  DESTINATION EventNtuple)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python EventNtuple)







