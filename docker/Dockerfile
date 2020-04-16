FROM ubuntu

USER root
SHELL [ "/bin/bash", "-c" ]
RUN apt-get update --fix-missing
RUN apt-get install -y python3.7 python3-pip



RUN apt-get install -y git \
                       wget \
                       dpkg-dev \
                       g++ \
                       gcc \
                       binutils \
                       libx11-dev \
                       libxpm-dev \
                       libxft-dev \
                       libxext-dev \
                       gfortran \
                       libssl-dev \
                       libpcre3-dev \
                       libglew1.5-dev \
                       libftgl-dev \
                       libldap2-dev \
                       python-dev \
                       libxml2-dev \
                       libkrb5-dev \
                       libgsl0-dev \
                       libqt4-dev \
                       libfftw3-dev \
                       libcfitsio-dev \
                       graphviz-dev \
                       libavahi-compat-libdnssd-dev \
                       libboost-all-dev \
                       vim \
                       qt4-default \
                       librange-v3-dev \
                       libboost-python-dev


RUN git clone https://github.com/amix/vimrc.git ${HOME}/.vim_runtime
RUN cd ${HOME}/.vim_runtime/ && source install_awesome_vimrc.sh 
RUN wget https://cmake.org/files/v3.8/cmake-3.8.2-Linux-x86_64.sh -P /usr/
RUN chmod 755 /usr/cmake-3.8.2-Linux-x86_64.sh
RUN cd /usr && ./cmake-3.8.2-Linux-x86_64.sh --skip-license

RUN ln -sf /usr/bin/python3.6 /usr/bin/python


# Install ROOT
RUN cd /opt && git clone https://github.com/root-project/root.git && \
cd root && git checkout v6-16-00 && mkdir buildthis && cd buildthis && cmake  --Dpython_version=3 .. && make -j16
ENV ROOTSYS "/opt/root/buildthis/"
ENV PATH "$ROOTSYS/bin:$PATH"
ENV LD_LIBRARY_PATH "$ROOTSYS/lib:$LD_LIBRARY_PATH"
ENV PYTHONPATH "/opt/root_cern/build/lib:$PYTHONPATH"


# Install Geant4
RUN apt-get install -y libxerces-c-dev
RUN ls && cd /opt && git clone https://github.com/jodafons/geant4.git && cd geant4 && mkdir buildthis && cd buildthis && cmake -DGEANT4_INSTALL_DATA=ON \
 -DGEANT4_BUILD_MULTITHREADED=ON -DGEANT4_USE_SYSTEM_ZLIB=ON -DGEANT4_USE_QT=ON -DGEANT4_USE_GDML=ON -DGEANT4_BUILD_MUONIC_ATOMS_IN_USE=ON .. && make -j16 && \
cp ../scripts/geant4_10.5.1.sh geant4.sh && source geant4.sh


# Install HEPMC
RUN cd /opt && git clone https://github.com/jodafons/hepmc.git && cd hepmc && ./configure --with-momentum=MEV --with-length=CM && make -j10 && make install


# Install Pythia8
RUN cd /opt && git clone https://github.com/jodafons/pythia.git && cd pythia && mv README.md README && ./configure && make -j8

# Install FastJet
RUN apt install curl -y
RUN cd /opt && curl -O http://fastjet.fr/repo/fastjet-3.3.3.tar.gz && tar zxvf fastjet-3.3.3.tar.gz && cd fastjet-3.3.3/ && ./configure && make -j10 && make install



COPY setup_envs.sh /
RUN chmod 777 /setup_envs.sh


