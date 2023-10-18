__all__ = ["BoostedEvents"]

from GaugiKernel import Cpp, EnumStringification
from GaugiKernel.macros import *
from GaugiKernel.constants import *

from ROOT import generator
from filters import Particle

class BoostedEvents( Cpp ):

  def __init__( self, name, gen,
                      Particle      : int=Particle.Electron, 
                      Energy        : float=1*GeV,
                      DeltaR        : float=0.2,
                      HasLifetime   : bool=False,
                      AtRest        : bool=False,
                      Seed          : int=512,
                      OutputLevel   : int=0   ): 
    

    Cpp.__init__(self, generator.BoostedEvents(name, gen))
    self.__gun = gen 
    self.setProperty( "Particle"    , Particle      )
    self.setProperty( "Energy"      , Energy        )
    self.setProperty( "DeltaR"      , DeltaR        )
    self.setProperty( "HasLifetime" , HasLifetime   )
    self.setProperty( "AtRest"      , AtRest        )
    #self.setProperty( "OutputLevel" , OutputLevel   )


  def gun(self):
    return self.__gun
