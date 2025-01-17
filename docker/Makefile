SHELL := /bin/bash

DOCKER_NAMESPACE=lorenzetti

all:  build

build:
	docker build --progress=plain --compress -t ${DOCKER_NAMESPACE}/lorenzetti:dev .

build_singularity:
	singularity build --sandbox lorenzetti_latest.sif docker://${DOCKER_NAMESPACE}/lorenzetti:dev

clean:
	docker system prune -a

pull:
	singularity pull docker://lorenzetti/lorenzetti:dev

push:
	docker push lorenzetti/lorenzetti:dev
	