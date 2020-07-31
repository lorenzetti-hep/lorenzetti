

[![DOI](https://zenodo.org/badge/248031762.svg)](https://zenodo.org/badge/latestdoi/248031762)

# Lorenzetti Simulator

Lorenzetti is an initiative to create HEP events with low level calorimetry information for machine learning studies. Since most part of High Energy Physics Experiments (like ATLAS or CMS) only provide datasets with high level information to the physics community or this type of information is only available for experts people, we decide to create this framework to help the community to obtain this type of data.

This framework was desing based on the athena reconstruction framework used by the ATLAS experiment. The Lorenzetti will allow you to generate events with the Pythia8 generator, propagate it throut of the generic calorimenter using the Geant4 simulator and recosntruct the eletromagnetic cluster information with low level information (like the eletronic pulse for each cell) using the Lorenzertti's reconstruction infrastruture. 

Table of Contents
=================

  *[Overview](#overview)
    *[Provided Functionalities](#provided-functionalities)
  *[Releases](#releases)
  *[Getting Started](#getting-started)

## Overview:



## Provided Functionalities:

These are the commom features (until now) supported by the reconstruction framework: 

### Pileup Event Simulation:

The framework is able to simulate in-time and out-of-time pileup. 

### Shower Shapes Feature Extraction:

### Low-Level Calorimetry Information:

### Two types of Ntuple:



## Releases:

This is the build status of each branch: 

|  Branch    | Build Status |
| ---------- | ------------ |
|   Master   |[![Build Status](https://travis-ci.org/jodafons/lorenzetti.svg?branch=master)](https://travisci.org/jodafons/lorenzetti)  |


## Getting Started:

There are two ways you can run Lorenzetti. Using a `Docker container` (stronglly recommended) or `locally` on your machine.
The locally installation usually can be used for geometry construction since allow the user to launch the `Qt`graphic inteface.

### Running with Singularity

If you take a look at the [Lorenzetti's DockerHub](https://hub.docker.com/r/lorenzetti/lorenzetti) you'll find two main images:

* lorenzett/lorenzett:latest
* lorenzett/lorenzett:cluster

We'll refer to the `latest` image as `base`, since `cluster` inherits from it. More details on how these images are generated [here](https://github.com/jodafons/lorenzetti/tree/master/docker), on the `docker/` directory. Our base image is meant for users that want to do a custom run on Lorenzett and make your developments. 

#### Using the base image (For develpments):

To setup the `lorenzett/lorenzett:base` image using singularity:

```
singularity run docker://lorenzetti/lorenzetti:base
```

As soon as your bash opens, you need to run:
```
source /setup_envs.sh
```
This will setup everything you need for Lorenzetti.


#### Using the *cluster* image

More details in [here](https://github.com/jodafons/lorenzetti/tree/master/docker/cluster):

```
singularity run docker://lorenzetti/lorenzetti:cluster -o zee.root -f PythiaGenerator/gen_zee.py -e 100 -j 10 -s 0 -r reco_trf.py -n physics -v $PWD --pileupAvg 10
```








#### Requirements

- Geant4 (opengl or qt4 is required for graphic interface, https://github.com/jodafons/geant4.git);
- ROOT (https://github.com/root-project/root.git);
- Pythia8 (https://github.com/jodafons/pythia.git);
- HEPMC (https://github.com/jodafons/hepmc.git);
- FastJet (https://github.com/jodafons/fastjet.git);
- Gaugi (pip3 install gaugi).



### Useful scripts:

You can convert the ROOT ntuple information into a `numpy` array using the `convert.py` script and the reconstruction ntuple output.

```
python3 scripts/convert.py -i zee.reco.root -o zee.reco.npz --nov -1
```

> *Notes:*
> - Only supported by the `physics` ntuple schemma.
> - All cells (in matrix format) will be available using this script.

## References:

- Athena framework: https://gitlab.cern.ch/atlas/athena


