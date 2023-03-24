

seed=2048
RunNumber=1406030

mkdir -p mb/HIT
mkdir -p mb/EVT
cd mb/EVT
# multi processing command

prun_evts.py -c "gen_minbias.py --pileupAvg 60 --pileupSigma 30 --runNumber $RunNumber" -nt 40 --nov 5000 --novPerJob 25 -o mc23.$RunNumber.MB.EVT.root --seed 512
#cd ../HIT
# simulation run in multi thread mode and save all hits into the HIT file
#simu_trf.py -i ../EVT/mb.EVT.root -o mb.HIT.root -nt 20 -m --saveAllHits -t 120

#cd ../..