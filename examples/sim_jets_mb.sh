
mkdir -p JF17_mb/HIT
mkdir -p JF17_mb/ESD
mkdir -p JF17_mb/EVT
mkdir -p JF17_mb/AOD

cd JF17_mb/HIT

prun_jobs.py -c "merge_trf.py -p ../../mb/HIT/mb.HIT.root" -i ../../JF17/HIT -o JF17_mb.HIT.root -mt 40

cd ../ESD
prun_jobs.py -c "digit_trf.py" -i ../HIT/ -o JF17_mb.ESD.root -mt 40

# reconstruction
cd ../AOD
prun_jobs.py -c "reco_trf.py" -i ../ESD -o JF17_mb.AOD.root -mt 40 -m

cd ../..

