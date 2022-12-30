#!/usr/bin/env python3
from GaugiKernel   import LoggingLevel, Logger
import argparse
import sys,os



mainLogger = Logger.getModuleLogger("gen_events")
parser = argparse.ArgumentParser(description = '', add_help = False)
parser = argparse.ArgumentParser()


#
# Mandatory arguments
#
parser.add_argument('-o','--output', action='store', dest='output', required = True,
                    help = "The event generated file.")

parser.add_argument('--nov', action='store', dest='nov', required = True, type=int,
                    help = "Number of events to be generated.")

parser.add_argument('-c', '--config' , action='store', dest='config', required = True,
                    help = "The Pythia8 configuration file.")

parser.add_argument('-s','--seed', action='store', dest='seed', required = False, type=int, default=0,
                    help = "Seed number.")

parser.add_argument('--eventNumber', action='store', dest='eventNumber', required = True, type=int,
                    help = "The event number offset.")

parser.add_argument('-e', '--executable', action='store', dest='executable', required = False, default='gun.exe',
                    help = "The executable program.")

if len(sys.argv)==1:
  parser.print_help()
  sys.exit(1)

args = parser.parse_args()
command = f"{args.executable} {args.nov} {args.eventNumber} {args.seed} {args.config} {args.output}"
print(command)
os.system(command)
