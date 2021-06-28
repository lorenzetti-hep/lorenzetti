#!/usr/bin/env python3

import argparse
from GaugiKernel import Pool
from Gaugi import Logger


mainLogger = Logger.getModuleLogger("prun.job")
parser = argparse.ArgumentParser(description = '', add_help = False)
parser = argparse.ArgumentParser()

parser.add_argument('-o','--outputFile', action='store', 
    dest='outputFile', required = True,
    help = "Output file")

parser.add_argument('-c','--command', action='store', 
    dest='command', required = True,
    help = "The command job")

parser.add_argument('-mt','--numberOfThreads', action='store', 
    dest='numberOfThreads', required = False, default = 1, type=int,
    help = "The number of threads")

parser.add_argument('--nov','--numberOfEvents', action='store', 
    dest='nov', required = True, default = 100, type=int,
    help = "The number of events")

parser.add_argument('-m','--merge', action='store_true', dest='merge', required = False, 
                    help = "Merge all output files.")



import sys,os
if len(sys.argv)==1:
  parser.print_help()
  sys.exit(1)
args = parser.parse_args()


# we will always create
def func(command, _, output):
    return command + ' -o ' + output + ' --evt 100'

if( args.nov % 100 > 0):
    print( 'number of event should be multiple of 100: 100, 200, 1000, 10000...')
    sys.exit(1)


njobs = int(args.nov/100)
flist = list(range(njobs))

prun = Pool( func, args.command, args.numberOfThreads, flist, args.outputFile )
prun.run()


if args.merge:
    prun.merge()
