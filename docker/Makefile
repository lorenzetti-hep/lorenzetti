SHELL := /bin/bash

DOCKER_NAMESPACE=lorenzetti
ROOT_VERSION=v6-34-00
GEANT4_VERSION=v11.2.2
FASTJET_VERSION=3.4.0
HEPMC_VERSION=3.3.0
PYTHIA8_VERSION=8313


all:  build

build:
	docker build --progress=plain --compress \
				 --build-arg ROOT_VERSION=${ROOT_VERSION} \
				 --build-arg GEANT4_VERSION=${GEANT4_VERSION} \
				 --build-arg FASTJET_VERSION=${FASTJET_VERSION} \
				 --build-arg PYTHIA8_VERSION=${PYTHIA8_VERSION} \
				 --build-arg HEPMC_VERSION=${HEPMC_VERSION} \
				 -t ${DOCKER_NAMESPACE}/lorenzetti:latest .

build_singularity:
	singularity build --sandbox lorenzetti_latest.sif docker://${DOCKER_NAMESPACE}/lorenzetti:latest

clean:
	docker system prune -a

pull:
	singularity pull docker://lorenzetti/lorenzetti:latest

push:
	docker push lorenzetti/lorenzetti:latest
	