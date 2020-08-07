Lorenzetti is provided as a tool for the HEP community to freely exploit the full potential of calorimetry data. We expect that it enables the community to fill the gap for R&D in calorimetry technology for parsing by providing:

 - Simulated low-level calorimetry information;
 - Free-to-use data. 
 
Hence, it allows the HEP community to freely work on proof-of-concepts (POCs) using full simulation (geant-based) that is currently difficult to obtain on experiments and publish them independently. We believe that the possibility to publish POCs apart from the experiments can be a powerful way to foster scientific exchange within the HEP community, but also to facilitate integration with the broader scientific community.

We welcome everyone to contribute!


## Citations

Please cite [![DOI](https://zenodo.org/badge/248031762.svg)](https://zenodo.org/badge/latestdoi/248031762) if you use the software.

[//]: # (and/or the applicable papers.)

## Getting Started:

The easiest way to use Lorenzetti is by employing either docker or singularity based on the images we provide at the [Lorenzetti's DockerHub](https://hub.docker.com/r/lorenzetti/lorenzetti). Technical details on how these images are generated are available [here](https://github.com/jodafons/lorenzetti/tree/master/docker).

### Setting up for standalone usage

When using lorenzetti on the same host, use `lorenzett/lorenzett:base`:

```
singularity run docker://lorenzetti/lorenzetti:base
```

After you get a prompt, do:

```
source /setup_envs.sh
```

This will setup everything you need for running Lorenzetti.

### LNCC (cluster) usage

More details [here](https://github.com/jodafons/lorenzetti/tree/master/docker/cluster):

## Running Lorenzetti

The pipeline has two steps:

1. Event generation with `pythia` wrappers;
1. Shower propagation with `geant` and reconstruction.

### Generation

A set of pythia wrappers is available on provided `generator/PythiaGenerator/share/`. I.e.

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

- [FCC software](https://github.com/HEP-FCC/FCCSW). Particularly, we hope to eventually integrate our framework on FCC software;
- [Delphes](https://github.com/delphes/delphes).
