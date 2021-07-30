


mkdir -p Zee_boosted/HIT
mkdir -p Zee_boosted/ESD
mkdir -p Zee_boosted/EVT
mkdir -p Zee_boosted/AOD

# generate 10k Zee events with pythia
cd Zee_boosted/EVT
prun_events.py -c "gen_zee_boosted.py --pileupAvg 0" -mt 40 --nov 10000 -o Zee_boosted.EVT.root -m


# generate hits around the truth particle seed
cd ../HIT
simu_trf.py -i ../EVT/Zee_boosted.EVT.root -o Zee_boosted.HIT.root -nt 40 


# digitalization
cd ../ESD
prun_jobs.py -c "digit_trf.py" -i ../HIT/ -o Zee_boosted.ESD.root -mt 40

# reconstruction
cd ../AOD
prun_jobs.py -c "reco_trf.py" -i ../ESD -o Zee_boosted.AOD.root -mt 40 -m



