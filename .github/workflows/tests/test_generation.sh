

# number of events
nov=10
# job output
seed=512

# main event generation command
gen_zee.py --pileup-avg 0 --nov $nov -o Zee.EVT.root -s $seed

# minimum bias event generation command
gen_minbias.py --pileup-avg 10 --nov $nov -o mb.EVT.root -s $seed

