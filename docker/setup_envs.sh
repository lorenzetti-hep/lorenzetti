#!/bin/bash

# HEP local packages

# ROOT
source /opt/root/buildthis/bin/thisroot.sh

# geant4
cd /opt/geant4/buildthis
source geant4.sh
cd /

# pythia
export PYTHIA8_INCLUDE=/opt/pythia/include
export PYTHIA8_LIBRARIES=/opt/pythia/lib

#fastjet
export FASTJET_INCLUDE=/opt/fastjet-3.3.3/include
export FASTJET_LIBRARIES=/usr/local/lib

#hepmc
export HEPMC_LIBRARIES=/usr/local/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HEPMC_LIBRARIES

export LD_PRELOAD=''
for file in /opt/geant4/buildthis/BuildProducts/lib/*.so
do
  echo $file
  export LD_PRELOAD=$file:$LD_PRELOAD
done 


cd /code/lorenzett 
git pull
mkdir build && cd build && cmake .. && make -j10 && cd .. && source setup.sh



