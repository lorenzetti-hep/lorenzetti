[![DOI](https://zenodo.org/badge/370090835.svg)](https://zenodo.org/badge/latestdoi/370090835)


# Lorenzetti Simulator

Lorenzetti is a framework for the HEP community to freely exploit the full potential of calorimetry data. We expect to enable the community to mitigate bottlenecks for R&D in processing algorithms using calorimetry data by providing:

 - Unified low-level calorimetry and physics information based on full simulation (geant);
 - Free-to-use data. 
 
In other words, it provides a way for the HEP community to work on proof-of-concepts (POCs) using simulated data that is currently difficult to obtain on experiments and to publish them independently. We believe that this possibility, i.e. to publish POCs apart from the experiments, can be a powerful way to foster scientific exchange within the HEP community, but also to facilitate the exchange of processing algorithms with the broader scientific community.

We welcome everyone to contribute!


## Citations

Please cite [![DOI](https://zenodo.org/badge/370090835.svg)](https://zenodo.org/badge/latestdoi/370090835) if you use the software.

[//]: # (and/or the applicable papers.)


## Detector Construction:

The standard detector in the Lorenzetti framework consist in a eletromagnetic calorimeter and a hadronic calorimeter using a cylinder shape. Each one has 3 layers with different granularities to capture the shower develop by the particles. Also, between regions, there is a small slice of dead material.


![Screenshot](geometry/DetectorATLASModel/doc/cut_view.png)

It is possible, by using Geant4 modules to change the geometry, the layers and the cell granularity, allowing a high level of customization of the full detector.



## Getting Started:

The easiest way to use Lorenzetti is by employing either docker or singularity based on the images we provide at the [Lorenzetti's DockerHub](https://hub.docker.com/r/lorenzetti/lorenzetti). Technical details on how these images are generated are available [here](https://github.com/jodafons/lorenzetti/tree/master/docker).

### Setting up for standalone usage

When using lorenzetti on the same host, use `lorenzett/lorenzett:base`:

```
singularity run docker://lorenzetti/lorenzetti:base
```

After singularity instance is created, do:

```
source /setup_envs.sh
```

## Local Installation

Lorenzetti has the following dependencies:

- Geant4 (opengl or qt4 is required);
- ROOT;
- Pythia8;
- HEPMC;
- FastJet;
- Gaugi (pip3 install gaugi).

To install the package, the following commands are needed to download the Lorenzetti repository and compile it:
```
git clone https://github.com/lorenzetti-hep/lorenzetti.git
cd lorenzetti
mkdir build
source $PWD/setup.sh
source $PWD/setup.sh --head
cd build && cmake .. && make  && cd .. && source $PWD/setup.sh && cd ..
```
This will setup everything you need for running Lorenzetti.

### LNCC (cluster) usage

More details [here](https://github.com/jodafons/lorenzetti/tree/master/docker/cluster).

## Running Lorenzetti

The pipeline has three steps:

1. Event generation with `pythia` wrappers;
2. Shower propagation with `geant` and digitalization;
3. Event reconstruction.

### Generation

A set of pythia wrappers is provided on `generator/PythiaGenerator/share/`. Usage example:

```
prun_job.py -c "gen_zee.py --evt 25 --pileupAvg 0" -mt 10 -n 10 -o Zee.EVT.root
```
It is also possible to use different simulators to generate the inputs for the Lorenzetti framework. For this, it is required to produce HepMC-compatible files after the simulation. These files may be used as inputs for the event propagation stage. See details below on the Customizing section.

### Shower Propagation
THis is an examplete how to propagate the particles through the Geant4 module (considering the file produced on the first example step):
```
simu_trf.py -i Zee.EVT.root -o Zee.HIT.root --enableMagneticField
```

###  Digitalization

The digitalization process may be configured to use different energy estimation methods such as Optimal Filter (OF) and Constrained Optimal Filter (COF). It is also possible to simulate the crosstalk effect between adjacent calorimeter cells. More details on how to use such features, check the Customization section. In the digitalization step, it is used second step output to feed `digit_trf` script. For instance: 

```
digit_trf.py -i Zee.EVT.root -o Zee.ESD.root -nt 1
```

### Event Reconstruction

Use the third step output to feed `reco_trf` script. For instance:

```
reco_trf.py -i Zee.ESD.root -o Zee.AOD.root
```

A set of examples files for each step can be found at the ```examples/datadaset``` folder. These small examples are for single electrons.
### Customizing the generation and full reconstruction chain

There is several options on Lorenzetti framework. For this customization, there are flags to be used within each command step.

In the Event Generation (EVT file as output) we have several useful scripts to generate single particles or decays (see generator/PythiaGenerator/share/ folder). For single particle generation, we have some useful flags

- ```--eta``` and ```--phi```: this sets the exactly calorimeter position which the particle will interact
- ```-e```: this sets the energy value, in GeV, for your particle
- ```--energy_min``` and ```--energy_max```: this provides a range of the particle's energy uniformly distributed
- ```--eta_min``` and ```--eta_max```: this sets the eta range for the particles. To use this, an additional flags is need: ```--doEtaRanged True```

Single electron example with 15 < E < 30 and -0.8 < eta < 0.8:

```gen_electron.py --energy_min 15 --energy_max 30 --doEtaRanged True --eta_min -0.8 --eta_max 0.8 -o SingleElectron.EVT.root```

Detector Simulation (HIT file as output) we can provide a HepMC ASCII file as input instead of the standard Lorenzetti file. For that purpose, a conversion is needed using the script ```gen_fromHepMC.py``` scritps:

```gen_fromHepMC.py -i /path/to/your/file/myFile.dat -o myOutputFile.EVT.root```

Note: in order to use the gen_fromHepMC.py scripts, the installation of pyhepmc is needed. So, to do it:

```
pip install pyhepmc
```

In case you want to use the file generated by the HepMC conversion scripts, a flag ```--isFromHepMC``` is needed like:

```simu_trf.py -i myOutputFile.EVT.root -o HIT.root --enableMagneticFiled --isFromHepMC```

Digitalization (ESD file as output) allows some customization concerning about CrossTalk simulation and energy estimation methods changes:

- ```--simulateCrossTalk```: this simulates the crosstalk efect on the calorimeter cells
- ```--estimationMethodHAD```: this change the energy estimation method on the hadronic calorimeter. Two options for now are allowd: ```OF``` and ```COF```

An example for full customized digitalization step:

```digit_trf.py -i HIT.root -o ESD.root --simulateCrossTalk --estimationMethodHAD COF```


## Framework status:

|  Branch    | Build Status |
| ---------- | ------------ |
|   Master   |[![Build Status](https://travis-ci.org/jodafons/lorenzetti.svg?branch=master)](https://travisci.org/jodafons/lorenzetti)  |

## Software considerations:


Lorenzetti is built on top of standard simulation technology employed on HEP experiments ([Pythia](http://home.thep.lu.se/~torbjorn/Pythia.html) and [Geant](https://geant4.web.cern.ch)). Lorenzetti's concept design was greatly inspired in the [Athena framework](https://gitlab.cern.ch/atlas/athena). Other frameworks of potential interest:

- [FCC software](https://github.com/HEP-FCC/FCCSW). Particularly, we consider to eventually merge Lorenzetti inside the FCC software;
- [Delphes](https://github.com/delphes/delphes).
