#!/bin/bash

# call_dir="${PWD}"
# echo "Loading Lorenzetti in $call_dir, print ${PWD}"
# lzt_repo="/sps/atlas/l/lboggia/lorenzetti"
# build_dir="/sps/atlas/l/lboggia/lorenzetti/build"
# cd $lzt_repo && (make -j4 |& tee $log_path) && rm -r build/lib
# echo "${PWD}"
# cd $call_dir

# n_workers=12 && \
NOV=100000 && \ls
seed=3973534 && \
base_dir="/sps/atlas/l/lboggia/lorenzetti/2025_06_04_100k_jets" && \
evt_dir="/sps/atlas/l/lboggia/lorenzetti/2025_05_06_100k_jets/EVT" && \
hit_dir="${base_dir}/HIT" && \
esd_dir="${base_dir}/ESD" && \
aod_dir="${base_dir}/AOD" && \
ntuple_dir="${base_dir}/NTUPLE" && \
cd "/sps/atlas/l/lboggia/lorenzetti/build" && source lzt_setup.sh
# generate events with pythia
# mkdir -p $evt_dir && cd $evt_dir && \
# echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started EVT sim" |& tee "${base_dir}/started_EVT.log" && \
# (gen_jets.py --output-file jets.EVT.root --nov $NOV --pileup-avg 0 --seed $seed -nt $n_workers --events-per-job 500 |& tee "${base_dir}/jets.EVT.log") && \
# echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished EVT sim" |& tee "${base_dir}/finished_EVT.log" && \
# generate hits around the truth particle seed
# mkdir -p $hit_dir && cd $hit_dir && \
# echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started HIT sim" |& tee "${base_dir}/started_HIT.log" && \
# (simu_trf.py -i $evt_dir -o "jets.HIT.root" -t 5 --doDefects |& tee "${base_dir}/jets_wnoise.HIT.log") && \
# echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished HIT sim" |& tee "${base_dir}/finished_HIT.log"
# digitalization
n_files=199
mkdir -p $esd_dir && cd $esd_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started ESD sim" |& tee "${base_dir}/started_ESD.log" && \
for j in $(seq 0 $n_files)
do
    echo "$j"
    echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Processing file $j of $n_files" |& tee "${base_dir}/started_ESD.log" && \
    # run the digitization
    (digit_trf.py -i $hit_dir/jets.HIT.$j.root -o "jets.ESD.$j.root" --doDefects --noiseFactor 20 -m |& tee "${base_dir}/jets_wnoise.ESD.log") && \
    echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished file $j of $n_files" |& tee "${base_dir}/finished_ESD.log"
done
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished ESD sim" |& tee "${base_dir}/finished_ESD.log"
# reconstruction
mkdir -p $aod_dir && cd $aod_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started AOD sim" |& tee "${base_dir}/started_AOD.log" && \
(reco_trf.py -i $esd_dir -o "jets.AOD.root" |& tee "${base_dir}/jets_wnoise.AOD.log" )  && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished AOD sim" |& tee "${base_dir}/finished_AOD.log"
# ntuple
# mkdir -p $ntuple_dir && cd $ntuple_dir && \
# echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started NTUPLE sim" |& tee "${base_dir}/started_NTUPLE.log" && \
# (ntuple_trf.py -i $aod_dir -o "jets.NTUPLE.root" -nt $n_workers |& tee "${base_dir}/jets.NTUPLE.log" )  && \
# echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished NTUPLE sim" |& tee "${base_dir}/finished_NTUPLE.log"

cd $call_dir