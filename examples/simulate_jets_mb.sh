
mkdir -p JF17_mb100/HIT
mkdir -p JF17_mb100/ESD
mkdir -p JF17_mb100/EVT
mkdir -p JF17_mb100/AOD

cd JF17_mb100/HIT

prun_jobs.py -c "merge_trf.py -p ../../mb/HIT/mb100.HIT.root" -i ../../JF17/HIT -o JF17_mb100.HIT.root -mt 40

cd ../ESD
prun_jobs.py -c "digit_trf.py" -i ../HIT/ -o JF17_mb100.ESD.root -mt 40

# reconstruction
cd ../AOD
prun_jobs.py -c "reco_trf.py" -i ../ESD -o JF17_mb100.AOD.root -mt 40 -m


