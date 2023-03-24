#!/usr/bin/env python3

import argparse
from GaugiKernel import Slot, Pool, chunks, expand_folders
from GaugiKernel import Logger
from GaugiKernel.macros import *

import time, os
import ROOT

mainLogger = Logger.getModuleLogger("prun_jobs")
parser = argparse.ArgumentParser(description = '', add_help = False)
parser = argparse.ArgumentParser()

parser.add_argument('-o','--output', action='store', 
    dest='output', required = True,
    help = "The output file. Use %%OUT to replace in command")

parser.add_argument('-c','--command', action='store', 
    dest='command', required = True,
    help = "The command")

parser.add_argument('-m','--merge', action='store_true', dest='merge', 
                    required = False, 
                    help = "Merge all output files.")

parser.add_argument('-nt', '--numberOfThreads', action='store', 
    dest='numberOfThreads', required = False, default = 1, type=int,
    help = "The number of threads.")

parser.add_argument('--seed', action='store', 
    dest='seed', required = False, default = 512, type=int,
    help = "The number of events per job. Use %%SEED to replace in command")

parser.add_argument('--nov','--numberOfEvents', action='store', 
    dest='nov', required = False, default = 100, type=int,
    help = "The number of events. Use %%NOV and %%OFFSET to replace in command")

parser.add_argument('--novPerJob', action='store', 
    dest='novPerJob', required = False, default = 100, type=int,
    help = "The number of events per job.")

parser.add_argument('--dry_run', action='store_true', dest='dry_run', 
                    required = False, 
                    help = "dry run command")


import sys,os
if len(sys.argv)==1:
  parser.print_help()
  sys.exit(1)

args = parser.parse_args()


inputs = list(range(args.nov))
inputs = list(chunks(inputs, args.novPerJob))

def func(event_numbers, outputfile):
  seed = str(args.seed+event_numbers[0])
  nov = str(len(event_numbers))
  offset =  str(event_numbers[0])
  command = args.command
  command = command.replace('%OFFSET' , offset)
  command+= f' -s {seed} -o {outputfile} --nov {nov}'
  command = command.replace('  ', ' ') # remove double spaces
  return command
    


prun = Pool( func, inputs, args.numberOfThreads, os.path.abspath(args.output), args.dry_run )
prun.run()
if args.merge:
  prun.merge()
