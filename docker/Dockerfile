ARG ROOT_VERSION=6.34.00-ubuntu24.10
FROM rootproject/root:${ROOT_VERSION}

WORKDIR /opt

# GEANT4 uses this
RUN apt install -y \
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

ARG GEANT4_VERSION
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
RUN wget https://pythia.org/download/pythia83/pythia$PYTHIA8_VERSION.tgz && \
    tar -vxf pythia$PYTHIA8_VERSION.tgz && rm -rf pythia$PYTHIA8_VERSION.tgz && mv pythia$PYTHIA8_VERSION pythia && \
    cd pythia && \
    ./configure --with-python-config=python3-config && \
    make -j$(nproc)

ARG FASTJET_VERSION
RUN wget http://fastjet.fr/repo/fastjet-$FASTJET_VERSION.tar.gz && \
    tar zxvf fastjet-$FASTJET_VERSION.tar.gz && \
    rm fastjet-$FASTJET_VERSION.tar.gz && \
    mv fastjet-$FASTJET_VERSION fastjet && \
    cd fastjet && \
    ./configure && \
    make -j$(nproc) && make install

ARG HEPMC_VERSION
RUN wget http://hepmc.web.cern.ch/hepmc/releases/HepMC3-${HEPMC_VERSION}.tar.gz && \
    tar -xzf HepMC3-${HEPMC_VERSION}.tar.gz && \
    rm HepMC3-${HEPMC_VERSION}.tar.gz && \
    mv HepMC3-${HEPMC_VERSION} hepmc && \
    cd hepmc && mkdir build && \
    cd build && \
    cmake -DHEPMC3_ENABLE_ROOTIO=ON -DHEPMC3_INSTALL_INTERFACES=ON -DHEPMC3_ENABLE_PROTOBUFIO=ON .. \
    make && \
    make install

COPY docker/scripts/setup_hep.sh setup_hep.sh
RUN chmod 774 setup_hep.sh

COPY docker/scripts/setup_geant.py /opt/geant4/build
RUN cd /opt/geant4/build/ && chmod 774 setup_geant.py && python setup_geant.py

COPY docker/requirements.txt requirements.txt
RUN apt-get update -y && \
    apt install -y python3-pip python3.12-venv && \
    python -m venv /opt/lorenzetti-python && \
    /opt/lorenzetti-python/bin/pip install -r requirements.txt

