

# number of events
nov=10
# job output
output=main.EVT.root

# event generation command
prun_jobs.py -c "gen_zee.py --pileupAvg 0 --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 1 --nov $nov -o $output -m

