__all__ = [ "Pool","Slot", "chunks"]


from GaugiKernel import LoggingLevel, Logger
from GaugiKernel.macros import *
from pprint import pprint
import argparse
import os, time
import subprocess

def chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i:i + n]



class Slot(object):

  def __init__( self ):
    self.__proc = None
    self.__lock = False

  def lock(self):
    self.__lock=True
  
  def unlock(self):
    self.__lock=False

  def update(self):
    if self.__proc and not self.__proc.poll():
      self.unlock()

  def run(self, command, dry_run=False):
    pprint(command)
    time.sleep(2)
    if not dry_run:
      self.__proc = subprocess.Popen(command, shell=True, env=os.environ)
      self.lock()

  def isAvailable(self):
    if self.__proc:
      if not self.__proc.poll() is None:
        self.unlock()
    return not self.__lock





class Pool( Logger ):

  def __init__(self, func, inputs,maxJobs, output, dry_run):
    
    Logger.__init__(self)
    self.__inputs = inputs
    self.__func = func
    self.__slots = [Slot() for _ in range(maxJobs)]
    self.__output = output
    self.__outputs = []
    self.dry_run = dry_run

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
    command = self.__func(inputs, output + '.tmp')
    # remove tmp name when complete the job
    command += f' && mv {output}.tmp {output}'
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
        if os.path.exists(output):
          MSG_WARNING(self, f"File {output} exist. Skip.")
          continue
        slot.run( command , self.dry_run)
    
    while self.busy():
      continue

  def merge(self):
    command = "hadd -f "+self.__output
    for fname in self.__outputs:
      command += ' '+fname
    os.system(command)
    for fname in self.__outputs:
      os.system( 'rm -rf '+fname)




