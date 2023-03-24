
mkdir -p Zee_mb/HIT
mkdir -p Zee_mb/ESD
mkdir -p Zee_mb/EVT
mkdir -p Zee_mb/AOD

cd Zee_mb/HIT

prun_jobs.py -c "merge_trf.py -p ../../mb/HIT/mb.HIT.root -i %IN -o %OUT" -i ../../Zee/HIT -o Zee_mb.HIT.root -nt 40

cd ../ESD
prun_jobs.py -c "digit_trf.py -i %IN -o %OUT" -i ../HIT/ -o Zee_mb.ESD.root -nt 40

# reconstruction
cd ../AOD
prun_jobs.py -c "reco_trf.py -i %IN -o %OUT" -i ../ESD -o Zee_mb.AOD.root -nt 40 -m


cd ../..
