SHELL := /bin/bash

DOCKER_NAMESPACE=lorenzetti

all:  build_sources

build_sources:
	mkdir -p build
	cd build && cmake .. && make -j${nproc} && cp ../scripts/lzt_setup.sh . && cd ..

test:
	source .github/workflows/tests/test_local.sh tests
