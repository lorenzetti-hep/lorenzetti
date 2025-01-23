#!/usr/bin/env python3

import argparse
from GaugiKernel import Pool, chunks
from GaugiKernel import Logger, get_argparser_formatter
from GaugiKernel.macros import *
from expand_folders import expand_folders

import time, os
import ROOT

mainLogger = Logger.getModuleLogger("prun_jobs")
parser = argparse.ArgumentParser(description = '', formatter_class=get_argparser_formatter() )


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

parser.add_argument('-i', '--inputs', action='store', 
    dest='inputs', required = False, default = None, 
    help = "The input files. Use %%IN to replace in command")

parser.add_argument('--dry_run', action='store_true', dest='dry_run', 
                    required = False, 
                    help = "dry run command")


import sys,os
if len(sys.argv)==1:
  parser.print_help()
  sys.exit(1)

args = parser.parse_args()

inputs = expand_folders(args.inputs)

def func(inputfile, outputfile):
  command = args.command
  command = command.replace('%IN'  , inputfile      )
  command = command.replace('%OUT' , outputfile     )
  return command

prun = Pool( func, inputs, args.numberOfThreads, os.path.abspath(args.output) , args.dry_run)
prun.run()
if args.merge:
  prun.merge()
