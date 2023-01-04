

# number of events
nov=10
# job output
seed=512

# main event generation command
gen_zee.py --pileupAvg 0 --nov $nov -o Zee.EVT.root -s $seed --eventNumber 0

# minimum bias event generation command
gen_minbias.py --pileupAvg 10 --nov $nov -o mb.EVT.root -s $seed

