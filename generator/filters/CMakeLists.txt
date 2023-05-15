
file(GLOB SOURCES src/*.cxx )
file(GLOB_RECURSE HEADERS 
                        src/Zee.h
                        src/JF17.h
                        src/Pileup.h
                        src/FixedRegion.h
                        src/SingleParticle.h
                        src/BoostedEvents.h
                          )


include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/G4Kernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../genkernel)

# included by hand, should be fixed for future
include_directories( ${FASTJET_INCLUDE_DIRS} )
include_directories( ${PYTHIA8_INCLUDE_DIRS} )
include_directories( ${HEPMC_INCLUDE_DIRS} )

ROOT_GENERATE_DICTIONARY(filtersDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE filters)
                                      
add_library(filters  OBJECT ${SOURCES} filtersDict.cxx)
install(FILES ${HEADERS}  DESTINATION filters)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python filters)







