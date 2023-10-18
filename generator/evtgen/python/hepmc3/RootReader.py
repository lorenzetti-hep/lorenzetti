__all__ = ["P8Gun"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from ROOT import generator


class P8Gun( Cpp ):

  def __init__( self, name,
                File        : str="",
                Seed        : int=512,
                OutputLevel : int=0 
              ): 

    Cpp.__init__(self, generator.P8Gun())
    self.setProperty( "File"        , File        )
    self.setProperty( "Seed"        , Seed        )
    self.setProperty( "OutputLevel" , OutputLevel )

