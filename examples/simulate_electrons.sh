
E=$1

mkdir -p electrons/HIT
mkdir -p electrons/ESD
mkdir -p electrons/EVT
mkdir -p electrons/AOD

cd electrons/EVT
prun_events.py -c "gen_electron.py -e $E -s 0" -mt 40 --nov 1000 -o electrons.EVT.root -m
cd ../HIT
simu_trf.py -i ../EVT/electrons.EVT.root -o electrons.HIT.root -nt 40 --enableMagneticField
# digitalization
cd ../ESD
prun_jobs.py -c "digit_trf.py" -i ../HIT/ -o electrons.ESD.root -mt 40
# reconstruction
cd ../AOD
prun_jobs.py -c "reco_trf.py" -i ../ESD -o electrons.AOD.root -mt 40 -m
cd ../..
