__all__ = ["CaloPhiRange"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class CaloPhiRange( Logger ):

  __allow_keys = [
                  ]


  def __init__( self ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import CaloPhiRange
    # Create the algorithm
    self.__core = CaloPhiRange()

  def core(self):
    return self.__core

  def diff(self, phi1, phi2):
    return self.core().diff(phi1,phi2)



