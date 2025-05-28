__all__ = ["Pileup"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from ROOT import generator


class Pileup( Cpp ):

  __allow_keys = [
                "EtaMax",
                "PileupAvg",
                "PileupSigma",
                "PileupPerBunch",
                "BunchIdStart",
                "BunchIdEnd",
                "Select",
                "DeltaEta",
                "DeltaPhi",
                "OutputLevel",
                ]

  def __init__( self, name, gen, 
                EtaMax         : float=1.4,
                PileupAvg      : float=0,
                PileupSigma    : float=0,
                PileupPerBunch : float=-1,
                BunchIdStart   : int=-8,
                BunchIdEnd     : int=7,
                Select         : int=2,
                DeltaEta       : float=0.22,
                DeltaPhi       : float=0.22,
                OutputLevel    : int=0
              ): 
    
    Cpp.__init__(self, generator.Pileup(name, gen.core()))
    self.__gen = gen

    self.setProperty( "EtaMax"        , EtaMax         )
    self.setProperty( "PileupAvg"     , PileupAvg      )
    self.setProperty( "PileupSigma"   , PileupSigma    )
    self.setProperty( "PileupPerBunch", PileupPerBunch )
    self.setProperty( "BunchIdStart"  , BunchIdStart   )
    self.setProperty( "BunchIdEnd"    , BunchIdEnd     )
    self.setProperty( "Select"        , Select         )
    self.setProperty( "DeltaEta"      , DeltaEta       )
    self.setProperty( "DeltaPhi"      , DeltaPhi       )


  def gun(self):
    return self.__gen


