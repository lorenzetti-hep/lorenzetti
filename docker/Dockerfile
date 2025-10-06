FROM ubuntu:22.04
WORKDIR /opt


ENV LC_ALL=C.UTF-8
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV TERM=screen
ENV TZ=America/New_York
ENV DEBIAN_FRONTEND=noninteractive


RUN apt-get update -y --fix-missing
RUN apt-get install -y wget \
                       build-essential \
                       dpkg-dev \
                       cmake \
                       curl \
                       dpkg-dev \
                       g++ \
                       gcc \
                       binutils \
                       libxpm-dev \
                       python3 \
                       python-dev-is-python3 \
                       python3-pip \
                       librange-v3-dev \
                       libssl-dev \
                       gfortran \
                       libpcre3-dev \
                       libglu1-mesa-dev \
                       libglew-dev \
                       libftgl-dev \
                       libmysqlclient-dev \
                       libfftw3-dev \
                       libcfitsio-dev \
                       libgraphviz-dev \
                       libxml2-dev \
                       libkrb5-dev \
                       libgsl-dev \
                       rsync\
                       cmake \
                       libxi-dev \
                       protobuf-compiler \
                       libprotobuf-dev
       
# GEANT4 uses this
RUN apt-get update && \
    apt install -y --fix-missing \
    libxerces-c-dev \
    libboost-python-dev \
    libavahi-compat-libdnssd-dev \
    libldap2-dev \
    libboost-all-dev

# DGEANT4_USE_OPENGL_X11=on uses this
RUN apt install -y \
    libx11-dev \
    libxpm-dev \
    libxft-dev \
    libxext-dev \
    libxmu-dev \
    mesa-common-dev

# DGEANT4_USE_QT=ON uses this
RUN apt install -y \
    qtcreator \
    qtbase5-dev \
    qt5-qmake 

RUN apt install -y git

ARG ROOT_VERSION
LABEL ROOT_VERSION=${ROOT_VERSION}
RUN git clone https://github.com/root-project/root.git
RUN cd root && git checkout ${ROOT_VERSION}
RUN cd root/ && \
    mkdir build && \
    cd build && \
    cmake  -Dpython_version=3 -Dxrootd=OFF -Dbuiltin_xrootd=OFF .. && \
    make -j$(nproc)


ENV ROOTSYS=/opt/root/build/
ENV PATH=$ROOTSYS/bin:$PATH
ENV LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
ENV PYTHONPATH=/opt/root/build/lib:$PYTHONPATH


ARG GEANT4_VERSION
LABEL GEANT4_VERSION=${GEANT4_VERSION}
ENV GEANT4_VERSION=${GEANT4_VERSION}
RUN git clone https://github.com/Geant4/geant4.git && \
    cd geant4 && \
    git config advice.detachedHead false && \
    git checkout ${GEANT4_VERSION} && \
    mkdir build && cd build && \
    cmake -DGEANT4_INSTALL_DATA=ON \
        -DGEANT4_BUILD_MULTITHREADED=ON \
        -DGEANT4_USE_SYSTEM_ZLIB=ON \
        -DGEANT4_USE_OPENGL_X11=ON \
        -DGEANT4_USE_QT=ON \
        -DGEANT4_USE_GDML=ON \
        -DGEANT4_INSTALL_DATA_TIMEOUT=999999 \
        -DGEANT4_BUILD_MUONIC_ATOMS_IN_USE=ON .. && \
    make -j$(nproc)

# General dependencies
RUN apt install -y wget

ARG PYTHIA8_VERSION
LABEL PYTHIA8_VERSION=${PYTHIA8_VERSION}
ENV PYTHIA8_VERSION=${PYTHIA8_VERSION}
RUN wget https://pythia.org/download/pythia83/pythia$PYTHIA8_VERSION.tgz && \
    tar -vxf pythia$PYTHIA8_VERSION.tgz && rm -rf pythia$PYTHIA8_VERSION.tgz && mv pythia$PYTHIA8_VERSION pythia && \
    cd pythia && \
    ./configure --with-python-config=python3-config && \
    make -j$(nproc)

ARG FASTJET_VERSION
LABEL FASTJET_VERSION=${FASTJET_VERSION}
ENV FASTJET_VERSION=${FASTJET_VERSION}
RUN wget http://fastjet.fr/repo/fastjet-$FASTJET_VERSION.tar.gz && \
    tar zxvf fastjet-$FASTJET_VERSION.tar.gz && \
    rm fastjet-$FASTJET_VERSION.tar.gz && \
    mv fastjet-$FASTJET_VERSION fastjet && \
    cd fastjet && \
    ./configure && \
    make -j$(nproc) && make install

