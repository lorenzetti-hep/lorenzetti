



#include_directories(${CMAKE_SOURCE_DIR}/source/global/HEPGeometry/include)

file(GLOB sources 
     ${PROJECT_SOURCE_DIR}/src/*.cxx
  )

file(GLOB headers 
     ${PROJECT_SOURCE_DIR}/include/*.h
     )



#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#add_executable(propagator scripts/propagator.cc ${sources} ${headers})
#target_link_libraries(propagator ${Geant4_LIBRARIES} ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES} ${ROOT_LIBRARIES} )













