__all__ = ["ParticleGun", "Particle"]

from GaugiKernel import Logger, EnumStringification
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue



class Particle(EnumStringification):
  Electron = 11
  Photon = 22
  Pion = 211





class ParticleGun( Logger ):

  __allow_keys = [
                "Eta",
                "Phi",
                "Particle",
                "Energy",
                "EnergyMin",
                "EnergyMax",
                "HasLifetime",
                "AtRest",
                "Seed",
                "OutputLevel",
                "DoRangedEta",
                "EtaMin",
                "EtaMax",
                ]

  def __init__( self, name, **kw ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import generator
    # Create the algorithm
    self.__core = generator.ParticleGun()
    for key, value in kw.items():
      self.setProperty( key,value )


  def core(self):
    return self.__core


  def setProperty( self, key, value ):
    if key in self.__allow_keys:
      setattr( self, '__' + key , value )
      self.core().setProperty( key, treatPropertyValue(value) )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

 
  def getProperty( self, key ):
    if key in self.__allow_keys:
      return getattr( self, '__' + key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)



