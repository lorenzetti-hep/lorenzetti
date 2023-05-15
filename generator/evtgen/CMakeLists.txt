
file(GLOB SOURCES src/*.cxx src/hepmc3/*.cxx )
file(GLOB_RECURSE HEADERS 
                        src/Pythia8Gen.h
                        src/hepmc3/RootReader.h
                          )


include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/GaugiKernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core/G4Kernel)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../genkernel)

# included by hand, should be fixed for future
include_directories( ${FASTJET_INCLUDE_DIRS} )
include_directories( ${PYTHIA8_INCLUDE_DIRS} )
include_directories( ${HEPMC_INCLUDE_DIRS} )

ROOT_GENERATE_DICTIONARY(evtgenDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/LinkDef.h  MODULE evtgen)
                                      
add_library(evtgen  OBJECT ${SOURCES} evtgenDict.cxx)
install(FILES ${HEADERS}  DESTINATION evtgen)


gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python evtgen)



# Add executables from share folder
add_executable( gun.exe ${CMAKE_CURRENT_SOURCE_DIR}/share/gun.cxx)
target_link_libraries(gun.exe ${PYTHIA8_LIBRARIES} 
                                            ${CMAKE_DL_LIBS} 
                                            ${HEPMC_LIBRARIES} 
                                            ${FASTJET_LIBRARIES}
                                            ${ROOT_LIBRARIES} 
                                            ${ROOT_COMPONENT_LIBRARIES} )




