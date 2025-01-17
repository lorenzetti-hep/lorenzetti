TAG=$1
git clone https://github.com/root-project/root.git && cd root && git checkout $TAG
cd root/
mkdir build && cd build && cmake  -Dpython_version=3 -Dxrootd=OFF -Dbuiltin_xrootd=OFF .. && make -j$(nproc)

