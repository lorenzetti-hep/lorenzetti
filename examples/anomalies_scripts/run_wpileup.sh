#!/bin/bash

n_workers=40 && \
NOV=100000 && \ls
seed=3973534 && \
base_dir="/sps/atlas/l/lboggia/lorenzetti/2025_06_04_100k_jets" && \
evt_dir="/sps/atlas/l/lboggia/lorenzetti/2025_05_06_100k_jets/EVT" && \
hit_dir="${base_dir}/HIT_wpileup40" && \
esd_dir="${base_dir}/ESD_wpileup40" && \
aod_dir="${base_dir}/AOD_wpileup40" && \
ntuple_dir="${base_dir}/NTUPLE_wpileup40" && \
cd "/sps/atlas/l/lboggia/lorenzetti/build" && source lzt_setup.sh

mb_file_path="/sps/atlas/l/lboggia/lorenzetti/mb.HIT.9.root"
# merge with minbias
mkdir -p $mb_dir && cd $mb_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started MINBIAS sim" |& tee "${base_dir}/started_MINBIAS.log" && \
(merge_trf.py -i $hit_dir -o "jets.HIT.root" -p $mb_file_path -nt $n_workers |& tee "${base_dir}/jets.MB.log") && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished MINBIAS sim" |& tee "${base_dir}/finished_MINBIAS.log"
# digitalization, loop over files to avoid memory issues
n_files=131
mkdir -p $esd_dir && cd $esd_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started ESD with pileup sim" |& tee "${base_dir}/started_ESD_wpileup.log" && \
for j in $(seq 0 $n_files)
do
    echo "$j"
    echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Processing file $j of $n_files" |& tee "${base_dir}/started_ESD_wpileup.log" && \
    # run the digitization
    (digit_trf.py -i $hit_dir/jets.HIT.$j.root -o "jets.ESD.$j.root" -nt $n_workers --doDefects --noiseFactor 10 |& tee "${base_dir}/jets_wpileup.ESD.log") && \
    echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished file $j of $n_files" |& tee "${base_dir}/finished_ESD_wpileup.log"
done
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished ESD with pileup sim" |& tee "${base_dir}/finished_ESD_wpileup.log"
# reconstruction
mkdir -p $aod_dir && cd $aod_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started AOD with pileup sim" |& tee "${base_dir}/started_AOD_wpileup.log" && \
(reco_trf.py -i $esd_dir -o "jets.AOD.root" -nt $n_workers |& tee "${base_dir}/jets_wpileup.AOD.log" )  && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished AOD with pileup sim" |& tee "${base_dir}/finished_AOD_wpileup.log"
# ntuple
# mkdir -p $ntuple_dir && cd $ntuple_dir && \
# echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started NTUPLE with pileup sim" |& tee "${base_dir}/started_NTUPLE_wpileup.log" && \
# (ntuple_trf.py -i $aod_dir -o "jets.NTUPLE.root" -nt $n_workers |& tee "${base_dir}/jets.NTUPLE.log" )  && \
# echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished NTUPLE with pileup sim" |& tee "${base_dir}/finished_NTUPLE_wpileup.log"

cd $call_dir