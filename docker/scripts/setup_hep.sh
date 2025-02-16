#!/bin/bash
current_path=$PWD

# geant4
echo "setup geant..."
cd /hep/geant4/build
source setup_geant.sh

# pythia
echo "setup pythia..."
export PYTHIA8_INCLUDE=/hep/pythia/include
export PYTHIA8_LIBRARIES=/hep/pythia/lib
export PYTHONPATH=$PYTHONPATH:/hep/pythia/lib

#fastjet
echo "setup fastjet..."
export FASTJET_INCLUDE=/hep/fastjet/include
export FASTJET_LIBRARIES=/usr/local/lib

#hepmc
echo "setup hepmc..."
export HEPMC_INCLUDE=/hep/hepmc/include/
export HEPMC_LIBRARIES=/usr/local/lib
#export PYTHONPATH=$PYTHONPATH:/hep/hepmc/build/python/3.8.10

# preload libs (fix)
export LD_PRELOAD=''
for file in /hep/geant4/build/BuildProducts/lib/*.so
do
  export LD_PRELOAD=$file:$LD_PRELOAD
done

cd $current_path

[[ -n "$PATH" ]] && echo "PATH=$PATH"
[[ -n "$LD_LIBRARY_PATH" ]] && echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
[[ -n "$PYTHONPATH" ]] && echo "PYTHONPATH=$PYTHONPATH"
[[ -n "$G4NEUTRONHPDATA" ]] && echo "G4NEUTRONHPDATA=$G4NEUTRONHPDATA"
[[ -n "$G4LEDATA" ]] && echo "G4LEDATA=$G4LEDATA"
[[ -n "$G4LEVELGAMMADATA" ]] && echo "G4LEVELGAMMADATA=$G4LEVELGAMMADATA"
[[ -n "$G4RADIOACTIVEDATA" ]] && echo "G4RADIOACTIVEDATA=$G4RADIOACTIVEDATA"
[[ -n "$G4PARTICLEXSDATA" ]] && echo "G4PARTICLEXSDATA=$G4PARTICLEXSDATA"
[[ -n "$G4PIIDATA" ]] && echo "G4PIIDATA=$G4PIIDATA"
[[ -n "$G4REALSURFACEDATA" ]] && echo "G4REALSURFACEDATA=$G4REALSURFACEDATA"
[[ -n "$G4SAIDXSDATA" ]] && echo "G4SAIDXSDATA=$G4SAIDXSDATA"
[[ -n "$G4ABLADATA" ]] && echo "G4ABLADATA=$G4ABLADATA"
[[ -n "$G4INCLDATA" ]] && echo "G4INCLDATA=$G4INCLDATA"
[[ -n "$G4ENSDFSTATEDATA" ]] && echo "G4ENSDFSTATEDATA=$G4ENSDFSTATEDATA"
[[ -n "$PYTHIA8_INCLUDE" ]] && echo "PYTHIA8_INCLUDE=$PYTHIA8_INCLUDE"
[[ -n "$PYTHIA8_LIBRARIES" ]] && echo "PYTHIA8_LIBRARIES=$PYTHIA8_LIBRARIES"
[[ -n "$FASTJET_INCLUDE" ]] && echo "FASTJET_INCLUDE=$FASTJET_INCLUDE"
[[ -n "$FASTJET_LIBRARIES" ]] && echo "FASTJET_LIBRARIES=$FASTJET_LIBRARIES"
[[ -n "$HEPMC_INCLUDE" ]] && echo "HEPMC_INCLUDE=$HEPMC_INCLUDE"
[[ -n "$HEPMC_LIBRARIES" ]] && echo "HEPMC_LIBRARIES=$HEPMC_LIBRARIES"
[[ -n "$LD_PRELOAD" ]] && echo "LD_PRELOAD=$LD_PRELOAD"

