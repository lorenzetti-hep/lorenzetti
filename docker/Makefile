
all: build

build:
	docker build --network host --compress -t lorenzetti/lorenzetti:latest .
	docker build --network host --compress -t lorenzetti/lorenzetti:base .
push:
	docker push lorenzetti/lorenzetti:latest
	docker push lorenzetti/lorenzetti:base
run:
	xhost + && docker run -e DISPLAY=$DISPLAY -v ${HOME}:${HOME} -v /tmp/.X11-unix:/tmp/.X11-unix -v $XAUTHORITY:/tmp/.XAuthority -e XAUTHORITY=/tmp/.XAuthority  -it lorenzetti/lorenzetti:latest
clean:
	docker system prune -a
pull:
	singularity pull docker://lorenzetti/lorenzetti:latest
	