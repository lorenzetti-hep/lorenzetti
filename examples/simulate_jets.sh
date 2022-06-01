


mkdir -p JF17/HIT
mkdir -p JF17/ESD
mkdir -p JF17/EVT
mkdir -p JF17/AOD

cd JF17/EVT
prun_events.py -c "gen_jets.py --pileupAvg 0" -mt 40 --nov 10000 -o JF17.EVT.root -m

cd ../HIT
simu_trf.py -i ../EVT/JF17.EVT.root -o JF17.HIT.root -nt 40

# digitalization
cd ../ESD
prun_jobs.py -c "digit_trf.py" -i ../HIT/ -o JF17.ESD.root -mt 40

# reconstruction
cd ../AOD
prun_jobs.py -c "reco_trf.py" -i ../ESD -o JF17.AOD.root -mt 40 -m

cd ../..
