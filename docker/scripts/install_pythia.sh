TAG=$1
wget https://pythia.org/download/pythia83/pythia$TAG.tgz
tar -vxf pythia$TAG.tgz && rm -rf pythia$TAG.tgz && mv pythia$TAG pythia
cd pythia
./configure --with-python-config=python3-config
make -j$(nproc)