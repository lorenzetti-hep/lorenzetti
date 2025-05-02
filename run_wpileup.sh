call_dir=$PWD
n_workers=2 && \
NOV=2000 && \
seed=3973534 && \
base_dir="/sps/atlas/l/lboggia/lorenzetti/2025_04_29_2k_jets_wpileup" && \
# evt_dir="${base_dir}/EVT" && \
# hit_dir="${base_dir}/HIT" && \
mb_dir="${base_dir}/MB" && \
esd_dir="${base_dir}/ESD" && \
aod_dir="${base_dir}/AOD" && \
ntuple_dir="${base_dir}/NTUPLE" && \
cd "/sps/atlas/l/lboggia/lorenzetti/build" && source lzt_setup.sh

hit_dir="/sps/atlas/l/lboggia/lorenzetti/2025_04_29_2k_jets/HIT"
# merge with minbias
mkdir -p $mb_dir && cd $mb_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started MINBIAS sim" |& tee "${base_dir}/started_MINBIAS.log" && \
(merge_trf.py -i $hit_dir -o "jets_wpileup.HIT.root" -p "/sps/atlas/l/lboggia/lorenzetti/mb.HIT.9.root" -nt $n_workers $|& tee "${base_dir}/jets.MB.log") && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished MINBIAS sim" |& tee "${base_dir}/finished_MINBIAS.log"
# digitalization
mkdir -p $esd_dir && cd $esd_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started ESD sim" |& tee "${base_dir}/started_ESD.log" && \
(digit_trf.py -i $mb_dir -o "jets.ESD.root" -nt $n_workers |& tee "${base_dir}/jets.ESD.log") && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished ESD sim" |& tee "${base_dir}/finished_ESD.log"
# reconstruction
mkdir -p $aod_dir && cd $aod_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started AOD sim" |& tee "${base_dir}/started_AOD.log" && \
(reco_trf.py -i $esd_dir -o "jets.AOD.root" -nt $n_workers |& tee "${base_dir}/jets.AOD.log" )  && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished AOD sim" |& tee "${base_dir}/finished_AOD.log"
# ntuple
mkdir -p $ntuple_dir && cd $ntuple_dir && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Started NTUPLE sim" |& tee "${base_dir}/started_NTUPLE.log" && \
(ntuple_trf.py -i $aod_dir -o "jets.NTUPLE.root" -nt $n_workers |& tee "${base_dir}/jets.NTUPLE.log" )  && \
echo "$(date -d "today" +"%Y/%m/%d %H-%M-%s") - Finished NTUPLE sim" |& tee "${base_dir}/finished_NTUPLE.log"

cd $call_dir