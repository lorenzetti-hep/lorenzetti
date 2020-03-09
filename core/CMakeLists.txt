
file(GLOB SOURCES Root/*.cxx Root/geant/*.cxx Root/reader/*.cxx)
file(GLOB_RECURSE HEADERS core/*.h core/geant/*.h core/reader/*.h)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../events/CaloCluster)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../events/CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../events/EventInfo)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../events/TruthParticle)



ROOT_GENERATE_DICTIONARY(coreDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/Root/LinkDef.h  MODULE core)
                                      
add_library(core  OBJECT ${SOURCES} coreDict.cxx)
install(FILES ${HEADERS}  DESTINATION core)










