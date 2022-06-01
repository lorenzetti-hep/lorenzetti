#
# Generate 10k min bias HITs to merge with the main event
#


mkdir -p mb/HIT
mkdir -p mb/EVT
cd mb/EVT
# multi processing command
prun_events.py -c "gen_minbias.py --pileupAvg 60" -mt 40 --nov 100 -o mb.EVT.root -m
cd ../HIT
# simulation run in multi thread mode and save all hits into the HIT file
simu_trf.py -i ../EVT/mb.EVT.root -o mb.HIT.root -nt 20 -m --saveAllHits

cd ../..

