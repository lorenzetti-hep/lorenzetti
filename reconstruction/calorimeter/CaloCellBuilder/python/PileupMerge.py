__all__ = ["PileupMerge"]

from GaugiKernel import Cpp, LoggingLevel
from GaugiKernel.macros import *
import ROOT

class PileupMerge( Cpp ):

  def __init__( self, name          : str, 
                UnitPileupInputFile : str,  
                HighPileupInputFile : str,
                InputHitsKey        : str="Hits",
                OutputHitsKey       : str="Hits_Merged",
                InputEventKey       : str="EventInfo",
                OutputEventKey      : str="EventInfo_Merged",
                OutputLevel         : int=LoggingLevel.toC('INFO'),
                NtupleName          : str="CollectionTree",
                PileupAvg           : float=0,
                PileupSigma         : float=0,
              ): 
    
    Cpp.__init__(self, ROOT.PileupMerge(name) )
    self.setProperty( "OutputLevel"           , OutputLevel              )
    self.setProperty( "InputHitsKey"          , InputHitsKey             )  
    self.setProperty( "OutputHitsKey"         , OutputHitsKey            )
    self.setProperty( "InputEventKey"         , InputEventKey            )
    self.setProperty( "OutputEventKey"        , OutputEventKey           )
    self.setProperty( "PileupAvg"             , PileupAvg                )
    self.setProperty( "PileupSigma"           , PileupSigma              )
    self.setProperty( "UnitPileupInputFile"   , UnitPileupInputFile      )
    self.setProperty( "HighPileupInputFile"   , HighPileupInputFile      )


