__all__ = ["CaloHitMerge"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *



class CaloHitMerge( Cpp ):

  def __init__( self, name, 
                InputCollectionKeys : list= [],
                OutputHitsKey       : str = "Hits",
                OutputLevel         : int = 10,
                ): 
    
    Cpp.__init__(self, name, "ROOT.CaloHitMerge", OutputLevel=OutputLevel)
    self.setProperty( "InputCollectionKeys" , InputCollectionKeys )
    self.setProperty( "OutputHitsKey"       , OutputHitsKey       ) 
    self.setProperty( "OutputLevel"         , OutputLevel         ) 















