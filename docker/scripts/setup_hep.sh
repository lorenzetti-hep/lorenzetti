#!/bin/bash
current_path=$PWD


# ROOT
echo "setup root..."
source /hep/root/build/bin/thisroot.sh

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
  echo $file
  export LD_PRELOAD=$file:$LD_PRELOAD
done


cd $current_path

