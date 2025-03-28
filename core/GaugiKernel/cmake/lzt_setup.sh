

CURRENT_PATH=$PWD
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

cd $SCRIPT_DIR



# Link all pcms
rm -rf lib
mkdir lib
# first level pcm
for file in "`pwd`"/*/*/*/*.pcm
do
  #echo "ln -sf $file"
  ln -sf $file
done

for file in "`pwd`"/*/*/*.pcm
do
  #echo "ln -sf $file"
  ln -sf $file
done


# Link all libs
if [[ "$OSTYPE" == "darwin"* ]]; then
  for file in "`pwd`"/*.dylib
  do
    #echo "ln -sf $file lib"
    ln -sf $file lib
  done
else # Unix system
  for file in "`pwd`"/*.so
  do
    #echo "ln -sf $file lib"
    ln -sf $file lib
  done
fi

# link all executables from generator package
rm -rf executables
mkdir executables 
for file in $PWD/*/*/*.exe
do
  #echo "copy executable: $file"
  ln -s $file executables
done

#
# Add libs and python to the path
#
export LD_LIBRARY_PATH=$PWD/lib:$LD_LIBRARY_PATH
export PATH=$PWD/lib:$PATH:$PWD:$PWD/executables:$PWD/scripts/filters:$PWD/scripts/reco:$PWD/scripts/ATLAS:$PWD/scripts/scripts
export PYTHONPATH=$PWD/python:$PYTHONPATH
export LORENZETTI_SCRIPTS_DIR=$PWD/scripts
export LORENZETTI_EVTGEN_DATA_DIR=$PWD/data/evtgen
export LORENZETTI_ATLAS_DATA_DIR=$PWD/data/ATLAS
export LC_ALL=''
export RCM_NO_COLOR=0
export RCM_GRID_ENV=0

echo "Lorenzetti framework set."
cd $CURRENT_PATH