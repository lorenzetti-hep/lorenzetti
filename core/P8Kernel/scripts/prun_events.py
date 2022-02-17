#!/usr/bin/env python3

import argparse
from GaugiKernel import Slot, chunks
from Gaugi import Logger
import time

mainLogger = Logger.getModuleLogger("prun.job")
parser = argparse.ArgumentParser(description = '', add_help = False)
parser = argparse.ArgumentParser()

parser.add_argument('-o','--outputFile', action='store', 
    dest='outputFile', required = True,
    help = "Output file")

parser.add_argument('-c','--command', action='store', 
    dest='command', required = True,
    help = "The command job.")

parser.add_argument('-mt','--numberOfThreads', action='store', 
    dest='numberOfThreads', required = False, default = 1, type=int,
    help = "The number of threads.")

parser.add_argument('--nov','--numberOfEvents', action='store', 
    dest='nov', required = True, default = 100, type=int,
    help = "The number of events.")

parser.add_argument('--numberOfEventsPerJob', action='store', 
    dest='numberOfEventsPerJob', required = False, default = 100, type=int,
    help = "The number of events per job.")

parser.add_argument('-m','--merge', action='store_true', dest='merge', required = False, 
                    help = "Merge all output files.")



import sys,os
if len(sys.argv)==1:
  parser.print_help()
  sys.exit(1)
args = parser.parse_args()




class Pool( Logger ):

  def __init__(self, command, maxJobs, nov, novPerJob, output ):
    
    Logger.__init__(self)
    self.__list_of_events = list(chunks(list(range(nov)), novPerJob))
    self.__command = command
    self.__output  = output
    self.__slots = [Slot() for _ in range(maxJobs)]
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
    list_of_events = self.__list_of_events.pop()
    list_of_events_str = str()
    for evt in list_of_events:
        list_of_events_str += str(evt) + ' '
    idx = len(self.__list_of_events) # output label number
    output = self.__output + '.' + str(idx) # output for this job
    self.__outputs.append(output) # use this to merge in the end
    command =  self.__command + ' --eventNumbers ' + list_of_events_str + ' -o ' +  output
    command = command.replace('  ', ' ') # fix in case of dob space
    print(command)
    return command

  def run( self ):

    while len(self.__list_of_events) > 0:
      slot = self.getAvailable()
      if slot:
        time.sleep(1)
        command = self.generate()
        #MSG_INFO( self,  ('adding process into the stack with id %d')%(len(self.__files)) )
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





prun = Pool( args.command, args.numberOfThreads, args.nov, args.numberOfEventsPerJob, args.outputFile )
prun.run()


if args.merge:
    prun.merge()
