__all__ = ["CaloHitMerge"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
import ROOT


class CaloHitMerge( Cpp ):

  def __init__( self, name          : str, 
                InputCollectionKeys : list= [],
                OutputHitsKey       : str = "Hits",
                OutputLevel         : int = 10,
                ): 
    
    Cpp.__init__(self, ROOT.CaloHitMerge(name) )
    self.setProperty( "InputCollectionKeys" , InputCollectionKeys )
    self.setProperty( "OutputHitsKey"       , OutputHitsKey       ) 
    self.setProperty( "OutputLevel"         , OutputLevel         ) 















