TAG=$1
wget http://fastjet.fr/repo/fastjet-$TAG.tar.gz && tar zxvf fastjet-$TAG.tar.gz && rm fastjet-$TAG.tar.gz
mv fastjet-$TAG fastjet
cd fastjet
./configure
make -j$(nproc) && make install
