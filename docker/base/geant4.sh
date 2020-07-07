#!/bin/sh

geant4_envbindir=$(pwd)
#-----------------------------------------------------------------------
# Setup Geant4 binary and library paths...
#

export PATH="$geant4_envbindir":${PATH}


export LD_LIBRARY_PATH="`cd $geant4_envbindir/BuildProducts/li* > /dev/null ; pwd`":${LD_LIBRARY_PATH}


#-----------------------------------------------------------------------
# Setup Third-party binary and library paths...
# - CLHEP
# - Builtin CLHEP used

# - XercesC
# GDML SUPPORT NOT AVAILABLE

#-----------------------------------------------------------------------
# Resource file paths
# - Datasets
export G4NEUTRONHPDATA="`cd $geant4_envbindir/data/G4NDL4.6 > /dev/null ; pwd`"
export G4LEDATA="`cd $geant4_envbindir/data/G4EMLOW7.9.1 > /dev/null ; pwd`"
export G4LEVELGAMMADATA="`cd $geant4_envbindir/data/PhotonEvaporation5.5 > /dev/null ; pwd`"
export G4RADIOACTIVEDATA="`cd $geant4_envbindir/data/RadioactiveDecay5.4 > /dev/null ; pwd`"
export G4PARTICLEXSDATA="`cd $geant4_envbindir/data/G4PARTICLEXS2.1 > /dev/null ; pwd`"
export G4PIIDATA="`cd $geant4_envbindir/data/G4PII1.3 > /dev/null ; pwd`"
export G4REALSURFACEDATA="`cd $geant4_envbindir/data/RealSurface2.1.1 > /dev/null ; pwd`"
export G4SAIDXSDATA="`cd $geant4_envbindir/data/G4SAIDDATA2.0 > /dev/null ; pwd`"
export G4ABLADATA="`cd $geant4_envbindir/data/G4ABLA3.1 > /dev/null ; pwd`"
export G4INCLDATA="`cd $geant4_envbindir/data/G4INCL1.0 > /dev/null ; pwd`"
export G4ENSDFSTATEDATA="`cd $geant4_envbindir/data/G4ENSDFSTATE2.2 > /dev/null ; pwd`"









