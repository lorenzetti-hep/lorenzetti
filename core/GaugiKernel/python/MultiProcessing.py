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

  def run(self, command):
    pprint(command)
    self.__proc = subprocess.Popen(command.split(' '))
    self.lock()

  def isAvailable(self):
    if self.__proc:
      if not self.__proc.poll() is None:
        self.unlock()
    return not self.__lock




class Pool( Logger ):

  def __init__(self, func, command, maxJobs, files, output ):
    
    Logger.__init__(self)
    self.__files = files
    self.__gen = func
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
    f = self.__files.pop()
    idx = len(self.__files)
    output = self.__output + '.' + str(idx)
    self.__outputs.append(output)
    return self.__gen(self.__command, f, output)


  def run( self ):

    while len(self.__files) > 0:
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





