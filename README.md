# Lorenzett Event Simulator

This is the first event generator for HEP using a generic calorimeter for machine learning 
studies based on athena framework.

## Getting Started

There are two ways you can run Lorenzett: using a Docker container (recommended) or locally on your machine.

### Running with Docker

If you take a look at the [Lorenzett's DockerHub](https://hub.docker.com/r/gabrielmilan/lorenzett) you'll find two main images:

* gabrielmilan/lorenzett:latest
* gabrielmilan/lorenzett:sdumont

We'll refer to the `latest` image as `base`, since `sdumont` inherits from it. More details on how these images are generated [here](https://github.com/jodafons/lorenzett/tree/master/docker), on the `docker/` directory.

Our base image is meant for users that want to do a custom run on Lorenzett. If you're a first-timer, you'll want the `gabrielmilan/lorenzett:sdumont` image.

#### Using the *sdumont* image

```
docker run -v <output-path>:/output gabrielmilan/lorenzett:sdumont <args>
```

For `<args>` information, see below:

```
lorenzett_run.sh - Script for running Lorenzett through docker in an one-line script
 
./lorenzett_run.sh [arguments]
 
Arguments:
-h, --help                show this message
-f, --filter              chooses which type of event you're interested
-e, --event               sets the desired number of events
-p, --pileup              sets the pileup average
-o, --output              sets the output filename
--bc_id_start             sets bc_id_start
--bc_id_end               sets bc_id_end
 
All arguments are required
```

#### Using the *latest* image

```
docker run -it -v <your-mount>:/volume gabrielmilan/lorenzett /bin/bash
```

> **Note**: You shall mount as many volumes as you wish.

As soon as your bash opens, you need to run
```
source setup_envs.sh
```

This will setup everything you need for Lorenzett.

For **event generation**, this is the command you're looking for (example for Zee):
```bash
prun_job.py -c "generator.py --filter Zee -i zee_config.cmnd --outputLevel 6 --seed 0 -evt <n_events> --pileupAvg <average-pileup> --bc_id_start -8 --bc_id_end 7" -o zee.root -mt <n_threads> -n <n_jobs>
```

> **Notes**
> - Files like `zee_config.cmnd` are located at `/code/lorenzett/generator/PythiaGenerator/data/`
> - Using seed=0 means that you'll use your system clock as seed.
> - If you would like to collect minimum bias for an especific calorimete region you just include the eta/phi region [here](https://github.com/jodafons/lorenzett/blob/master/scripts/generator.py#L112) and use the `--filter` with name `MinimumBias`.


For **reconstruction**, after generate the events using the `generator.py` command, you must pass the output file as input to the reconstruction transformation. To run the reconstruction use this command:

```bash
reco_trf.py -i zee.root --outputLevel 6 -nt <n_jobs> -o reco_zee.root
```

### Running locally

#### Requirements

- Geant4 (opengl or qt4 is required for graphic interface, https://github.com/jodafons/geant4.git);
- ROOT (https://github.com/root-project/root.git);
- Pythia8 (https://github.com/jodafons/pythia.git);
- HEPMC (https://github.com/jodafons/hepmc.git);
- FastJet (https://github.com/jodafons/fastjet.git).


## References:

- Athena framework: https://gitlab.cern.ch/atlas/athena


