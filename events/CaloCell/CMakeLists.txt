
file(GLOB SOURCES src/*.cxx )
file(GLOB_RECURSE HEADERS CaloCell/*.h )

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
ROOT_GENERATE_DICTIONARY(CaloCellDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE CaloCell)
                                      
add_library(CaloCell  OBJECT ${SOURCES} CaloCellDict.cxx)
install(FILES ${HEADERS}  DESTINATION CaloCell)



gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python CaloCell)









