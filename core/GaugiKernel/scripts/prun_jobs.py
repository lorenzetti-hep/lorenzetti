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

parser.add_argument('-i','--inputFiles', action='store', 
    dest='inputFiles', required = False, nargs='+', default = None, 
    help = "Input files")

parser.add_argument('-c','--command', action='store', 
    dest='command', required = True,
    help = "The command job")

parser.add_argument('-mt','--numberOfThreads', action='store', 
    dest='numberOfThreads', required = False, default = 1, type=int,
    help = "The number of threads")

parser.add_argument('-m','--merge', action='store_true', dest='merge', required = False, 
                    help = "Merge all output files.")



import sys,os
if len(sys.argv)==1:
  parser.print_help()
  sys.exit(1)
args = parser.parse_args()



def func(command, input, output):
    return command + ' -i ' + input + ' -o ' + output



from Gaugi import expand_folders
files = expand_folders( args.inputFiles )

prun = Pool( func, args.command, args.numberOfThreads, files, args.outputFile )
prun.run()

if args.merge:
    prun.merge()

