SHELL := /bin/bash

DOCKER_NAMESPACE=lorenzetti

all:  build_sources

build_image:
	docker build --progress=plain --compress -t ${DOCKER_NAMESPACE}/lorenzetti:latest .

build_singularity:
	singularity build --sandbox lorenzetti_latest.sif docker://${DOCKER_NAMESPACE}/lorenzetti:latest

build_sources:
	mkdir -p build
	cd build && cmake .. && make -j10 && cd ..

test:
	source .github/workflows/tests/test_local.sh tests

push:
	docker push ${DOCKER_NAMESPACE}/lorenzetti:latest

run:
	xhost + && docker run -e DISPLAY=${DISPLAY} -v ${HOME}:${HOME} -v /tmp/.X11-unix:/tmp/.X11-unix -v ${XAUTHORITY}:/tmp/.XAuthority -e XAUTHORITY=/tmp/.XAuthority  -it ${DOCKER_NAMESPACE}/lorenzetti:latest

clean:
	docker system prune -a

pull:
	singularity pull docker://lorenzetti/lorenzetti:latest
	