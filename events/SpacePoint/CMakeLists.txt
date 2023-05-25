
file(GLOB SOURCES src/*.cxx src/helper/*.cxx )
file(GLOB_RECURSE HEADERS SpacePoint/*.h SpacePoint/helper/*.h)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CaloCell)

ROOT_GENERATE_DICTIONARY(SpacePointDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE SpacePoint)
                                      
add_library(SpacePoint  OBJECT ${SOURCES} SpacePointDict.cxx)
install(FILES ${HEADERS}  DESTINATION SpacePoint)



gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python SpacePoint)









