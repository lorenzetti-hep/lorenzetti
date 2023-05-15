
file(GLOB SOURCES src/*.cxx )
file(GLOB_RECURSE HEADERS GenKernel/*.h )


include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)

# included by hand, should be fixed for future
include_directories( ${FASTJET_INCLUDE_DIRS} )
include_directories( ${PYTHIA8_INCLUDE_DIRS} )
include_directories( ${HEPMC_INCLUDE_DIRS} )

ROOT_GENERATE_DICTIONARY(GenKernelDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE GenKernel)
                                      
add_library(GenKernel  OBJECT ${SOURCES} GenKernelDict.cxx)
install(FILES ${HEADERS}  DESTINATION GenKernel)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python GenKernel)







