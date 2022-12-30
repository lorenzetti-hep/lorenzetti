

NOV=10000

mkdir -p Zee/EVT
mkdir -p Zee/HIT
mkdir -p Zee/ESD
mkdir -p Zee/AOD

# generate 10k Zee events with pythia
cd Zee/EVT
prun_jobs.py -c "gen_zee.py --pileupAvg 0 --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 40 --nov $NOV --novPerJob 200 -o Zee.EVT.root -m
cd ..

# generate hits around the truth particle seed
cd HIT
simu_trf.py -i ../EVT/Zee.EVT.root -o Zee.HIT.root -nt 40
cd ..

# digitalization
cd ESD
prun_jobs.py -c "digit_trf.py -i %IN -o %OUT" -i ../HIT/ -o Zee.ESD.root -nt 40
cd ..

# reconstruction
cd AOD
prun_jobs.py -c "reco_trf.py -i %IN -o %OUT" -i ../ESD/ -o Zee.AOD.root -nt 40 -m
cd ..

