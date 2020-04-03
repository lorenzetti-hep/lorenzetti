
file(GLOB SOURCES src/*.cxx)
file(GLOB_RECURSE HEADERS G4Kernel/*.h )
include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../GaugiKernel)

ROOT_GENERATE_DICTIONARY(G4KernelDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE G4Kernel)
                                      
add_library(G4Kernel  OBJECT ${SOURCES} G4KernelDict.cxx)
install(FILES ${HEADERS}  DESTINATION G4Kernel)










