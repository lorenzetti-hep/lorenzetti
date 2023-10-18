__all__ = ["PileupMerge"]

from GaugiKernel import Cpp, LoggingLevel
from GaugiKernel.macros import *
import ROOT

class PileupMerge( Cpp ):

  def __init__( self, name      : str, 
                InputFile       : str,
                InputHitsKey    : str="Hits",
                OutputHitsKey   : str="Hits_Merged",
                InputEventKey   : str="EventInfo",
                OutputEventKey  : str="EventInfo_Merged",
                OutputLevel     : int=LoggingLevel.toC('INFO'),
                NtupleName      : str="CollectionTree"
              ): 
    
    Cpp.__init__(self, ROOT.PileupMerge(name) )
    self.setProperty( "OutputLevel"   , OutputLevel    )
    self.setProperty( "InputHitsKey"  , InputHitsKey   )  
    self.setProperty( "OutputHitsKey" , OutputHitsKey  )
    self.setProperty( "InputEventKey" , InputEventKey  )
    self.setProperty( "OutputEventKey", OutputEventKey )
    self.setProperty( "InputFile"     , InputFile      )

