__all__ = ["JF17"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from ROOT import generator

class JF17( Cpp ):

  def __init__( self, name, gen,
                EtaMax      : float=1.4,
                EtaMin      : float=0.0,
                MinPt       : float=0.0, 
                Select      : int=2,
                OutputLevel : int=0, 
                EtaWindow   : float=0.4, 
                PhiWindow   : float=0.4, 
              ): 

    Cpp.__init__(self, generator.JF17(name, gen.core()))
    self.__gen = gen
    self.setProperty( "EtaMax"      , EtaMax      )
    self.setProperty( "EtaMin"      , EtaMin      )
    self.setProperty( "MinPt"       , MinPt       )
    self.setProperty( "Select"      , Select      )
    #self.setProperty( "OutputLevel" , OutputLevel )
    self.setProperty( "EtaWindow"   , EtaWindow   )
    self.setProperty( "PhiWindow"   , PhiWindow   )


  def gun(self):
    return self.__gen
