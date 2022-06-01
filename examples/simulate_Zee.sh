


mkdir -p Zee/HIT
mkdir -p Zee/ESD
mkdir -p Zee/EVT
mkdir -p Zee/AOD

# generate 10k Zee events with pythia
cd Zee/EVT
prun_events.py -c "gen_zee.py --pileupAvg 0" -mt 40 --nov 10000 -o Zee.EVT.root -m


# generate hits around the truth particle seed
cd ../HIT
simu_trf.py -i ../EVT/Zee.EVT.root -o Zee.HIT.root -nt 40


# digitalization
cd ../ESD
prun_jobs.py -c "digit_trf.py" -i ../HIT/ -o Zee.ESD.root -mt 40

# reconstruction
cd ../AOD
prun_jobs.py -c "reco_trf.py" -i ../ESD -o Zee.AOD.root -mt 40 -m

cd ../..

