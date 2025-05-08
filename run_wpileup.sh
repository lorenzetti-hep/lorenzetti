#!/bin/bash

call_dir="${PWD}"
echo "Loading Lorenzetti in $call_dir, print ${PWD}"
lzt_repo="/sps/atlas/l/lboggia/lorenzetti"
build_dir="/sps/atlas/l/lboggia/lorenzetti/build"
cd $lzt_repo && (make -j4 |& tee $log_path) && rm -r build/lib
echo "${PWD}"
cd $call_dir

n_workers=4 && \
NOV=200 && \ls
seed=3973534 && \
base_dir="/sps/atlas/l/lboggia/test_lorenzetti/lorenzetti/2025_05_02_2k_jets" && \
evt_dir="${base_dir}/EVT" && \
hit_dir="${base_dir}/HIT" && \
mb_dir="${base_dir}/MB_wpileup" && \
esd_dir="${base_dir}/ESD_wpileup" && \
aod_dir="${base_dir}/AOD_wpileup" && \
ntuple_dir="${base_dir}/NTUPLE_wpileup" && \
cd "/sps/atlas/l/lboggia/lorenzetti/build" && source lzt_setup.sh

mb_file_path="/sps/atlas/l/lboggia/lorenzetti/mb.HIT.9.root"

# merge with minbias
mkdir -p $mb_dir && cd $mb_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started MINBIAS sim" |& tee "${base_dir}/started_MINBIAS.log" && \
(merge_trf.py -i $hit_dir -o "jets.HIT.root" -p $mb_file_path -nt $n_workers |& tee "${base_dir}/jets.MB.log") && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished MINBIAS sim" |& tee "${base_dir}/finished_MINBIAS.log"
# digitalization
mkdir -p $esd_dir && cd $esd_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started ESD with pileup sim" |& tee "${base_dir}/started_ESD_wpileup.log" && \
(digit_trf.py -i $mb_dir -o "jets.ESD.root" -nt $n_workers |& tee "${base_dir}/jets_wpileup.ESD.log") && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished ESD with pileup sim" |& tee "${base_dir}/finished_ESD_wpileup.log"
# reconstruction
mkdir -p $aod_dir && cd $aod_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started AOD with pileup sim" |& tee "${base_dir}/started_AOD_wpileup.log" && \
(reco_trf.py -i $esd_dir -o "jets.AOD.root" -nt $n_workers -m |& tee "${base_dir}/jets_wpileup.AOD.log" )  && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished AOD with pileup sim" |& tee "${base_dir}/finished_AOD_wpileup.log"
# ntuple
mkdir -p $ntuple_dir && cd $ntuple_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started NTUPLE with pileup sim" |& tee "${base_dir}/started_NTUPLE_wpileup.log" && \
(ntuple_trf.py -i $aod_dir -o "jets.NTUPLE.root" -nt $n_workers -m |& tee "${base_dir}/jets_wpileup.NTUPLE.log" )  && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished NTUPLE with pileup sim" |& tee "${base_dir}/finished_NTUPLE_wpileup.log"

cd $call_dir