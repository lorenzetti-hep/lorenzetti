

NOV=10000
seed=512

mkdir -p Zee/EVT
mkdir -p Zee/HIT
mkdir -p Zee/ESD
mkdir -p Zee/AOD

# generate 10k Zee events with pythia
cd Zee/EVT
prun_jobs.py -c "gen_zee.py --pileupAvg 0 --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 40 --nov $NOV --seed $seed --novPerJob 200 -o Zee.EVT.root
cd ..

# generate hits around the truth particle seed
cd HIT
prun_jobs.py -c "simu_trf.py -i %IN -o %OUT -nt 20 --enableMagneticField -t 10" -nt 1 -i ../EVT -o Zee.HIT.root
cd ..

# digitalization
cd ESD
prun_jobs.py -c "digit_trf.py -i %IN -o %OUT" -i ../HIT/ -o Zee.ESD.root -nt 40
cd ..

# reconstruction
cd AOD
prun_jobs.py -c "reco_trf.py -i %IN -o %OUT" -i ../ESD/ -o Zee.AOD.root -nt 40 -m
cd ..