ARG HEPMC_VERSION
LABEL HEPMC_VERSION=${HEPMC_VERSION}
ENV HEPMC_VERSION=${HEPMC_VERSION}

# HepMC DHEPMC3_ENABLE_PROTOBUFIO=ON uses this
RUN apt-get install -y protobuf-compiler libprotobuf-dev 

RUN wget http://hepmc.web.cern.ch/hepmc/releases/HepMC3-${HEPMC_VERSION}.tar.gz && \
    tar -xzf HepMC3-${HEPMC_VERSION}.tar.gz && \
    rm HepMC3-${HEPMC_VERSION}.tar.gz && \
    mv HepMC3-${HEPMC_VERSION} hepmc && \
    cd hepmc && mkdir build && \
    cd build && \
    cmake -DHEPMC3_ENABLE_ROOTIO=ON -DHEPMC3_INSTALL_INTERFACES=ON -DHEPMC3_ENABLE_PROTOBUFIO=ON .. && \
    make -j$(nproc) && \
    make install

COPY requirements.txt /requirements.txt
RUN apt-get update -y && \
    apt install -y python3-pip python3-venv && \
    python -m venv /opt/lorenzetti-python && \
    /opt/lorenzetti-python/bin/pip install -r /requirements.txt



ENV PATH=/opt/lorenzetti-python/bin:/opt/geant4/build:/opt/root/build/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
ENV LD_LIBRARY_PATH=opt/geant4/build/BuildProducts/lib/libG4intercoms.so:/opt/geant4/build/BuildProducts/lib/libG4particles.so:/opt/geant4/build/BuildProducts/lib/libG4parmodels.so:/opt/geant4/build/BuildProducts/lib/libG4materials.so:/opt/geant4/build/BuildProducts/lib/libG4interfaces.so:/opt/geant4/build/BuildProducts/lib/libG4track.so:/opt/geant4/build/BuildProducts/lib/libG4graphics_reps.so:/opt/geant4/build/BuildProducts/lib/libG4FR.so:/opt/geant4/build/BuildProducts/lib/libG4tracking.so:/opt/geant4/build/BuildProducts/lib/libG4mctruth.so:/opt/geant4/build/BuildProducts/lib/libG4OpenGL.so:/opt/geant4/build/BuildProducts/lib/libG4geomtext.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so:/opt/geant4/build/BuildProducts/lib/libG4vis_management.so:/opt/geant4/build/BuildProducts/lib/libG4visHepRep.so:/opt/geant4/build/BuildProducts/lib/Linux-g++:/opt/geant4/build/BuildProducts/lib/libG4global.so:/opt/geant4/build/BuildProducts/lib/libG4clhep.so:/opt/geant4/build/BuildProducts/lib/libG4analysis.so:/opt/geant4/build/BuildProducts/lib/libG4Tree.so:/opt/geant4/build/BuildProducts/lib/libG4digits_hits.so:/opt/geant4/build/BuildProducts/lib/libG4modeling.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so.2.3.3:/opt/geant4/build/BuildProducts/lib/libG4geometry.so:/opt/geant4/build/BuildProducts/lib/libG4gdml.so:/opt/geant4/build/BuildProducts/lib/libG4ToolsSG.so:/opt/geant4/build/BuildProducts/lib/libG4event.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so.2:/opt/geant4/build/BuildProducts/lib/libG4GMocren.so:/opt/geant4/build/BuildProducts/lib/libG4physicslists.so:/opt/geant4/build/BuildProducts/lib/libG4readout.so:/opt/geant4/build/BuildProducts/lib/libG4processes.so:/opt/geant4/build/BuildProducts/lib/libG4run.so:/opt/geant4/build/BuildProducts/lib/libG4VRML.so:/opt/geant4/build/BuildProducts/lib/libG4error_propagation.so:/opt/geant4/build/BuildProducts/lib/libG4RayTracer.so:opt/geant4/build/BuildProducts/lib/libG4intercoms.so:/opt/geant4/build/BuildProducts/lib/libG4particles.so:/opt/geant4/build/BuildProducts/lib/libG4parmodels.so:/opt/geant4/build/BuildProducts/lib/libG4materials.so:/opt/geant4/build/BuildProducts/lib/libG4interfaces.so:/opt/geant4/build/BuildProducts/lib/libG4track.so:/opt/geant4/build/BuildProducts/lib/libG4graphics_reps.so:/opt/geant4/build/BuildProducts/lib/libG4FR.so:/opt/geant4/build/BuildProducts/lib/libG4tracking.so:/opt/geant4/build/BuildProducts/lib/libG4mctruth.so:/opt/geant4/build/BuildProducts/lib/libG4OpenGL.so:/opt/geant4/build/BuildProducts/lib/libG4geomtext.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so:/opt/geant4/build/BuildProducts/lib/libG4vis_management.so:/opt/geant4/build/BuildProducts/lib/libG4visHepRep.so:/opt/geant4/build/BuildProducts/lib/Linux-g++:/opt/geant4/build/BuildProducts/lib/libG4global.so:/opt/geant4/build/BuildProducts/lib/libG4clhep.so:/opt/geant4/build/BuildProducts/lib/libG4analysis.so:/opt/geant4/build/BuildProducts/lib/libG4Tree.so:/opt/geant4/build/BuildProducts/lib/libG4digits_hits.so:/opt/geant4/build/BuildProducts/lib/libG4modeling.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so.2.3.3:/opt/geant4/build/BuildProducts/lib/libG4geometry.so:/opt/geant4/build/BuildProducts/lib/libG4gdml.so:/opt/geant4/build/BuildProducts/lib/libG4ToolsSG.so:/opt/geant4/build/BuildProducts/lib/libG4event.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so.2:/opt/geant4/build/BuildProducts/lib/libG4GMocren.so:/opt/geant4/build/BuildProducts/lib/libG4physicslists.so:/opt/geant4/build/BuildProducts/lib/libG4readout.so:/opt/geant4/build/BuildProducts/lib/libG4processes.so:/opt/geant4/build/BuildProducts/lib/libG4run.so:/opt/geant4/build/BuildProducts/lib/libG4VRML.so:/opt/geant4/build/BuildProducts/lib/libG4error_propagation.so:/opt/geant4/build/BuildProducts/lib/libG4RayTracer.so:opt/geant4/build/BuildProducts/lib/libG4intercoms.so:/opt/geant4/build/BuildProducts/lib/libG4particles.so:/opt/geant4/build/BuildProducts/lib/libG4parmodels.so:/opt/geant4/build/BuildProducts/lib/libG4materials.so:/opt/geant4/build/BuildProducts/lib/libG4interfaces.so:/opt/geant4/build/BuildProducts/lib/libG4track.so:/opt/geant4/build/BuildProducts/lib/libG4graphics_reps.so:/opt/geant4/build/BuildProducts/lib/libG4FR.so:/opt/geant4/build/BuildProducts/lib/libG4tracking.so:/opt/geant4/build/BuildProducts/lib/libG4mctruth.so:/opt/geant4/build/BuildProducts/lib/libG4OpenGL.so:/opt/geant4/build/BuildProducts/lib/libG4geomtext.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so:/opt/geant4/build/BuildProducts/lib/libG4vis_management.so:/opt/geant4/build/BuildProducts/lib/libG4visHepRep.so:/opt/geant4/build/BuildProducts/lib/Linux-g++:/opt/geant4/build/BuildProducts/lib/libG4global.so:/opt/geant4/build/BuildProducts/lib/libG4clhep.so:/opt/geant4/build/BuildProducts/lib/libG4analysis.so:/opt/geant4/build/BuildProducts/lib/libG4Tree.so:/opt/geant4/build/BuildProducts/lib/libG4digits_hits.so:/opt/geant4/build/BuildProducts/lib/libG4modeling.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so.2.3.3:/opt/geant4/build/BuildProducts/lib/libG4geometry.so:/opt/geant4/build/BuildProducts/lib/libG4gdml.so:/opt/geant4/build/BuildProducts/lib/libG4ToolsSG.so:/opt/geant4/build/BuildProducts/lib/libG4event.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so.2:/opt/geant4/build/BuildProducts/lib/libG4GMocren.so:/opt/geant4/build/BuildProducts/lib/libG4physicslists.so:/opt/geant4/build/BuildProducts/lib/libG4readout.so:/opt/geant4/build/BuildProducts/lib/libG4processes.so:/opt/geant4/build/BuildProducts/lib/libG4run.so:/opt/geant4/build/BuildProducts/lib/libG4VRML.so:/opt/geant4/build/BuildProducts/lib/libG4error_propagation.so:/opt/geant4/build/BuildProducts/lib/libG4RayTracer.so:
ENV PYTHONPATH=/opt/root/build/lib:/opt/pythia/lib
ENV G4NEUTRONHPDATA=/opt/geant4/build/data/G4NDL4.7.1
ENV G4LEDATA=/opt/geant4/build/data/G4EMLOW8.5
ENV G4LEVELGAMMADATA=/opt/geant4/build/data/PhotonEvaporation5.7
ENV G4RADIOACTIVEDATA=/opt/geant4/build/data/RadioactiveDecay5.6
ENV G4PARTICLEXSDATA=/opt/geant4/build/data/G4PARTICLEXS4.0
ENV G4PIIDATA=/opt/geant4/build/data/G4PII1.3
ENV G4REALSURFACEDATA=/opt/geant4/build/data/RealSurface2.2
ENV G4SAIDXSDATA=/opt/geant4/build/data/G4SAIDDATA2.0
ENV G4ABLADATA=/opt/geant4/build/data/G4ABLA3.3
ENV G4INCLDATA=/opt/geant4/build/data/G4INCL1.2
ENV G4ENSDFSTATEDATA=/opt/geant4/build/data/G4ENSDFSTATE2.3
ENV PYTHIA8_INCLUDE=/opt/pythia/include
ENV PYTHIA8_LIBRARIES=/opt/pythia/lib
ENV FASTJET_INCLUDE=/opt/fastjet/include
ENV FASTJET_LIBRARIES=/usr/local/lib
ENV HEPMC_INCLUDE=/opt/optmc/include/
ENV HEPMC_LIBRARIES=/usr/local/lib
ENV LD_PRELOAD=/opt/geant4/build/BuildProducts/lib/libG4vis_management.so:/opt/geant4/build/BuildProducts/lib/libG4visHepRep.so:/opt/geant4/build/BuildProducts/lib/libG4tracking.so:/opt/geant4/build/BuildProducts/lib/libG4track.so:/opt/geant4/build/BuildProducts/lib/libG4run.so:/opt/geant4/build/BuildProducts/lib/libG4readout.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so:/opt/geant4/build/BuildProducts/lib/libG4processes.so:/opt/geant4/build/BuildProducts/lib/libG4physicslists.so:/opt/geant4/build/BuildProducts/lib/libG4particles.so:/opt/geant4/build/BuildProducts/lib/libG4parmodels.so:/opt/geant4/build/BuildProducts/lib/libG4modeling.so:/opt/geant4/build/BuildProducts/lib/libG4mctruth.so:/opt/geant4/build/BuildProducts/lib/libG4materials.so:/opt/geant4/build/BuildProducts/lib/libG4interfaces.so:/opt/geant4/build/BuildProducts/lib/libG4intercoms.so:/opt/geant4/build/BuildProducts/lib/libG4graphics_reps.so:/opt/geant4/build/BuildProducts/lib/libG4global.so:/opt/geant4/build/BuildProducts/lib/libG4geomtext.so:/opt/geant4/build/BuildProducts/lib/libG4geometry.so:/opt/geant4/build/BuildProducts/lib/libG4gdml.so:/opt/geant4/build/BuildProducts/lib/libG4event.so:/opt/geant4/build/BuildProducts/lib/libG4error_propagation.so:/opt/geant4/build/BuildProducts/lib/libG4digits_hits.so:/opt/geant4/build/BuildProducts/lib/libG4clhep.so:/opt/geant4/build/BuildProducts/lib/libG4analysis.so:/opt/geant4/build/BuildProducts/lib/libG4VRML.so:/opt/geant4/build/BuildProducts/lib/libG4Tree.so:/opt/geant4/build/BuildProducts/lib/libG4ToolsSG.so:/opt/geant4/build/BuildProducts/lib/libG4RayTracer.so:/opt/geant4/build/BuildProducts/lib/libG4OpenGL.so:/opt/geant4/build/BuildProducts/lib/libG4GMocren.so:/opt/geant4/build/BuildProducts/lib/libG4FR.so:

#ARG LORENZETTI_BUILD_DIR=""
#ENV LORENZETTI_BUILD_DIR=${LORENZETTI_BUILD_DIR}
#COPY entrypoint.sh /opt/entrypoint.sh
