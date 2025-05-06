#!/bin/bash

# no need to re-build if build is up to daye
call_dir="${PWD}"
# echo "Loading Lorenzetti in $call_dir, print ${PWD}"
# lzt_repo="/sps/atlas/l/lboggia/lorenzetti"
# build_dir="/sps/atlas/l/lboggia/lorenzetti/build"
# cd $lzt_repo && (make -j4 |& tee $log_path) && rm -r build/lib
# echo "${PWD}"
# cd $call_dir

n_workers=4 && \
NOV=2000 && \ls
seed=3973534 && \
base_dir="/sps/atlas/l/lboggia/lorenzetti/2025_05_02_2k_jets" && \
mb_dir="${base_dir}/MB_wpileup" && \
esd_dir="${base_dir}/ESD_wanomalies" && \
aod_dir="${base_dir}/AOD_wanomalies" && \
ntuple_dir="${base_dir}/NTUPLE_wanomalies" && \
cd "/sps/atlas/l/lboggia/lorenzetti/build" && source lzt_setup.sh

# # digitalization
# mkdir -p $esd_dir && cd $esd_dir && \
# echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started ESD with anomalies sim" |& tee "${base_dir}/started_ESD_wanomalies.log" && \
# (digit_trf.py -i $mb_dir -o "jets.ESD.root" -nt $n_workers -m |& tee "${base_dir}/jets_wanomalies.ESD.log") && \
# echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished ESD with anomalies sim" |& tee "${base_dir}/finished_ESD_wanomalies.log"
# reconstruction
mkdir -p $aod_dir && cd $aod_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started AOD with anomalies sim" |& tee "${base_dir}/started_AOD_wanomalies.log" && \
(reco_trf.py -i $esd_dir -o "jets.AOD.root" -nt $n_workers |& tee "${base_dir}/jets_wanomalies.AOD.log" )  && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished AOD with anomalies sim" |& tee "${base_dir}/finished_AOD_wanomalies.log"
# ntuple
mkdir -p $ntuple_dir && cd $ntuple_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started NTUPLE with anomalies sim" |& tee "${base_dir}/started_NTUPLE_wanomalies.log" && \
(ntuple_trf.py -i $aod_dir -o "jets.NTUPLE.root" -nt $n_workers |& tee "${base_dir}/jets_wanomalies.NTUPLE.log" )  && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished NTUPLE with anomalies sim" |& tee "${base_dir}/finished_NTUPLE_wanomalies.log"

cd $call_dir