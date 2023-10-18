__all__ = ["CaloHitMerge"]

from GaugiKernel import Cpp, LoggingLevel
from GaugiKernel.macros import *
import ROOT


class CaloHitMerge( Cpp ):

  def __init__( self, name          : str, 
                InputCollectionKeys : list= [],
                OutputHitsKey       : str = "Hits",
                OutputLevel         : int = LoggingLevel.toC('INFO'),
                ): 
    
    Cpp.__init__(self, ROOT.CaloHitMerge(name) )
    self.setProperty( "InputCollectionKeys" , InputCollectionKeys )
    self.setProperty( "OutputHitsKey"       , OutputHitsKey       ) 
    self.setProperty( "OutputLevel"         , OutputLevel         ) 















