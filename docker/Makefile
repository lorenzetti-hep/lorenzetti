SHELL := /bin/bash

DOCKER_NAMESPACE=lorenzetti

all:  build

build:
	docker build --progress=plain --compress -t ${DOCKER_NAMESPACE}/lorenzetti:latest .

build_singularity:
	singularity build --sandbox lorenzetti_latest.sif docker://${DOCKER_NAMESPACE}/lorenzetti:latest

clean:
	docker system prune -a

pull:
	singularity pull docker://lorenzetti/lorenzetti:latest

push:
	docker push lorenzetti/lorenzetti:latest
	