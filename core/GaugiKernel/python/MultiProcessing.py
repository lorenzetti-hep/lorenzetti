__all__ = [ "Pool","chunks"]

import os, time
import subprocess

from GaugiKernel        import Logger
from GaugiKernel.macros import *
from pprint             import pprint
from typing             import List



def chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i:i + n]

class Pool( Logger ):

  def __init__(self, 
               func, 
               inputs  : List[str],
               maxJobs : int, 
               output  : str, 
  ):
    
    Logger.__init__(self)
    self.inputs = inputs
    self.func   = func
    class Worker:
      def __init__( self ):
        self.proc = None
        self.busy = False
      def run(self, command):
        pprint(command)
        time.sleep(2)
        self.proc = subprocess.Popen(command, shell=True, env=os.environ)
        self.busy=True
      def is_busy(self):
        return True if (self.proc and (self.proc.poll() is not None)) else False

    self.workers = [Worker() for _ in range(maxJobs)]
    self.output = output
    self.outputs = []

  def get_worker(self):
    for worker in self.workers:
      if worker.isAvailable():
        return worker
    return None
  
  def busy(self):
    for worker in self.workers:
      if not worker.isAvailable():
        return True
    return False

  def next(self):
    # prepare the command job
    inputs  = self.inputs.pop()
    idx     = len(self.inputs) # output label number
    output  = self.output + '.' + str(idx)
    command = self.func(inputs, output + '.tmp')
    # remove tmp name when complete the job
    command += f' && mv {output}.tmp {output}'
    self.outputs.append(output)
    print(command)
    return command, output

  def merge(self):
    command = "hadd -f "+self.output
    for fname in self.outputs:
      command += ' '+fname
    os.system(command)
    [os.system( 'rm -rf '+fname) for fname in self.outputs]

  def run( self , merge : bool=True):
    while len(self.inputs) > 0:
      worker = self.get_worker()
      if worker:
        command, output = self.generate()
        if os.path.exists(output):
          MSG_WARNING(self, f"File {output} exist. Skip.")
          continue
        worker.run( command , self.dry_run)
    while self.busy():
      continue
    if merge:
      self.merge()

