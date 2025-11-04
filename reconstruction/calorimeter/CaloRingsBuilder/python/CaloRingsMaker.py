__all__ = ["CaloRingsMaker"]


from GaugiKernel import Cpp
from GaugiKernel.macros import *
from typing import List
import ROOT
import numpy as np
pi = np.pi


class CaloRingsMaker( Cpp ):
  
  def __init__( self, name,
                InputClusterKey  : str, 
                OutputRingerKey  : str, 
                DeltaEtaRings    : list,
                DeltaPhiRings    : list,
                NRings           : list,
                LayerRings       : list,
                OutputLevel      : int=0, 
                HistogramPath    : str="Expert/Rings",
                EtaRange         : List[float] = [0, 2.5],
                DoSigmaCut       : bool = False,
                SigmaCut         : float = 2.0
              ):

    Cpp.__init__(self, ROOT.CaloRingsMaker(name) )
 
    self.setProperty( "OutputRingerKey"    , OutputRingerKey  )
    self.setProperty( "InputClusterKey"    , InputClusterKey  )
    self.setProperty( "DeltaEtaRings"      , DeltaEtaRings    )
    self.setProperty( "DeltaPhiRings"      , DeltaPhiRings    )
    self.setProperty( "NRings"             , NRings           )
    self.setProperty( "LayerRings"         , LayerRings       )
    self.setProperty( "HistogramPath"      , HistogramPath    )
    self.setProperty( "OutputLevel"        , OutputLevel      )
    self.setProperty( "EtaRange"           , EtaRange         )
    self.setProperty( "DoSigmaCut"         , DoSigmaCut       )
    self.setProperty( "SigmaCut"           , SigmaCut         )
