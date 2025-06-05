__all__ = ["PileupMerge"]

from GaugiKernel        import Cpp, LoggingLevel
from GaugiKernel.macros import *
from expand_folders     import expand_folders
from typing             import List
import ROOT

class PileupMerge( Cpp ):

  def __init__( self, name          : str, 
                LowPileupInputFiles : List[str],  
                HighPileupInputFiles: List[str],
                InputHitsKey        : str="Hits",
                OutputHitsKey       : str="Hits_Merged",
                InputEventKey       : str="EventInfo",
                OutputEventKey      : str="EventInfo_Merged",
                OutputLevel         : int=LoggingLevel.toC('INFO'),
                NtupleName          : str="CollectionTree",
                PileupAvg           : float=0,
                PileupSigma         : float=0,
                BunchIdStart        : int=-8,
                BunchIdEnd          : int=7,
              ): 
    
    Cpp.__init__(self, ROOT.PileupMerge(name) )
    self.setProperty( "OutputLevel"           , OutputLevel              )
    self.setProperty( "InputHitsKey"          , InputHitsKey             )  
    self.setProperty( "OutputHitsKey"         , OutputHitsKey            )
    self.setProperty( "InputEventKey"         , InputEventKey            )
    self.setProperty( "OutputEventKey"        , OutputEventKey           )
    self.setProperty( "PileupAvg"             , PileupAvg                )
    self.setProperty( "PileupSigma"           , PileupSigma              )
    self.setProperty( "BunchIdStart"          , BunchIdStart             )
    self.setProperty( "BunchIdEnd"            , BunchIdEnd               )
    self.setProperty( "NtupleName"            , NtupleName               )
    self.setProperty( "LowPileupInputFiles"   , LowPileupInputFiles      )
    self.setProperty( "HighPileupInputFiles"  , HighPileupInputFiles     )
