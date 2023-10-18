__all__ = ["PileupMerge"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *


class PileupMerge( Cpp ):

  def __init__( self, name, 
                InputFile       : str,
                InputHitsKey    : str="Hits",
                OutputHitsKey   : str="Hits_Merged",
                InputEventKey   : str="EventInfo",
                OutputEventKey  : str="EventInfo_Merged",
                OutputLevel     : int=0,
                NtupleName      : str="CollectionTree"
              ): 
    
    Cpp.__init__(self, name, "ROOT.PileupMerge", OutputLevel=OutputLevel)
    self.setProperty( "InputHitsKey"  , InputHitsKey   )  
    self.setProperty( "OutputHitsKey" , OutputHitsKey  )
    self.setProperty( "InputEventKey" , InputEventKey  )
    self.setProperty( "OutputEventKey", OutputEventKey )
    self.setProperty( "InputFile"     , InputFile      )

