
__all__ = ["ComponentAccumulator"]

from GaugiKernel.constants import *
from GaugiKernel import Logger
from GaugiKernel.macros import *
import os, time, gc


class ComponentAccumulator( Logger ):

  def __init__( self, name , detector, 
                OutputFile      : str="output.root",
                Seed            : int=512,
                NumberOfThreads : int=1,
                RunVis          : bool=False,
                Timeout         : int=120*MINUTES,
              ):

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import RunManager
    self.__core = RunManager(name)

    # convert python to geant4
    self.__detector = detector
    self.__detector.compile()
    # set the geant detector into the geant 
    self.__core.setDetectorConstruction( self.__detector.core()   )
    self.setProperty( "VisMac"          , self.__detector.VisMac  )
    self.setProperty( "NumberOfThreads" , NumberOfThreads         )
    self.setProperty( "Timeout"         , Timeout                 )
    self.setProperty( "RunVis"          , RunVis                  )
    self.setProperty( "Seed"            , Seed                    )
    # Set the vis mac file into the manager core
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
    if key in self.core().hasProperty(key):
      setattr( self, key , value )
      self.core().setProperty( key, treatPropertyValue(value) )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

 
  def getProperty( self, key ):
    if hasattr(self, key):
      return getattr( self, key )
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
    os.system(command)
    # remove thread files
    for fname in self.outputFiles:
      os.system( 'rm '+ fname )
  