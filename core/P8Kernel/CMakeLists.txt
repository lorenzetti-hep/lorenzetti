
file(GLOB SOURCES src/*.cxx)
file(GLOB_RECURSE HEADERS P8Kernel/*.h )


include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../G4Kernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/EventInfo)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/TruthParticle)

# included by hand, should be fixed for future
include_directories( ${FASTJET_INCLUDE_DIRS} )
include_directories( ${PYTHIA8_INCLUDE_DIRS} )
include_directories( ${HEPMC_INCLUDE_DIRS} )

ROOT_GENERATE_DICTIONARY(P8KernelDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE P8Kernel )
                                      
add_library(P8Kernel  OBJECT ${SOURCES} P8KernelDict.cxx)
install(FILES ${HEADERS}  DESTINATION P8Kernel )


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python P8Kernel)







