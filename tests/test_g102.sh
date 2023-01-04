


# number of events
nov=10
# job output
output=mb.EVT.root

# event generation command
prun_jobs.py -c "gen_minbias.py --pileupAvg 10 --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 1 --nov $nov -o $output -m
