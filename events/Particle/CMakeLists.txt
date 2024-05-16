
file(GLOB SOURCES src/*.cxx )
file(GLOB_RECURSE HEADERS Particle/*.h)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/G4Kernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCluster)

ROOT_GENERATE_DICTIONARY(ParticleDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE Particle)
                                      
add_library(Particle  OBJECT ${SOURCES} ParticleDict.cxx)
install(FILES ${HEADERS}  DESTINATION Particle)
gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python Particle)
