#!/usr/bin/env python3


from Gaugi.messenger import LoggingLevel, Logger
from Gaugi.messenger.macros import *
import argparse


class Parallel( Logger ):

  def __init__(self, command, njobs, maxJobs, output ):
    Logger.__init__(self)
    self.process_pipe = []
    self.output_to_merge = []
    import random
    import time
    random.seed(time.time())
    self._base_id = random.randrange(100000)
    self._jobList = list(range(njobs))
    self._maxJobs = maxJobs
    self._command = command
    self._output  = output

  def run( self ):
    import os, time
    import subprocess
    from pprint import pprint
    while len(self._jobList) > 0:
      if len(self.process_pipe) < int(self._maxJobs):
        time.sleep(2)
        job_id = len(self._jobList)
        self._jobList.pop()
        oname = ('output_%d_%d.root') % (self._base_id, job_id) 
        self.output_to_merge.append( ('output_%d_%d.root') % (self._base_id, job_id) )
        command = self._command + (' -o %s') % (self.output_to_merge[-1])
        MSG_INFO( self,  ('adding process into the stack with id %d')%(job_id) )
        pprint(command)
        proc = subprocess.Popen(command.split(' '))
        self.process_pipe.append( (job_id, proc) )
      for proc in self.process_pipe:
        if not proc[1].poll() is None:
          MSG_INFO( self,  ('pop process id (%d) from the stack')%(proc[0]) )
          self.process_pipe.remove(proc)
    
    # Check pipe process
    # Protection for the last jobs
    while len(self.process_pipe)>0:
      for proc in self.process_pipe:
        if not proc[1].poll() is None:
          #MSG_INFO( self,  ('pop process id (%d) from the stack')%(proc[0]), extra={'color':'0;35'})
          MSG_INFO( self,  ('pop process id (%d) from the stack')%(proc[0]) )
          self.process_pipe.remove(proc)

    # Merge
    command = "hadd -f "+self._output
    for fname in self.output_to_merge:
      command += ' '+fname
    os.system(command)
    for fname in self.output_to_merge:
      os.system( 'rm -rf '+fname)


mainLogger = Logger.getModuleLogger("prun.job")
parser = argparse.ArgumentParser(description = '', add_help = False)
parser = argparse.ArgumentParser()

parser.add_argument('-o','--outputFile', action='store', 
    dest='outputFile', required = True,
    help = "The input files.")

parser.add_argument('-c','--command', action='store', 
    dest='command', required = True,
    help = "The command job")

parser.add_argument('-mt','--numberOfThreads', action='store', 
    dest='numberOfThreads', required = False, default = 1, type=int,
    help = "The number of threads")

parser.add_argument('-n','--numberOfJobs', action='store', 
    dest='numberOfJobs', required = False, default = 1, type=int,
    help = "The number of jobs")



import sys,os
if len(sys.argv)==1:
  parser.print_help()
  sys.exit(1)
args = parser.parse_args()

prun = Parallel( args.command, args.numberOfJobs, args.numberOfThreads, args.outputFile )
prun.run()


