__all__ = ["SingleParticle", "Particle"]

from GaugiKernel import Cpp, EnumStringification
from GaugiKernel.constants import *
from GaugiKernel.macros import *
from ROOT import generator


class Particle(EnumStringification):
  Electron = 11
  Photon = 22
  Pion = 211


class SingleParticle( Cpp ):

  def __init__( self, name, gen,  
                Eta          : float=0.0,
                Phi          : float=0.0,
                Particle     : int=Particle.Electron,
                Energy       : float=-1,
                EnergyMin    : float=0.0*GeV,
                EnergyMax    : float=100*GeV,
                HasLifetime  : bool=False,
                AtRest       : bool=False,
                OutputLevel  : int=0,
                DoRangedEta  : bool=False,
                EtaMin       : float=-2.5,
                EtaMax       : float=2.5,
                DoRangedPhi  : bool=False,
                PhiMin       : float=-3.14,
                PhiMax       : float=3.14,
              ):

    Cpp.__init__(self, generator.SingleParticle(name, gen.core()) )
    self.__gen = gen
    self.setProperty( "Eta"         , Eta         )
    self.setProperty( "Phi"         , Phi         )
    self.setProperty( "Particle"    , Particle    )
    self.setProperty( "Energy"      , Energy      )
    self.setProperty( "EnergyMin"   , EnergyMin   )
    self.setProperty( "EnergyMax"   , EnergyMax   )
    self.setProperty( "HasLifetime" , HasLifetime )
    self.setProperty( "AtRest"      , AtRest      )
    #self.setProperty( "OutputLevel" , OutputLevel )
    self.setProperty( "DoRangedEta" , DoRangedEta )
    self.setProperty( "EtaMin"      , EtaMin      )
    self.setProperty( "EtaMax"      , EtaMax      )
    self.setProperty( "DoRangedPhi" , DoRangedPhi )
    self.setProperty( "PhiMin"      , PhiMin      )
    self.setProperty( "PhiMax"      , PhiMax      )


  def gun(self):
    return self.__gen
