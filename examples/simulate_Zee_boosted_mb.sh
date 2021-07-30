
mkdir -p Zee_boosted_mb/HIT
mkdir -p Zee_boosted_mb/ESD
mkdir -p Zee_boosted_mb/EVT
mkdir -p Zee_boosted_mb/AOD

cd Zee_boosted_mb/HIT

prun_jobs.py -c "merge_trf.py -p ../../mb/HIT/mb.HIT.root" -i ../../Zee_boosted/HIT -o Zee_boosted_mb.HIT.root -mt 40

cd ../ESD
prun_jobs.py -c "digit_trf.py" -i ../HIT/ -o Zee_boosted_mb.ESD.root -mt 40

# reconstruction
cd ../AOD
prun_jobs.py -c "reco_trf.py" -i ../ESD -o Zee_boosted_mb.AOD.root -mt 40 -m


