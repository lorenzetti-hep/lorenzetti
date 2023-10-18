
file(GLOB SOURCES src/*.cxx src/helper/*.cxx )
file(GLOB_RECURSE HEADERS CaloHit/*.h CaloHit/helper/*.h)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCell)

ROOT_GENERATE_DICTIONARY(CaloHitDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE CaloHit)
                                      
add_library(CaloHit  OBJECT ${SOURCES} CaloHitDict.cxx)
install(FILES ${HEADERS}  DESTINATION CaloHit)



#gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python CaloHit)









