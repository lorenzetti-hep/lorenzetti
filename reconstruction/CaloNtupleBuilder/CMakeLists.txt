
file(GLOB SOURCES src/*.cxx)
file(GLOB_RECURSE HEADERS src/CaloNtupleMaker.h src/RawNtupleMaker.h)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloCluster)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/EventInfo)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/TruthParticle)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/CaloRings)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/G4Kernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../external/cpk/include)



ROOT_GENERATE_DICTIONARY(CaloNtupleBuilderDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE CaloNtupleBuilder)

add_library(CaloNtupleBuilder  OBJECT ${SOURCES} CaloNtupleBuilderDict.cxx)
install(FILES ${HEADERS}  DESTINATION CaloNtupleBuilder)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python CaloNtupleBuilder)







