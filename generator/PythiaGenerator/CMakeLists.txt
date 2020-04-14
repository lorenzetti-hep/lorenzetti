
file(GLOB SOURCES src/*.cxx)
file(GLOB_RECURSE HEADERS src/EventGenerator.h src/ParticleFilter.h Zee.h)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)


include_directories( /Users/jodafons/.bin/fastjet-3.3.3/include)
include_directories( /Users/jodafons/.bin/pythia8244/include)

ROOT_GENERATE_DICTIONARY(PythiaGeneratorDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE PythiaGenerator)
                                      
add_library(PythiaGenerator  OBJECT ${SOURCES} PythiaGeneratorDict.cxx)
install(FILES ${HEADERS}  DESTINATION PythiaGenerator)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python PythiaGenerator)







