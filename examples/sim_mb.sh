

seed=512

mkdir -p mb/HIT
mkdir -p mb/EVT
cd mb/EVT
# multi processing command
prun_jobs.py -c "gen_minbias.py --pileupAvg 60 --nov %NOV -o %OUT -s %SEED" -nt 40 --nov 20 --novPerJob 10 -o mb.EVT.root --seed $seed -m
cd ../HIT
# simulation run in multi thread mode and save all hits into the HIT file
simu_trf.py -i ../EVT/mb.EVT.root -o mb.HIT.root -nt 20 -m --saveAllHits -t 120

cd ../..