ENV PATH=/opt/lorenzetti-python/bin:/opt/geant4/build:/opt/root/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
ENV LD_LIBRARY_PATH=opt/geant4/build/BuildProducts/lib/libG4intercoms.so:/opt/geant4/build/BuildProducts/lib/libG4particles.so:/opt/geant4/build/BuildProducts/lib/libG4parmodels.so:/opt/geant4/build/BuildProducts/lib/libG4materials.so:/opt/geant4/build/BuildProducts/lib/libG4interfaces.so:/opt/geant4/build/BuildProducts/lib/libG4track.so:/opt/geant4/build/BuildProducts/lib/libG4graphics_reps.so:/opt/geant4/build/BuildProducts/lib/libG4FR.so:/opt/geant4/build/BuildProducts/lib/libG4tracking.so:/opt/geant4/build/BuildProducts/lib/libG4mctruth.so:/opt/geant4/build/BuildProducts/lib/libG4OpenGL.so:/opt/geant4/build/BuildProducts/lib/libG4geomtext.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so:/opt/geant4/build/BuildProducts/lib/libG4vis_management.so:/opt/geant4/build/BuildProducts/lib/libG4visHepRep.so:/opt/geant4/build/BuildProducts/lib/Linux-g++:/opt/geant4/build/BuildProducts/lib/libG4global.so:/opt/geant4/build/BuildProducts/lib/libG4clhep.so:/opt/geant4/build/BuildProducts/lib/libG4analysis.so:/opt/geant4/build/BuildProducts/lib/libG4Tree.so:/opt/geant4/build/BuildProducts/lib/libG4digits_hits.so:/opt/geant4/build/BuildProducts/lib/libG4modeling.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so.2.3.3:/opt/geant4/build/BuildProducts/lib/libG4geometry.so:/opt/geant4/build/BuildProducts/lib/libG4gdml.so:/opt/geant4/build/BuildProducts/lib/libG4ToolsSG.so:/opt/geant4/build/BuildProducts/lib/libG4event.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so.2:/opt/geant4/build/BuildProducts/lib/libG4GMocren.so:/opt/geant4/build/BuildProducts/lib/libG4physicslists.so:/opt/geant4/build/BuildProducts/lib/libG4readout.so:/opt/geant4/build/BuildProducts/lib/libG4processes.so:/opt/geant4/build/BuildProducts/lib/libG4run.so:/opt/geant4/build/BuildProducts/lib/libG4VRML.so:/opt/geant4/build/BuildProducts/lib/libG4error_propagation.so:/opt/geant4/build/BuildProducts/lib/libG4RayTracer.so:opt/geant4/build/BuildProducts/lib/libG4intercoms.so:/opt/geant4/build/BuildProducts/lib/libG4particles.so:/opt/geant4/build/BuildProducts/lib/libG4parmodels.so:/opt/geant4/build/BuildProducts/lib/libG4materials.so:/opt/geant4/build/BuildProducts/lib/libG4interfaces.so:/opt/geant4/build/BuildProducts/lib/libG4track.so:/opt/geant4/build/BuildProducts/lib/libG4graphics_reps.so:/opt/geant4/build/BuildProducts/lib/libG4FR.so:/opt/geant4/build/BuildProducts/lib/libG4tracking.so:/opt/geant4/build/BuildProducts/lib/libG4mctruth.so:/opt/geant4/build/BuildProducts/lib/libG4OpenGL.so:/opt/geant4/build/BuildProducts/lib/libG4geomtext.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so:/opt/geant4/build/BuildProducts/lib/libG4vis_management.so:/opt/geant4/build/BuildProducts/lib/libG4visHepRep.so:/opt/geant4/build/BuildProducts/lib/Linux-g++:/opt/geant4/build/BuildProducts/lib/libG4global.so:/opt/geant4/build/BuildProducts/lib/libG4clhep.so:/opt/geant4/build/BuildProducts/lib/libG4analysis.so:/opt/geant4/build/BuildProducts/lib/libG4Tree.so:/opt/geant4/build/BuildProducts/lib/libG4digits_hits.so:/opt/geant4/build/BuildProducts/lib/libG4modeling.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so.2.3.3:/opt/geant4/build/BuildProducts/lib/libG4geometry.so:/opt/geant4/build/BuildProducts/lib/libG4gdml.so:/opt/geant4/build/BuildProducts/lib/libG4ToolsSG.so:/opt/geant4/build/BuildProducts/lib/libG4event.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so.2:/opt/geant4/build/BuildProducts/lib/libG4GMocren.so:/opt/geant4/build/BuildProducts/lib/libG4physicslists.so:/opt/geant4/build/BuildProducts/lib/libG4readout.so:/opt/geant4/build/BuildProducts/lib/libG4processes.so:/opt/geant4/build/BuildProducts/lib/libG4run.so:/opt/geant4/build/BuildProducts/lib/libG4VRML.so:/opt/geant4/build/BuildProducts/lib/libG4error_propagation.so:/opt/geant4/build/BuildProducts/lib/libG4RayTracer.so:opt/geant4/build/BuildProducts/lib/libG4intercoms.so:/opt/geant4/build/BuildProducts/lib/libG4particles.so:/opt/geant4/build/BuildProducts/lib/libG4parmodels.so:/opt/geant4/build/BuildProducts/lib/libG4materials.so:/opt/geant4/build/BuildProducts/lib/libG4interfaces.so:/opt/geant4/build/BuildProducts/lib/libG4track.so:/opt/geant4/build/BuildProducts/lib/libG4graphics_reps.so:/opt/geant4/build/BuildProducts/lib/libG4FR.so:/opt/geant4/build/BuildProducts/lib/libG4tracking.so:/opt/geant4/build/BuildProducts/lib/libG4mctruth.so:/opt/geant4/build/BuildProducts/lib/libG4OpenGL.so:/opt/geant4/build/BuildProducts/lib/libG4geomtext.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so:/opt/geant4/build/BuildProducts/lib/libG4vis_management.so:/opt/geant4/build/BuildProducts/lib/libG4visHepRep.so:/opt/geant4/build/BuildProducts/lib/Linux-g++:/opt/geant4/build/BuildProducts/lib/libG4global.so:/opt/geant4/build/BuildProducts/lib/libG4clhep.so:/opt/geant4/build/BuildProducts/lib/libG4analysis.so:/opt/geant4/build/BuildProducts/lib/libG4Tree.so:/opt/geant4/build/BuildProducts/lib/libG4digits_hits.so:/opt/geant4/build/BuildProducts/lib/libG4modeling.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so.2.3.3:/opt/geant4/build/BuildProducts/lib/libG4geometry.so:/opt/geant4/build/BuildProducts/lib/libG4gdml.so:/opt/geant4/build/BuildProducts/lib/libG4ToolsSG.so:/opt/geant4/build/BuildProducts/lib/libG4event.so:/opt/geant4/build/BuildProducts/lib/libG4ptl.so.2:/opt/geant4/build/BuildProducts/lib/libG4GMocren.so:/opt/geant4/build/BuildProducts/lib/libG4physicslists.so:/opt/geant4/build/BuildProducts/lib/libG4readout.so:/opt/geant4/build/BuildProducts/lib/libG4processes.so:/opt/geant4/build/BuildProducts/lib/libG4run.so:/opt/geant4/build/BuildProducts/lib/libG4VRML.so:/opt/geant4/build/BuildProducts/lib/libG4error_propagation.so:/opt/geant4/build/BuildProducts/lib/libG4RayTracer.so:
ENV PYTHONPATH=/opt/root/lib:/opt/pythia/lib
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
