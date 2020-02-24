
file(GLOB SOURCES Root/*.cxx )
file(GLOB_RECURSE HEADERS reader/*.h )

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

ROOT_GENERATE_DICTIONARY(readerDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/Root/LinkDef.h  MODULE reader)
                                      
add_library(reader  OBJECT ${SOURCES} readerDict.cxx)
install(FILES ${HEADERS}  DESTINATION reader)










