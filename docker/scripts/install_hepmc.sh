TAG=$1
git clone https://gitlab.cern.ch/hepmc/HepMC3.git
mv HepMC3 hepmc && cd hepmc
git checkout $TAG
mkdir build && cd build
cmake -DHEPMC3_ENABLE_ROOTIO=ON -DHEPMC3_INSTALL_INTERFACES=ON -DHEPMC3_ENABLE_PROTOBUFIO=ON .. && make -j$(nproc) && make install
