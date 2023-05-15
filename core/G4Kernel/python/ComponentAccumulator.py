
__all__ = ["ComponentAccumulator"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
import os, time, gc


class ComponentAccumulator( Logger ):

  __allow_keys = ["NumberOfThreads", "OutputFile", "RunVis", "Seed", "Timeout", "VisMac"]

  def __init__( self, name , detector, **kw):

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import RunManager
    self.__core = RunManager(name)

    # convert python to geant4
    self.__detector = detector
    self.__detector.compile()
    # set the geant detector into the geant 
    self.__core.setDetectorConstruction( self.__detector.core() )
    for key, value in kw.items():
      self.setProperty( key, value )
    # Set the vis mac file into the manager core
    self.setProperty("VisMac", self.__detector.VisMac)

    self.outputFiles = [ (self.OutputFile+".%d"%thread) for thread in range(self.NumberOfThreads)]


  def __del__(self):
    del self.__core
    gc.collect()
    time.sleep(2)
    self.merge()

  #
  # Run events
  #
  def run( self, evt=None ):
    if evt is None:
      evt = self.__numberOfEvents
    elif evt > self.__numberOfEvents:
      evt = self.__numberOfEvents
    self.__core.run(evt)


  def setProperty( self, key, value ):
    if key in self.__allow_keys:
      setattr( self,  key , value )
      self.core().setProperty( key, value )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

 
  def getProperty( self, key ):
    if key in self.__allow_keys:
      return getattr( self, '__' + key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)


  def __add__( self, algs ):
    if type(algs) is not list:
      algs =[algs]
    for alg in algs:
      self.__core.push_back( alg.core() )
    return self

  
  def core(self):
    return self.__core


  #
  # Set generator reader
  #
  def setGenerator( self, gen ):
    self.__numberOfEvents = gen.GetEntries()
    self.core().setGenerator( gen.core() )


  #
  # Get detector contruction object
  #
  def detector(self):
    return self.__detector


  #
  # Destructor
  #
  def merge(self):
    files = ' '.join(self.outputFiles)
    command = f"hadd -f {self.OutputFile} {files}"
    print( command )
    os.system(command)
    # remove thread files
    for fname in self.outputFiles:
      os.system( 'rm '+ fname )
  