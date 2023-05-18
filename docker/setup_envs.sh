#!/bin/bash
current_path=$PWD


# ROOT
echo "setup root..."
source /physics/root/build/bin/thisroot.sh

# geant4
echo "setup geant..."
cd /physics/geant/build
source geant4.sh

# pythia
echo "setup pythia..."
export PYTHIA8_INCLUDE=/physics/pythia8/include
export PYTHIA8_LIBRARIES=/physics/pythia8/lib
export PYTHONPATH=$PYTHONPATH:/physics/pythia8/lib

#fastjet
echo "setup fastjet..."
export FASTJET_INCLUDE=/physics/fastjet-3.3.3/include
export FASTJET_LIBRARIES=/usr/local/lib

#hepmc
echo "setup hepmc..."
export HEPMC_INCLUDE=/physics/hepmc/hepmc3/include/
export HEPMC_LIBRARIES=/usr/local/lib
export PYTHONPATH=$PYTHONPATH:/physics/hepmc/build/python/3.8.10

# preload libs (fix)
export LD_PRELOAD=''
for file in /physics/geant/build/BuildProducts/lib/*.so
do
  echo $file
  export LD_PRELOAD=$file:$LD_PRELOAD
done


cd $current_path

