
FROM ubuntu:22.04
LABEL maintainer "Joao Victor Pinto <jodafons@cern.ch>"

# HEP packages
ENV GEANT4_VERSION=v11.2.2
ENV ROOT_VERSION=v6-34-00
ENV FASTJET_VERSION=3.4.0
ENV HEPMC_VERSION=3.3.0
ENV PYTHIA8_VERSION=8313

ENV LC_ALL=C.UTF-8
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV TERM=screen
ENV TZ=America/New_York
ENV DEBIAN_FRONTEND=noninteractive

RUN mkdir -p /hep
WORKDIR /hep

COPY scripts/install_base.sh /hep
RUN bash install_base.sh

COPY scripts/install_root.sh /hep
RUN bash install_root.sh ${ROOT_VERSION}
ENV ROOTSYS="/hep/root/build/"
ENV PATH="$ROOTSYS/bin:$PATH"
ENV LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"
ENV PYTHONPATH="/hep/root/build/lib:$PYTHONPATH"

COPY scripts/install_geant.sh /hep
RUN bash install_geant.sh ${GEANT4_VERSION}
COPY scripts/setup_geant.py /hep/geant4/build
RUN cd geant4/build && python setup_geant.py && bash setup_geant.sh

COPY scripts/install_pythia.sh /hep
RUN bash install_pythia.sh ${PYTHIA8_VERSION}

COPY scripts/install_fastjet.sh /hep
RUN bash install_fastjet.sh ${FASTJET_VERSION}

COPY scripts/install_hepmc.sh /hep
RUN bash install_hepmc.sh ${HEPMC_VERSION}

RUN rm /hep/*.sh
COPY scripts/setup_hep.sh /hep
RUN chmod 774 setup_hep.sh

COPY requirements.txt .
RUN pip install -r requirements.txt

WORKDIR /home