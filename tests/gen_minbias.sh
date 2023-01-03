

prun_jobs.py -c "gen_minbias.py --pileupAvg 10 --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 1 --nov 10 --novPerJob 100 -o mb.EVT.root -m
