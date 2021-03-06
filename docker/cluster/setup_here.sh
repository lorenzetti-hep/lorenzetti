#!/bin/bash

current=$1

cd $(mktemp -d)
temp=$PWD


# Envs
CPU_N=$(grep -c ^processor /proc/cpuinfo)



# ROOT
echo "setup root..."
source /opt/root/buildthis/bin/thisroot.sh


# geant4
echo "setup geant..."
cd /opt/geant4/buildthis
source /opt/geant4/buildthis/geant4.sh


# pythia
echo "setup pythia..."
export PYTHIA8_INCLUDE=/opt/pythia/include
export PYTHIA8_LIBRARIES=/opt/pythia/lib

#fastjet
echo "setup fastjet..."
export FASTJET_INCLUDE=/opt/fastjet-3.3.3/include
export FASTJET_LIBRARIES=/usr/local/lib

#hepmc
echo "setup hepmc..."
export HEPMC_LIBRARIES=/usr/local/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HEPMC_LIBRARIES


# preload libs (fix)
export LD_PRELOAD=''
for file in /opt/geant4/buildthis/BuildProducts/lib/*.so
do
  echo $file
  export LD_PRELOAD=$file:$LD_PRELOAD
done

cd $temp

echo "compile lorenzetti..."
echo $CPU_N
git clone https://github.com/lorenzetti-hep/lorenzetti.git && cd lorenzetti
mkdir build && cd build && cmake .. && make -j$CPU_N
cd ..
source $PWD/setup.sh

echo $LZT_PATH
echo "back to the base dir..."
cd $current


