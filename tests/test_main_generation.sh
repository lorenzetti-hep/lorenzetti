

nov=10
output=main.EVT.root

echo "Main event generation..."
mkdir main_generation && main_generation

prun_jobs.py -c "gen_zee.py --pileupAvg 0 --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 1 --nov $nov -o $output -m
echo "done"