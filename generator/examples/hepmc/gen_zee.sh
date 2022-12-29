

CPU_N=$(grep -c ^processor /proc/cpuinfo)

# Generate all HEPMC files from pythia generator. The user can substitute this to Sherpa
# or any other HEP generator here.
mkdir HEPMC & cd HEPMC
ZEE_CONFIG=$LZT_PATH/generator/guns/data/zee_config.cmnd
prun_events.py -c "gun_events.py -o %OUT --nov %NOV -c $ZEE_CONFIG -s %SEED --event_number %OFFSET" -nt $CPU_N -o zee.HEPMC.root --nov 10000
cd ..


mkdir EVT & cd EVT
# Generate events 
prun_events.py -c "gen_zee.py -o %OUT -i %IN -s %SEED" -nt $CPU_N -o zee.EVT.root -i $PWD/../HEPMC -m
cd ..
