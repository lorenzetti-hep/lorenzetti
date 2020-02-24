cmake_minimum_required(VERSION 3.0)
project(minerva)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DDROP_CGAL")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)



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
# Find HepMC (required package)
find_package(HepMC REQUIRED)

#----------------------------------------------------------------------------
# Find ROOT (required package)
find_package(ROOT COMPONENTS EG Eve Geom Gui GuiHtml GenVector Hist Physics Matrix Graf RIO Tree Gpad RGL MathCore)
include(${ROOT_USE_FILE})


#----------------------------------------------------------------------------
include(gaugi/cmake/InstallGaugiFunctions.cmake)
gaugi_initialize()



#set( Python_ADDITIONAL_VERSIONS 3.0)
find_package( PythonInterp REQUIRED)
find_package( PythonLibs REQUIRED)
find_package( Boost COMPONENTS python system filesystem)
include_directories( ${Boost_INCLUDE_DIR} )
link_directories( ${Boost_LIBRARY_DIR} )





add_subdirectory( core )
add_subdirectory( events/CaloCluster )
add_subdirectory( events/CaloCell )


add_library(atropos SHARED
  $<TARGET_OBJECTS:core>
  $<TARGET_OBJECTS:CaloCluster>
  $<TARGET_OBJECTS:CaloCell>
)


target_link_Libraries(atropos ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES} ${Boost_PYTHON_LIBRARIES} ${PYTHON_LIBRARIES} 
  ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Geant4_LIBRARIES} ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES} ${ROOT_LIBRARIES} )
#----------------------------------------------------------------------------


