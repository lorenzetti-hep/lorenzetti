

rm -rf build
mkdir build
cd build
ln -s ../Gaugi/python Gaugi
ln -s ../lorenzet/python lorenzet
export PYTHONPATH=`pwd`:$PYTHONPATH
export PYTHONPATH=`pwd`:$PYTHONPATH
cd ..
#export PATH=`pwd`/scripts:$PATH



