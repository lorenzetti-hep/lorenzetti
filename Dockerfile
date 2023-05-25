
FROM ubuntu:20.04
LABEL maintainer "Joao Victor Pinto <jodafons@cern.ch>"

ENV LC_ALL C.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV TERM screen
ENV TZ=America/New_York
ENV DEBIAN_FRONTEND=noninteractive

# update all palsckages
RUN apt-get update -y --fix-missing
RUN apt-get update

# Install all dependencies 
RUN apt-get install -y wget 
RUN apt-get install -y build-essential 
RUN apt-get install -y dpkg-dev 
RUN apt-get install -y cmake 
RUN apt-get install -y git 
RUN apt-get install -y curl 
RUN apt-get install -y dpkg-dev 
RUN apt-get install -y g++ 
RUN apt-get install -y gcc 
RUN apt-get install -y binutils 
RUN apt-get install -y libx11-dev 
RUN apt-get install -y libxpm-dev 
RUN apt-get install -y libxft-dev 
RUN apt-get install -y libxext-dev 
RUN apt-get install -y python3 
RUN apt-get install -y python-dev-is-python3 
RUN apt-get install -y python3-pip 
RUN apt-get install -y librange-v3-dev 
RUN apt-get install -y libboost-python-dev 
RUN apt-get install -y libxft-dev 
RUN apt-get install -y libxext-dev  
RUN apt-get install -y libssl-dev 
RUN apt-get install -y gfortran 
RUN apt-get install -y libpcre3-dev
RUN apt-get install -y libglu1-mesa-dev 
RUN apt-get install -y libglew-dev 
RUN apt-get install -y libftgl-dev 
RUN apt-get install -y libxerces-c-dev
RUN apt-get install -y libmysqlclient-dev 
RUN apt-get install -y libfftw3-dev 
RUN apt-get install -y libcfitsio-dev
RUN apt-get install -y libgraphviz-dev 
RUN apt-get install -y libavahi-compat-libdnssd-dev
RUN apt-get install -y libldap2-dev 
RUN apt-get install -y libxml2-dev 
RUN apt-get install -y libkrb5-dev 
RUN apt-get install -y libgsl-dev 
RUN apt-get install -y rsync
RUN apt-get install -y libboost-all-dev


# Install QT5
RUN apt-get install -y  mesa-common-dev qtcreator qt5-default

WORKDIR /physics/root

# install ROOT
RUN git clone https://github.com/root-project/root.git && cd root && git checkout v6-28-02
RUN mkdir build && cd build && cmake  -Dpython_version=3 -Dxrootd=OFF -Dbuiltin_xrootd=OFF ../root && make -j$(nproc)
ENV ROOTSYS "/physics/root/build/"
ENV PATH "$ROOTSYS/bin:$PATH"
ENV LD_LIBRARY_PATH "$ROOTSYS/lib:$LD_LIBRARY_PATH"
ENV PYTHONPATH "/physics/root/build/lib:$PYTHONPATH"


# Install Geant4
WORKDIR /physics/geant
RUN git clone https://github.com/lorenzetti-hep/geant4.git
RUN mkdir build && cd build && cmake -DGEANT4_INSTALL_DATA=ON \
-DGEANT4_BUILD_MULTITHREADED=ON -DGEANT4_USE_SYSTEM_ZLIB=ON DGEANT4_USE_OPENGL_X11=ON \
-DGEANT4_USE_QT=ON -DGEANT4_USE_GDML=ON -DGEANT4_BUILD_MUONIC_ATOMS_IN_USE=ON ../geant4 && make -j$(nproc)
RUN cd build && cp ../geant4/scripts/geant4_10.5.1.sh geant4.sh


# Install Pythia8
RUN apt-get install -u 
WORKDIR /physics
RUN git clone https://github.com/lorenzetti-hep/pythia8.git && cd pythia8 && ./configure --with-python-config=python3-config && make -j$(nproc)

# Install FastJet
RUN curl -O http://fastjet.fr/repo/fastjet-3.3.3.tar.gz && tar zxvf fastjet-3.3.3.tar.gz && cd fastjet-3.3.3/ && ./configure && make -j$(nproc) && make install


# Install HEPMC
WORKDIR /physics/hepmc
RUN git clone https://github.com/lorenzetti-hep/hepmc3.git && cd hepmc3 && git checkout 3.2.5 
RUN mkdir build && cd build && cmake -DHEPMC3_ENABLE_ROOTIO=ON -DHEPMC3_INSTALL_INTERFACES=ON -DHEPMC3_ENABLE_PROTOBUFIO=ON ../hepmc3 && make -j$(nproc) && make install


# Install pip packages
RUN pip3 install --no-cache-dir setuptools pandas sklearn seaborn jupyterlab tqdm atlas-mpl-style twine pyhepmc colorama prettytable
RUN apt-get install -y xorg

# setup all environments before start bash terminal
COPY setup_envs.sh /
RUN chmod 774 /setup_envs.sh
WORKDIR /home
