__all__ = ["EventReader"]

from GaugiKernel import Cpp, LoggingLevel
from GaugiKernel.macros import *
import ROOT, os

class EventReader( Cpp ):

  def __init__( self, name,  
                InputFileName  : str = "", 
                OutputEventKey : str = "EventInfo",
                OutputTruthKey : str = "Particles",
                OutputSeedKey  : str = "Seeds",
                BunchDuration  : int = 25,
                OutputLevel    : int = LoggingLevel.toC('INFO'), 
              ): 
    
    Cpp.__init__(self, ROOT.generator.EventReader(name) )

    self.setProperty( "InputFileName"  , InputFileName  )
    self.setProperty( "OutputEventKey" , OutputEventKey )
    self.setProperty( "OutputSeedKey"  , OutputSeedKey  )
    self.setProperty( "OutputTruthKey" , OutputTruthKey )
    self.setProperty( "BunchDuration"  , BunchDuration  )
    #self.setProperty( "OutputLevel"    , OutputLevel    )

    if self.InputFileName != "":
      if not os.path.exists(self.InputFileName):
        MSG_FATAL(self, f"Input file not found into the system: {self.InputFileName}")
      f = ROOT.TFile( InputFileName )
      t = f.Get("particles")
      self.__entries = t.GetEntries()
      f.Close()
    else:
      self.__entries = 0

 
  def merge( self, acc ):
    acc.setGenerator( self )
  

  def GetEntries(self):
    return self.__entries