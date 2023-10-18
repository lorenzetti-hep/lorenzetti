__all__ = ["Pythia8"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from ROOT import generator


class Pythia8( Cpp ):

  def __init__( self, name, 
                File        : str="",
                Seed        : int=512,
                OutputLevel : int=0,
                EventNumber : int=-1,
               ): 
    
    Cpp.__init__(self, generator.Pythia8Gen())
    self.setProperty( "Seed"        , Seed        )
    self.setProperty( "EventNumber" , EventNumber )
    #self.setProperty( "OutputLevel" , OutputLevel )
    self.setProperty( "File"        , File        )
