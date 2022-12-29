


CPU_N=$(grep -c ^processor /proc/cpuinfo)

mkdir EVT && cd EVT

# Generate events 
prun_jobs.py -c "gen_minbias.py -o %OUT --nov %NOV -s %SEED --pileupAvg 100" -nt $CPU_N -o jets.EVT.root --nov 10000 -m

cd ..