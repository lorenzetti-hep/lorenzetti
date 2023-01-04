

# number of events
nov=10
# job output
seed=512

# main event generation command
prun_jobs.py -c "gen_zee.py --pileupAvg 0 --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 1 -s $seed --nov $nov -o Zee.EVT.root -m

# minimum bias event generation command
prun_jobs.py -c "gen_minbias.py --pileupAvg 10 --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 1 -s $seed --nov $nov -o mb.EVT.root -m

