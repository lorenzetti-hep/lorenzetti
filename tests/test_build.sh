
cd
cd lorenzetti
CPU_N=$(grep -c ^processor /proc/cpuinfo)
mkdir build
cd build
echo "Start compilation..."
cmake ..
make -j$CPU_N
cd ..
source setup.sh
echo "done!"