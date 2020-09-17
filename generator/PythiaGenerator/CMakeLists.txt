
file(GLOB SOURCES src/*.cxx )
file(GLOB_RECURSE HEADERS src/Pileup.h 
                          src/Zee.h
                          src/JF17.h
                          src/ParticleGun.h
                          src/LLPGun.h
                          src/FixedRegion.h
                          src/BoostedEvents.h
                          )


include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/G4Kernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/P8Kernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../events/EventInfo)

# included by hand, should be fixed for future
include_directories( ${FASTJET_INCLUDE_DIRS} )
include_directories( ${PYTHIA8_INCLUDE_DIRS} )
include_directories( ${HEPMC_INCLUDE_DIRS} )

ROOT_GENERATE_DICTIONARY(PythiaGeneratorDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE PythiaGenerator)
                                      
add_library(PythiaGenerator  OBJECT ${SOURCES} PythiaGeneratorDict.cxx)
install(FILES ${HEADERS}  DESTINATION PythiaGenerator)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python PythiaGenerator)







