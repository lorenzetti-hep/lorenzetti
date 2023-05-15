
export LC_ALL=''
export RCM_NO_COLOR=0
export RCM_GRID_ENV=0
export LZT_PATH=$PWD

cd build


#
# Link all pcms
#
rm -rf lib
mkdir lib
# first level pcm
for file in "`pwd`"/*/*/*/*.pcm
do
  echo "ln -sf $file"
  ln -sf $file
done

for file in "`pwd`"/*/*/*.pcm
do
  echo "ln -sf $file"
  ln -sf $file
done

#
# Link all libs
#
if [[ "$OSTYPE" == "darwin"* ]]; then
  for file in "`pwd`"/*.dylib
  do
    echo "ln -sf $file lib"
    ln -sf $file lib
  done
else # Unix system
  for file in "`pwd`"/*.so
  do
    echo "ln -sf $file lib"
    ln -sf $file lib
  done
fi




#
# copy all executables from generator package
#
rm -rf executables
mkdir executables 
for file in $PWD/*/*/*.exe
do
  echo "copy executable: $file"
  ln -s $file executables
done

#
# Add libs and python to the path
#
export LD_LIBRARY_PATH=$PWD/lib:$LD_LIBRARY_PATH
export PATH=$PWD/lib:$PATH:$PWD:$PWD/executables
export PYTHONPATH=$PWD/python:$PYTHONPATH



# back to the basepath
cd $LZT_PATH

#
# add scripts and executables to the $PATH
#
export PATH=$PATH:$PWD/core/GaugiKernel/scripts
export PATH=$PATH:$PWD/generator/scripts
export PATH=$PATH:$PWD/geometry/DetectorATLASModel/scripts
export PATH=$PATH:$PWD/scripts

