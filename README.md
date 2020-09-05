[![DOI](https://zenodo.org/badge/248031762.svg)](https://zenodo.org/badge/latestdoi/248031762)

# Lorenzetti Simulator

Lorenzetti is a framework for the HEP community to freely exploit the full potential of calorimetry data. We expect to enable the community to mitigate bottlenecks for R&D in processing algorithms using calorimetry data by providing:

 - Unified low-level calorimetry and physics information based on full simulation (geant);
 - Free-to-use data. 
 
In other words, it provides a way for the HEP community to work on proof-of-concepts (POCs) using simulated data that is currently difficult to obtain on experiments and to publish them independently. We believe that this possibility, i.e. to publish POCs apart from the experiments, can be a powerful way to foster scientific exchange within the HEP community, but also to facilitate the exchange of processing algorithms with the broader scientific community.

We welcome everyone to contribute!


## Citations

Please cite [![DOI](https://zenodo.org/badge/248031762.svg)](https://zenodo.org/badge/latestdoi/248031762) if you use the software.

[//]: # (and/or the applicable papers.)


## Detector Construction:

We should include some description here.


![Screenshot](geometry/DetectorATLASModel/doc/cut_view.png)




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

This will setup everything you need for running Lorenzetti.

### LNCC (cluster) usage

More details [here](https://github.com/jodafons/lorenzetti/tree/master/docker/cluster).

## Running Lorenzetti

The pipeline has two steps:

1. Event generation with `pythia` wrappers;
1. Shower propagation with `geant` and reconstruction.

### Generation

A set of pythia wrappers is provided on `generator/PythiaGenerator/share/`. Usage example:

```
python generator/PythiaGenerator/share/gen_zee.py -o zee.root --nov 100 --seed 0 --pileupAvg 100
```

### Shower Propagation and Reconstruction

Use first step output to feed `reco_trf` script. For instance: 

```
python scripts/reco_trf.py -i zee.root -o zee.reco.root
```

### Export data to numpy array

If needed, we provide the `convert.py` transformation job:

```
python scripts/convert.py -i zee.reco.root -o zee.reco.npz --nov -1
```

## Local Installation

Lorenzetti has the following dependencies:

- Geant4 (opengl or qt4 is required);
- ROOT;
- Pythia8;
- HEPMC;
- FastJet;
- Gaugi (pip3 install gaugi).

## Framework status:

|  Branch    | Build Status |
| ---------- | ------------ |
|   Master   |[![Build Status](https://travis-ci.org/jodafons/lorenzetti.svg?branch=master)](https://travisci.org/jodafons/lorenzetti)  |

## Software considerations:


Lorenzetti is built on top of standard simulation technology employed on HEP experiments ([Pythia](http://home.thep.lu.se/~torbjorn/Pythia.html) and [Geant](https://geant4.web.cern.ch)). Lorenzetti's concept design was greatly inspired in the [Athena framework](https://gitlab.cern.ch/atlas/athena). Other frameworks of potential interest:

- [FCC software](https://github.com/HEP-FCC/FCCSW). Particularly, we consider to eventually merge Lorenzetti inside the FCC software;
- [Delphes](https://github.com/delphes/delphes).
