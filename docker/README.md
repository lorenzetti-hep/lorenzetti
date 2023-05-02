# Docker image

To compile and push:

```
make
make push
```

## Running GUI Application (Only LINUX)

Install on host machine:

```
apt-get install xorg
```

Than disable the access control to any host 

```
xhost + 
```


### Docker:

```
docker run -e DISPLAY=$DISPLAY -v ${HOME}:${HOME} -v /tmp/.X11-unix:/tmp/.X11-unix -v $XAUTHORITY:/tmp/.XAuthority -e XAUTHORITY=/tmp/.XAuthority  -it lorenzetti/lorenzetti:latest
```

After run the container, you must execute this script to
setup all simulation dependences:

```
source /setup_envs.sh
```


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