SHELL := /bin/bash

DOCKER_NAMESPACE=lorenzetti
SETUP_SCRIPT=../core/GaugiKernel/cmake/lzt_setup.sh

all:  build_sources

build_sources:
	mkdir -p build
	cd build && cmake .. && make -j${nproc} && cp ${SETUP_SCRIPT} . && cd ..

test:
	source .github/workflows/tests/test_local.sh tests
