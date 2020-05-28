docker run --network host -v $PWD:/output -it lorenzett/lorenzett:lps /bin/bash


#docker run --network host -v $PWD:/output -it lorenzett/lorenzett:lps /bin/bash -c ". /setup_envs.sh && /parser.py -o output/reco.root -f Zee -e 10 -j 1 --pileupAvg 0 --bc_id_start -24 --bc_id_end 7 --bc_duration 25 --outputLevel 6 -s 0 -r reco_trf.py"
