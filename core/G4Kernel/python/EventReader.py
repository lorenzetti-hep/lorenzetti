__all__ = ["EventReader"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class EventReader( Logger ):

  def __init__( self, name,  
                InputFileName  : str = "", 
                OutputEventKey : str = "EventInfo",
                OutputTruthKey : str = "Particles",
                OutputSeedKey  : str = "Seeds",
                BunchDuration  : int = 25 
              ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import generator
    # Create the algorithm
    self.__core = generator.EventReader(name)
    self.setProperty( "InputFileName"  , InputFileName  )
    self.setProperty( "OutputEventKey" , OutputEventKey )
    self.setProperty( "OutputSeedKey"  , OutputSeedKey  )
    self.setProperty( "OutputTruthKey" , OutputTruthKey )
    self.setProperty( "BunchDuration"  , BunchDuration  )

    if InputFileName != "":
      from ROOT import TFile, TTree
      f = TFile( InputFileName )
      t = f.Get("particles")
      self.__entries = t.GetEntries()
      f.Close()
    else:
      self.__entries = 0

  def core(self):
    return self.__core


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


  def merge( self, acc ):
    acc.setGenerator( self )
  

  def GetEntries(self):
    return self.__entries