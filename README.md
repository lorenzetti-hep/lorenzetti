# Lorenzetti Event Simulator

This is the first event generator for HEP using a generic calorimeter for machine learning 
studies based on athena framework (ATLAS).



## Detector construction:

It's a composite cilindric calorimeter with eletromagnetic and hadronic layers with 6.12m in z starting 90cm of disntance
in the collision line and 148cm of material.

Description:

- First EM layer (stripts) with 15 cm depth (xy) and 2 segments (layers) of LAr and Pb (0.003 x 0.003); 
- Second EM layer with 30 cm depth (xy) and 2 segments (layers) of LAr and Pb (0.025 x 0.025); 
- Third EM layer with 3 cm depth (xy) and 2 segments (layers) of LAr and Pb (0.05 x 0.025); 
- First HAD layer with 40 cm depth (xy) and 2 segments (layers) of Cesium iodide and iron (0.1 x 0.1); 
- Second HAD layer with 40 cm depth (xy) and 2 segments (layers) of Cesium iodide and iron (0.1 x 0.1); 
- Third HAD layer with 20 cm depth (xy) and 1 segments (layers) of Cesium iodide and iron (0.2 x 0.1); 





## Requirements:

- Geant4 (opengl or qt4 is required for graphic interface, https://github.com/jodafons/geant4.git);
- ROOT (https://github.com/root-project/root.git);
- Pythia8 (https://github.com/jodafons/pythia.git);
- HEPMC (https://github.com/jodafons/hepmc.git);
- FastJet (https://github.com/jodafons/fastjet.git).


## Installation:

Use this example to build your hep installation in your local machine.
The path for each HEP package depends of your local installation. Take 
this as example to build your setup script.


```bash
# ROOT
source ~/.bin/root/build_root/bin/thisroot.sh
# geant4
cd ~/.bin/geant4_10.5/build
source geant4.sh
cd ~
# hepmc
export HEPMC_INCLUDE=/Users/jodafons/.bin/HepMC/HepMC-2.06.09/HepMC
export HEPMC_LIBRARY=/Users/jodafons/.bin/HepMC/build/lib
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$HEPMC_LIBRARY
# pythia
export PYTHIA8_INCLUDE=/Users/jodafons/.bin/pythia8244/include
export PYTHIA8_LIBRARY=/Users/jodafons/.bin/pythia8244/lib
#fastjet
export FASTJET_INCLUDE=/Users/jodafons/.bin/fastjet-3.3.3/include
export FASTJET_LIBRARY=/Users/jodafons/.bin/fastjet-3.3.3/lib
# ROOT extra envs
export ROOT_INCLUDE=/Users/jodafons/.bin/root/build_root/include
export ROOT_LIBRARY=/Users/jodafons/.bin/root/build_root/lib
```

### Compile the simulator package:

The simulator will use this envirements to locate all necessary infomation to build and
link all dependencies. After you install all dependencies and set all necessary envirementes 
just compile the simulator package using the follow commands.


```bash
mkdir build && cd build
cmake ..
make
```

### Compile the pythia event generator:

This sub package is used to create jet patcicles using pythia and dump this information
inside a ttree. This root file will be used by the gnerator to read all jets and propagate
this particles throut out the calorimeter.

```bash
cd generator/
make generator
```


## Usage:



## References:

- Athena framework: https://gitlab.cern.ch/atlas/athena


