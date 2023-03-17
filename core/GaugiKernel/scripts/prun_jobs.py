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

    

#
# Loop over files
#

parser.add_argument('-i', '--inputs', action='store', 
    dest='inputs', required = False, default = None, 
    help = "The input files. Use %%IN to replace in command")


#
# Loop over the number of events
#

parser.add_argument('--nov','--numberOfEvents', action='store', 
    dest='nov', required = False, default = 100, type=int,
    help = "The number of events. Use %%NOV and %%OFFSET to replace in command")

parser.add_argument('--novPerJob', action='store', 
    dest='novPerJob', required = False, default = 100, type=int,
    help = "The number of events per job.")




import sys,os
if len(sys.argv)==1:
  parser.print_help()
  sys.exit(1)

args = parser.parse_args()



if args.inputs:
  #
  # Examples:
  # prun_events.py -c "gen_zee.py -i %IN -s %SEED -o %OUT" 
  #
  inputs = expand_folders(args.inputs)

  def func_command(inputfile, outputfile):
    command = args.command
    command = command.replace('%IN'  , inputfile      )
    command = command.replace('%OUT' , outputfile     )
    command = command.replace('%SEED', str(args.seed) )
    return command

  func = func_command

else: # We dont have inputs (only for event generator step)
  #
  # Examples:
  # prun_events.py -c "gen_zee.py -c data.cmnd -s %SEED --nov %NOV --event_number %OFFSET -o %OUT" 
  # prun_events.py -c "gun_events.py -e p8gun.exe -c data.cmnd -s %SEED --nov %NOV --event_number %OFFSET -o %OUT"
  #
  inputs = list(range(args.nov))
  inputs = list(chunks(inputs, args.novPerJob))

  def func_command(event_numbers, outputfile):
    command = args.command
    command = command.replace('%NOV'   , str(len(event_numbers)))
    command = command.replace('%SEED'  , str(args.seed+event_numbers[0]) )
    command = command.replace('%OFFSET', str(event_numbers[0]))
    command = command.replace('%OUT'   , outputfile)
    return command

  func = func_command
    


prun = Pool( func, inputs, args.numberOfThreads, os.path.abspath(args.output) )
prun.run()
if args.merge:
  prun.merge()
