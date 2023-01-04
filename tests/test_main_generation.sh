

nov=10
output=main.EVT.root


prun_jobs.py -c "gen_zee.py --pileupAvg 0 --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 1 --nov $nov -o $output -m

