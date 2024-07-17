__all__ = ["BoostedEvents"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from GaugiKernel.constants import  GeV

from ROOT import generator
from filters import Particle


class BoostedEvents(Cpp):

    def __init__(self, name, gen,
                 Particle: int = Particle.Electron,
                 Energy: float = 1*GeV,
                 DeltaR: float = 0.2,
                 HasLifetime: bool = False,
                 AtRest: bool = False,
                 OutputLevel: int = 0):

        Cpp.__init__(self, generator.BoostedEvents(name, gen.core()))
        self.__gen = gen
        self.setProperty("Particle", Particle)
        self.setProperty("Energy", Energy)
        self.setProperty("DeltaR", DeltaR)
        self.setProperty("HasLifetime", HasLifetime)
        self.setProperty("AtRest", AtRest)
        # self.setProperty( "OutputLevel" , OutputLevel   )

    def gun(self):
        return self.__gen
