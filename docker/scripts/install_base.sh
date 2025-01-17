#!/bin/sh
set -e
apt-get update -y --fix-missing
apt-get install -y wget \
                   build-essential \
                   dpkg-dev \
                   cmake \
                   git \
                   curl \
                   dpkg-dev \
                   g++ \
                   gcc \
                   binutils \
                   libx11-dev \
                   libxpm-dev \
                   libxft-dev \
                   libxext-dev \
                   python3 \
                   python-dev-is-python3 \
                   python3-pip \
                   librange-v3-dev \
                   libboost-python-dev \
                   libxft-dev \
                   libxext-dev \
                   libssl-dev \
                   gfortran \
                   libpcre3-dev \
                   libglu1-mesa-dev \
                   libglew-dev \
                   libftgl-dev \
                   libxerces-c-dev \
                   libmysqlclient-dev \
                   libfftw3-dev \
                   libcfitsio-dev \
                   libgraphviz-dev \
                   libavahi-compat-libdnssd-dev \
                   libldap2-dev \
                   libxml2-dev \
                   libkrb5-dev \
                   libgsl-dev \
                   rsync\
                   libboost-all-dev \
                   mesa-common-dev \
                   qtcreator \
                   qtbase5-dev \
                   qt5-qmake \
                   cmake \
                   libxmu-dev \
                   libxi-dev \
                   protobuf-compiler \
                   libprotobuf-dev
