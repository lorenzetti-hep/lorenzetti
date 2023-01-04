#!/usr/bin/env python3

from GaugiKernel          import LoggingLevel, Logger
import numpy as np
import argparse
import sys,os


mainLogger = Logger.getModuleLogger("job")
parser = argparse.ArgumentParser(description = '', add_help = False)
parser = argparse.ArgumentParser()



parser.add_argument('--nov','--numberOfEvents', action='store', dest='numberOfEvents', required = False, type=int, default=-1,
                    help = "The number of events to apply the reconstruction.")


if len(sys.argv)==1:
  parser.print_help()
  sys.exit(1)

args = parser.parse_args()


tests = {
    'EVT' : 
        {
            'command':'prun_jobs.py -c "gen_zee.py --pileupAvg 0 \
                --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 1 --nov $nov -o $output -m',
            'seed'   : 512,
            "output" : 
        },
    'g102',

    'EVTtoHIT':

}
