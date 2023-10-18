__all__ = ["FixedRegion"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from ROOT import generator


class FixedRegion( Cpp ):

  def __init__( self, name,
                      Eta         : float=0.0,
                      Phi         : float=0.0,
                      OutputLevel : int=0 ): 
    
    Cpp.__init__(self, generator.FixedRegion(name))
    self.setProperty( "Eta"          , Eta           )
    self.setProperty( "Phi"          , Phi           )
    self.setProperty( "OutputLevel"  , OutputLevel   )



