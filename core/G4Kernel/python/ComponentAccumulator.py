
__all__ = ["ComponentAccumulator"]

from GaugiKernel import Cpp, LoggingLevel
from GaugiKernel.constants import *
from GaugiKernel.macros import *
import os, time, gc
import ROOT

class ComponentAccumulator( Cpp ):

  def __init__( self, name , detector, 
                OutputFile      : str="output.root",
                Seed            : int=512,
                NumberOfThreads : int=1,
                RunVis          : bool=False,
                Timeout         : int=120*MINUTES,
                OutputLevel     : int=LoggingLevel.toC('INFO'),
              ):

    Cpp.__init__(self, ROOT.RunManager(name) )
    # convert python to geant4
    self.__detector = detector
    self.__detector.compile()
    # set the geant detector into the geant 
    self._core.setDetectorConstruction( self.__detector.core()   )
    self.setProperty( "OutputFile"      , OutputFile              )
    self.setProperty( "VisMac"          , self.__detector.VisMac  )
    self.setProperty( "NumberOfThreads" , NumberOfThreads         )
    self.setProperty( "Timeout"         , Timeout                 )
    self.setProperty( "RunVis"          , RunVis                  )
    self.setProperty( "Seed"            , Seed                    )
    #self.setProperty( "OutputLevel"     , OutputLevel             )
    # Set the vis mac file into the manager core
    self.outputFiles = [ (self.OutputFile+".%d"%thread) for thread in range(self.NumberOfThreads)]


  def __del__(self):
    del self._core
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
    self._core.run(evt)


  def __add__( self, algs ):
    if type(algs) is not list:
      algs =[algs]
    for alg in algs:
      self._core.push_back( alg.core() )
    return self


  #
  # Set generator reader
  #
  def setGenerator( self, gen ):
    self.__numberOfEvents = gen.GetEntries()
    self._core.setGenerator( gen.core() )


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
  