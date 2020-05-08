
file(GLOB SOURCES src/*.cxx)
file(GLOB_RECURSE HEADERS src/C*.h src/DetectorATLASConstruction.h )

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/G4Kernel)



ROOT_GENERATE_DICTIONARY(CaloRecDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE CaloRec)
                                      
add_library(CaloRec  OBJECT ${SOURCES} CaloRecDict.cxx)
install(FILES ${HEADERS}  DESTINATION CaloRec)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python CaloRec)







