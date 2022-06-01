
mkdir -p SingleElectron/HIT
mkdir -p SingleElectron/ESD
mkdir -p SingleElectron/EVT
mkdir -p SingleElectron/AOD

# generate 10k SingleElectron events with pythia
cd SingleElectron/EVT
# prun_events.py -c "gen_electron.py --pileupAvg 0" --nov 100 -o SingleElectron.EVT.root -m

gen_electron.py --pileupAvg 0 --evt 10 -o SingleElectron.EVT.root


# generate hits around the truth particle seed
cd ../HIT
simu_trf.py -i ../EVT/SingleElectron.EVT.root -o SingleElectron.HIT.root


# digitalization
cd ../ESD
# prun_jobs.py -c "digit_trf.py" -i ../HIT/ -o SingleElectron.ESD.root -mt 40

digit_trf.py -i ../HIT/* -o SingleElectron.ESD.root

# reconstruction
cd ../AOD
# prun_jobs.py -c "reco_trf.py" -i ../ESD -o SingleElectron.AOD.root -mt 40 -m
reco_trf.py -i ../ESD/SingleElectron.ESD.root -o SingleElectron.AOD.root


