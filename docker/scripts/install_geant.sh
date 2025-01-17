
TAG=$1
git clone https://github.com/Geant4/geant4.git
cd geant4 && git checkout $TAG
mkdir build && cd build
cmake -DGEANT4_INSTALL_DATA=ON \
      -DGEANT4_BUILD_MULTITHREADED=ON \
      -DGEANT4_USE_SYSTEM_ZLIB=ON \
      -DGEANT4_USE_OPENGL_X11=ON \
      -DGEANT4_USE_QT=ON \
      -DGEANT4_USE_GDML=ON \
      -DGEANT4_INSTALL_DATA_TIMEOUT=999999 \
      -DGEANT4_BUILD_MUONIC_ATOMS_IN_USE=ON ..
make -j$(nproc)

