#!/usr/bin/env python3

import argparse
from GaugiKernel import Slot, chunks, expand_folders
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

else:
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
    


class Pool( Logger ):

  def __init__(self, func, inputs,maxJobs, output):
    
    Logger.__init__(self)
    self.__inputs = inputs
    self.__func = func
    self.__slots = [Slot() for _ in range(maxJobs)]
    self.__output = output
    self.__outputs = []


  def getAvailable(self):
    for slot in self.__slots:
      if slot.isAvailable():
        return slot
    return None

  
  def busy(self):
    for slot in self.__slots:
      if not slot.isAvailable():
        return True
    return False


  def generate(self):
    # prepare the command job
    inputs = self.__inputs.pop()
    idx = len(self.__inputs) # output label number
    output = self.__output + '.' + str(idx)
    command = self.__func(inputs, output)
    self.__outputs.append(output)
    print(command)
    return command, output


  #
  # Run jobs
  #
  def run( self ):

    while len(self.__inputs) > 0:
      slot = self.getAvailable()
      if slot:
        command, output = self.generate()
        if self.exist(output):
          MSG_WARNING(self, f"File {output} exist. Skip.")
          continue
        slot.run( command )
    
    while self.busy():
      continue

  def merge(self):
    command = "hadd -f "+self.__output
    for fname in self.__outputs:
      command += ' '+fname
    os.system(command)
    for fname in self.__outputs:
      os.system( 'rm -rf '+fname)

  #
  # Check if file exist or his consistencels
  #
  def exist(self, f):
    if not os.path.exists(f):
      return False
    try:
      f = ROOT.TFile(f, 'read')
      is_open = f.IsOpen()
      f.Close()
      return is_open
    except:
      return False








prun = Pool( func, inputs, args.numberOfThreads, args.output)
prun.run()
if args.merge:
  prun.merge()
