cmake_minimum_required(VERSION 3.0)
project(lorenzetti)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DDROP_CGAL")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# Set by hand pythia and fastjet
set( FASTJET_INCLUDE_DIRS $ENV{FASTJET_INCLUDE})
set( PYTHIA8_INCLUDE_DIRS $ENV{PYTHIA8_INCLUDE})
set( HEPMC_INCLUDE_DIRS $ENV{HEPMC_INCLUDE})
file(GLOB FASTJET_LIBRARIES $ENV{FASTJET_LIBRARIES}/*.dylib $ENV{FASTJET_LIBRARIES}/*.so)
file(GLOB PYTHIA8_LIBRARIES $ENV{PYTHIA8_LIBRARIES}/*.a $ENV{PYTHIA8_LIBRARIES}/*.so)



#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})


#----------------------------------------------------------------------------
# Find ROOT (required package)
find_package(ROOT COMPONENTS EG Eve Geom Gui GuiHtml GenVector Hist Physics Matrix Graf RIO Tree Gpad RGL MathCore)
include(${ROOT_USE_FILE})


#----------------------------------------------------------------------------
include(core/GaugiKernel/cmake/InstallGaugiFunctions.cmake)
gaugi_initialize()




#set( Python_ADDITIONAL_VERSIONS 3.0)
find_package( PythonInterp REQUIRED)
find_package( PythonLibs REQUIRED)
#find_package( Boost COMPONENTS python system filesystem)
#include_directories( ${Boost_INCLUDE_DIR} )
#link_directories( ${Boost_LIBRARY_DIR} )

# include simulation tools
include_directories( ${FASTJET_INCLUDE_DIRS} )
include_directories( ${PYTHIA8_INCLUDE_DIRS} )
include_directories( ${HEPMC_INCLUDE_DIRS} )


# core
add_subdirectory( core/GaugiKernel )
add_subdirectory( core/G4Kernel )

# event packages
add_subdirectory( events/CaloHit )
add_subdirectory( events/CaloCell )
add_subdirectory( events/SpacePoint )
add_subdirectory( events/CaloCluster )
add_subdirectory( events/EventInfo )
add_subdirectory( events/TruthParticle )
add_subdirectory( events/CaloRings )

# reconstruction package
add_subdirectory( reconstruction/calorimeter/CaloCellBuilder )
add_subdirectory( reconstruction/calorimeter/CaloClusterBuilder )
add_subdirectory( reconstruction/calorimeter/CaloRingsBuilder )
add_subdirectory( reconstruction/io/RootStreamBuilder )

# detector definitions
add_subdirectory( geometry/ATLAS )

# generator package
add_subdirectory( generator/genkernel )
add_subdirectory( generator/filters )
add_subdirectory( generator/evtgen )






add_library(lorenzetti SHARED
  $<TARGET_OBJECTS:GaugiKernel>
  $<TARGET_OBJECTS:G4Kernel>
  $<TARGET_OBJECTS:CaloHit>
  $<TARGET_OBJECTS:CaloCell>
  $<TARGET_OBJECTS:CaloCluster>
  $<TARGET_OBJECTS:EventInfo>
  $<TARGET_OBJECTS:TruthParticle>
  $<TARGET_OBJECTS:CaloRings>
  $<TARGET_OBJECTS:SpacePoint>
  $<TARGET_OBJECTS:CaloCellBuilder>
  $<TARGET_OBJECTS:CaloClusterBuilder>
  $<TARGET_OBJECTS:CaloRingsBuilder>
  $<TARGET_OBJECTS:RootStreamBuilder>
  $<TARGET_OBJECTS:GenKernel>
  $<TARGET_OBJECTS:filters>
  $<TARGET_OBJECTS:evtgen>
)

message( STATUS ${FASTJET_LIBRARIES} )
message( STATUS ${PYTHIA8_LIBRARIES} )
message( STATUS ${HEPMC_LIBRARIES} )

target_link_Libraries(lorenzetti ${ROOT_LIBRARIES} 
                                 ${ROOT_COMPONENT_LIBRARIES} 
                                 #${Boost_PYTHON_LIBRARIES} 
                                 ${PYTHON_LIBRARIES} 
                                 #${Boost_FILESYSTEM_LIBRARY} 
                                 #${Boost_SYSTEM_LIBRARY} 
                                 ${Geant4_LIBRARIES} 
                                 ${FASTJET_LIBRARIES} 
                                 ${HEPMC_LIBRARIES}
                                 ${PYTHIA8_LIBRARIES} )



