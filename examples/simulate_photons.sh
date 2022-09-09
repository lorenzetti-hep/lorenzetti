
E=$1

mkdir -p photons/HIT
mkdir -p photons/ESD
mkdir -p photons/EVT
mkdir -p photons/AOD

cd photons/EVT
prun_events.py -c "gen_photon.py -e $E -s 0" -mt 40 --nov 1000 -o electrons.EVT.root -m
cd ../HIT
simu_trf.py -i ../EVT/photons.EVT.root -o photons.HIT.root --enableMagneticField -nt 40
# digitalization
cd ../ESD
prun_jobs.py -c "digit_trf.py" -i ../HIT/ -o photons.ESD.root -mt 40
# reconstruction
cd ../AOD
prun_jobs.py -c "reco_trf.py" -i ../ESD -o photons.AOD.root -m -mt 40
cd ../..
