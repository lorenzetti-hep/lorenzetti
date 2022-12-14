#!/bin/bash
current_path=$PWD

# Envs
CPU_N=$(grep -c ^processor /proc/cpuinfo)

# ROOT
echo "setup root..."
source /apt/root/buildthis/bin/thisroot.sh

# geant4
echo "setup geant..."
cd /apt/geant4/buildthis
source /apt/geant4/buildthis/geant4.sh

# pythia
echo "setup pythia..."
export PYTHIA8_INCLUDE=/apt/pythia8/include
export PYTHIA8_LIBRARIES=/apt/pythia8/lib

#fastjet
echo "setup fastjet..."
export FASTJET_INCLUDE=/apt/fastjet-3.3.3/include
export FASTJET_LIBRARIES=/usr/local/lib

#hepmc
echo "setup hepmc..."
export HEPMC_LIBRARIES=/usr/local/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HEPMC_LIBRARIES


# preload libs (fix)
export LD_PRELOAD=''
for file in /apt/geant4/buildthis/BuildProducts/lib/*.so
do
  echo $file
  export LD_PRELOAD=$file:$LD_PRELOAD
done


#cd $current_path
#echo "compile lorenzetti"
#git clone https://github.com/lorenzetti-hep/lorenzetti.git
#cd lorenzetti
#source $PWD/setup_module.sh
#source $PWD/setup_module.sh --head
#mkdir build && cd build && cmake .. && make -j$CPU_N && cd .. && source $PWD/setup.sh
#echo "back to the base dir..."
cd $current_path

