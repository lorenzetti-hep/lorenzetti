

[![DOI](https://zenodo.org/badge/248031762.svg)](https://zenodo.org/badge/latestdoi/248031762)

# Lorenzetti Simulator

Werner should provide an introduction here.

Table of Contents
=================

  * [Overview](#overview)
    * [Provided Functionalities](#provided-functionalities)
  * [Releases](#releases)
  * [Getting Started](#getting-started)
    * [Running with Singularity](#running-with-singularity)
      * [Using the base image](#using-the-base-image)
      * [Using the cluster image](#using-the-cluster-image)
    * [Useful scripts](#useful-scripts)
  * [Requirements](#requirements)
  * [References](#references)


## Overview:



## Provided Functionalities:

These are the commom features (until now) supported by the reconstruction framework: 

- *Pileup Simulation*: The framework is able to simulate `in-time` and `out-of-time` pileup using the [pulse generator]() to emulate the eletronic pulse of each cell and each bunch-crossing. The estimated cell's energy is calculated using the [optimal filter]() and the generated eletronic pulse;
- *Ringer reconstruction*: 
- *Shower shapes*:
- *Low-Level information*:




## Releases:

This is the build status of each branch: 

|  Branch    | Build Status |
| ---------- | ------------ |
|   Master   |[![Build Status](https://travis-ci.org/jodafons/lorenzetti.svg?branch=master)](https://travisci.org/jodafons/lorenzetti)  |


## Getting Started:

There are two ways you can run Lorenzetti. Using a `Docker container` (stronglly recommended) or `locally` on your machine.

### Running with Singularity

If you take a look at the [Lorenzetti's DockerHub](https://hub.docker.com/r/lorenzetti/lorenzetti) you'll find two main images:

* lorenzett/lorenzett:latest
* lorenzett/lorenzett:cluster

We'll refer to the `latest` image as `base`, since `cluster` inherits from it. More details on how these images are generated [here](https://github.com/jodafons/lorenzetti/tree/master/docker), on the `docker/` directory. Our base image is meant for users that want to do a custom run on Lorenzett and make your developments. 

#### Using the *base* image

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




### Useful scripts

You can convert the ROOT ntuple information into a `numpy` array using the `convert.py` script and the reconstruction ntuple output.

```
python3 scripts/convert.py -i zee.reco.root -o zee.reco.npz --nov -1
```


## Requirements

- Geant4 (opengl or qt4 is required for graphic interface, https://github.com/jodafons/geant4.git);
- ROOT (https://github.com/root-project/root.git);
- Pythia8 (https://github.com/jodafons/pythia.git);
- HEPMC (https://github.com/jodafons/hepmc.git);
- FastJet (https://github.com/jodafons/fastjet.git);
- Gaugi (pip3 install gaugi).

## References:

- Athena framework: https://gitlab.cern.ch/atlas/athena


