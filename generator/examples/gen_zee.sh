

CPU_N=$(grep -c ^processor /proc/cpuinfo)

mkdir EVT && cd EVT

# Generate events 
prun_events.py -c "gen_zee.py -o %OUT --nov %NOV -s %SEED --eventNumber %OFFSET" -nt $CPU_N -o zee.EVT.root --nov 10000 -m

cd ..