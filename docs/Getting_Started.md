

# Getting Started:

The easiest way to use Lorenzetti is by employing either docker or singularity based on the images we provide at the [Lorenzetti's DockerHub](https://hub.docker.com/r/lorenzetti/lorenzetti). Technical details on how these images are generated are available [here](https://github.com/jodafons/lorenzetti/tree/master/docker).


## GUI Application (Optional)

This is a optional step only for linux machines. First, install on host machine:

```
apt-get install xorg
```

Than disable the access control to any host 

```
xhost + 
```

These commands will allow you to forward the GUI interface from docker/singularity to the host machine.

## Running as Container:


### Singularity (Recommended):


To `pull` using singularity engine :

```
singularity pull docker://lorenzetti/lorenzetti:latest
```

To `run` using singularity engine:

```
singularity run lorenzetti_latest.sif
```

After `run` the container, you must execute this script to
setup all simulation dependences:

```
source /setup_envs.sh
```

Singularity passforward the GUI interface by default. You just only need to have installed the xorg package 
into you host linux machine.


### Docker:

We recoment to use singularity, but if you dont have it:

```
docker run -e DISPLAY=$DISPLAY -v ${HOME}:${HOME} -v /tmp/.X11-unix:/tmp/.X11-unix -v $XAUTHORITY:/tmp/.XAuthority -e XAUTHORITY=/tmp/.XAuthority  -it lorenzetti/lorenzetti:latest
```

After run the container, you must execute this script to
setup all simulation dependences:

```
source /setup_envs.sh
```

## Installation

After enter and setup all dependencies inside of the container, just follow these commands to download and install the simulator.

```
git clone https://github.com/lorenzetti-hep/lorenzetti.git && cd lorenzetti
make
source setup.sh
```

This will setup everything you need for running the Lorenzetti framework.

## Test Stages Locally:

```
make test
```