__all__ = [ "Pool"]


from Gaugi.messenger import LoggingLevel, Logger
from Gaugi.messenger.macros import *
import argparse


class Pool( Logger ):

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