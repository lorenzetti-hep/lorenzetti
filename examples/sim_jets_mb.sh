
mkdir -p JF17_mb/HIT
mkdir -p JF17_mb/ESD
mkdir -p JF17_mb/EVT
mkdir -p JF17_mb/AOD

cd JF17_mb/HIT

prun_jobs.py -c "merge_trf.py -p ../../mb/HIT/mb.HIT.root -i %IN -o %OUT" -i ../../JF17/HIT -o JF17_mb.HIT.root -nt 40

cd ../ESD
prun_jobs.py -c "digit_trf.py -i %IN -o %OUT" -i ../HIT/ -o JF17_mb.ESD.root -nt 40

cd ../AOD
prun_jobs.py -c "reco_trf.py -i %IN -o %OUT" -i ../ESD -o JF17_mb.AOD.root -nt 40 -m


cd ../..

