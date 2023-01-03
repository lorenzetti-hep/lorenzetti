

cd /volume/data/main/
prun_jobs.py -c "gen_zee.py --pileupAvg 0 --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 1 --nov 10 --novPerJob 100 -o main.EVT.root -m

