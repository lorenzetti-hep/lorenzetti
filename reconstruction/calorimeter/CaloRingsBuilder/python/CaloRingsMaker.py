__all__ = ["CaloRingsMaker"]


from GaugiKernel import Cpp
from GaugiKernel.macros import *
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
                DoForward        : bool=False
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
    self.setProperty( "DoForward"          , DoForward        )





