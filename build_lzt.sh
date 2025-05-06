call_dir=$PWD
echo "Building Lorenzetti in $call_dir"
logs_dir="${call_dir}/logs/lorenzetti"
log_path="${logs_dir}/lorenzetti_build_$(date -d "today" +"%Y_%m_%d_%H_%M_%s").log"
mkdir -p $logs_dir
# |& redirects stderr to stdout from the previous command do the stdin of the next command
# in this case redirects stderr and stdout to tee, which writes to a file and stdout
lzt_repo="${call_dir}"
build_dir=$lzt_repo/build
if [ -d "$build_dir" ] && [ "$1" == "overwrite" ]; then
    rm -r $build_dir
fi
cd $lzt_repo && (make -j4 |& tee $log_path) && rm -r build/lib
cd $call_dir