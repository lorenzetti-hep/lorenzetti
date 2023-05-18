xhost + 
docker run -e DISPLAY=$DISPLAY -v ${HOME}:${HOME} -v /tmp/.X11-unix:/tmp/.X11-unix -v $XAUTHORITY:/tmp/.XAuthority -e XAUTHORITY=/tmp/.XAuthority  -it lorenzetti/lorenzetti:latest
