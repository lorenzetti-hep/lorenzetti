

NOV=10000
seed=512

mkdir -p JF17/EVT
mkdir -p JF17/HIT
mkdir -p JF17/ESD
mkdir -p JF17/AOD

# generate 10k JF17 events with pythia
cd JF17/EVT
prun_jobs.py -c "gen_jets.py --pileupAvg 0 --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 40 --nov $NOV --seed $seed --novPerJob 200 -o JF17.EVT.root
cd ..

# generate hits around the truth particle seed
cd HIT 
prun_jobs.py -c "simu_trf.py -i %IN -o %OUT -nt 20 --enableMagneticField -t 10" -nt 1 -i ../EVT -o JF17.HIT.root
cd ..

# digitalization
cd ESD
prun_jobs.py -c "digit_trf.py -i %IN -o %OUT" -i ../HIT/ -o JF17.ESD.root -nt 30
cd ..

# reconstruction
cd AOD
prun_jobs.py -c "reco_trf.py -i %IN -o %OUT" -i ../ESD/ -o JF17.AOD.root -nt 30 -m
cd ..

